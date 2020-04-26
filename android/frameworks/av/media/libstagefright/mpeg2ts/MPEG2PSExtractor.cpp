/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "MPEG2PSExtractor"
#include <utils/Log.h>

#include "include/MPEG2PSExtractor.h"

#include "AnotherPacketSource.h"
#include "ESQueue.h"

#include <media/stagefright/foundation/ABitReader.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/Utils.h>
#include <utils/String8.h>
#ifdef RTK_PLATFORM
#include <cutils/properties.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <pthread.h>
#include <cutils/properties.h>
#endif
#include <inttypes.h>

namespace android {

struct MPEG2PSExtractor::Track : public MediaSource {
    Track(MPEG2PSExtractor *extractor,
          unsigned stream_id, unsigned stream_type);

    virtual status_t start(MetaData *params);
    virtual status_t stop();
    virtual sp<MetaData> getFormat();
    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options);
protected:
    virtual ~Track();

private:
    friend struct MPEG2PSExtractor;

    MPEG2PSExtractor *mExtractor;

    unsigned mStreamID;
    unsigned mStreamType;
    ElementaryStreamQueue *mQueue;
    sp<AnotherPacketSource> mSource;
#ifdef RTK_PLATFORM
    unsigned mReadTimeOutCount;
    int64_t mMaxTimeUs;
    bool mTrackUseless;
#endif
    status_t appendPESData(
            unsigned PTS_DTS_flags,
            uint64_t PTS, uint64_t DTS,
            const uint8_t *data, size_t size);
#ifdef RTK_PLATFORM
    int64_t getTrackDuration();
    int64_t getPTS();
    bool isVideo();
    bool isAudio();
    void signalDiscontinuity(const bool bKeepFormat);
    void setESSeeking();
    void setSeekStatusOver();
    unsigned getStreamType();
#endif

    DISALLOW_EVIL_CONSTRUCTORS(Track);
};

struct MPEG2PSExtractor::WrappedTrack : public MediaSource {
    WrappedTrack(const sp<MPEG2PSExtractor> &extractor, const sp<Track> &track);

    virtual status_t start(MetaData *params);
    virtual status_t stop();
    virtual sp<MetaData> getFormat();

    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options);

protected:
    virtual ~WrappedTrack();

private:
    sp<MPEG2PSExtractor> mExtractor;
    sp<MPEG2PSExtractor::Track> mTrack;

    DISALLOW_EVIL_CONSTRUCTORS(WrappedTrack);
};

////////////////////////////////////////////////////////////////////////////////

MPEG2PSExtractor::MPEG2PSExtractor(const sp<DataSource> &source)
    : mDataSource(source),
      mOffset(0),
      mFinalResult(OK),
      mBuffer(new ABuffer(0)),
      mScanning(true),
      mProgramStreamMapValid(false)
#ifdef RTK_PLATFORM
      ,mStopThread(false)
      ,mFileSize(0)
      ,mDurationUs(0)
      ,mNeedDequeuePES(true)
      ,mSeekTimeUs(0)
      ,mSeeking(false)
      ,mMaxcount(0)
      ,mSeekingOffset(0)
      ,mMinOffset(0)
      ,mMaxOffset(0)
      ,mBaseTime(-1)
      ,mLastSeekTimeUs(-1000000)
      ,mPriExtStreamId(0)
#endif
      {
#ifdef RTK_PLATFORM
    parseMaxPTS();
#endif
    for (size_t i = 0; i < 1000; ++i) {
        if (feedMore() != OK) {
            break;
        }
    }

    // Remove all tracks that were unable to determine their format.
    for (size_t i = mTracks.size(); i > 0;) {
        i--;
        if (mTracks.valueAt(i)->getFormat() == NULL) {
            mTracks.removeItemsAt(i);
        }
    }

#ifdef RTK_PLATFORM
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&mThread, &attr, ThreadWrapper, this);
    pthread_attr_destroy(&attr);
#endif
    mScanning = false;
}

MPEG2PSExtractor::~MPEG2PSExtractor() {
#ifdef RTK_PLATFORM
    mStopThread = true;
    void *dummy = 0;
    pthread_join(mThread, &dummy);
#endif
}

size_t MPEG2PSExtractor::countTracks() {
    return mTracks.size();
}

sp<IMediaSource> MPEG2PSExtractor::getTrack(size_t index) {
    if (index >= mTracks.size()) {
        return NULL;
    }

    return new WrappedTrack(this, mTracks.valueAt(index));
}

sp<MetaData> MPEG2PSExtractor::getTrackMetaData(
        size_t index, uint32_t /* flags */) {
    if (index >= mTracks.size()) {
        return NULL;
    }

#ifdef RTK_PLATFORM
    sp<MetaData> meta = mTracks.editValueAt(index)->getFormat()  ;
    meta->setInt64(kKeyDuration,  getDurationUs());
#endif
    return mTracks.valueAt(index)->getFormat();
}

sp<MetaData> MPEG2PSExtractor::getMetaData() {
    sp<MetaData> meta = new MetaData;
    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_CONTAINER_MPEG2PS);
#ifdef RTK_PLATFORM
    meta->setInt64(kKeyDuration,  getDurationUs());
#endif
    return meta;
}

uint32_t MPEG2PSExtractor::flags() const {
    return CAN_PAUSE;
}

status_t MPEG2PSExtractor::feedMore() {
    Mutex::Autolock autoLock(mLock);

    // How much data we're reading at a time
    static const size_t kChunkSize = 8192;
#if RTK_PLATFORM
    if (mSeeking) {
        int64_t pts = getMaxPTS() - mBaseTime;
        //ALOGE("feedMore - Check Time Diff: %lld %lld ",pts/1000, mSeekTimeUs/1000);

        if (pts > 0 && mSeekTimeUs != 0) {
            mMaxcount++;
            if (((pts - mSeekTimeUs < 500000) && (pts - mSeekTimeUs > -500000)) || mMinOffset == mMaxOffset || mMaxcount > 13) {
                signalDiscontinuity(false);
                mOffset = SearchValidOffset(mOffset);
                mBuffer->setRange(0, 0); //Reset mBuffer Data
                mSeeking = false;
                for (size_t i = 0; i < mTracks.size(); ++i)
                {
                    mTracks.editValueAt(i)->setESSeeking();
                    mTracks.editValueAt(i)->setSeekStatusOver();
                }
                setDequeueState(true);
                ALOGD("Find seek offset %" PRIu64 " pts %" PRId64 "\n", mOffset, pts/1000);
            }
            else {
                signalDiscontinuity(false);
                updateSeekOffset(pts);
                mOffset = mSeekingOffset;
                mBuffer->setRange(0, 0); //Reset mBuffer Data
                mOffset = SearchValidOffset(mOffset);
                ALOGD("pts=%" PRId64 ", mSeekTimeUs=%" PRId64 ", mMaxcount=%" PRIu64 ", mMinOffset=%" PRIu64 ", mMaxOffset=%" PRIu64 ", mSeekingOffset=%" PRIu64 " , mOffset = %" PRIu64 "\n",
                pts/1000, mSeekTimeUs/1000, mMaxcount, mMinOffset, mMaxOffset, mSeekingOffset, mOffset);
            }
        }
        else if(mSeekTimeUs == 0)
        {
            signalDiscontinuity(false);
            mOffset = SearchValidOffset(0);
            mBuffer->setRange(0, 0);
            mSeeking = false;
            for (size_t i = 0; i < mTracks.size(); ++i)
            {
                mTracks.editValueAt(i)->setESSeeking();
                mTracks.editValueAt(i)->setSeekStatusOver();
            }
            setDequeueState(true);
        }
    }
#endif
    for (;;) {
        status_t err = dequeueChunk();

        if (err == -EAGAIN && mFinalResult == OK) {
            memmove(mBuffer->base(), mBuffer->data(), mBuffer->size());
            mBuffer->setRange(0, mBuffer->size());

            if (mBuffer->size() + kChunkSize > mBuffer->capacity()) {
                size_t newCapacity = mBuffer->capacity() + kChunkSize;
                sp<ABuffer> newBuffer = new ABuffer(newCapacity);
                memcpy(newBuffer->data(), mBuffer->data(), mBuffer->size());
                newBuffer->setRange(0, mBuffer->size());
                mBuffer = newBuffer;
            }

            ssize_t n = mDataSource->readAt(
                    mOffset, mBuffer->data() + mBuffer->size(), kChunkSize);

            if (n < (ssize_t)kChunkSize) {
                mFinalResult = (n < 0) ? (status_t)n : ERROR_END_OF_STREAM;
                return mFinalResult;
            }

            mBuffer->setRange(mBuffer->offset(), mBuffer->size() + n);
            mOffset += n;
        } else if (err != OK) {
            mFinalResult = err;
            return err;
        } else {
            return OK;
        }
    }
}

status_t MPEG2PSExtractor::dequeueChunk() {
    if (mBuffer->size() < 4) {
        return -EAGAIN;
    }

#ifdef RTK_PLATFORM
    size_t offset = 0;
    while(memcmp("\x00\x00\x01", mBuffer->data()+offset, 3))//Find start code
    {
        offset++;
        if(offset + 3 > mBuffer->size())
        {
            mBuffer->setRange(0, 0);
            return -EAGAIN;
        }
    }

    if(offset != 0)
    {
        //ALOGI("data 0x%x 0x%x 0x%x 0x%x offset %zu ", mBuffer->data()[offset], mBuffer->data()[offset+1], mBuffer->data()[offset+2], mBuffer->data()[offset+3], offset);
        mBuffer->setRange(mBuffer->offset()+offset, mBuffer->size()-offset);
    }
#else
    if (memcmp("\x00\x00\x01", mBuffer->data(), 3)) {
        return ERROR_MALFORMED;
    }
#endif

    unsigned chunkType = mBuffer->data()[3];

    ssize_t res;

    switch (chunkType) {
        case 0xba:
        {
#ifdef RTK_PLATFORM
            if(mScanning)
            {
                if(mBuffer->data()[4]>>6 == 0x1)
                    mIsMPEG1 = false;
                else if(mBuffer->data()[4]>>4 == 0x2)
                    mIsMPEG1 = true;
            }
#endif
            res = dequeuePack();
            break;
        }

        case 0xbb:
        {
            res = dequeueSystemHeader();
            break;
        }

        default:
        {
            res = dequeuePES();
            break;
        }
    }

    if (res > 0) {
        if (mBuffer->size() < (size_t)res) {
            return -EAGAIN;
        }

        mBuffer->setRange(mBuffer->offset() + res, mBuffer->size() - res);
        res = OK;
    }

    return res;
}

ssize_t MPEG2PSExtractor::dequeuePack() {
    // 32 + 2 + 3 + 1 + 15 + 1 + 15+ 1 + 9 + 1 + 22 + 1 + 1 | +5

#ifdef RTK_PLATFORM
    if(mIsMPEG1)
    {
        if (mBuffer->size() < 12) {
            return -EAGAIN;
        }
        return 12;
    }
    else
    {
#endif
    if (mBuffer->size() < 14) {
        return -EAGAIN;
    }

    unsigned pack_stuffing_length = mBuffer->data()[13] & 7;

    return pack_stuffing_length + 14;
#ifdef RTK_PLATFORM
    }
#endif
}

ssize_t MPEG2PSExtractor::dequeueSystemHeader() {
    if (mBuffer->size() < 6) {
        return -EAGAIN;
    }

    unsigned header_length = U16_AT(mBuffer->data() + 4);

    return header_length + 6;
}

ssize_t MPEG2PSExtractor::dequeuePES() {
    if (mBuffer->size() < 6) {
        return -EAGAIN;
    }

    unsigned PES_packet_length = U16_AT(mBuffer->data() + 4);
    if (PES_packet_length == 0u) {
        ALOGE("PES_packet_length is 0");
        return -EAGAIN;
    }

    size_t n = PES_packet_length + 6;

    if (mBuffer->size() < n) {
        return -EAGAIN;
    }

    ABitReader br(mBuffer->data(), n);


    unsigned packet_startcode_prefix = br.getBits(24);

    ALOGV("packet_startcode_prefix = 0x%08x", packet_startcode_prefix);

    if (packet_startcode_prefix != 1) {
        ALOGV("Supposedly payload_unit_start=1 unit does not start "
             "with startcode.");

        return ERROR_MALFORMED;
    }

    if (packet_startcode_prefix != 0x000001u) {
        ALOGE("Wrong PES prefix");
        return ERROR_MALFORMED;
    }

    unsigned stream_id = br.getBits(8);
    ALOGV("stream_id = 0x%02x", stream_id);

    /* unsigned PES_packet_length = */br.getBits(16);

    if (stream_id == 0xbc) {
        // program_stream_map

        if (!mScanning) {
            return n;
        }

        mStreamTypeByESID.clear();

        /* unsigned current_next_indicator = */br.getBits(1);
        /* unsigned reserved = */br.getBits(2);
        /* unsigned program_stream_map_version = */br.getBits(5);
        /* unsigned reserved = */br.getBits(7);
        /* unsigned marker_bit = */br.getBits(1);
        unsigned program_stream_info_length = br.getBits(16);

        size_t offset = 0;
        while (offset < program_stream_info_length) {
            if (offset + 2 > program_stream_info_length) {
                return ERROR_MALFORMED;
            }

            unsigned descriptor_tag = br.getBits(8);
            unsigned descriptor_length = br.getBits(8);

            ALOGI("found descriptor tag 0x%02x of length %u",
                 descriptor_tag, descriptor_length);

            if (offset + 2 + descriptor_length > program_stream_info_length) {
                return ERROR_MALFORMED;
            }

            br.skipBits(8 * descriptor_length);

            offset += 2 + descriptor_length;
        }

        unsigned elementary_stream_map_length = br.getBits(16);

        offset = 0;
        while (offset < elementary_stream_map_length) {
            if (offset + 4 > elementary_stream_map_length) {
                return ERROR_MALFORMED;
            }

            unsigned stream_type = br.getBits(8);
            unsigned elementary_stream_id = br.getBits(8);

            ALOGI("elementary stream id 0x%02x has stream type 0x%02x",
                 elementary_stream_id, stream_type);

            mStreamTypeByESID.add(elementary_stream_id, stream_type);

            unsigned elementary_stream_info_length = br.getBits(16);

            if (offset + 4 + elementary_stream_info_length
                    > elementary_stream_map_length) {
                return ERROR_MALFORMED;
            }

            offset += 4 + elementary_stream_info_length;
        }

        /* unsigned CRC32 = */br.getBits(32);

        mProgramStreamMapValid = true;
    } else if (stream_id != 0xbe  // padding_stream
            && stream_id != 0xbf  // private_stream_2
            && stream_id != 0xf0  // ECM
            && stream_id != 0xf1  // EMM
            && stream_id != 0xff  // program_stream_directory
            && stream_id != 0xf2  // DSMCC
            && stream_id != 0xf8) {  // H.222.1 type E
#ifdef RTK_PLATFORM

    uint64_t PTS = 0, DTS = 0;
    unsigned PTS_DTS_flags = 0;
    unsigned dataLength = 0;

    if(mIsMPEG1 == true)
    {
        while (br.data()[0] == 0xFF) { //stuffing_byte
            br.skipBits(8);
        }

        if((br.data()[0] & 0xC0) == 0x40)// Check STD info
        {
            br.skipBits(16);//Skip STD info
        }

        while((br.data()[0] & 0xC0) != 0x00)// Find PTS/DTS info
        {
            br.skipBits(8);
        }

        br.skipBits(2);
        PTS_DTS_flags = br.getBits(2);
        ALOGV("PTS_DTS_flags = %u", PTS_DTS_flags);

        if (PTS_DTS_flags == 2 || PTS_DTS_flags == 3) {
            PTS = ((uint64_t)br.getBits(3)) << 30;
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }
            PTS |= ((uint64_t)br.getBits(15)) << 15;
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }
            PTS |= br.getBits(15);
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }

            ALOGV("PTS = %" PRIu64, PTS);
            // ALOGI("PTS = %.2f secs", PTS / 90000.0f);

            if (PTS_DTS_flags == 3) {

                if (br.getBits(4) != 1u) {
                    return ERROR_MALFORMED;
                }

                DTS = ((uint64_t)br.getBits(3)) << 30;
                if (br.getBits(1) != 1u) {
                    return ERROR_MALFORMED;
                }
                DTS |= ((uint64_t)br.getBits(15)) << 15;
                if (br.getBits(1) != 1u) {
                    return ERROR_MALFORMED;
                }
                DTS |= br.getBits(15);
                if (br.getBits(1) != 1u) {
                    return ERROR_MALFORMED;
                }

                ALOGV("DTS = %" PRIu64, DTS);
            }
        }
        else
            br.skipBits(4);

        dataLength = br.numBitsLeft()/8;
#if 0//DBG USE
        if(stream_id >= 0xdf && stream_id <= 0xe0)
        {
            ALOGD("%s 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x dataLength %d", __FUNCTION__, br.data()[0], br.data()[1], br.data()[2], br.data()[3], br.data()[4], br.data()[5], br.data()[6], br.data()[7], dataLength);
        }
        else if(stream_id >= 0xC0 && stream_id <= 0xde)
            ALOGD("%s 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x dataLength %d", __FUNCTION__, br.data()[0], br.data()[1], br.data()[2], br.data()[3], br.data()[4], br.data()[5], br.data()[6], br.data()[7], dataLength);
#endif
    }
    else
    {
#endif
        /* unsigned PES_marker_bits = */br.getBits(2);  // should be 0x2(hex)
        /* unsigned PES_scrambling_control = */br.getBits(2);
        /* unsigned PES_priority = */br.getBits(1);
        /* unsigned data_alignment_indicator = */br.getBits(1);
        /* unsigned copyright = */br.getBits(1);
        /* unsigned original_or_copy = */br.getBits(1);

#ifndef RTK_PLATFORM
        unsigned PTS_DTS_flags = br.getBits(2);
#else
        PTS_DTS_flags = br.getBits(2);
#endif
        ALOGV("PTS_DTS_flags = %u", PTS_DTS_flags);

        unsigned ESCR_flag = br.getBits(1);
        ALOGV("ESCR_flag = %u", ESCR_flag);

        unsigned ES_rate_flag = br.getBits(1);
        ALOGV("ES_rate_flag = %u", ES_rate_flag);

        unsigned DSM_trick_mode_flag = br.getBits(1);
        ALOGV("DSM_trick_mode_flag = %u", DSM_trick_mode_flag);

        unsigned additional_copy_info_flag = br.getBits(1);
        ALOGV("additional_copy_info_flag = %u", additional_copy_info_flag);

        /* unsigned PES_CRC_flag = */br.getBits(1);
        /* PES_extension_flag = */br.getBits(1);

        unsigned PES_header_data_length = br.getBits(8);
        ALOGV("PES_header_data_length = %u", PES_header_data_length);

        unsigned optional_bytes_remaining = PES_header_data_length;
#ifndef RTK_PLATFORM
        uint64_t PTS = 0, DTS = 0;
#endif
        if (PTS_DTS_flags == 2 || PTS_DTS_flags == 3) {
            if (optional_bytes_remaining < 5u) {
                return ERROR_MALFORMED;
            }

            if (br.getBits(4) != PTS_DTS_flags) {
                return ERROR_MALFORMED;
            }

            PTS = ((uint64_t)br.getBits(3)) << 30;
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }
            PTS |= ((uint64_t)br.getBits(15)) << 15;
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }
            PTS |= br.getBits(15);
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }

            ALOGV("PTS = %" PRIu64, PTS);
            // ALOGI("PTS = %.2f secs", PTS / 90000.0f);

            optional_bytes_remaining -= 5;

            if (PTS_DTS_flags == 3) {
                if (optional_bytes_remaining < 5u) {
                    return ERROR_MALFORMED;
                }

                if (br.getBits(4) != 1u) {
                    return ERROR_MALFORMED;
                }

                DTS = ((uint64_t)br.getBits(3)) << 30;
                if (br.getBits(1) != 1u) {
                    return ERROR_MALFORMED;
                }
                DTS |= ((uint64_t)br.getBits(15)) << 15;
                if (br.getBits(1) != 1u) {
                    return ERROR_MALFORMED;
                }
                DTS |= br.getBits(15);
                if (br.getBits(1) != 1u) {
                    return ERROR_MALFORMED;
                }

                ALOGV("DTS = %" PRIu64, DTS);

                optional_bytes_remaining -= 5;
            }
        }

        if (ESCR_flag) {
            if (optional_bytes_remaining < 6u) {
                return ERROR_MALFORMED;
            }

            br.getBits(2);

            uint64_t ESCR = ((uint64_t)br.getBits(3)) << 30;
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }
            ESCR |= ((uint64_t)br.getBits(15)) << 15;
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }
            ESCR |= br.getBits(15);
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }

            ALOGV("ESCR = %" PRIu64, ESCR);
            /* unsigned ESCR_extension = */br.getBits(9);

            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }

            optional_bytes_remaining -= 6;
        }

        if (ES_rate_flag) {
            if (optional_bytes_remaining < 3u) {
                return ERROR_MALFORMED;
            }

            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }
            /* unsigned ES_rate = */br.getBits(22);
            if (br.getBits(1) != 1u) {
                return ERROR_MALFORMED;
            }

            optional_bytes_remaining -= 3;
        }

        if (br.numBitsLeft() < optional_bytes_remaining * 8) {
            return ERROR_MALFORMED;
        }

        br.skipBits(optional_bytes_remaining * 8);

        // ES data follows.

        if (PES_packet_length < PES_header_data_length + 3) {
            return ERROR_MALFORMED;
        }

#ifndef RTK_PLATFORM
        unsigned dataLength =
            PES_packet_length - 3 - PES_header_data_length;
#else
        dataLength =
            PES_packet_length - 3 - PES_header_data_length;
#endif

        if (br.numBitsLeft() < dataLength * 8) {
            ALOGE("PES packet does not carry enough data to contain "
                 "payload. (numBitsLeft = %zu, required = %u)",
                 br.numBitsLeft(), dataLength * 8);

            return ERROR_MALFORMED;
        }

        if (br.numBitsLeft() < dataLength * 8) {
            return ERROR_MALFORMED;
        }
#ifdef RTK_PLATFORM
    }
#endif
        if((stream_id < 0xc0 || stream_id > 0xef) && stream_id != 0xbd) {
            ALOGD("Got strange stream_id 0x%x ", stream_id);
            return 3;//Skip sync word only
        }
        ssize_t index = mTracks.indexOfKey(stream_id);
        if (index < 0 && mScanning) {
            unsigned streamType = 0;

            ssize_t streamTypeIndex;
            if (mProgramStreamMapValid
                    && (streamTypeIndex =
                            mStreamTypeByESID.indexOfKey(stream_id)) >= 0) {
#ifdef RTK_PLATFORM
                mPriExtStreamId = *(br.data());
#endif
                streamType = mStreamTypeByESID.valueAt(streamTypeIndex);
            } else if ((stream_id & ~0x1f) == 0xc0) {
                // ISO/IEC 13818-3 or ISO/IEC 11172-3 or ISO/IEC 13818-7
                // or ISO/IEC 14496-3 audio
                streamType = ATSParser::STREAMTYPE_MPEG2_AUDIO;
            } else if ((stream_id & ~0x0f) == 0xe0) {
                // ISO/IEC 13818-2 or ISO/IEC 11172-2 or ISO/IEC 14496-2 video
#ifdef RTK_PLATFORM
                //Video Part
                unsigned StartCode[5];
                StartCode[0] = *(br.data());
                StartCode[1] = *(br.data()+1);
                StartCode[2] = *(br.data()+2);
                StartCode[3] = *(br.data()+3);

                //ALOGD("start code 0x%x 0x%x 0x%x 0x%x 0x%x", StartCode[0], StartCode[1], StartCode[2], StartCode[3]);
                //MPEG2 or MPEG1
                if ((StartCode[0] == 0x0) && (StartCode[1] == 0x0) && (StartCode[2] == 0x01) && (StartCode[3] == 0xB3)) {
                    //[MP2PS] add to distinct MPEG1 and MPEG2
                    if(mIsMPEG1 == true) {
                        streamType = ATSParser::STREAMTYPE_MPEG1_VIDEO;
                    } else {
                        streamType = ATSParser::STREAMTYPE_MPEG2_VIDEO;
                    }
                } else if ((StartCode[0] == 0x0) && (StartCode[1] == 0x0) && (StartCode[2] == 0x00) && (StartCode[3] == 0x01)) {
                    streamType = ATSParser::STREAMTYPE_H264;
                } else if ((StartCode[0] == 0x0) && (StartCode[1] == 0x0) && (StartCode[2] == 0x01) && ((StartCode[3] == 0xB0))) {
                    streamType = ATSParser::STREAMTYPE_MPEG4_VIDEO;
                } else {
                    //TODO
                    streamType = ATSParser::STREAMTYPE_MPEG2_VIDEO;

                    //try to sequentially search 128 bytes
                    uint8_t* paucTmpBuf = (uint8_t*)br.data();
                    int32_t idx, buf_size = ((br.numBitsLeft()>>3) > 128)?128:(br.numBitsLeft()>>3);
                    for(idx = 0;idx < buf_size-5; idx++) {
                        if (!memcmp("\x00\x00\x00\x01", &paucTmpBuf[idx], 4)) {
                            streamType = ATSParser::STREAMTYPE_H264;
                            break;
                        } else if(!memcmp("\x00\x00\x01", &paucTmpBuf[idx], 3)) {
                            if(paucTmpBuf[idx + 3] == 0xB3) {
                                //[MP2PS] add to distinct MPEG1 and MPEG2
                                if(mIsMPEG1 == true) {
                                    streamType = ATSParser::STREAMTYPE_MPEG1_VIDEO;
                                } else {
                                    streamType = ATSParser::STREAMTYPE_MPEG2_VIDEO;
                                }
                                break;
                            } else if(paucTmpBuf[idx + 3] == 0xB0) {
                                streamType = ATSParser::STREAMTYPE_MPEG4_VIDEO;
                                break;
                            } else if(paucTmpBuf[idx + 3] == 0x00 && paucTmpBuf[idx + 4] == 0x02) {
                                streamType = ATSParser::STREAMTYPE_MPEG2_VIDEO;
                                break;
                            }
                        }
                    }
                    ALOGD("V:No PMT and search type %d\n",streamType);

                }
#else
                streamType = ATSParser::STREAMTYPE_MPEG2_VIDEO;
#endif
#ifdef RTK_PLATFORM
            } else if ((stream_id & 0xff) == 0xbd) {
                // Private Stream 1
                unsigned stream_id_extrend = *(br.data());
                ALOGI("stream_id_extrend 0x%x ", stream_id_extrend);
                if(stream_id_extrend >= 0x80 && stream_id_extrend <= 0x90)
                    streamType = ATSParser::STREAMTYPE_AC3;
                else if(stream_id_extrend >= 0xa0 && stream_id_extrend <= 0xb0)
                    streamType = ATSParser::STREAMTYPE_LPCM_AC3;
                else
                    streamType = ATSParser::STREAMTYPE_RESERVED;
                mPriExtStreamId = stream_id_extrend;
#endif
            } else {
                streamType = ATSParser::STREAMTYPE_RESERVED;
            }

            index = mTracks.add(
                    stream_id, new Track(this, stream_id, streamType));
        }

        status_t err = OK;

        if (index >= 0) {
#ifdef RTK_PLATFORM
            if((stream_id & 0xff) == 0xbd)
            {
                unsigned stream_id_extrend = *(br.data());
                if(stream_id_extrend == mPriExtStreamId)
                {
                    err = mTracks.editValueAt(index)->appendPESData(
                        PTS_DTS_flags, PTS, DTS, br.data(), dataLength);
                }
            }
            else
#endif
                err = mTracks.editValueAt(index)->appendPESData(
                    PTS_DTS_flags, PTS, DTS, br.data(), dataLength);
        }

        br.skipBits(dataLength * 8);

        if (err != OK) {
            return err;
        }
    } else if (stream_id == 0xbe) {  // padding_stream
        if (PES_packet_length == 0u) {
            return ERROR_MALFORMED;
        }
        br.skipBits(PES_packet_length * 8);
    } else {
        if (PES_packet_length == 0u) {
            return ERROR_MALFORMED;
        }
        br.skipBits(PES_packet_length * 8);
    }

    return n;
}

#ifdef RTK_PLATFORM
void *MPEG2PSExtractor::ThreadWrapper(void *me)
{
    ALOGD("MPEG2TSExtractor::threadWrapper");
    setpriority(PRIO_PROCESS, 0, ANDROID_PRIORITY_NORMAL);
    static_cast<MPEG2PSExtractor *>(me)->ThreadEntry();
    return NULL;
}

void MPEG2PSExtractor::ThreadEntry()
{
    prctl(PR_SET_NAME, (unsigned long)"MPEG2PSExtractorThread", 0, 0, 0);
    int t_pid = 0;
    int pri = getpriority(PRIO_PROCESS, t_pid);
    status_t status = OK;
    bool lowDuration;
    ALOGD("MPEG2TSExtractor::ThreadEntry ,priority = %d,pid=%d", pri, getpid());
    if(mTracks.size() < 1)
        return;
    while (!mStopThread)
    {
        lowDuration = false;
        for(size_t i=0;i<mTracks.size(); i++)
        {
            if(mTracks.editValueAt(i)->getTrackDuration() < 10000000)
            {
                lowDuration = true;
                break;
            }
        }
        if(lowDuration == true || mSeeking == true)
            status = feedMore();
        else
            usleep(1000);
        //if(status != OK)
        //    break;
    }
    mStopThread = true;
    return;
}

void MPEG2PSExtractor::setDequeueState(bool needDequeuePES) {
    mNeedDequeuePES = needDequeuePES;
}

bool MPEG2PSExtractor::getDequeueState() {
    return mNeedDequeuePES;
}

void MPEG2PSExtractor::updateSeekOffset(int64_t pts) {
    ALOGV("updateSeekOffset:now pts %" PRId64 "\n",pts/1000);
    if (1 == mMaxcount) {
        mSeekingOffset = mSeekTimeUs * mAverageByteRate/1000000;
        if (mSeekingOffset > mFileSize){
            mSeekingOffset = mFileSize;
        }
        else if (mSeekingOffset < 0){
            mSeekingOffset = 0;
        }
        ALOGV("updateSeekOffset:mMaxcount==1,mSeekingOffset:%" PRIu64 ",mAverageByteRate:%" PRId64 "\n", mSeekingOffset, mAverageByteRate);
        return ;
    }
    if ((pts < mSeekTimeUs) && (mSeekingOffset > mMinOffset)) {
        mMinOffset = mSeekingOffset;
        ALOGV("updateSeekOffset:(pts < mSeekTimeUs) && (mSeekingOffset > mMinOffset),mMinOffset = mSeekingOffset,%" PRIu64 "\n", mSeekingOffset);
    }
    else if ((pts > mSeekTimeUs) && (mSeekingOffset < mMaxOffset)) {
        mMaxOffset = mSeekingOffset;
        ALOGV("updateSeekOffset:(pts > mSeekTimeUs) && (mSeekingOffset < mMaxOffset),mMaxOffset = mSeekingOffset,%" PRIu64 "\n", mSeekingOffset);
    }

    if (mMaxOffset - mMinOffset < 100000) {
        mSeekingOffset = mMinOffset;
        ALOGV("updateSeekOffset:mMaxOffset - mMinOffset < 100kB,mSeekingOffset = mMinOffset,%" PRIu64 "\n", mSeekingOffset);
    }
    else if (mMaxOffset == mFileSize || mMinOffset == 0) {
        ALOGV("updateSeekOffset:mMaxOffset == mFileSize || mMinOffset == 0");
        mSeekingOffset += (mSeekTimeUs - pts) * mAverageByteRate /(1000000>>1);
        ALOGV("mSeekingOffset:%" PRIu64 " ,mSeekTimeUs:%" PRIu64 " ,pts:%" PRId64 " ,mAverageByteRate:%" PRIu64 "\n", mSeekingOffset, mSeekTimeUs, pts/1000, mAverageByteRate);
        if ((mSeekingOffset >= mFileSize) || (mSeekingOffset <= 0)) {
            if (mSeekingOffset >= mFileSize) {
                mMaxOffset = mFileSize-1;
                ALOGV("updateSeekOffset:mSeekingOffset >= mFileSize");
            }
            if (mSeekingOffset <= 0) {
                mMinOffset = 1;
                ALOGV("updateSeekOffset:mSeekingOffset <= 0");
            }
            mSeekingOffset = mFileSize >>1;
            ALOGV("updateSeekOffset:mSeekingOffset %" PRIu64 "\n", mSeekingOffset);
        }
    }
    else {
        mSeekingOffset = (mMinOffset + mMaxOffset)>>1;
        ALOGV("updateSeekOffset:mSeekingOffset %" PRIu64 "\n", mSeekingOffset);
    }
}


int64_t MPEG2PSExtractor::SearchPES(const void* data, int size)
{
    uint8_t* p = (uint8_t*)data;
    int offset = 0;
    unsigned stream_id_Video = 0xFF;
    unsigned stream_id_Audio = 0xFF;
    size_t index = 0;

    for (index=0; index<mTracks.size(); index++) {
       if (mTracks.valueAt(index)->isVideo())
            stream_id_Video = mTracks.valueAt(index)->mStreamID;
       if (mTracks.valueAt(index)->isAudio())
            stream_id_Audio = mTracks.valueAt(index)->mStreamID;
    }

    while(offset < size - 4) {
        //find start code
        if(p[0] == 0x00 && p[1] == 0x00 && p[2] == 0x01 && (p[3] == stream_id_Video || p[3] == stream_id_Audio)) 
            return offset;
        else {
            p++;
            offset++;
        }
    }
    return -1;
}


int64_t MPEG2PSExtractor::SearchValidOffset(off64_t currentoffset)
{
    //Search Start Code & StreamID
    int length = 0;
    size_t kChunkSize = 16384;
    int bufsize = kChunkSize;
    char* buf = new char[bufsize];
    off64_t offset = currentoffset;
    bool found=false;

    if (buf == NULL)
        return -1;

    while((length = mDataSource->readAt(offset, buf, bufsize)) == bufsize) {
        int64_t result = SearchPES(buf, length);

        if (result >= 0) {
            offset = offset + result;
            found = true;
            break;
        }
        else
            offset = offset + length;
    }

    if(buf != NULL) {
        free(buf);
        buf = NULL;
    }
    if(found == true)
        return offset;
    else
        return -1;
}

int64_t MPEG2PSExtractor::getMaxPTS() {     //For Seek
    int64_t maxPTS = 0;
    int64_t minPTS = -1;
    for (size_t i = 0; i < mTracks.size(); ++i) {
        int64_t pts = mTracks.editValueAt(i)->getPTS();

        if (maxPTS < pts) {
            maxPTS = pts;
        }

        if(minPTS == -1 || pts < minPTS) {
            minPTS = pts;
        }
    }

    if(mBaseTime == -1) {
        mBaseTime = minPTS;
        ALOGD("Got base time %" PRId64 "\n", mBaseTime/1000);
    }
    return maxPTS;
}

void MPEG2PSExtractor::seekTo(int64_t seekTimeUs) {
    Mutex::Autolock autoLock(mLock);

    char value[PROPERTY_VALUE_MAX];
    if (property_get("rtk.omx.running", value, NULL) && (!strcmp(value, "1") || !strcasecmp(value, "true")))
    {
        ALOGI("Background thumbnail decoding, skip seeking for performance");
        return;
    }

    signalDiscontinuity(false);
    mSeekingOffset = mOffset;
    mSeekTimeUs = seekTimeUs;
    mMinOffset = 0;
    mMaxOffset = mFileSize;
    mMaxcount=0;
    setDequeueState(false);
    mSeeking=true;
    mBuffer->setRange(0, 0);
    mFinalResult = OK;
    ALOGI("seekTo: mDurationMs =%" PRId64 " ,seekTimeMs= %" PRId64 " moffset: %" PRIu64" mMaxOffset %" PRIu64 "\n", mDurationUs/1000, seekTimeUs/1000, mOffset, mMaxOffset);
    return;
}


void MPEG2PSExtractor::parseMaxPTS() {
    size_t index = 0;
    size_t kChunkSize = 16384;
    int64_t timeOutUs = 3000000LL;
    mDataSource->getSize(&mFileSize);
    setDequeueState(false);
    signalDiscontinuity(false);

    int64_t maxPTSStart = systemTime()/1000;
    off64_t mOffsetSearch = mFileSize;

    for (size_t i = 0; i < 50; ++i) {
        if (feedMore() != OK) {
            return;
        }
        else if(getMaxPTS() > 0) {
            ALOGD("%s Got valid PTS", __FUNCTION__);
            break;
        }
    }
    for (off64_t i = 1; mOffsetSearch >= (off64_t)(kChunkSize/100); i++) {
        int64_t maxPTSDuration = (int64_t)(systemTime()/1000 - maxPTSStart);
        if (maxPTSDuration > timeOutUs) {
            ALOGD("%s TimeOut find PTS", __FUNCTION__);
            break;
        }
        if (mOffsetSearch > (off64_t)(100 * i * kChunkSize)) {
            mOffsetSearch= (off64_t)(mOffsetSearch - 100 * i * kChunkSize);
        }
        else
            mOffsetSearch = 0;

        mOffset = SearchValidOffset(mOffsetSearch);
        ALOGD("%s Parse %" PRIu64 " times, Search offset %" PRIu64 ", Valid Offset: %" PRIu64 ", %2.2f%% of Total File", __FUNCTION__, i, mOffsetSearch, mOffset, (float)(100 * mOffsetSearch / mFileSize));
        mFinalResult = OK;
        mBuffer->setRange(0, 0);
        while (feedMore() == OK) {
            int64_t maxPTSDuration = systemTime()/1000 - maxPTSStart;
            if (maxPTSDuration > timeOutUs) {
                ALOGD("%s TimeOut find PTS", __FUNCTION__);
                break;
            }

            if(((mOffset - mOffsetSearch) > (off64_t)(100 * kChunkSize)) && (getMaxPTS() == 0) ) {
                ALOGD("%s stop feedmore (no PES) ", __FUNCTION__);
                break;
            }

            if((mOffset - mOffsetSearch) > (off64_t)(100 * i * kChunkSize)) {
                ALOGD("%s stop feedmore (enough data) ", __FUNCTION__);
                break;
            }
        }
        mDurationUs = getMaxPTS() - mBaseTime;
        if (mDurationUs) {
            ALOGD("parseMaxPTS:mDurationUs=%" PRId64 ", mOffset=%" PRId64 "\n", mDurationUs/1000, mOffset);
            break;
        }
    }

    setDequeueState(true);
    mFinalResult = OK;
    mBuffer->setRange(0, 0);

    //Reset Max PTS
    for (index=0; index<mTracks.size(); index++) {
        mTracks.valueAt(index)->mMaxTimeUs = 0x0;
    }
    mAverageByteRate = (mDurationUs>0)?(mFileSize*1000000/mDurationUs):0;
    ALOGD("getMaxPTS->mDurationUs:%" PRId64 ", Track Number: %zu AverageByteRate %" PRIu64 "\n" , mDurationUs/1000, mTracks.size(), mAverageByteRate);
    signalDiscontinuity(false);
    mOffset = 0;
}

int64_t MPEG2PSExtractor::getDurationUs() {
    return mDurationUs;
}

void MPEG2PSExtractor::signalDiscontinuity(const bool bKeepFormat)
{
    mBuffer->setRange(0, 0);

    for (size_t i = 0; i < mTracks.size(); ++i)
        mTracks.valueAt(i)->signalDiscontinuity(bKeepFormat);
}

#endif
////////////////////////////////////////////////////////////////////////////////

MPEG2PSExtractor::Track::Track(
        MPEG2PSExtractor *extractor, unsigned stream_id, unsigned stream_type)
    : mExtractor(extractor),
      mStreamID(stream_id),
      mStreamType(stream_type),
      mQueue(NULL)
#ifdef RTK_PLATFORM
      ,mReadTimeOutCount(3)
      ,mMaxTimeUs(0)
      ,mTrackUseless(false)
#endif
      {
    bool supported = true;
    ElementaryStreamQueue::Mode mode;

    switch (mStreamType) {
        case ATSParser::STREAMTYPE_H264:
            mode = ElementaryStreamQueue::H264;
            break;
        case ATSParser::STREAMTYPE_MPEG2_AUDIO_ADTS:
            mode = ElementaryStreamQueue::AAC;
            break;
        case ATSParser::STREAMTYPE_MPEG1_AUDIO:
        case ATSParser::STREAMTYPE_MPEG2_AUDIO:
            mode = ElementaryStreamQueue::MPEG_AUDIO;
            break;
#ifdef RTK_PLATFORM
        case ATSParser::STREAMTYPE_AC3:
            mode = ElementaryStreamQueue::AC3;
            break;
        case ATSParser::STREAMTYPE_LPCM_AC3:
            mode = ElementaryStreamQueue::LPCM_AUDIO;
            break;
        case ATSParser::STREAMTYPE_AVS:
            mode = ElementaryStreamQueue::AVS;
            break;          
#endif
        case ATSParser::STREAMTYPE_MPEG1_VIDEO:
        case ATSParser::STREAMTYPE_MPEG2_VIDEO:
            mode = ElementaryStreamQueue::MPEG_VIDEO;
            break;

        case ATSParser::STREAMTYPE_MPEG4_VIDEO:
            mode = ElementaryStreamQueue::MPEG4_VIDEO;
            break;
        default:
            supported = false;
            break;
    }

    if (supported) {
        mQueue = new ElementaryStreamQueue(mode);
    } else {
        ALOGI("unsupported stream ID 0x%02x", stream_id);
    }
}

MPEG2PSExtractor::Track::~Track() {
    delete mQueue;
    mQueue = NULL;
}

status_t MPEG2PSExtractor::Track::start(MetaData *params) {
    if (mSource == NULL) {
        return NO_INIT;
    }

    return mSource->start(params);
}

status_t MPEG2PSExtractor::Track::stop() {
    if (mSource == NULL) {
        return NO_INIT;
    }

    return mSource->stop();
}

sp<MetaData> MPEG2PSExtractor::Track::getFormat() {
    if (mSource == NULL) {
        return NULL;
    }

    return mSource->getFormat();
}

status_t MPEG2PSExtractor::Track::read(
        MediaBuffer **buffer, const ReadOptions *options) {
    if (mSource == NULL) {
        return NO_INIT;
    }

    status_t finalResult;
#ifdef RTK_PLATFORM
    unsigned readCount = 0;
    int64_t seekTimeUs;
    ReadOptions::SeekMode seekMode;
    //if (mExtractor->mSeeking == false && options && options->getSeekTo(&seekTimeUs, &seekMode)) {
    if (options && options->getSeekTo(&seekTimeUs, &seekMode)) {
        if(mExtractor->mStopThread == true)
            return ERROR_UNSUPPORTED;
        //if(((mExtractor->mLastSeekTimeUs - seekTimeUs < 500000) && (mExtractor->mLastSeekTimeUs - seekTimeUs > -500000)))
        //    ALOGD("Seek time too close, ignore it");
        //else {
            mExtractor->mLastSeekTimeUs = seekTimeUs;
            mExtractor->seekTo(seekTimeUs);
            int seekWaitTimeout = 5;
            while(mExtractor->mSeeking == true && seekWaitTimeout > 0) {
                sleep(1);
                seekWaitTimeout--;
            }

            if(mExtractor->mSeeking == true)
                ALOGD("Wait seek timeout ...");
        //}
    }
#endif
    while (!mSource->hasBufferAvailable(&finalResult)) {
        if (finalResult != OK) {
            return ERROR_END_OF_STREAM;
        }

        status_t err = mExtractor->feedMore();
#ifdef RTK_PLATFORM
        readCount++;
        if(readCount > mReadTimeOutCount)
        {
            char value[PROPERTY_VALUE_MAX]={0};
            property_get("rtk.media.USE_GenericSource", value, NULL);
            if( (!strcmp("1", value) || !strcasecmp("true", value))&&strlen(value)>0)
            {
                return WOULD_BLOCK;
            }
        }
#endif

        if (err != OK) {
            mSource->signalEOS(err);
        }
    }

    return mSource->read(buffer, options);
}

#ifdef RTK_PLATFORM
int64_t MPEG2PSExtractor::Track::getTrackDuration() {
    status_t status = OK;
    int64_t duration = 0;
    duration = mSource->getBufferedDurationUs(&status);
    if(status == OK)
        return duration;
    else
        return -1;
}

int64_t MPEG2PSExtractor::Track::getPTS() {
    return mMaxTimeUs;
}

bool MPEG2PSExtractor::Track::isVideo(){
    switch (mStreamType) {
        case ATSParser::STREAMTYPE_H264:
        case ATSParser::STREAMTYPE_AVS:
        case ATSParser::STREAMTYPE_MPEG1_VIDEO:
        case ATSParser::STREAMTYPE_MPEG2_VIDEO:
        case ATSParser::STREAMTYPE_MPEG4_VIDEO:
            return true;
        default:
            return false;
    }
}

bool MPEG2PSExtractor::Track::isAudio(){
    switch (mStreamType) {
        case ATSParser::STREAMTYPE_MPEG1_AUDIO:
        case ATSParser::STREAMTYPE_MPEG2_AUDIO:
        case ATSParser::STREAMTYPE_MPEG2_AUDIO_ADTS:
        case ATSParser::STREAMTYPE_LPCM_AC3:
        case ATSParser::STREAMTYPE_AC3:
            return true;
        default:
            return false;
    }
}

void MPEG2PSExtractor::Track::signalDiscontinuity(const bool bKeepFormat)
{
    if (mQueue == NULL)
        return;

    if(bKeepFormat == true)
        ALOGD("signal but keep format");

    if (!mExtractor->getDequeueState())
    {
        mMaxTimeUs = 0;
        return;
    }

    mQueue->clear(false);
    if(mSource.get()) {
        mSource->clear();
        sp<MetaData> meta = mQueue->getFormat();
        if(meta != NULL)
            mSource->setFormat(meta);
    }
    else
        ALOGE("[error]this stream has no source\n");
    return;
}

void MPEG2PSExtractor::Track::setESSeeking() {
    if(mQueue == NULL)
        return;

    mQueue->setSeeking();
    return;
}

unsigned MPEG2PSExtractor::Track::getStreamType() {
    return mStreamType;
}

void MPEG2PSExtractor::Track::setSeekStatusOver() {

    if(mQueue == NULL)
        return;

    mQueue->setSeekStatusOver();
    return;
}
#endif

status_t MPEG2PSExtractor::Track::appendPESData(
        unsigned PTS_DTS_flags,
        uint64_t PTS, uint64_t /* DTS */,
        const uint8_t *data, size_t size) {
    if (mQueue == NULL) {
        return OK;
    }

    int64_t timeUs;
    if (PTS_DTS_flags == 2 || PTS_DTS_flags == 3) {
        timeUs = (PTS * 100) / 9;
    } else {
        timeUs = 0;
    }

#ifdef RTK_PLATFORM
        if ((timeUs > mMaxTimeUs) && (timeUs != 0xFFFFFFFF) && (timeUs != 0))
        {
            mMaxTimeUs = timeUs;
        }

        if (!mExtractor->getDequeueState())
        {
            return OK;
        }
#endif

    status_t err = mQueue->appendData(data, size, timeUs);

    if (err != OK) {
        return err;
    }

    sp<ABuffer> accessUnit;
    while ((accessUnit = mQueue->dequeueAccessUnit()) != NULL) {
        if (mSource == NULL) {
            sp<MetaData> meta = mQueue->getFormat();

            if (meta != NULL) {
                ALOGV("Stream ID 0x%02x now has data.", mStreamID);

                mSource = new AnotherPacketSource(meta);
                mSource->queueAccessUnit(accessUnit);
#ifdef RTK_PLATFORM
                if(mExtractor->mBaseTime != -1)
                    mSource->setBaseTimeUs(mExtractor->mBaseTime);
#endif
            }
#ifdef RTK_PLATFORM
        } else if (mQueue->getFormat() != NULL && mTrackUseless == false) {
#else
        } else if (mQueue->getFormat() != NULL) {
#endif

#ifdef RTK_PLATFORM
            char value[PROPERTY_VALUE_MAX];
            size_t pendingBufferSize = mSource->getPendingBufferSize();
            size_t threshold = isVideo()?20971520 /*20MB*/: 10485760 /*10MB*/;
            if(pendingBufferSize > threshold)
            {
                if (property_get("rtk.omx.running", value, NULL) && (!strcmp(value, "1") || !strcasecmp(value, "true")))
                {
                    mTrackUseless = false;
                    mSource->queueAccessUnit(accessUnit);
                }
                else
                {
                    mTrackUseless = true;
                    mSource->clear();
                    ALOGD("%s track seems like no consumer pendingBufferSize %zu ", isVideo()?"Video":"Audio", pendingBufferSize);
                }
            }
            else
#endif
                mSource->queueAccessUnit(accessUnit);
        }
    }

    return OK;
}

////////////////////////////////////////////////////////////////////////////////

MPEG2PSExtractor::WrappedTrack::WrappedTrack(
        const sp<MPEG2PSExtractor> &extractor, const sp<Track> &track)
    : mExtractor(extractor),
      mTrack(track) {
}

MPEG2PSExtractor::WrappedTrack::~WrappedTrack() {
}

status_t MPEG2PSExtractor::WrappedTrack::start(MetaData *params) {
    return mTrack->start(params);
}

status_t MPEG2PSExtractor::WrappedTrack::stop() {
    return mTrack->stop();
}

sp<MetaData> MPEG2PSExtractor::WrappedTrack::getFormat() {
#ifdef RTK_PLATFORM
    sp<MetaData> meta = mTrack->getFormat();
    if(meta != NULL)
        meta->setInt64(kKeyDuration, mExtractor->getDurationUs());
#endif
    return mTrack->getFormat();
}

status_t MPEG2PSExtractor::WrappedTrack::read(
        MediaBuffer **buffer, const ReadOptions *options) {
    return mTrack->read(buffer, options);
}

////////////////////////////////////////////////////////////////////////////////

bool SniffMPEG2PS(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *) {
#ifdef RTK_PLATFORM
    size_t readSize = 100*1024;//Increase preload data size
    uint8_t *preloadData = (uint8_t *)malloc(readSize);
    while (source->readAt(0, preloadData, readSize) < (ssize_t)readSize) {
        readSize = readSize/2;
        if(readSize < 512)
        {
            free(preloadData);
            return false;//Too small
        }
        else
        {
            free(preloadData);
            preloadData = (uint8_t *)malloc(readSize);
        }
    }

    size_t offset = 0;
    while(preloadData[offset] != 0x00 || preloadData[offset+1] != 0x00 || preloadData[offset+2] != 0x01)
    {
        offset++;
        if(offset > readSize-3)
        {
            free(preloadData);
            return false;
        }
    }
    uint8_t header[5];
    memcpy(header, preloadData+offset, 5);
#else
    uint8_t header[5];
    if (source->readAt(0, header, sizeof(header)) < (ssize_t)sizeof(header)) {
        return false;
    }
#endif

#ifdef RTK_PLATFORM
    if (memcmp("\x00\x00\x01\xba", header, 4))
    {
        free(preloadData);
        return false;
    }
    if(((header[4] >> 6) != 1) && ((header[4] >> 4) != 2))//Check MPEG1 and MPEG2 case
    {
        free(preloadData);
        return false;
    }
#else
    if (memcmp("\x00\x00\x01\xba", header, 4) || (header[4] >> 6) != 1) {
        return false;
    }
#endif

    *confidence = 0.25f;  // Slightly larger than .mp3 extractor's confidence

    mimeType->setTo(MEDIA_MIMETYPE_CONTAINER_MPEG2PS);
    free(preloadData);
    return true;
}

}  // namespace android

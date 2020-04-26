/*
 * Copyright (C) 2010 The Android Open Source Project
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
#define LOG_TAG "ESQueue"
#include <media/stagefright/foundation/ADebug.h>

#include "ESQueue.h"

#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/foundation/ABitReader.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/Utils.h>

#include "include/avc_utils.h"
#include "include/hevc_utils.h"
#include <inttypes.h>
#include <netinet/in.h>
#ifdef RTK_PLATFORM
#include <string.h>
#endif
#define lit_big_TransShort(a_data) (((a_data) & 0x00ff) << 8 | ((a_data) & 0xff00) >> 8)
#define MAX_AUDIO_BUFFER_SIZE 32768
namespace android {

ElementaryStreamQueue::ElementaryStreamQueue(Mode mode, uint32_t flags)
    : mMode(mode),
      mFlags(flags),
      mEOSReached(false)
#ifdef RTK_PLATFORM
      ,mOffset(0)
      ,mLastTimeUs(0)
      ,mSeeking(false)
      ,mVideoSeekOver(false)
      ,mGOPTimeSeeking(false)
      ,mGOPTimeDiff(0)
      ,mGOPTimeTmp(0)
      ,mGOPTimeOffset(0)
      ,mGOPOffset(0)
      ,mCheckDisableGOPTPS(0)
      ,mLastGOPOffset(0)
      ,mFirstFrameTimestamp(-1)
      ,mSeekOverTimestamp(-1)
      ,mSkipFrameBeforeTimestamp(-1)
      ,mSkipFrameAfterSeekTimestamp(-1)
      ,mPayloadSize(0)
      ,mEAC3CheckCount(0)
      ,mMPEGVideoSeekTimeStamp(-1)
      ,mMPEGVideoLastPTS(0)
#endif
      {
#ifdef RTK_PLATFORM
      ALOGD("Create ESQueue with mode %s ", getTrackModetoString(mMode));
#endif
}

sp<MetaData> ElementaryStreamQueue::getFormat() {
    return mFormat;
}

#ifdef RTK_PLATFORM
void ElementaryStreamQueue::setSeeking() {
    mVideoSeekOver = false;
    mSeeking = true;
    mGOPTimeSeeking = true;
    mSkipFrameAfterSeekTimestamp =-1;
}

bool ElementaryStreamQueue::getSeekOverStatus() {
    return mVideoSeekOver;
}


void ElementaryStreamQueue::setSeekStatusOver() {
    mSeeking = false;
}

bool ElementaryStreamQueue::getSeekStatus() {
    return mSeeking;
}

char* ElementaryStreamQueue::getTrackModetoString(Mode mode) {
    switch(mode)
    {
        case H264: return (char*)"H264";
        case H265: return (char*)"H265";
        case WVC1: return (char*)"WVC1";
        case AAC: return (char*)"AAC";
        case AC3: return (char*)"AC3";
        case MPEG_AUDIO: return (char*)"MPEG_AUDIO";
        case MPEG_VIDEO: return (char*)"MPEG_VIDEO";
        case MPEG4_VIDEO: return (char*)"MPEG4_VIDEO";
        case PCM_AUDIO: return (char*)"PCM_AUDIO";
        case LPCM_AUDIO: return (char*)"LPCM_AUDIO";
        case DTS: return (char*)"DTS";
        case DTS_HD: return (char*)"DTS_HD";
        case TRUEHD: return (char*)"TRUEHD";
        case AAC_LATM: return (char*)"AAC_LATM";
        case METADATA: return (char*)"METADATA";
        case AVS: return (char*)"AVS";
        default: return (char*)"UNKNOW";
    }
}

bool ElementaryStreamQueue::IsIFrame(uint8_t * nalStart, size_t nalSize) {
    unsigned nalType = nalStart[0] & 0x1f;
    unsigned slice_type = 0;

    if ((nalType > 0 && nalType < 6) || nalType == 19) {
        ABitReader br(nalStart + 1, nalSize);
        //unsigned first_mb_in_slice = parseUE(&br);
        //ALOGI("first_mb_in_slice %d ", first_mb_in_slice);
        slice_type = parseUE(&br);
    }

    if ((nalType == 7) ||    //PPS
        ((nalType == 5 || slice_type == 2 || slice_type == 7))) {   // I frame
        ALOGI("%s() nalType=%d slice_type=%d ,nalSize:%zu", __FUNCTION__, nalType, slice_type, nalSize);
        return true;
    }
    return false;
}

int64_t ElementaryStreamQueue::CheckGOPPTS(int64_t timeUs) {
    int64_t newTimeUs = timeUs;
    if(mLastTimeUs > 0 && timeUs == 0)
        timeUs = mGOPTimeTmp;
    if(timeUs == mGOPTimeTmp ) {
        if(mGOPTimeOffset > 0)
            newTimeUs = timeUs + (mGOPOffset*mGOPTimeOffset);
        mGOPOffset++;
    }
    else {
        if(mGOPTimeDiff > 0 && mGOPOffset > 1) {
            mGOPTimeOffset = (mGOPTimeDiff / mGOPOffset);
            //ALOGD("mGOPTimeDiff %" PRId64 " mGOPTimeOffset %" PRId64 " mGOPOffset %d", mGOPTimeDiff, mGOPTimeOffset, mGOPOffset);
        }
        mGOPTimeTmp = timeUs;
        mGOPOffset = 1;
    }

    //Check if we can disable GOP PTS check
    if(mLastGOPOffset == mGOPOffset)
        mCheckDisableGOPTPS++;
    else
        mCheckDisableGOPTPS = 0;
    mLastGOPOffset = mGOPOffset;
    if(mCheckDisableGOPTPS > 20)
        mGOPTimeOffset = 0;//Reset offset, it should be normal GOP case
    return newTimeUs;
}

int64_t ElementaryStreamQueue::getFirstFrameTimestamp() {
    return mFirstFrameTimestamp;
}

int64_t ElementaryStreamQueue::getSeekOverTimestamp() {
    return mSeekOverTimestamp;
}

void ElementaryStreamQueue::setSkipFrameAfterSeekTimestamp(int64_t timeUs) {
    mSkipFrameAfterSeekTimestamp = timeUs;
}


void ElementaryStreamQueue::setSkipFrameBeforeTimestamp(int64_t timeUs) {
    if(timeUs > 200000)
    {
        //200ms threshold for testPlayHlsStreamWithTimedId3
        mSkipFrameBeforeTimestamp = timeUs - 200000;
    }
    else
        mSkipFrameBeforeTimestamp = 0;
}

int64_t ElementaryStreamQueue::getSkipFrmaeBeforeTimestamp() {
    return mSkipFrameBeforeTimestamp;
}

#endif

void ElementaryStreamQueue::clear(bool clearFormat) {
    if (mBuffer != NULL) {
        mBuffer->setRange(0, 0);
    }

    mRangeInfos.clear();

#ifdef RTK_PLATFORM
    if(mMode == H264) {
        accessUnits.clear();
    }
    mGOPTimeTmp = 0;
    mLastTimeUs = 0;
    mGOPOffset = 0;
#endif

    if (clearFormat) {
        mFormat.clear();
    }

    mEOSReached = false;
}

// Parse AC3 header assuming the current ptr is start position of syncframe,
// update metadata only applicable, and return the payload size
#ifdef RTK_PLATFORM
unsigned ElementaryStreamQueue::parseAC3SyncFrame(
#else
static unsigned parseAC3SyncFrame(
#endif
        const uint8_t *ptr, size_t size, sp<MetaData> *metaData) {
    static const unsigned channelCountTable[] = {2, 1, 2, 3, 3, 4, 4, 5};
    static const unsigned samplingRateTable[] = {48000, 44100, 32000};
    //static const uint8_t eac3_blocks[4] = {1, 2, 3, 6};
    static const unsigned frameSizeTable[19][3] = {
        { 64, 69, 96 },
        { 80, 87, 120 },
        { 96, 104, 144 },
        { 112, 121, 168 },
        { 128, 139, 192 },
        { 160, 174, 240 },
        { 192, 208, 288 },
        { 224, 243, 336 },
        { 256, 278, 384 },
        { 320, 348, 480 },
        { 384, 417, 576 },
        { 448, 487, 672 },
        { 512, 557, 768 },
        { 640, 696, 960 },
        { 768, 835, 1152 },
        { 896, 975, 1344 },
        { 1024, 1114, 1536 },
        { 1152, 1253, 1728 },
        { 1280, 1393, 1920 },
    };

    ABitReader bits(ptr, size);
    if (bits.numBitsLeft() < 16) {
        return 0;
    }
    if (bits.getBits(16) != 0x0B77) {
        return 0;
    }

    if (bits.numBitsLeft() < 16 + 2 + 6 + 5 + 3 + 3) {
        ALOGV("Not enough bits left for further parsing");
        return 0;
    }
    unsigned preloadBsid = bits.data()[3]>>3;
    if (preloadBsid >= 10 && preloadBsid <= 16) {//E-AC3 case
        unsigned int strmtyp, substreamid, frmsiz, fscod2, numblkscod, sample_rate, acmod, lfeon;
        strmtyp = bits.getBits(2);
        substreamid = bits.getBits(3);

        if (strmtyp == 3)//EAC3_FRAME_TYPE_RESERVED
            return 0;

        frmsiz = bits.getBits(11) + 1;
        unsigned fscod = bits.getBits(2);
        if (fscod == 3) {
            fscod2 = bits.getBits(2);
            numblkscod = 3;

            if(fscod2 == 3)
                return 0;

            sample_rate = samplingRateTable[fscod2] / 2;
        } else {
            numblkscod = bits.getBits(2);

            sample_rate = samplingRateTable[fscod];
        }

        acmod = bits.getBits(3);
        lfeon = bits.getBits(1);

        //unsigned samples = eac3_blocks[numblkscod] * 256;
        //unsigned bit_rate = frmsiz * sample_rate * 16 / (samples);
        unsigned channels = channelCountTable[acmod] + lfeon;
        if (metaData != NULL) {
            (*metaData)->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_EAC3);
            (*metaData)->setInt32(kKeyChannelCount, channels);
            (*metaData)->setInt32(kKeySampleRate, sample_rate);
        }
        return frmsiz * 2;
    }
    else if (preloadBsid <= 8) {//AC3 case
        bits.skipBits(16);  // crc1

        unsigned fscod = bits.getBits(2);
        if (fscod == 3) {
            ALOGW("Incorrect fscod in AC3 header");
            return 0;
        }

        unsigned frmsizecod = bits.getBits(6);
        if (frmsizecod > 37) {
            ALOGW("Incorrect frmsizecod in AC3 header");
            return 0;
        }

        bits.skipBits(5);//bsid
        unsigned bsmod __unused = bits.getBits(3);
        unsigned acmod = bits.getBits(3);
        unsigned cmixlev __unused = 0;
        unsigned surmixlev __unused = 0;
        unsigned dsurmod __unused = 0;

        if ((acmod & 1) > 0 && acmod != 1) {
            if (bits.numBitsLeft() < 2) {
                return 0;
            }
            cmixlev = bits.getBits(2);
        }
        if ((acmod & 4) > 0) {
            if (bits.numBitsLeft() < 2) {
                return 0;
            }
            surmixlev = bits.getBits(2);
        }
        if (acmod == 2) {
            if (bits.numBitsLeft() < 2) {
                return 0;
            }
            dsurmod = bits.getBits(2);
        }

        if (bits.numBitsLeft() < 1) {
            return 0;
        }
        unsigned lfeon = bits.getBits(1);

        unsigned samplingRate = samplingRateTable[fscod];
        unsigned payloadSize = frameSizeTable[frmsizecod >> 1][fscod];
        if (fscod == 1) {
            payloadSize += frmsizecod & 1;
        }
        payloadSize <<= 1;  // convert from 16-bit words to bytes

        unsigned channelCount = channelCountTable[acmod] + lfeon;

        if (metaData != NULL) {
            (*metaData)->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AC3);
            (*metaData)->setInt32(kKeyChannelCount, channelCount);
            (*metaData)->setInt32(kKeySampleRate, samplingRate);
        }

        return payloadSize;
    }
    else {
        ALOGW("%s cannot recognized format", __FUNCTION__);
        return 0;
    }
}
#if 0
static bool IsSeeminglyValidAC3Header(const uint8_t *ptr, size_t size) {
    return parseAC3SyncFrame(ptr, size, NULL) > 0;
}
#endif

static bool IsSeeminglyValidADTSHeader(
        const uint8_t *ptr, size_t size, size_t *frameLength) {
    if (size < 7) {
        // Not enough data to verify header.
        return false;
    }

    if (ptr[0] != 0xff || (ptr[1] >> 4) != 0x0f) {
        return false;
    }

    unsigned layer = (ptr[1] >> 1) & 3;

    if (layer != 0) {
        return false;
    }

    unsigned ID = (ptr[1] >> 3) & 1;
    unsigned profile_ObjectType = ptr[2] >> 6;

    if (ID == 1 && profile_ObjectType == 3) {
        // MPEG-2 profile 3 is reserved.
        return false;
    }

    size_t frameLengthInHeader =
            ((ptr[3] & 3) << 11) + (ptr[4] << 3) + ((ptr[5] >> 5) & 7);
    if (frameLengthInHeader > size) {
        return false;
    }

    *frameLength = frameLengthInHeader;
    return true;
}

static bool IsSeeminglyValidMPEGAudioHeader(const uint8_t *ptr, size_t size) {
    if (size < 3) {
        // Not enough data to verify header.
        return false;
    }

    if (ptr[0] != 0xff || (ptr[1] >> 5) != 0x07) {
        return false;
    }

    unsigned ID = (ptr[1] >> 3) & 3;

    if (ID == 1) {
        return false;  // reserved
    }

    unsigned layer = (ptr[1] >> 1) & 3;

    if (layer == 0) {
        return false;  // reserved
    }

    unsigned bitrateIndex = (ptr[2] >> 4);

    if (bitrateIndex == 0x0f) {
        return false;  // reserved
    }

    unsigned samplingRateIndex = (ptr[2] >> 2) & 3;

    if (samplingRateIndex == 3) {
        return false;  // reserved
    }

    return true;
}

status_t ElementaryStreamQueue::appendData(
        const void *data, size_t size, int64_t timeUs) {
    if (mEOSReached) {
        ALOGE("appending data after EOS");
        return ERROR_MALFORMED;
    }
    if (mBuffer == NULL || mBuffer->size() == 0) {
        switch (mMode) {
            case H264:
#ifndef RTK_PLATFORM
            case MPEG_VIDEO:
#endif
            {
#if 0
                if (size < 4 || memcmp("\x00\x00\x00\x01", data, 4)) {
                    return ERROR_MALFORMED;
                }
#else
                uint8_t *ptr = (uint8_t *)data;

                ssize_t startOffset = -1;
                for (size_t i = 0; i + 2 < size; ++i) {
                    if (!memcmp("\x00\x00\x01", &ptr[i], 3)) {
                        startOffset = i;
                        break;
                    }
                }

                if (startOffset < 0) {
                    return ERROR_MALFORMED;
                }

                if (startOffset > 0) {
                    ALOGI("found something resembling an H.264/MPEG syncword "
                          "at offset %zd",
                          startOffset);
                }

                data = &ptr[startOffset];
                size -= startOffset;
#endif
                break;
            }

            case MPEG4_VIDEO:
            {
#if 0
                if (size < 3 || memcmp("\x00\x00\x01", data, 3)) {
                    return ERROR_MALFORMED;
                }
#else
                uint8_t *ptr = (uint8_t *)data;

                ssize_t startOffset = -1;
                for (size_t i = 0; i + 2 < size; ++i) {
                    if (!memcmp("\x00\x00\x01", &ptr[i], 3)) {
                        startOffset = i;
                        break;
                    }
                }

                if (startOffset < 0) {
                    return ERROR_MALFORMED;
                }

                if (startOffset > 0) {
                    ALOGI("found something resembling an H.264/MPEG syncword "
                          "at offset %zd",
                          startOffset);
                }

                data = &ptr[startOffset];
                size -= startOffset;
#endif
                break;
            }
            case AAC:
            {
                uint8_t *ptr = (uint8_t *)data;

#if 0
                if (size < 2 || ptr[0] != 0xff || (ptr[1] >> 4) != 0x0f) {
                    return ERROR_MALFORMED;
                }
#else
                ssize_t startOffset = -1;
                size_t frameLength;
                for (size_t i = 0; i < size; ++i) {
                    if (IsSeeminglyValidADTSHeader(
                            &ptr[i], size - i, &frameLength)) {
                        startOffset = i;
                        break;
                    }
                }

                if (startOffset < 0) {
                    return ERROR_MALFORMED;
                }

                if (startOffset > 0) {
                    ALOGI("found something resembling an AAC syncword at "
                          "offset %zd",
                          startOffset);
                }

                if (frameLength != size - startOffset) {
                    ALOGV("First ADTS AAC frame length is %zd bytes, "
                          "while the buffer size is %zd bytes.",
                          frameLength, size - startOffset);
                }

                data = &ptr[startOffset];
                size -= startOffset;
#endif
                break;
            }
#if 0
            case AC3:
            {
                uint8_t *ptr = (uint8_t *)data;

                ssize_t startOffset = -1;
                for (size_t i = 0; i < size; ++i) {
                    if (IsSeeminglyValidAC3Header(&ptr[i], size - i)) {
                        startOffset = i;
                        break;
                    }
                }

                if (startOffset < 0) {
                    return ERROR_MALFORMED;
                }

                if (startOffset > 0) {
                    ALOGI("found something resembling an AC3 syncword at "
                          "offset %zd",
                          startOffset);
                }

                data = &ptr[startOffset];
                size -= startOffset;
                break;
            }
#endif
            case MPEG_AUDIO:
            {
                uint8_t *ptr = (uint8_t *)data;

                ssize_t startOffset = -1;
                for (size_t i = 0; i < size; ++i) {
                    if (IsSeeminglyValidMPEGAudioHeader(&ptr[i], size - i)) {
                        startOffset = i;
                        break;
                    }
                }

                if (startOffset < 0) {
                    return ERROR_MALFORMED;
                }

                if (startOffset > 0) {
                    ALOGI("found something resembling an MPEG audio "
                          "syncword at offset %zd",
                          startOffset);
                }

                data = &ptr[startOffset];
                size -= startOffset;
                break;
            }
#ifdef RTK_PLATFORM //Directly dump, skip parsing case
            case TRUEHD:
            case DTS:
            case DTS_HD:
            case MPEG_VIDEO:
            case H265:
            case WVC1:
            case AAC_LATM:
            case AVS:
            case AC3:
            case LPCM_AUDIO:
#endif
            case PCM_AUDIO:
            case METADATA:
            {
                break;
            }

            default:
                ALOGE("Unknown mode: %d", mMode);
                return ERROR_MALFORMED;
        }
    }
#ifdef RTK_PLATFORM
    uint8_t *ptr = (uint8_t *)data;
    switch(mMode)//For Specific file source exception handler
    {
        case TRUEHD:
        {
            if(ptr[0] == 0xb && ptr[1] == 0x77)
                return OK;//AC3 case, skip it
            break;
        }
        case AC3:
        {
            if(ptr[0] >= 0x80 && ptr[0] <= 0x89)//AC3 extend ID range
            {
                //Some PES header was not 1 on 1 mapping to AC3 frame
                data = &ptr[4];
                size -= 4;
            }
            if(timeUs == 0 && mLastTimeUs != 0)
                timeUs = mLastTimeUs;
            break;
        }
        default:
            break;
    }
#endif

    size_t neededSize = (mBuffer == NULL ? 0 : mBuffer->size()) + size;
    if (mBuffer == NULL || neededSize > mBuffer->capacity()) {
        neededSize = (neededSize + 65535) & ~65535;

        ALOGV("resizing buffer to size %zu", neededSize);

        sp<ABuffer> buffer = new ABuffer(neededSize);
        if (mBuffer != NULL) {
            memcpy(buffer->data(), mBuffer->data(), mBuffer->size());
            buffer->setRange(0, mBuffer->size());
        } else {
            buffer->setRange(0, 0);
        }

        mBuffer = buffer;
    }

    memcpy(mBuffer->data() + mBuffer->size(), data, size);
    mBuffer->setRange(0, mBuffer->size() + size);

    RangeInfo info;
    info.mLength = size;
    info.mTimestampUs = timeUs;
#ifdef RTK_PLATFORM
    if(mLastTimeUs > 0 && timeUs == 0)
        timeUs = mLastTimeUs;

    if(mLastTimeUs != timeUs)
    {
        if(!mGOPTimeSeeking){
            mGOPTimeDiff = timeUs - mLastTimeUs;
        }else{
            if((timeUs - mLastTimeUs)<=mGOPTimeDiff){
                mGOPTimeSeeking = false;
            }
        }
    }

    if(mLastTimeUs == 0 || (mLastTimeUs > 0 && timeUs != 0))
        mLastTimeUs = timeUs;
#endif
    mRangeInfos.push_back(info);

    return OK;
}

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnit() {
    if ((mFlags & kFlag_AlignedData) && mMode == H264) {
        if (mRangeInfos.empty()) {
            return NULL;
        }

        RangeInfo info = *mRangeInfos.begin();
        mRangeInfos.erase(mRangeInfos.begin());

        sp<ABuffer> accessUnit = new ABuffer(info.mLength);
        memcpy(accessUnit->data(), mBuffer->data(), info.mLength);
#ifdef RTK_PLATFORM
        if(mFirstFrameTimestamp == -1)
            mFirstFrameTimestamp = info.mTimestampUs;
#endif
        accessUnit->meta()->setInt64("timeUs", info.mTimestampUs);

        memmove(mBuffer->data(),
                mBuffer->data() + info.mLength,
                mBuffer->size() - info.mLength);

        mBuffer->setRange(0, mBuffer->size() - info.mLength);

        if (mFormat == NULL) {
            mFormat = MakeAVCCodecSpecificData(accessUnit);
        }

        return accessUnit;
    }

    switch (mMode) {
        case H264:
            return dequeueAccessUnitH264();
        case AAC:
            return dequeueAccessUnitAAC();
        case AC3:
            return dequeueAccessUnitAC3();
        case MPEG_VIDEO:
            return dequeueAccessUnitMPEGVideo();
        case MPEG4_VIDEO:
            return dequeueAccessUnitMPEG4Video();
        case PCM_AUDIO:
            return dequeueAccessUnitPCMAudio();
#ifdef RTK_PLATFORM
        case H265:
            return dequeueAccessUnitH265();
        case WVC1:
            return dequeueAccessUnitWVC1();
        case TRUEHD:
            return dequeueAccessUnitTrueHD();
        case DTS:
        case DTS_HD:
            return dequeueAccessUnitDTS();
        case AAC_LATM:
            return dequeueAccessUnitAACLATM();
        case AVS:
            return dequeueAccessUnitAVS();
        case LPCM_AUDIO:
            return dequeueAccessUnitPCMAudio();
#endif
        case METADATA:
            return dequeueAccessUnitMetadata();
        default:
            if (mMode != MPEG_AUDIO) {
                ALOGE("Unknown mode");
                return NULL;
            }
            return dequeueAccessUnitMPEGAudio();
    }
}


#ifdef RTK_PLATFORM
sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitTrueHD() {
    static const unsigned trueHDsamplingRateTable[] = {48000, 96000, 192000, 0, 0, 0, 0, 0, 44100, 88200, 176400};
    static const uint8_t thd_chancount[13] = {
    //  LR  C  LFE  LRs  LRvh  LRc  LRrs  Cs  Ts  LRsd  LRw Cvh  LFE2
        2, 1, 1,  2,   2,    2,  2,   1,  1,  2,   2,  1,   1 };

    if (mBuffer == NULL || mBuffer->size() == 0) {
        return NULL;
    }

    if (mRangeInfos.empty()) {
        return NULL;
    }

    size_t offset = 0;
    if(offset + 32 > mBuffer->size())
        return NULL;

    if(mFormat == NULL) {
        const uint8_t *data = mBuffer->data();
        int channels = 0;
        int sampleRate = 0;
        while(offset + 7 < mBuffer->size()) {
            if(data[offset+4] == 0xf8 && data[offset+5] == 0x72 && data[offset+6] == 0x6f && (data[offset+7]&0xfe) == 0xba) {
                ABitReader bits(mBuffer->data() + offset, mBuffer->size() - offset);
                bits.skipBits(4);//check_nibble
                bits.skipBits(12);//access_unit_length
                bits.skipBits(16);//input_timming
                bits.skipBits(32);//sync word
                unsigned rateBits = bits.getBits(4);
                if(rateBits < 11)
                    sampleRate = trueHDsamplingRateTable[rateBits];
                bits.skipBits(8);
                unsigned channelArrange = bits.getBits(5);//6ch_channel_assignment
                int i;
                int ch6Cnt = 0;
                for (i = 0; i < 13; i++)
                    ch6Cnt += thd_chancount[i] * ((channelArrange >> i) & 1);
                bits.skipBits(2);
                int ch8Cnt = 0;
                channelArrange = bits.getBits(13);//8ch_channel_assignment
                for (i = 0; i < 13; i++)
                    ch8Cnt += thd_chancount[i] * ((channelArrange >> i) & 1);
                if(ch8Cnt != 0)
                    channels = ch8Cnt;
                else
                    channels = ch6Cnt;
                break;
            }
            offset++;
        }
        if(offset + 8 >= mBuffer->size()) {
            offset = mBuffer->size();
            fetchTimestamp(offset);
            memmove(mBuffer->data(), mBuffer->data() + offset, mBuffer->size() - offset);
            mBuffer->setRange(0, mBuffer->size() - offset);
            return NULL;
        }
        mFormat = new MetaData;
        mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MLP);
        mFormat->setInt32(kKeyChannelCount, channels);
        mFormat->setInt32(kKeySampleRate, sampleRate);
        ALOGD("TrueHD channel %d sampleRate %d", channels, sampleRate);
    }

    if(mBuffer->size() < 4096)//Buffer 4K data for performance
        return NULL;

    offset = mBuffer->size();
    if(offset > MAX_AUDIO_BUFFER_SIZE)//Over codec buffer size
        offset = MAX_AUDIO_BUFFER_SIZE;
    int64_t timeUs = fetchTimestamp(offset);
    sp<ABuffer> accessUnit = new ABuffer(offset);
    memcpy(accessUnit->data(), mBuffer->data(), offset);
    memmove(mBuffer->data(), mBuffer->data() + offset, mBuffer->size() - offset);
    mBuffer->setRange(0, mBuffer->size() - offset);
    accessUnit->meta()->setInt64("timeUs", timeUs);
    accessUnit->meta()->setInt32("isSync", 1);
    if(timeUs < mSkipFrameBeforeTimestamp)
    {
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameBeforeTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameBeforeTimestamp);
        return NULL;
    }
    return accessUnit;
}
#endif

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitAC3() {
    unsigned syncStartPos = 0;  // in bytes
    unsigned payloadSize = 0;
    sp<MetaData> format = new MetaData;
    while (true) {
        if (syncStartPos + 2 >= mBuffer->size()) {
            return NULL;
        }

        payloadSize = parseAC3SyncFrame(
                mBuffer->data() + syncStartPos,
                mBuffer->size() - syncStartPos,
                &format);
        if (payloadSize > 0) {
            break;
        }
        ++syncStartPos;
    }

    if (mBuffer->size() < syncStartPos + payloadSize) {
        ALOGV("Not enough buffer size for AC3");
        return NULL;
    }

    if (mFormat == NULL) {
        mEAC3CheckCount++;
        if(mEAC3CheckCount < 4)//Check 4 audio frame to make sure there exist E-AC3 frame
        {
            const char *mime;
            format->findCString(kKeyMIMEType, &mime);
            if(!strcmp(mime, MEDIA_MIMETYPE_AUDIO_EAC3))
            {
                mFormat = format;
            }
        }
        else
            mFormat = format;
    }

#ifndef RTK_PLATFORM
    sp<ABuffer> accessUnit = new ABuffer(syncStartPos + payloadSize);
    memcpy(accessUnit->data(), mBuffer->data(), syncStartPos + payloadSize);
#else
    sp<ABuffer> accessUnit = new ABuffer(payloadSize);
    memcpy(accessUnit->data(), mBuffer->data()+syncStartPos, payloadSize);
#endif

    int64_t timeUs = fetchTimestamp(syncStartPos + payloadSize);
    if (timeUs < 0ll) {
        ALOGE("negative timeUs");
        return NULL;
    }
    accessUnit->meta()->setInt64("timeUs", timeUs);
    accessUnit->meta()->setInt32("isSync", 1);

    memmove(
            mBuffer->data(),
            mBuffer->data() + syncStartPos + payloadSize,
            mBuffer->size() - syncStartPos - payloadSize);

    mBuffer->setRange(0, mBuffer->size() - syncStartPos - payloadSize);
#ifdef RTK_PLATFORM
    if(timeUs <= mSkipFrameBeforeTimestamp)
    {
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameBeforeTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameBeforeTimestamp);
        return NULL;
    }
    if(mSeeking){
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameAfterSeekTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameAfterSeekTimestamp);
        return NULL;
    }
    if(mSkipFrameAfterSeekTimestamp != -1)
    {
        if(timeUs < mSkipFrameAfterSeekTimestamp)
            return NULL;
        else
            mSkipFrameAfterSeekTimestamp = -1;
    }
#endif
    return accessUnit;
}

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitPCMAudio() {
#ifdef RTK_PLATFORM
    static const uint8_t bits_per_samples[4] = { 0, 16, 20, 24 };
    static const uint8_t channels[16] = {
        0, 1, 0, 2, 3, 3, 4, 4, 5, 6, 7, 8, 0, 0, 0, 0
    };

    unsigned lengthHeadr = 4; //Audio substream headers + :PCM specific header
    if(mMode == PCM_AUDIO)
        lengthHeadr = 4;
    else if(mMode == LPCM_AUDIO)
        lengthHeadr = 7;
    uint8_t startCode[lengthHeadr];
    if (mBuffer->size() < lengthHeadr) {
#else
    if (mBuffer->size() < 4) {
#endif
        return NULL;
    }

#ifdef RTK_PLATFORM
    ABitReader bits(mBuffer->data(), lengthHeadr);
#else
    ABitReader bits(mBuffer->data(), 4);
#endif

    const uint8_t *data = mBuffer->data();

#ifndef RTK_PLATFORM
    unsigned numAUs = bits.getBits(8);
    bits.skipBits(8);
    unsigned quantization_word_length __unused = bits.getBits(2);
    unsigned audio_sampling_frequency = bits.getBits(3);
    unsigned num_channels = bits.getBits(3);

    if (audio_sampling_frequency != 2) {
        ALOGE("Wrong sampling freq");
        return NULL;
    }
    if (num_channels != 1u) {
        ALOGE("Wrong channel #");
        return NULL;
    }

    if (mFormat == NULL) {
        mFormat = new MetaData;
        mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_RAW);
        mFormat->setInt32(kKeyChannelCount, 2);
        mFormat->setInt32(kKeySampleRate, 48000);
        mFormat->setInt32(kKeyPcmEncoding, kAudioEncodingPcm16bit);
    }

    static const size_t kFramesPerAU = 80;
    size_t frameSize = 2 /* numChannels */ * sizeof(int16_t);

    size_t payloadSize = numAUs * frameSize * kFramesPerAU;

    if (mBuffer->size() < 4 + payloadSize) {
        return NULL;
    }

    sp<ABuffer> accessUnit = new ABuffer(payloadSize);
    memcpy(accessUnit->data(), mBuffer->data() + 4, payloadSize);

    int64_t timeUs = fetchTimestamp(payloadSize + 4);
    if (timeUs < 0ll) {
        ALOGE("Negative timeUs");
        return NULL;
    }
    accessUnit->meta()->setInt64("timeUs", timeUs);
    accessUnit->meta()->setInt32("isSync", 1);

    int16_t *ptr = (int16_t *)accessUnit->data();
    for (size_t i = 0; i < payloadSize / sizeof(int16_t); ++i) {
        ptr[i] = ntohs(ptr[i]);
    }

    memmove(
            mBuffer->data(),
            mBuffer->data() + 4 + payloadSize,
            mBuffer->size() - 4 - payloadSize);

    mBuffer->setRange(0, mBuffer->size() - 4 - payloadSize);
    if(timeUs < mSkipFrameBeforeTimestamp)
    {
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameBeforeTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameBeforeTimestamp);
        return NULL;
    }
#else
    int32_t channelCount = channels[data[2] >> 4];
    int32_t sampleRate = 0;
    int32_t bitPerSample = bits_per_samples[data[3] >> 6];

    switch (data[2] & 0x0f) {
        case 1:
            sampleRate = 48000;
            break;
        case 4:
            sampleRate = 96000;
            break;
        case 5:
            sampleRate = 192000;
            break;
        default:
            sampleRate = -1;
            break;
    }

    if(mMode == LPCM_AUDIO) {
        //static const uint32_t frequencies[4] = { 48000, 96000, 44100, 32000 };
        //channelCount = 1 + (data[1] & 7);
        //sampleRate = frequencies[data[1] >> 4 & 3];
        //ALOGD("channelCount %d sampleRate %d ", channelCount, sampleRate);
        channelCount = 2;
        bitPerSample = 16;
        sampleRate = 48000;
    }

    if (mFormat == NULL) {
        mFormat = new MetaData;
        mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_RAW);
        mFormat->setInt32(kKeyChannelCount, channelCount);
        mFormat->setInt32(kKeyAudioBitsPerSample, bitPerSample);
        mFormat->setInt32(kKeySampleRate, sampleRate);
        for(size_t i=0; i<lengthHeadr; i++) {
            startCode[i] = data[i];
            //ALOGV("startCode 0x%x ", startCode[i]);
        }

        //Try to find frame size to avoid input buffer overflow
        for(size_t i=lengthHeadr; i<mBuffer->size() - lengthHeadr; i++) {
            bool gotNextStartCode = true;
            for (size_t j=0; j<lengthHeadr; j++)
                if(startCode[j] != data[i+j]) {
                    gotNextStartCode = false;
                    break;
                }
            if(gotNextStartCode) {
                mPayloadSize = i - lengthHeadr;
                break;
            }
        }
    }

    size_t payloadSize = 0;
    if(mPayloadSize > 0)
        payloadSize = mPayloadSize;
    else
        payloadSize = mBuffer->size() - lengthHeadr;

    sp<ABuffer> accessUnit = new ABuffer(payloadSize);
    memcpy(accessUnit->data(), mBuffer->data() + lengthHeadr, payloadSize);

    int64_t timeUs = fetchTimestamp(payloadSize + lengthHeadr);
    if (timeUs < 0ll) {
        ALOGE("Negative timeUs");
        return NULL;
    }
    accessUnit->meta()->setInt64("timeUs", timeUs);
    accessUnit->meta()->setInt32("isSync", 1);

    int16_t *ptr = (int16_t *)accessUnit->data();
    for (size_t i = 0; i < payloadSize / sizeof(int16_t); ++i) {
        ptr[i] = ntohs(ptr[i]);
    }

    memmove(
            mBuffer->data(),
            mBuffer->data() + lengthHeadr + payloadSize,
            mBuffer->size() - lengthHeadr - payloadSize);

    mBuffer->setRange(0, mBuffer->size() - lengthHeadr - payloadSize);
#endif
    return accessUnit;
}

#ifdef RTK_PLATFORM
sp<MetaData> MakeDTSCodecSpecificData(
        unsigned channel_configuration, unsigned sampling_freq_index, bool isDTS_HD) {
    sp<MetaData> meta = new MetaData;
    if(isDTS_HD)
        meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_DTS_HD);
    else
        meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_DTS);

    static const int32_t kSamplingFreq[] = {
        -1, 8000, 16000, 32000, -1, -1, 11025, 22050,
        44100, -1, -1, 12000, 24000, 48000, -1, -1
    };

    static const int32_t kChannelArrage[] = {
        1, 2, 2, 2, 2, 3, 3, 4,
        4, 5, 6, 6, 6, 7, 8, 8
    };

    if(sampling_freq_index < 16u)
        meta->setInt32(kKeySampleRate, kSamplingFreq[sampling_freq_index]);
    else
        return NULL;
    if(channel_configuration < 16u)
        meta->setInt32(kKeyChannelCount, kChannelArrage[channel_configuration]);
    else
        return NULL;

    ALOGD("MakeDTSCodecSpecificData get audio config %d channel count, %d sample rate "
         , kChannelArrage[channel_configuration], kSamplingFreq[sampling_freq_index]);
    return meta;
}

void ElementaryStreamQueue::endianChange(uint8_t *data,size_t size){
    uint16_t *p =(uint16_t *)data;
    for(uint16_t i=0;i<size/2;i++){
        *p = lit_big_TransShort(*p);
        p++;
    }
}

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitDTS() {
    if (mBuffer->size() == 0) {
        return NULL;
    }

    if (mRangeInfos.empty()) {
        return NULL;
    }

    const RangeInfo &info = *mRangeInfos.begin();
    if (mBuffer->size() < info.mLength) {
        return NULL;
    }

    if (info.mTimestampUs < 0ll) {
        ALOGE("Negative info.mTimestampUs");
        return NULL;
    }

    size_t offset = 0;
    size_t frame_size = 0;

    if(offset + 9 > mBuffer->size())
        return NULL;

    ABitReader bits(mBuffer->data() + offset, mBuffer->size() - offset);
    uint8_t *indata_ptr =mBuffer->data(); //get DTS frame
    int word_mode; //define bitstream
    int le_mode; //define endian mode
    unsigned syncWord = bits.getBits(32);
    switch(syncWord){
        //14 bits LE
        case 0xff1f00e8:
            //make sure frame type is DTS
            if ((indata_ptr[4]&0xf0) != 0xf0 || indata_ptr[5] != 0x07){
                ALOGD("wrong DTS start code");
                return NULL;
            }
            word_mode = 0;
            le_mode = 1;
            break;
        //14 bit BE
        case 0x1fffe800:
            //make sure frame type is DTS
            if (indata_ptr[4] != 0x07 || (indata_ptr[5]&0xf0) != 0xf0){
                ALOGD("wrong DTS start code");
                return NULL;
            }
            word_mode =0;
            le_mode = 0;
            break;
        //16 bits LE
        case 0xfe7f0180:
            word_mode = 1;
            le_mode = 1;
            break;
        // 16 bits BE
        case 0x5a5a5a5a://XCH
        case 0x1D95F262://X96K
        case 0x64582025://EXSS
        case 0x7ffe8001:
            word_mode = 1;
            le_mode = 0;
            break;
        default:
            offset = info.mLength;
            fetchTimestamp(offset);
            memmove(mBuffer->data(), mBuffer->data() + offset, mBuffer->size() - offset);
            mBuffer->setRange(0, mBuffer->size() - offset);
            ALOGV("Wrong DTS start code 0x%x , drop buffer size %zu ", syncWord, offset);
            return NULL;
    }

    if(offset + frame_size > mBuffer->size())
        return NULL;
    //parse DTS head
    if(word_mode){
        frame_size = (indata_ptr[5-le_mode] & 0x03) << 12 | indata_ptr[6+le_mode] << 4| indata_ptr[7-le_mode] >> 4;
        ++frame_size;//frame size
        unsigned amode = (indata_ptr[7-le_mode] & 0x0f) << 2 | indata_ptr[8+le_mode] >> 6;//amode
        unsigned sfreq = indata_ptr[8+le_mode] >>2 & 0x0f;//sfreq
        if(mFormat == NULL)
            mFormat = MakeDTSCodecSpecificData(amode, sfreq, mMode==DTS_HD);
    }else{
        frame_size = (indata_ptr[6+le_mode] & 0x03) << 12 | indata_ptr[7-le_mode] << 4| (indata_ptr[8+le_mode] & 0x3f) >> 2;//frame size
        ++frame_size;
        unsigned amode = (indata_ptr[8+le_mode] & 0x03) << 4| (indata_ptr[9-le_mode] & 0xf0) >> 4;//amode
        unsigned sfreq = indata_ptr[9-le_mode] & 0x0f;//sfreq
        if(mFormat == NULL)
            mFormat = MakeDTSCodecSpecificData(amode, sfreq, mMode==DTS_HD);
    }
    //endianChange if Little endian;
    if(le_mode){
        ALOGD("little endian and into endianChange");
        endianChange(mBuffer->data(),mBuffer->size());
    }

    offset = info.mLength;
    //ALOGD("Buffer %zu frame %zu info %zu ", mBuffer->size(), frame_size, offset);

    int64_t timeUs = fetchTimestamp(offset);
    sp<ABuffer> accessUnit = new ABuffer(offset);
    memcpy(accessUnit->data(), mBuffer->data(), offset);
    memmove(mBuffer->data(), mBuffer->data() + offset, mBuffer->size() - offset);
    mBuffer->setRange(0, mBuffer->size() - offset);
    accessUnit->meta()->setInt64("timeUs", timeUs);
    accessUnit->meta()->setInt32("isSync", 1);
    if(timeUs < mSkipFrameBeforeTimestamp)
    {
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameBeforeTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameBeforeTimestamp);
        return NULL;
    }
    return accessUnit;
}
#endif

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitAAC() {
    if (mBuffer->size() == 0) {
        return NULL;
    }

    if (mRangeInfos.empty()) {
        return NULL;
    }

    const RangeInfo &info = *mRangeInfos.begin();
    if (mBuffer->size() < info.mLength) {
        return NULL;
    }

    if (info.mTimestampUs < 0ll) {
        ALOGE("Negative info.mTimestampUs");
        return NULL;
    }

    // The idea here is consume all AAC frames starting at offsets before
    // info.mLength so we can assign a meaningful timestamp without
    // having to interpolate.
    // The final AAC frame may well extend into the next RangeInfo but
    // that's ok.
    size_t offset = 0;
    while (offset < info.mLength) {
        if (offset + 7 > mBuffer->size()) {
            return NULL;
        }

        ABitReader bits(mBuffer->data() + offset, mBuffer->size() - offset);

        // adts_fixed_header

        if (bits.getBits(12) != 0xfffu) {
            ALOGE("Wrong atds_fixed_header");
            return NULL;
        }
        bits.skipBits(3);  // ID, layer
        bool protection_absent __unused = bits.getBits(1) != 0;

        if (mFormat == NULL) {
            unsigned profile = bits.getBits(2);
            if (profile == 3u) {
                ALOGE("profile should not be 3");
                return NULL;
            }
            unsigned sampling_freq_index = bits.getBits(4);
            bits.getBits(1);  // private_bit
            unsigned channel_configuration = bits.getBits(3);
            if (channel_configuration == 0u) {
                ALOGE("channel_config should not be 0");
                return NULL;
            }
            bits.skipBits(2);  // original_copy, home

            mFormat = MakeAACCodecSpecificData(
                    profile, sampling_freq_index, channel_configuration);

            mFormat->setInt32(kKeyIsADTS, true);
            mFormat->setInt32(kKeyMaxInputSize, 10*1024);//Workaround for SW AAC

            int32_t sampleRate;
            int32_t numChannels;
            if (!mFormat->findInt32(kKeySampleRate, &sampleRate)) {
                ALOGE("SampleRate not found");
                return NULL;
            }
            if (!mFormat->findInt32(kKeyChannelCount, &numChannels)) {
                ALOGE("ChannelCount not found");
                return NULL;
            }

            ALOGI("found AAC codec config (%d Hz, %d channels)",
                 sampleRate, numChannels);
        } else {
            // profile_ObjectType, sampling_frequency_index, private_bits,
            // channel_configuration, original_copy, home
            bits.skipBits(12);
        }

        // adts_variable_header

        // copyright_identification_bit, copyright_identification_start
        bits.skipBits(2);

        unsigned aac_frame_length = bits.getBits(13);

        bits.skipBits(11);  // adts_buffer_fullness

        unsigned number_of_raw_data_blocks_in_frame = bits.getBits(2);

        if (number_of_raw_data_blocks_in_frame != 0) {
            // To be implemented.
            ALOGE("Should not reach here.");
            return NULL;
        }

        if (offset + aac_frame_length > mBuffer->size()) {
            return NULL;
        }

        size_t headerSize __unused = protection_absent ? 7 : 9;

        offset += aac_frame_length;
    }

    int64_t timeUs = fetchTimestamp(offset);

    sp<ABuffer> accessUnit = new ABuffer(offset);
    memcpy(accessUnit->data(), mBuffer->data(), offset);

    memmove(mBuffer->data(), mBuffer->data() + offset,
            mBuffer->size() - offset);
    mBuffer->setRange(0, mBuffer->size() - offset);

    accessUnit->meta()->setInt64("timeUs", timeUs);
    accessUnit->meta()->setInt32("isSync", 1);
#ifdef RTK_PLATFORM
    if(timeUs < mSkipFrameBeforeTimestamp)
    {
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameBeforeTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameBeforeTimestamp);
        return NULL;
    }
    if(mSeeking){
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameAfterSeekTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameAfterSeekTimestamp);
        return NULL;
    }
#endif
    return accessUnit;
}



int64_t ElementaryStreamQueue::fetchTimestamp(size_t size) {
    int64_t timeUs = -1;
    bool first = true;

    while (size > 0) {
        if (mRangeInfos.empty()) {
            return timeUs;
        }

        RangeInfo *info = &*mRangeInfos.begin();

        if (first) {
            timeUs = info->mTimestampUs;
            first = false;
        }

        if (info->mLength > size) {
            info->mLength -= size;
            size = 0;
        } else {
            size -= info->mLength;

            mRangeInfos.erase(mRangeInfos.begin());
            info = NULL;
        }

    }

    if (timeUs == 0ll) {
        ALOGV("Returning 0 timestamp");
    }

    return timeUs;
}

#if 0//Performance mode, but we got wrong slice from HLS case, disable it
sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitH264() {
    size_t size = mBuffer->size();
    const uint8_t *data = mBuffer->data();
    size_t offset = 0;

    if(size == 0)
        return NULL;
    offset = size;
    sp<ABuffer> accessUnit = new ABuffer(offset);
    memcpy(accessUnit->data(), data, offset);
    //hexdump(data, 16);
    memmove(mBuffer->data(),
            mBuffer->data() + offset,
            mBuffer->size() - offset);
    //ALOGD("Dequeue access unit size %zu ", offset);
    mBuffer->setRange(0, mBuffer->size() - offset);
#ifdef RTK_PLATFORM
    if(mFirstFrameTimestamp == -1)
        mFirstFrameTimestamp = mLastTimeUs;
#endif
    accessUnit->meta()->setInt64("timeUs", mLastTimeUs);

    if(mFormat == NULL)
        mFormat = MakeAVCCodecSpecificData(accessUnit);
    return accessUnit;
}
#endif

#ifdef RTK_PLATFORM
sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitH264() {
    if (accessUnits.empty()) {
        const uint8_t *data = mBuffer->data();
        size_t size = mBuffer->size();
        Vector<NALPosition> nals;
        size_t totalSize = 0;
        status_t err;
        const uint8_t *nalStart;
        size_t nalSize;
        bool foundSlice = false;
        bool over = false;
        size_t seiCount = 0;
        while (!over &&(((err =getNextNALUnit(&data, &size, &nalStart, &nalSize)) == OK)
                || ((err != OK) && (nalStart != NULL)
                    && ((nalStart[0] & 0x1f) != 1)
                    && ((nalStart[0] & 0x1f) != 5)))) {
            unsigned nalType = nalStart[0] & 0x1f;
            bool flush = false;
            if (err != OK) {
                over = true;
            }
            if (nalType == 1 || nalType == 5) {
                CHECK_GT(nalSize, 0u);
                if (foundSlice) {
                    ABitReader br(nalStart + 1, nalSize);
                    unsigned first_mb_in_slice = parseUE(&br);
                    if (first_mb_in_slice == 0) {
                        // This slice starts a new frame.
                        flush = true;
                    }
                }

                foundSlice = true;
            } else if ((nalType == 9 || nalType == 7 || nalType == 8)
                       && foundSlice) {
                // Access unit delimiter and SPS will be associated with the
                // next frame.
                flush = true;
            } else if(nalType == 6 && nalSize > 0) {
                //found non-zero sized SEI
                ++seiCount;
            }

            if (flush) {
                // The access unit will contain all nal units up to, but excluding
                // the current one, separated by 0x00 0x00 0x00 0x01 startcodes.
                size_t auSize = 4 * nals.size() + totalSize;
                sp<ABuffer> MultiAccessUnit = new ABuffer(auSize);
                sp<ABuffer> MultiNal = new ABuffer(auSize);
                size_t dstOffset = 0;
                size_t dstOffset1 = 0;
                sp<ABuffer> sei;

                if (seiCount > 0) {
                    sei = new ABuffer(seiCount * sizeof(NALPosition));
                    MultiNal->meta()->setBuffer("sei", sei);
                }
                /*skip redunt data for fetch right timestamp */
                size_t scanoffset = 0;
                for (size_t i = 0; i < nals.size(); ++i) {
                    const NALPosition & tmppos = nals.itemAt(i);
                    uint8_t *tmpnalStart = mBuffer->data() + tmppos.nalOffset;
                    unsigned nalType = tmpnalStart[0] & 0x1f;
                    if ((nalType != 9) && (nalType != 7)
                        && (nalType != 8) && (nalType != 6)) {
                        //eaqul flush nal type
                        if (i != 0) {
                            const NALPosition & tmppos = nals.itemAt(i - 1);
                            scanoffset = tmppos.nalOffset + tmppos.nalSize;
                            fetchTimestamp(tmppos.nalOffset + tmppos.nalSize);
                        }
                        break;
                    }
                }
                const NALPosition & pos0 = nals.itemAt(nals.size() - 1);
                size_t TotalnextScan = pos0.nalOffset + pos0.nalSize;
                size_t TimestampNextScan =
                    pos0.nalOffset + pos0.nalSize - scanoffset;

                int64_t timeUs;
                bool isSeekOver = false;
                size_t seiIndex = 0;
                for (size_t i = 0; i < nals.size(); ++i) {
                    const NALPosition & pos = nals.itemAt(i);
                    uint8_t *nalStart = mBuffer->data() + pos.nalOffset;
                    unsigned nalType = nalStart[0] & 0x1f;
                    if (nalType == 6 && pos.nalSize > 0) {
                        if (seiIndex >= sei->size() / sizeof(NALPosition)) {
                            ALOGE("Wrong seiIndex");
                            return NULL;
                        }
                        NALPosition &seiPos = ((NALPosition *)sei->data())[seiIndex++];
                        seiPos.nalOffset = dstOffset + 4;
                        seiPos.nalSize = pos.nalSize;
                    }
                    //unsigned nalType = mBuffer->data()[pos.nalOffset] & 0x1f;
                    //ALOGI("nalType 0x%x ", nalType);
                    if (!mSeeking) {
                        mSeeking = false;   // found I frame, seek complete
                        memcpy(MultiNal->data() + dstOffset1,
                               "\x00\x00\x00\x01", 4);
                        memcpy(MultiNal->data() + dstOffset1 + 4,
                               mBuffer->data() + pos.nalOffset, pos.nalSize);
                        dstOffset1 += pos.nalSize + 4;

                    }else if(mSeeking
                            && IsIFrame(mBuffer->data() + pos.nalOffset,
                                        pos.nalSize)){
                        isSeekOver = true;
                        mSeeking = false;   // found I frame, seek complete
                        memcpy(MultiNal->data() + dstOffset1,
                               "\x00\x00\x00\x01", 4);
                        memcpy(MultiNal->data() + dstOffset1 + 4,
                               mBuffer->data() + pos.nalOffset, pos.nalSize);
                        dstOffset1 += pos.nalSize + 4;
                    }
                    memcpy(MultiAccessUnit->data() + dstOffset,
                           "\x00\x00\x00\x01", 4);
                    memcpy(MultiAccessUnit->data() + dstOffset + 4,
                           mBuffer->data() + pos.nalOffset, pos.nalSize);
                    dstOffset += pos.nalSize + 4;
                    }
                memmove(mBuffer->data(),
                        mBuffer->data() + TotalnextScan,
                        mBuffer->size() - TotalnextScan);

                mBuffer->setRange(0, mBuffer->size() - TotalnextScan);

                if (mFormat == NULL) {
                    mFormat = MakeAVCCodecSpecificData(MultiAccessUnit);
                }
                timeUs = fetchTimestamp(TimestampNextScan);
                if(isSeekOver){
                    isSeekOver = false;
                    mSeekOverTimestamp = timeUs;
                    mVideoSeekOver = true;
                }
                if(mFirstFrameTimestamp == -1 && mSeeking == false)
                    mFirstFrameTimestamp = timeUs;
                if (dstOffset1 != 0) {  //mUseFrameBase
                    //ALOGI("flush timeUs = %" PRId64 "\n", timeUs);
                    MultiNal->setRange(0, dstOffset1);
                    MultiNal->meta()->setInt64("timeUs", timeUs);
                    accessUnits.push_back(MultiNal);
                }
                over = true;
            }
            if (nalSize != 0) {
                NALPosition pos;
                pos.nalOffset = nalStart - mBuffer->data();
                pos.nalSize = nalSize;
                nals.push(pos);
                totalSize += nalSize;
            }
        }
    }

    if (!accessUnits.empty()) {
        sp<ABuffer> accessUnit = *accessUnits.begin();
        accessUnits.erase(accessUnits.begin());
        return accessUnit;
    } else {
        return NULL;
    }
}
#else
sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitH264() {
    const uint8_t *data = mBuffer->data();

    size_t size = mBuffer->size();
    Vector<NALPosition> nals;

    size_t totalSize = 0;
    size_t seiCount = 0;

    status_t err;
    const uint8_t *nalStart;
    size_t nalSize;
    bool foundSlice = false;
    bool foundIDR = false;
    while ((err = getNextNALUnit(&data, &size, &nalStart, &nalSize)) == OK) {
        if (nalSize == 0) continue;

        unsigned nalType = nalStart[0] & 0x1f;
        bool flush = false;

        if (nalType == 1 || nalType == 5) {
            if (nalType == 5) {
                foundIDR = true;
            }
            if (foundSlice) {
                ABitReader br(nalStart + 1, nalSize);
                unsigned first_mb_in_slice = parseUE(&br);

                if (first_mb_in_slice == 0) {
                    // This slice starts a new frame.

                    flush = true;
                }
            }

            foundSlice = true;
        } else if ((nalType == 9 || nalType == 7) && foundSlice) {
            // Access unit delimiter and SPS will be associated with the
            // next frame.

            flush = true;
        } else if (nalType == 6 && nalSize > 0) {
            // found non-zero sized SEI
            ++seiCount;
        }

        if (flush) {
            // The access unit will contain all nal units up to, but excluding
            // the current one, separated by 0x00 0x00 0x00 0x01 startcodes.

            size_t auSize = 4 * nals.size() + totalSize;
            sp<ABuffer> accessUnit = new ABuffer(auSize);
            sp<ABuffer> sei;

            if (seiCount > 0) {
                sei = new ABuffer(seiCount * sizeof(NALPosition));
                accessUnit->meta()->setBuffer("sei", sei);
            }

#if !LOG_NDEBUG
            AString out;
#endif

            size_t dstOffset = 0;
            size_t seiIndex = 0;
            for (size_t i = 0; i < nals.size(); ++i) {
                const NALPosition &pos = nals.itemAt(i);

                unsigned nalType = mBuffer->data()[pos.nalOffset] & 0x1f;

                if (nalType == 6 && pos.nalSize > 0) {
                    if (seiIndex >= sei->size() / sizeof(NALPosition)) {
                        ALOGE("Wrong seiIndex");
                        return NULL;
                    }
                    NALPosition &seiPos = ((NALPosition *)sei->data())[seiIndex++];
                    seiPos.nalOffset = dstOffset + 4;
                    seiPos.nalSize = pos.nalSize;
                }

#if !LOG_NDEBUG
                char tmp[128];
                sprintf(tmp, "0x%02x", nalType);
                if (i > 0) {
                    out.append(", ");
                }
                out.append(tmp);
#endif

                memcpy(accessUnit->data() + dstOffset, "\x00\x00\x00\x01", 4);

                memcpy(accessUnit->data() + dstOffset + 4,
                       mBuffer->data() + pos.nalOffset,
                       pos.nalSize);

                dstOffset += pos.nalSize + 4;
            }

#if !LOG_NDEBUG
            ALOGV("accessUnit contains nal types %s", out.c_str());
#endif

            const NALPosition &pos = nals.itemAt(nals.size() - 1);
            size_t nextScan = pos.nalOffset + pos.nalSize;

            memmove(mBuffer->data(),
                    mBuffer->data() + nextScan,
                    mBuffer->size() - nextScan);

            mBuffer->setRange(0, mBuffer->size() - nextScan);

            int64_t timeUs = fetchTimestamp(nextScan);
            if (timeUs < 0ll) {
                ALOGE("Negative timeUs");
                return NULL;
            }

#ifdef RTK_PLATFORM
                if(mFirstFrameTimestamp == -1)
                    mFirstFrameTimestamp = timeUs;
#endif
            accessUnit->meta()->setInt64("timeUs", timeUs);
            if (foundIDR) {
                accessUnit->meta()->setInt32("isSync", 1);
            }

            if (mFormat == NULL) {
                mFormat = MakeAVCCodecSpecificData(accessUnit);
            }

            return accessUnit;
        }

        NALPosition pos;
        pos.nalOffset = nalStart - mBuffer->data();
        pos.nalSize = nalSize;

        nals.push(pos);

        totalSize += nalSize;
    }
    if (err != (status_t)-EAGAIN) {
        ALOGE("Unexpeted err");
        return NULL;
    }

    return NULL;
}
#endif

#ifdef RTK_PLATFORM
sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitH265() {
    const uint8_t *data = mBuffer->data();
    size_t size = mBuffer->size();
    Vector<NALPosition> nals;
    size_t totalSize = 0;

    status_t err;
    const uint8_t *nalStart;
    size_t nalSize;
    bool foundSlice = false;
    int preVCLIndex = -1;
    bool foundIDR = false;

    if(!mSeeking && mFormat != NULL && mLastTimeUs > 1000000 && mGOPTimeOffset == 0)// 1 buffer with 1 frame
    {
        if(size == 0)
            return NULL;
        size_t offset = 0;

        RangeInfo *info = &*mRangeInfos.begin();
        offset = info->mLength;
        int64_t timeUs = info->mTimestampUs;
        mRangeInfos.erase(mRangeInfos.begin());
        info = NULL;
        sp<ABuffer> accessUnit = new ABuffer(offset);
        memcpy(accessUnit->data(), data, offset);
        memmove(mBuffer->data(),
                mBuffer->data() + offset,
                mBuffer->size() - offset);
        mBuffer->setRange(0, mBuffer->size() - offset);
        if(mFirstFrameTimestamp == -1)
            mFirstFrameTimestamp = timeUs;
        accessUnit->meta()->setInt64("timeUs", timeUs);
        return accessUnit;
    }

    while ((err = getNextNALUnit(&data, &size, &nalStart, &nalSize)) == OK) {
        CHECK_GT(nalSize, 0u);

        int sliceType = (nalStart[0] & 0x7E) >> 1;

        bool flush = false;

        if (sliceType == 35) {
            if (foundSlice && (preVCLIndex != -1)) {
                flush = true;
            }
            foundSlice = true;
        } else if ((sliceType >= 0 && sliceType <= 3)
                   || (sliceType >= 16 && sliceType <= 21)) {
            unsigned firstSlice = (nalStart[2] & 0x80) >> 7;

            if (firstSlice) {
                if (foundSlice && (preVCLIndex != -1)) {
                    flush = true;
                }
                foundSlice = true;
            }
        }

        if (flush) {
            const NALPosition & pos = nals.itemAt(preVCLIndex - 1);
            size_t nextScan = pos.nalOffset + pos.nalSize;
            sp<ABuffer> accessUnit = new ABuffer(nextScan);
            memcpy(accessUnit->data(), mBuffer->data(), nextScan);
            memmove(mBuffer->data(),
                    mBuffer->data() + nextScan, mBuffer->size() - nextScan);

            mBuffer->setRange(0, mBuffer->size() - nextScan);

            int64_t timeUs = fetchTimestamp(nextScan);
            CHECK_GE(timeUs, 0ll);
            if(mFirstFrameTimestamp == -1)
                mFirstFrameTimestamp = timeUs;
            timeUs = CheckGOPPTS(timeUs);

            accessUnit->meta()->setInt64("timeUs", timeUs);

            if (mFormat == NULL) {
                mFormat = MakeHEVCMetaData(accessUnit);
            }

            if(!mSeeking || foundIDR)
            {
                if(mSeeking) mSeeking = false;
                return accessUnit;
            }
            else
                return NULL;
        }

        NALPosition pos;
        pos.nalOffset = nalStart - mBuffer->data();
        pos.nalSize = nalSize;
        nals.push_back(pos);
        totalSize += nalSize;
        if (sliceType <= 31) {
            preVCLIndex = nals.size();
            if(sliceType >= 19 && sliceType <= 21)
                foundIDR = true;
        }
    }
    CHECK_EQ(err, (status_t) - EAGAIN);

    return NULL;
}
typedef struct {
    uint32_t   biSize;
    uint32_t   biWidth;
    uint32_t   biHeight;
    uint16_t   biPlanes;
    uint16_t   biBitCount;
    uint32_t   biCompression;
    uint32_t   biSizeImage;
    uint32_t   biXPelsPerMeter;
    uint32_t   biYPelsPerMeter;
    uint32_t   biClrUsed;
    uint32_t   biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER, *LPBITMAPINFOHEADER;

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitWVC1() {
    const uint8_t *data = mBuffer->data();
    size_t size = mBuffer->size();

    int prevStartCode = -1;
    int currentStartCode = -1;
    int pprevStartCode = -1;
    bool sawPictureStart = false;

    size_t offset = 0;
    if(mSeeking)
        mOffset = 0;
    offset = mOffset;
    if(mOffset == size)
        return NULL;

    if(mFormat != NULL && mOffset != 0)
        sawPictureStart = true;

    if(mFormat != NULL && mSeeking == false&& mLastTimeUs > 1000000 && mGOPTimeOffset == 0)//VE can recognize start code, we can just feed full ES stream
    {
        if(size == 0)
            return NULL;
        RangeInfo info = *mRangeInfos.begin();
        offset = info.mLength;
        if(offset > size)
            return NULL;
        sp<ABuffer> accessUnit = new ABuffer(offset);
        memcpy(accessUnit->data(), data, offset);
        //hexdump(data, 16);
        memmove(mBuffer->data(),
                mBuffer->data() + offset,
                mBuffer->size() - offset);
        //ALOGD("Dequeue access unit size %zu ", offset);
        mBuffer->setRange(0, mBuffer->size() - offset);
        int64_t timeUs = fetchTimestamp(offset);
        if(mFirstFrameTimestamp == -1)
            mFirstFrameTimestamp = timeUs;
        accessUnit->meta()->setInt64("timeUs", timeUs);
        return accessUnit;
    }

    while (offset + 3 < size) {
        if (memcmp(&data[offset], "\x00\x00\x01", 3)) {
            ++offset;
            continue;
        }
        ALOGV("%02x %02x %02x %02x", data[offset + 0], data[offset + 1], data[offset + 2], data[offset + 3]);
        pprevStartCode = prevStartCode;
        prevStartCode = currentStartCode;
        currentStartCode = data[offset + 3];

        //video sequence start, video_edit_code
        if (currentStartCode == 0x0F && mFormat == NULL) {
            if (offset) {
                memmove(mBuffer->data(), mBuffer->data() + offset,
                        size - offset);
                size -= offset;
                (void) fetchTimestamp(offset);
                mBuffer->setRange(0, size);
                offset = 0;
            }
        }

        //sequence_header
        if ((prevStartCode == 0x0F && currentStartCode != 0x0E)
            || (pprevStartCode == 0x0F && prevStartCode == 0x0E)) {
            if (mFormat == NULL) {
                ABitReader bits(data, offset);

                bits.skipBits(32);  //start code 0000010F
                uint8_t profile = bits.getBits(2);
                ALOGD("profile is:%u", profile);
                if (profile != 3) {
                    return NULL;
                }

                uint8_t level = bits.getBits(3);
                ALOGD("level is:%u", level);

                bits.skipBits(2);   //COLORDIFF_FORMAT
                bits.skipBits(3);   //FRMRTQ_POSTPROC
                bits.skipBits(5);   //BITRTQ_POSTPROC
                bits.skipBits(1);   //POSTPROCFLAG
                uint32_t pictureWidth = (bits.getBits(12) * 2) + 2;
                ALOGD("pictureWidth:%u", pictureWidth);

                uint32_t pictureHeight = (bits.getBits(12) * 2) + 2;
                ALOGD("pictureHeight:%u", pictureHeight);

                sp<ABuffer> WMVCData = new ABuffer(offset + sizeof(BITMAPINFOHEADER));
                BITMAPINFOHEADER header;
                memset(&header, 0, sizeof(BITMAPINFOHEADER));
                header.biSize = offset + sizeof(BITMAPINFOHEADER);
                header.biWidth = pictureWidth;
                header.biHeight = pictureHeight;
                memcpy(&(header.biCompression), "WVC1", 4);
                memcpy(WMVCData->data(), (void *) &header, sizeof(BITMAPINFOHEADER));
                memcpy(WMVCData->data() + sizeof(BITMAPINFOHEADER), data, offset);

                mFormat = new MetaData;
                mFormat->setData(kKeyWVC1, 0, WMVCData->data(), WMVCData->size());
                mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_WVC1);
                mFormat->setInt32(kKeyWidth, pictureWidth);
                mFormat->setInt32(kKeyHeight, pictureHeight);
                memmove(mBuffer->data(), mBuffer->data() + offset, size - offset);
                size -= offset;
                (void) fetchTimestamp(offset);
                mBuffer->setRange(0, mBuffer->size() - offset);
                offset = 0;
                return NULL;
            }

        }
        if (currentStartCode == 0x0E && mFormat != NULL && mSeeking) {
            mSeeking = false;
            memmove(mBuffer->data(), mBuffer->data() + offset, size - offset);
            size -= offset;
            (void) fetchTimestamp(offset);
            offset = 0;
            mBuffer->setRange(0, size);
            ALOGI("Found entry header when seeking");
        }
        if (currentStartCode == 0x0D && mFormat != NULL && !mSeeking) {
            if (!sawPictureStart) {
                sawPictureStart = true;
                mOffset = offset;
            } else {
                sp<ABuffer> accessUnit = new ABuffer(offset);
                memcpy(accessUnit->data(), data, offset);
                memmove(mBuffer->data(), mBuffer->data() + offset, mBuffer->size() - offset);
                mBuffer->setRange(0, mBuffer->size() - offset);
                int64_t timeUs = fetchTimestamp(offset);
                if(mFirstFrameTimestamp == -1)
                    mFirstFrameTimestamp = timeUs;
                offset = 0;
                mOffset = 0;
                timeUs = CheckGOPPTS(timeUs);
                accessUnit->meta()->setInt64("timeUs", timeUs);
                return accessUnit;
            }
        }
        ++offset;
    }

    if (mSeeking) {
        (void) fetchTimestamp(offset);
        memmove(mBuffer->data(), mBuffer->data() + offset, mBuffer->size() - offset);
        size -= offset;
        mBuffer->setRange(0, mBuffer->size() - offset);
        offset = 0;
    }
    else
        mOffset = offset;
    return NULL;
}

#endif

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitMPEGAudio() {
    const uint8_t *data = mBuffer->data();
    size_t size = mBuffer->size();

    if (size < 4) {
        return NULL;
    }

#ifdef RTK_PLATFORM
    size_t offset = 0;
    while(mBuffer->data()[0] != 0xFF)//Find sync byte
    {
        offset++;
        if(offset > mBuffer->size() - 4)
        {
            mBuffer->setRange(0, 0);
            return NULL;
        }
    }
    if(offset != 0)
    {
        mBuffer->setRange(mBuffer->offset()+offset, mBuffer->size()-offset);
        data = mBuffer->data();
        size = mBuffer->size();
    }
#endif
    uint32_t header = U32_AT(data);

    size_t frameSize;
    int samplingRate, numChannels, bitrate, numSamples;
    if (!GetMPEGAudioFrameSize(
                header, &frameSize, &samplingRate, &numChannels,
                &bitrate, &numSamples)) {
        ALOGE("Failed to get audio frame size");
        return NULL;
    }

    if (size < frameSize) {
        return NULL;
    }

    unsigned layer = 4 - ((header >> 17) & 3);

    sp<ABuffer> accessUnit = new ABuffer(frameSize);
    memcpy(accessUnit->data(), data, frameSize);

    memmove(mBuffer->data(),
            mBuffer->data() + frameSize,
            mBuffer->size() - frameSize);

    mBuffer->setRange(0, mBuffer->size() - frameSize);

    int64_t timeUs = fetchTimestamp(frameSize);
    if (timeUs < 0ll) {
        ALOGE("Negative timeUs");
        return NULL;
    }

    accessUnit->meta()->setInt64("timeUs", timeUs);
    accessUnit->meta()->setInt32("isSync", 1);

    if (mFormat == NULL) {
        mFormat = new MetaData;

        switch (layer) {
            case 1:
                mFormat->setCString(
                        kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_I);
                break;
            case 2:
                mFormat->setCString(
                        kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_II);
                break;
            case 3:
                mFormat->setCString(
                        kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MPEG);
                break;
            default:
                return NULL;
        }

        mFormat->setInt32(kKeySampleRate, samplingRate);
        mFormat->setInt32(kKeyChannelCount, numChannels);
    }
#ifdef RTK_PLATFORM
    if(timeUs < mSkipFrameBeforeTimestamp)
    {
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameBeforeTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameBeforeTimestamp);
        return NULL;
    }
#endif
    return accessUnit;
}

static void EncodeSize14(uint8_t **_ptr, size_t size) {
    if (size > 0x3fff) {
        ALOGE("Wrong size");
        return;
    }

    uint8_t *ptr = *_ptr;

    *ptr++ = 0x80 | (size >> 7);
    *ptr++ = size & 0x7f;

    *_ptr = ptr;
}

static sp<ABuffer> MakeMPEGVideoESDS(const sp<ABuffer> &csd) {
    sp<ABuffer> esds = new ABuffer(csd->size() + 25);

    uint8_t *ptr = esds->data();
    *ptr++ = 0x03;
    EncodeSize14(&ptr, 22 + csd->size());

    *ptr++ = 0x00;  // ES_ID
    *ptr++ = 0x00;

    *ptr++ = 0x00;  // streamDependenceFlag, URL_Flag, OCRstreamFlag

    *ptr++ = 0x04;
    EncodeSize14(&ptr, 16 + csd->size());

    *ptr++ = 0x40;  // Audio ISO/IEC 14496-3

    for (size_t i = 0; i < 12; ++i) {
        *ptr++ = 0x00;
    }

    *ptr++ = 0x05;
    EncodeSize14(&ptr, csd->size());

    memcpy(ptr, csd->data(), csd->size());

    return esds;
}

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitMPEGVideo() {
    const uint8_t *data = mBuffer->data();
    size_t size = mBuffer->size();

    Vector<size_t> userDataPositions;

    bool sawPictureStart = false;
    int pprevStartCode = -1;
    int prevStartCode = -1;
    int currentStartCode = -1;
    bool gopFound = false;
    bool isClosedGop = false;
    bool brokenLink = false;

#ifdef RTK_PLATFORM
    size_t offset = mOffset;
    bool isSync = false;

    if(offset != 0)
        sawPictureStart = true;
    while (offset + 3 < size) {
        if (data[offset] != 0x0 || data[offset+1] != 0x0 || data[offset+2] != 0x1) {
            ++offset;
            continue;
        }
#else
    size_t offset = 0;
    while (offset + 3 < size) {
        if (memcmp(&data[offset], "\x00\x00\x01", 3)) {
            ++offset;
            continue;
        }
#endif
        pprevStartCode = prevStartCode;
        prevStartCode = currentStartCode;
        currentStartCode = data[offset + 3];

        if (currentStartCode == 0xb3 && mFormat == NULL) {
            memmove(mBuffer->data(), mBuffer->data() + offset, size - offset);
            size -= offset;
            (void)fetchTimestamp(offset);
            offset = 0;
            mBuffer->setRange(0, size);
        }

        if ((prevStartCode == 0xb3 && currentStartCode != 0xb5)
                || (pprevStartCode == 0xb3 && prevStartCode == 0xb5)) {
            // seqHeader without/with extension

            if (mFormat == NULL) {
                if (size < 7u) {
                    ALOGE("Size too small");
                    return NULL;
                }

                unsigned width =
                    (data[4] << 4) | data[5] >> 4;

                unsigned height =
                    ((data[5] & 0x0f) << 8) | data[6];

                mFormat = new MetaData;
                mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_MPEG2);
                mFormat->setInt32(kKeyWidth, width);
                mFormat->setInt32(kKeyHeight, height);

                ALOGI("found MPEG2 video codec config (%d x %d)", width, height);

                sp<ABuffer> csd = new ABuffer(offset);
                memcpy(csd->data(), data, offset);

                memmove(mBuffer->data(),
                        mBuffer->data() + offset,
                        mBuffer->size() - offset);

                mBuffer->setRange(0, mBuffer->size() - offset);
                size -= offset;
                (void)fetchTimestamp(offset);
                offset = 0;

                // hexdump(csd->data(), csd->size());

                sp<ABuffer> esds = MakeMPEGVideoESDS(csd);
                mFormat->setData(
                        kKeyESDS, kTypeESDS, esds->data(), esds->size());

                return NULL;
            }
        }

        if (mFormat != NULL && currentStartCode == 0xb8) {
            // GOP layer
            if (offset + 7 >= size) {
                ALOGE("Size too small");
                return NULL;
            }
            gopFound = true;
            isClosedGop = (data[offset + 7] & 0x40) != 0;
            brokenLink = (data[offset + 7] & 0x20) != 0;
        }

        if (mFormat != NULL && currentStartCode == 0xb2) {
            userDataPositions.add(offset);
        }

        if (mFormat != NULL && currentStartCode == 0x00) {
            // Picture start

            if (!sawPictureStart) {
                sawPictureStart = true;
#ifdef RTK_PLATFORM
                mOffset = offset;
#endif
            } else {
#ifdef RTK_PLATFORM
                isSync = ((data[5] >> 3) & 0x7) == 0x1;
#endif
                sp<ABuffer> accessUnit = new ABuffer(offset);
                memcpy(accessUnit->data(), data, offset);

                memmove(mBuffer->data(),
                        mBuffer->data() + offset,
                        mBuffer->size() - offset);

                mBuffer->setRange(0, mBuffer->size() - offset);

                int64_t timeUs = fetchTimestamp(offset);

                if (timeUs < 0ll) {
                    ALOGE("Negative timeUs");
                    return NULL;
                }

#ifdef RTK_PLATFORM
                if(mFirstFrameTimestamp == -1)
                    mFirstFrameTimestamp = timeUs;
#endif

                offset = 0;
#ifdef RTK_PLATFORM
                mOffset = 0;
                if(mMPEGVideoLastPTS != timeUs)
                    mMPEGVideoLastPTS = timeUs;
                else if(mMPEGVideoLastPTS != 0 && mMPEGVideoLastPTS == timeUs && mEOSReached == false)
                    timeUs = 0;
#endif
                //timeUs = CheckGOPPTS(timeUs);
                accessUnit->meta()->setInt64("timeUs", timeUs);
                if (gopFound && (!brokenLink || isClosedGop)) {
                    accessUnit->meta()->setInt32("isSync", 1);
                }

                ALOGV("returning MPEG video access unit at time %" PRId64 " us", timeUs);

                // hexdump(accessUnit->data(), accessUnit->size());

                if (userDataPositions.size() > 0) {
                    sp<ABuffer> mpegUserData =
                        new ABuffer(userDataPositions.size() * sizeof(size_t));
                    if (mpegUserData != NULL && mpegUserData->data() != NULL) {
                        for (size_t i = 0; i < userDataPositions.size(); ++i) {
                            memcpy(
                                    mpegUserData->data() + i * sizeof(size_t),
                                    &userDataPositions[i], sizeof(size_t));
                        }
                        accessUnit->meta()->setBuffer("mpegUserData", mpegUserData);
                    }
                }
#ifdef RTK_PLATFORM
                if(!mSeeking || isSync)
                {
                    if(mSeeking)
                    {
                        if(timeUs != 0)//Find useful time
                        {
                            mMPEGVideoSeekTimeStamp = timeUs;
                            mSeeking = false;
                            return accessUnit;
                        }
                        else
                            return NULL;
                    }

                    if(mMPEGVideoSeekTimeStamp != -1)//Drop the frame which PTS is earlier than I frame
                    {
                        if(timeUs < mMPEGVideoSeekTimeStamp)
                            return NULL;
                        else
                        {
                            mMPEGVideoSeekTimeStamp = -1;
                            return accessUnit;
                        }
                    }
                    else
                        return accessUnit;
                }
                else
                    return NULL;
#else
                return accessUnit;
#endif
            }
        }

        ++offset;
    }

#ifdef RTK_PLATFORM
    if(sawPictureStart == true)
        mOffset = offset;
#endif
    return NULL;
}

static ssize_t getNextChunkSize(
        const uint8_t *data, size_t size) {
    static const char kStartCode[] = "\x00\x00\x01";

    if (size < 3) {
        return -EAGAIN;
    }

    if (memcmp(kStartCode, data, 3)) {
        return -EAGAIN;
    }

    size_t offset = 3;
    while (offset + 2 < size) {
        if (!memcmp(&data[offset], kStartCode, 3)) {
            return offset;
        }

        ++offset;
    }

    return -EAGAIN;
}

#ifdef RTK_PLATFORM
sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitAACLATM() {

    const RangeInfo &info = *mRangeInfos.begin();
    if (mBuffer->size() == 0 || mBuffer->size() < info.mLength) {
        return NULL;
    }

    if (info.mTimestampUs < 0ll) {
        ALOGE("Negative info.mTimestampUs");
        return NULL;
    }

    size_t offset = info.mLength;
    if(mFormat == NULL)
    {
        sp<MetaData> meta = new MetaData;
        meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AAC_LATM);
        meta->setInt32(kKeySampleRate, 48000);
        meta->setInt32(kKeyChannelCount, 2);
        mFormat = meta;
    }

    int64_t timeUs = fetchTimestamp(offset);
    sp<ABuffer> accessUnit = new ABuffer(offset);
    memcpy(accessUnit->data(), mBuffer->data(), offset);
    memmove(mBuffer->data(), mBuffer->data() + offset, mBuffer->size() - offset);
    mBuffer->setRange(0, mBuffer->size() - offset);
    accessUnit->meta()->setInt64("timeUs", timeUs);
    accessUnit->meta()->setInt32("isSync", 1);
    if(timeUs < mSkipFrameBeforeTimestamp)
    {
        ALOGV("Audio timeUs %" PRId64 " mSkipFrameBeforeTimestamp %" PRId64 " drop it ", timeUs, mSkipFrameBeforeTimestamp);
        return NULL;
    }
    return accessUnit;
}


static ssize_t getNextChunkSize_avs(
    const uint8_t *data, size_t size) {
    static const char kStartCode_start[] = "\x00\x00\x01\xb0";
    //static const char kStartCode_I[] = "\x00\x00\x01\xb3";
    static const char kStartCode_PB[] = "\x00\x00\x01\xb6";

    if (size < 4) { 
        return -EAGAIN;
    }
    size_t offset = 4;
    while (offset + 3< size) {
        if(memcmp(&data[offset], kStartCode_PB, 4)==0||memcmp(&data[offset], kStartCode_start, 4)==0){
            return offset;          
        }
        ++offset;
    }
    return -EAGAIN;
} 


sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitAVS(){
    uint8_t *data = mBuffer->data();
    size_t size = mBuffer->size();
    size_t offset = 0;
    ssize_t chunkSize=0;  
    while ((chunkSize = getNextChunkSize_avs(
                    &data[offset], size - offset)) >0) {

        unsigned chunkType = (data[offset + 0] << 24) | (data[offset + 1]<<16) | (data[offset + 2]<<8) | (data[offset + 3]);
        uint32_t width = 0;
        uint32_t height = 0; 
        bool discard = false;
        switch (chunkType){
            case 0x000001b0:    
                if(mFormat==NULL){
                    ABitReader bits(mBuffer->data(), size);
                    bits.skipBits(32);   //video_sequence_start_code
                    bits.skipBits(8);   //profile_id
                    bits.skipBits(8);   //level_id
                    bits.skipBits(1);   //progressive_sequence
                    width =bits.getBits(14);
                    height =bits.getBits(14);   
                    mFormat = new MetaData;
                    mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVS); //type
                    mFormat->setInt32(kKeyWidth, width);    //height
                    mFormat->setInt32(kKeyHeight, height); //width                      
                }
                discard = false;
                break;
            case 0x000001b6:
                discard = false;
                break;
            default:            
                discard = true;
                break;          
        } 
        if(discard){
            offset +=chunkSize;         
        }else{
            offset +=chunkSize;
            sp<ABuffer> accessUnit = new ABuffer(offset);
            memcpy(accessUnit->data(), data, offset);
            if(mFirstFrameTimestamp == -1)
                mFirstFrameTimestamp = mLastTimeUs;
            accessUnit->meta()->setInt64("timeUs", mLastTimeUs);
            memmove(data, &data[offset], size - offset);
            size -= offset;
            mBuffer->setRange(0, size); 
            return accessUnit;          
        }

    }
    return NULL;
}
#endif


sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitMPEG4Video() {
    uint8_t *data = mBuffer->data();
    size_t size = mBuffer->size();

    enum {
        SKIP_TO_VISUAL_OBJECT_SEQ_START,
        EXPECT_VISUAL_OBJECT_START,
        EXPECT_VO_START,
        EXPECT_VOL_START,
        WAIT_FOR_VOP_START,
        SKIP_TO_VOP_START,

    } state;

    if (mFormat == NULL) {
        state = SKIP_TO_VISUAL_OBJECT_SEQ_START;
    } else {
        state = SKIP_TO_VOP_START;
    }

    int32_t width = -1, height = -1;

    size_t offset = 0;
    ssize_t chunkSize;
    while ((chunkSize = getNextChunkSize(
                    &data[offset], size - offset)) > 0) {
        bool discard = false;

        unsigned chunkType = data[offset + 3];

        switch (state) {
            case SKIP_TO_VISUAL_OBJECT_SEQ_START:
            {
                if (chunkType == 0xb0) {
                    // Discard anything before this marker.

                    state = EXPECT_VISUAL_OBJECT_START;
                } else {
                    discard = true;
                }
                break;
            }

            case EXPECT_VISUAL_OBJECT_START:
            {
                if (chunkType != 0xb5) {
                    ALOGE("Unexpected chunkType");
                    return NULL;
                }
                state = EXPECT_VO_START;
                break;
            }

            case EXPECT_VO_START:
            {
                if (chunkType > 0x1f) {
                    ALOGE("Unexpected chunkType");
                    return NULL;
                }
                state = EXPECT_VOL_START;
                break;
            }

            case EXPECT_VOL_START:
            {
                if ((chunkType & 0xf0) != 0x20) {
                    ALOGE("Wrong chunkType");
                    return NULL;
                }

                if (!ExtractDimensionsFromVOLHeader(
                            &data[offset], chunkSize,
                            &width, &height)) {
                    ALOGE("Failed to get dimension");
                    return NULL;
                }

                state = WAIT_FOR_VOP_START;
                break;
            }

            case WAIT_FOR_VOP_START:
            {
                if (chunkType == 0xb3 || chunkType == 0xb6) {
                    // group of VOP or VOP start.

                    mFormat = new MetaData;
                    mFormat->setCString(
                            kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_MPEG4);

                    mFormat->setInt32(kKeyWidth, width);
                    mFormat->setInt32(kKeyHeight, height);

                    ALOGI("found MPEG4 video codec config (%d x %d)",
                         width, height);

                    sp<ABuffer> csd = new ABuffer(offset);
                    memcpy(csd->data(), data, offset);

                    // hexdump(csd->data(), csd->size());

                    sp<ABuffer> esds = MakeMPEGVideoESDS(csd);
                    mFormat->setData(
                            kKeyESDS, kTypeESDS,
                            esds->data(), esds->size());

                    discard = true;
                    state = SKIP_TO_VOP_START;
                }

                break;
            }

            case SKIP_TO_VOP_START:
            {
                if (chunkType == 0xb6) {
                    int vopCodingType = (data[offset + 4] & 0xc0) >> 6;

                    offset += chunkSize;

                    sp<ABuffer> accessUnit = new ABuffer(offset);
                    memcpy(accessUnit->data(), data, offset);

                    memmove(data, &data[offset], size - offset);
                    size -= offset;
                    mBuffer->setRange(0, size);

                    int64_t timeUs = fetchTimestamp(offset);
                    if (timeUs < 0ll) {
                        ALOGE("Negative timeus");
                        return NULL;
                    }

                    offset = 0;
#ifdef RTK_PLATFORM
                    if(mFirstFrameTimestamp == -1)
                        mFirstFrameTimestamp = timeUs;
#endif
                    accessUnit->meta()->setInt64("timeUs", timeUs);
                    if (vopCodingType == 0) {  // intra-coded VOP
                        accessUnit->meta()->setInt32("isSync", 1);
                    }

                    ALOGV("returning MPEG4 video access unit at time %" PRId64 " us",
                         timeUs);

                    // hexdump(accessUnit->data(), accessUnit->size());

                    return accessUnit;
                } else if (chunkType != 0xb3) {
                    offset += chunkSize;
                    discard = true;
                }

                break;
            }

            default:
                ALOGE("Unknown state: %d", state);
                return NULL;
        }

        if (discard) {
#ifdef RTK_PLATFORM//testStagefright_bug_26070014
            if(state == SKIP_TO_VISUAL_OBJECT_SEQ_START && offset == 0)
                offset = chunkSize;
#endif
            (void)fetchTimestamp(offset);
            memmove(data, &data[offset], size - offset);
            size -= offset;
            offset = 0;
            mBuffer->setRange(0, size);
        } else {
            offset += chunkSize;
        }
    }

    return NULL;
}

void ElementaryStreamQueue::signalEOS() {
    if (!mEOSReached) {
        if (mMode == MPEG_VIDEO) {
            const char *theEnd = "\x00\x00\x01\x00";
            appendData(theEnd, 4, 0);
        }
        mEOSReached = true;
    } else {
        ALOGW("EOS already signaled");
    }
}

sp<ABuffer> ElementaryStreamQueue::dequeueAccessUnitMetadata() {
    size_t size = mBuffer->size();
    if (!size) {
        return NULL;
    }

    sp<ABuffer> accessUnit = new ABuffer(size);
    int64_t timeUs = fetchTimestamp(size);
    accessUnit->meta()->setInt64("timeUs", timeUs);

    memcpy(accessUnit->data(), mBuffer->data(), size);
    mBuffer->setRange(0, 0);

    if (mFormat == NULL) {
        mFormat = new MetaData;
        mFormat->setCString(kKeyMIMEType, MEDIA_MIMETYPE_DATA_TIMED_ID3);
    }

    return accessUnit;
}

}  // namespace android

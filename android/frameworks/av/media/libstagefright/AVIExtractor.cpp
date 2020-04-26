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
#define LOG_TAG "AVIExtractor"
#include <utils/Log.h>

#include "include/avc_utils.h"
#include "include/AVIExtractor.h"

#include <binder/ProcessState.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/Utils.h>

#ifdef RTK_PLATFORM
//#define DUMP_AVI_AUDIO_DATA 1
//#define DUMP_AVI_VIDEO_DATA 1
#endif

namespace android {

struct AVIExtractor::AVISource : public MediaSource {
    AVISource(const sp<AVIExtractor> &extractor, size_t trackIndex);

    virtual status_t start(MetaData *params);
    virtual status_t stop();

    virtual sp<MetaData> getFormat();

    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options);

protected:
    virtual ~AVISource();

private:
    sp<AVIExtractor> mExtractor;
    size_t mTrackIndex;
    const AVIExtractor::Track &mTrack;
    MediaBufferGroup *mBufferGroup;
    size_t mSampleIndex;

    sp<MP3Splitter> mSplitter;

    DISALLOW_EVIL_CONSTRUCTORS(AVISource);
};

////////////////////////////////////////////////////////////////////////////////

struct AVIExtractor::MP3Splitter : public RefBase {
    MP3Splitter();

    void clear();
    void append(MediaBuffer *buffer);
    status_t read(MediaBuffer **buffer);

protected:
    virtual ~MP3Splitter();

private:
    bool mFindSync;
    int64_t mBaseTimeUs;
    int64_t mNumSamplesRead;
    sp<ABuffer> mBuffer;

    bool resync();

    DISALLOW_EVIL_CONSTRUCTORS(MP3Splitter);
};

////////////////////////////////////////////////////////////////////////////////

AVIExtractor::AVISource::AVISource(
        const sp<AVIExtractor> &extractor, size_t trackIndex)
    : mExtractor(extractor),
      mTrackIndex(trackIndex),
      mTrack(mExtractor->mTracks.itemAt(trackIndex)),
      mBufferGroup(NULL) {
}

AVIExtractor::AVISource::~AVISource() {
    if (mBufferGroup) {
        stop();
    }
}

status_t AVIExtractor::AVISource::start(MetaData *params) {
    CHECK(!mBufferGroup);
#ifdef RTK_PLATFORM
    if (!params)
        ALOGD("\033[0;33m [%d]  params is NULL \033[m", __LINE__);
#endif

    mBufferGroup = new MediaBufferGroup;

    mBufferGroup->add_buffer(new MediaBuffer(mTrack.mMaxSampleSize));
    mSampleIndex = 0;

    const char *mime;
    CHECK(mTrack.mMeta->findCString(kKeyMIMEType, &mime));

    if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_MPEG)) {
        mSplitter = new MP3Splitter;
    } else {
        mSplitter.clear();
    }

    return OK;
}

status_t AVIExtractor::AVISource::stop() {
    CHECK(mBufferGroup);

    delete mBufferGroup;
    mBufferGroup = NULL;

    mSplitter.clear();

    return OK;
}

sp<MetaData> AVIExtractor::AVISource::getFormat() {
    return mTrack.mMeta;
}

status_t AVIExtractor::AVISource::read(
        MediaBuffer **buffer, const ReadOptions *options) {
    CHECK(mBufferGroup);

    *buffer = NULL;

    int64_t seekTimeUs;
    ReadOptions::SeekMode seekMode;
    if (options && options->getSeekTo(&seekTimeUs, &seekMode)) {
#ifdef RTK_PLATFORM
        if ((seekTimeUs/1000000) >= (mExtractor->FiledurationUs/1000000))
        {
            ALOGD("\033[0;32m [%d]  return  ERROR_END_OF_STREAM     seekTimeUs= %lld,   FiledurationUs= %lld\033[m", __LINE__, (long long)seekTimeUs, (long long)mExtractor->FiledurationUs);
            return ERROR_END_OF_STREAM;
        }
        else
            ALOGD("\033[0;32m [%d]     seekTimeUs= %lld   mTrackIndex= %d\033[m", __LINE__, (long long)seekTimeUs, (int)mTrackIndex);
#endif

        status_t err =
            mExtractor->getSampleIndexAtTime(
                    mTrackIndex, seekTimeUs, seekMode, &mSampleIndex);

        if (err != OK) {
#ifdef RTK_PLATFORM
            //ALOGD("\033[0;31m [%d]  return  err (0x%X)   ERROR_END_OF_STREAM= 0x%X\033[m", __LINE__, err, ERROR_END_OF_STREAM);
#endif
            return ERROR_END_OF_STREAM;
        }

        if (mSplitter != NULL) {
            mSplitter->clear();
        }
    }

    for (;;) {
        if (mSplitter != NULL) {
            status_t err = mSplitter->read(buffer);

            if (err == OK) {
                break;
            } else if (err != -EAGAIN) {
                return err;
            }
        }

        off64_t offset;
        size_t size;
        bool isKey;
        int64_t timeUs;
        status_t err = mExtractor->getSampleInfo(
                mTrackIndex, mSampleIndex, &offset, &size, &isKey, &timeUs);

        ++mSampleIndex;

        if (err != OK) {
#ifdef RTK_PLATFORM
            //ALOGD("\033[0;31m [%d]  return  err (0x%X)   ERROR_END_OF_STREAM= 0x%X\033[m", __LINE__, err, ERROR_END_OF_STREAM);
#endif
            return ERROR_END_OF_STREAM;
        }

        MediaBuffer *out;
        CHECK_EQ(mBufferGroup->acquire_buffer(&out), (status_t)OK);

#ifdef RTK_PLATFORM
        ssize_t n = 0;
        if (mTrack.mVC1Type == 1)
        {
            ssize_t m = 0;
            if (size < 3)
                m = mExtractor->mDataSource->readAt(offset, out->data(), size);
            else
                m = mExtractor->mDataSource->readAt(offset, out->data(), 3);
            uint8_t *tmp_buf = (uint8_t *)out->data();

            if (tmp_buf[0] != 0 || tmp_buf[1] != 0 || tmp_buf[2] != 1 || size <= 3) // check start code as prefix (0x00, 0x00, 0x01)
            {
                tmp_buf[4] = tmp_buf[0];
                if (size >= 2)
                    tmp_buf[5] = tmp_buf[1];
                if (size >= 3)
                    tmp_buf[6] = tmp_buf[2];

                tmp_buf[0] = 0x00;
                tmp_buf[1] = 0x00;
                tmp_buf[2] = 0x01;
                tmp_buf[3] = 0x0D;
                if (size > 3)
                    n = mExtractor->mDataSource->readAt(offset + m, ((uint8_t *)out->data()) + 7, size - m);
                n += (m + 4);
            }
            else
            {
                n = mExtractor->mDataSource->readAt(offset + m, ((uint8_t *)out->data()) + m, size - m);
                n += m;
            }
        }
        else
            n = mExtractor->mDataSource->readAt(offset, out->data(), size);
#else
        ssize_t n = mExtractor->mDataSource->readAt(offset, out->data(), size);
#endif

#ifdef RTK_PLATFORM
        //ALOGD("\033[0;33m [%d]  mTrackIndex= %d   offset= %lld   size= %d \033[m", __LINE__, (int)mTrackIndex, (long long)offset, (int)size);
#ifdef DUMP_AVI_AUDIO_DATA
        if (mTrackIndex == 1) // dump first audio track data
        {
            FILE *fp1 = NULL;
            if((fp1 = fopen("/data/AVIExt_dump_audio", "ab")) != NULL)
            {
                fwrite(out->data(), size, 1, fp1);
                fclose(fp1);
            }
        }
#endif
#ifdef DUMP_AVI_VIDEO_DATA
        if (mTrackIndex == 0) // dump first video track data
        {
            FILE *fp1 = NULL;
            if((fp1 = fopen("/data/AVIExt_dump_video", "ab")) != NULL)
            {
                fwrite(out->data(), size, 1, fp1);
                fclose(fp1);
            }
        }
#endif
#endif

        if (n < (ssize_t)size) {
            return n < 0 ? (status_t)n : (status_t)ERROR_MALFORMED;
        }

#ifdef RTK_PLATFORM
        if (mTrack.mVC1Type == 1)
            out->set_range(0, n);
        else
            out->set_range(0, size);
#else
        out->set_range(0, size);
#endif

        out->meta_data()->setInt64(kKeyTime, timeUs);

        if (isKey) {
            out->meta_data()->setInt32(kKeyIsSyncFrame, 1);
        }

        if (mSplitter == NULL) {
            *buffer = out;
            break;
        }

        mSplitter->append(out);
        out->release();
        out = NULL;
    }

    return OK;
}

////////////////////////////////////////////////////////////////////////////////

AVIExtractor::MP3Splitter::MP3Splitter()
    : mFindSync(true),
      mBaseTimeUs(-1ll),
      mNumSamplesRead(0) {
}

AVIExtractor::MP3Splitter::~MP3Splitter() {
}

void AVIExtractor::MP3Splitter::clear() {
    mFindSync = true;
    mBaseTimeUs = -1ll;
    mNumSamplesRead = 0;

    if (mBuffer != NULL) {
        mBuffer->setRange(0, 0);
    }
}

void AVIExtractor::MP3Splitter::append(MediaBuffer *buffer) {
    size_t prevCapacity = (mBuffer != NULL) ? mBuffer->capacity() : 0;

    if (mBaseTimeUs < 0) {
        CHECK(mBuffer == NULL || mBuffer->size() == 0);
        CHECK(buffer->meta_data()->findInt64(kKeyTime, &mBaseTimeUs));
        mNumSamplesRead = 0;
    }

    if (mBuffer != NULL && mBuffer->offset() > 0) {
        memmove(mBuffer->base(), mBuffer->data(), mBuffer->size());
        mBuffer->setRange(0, mBuffer->size());
    }

    if (mBuffer == NULL
            || mBuffer->size() + buffer->range_length() > prevCapacity) {
        size_t newCapacity =
            (prevCapacity + buffer->range_length() + 1023) & ~1023;

        sp<ABuffer> newBuffer = new ABuffer(newCapacity);
        if (mBuffer != NULL) {
            memcpy(newBuffer->data(), mBuffer->data(), mBuffer->size());
            newBuffer->setRange(0, mBuffer->size());
        } else {
            newBuffer->setRange(0, 0);
        }
        mBuffer = newBuffer;
    }

    memcpy(mBuffer->data() + mBuffer->size(),
           (const uint8_t *)buffer->data() + buffer->range_offset(),
           buffer->range_length());

    mBuffer->setRange(0, mBuffer->size() + buffer->range_length());
}

bool AVIExtractor::MP3Splitter::resync() {
    if (mBuffer == NULL) {
        return false;
    }

    bool foundSync = false;
    for (size_t offset = 0; offset + 3 < mBuffer->size(); ++offset) {
        uint32_t firstHeader = U32_AT(mBuffer->data() + offset);

        size_t frameSize;
        if (!GetMPEGAudioFrameSize(firstHeader, &frameSize)) {
            continue;
        }

        size_t subsequentOffset = offset + frameSize;
        size_t i = 3;
        while (i > 0) {
            if (subsequentOffset + 3 >= mBuffer->size()) {
                break;
            }

            static const uint32_t kMask = 0xfffe0c00;

            uint32_t header = U32_AT(mBuffer->data() + subsequentOffset);
            if ((header & kMask) != (firstHeader & kMask)) {
                break;
            }

            if (!GetMPEGAudioFrameSize(header, &frameSize)) {
                break;
            }

            subsequentOffset += frameSize;
            --i;
        }

        if (i == 0) {
            foundSync = true;
            memmove(mBuffer->data(),
                    mBuffer->data() + offset,
                    mBuffer->size() - offset);

            mBuffer->setRange(0, mBuffer->size() - offset);
            break;
        }
    }

    return foundSync;
}

status_t AVIExtractor::MP3Splitter::read(MediaBuffer **out) {
    *out = NULL;

    if (mFindSync) {
        if (!resync()) {
            return -EAGAIN;
        }

        mFindSync = false;
    }

    if (mBuffer->size() < 4) {
        return -EAGAIN;
    }

    uint32_t header = U32_AT(mBuffer->data());
    size_t frameSize;
    int sampleRate;
    int numSamples;
    if (!GetMPEGAudioFrameSize(
                header, &frameSize, &sampleRate, NULL, NULL, &numSamples)) {
        return ERROR_MALFORMED;
    }

    if (mBuffer->size() < frameSize) {
        return -EAGAIN;
    }

    MediaBuffer *mbuf = new MediaBuffer(frameSize);
    memcpy(mbuf->data(), mBuffer->data(), frameSize);

    int64_t timeUs = mBaseTimeUs + (mNumSamplesRead * 1000000ll) / sampleRate;
    mNumSamplesRead += numSamples;

    mbuf->meta_data()->setInt64(kKeyTime, timeUs);

    mBuffer->setRange(
            mBuffer->offset() + frameSize, mBuffer->size() - frameSize);

    *out = mbuf;

    return OK;
}

////////////////////////////////////////////////////////////////////////////////

AVIExtractor::AVIExtractor(const sp<DataSource> &dataSource)
    : mDataSource(dataSource) {
    mInitCheck = parseHeaders();

    if (mInitCheck != OK) {
        mTracks.clear();
    }
}

AVIExtractor::~AVIExtractor() {
#ifdef RTK_PLATFORM
    if(avi2SuperIndex != NULL) {
        for(size_t i = 0 ; i < mainHeader.dwStreams ; i++) {
            if(avi2SuperIndex[i].aIndex != NULL) {
                free(avi2SuperIndex[i].aIndex);
                avi2SuperIndex[i].aIndex = NULL;
            }
        }
        free(avi2SuperIndex);
        avi2SuperIndex = NULL;
    }
#endif
}

size_t AVIExtractor::countTracks() {
    return mTracks.size();
}

sp<IMediaSource> AVIExtractor::getTrack(size_t index) {
    return index < mTracks.size() ? new AVISource(this, index) : NULL;
}

sp<MetaData> AVIExtractor::getTrackMetaData(
        size_t index, uint32_t flags) {
#ifdef RTK_PLATFORM
    if (flags & kIncludeExtensiveMetaData)
        ALOGD("\033[0;32m [%d]  flags= %d, kIncludeExtensiveMetaData= %d  \033[m", __LINE__, flags, kIncludeExtensiveMetaData);
#endif

    return index < mTracks.size() ? mTracks.editItemAt(index).mMeta : NULL;
}

sp<MetaData> AVIExtractor::getMetaData() {
    sp<MetaData> meta = new MetaData;

    if (mInitCheck == OK) {
        meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_CONTAINER_AVI);
    }

    return meta;
}

status_t AVIExtractor::parseHeaders() {
    mTracks.clear();
    mMovieOffset = 0;
    mFoundIndex = false;
    mOffsetsAreAbsolute = false;
#ifdef RTK_PLATFORM
    memset(&mainHeader, 0, sizeof(AVIMAINHEADER));
    avi2SuperIndex = NULL;
    indexType = NO_INDEX;
    isSend_MPEG4_ESDS = false;
#endif

    ssize_t res = parseChunk(0ll, -1ll);

    if (res < 0) {
#ifdef RTK_PLATFORM
        ALOGD("\033[0;33m [%d]  res = %d  \033[m", __LINE__, (int)res);
#endif
        return (status_t)res;
    }

    if (mMovieOffset == 0ll || !mFoundIndex) {
#ifdef RTK_PLATFORM
        ALOGD("\033[0;33m [%d]  mMovieOffset= %lld, mFoundIndex = %d  \033[m", __LINE__, (long long)mMovieOffset, mFoundIndex);
#endif
        return ERROR_MALFORMED;
    }

    return OK;
}

ssize_t AVIExtractor::parseChunk(off64_t offset, off64_t size, int depth) {
    if (size >= 0 && size < 8) {
        return ERROR_MALFORMED;
    }

    uint8_t tmp[12];
    ssize_t n = mDataSource->readAt(offset, tmp, 8);

    if (n < 8) {
        return (n < 0) ? n : (ssize_t)ERROR_MALFORMED;
    }

    uint32_t fourcc = U32_AT(tmp);
    uint32_t chunkSize = U32LE_AT(&tmp[4]);
#ifdef RTK_PLATFORM
    //ALOGD("\033[0;32m [%d]  fourcc= 0x%X [%.4s], chunkSize= %d, depth= %d   offset= %lld   size= %lld  \033[m", __LINE__, fourcc, (unsigned char *)&fourcc, chunkSize, depth, (long long)offset, (long long)size);
#endif

    if (size >= 0 && chunkSize + 8 > size) {
        return ERROR_MALFORMED;
    }

    static const char kPrefix[] = "                              ";
    const char *prefix = &kPrefix[strlen(kPrefix) - 2 * depth];

    if (fourcc == FOURCC('L', 'I', 'S', 'T')
            || fourcc == FOURCC('R', 'I', 'F', 'F')) {
        // It's a list of chunks

        if (size >= 0 && size < 12) {
            return ERROR_MALFORMED;
        }

        n = mDataSource->readAt(offset + 8, &tmp[8], 4);

        if (n < 4) {
            return (n < 0) ? n : (ssize_t)ERROR_MALFORMED;
        }

        uint32_t subFourcc = U32_AT(&tmp[8]);
        if (chunkSize == 0)
        {
            long long streamTotalBytes = 0;
            if (mDataSource->getSize((off64_t *)&streamTotalBytes) != OK)
                streamTotalBytes = 0;
            if (streamTotalBytes != 0)
                chunkSize = (uint32_t)(streamTotalBytes - (offset + 8));
            ALOGD("\033[0;32m [%d]     offset= %lld   size= %lld, Size is 0, try to set new size: %d  \033[m", __LINE__, (long long)offset, (long long)size, chunkSize);
        }

        ALOGV("%s offset 0x%08llx LIST of '%c%c%c%c', size %d",
             prefix,
             (long long)offset,
             (char)(subFourcc >> 24),
             (char)((subFourcc >> 16) & 0xff),
             (char)((subFourcc >> 8) & 0xff),
             (char)(subFourcc & 0xff),
             chunkSize - 4);

        if (subFourcc == FOURCC('m', 'o', 'v', 'i')) {
            // We're not going to parse this, but will take note of the
            // offset.

            mMovieOffset = offset;

            const char *tmp = NULL;
            bool isAVS = false;
            for (size_t i = 0; i < mTracks.size(); i++)
            {
                Track *track = &mTracks.editItemAt(i);
                track->mMeta->findCString(kKeyMIMEType, &tmp);
                if (!strcasecmp(tmp, MEDIA_MIMETYPE_VIDEO_AVS))
                {
                    isAVS = true;
                    break;
                }
            }

            if (isAVS)
            {
                status_t err = Build_AVS_index(offset, chunkSize);
                if (err == OK)
                    mFoundIndex = true;
            }
        } else {
            off64_t subOffset = offset + 12;
            off64_t subOffsetLimit = subOffset + chunkSize - 4;
            while (subOffset < subOffsetLimit) {
                ssize_t res =
                    parseChunk(subOffset, subOffsetLimit - subOffset, depth + 1);

                if (res < 0) {
                    return res;
                }

                subOffset += res;
            }
        }
    } else {
        ALOGV("%s offset 0x%08llx CHUNK '%c%c%c%c'",
             prefix,
             (long long)offset,
             (char)(fourcc >> 24),
             (char)((fourcc >> 16) & 0xff),
             (char)((fourcc >> 8) & 0xff),
             (char)(fourcc & 0xff));

        status_t err = OK;

        switch (fourcc) {
            case FOURCC('s', 't', 'r', 'h'):
            {
                err = parseStreamHeader(offset + 8, chunkSize);
                break;
            }

            case FOURCC('s', 't', 'r', 'f'):
            {
                err = parseStreamFormat(offset + 8, chunkSize);
                break;
            }

            case FOURCC('i', 'd', 'x', '1'):
            {
#ifdef RTK_PLATFORM
                if (!(mFoundIndex == true && indexType == AVI2_TWO_TIERED_INDEX))
#endif
                err = parseIndex(offset + 8, chunkSize);
                break;
            }

#ifdef RTK_PLATFORM
            case FOURCC('a', 'v', 'i', 'h'):
            {
                err = ReadavihInfo(offset, chunkSize+8);
                if (err == OK)
                {
                    //ALOGD("\033[0;33m [%d]  avih    fcc=%.4s   cb= %d   dwMicroSecPerFrame= %d    dwStreams= %d    dwWidth= %d    dwHeight= %d\033[m", __LINE__, (unsigned char *)&mainHeader.fcc, mainHeader.cb, mainHeader.dwMicroSecPerFrame, mainHeader.dwStreams, mainHeader.dwWidth, mainHeader.dwHeight);
                    if (avi2SuperIndex == NULL)
                    {
                        avi2SuperIndex = (AVISUPERINDEX *)malloc(sizeof(AVISUPERINDEX) * mainHeader.dwStreams);
                        memset(avi2SuperIndex, 0, sizeof(AVISUPERINDEX) * mainHeader.dwStreams);
                    }
                }
                else
                    ALOGD("\033[0;33m [%d]  avih   chunkSize= %d   sizeof(AVIMAINHEADER)= %d  \033[m", __LINE__, (int)chunkSize, (int)sizeof(AVIMAINHEADER));
                break;
            }

            case FOURCC('i', 'n', 'd', 'x'):
            {
                // try if there is index chunk for AVI2.0 Super Indexing (Optional)
                err = ReadindxInfo(offset, chunkSize+8);
                break;
            }
#endif

            default:
#ifdef RTK_PLATFORM
                //ALOGD("\033[0;33m [%d]  fourcc= 0x%X [%.4s]  \033[m", __LINE__, fourcc, (unsigned char *)&fourcc);
#endif
                break;
        }

        if (err != OK) {
            return err;
        }
    }

    if (chunkSize & 1) {
        ++chunkSize;
    }

    return chunkSize + 8;
}

static const char *GetMIMETypeForHandler(uint32_t handler) {
    switch (handler) {
        // Wow... shamelessly copied from
        // http://wiki.multimedia.cx/index.php?title=ISO_MPEG-4

        case FOURCC('3', 'I', 'V', '2'):
        case FOURCC('3', 'i', 'v', '2'):
        case FOURCC('B', 'L', 'Z', '0'):
        case FOURCC('D', 'I', 'G', 'I'):
        case FOURCC('D', 'I', 'V', '1'):
        case FOURCC('d', 'i', 'v', '1'):
        case FOURCC('D', 'I', 'V', 'X'):
        case FOURCC('d', 'i', 'v', 'x'):
        case FOURCC('D', 'X', '5', '0'):
        case FOURCC('d', 'x', '5', '0'):
        case FOURCC('D', 'X', 'G', 'M'):
        case FOURCC('E', 'M', '4', 'A'):
        case FOURCC('E', 'P', 'H', 'V'):
        case FOURCC('F', 'M', 'P', '4'):
        case FOURCC('f', 'm', 'p', '4'):
        case FOURCC('F', 'V', 'F', 'W'):
        case FOURCC('H', 'D', 'X', '4'):
        case FOURCC('h', 'd', 'x', '4'):
        case FOURCC('M', '4', 'C', 'C'):
        case FOURCC('M', '4', 'S', '2'):
        case FOURCC('m', '4', 's', '2'):
        case FOURCC('M', 'P', '4', 'S'):
        case FOURCC('m', 'p', '4', 's'):
        case FOURCC('M', 'P', '4', 'V'):
        case FOURCC('m', 'p', '4', 'v'):
        case FOURCC('M', 'V', 'X', 'M'):
        case FOURCC('R', 'M', 'P', '4'):
        case FOURCC('S', 'E', 'D', 'G'):
        case FOURCC('S', 'M', 'P', '4'):
        case FOURCC('U', 'M', 'P', '4'):
        case FOURCC('W', 'V', '1', 'F'):
        case FOURCC('X', 'V', 'I', 'D'):
        case FOURCC('X', 'v', 'i', 'D'):
        case FOURCC('x', 'v', 'i', 'd'):
        case FOURCC('X', 'V', 'I', 'X'):
            return MEDIA_MIMETYPE_VIDEO_MPEG4;

        // from http://wiki.multimedia.cx/index.php?title=H264
        case FOURCC('a', 'v', 'c', '1'):
        case FOURCC('d', 'a', 'v', 'c'):
        case FOURCC('x', '2', '6', '4'):
        case FOURCC('H', '2', '6', '4'):
        case FOURCC('v', 's', 's', 'h'):
            return MEDIA_MIMETYPE_VIDEO_AVC;

#ifdef RTK_PLATFORM
        case FOURCC('M', 'J', 'P', 'G'):
        case FOURCC('m', 'j', 'p', 'g'):
            return MEDIA_MIMETYPE_VIDEO_MJPEG;

        case FOURCC('W', 'M', 'V', '3'):
        case FOURCC('w', 'm', 'v', '3'):
            return MEDIA_MIMETYPE_VIDEO_WMV3;

        case FOURCC('W', 'V', 'C', '1'):
        case FOURCC('w', 'v', 'c', '1'):
            return MEDIA_MIMETYPE_VIDEO_WVC1;

        case FOURCC('M', 'P', '4', '3'):
        case FOURCC('m', 'p', '4', '3'):
            return MEDIA_MIMETYPE_VIDEO_MP43;

        case FOURCC('M', 'P', 'G', '2'):
        case FOURCC('m', 'p', 'g', '2'):
            return MEDIA_MIMETYPE_VIDEO_MPEG2;

        case FOURCC('C', 'A', 'V', 'S'):
        case FOURCC('c', 'a', 'v', 's'):
            return MEDIA_MIMETYPE_VIDEO_AVS;
#endif

        default:
#ifdef RTK_PLATFORM
            ALOGD("\033[0;32m [%d]  handler=0x%X  \033[m", __LINE__, handler);
#endif
            return NULL;
    }
}

status_t AVIExtractor::parseStreamHeader(off64_t offset, size_t size) {
    //if (size != 56) {
    if (size < 56) {
        return ERROR_MALFORMED;
    }

    if (mTracks.size() > 99) {
        return -ERANGE;
    }

    sp<ABuffer> buffer = new ABuffer(size);
    ssize_t n = mDataSource->readAt(offset, buffer->data(), buffer->size());

    if (n < (ssize_t)size) {
        return n < 0 ? (status_t)n : ERROR_MALFORMED;
    }

    const uint8_t *data = buffer->data();

    uint32_t type = U32_AT(data);
    uint32_t handler = U32_AT(&data[4]);
#ifndef RTK_PLATFORM
    uint32_t flags = U32LE_AT(&data[8]);
#endif

    sp<MetaData> meta = new MetaData;

#ifdef RTK_PLATFORM
    uint32_t scale = U32LE_AT(&data[20]);
    uint32_t rate = U32LE_AT(&data[24]);
    uint32_t dwStart = U32LE_AT(&data[28]);
#else
    uint32_t rate = U32LE_AT(&data[20]);
    uint32_t scale = U32LE_AT(&data[24]);
#endif

    uint32_t sampleSize = U32LE_AT(&data[44]);

    const char *mime = NULL;
    Track::Kind kind = Track::OTHER;

    if (type == FOURCC('v', 'i', 'd', 's')) {
        mime = GetMIMETypeForHandler(handler);
#ifdef RTK_PLATFORM
        //ALOGD("\033[0;32m [%d]  mime= %s  \033[m", __LINE__, mime);
        FiledurationUs = 0;
#endif

        if (mime && strncasecmp(mime, "video/", 6)) {
            return ERROR_MALFORMED;
        }

        if (mime == NULL) {
            ALOGW("Unsupported video format '%c%c%c%c'",
                 (char)(handler >> 24),
                 (char)((handler >> 16) & 0xff),
                 (char)((handler >> 8) & 0xff),
                 (char)(handler & 0xff));
        }

        kind = Track::VIDEO;
#ifdef RTK_PLATFORM
        float fFrameRate = ((float)rate) / ((float)scale);
        meta->setFloat(kKeyFrameRateFloat, fFrameRate);

        int32_t frameRate = rate / scale;
        meta->setInt32(kKeyFrameRate, frameRate);
#endif
    } else if (type == FOURCC('a', 'u', 'd', 's')) {
#ifdef RTK_PLATFORM
        //ALOGD("\033[0;32m [%d]  mime= %s  \033[m", __LINE__, mime);
#endif
        if (mime && strncasecmp(mime, "audio/", 6)) {
            return ERROR_MALFORMED;
        }

        kind = Track::AUDIO;
    }

    if (!mime) {
        mime = "application/octet-stream";
    }

    meta->setCString(kKeyMIMEType, mime);

    mTracks.push();
    Track *track = &mTracks.editItemAt(mTracks.size() - 1);
#ifdef RTK_PLATFORM
    ALOGD("\033[0;33m [%d]  mime= %s   mTracks.size()= %d, rate= %d, scale= %d, dwStart= %d\033[m", __LINE__, mime, (int)mTracks.size(), rate, scale, dwStart);
    track->mStart = dwStart;
    audioFrameCount = 0;
    if (mime && !strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_WVC1))
        track->mVC1Type = 1;
    else if (mime && !strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_WMV3))
        track->mVC1Type = 2;
    else
        track->mVC1Type = 0;
    if (kind == Track::VIDEO)
        ALOGD("\033[0;33m [%d]  track->mVC1Type= %d \033[m", __LINE__, track->mVC1Type);
#endif

    track->mMeta = meta;
    track->mRate = rate;
    track->mScale = scale;
    track->mBytesPerSample = sampleSize;
    track->mKind = kind;
    track->mNumSyncSamples = 0;
    track->mThumbnailSampleSize = 0;
    track->mThumbnailSampleIndex = -1;
    track->mMaxSampleSize = 0;
    track->mAvgChunkSize = 1.0;
    track->mFirstChunkSize = 0;

    return OK;
}

#ifdef RTK_PLATFORM
bool IsDivxCodecID(uint32_t CodecID)
{
    switch (CodecID)
    {
        case FOURCC('D', 'I', 'V', '3'):
        case FOURCC('d', 'i', 'v', '3'):
        case FOURCC('D', 'I', 'V', '4'):
        case FOURCC('d', 'i', 'v', '4'):
        case FOURCC('D', 'I', 'V', 'X'):
        case FOURCC('D', 'i', 'v', 'x'):
        case FOURCC('d', 'i', 'v', 'x'):
        case FOURCC('D', 'X', '5', '0'):
        case FOURCC('d', 'x', '5', '0'):
            return true;

        default:
            return false;
    }
}
#endif

status_t AVIExtractor::parseStreamFormat(off64_t offset, size_t size) {
    if (mTracks.isEmpty()) {
        return ERROR_MALFORMED;
    }

    Track *track = &mTracks.editItemAt(mTracks.size() - 1);

    if (track->mKind == Track::OTHER) {
        // We don't support this content, but that's not a parsing error.
        return OK;
    }

    bool isVideo = (track->mKind == Track::VIDEO);
#ifdef RTK_PLATFORM
    //ALOGD("\033[0;33m [%d]  isVideo= %d, strf size = %d  \033[m", __LINE__, isVideo, (int)size);
#endif

    if ((isVideo && size < 40) || (!isVideo && size < 16)) {
        // Expected a BITMAPINFO or WAVEFORMAT(EX) structure, respectively.
        return ERROR_MALFORMED;
    }

    sp<ABuffer> buffer = new ABuffer(size);
    ssize_t n = mDataSource->readAt(offset, buffer->data(), buffer->size());

    if (n < (ssize_t)size) {
        return n < 0 ? (status_t)n : ERROR_MALFORMED;
    }

    const uint8_t *data = buffer->data();

    if (isVideo) {
        uint32_t width = U32LE_AT(&data[4]);
        uint32_t height = U32LE_AT(&data[8]);
#ifdef RTK_PLATFORM
        ALOGD("\033[0;33m [%d]  Video:  width= %d, height = %d  \033[m", __LINE__, width, height);

        uint32_t biSize = U32LE_AT(&data[0]);
        uint32_t biCompression = U32_AT(&data[16]);
        if (IsDivxCodecID(biCompression))
        {
            ALOGD("\033[0;31m [%d]  video disabled due to Divx CodecID\033[m", __LINE__);
            return ERROR_UNSUPPORTED;
        }

        if ((track->mVC1Type == 1) || (track->mVC1Type == 2)) // WVC1 or WMV3
        {
            uint8_t extraData[128];
            memset(&extraData[0], 0, 128);

            if ((biSize >= 40) && ((biSize - 40) <= 128)) // 40 means sizeof(BITMAPINFOHEADER)
            {
                memcpy(&extraData[0], &data[40], biSize - 40);
                track->mMeta->setData(kKeyPrivateInfo, 0, extraData, biSize - 40);
            }
        }
        else
        {
            const char *tmp;
            track->mMeta->findCString(kKeyMIMEType, &tmp);
            AString mime = tmp;

            if (!strcasecmp(mime.c_str(), MEDIA_MIMETYPE_VIDEO_MPEG4))
            {
                if (biSize > 40) // 40 means sizeof(BITMAPINFOHEADER)
                {
                    sp<ABuffer> payloadData = new ABuffer(biSize - 40);
                    memcpy(payloadData->data(), &data[40], biSize - 40);
                    payloadData->setRange(0, biSize - 40);

                    sp<ABuffer> csd = MakeMPEG4VideoCodecSpecificData(payloadData);
                    track->mMeta->setData(kKeyESDS, kTypeESDS, csd->data(), csd->size());
                }
                isSend_MPEG4_ESDS = true;
                ALOGD("\033[0;32m [%d]  biSize= %d   isSend_MPEG4_ESDS= %d\033[m", __LINE__, biSize, isSend_MPEG4_ESDS);
            }
        }
#endif

        track->mMeta->setInt32(kKeyWidth, width);
        track->mMeta->setInt32(kKeyHeight, height);
    } else {
        uint32_t format = U16LE_AT(data);

        if (format == 0x55) {
            track->mMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MPEG);
#ifdef RTK_PLATFORM
        } else if (format == 0x50) {
            track->mMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_II);
        } else if (format == 0x2000) {
            track->mMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AC3);
        } else if (format == 0x0001) {
            track->mMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_RAW);
        } else if (format == 0x11 || format == 0x2) { // adpcm
            track->mMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_ADPCM);
        } else if (format == 0x2001) {
            track->mMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_DTS);
#endif
        } else {
            ALOGW("Unsupported audio format = 0x%04x", format);
        }

        uint32_t numChannels = U16LE_AT(&data[2]);
        uint32_t sampleRate = U32LE_AT(&data[4]);
#ifdef RTK_PLATFORM
        uint32_t BitsPerSample = U16LE_AT(&data[14]);
        track->mMeta->setInt32(kKeyAudioBitsPerSample, BitsPerSample);

        if (format == 0x0001) // little endian pcm
        {
            //ALOGD("\033[0;36m [%d] sample_size= %d \033[m", __LINE__, BitsPerSample);
            if (BitsPerSample == 8)
                track->mMeta->setInt32(kKeyPcmEncoding, kAudioEncodingPcm8bit);
            else if (BitsPerSample == 16)
                track->mMeta->setInt32(kKeyPcmEncoding, kAudioEncodingPcm16bit);
            else if (BitsPerSample == 32)
                track->mMeta->setInt32(kKeyPcmEncoding, kAudioEncodingPcmFloat);
        }

        ALOGD("\033[0;33m [%d]  Audio:   format= 0x%X, numChannels= %d, sampleRate = %d, BitsPerSample= %d  \033[m", __LINE__, format, numChannels, sampleRate, BitsPerSample);
        memset((uint8_t *)&track->waveformHeader, 0, (size_t)sizeof(track->waveformHeader));
        if (n >= (ssize_t)(sizeof(track->waveformHeader) - 2))
        {
            // sizeof(WAVEFORMATEX): 20 bytes, but only use 18 bytes
            memcpy((uint8_t *)&track->waveformHeader, data, (size_t)(sizeof(track->waveformHeader) - 2));
            if (format == 0x11 || format == 0x2) // adpcm
                track->mMeta->setData(kKeyPrivateInfo, 0, (uint8_t *)&track->waveformHeader, sizeof(track->waveformHeader));
        }
        else
            memcpy((uint8_t *)&track->waveformHeader, data, n);
        //ALOGD("\033[0;33m [%d]  Audio:   wFormatTag= 0x%X, nChannels= %d, nSamplesPerSec = %d   nAvgBytesPerSec= %d    nBlockAlign= %d    wBitsPerSample= %d   cbSize= %d  \033[m", __LINE__, track->waveformHeader.wFormatTag, track->waveformHeader.nChannels, track->waveformHeader.nSamplesPerSec, track->waveformHeader.nAvgBytesPerSec, track->waveformHeader.nBlockAlign, track->waveformHeader.wBitsPerSample, track->waveformHeader.cbSize);
#endif

        track->mMeta->setInt32(kKeyChannelCount, numChannels);
        track->mMeta->setInt32(kKeySampleRate, sampleRate);
    }

    return OK;
}

// static
bool AVIExtractor::IsCorrectChunkType(
        ssize_t trackIndex, Track::Kind kind, uint32_t chunkType) {
    uint32_t chunkBase = chunkType & 0xffff;

    switch (kind) {
        case Track::VIDEO:
        {
            if (chunkBase != FOURCC(0, 0, 'd', 'c')
                    && chunkBase != FOURCC(0, 0, 'd', 'b')) {
                return false;
            }
            break;
        }

        case Track::AUDIO:
        {
            if (chunkBase != FOURCC(0, 0, 'w', 'b')) {
                return false;
            }
            break;
        }

        default:
            break;
    }

    if (trackIndex < 0) {
        return true;
    }

    uint8_t hi = chunkType >> 24;
    uint8_t lo = (chunkType >> 16) & 0xff;

    if (hi < '0' || hi > '9' || lo < '0' || lo > '9') {
        return false;
    }

    if (trackIndex != (10 * (hi - '0') + (lo - '0'))) {
        return false;
    }

    return true;
}

status_t AVIExtractor::parseIndex(off64_t offset, size_t size) {
    if ((size % 16) != 0) {
        return ERROR_MALFORMED;
    }

    sp<ABuffer> buffer = new ABuffer(size);
    ssize_t n = mDataSource->readAt(offset, buffer->data(), buffer->size());

    if (n < (ssize_t)size) {
        return n < 0 ? (status_t)n : ERROR_MALFORMED;
    }

    const uint8_t *data = buffer->data();

    while (size > 0) {
        uint32_t chunkType = U32_AT(data);

        uint8_t hi = chunkType >> 24;
        uint8_t lo = (chunkType >> 16) & 0xff;

        if (hi < '0' || hi > '9' || lo < '0' || lo > '9') {
            return ERROR_MALFORMED;
        }

        size_t trackIndex = 10 * (hi - '0') + (lo - '0');

        if (trackIndex >= mTracks.size()) {
            return ERROR_MALFORMED;
        }

        Track *track = &mTracks.editItemAt(trackIndex);

        if (!IsCorrectChunkType(-1, track->mKind, chunkType)) {
            return ERROR_MALFORMED;
        }

        if (track->mKind == Track::OTHER) {
            data += 16;
            size -= 16;
            continue;
        }

        uint32_t flags = U32LE_AT(&data[4]);
        uint32_t offset = U32LE_AT(&data[8]);
        uint32_t chunkSize = U32LE_AT(&data[12]);

        if (chunkSize > track->mMaxSampleSize) {
            track->mMaxSampleSize = chunkSize;
        }

        track->mSamples.push();

        SampleInfo *info =
            &track->mSamples.editItemAt(track->mSamples.size() - 1);

        info->mOffset = offset;
        info->mIsKey = (flags & 0x10) != 0;
#ifdef RTK_PLATFORM
        info->mSize = chunkSize;
#endif

        if (info->mIsKey) {
            static const size_t kMaxNumSyncSamplesToScan = 20;

            if (track->mNumSyncSamples < kMaxNumSyncSamplesToScan) {
                if (chunkSize > track->mThumbnailSampleSize) {
                    track->mThumbnailSampleSize = chunkSize;

                    track->mThumbnailSampleIndex =
                        track->mSamples.size() - 1;
                }
            }

            ++track->mNumSyncSamples;
        }

        data += 16;
        size -= 16;
    }

    if (!mTracks.isEmpty()) {
        off64_t offset;
        size_t size;
        bool isKey;
        int64_t timeUs;
        status_t err = getSampleInfo(0, 0, &offset, &size, &isKey, &timeUs);

        if (err != OK) {
            mOffsetsAreAbsolute = !mOffsetsAreAbsolute;
            err = getSampleInfo(0, 0, &offset, &size, &isKey, &timeUs);

            if (err != OK) {
                return err;
            }
        }

        ALOGV("Chunk offsets are %s",
             mOffsetsAreAbsolute ? "absolute" : "movie-chunk relative");
    }

    for (size_t i = 0; i < mTracks.size(); ++i) {
        Track *track = &mTracks.editItemAt(i);

        if (track->mBytesPerSample > 0) {
            // Assume all chunks are roughly the same size for now.

            // Compute the avg. size of the first 128 chunks (if there are
            // that many), but exclude the size of the first one, since
            // it may be an outlier.
            size_t numSamplesToAverage = track->mSamples.size();
            if (numSamplesToAverage > 256) {
                numSamplesToAverage = 256;
            }

            double avgChunkSize = 0;
            size_t j;
#ifdef RTK_PLATFORM
            for (j = 0; j < numSamplesToAverage; ++j) {
#else
            for (j = 0; j <= numSamplesToAverage; ++j) {
#endif
                off64_t offset;
                size_t size;
                bool isKey;
                int64_t dummy;

                status_t err =
                    getSampleInfo(
                            i, j,
                            &offset, &size, &isKey, &dummy);

                if (err != OK) {
                    return err;
                }

                if (j == 0) {
                    track->mFirstChunkSize = size;
                    continue;
                }

                avgChunkSize += size;
            }

            avgChunkSize /= numSamplesToAverage;

            track->mAvgChunkSize = avgChunkSize;
        }

        int64_t durationUs;
        CHECK_EQ((status_t)OK,
                 getSampleTime(i, track->mSamples.size() - 1, &durationUs));

        ALOGV("track %d duration = %.2f secs", (int)i, durationUs / 1E6);

#ifdef RTK_PLATFORM
    if (track->mVC1Type == 1)
        track->mMaxSampleSize += 4;

    if ((durationUs > FiledurationUs) && (durationUs != 0))
        FiledurationUs = durationUs;
#endif
        track->mMeta->setInt64(kKeyDuration, durationUs);
        track->mMeta->setInt32(kKeyMaxInputSize, track->mMaxSampleSize);

        const char *tmp;
        CHECK(track->mMeta->findCString(kKeyMIMEType, &tmp));

        AString mime = tmp;

        if (!strncasecmp("video/", mime.c_str(), 6)) {
            if (track->mThumbnailSampleIndex >= 0) {
                int64_t thumbnailTimeUs;
                CHECK_EQ((status_t)OK,
                         getSampleTime(i, track->mThumbnailSampleIndex,
                                       &thumbnailTimeUs));

                track->mMeta->setInt64(kKeyThumbnailTime, thumbnailTimeUs);
            }

            status_t err = OK;

            if (!strcasecmp(mime.c_str(), MEDIA_MIMETYPE_VIDEO_MPEG4)) {
#ifdef RTK_PLATFORM
                if (!isSend_MPEG4_ESDS)
#endif
                err = addMPEG4CodecSpecificData(i);
            } else if (!strcasecmp(mime.c_str(), MEDIA_MIMETYPE_VIDEO_AVC)) {
                err = addH264CodecSpecificData(i);
            }

            if (err != OK) {
                return err;
            }
        }
    }

    mFoundIndex = true;

    return OK;
}

#ifdef RTK_PLATFORM
status_t AVIExtractor::ReadavihInfo(off64_t offset, size_t size) {
    ssize_t n = mDataSource->readAt(offset, (void *)&mainHeader, sizeof(AVIMAINHEADER));

    if (n < (ssize_t)size) {
        return n < 0 ? (status_t)n : ERROR_MALFORMED;
    }

    return OK;
}
status_t AVIExtractor::ReadindxInfo(off64_t offset, size_t size) {
    if (mTracks.isEmpty()) {
        return ERROR_MALFORMED;
    }

    Track *track = &mTracks.editItemAt(mTracks.size() - 1);
    size_t trackIndex = mTracks.size() - 1;

    ssize_t n = mDataSource->readAt(offset, (void *)&(avi2SuperIndex[trackIndex]), 32);
    if (n > (ssize_t)size) {
        return n < 0 ? (status_t)n : ERROR_MALFORMED;
    }
    indexType = AVI2_TWO_TIERED_INDEX;
    mOffsetsAreAbsolute = true;

    //ALOGD("\033[0;33m [%d]  fcc= %.4s    dwChunkId= %.4s\033[m", __LINE__, (unsigned char *)&(avi2SuperIndex[trackIndex].fcc), (unsigned char *)&(avi2SuperIndex[trackIndex].dwChunkId));
    //ALOGD("\033[0;33m [%d]  nEntriesInUse= %d    wLongsPerEntry= %d    bIndexType= %d   bIndexSubType= %d\033[m", __LINE__, avi2SuperIndex[trackIndex].nEntriesInUse, avi2SuperIndex[trackIndex].wLongsPerEntry, avi2SuperIndex[trackIndex].bIndexType, avi2SuperIndex[trackIndex].bIndexSubType);
    avi2SuperIndex[trackIndex].aIndex = (struct _avisuperindex_entry *)malloc(sizeof(AVISUPERINDEX_ENTRY) * avi2SuperIndex[trackIndex].nEntriesInUse);

    n = mDataSource->readAt(offset+32, (void *)avi2SuperIndex[trackIndex].aIndex, sizeof(AVISUPERINDEX_ENTRY) * avi2SuperIndex[trackIndex].nEntriesInUse);
    if (n < (ssize_t)(sizeof(AVISUPERINDEX_ENTRY) * avi2SuperIndex[trackIndex].nEntriesInUse)) {
        return n < 0 ? (status_t)n : ERROR_MALFORMED;
    }

    // Building ODML index
    AVISTDINDEX STDINDEX;
    memset((uint8_t *)&STDINDEX, 0, sizeof(AVISTDINDEX));
    for (size_t i = 0; i < avi2SuperIndex[trackIndex].nEntriesInUse; i++) {
        n = mDataSource->readAt(avi2SuperIndex[trackIndex].aIndex[i].qwOffset, (void *)&STDINDEX, sizeof(AVISTDINDEX));
        if (n < (ssize_t)sizeof(AVISTDINDEX)) {
            return n < 0 ? (status_t)n : ERROR_MALFORMED;
        }
        //ALOGD("\033[0;33m [%d]  qwOffset= %lld    dwSize= %d   dwDuration= %d    sizeof(AVISTDINDEX)= %d\033[m", __LINE__, (long long)avi2SuperIndex[trackIndex].aIndex[i].qwOffset, avi2SuperIndex[trackIndex].aIndex[i].dwSize, avi2SuperIndex[trackIndex].aIndex[i].dwDuration, (int)sizeof(AVISTDINDEX));
        //ALOGD("\033[0;33m [%d]  fcc= %.4s    cb= %d   wLongsPerEntry= %d   bIndexSubType= %d    bIndexType= %d   nEntriesInUse= %d   dwChunkId= %.4s   qwBaseOffset1= %u   qwBaseOffset2= %u\033[m", __LINE__, (unsigned char *)&(STDINDEX.fcc), STDINDEX.cb, STDINDEX.wLongsPerEntry, STDINDEX.bIndexSubType, STDINDEX.bIndexType, STDINDEX.nEntriesInUse, (unsigned char *)&(STDINDEX.dwChunkId), STDINDEX.qwBaseOffset1, STDINDEX.qwBaseOffset2);

        if (STDINDEX.wLongsPerEntry == 2 && STDINDEX.bIndexSubType == 0 && STDINDEX.bIndexType == AVI_INDEX_OF_CHUNKS)
        {
            sp<ABuffer> buffer = new ABuffer(avi2SuperIndex[trackIndex].aIndex[i].dwSize - sizeof(AVISTDINDEX));
            n = mDataSource->readAt(avi2SuperIndex[trackIndex].aIndex[i].qwOffset + sizeof(AVISTDINDEX), buffer->data(), buffer->size());

            if (n < (ssize_t)(avi2SuperIndex[trackIndex].aIndex[i].dwSize - sizeof(AVISTDINDEX))) {
                return n < 0 ? (status_t)n : ERROR_MALFORMED;
            }

            const uint8_t *data = buffer->data();
            AVISTDINDEX_ENTRY *IDX_ENTRY = (AVISTDINDEX_ENTRY *)data;
            for (size_t j = 0; j < STDINDEX.nEntriesInUse; j++)
            {
                track->mSamples.push();
                SampleInfo *info = &track->mSamples.editItemAt(track->mSamples.size() - 1);

                info->mOffset = (STDINDEX.qwBaseOffset1 + ((uint64_t)(STDINDEX.qwBaseOffset2)<<32) + IDX_ENTRY[j].dwOffset - 8);
                info->mSize = (IDX_ENTRY[j].dwSize & AVISTDINDEX_SIZEMASK);
                info->mIsKey = !(IDX_ENTRY[j].dwSize & AVISTDINDEX_DELTAFRAME);
                if ((IDX_ENTRY[j].dwSize & AVISTDINDEX_SIZEMASK) > track->mMaxSampleSize) {
                    track->mMaxSampleSize = (IDX_ENTRY[j].dwSize & AVISTDINDEX_SIZEMASK);
                }
                if (info->mIsKey) {
                    static const size_t kMaxNumSyncSamplesToScan = 20;

                    if (track->mNumSyncSamples < kMaxNumSyncSamplesToScan) {
                        if ((IDX_ENTRY[j].dwSize & AVISTDINDEX_SIZEMASK) > track->mThumbnailSampleSize) {
                            track->mThumbnailSampleSize = (IDX_ENTRY[j].dwSize & AVISTDINDEX_SIZEMASK);

                            track->mThumbnailSampleIndex = track->mSamples.size() - 1;
                        }
                    }

                    ++track->mNumSyncSamples;
                }
            }
            //ALOGD("\033[0;33m [%d]   mThumbnailSampleIndex= %d, mNumSyncSamples= %d   mSamples.size()= %d    mBytesPerSample= %d\033[m", __LINE__, (int)track->mThumbnailSampleIndex, (int)track->mNumSyncSamples, (int)track->mSamples.size(), track->mBytesPerSample);
        }
        else
            ALOGD("\033[0;33m [%d]   Wrong STDINDEX  wLongsPerEntry= %d, bIndexSubType= %d, bIndexType= %d\033[m", __LINE__, STDINDEX.wLongsPerEntry, STDINDEX.bIndexSubType, STDINDEX.bIndexType);
    }
    int64_t durationUs;
    CHECK_EQ((status_t)OK, getSampleTime(mTracks.size() - 1, track->mSamples.size() - 1, &durationUs));

    //ALOGD("\033[0;33m [%d]   track %d duration = %.2f secs     mMaxSampleSize= %d\033[m", __LINE__, (int)mTracks.size() - 1, durationUs / 1E6, (int)track->mMaxSampleSize);
    ALOGV("track %d duration = %.2f secs", (int)mTracks.size() - 1, durationUs / 1E6);
    if ((durationUs > FiledurationUs) && (durationUs != 0))
        FiledurationUs = durationUs;

    const char *tmp;
    CHECK(track->mMeta->findCString(kKeyMIMEType, &tmp));

    track->mMeta->setInt64(kKeyDuration, durationUs);
    if (track->mVC1Type == 1)
        track->mMaxSampleSize += 4;
    track->mMeta->setInt32(kKeyMaxInputSize, track->mMaxSampleSize);

    AString mime = tmp;

    if (!strncasecmp("video/", mime.c_str(), 6)) {
        if (track->mThumbnailSampleIndex >= 0) {
            int64_t thumbnailTimeUs;
            CHECK_EQ((status_t)OK, getSampleTime(mTracks.size() - 1, track->mThumbnailSampleIndex, &thumbnailTimeUs));

            track->mMeta->setInt64(kKeyThumbnailTime, thumbnailTimeUs);
        }

        status_t err = OK;

        if (!strcasecmp(mime.c_str(), MEDIA_MIMETYPE_VIDEO_MPEG4)) {
            if (!isSend_MPEG4_ESDS)
                err = addMPEG4CodecSpecificData(mTracks.size() - 1);
        } else if (!strcasecmp(mime.c_str(), MEDIA_MIMETYPE_VIDEO_AVC)) {
            err = addH264CodecSpecificData(mTracks.size() - 1);
        }

        if (err != OK) {
            return err;
        }
    }

    mFoundIndex = true;
    return OK;
}

status_t AVIExtractor::Build_AVS_index(off64_t offset, size_t size) {
    int64_t durationUs = 0;
    uint8_t tmp[16] = {0};
    int i = 0;
    bool find_start_code = false;

    off64_t pos = offset + 12;

    while (pos < (offset + size))
    {
        find_start_code = false;
        ssize_t n = mDataSource->readAt(pos, tmp, 16);
        if (tmp[0] == 0) // skip byte of 0x00
        {
            pos++;
            n = mDataSource->readAt(pos, tmp, 16);
        }

        if (n < 16) {
            return (n < 0) ? n : (ssize_t)ERROR_MALFORMED;
        }

        uint32_t idxfourcc = U32_AT(tmp);
        uint32_t chunkSize = U32LE_AT(&tmp[4]);
        //ALOGD("\033[0;32m [%d]     chunkSize= %d   \033[m", __LINE__, chunkSize);
        if ((idxfourcc == FOURCC('0', '0', 'd', 'c')) || (idxfourcc == FOURCC('0', '0', 'd', 'b')))
        {
            Track *track = &mTracks.editItemAt(0);
            track->mSamples.push();
            SampleInfo *info = &track->mSamples.editItemAt(track->mSamples.size() - 1);

            info->mOffset = (uint64_t)(pos - offset - 8);
            //ALOGD("\033[0;36m [%d]     pos= %lld   chunkSize= %d   info->mOffset= %lld   track->mMaxSampleSize= %d\033[m", __LINE__, (long long)pos, chunkSize, (long long)info->mOffset, (int)track->mMaxSampleSize);
            for (i=0; i<(int)(n-3); i++)
            {
                /*if (tmp[i] == 0 && tmp[i+1] == 0 && tmp[i+2] == 1)
                    ALOGD("\033[0;36m [%d]     start code: 00 00 01 0x%.2X %.2X\033[m", __LINE__, tmp[i+3], tmp[i+4]);*/
                if (tmp[i] == 0 && tmp[i+1] == 0 && tmp[i+2] == 1 && tmp[i+3] == 0xB0) // visual_object_sequence_start_code
                {
                    find_start_code = true;
                    break;
                }
            }
            info->mIsKey = find_start_code;
            if (chunkSize > track->mMaxSampleSize)
                track->mMaxSampleSize = (size_t)chunkSize;
            pos += (chunkSize + 8);
        }
        else if ((idxfourcc == FOURCC('0', '0', 'w', 'b')) || (idxfourcc == FOURCC('0', '1', 'w', 'b')))
        {
            Track *track = &mTracks.editItemAt(1);
            track->mSamples.push();
            SampleInfo *info = &track->mSamples.editItemAt(track->mSamples.size() - 1);

            info->mOffset = (uint64_t)(pos - offset - 8);
            info->mIsKey = true;
            //ALOGD("\033[0;35m [%d]     pos= %lld   chunkSize= %d   info->mOffset= %lld   track->mMaxSampleSize= %d\033[m", __LINE__, (long long)pos, chunkSize, (long long)info->mOffset, (int)track->mMaxSampleSize);
            if (chunkSize > track->mMaxSampleSize)
                track->mMaxSampleSize = (size_t)chunkSize;
            pos += (chunkSize + 8);
        }
        else
            pos += (chunkSize + 8);
    }


    for (i=0; i < (int)mTracks.size(); i++)
    {
        durationUs = 0;
        Track *track = &mTracks.editItemAt(i);
        getSampleTime(i, track->mSamples.size() - 1, &durationUs);
        ALOGD("\033[0;33m [%d]   track %d duration = %.2f secs     mMaxSampleSize= %d\033[m", __LINE__, i, durationUs / 1E6, (int)track->mMaxSampleSize);

        if ((durationUs > FiledurationUs) && (durationUs != 0))
            FiledurationUs = durationUs;

        track->mMeta->setInt64(kKeyDuration, durationUs);
        track->mMeta->setInt32(kKeyMaxInputSize, track->mMaxSampleSize);
    }

    return OK;
}
#endif

static size_t GetSizeWidth(size_t x) {
    size_t n = 1;
    while (x > 127) {
        ++n;
        x >>= 7;
    }
    return n;
}

static uint8_t *EncodeSize(uint8_t *dst, size_t x) {
    while (x > 127) {
        *dst++ = (x & 0x7f) | 0x80;
        x >>= 7;
    }
    *dst++ = x;
    return dst;
}

#ifdef RTK_PLATFORM
sp<ABuffer> AVIExtractor::MakeMPEG4VideoCodecSpecificData(const sp<ABuffer> &config) {
#else
sp<ABuffer> MakeMPEG4VideoCodecSpecificData(const sp<ABuffer> &config) {
#endif
    size_t len1 = config->size() + GetSizeWidth(config->size()) + 1;
    size_t len2 = len1 + GetSizeWidth(len1) + 1 + 13;
    size_t len3 = len2 + GetSizeWidth(len2) + 1 + 3;

    sp<ABuffer> csd = new ABuffer(len3);
    uint8_t *dst = csd->data();
    *dst++ = 0x03;
    dst = EncodeSize(dst, len2 + 3);
    *dst++ = 0x00;  // ES_ID
    *dst++ = 0x00;
    *dst++ = 0x00;  // streamDependenceFlag, URL_Flag, OCRstreamFlag

    *dst++ = 0x04;
    dst = EncodeSize(dst, len1 + 13);
    *dst++ = 0x01;  // Video ISO/IEC 14496-2 Simple Profile
    for (size_t i = 0; i < 12; ++i) {
        *dst++ = 0x00;
    }

    *dst++ = 0x05;
    dst = EncodeSize(dst, config->size());
    memcpy(dst, config->data(), config->size());
    dst += config->size();

    // hexdump(csd->data(), csd->size());

    return csd;
}

status_t AVIExtractor::addMPEG4CodecSpecificData(size_t trackIndex) {
    Track *track = &mTracks.editItemAt(trackIndex);

    off64_t offset;
    size_t size;
    bool isKey;
    int64_t timeUs;
    status_t err =
        getSampleInfo(trackIndex, 0, &offset, &size, &isKey, &timeUs);

    if (err != OK) {
        return err;
    }

#ifdef RTK_PLATFORM
    if (offset && !size)
    {
        ALOGD("\033[0;33m [addMPEG4CodecSpecificData - %d]  due to video sample size is 0 from index table, set read 2KBytes data as default for parsing.  \033[m", __LINE__);
        size = 2048;
    }
    //ALOGD("\033[0;33m [addMPEG4CodecSpecificData - %d]   offset= %lld, size= %d, isKey= %d, timeUs= %lld  \033[m", __LINE__, (long long)offset, (int)size, isKey, (long long)timeUs);
#endif

    sp<ABuffer> buffer = new ABuffer(size);
    ssize_t n = mDataSource->readAt(offset, buffer->data(), buffer->size());

    if (n < (ssize_t)size) {
        return n < 0 ? (status_t)n : ERROR_MALFORMED;
    }

    // Extract everything up to the first VOP start code from the first
    // frame's encoded data and use it to construct an ESDS with the
    // codec specific data.

    size_t i = 0;
    bool found = false;
    while (i + 3 < buffer->size()) {
        if (!memcmp("\x00\x00\x01\xb6", &buffer->data()[i], 4)) {
            found = true;
            break;
        }

        ++i;
    }

    if (!found) {
        return ERROR_MALFORMED;
    }

    buffer->setRange(0, i);

    sp<ABuffer> csd = MakeMPEG4VideoCodecSpecificData(buffer);
    track->mMeta->setData(kKeyESDS, kTypeESDS, csd->data(), csd->size());

    return OK;
}

status_t AVIExtractor::addH264CodecSpecificData(size_t trackIndex) {
    Track *track = &mTracks.editItemAt(trackIndex);

    off64_t offset;
    size_t size;
    bool isKey;
    int64_t timeUs;

    // Extract codec specific data from the first non-empty sample.

    size_t sampleIndex = 0;
    for (;;) {
        status_t err =
            getSampleInfo(
                    trackIndex, sampleIndex, &offset, &size, &isKey, &timeUs);

        if (err != OK) {
            return err;
        }

        if (size > 0) {
            break;
        }

        ++sampleIndex;
    }

    sp<ABuffer> buffer = new ABuffer(size);
    ssize_t n = mDataSource->readAt(offset, buffer->data(), buffer->size());

    if (n < (ssize_t)size) {
        return n < 0 ? (status_t)n : ERROR_MALFORMED;
    }

    sp<MetaData> meta = MakeAVCCodecSpecificData(buffer);

    if (meta == NULL) {
        ALOGE("Unable to extract AVC codec specific data");
        return ERROR_MALFORMED;
    }

    int32_t width, height;
    CHECK(meta->findInt32(kKeyWidth, &width));
    CHECK(meta->findInt32(kKeyHeight, &height));

    uint32_t type;
    const void *csd;
    size_t csdSize;
    CHECK(meta->findData(kKeyAVCC, &type, &csd, &csdSize));

    track->mMeta->setInt32(kKeyWidth, width);
    track->mMeta->setInt32(kKeyHeight, height);
    track->mMeta->setData(kKeyAVCC, type, csd, csdSize);

    return OK;
}

status_t AVIExtractor::getSampleInfo(
        size_t trackIndex, size_t sampleIndex,
        off64_t *offset, size_t *size, bool *isKey,
        int64_t *sampleTimeUs) {
    if (trackIndex >= mTracks.size()) {
        return -ERANGE;
    }

    const Track &track = mTracks.itemAt(trackIndex);

    if (sampleIndex >= track.mSamples.size()) {
        return -ERANGE;
    }

    const SampleInfo &info = track.mSamples.itemAt(sampleIndex);

    if (!mOffsetsAreAbsolute) {
        *offset = info.mOffset + mMovieOffset + 8;
    } else {
        *offset = info.mOffset;
    }

    *size = 0;

    uint8_t tmp[8];
    ssize_t n = mDataSource->readAt(*offset, tmp, 8);

    if (n < 8) {
        return n < 0 ? (status_t)n : (status_t)ERROR_MALFORMED;
    }

    uint32_t chunkType = U32_AT(tmp);

    if (!IsCorrectChunkType(trackIndex, track.mKind, chunkType)) {
        return ERROR_MALFORMED;
    }

    *offset += 8;
    *size = U32LE_AT(&tmp[4]);

    *isKey = info.mIsKey;

#ifdef RTK_PLATFORM
    if (indexType == AVI2_TWO_TIERED_INDEX)
    {
        if (track.mKind == Track::VIDEO)
            *sampleTimeUs = ((sampleIndex + track.mStart) * 1000000ll * track.mScale) / track.mRate;
        else if (track.mKind == Track::AUDIO)
        {
            uint32_t nBlockAlign = (uint32_t)track.waveformHeader.nBlockAlign;

            if (((track.mRate * nBlockAlign) / track.mRate) != track.waveformHeader.nAvgBytesPerSec)
            {
                *sampleTimeUs = ((audioFrameCount * 1000000ll) / (int64_t)track.waveformHeader.nAvgBytesPerSec) + ((track.mStart * 1000000ll * track.mScale) / track.mRate);
            }
            else
            {
                *sampleTimeUs = ((audioFrameCount * 1000000ll * track.mScale) / (int64_t)(track.mRate * nBlockAlign)) + ((track.mStart * 1000000ll * track.mScale) / track.mRate);
            }
            audioFrameCount += (int64_t)*size;
        }
    }
    else
    {
        if (track.mBytesPerSample > 0) {
            size_t sampleStartInBytes;
            if (sampleIndex == 0) {
                sampleStartInBytes = 0;
                //if (track.mKind == Track::AUDIO && (track.waveformHeader.wFormatTag == 0x2001)) // DTS
                if (track.mKind == Track::AUDIO)
                    audioFrameCount = 0;
            } else {
                sampleStartInBytes =
                    track.mFirstChunkSize + track.mAvgChunkSize * (sampleIndex - 1);
            }

            sampleIndex = sampleStartInBytes / track.mBytesPerSample;
        }

        //if (track.mKind == Track::AUDIO && (track.waveformHeader.wFormatTag == 0x2001)) // DTS
        if (track.mKind == Track::AUDIO && (track.waveformHeader.wFormatTag != 0x55))
        {
            uint32_t nBlockAlign = (uint32_t)track.waveformHeader.nBlockAlign;

            if (((track.mRate * nBlockAlign) / track.mScale) != track.waveformHeader.nAvgBytesPerSec)
            {
                *sampleTimeUs = ((audioFrameCount * 1000000ll) / (int64_t)track.waveformHeader.nAvgBytesPerSec) + ((track.mStart * 1000000ll * track.mScale) / track.mRate);
            }
            else
            {
                *sampleTimeUs = ((audioFrameCount * 1000000ll * track.mScale) / (int64_t)(track.mRate * nBlockAlign)) + ((track.mStart * 1000000ll * track.mScale) / track.mRate);
            }
            //ALOGD("\033[0;33m [%d]  (%s  -  %d)  audioFrameCount= %d  sampleTimeUs= %lld  track.mRate= %d  track.mScale= %d \033[m", __LINE__, (track.mKind == Track::VIDEO) ? "VIDEO": "AUDIO", (int)sampleIndex, (int)audioFrameCount, (long long)*sampleTimeUs, track.mRate, track.mScale);
            audioFrameCount += (int64_t)*size;
        }
        else
            *sampleTimeUs = (sampleIndex * 1000000ll * track.mScale) / track.mRate;
    }
    //ALOGD("\033[0;36m [%d]  (%s  -  %d)  track.mSamples.size()= %d  sampleTimeUs= %lld  track.mRate= %d  track.mScale= %d \033[m", __LINE__, (track.mKind == Track::VIDEO) ? "VIDEO": "AUDIO", (int)sampleIndex, (int)track.mSamples.size(), (long long)*sampleTimeUs, track.mRate, track.mScale);
#else
    if (track.mBytesPerSample > 0) {
        size_t sampleStartInBytes;
        if (sampleIndex == 0) {
            sampleStartInBytes = 0;
        } else {
            sampleStartInBytes =
                track.mFirstChunkSize + track.mAvgChunkSize * (sampleIndex - 1);
        }

        sampleIndex = sampleStartInBytes / track.mBytesPerSample;
    }

    *sampleTimeUs = (sampleIndex * 1000000ll * track.mRate) / track.mScale;
#endif

    return OK;
}

status_t AVIExtractor::getSampleTime(
        size_t trackIndex, size_t sampleIndex, int64_t *sampleTimeUs) {
    off64_t offset;
    size_t size;
    bool isKey;
    return getSampleInfo(
            trackIndex, sampleIndex, &offset, &size, &isKey, sampleTimeUs);
}

#ifdef RTK_PLATFORM
status_t AVIExtractor::getSampleIndexAtTime(
        size_t trackIndex,
        int64_t timeUs, MediaSource::ReadOptions::SeekMode mode,
        size_t *sampleIndex) {
#else
status_t AVIExtractor::getSampleIndexAtTime(
        size_t trackIndex,
        int64_t timeUs, MediaSource::ReadOptions::SeekMode mode,
        size_t *sampleIndex) const {
#endif
    if (trackIndex >= mTracks.size()) {
        return -ERANGE;
    }

    const Track &track = mTracks.itemAt(trackIndex);

    ssize_t closestSampleIndex;

#ifdef RTK_PLATFORM
    if ((indexType == AVI2_TWO_TIERED_INDEX) || (track.waveformHeader.wFormatTag == 0x2001))
    {
        closestSampleIndex = 0;
        //ALOGD("\033[0;35m [%d]  ===>  SEEK ?! track.mBytesPerSample= %d   trackIndex= %d   mode= %d   timeUs= %lld \033[m", __LINE__, track.mBytesPerSample, trackIndex, mode, timeUs);

        if (track.mKind == Track::VIDEO)
        {
            closestSampleIndex = timeUs / track.mScale * track.mRate / 1000000ll;
        }
        else if (track.mKind == Track::AUDIO)
        {
            int64_t durationUs = 0;

            track.mMeta->findInt64(kKeyDuration, &durationUs);
            closestSampleIndex = (timeUs * track.mSamples.size()) / FiledurationUs;
            //ALOGD("\033[0;35m [%d]  AUDIO:  durationUs= %lld    FiledurationUs= %lld   sample_size= %d   closestSampleIndex= %d \033[m", __LINE__, (long long)durationUs, (long long)FiledurationUs, (int)track.mSamples.size(), (int)closestSampleIndex);
        }
    }
    else
    {
        if (track.mBytesPerSample > 0) {
            size_t closestByteOffset =
                (timeUs * track.mBytesPerSample)
                    / track.mScale * track.mRate / 1000000ll;

            if (closestByteOffset <= track.mFirstChunkSize) {
                closestSampleIndex = 0;
            } else {
                closestSampleIndex =
                    (closestByteOffset - track.mFirstChunkSize)
                        / track.mAvgChunkSize;
            }
        } else {
            // Each chunk contains a single sample.
            closestSampleIndex = timeUs / track.mScale * track.mRate / 1000000ll;
        }
    }
#else
    if (track.mBytesPerSample > 0) {
        size_t closestByteOffset =
            (timeUs * track.mBytesPerSample)
                / track.mRate * track.mScale / 1000000ll;

        if (closestByteOffset <= track.mFirstChunkSize) {
            closestSampleIndex = 0;
        } else {
            closestSampleIndex =
                (closestByteOffset - track.mFirstChunkSize)
                    / track.mAvgChunkSize;
        }
    } else {
        // Each chunk contains a single sample.
        closestSampleIndex = timeUs / track.mRate * track.mScale / 1000000ll;
    }
#endif

    ssize_t numSamples = track.mSamples.size();

    if (closestSampleIndex < 0) {
        closestSampleIndex = 0;
    } else if (closestSampleIndex >= numSamples) {
        closestSampleIndex = numSamples - 1;
    }

    if (mode == MediaSource::ReadOptions::SEEK_CLOSEST) {
        *sampleIndex = closestSampleIndex;

        return OK;
    }

    ssize_t prevSyncSampleIndex = closestSampleIndex;
    while (prevSyncSampleIndex >= 0) {
        const SampleInfo &info =
            track.mSamples.itemAt(prevSyncSampleIndex);

        if (info.mIsKey) {
            break;
        }

        --prevSyncSampleIndex;
    }

    ssize_t nextSyncSampleIndex = closestSampleIndex;
    while (nextSyncSampleIndex < numSamples) {
        const SampleInfo &info =
            track.mSamples.itemAt(nextSyncSampleIndex);

        if (info.mIsKey) {
            break;
        }

        ++nextSyncSampleIndex;
    }

    switch (mode) {
        case MediaSource::ReadOptions::SEEK_PREVIOUS_SYNC:
        {
            *sampleIndex = prevSyncSampleIndex;
#ifdef RTK_PLATFORM
            //if ((track.mKind == Track::AUDIO) && ((indexType == AVI2_TWO_TIERED_INDEX) || (track.waveformHeader.wFormatTag == 0x2001)))
            if ((track.mKind == Track::AUDIO) && (track.waveformHeader.wFormatTag != 0x55))
            {
                audioFrameCount = 0;
                for (size_t i = 0; i < (size_t)prevSyncSampleIndex; i++)
                {
                    const SampleInfo &info = track.mSamples.itemAt(i);
                    audioFrameCount += info.mSize;
                }
            }
#endif

            return prevSyncSampleIndex >= 0 ? OK : UNKNOWN_ERROR;
        }

        case MediaSource::ReadOptions::SEEK_NEXT_SYNC:
        {
            *sampleIndex = nextSyncSampleIndex;

            return nextSyncSampleIndex < numSamples ? OK : UNKNOWN_ERROR;
        }

        case MediaSource::ReadOptions::SEEK_CLOSEST_SYNC:
        {
            if (prevSyncSampleIndex < 0 && nextSyncSampleIndex >= numSamples) {
                return UNKNOWN_ERROR;
            }

            if (prevSyncSampleIndex < 0) {
                *sampleIndex = nextSyncSampleIndex;
                return OK;
            }

            if (nextSyncSampleIndex >= numSamples) {
                *sampleIndex = prevSyncSampleIndex;
                return OK;
            }

            size_t dist1 = closestSampleIndex - prevSyncSampleIndex;
            size_t dist2 = nextSyncSampleIndex - closestSampleIndex;

            *sampleIndex =
                (dist1 < dist2) ? prevSyncSampleIndex : nextSyncSampleIndex;

            return OK;
        }

        default:
            TRESPASS();
            break;
    }
}

bool SniffAVI(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *) {
    char tmp[12];
    if (source->readAt(0, tmp, 12) < 12) {
        return false;
    }

    if (!memcmp(tmp, "RIFF", 4) && !memcmp(&tmp[8], "AVI ", 4)) {
        mimeType->setTo(MEDIA_MIMETYPE_CONTAINER_AVI);

        // Just a tad over the mp3 extractor's confidence, since
        // these .avi files may contain .mp3 content that otherwise would
        // mistakenly lead to us identifying the entire file as a .mp3 file.
        *confidence = 0.21;

        return true;
    }

    return false;
}

}  // namespace android

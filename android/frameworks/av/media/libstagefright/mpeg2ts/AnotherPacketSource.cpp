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
#define LOG_TAG "AnotherPacketSource"

#include "AnotherPacketSource.h"

#include "include/avc_utils.h"

#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/Utils.h>
#include <utils/Vector.h>

#include <inttypes.h>

namespace android {

const int64_t kNearEOSMarkUs = 2000000ll; // 2 secs

AnotherPacketSource::AnotherPacketSource(const sp<MetaData> &meta)
    : mIsAudio(false),
      mIsVideo(false),
      mEnabled(true),
      mFormat(NULL),
      mLastQueuedTimeUs(0),
      mEOSResult(OK),
      mLatestEnqueuedMeta(NULL),
      mLatestDequeuedMeta(NULL)
#ifdef RTK_PLATFORM
      ,mPendingBuffer(0)
      ,mCurBufferSize(0)
      ,mBaseTime(0)
      ,mCalPTS(false)
#endif
      {
    setFormat(meta);

    mDiscontinuitySegments.push_back(DiscontinuitySegment());
}

void AnotherPacketSource::checkInterpolationType(const char *mime)
{
    if(!strcmp(mime, MEDIA_MIMETYPE_VIDEO_MPEG2))
    {
        mCalPTS = true;
    }
}

void AnotherPacketSource::setFormat(const sp<MetaData> &meta) {
    if (mFormat != NULL) {
        // Only allowed to be set once. Requires explicit clear to reset.
        return;
    }

    mIsAudio = false;
    mIsVideo = false;

    if (meta == NULL) {
        return;
    }

    mFormat = meta;
    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    if (!strncasecmp("audio/", mime, 6)) {
        mIsAudio = true;
    } else  if (!strncasecmp("video/", mime, 6)) {
        mIsVideo = true;
        checkInterpolationType(mime);
    } else {
        CHECK(!strncasecmp("text/", mime, 5) || !strncasecmp("application/", mime, 12));
    }
}

AnotherPacketSource::~AnotherPacketSource() {
}

status_t AnotherPacketSource::start(MetaData * /* params */) {
    return OK;
}

status_t AnotherPacketSource::stop() {
    return OK;
}

sp<MetaData> AnotherPacketSource::getFormat() {
    Mutex::Autolock autoLock(mLock);
    if (mFormat != NULL) {
        return mFormat;
    }

    List<sp<ABuffer> >::iterator it = mBuffers.begin();
    while (it != mBuffers.end()) {
        sp<ABuffer> buffer = *it;
        int32_t discontinuity;
        if (!buffer->meta()->findInt32("discontinuity", &discontinuity)) {
            sp<RefBase> object;
            if (buffer->meta()->findObject("format", &object)) {
                setFormat(static_cast<MetaData*>(object.get()));
                return mFormat;
            }
        }

        ++it;
    }
    return NULL;
}

status_t AnotherPacketSource::dequeueAccessUnit(sp<ABuffer> *buffer) {
    buffer->clear();

    Mutex::Autolock autoLock(mLock);
    while (mEOSResult == OK && mBuffers.empty()) {
        mCondition.wait(mLock);
    }

    if (!mBuffers.empty()) {
        *buffer = *mBuffers.begin();
        mBuffers.erase(mBuffers.begin());
#if RTK_PLATFORM
        if(mCurBufferSize > (*buffer)->size())
            mCurBufferSize = mCurBufferSize - (*buffer)->size();
        else
            mCurBufferSize = 0;
#endif
        int32_t discontinuity;
        if ((*buffer)->meta()->findInt32("discontinuity", &discontinuity)) {
            if (wasFormatChange(discontinuity)) {
                mFormat.clear();
            }

            mDiscontinuitySegments.erase(mDiscontinuitySegments.begin());
            // CHECK(!mDiscontinuitySegments.empty());
            return INFO_DISCONTINUITY;
        }

        // CHECK(!mDiscontinuitySegments.empty());
        DiscontinuitySegment &seg = *mDiscontinuitySegments.begin();

        int64_t timeUs;
        mLatestDequeuedMeta = (*buffer)->meta()->dup();
        CHECK(mLatestDequeuedMeta->findInt64("timeUs", &timeUs));
        if (timeUs > seg.mMaxDequeTimeUs) {
            seg.mMaxDequeTimeUs = timeUs;
        }

        sp<RefBase> object;
        if ((*buffer)->meta()->findObject("format", &object)) {
            setFormat(static_cast<MetaData*>(object.get()));
        }

        return OK;
    }

    return mEOSResult;
}

void AnotherPacketSource::requeueAccessUnit(const sp<ABuffer> &buffer) {
    // TODO: update corresponding book keeping info.
    Mutex::Autolock autoLock(mLock);
    mBuffers.push_front(buffer);
}

status_t AnotherPacketSource::read(
        MediaBuffer **out, const ReadOptions *) {
    *out = NULL;

    Mutex::Autolock autoLock(mLock);
    while (mEOSResult == OK && mBuffers.empty()) {
        mCondition.wait(mLock);
    }

    if (!mBuffers.empty()) {

        const sp<ABuffer> buffer = *mBuffers.begin();
        mBuffers.erase(mBuffers.begin());
#ifdef RTK_PLATFORM
        mCurBufferSize = mCurBufferSize - buffer->size();
#endif
        int32_t discontinuity;
        if (buffer->meta()->findInt32("discontinuity", &discontinuity)) {
            if (wasFormatChange(discontinuity)) {
                mFormat.clear();
            }

            mDiscontinuitySegments.erase(mDiscontinuitySegments.begin());
            // CHECK(!mDiscontinuitySegments.empty());
            buffer->meta()->setInt32("discontinuity", static_cast<int32_t>(ATSParser::DISCONTINUITY_TIME));
            MediaBuffer *mediaBuffer = new MediaBuffer(buffer);
            *out = mediaBuffer;
            return INFO_DISCONTINUITY;
        }

        mLatestDequeuedMeta = buffer->meta()->dup();

        sp<RefBase> object;
        if (buffer->meta()->findObject("format", &object)) {
            setFormat(static_cast<MetaData*>(object.get()));
        }

        int64_t timeUs;
        CHECK(buffer->meta()->findInt64("timeUs", &timeUs));
        // CHECK(!mDiscontinuitySegments.empty());
        DiscontinuitySegment &seg = *mDiscontinuitySegments.begin();
        if (timeUs > seg.mMaxDequeTimeUs) {
            seg.mMaxDequeTimeUs = timeUs;
        }

        MediaBuffer *mediaBuffer = new MediaBuffer(buffer);
#ifdef RTK_PLATFORM
        if(mBaseTime > 0)
        {
            if(timeUs < mBaseTime)
                timeUs = 0;
            else
                timeUs = timeUs - mBaseTime;
        }

#endif
        mediaBuffer->meta_data()->setInt64(kKeyTime, timeUs);
        int32_t isSync;
        if (buffer->meta()->findInt32("isSync", &isSync)) {
            mediaBuffer->meta_data()->setInt32(kKeyIsSyncFrame, isSync);
        }

        sp<ABuffer> sei;
        if (buffer->meta()->findBuffer("sei", &sei) && sei != NULL) {
            mediaBuffer->meta_data()->setData(kKeySEI, 0, sei->data(), sei->size());
        }

        sp<ABuffer> mpegUserData;
        if (buffer->meta()->findBuffer("mpegUserData", &mpegUserData) && mpegUserData != NULL) {
            mediaBuffer->meta_data()->setData(
                    kKeyMpegUserData, 0, mpegUserData->data(), mpegUserData->size());
        }

        *out = mediaBuffer;
        return OK;
    }

    return mEOSResult;
}

bool AnotherPacketSource::wasFormatChange(
        int32_t discontinuityType) const {
    if (mIsAudio) {
        return (discontinuityType & ATSParser::DISCONTINUITY_AUDIO_FORMAT) != 0;
    }

    if (mIsVideo) {
        return (discontinuityType & ATSParser::DISCONTINUITY_VIDEO_FORMAT) != 0;
    }

    return false;
}

void AnotherPacketSource::queueAccessUnit(const sp<ABuffer> &buffer) {
    int32_t damaged;
    if (buffer->meta()->findInt32("damaged", &damaged) && damaged) {
        // LOG(VERBOSE) << "discarding damaged AU";
        return;
    }

    Mutex::Autolock autoLock(mLock);
    mBuffers.push_back(buffer);
    mCondition.signal();

    int32_t discontinuity;
    if (buffer->meta()->findInt32("discontinuity", &discontinuity)){
        ALOGV("queueing a discontinuity with queueAccessUnit");

        mLastQueuedTimeUs = 0ll;
        mEOSResult = OK;
        mLatestEnqueuedMeta = NULL;

        mDiscontinuitySegments.push_back(DiscontinuitySegment());
        return;
    }

    int64_t lastQueuedTimeUs;
    CHECK(buffer->meta()->findInt64("timeUs", &lastQueuedTimeUs));
    mLastQueuedTimeUs = lastQueuedTimeUs;
    ALOGV("queueAccessUnit timeUs=%" PRIi64 " us (%.2f secs)",
            mLastQueuedTimeUs, mLastQueuedTimeUs / 1E6);

    if(mCalPTS && mBuffers.size() > 1 && mLastQueuedTimeUs != 0)
    {
        List<sp<ABuffer> >::iterator it = mBuffers.end();
        it--;
        int32_t timeOffset = 0;
        for (; it != mBuffers.begin(); it--) {
            timeOffset++;
            int64_t tmpTime;
            if ((*it)->meta()->findInt64("timeUs", &tmpTime) && tmpTime != 0) {
                if(timeOffset != 1)
                {
                    int64_t eachGap = (mLastQueuedTimeUs - tmpTime)/timeOffset;
                    ALOGV("find !! tmpTime %lld timeUs %lld offset %d timeGap %lld ", (long long)tmpTime/1000, (long long)mLastQueuedTimeUs/1000, timeOffset, (long long)eachGap);
                    for(int32_t i=1; i<(timeOffset-1); i++)
                    {
                        it++;
                        int64_t newTime = tmpTime + (i*eachGap);
                        (*it)->meta()->setInt64("timeUs", newTime);
                    }
                    break;
                }
            }
        }

    }
    // CHECK(!mDiscontinuitySegments.empty());
    DiscontinuitySegment &tailSeg = *(--mDiscontinuitySegments.end());
    if (lastQueuedTimeUs > tailSeg.mMaxEnqueTimeUs) {
        tailSeg.mMaxEnqueTimeUs = lastQueuedTimeUs;
    }
#ifdef RTK_PLATFORM
    else if(lastQueuedTimeUs < (tailSeg.mMaxEnqueTimeUs - 1*1E6) && tailSeg.mMaxDequeTimeUs < tailSeg.mMaxEnqueTimeUs && tailSeg.mMaxDequeTimeUs < lastQueuedTimeUs)
    {
        ALOGD("[%s][%d] update mMaxEnqueTimeUs::%f to %f, mMaxDequeTimeUs::%f\n",__FUNCTION__,__LINE__, (double)tailSeg.mMaxEnqueTimeUs/1E6,(double)lastQueuedTimeUs/1E6, (double)tailSeg.mMaxDequeTimeUs/1E6);
        tailSeg.mMaxEnqueTimeUs = lastQueuedTimeUs;
    }
#endif
    if (tailSeg.mMaxDequeTimeUs == -1) {
        tailSeg.mMaxDequeTimeUs = lastQueuedTimeUs;
    }

    if (mLatestEnqueuedMeta == NULL) {
        mLatestEnqueuedMeta = buffer->meta()->dup();
    } else {
        int64_t latestTimeUs = 0;
        int64_t frameDeltaUs = 0;
        CHECK(mLatestEnqueuedMeta->findInt64("timeUs", &latestTimeUs));
        if (lastQueuedTimeUs > latestTimeUs) {
            mLatestEnqueuedMeta = buffer->meta()->dup();
            frameDeltaUs = lastQueuedTimeUs - latestTimeUs;
            mLatestEnqueuedMeta->setInt64("durationUs", frameDeltaUs);
        } else if (!mLatestEnqueuedMeta->findInt64("durationUs", &frameDeltaUs)) {
            // For B frames
            frameDeltaUs = latestTimeUs - lastQueuedTimeUs;
            mLatestEnqueuedMeta->setInt64("durationUs", frameDeltaUs);
        }
    }
#ifdef RTK_PLATFORM
    if(mLatestDequeuedMeta == NULL)
        mPendingBuffer = mPendingBuffer + buffer->size();
    mCurBufferSize = mCurBufferSize + buffer->size();
    ALOGV("%s mCurBufferSize %zu ", mIsAudio?"Audio":"Video", mCurBufferSize/1024);
#endif
}

void AnotherPacketSource::clear() {
    Mutex::Autolock autoLock(mLock);

    mBuffers.clear();
    mCurBufferSize = 0;
    mEOSResult = OK;

    mDiscontinuitySegments.clear();
    mDiscontinuitySegments.push_back(DiscontinuitySegment());

    mFormat = NULL;
    mLatestEnqueuedMeta = NULL;
}

void AnotherPacketSource::queueDiscontinuity(
        ATSParser::DiscontinuityType type,
        const sp<AMessage> &extra,
        bool discard) {
    Mutex::Autolock autoLock(mLock);
#ifdef RTK_PLATFORM
    if(type == ATSParser::DISCONTINUITY_TIME) {
    if (!mBuffers.empty()) {
            mBuffers.clear();
            mCurBufferSize = 0;
        }
        return;
    }
#endif

    if (discard) {
        // Leave only discontinuities in the queue.
        List<sp<ABuffer> >::iterator it = mBuffers.begin();
        while (it != mBuffers.end()) {
            sp<ABuffer> oldBuffer = *it;

            int32_t oldDiscontinuityType;
            if (!oldBuffer->meta()->findInt32(
                        "discontinuity", &oldDiscontinuityType)) {
                it = mBuffers.erase(it);
                continue;
            }

            ++it;
        }

        for (List<DiscontinuitySegment>::iterator it2 = mDiscontinuitySegments.begin();
                it2 != mDiscontinuitySegments.end();
                ++it2) {
            DiscontinuitySegment &seg = *it2;
            seg.clear();
        }

    }

    mEOSResult = OK;
    mLastQueuedTimeUs = 0;
    mLatestEnqueuedMeta = NULL;

    if (type == ATSParser::DISCONTINUITY_NONE) {
        return;
    }

    mDiscontinuitySegments.push_back(DiscontinuitySegment());

    sp<ABuffer> buffer = new ABuffer(0);
    buffer->meta()->setInt32("discontinuity", static_cast<int32_t>(type));
    buffer->meta()->setMessage("extra", extra);

    mBuffers.push_back(buffer);
    mCondition.signal();
}

void AnotherPacketSource::signalEOS(status_t result) {
    CHECK(result != OK);

    Mutex::Autolock autoLock(mLock);
    mEOSResult = result;
    mCondition.signal();
}

bool AnotherPacketSource::hasBufferAvailable(status_t *finalResult) {
    Mutex::Autolock autoLock(mLock);
    *finalResult = OK;
    if (!mEnabled) {
        return false;
    }
    if (!mBuffers.empty()) {
        return true;
    }

    *finalResult = mEOSResult;
    return false;
}

bool AnotherPacketSource::hasDataBufferAvailable(status_t *finalResult) {
    Mutex::Autolock autoLock(mLock);
    *finalResult = OK;
    if (!mEnabled) {
        return false;
    }
    List<sp<ABuffer> >::iterator it;
    for (it = mBuffers.begin(); it != mBuffers.end(); it++) {
        int32_t discontinuity;
        if (!(*it)->meta()->findInt32("discontinuity", &discontinuity)) {
            return true;
        }
    }

    *finalResult = mEOSResult;
    return false;
}

size_t AnotherPacketSource::getAvailableBufferCount(status_t *finalResult) {
    Mutex::Autolock autoLock(mLock);

    *finalResult = OK;
    if (!mEnabled) {
        return 0;
    }
    if (!mBuffers.empty()) {
        return mBuffers.size();
    }
    *finalResult = mEOSResult;
    return 0;
}

int64_t AnotherPacketSource::getBufferedDurationUs(status_t *finalResult) {
    Mutex::Autolock autoLock(mLock);
    *finalResult = mEOSResult;

    int64_t durationUs = 0;
    for (List<DiscontinuitySegment>::iterator it = mDiscontinuitySegments.begin();
            it != mDiscontinuitySegments.end();
            ++it) {
        const DiscontinuitySegment &seg = *it;
        // dequeued access units should be a subset of enqueued access units
        // CHECK(seg.maxEnqueTimeUs >= seg.mMaxDequeTimeUs);
        durationUs += (seg.mMaxEnqueTimeUs - seg.mMaxDequeTimeUs);
    }
#ifdef RTK_PLATFORM
    if(durationUs < 0)
        durationUs = 0;
#endif

    return durationUs;
}

status_t AnotherPacketSource::nextBufferTime(int64_t *timeUs) {
    *timeUs = 0;

    Mutex::Autolock autoLock(mLock);

    if (mBuffers.empty()) {
        return mEOSResult != OK ? mEOSResult : -EWOULDBLOCK;
    }

    sp<ABuffer> buffer = *mBuffers.begin();
    CHECK(buffer->meta()->findInt64("timeUs", timeUs));

    return OK;
}

bool AnotherPacketSource::isFinished(int64_t duration) const {
    if (duration > 0) {
        int64_t diff = duration - mLastQueuedTimeUs;
        if (diff < kNearEOSMarkUs && diff > -kNearEOSMarkUs) {
            ALOGV("Detecting EOS due to near end");
            return true;
        }
    }
    return (mEOSResult != OK);
}

sp<AMessage> AnotherPacketSource::getLatestEnqueuedMeta() {
    Mutex::Autolock autoLock(mLock);
    return mLatestEnqueuedMeta;
}

sp<AMessage> AnotherPacketSource::getLatestDequeuedMeta() {
    Mutex::Autolock autoLock(mLock);
    return mLatestDequeuedMeta;
}

void AnotherPacketSource::enable(bool enable) {
    Mutex::Autolock autoLock(mLock);
    mEnabled = enable;
}

/*
 * returns the sample meta that's delayUs after queue head
 * (NULL if such sample is unavailable)
 */
sp<AMessage> AnotherPacketSource::getMetaAfterLastDequeued(int64_t delayUs) {
    Mutex::Autolock autoLock(mLock);
    int64_t firstUs = -1;
    int64_t lastUs = -1;
    int64_t durationUs = 0;

    List<sp<ABuffer> >::iterator it;
    for (it = mBuffers.begin(); it != mBuffers.end(); ++it) {
        const sp<ABuffer> &buffer = *it;
        int32_t discontinuity;
        if (buffer->meta()->findInt32("discontinuity", &discontinuity)) {
            durationUs += lastUs - firstUs;
            firstUs = -1;
            lastUs = -1;
            continue;
        }
        int64_t timeUs;
        if (buffer->meta()->findInt64("timeUs", &timeUs)) {
            if (firstUs < 0) {
                firstUs = timeUs;
            }
            if (lastUs < 0 || timeUs > lastUs) {
                lastUs = timeUs;
            }
            if (durationUs + (lastUs - firstUs) >= delayUs) {
                return buffer->meta();
            }
        }
    }
    return NULL;
}

/*
 * removes samples with time equal or after meta
 */
void AnotherPacketSource::trimBuffersAfterMeta(
        const sp<AMessage> &meta) {
    if (meta == NULL) {
        ALOGW("trimming with NULL meta, ignoring");
        return;
    }

    Mutex::Autolock autoLock(mLock);
    if (mBuffers.empty()) {
        return;
    }

    HLSTime stopTime(meta);
    ALOGV("trimBuffersAfterMeta: discontinuitySeq %d, timeUs %lld",
            stopTime.mSeq, (long long)stopTime.mTimeUs);

    List<sp<ABuffer> >::iterator it;
    List<DiscontinuitySegment >::iterator it2;
    sp<AMessage> newLatestEnqueuedMeta = NULL;
    int64_t newLastQueuedTimeUs = 0;
    for (it = mBuffers.begin(), it2 = mDiscontinuitySegments.begin(); it != mBuffers.end(); ++it) {
        const sp<ABuffer> &buffer = *it;
        int32_t discontinuity;
        if (buffer->meta()->findInt32("discontinuity", &discontinuity)) {
            // CHECK(it2 != mDiscontinuitySegments.end());
            ++it2;
            continue;
        }

        HLSTime curTime(buffer->meta());
        if (!(curTime < stopTime)) {
            ALOGV("trimming from %lld (inclusive) to end",
                    (long long)curTime.mTimeUs);
            break;
        }
        newLatestEnqueuedMeta = buffer->meta();
        newLastQueuedTimeUs = curTime.mTimeUs;
    }

    mBuffers.erase(it, mBuffers.end());
    mLatestEnqueuedMeta = newLatestEnqueuedMeta;
    mLastQueuedTimeUs = newLastQueuedTimeUs;

    DiscontinuitySegment &seg = *it2;
    if (newLatestEnqueuedMeta != NULL) {
        seg.mMaxEnqueTimeUs = newLastQueuedTimeUs;
    } else {
        seg.clear();
    }
    mDiscontinuitySegments.erase(++it2, mDiscontinuitySegments.end());
}

/*
 * removes samples with time equal or before meta;
 * returns first sample left in the queue.
 *
 * (for AVC, if trim happens, the samples left will always start
 * at next IDR.)
 */
sp<AMessage> AnotherPacketSource::trimBuffersBeforeMeta(
        const sp<AMessage> &meta) {
    HLSTime startTime(meta);
    ALOGV("trimBuffersBeforeMeta: discontinuitySeq %d, timeUs %lld",
            startTime.mSeq, (long long)startTime.mTimeUs);

    sp<AMessage> firstMeta;
    int64_t firstTimeUs = -1;
    Mutex::Autolock autoLock(mLock);
    if (mBuffers.empty()) {
        return NULL;
    }

    sp<MetaData> format;
    bool isAvc = false;

    List<sp<ABuffer> >::iterator it;
    for (it = mBuffers.begin(); it != mBuffers.end(); ++it) {
        const sp<ABuffer> &buffer = *it;
        int32_t discontinuity;
        if (buffer->meta()->findInt32("discontinuity", &discontinuity)) {
            mDiscontinuitySegments.erase(mDiscontinuitySegments.begin());
            // CHECK(!mDiscontinuitySegments.empty());
            format = NULL;
            isAvc = false;
            continue;
        }
        if (format == NULL) {
            sp<RefBase> object;
            if (buffer->meta()->findObject("format", &object)) {
                const char* mime;
                format = static_cast<MetaData*>(object.get());
                isAvc = format != NULL
                        && format->findCString(kKeyMIMEType, &mime)
                        && !strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC);
            }
        }
        if (isAvc && !IsIDR(buffer)) {
            continue;
        }

        HLSTime curTime(buffer->meta());
        if (startTime < curTime) {
            ALOGV("trimming from beginning to %lld (not inclusive)",
                    (long long)curTime.mTimeUs);
            firstMeta = buffer->meta();
            firstTimeUs = curTime.mTimeUs;
            break;
        }
    }
    mBuffers.erase(mBuffers.begin(), it);
    mLatestDequeuedMeta = NULL;

    // CHECK(!mDiscontinuitySegments.empty());
    DiscontinuitySegment &seg = *mDiscontinuitySegments.begin();
    if (firstTimeUs >= 0) {
        seg.mMaxDequeTimeUs = firstTimeUs;
    } else {
        seg.clear();
    }

    return firstMeta;
}

#ifdef RTK_PLATFORM
size_t AnotherPacketSource::getPendingBufferSize() {
    return mPendingBuffer;
}

size_t AnotherPacketSource::getCurBufferSize() {
    if(mLatestDequeuedMeta == NULL)
        return 0;
    else
        return mCurBufferSize;
}

void AnotherPacketSource::setBaseTimeUs(int64_t timeUs) {
    mBaseTime = timeUs;
}

void AnotherPacketSource::setDiscTimeUs(int64_t timeUs)
{
    Mutex::Autolock autoLock(mLock);
    for (List<DiscontinuitySegment>::iterator it = mDiscontinuitySegments.begin();
            it != mDiscontinuitySegments.end();
            ++it)
    {
        DiscontinuitySegment &seg = *it;
        ALOGV("[%s][%d] set %f to %f , %f \n",__FUNCTION__,__LINE__,(double)seg.mMaxDequeTimeUs/1E6,(double)timeUs/1E6,(double)seg.mMaxEnqueTimeUs/1E6);
        seg.mMaxDequeTimeUs = timeUs;
    }
}

bool AnotherPacketSource::isVideoSource()
{
    if(mIsVideo)
        return true;
    else
        return false;
}
#endif

}  // namespace android

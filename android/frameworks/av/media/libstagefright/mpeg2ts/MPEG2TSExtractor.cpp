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
#define LOG_TAG "MPEG2TSExtractor"

#include <inttypes.h>
#include <utils/Log.h>

#include "include/MPEG2TSExtractor.h"
#include "include/NuCachedSource2.h"

#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/IStreamSource.h>
#include <utils/String8.h>
#ifdef RTK_PLATFORM
#include <cutils/properties.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <pthread.h>
#endif

#include "AnotherPacketSource.h"
#include "ATSParser.h"

namespace android {

static const size_t kTSPacketSize = 188;

#ifdef RTK_PLATFORM
static const size_t kMTSPacketSize = 192;
static const size_t kFECPacketSize = 204;
static size_t kIOReadNumPacket = 256;
static size_t kSearchValidSyncWordOffset = 0x3000;
#endif
struct MPEG2TSSource : public MediaSource {
    MPEG2TSSource(
            const sp<MPEG2TSExtractor> &extractor,
            const sp<AnotherPacketSource> &impl,
            bool doesSeek);

    virtual status_t start(MetaData *params = NULL);
    virtual status_t stop();
    virtual sp<MetaData> getFormat();

    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options = NULL);

private:
    sp<MPEG2TSExtractor> mExtractor;
    sp<AnotherPacketSource> mImpl;

    // If there are both audio and video streams, only the video stream
    // will signal seek on the extractor; otherwise the single stream will seek.
    bool mDoesSeek;

    DISALLOW_EVIL_CONSTRUCTORS(MPEG2TSSource);
};

MPEG2TSSource::MPEG2TSSource(
        const sp<MPEG2TSExtractor> &extractor,
        const sp<AnotherPacketSource> &impl,
        bool doesSeek)
    : mExtractor(extractor),
      mImpl(impl),
      mDoesSeek(doesSeek) {

}

status_t MPEG2TSSource::start(MetaData *params) {
    return mImpl->start(params);
}

status_t MPEG2TSSource::stop() {
    return mImpl->stop();
}

sp<MetaData> MPEG2TSSource::getFormat() {
#ifdef RTK_PLATFORM
    sp<MetaData> meta = mImpl->getFormat();
    if(meta == NULL)
        return NULL;
    meta->setInt64(kKeyDuration, mExtractor->getDurationUs());
    return meta;
#else
    return mImpl->getFormat();
#endif
}

status_t MPEG2TSSource::read(
        MediaBuffer **out, const ReadOptions *options) {
    *out = NULL;

    int64_t seekTimeUs;
    ReadOptions::SeekMode seekMode;
    if (mDoesSeek && options && options->getSeekTo(&seekTimeUs, &seekMode)) {
        // seek is needed
        status_t err = OK;
#ifdef RTK_PLATFORM
        if(mExtractor->mStopThread == true)
            return ERROR_UNSUPPORTED;
        mExtractor->seekTo(seekTimeUs);
        int seekWaitTimeout = 5;
        while(mExtractor->mSeeking == true && seekWaitTimeout > 0) {
            sleep(1);
            seekWaitTimeout--;
        }

        if(mExtractor->mSeeking == true)
        {
            ALOGD("Wait seek timeout, seek to head");
            mExtractor->seekToHead();
        }
#else
        err = mExtractor->seek(seekTimeUs, seekMode);
#endif
        if (err != OK) {
            return err;
        }
    }

#ifdef RTK_PLATFORM
    status_t err = mExtractor->feedUntilBufferAvailable(mImpl);
    if(err != OK)
        return err;
#else
    if (mExtractor->feedUntilBufferAvailable(mImpl) != OK) {
        return ERROR_END_OF_STREAM;
    }
#endif

    return mImpl->read(out, options);
}

////////////////////////////////////////////////////////////////////////////////

MPEG2TSExtractor::MPEG2TSExtractor(const sp<DataSource> &source)
    : mDataSource(source),
      mParser(new ATSParser),
      mLastSyncEvent(0),
      mOffset(0)
#ifdef RTK_PLATFORM
      ,mRemainder(kIOReadNumPacket)
      ,mRealPacketSize(kTSPacketSize)
      ,mIOReadSize(kTSPacketSize*kIOReadNumPacket)
      ,mReadTimeOutCount(100)
      ,mStopThread(false)
      ,mDurationUs(0)
      ,mFileSize(0)
      ,mSeekTimeUs(0)
      ,mSeeking(false)
      ,mMaxcount(0)
      ,mSeekingOffset(0)
      ,mMinOffset(0)
      ,mMaxOffset(0)
      ,mBaseTime(-1)
      ,mStartOffset(0)
      ,mSeekSection(50000)
      ,mSeekCount(13)
      ,mIsAudioOnly(false)
#endif
      {
    init();
}

#ifdef RTK_PLATFORM
MPEG2TSExtractor::~MPEG2TSExtractor() {
    mStopThread = true;
    void *dummy = 0;
    pthread_join(mThread, &dummy);
}
#endif

size_t MPEG2TSExtractor::countTracks() {
    return mSourceImpls.size();
}

sp<IMediaSource> MPEG2TSExtractor::getTrack(size_t index) {
    if (index >= mSourceImpls.size()) {
        return NULL;
    }

    // The seek reference track (video if present; audio otherwise) performs
    // seek requests, while other tracks ignore requests.
    return new MPEG2TSSource(this, mSourceImpls.editItemAt(index),
#ifdef RTK_PLATFORM
            mIsAudioOnly?true:mSourceImpls.editItemAt(index)->isVideoSource());
#else
            (mSeekSyncPoints == &mSyncPoints.editItemAt(index)));
#endif
}

sp<MetaData> MPEG2TSExtractor::getTrackMetaData(
        size_t index, uint32_t /* flags */) {
#if RTK_PLATFORM
    if (index >= mSourceImpls.size())
        return NULL;
    sp<MetaData> meta = mSourceImpls.editItemAt(index)->getFormat();
    if (meta != NULL && mDurationUs > 0) {
        meta->setInt64(kKeyDuration, mDurationUs);
    }
#endif
    return index < mSourceImpls.size()
        ? mSourceImpls.editItemAt(index)->getFormat() : NULL;
}

sp<MetaData> MPEG2TSExtractor::getMetaData() {
    sp<MetaData> meta = new MetaData;
    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_CONTAINER_MPEG2TS);

    return meta;
}

#ifdef RTK_PLATFORM
off64_t MPEG2TSExtractor::checkRealPacketLength() {
    bool checkResult_MTS = true;
    bool checkResult_FEC = true;
    size_t sync_offset = 0;
    size_t searchCount_MTS = 0;
    size_t searchCount_FEC = 0;
    char header;
    for (int i = 0; i < 5; ++i) {
        for (; sync_offset < kSearchValidSyncWordOffset; ++sync_offset) {
            char sync;
            if (mDataSource->readAt(sync_offset, &sync, 1) == 1 && sync == 0x47) {
                ALOGD("checkRealPacketLength find sync word offset %zu ", sync_offset);
                break;
            }
        }
        if (mDataSource->readAt(sync_offset + (kMTSPacketSize * i), &header, 1) != 1 || header != 0x47) {
            searchCount_MTS++;
            if(searchCount_MTS > 1) {
                //ALOGD("checkResult_MTS false");
                checkResult_MTS = false;
            }
            sync_offset++;
        }
    }
    if(checkResult_MTS == true) {//MTS case
        mRealPacketSize = kMTSPacketSize;
        mIOReadSize = kIOReadNumPacket * kMTSPacketSize;
        sync_offset -= 4;
        //ALOGD("mRealPacketSize = kMTSPacketSize ");
        return sync_offset;
    }else{
        sync_offset = 0;
        for (; sync_offset < kSearchValidSyncWordOffset; ++sync_offset) {
            char sync;
            if (mDataSource->readAt(sync_offset, &sync, 1) == 1 && sync == 0x47) {
                ALOGD("check leangth sniff find sync word offset %zu ", sync_offset);
                break;
            }
        }
        for (int i = 0; i < 5; ++i) {
            if(mDataSource->readAt(sync_offset + (kFECPacketSize * i), &header, 1) != 1 || header != 0x47) {
                searchCount_FEC++;
                if(searchCount_FEC > 1) {
                    //ALOGD("checkResult_FEC false");
                    checkResult_FEC = false;
                }
            }
        }
         if(checkResult_FEC == true) {
            //FEC case
            mRealPacketSize = kFECPacketSize;
            mIOReadSize = kIOReadNumPacket * kFECPacketSize;
            //ALOGD("mRealPacketSize = kFECPacketSize ");
            return sync_offset;
        }
    }

    return 0;//TS case
}

void *MPEG2TSExtractor::ThreadWrapper(void *me)
{
    ALOGD("MPEG2TSExtractor::threadWrapper");
    setpriority(PRIO_PROCESS, 0, ANDROID_PRIORITY_NORMAL);
    static_cast<MPEG2TSExtractor *>(me)->ThreadEntry();
    return NULL;
}

void MPEG2TSExtractor::ThreadEntry()
{
    prctl(PR_SET_NAME, (unsigned long)"MPEG2TSExtractorThread", 0, 0, 0);
    int t_pid = 0;
    int pri = getpriority(PRIO_PROCESS, t_pid);
    status_t status = OK;
    ALOGD("MPEG2TSExtractor::ThreadEntry ,priority = %d,pid=%d", pri, getpid());
    sp<AnotherPacketSource> impl = NULL;
    usleep(3000000);//Waiting for player ready, otherwise parser may clear pending source
    if(mIsAudioOnly)
    {
        const sp<AnotherPacketSource> &tmpImpl = mSourceImpls[0];
        impl = tmpImpl;
    }
    else
    {
        const sp<AnotherPacketSource> tmpImpl = (AnotherPacketSource *)mParser->getSource(ATSParser::VIDEO).get();
        impl = tmpImpl;
    }

    if(impl == NULL)
        return;

    int64_t bufferingTimeUs = 0;
    size_t trackCount = countTracks();
    if(trackCount >= 8)
        bufferingTimeUs = 1000000;
    else
        bufferingTimeUs = (8 - trackCount) * 1000000;

    ALOGD("Set bufferingTime %" PRId64 " sec", bufferingTimeUs/1000000);

    while (!mStopThread)
    {
        if(impl->getFormat() == NULL)
        {
            ALOGV("wait source format ready ");
            usleep(100000);
            continue;
        }
        int64_t duration = impl->getBufferedDurationUs(&status);
        if(duration < bufferingTimeUs || mSeeking == true)
        {
            if(duration < 1000000)
                ALOGV("low water level %" PRId64, duration);
            status = feedMore();
        }
        else
            usleep(1000);
        //if(status != OK)
        //    break;
    }
    mStopThread = true;
    return;
}

bool findSyncWord(const sp<DataSource> &source,off64_t StartOffset, uint64_t size, size_t PacketSize, off64_t &NewOffset) {

    uint8_t packet[PacketSize];
    off64_t Offset = StartOffset;

    size_t n = source->readAt(Offset, packet, PacketSize);
    if(n < PacketSize)
        ALOGE("%s source only read size %zu", __FUNCTION__, n);

    if(((PacketSize == kTSPacketSize) && packet[0] != 0x47) ||
        ((PacketSize == kMTSPacketSize) && packet[4] != 0x47)||((PacketSize == kFECPacketSize) && packet[0] != 0x47)){
        uint8_t packetTempS[PacketSize*3];
        size_t index = 0;
        for (;Offset < (off64_t)(StartOffset + size - 3*PacketSize);) {

            Offset = Offset + PacketSize;
            size_t n = source->readAt(Offset, packetTempS, PacketSize*3);
            if(n < PacketSize*3)
                ALOGE("%s source only read size %zu", __FUNCTION__, n);
            for (index = 0; index <  PacketSize; index++) {
                if((packetTempS[index] == 0x47) && (packetTempS[index+ PacketSize] == 0x47) && (packetTempS[index+ PacketSize*2] == 0x47)){
                    break;
                    }
            }
            if(index >= PacketSize){
                ALOGE("%s can not find sync word", __FUNCTION__);
            }else if (index > 0) {
                if(PacketSize == kTSPacketSize||PacketSize == kFECPacketSize) {
                    NewOffset = Offset + index + 2 * PacketSize;
                }else {
                    NewOffset = Offset + index - 4 + 2 * PacketSize;
                }
                return true;
            }
        }
        ALOGE("%s can not find sync word", __FUNCTION__);
    }else {
        return true;
    }
    return false;
}

status_t MPEG2TSExtractor::parseMaxPTS() {
    off64_t counts = mFileSize / mRealPacketSize;
    int64_t maxPTSStart = systemTime() / 1000;
    mParser->setDequeueState(false);
    bool foundFirstPTS = false;
    while (feedMore() == OK) {
        if (mParser->getMaxPTS() > 0) {
            ALOGD("parseMaxPTS:firstPTSIsValid, mOffset %" PRIu64, mOffset);
            foundFirstPTS = true;
            break;
        }
    }
    if (!foundFirstPTS) {
        ALOGI("not found first PTS");
        return OK;
    }
    //clear
    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_SEEK, NULL);
    //get duration
    mParser->setEnableTimestamp(false);
    int64_t timeOutUs  = 3000000LL;
    for (off64_t i = 1; i <= counts; i++) {
        int64_t maxPTSDuration = systemTime() / 1000 - maxPTSStart;
        if (maxPTSDuration > timeOutUs) {
            ALOGD("Find MAX PTS timeout !!");
            return UNKNOWN_ERROR;
        }

        if(mFileSize > (off64_t)(5000 * i * i * mRealPacketSize)){
            mFileSize = (off64_t)(mFileSize - 5000 * i * i * mRealPacketSize);
        }else {
            mFileSize = 0;
        }

        mOffset = mFileSize;
        if (findSyncWord(mDataSource,mFileSize,1000 * mRealPacketSize, mRealPacketSize, mOffset)) {//find last PAT
            ALOGD("findSyncWord %" PRIu64 " mFileSize %" PRIu64 "\n", mOffset, mFileSize);
            mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
            while (feedMore() == OK) {
                if(((mOffset - mFileSize) > (off64_t)(10000 * mRealPacketSize)) && (mParser->getMaxPTS() == 0)) {
                    ALOGD("stop feedmore (no PES) mOffset=%" PRIu64 " mFileSize=%" PRIu64 "\n", mOffset, mFileSize);
                    break;
                 }
            }
            mDurationUs = mParser->getMaxPTS();
            if (mDurationUs)
                break;
            }
    }
    mParser->setEnableTimestamp(true);
    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_SEEK, NULL);
    mParser->setDequeueState(true);
    ALOGD("%s getMaxPTS->mDurationUs:%" PRId64 "\n", __FUNCTION__, mDurationUs);
    return OK;
}

void MPEG2TSExtractor::resetSeekSectionAndCount() {
    if (mDataSource->flags() & DataSource::kIsCachingDataSource) {
        mSeekCount = 3;
        if(mDurationUs<=1800000000){ //duration <=30min mSeekSection = 1s
            mSeekSection = 1000000;
        }else if(1800000000<mDurationUs&&mDurationUs<=3600000000){ // 30min<duration <=60min mSeekSection = 2s
            mSeekSection = 2000000;
        }else if(3600000000<mDurationUs&&mDurationUs<=5400000000){ // 60min <duration <=90min mSeekSection = 3s
            mSeekSection = 3000000;
        }else{    // 90 <duration   mSeekSection = 4s
            mSeekSection = 4000000;
        }
        //ALOGD("Http playback and reset play  mSeekSection is %" PRId64,mSeekSection);
    }
}



int64_t MPEG2TSExtractor::getDurationUs() {
    return mDurationUs;
}

void MPEG2TSExtractor::seekTo(int64_t seekTimeUs) {
    Mutex::Autolock autoLock(mLock);

    ALOGI("seekTo: mDurationMs =%" PRId64 ",seekTimeMs= %" PRId64 ", mOffset:%" PRIu64 "\n", mDurationUs / 1000, seekTimeUs / 1000, mOffset);
    char value[PROPERTY_VALUE_MAX];
    if (property_get("rtk.omx.running", value, NULL) && (!strcmp(value, "1") || !strcasecmp(value, "true")))
    {
        ALOGI("Background thumbnail decoding, skip seeking for performance");
        return;
    }

    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
    //mSeekingOffset=(off64_t)((seekTimeUs*mFileSize/mDurationUs)/mRealPacketSize)* mRealPacketSize;
    mSeekingOffset = mOffset;
    mSeekTimeUs = seekTimeUs;
    mMinOffset = 0;
    mMaxOffset = mFileSize;
    mMinTime = 0;
    mMaxTime = mDurationUs;
    mMaxcount = 0;
    mParser->setDequeueState(false);
    mSeeking = true;
    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
    return;
}

off64_t MPEG2TSExtractor::SearchValidOffset(off64_t currentoffset)
{
    off64_t sync_offset = 0;
    if(mRealPacketSize == kMTSPacketSize)
        sync_offset = 4;
    for (; sync_offset < (off64_t)kSearchValidSyncWordOffset; ++sync_offset) {
        char sync;
        if(mFileSize > 0 && currentoffset + sync_offset >= mFileSize) {
            return currentoffset;
        }
        if(mDataSource->readAt(currentoffset + sync_offset, &sync, 1) == 1 && sync == 0x47) {
            if(mDataSource->readAt(currentoffset + sync_offset + mRealPacketSize, &sync, 1) == 1 && sync == 0x47)
            {
                //ALOGV("currentoffset %llu SearchValidOffset offset %llu ", currentoffset, sync_offset);
                break;
            }
        }
    }
    if(sync_offset >= (off64_t)kSearchValidSyncWordOffset) {
        return currentoffset;
    }
    else {
        if(mRealPacketSize == kMTSPacketSize)
        {
            if(sync_offset>=4)
                sync_offset -= 4;
            else
            {
                char sync;
                if(mDataSource->readAt(currentoffset + 4, &sync, 1) == 1 && sync == 0x47)
                    sync_offset = 0;
                else
                    return currentoffset + sync_offset - 4;
            }
        }
        return sync_offset + currentoffset;
    }
}

void MPEG2TSExtractor::seekToHead()
{
    mOffset = mStartOffset;
    mSeeking = false;
    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_SEEK, NULL);
    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
    mParser->setESSeeking();
    mParser->setDequeueState(true);
}
#endif

void MPEG2TSExtractor::init() {
    bool haveAudio = false;
    bool haveVideo = false;

#ifdef RTK_PLATFORM
    mDataSource->getSize(&mFileSize);
    if(mDataSource->flags() == 0)//local file playback
        mParser->setCreator(ATSParser::SOURCE_FROM_LOCAL);
    else
        mParser->setCreator(ATSParser::SOURCE_FROM_CACHED);
    mStartOffset = checkRealPacketLength();
    mOffset = mStartOffset;
    status_t err = parseMaxPTS();
    resetSeekSectionAndCount();
    if (err != OK) {
        ALOGD("parseMaxPTS ERROR !?");
        return;
    }
    mOffset = mStartOffset;
    //mOffset = 320;
    bool forceStop = false;
#endif
    int64_t startTime = ALooper::GetNowUs();
#ifndef RTK_PLATFORM
    while (feedMore(true /* isInit */) == OK) {
        if (haveAudio && haveVideo) {
            addSyncPoint_l(mLastSyncEvent);
            mLastSyncEvent.reset();
            break;
        }
#else
    while (forceStop == false && feedMore(true /* isInit */) == OK) {
#endif
        if (!haveVideo) {
            sp<AnotherPacketSource> impl =
                (AnotherPacketSource *)mParser->getSource(
                        ATSParser::VIDEO).get();

            if (impl != NULL) {
                haveVideo = true;
#ifdef RTK_PLATFORM
                if(mBaseTime == -1) {
                    mBaseTime = mParser->getBaseTime();
                    mDurationUs -= mBaseTime;
                    ALOGD("mBaseTime=%" PRId64 ", mDurationUs=%" PRId64 " \n", mBaseTime, mDurationUs);
                }
                impl->setBaseTimeUs(mBaseTime);
#endif
                mSourceImpls.push(impl);
                mSyncPoints.push();
                mSeekSyncPoints = &mSyncPoints.editTop();
            }
        }
#ifndef RTK_PLATFORM
        if (!haveAudio && (haveVideo || ALooper::GetNowUs() - startTime > 1000000ll)) {
#endif
            sp<AnotherPacketSource> impl =
                (AnotherPacketSource *)mParser->getSource(
                        ATSParser::AUDIO).get();

            if (impl != NULL) {
                haveAudio = true;
#ifdef RTK_PLATFORM
                if(mBaseTime == -1) {
                    mBaseTime = mParser->getBaseTime();
                    mDurationUs -= mBaseTime;
                    ALOGD("mBaseTime=%" PRId64 ", mDurationUs=%" PRId64 " \n", mBaseTime, mDurationUs);
                }
                impl->setBaseTimeUs(mBaseTime);
#endif
                mSourceImpls.push(impl);
                mSyncPoints.push();
                if (!haveVideo) {
                    mSeekSyncPoints = &mSyncPoints.editTop();
                }
            }
#ifndef RTK_PLATFORM
        }
#endif
        addSyncPoint_l(mLastSyncEvent);
        mLastSyncEvent.reset();

        // Wait only for 2 seconds to detect audio/video streams.
        if (ALooper::GetNowUs() - startTime > 2000000ll) {
            break;
        }
#ifdef RTK_PLATFORM
        // Avoid use memory too much in init state
        for (size_t i = 0; i < mSourceImpls.size(); ++i) {
            const sp<AnotherPacketSource> &impl = mSourceImpls[i];
            if(impl->getPendingBufferSize() > 10485760 /* 10MB */)
            {
                forceStop = true;
                break;
            }
        }
#endif
    }
#ifdef RTK_PLATFORM
    mParser->eraseOtherPrograms();
    if(!haveVideo && haveAudio)
        mIsAudioOnly = true;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&mThread, &attr, ThreadWrapper, this);
    pthread_attr_destroy(&attr);

#else
    off64_t size;
    if (mDataSource->getSize(&size) == OK && (haveAudio || haveVideo)) {
        sp<AnotherPacketSource> impl = haveVideo
                ? (AnotherPacketSource *)mParser->getSource(
                        ATSParser::VIDEO).get()
                : (AnotherPacketSource *)mParser->getSource(
                        ATSParser::AUDIO).get();
        size_t prevSyncSize = 1;
        int64_t durationUs = -1;
        List<int64_t> durations;
        // Estimate duration --- stabilize until you get <500ms deviation.
        while (feedMore() == OK
                && ALooper::GetNowUs() - startTime <= 2000000ll) {
            if (mSeekSyncPoints->size() > prevSyncSize) {
                prevSyncSize = mSeekSyncPoints->size();
                int64_t diffUs = mSeekSyncPoints->keyAt(prevSyncSize - 1)
                        - mSeekSyncPoints->keyAt(0);
                off64_t diffOffset = mSeekSyncPoints->valueAt(prevSyncSize - 1)
                        - mSeekSyncPoints->valueAt(0);
                durationUs = size * diffUs / diffOffset;
                durations.push_back(durationUs);
                if (durations.size() > 5) {
                    durations.erase(durations.begin());
                    int64_t min = *durations.begin();
                    int64_t max = *durations.begin();
                    for (List<int64_t>::iterator i = durations.begin();
                            i != durations.end();
                            ++i) {
                        if (min > *i) {
                            min = *i;
                        }
                        if (max < *i) {
                            max = *i;
                        }
                    }
                    if (max - min < 500 * 1000) {
                        break;
                    }
                }
            }
        }
        status_t err;
        int64_t bufferedDurationUs;
        bufferedDurationUs = impl->getBufferedDurationUs(&err);
        if (err == ERROR_END_OF_STREAM) {
            durationUs = bufferedDurationUs;
        }
        if (durationUs > 0) {
            const sp<MetaData> meta = impl->getFormat();
            meta->setInt64(kKeyDuration, durationUs);
            impl->setFormat(meta);
        }
    }
#endif
    ALOGI("haveAudio=%d, haveVideo=%d, elaspedTime=%" PRId64,
            haveAudio, haveVideo, ALooper::GetNowUs() - startTime);
}

status_t MPEG2TSExtractor::feedMore(bool isInit) {
    Mutex::Autolock autoLock(mLock);
#ifdef RTK_PLATFORM
    sp<AnotherPacketSource> impl = NULL;
    if(mSourceImpls.size() > 1)
    {
        if(mIsAudioOnly)
        {
            const sp<AnotherPacketSource> &tmpImpl = mSourceImpls[0];
            impl = tmpImpl;
        }
        else
        {
            const sp<AnotherPacketSource> tmpImpl = (AnotherPacketSource *)mParser->getSource(ATSParser::VIDEO).get();
            impl = tmpImpl;
        }
    }

    if (mSeeking) {
        int64_t pts = mParser->getMaxPTS() - mBaseTime;
        if (pts > 0 && mSeekTimeUs != 0) {
            mMaxcount++;
            if ((pts - mSeekTimeUs < mSeekSection && pts - mSeekTimeUs > -mSeekSection)
                || mMinOffset == mMaxOffset || mMaxcount > mSeekCount) {
                ALOGE ("SEEK ON TIME pts=%" PRId64 ",mSeekTimeUs=%" PRId64" ,mMaxcount=%" PRIu64 ",mOffset=%" PRIu64 ",mMinOffset=%" PRIu64 ",mMaxOffset=%" PRIu64 "\n",
                 pts / 1000, mSeekTimeUs / 1000, mMaxcount, mOffset, mMinOffset, mMaxOffset);
                mSeeking = false;
                mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
                mParser->setESSeeking();
                mParser->setDequeueState(true);
            } else {
                mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_SEEK, NULL);
                mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
                if (pts < mSeekTimeUs) {
                    mMinOffset = mSeekingOffset/mRealPacketSize*mRealPacketSize;
                    mMinTime = pts;
                } else {
                    mMaxOffset = mSeekingOffset/mRealPacketSize*mRealPacketSize;
                    mMaxTime = pts;
                }
                mSeekingOffset = (off64_t) (((mMaxOffset-(((double)(mMaxTime-mSeekTimeUs))/((double)(mMaxTime-mMinTime))*(mMaxOffset-mMinOffset)))/ mRealPacketSize) * mRealPacketSize);
                mOffset = mSeekingOffset;
            }
            ALOGE ("pts=%" PRId64 ",mSeekTimeUs=%" PRId64" ,mMaxcount=%" PRIu64 ",mOffset=%" PRIu64 ",mMinOffset=%" PRIu64 ",mMaxOffset=%" PRIu64 "\n",
                 pts / 1000, mSeekTimeUs / 1000, mMaxcount, mOffset, mMinOffset, mMaxOffset);

        }
        else if(mSeekTimeUs == 0)
        {
            mOffset = mStartOffset;
            mSeeking = false;
            mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_SEEK, NULL);
            mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
            mParser->setESSeeking();
            mParser->setDequeueState(true);
        }
    }
    else if(impl != NULL && impl->getCurBufferSize() > 10 * 1024 * 1024)//10MB for avoid mem allocation fail
        return OK;
#endif


#ifdef RTK_PLATFORM
    uint8_t packet[mRealPacketSize*kIOReadNumPacket];
    //if(mRealPacketSize == kTSPacketSize)
    mOffset = SearchValidOffset(mOffset);
    ssize_t n = mDataSource->readAt(mOffset, packet, mRealPacketSize*kIOReadNumPacket);
    if (n < (ssize_t)(mRealPacketSize*kIOReadNumPacket)) {
#else
    uint8_t packet[kTSPacketSize];
    ssize_t n = mDataSource->readAt(mOffset, packet, kTSPacketSize);
    if (n < (ssize_t)kTSPacketSize) {
#endif
        if (n >= 0 && n < (ssize_t)mRealPacketSize) {
            mParser->signalEOS(ERROR_END_OF_STREAM);
        }
        return (n < 0) ? (status_t)n : ERROR_END_OF_STREAM;
    }

    status_t err;
#ifdef RTK_PLATFORM
    for(size_t k=0;k<kIOReadNumPacket;k++)
    {
        mOffset += mRealPacketSize;
#else
        mOffset += n;
#endif
        ATSParser::SyncEvent event(mOffset);
#ifdef RTK_PLATFORM
        if(mRealPacketSize == kMTSPacketSize)
        {
            if(packet[k*mRealPacketSize+4] != 0x47)
            {
                mOffset -= mRealPacketSize;
                break;
            }

            err = mParser->feedTSPacket(packet+4+(k*mRealPacketSize), mRealPacketSize-4, &event);
        }else if(mRealPacketSize == kFECPacketSize){
           err = mParser->feedTSPacket(packet+(k*mRealPacketSize), mRealPacketSize-16, &event);
        }
        else
        {
            if(packet[k*mRealPacketSize] != 0x47)
            {
                mOffset -= mRealPacketSize;
                break;
            }
            err = mParser->feedTSPacket(packet+(k*mRealPacketSize), mRealPacketSize, &event);

        }
#else
        status_t err = mParser->feedTSPacket(packet, kTSPacketSize, &event);
#endif
        if (event.hasReturnedData()) {
            if (isInit) {
                mLastSyncEvent = event;
            } else {
                addSyncPoint_l(event);
            }
        }

#ifdef RTK_PLATFORM
        if(mSeeking || err != OK)
            k = kIOReadNumPacket;
    }
#endif
    return err;
}

void MPEG2TSExtractor::addSyncPoint_l(const ATSParser::SyncEvent &event) {
    if (!event.hasReturnedData()) {
        return;
    }
#ifndef RTK_PLATFORM
    for (size_t i = 0; i < mSourceImpls.size(); ++i) {
        if (mSourceImpls[i].get() == event.getMediaSource().get()) {
            KeyedVector<int64_t, off64_t> *syncPoints = &mSyncPoints.editItemAt(i);
            syncPoints->add(event.getTimeUs(), event.getOffset());
            // We're keeping the size of the sync points at most 5mb per a track.
            size_t size = syncPoints->size();
            if (size >= 327680) {
                int64_t firstTimeUs = syncPoints->keyAt(0);
                int64_t lastTimeUs = syncPoints->keyAt(size - 1);
                if (event.getTimeUs() - firstTimeUs > lastTimeUs - event.getTimeUs()) {
                    syncPoints->removeItemsAt(0, 4096);
                } else {
                    syncPoints->removeItemsAt(size - 4096, 4096);
                }
            }
            break;
        }
    }
#endif
}

uint32_t MPEG2TSExtractor::flags() const {
    return CAN_PAUSE | CAN_SEEK_BACKWARD | CAN_SEEK_FORWARD;
}

status_t MPEG2TSExtractor::seek(int64_t seekTimeUs,
        const MediaSource::ReadOptions::SeekMode &seekMode) {
    if (mSeekSyncPoints == NULL || mSeekSyncPoints->isEmpty()) {
        ALOGW("No sync point to seek to.");
        // ... and therefore we have nothing useful to do here.
        return OK;
    }

    // Determine whether we're seeking beyond the known area.
    bool shouldSeekBeyond =
            (seekTimeUs > mSeekSyncPoints->keyAt(mSeekSyncPoints->size() - 1));

    // Determine the sync point to seek.
    size_t index = 0;
    for (; index < mSeekSyncPoints->size(); ++index) {
        int64_t timeUs = mSeekSyncPoints->keyAt(index);
        if (timeUs > seekTimeUs) {
            break;
        }
    }

    switch (seekMode) {
        case MediaSource::ReadOptions::SEEK_NEXT_SYNC:
            if (index == mSeekSyncPoints->size()) {
                ALOGW("Next sync not found; starting from the latest sync.");
                --index;
            }
            break;
        case MediaSource::ReadOptions::SEEK_CLOSEST_SYNC:
        case MediaSource::ReadOptions::SEEK_CLOSEST:
            ALOGW("seekMode not supported: %d; falling back to PREVIOUS_SYNC",
                    seekMode);
            // fall-through
        case MediaSource::ReadOptions::SEEK_PREVIOUS_SYNC:
            if (index == 0) {
                ALOGW("Previous sync not found; starting from the earliest "
                        "sync.");
            } else {
                --index;
            }
            break;
    }
    if (!shouldSeekBeyond || mOffset <= mSeekSyncPoints->valueAt(index)) {
        int64_t actualSeekTimeUs = mSeekSyncPoints->keyAt(index);
        mOffset = mSeekSyncPoints->valueAt(index);
        status_t err = queueDiscontinuityForSeek(actualSeekTimeUs);
        if (err != OK) {
            return err;
        }
    }

    if (shouldSeekBeyond) {
        status_t err = seekBeyond(seekTimeUs);
        if (err != OK) {
            return err;
        }
    }

    // Fast-forward to sync frame.
    for (size_t i = 0; i < mSourceImpls.size(); ++i) {
        const sp<AnotherPacketSource> &impl = mSourceImpls[i];
        status_t err;
        feedUntilBufferAvailable(impl);
        while (impl->hasBufferAvailable(&err)) {
            sp<AMessage> meta = impl->getMetaAfterLastDequeued(0);
            sp<ABuffer> buffer;
            if (meta == NULL) {
                return UNKNOWN_ERROR;
            }
            int32_t sync;
            if (meta->findInt32("isSync", &sync) && sync) {
                break;
            }
            err = impl->dequeueAccessUnit(&buffer);
            if (err != OK) {
                return err;
            }
            feedUntilBufferAvailable(impl);
        }
    }

    return OK;
}

status_t MPEG2TSExtractor::queueDiscontinuityForSeek(int64_t actualSeekTimeUs) {
    // Signal discontinuity
    sp<AMessage> extra(new AMessage);
    extra->setInt64(IStreamListener::kKeyMediaTimeUs, actualSeekTimeUs);
    mParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, extra);

    // After discontinuity, impl should only have discontinuities
    // with the last being what we queued. Dequeue them all here.
    for (size_t i = 0; i < mSourceImpls.size(); ++i) {
        const sp<AnotherPacketSource> &impl = mSourceImpls.itemAt(i);
        sp<ABuffer> buffer;
        status_t err;
        while (impl->hasBufferAvailable(&err)) {
            if (err != OK) {
                return err;
            }
            err = impl->dequeueAccessUnit(&buffer);
            // If the source contains anything but discontinuity, that's
            // a programming mistake.
            CHECK(err == INFO_DISCONTINUITY);
        }
    }

    // Feed until we have a buffer for each source.
    for (size_t i = 0; i < mSourceImpls.size(); ++i) {
        const sp<AnotherPacketSource> &impl = mSourceImpls.itemAt(i);
        sp<ABuffer> buffer;
        status_t err = feedUntilBufferAvailable(impl);
        if (err != OK) {
            return err;
        }
    }

    return OK;
}

status_t MPEG2TSExtractor::seekBeyond(int64_t seekTimeUs) {
    // If we're seeking beyond where we know --- read until we reach there.
    size_t syncPointsSize = mSeekSyncPoints->size();

    while (seekTimeUs > mSeekSyncPoints->keyAt(
            mSeekSyncPoints->size() - 1)) {
        status_t err;
        if (syncPointsSize < mSeekSyncPoints->size()) {
            syncPointsSize = mSeekSyncPoints->size();
            int64_t syncTimeUs = mSeekSyncPoints->keyAt(syncPointsSize - 1);
            // Dequeue buffers before sync point in order to avoid too much
            // cache building up.
            sp<ABuffer> buffer;
            for (size_t i = 0; i < mSourceImpls.size(); ++i) {
                const sp<AnotherPacketSource> &impl = mSourceImpls[i];
                int64_t timeUs;
                while ((err = impl->nextBufferTime(&timeUs)) == OK) {
                    if (timeUs < syncTimeUs) {
                        impl->dequeueAccessUnit(&buffer);
                    } else {
                        break;
                    }
                }
                if (err != OK && err != -EWOULDBLOCK) {
                    return err;
                }
            }
        }
        if (feedMore() != OK) {
            return ERROR_END_OF_STREAM;
        }
    }

    return OK;
}

status_t MPEG2TSExtractor::feedUntilBufferAvailable(
        const sp<AnotherPacketSource> &impl) {
    status_t finalResult;
#ifdef RTK_PLATFORM
    unsigned readCount = 0;
#endif
    while (!impl->hasBufferAvailable(&finalResult)) {
        if (finalResult != OK) {
            return finalResult;
        }

        status_t err = feedMore();
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
            impl->signalEOS(err);
        }
    }
    return OK;
}

////////////////////////////////////////////////////////////////////////////////

bool SniffMPEG2TS(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *) {
    bool sniffResult = true;
#ifdef RTK_PLATFORM
    size_t sync_offset = 0;
    sp<ABuffer> buffer = new ABuffer(64);
    buffer->setRange(0, 0);

    source->readAt(0, buffer->data(), 64);
    unsigned char* buf = buffer->data();
    if (strstr((char *)buf, "EXTM3U"))
    {
        ALOGD("This is M3U8 source, will reject it ");
        return false;
    }

    for (sync_offset = 0; sync_offset < kSearchValidSyncWordOffset; ++sync_offset) {
        char sync;
        if (source->readAt(sync_offset, &sync, 1) == 1 && sync == 0x47) {
            ALOGV("sniff find sync word offset %zu ", sync_offset);
            break;
        }
    }
#endif
    for (int i = 0; i < 5; ++i) {
        char header;
#ifndef RTK_PLATFORM
        if (source->readAt(kTSPacketSize * i, &header, 1) != 1
#else
        if (source->readAt(sync_offset + (kTSPacketSize * i), &header, 1) != 1
#endif
                || header != 0x47) {
            sniffResult = false;
        }
    }

    if(sniffResult == true) {
        ALOGV("Find TS format");
    }
#ifdef RTK_PLATFORM
    else
    {
        sniffResult = true;
        for (int i = 0; i < 5; ++i) {
            char headerFEC;
            if (source->readAt(sync_offset+(kFECPacketSize * i), &headerFEC, 1) != 1 || headerFEC != 0x47) {
                sniffResult = false;
            }
        }
        if(sniffResult == true){
            ALOGV("Find kFECPacketSize TS format");
        }else{
            size_t searchCount = 0;
            sniffResult = true;
            for (int i = 0; i < 5; ++i) {
                char header;
                for (; sync_offset < kSearchValidSyncWordOffset; ++sync_offset) {
                    char sync;
                    if (source->readAt(sync_offset, &sync, 1) == 1 && sync == 0x47) {
                        ALOGV("sniff find sync word offset %zu ", sync_offset);
                        break;
                    }
                }
                if (source->readAt(sync_offset + (kMTSPacketSize * i), &header, 1) != 1 || header != 0x47) {
                    searchCount++;
                    sync_offset++;
                    if(searchCount > 1) {
                        sniffResult = false;
                        return sniffResult;
                    }
                }
            }
            if(sniffResult == true)
                ALOGV("Find MTS format");
        }
    }
#else
    else
        return sniffResult;
#endif

    *confidence = 0.1f;
    mimeType->setTo(MEDIA_MIMETYPE_CONTAINER_MPEG2TS);

    return true;
}

}  // namespace android

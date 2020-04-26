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

#ifndef MPEG2_TS_EXTRACTOR_H_

#define MPEG2_TS_EXTRACTOR_H_

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaSource.h>
#include <utils/threads.h>
#include <utils/KeyedVector.h>
#include <utils/Vector.h>
#ifdef RTK_PLATFORM
#include <pthread.h>
#endif

#include "mpeg2ts/ATSParser.h"

namespace android {

struct AMessage;
struct AnotherPacketSource;
struct ATSParser;
class DataSource;
struct MPEG2TSSource;
class String8;

struct MPEG2TSExtractor : public MediaExtractor {
    MPEG2TSExtractor(const sp<DataSource> &source);

    virtual size_t countTracks();
    virtual sp<IMediaSource> getTrack(size_t index);
    virtual sp<MetaData> getTrackMetaData(size_t index, uint32_t flags);

    virtual sp<MetaData> getMetaData();

    virtual uint32_t flags() const;
    virtual const char * name() { return "MPEG2TSExtractor"; }

#ifdef RTK_PLATFORM
protected:
    virtual ~MPEG2TSExtractor();
#endif

private:
    friend struct MPEG2TSSource;

    mutable Mutex mLock;

    sp<DataSource> mDataSource;

    sp<ATSParser> mParser;

    // Used to remember SyncEvent occurred in feedMore() when called from init(),
    // because init() needs to update |mSourceImpls| before adding SyncPoint.
    ATSParser::SyncEvent mLastSyncEvent;

    Vector<sp<AnotherPacketSource> > mSourceImpls;

    Vector<KeyedVector<int64_t, off64_t> > mSyncPoints;
    // Sync points used for seeking --- normally one for video track is used.
    // If no video track is present, audio track will be used instead.
    KeyedVector<int64_t, off64_t> *mSeekSyncPoints;

    off64_t mOffset;
#ifdef RTK_PLATFORM
    uint8_t mPacket[49344];//257x192 byte
    size_t mRemainder;
    size_t mRealPacketSize;
    size_t mIOReadSize;
    unsigned mReadTimeOutCount;
    pthread_t mThread;
    bool mStopThread;
    int64_t mDurationUs;
    off64_t mFileSize;
    int64_t mSeekTimeUs;
    bool mSeeking;
    bool isVideoSeekOver;
    uint64_t mMaxcount;
    off64_t mSeekingOffset;
    off64_t mMinOffset;
    off64_t mMaxOffset;
    int64_t mMinTime;
    int64_t mMaxTime;
    int64_t mBaseTime;
    off64_t mStartOffset;
    int64_t mSeekSection;
    size_t mSeekCount;
    bool mIsAudioOnly;
#endif

    void init();
#ifdef RTK_PLATFORM
    off64_t checkRealPacketLength();
    static void *ThreadWrapper(void *me);
    void ThreadEntry();
    status_t parseMaxPTS();
    int64_t getDurationUs();
    void resetSeekSectionAndCount();
    void seekTo(int64_t seekTimeUs);
    off64_t SearchValidOffset(off64_t currentoffset);
    void seekToHead();
#endif
    // Try to feed more data from source to parser.
    // |isInit| means this function is called inside init(). This is a signal to
    // save SyncEvent so that init() can add SyncPoint after it updates |mSourceImpls|.
    // This function returns OK if expected amount of data is fed from DataSource to
    // parser and is successfully parsed. Otherwise, various error codes could be
    // returned, e.g., ERROR_END_OF_STREAM, or no data availalbe from DataSource, or
    // the data has syntax error during parsing, etc.
    status_t feedMore(bool isInit = false);
    status_t seek(int64_t seekTimeUs,
            const MediaSource::ReadOptions::SeekMode& seekMode);
    status_t queueDiscontinuityForSeek(int64_t actualSeekTimeUs);
    status_t seekBeyond(int64_t seekTimeUs);

    status_t feedUntilBufferAvailable(const sp<AnotherPacketSource> &impl);

    // Add a SynPoint derived from |event|.
    void addSyncPoint_l(const ATSParser::SyncEvent &event);

    DISALLOW_EVIL_CONSTRUCTORS(MPEG2TSExtractor);
};

bool SniffMPEG2TS(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *);

}  // namespace android

#endif  // MPEG2_TS_EXTRACTOR_H_

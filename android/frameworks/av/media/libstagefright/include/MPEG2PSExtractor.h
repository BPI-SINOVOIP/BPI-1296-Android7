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

#ifndef MPEG2_PS_EXTRACTOR_H_

#define MPEG2_PS_EXTRACTOR_H_

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/MediaExtractor.h>
#include <utils/threads.h>
#include <utils/KeyedVector.h>
#ifdef RTK_PLATFORM
#include <pthread.h>
#endif

namespace android {

struct ABuffer;
struct AMessage;
struct Track;
class String8;

struct MPEG2PSExtractor : public MediaExtractor {
    MPEG2PSExtractor(const sp<DataSource> &source);

    virtual size_t countTracks();
    virtual sp<IMediaSource> getTrack(size_t index);
    virtual sp<MetaData> getTrackMetaData(size_t index, uint32_t flags);

    virtual sp<MetaData> getMetaData();

    virtual uint32_t flags() const;
    virtual const char * name() { return "MPEG2PSExtractor"; }

protected:
    virtual ~MPEG2PSExtractor();

private:
    struct Track;
    struct WrappedTrack;

    mutable Mutex mLock;
    sp<DataSource> mDataSource;

    off64_t mOffset;
    status_t mFinalResult;
    sp<ABuffer> mBuffer;
    KeyedVector<unsigned, sp<Track> > mTracks;
    bool mScanning;

    bool mProgramStreamMapValid;
    KeyedVector<unsigned, unsigned> mStreamTypeByESID;

#ifdef RTK_PLATFORM
    bool mIsMPEG1;
    pthread_t mThread;
    bool mStopThread;
    off64_t mFileSize;
    int64_t mDurationUs;
    bool mNeedDequeuePES;
    off64_t mAverageByteRate;
    int64_t mSeekTimeUs;
    bool mSeeking;
    uint64_t mMaxcount;
    off64_t mSeekingOffset;
    off64_t mMinOffset;
    off64_t mMaxOffset;
    int64_t mBaseTime;
    int64_t mLastSeekTimeUs;
    unsigned mPriExtStreamId;
#endif

    status_t feedMore();

    status_t dequeueChunk();
    ssize_t dequeuePack();
    ssize_t dequeueSystemHeader();
    ssize_t dequeuePES();
#ifdef RTK_PLATFORM
    static void *ThreadWrapper(void *me);
    void ThreadEntry();
    void setDequeueState(bool needDequeuePES);
    bool getDequeueState();
    void updateSeekOffset(int64_t pts);
    int64_t SearchPES(const void* data, int size);
    int64_t SearchValidOffset(off64_t currentoffset);
    int64_t getMaxPTS();
    void seekTo(int64_t seekTimeUs);
    void parseMaxPTS();
    int64_t getDurationUs();
    void signalDiscontinuity(const bool bKeepFormat);
#endif
    DISALLOW_EVIL_CONSTRUCTORS(MPEG2PSExtractor);
};

bool SniffMPEG2PS(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *);

}  // namespace android

#endif  // MPEG2_PS_EXTRACTOR_H_


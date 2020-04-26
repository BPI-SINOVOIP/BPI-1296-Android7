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

#ifndef ES_QUEUE_H_

#define ES_QUEUE_H_

#include <media/stagefright/foundation/ABase.h>
#include <utils/Errors.h>
#include <utils/List.h>
#include <utils/RefBase.h>

namespace android {

struct ABuffer;
class MetaData;

struct ElementaryStreamQueue {
    enum Mode {
        H264,
        H265,
        WVC1,
        AAC,
        AC3,
        MPEG_AUDIO,
        MPEG_VIDEO,
        MPEG4_VIDEO,
        PCM_AUDIO,
        LPCM_AUDIO,
        DTS,
        DTS_HD,
        TRUEHD,
        AAC_LATM,
        METADATA,
        AVS,
    };

    enum Flags {
        // Data appended to the queue is always at access unit boundaries.
        kFlag_AlignedData = 1,
    };
    ElementaryStreamQueue(Mode mode, uint32_t flags = 0);

    status_t appendData(const void *data, size_t size, int64_t timeUs);
    void signalEOS();
    void clear(bool clearFormat);

    sp<ABuffer> dequeueAccessUnit();

    sp<MetaData> getFormat();
#ifdef RTK_PLATFORM
    void setSeeking();
    bool getSeekOverStatus();
    bool getSeekStatus();
    void setSeekStatusOver();
    int64_t getFirstFrameTimestamp();
    void setSkipFrameBeforeTimestamp(int64_t timeUs);
    int64_t getSeekOverTimestamp();
    void setSkipFrameAfterSeekTimestamp(int64_t timeUs);
    int64_t getSkipFrmaeBeforeTimestamp();
#endif

private:
    struct RangeInfo {
        int64_t mTimestampUs;
        size_t mLength;
    };

    Mode mMode;
    uint32_t mFlags;
    bool mEOSReached;

    sp<ABuffer> mBuffer;
    List<RangeInfo> mRangeInfos;

    sp<MetaData> mFormat;
#ifdef RTK_PLATFORM
    size_t mOffset;
    int64_t mLastTimeUs;
    bool mSeeking;
    bool mVideoSeekOver;
    bool mGOPTimeSeeking;
    int64_t mGOPTimeDiff;
    int64_t mGOPTimeTmp;
    int64_t mGOPTimeOffset;
    size_t mGOPOffset;
    size_t mCheckDisableGOPTPS;
    size_t mLastGOPOffset;
    List<sp<ABuffer> > accessUnits;
    int64_t mFirstFrameTimestamp;
    int64_t mSeekOverTimestamp;
    int64_t mSkipFrameBeforeTimestamp;
    int64_t mSkipFrameAfterSeekTimestamp;
    size_t mPayloadSize;
    size_t mEAC3CheckCount;
    int64_t mMPEGVideoSeekTimeStamp;
    int64_t mMPEGVideoLastPTS;
#endif
    sp<ABuffer> dequeueAccessUnitH264();
    sp<ABuffer> dequeueAccessUnitAAC();
    sp<ABuffer> dequeueAccessUnitAC3();
    sp<ABuffer> dequeueAccessUnitMPEGAudio();
    sp<ABuffer> dequeueAccessUnitMPEGVideo();
    sp<ABuffer> dequeueAccessUnitMPEG4Video();
    sp<ABuffer> dequeueAccessUnitPCMAudio();
    sp<ABuffer> dequeueAccessUnitMetadata();
#ifdef RTK_PLATFORM
    sp<ABuffer> dequeueAccessUnitH265();
    sp<ABuffer> dequeueAccessUnitWVC1();
    sp<ABuffer> dequeueAccessUnitTrueHD();
    sp<ABuffer> dequeueAccessUnitAACLATM();
    sp<ABuffer> dequeueAccessUnitDTS();
    sp<ABuffer> dequeueAccessUnitAVS();
    char* getTrackModetoString(Mode mode);
    bool IsIFrame(uint8_t *nalStart, size_t nalSize);
    int64_t CheckGOPPTS(int64_t timeUs);
    unsigned parseAC3SyncFrame(const uint8_t *ptr, size_t size, sp<MetaData> *metaData);
    void endianChange(uint8_t *data,size_t size);
#endif
    // consume a logical (compressed) access unit of size "size",
    // returns its timestamp in us (or -1 if no time information).
    int64_t fetchTimestamp(size_t size);
    DISALLOW_EVIL_CONSTRUCTORS(ElementaryStreamQueue);
};

}  // namespace android

#endif  // ES_QUEUE_H_

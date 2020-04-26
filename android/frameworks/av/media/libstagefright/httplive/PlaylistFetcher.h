/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef PLAYLIST_FETCHER_H_

#define PLAYLIST_FETCHER_H_

#include <media/stagefright/foundation/AHandler.h>

#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaBuffer.h>
#include "include/HTTPBase.h"

#include "mpeg2ts/ATSParser.h"
#include "LiveSession.h"

#ifdef RTK_PLATFORM
#define RTK_PROPERTY_HLS_ERR_COUNT  "rtk.media.video.hlserrcount"
#define codec_show_pts(pts) ((double)(pts/1E6))
#endif

namespace android {

struct ABuffer;
struct AnotherPacketSource;
class DataSource;
struct HTTPBase;
struct LiveDataSource;
struct M3UParser;
class String8;

struct NuCachedSource2;

struct PlaylistFetcher : public AHandler {
    static const int64_t kMinBufferedDurationUs;
#ifdef RTK_PLATFORM
    static const int64_t kMinBufferedAudioDurationUs;
#endif
    static const int32_t kDownloadBlockSize;
    static const int64_t kFetcherResumeThreshold;

#ifdef RTK_PLATFORM
    static const int32_t kWaitDownloadErrTime;
    static const int32_t kWaitDownloadErrIniCount;
    int32_t kWaitDownloadErrCount;
#endif

    enum {
        kWhatStarted,
        kWhatPaused,
        kWhatStopped,
        kWhatError,
        kWhatDurationUpdate,
        kWhatTargetDurationUpdate,
        kWhatPrepared,
        kWhatPreparationFailed,
        kWhatStartedAt,
        kWhatStopReached,
        kWhatPlaylistFetched,
        kWhatMetadataDetected,
    };

    PlaylistFetcher(
            const sp<AMessage> &notify,
            const sp<LiveSession> &session,
            const char *uri,
            int32_t id,
            int32_t subtitleGeneration);

    int32_t getFetcherID() const;

    void startAsync(
            const sp<AnotherPacketSource> &audioSource,
            const sp<AnotherPacketSource> &videoSource,
            const sp<AnotherPacketSource> &subtitleSource,
            const sp<AnotherPacketSource> &metadataSource,
            int64_t startTimeUs = -1ll,         // starting timestamps
            int64_t segmentStartTimeUs = -1ll, // starting position within playlist
            // startTimeUs!=segmentStartTimeUs only when playlist is live
            int32_t startDiscontinuitySeq = -1,
#ifdef RTK_PLATFORM
            bool bIsBandwidthSwitchFlw = false,
#endif
            LiveSession::SeekMode seekMode = LiveSession::kSeekModeExactPosition);

    void pauseAsync(float thresholdRatio, bool disconnect);

    void stopAsync(bool clear = true);

    void resumeUntilAsync(const sp<AMessage> &params);

    void fetchPlaylistAsync();

    uint32_t getStreamTypeMask() const {
        return mStreamTypeMask;
    }
#ifdef RTK_PLATFORM
    void signalDiscontinuity(int32_t count);
    void signalSeekto();
    void onRTKCustomerHLSerrCntChk();
    enum {
        kPlaylistStatus_OK = 0,
        kPlaylistStatus_None,
        kPlaylistStatus_Normal,
        kPlaylistStatus_FetchFile,
        kPlaylistStatus_initDownloadState,
        kPlaylistStatus_FetchFileBlocked,
        kPlaylistStatus_FetchStatusError,
        kPlaylistStatus_MAX,
    };
    int32_t checkStatus();
    int32_t chkIsSeqDiscontinuity(ssize_t streamIdx, ssize_t dstreamIdx, int32_t curSeqNumber, int64_t timeUs, bool *IsSeqBoundary);
    int32_t updateMinSeq(int32_t minSeqNum);
    void LogSeqInfo(int32_t flag);
#endif
protected:
    virtual ~PlaylistFetcher();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum {
        kMaxNumRetries         = 100,
#ifdef RTK_PLATFORM
        kPlaylistOverTime      = 300000,//300 seconds.
#endif
    };

    enum {
        kWhatStart          = 'strt',
        kWhatPause          = 'paus',
        kWhatStop           = 'stop',
        kWhatMonitorQueue   = 'moni',
        kWhatResumeUntil    = 'rsme',
        kWhatDownloadNext   = 'dlnx',
        kWhatFetchPlaylist  = 'flst'
    };

    struct DownloadState;

    static const int64_t kMaxMonitorDelayUs;
    static const int32_t kNumSkipFrames;

    static bool bufferStartsWithTsSyncByte(const sp<ABuffer>& buffer);
    static bool bufferStartsWithWebVTTMagicSequence(const sp<ABuffer>& buffer);

    // notifications to mSession
    sp<AMessage> mNotify;
    sp<AMessage> mStartTimeUsNotify;

    sp<HTTPDownloader> mHTTPDownloader;
    sp<LiveSession> mSession;
    AString mURI;

    int32_t mFetcherID;

    uint32_t mStreamTypeMask;
    int64_t mStartTimeUs;

    // Start time relative to the beginning of the first segment in the initial
    // playlist. It's value is initialized to a non-negative value only when we are
    // adapting or switching tracks.
    int64_t mSegmentStartTimeUs;

    int32_t mDiscontinuitySeq;
    bool mStartTimeUsRelative;
    sp<AMessage> mStopParams; // message containing the latest timestamps we should fetch.

    KeyedVector<LiveSession::StreamType, sp<AnotherPacketSource> >
        mPacketSources;

    KeyedVector<AString, sp<ABuffer> > mAESKeyForURI;

    int64_t mLastPlaylistFetchTimeUs;
    int64_t mPlaylistTimeUs;
    sp<M3UParser> mPlaylist;
    int32_t mSeqNumber;
    int32_t mNumRetries;
    bool mStartup;
    bool mIDRFound;
    int32_t mSeekMode;
    bool mTimeChangeSignaled;
    int64_t mNextPTSTimeUs;

    int32_t mMonitorQueueGeneration;
    const int32_t mSubtitleGeneration;

    int32_t mLastDiscontinuitySeq;

    enum RefreshState {
        INITIAL_MINIMUM_RELOAD_DELAY,
        FIRST_UNCHANGED_RELOAD_ATTEMPT,
        SECOND_UNCHANGED_RELOAD_ATTEMPT,
        THIRD_UNCHANGED_RELOAD_ATTEMPT
    };
    RefreshState mRefreshState;

    uint8_t mPlaylistHash[16];

    sp<ATSParser> mTSParser;

    bool mFirstPTSValid;
    int64_t mFirstTimeUs;
    int64_t mSegmentFirstPTS;
    sp<AnotherPacketSource> mVideoBuffer;

    // Stores the initialization vector to decrypt the next block of cipher text, which can
    // either be derived from the sequence number, read from the manifest, or copied from
    // the last block of cipher text (cipher-block chaining).
    unsigned char mAESInitVec[16];

    Mutex mThresholdLock;
    float mThresholdRatio;

    sp<DownloadState> mDownloadState;

    bool mHasMetadata;

    // Set first to true if decrypting the first segment of a playlist segment. When
    // first is true, reset the initialization vector based on the available
    // information in the manifest; otherwise, use the initialization vector as
    // updated by the last call to AES_cbc_encrypt.
    //
    // For the input to decrypt correctly, decryptBuffer must be called on
    // consecutive byte ranges on block boundaries, e.g. 0..15, 16..47, 48..63,
    // and so on.
    status_t decryptBuffer(
            size_t playlistIndex, const sp<ABuffer> &buffer,
            bool first = true);
    status_t checkDecryptPadding(const sp<ABuffer> &buffer);

    void postMonitorQueue(int64_t delayUs = 0, int64_t minDelayUs = 0);
    void cancelMonitorQueue();
    void setStoppingThreshold(float thresholdRatio, bool disconnect);
    void resetStoppingThreshold(bool disconnect);
    float getStoppingThreshold();
    bool shouldPauseDownload();

    int64_t delayUsToRefreshPlaylist() const;
    status_t refreshPlaylist();

    // Returns the media time in us of the segment specified by seqNumber.
    // This is computed by summing the durations of all segments before it.
    int64_t getSegmentStartTimeUs(int32_t seqNumber) const;
    // Returns the duration time in us of the segment specified.
    int64_t getSegmentDurationUs(int32_t seqNumber) const;

    status_t onStart(const sp<AMessage> &msg);
    void onPause();
    void onStop(const sp<AMessage> &msg);
    void onMonitorQueue();
    void onDownloadNext();
    bool initDownloadState(
            AString &uri,
            sp<AMessage> &itemMeta,
            int32_t &firstSeqNumberInPlaylist,
            int32_t &lastSeqNumberInPlaylist);

    // Resume a fetcher to continue until the stopping point stored in msg.
    status_t onResumeUntil(const sp<AMessage> &msg);

    const sp<ABuffer> &setAccessUnitProperties(
            const sp<ABuffer> &accessUnit,
            const sp<AnotherPacketSource> &source,
            bool discard = false);
    bool isStartTimeReached(int64_t timeUs);
    status_t extractAndQueueAccessUnitsFromTs(const sp<ABuffer> &buffer);

    status_t extractAndQueueAccessUnits(
            const sp<ABuffer> &buffer, const sp<AMessage> &itemMeta);

    void notifyStopReached();
    void notifyError(status_t err);

    void queueDiscontinuity(
            ATSParser::DiscontinuityType type, const sp<AMessage> &extra);

    bool adjustSeqNumberWithAnchorTime(int64_t anchorTimeUs);
    int32_t getSeqNumberForDiscontinuity(size_t discontinuitySeq) const;
    int32_t getSeqNumberForTime(int64_t timeUs) const;

    void updateDuration();
    void updateTargetDuration();

    DISALLOW_EVIL_CONSTRUCTORS(PlaylistFetcher);

    struct Track {
        size_t mIndex;
        sp<IMediaSource> mSource;
        sp<AnotherPacketSource> mPackets;
    };

    Track mAudioTrack;
    int64_t mAudioTimeUs;
    int64_t mAudioLastDequeueTimeUs;
    Track mVideoTrack;
    int64_t mVideoTimeUs;
    int64_t mVideoLastDequeueTimeUs;
    sp<IMediaExtractor> mExtractor;
    sp<MetaData> mFileMeta;
    sp<IMediaHTTPService> mHTTPService;
    sp<DataSource> mDataSource;
    sp<DataSource> mHttpSource;
    KeyedVector<String8, String8> mUriHeaders;
    sp<HTTPBase> mHTTPDataSource;
    sp<NuCachedSource2> mCachedSource;
    int32_t dIsAudioEOF;
    int32_t dIsVideoEOF;
    int32_t dsmallTimeTrack;
    void resetDataSource();
    status_t ParseTsStreamFromRtkExt();
    status_t initFromDataSource(AString uri);
    sp<ABuffer> mediaBufferToABuffer(MediaBuffer* mb,media_track_type trackType);
    int32_t mUseRTKExtFlow;
    struct timeval mDownloadErrtime;
#ifdef RTK_PLATFORM
    int32_t mFetchBlockFailedCnt;
    static const int32_t kWaitFetchBlockFailedCnt;
    bool mIsBufferFilled;
    int32_t mDownloadErrCount;
    int32_t mDownloadOverTimeCnt;
    int32_t mSignalSeekTo;
    long long mDisconnectRxByte;
    int FindTsSyncByte(const sp<ABuffer>& buffer);

    int32_t mSkipNewPlaylistFlg;

    int32_t mStatusLineInfo;
    int32_t mStatusFlowInfo;
    struct timeval start_tv;
    struct timeval end_tv;
    struct timeval mSeqUpdate_tv;
    void SetLineInfo(int32_t dLineInfo, int32_t dFlowInfo = kPlaylistStatus_Normal);
    void StatusUpdateRecTime();
    unsigned int StatusTakeDurTime(struct timeval *start_tv);
    void StatusUpdatemSeqNumber();

#define MEDIA_AUDIO 0
#define MEDIA_VIDEO 1
#define MEDIA_TIME_START_STORE_NUM 5
#define MEDIA_TIME_NUM 20
#define MEDIA_DISCONTINUITY_THLD (5*1E6)
    struct strSeqInfo
    {
        int32_t mCurSeqNumber;
        int32_t mIdx;
        int64_t mTimeUs[MEDIA_TIME_NUM];
        int64_t mLvSnLastMedTimeUs;//LiveSession last media timeUs;
        bool    mIsNeedRebase;
        int64_t mOffsetUs;
        int64_t mdurationUs;
    };
    Vector<strSeqInfo> mStrSeqQue[2];
    strSeqInfo mCurSeqInfo[2];
    int64_t mSeqQTotalUs[2];
    void initSeqQue();
    void QueueSeqInfo(ssize_t streamIdx);
    int64_t diff(int64_t num1, int64_t num2);
    void initCurSeqInfo(int64_t durationUs);
    void QueueSeqInfoPernSec(LiveSession::StreamType stream, int64_t timeUs);
    ssize_t typeToIndex(int32_t type);
    int32_t chkIsSeqDiscontinuity_(ssize_t streamIdx, int32_t curSeqNumber, int64_t timeUs, bool *IsSeqBoundary);
    AString mPreUri;
    bool ChkNewSeqNumIsNotInRange(
            int32_t newfirstSeqNumberInPlaylist, int32_t newlastSeqNumberInPlaylist,
            int32_t firstSeqNumberInPlaylist, int32_t lastSeqNumberInPlaylist);

#endif
};

}  // namespace android

#endif  // PLAYLIST_FETCHER_H_


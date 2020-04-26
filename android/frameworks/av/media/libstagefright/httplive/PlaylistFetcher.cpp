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

//#define LOG_NDEBUG 0
#define LOG_TAG "PlaylistFetcher"
#include <utils/Log.h>
#include <utils/misc.h>
#include <fcntl.h>

#include "PlaylistFetcher.h"
#include "HTTPDownloader.h"
#include "LiveSession.h"
#include "M3UParser.h"
#include "include/avc_utils.h"
#include "include/ID3.h"
#include "mpeg2ts/AnotherPacketSource.h"

#include <media/stagefright/foundation/ABitReader.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/Utils.h>

#include <ctype.h>
#include <inttypes.h>
#include <openssl/aes.h>

#define FLOGV(fmt, ...) ALOGV("[fetcher-%d] " fmt, mFetcherID, ##__VA_ARGS__)
#define FSLOGV(stream, fmt, ...) ALOGV("[fetcher-%d] [%s] " fmt, mFetcherID, \
         LiveSession::getNameForStream(stream), ##__VA_ARGS__)

#include <media/IMediaHTTPConnection.h>
#include <media/IMediaHTTPService.h>
#include <cutils/properties.h>
#include "../include/NuCachedSource2.h"
#include <media/stagefright/MediaHTTP.h>
//#define ENABLE_NUCACHESOURCE

namespace android {

// static
#ifdef RTK_PLATFORM
const int64_t PlaylistFetcher::kMinBufferedDurationUs = 120000000ll;
const int64_t PlaylistFetcher::kMinBufferedAudioDurationUs = 90000000ll;
#else
const int64_t PlaylistFetcher::kMinBufferedDurationUs = 30000000ll;
#endif

const int64_t PlaylistFetcher::kMaxMonitorDelayUs = 3000000ll;
// LCM of 188 (size of a TS packet) & 1k works well
#ifdef RTK_PLATFORM
const int32_t PlaylistFetcher::kDownloadBlockSize = 16 * 1024;
#else
const int32_t PlaylistFetcher::kDownloadBlockSize = 47 * 1024;
#endif

const int32_t PlaylistFetcher::kWaitDownloadErrTime = 3000;
const int32_t PlaylistFetcher::kWaitDownloadErrIniCount = 10;
const int32_t PlaylistFetcher::kWaitFetchBlockFailedCnt = 15;

struct PlaylistFetcher::DownloadState : public RefBase {
    DownloadState();
    void resetState();
    bool hasSavedState() const;
    void restoreState(
            AString &uri,
            sp<AMessage> &itemMeta,
            sp<ABuffer> &buffer,
            sp<ABuffer> &tsBuffer,
            int32_t &firstSeqNumberInPlaylist,
            int32_t &lastSeqNumberInPlaylist);
    void saveState(
            AString &uri,
            sp<AMessage> &itemMeta,
            sp<ABuffer> &buffer,
            sp<ABuffer> &tsBuffer,
            int32_t &firstSeqNumberInPlaylist,
            int32_t &lastSeqNumberInPlaylist);

private:
    bool mHasSavedState;
    AString mUri;
    sp<AMessage> mItemMeta;
    sp<ABuffer> mBuffer;
    sp<ABuffer> mTsBuffer;
    int32_t mFirstSeqNumberInPlaylist;
    int32_t mLastSeqNumberInPlaylist;
};

#ifdef RTK_PLATFORM
int _Getcarrier(void);
long long _GetrxByte(int);
#endif

PlaylistFetcher::DownloadState::DownloadState() {
    resetState();
}

bool PlaylistFetcher::DownloadState::hasSavedState() const {
    return mHasSavedState;
}

void PlaylistFetcher::DownloadState::resetState() {
    mHasSavedState = false;

    mUri.clear();
    mItemMeta = NULL;
    mBuffer = NULL;
    mTsBuffer = NULL;
    mFirstSeqNumberInPlaylist = 0;
    mLastSeqNumberInPlaylist = 0;
}

void PlaylistFetcher::DownloadState::restoreState(
        AString &uri,
        sp<AMessage> &itemMeta,
        sp<ABuffer> &buffer,
        sp<ABuffer> &tsBuffer,
        int32_t &firstSeqNumberInPlaylist,
        int32_t &lastSeqNumberInPlaylist) {
    if (!mHasSavedState) {
        return;
    }

    uri = mUri;
    itemMeta = mItemMeta;
    buffer = mBuffer;
    tsBuffer = mTsBuffer;
    firstSeqNumberInPlaylist = mFirstSeqNumberInPlaylist;
    lastSeqNumberInPlaylist = mLastSeqNumberInPlaylist;

    resetState();
}

void PlaylistFetcher::DownloadState::saveState(
        AString &uri,
        sp<AMessage> &itemMeta,
        sp<ABuffer> &buffer,
        sp<ABuffer> &tsBuffer,
        int32_t &firstSeqNumberInPlaylist,
        int32_t &lastSeqNumberInPlaylist) {
    mHasSavedState = true;

    mUri = uri;
    mItemMeta = itemMeta;
    mBuffer = buffer;
    mTsBuffer = tsBuffer;
    mFirstSeqNumberInPlaylist = firstSeqNumberInPlaylist;
    mLastSeqNumberInPlaylist = lastSeqNumberInPlaylist;
}

PlaylistFetcher::PlaylistFetcher(
        const sp<AMessage> &notify,
        const sp<LiveSession> &session,
        const char *uri,
        int32_t id,
        int32_t subtitleGeneration)
    : mNotify(notify),
      mSession(session),
      mURI(uri),
      mFetcherID(id),
      mStreamTypeMask(0),
      mStartTimeUs(-1ll),
      mSegmentStartTimeUs(-1ll),
      mDiscontinuitySeq(-1ll),
      mStartTimeUsRelative(false),
      mLastPlaylistFetchTimeUs(-1ll),
      mPlaylistTimeUs(-1ll),
      mSeqNumber(-1),
      mNumRetries(0),
      mStartup(true),
      mIDRFound(false),
      mSeekMode(LiveSession::kSeekModeExactPosition),
      mTimeChangeSignaled(false),
      mNextPTSTimeUs(-1ll),
      mMonitorQueueGeneration(0),
      mSubtitleGeneration(subtitleGeneration),
      mLastDiscontinuitySeq(-1ll),
      mRefreshState(INITIAL_MINIMUM_RELOAD_DELAY),
      mFirstPTSValid(false),
      mFirstTimeUs(-1ll),
      mVideoBuffer(new AnotherPacketSource(NULL)),
      mThresholdRatio(-1.0f),
      mDownloadState(new DownloadState()),
      mHasMetadata(false),
      mUseRTKExtFlow(0)
      {
    memset(mPlaylistHash, 0, sizeof(mPlaylistHash));
    mHTTPDownloader = mSession->getHTTPDownloader();
#ifdef RTK_PLATFORM
    gettimeofday(&mDownloadErrtime, NULL);
    mFetchBlockFailedCnt = 0;
    mIsBufferFilled = false;
    mDownloadErrCount = 0;
    mDownloadOverTimeCnt = 0;
    onRTKCustomerHLSerrCntChk();
    mSignalSeekTo = 0;
    mDisconnectRxByte = 0;
    mSkipNewPlaylistFlg = 0;
    mPlaylist = NULL;
    SetLineInfo(__LINE__);
    StatusUpdateRecTime();
    initSeqQue();
    StatusUpdatemSeqNumber();
#endif

}

PlaylistFetcher::~PlaylistFetcher() {
}

int32_t PlaylistFetcher::getFetcherID() const {
    return mFetcherID;
}

int64_t PlaylistFetcher::getSegmentStartTimeUs(int32_t seqNumber) const {
    CHECK(mPlaylist != NULL);

    int32_t firstSeqNumberInPlaylist, lastSeqNumberInPlaylist;
    mPlaylist->getSeqNumberRange(
            &firstSeqNumberInPlaylist, &lastSeqNumberInPlaylist);

#ifdef RTK_PLATFORM
    if(lastSeqNumberInPlaylist == -1)
        return -1ll;
#endif
    CHECK_GE(seqNumber, firstSeqNumberInPlaylist);
    CHECK_LE(seqNumber, lastSeqNumberInPlaylist);

    int64_t segmentStartUs = 0ll;
    for (int32_t index = 0;
            index < seqNumber - firstSeqNumberInPlaylist; ++index) {
        sp<AMessage> itemMeta;
        CHECK(mPlaylist->itemAt(
                    index, NULL /* uri */, &itemMeta));

        int64_t itemDurationUs;
        CHECK(itemMeta->findInt64("durationUs", &itemDurationUs));

        segmentStartUs += itemDurationUs;
    }

    return segmentStartUs;
}

int64_t PlaylistFetcher::getSegmentDurationUs(int32_t seqNumber) const {
    CHECK(mPlaylist != NULL);

    int32_t firstSeqNumberInPlaylist, lastSeqNumberInPlaylist;
    mPlaylist->getSeqNumberRange(
            &firstSeqNumberInPlaylist, &lastSeqNumberInPlaylist);

    CHECK_GE(seqNumber, firstSeqNumberInPlaylist);
    CHECK_LE(seqNumber, lastSeqNumberInPlaylist);

    int32_t index = seqNumber - firstSeqNumberInPlaylist;
    sp<AMessage> itemMeta;
    CHECK(mPlaylist->itemAt(
                index, NULL /* uri */, &itemMeta));

    int64_t itemDurationUs;
    CHECK(itemMeta->findInt64("durationUs", &itemDurationUs));

    return itemDurationUs;
}

int64_t PlaylistFetcher::delayUsToRefreshPlaylist() const {
    int64_t nowUs = ALooper::GetNowUs();

    if (mPlaylist == NULL || mLastPlaylistFetchTimeUs < 0ll) {
        CHECK_EQ((int)mRefreshState, (int)INITIAL_MINIMUM_RELOAD_DELAY);
        return 0ll;
    }

    if (mPlaylist->isComplete()) {
        return (~0llu >> 1);
    }

    int64_t targetDurationUs = mPlaylist->getTargetDuration();

    int64_t minPlaylistAgeUs;

    switch (mRefreshState) {
        case INITIAL_MINIMUM_RELOAD_DELAY:
        {
            size_t n = mPlaylist->size();
            if (n > 0) {
                sp<AMessage> itemMeta;
                CHECK(mPlaylist->itemAt(n - 1, NULL /* uri */, &itemMeta));

                int64_t itemDurationUs;
                CHECK(itemMeta->findInt64("durationUs", &itemDurationUs));

                minPlaylistAgeUs = itemDurationUs;
                break;
            }

            // fall through
        }

        case FIRST_UNCHANGED_RELOAD_ATTEMPT:
        {
            minPlaylistAgeUs = targetDurationUs / 2;
            break;
        }

        case SECOND_UNCHANGED_RELOAD_ATTEMPT:
        {
            minPlaylistAgeUs = (targetDurationUs * 3) / 2;
            break;
        }

        case THIRD_UNCHANGED_RELOAD_ATTEMPT:
        {
            minPlaylistAgeUs = targetDurationUs * 3;
            break;
        }

        default:
            TRESPASS();
            break;
    }

    int64_t delayUs = mLastPlaylistFetchTimeUs + minPlaylistAgeUs - nowUs;
#ifdef RTK_PLATFORM
    if(delayUs > kMaxMonitorDelayUs)
    {
        if(delayUs > kMaxMonitorDelayUs * 10)
            ALOGW("[WANG]delayUsToRefreshPlaylist pausing for %lld to %lld, targetDurationUs,=%lld, mRefreshState::%d\n",
                    (long long)delayUs, (long long)kMaxMonitorDelayUs, (long long)targetDurationUs, mRefreshState);

        delayUs = kMaxMonitorDelayUs;
    }
#endif
    return delayUs > 0ll ? delayUs : 0ll;
}

status_t PlaylistFetcher::decryptBuffer(
        size_t playlistIndex, const sp<ABuffer> &buffer,
        bool first) {
    sp<AMessage> itemMeta;
    bool found = false;
    AString method;

    for (ssize_t i = playlistIndex; i >= 0; --i) {
        AString uri;
        CHECK(mPlaylist->itemAt(i, &uri, &itemMeta));

        if (itemMeta->findString("cipher-method", &method)) {
            found = true;
            break;
        }
    }

    if (!found) {
        method = "NONE";
    }
    buffer->meta()->setString("cipher-method", method.c_str());

    if (method == "NONE") {
        return OK;
    } else if (!(method == "AES-128")) {
        ALOGE("Unsupported cipher method '%s'", method.c_str());
        return ERROR_UNSUPPORTED;
    }

    AString keyURI;
    if (!itemMeta->findString("cipher-uri", &keyURI)) {
        ALOGE("Missing key uri");
        return ERROR_MALFORMED;
    }

    ssize_t index = mAESKeyForURI.indexOfKey(keyURI);

    sp<ABuffer> key;
    if (index >= 0) {
        key = mAESKeyForURI.valueAt(index);
    } else {
        ssize_t err = mHTTPDownloader->fetchFile(keyURI.c_str(), &key);

        if (err == ERROR_NOT_CONNECTED) {
            return ERROR_NOT_CONNECTED;
        } else if (err < 0) {
            ALOGE("failed to fetch cipher key from '%s'.", keyURI.c_str());
            return ERROR_IO;
        } else if (key->size() != 16) {
            ALOGE("key file '%s' wasn't 16 bytes in size.", keyURI.c_str());
            return ERROR_MALFORMED;
        }

        mAESKeyForURI.add(keyURI, key);
    }

    AES_KEY aes_key;
    if (AES_set_decrypt_key(key->data(), 128, &aes_key) != 0) {
        ALOGE("failed to set AES decryption key.");
        return UNKNOWN_ERROR;
    }

    size_t n = buffer->size();
    if (!n) {
        return OK;
    }

    if (n < 16 || n % 16) {
        ALOGE("not enough or trailing bytes (%zu) in encrypted buffer", n);
        return ERROR_MALFORMED;
    }

    if (first) {
        // If decrypting the first block in a file, read the iv from the manifest
        // or derive the iv from the file's sequence number.

        AString iv;
        if (itemMeta->findString("cipher-iv", &iv)) {
            if ((!iv.startsWith("0x") && !iv.startsWith("0X"))
                    || iv.size() > 16 * 2 + 2) {
                ALOGE("malformed cipher IV '%s'.", iv.c_str());
                return ERROR_MALFORMED;
            }

            while (iv.size() < 16 * 2 + 2) {
                iv.insert("0", 1, 2);
            }

            memset(mAESInitVec, 0, sizeof(mAESInitVec));
            for (size_t i = 0; i < 16; ++i) {
                char c1 = tolower(iv.c_str()[2 + 2 * i]);
                char c2 = tolower(iv.c_str()[3 + 2 * i]);
                if (!isxdigit(c1) || !isxdigit(c2)) {
                    ALOGE("malformed cipher IV '%s'.", iv.c_str());
                    return ERROR_MALFORMED;
                }
                uint8_t nibble1 = isdigit(c1) ? c1 - '0' : c1 - 'a' + 10;
                uint8_t nibble2 = isdigit(c2) ? c2 - '0' : c2 - 'a' + 10;

                mAESInitVec[i] = nibble1 << 4 | nibble2;
            }
        } else {
            memset(mAESInitVec, 0, sizeof(mAESInitVec));
            mAESInitVec[15] = mSeqNumber & 0xff;
            mAESInitVec[14] = (mSeqNumber >> 8) & 0xff;
            mAESInitVec[13] = (mSeqNumber >> 16) & 0xff;
            mAESInitVec[12] = (mSeqNumber >> 24) & 0xff;
        }
    }

    AES_cbc_encrypt(
            buffer->data(), buffer->data(), buffer->size(),
            &aes_key, mAESInitVec, AES_DECRYPT);

    return OK;
}

status_t PlaylistFetcher::checkDecryptPadding(const sp<ABuffer> &buffer) {
    AString method;
    CHECK(buffer->meta()->findString("cipher-method", &method));
    if (method == "NONE") {
        return OK;
    }

    uint8_t padding = 0;
    if (buffer->size() > 0) {
        padding = buffer->data()[buffer->size() - 1];
    }

    if (padding > 16) {
        return ERROR_MALFORMED;
    }

    for (size_t i = buffer->size() - padding; i < padding; i++) {
        if (buffer->data()[i] != padding) {
            return ERROR_MALFORMED;
        }
    }

    buffer->setRange(buffer->offset(), buffer->size() - padding);
    return OK;
}

void PlaylistFetcher::postMonitorQueue(int64_t delayUs, int64_t minDelayUs) {
    int64_t maxDelayUs = delayUsToRefreshPlaylist();
    if (maxDelayUs < minDelayUs) {
        maxDelayUs = minDelayUs;
    }
    if (delayUs > maxDelayUs) {
        FLOGV("Need to refresh playlist in %lld", (long long)maxDelayUs);
        delayUs = maxDelayUs;
    }
    sp<AMessage> msg = new AMessage(kWhatMonitorQueue, this);
    msg->setInt32("generation", mMonitorQueueGeneration);
    msg->post(delayUs);
}

void PlaylistFetcher::cancelMonitorQueue() {
    ++mMonitorQueueGeneration;
}

void PlaylistFetcher::setStoppingThreshold(float thresholdRatio, bool disconnect) {
    {
        AutoMutex _l(mThresholdLock);
        mThresholdRatio = thresholdRatio;
        ALOGI("[%s][%d] thresholdRatio set :: %f  \n ",__FUNCTION__,__LINE__,thresholdRatio);
#ifdef RTK_PLATFORM
        initSeqQue();
#endif
    }
    if (disconnect) {
        ALOGW("[%s][%d] mHTTPDownloader->disconnect(); \n ",__FUNCTION__,__LINE__);
        mHTTPDownloader->disconnect();
    }
}

void PlaylistFetcher::resetStoppingThreshold(bool disconnect) {
    {
        AutoMutex _l(mThresholdLock);
        mThresholdRatio = -1.0f;
    }
    if (disconnect) {
        mHTTPDownloader->disconnect();
    } else {
        // allow reconnect
        mHTTPDownloader->reconnect();
    }
}

float PlaylistFetcher::getStoppingThreshold() {
    AutoMutex _l(mThresholdLock);
    return mThresholdRatio;
}

void PlaylistFetcher::startAsync(
        const sp<AnotherPacketSource> &audioSource,
        const sp<AnotherPacketSource> &videoSource,
        const sp<AnotherPacketSource> &subtitleSource,
        const sp<AnotherPacketSource> &metadataSource,
        int64_t startTimeUs,
        int64_t segmentStartTimeUs,
        int32_t startDiscontinuitySeq,
#ifdef RTK_PLATFORM
        bool bIsBandwidthSwitchFlw,
#endif
        LiveSession::SeekMode seekMode) {
    sp<AMessage> msg = new AMessage(kWhatStart, this);

    uint32_t streamTypeMask = 0ul;

    if (audioSource != NULL) {
        msg->setPointer("audioSource", audioSource.get());
        streamTypeMask |= LiveSession::STREAMTYPE_AUDIO;
    }

    if (videoSource != NULL) {
        msg->setPointer("videoSource", videoSource.get());
        streamTypeMask |= LiveSession::STREAMTYPE_VIDEO;
    }

    if (subtitleSource != NULL) {
        msg->setPointer("subtitleSource", subtitleSource.get());
        streamTypeMask |= LiveSession::STREAMTYPE_SUBTITLES;
    }

    if (metadataSource != NULL) {
        msg->setPointer("metadataSource", metadataSource.get());
        // metadataSource does not affect streamTypeMask.
    }

    msg->setInt32("streamTypeMask", streamTypeMask);
    msg->setInt64("startTimeUs", startTimeUs);
    msg->setInt64("segmentStartTimeUs", segmentStartTimeUs);
    msg->setInt32("startDiscontinuitySeq", startDiscontinuitySeq);
    msg->setInt32("seekMode", seekMode);
#ifdef RTK_PLATFORM
    msg->setInt32("IsBandwidthSwitchFlw", 0);
    if(bIsBandwidthSwitchFlw == true)
        msg->setInt32("IsBandwidthSwitchFlw", 1);
#endif
    msg->post();
}

/*
 * pauseAsync
 *
 * threshold: 0.0f - pause after current fetch block (default 47Kbytes)
 *           -1.0f - pause after finishing current segment
 *        0.0~1.0f - pause if remaining of current segment exceeds threshold
 */
void PlaylistFetcher::pauseAsync(
        float thresholdRatio, bool disconnect) {
    setStoppingThreshold(thresholdRatio, disconnect);

    (new AMessage(kWhatPause, this))->post();
}

void PlaylistFetcher::stopAsync(bool clear) {
    setStoppingThreshold(0.0f, true /* disconncect */);

    sp<AMessage> msg = new AMessage(kWhatStop, this);
    msg->setInt32("clear", clear);
    msg->post();
}

void PlaylistFetcher::resumeUntilAsync(const sp<AMessage> &params) {
    FLOGV("resumeUntilAsync: params=%s", params->debugString().c_str());

    AMessage* msg = new AMessage(kWhatResumeUntil, this);
    msg->setMessage("params", params);
    msg->post();
}

void PlaylistFetcher::fetchPlaylistAsync() {
    (new AMessage(kWhatFetchPlaylist, this))->post();
}

void PlaylistFetcher::onMessageReceived(const sp<AMessage> &msg) {
    switch (msg->what()) {
        case kWhatStart:
        {
            status_t err = onStart(msg);

            sp<AMessage> notify = mNotify->dup();
            notify->setInt32("what", kWhatStarted);
            notify->setInt32("err", err);
            notify->post();
            break;
        }

        case kWhatPause:
        {
            onPause();

            sp<AMessage> notify = mNotify->dup();
            notify->setInt32("what", kWhatPaused);
            notify->setInt32("seekMode",
                    mDownloadState->hasSavedState()
                    ? LiveSession::kSeekModeNextSample
                    : LiveSession::kSeekModeNextSegment);
            notify->post();
            mSignalSeekTo = 0;
            break;
        }

        case kWhatStop:
        {
            onStop(msg);

            sp<AMessage> notify = mNotify->dup();
            notify->setInt32("what", kWhatStopped);
            notify->post();
            break;
        }

        case kWhatFetchPlaylist:
        {
            bool unchanged;
#ifdef RTK_PLATFORM
            int32_t dRetryCount = 0;
            sp<M3UParser> playlist = NULL;
            onRTKCustomerHLSerrCntChk();
            while(dRetryCount <= kWaitDownloadErrCount)
            {
                float thresholdRatio = getStoppingThreshold();

                if (thresholdRatio == 0.0f) {
                    // immediately abort
                    ALOGI("[%s][%d] shouldPauseDownload ret true  \n",__FUNCTION__,__LINE__);
                    break;
                }
                int64_t startTimeUs = ALooper::GetNowUs();
                int64_t endTimeUs = 0;
                playlist = mHTTPDownloader->fetchPlaylist(
                        mURI.c_str(), NULL /* curPlaylistHash */, &unchanged);
                if(playlist!=NULL)
                    break;

                endTimeUs = ALooper::GetNowUs();
                if(endTimeUs - startTimeUs > 8 * 1E6)
                {
                    ALOGW("[%s][%d] fetchPlaylist over time! Http connect time out error.\n",__FUNCTION__,__LINE__);
                    break;
                }

                dRetryCount++;
                ALOGW("[%s][%d] kWhatFetchPlaylist: connet fail retry:%d time\n",__FUNCTION__,__LINE__,dRetryCount);
            }
#else
            sp<M3UParser> playlist = mHTTPDownloader->fetchPlaylist(
                    mURI.c_str(), NULL /* curPlaylistHash */, &unchanged);
#endif

            sp<AMessage> notify = mNotify->dup();
            notify->setInt32("what", kWhatPlaylistFetched);
            notify->setObject("playlist", playlist);
            notify->post();
            break;
        }

        case kWhatMonitorQueue:
        case kWhatDownloadNext:
        {
            int32_t generation;
            CHECK(msg->findInt32("generation", &generation));

            if (generation != mMonitorQueueGeneration) {
                // Stale event
                break;
            }

            if (msg->what() == kWhatMonitorQueue) {
                onMonitorQueue();
            } else {
                onDownloadNext();
            }
            break;
        }

        case kWhatResumeUntil:
        {
            onResumeUntil(msg);
            break;
        }

        default:
            TRESPASS();
    }
}

status_t PlaylistFetcher::onStart(const sp<AMessage> &msg) {
    mPacketSources.clear();
    mStopParams.clear();
#ifdef RTK_PLATFORM
    int32_t IsBandwidthSwitchFlw = 0;
    msg->findInt32("IsBandwidthSwitchFlw", &IsBandwidthSwitchFlw);
    if(mTSParser != NULL && IsBandwidthSwitchFlw == 0)
    {
        mTSParser->signalDiscontinuity(ATSParser::DISCONTINUITY_SEEK, NULL);
        mTSParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
    }
#endif
    mStartTimeUsNotify = mNotify->dup();
    mStartTimeUsNotify->setInt32("what", kWhatStartedAt);
    mStartTimeUsNotify->setString("uri", mURI);

    uint32_t streamTypeMask;
    CHECK(msg->findInt32("streamTypeMask", (int32_t *)&streamTypeMask));

    int64_t startTimeUs;
    int64_t segmentStartTimeUs;
    int32_t startDiscontinuitySeq;
    int32_t seekMode;
    CHECK(msg->findInt64("startTimeUs", &startTimeUs));
    CHECK(msg->findInt64("segmentStartTimeUs", &segmentStartTimeUs));
    CHECK(msg->findInt32("startDiscontinuitySeq", &startDiscontinuitySeq));
    CHECK(msg->findInt32("seekMode", &seekMode));

    if (streamTypeMask & LiveSession::STREAMTYPE_AUDIO) {
        void *ptr;
        CHECK(msg->findPointer("audioSource", &ptr));

        mPacketSources.add(
                LiveSession::STREAMTYPE_AUDIO,
                static_cast<AnotherPacketSource *>(ptr));
    }

    if (streamTypeMask & LiveSession::STREAMTYPE_VIDEO) {
        void *ptr;
        CHECK(msg->findPointer("videoSource", &ptr));

        mPacketSources.add(
                LiveSession::STREAMTYPE_VIDEO,
                static_cast<AnotherPacketSource *>(ptr));
    }

    if (streamTypeMask & LiveSession::STREAMTYPE_SUBTITLES) {
        void *ptr;
        CHECK(msg->findPointer("subtitleSource", &ptr));

        mPacketSources.add(
                LiveSession::STREAMTYPE_SUBTITLES,
                static_cast<AnotherPacketSource *>(ptr));
    }

    void *ptr;
    // metadataSource is not part of streamTypeMask
    if ((streamTypeMask & (LiveSession::STREAMTYPE_AUDIO | LiveSession::STREAMTYPE_VIDEO))
            && msg->findPointer("metadataSource", &ptr)) {
        mPacketSources.add(
                LiveSession::STREAMTYPE_METADATA,
                static_cast<AnotherPacketSource *>(ptr));
    }

    mStreamTypeMask = streamTypeMask;

    mSegmentStartTimeUs = segmentStartTimeUs;

    if (startDiscontinuitySeq >= 0) {
        mDiscontinuitySeq = startDiscontinuitySeq;
    }

    mRefreshState = INITIAL_MINIMUM_RELOAD_DELAY;
    mSeekMode = (LiveSession::SeekMode) seekMode;

    if (startTimeUs >= 0 || mSeekMode == LiveSession::kSeekModeNextSample) {
        mStartup = true;
        mIDRFound = false;
        mVideoBuffer->clear();
    }

#ifdef RTK_PLATFORM
    if(mPlaylist != NULL && !mPlaylist->isComplete() && IsBandwidthSwitchFlw == 0)//Live streaming
        startTimeUs = 0;
#endif
    if (startTimeUs >= 0) {
        mStartTimeUs = startTimeUs;
        mFirstPTSValid = false;
        mSeqNumber = -1;
        mTimeChangeSignaled = false;
        mDownloadState->resetState();
    }

    postMonitorQueue();

    return OK;
}

void PlaylistFetcher::onPause() {
    cancelMonitorQueue();
    mLastDiscontinuitySeq = mDiscontinuitySeq;

    resetStoppingThreshold(false /* disconnect */);
}

void PlaylistFetcher::onStop(const sp<AMessage> &msg) {
    cancelMonitorQueue();

    int32_t clear;
    CHECK(msg->findInt32("clear", &clear));
    if (clear) {
        for (size_t i = 0; i < mPacketSources.size(); i++) {
            sp<AnotherPacketSource> packetSource = mPacketSources.valueAt(i);
            packetSource->clear();
        }
    }

    mDownloadState->resetState();
    mPacketSources.clear();
    mStreamTypeMask = 0;

    resetStoppingThreshold(true /* disconnect */);
}

// Resume until we have reached the boundary timestamps listed in `msg`; when
// the remaining time is too short (within a resume threshold) stop immediately
// instead.
status_t PlaylistFetcher::onResumeUntil(const sp<AMessage> &msg) {
    sp<AMessage> params;
    CHECK(msg->findMessage("params", &params));

    mStopParams = params;
    onDownloadNext();

    return OK;
}

void PlaylistFetcher::notifyStopReached() {
    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", kWhatStopReached);
    notify->post();
}

void PlaylistFetcher::notifyError(status_t err) {
    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", kWhatError);
    notify->setInt32("err", err);
    notify->post();
}

void PlaylistFetcher::queueDiscontinuity(
        ATSParser::DiscontinuityType type, const sp<AMessage> &extra) {
    for (size_t i = 0; i < mPacketSources.size(); ++i) {
        // do not discard buffer upon #EXT-X-DISCONTINUITY tag
        // (seek will discard buffer by abandoning old fetchers)
        mPacketSources.valueAt(i)->queueDiscontinuity(
                type, extra, false /* discard */);
    }
}

void PlaylistFetcher::onMonitorQueue() {
    // in the middle of an unfinished download, delay
    // playlist refresh as it'll change seq numbers

#ifdef RTK_PLATFORM
    status_t err = OK;
    if (!mDownloadState->hasSavedState())
    {
        err = refreshPlaylist();
        if(err != OK && (mSeqNumber > 0 ||mSkipNewPlaylistFlg > kMaxNumRetries)
                && !mPlaylist->isComplete())
        {
            float thresholdRatio = getStoppingThreshold();
            if (thresholdRatio == 0.0f)
            {
                // immediately abort
                ALOGI("[%s][%d] shouldPauseDownload ret true  \n",__FUNCTION__,__LINE__);
                return;
            }
            if(mSkipNewPlaylistFlg > kMaxNumRetries)
            {
                ALOGI("[%s][%d] stop PlaylistFetcher and return err.\n",__FUNCTION__,__LINE__);
                notifyError(err);
                return;
            }
        }
    }
#else
    if (!mDownloadState->hasSavedState()) {
        refreshPlaylist();
    }
#endif

    int64_t targetDurationUs = kMinBufferedDurationUs;
    if (mPlaylist != NULL) {
        targetDurationUs = mPlaylist->getTargetDuration();
    }

    int64_t bufferedDurationUs = 0ll;
#ifdef RTK_PLATFORM
    int64_t bufferedAudioDurationUs = 0ll;
#endif
    status_t finalResult = OK;
    if (mStreamTypeMask == LiveSession::STREAMTYPE_SUBTITLES) {
        sp<AnotherPacketSource> packetSource =
            mPacketSources.valueFor(LiveSession::STREAMTYPE_SUBTITLES);

        bufferedDurationUs =
                packetSource->getBufferedDurationUs(&finalResult);
    } else {
        // Use min stream duration, but ignore streams that never have any packet
        // enqueued to prevent us from waiting on a non-existent stream;
        // when we cannot make out from the manifest what streams are included in
        // a playlist we might assume extra streams.
        bufferedDurationUs = -1ll;
        for (size_t i = 0; i < mPacketSources.size(); ++i) {
            if ((mStreamTypeMask & mPacketSources.keyAt(i)) == 0
                    || mPacketSources[i]->getLatestEnqueuedMeta() == NULL) {
                continue;
            }

            int64_t bufferedStreamDurationUs =
                mPacketSources.valueAt(i)->getBufferedDurationUs(&finalResult);

            FSLOGV(mPacketSources.keyAt(i), "buffered %lld", (long long)bufferedStreamDurationUs);

#ifdef RTK_PLATFORM
            // if audio > 10s && video > 30s , then start playback.
            if(i == LiveSession::STREAMTYPE_AUDIO)
                bufferedAudioDurationUs = bufferedStreamDurationUs;
            else
                bufferedDurationUs = bufferedStreamDurationUs;
#else
            if (bufferedDurationUs == -1ll
                 || bufferedStreamDurationUs < bufferedDurationUs) {
                bufferedDurationUs = bufferedStreamDurationUs;
            }
#endif
        }
        if (bufferedDurationUs == -1ll) {
            bufferedDurationUs = 0ll;
        }
    }

#ifdef RTK_PLATFORM
    if (finalResult == OK &&( bufferedDurationUs < kMinBufferedDurationUs
            || bufferedAudioDurationUs < kMinBufferedAudioDurationUs || mSeqNumber == -1))
#else
    if (finalResult == OK && bufferedDurationUs < kMinBufferedDurationUs)
#endif
    {
        FLOGV("monitoring, buffered=%lld < %lld",
                (long long)bufferedDurationUs, (long long)kMinBufferedDurationUs);

        // delay the next download slightly; hopefully this gives other concurrent fetchers
        // a better chance to run.
        // onDownloadNext();
        sp<AMessage> msg = new AMessage(kWhatDownloadNext, this);
        msg->setInt32("generation", mMonitorQueueGeneration);
#ifdef RTK_PLATFORM
        mIsBufferFilled = false;
        StatusUpdatemSeqNumber();
        if(err != OK && bufferedDurationUs > kMaxMonitorDelayUs)
            msg->post(1000000l);
        else
            msg->post(1000l);
#else
        msg->post(1000l);
#endif
    } else {
        // We'd like to maintain buffering above durationToBufferUs, so try
        // again when buffer just about to go below durationToBufferUs
        // (or after targetDurationUs / 2, whichever is smaller).
        int64_t delayUs = bufferedDurationUs - kMinBufferedDurationUs + 1000000ll;
        if (delayUs > targetDurationUs / 2) {
            delayUs = targetDurationUs / 2;
        }

        FLOGV("pausing for %lld, buffered=%lld > %lld",
                (long long)delayUs,
                (long long)bufferedDurationUs,
                (long long)kMinBufferedDurationUs);

#ifdef RTK_PLATFORM
        if(delayUs > kMaxMonitorDelayUs)
        {
            ALOGW("[WANG]pausing for %lld, buffered=%lld > %lld",
                    (long long)delayUs,
                    (long long)bufferedDurationUs,
                    (long long)kMinBufferedDurationUs);
            delayUs = kMaxMonitorDelayUs;
            ALOGI("pausing for %lld, buffered=%lld > %lld",
                    (long long)delayUs,
                    (long long)bufferedDurationUs,
                    (long long)kMinBufferedDurationUs);
        }
        mIsBufferFilled = true;
#endif
        postMonitorQueue(delayUs);
    }
}
bool PlaylistFetcher::ChkNewSeqNumIsNotInRange(
        int32_t newfirstSeqNumberInPlaylist, int32_t newlastSeqNumberInPlaylist,
        int32_t firstSeqNumberInPlaylist, int32_t lastSeqNumberInPlaylist)
{
    if(mSeqNumber > 0 && newfirstSeqNumberInPlaylist == 0 && newlastSeqNumberInPlaylist == -1)
        return false;

    if(newfirstSeqNumberInPlaylist > lastSeqNumberInPlaylist)
        return false;

    if(newlastSeqNumberInPlaylist < firstSeqNumberInPlaylist)
        return true;

    if(((uint64_t)newfirstSeqNumberInPlaylist + (uint64_t)newlastSeqNumberInPlaylist) <
            ((uint64_t)firstSeqNumberInPlaylist + (uint64_t)lastSeqNumberInPlaylist))
    {
        ALOGW("[%s][%d] Error SeqNumberInPlaylist:: old:: %d, %d, new:: %d, %d.\n",__FUNCTION__,__LINE__,
                firstSeqNumberInPlaylist, lastSeqNumberInPlaylist,
                newfirstSeqNumberInPlaylist, newlastSeqNumberInPlaylist);
    }

    return false;
}
status_t PlaylistFetcher::refreshPlaylist() {
#ifdef RTK_PLATFORM
    if (delayUsToRefreshPlaylist() <= 0 || mSkipNewPlaylistFlg > 0)
#else
    if (delayUsToRefreshPlaylist() <= 0)
#endif
    {
        bool unchanged;

#ifdef RTK_PLATFORM
        sp<M3UParser> playlist;
        if(_Getcarrier() == 0)
        {
            mDisconnectRxByte = _GetrxByte(_Getcarrier());
            return OK;
        }
        if(mDisconnectRxByte > 0 && _Getcarrier())
        {
            if(_GetrxByte(_Getcarrier()) > mDisconnectRxByte)
            {
                mDisconnectRxByte = 0;
                playlist = mHTTPDownloader->fetchPlaylist(
                        mURI.c_str(), mPlaylistHash, &unchanged);
            }
            else
                return OK;
        }
        else
            playlist = mHTTPDownloader->fetchPlaylist(
                    mURI.c_str(), mPlaylistHash, &unchanged);
#else
        sp<M3UParser> playlist = mHTTPDownloader->fetchPlaylist(
                mURI.c_str(), mPlaylistHash, &unchanged);
#endif

#ifdef RTK_PLATFORM
        int32_t newfirstSeqNumberInPlaylist = 0, newlastSeqNumberInPlaylist = -1;
        int32_t firstSeqNumberInPlaylist, lastSeqNumberInPlaylist;
        if(mPlaylist != NULL)
        {
            //check the playlist is valid or not.
            mPlaylist->getSeqNumberRange(
                    &firstSeqNumberInPlaylist, &lastSeqNumberInPlaylist);

            if(playlist != NULL)
            playlist->getSeqNumberRange(
                    &newfirstSeqNumberInPlaylist, &newlastSeqNumberInPlaylist);

            if(newfirstSeqNumberInPlaylist == 0 && newlastSeqNumberInPlaylist == -1 &&
                    lastSeqNumberInPlaylist > firstSeqNumberInPlaylist)
            {
                ALOGI("[%s][%d] old: %d  %d new:%d %d \n",__FUNCTION__,__LINE__, firstSeqNumberInPlaylist, lastSeqNumberInPlaylist,
                        newfirstSeqNumberInPlaylist, newlastSeqNumberInPlaylist);
                ALOGW("New playlist is invalid. Skip it. \n");
                mSkipNewPlaylistFlg++;
            }
            else if(ChkNewSeqNumIsNotInRange(newfirstSeqNumberInPlaylist, newlastSeqNumberInPlaylist, firstSeqNumberInPlaylist, lastSeqNumberInPlaylist) == true)
            {
                ALOGE("[%s][%d] Error SeqNumberInPlaylist:: old:: %d, %d, new:: %d, %d.\n",__FUNCTION__,__LINE__,
                        firstSeqNumberInPlaylist, lastSeqNumberInPlaylist,
                        newfirstSeqNumberInPlaylist, newlastSeqNumberInPlaylist);
                ALOGE("[%s][%d]notifyError to player.\n",__FUNCTION__,__LINE__);
                notifyError(ERROR_IO);
                return ERROR_IO;
            }
            else
            {
                if(mSkipNewPlaylistFlg > 0)
                {
                    ALOGI("[%s][%d] old: %d  %d new:%d %d \n",__FUNCTION__,__LINE__, firstSeqNumberInPlaylist, lastSeqNumberInPlaylist,
                            newfirstSeqNumberInPlaylist, newlastSeqNumberInPlaylist);
                }
                mSkipNewPlaylistFlg = 0;
            }

            if(mSkipNewPlaylistFlg > kMaxNumRetries)
            {
                return ERROR_IO;
            }
        }

#endif
#ifdef RTK_PLATFORM
        if (playlist == NULL)
        {
            if (mPlaylist != NULL && mSeqNumber <= lastSeqNumberInPlaylist && mSeqNumber > 0)
            {
                unchanged = 1;
                ALOGD("[%s][%d] unchanged set\n",__FUNCTION__,__LINE__);
            }
        }
#endif
        if (playlist == NULL)
        {
            if (unchanged) {
                // We succeeded in fetching the playlist, but it was
                // unchanged from the last time we tried.

                if (mRefreshState != THIRD_UNCHANGED_RELOAD_ATTEMPT) {
                    mRefreshState = (RefreshState)(mRefreshState + 1);
                }
            } else {
                ALOGE("failed to load playlist at url '%s'", uriDebugString(mURI).c_str());
                ALOGE("failed to load playlist at url '%s'", mURI.c_str());
                return ERROR_IO;
            }
        } else {
            mRefreshState = INITIAL_MINIMUM_RELOAD_DELAY;
#ifdef RTK_PLATFORM
            if(mSkipNewPlaylistFlg == 0)
                mPlaylist = playlist;
#else
            mPlaylist = playlist;
#endif

            if (mPlaylist->isComplete() || mPlaylist->isEvent()) {
                updateDuration();
            }
            // Notify LiveSession to use target-duration based buffering level
            // for up/down switch. Default LiveSession::kUpSwitchMark may not
            // be reachable for live streams, as our max buffering amount is
            // limited to 3 segments.
            if (!mPlaylist->isComplete()) {
                updateTargetDuration();
            }
            mPlaylistTimeUs = ALooper::GetNowUs();
        }

        mLastPlaylistFetchTimeUs = ALooper::GetNowUs();
    }
    return OK;
}

#ifdef RTK_PLATFORM
int PlaylistFetcher::FindTsSyncByte(const sp<ABuffer>& buffer)
{
    int index = 0;
    uint32_t PacketSize = 188;
    if(buffer->size() > (3 * PacketSize))
    {
        const uint8_t *packetTempS = (const uint8_t *)(buffer->data());

        for (index = 0; index <= ((int)buffer->size() - (int)(3 * PacketSize)); index++)
        {
            if((packetTempS[index] == 0x47) && (packetTempS[index+ PacketSize] == 0x47) && (packetTempS[index+ PacketSize*2] == 0x47))
            {
                if(index > 0)
                {
                    ALOGV("%s find next sync word index::%d, buffer->size()::%d, buffer->capacity()::%d.,buffer->offset()::%d\n", __FUNCTION__, index, buffer->size(),buffer->capacity(),buffer->offset());

                    if(buffer->offset() + buffer->size() > buffer->capacity())
                        return -1;

                    if(index > (int)buffer->size())
                        return -1;
                }
                break;
            }
        }
    }
    return index;
}
#endif

// static
bool PlaylistFetcher::bufferStartsWithTsSyncByte(const sp<ABuffer>& buffer) {
    return buffer->size() > 0 && buffer->data()[0] == 0x47;
}


void PlaylistFetcher::resetDataSource()
{
#ifdef ENABLE_NUCACHESOURCE
    mCachedSource.clear();
#else
    mHTTPDataSource->disconnect();
    mHTTPDataSource.clear();
#endif
    mAudioTrack.mSource->stop();
    mAudioTrack.mSource=NULL;
    mVideoTrack.mSource->stop();
    mVideoTrack.mSource=NULL;
    mVideoTrack.mPackets->clear();
    mAudioTrack.mPackets->clear();
    mExtractor.clear();
}
status_t PlaylistFetcher::initFromDataSource(AString uri)
{
    status_t err = OK;
    sp<DataSource> source;
    String8 mContentType;

    mHTTPService = mSession->getHttphandle();
#ifdef ENABLE_NUCACHESOURCE
    mHttpSource = DataSource::CreateMediaHTTP(mHTTPService);
    mDataSource = DataSource::CreateFromURI(
            mHTTPService, uri.c_str() , &mUriHeaders, &mContentType,
            static_cast<HTTPBase *>(mHttpSource.get()));
    source = mDataSource;
    mCachedSource  = static_cast<NuCachedSource2 *>(mDataSource.get());
#else
    mHTTPDataSource = new MediaHTTP(mHTTPService->makeHTTPConnection());
    mHTTPDataSource->connect(String8(uri.c_str()), &mUriHeaders);
    source = mHTTPDataSource;
#endif

    dIsAudioEOF = 0;
    dIsVideoEOF = 0;

    mExtractor = MediaExtractor::Create(source,"RTK/HLS");
    if (mExtractor == NULL) {
        return ERROR_OUT_OF_RANGE;
    }
    ALOGV("\033[1;34m[PlaylistFetcher]\033[m[%d]:%s() Create OK  \n",__LINE__, __FUNCTION__);
    mFileMeta = mExtractor->getMetaData();
    if (mFileMeta != NULL) {
        int64_t duration;
        if (mFileMeta->findInt64(kKeyDuration, &duration)) {
            ALOGV("duration :: %lld \n" , (long long)duration);
            //            mDurationUs = duration;
        }

    }


    size_t numtracks = mExtractor->countTracks();
    if (numtracks == 0) {
        ALOGV("\033[1;34m[PlaylistFetcher]\033[m[%d]:%s() \n",__LINE__, __FUNCTION__);
        return ERROR_OUT_OF_RANGE;
    }

    ALOGV("\033[1;34m[PlaylistFetcher]\033[m[%d]:%s() numtracks::%d\n",__LINE__, __FUNCTION__, (int)numtracks);
    for (size_t i = 0; i < numtracks; ++i) {
        sp<IMediaSource> track = mExtractor->getTrack(i);
        sp<MetaData> meta = mExtractor->getTrackMetaData(i);

        const char *mime;
        CHECK(meta->findCString(kKeyMIMEType, &mime));
        if (!strncasecmp(mime, "audio/", 6)) {
            ALOGV("\033[1;34m[PlaylistFetcher]\033[m[%d]:%s() audio \n",__LINE__, __FUNCTION__);
            if (mAudioTrack.mSource == NULL) {
                mAudioTrack.mIndex = i;
                mAudioTrack.mSource = track;
                mAudioTrack.mPackets =
                    new AnotherPacketSource(mAudioTrack.mSource->getFormat());

                if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_VORBIS)) {
                    ALOGD("mAudioIsVorbis = true\n");
                    //         mAudioIsVorbis = true;
                }
            }
        } else if (!strncasecmp(mime, "video/", 6)) {
            ALOGV("\033[1;34m[PlaylistFetcher]\033[m[%d]:%s()  video \n",__LINE__, __FUNCTION__);
            if (mVideoTrack.mSource == NULL) {
                mVideoTrack.mIndex = i;
                mVideoTrack.mSource = track;
                mVideoTrack.mPackets =
                    new AnotherPacketSource(mVideoTrack.mSource->getFormat());

                int32_t secure;
                if (meta->findInt32(kKeyRequiresSecureBuffers, &secure)
                        && secure) {
                    ALOGD("mIsSecure = true;\n");
                    //       mIsSecure = true;
                    //     if (mUIDValid) {
                    //     mExtractor->setUID(mUID);
                    //   }
                }
            }
        }

    }

    ALOGV("SetDataSource OK \n");

    ALOGV("\033[0;32m[RtkExtSource]\033[m[%d]:%s()   start Sources\n",__LINE__, __FUNCTION__);
    if (mAudioTrack.mSource != NULL && mAudioTrack.mSource->start() != OK) {
        ALOGE("failed to start audio track!");
        return ERROR_OUT_OF_RANGE;
    }

    if (mVideoTrack.mSource != NULL && mVideoTrack.mSource->start() != OK) {
        ALOGE("failed to start video track!");
        return ERROR_OUT_OF_RANGE;
    }

    return err;
}


bool PlaylistFetcher::shouldPauseDownload() {
    if (mStreamTypeMask == LiveSession::STREAMTYPE_SUBTITLES) {
        // doesn't apply to subtitles
        return false;
    }

    // Calculate threshold to abort current download
    float thresholdRatio = getStoppingThreshold();

    if (thresholdRatio < 0.0f) {
        // never abort
        return false;
    } else if (thresholdRatio == 0.0f) {
        // immediately abort
        ALOGI("[%s][%d] shouldPauseDownload ret true  \n",__FUNCTION__,__LINE__);
        return true;
    }

    // now we have a positive thresholdUs, abort if remaining
    // portion to download is over that threshold.
    if (mSegmentFirstPTS < 0) {
        // this means we haven't even find the first access unit,
        // abort now as we must be very far away from the end.
        ALOGI("[%s][%d] shouldPauseDownload ret true  \n",__FUNCTION__,__LINE__);
        return true;
    }
    int64_t lastEnqueueUs = mSegmentFirstPTS;
    for (size_t i = 0; i < mPacketSources.size(); ++i) {
        if ((mStreamTypeMask & mPacketSources.keyAt(i)) == 0) {
            continue;
        }
        sp<AMessage> meta = mPacketSources[i]->getLatestEnqueuedMeta();
        int32_t type;
        if (meta == NULL || meta->findInt32("discontinuity", &type)) {
            continue;
        }
        int64_t tmpUs;
        CHECK(meta->findInt64("timeUs", &tmpUs));
        if (tmpUs > lastEnqueueUs) {
            lastEnqueueUs = tmpUs;
        }
    }
    lastEnqueueUs -= mSegmentFirstPTS;

    int64_t targetDurationUs = mPlaylist->getTargetDuration();
    int64_t thresholdUs = thresholdRatio * targetDurationUs;

    ALOGI("%spausing now, thresholdUs %lld, remaining %lld",
            targetDurationUs - lastEnqueueUs > thresholdUs ? "" : "not ",
            (long long)thresholdUs,
            (long long)(targetDurationUs - lastEnqueueUs));

    if (targetDurationUs - lastEnqueueUs > thresholdUs) {
        ALOGI("[%s][%d] shouldPauseDownload ret true  \n",__FUNCTION__,__LINE__);
        return true;
    }
    return false;
}

bool PlaylistFetcher::initDownloadState(
        AString &uri,
        sp<AMessage> &itemMeta,
        int32_t &firstSeqNumberInPlaylist,
        int32_t &lastSeqNumberInPlaylist) {
    status_t err = refreshPlaylist();
    firstSeqNumberInPlaylist = 0;
#ifdef RTK_PLATFORM
    lastSeqNumberInPlaylist = -1;
#else
    lastSeqNumberInPlaylist = 0;
#endif
    bool discontinuity = false;

    if (mPlaylist != NULL) {
        mPlaylist->getSeqNumberRange(
                &firstSeqNumberInPlaylist, &lastSeqNumberInPlaylist);

        if (mDiscontinuitySeq < 0) {
            mDiscontinuitySeq = mPlaylist->getDiscontinuitySeq();
        }
    }

    mSegmentFirstPTS = -1ll;


    ALOGV("mStartTimeUs :: %lld \n", (long long)mStartTimeUs);
    if (mPlaylist != NULL && mSeqNumber < 0) {
        CHECK_GE(mStartTimeUs, 0ll);

        if (mSegmentStartTimeUs < 0) {
            if (!mPlaylist->isComplete() && !mPlaylist->isEvent()) {
                // If this is a live session, start 3 segments from the end on connect
#ifdef RTK_PLATFORM
                mSeqNumber = lastSeqNumberInPlaylist/2 + firstSeqNumberInPlaylist/2;

                if((lastSeqNumberInPlaylist - firstSeqNumberInPlaylist) < 6 || mSeqNumber < firstSeqNumberInPlaylist)
                    mSeqNumber = firstSeqNumberInPlaylist;
#else
                mSeqNumber = lastSeqNumberInPlaylist - 3;
#endif
                if (mSeqNumber < firstSeqNumberInPlaylist) {
                    mSeqNumber = firstSeqNumberInPlaylist;
                }
            } else {
                // When seeking mSegmentStartTimeUs is unavailable (< 0), we
                // use mStartTimeUs (client supplied timestamp) to determine both start segment
                // and relative position inside a segment
                mSeqNumber = getSeqNumberForTime(mStartTimeUs);
#ifdef RTK_PLATFORM
                if(mSeqNumber > lastSeqNumberInPlaylist)
                {
                    ALOGE("[%s][%d] error mSeq::%d last::%d first::%d \n",__FUNCTION__,__LINE__, (int)mSeqNumber, (int)lastSeqNumberInPlaylist, (int)firstSeqNumberInPlaylist);
                    mSeqNumber = lastSeqNumberInPlaylist;
                }
                if (mSeqNumber < firstSeqNumberInPlaylist)
                {
                    ALOGE("[%s][%d] error %d %d %d \n",__FUNCTION__,__LINE__,mSeqNumber,lastSeqNumberInPlaylist,firstSeqNumberInPlaylist);
                    mSeqNumber = firstSeqNumberInPlaylist;
                }
                if(mSeqNumber >= 0)
                {
                    mStartTimeUs -= getSegmentStartTimeUs(mSeqNumber);
                }
#else
                mStartTimeUs -= getSegmentStartTimeUs(mSeqNumber);
#endif
            }

#ifdef RTK_PLATFORM
            if(lastSeqNumberInPlaylist >= 0)
            {
                mStartTimeUsRelative = true;
            }
            else
            {
                ALOGE("[%s][%d]M3UParser error, retry.\n",__FUNCTION__,__LINE__);
                mSeqNumber = -1;
                err = ERROR_IO;
            }
#else
            mStartTimeUsRelative = true;
#endif
            FLOGV("Initial sequence number for time %lld is %d from (%d .. %d)",
                    (long long)mStartTimeUs, mSeqNumber, firstSeqNumberInPlaylist,
                    lastSeqNumberInPlaylist);
        } else {
            // When adapting or track switching, mSegmentStartTimeUs (relative
            // to media time 0) is used to determine the start segment; mStartTimeUs (absolute
            // timestamps coming from the media container) is used to determine the position
            // inside a segments.
            if (mStreamTypeMask != LiveSession::STREAMTYPE_SUBTITLES
                    && mSeekMode != LiveSession::kSeekModeNextSample) {
                // avoid double fetch/decode
                // Use (mSegmentStartTimeUs + 1/2 * targetDurationUs) to search
                // for the starting segment in new variant.
                // If the two variants' segments are aligned, this gives the
                // next segment. If they're not aligned, this gives the segment
                // that overlaps no more than 1/2 * targetDurationUs.
                mSeqNumber = getSeqNumberForTime(mSegmentStartTimeUs
                        + mPlaylist->getTargetDuration() / 2);
            } else {
                mSeqNumber = getSeqNumberForTime(mSegmentStartTimeUs);
            }
            ssize_t minSeq = getSeqNumberForDiscontinuity(mDiscontinuitySeq);
            if (mSeqNumber < minSeq) {
                mSeqNumber = minSeq;
            }

            if (mSeqNumber < firstSeqNumberInPlaylist) {
                mSeqNumber = firstSeqNumberInPlaylist;
            }

            if (mSeqNumber > lastSeqNumberInPlaylist) {
                mSeqNumber = lastSeqNumberInPlaylist;
            }
            FLOGV("Initial sequence number is %d from (%d .. %d)",
                    mSeqNumber, firstSeqNumberInPlaylist,
                    lastSeqNumberInPlaylist);
        }
#ifdef RTK_PLATFORM
        StatusUpdatemSeqNumber();
#endif
    }

    // if mPlaylist is NULL then err must be non-OK; but the other way around might not be true
    if (mSeqNumber < firstSeqNumberInPlaylist
            || mSeqNumber > lastSeqNumberInPlaylist
            || err != OK) {
#ifdef RTK_PLATFORM
        if ((err != OK || mPlaylist == NULL || (mPlaylist != NULL && !mPlaylist->isComplete())) && mNumRetries < kMaxNumRetries)
#else
        if ((err != OK || !mPlaylist->isComplete()) && mNumRetries < kMaxNumRetries)
#endif
        {
            ++mNumRetries;
            ALOGW("[%s][%d] kMaxNumRetries::%d  mNumRetries::%d  \n",__FUNCTION__,__LINE__,kMaxNumRetries,mNumRetries);

            if (mSeqNumber > lastSeqNumberInPlaylist || err != OK || mPlaylist == NULL) {
                // make sure we reach this retry logic on refresh failures
                // by adding an err != OK clause to all enclosing if's.

                // refresh in increasing fraction (1/2, 1/3, ...) of the
                // playlist's target duration or 3 seconds, whichever is less
                int64_t delayUs = kMaxMonitorDelayUs;
                if (mPlaylist != NULL) {
                    delayUs = mPlaylist->size() * mPlaylist->getTargetDuration()
                            / (1 + mNumRetries);
                }
                if (delayUs > kMaxMonitorDelayUs) {
                    delayUs = kMaxMonitorDelayUs;
                }
#ifdef RTK_PLATFORM
                if(err != OK && delayUs < 1000000l)
                    delayUs = 1000000l;
#endif
                FLOGV("sequence number high: %d from (%d .. %d), "
                      "monitor in %lld (retry=%d)",
                        mSeqNumber, firstSeqNumberInPlaylist,
                        lastSeqNumberInPlaylist, (long long)delayUs, mNumRetries);
                postMonitorQueue(delayUs);
                return false;
            }

            if (err != OK) {
                notifyError(err);
                ALOGE("[%s][%d] refreshPlaylist err return false \n",__FUNCTION__,__LINE__);
                return false;
            }

            // we've missed the boat, let's start 3 segments prior to the latest sequence
            // number available and signal a discontinuity.

            ALOGI("We've missed the boat, restarting playback."
                  "  mStartup=%d, was  looking for %d in %d-%d",
                    mStartup, mSeqNumber, firstSeqNumberInPlaylist,
                    lastSeqNumberInPlaylist);
            if (mStopParams != NULL) {
                // we should have kept on fetching until we hit the boundaries in mStopParams,
                // but since the segments we are supposed to fetch have already rolled off
                // the playlist, i.e. we have already missed the boat, we inevitably have to
                // skip.
                notifyStopReached();
                ALOGE("[%s][%d] notifyStopReached return false\n",__FUNCTION__,__LINE__);
                return false;
            }
            mSeqNumber = lastSeqNumberInPlaylist - 3;
            if (mSeqNumber < firstSeqNumberInPlaylist) {
                mSeqNumber = firstSeqNumberInPlaylist;
            }
#ifdef RTK_PLATFORM
            StatusUpdatemSeqNumber();
#endif
            discontinuity = true;

            // fall through
        } else {
            if (mPlaylist != NULL) {
                if (mSeqNumber >= firstSeqNumberInPlaylist + (int32_t)mPlaylist->size()
                        && !mPlaylist->isComplete()) {
                    // Live playlists
                    ALOGW("sequence number %d not yet available", mSeqNumber);
#ifdef RTK_PLATFORM
                    if(mNumRetries > kMaxNumRetries)
                    {
                        ALOGE("[%s][%d] mNumRetries :: %d, need reconnect.\n",__FUNCTION__,__LINE__, mNumRetries);
                        notifyError(ERROR_IO);
                    }
#endif

                    postMonitorQueue(delayUsToRefreshPlaylist());
                    return false;
                }
                ALOGE("Cannot find sequence number %d in playlist "
                     "(contains %d - %d)",
                     mSeqNumber, firstSeqNumberInPlaylist,
                      firstSeqNumberInPlaylist + (int32_t)mPlaylist->size() - 1);

                if (mTSParser != NULL) {
                    mTSParser->signalEOS(ERROR_END_OF_STREAM);
                    // Use an empty buffer; we don't have any new data, just want to extract
                    // potential new access units after flush.  Reset mSeqNumber to
                    // lastSeqNumberInPlaylist such that we set the correct access unit
                    // properties in extractAndQueueAccessUnitsFromTs.
                    sp<ABuffer> buffer = new ABuffer(0);
                    mSeqNumber = lastSeqNumberInPlaylist;
                    extractAndQueueAccessUnitsFromTs(buffer);
                }
#ifdef RTK_PLATFORM
                if(mSeqNumber == -1)
                {
                    ALOGD("[%s][%d] mSeqNumber -1 notifyError ERROR_CANNOT_CONNECT.\n",__FUNCTION__,__LINE__);
                    notifyError(ERROR_CANNOT_CONNECT);
                }else
                    notifyError(ERROR_END_OF_STREAM);
#else
                notifyError(ERROR_END_OF_STREAM);
#endif
            } else {
                // It's possible that we were never able to download the playlist.
                // In this case we should notify error, instead of EOS, as EOS during
                // prepare means we succeeded in downloading everything.
                ALOGE("Failed to download playlist!");
                notifyError(ERROR_IO);
            }

            ALOGE("[%s][%d] return false \n",__FUNCTION__,__LINE__);
            return false;
        }
    }

    mNumRetries = 0;

    CHECK(mPlaylist->itemAt(
                mSeqNumber - firstSeqNumberInPlaylist,
                &uri,
                &itemMeta));

#ifdef RTK_PLATFORM
    const char *kUriStr =mPreUri.c_str();
    if(!strncmp(uri.c_str(),kUriStr, strlen(mPreUri.c_str())) && strlen(mPreUri.c_str()) > 0)
    {
        ALOGW("[%s][%d] uri is the same, skip this uri.  \n",__FUNCTION__,__LINE__);
        ALOGD("[%s][%d] mSeqNumber::%d &firstSeqNumberInPlaylist :: %d  &lastSeqNumberInPlaylist::%d\n",__FUNCTION__,__LINE__,mSeqNumber
                ,firstSeqNumberInPlaylist, lastSeqNumberInPlaylist);

        mSeqNumber++;
        StatusUpdatemSeqNumber();
        if(mSeqNumber > lastSeqNumberInPlaylist)
            mSeqNumber = lastSeqNumberInPlaylist;
        CHECK(mPlaylist->itemAt(
                    mSeqNumber - firstSeqNumberInPlaylist,
                    &uri,
                    &itemMeta));
    }
#endif

    CHECK(itemMeta->findInt32("discontinuity-sequence", &mDiscontinuitySeq));

    int32_t val;
    if (itemMeta->findInt32("discontinuity", &val) && val != 0) {
        discontinuity = true;
    } else if (mLastDiscontinuitySeq >= 0
            && mDiscontinuitySeq != mLastDiscontinuitySeq) {
        // Seek jumped to a new discontinuity sequence. We need to signal
        // a format change to decoder. Decoder needs to shutdown and be
        // created again if seamless format change is unsupported.
        FLOGV("saw discontinuity: mStartup %d, mLastDiscontinuitySeq %d, "
                "mDiscontinuitySeq %d, mStartTimeUs %lld",
                mStartup, mLastDiscontinuitySeq, mDiscontinuitySeq, (long long)mStartTimeUs);
        discontinuity = true;
    }
    mLastDiscontinuitySeq = -1;

    // decrypt a junk buffer to prefetch key; since a session uses only one http connection,
    // this avoids interleaved connections to the key and segment file.
    {
        sp<ABuffer> junk = new ABuffer(16);
        junk->setRange(0, 16);
        status_t err = decryptBuffer(mSeqNumber - firstSeqNumberInPlaylist, junk,
                true /* first */);
#ifdef RTK_PLATFORM
        if (err != OK)
        {
            notifyError(err);
            ALOGE("[%s][%d] return false \n",__FUNCTION__,__LINE__);
            return false;
        }
#else
        if (err == ERROR_NOT_CONNECTED) {
            return false;
        } else if (err != OK) {
            notifyError(err);
            return false;
        }
#endif
    }

    if ((mStartup && !mTimeChangeSignaled) || discontinuity) {
        // We need to signal a time discontinuity to ATSParser on the
        // first segment after start, or on a discontinuity segment.
        // Setting mNextPTSTimeUs informs extractAndQueueAccessUnitsXX()
        // to send the time discontinuity.
        if (mPlaylist->isComplete() || mPlaylist->isEvent()) {
            // If this was a live event this made no sense since
            // we don't have access to all the segment before the current
            // one.
            mNextPTSTimeUs = getSegmentStartTimeUs(mSeqNumber);
        }

        // Setting mTimeChangeSignaled to true, so that if start time
        // searching goes into 2nd segment (without a discontinuity),
        // we don't reset time again. It causes corruption when pending
        // data in ATSParser is cleared.
        mTimeChangeSignaled = true;
    }

    if (discontinuity) {
        ALOGI("queueing discontinuity (explicit=%d)", discontinuity);

        // Signal a format discontinuity to ATSParser to clear partial data
        // from previous streams. Not doing this causes bitstream corruption.
        if (mTSParser != NULL) {
            mTSParser.clear();
        }

        queueDiscontinuity(
                ATSParser::DISCONTINUITY_FORMAT_ONLY,
                NULL /* extra */);

        if (mStartup && mStartTimeUsRelative && mFirstPTSValid) {
            // This means we guessed mStartTimeUs to be in the previous
            // segment (likely very close to the end), but either video or
            // audio has not found start by the end of that segment.
            //
            // If this new segment is not a discontinuity, keep searching.
            //
            // If this new segment even got a discontinuity marker, just
            // set mStartTimeUs=0, and take all samples from now on.
            mStartTimeUs = 0;
            mFirstPTSValid = false;
            mIDRFound = false;
            mVideoBuffer->clear();
        }
    }

    FLOGV("fetching segment %d from (%d .. %d)",
            mSeqNumber, firstSeqNumberInPlaylist, lastSeqNumberInPlaylist);
    return true;
}

#ifdef RTK_PLATFORM
void PlaylistFetcher::onRTKCustomerHLSerrCntChk()
{
    char value[PROPERTY_VALUE_MAX];
    int ctrl = 0;
    if (property_get(RTK_PROPERTY_HLS_ERR_COUNT, value, NULL))
    {
        ctrl = atoi(value);
        if(ctrl >= 0)
        {
            if(kWaitDownloadErrCount != ctrl)
                ALOGI("[%s][%d] set RTK_PROPERTY_HLS_ERR_COUNT :: %d num.\n",__FUNCTION__,__LINE__,ctrl);
            kWaitDownloadErrCount = ctrl;
        }
    }
    else
        kWaitDownloadErrCount = kWaitDownloadErrIniCount;
}
#endif

#ifdef RTK_PLATFORM
int _Getcarrier(void)
{
    char c[1]={0};
    int in = open("/sys/class/net/eth0/carrier", O_RDONLY); // is eth0 connected?
    int output = 0;
    if(read(in,c,1)==1)
        output = atoi(c);
    else
        ALOGE("[%s][%d] open /sys/class/net/eth0/carrier fail \n", __FILE__, __LINE__);

    if(output == 0)
    {
        close(in);
        in = open("/sys/class/net/wlan0/carrier", O_RDONLY); // is wlan0 connected ?
        if(read(in,c,1)==1)
        {
            output = atoi(c);
            if(output == 1)
                output = 2;
        }
        else
            ALOGE("[%s][%d] open /sys/class/net/wlan0/carrier fail \n", __FILE__, __LINE__);
    }

    close(in);
    return output;
}


long long _GetrxByte(int type)
{

    int in = 0;
    if(type == 2)
        in = open("/sys/class/net/wlan0/statistics/rx_bytes", O_RDONLY); // rx_bytes: number of bytes received
    else
        in = open("/sys/class/net/eth0/statistics/rx_bytes", O_RDONLY);

    char stchar[8]={0};
    long long lVal = 0;
    if(read(in,stchar,8)>0)
        lVal = atol(stchar);
    else
        ALOGE("[%s][%d] open /sys/class/net/eth0/statistics/rx_bytes fail \n", __FILE__, __LINE__);

    close(in);
    return lVal;
}
#endif

void PlaylistFetcher::onDownloadNext() {
    AString uri;
    sp<AMessage> itemMeta;
    sp<ABuffer> buffer;
    sp<ABuffer> tsBuffer;
    int32_t firstSeqNumberInPlaylist = 0;
    int32_t lastSeqNumberInPlaylist = 0;
    bool connectHTTP = true;
#ifdef RTK_PLATFORM
    ssize_t errStatus = OK;
#endif

    if(mUseRTKExtFlow == 1)
    {
        ALOGV("mStartTimeUs :: %lld \n", (long long)mStartTimeUs);
        //mStartTimeUs = 0 ;
    }
#ifdef RTK_PLATFORM
    if(mStatusFlowInfo != kPlaylistStatus_initDownloadState)
    {
        SetLineInfo(__LINE__, kPlaylistStatus_initDownloadState);
        StatusUpdateRecTime();
    }
#endif

    if (mDownloadState->hasSavedState()) {
        mDownloadState->restoreState(
                uri,
                itemMeta,
                buffer,
                tsBuffer,
                firstSeqNumberInPlaylist,
                lastSeqNumberInPlaylist);
#ifdef RTK_PLATFORM
        connectHTTP = true;
#else
        connectHTTP = false;
#endif
        FLOGV("resuming: '%s'", uri.c_str());
    } else {
        if (!initDownloadState(
                uri,
                itemMeta,
                firstSeqNumberInPlaylist,
                lastSeqNumberInPlaylist)) {
            ALOGW("[%s][%d] initDownloadState fail return\n",__FUNCTION__,__LINE__);
#ifdef RTK_PLATFORM
            usleep(200000);
#endif
            return;
        }
        FLOGV("fetching: '%s'", uri.c_str());
    }
#ifdef RTK_PLATFORM
    SetLineInfo(__LINE__);
    StatusUpdateRecTime();
    if(mSeqNumber>=0)
    {
        ALOGI("[%s][%d] mSeqNumber::%d  lastSeqNumberInPlaylist::%d getSegmentStartTimeUs:: %lld\n",__FUNCTION__,__LINE__,mSeqNumber,lastSeqNumberInPlaylist,(long long)getSegmentStartTimeUs(mSeqNumber));
        ALOGI("[%s][%d] uri::%s \n",__FUNCTION__,__LINE__,uri.c_str());
    }

    mSession->setLastSessionFlg(1);
#endif
    int64_t range_offset, range_length;
    if (!itemMeta->findInt64("range-offset", &range_offset)
            || !itemMeta->findInt64("range-length", &range_length)) {
        range_offset = 0;
        range_length = -1;
    }

    // block-wise download
    if(mUseRTKExtFlow == 1)
    {
    ALOGD("\033[1;34m[PlaylistFetcher]\033[m[%d]:%s() NUPLAYER_M3U8_WITH_RTKEXT start \n",__LINE__, __FUNCTION__);
    initFromDataSource(uri);
    int32_t dCount =0;

    status_t err=OK;
    do{
        //QueueAccessUnit
        err = ParseTsStreamFromRtkExt();

        err = extractAndQueueAccessUnitsFromTs(buffer);

        if (err == -EAGAIN) {
            // starting sequence number too low/high
            mAudioTrack.mSource->stop();
            mVideoTrack.mSource->stop();
            for (size_t i = 0; i < mPacketSources.size(); i++) {
                sp<AnotherPacketSource> packetSource = mPacketSources.valueAt(i);
                packetSource->clear();
            }
            postMonitorQueue();
            return;
        } else if (err == ERROR_OUT_OF_RANGE) {
            // reached stopping point
            stopAsync(/* clear = */ false);
            return;
        } else if (err != OK) {
            notifyError(err);
            return;
        }

        if(dIsAudioEOF == 1 && dIsVideoEOF == 1)
        {
            ALOGV("dIsAudioEOF and dIsVideoEOF All EOF break;");
            break;
        }
        dCount++ ;
        ALOGV("dCount == %d \n",dCount);
    }while(dIsAudioEOF == 0 || dIsVideoEOF == 0);

    sp<AMessage> meta;
    int64_t tempSmallTimeUs = 0;
    for (size_t i = mPacketSources.size(); i-- > 0;)
    {
        int64_t timeUs;
        meta = mPacketSources[i]->getLatestEnqueuedMeta();
        if(meta != NULL)
        {
            CHECK(meta->findInt64("timeUs", &timeUs));
            if(tempSmallTimeUs == 0 || timeUs < tempSmallTimeUs)
            {
                tempSmallTimeUs = timeUs;
                dsmallTimeTrack = i;
            }
        }
    }
    err = OK ;
    resetDataSource();

    if (err != OK) {
        notifyError(err);
        return;
    }

    ++mSeqNumber;
    postMonitorQueue();
    return;

    }else
    {
    //persist.media.RtkExtractor false mUseRTKExtFlow == 0  ,NuPlayer original flow
    // block-wise download
    bool shouldPause = false;
    ssize_t bytesRead;
#ifdef RTK_PLATFORM
    int dErrFlg=0;
    int dDataErrFlg=0;

    int64_t durationUs;
    itemMeta->findInt64("durationUs", &durationUs);
    initCurSeqInfo(durationUs);
#endif

    do {
        int64_t startUs = ALooper::GetNowUs();
#ifdef RTK_PLATFORM
        SetLineInfo(__LINE__, kPlaylistStatus_FetchFile);
        StatusUpdateRecTime();
        if(mDisconnectRxByte > 0 && _Getcarrier())
        {
            if(_GetrxByte(_Getcarrier()) > mDisconnectRxByte) {
                mDisconnectRxByte = 0;
                bytesRead = mHTTPDownloader->fetchBlock(
                    uri.c_str(), &buffer, range_offset, range_length, kDownloadBlockSize,
                    NULL /* actualURL */, connectHTTP);
            }
            else
                bytesRead = ERROR_NOT_CONNECTED;
        }
        else
#endif
            bytesRead = mHTTPDownloader->fetchBlock(
                uri.c_str(), &buffer, range_offset, range_length, kDownloadBlockSize,
                NULL /* actualURL */, connectHTTP);

#ifdef RTK_PLATFORM
        SetLineInfo(__LINE__);
        StatusUpdateRecTime();
#endif

        int64_t delayUs = ALooper::GetNowUs() - startUs;
        if (bytesRead == ERROR_NOT_CONNECTED) {
            ALOGE("[%s][%d] ERROR_NOT_CONNECTED  uri.c_str()::%s",__FUNCTION__,__LINE__, uri.c_str());
#ifdef RTK_PLATFORM
            ALOGE("[%s][%d] RTK_PLATFORM Skip notifyError(err) :: %d \n",__FUNCTION__,__LINE__,(int32_t)bytesRead);
            dErrFlg =1;
            break;
#else
            return;
#endif
        }
#ifdef RTK_PLATFORM
        if(bytesRead == ERROR_CANNOT_CONNECT)
        {
            ALOGE("[%s][%d] ERROR_CANNOT_CONNECT notifyError(err) :: %d \n",__FUNCTION__,__LINE__,(int32_t)bytesRead);
            if(mPlaylist->isComplete())
            {
                errStatus = ERROR_CANNOT_CONNECT;
                dErrFlg = 1;
                break;
            }
            notifyError(bytesRead);
            return;
        }
#endif

        if (bytesRead < 0) {
            status_t err = bytesRead;
            ALOGE("failed to fetch .ts segment at url '%s'", uri.c_str());
#ifdef RTK_PLATFORM
            ALOGE("[%s][%d] bytesRead :: %d \n",__FUNCTION__,__LINE__,(int32_t)bytesRead);
            ALOGE("[%s][%d] RTK_PLATFORM Skip notifyError(err) :: %d mDownloadState saveState.\n",__FUNCTION__,__LINE__,err);
            dErrFlg =1;
            if(mPlaylist->isComplete())
            {
                mDownloadState->saveState(
                        uri,
                        itemMeta,
                        buffer,
                        tsBuffer,
                        firstSeqNumberInPlaylist,
                        lastSeqNumberInPlaylist);
            }
            mFetchBlockFailedCnt++;
            if (mFetchBlockFailedCnt > kWaitFetchBlockFailedCnt)
            {
                ALOGE("[%s][%d] mFetchBlockFailedCnt = %d, notifyError(err) :: %d \n",__FUNCTION__,__LINE__, mFetchBlockFailedCnt, err);
                mFetchBlockFailedCnt = 0;
                notifyError(err);
                return;
            }
            break;
#else
            notifyError(err);
            return;
#endif
        } else {
            mFetchBlockFailedCnt = 0;
        }

        // add sample for bandwidth estimation, excluding samples from subtitles (as
        // its too small), or during startup/resumeUntil (when we could have more than
        // one connection open which affects bandwidth)
        if (!mStartup && mStopParams == NULL && bytesRead > 0
                && (mStreamTypeMask
                        & (LiveSession::STREAMTYPE_AUDIO
                        | LiveSession::STREAMTYPE_VIDEO))) {
            mSession->addBandwidthMeasurement(bytesRead, delayUs);
            if (delayUs > 2000000ll) {
#ifdef RTK_PLATFORM
                ALOGD("bytesRead %zd took %.2f seconds - abnormal bandwidth dip",
                        bytesRead, (double)delayUs / 1.0e6);
#endif
                FLOGV("bytesRead %zd took %.2f seconds - abnormal bandwidth dip",
                        bytesRead, (double)delayUs / 1.0e6);
            }
        }

        connectHTTP = false;

        CHECK(buffer != NULL);

        size_t size = buffer->size();
        // Set decryption range.
        buffer->setRange(size - bytesRead, bytesRead);
        status_t err = decryptBuffer(mSeqNumber - firstSeqNumberInPlaylist, buffer,
                buffer->offset() == 0 /* first */);
        // Unset decryption range.
        buffer->setRange(0, size);

        if (err != OK) {
            ALOGE("decryptBuffer failed w/ error %d", err);
#ifdef RTK_PLATFORM
            ALOGE("[%s][%d] RTK_PLATFORM Skip notifyError(err) :: %d \n",__FUNCTION__,__LINE__,err);
            dDataErrFlg = 1;
            break;
#else
            notifyError(err);
            return;
#endif
        }

        bool startUp = mStartup; // save current start up state

#ifdef RTK_PLATFORM
        int index = 0;
        if (!bufferStartsWithTsSyncByte(buffer))
        {
           index = FindTsSyncByte(buffer);
           if(index < 0)
           {
               ALOGE("[%s][%d] RTK_PLATFORM FindTsSyncByte err.\n",__FUNCTION__,__LINE__);
               dDataErrFlg = 1;
               break;
           }

        }
#endif

        err = OK;
        if (bufferStartsWithTsSyncByte(buffer) || index > 0) {
            // Incremental extraction is only supported for MPEG2 transport streams.
            if (tsBuffer == NULL) {
                tsBuffer = new ABuffer(buffer->data(), buffer->capacity());
                tsBuffer->setRange(0, 0);
            } else if (tsBuffer->capacity() != buffer->capacity()) {
                size_t tsOff = tsBuffer->offset(), tsSize = tsBuffer->size();
                tsBuffer = new ABuffer(buffer->data(), buffer->capacity());
                tsBuffer->setRange(tsOff, tsSize);
            }
            tsBuffer->setRange(tsBuffer->offset(), tsBuffer->size() + bytesRead);
            err = extractAndQueueAccessUnitsFromTs(tsBuffer);
        }

        if (err == -EAGAIN) {
            // starting sequence number too low/high
            mTSParser.clear();
            for (size_t i = 0; i < mPacketSources.size(); i++) {
                sp<AnotherPacketSource> packetSource = mPacketSources.valueAt(i);
                packetSource->clear();
            }
            postMonitorQueue();
            return;
        } else if (err == ERROR_OUT_OF_RANGE) {
            // reached stopping point
            ALOGW("[%s][%d] notifyStopReached ERROR_OUT_OF_RANGE :: %d \n",__FUNCTION__,__LINE__,err);
            notifyStopReached();
            return;
        }
#ifdef RTK_PLATFORM
        else if(err == ERROR_UNSUPPORTED)
        {
            ALOGW("[%s][%d] notifyError ERROR_UNSUPPORTED :: %d \n",__FUNCTION__,__LINE__,err);
            notifyError(err);
            return;
        }
#endif
        else if (err != OK) {
#ifdef RTK_PLATFORM
            ALOGE("[%s][%d] RTK_PLATFORM Skip notifyError(err) :: %d \n",__FUNCTION__,__LINE__,err);
            dDataErrFlg = 1;
            break;
#else
            notifyError(err);
            return;
#endif
        }
        // If we're switching, post start notification
        // this should only be posted when the last chunk is full processed by TSParser
        if (mSeekMode != LiveSession::kSeekModeExactPosition && startUp != mStartup) {
            CHECK(mStartTimeUsNotify != NULL);
            mStartTimeUsNotify->post();
            mStartTimeUsNotify.clear();
            shouldPause = true;
            ALOGI("[%s][%d] shouldPause set true %d %d %d %d \n",__FUNCTION__,__LINE__,mSeekMode, LiveSession::kSeekModeExactPosition, startUp, mStartup);
        }
        if (shouldPause || shouldPauseDownload()) {
            // save state and return if this is not the last chunk,
            // leaving the fetcher in paused state.
            if (bytesRead != 0) {
                mDownloadState->saveState(
                        uri,
                        itemMeta,
                        buffer,
                        tsBuffer,
                        firstSeqNumberInPlaylist,
                        lastSeqNumberInPlaylist);
                ALOGW("[%s][%d] shouldPause::%d || shouldPauseDownload()::%d return\n",__FUNCTION__,__LINE__,shouldPause,shouldPauseDownload());
                return;
            }
            shouldPause = true;
        }
    } while (bytesRead != 0);

#ifdef RTK_PLATFORM
    if (dErrFlg == 0 && bufferStartsWithTsSyncByte(buffer))
#else
    if (bufferStartsWithTsSyncByte(buffer))
#endif
    {
        // If we don't see a stream in the program table after fetching a full ts segment
        // mark it as nonexistent.
        ATSParser::SourceType srcTypes[] =
                { ATSParser::VIDEO, ATSParser::AUDIO };
        LiveSession::StreamType streamTypes[] =
                { LiveSession::STREAMTYPE_VIDEO, LiveSession::STREAMTYPE_AUDIO };
        const size_t kNumTypes = NELEM(srcTypes);

        for (size_t i = 0; i < kNumTypes; i++) {
            ATSParser::SourceType srcType = srcTypes[i];
            LiveSession::StreamType streamType = streamTypes[i];

            sp<AnotherPacketSource> source =
                static_cast<AnotherPacketSource *>(
                    mTSParser->getSource(srcType).get());

            if (!mTSParser->hasSource(srcType)) {
                ALOGW("MPEG2 Transport stream does not contain %s data.",
                      srcType == ATSParser::VIDEO ? "video" : "audio");

                mStreamTypeMask &= ~streamType;
                mPacketSources.removeItem(streamType);
            }
        }

    }
#ifdef RTK_PLATFORM
    if (dErrFlg == 0 && checkDecryptPadding(buffer) != OK)
#else
    if (checkDecryptPadding(buffer) != OK)
#endif
    {
        ALOGE("Incorrect padding bytes after decryption.");
#ifdef RTK_PLATFORM
        ALOGE("[%s][%d] RTK_PLATFORM Skip notifyError(err) :: %d \n",__FUNCTION__,__LINE__,ERROR_MALFORMED);
#else
        notifyError(ERROR_MALFORMED);
        return;
#endif
    }

#ifdef RTK_PLATFORM
    if (dErrFlg == 0 && tsBuffer != NULL)
#else
    if (tsBuffer != NULL)
#endif
    {
        AString method;
        CHECK(buffer->meta()->findString("cipher-method", &method));
        if ((tsBuffer->size() > 0 && method == "NONE")
                || tsBuffer->size() > 16) {
            ALOGE("MPEG2 transport stream is not an even multiple of 188 "
                    "bytes in length.");
#ifdef RTK_PLATFORM
            ALOGD("[%s][%d] RTK skip this notifyError(ERROR_MALFORMED)  cipher size ::%zu  \n",__FUNCTION__,__LINE__,tsBuffer->size());
            dDataErrFlg = 1;
#else
            notifyError(ERROR_MALFORMED);
            return;
#endif
        }
    }

    // bulk extract non-ts files
    bool startUp = mStartup;
#ifdef RTK_PLATFORM
    if (dErrFlg == 0 && tsBuffer == NULL && buffer->size() > 0)
#else
    if (tsBuffer == NULL)
#endif
    {
        status_t err = extractAndQueueAccessUnits(buffer, itemMeta);
        if (err == -EAGAIN) {
            // starting sequence number too low/high
            postMonitorQueue();
            return;
        } else if (err == ERROR_OUT_OF_RANGE) {
            // reached stopping point
            notifyStopReached();
            return;
        } else if (err != OK) {
#ifdef RTK_PLATFORM
            ALOGE("[%s][%d] RTK_PLATFORM Skip notifyError(err) :: %d \n",__FUNCTION__,__LINE__,err);
            dDataErrFlg = 1;
#else
            notifyError(err);
            return;
#endif
        }
    }

#ifdef RTK_PLATFORM
    onRTKCustomerHLSerrCntChk();
    if(dErrFlg == 1 || dDataErrFlg == 1)
    {
        struct timeval tmptime;
        struct timeval * caltime = &mDownloadErrtime;
        gettimeofday(&tmptime, NULL);
        unsigned int duration =  (tmptime.tv_sec - caltime->tv_sec)*1000 + (tmptime.tv_usec - caltime->tv_usec)/1000;

        int32_t IniSeqNumber = mSeqNumber;
        if ((duration > kWaitDownloadErrTime && mDownloadErrCount > 0)
                    || mDownloadErrCount > kWaitDownloadErrCount
                || dDataErrFlg == 1 || errStatus != OK)
        {
            if(duration > kWaitDownloadErrTime)
            {
                gettimeofday(caltime, NULL);
                mDownloadOverTimeCnt++;
            }
            ALOGW("[%s][%d] Download Error uri::%s retry %d times error,need re-buffer.%d %d %d \n",__FUNCTION__,__LINE__,uri.c_str()
                    ,kWaitDownloadErrCount, mDownloadErrCount, duration,(duration > kWaitDownloadErrTime));
            if (shouldPauseDownload())
            {
                shouldPause = true;
            }
            if(mSignalSeekTo == 1)
            {
                mSignalSeekTo = 0;
                mStartTimeUsNotify->post();
                mStartTimeUsNotify.clear();
                shouldPause = true;
                gettimeofday(&mDownloadErrtime, NULL);
                ALOGW("[%s][%d]shouldPause == true \n",__FUNCTION__,__LINE__);
            }
            if(mPlaylist->isComplete())
            {
                ALOGD("[%s][%d] need check next segment playback mDownloadOverTimeCnt::%d\n",__FUNCTION__,__LINE__,mDownloadOverTimeCnt);
                if(errStatus == ERROR_CANNOT_CONNECT)
                {
                    ALOGE("[%s][%d] VOD playback keep mSeqNumber :: %d and skip Err. \n",__FUNCTION__,__LINE__,mSeqNumber);
                }else if(mDownloadOverTimeCnt > 10)
                {
                    ALOGD("[%s][%d] need check next segment playback\n",__FUNCTION__,__LINE__);

                    int mSeq = 0;
                    ALOGI("[%s][%d] Try fetch all file from ::%d to %d \n",__FUNCTION__,__LINE__,mSeqNumber,lastSeqNumberInPlaylist);
                    for(mSeq = mSeqNumber; mSeq < mSeqNumber + 3; mSeq++)
                    {
                        if(mSeq <= lastSeqNumberInPlaylist)
                        {
                            ALOGI("[%s][%d] Try fetch ::%d \n",__FUNCTION__,__LINE__,mSeq);
                            CHECK(mPlaylist->itemAt(
                                        (mSeq - firstSeqNumberInPlaylist),
                                        &uri,
                                        &itemMeta));
                            connectHTTP = true;
                            bytesRead = -1;
                            if(mDisconnectRxByte > 0 && _Getcarrier())
                            {
                                if(_GetrxByte(_Getcarrier()) > mDisconnectRxByte) {
                                    mDisconnectRxByte = 0;
                                    bytesRead = mHTTPDownloader->fetchBlock(
                                            uri.c_str(), &buffer, range_offset, range_length, kDownloadBlockSize,
                                            NULL /* actualURL */, connectHTTP);
                                }
                                else
                                    bytesRead = ERROR_NOT_CONNECTED;
                            }
                            else
                                bytesRead = mHTTPDownloader->fetchBlock(
                                        uri.c_str(), &buffer, range_offset, range_length, kDownloadBlockSize,
                                        NULL /* actualURL */, connectHTTP);

                            if(bytesRead > 0)
                            {
                                ALOGD("[%s][%d] Try fetch ::%d ok\n",__FUNCTION__,__LINE__,mSeq);
                                ALOGD("[%s][%d] change to next segment\n",__FUNCTION__,__LINE__);
                                mDownloadState->resetState();
                                mSeqNumber = mSeq;
                                mTSParser->signalDiscontinuity(ATSParser::DISCONTINUITY_TIME, NULL);
                                break;
                            }
                        }
                        CHECK(mPlaylist->itemAt(
                                    (mSeqNumber - firstSeqNumberInPlaylist),
                                    &uri,
                                    &itemMeta));
                    }

                }
                if(dDataErrFlg == 1)
                {
                    ++mSeqNumber;
                    mDownloadState->resetState();
                }
            }
            else if (!mPlaylist->isComplete())
            {
                if(dDataErrFlg == 1 || mSegmentFirstPTS == -1 || mDownloadOverTimeCnt > 20)
                {
                    if(_Getcarrier() == 0) {
                        mDisconnectRxByte = _GetrxByte(_Getcarrier());
                    }
                    else if(_GetrxByte(_Getcarrier()) > mDisconnectRxByte)
                    {
                        mDownloadState->resetState();
                        ++mSeqNumber;
                        if(mSeqNumber > lastSeqNumberInPlaylist)
                            mSeqNumber = lastSeqNumberInPlaylist;
                        ALOGI("[%s][%d] mSeqNumber :%d , lastSeqNumberInPlaylist = %d \n",__FUNCTION__,__LINE__,
                                mSeqNumber , lastSeqNumberInPlaylist);

                    }
                    if(mDownloadOverTimeCnt > 20)
                    {
                        mDownloadState->resetState();
                    }
                }
            }
        }else if(mDownloadErrCount < kWaitDownloadErrCount)
        {
            gettimeofday(caltime, NULL);
        }
        mDownloadErrCount++;
        if(IniSeqNumber == mSeqNumber)
        {
            mDownloadState->saveState(
                    uri,
                    itemMeta,
                    buffer,
                    tsBuffer,
                    firstSeqNumberInPlaylist,
                    lastSeqNumberInPlaylist);
        }

        if(IniSeqNumber != mSeqNumber && mDownloadState->hasSavedState())
        {
            mDownloadState->resetState();
        }
    }
    else
    {
        mSignalSeekTo = 0;
        gettimeofday(&mDownloadErrtime, NULL);
        mDownloadErrCount = 0;
        mDownloadOverTimeCnt = 0;

        mPreUri = uri;
        ++mSeqNumber;
        StatusUpdatemSeqNumber();
        QueueSeqInfo(MEDIA_AUDIO);
        QueueSeqInfo(MEDIA_VIDEO);
//        LogSeqInfo(0);
    }
#else
    ++mSeqNumber;
#endif

    ALOGI("[%s][%d] mSeqNumber::%d \n",__FUNCTION__,__LINE__,mSeqNumber);

    // if adapting, pause after found the next starting point
    if (mSeekMode != LiveSession::kSeekModeExactPosition && startUp != mStartup) {
        CHECK(mStartTimeUsNotify != NULL);
        mStartTimeUsNotify->post();
        mStartTimeUsNotify.clear();
        shouldPause = true;
    }

    if (!shouldPause) {
#ifdef RTK_PLATFORM
        if(dErrFlg == 1 || dDataErrFlg == 1)
            postMonitorQueue(200000,200000);
        else
            postMonitorQueue();
#else
        postMonitorQueue();
#endif
    }
    }//end if (mUseRTKExtFlow == 0 ,persist.media.RtkExtractor false ,NuPlayer original flow)
}

/*
 * returns true if we need to adjust mSeqNumber
 */
bool PlaylistFetcher::adjustSeqNumberWithAnchorTime(int64_t anchorTimeUs) {
    int32_t firstSeqNumberInPlaylist = mPlaylist->getFirstSeqNumber();

    int64_t minDiffUs, maxDiffUs;
    if (mSeekMode == LiveSession::kSeekModeNextSample) {
        // if the previous fetcher paused in the middle of a segment, we
        // want to start at a segment that overlaps the last sample
        minDiffUs = -mPlaylist->getTargetDuration();
        maxDiffUs = 0ll;
    } else {
        // if the previous fetcher paused at the end of a segment, ideally
        // we want to start at the segment that's roughly aligned with its
        // next segment, but if the two variants are not well aligned we
        // adjust the diff to within (-T/2, T/2)
        minDiffUs = -mPlaylist->getTargetDuration() / 2;
        maxDiffUs = mPlaylist->getTargetDuration() / 2;
    }

    int32_t oldSeqNumber = mSeqNumber;
    ssize_t index = mSeqNumber - firstSeqNumberInPlaylist;

    // adjust anchorTimeUs to within (minDiffUs, maxDiffUs) from mStartTimeUs
    int64_t diffUs = anchorTimeUs - mStartTimeUs;
    if (diffUs > maxDiffUs) {
        while (index > 0 && diffUs > maxDiffUs) {
            --index;

            sp<AMessage> itemMeta;
            CHECK(mPlaylist->itemAt(index, NULL /* uri */, &itemMeta));

            int64_t itemDurationUs;
            CHECK(itemMeta->findInt64("durationUs", &itemDurationUs));

            diffUs -= itemDurationUs;
        }
    } else if (diffUs < minDiffUs) {
        while (index + 1 < (ssize_t) mPlaylist->size()
                && diffUs < minDiffUs) {
            ++index;

            sp<AMessage> itemMeta;
            CHECK(mPlaylist->itemAt(index, NULL /* uri */, &itemMeta));

            int64_t itemDurationUs;
            CHECK(itemMeta->findInt64("durationUs", &itemDurationUs));

            diffUs += itemDurationUs;
        }
    }

    mSeqNumber = firstSeqNumberInPlaylist + index;

    if (mSeqNumber != oldSeqNumber) {
        FLOGV("guessed wrong seg number: diff %lld out of [%lld, %lld]",
                (long long) anchorTimeUs - mStartTimeUs,
                (long long) minDiffUs,
                (long long) maxDiffUs);
        return true;
    }
    return false;
}

int32_t PlaylistFetcher::getSeqNumberForDiscontinuity(size_t discontinuitySeq) const {
    int32_t firstSeqNumberInPlaylist = mPlaylist->getFirstSeqNumber();

    size_t index = 0;
    while (index < mPlaylist->size()) {
        sp<AMessage> itemMeta;
        CHECK(mPlaylist->itemAt( index, NULL /* uri */, &itemMeta));
        size_t curDiscontinuitySeq;
        CHECK(itemMeta->findInt32("discontinuity-sequence", (int32_t *)&curDiscontinuitySeq));
        int32_t seqNumber = firstSeqNumberInPlaylist + index;
        if (curDiscontinuitySeq == discontinuitySeq) {
            return seqNumber;
        } else if (curDiscontinuitySeq > discontinuitySeq) {
            return seqNumber <= 0 ? 0 : seqNumber - 1;
        }

        ++index;
    }

    return firstSeqNumberInPlaylist + mPlaylist->size();
}

int32_t PlaylistFetcher::getSeqNumberForTime(int64_t timeUs) const {
    size_t index = 0;
    int64_t segmentStartUs = 0;
    while (index < mPlaylist->size()) {
        sp<AMessage> itemMeta;
        CHECK(mPlaylist->itemAt(
                    index, NULL /* uri */, &itemMeta));

        int64_t itemDurationUs;
        CHECK(itemMeta->findInt64("durationUs", &itemDurationUs));

        if (timeUs < segmentStartUs + itemDurationUs) {
            break;
        }

        segmentStartUs += itemDurationUs;
        ++index;
    }
#ifdef RTK_PLATFORM
    if (index >= mPlaylist->size() &&  mPlaylist->size() > 0)
#else
    if (index >= mPlaylist->size())
#endif
    {
        index = mPlaylist->size() - 1;
    }
#ifdef RTK_PLATFORM
    ALOGI("[%s][%d] index::%d  mPlaylist->size()::%d mPlaylist->getFirstSeqNumber()::%d\n",__FUNCTION__,__LINE__, (int)index, (int)mPlaylist->size(), (int)mPlaylist->getFirstSeqNumber());
#endif

    return mPlaylist->getFirstSeqNumber() + index;
}

const sp<ABuffer> &PlaylistFetcher::setAccessUnitProperties(
        const sp<ABuffer> &accessUnit, const sp<AnotherPacketSource> &source, bool discard) {
    sp<MetaData> format = source->getFormat();
    if (format != NULL) {
        // for simplicity, store a reference to the format in each unit
        accessUnit->meta()->setObject("format", format);
    }

    if (discard) {
        accessUnit->meta()->setInt32("discard", discard);
    }
#ifdef RTK_PLATFORM
    accessUnit->meta()->setInt32("mSeqNumber", mSeqNumber);
#endif
    accessUnit->meta()->setInt32("discontinuitySeq", mDiscontinuitySeq);
    accessUnit->meta()->setInt64("segmentStartTimeUs", getSegmentStartTimeUs(mSeqNumber));
    accessUnit->meta()->setInt64("segmentFirstTimeUs", mSegmentFirstPTS);
    accessUnit->meta()->setInt64("segmentDurationUs", getSegmentDurationUs(mSeqNumber));
    if (!mPlaylist->isComplete() && !mPlaylist->isEvent()) {
        accessUnit->meta()->setInt64("playlistTimeUs", mPlaylistTimeUs);
    }
    return accessUnit;
}

#ifndef RTK_PLATFORM
bool PlaylistFetcher::isStartTimeReached(int64_t timeUs) {
    if (!mFirstPTSValid) {
        mFirstTimeUs = timeUs;
        mFirstPTSValid = true;
    }
    bool startTimeReached = true;
    if (mStartTimeUsRelative) {
        FLOGV("startTimeUsRelative, timeUs (%lld) - %lld = %lld",
                (long long)timeUs,
                (long long)mFirstTimeUs,
                (long long)(timeUs - mFirstTimeUs));
        timeUs -= mFirstTimeUs;
        if (timeUs < 0) {
            FLOGV("clamp negative timeUs to 0");
            timeUs = 0;
        }
        startTimeReached = (timeUs >= mStartTimeUs);
    }
    return startTimeReached;
}
#endif

sp<ABuffer> PlaylistFetcher::mediaBufferToABuffer(
        MediaBuffer* mb,
        media_track_type /*trackType*/)
{
    size_t outLength = mb->range_length();

#if 0
    bool audio = trackType == MEDIA_TRACK_TYPE_AUDIO;
    if (audio && mAudioIsVorbis) {
        outLength += sizeof(int32_t);
    }
#endif

    sp<ABuffer> ab;
#if 1
    ab = new ABuffer(outLength);
    memcpy(ab->data(),
            (const uint8_t *)mb->data() + mb->range_offset(),
            mb->range_length());
#else
    if (!audio) {
        // data is already provided in the buffer
        ab = new ABuffer(NULL, mb->range_length());
        mb->add_ref();
        ab->setMediaBufferBase(mb);
    } else {
        ab = new ABuffer(outLength);
        memcpy(ab->data(),
                (const uint8_t *)mb->data() + mb->range_offset(),
                mb->range_length());
    }

    if (audio && mAudioIsVorbis) {
        int32_t numPageSamples;
        if (!mb->meta_data()->findInt32(kKeyValidSamples, &numPageSamples)) {
            numPageSamples = -1;
        }

        uint8_t* abEnd = ab->data() + mb->range_length();
        memcpy(abEnd, &numPageSamples, sizeof(numPageSamples));
    }
#endif

    sp<AMessage> meta = ab->meta();

    int64_t timeUs;
    CHECK(mb->meta_data()->findInt64(kKeyTime, &timeUs));
    meta->setInt64("timeUs", timeUs);

#if 0
    if (trackType == MEDIA_TRACK_TYPE_TIMEDTEXT) {
        const char *mime;
        CHECK(mTimedTextTrack.mSource != NULL
                && mTimedTextTrack.mSource->getFormat()->findCString(kKeyMIMEType, &mime));
        meta->setString("mime", mime);
    }
#endif

    int64_t durationUs;
    if (mb->meta_data()->findInt64(kKeyDuration, &durationUs)) {
        meta->setInt64("durationUs", durationUs);
        ALOGV("durationUs::%lld\n", (long long)durationUs);
    }
#if 0
    if (trackType == MEDIA_TRACK_TYPE_SUBTITLE) {
        meta->setInt32("trackIndex", mSubtitleTrack.mIndex);
    }

    if (actualTimeUs) {
        *actualTimeUs = timeUs;
    }
#endif

    mb->release();
    mb = NULL;

    return ab;
}

status_t PlaylistFetcher::ParseTsStreamFromRtkExt()
{
    sp<MetaData> meta = mExtractor->getTrackMetaData(0);
    IMediaSource::ReadOptions options;
    Track *track;
    status_t err = OK;
    MediaBuffer *mbuf;
    media_track_type trackType;

    int32_t  dCountCopy = 0;
    //uint8_t * cBuf;
    int64_t timeUs;
    //AUDIO
    dCountCopy = 0 ;
    while(dCountCopy<64 && dIsAudioEOF == 0)
    {
        track = &mAudioTrack;
        if (track->mSource == NULL) {
            return -EAGAIN;
        }
        trackType = MEDIA_TRACK_TYPE_AUDIO;
        err = track->mSource->read(&mbuf, &options);
        options.clearSeekTo();
        if(err == OK)
        {
            CHECK(mbuf->meta_data()->findInt64(kKeyTime, &timeUs));
            mAudioTimeUs = timeUs;
            mbuf->meta_data()->setInt64(kKeyTime, mAudioTimeUs);
            sp<ABuffer> buffer = mediaBufferToABuffer(mbuf, trackType);
            track->mPackets->queueAccessUnit(buffer);
            mbuf = NULL;
        }else if(err == WOULD_BLOCK)
        {
            break;
        }else
        {
            ALOGE("\033[0;32m[ParseTs]\033[m[%d]:%s(), signalEOS \n",__LINE__, __FUNCTION__);
            track->mPackets->signalEOS(err);
            dIsAudioEOF = 1;
            break;
        }

        ALOGV("[Audio]dCountCopy == %d  mVideoTimeUs::%lld  ,  mAudioTimeUs::%lld \n" ,dCountCopy, (long long)mVideoTimeUs , (long long)mAudioTimeUs);
        dCountCopy++;
    }


    err = OK;
    dCountCopy = 0;
    //VIDEO
    while(dIsVideoEOF == 0 &&(mVideoTimeUs<mAudioTimeUs||(dIsAudioEOF==1 &&dCountCopy<3)))
    {
        track = &mVideoTrack;
        if (track->mSource == NULL) {
            return -EAGAIN;
        }
        trackType = MEDIA_TRACK_TYPE_VIDEO;

        err = track->mSource->read(&mbuf, &options);
        options.clearSeekTo();

        if(err == OK)
        {
            CHECK(mbuf->meta_data()->findInt64(kKeyTime, &timeUs));
            mVideoTimeUs = timeUs;
            mbuf->meta_data()->setInt64(kKeyTime, mVideoTimeUs);
            sp<ABuffer> buffer = mediaBufferToABuffer(mbuf, trackType);
            track->mPackets->queueAccessUnit(buffer);
            mbuf = NULL;
        }else if (err== WOULD_BLOCK)
        {
            break;
        }
        else
        {
            ALOGE("\033[0;32m[Parse]\033[m[%d]:%s(),signal EOS \n",__LINE__, __FUNCTION__);
            track->mPackets->signalEOS(err);
            dIsVideoEOF = 1;
            break;
        }

        dCountCopy++;
        ALOGV("[Video]dCountCopy == %d  mVideoTimeUs::%lld  ,  mAudioTimeUs::%lld \n" ,dCountCopy, (long long)mVideoTimeUs , (long long)mAudioTimeUs);
    }

    err = OK;
    return err;
}

status_t PlaylistFetcher::extractAndQueueAccessUnitsFromTs(const sp<ABuffer> &buffer) {
    ALOGV("\033[1;34m[PlaylistFetcher]\033[m[%d]:%s()\n",__LINE__, __FUNCTION__);


    Track *track = NULL;
    if(mUseRTKExtFlow == 1)
    {

    if (mNextPTSTimeUs >= 0ll)
    {
        mNextPTSTimeUs = -1ll;
//        mFirstPTSValid = false;
    }

    }else
    {// mUseRTKExtFlow == 0 ,original flow

    if (mTSParser == NULL) {
        // Use TS_TIMESTAMPS_ARE_ABSOLUTE so pts carry over between fetchers.
        mTSParser = new ATSParser(ATSParser::TS_TIMESTAMPS_ARE_ABSOLUTE);
    }

    if (mNextPTSTimeUs >= 0ll) {
        sp<AMessage> extra = new AMessage;
        // Since we are using absolute timestamps, signal an offset of 0 to prevent
        // ATSParser from skewing the timestamps of access units.
        extra->setInt64(IStreamListener::kKeyMediaTimeUs, 0);

        // When adapting, signal a recent media time to the parser,
        // so that PTS wrap around is handled for the new variant.
        if (mStartTimeUs >= 0 && !mStartTimeUsRelative) {
            extra->setInt64(IStreamListener::kKeyRecentMediaTimeUs, mStartTimeUs);
        }

        mTSParser->signalDiscontinuity(
                ATSParser::DISCONTINUITY_TIME, extra);

        mNextPTSTimeUs = -1ll;
    }

    size_t offset = 0;
    while (offset + 188 <= buffer->size()) {
        status_t err = mTSParser->feedTSPacket(buffer->data() + offset, 188);

#ifdef RTK_PLATFORM
        if(err==BAD_VALUE) {
            uint32_t PacketSize = 188;
            const uint8_t *packetTempS = (const uint8_t *)(buffer->data()+offset);
            for (int index = 0; index <= (int)(buffer->size() - 3 * PacketSize); index++) {
                if((packetTempS[index] == 0x47) && (packetTempS[index+ PacketSize] == 0x47) && (packetTempS[index+ PacketSize*2] == 0x47)){
                    ALOGD("%s find next sync word", __FUNCTION__);
                    offset += index;
                    err = OK;
                    break;
                }
            }
            if(err==BAD_VALUE) {
                ALOGE("%s can not find sync word", __FUNCTION__);
            }
        }
#endif
        if (err != OK) {
            return err;
        }

        offset += 188;
    }
    // setRange to indicate consumed bytes.
    buffer->setRange(buffer->offset() + offset, buffer->size() - offset);
    }//end else  mUseRTKExtFlow

#ifdef RTK_PLATFORM
    status_t err = mTSParser->isVideoSizeSuitable();
    if(err != OK)
        return err;
#endif

    if (mSegmentFirstPTS < 0ll) {
        // get the smallest first PTS from all streams present in this parser
        for (size_t i = mPacketSources.size(); i > 0;) {
            i--;
            const LiveSession::StreamType stream = mPacketSources.keyAt(i);
            if (stream == LiveSession::STREAMTYPE_SUBTITLES) {
                ALOGE("MPEG2 Transport streams do not contain subtitles.");

                return ERROR_MALFORMED;
            }
            if (stream == LiveSession::STREAMTYPE_METADATA) {

                continue;
            }

            sp<AnotherPacketSource> source =NULL;
            if(mUseRTKExtFlow == 1)
            {
                //track = (stream == LiveSession::STREAMTYPE_VIDEO) ? &mVideoTrack : &mAudioTrack;
                if(stream == LiveSession::STREAMTYPE_VIDEO)
                    track = &mVideoTrack;
                else if(stream == LiveSession::STREAMTYPE_AUDIO)
                    track = &mAudioTrack;
                if(track)
                    source =track->mPackets;
            }else
            {
                ATSParser::SourceType type =LiveSession::getSourceTypeForStream(stream);
                source = static_cast<AnotherPacketSource *>(
                        mTSParser->getSource(type).get());
            }

            if (source == NULL) {
                continue;
            }
            sp<AMessage> meta = source->getMetaAfterLastDequeued(0);
            if (meta != NULL) {
                int64_t timeUs;
                CHECK(meta->findInt64("timeUs", &timeUs));
                if (mSegmentFirstPTS < 0ll || timeUs < mSegmentFirstPTS) {
                    mSegmentFirstPTS = timeUs;
                }
            }
        }
        if (mSegmentFirstPTS < 0ll) {
            // didn't find any TS packet, can return early
            return OK;
        }
        if (!mStartTimeUsRelative) {
            // mStartup
            //   mStartup is true until we have queued a packet for all the streams
            //   we are fetching. We queue packets whose timestamps are greater than
            //   mStartTimeUs.
            // mSegmentStartTimeUs >= 0
            //   mSegmentStartTimeUs is non-negative when adapting or switching tracks
            // adjustSeqNumberWithAnchorTime(timeUs) == true
            //   we guessed a seq number that's either too large or too small.
            // If this happens, we'll adjust mSeqNumber and restart fetching from new
            // location. Note that we only want to adjust once, so set mSegmentStartTimeUs
            // to -1 so that we don't enter this chunk next time.
            if (mStartup && mSegmentStartTimeUs >= 0
                    && adjustSeqNumberWithAnchorTime(mSegmentFirstPTS)) {
                mStartTimeUsNotify = mNotify->dup();
                mStartTimeUsNotify->setInt32("what", kWhatStartedAt);
                mStartTimeUsNotify->setString("uri", mURI);
                mIDRFound = false;
                mSegmentStartTimeUs = -1;
                return -EAGAIN;
            }
        }
    }
    err = OK;
    for (size_t i = mPacketSources.size(); i > 0;) {
        i--;
        sp<AnotherPacketSource> packetSource = mPacketSources.valueAt(i);

        const LiveSession::StreamType stream = mPacketSources.keyAt(i);
        if (stream == LiveSession::STREAMTYPE_SUBTITLES) {
            ALOGE("MPEG2 Transport streams do not contain subtitles.");
            return ERROR_MALFORMED;
        }

        const char *key = LiveSession::getKeyForStream(stream);
        sp<AnotherPacketSource> source =NULL;
        if(mUseRTKExtFlow == 1)
        {
            track = NULL;
            if(stream == LiveSession::STREAMTYPE_VIDEO)
                track = &mVideoTrack;
            else if(stream == LiveSession::STREAMTYPE_AUDIO)
                track = &mAudioTrack;

            if(track)
                source = track->mPackets;

        }else
        {
            ATSParser::SourceType type =LiveSession::getSourceTypeForStream(stream);
            source = static_cast<AnotherPacketSource *>(
                    mTSParser->getSource(type).get());
        }

        if (source == NULL) {
            continue;
        }

        const char *mime;
        sp<MetaData> format  = source->getFormat();
        bool isAvc = format != NULL && format->findCString(kKeyMIMEType, &mime)
                && !strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC);

        sp<ABuffer> accessUnit;
        status_t finalResult;
        while (source->hasBufferAvailable(&finalResult)
                && source->dequeueAccessUnit(&accessUnit) == OK) {

            int64_t timeUs;
            CHECK(accessUnit->meta()->findInt64("timeUs", &timeUs));

            if (mStartup) {
#ifndef RTK_PLATFORM
                bool startTimeReached = isStartTimeReached(timeUs);
#else
                //Skip seeking internally for better performance and smooth playback
                bool startTimeReached = true;
#endif

                if (!startTimeReached || (isAvc && !mIDRFound)) {
                    // buffer up to the closest preceding IDR frame in the next segement,
                    // or the closest succeeding IDR frame after the exact position
                    FSLOGV(stream, "timeUs(%lld)-mStartTimeUs(%lld)=%lld, mIDRFound=%d",
                            (long long)timeUs,
                            (long long)mStartTimeUs,
                            (long long)timeUs - mStartTimeUs,
                            mIDRFound);
                    if (isAvc) {
                        if (IsIDR(accessUnit)) {
                            mVideoBuffer->clear();
                            FSLOGV(stream, "found IDR, clear mVideoBuffer");
                            mIDRFound = true;
                        }
                        if (mIDRFound && mStartTimeUsRelative && !startTimeReached) {
                            mVideoBuffer->queueAccessUnit(accessUnit);
                            FSLOGV(stream, "saving AVC video AccessUnit");
                        }
                    }
#ifdef RTK_PLATFORM
                    if (!startTimeReached)
#else
                    if (!startTimeReached || (isAvc && !mIDRFound))
#endif
                    {
                        continue;
                    }
                }
            }

            if (mStartTimeUsNotify != NULL) {
                uint32_t streamMask = 0;
                mStartTimeUsNotify->findInt32("streamMask", (int32_t *) &streamMask);
                if ((mStreamTypeMask & mPacketSources.keyAt(i))
                        && !(streamMask & mPacketSources.keyAt(i))) {
                    streamMask |= mPacketSources.keyAt(i);
                    mStartTimeUsNotify->setInt32("streamMask", streamMask);
                    FSLOGV(stream, "found start point, timeUs=%lld, streamMask becomes %x",
                            (long long)timeUs, streamMask);

                    if (streamMask == mStreamTypeMask) {
                        FLOGV("found start point for all streams");
                        mStartup = false;
                    }
                }
            }

            if (mStopParams != NULL) {
                int32_t discontinuitySeq;
                int64_t stopTimeUs;
                if (!mStopParams->findInt32("discontinuitySeq", &discontinuitySeq)
                        || discontinuitySeq > mDiscontinuitySeq
                        || !mStopParams->findInt64(key, &stopTimeUs)
                        || (discontinuitySeq == mDiscontinuitySeq
                                && timeUs >= stopTimeUs)) {
                    FSLOGV(stream, "reached stop point, timeUs=%lld", (long long)timeUs);
                    mStreamTypeMask &= ~stream;
                    mPacketSources.removeItemsAt(i);
                    break;
                }
            }

            if (stream == LiveSession::STREAMTYPE_VIDEO) {
                const bool discard = true;
                status_t status;
                while (mVideoBuffer->hasBufferAvailable(&status)) {
                    sp<ABuffer> videoBuffer;
                    mVideoBuffer->dequeueAccessUnit(&videoBuffer);
                    setAccessUnitProperties(videoBuffer, source, discard);
                    packetSource->queueAccessUnit(videoBuffer);
                    int64_t bufferTimeUs;
                    CHECK(videoBuffer->meta()->findInt64("timeUs", &bufferTimeUs));
                    FSLOGV(stream, "queueAccessUnit (saved), timeUs=%lld",
                            (long long)bufferTimeUs);
                }
            } else if (stream == LiveSession::STREAMTYPE_METADATA && !mHasMetadata) {
                mHasMetadata = true;
                sp<AMessage> notify = mNotify->dup();
                notify->setInt32("what", kWhatMetadataDetected);
                notify->post();
            }

            setAccessUnitProperties(accessUnit, source);
            packetSource->queueAccessUnit(accessUnit);
            FSLOGV(stream, "queueAccessUnit, timeUs=%lld", (long long)timeUs);
#ifdef RTK_PLATFORM
            QueueSeqInfoPernSec(stream, timeUs);
#endif

        }

        if (err != OK) {
            break;
        }
    }

    if (err != OK) {
        for (size_t i = mPacketSources.size(); i > 0;) {
            i--;
            sp<AnotherPacketSource> packetSource = mPacketSources.valueAt(i);
            packetSource->clear();
        }
        return err;
    }

    if (!mStreamTypeMask) {
        // Signal gap is filled between original and new stream.
        FLOGV("reached stop point for all streams");
        return ERROR_OUT_OF_RANGE;
    }

    return OK;
}

/* static */
bool PlaylistFetcher::bufferStartsWithWebVTTMagicSequence(
        const sp<ABuffer> &buffer) {
    size_t pos = 0;

    // skip possible BOM
    if (buffer->size() >= pos + 3 &&
            !memcmp("\xef\xbb\xbf", buffer->data() + pos, 3)) {
        pos += 3;
    }

    // accept WEBVTT followed by SPACE, TAB or (CR) LF
    if (buffer->size() < pos + 6 ||
            memcmp("WEBVTT", buffer->data() + pos, 6)) {
        return false;
    }
    pos += 6;

    if (buffer->size() == pos) {
        return true;
    }

    uint8_t sep = buffer->data()[pos];
    return sep == ' ' || sep == '\t' || sep == '\n' || sep == '\r';
}

status_t PlaylistFetcher::extractAndQueueAccessUnits(
        const sp<ABuffer> &buffer, const sp<AMessage> &itemMeta) {
    if (bufferStartsWithWebVTTMagicSequence(buffer)) {
        if (mStreamTypeMask != LiveSession::STREAMTYPE_SUBTITLES) {
            ALOGE("This stream only contains subtitles.");
            return ERROR_MALFORMED;
        }

        const sp<AnotherPacketSource> packetSource =
            mPacketSources.valueFor(LiveSession::STREAMTYPE_SUBTITLES);

        int64_t durationUs;
        CHECK(itemMeta->findInt64("durationUs", &durationUs));
        buffer->meta()->setInt64("timeUs", getSegmentStartTimeUs(mSeqNumber));
        buffer->meta()->setInt64("durationUs", durationUs);
        buffer->meta()->setInt64("segmentStartTimeUs", getSegmentStartTimeUs(mSeqNumber));
        buffer->meta()->setInt32("discontinuitySeq", mDiscontinuitySeq);
        buffer->meta()->setInt32("subtitleGeneration", mSubtitleGeneration);
        packetSource->queueAccessUnit(buffer);
        return OK;
    }

    if (mNextPTSTimeUs >= 0ll) {
        mNextPTSTimeUs = -1ll;
    }

    // This better be an ISO 13818-7 (AAC) or ISO 13818-1 (MPEG) audio
    // stream prefixed by an ID3 tag.

    bool firstID3Tag = true;
    uint64_t PTS = 0;

    for (;;) {
        // Make sure to skip all ID3 tags preceding the audio data.
        // At least one must be present to provide the PTS timestamp.

        ID3 id3(buffer->data(), buffer->size(), true /* ignoreV1 */);
        if (!id3.isValid()) {
            if (firstID3Tag) {
                ALOGE("Unable to parse ID3 tag.");
                return ERROR_MALFORMED;
            } else {
                break;
            }
        }

        if (firstID3Tag) {
            bool found = false;

            ID3::Iterator it(id3, "PRIV");
            while (!it.done()) {
                size_t length;
                const uint8_t *data = it.getData(&length);
                if (!data) {
                    return ERROR_MALFORMED;
                }

                static const char *kMatchName =
                    "com.apple.streaming.transportStreamTimestamp";
                static const size_t kMatchNameLen = strlen(kMatchName);

                if (length == kMatchNameLen + 1 + 8
                        && !strncmp((const char *)data, kMatchName, kMatchNameLen)) {
                    found = true;
                    PTS = U64_AT(&data[kMatchNameLen + 1]);
                }

                it.next();
            }

            if (!found) {
                ALOGE("Unable to extract transportStreamTimestamp from ID3 tag.");
                return ERROR_MALFORMED;
            }
        }

        // skip the ID3 tag
        buffer->setRange(
                buffer->offset() + id3.rawSize(), buffer->size() - id3.rawSize());

        firstID3Tag = false;
    }

    if (mStreamTypeMask != LiveSession::STREAMTYPE_AUDIO) {
        ALOGW("This stream only contains audio data!");

        mStreamTypeMask &= LiveSession::STREAMTYPE_AUDIO;

        if (mStreamTypeMask == 0) {
            return OK;
        }
    }

    sp<AnotherPacketSource> packetSource =
        mPacketSources.valueFor(LiveSession::STREAMTYPE_AUDIO);

    if (packetSource->getFormat() == NULL && buffer->size() >= 7) {
        ABitReader bits(buffer->data(), buffer->size());

        // adts_fixed_header

        CHECK_EQ(bits.getBits(12), 0xfffu);
        bits.skipBits(3);  // ID, layer
        bool protection_absent __unused = bits.getBits(1) != 0;

        unsigned profile = bits.getBits(2);
        CHECK_NE(profile, 3u);
        unsigned sampling_freq_index = bits.getBits(4);
        bits.getBits(1);  // private_bit
        unsigned channel_configuration = bits.getBits(3);
        CHECK_NE(channel_configuration, 0u);
        bits.skipBits(2);  // original_copy, home

        sp<MetaData> meta = MakeAACCodecSpecificData(
                profile, sampling_freq_index, channel_configuration);

        meta->setInt32(kKeyIsADTS, true);

        packetSource->setFormat(meta);
    }

    int64_t numSamples = 0ll;
    int32_t sampleRate;
    CHECK(packetSource->getFormat()->findInt32(kKeySampleRate, &sampleRate));

    int64_t timeUs = (PTS * 100ll) / 9ll;
    if (mStartup && !mFirstPTSValid) {
        mFirstPTSValid = true;
        mFirstTimeUs = timeUs;
    }

    if (mSegmentFirstPTS < 0ll) {
        mSegmentFirstPTS = timeUs;
        if (!mStartTimeUsRelative) {
            // Duplicated logic from how we handle .ts playlists.
            if (mStartup && mSegmentStartTimeUs >= 0
                    && adjustSeqNumberWithAnchorTime(timeUs)) {
                mSegmentStartTimeUs = -1;
                return -EAGAIN;
            }
        }
    }

    size_t offset = 0;
    while (offset < buffer->size()) {
        const uint8_t *adtsHeader = buffer->data() + offset;
        CHECK_LT(offset + 5, buffer->size());

        unsigned aac_frame_length =
            ((adtsHeader[3] & 3) << 11)
            | (adtsHeader[4] << 3)
            | (adtsHeader[5] >> 5);

        if (aac_frame_length == 0) {
            const uint8_t *id3Header = adtsHeader;
            if (!memcmp(id3Header, "ID3", 3)) {
                ID3 id3(id3Header, buffer->size() - offset, true);
                if (id3.isValid()) {
                    offset += id3.rawSize();
                    continue;
                };
            }
            return ERROR_MALFORMED;
        }

        CHECK_LE(offset + aac_frame_length, buffer->size());

        int64_t unitTimeUs = timeUs + numSamples * 1000000ll / sampleRate;
        offset += aac_frame_length;

        // Each AAC frame encodes 1024 samples.
        numSamples += 1024;

        if (mStartup) {
            int64_t startTimeUs = unitTimeUs;
            if (mStartTimeUsRelative) {
                startTimeUs -= mFirstTimeUs;
                if (startTimeUs  < 0) {
                    startTimeUs = 0;
                }
            }
            if (startTimeUs < mStartTimeUs) {
                continue;
            }

            if (mStartTimeUsNotify != NULL) {
                mStartTimeUsNotify->setInt32("streamMask", LiveSession::STREAMTYPE_AUDIO);
                mStartup = false;
            }
        }

        if (mStopParams != NULL) {
            int32_t discontinuitySeq;
            int64_t stopTimeUs;
            if (!mStopParams->findInt32("discontinuitySeq", &discontinuitySeq)
                    || discontinuitySeq > mDiscontinuitySeq
                    || !mStopParams->findInt64("timeUsAudio", &stopTimeUs)
                    || (discontinuitySeq == mDiscontinuitySeq && unitTimeUs >= stopTimeUs)) {
                mStreamTypeMask = 0;
                mPacketSources.clear();
                return ERROR_OUT_OF_RANGE;
            }
        }

        sp<ABuffer> unit = new ABuffer(aac_frame_length);
        memcpy(unit->data(), adtsHeader, aac_frame_length);

        unit->meta()->setInt64("timeUs", unitTimeUs);
        setAccessUnitProperties(unit, packetSource);
        packetSource->queueAccessUnit(unit);
    }

    return OK;
}

void PlaylistFetcher::updateDuration() {
    int64_t durationUs = 0ll;
    for (size_t index = 0; index < mPlaylist->size(); ++index) {
        sp<AMessage> itemMeta;
        CHECK(mPlaylist->itemAt(
                    index, NULL /* uri */, &itemMeta));

        int64_t itemDurationUs;
        CHECK(itemMeta->findInt64("durationUs", &itemDurationUs));

        durationUs += itemDurationUs;
    }

    sp<AMessage> msg = mNotify->dup();
    msg->setInt32("what", kWhatDurationUpdate);
    msg->setInt64("durationUs", durationUs);
    msg->post();
}

void PlaylistFetcher::updateTargetDuration() {
    sp<AMessage> msg = mNotify->dup();
    msg->setInt32("what", kWhatTargetDurationUpdate);
    msg->setInt64("targetDurationUs", mPlaylist->getTargetDuration());
    msg->post();
}

#ifdef RTK_PLATFORM

void PlaylistFetcher::signalSeekto()
{
    mSignalSeekTo = 1;
}

int32_t PlaylistFetcher::checkStatus()
{
    int32_t dStatus = kPlaylistStatus_OK;

    if(mIsBufferFilled){
        return kPlaylistStatus_OK;
    }

    if(StatusTakeDurTime(&mSeqUpdate_tv) > kPlaylistOverTime)
    {
        ALOGE("[%s][%d] kPlaylistStatus_FetchFile LineInfo:: %d overtime::%d need reconnect.\n",__FUNCTION__,__LINE__, mStatusLineInfo, StatusTakeDurTime(&mSeqUpdate_tv));
        return kPlaylistStatus_FetchFileBlocked;
    }

    if(mStatusFlowInfo == kPlaylistStatus_Normal)
        return kPlaylistStatus_OK;

    if(mStatusFlowInfo == kPlaylistStatus_FetchFile && StatusTakeDurTime(&start_tv) > kPlaylistOverTime)
    {
        ALOGE("[%s][%d] kPlaylistStatus_FetchFile LineInfo:: %d overtime::%d need reconnect.\n",__FUNCTION__,__LINE__, mStatusLineInfo, StatusTakeDurTime(&start_tv));
        return kPlaylistStatus_FetchFileBlocked;
    }
    if(mStatusFlowInfo == kPlaylistStatus_initDownloadState && StatusTakeDurTime(&start_tv) > kPlaylistOverTime)
    {
        ALOGE("[%s][%d] kPlaylistStatus_initDownloadState LineInfo:: %d overtime::%d need reconnect.\n",__FUNCTION__,__LINE__, mStatusLineInfo, StatusTakeDurTime(&start_tv));
        return kPlaylistStatus_FetchStatusError;
    }
    if(mStatusFlowInfo != kPlaylistStatus_Normal && StatusTakeDurTime(&start_tv) > (kPlaylistOverTime / 2))
    {
        ALOGI("[%s][%d] kPlaylistStatus:: LineInfo:: %d overtime::%d need reconnect.\n",__FUNCTION__,__LINE__, mStatusLineInfo, StatusTakeDurTime(&start_tv));
    }


    return dStatus;

}

void PlaylistFetcher::SetLineInfo(int32_t dLineInfo, int32_t dFlowInfo)
{
    mStatusLineInfo = dLineInfo;
    mStatusFlowInfo = dFlowInfo;
    ALOGV("[%s][%d] mStatusLineInfo:: %4d, mStatusFlowInfo:: %4d.\n",__FUNCTION__,__LINE__, mStatusLineInfo, mStatusFlowInfo);
}

void PlaylistFetcher::StatusUpdateRecTime()
{
    gettimeofday(&start_tv, NULL);
}

void PlaylistFetcher::StatusUpdatemSeqNumber()
{
    gettimeofday(&mSeqUpdate_tv, NULL);
}

unsigned int PlaylistFetcher::StatusTakeDurTime(struct timeval *start_tv)
{
    gettimeofday(&end_tv, NULL);
    return (unsigned int)((end_tv.tv_sec - start_tv->tv_sec)*1000 + (end_tv.tv_usec - start_tv->tv_usec)/1000);
}
void PlaylistFetcher::initSeqQue()
{
    initCurSeqInfo(0);
    mStrSeqQue[MEDIA_AUDIO].clear();
    mStrSeqQue[MEDIA_VIDEO].clear();
}

void PlaylistFetcher::initCurSeqInfo(int64_t durationUs)
{
    mCurSeqInfo[MEDIA_AUDIO].mdurationUs = durationUs;
    mCurSeqInfo[MEDIA_AUDIO].mCurSeqNumber = mSeqNumber;
    mCurSeqInfo[MEDIA_AUDIO].mIdx = -1;
    mCurSeqInfo[MEDIA_AUDIO].mIsNeedRebase =false;
    mCurSeqInfo[MEDIA_AUDIO].mOffsetUs = 0;
    mCurSeqInfo[MEDIA_AUDIO].mTimeUs[MEDIA_TIME_NUM - 1] = 0;

    mCurSeqInfo[MEDIA_VIDEO].mdurationUs = durationUs;
    mCurSeqInfo[MEDIA_VIDEO].mCurSeqNumber = mSeqNumber;
    mCurSeqInfo[MEDIA_VIDEO].mIdx = -1;
    mCurSeqInfo[MEDIA_VIDEO].mIsNeedRebase =false;
    mCurSeqInfo[MEDIA_VIDEO].mOffsetUs = 0;
    mCurSeqInfo[MEDIA_VIDEO].mTimeUs[MEDIA_TIME_NUM - 1] = 0;

}
int64_t PlaylistFetcher::diff(int64_t num1, int64_t num2)
{
    if(num1 > num2)
        return (num1 - num2);
    return (num2 - num1);
}

void PlaylistFetcher::QueueSeqInfoPernSec(LiveSession::StreamType stream, int64_t timeUs)
{
    ssize_t streamIdx = typeToIndex(stream);
    if(streamIdx == -1)
        return;

    strSeqInfo *strSeqQue;
    strSeqQue = &mCurSeqInfo[streamIdx];
    int32_t mIdx = strSeqQue->mIdx;
    if((strSeqQue->mIdx < MEDIA_TIME_START_STORE_NUM) &&
            (mIdx < MEDIA_TIME_NUM - 1 && diff(timeUs , strSeqQue->mTimeUs[mIdx]) > 2*1E6))
    {
        mIdx++;
        strSeqQue->mTimeUs[mIdx] = timeUs;
        strSeqQue->mIdx = mIdx;
    }
    //MEDIA_TIME_NUM - 1 is last timeUs.
    strSeqQue->mTimeUs[MEDIA_TIME_NUM - 1] = timeUs;
}

ssize_t PlaylistFetcher::typeToIndex(int32_t type)
{
    switch (type) {
        case LiveSession::STREAMTYPE_AUDIO:
            return 0;
        case LiveSession::STREAMTYPE_VIDEO:
            return 1;
        default:
            return -1;
    };
    return -1;
}

void PlaylistFetcher::QueueSeqInfo(ssize_t streamIdx)
{
    Vector<strSeqInfo> *strSeqQue;
    strSeqQue = &mStrSeqQue[streamIdx];

    strSeqQue->push(mCurSeqInfo[streamIdx]);
}
void PlaylistFetcher::LogSeqInfo(int32_t flag)
{
    size_t index = 0;
    Vector<strSeqInfo> *strSeqQue;
    int32_t Idx = 0, IsShowAll = flag, n = 0;

    for(Idx = 0; Idx <= 1; Idx++)
    {
        strSeqQue = &mStrSeqQue[Idx];
        ALOGI("[%s][%d] [%s] Idx::%d size::%d\n",__FUNCTION__,__LINE__,(Idx == 0)?"AUDIO":"VIDEO", (int)Idx, (int)strSeqQue->size());
        for (index = 0; index <= strSeqQue->size(); index++)
        {
            if(strSeqQue->size() == 0)
                break;
            strSeqInfo item = (*strSeqQue)[index];
            if(index < strSeqQue->size())
            {
                item = (*strSeqQue)[index];
            }else
            {
                item = mCurSeqInfo[Idx];
                ALOGI("[%s][%d] mCurSeqInfo:: \n",__FUNCTION__,__LINE__);
            }
            if(IsShowAll == 0)
            {
                if(item.mIdx > 0)
                ALOGD("[%s][%d] Seq[%2d] Num:[%4d],timeUs [0]::%5.3f \t[mIdx:%2d]::%5.3f, \tNreBase:%d. Off::%5.3f.\t durationUs::%5.3f\n", __FUNCTION__,__LINE__, (int)index,
                        item.mCurSeqNumber, codec_show_pts(item.mTimeUs[0]), item.mIdx,codec_show_pts(item.mTimeUs[item.mIdx]), item.mIsNeedRebase,
                        codec_show_pts(item.mOffsetUs), codec_show_pts(item.mdurationUs));
            }
            else
            {
                ALOGD("[%s][%d] Seq[%2d] Num:[%4d],timeUs mIdx:%2d NreBase:%d. Off::%5.3f.\t durationUs::%5.3f\n", __FUNCTION__,__LINE__, (int)index,
                        item.mCurSeqNumber, item.mIdx, item.mIsNeedRebase,
                        codec_show_pts(item.mOffsetUs), codec_show_pts(item.mdurationUs));
                for (n = 0; n <= item.mIdx; n++)
                {
                    ALOGI("[%s][%d] [%2d]%5.3f.\n",__FUNCTION__,__LINE__,n,codec_show_pts(item.mTimeUs[n]));
                }
                ALOGI("[%s][%d] [%2d]%5.3f.\n",__FUNCTION__,__LINE__,MEDIA_TIME_NUM - 1,codec_show_pts(item.mTimeUs[MEDIA_TIME_NUM - 1]));
            }

            if(index >= 10)
                break;

        }
    }

}

int32_t PlaylistFetcher::chkIsSeqDiscontinuity_(ssize_t streamIdx, int32_t curSeqNumber, int64_t timeUs, bool *IsSeqBoundary)
{
    size_t index = 0;
    Vector<strSeqInfo> *strSeqQue;
    strSeqQue = &mStrSeqQue[streamIdx];
    int32_t n = 0,flag = 0, nCnt = 0;

    for (index = 0; index <= strSeqQue->size(); index++)
    {
        strSeqInfo item;
        if(index < strSeqQue->size())
        {
            item = (*strSeqQue)[index];
        }else
        {
            item = mCurSeqInfo[streamIdx];
            ALOGI("[%s][%d] set mCurSeqInfo for item\n",__FUNCTION__,__LINE__);
        }

        if(item.mCurSeqNumber == curSeqNumber && curSeqNumber > 0)
        {
            //check item.mdurationUs
            n = MEDIA_TIME_START_STORE_NUM - 1;
            if(item.mIdx < MEDIA_TIME_START_STORE_NUM)
                 n = item.mIdx - 1;
            if(item.mTimeUs[MEDIA_TIME_NUM - 1] - item.mTimeUs[n] > item.mdurationUs)
            {
                ALOGW("[%s][%d] reset item.mdurationUs ::from %f to %f = %f - %f n: %d.\n",__FUNCTION__,__LINE__,
                        codec_show_pts(item.mdurationUs), codec_show_pts((item.mTimeUs[MEDIA_TIME_NUM - 1] - item.mTimeUs[n])),
                        codec_show_pts(item.mTimeUs[MEDIA_TIME_NUM - 1]), codec_show_pts(item.mTimeUs[n]),n);
                item.mdurationUs = item.mTimeUs[MEDIA_TIME_NUM - 1] - item.mTimeUs[n];
            }

            //check current Idx discontinuity.
            for (n = 0; n < MEDIA_TIME_START_STORE_NUM; n++)
            {
                if(n == item.mIdx)
                    break;
                if(item.mTimeUs[MEDIA_TIME_NUM - 1] > 0)
                {
                    if(diff(item.mTimeUs[MEDIA_TIME_NUM - 1] , item.mTimeUs[n]) > item.mdurationUs &&
                            diff(item.mTimeUs[n], item.mTimeUs[n + 1]) > MEDIA_DISCONTINUITY_THLD)
                    {
                        ALOGI("[%s][%d] check MEDIA_DISCONTINUITY_THLD %f %f\n",__FUNCTION__,__LINE__, codec_show_pts(item.mTimeUs[n + 1]), codec_show_pts(timeUs));
                        flag = 1;
                    }
                }
                if(IsSeqBoundary != NULL && *IsSeqBoundary == false)
                {
                    if(diff(item.mTimeUs[n], timeUs) < MEDIA_DISCONTINUITY_THLD)
                    {
                        *IsSeqBoundary = true;
                        ALOGI("[%s][%d] IsSeqBoundary true \n",__FUNCTION__,__LINE__);
                    }
                }
            }

            //check pre index discontinuity.
            if(index > 0)
            {
                const strSeqInfo &Preitem = (*strSeqQue)[index - 1];
                if(diff(item.mTimeUs[0],Preitem.mTimeUs[MEDIA_TIME_NUM - 1]) > MEDIA_DISCONTINUITY_THLD)
                {
                    ALOGI("[%s][%d] check MEDIA_DISCONTINUITY_THLD %f %f\n",__FUNCTION__,__LINE__, codec_show_pts(Preitem.mTimeUs[MEDIA_TIME_NUM - 1]), codec_show_pts(timeUs));
                    flag = 1;
                }

                if(IsSeqBoundary != NULL && *IsSeqBoundary == false)
                {
                    if(diff(Preitem.mTimeUs[MEDIA_TIME_NUM - 1], timeUs) < MEDIA_DISCONTINUITY_THLD)
                    {
                        *IsSeqBoundary = true;
                        ALOGI("[%s][%d] IsSeqBoundary true \n",__FUNCTION__,__LINE__);
                    }
                }
            }
            if(flag == 1)
                break;
        }
        nCnt++;
        if(item.mCurSeqNumber > curSeqNumber)
            break;
    }

    return flag;

}
int32_t PlaylistFetcher::chkIsSeqDiscontinuity(ssize_t streamIdx, ssize_t dstreamIdx, int32_t curSeqNumber, int64_t timeUs, bool *IsSeqBoundary)
{
    ALOGD("[%s][%d] curSeqNumber::%d timeUs::%f \n",__FUNCTION__,__LINE__,curSeqNumber,codec_show_pts(timeUs));
    LogSeqInfo(1);
    if(IsSeqBoundary != NULL)
        *IsSeqBoundary = false;

    if(chkIsSeqDiscontinuity_(streamIdx, curSeqNumber, timeUs, IsSeqBoundary) == 1 &&
            chkIsSeqDiscontinuity_(dstreamIdx, curSeqNumber, timeUs, 0) == 1)
        return 1;

    return 0;
}

int32_t PlaylistFetcher::updateMinSeq(int32_t minSeqNum)
{
    Vector<strSeqInfo> *strSeqQue;
    size_t index = 0;
    int dIsInListFlg = 0, dIsCurListFlg = 0;

    //check minSeqNum is in list or not.
    for(int n = 0; n < 2; n++)
    {
        strSeqQue = &mStrSeqQue[n];
        for (index = 0; index <= strSeqQue->size(); index++)
        {
            strSeqInfo item;
            if(index < strSeqQue->size())
                item = (*strSeqQue)[index];
            else
            {
                item = mCurSeqInfo[n];
                dIsCurListFlg = 1;
            }

            if(item.mCurSeqNumber == minSeqNum)
                dIsInListFlg = 1;
            if(dIsInListFlg == 1)
                break;
        }
        if(dIsInListFlg == 1)
            break;
    }
    if(dIsInListFlg == 1 && dIsCurListFlg == 0)
    {
        for(int n = 0; n < 2; n++)
        {
            strSeqQue = &mStrSeqQue[n];
            while(strSeqQue->size() > 0)//clear old mStrSeqQue
            {
                const strSeqInfo &item = (*strSeqQue)[0];
                if(item.mCurSeqNumber >= minSeqNum)
                    break;
                strSeqQue->erase(strSeqQue->begin());
            }
        }
    }
    else if(dIsCurListFlg == 0)
    {
        ALOGW("[%s][%d] minSeqNum::%d not in list.\n",__FUNCTION__,__LINE__,minSeqNum);
        LogSeqInfo(1);
    }
    return 0;
}

#endif


}  // namespace android

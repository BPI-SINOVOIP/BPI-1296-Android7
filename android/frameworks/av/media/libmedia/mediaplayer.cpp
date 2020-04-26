/*
**
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

//#define LOG_NDEBUG 0
#define LOG_TAG "MediaPlayer"

#include <fcntl.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <utils/Log.h>

#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include <gui/Surface.h>

#include <media/mediaplayer.h>
#include <media/AudioResamplerPublic.h>
#include <media/AudioSystem.h>
#include <media/AVSyncSettings.h>
#include <media/IDataSource.h>

#include <binder/MemoryBase.h>

#include <utils/KeyedVector.h>
#include <utils/String8.h>

#include <system/audio.h>
#include <system/window.h>

#ifdef USE_RT_MEDIA_PLAYER
#include <cutils/properties.h>
#endif

namespace android {

MediaPlayer::MediaPlayer()
{
    ALOGV("constructor");
    mListener = NULL;
    mCookie = NULL;
    mStreamType = AUDIO_STREAM_MUSIC;
    mAudioAttributesParcel = NULL;
    mCurrentPosition = -1;
    mSeekPosition = -1;
    mCurrentState = MEDIA_PLAYER_IDLE;
    mPrepareSync = false;
    mPrepareStatus = NO_ERROR;
    mLoop = false;
    mLeftVolume = mRightVolume = 1.0;
    mVideoWidth = mVideoHeight = 0;
    mLockThreadId = 0;
    mAudioSessionId = (audio_session_t) AudioSystem::newAudioUniqueId(AUDIO_UNIQUE_ID_USE_SESSION);
    AudioSystem::acquireAudioSessionId(mAudioSessionId, -1);
    mSendLevel = 0;
    mRetransmitEndpointValid = false;
#ifdef USE_RT_MEDIA_PLAYER
    m_psSetGetNavAddr = 0;
    m_psSetGetNavVirAddr = 0;
    char value[PROPERTY_VALUE_MAX];
    if (property_get("rtk.media.FORCE_RTMediaPlayer", value, NULL)
            && (!strcmp("1", value) || !strcasecmp("true", value)))
        mForceUseRTMediaPlayer = FORCE_RT_MEDIAPLAYER;
    else
        mForceUseRTMediaPlayer = FORCE_NONE;
#endif // USE_RT_MEDIA_PLAYER
}

MediaPlayer::~MediaPlayer()
{
    ALOGV("destructor");
    if (mAudioAttributesParcel != NULL) {
        delete mAudioAttributesParcel;
        mAudioAttributesParcel = NULL;
    }
    AudioSystem::releaseAudioSessionId(mAudioSessionId, -1);
    disconnect();
    IPCThreadState::self()->flushCommands();
}

void MediaPlayer::disconnect()
{
    ALOGV("disconnect");
#ifdef USE_RT_MEDIA_PLAYER
    Mutex::Autolock _R(mRtkLock);
#endif // USE_RT_MEDIA_PLAYER
    sp<IMediaPlayer> p;
    {
        Mutex::Autolock _l(mLock);
        p = mPlayer;
        mPlayer.clear();
    }

    if (p != 0) {
        p->disconnect();
    }
}

// always call with lock held
void MediaPlayer::clear_l()
{
    mCurrentPosition = -1;
    mSeekPosition = -1;
    mVideoWidth = mVideoHeight = 0;
    mRetransmitEndpointValid = false;
}

status_t MediaPlayer::setListener(const sp<MediaPlayerListener>& listener)
{
    ALOGV("setListener");
    Mutex::Autolock _l(mLock);
    mListener = listener;
    return NO_ERROR;
}


status_t MediaPlayer::attachNewPlayer(const sp<IMediaPlayer>& player)
{
    status_t err = UNKNOWN_ERROR;
    sp<IMediaPlayer> p;
    { // scope for the lock
        Mutex::Autolock _l(mLock);

        if ( !( (mCurrentState & MEDIA_PLAYER_IDLE) ||
                (mCurrentState == MEDIA_PLAYER_STATE_ERROR ) ) ) {
            ALOGE("attachNewPlayer called in state %d", mCurrentState);
            return INVALID_OPERATION;
        }

        clear_l();
        p = mPlayer;
        mPlayer = player;
        if (player != 0) {
            mCurrentState = MEDIA_PLAYER_INITIALIZED;
            err = NO_ERROR;
        } else {
            ALOGE("Unable to create media player");
        }
    }

    if (p != 0) {
        p->disconnect();
    }

    return err;
}

status_t MediaPlayer::setDataSource(
        const sp<IMediaHTTPService> &httpService,
        const char *url, const KeyedVector<String8, String8> *headers)
{
    ALOGV("setDataSource(%s)", url);
    status_t err = BAD_VALUE;
    if (url != NULL) {
#ifdef USE_RT_MEDIA_PLAYER
        Mutex::Autolock _R(mRtkLock);
#endif // USE_RT_MEDIA_PLAYER
        const sp<IMediaPlayerService> service(getMediaPlayerService());
        if (service != 0) {
            sp<IMediaPlayer> player(service->create(this, mAudioSessionId));
#ifdef USE_RT_MEDIA_PLAYER
            player->useRTMediaPlayer(mForceUseRTMediaPlayer);
#endif // USE_RT_MEDIA_PLAYER
            if ((NO_ERROR != doSetRetransmitEndpoint(player)) ||
                (NO_ERROR != player->setDataSource(httpService, url, headers))) {
                player.clear();
            }
            err = attachNewPlayer(player);
        }
    }
    return err;
}

status_t MediaPlayer::setDataSource(int fd, int64_t offset, int64_t length)
{
    ALOGV("setDataSource(%d, %" PRId64 ", %" PRId64 ")", fd, offset, length);
    status_t err = UNKNOWN_ERROR;
    const sp<IMediaPlayerService> service(getMediaPlayerService());
    if (service != 0) {
        sp<IMediaPlayer> player(service->create(this, mAudioSessionId));
#ifdef USE_RT_MEDIA_PLAYER
        player->useRTMediaPlayer(mForceUseRTMediaPlayer);
#endif // USE_RT_MEDIA_PLAYER        
        if ((NO_ERROR != doSetRetransmitEndpoint(player)) ||
            (NO_ERROR != player->setDataSource(fd, offset, length))) {
            player.clear();
        }
        err = attachNewPlayer(player);
    }
    return err;
}

status_t MediaPlayer::setDataSource(const sp<IDataSource> &source)
{
    ALOGV("setDataSource(IDataSource)");
    status_t err = UNKNOWN_ERROR;
    const sp<IMediaPlayerService> service(getMediaPlayerService());
    if (service != 0) {
        sp<IMediaPlayer> player(service->create(this, mAudioSessionId));
        if ((NO_ERROR != doSetRetransmitEndpoint(player)) ||
            (NO_ERROR != player->setDataSource(source))) {
            player.clear();
        }
        err = attachNewPlayer(player);
    }
    return err;
}

status_t MediaPlayer::invoke(const Parcel& request, Parcel *reply)
{
    Mutex::Autolock _l(mLock);
    const bool hasBeenInitialized =
            (mCurrentState != MEDIA_PLAYER_STATE_ERROR) &&
            ((mCurrentState & MEDIA_PLAYER_IDLE) != MEDIA_PLAYER_IDLE);
    if ((mPlayer != NULL) && hasBeenInitialized) {
        ALOGV("invoke %zu", request.dataSize());
        return  mPlayer->invoke(request, reply);
    }
    ALOGE("invoke failed: wrong state %X, mPlayer(%p)", mCurrentState, mPlayer.get());
    return INVALID_OPERATION;
}

status_t MediaPlayer::setMetadataFilter(const Parcel& filter)
{
    ALOGD("setMetadataFilter");
    Mutex::Autolock lock(mLock);
    if (mPlayer == NULL) {
        return NO_INIT;
    }
    return mPlayer->setMetadataFilter(filter);
}

status_t MediaPlayer::getMetadata(bool update_only, bool apply_filter, Parcel *metadata)
{
    ALOGD("getMetadata");
    Mutex::Autolock lock(mLock);
    if (mPlayer == NULL) {
        return NO_INIT;
    }
    return mPlayer->getMetadata(update_only, apply_filter, metadata);
}

status_t MediaPlayer::setVideoSurfaceTexture(
        const sp<IGraphicBufferProducer>& bufferProducer)
{
    ALOGV("setVideoSurfaceTexture");
    Mutex::Autolock _l(mLock);
    if (mPlayer == 0) return NO_INIT;
    return mPlayer->setVideoSurfaceTexture(bufferProducer);
}

// must call with lock held
status_t MediaPlayer::prepareAsync_l()
{
    if ( (mPlayer != 0) && ( mCurrentState & (MEDIA_PLAYER_INITIALIZED | MEDIA_PLAYER_STOPPED) ) ) {
        if (mAudioAttributesParcel != NULL) {
            mPlayer->setParameter(KEY_PARAMETER_AUDIO_ATTRIBUTES, *mAudioAttributesParcel);
        } else {
            mPlayer->setAudioStreamType(mStreamType);
        }
        mCurrentState = MEDIA_PLAYER_PREPARING;
        return mPlayer->prepareAsync();
    }
    ALOGE("prepareAsync called in state %d, mPlayer(%p)", mCurrentState, mPlayer.get());
    return INVALID_OPERATION;
}

// TODO: In case of error, prepareAsync provides the caller with 2 error codes,
// one defined in the Android framework and one provided by the implementation
// that generated the error. The sync version of prepare returns only 1 error
// code.
status_t MediaPlayer::prepare()
{
    ALOGV("prepare");
    Mutex::Autolock _l(mLock);
    mLockThreadId = getThreadId();
    if (mPrepareSync) {
        mLockThreadId = 0;
        return -EALREADY;
    }
    mPrepareSync = true;
    status_t ret = prepareAsync_l();
    if (ret != NO_ERROR) {
        mLockThreadId = 0;
        return ret;
    }

    if (mPrepareSync) {
        mSignal.wait(mLock);  // wait for prepare done
        mPrepareSync = false;
    }
    ALOGV("prepare complete - status=%d", mPrepareStatus);
    mLockThreadId = 0;
    return mPrepareStatus;
}

status_t MediaPlayer::prepareAsync()
{
    ALOGV("prepareAsync");
    Mutex::Autolock _l(mLock);
    return prepareAsync_l();
}

status_t MediaPlayer::start()
{
    ALOGV("start");

    status_t ret = NO_ERROR;
    Mutex::Autolock _l(mLock);

    mLockThreadId = getThreadId();

    if (mCurrentState & MEDIA_PLAYER_STARTED) {
        ret = NO_ERROR;
    } else if ( (mPlayer != 0) && ( mCurrentState & ( MEDIA_PLAYER_PREPARED |
                    MEDIA_PLAYER_PLAYBACK_COMPLETE | MEDIA_PLAYER_PAUSED ) ) ) {
        mPlayer->setLooping(mLoop);
        mPlayer->setVolume(mLeftVolume, mRightVolume);
        mPlayer->setAuxEffectSendLevel(mSendLevel);
        mCurrentState = MEDIA_PLAYER_STARTED;
        ret = mPlayer->start();
        if (ret != NO_ERROR) {
            mCurrentState = MEDIA_PLAYER_STATE_ERROR;
        } else {
            if (mCurrentState == MEDIA_PLAYER_PLAYBACK_COMPLETE) {
                ALOGV("playback completed immediately following start()");
            }
        }
    } else {
        ALOGE("start called in state %d, mPlayer(%p)", mCurrentState, mPlayer.get());
        ret = INVALID_OPERATION;
    }

    mLockThreadId = 0;

    return ret;
}

status_t MediaPlayer::stop()
{
    ALOGV("stop");
    Mutex::Autolock _l(mLock);
    if (mCurrentState & MEDIA_PLAYER_STOPPED) return NO_ERROR;
    if ( (mPlayer != 0) && ( mCurrentState & ( MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PREPARED |
                    MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_PLAYBACK_COMPLETE ) ) ) {
        status_t ret = mPlayer->stop();
        if (ret != NO_ERROR) {
            mCurrentState = MEDIA_PLAYER_STATE_ERROR;
        } else {
            mCurrentState = MEDIA_PLAYER_STOPPED;
        }
        return ret;
    }
    ALOGE("stop called in state %d, mPlayer(%p)", mCurrentState, mPlayer.get());
    return INVALID_OPERATION;
}

status_t MediaPlayer::pause()
{
    ALOGV("pause");
    Mutex::Autolock _l(mLock);
    if (mCurrentState & (MEDIA_PLAYER_PAUSED|MEDIA_PLAYER_PLAYBACK_COMPLETE))
        return NO_ERROR;
    if ((mPlayer != 0) && (mCurrentState & MEDIA_PLAYER_STARTED)) {
        status_t ret = mPlayer->pause();
        if (ret != NO_ERROR) {
            mCurrentState = MEDIA_PLAYER_STATE_ERROR;
        } else {
            mCurrentState = MEDIA_PLAYER_PAUSED;
        }
        return ret;
    }
    ALOGE("pause called in state %d, mPlayer(%p)", mCurrentState, mPlayer.get());
    return INVALID_OPERATION;
}

bool MediaPlayer::isPlaying()
{
    Mutex::Autolock _l(mLock);
    if (mPlayer != 0) {
        bool temp = false;
        mPlayer->isPlaying(&temp);
        ALOGV("isPlaying: %d", temp);
        if ((mCurrentState & MEDIA_PLAYER_STARTED) && ! temp) {
            ALOGE("internal/external state mismatch corrected");
            mCurrentState = MEDIA_PLAYER_PAUSED;
        } else if ((mCurrentState & MEDIA_PLAYER_PAUSED) && temp) {
            ALOGE("internal/external state mismatch corrected");
            mCurrentState = MEDIA_PLAYER_STARTED;
        }
        return temp;
    }
    ALOGV("isPlaying: no active player");
    return false;
}

status_t MediaPlayer::setPlaybackSettings(const AudioPlaybackRate& rate)
{
    ALOGV("setPlaybackSettings: %f %f %d %d",
            rate.mSpeed, rate.mPitch, rate.mFallbackMode, rate.mStretchMode);
    // Negative speed and pitch does not make sense. Further validation will
    // be done by the respective mediaplayers.
    if (rate.mSpeed < 0.f || rate.mPitch < 0.f) {
        return BAD_VALUE;
    }
    Mutex::Autolock _l(mLock);
    if (mPlayer == 0 || (mCurrentState & MEDIA_PLAYER_STOPPED)) {
        return INVALID_OPERATION;
    }

    if (rate.mSpeed != 0.f && !(mCurrentState & MEDIA_PLAYER_STARTED)
            && (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_PAUSED
                    | MEDIA_PLAYER_PLAYBACK_COMPLETE))) {
        mPlayer->setLooping(mLoop);
        mPlayer->setVolume(mLeftVolume, mRightVolume);
        mPlayer->setAuxEffectSendLevel(mSendLevel);
    }

    status_t err = mPlayer->setPlaybackSettings(rate);
    if (err == OK) {
        if (rate.mSpeed == 0.f && mCurrentState == MEDIA_PLAYER_STARTED) {
            mCurrentState = MEDIA_PLAYER_PAUSED;
        } else if (rate.mSpeed != 0.f
                && (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_PAUSED
                    | MEDIA_PLAYER_PLAYBACK_COMPLETE))) {
            mCurrentState = MEDIA_PLAYER_STARTED;
        }
    }
    return err;
}

status_t MediaPlayer::getPlaybackSettings(AudioPlaybackRate* rate /* nonnull */)
{
    Mutex::Autolock _l(mLock);
    if (mPlayer == 0) return INVALID_OPERATION;
    return mPlayer->getPlaybackSettings(rate);
}

status_t MediaPlayer::setSyncSettings(const AVSyncSettings& sync, float videoFpsHint)
{
    ALOGV("setSyncSettings: %u %u %f %f",
            sync.mSource, sync.mAudioAdjustMode, sync.mTolerance, videoFpsHint);
    Mutex::Autolock _l(mLock);
    if (mPlayer == 0) return INVALID_OPERATION;
    return mPlayer->setSyncSettings(sync, videoFpsHint);
}

status_t MediaPlayer::getSyncSettings(
        AVSyncSettings* sync /* nonnull */, float* videoFps /* nonnull */)
{
    Mutex::Autolock _l(mLock);
    if (mPlayer == 0) return INVALID_OPERATION;
    return mPlayer->getSyncSettings(sync, videoFps);
}

status_t MediaPlayer::getVideoWidth(int *w)
{
    ALOGV("getVideoWidth");
    status_t ret = NO_ERROR;
    Mutex::Autolock _l(mLock);
    if (mPlayer == 0) return INVALID_OPERATION;
    if ((mVideoWidth  == 0) && (mVideoHeight == 0))
        ret = getVideoInfo(&mVideoWidth, &mVideoHeight);
    *w = mVideoWidth;
    return ret;
}

status_t MediaPlayer::getVideoHeight(int *h)
{
    ALOGV("getVideoHeight");
    status_t ret = NO_ERROR;
    Mutex::Autolock _l(mLock);
    if (mPlayer == 0) return INVALID_OPERATION;
    if ((mVideoWidth  == 0) && (mVideoHeight == 0))
        ret = getVideoInfo(&mVideoWidth, &mVideoHeight);
    *h = mVideoHeight;
    return ret;
}

status_t MediaPlayer::getCurrentPosition(int *msec)
{
    ALOGV("getCurrentPosition");
    Mutex::Autolock _l(mLock);
    if (mPlayer != 0) {
        if (mCurrentPosition >= 0) {
            ALOGV("Using cached seek position: %d", mCurrentPosition);
            *msec = mCurrentPosition;
            return NO_ERROR;
        }
        return mPlayer->getCurrentPosition(msec);
    }
    return INVALID_OPERATION;
}

status_t MediaPlayer::getDuration_l(int *msec)
{
    ALOGV("getDuration_l");
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED |
            MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        int durationMs;
        status_t ret = mPlayer->getDuration(&durationMs);

        if (ret != OK) {
            // Do not enter error state just because no duration was available.
            durationMs = -1;
            ret = OK;
        }

        if (msec) {
            *msec = durationMs;
        }
        return ret;
    }
    ALOGE("Attempt to call getDuration in wrong state: mPlayer=%p, mCurrentState=%u",
            mPlayer.get(), mCurrentState);
    return INVALID_OPERATION;
}

status_t MediaPlayer::getDuration(int *msec)
{
    Mutex::Autolock _l(mLock);
    return getDuration_l(msec);
}

status_t MediaPlayer::seekTo_l(int msec)
{
    ALOGV("seekTo %d", msec);
    if ((mPlayer != 0) && ( mCurrentState & ( MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PREPARED |
            MEDIA_PLAYER_PAUSED |  MEDIA_PLAYER_PLAYBACK_COMPLETE) ) ) {
        if ( msec < 0 ) {
            ALOGW("Attempt to seek to invalid position: %d", msec);
            msec = 0;
        }

        int durationMs;
        status_t err = mPlayer->getDuration(&durationMs);

        if (err != OK) {
            ALOGW("Stream has no duration and is therefore not seekable.");
            return err;
        }

        if (msec > durationMs) {
            ALOGW("Attempt to seek to past end of file: request = %d, "
                  "durationMs = %d",
                  msec,
                  durationMs);

            msec = durationMs;
        }

        // cache duration
        mCurrentPosition = msec;
        if (mSeekPosition < 0) {
            mSeekPosition = msec;
            return mPlayer->seekTo(msec);
        }
        else {
            ALOGV("Seek in progress - queue up seekTo[%d]", msec);
            return NO_ERROR;
        }
    }
    ALOGE("Attempt to perform seekTo in wrong state: mPlayer=%p, mCurrentState=%u", mPlayer.get(),
            mCurrentState);
    return INVALID_OPERATION;
}

status_t MediaPlayer::seekTo(int msec)
{
    mLockThreadId = getThreadId();
    Mutex::Autolock _l(mLock);
    status_t result = seekTo_l(msec);
    mLockThreadId = 0;

    return result;
}

status_t MediaPlayer::reset_l()
{
    mLoop = false;
    if (mCurrentState == MEDIA_PLAYER_IDLE) return NO_ERROR;
    mPrepareSync = false;
    if (mPlayer != 0) {
        status_t ret = mPlayer->reset();
        if (ret != NO_ERROR) {
            ALOGE("reset() failed with return code (%d)", ret);
            mCurrentState = MEDIA_PLAYER_STATE_ERROR;
        } else {
            mPlayer->disconnect();
            mCurrentState = MEDIA_PLAYER_IDLE;
        }
        // setDataSource has to be called again to create a
        // new mediaplayer.
        mPlayer = 0;
        return ret;
    }
    clear_l();
    return NO_ERROR;
}

status_t MediaPlayer::doSetRetransmitEndpoint(const sp<IMediaPlayer>& player) {
    Mutex::Autolock _l(mLock);

    if (player == NULL) {
        return UNKNOWN_ERROR;
    }

    if (mRetransmitEndpointValid) {
        return player->setRetransmitEndpoint(&mRetransmitEndpoint);
    }

    return OK;
}

status_t MediaPlayer::reset()
{
    ALOGV("reset");
#ifdef USE_RT_MEDIA_PLAYER
    Mutex::Autolock _R(mRtkLock);
    mForceUseRTMediaPlayer = FORCE_NONE;
#endif // USE_RT_MEDIA_PLAYER
    Mutex::Autolock _l(mLock);
    return reset_l();
}

status_t MediaPlayer::setAudioStreamType(audio_stream_type_t type)
{
    ALOGV("MediaPlayer::setAudioStreamType");
    Mutex::Autolock _l(mLock);
    if (mStreamType == type) return NO_ERROR;
    if (mCurrentState & ( MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED |
                MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_PLAYBACK_COMPLETE ) ) {
        // Can't change the stream type after prepare
        ALOGE("setAudioStream called in state %d", mCurrentState);
        return INVALID_OPERATION;
    }
    // cache
    mStreamType = type;
    return OK;
}

status_t MediaPlayer::getAudioStreamType(audio_stream_type_t *type)
{
    ALOGV("getAudioStreamType");
    Mutex::Autolock _l(mLock);
    *type = mStreamType;
    return OK;
}

status_t MediaPlayer::setLooping(int loop)
{
    ALOGV("MediaPlayer::setLooping");
    Mutex::Autolock _l(mLock);
    mLoop = (loop != 0);
    if (mPlayer != 0) {
        return mPlayer->setLooping(loop);
    }
    return OK;
}

bool MediaPlayer::isLooping() {
    ALOGV("isLooping");
    Mutex::Autolock _l(mLock);
    if (mPlayer != 0) {
        return mLoop;
    }
    ALOGV("isLooping: no active player");
    return false;
}

status_t MediaPlayer::setVolume(float leftVolume, float rightVolume)
{
    ALOGV("MediaPlayer::setVolume(%f, %f)", leftVolume, rightVolume);
    Mutex::Autolock _l(mLock);
    mLeftVolume = leftVolume;
    mRightVolume = rightVolume;
    if (mPlayer != 0) {
        return mPlayer->setVolume(leftVolume, rightVolume);
    }
    return OK;
}

status_t MediaPlayer::setAudioSessionId(audio_session_t sessionId)
{
    ALOGV("MediaPlayer::setAudioSessionId(%d)", sessionId);
    Mutex::Autolock _l(mLock);
    if (!(mCurrentState & MEDIA_PLAYER_IDLE)) {
        ALOGE("setAudioSessionId called in state %d", mCurrentState);
        return INVALID_OPERATION;
    }
    if (sessionId < 0) {
        return BAD_VALUE;
    }
    if (sessionId != mAudioSessionId) {
        AudioSystem::acquireAudioSessionId(sessionId, -1);
        AudioSystem::releaseAudioSessionId(mAudioSessionId, -1);
        mAudioSessionId = sessionId;
    }
    return NO_ERROR;
}

audio_session_t MediaPlayer::getAudioSessionId()
{
    Mutex::Autolock _l(mLock);
    return mAudioSessionId;
}

status_t MediaPlayer::setAuxEffectSendLevel(float level)
{
    ALOGV("MediaPlayer::setAuxEffectSendLevel(%f)", level);
    Mutex::Autolock _l(mLock);
    mSendLevel = level;
    if (mPlayer != 0) {
        return mPlayer->setAuxEffectSendLevel(level);
    }
    return OK;
}

status_t MediaPlayer::attachAuxEffect(int effectId)
{
    ALOGV("MediaPlayer::attachAuxEffect(%d)", effectId);
    Mutex::Autolock _l(mLock);
    if (mPlayer == 0 ||
        (mCurrentState & MEDIA_PLAYER_IDLE) ||
        (mCurrentState == MEDIA_PLAYER_STATE_ERROR )) {
        ALOGE("attachAuxEffect called in state %d, mPlayer(%p)", mCurrentState, mPlayer.get());
        return INVALID_OPERATION;
    }

    return mPlayer->attachAuxEffect(effectId);
}

// always call with lock held
status_t MediaPlayer::checkStateForKeySet_l(int key)
{
    switch(key) {
    case KEY_PARAMETER_AUDIO_ATTRIBUTES:
        if (mCurrentState & ( MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED |
                MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_PLAYBACK_COMPLETE) ) {
            // Can't change the audio attributes after prepare
            ALOGE("trying to set audio attributes called in state %d", mCurrentState);
            return INVALID_OPERATION;
        }
        break;
    default:
        // parameter doesn't require player state check
        break;
    }
    return OK;
}

status_t MediaPlayer::setParameter(int key, const Parcel& request)
{
    ALOGV("MediaPlayer::setParameter(%d)", key);
    status_t status = INVALID_OPERATION;
    Mutex::Autolock _l(mLock);
    if (checkStateForKeySet_l(key) != OK) {
        return status;
    }
    switch (key) {
    case KEY_PARAMETER_AUDIO_ATTRIBUTES:
        // save the marshalled audio attributes
        if (mAudioAttributesParcel != NULL) { delete mAudioAttributesParcel; };
        mAudioAttributesParcel = new Parcel();
        mAudioAttributesParcel->appendFrom(&request, 0, request.dataSize());
        status = OK;
        break;
    default:
        ALOGV_IF(mPlayer == NULL, "setParameter: no active player");
        break;
    }

    if (mPlayer != NULL) {
        status = mPlayer->setParameter(key, request);
    }
    return status;
}

status_t MediaPlayer::getParameter(int key, Parcel *reply)
{
    ALOGV("MediaPlayer::getParameter(%d)", key);
    Mutex::Autolock _l(mLock);
    if (mPlayer != NULL) {
        return  mPlayer->getParameter(key, reply);
    }
    ALOGV("getParameter: no active player");
    return INVALID_OPERATION;
}

status_t MediaPlayer::setRetransmitEndpoint(const char* addrString,
                                            uint16_t port) {
    ALOGV("MediaPlayer::setRetransmitEndpoint(%s:%hu)",
            addrString ? addrString : "(null)", port);

    Mutex::Autolock _l(mLock);
    if ((mPlayer != NULL) || (mCurrentState != MEDIA_PLAYER_IDLE))
        return INVALID_OPERATION;

    if (NULL == addrString) {
        mRetransmitEndpointValid = false;
        return OK;
    }

    struct in_addr saddr;
    if(!inet_aton(addrString, &saddr)) {
        return BAD_VALUE;
    }

    memset(&mRetransmitEndpoint, 0, sizeof(mRetransmitEndpoint));
    mRetransmitEndpoint.sin_family = AF_INET;
    mRetransmitEndpoint.sin_addr   = saddr;
    mRetransmitEndpoint.sin_port   = htons(port);
    mRetransmitEndpointValid       = true;

    return OK;
}

void MediaPlayer::notify(int msg, int ext1, int ext2, const Parcel *obj)
{
    ALOGV("message received msg=%d, ext1=%d, ext2=%d", msg, ext1, ext2);
    bool send = true;
    bool locked = false;

    // TODO: In the future, we might be on the same thread if the app is
    // running in the same process as the media server. In that case,
    // this will deadlock.
    //
    // The threadId hack below works around this for the care of prepare,
    // seekTo and start within the same process.
    // FIXME: Remember, this is a hack, it's not even a hack that is applied
    // consistently for all use-cases, this needs to be revisited.
    if (mLockThreadId != getThreadId()) {
        mLock.lock();
        locked = true;
    }

    // Allows calls from JNI in idle state to notify errors
    if (!(msg == MEDIA_ERROR && mCurrentState == MEDIA_PLAYER_IDLE) && mPlayer == 0) {
        ALOGV("notify(%d, %d, %d) callback on disconnected mediaplayer", msg, ext1, ext2);
        if (locked) mLock.unlock();   // release the lock when done.
        return;
    }

    switch (msg) {
    case MEDIA_NOP: // interface test message
        break;
    case MEDIA_PREPARED:
        ALOGV("prepared");
        mCurrentState = MEDIA_PLAYER_PREPARED;
        if (mPrepareSync) {
            ALOGV("signal application thread");
            mPrepareSync = false;
            mPrepareStatus = NO_ERROR;
            mSignal.signal();
        }
        break;
    case MEDIA_PLAYBACK_COMPLETE:
        ALOGV("playback complete");
        if (mCurrentState == MEDIA_PLAYER_IDLE) {
            ALOGE("playback complete in idle state");
        }
        if (!mLoop) {
            mCurrentState = MEDIA_PLAYER_PLAYBACK_COMPLETE;
        }
        break;
    case MEDIA_ERROR:
        // Always log errors.
        // ext1: Media framework error code.
        // ext2: Implementation dependant error code.
        ALOGE("error (%d, %d)", ext1, ext2);
        mCurrentState = MEDIA_PLAYER_STATE_ERROR;
        if (mPrepareSync)
        {
            ALOGV("signal application thread");
            mPrepareSync = false;
            mPrepareStatus = ext1;
            mSignal.signal();
            send = false;
        }
        break;
    case MEDIA_INFO:
        // ext1: Media framework error code.
        // ext2: Implementation dependant error code.
        if (ext1 != MEDIA_INFO_VIDEO_TRACK_LAGGING) {
            ALOGW("info/warning (%d, %d)", ext1, ext2);
        }
        break;
    case MEDIA_SEEK_COMPLETE:
        ALOGV("Received seek complete");
        if (mSeekPosition != mCurrentPosition) {
            ALOGV("Executing queued seekTo(%d)", mSeekPosition);
            mSeekPosition = -1;
            seekTo_l(mCurrentPosition);
        }
        else {
            ALOGV("All seeks complete - return to regularly scheduled program");
            mCurrentPosition = mSeekPosition = -1;
        }
        break;
    case MEDIA_BUFFERING_UPDATE:
        ALOGV("buffering %d", ext1);
        break;
    case MEDIA_SET_VIDEO_SIZE:
        ALOGV("New video size %d x %d", ext1, ext2);
        mVideoWidth = ext1;
        mVideoHeight = ext2;
        break;
    case MEDIA_TIMED_TEXT:
        ALOGV("Received timed text message");
        break;
    case MEDIA_SUBTITLE_DATA:
        ALOGV("Received subtitle data message");
        break;
    case MEDIA_META_DATA:
        ALOGV("Received timed metadata message");
        break;
    case MEDIA_SPEED_CHANGE:
        ALOGV("Received speed change %d", ext1);
        break;
    default:
        ALOGV("unrecognized message: (%d, %d, %d)", msg, ext1, ext2);
        break;
    }

    sp<MediaPlayerListener> listener = mListener;
    if (locked) mLock.unlock();

    // this prevents re-entrant calls into client code
    if ((listener != 0) && send) {
        Mutex::Autolock _l(mNotifyLock);
        ALOGV("callback application");
        listener->notify(msg, ext1, ext2, obj);
        ALOGV("back from callback");
    }
}

void MediaPlayer::died()
{
    ALOGV("died");
    notify(MEDIA_ERROR, MEDIA_ERROR_SERVER_DIED, 0);
}

status_t MediaPlayer::setNextMediaPlayer(const sp<MediaPlayer>& next) {
    Mutex::Autolock _l(mLock);
    if (mPlayer == NULL) {
        return NO_INIT;
    }

    if (next != NULL && !(next->mCurrentState &
            (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_PLAYBACK_COMPLETE))) {
        ALOGE("next player is not prepared");
        return INVALID_OPERATION;
    }

    return mPlayer->setNextPlayer(next == NULL ? NULL : next->mPlayer);
}

#ifdef USE_RT_MEDIA_PLAYER
status_t MediaPlayer::useRTMediaPlayer(int forceType)
{
    ALOGI("[%s,%s,%d] useRTMediaPlayer: %d",__FILE__,__func__,__LINE__ , forceType);
    mForceUseRTMediaPlayer = forceType;
    return OK;
}

status_t    MediaPlayer::fastforward(int speed)
{
    ALOGI("[%s,%s,%d] speed = %d\n",__FILE__,__func__,__LINE__,speed);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        status_t ret = NO_ERROR;
        ret = mPlayer->fastforward(speed);
        return ret;
    }
    ALOGE("Attempt to call fastforward without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t    MediaPlayer::fastrewind(int speed)
{
    ALOGI("[%s,%s,%d] speed = %d\n",__FILE__,__func__,__LINE__,speed);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        status_t ret = NO_ERROR;
        ret = mPlayer->fastrewind(speed);
        return ret;
    }
    ALOGE("Attempt to call fastrewind without a valid mediaplayer");
    return INVALID_OPERATION;
}

int *MediaPlayer::getSubtitleInfo(int streamNum)
{
    ALOGI("[%s,%s,%d] streamNum = %d\n",__FILE__,__func__,__LINE__,streamNum);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        int *tmp=mPlayer->getSubtitleInfo(streamNum);
        ALOGI("[%s,%s,%d] m_subtitleInfo 0x%x = %d,%d,%d,%d,%d\n",__FILE__,__func__,__LINE__,(int)*tmp,tmp[0],tmp[1],tmp[2],tmp[3],tmp[4]);
        return tmp;
    }
    ALOGE("Attempt to call getSubtitleInfo without a valid mediaplayer");
    return NULL;
}

status_t MediaPlayer::getVideoInfo(int *width, int *height)
{
    status_t ret = INVALID_OPERATION;
    if (mPlayer != 0)
        ret = mPlayer->getVideoInfo(width, height);
    else
        ALOGE("No nPlayer in %s:%s:%d", __FILE__, __func__, __LINE__);
    ALOGV("[%s,%s,%d] width = %d, height = %d", __FILE__, __func__, __LINE__, *width, *height);
    return ret;
}

status_t        MediaPlayer::setSubtitleInfo(int streamNum,int enable,int textEncoding,int textColor,int fontSize, int syncTime, int offset)
{
    ALOGI("[%s,%s,%d] streamNum= %d,enable= %d,textEncoding= %d,textColor= %d,fontSize= %d\n",__FILE__,__func__,__LINE__,streamNum,enable,textEncoding,textColor,fontSize);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        status_t ret = NO_ERROR;
        if (streamNum >= 1)
            ret = mPlayer->setSubtitleInfo(streamNum,enable,textEncoding,textColor,fontSize,syncTime,offset);
        return ret;
    }
    ALOGE("Attempt to call setSubtitleInfo without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t    MediaPlayer::setAutoScanExtSubtitle(int autoScan)
{
    ALOGI("[%s,%s,%d] autoScan = %d\n",__FILE__,__func__,__LINE__,autoScan);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_INITIALIZED));
    if (mPlayer != 0 && isValidState) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setAutoScanExtSubtitle(autoScan);
        return ret;
    }
    ALOGE("Attempt to call setAutoScanExtSubtitle without a valid mediaplayer or in a invalid state %d : only can call at Initialized state",mCurrentState);
    return INVALID_OPERATION;
}

status_t    MediaPlayer::setExtSubtitlePath(const char *path)
{
    ALOGI("[%s,%s,%d] path = %s\n",__FILE__,__func__,__LINE__,path);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setExtSubtitlePath(path);
        return ret;
    }
    ALOGE("Attempt to call setExtSubtitlePath without a valid mediaplayer or in a invalid state %d ",mCurrentState);
    return INVALID_OPERATION;
}

int           *MediaPlayer::getAudioTrackInfo(int streamNum)
{
    ALOGI("[%s,%s,%d] streamNum = %d \n",__FILE__,__func__,__LINE__,streamNum);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        int *tmp=mPlayer->getAudioTrackInfo(streamNum);
        ALOGI("[%s,%s,%d] m_AudioTrackInfo 0x%x = %d,%d,%d,%d,%d,%d,%d,%d\n",__FILE__,__func__,__LINE__,(int)*tmp,tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7]);
        return tmp;
    }
    ALOGE("Attempt to call getAudioTrackInfo without a valid mediaplayer");
    return NULL;
}

status_t        MediaPlayer::setAudioTrackInfo(int streamNum)
{
    ALOGI("[%s,%s,%d] streamNum = %d \n",__FILE__,__func__,__LINE__,streamNum);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        status_t ret = NO_ERROR;
        if (streamNum >= 1)
            ret = mPlayer->setAudioTrackInfo(streamNum);
        return ret;
    }
    ALOGE("Attempt to call setAudioTrackInfo without a valid mediaplayer");
    return INVALID_OPERATION;
}
#define DEV_MEM "/dev/mem"

status_t    MediaPlayer::execSetGetNavProperty(int propertyID,unsigned char *ucpInput,int inDataSize,unsigned char **outData,int *outDataSize)
{
    ALOGI("[%s,%s,%d] propertyID = %d \n",__FILE__,__func__,__LINE__,propertyID);
    ALOGI("[%s,%d] ucpInput = %x\n",__FILE__,__LINE__,*(uint32_t*)ucpInput);
    for(int i=0;i<inDataSize;i++)
        ALOGI("[%s,%d] ucpInput[%d] = %d\n",__FILE__,__LINE__,i,(int)ucpInput[i]);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        status_t ret = UNKNOWN_ERROR;
        if (propertyID >= 0)
        {
            unsigned long phyAddr=0;
            int outDataOffset=0;
            if(m_psSetGetNavVirAddr ==0)
            {
                long requiredSize = SETGETNAV_SHEMEM_SIZE;
                ret = mPlayer->execSetGetNavProperty(-1,inDataSize,outDataOffset,&phyAddr);
                if(ret==NO_ERROR && phyAddr!=0)
                { // get m_psSetGetNavVirAddr
                    m_psSetGetNavAddr=phyAddr;
                    //memory is ready, register it to playback server
                    ALOGI("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
                    if(m_psSetGetNavAddr > 0)
                    {
                        ALOGI("[%s,%s,%d],phys=0x%lx, len=0x%08x\n",__FILE__,__func__,__LINE__,m_psSetGetNavAddr,(int)requiredSize);
                        //get virtual address at the first using
                        int fd = open(DEV_MEM, O_RDWR|O_SYNC ); //map to non-cache address
                        if (fd < 0)
                        {
                            ALOGI( "System/DevMem: Opening '%s' failed!\n", DEV_MEM );
                        }
                        else
                        {
                            void* ppMap;
                            ppMap = mmap( (void*)NULL/*determined by kernel*/, (unsigned int)requiredSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, m_psSetGetNavAddr );
                            close(fd);
                            if (ppMap == MAP_FAILED)
                            {
                                ALOGI( "Mapping %d bytes at 0x%08lx via '%s' failed!, errno = %d\n", (int) requiredSize,m_psSetGetNavAddr, DEV_MEM , errno);
                            }
                            else
                            {
                                m_psSetGetNavVirAddr = (unsigned char*) ppMap;
                                ALOGI("[%s,%s,%d] m_psSetGetNavAddr = 0x%x,m_psSetGetNavVirAddr = 0x%p\n",__FILE__,__func__,__LINE__,(int)m_psSetGetNavAddr,m_psSetGetNavVirAddr);
                                //reset memory to clear elapsed time left at preceding playback
                                memset(m_psSetGetNavVirAddr, 0, requiredSize);
                            }
                        }
                    }
                }
            }
            if(m_psSetGetNavVirAddr)
            {
                // do put input data 
                ALOGI("[%s,%s,%d] m_psSetGetNavAddr = 0x%x,m_psSetGetNavVirAddr = 0x%p\n",__FILE__,__func__,__LINE__,(int)m_psSetGetNavAddr,m_psSetGetNavVirAddr);
                ALOGI("[%s,%d] ucpInput = %x\n",__FILE__,__LINE__,*(uint32_t *)ucpInput);
                for(int i=0;i<inDataSize;i++)
                {
                    *((unsigned char *)(m_psSetGetNavVirAddr+i))=ucpInput[i];
                    ALOGI("[%s,%d] ucpInput[%d] = %d , m_psSetGetNavVirAddr[%d] = %d \n",__FILE__,__LINE__,i,(int)ucpInput[i],i,(int)m_psSetGetNavVirAddr[i]);
                }

                // do set get property ..
                ret = mPlayer->execSetGetNavProperty(propertyID,inDataSize,outDataOffset,&phyAddr);

                // get outData 
                *outDataSize=*((int *)(m_psSetGetNavVirAddr+inDataSize));
                *outData=m_psSetGetNavVirAddr+inDataSize+4;
                ALOGI("[%s,%s,%d] outDataSize = %d \n",__FILE__,__func__,__LINE__,*outDataSize);
                for(int i=0;i<*outDataSize;i++)
                {
                    ALOGV("[%s,%d] outData[%d] = %p ,outData[%d] = %d\n",__FILE__,__LINE__,i,outData[i],i,(int)(*outData)[i]);
                }
            }
        }
        return ret;
    }
    ALOGE("Attempt to call execSetGetNavProperty without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::deliverNaviControlCmd(int action, int status)
{
    ALOGI("[%s,%s,%d] action = %d, status = %d\n",__FILE__,__func__,__LINE__, action, status);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        status_t ret = NO_ERROR;
        ret = mPlayer->deliverNaviControlCmd(action, status);
        return ret;
    }
    ALOGE("Attempt to call deliverNaviControlCmd without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::getPlayerCurrentState(int *pstate)
{
    ALOGV("getPlayerCurrentState : %d",mCurrentState);
    if (mPlayer != 0)
    {
        *pstate=mCurrentState;
        return NO_ERROR;
    }
    ALOGE("Attempt to call getPlayerCurrentState without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::setStartTime(int msec)
{
    ALOGV("setStartTime: %d", msec);
    bool isValidState = (mCurrentState & (MEDIA_PLAYER_PREPARED | MEDIA_PLAYER_STARTED | MEDIA_PLAYER_PAUSED | MEDIA_PLAYER_STOPPED | MEDIA_PLAYER_PLAYBACK_COMPLETE));
    if (mPlayer != 0 && isValidState) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setStartTime(msec);
        return ret;
    }
    ALOGE("Attempt to call setStartTime without a valid mediaplayer");
    return INVALID_OPERATION;
}

 // for transcode
status_t MediaPlayer::setTranscodeMode(bool bEnable)
{
    ALOGV("setTranscodeMode: %d", bEnable);
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setTranscodeMode(bEnable);
        return ret;
    }
    ALOGE("Attempt to call setTranscodeMode without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::startTranscode()
{
    ALOGV("startTranscode");
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->startTranscode();
        return ret;
    }
    ALOGE("Attempt to call startTranscode without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::pauseTranscode()
{
    ALOGV("pauseTranscode");
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->pauseTranscode();
        return ret;
    }
    ALOGE("Attempt to call pauseTranscode without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::stopTranscode()
{
    ALOGV("stopTranscode");
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->stopTranscode();
        return ret;
    }
    ALOGE("Attempt to call stopTranscode without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::setOutputFormat(int outputType,
                                int outputFormat,
                                int fileDesc,
                                const char *fileName,
                                sp<IMemory> &ashmemData,
                                sp<IMemory> &ashmemInfo,
                                const char *udpIp,
                                int udpPort,
                                bool rtpHeader)
{
    ALOGV("setOutputFormat");
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setOutputFormat(outputType,
                            outputFormat,
                            fileDesc,
                            fileName,
                            ashmemData,
                            ashmemInfo,
                            udpIp,
                            udpPort,
                            rtpHeader);
        return ret;
    }
    ALOGE("Attempt to call setOutputFormat without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::configureTargetFormat(// video
                                            int width,
                                            int height,
                                            int fps,
                                            int v_bitrate,
                                            int iframeInterval,
                                            int rateControl,
                                            int aspectRatio,
                                            int interlace,
                                            int shareWBbuf,
                                            // audio
                                            int channelCount,
                                            int channelMode,
                                            int sampleRate,
                                            int a_bitrate)
{
    ALOGV("configureTargetFormat");
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->configureTargetFormat(width,
                                            height,
                                            fps,
                                            v_bitrate,
                                            iframeInterval,
                                            rateControl,
                                            aspectRatio,
                                            interlace,
                                            shareWBbuf,
                                            channelCount,
                                            channelMode,
                                            sampleRate,
                                            a_bitrate);
        return ret;
    }
    ALOGE("Attempt to call configureTargetFormat without a valid mediaplayer");
    return INVALID_OPERATION;
}
//status_t        registerStatusHandler(sp<AHandler> handlerId);

 // for RVSD
status_t MediaPlayer::setTranscodeVOWriteback(const int32_t flag)
{
    ALOGV("setTranscodeVOWriteback %d",flag);
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setTranscodeVOWriteback(flag);
        return ret;
    }
    ALOGE("Attempt to call setTranscodeVOWriteback without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::setTranscodeOSDStatus(const int32_t flag)
{
    ALOGV("setTranscodeOSDStatus %d",flag);
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setTranscodeOSDStatus(flag);
        return ret;
    }
    ALOGE("Attempt to call setTranscodeOSDStatus without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::setLayerOrderValue(const uint32_t value)
{
    ALOGV("setLayerOrderValue %d",value);
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setLayerOrderValue(value);
        return ret;
    }
    ALOGE("Attempt to call setLayerOrderValue without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::setPlaySpeedValue(const int32_t nSpeed)
{
    ALOGV("setPlaySpeedValue %d",nSpeed);
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->setPlaySpeedValue(nSpeed);
        return ret;
    }
    ALOGE("Attempt to call setPlaySpeedValue without a valid mediaplayer");
    return INVALID_OPERATION;
}

status_t MediaPlayer::initCheck()
{
    ALOGV("initCheck");
    if(mPlayer != 0) {
        status_t ret = NO_ERROR;
        ret = mPlayer->initCheck();
        return ret;
    }
    ALOGE("Attempt to call initCheck without a valid mediaplayer");
    return INVALID_OPERATION;
}

void MediaPlayer::SetContentMode(/*RTK_PLAYER_CONTENTMODE_e*/ int contentMode)
{
    ALOGV("SetContentMode %d",contentMode);
    if(mPlayer != 0) {
        mPlayer->SetContentMode(contentMode);
        return;
    }
    ALOGE("Attempt to call SetContentMode without a valid mediaplayer");
    return;
}

int MediaPlayer::checkLoadMedia()
{
    ALOGV("checkLoadMedia");
    if(mPlayer != 0) {
        int ret = NO_ERROR;
        ret = mPlayer->checkLoadMedia();
        return ret;
    }
    ALOGE("Attempt to call checkLoadMedia without a valid mediaplayer");
    return INVALID_OPERATION;
}

int MediaPlayer::getState()
{
    ALOGV("getState");
    if(mPlayer != 0) {
        int ret = NO_ERROR;
        ret = mPlayer->getState();
        return ret;
    }
    ALOGE("Attempt to call checkLoadMedia without a valid mediaplayer");
    return INVALID_OPERATION;
}

int MediaPlayer::SetVideoWindow(int x, int y, int width, int height)
{
    ALOGV("SetVideoWindow %d %d %d %d",x,y,width,height);
    if(mPlayer != 0) {
        int ret = NO_ERROR;
        ret = mPlayer->SetVideoWindow(x,y,width,height);
        return ret;
    }
    ALOGE("Attempt to call SetVideoWindow without a valid mediaplayer");
    return INVALID_OPERATION;
}

int MediaPlayer::VideoShow()
{
    ALOGV("VideoShow");
    if(mPlayer != 0) {
        int ret = NO_ERROR;
        ret = mPlayer->VideoShow();
        return ret;
    }
    ALOGE("Attempt to call VideoShow without a valid mediaplayer");
    return INVALID_OPERATION;
}

int MediaPlayer::VideoHide()
{
    ALOGV("VideoHide");
    if(mPlayer != 0) {
        int ret = NO_ERROR;
        ret = mPlayer->VideoHide();
        return ret;
    }
    ALOGE("Attempt to call VideoHide without a valid mediaplayer");
    return INVALID_OPERATION;
}

void MediaPlayer::setVideoDisapper(bool disapper)
{
    ALOGV("setVideoDisapper %d",disapper);
    if(mPlayer != 0) {
        mPlayer->setVideoDisapper(disapper);
        return;
    }
    ALOGE("Attempt to call setVideoDisapper without a valid mediaplayer");
    return;
}

#endif // USE_RT_MEDIA_PLAYER

} // namespace android

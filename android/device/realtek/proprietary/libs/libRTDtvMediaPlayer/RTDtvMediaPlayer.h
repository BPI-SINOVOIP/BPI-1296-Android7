/*
**
** Copyright 2009, The Android Open Source Project
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

#ifndef REALTEK_DTV_MEDIAPLAYER_H
#define REALTEK_DTV_MEDIAPLAYER_H

#include <semaphore.h>
#include <pthread.h>
#include <media/MediaPlayerInterface.h>

#include <RT_IPC/RT_IPC.h>

#include <transcode/RtkTranscodeCommon.h>

class RVSD;

namespace android {

class RtkTranscodePlayer;

enum {
    ERROR_DTV_TRANSCODE_NOT_ENABLED     = MEDIA_ERROR_BASE - 2000,
    ERROR_DTV_TRANSCODE_NOT_CONFIGURED  = MEDIA_ERROR_BASE - 2001,
    ERROR_DTV_TRANSCODE_WRONG_STATE     = MEDIA_ERROR_BASE - 2002,
};

/**
 * This is just a fake media player. Don't support any
 * playback functions. It is used to let MediaPlayer
 * stay on correct state as expected such that VideoView
 * and other view for media player can work. 
 *
 * As the name RTDtvMediaPlayer, we create it for DTV player.
 * DTV player use TvManagerService to control real DTV playback
 * flow, not using this class.
 *
 * TODO: In the future, we may extend the class to support full
 * DTV playback control. In other word, the flow controlled in 
 * TvManagerService can be implemented in here. Not sure.
 *
 */
class RTDtvMediaPlayer : public MediaPlayerHWInterface {
public:
    RTDtvMediaPlayer();
    virtual ~RTDtvMediaPlayer();

    virtual status_t initCheck();

    virtual status_t setUID(uid_t uid);

    using MediaPlayerBase::setDataSource; //avoid "xxx hides overloaded virtual function"
    virtual status_t setDataSource(const sp<IMediaHTTPService> &httpService, const char *url, const KeyedVector<String8, String8> * headers = NULL);
    virtual status_t setDataSource(const char *url, const KeyedVector<String8, String8> *headers = NULL);
    virtual status_t setDataSource(int fd, int64_t offset, int64_t length);

#if defined(Android_JB)
    virtual status_t setVideoSurfaceTexture(
            const sp<ISurfaceTexture> &surfaceTexture);
#elif defined(Android_KitKate)
    virtual status_t setVideoSurfaceTexture(
            const sp<IGraphicBufferProducer>& bufferProducer);
#elif defined(__LINARO_SDK__)
	virtual status_t    setVideoSurfaceTexture(
			const sp<IGraphicBufferProducer> &bufferProducer);
#else
    virtual status_t    setVideoSurfaceTexture(
        const sp<IGraphicBufferProducer> &bufferProducer);
#endif

    virtual status_t prepare();
    virtual status_t prepareAsync();
    virtual status_t start();
    virtual status_t stop();
    virtual status_t pause();
    virtual bool isPlaying();
    virtual status_t seekTo(int msec);
    virtual status_t getCurrentPosition(int *msec);
    virtual status_t getDuration(int *msec);
    virtual status_t reset();
    virtual status_t setLooping(int loop);
    virtual player_type playerType();
    virtual status_t invoke(const Parcel &request, Parcel *reply);
    virtual status_t setParameter(int key, const Parcel &request);
    virtual status_t getParameter(int key, Parcel *reply);
    virtual status_t setVolume(float leftVolume, float rightVolume);
    virtual status_t setAudioStreamType(audio_stream_type_t streamType);

    void xProcEntry();
    static void* procEntry(void* pInstance);

    // APIs for transcode
    /**
     * Initialize transcode manager if bEnable is true; otherwise
     * stop transcoding and release transcode manger.
     *
     * When it is enabled, the default state of transcoding is pause.
     */
    virtual status_t    setTranscodeMode(bool bEnable);
    virtual status_t	setAudioDecoderAgentID(long id);

    // APIs for LayerOrder
    /**
     * Set LayerOrder value
     */
    virtual status_t setLayerOrderValue(const uint32_t value);

    // APIs for g_play_speed
    /**
     * Set g_play_speed value
     */
    virtual status_t setPlaySpeedValue(const int32_t nSpeed);

    /**
     * Start trancoding.
     * Before calling it, you need to complete the configuration of output protocol
     * and target a/v format.
     */
    virtual status_t    startTranscode();

    /**
     * Pause transcoding.
     */
    virtual status_t    pauseTranscode();

    /**
     * Stop transcoding. It would stop transcode maanger and release it.
     * If you want to restart it again, you need to 
     *   call setTranscodeMode to initialize it then call startTranscode to
     *   start it.
     */
    virtual status_t    stopTranscode();

    /**
     * Set the output protocol of TS stream.
     * It must be called before calling startTranscode; otherwise it
     * would return ERROR_TRANSCODE_WRONG_STATE.
     */
    virtual status_t    setOutputFormat(RtkTranscodeConf &conf);

    /**
     * new added API for MediaPlayer interface
     */
    virtual status_t    setOutputFormat(int outputType,
                                int outputFormat,
                                int fileDesc,
                                const char *fileName,
                                sp<IMemory> &ashmemData,
                                sp<IMemory> &ashmemInfo,
                                const char *udpIp,
                                int udpPort,
                                bool rtpHeader);

    /**
     * Configure the A/V format of target media (output media stream).
     * It must be called before calling startTranscode; othewise it
     * would return ERROR_TRANSCODE_WRONG_STATE.
     */
    virtual status_t    configureTargetFormat(AV_FORMAT_CONFIG &targetFmt);

    /**
     * new added API for MediaPlayer interface
     */
    virtual status_t    configureTargetFormat(  // video
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
                                        int a_bitrate);

    /**
     * register your ALooper hanlder.
     * It should be called before calling startTranscode; otherwise it
     * would return ERROR_TRANSCODE_WRONG_STATE.
     */
    virtual status_t    registerStatusHandler(sp<AHandler> &handlerId);

    /**
     * Set focus
     * @params - 
     * focus[in] : 0 means UnFocus and 1 mean Focus. Focus would
     *             allow audio to be connected to AO; otherwise audio
     *             would not be connected to AO (in other words, it would
     *             be muted). If there is subtitle displayed, it would have
     *             the same effect as audio.
     * @return - NO_ERROR
     */
    status_t            setFocus(int focus);

private:
    uid_t mUID;
    bool mbSetDataSource;
    uint32_t mFlags;
    sem_t m_semToWaitResponse;
    pthread_t m_thread;
    pthread_mutex_t m_cmdLock;
    volatile bool m_bAskToStopThread;
    bool m_connectClosed;
    RT_IPC m_ipc;
    RVSD *mRvsd;

    // for transcode
    enum {
        TRANSCODE_CONFIGURED_TARGET_AV_FORMAT = 0x1,
        TRANSCODE_CONFIGURED_OUTPUT_PROTOCOL  = 0x2,
    };
    sp<RtkTranscodePlayer> mTranscodeManager;
    sp<ALooper> mTranscodeManagerLooper;
    bool m_bTranscodeMode;
    int32_t m_TranscodeConfiguration;
    long m_audioDecAgentId;

    RTDtvMediaPlayer(const RTDtvMediaPlayer &);
    RTDtvMediaPlayer &operator=(const RTDtvMediaPlayer &);

    enum FlagMode{
        SET,
        CLEAR,
        ASSIGN
    };
    enum {
        PLAYING     = 0x01,
        PREPARED    = 0x02,
    };
    void modifyFlags(unsigned value, FlagMode mode);

    void notifyListener_l(int msg, int ext1 = 0, int ext2 = 0);

    // for transcode
    /**
     * Get the audio decoder agent ID from DvdPlayer
     */
    long getAudioDecoderAgentID();

    /**
     * Set the agent ID to RVSD.
     */
    status_t registerDvdPlayerAgentID(sp<AHandler> &handlerId);

    /**
     * Initialize Transcode Manager.
     */
    status_t setupTranscodeManager();

};

}  // namespace android

#endif  // REALTEK_DTV_MEDIAPLAYER_H

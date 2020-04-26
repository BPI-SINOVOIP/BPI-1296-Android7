/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef ANDROID_IMEDIAPLAYER_H
#define ANDROID_IMEDIAPLAYER_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IMemory.h>
#include <utils/KeyedVector.h>
#include <system/audio.h>

// Fwd decl to make sure everyone agrees that the scope of struct sockaddr_in is
// global, and not in android::
struct sockaddr_in;

namespace android {

class Parcel;
class Surface;
class IDataSource;
struct IStreamSource;
class IGraphicBufferProducer;

#if 0
struct _AV_FORMAT_CONFIG;
typedef struct _AV_FORMAT_CONFIG AV_FORMAT_CONFIG;
struct RtkTranscodeConf;
#endif

struct IMediaHTTPService;
struct AudioPlaybackRate;
struct AVSyncSettings;
struct AHandler;

// TODO: (review me later) try to define transcode output ENUM here
enum {
    OUTPUT_FILE = 0,
    OUTPUT_STDOUT = 1,
    OUTPUT_ASHMEM = 2,
    OUTPUT_ASHMEM_RTP = 3,
    OUTPUT_UDP = 4,
    OUTPUT_UDP_RTP = 5,
    OUTPUT_UDP_ASHMEM = 6,  //OUTPUT_RTP_BOTH
    OUTPUT_FD = 7,
    OUTPUT_MAX
};

class IMediaPlayer: public IInterface
{
public:
    DECLARE_META_INTERFACE(MediaPlayer);

    virtual void            disconnect() = 0;

    virtual status_t        setDataSource(
            const sp<IMediaHTTPService> &httpService,
            const char *url,
            const KeyedVector<String8, String8>* headers) = 0;

    virtual status_t        setDataSource(int fd, int64_t offset, int64_t length) = 0;
    virtual status_t        setDataSource(const sp<IStreamSource>& source) = 0;
    virtual status_t        setDataSource(const sp<IDataSource>& source) = 0;
    virtual status_t        setVideoSurfaceTexture(
                                    const sp<IGraphicBufferProducer>& bufferProducer) = 0;
    virtual status_t        prepareAsync() = 0;
    virtual status_t        start() = 0;
    virtual status_t        stop() = 0;
    virtual status_t        pause() = 0;
    virtual status_t        isPlaying(bool* state) = 0;
    virtual status_t        setPlaybackSettings(const AudioPlaybackRate& rate) = 0;
    virtual status_t        getPlaybackSettings(AudioPlaybackRate* rate /* nonnull */) = 0;
    virtual status_t        setSyncSettings(const AVSyncSettings& sync, float videoFpsHint) = 0;
    virtual status_t        getSyncSettings(AVSyncSettings* sync /* nonnull */,
                                            float* videoFps /* nonnull */) = 0;
    virtual status_t        seekTo(int msec) = 0;
    virtual status_t        getCurrentPosition(int* msec) = 0;
    virtual status_t        getDuration(int* msec) = 0;
    virtual status_t        reset() = 0;
    virtual status_t        setAudioStreamType(audio_stream_type_t type) = 0;
    virtual status_t        setLooping(int loop) = 0;
    virtual status_t        setVolume(float leftVolume, float rightVolume) = 0;
    virtual status_t        setAuxEffectSendLevel(float level) = 0;
    virtual status_t        attachAuxEffect(int effectId) = 0;
    virtual status_t        setParameter(int key, const Parcel& request) = 0;
    virtual status_t        getParameter(int key, Parcel* reply) = 0;
    virtual status_t        setRetransmitEndpoint(const struct sockaddr_in* endpoint) = 0;
    virtual status_t        getRetransmitEndpoint(struct sockaddr_in* endpoint) = 0;
    virtual status_t        setNextPlayer(const sp<IMediaPlayer>& next) = 0;

    // Invoke a generic method on the player by using opaque parcels
    // for the request and reply.
    // @param request Parcel that must start with the media player
    // interface token.
    // @param[out] reply Parcel to hold the reply data. Cannot be null.
    // @return OK if the invocation was made successfully.
    virtual status_t        invoke(const Parcel& request, Parcel *reply) = 0;

    // Set a new metadata filter.
    // @param filter A set of allow and drop rules serialized in a Parcel.
    // @return OK if the invocation was made successfully.
    virtual status_t        setMetadataFilter(const Parcel& filter) = 0;

    // Retrieve a set of metadata.
    // @param update_only Include only the metadata that have changed
    //                    since the last invocation of getMetadata.
    //                    The set is built using the unfiltered
    //                    notifications the native player sent to the
    //                    MediaPlayerService during that period of
    //                    time. If false, all the metadatas are considered.
    // @param apply_filter If true, once the metadata set has been built based
    //                     on the value update_only, the current filter is
    //                     applied.
    // @param[out] metadata On exit contains a set (possibly empty) of metadata.
    //                      Valid only if the call returned OK.
    // @return OK if the invocation was made successfully.
    virtual status_t        getMetadata(bool update_only,
                                        bool apply_filter,
                                        Parcel *metadata) = 0;

    // Don't enclose these RTK's extra functions with USE_RT_MEDIA_PLAYER define
    // to fix mediaplayer crash issue if USE_RT_MEDIA_PLAYER is not set to true.
    // Here we need to return invalid_operation or null/0 to to avoid building failed
    // due to function not implemented.
    //===== RTK added below==============================
    virtual status_t        useRTMediaPlayer(int forceType) = 0;
    virtual status_t        fastforward(int speed) = 0;
    virtual status_t        fastrewind(int speed) = 0;
    virtual int*            getSubtitleInfo(int streamNum) = 0;
    virtual status_t        setSubtitleInfo(int streamNum,int enable,int textEncoding,int textColor,int fontSize, int syncTime, int offset) = 0;
    virtual status_t        setAutoScanExtSubtitle(int autoScan) = 0;
    virtual status_t        setExtSubtitlePath(const char *path) = 0;
    virtual int*            getAudioTrackInfo(int streamNum) = 0;
    virtual status_t        setAudioTrackInfo(int streamNum) = 0;
    virtual status_t        execSetGetNavProperty(int propertyID,int inDataSize,int outDataOffset,unsigned long *phyAddr) = 0;
    virtual status_t        deliverNaviControlCmd(int action, int status) = 0;
    virtual status_t        setStartTime(int msec) = 0;
    virtual status_t        getVideoInfo(int *width, int *height) = 0;
     // for transcode
    virtual status_t        setTranscodeMode(bool bEnable) = 0;
    virtual status_t        startTranscode() = 0;
    virtual status_t        pauseTranscode() = 0;
    virtual status_t        stopTranscode() = 0;
    // virtual status_t        setOutputFormat(RtkTranscodeConf &conf) = 0;
    virtual status_t        setOutputFormat(int outputType,
                                        int outputFormat,
                                        int fileDesc,
                                        const char *fileName,
                                        sp<IMemory> &ashmemData,
                                        sp<IMemory> &ashmemInfo,
                                        const char *udpIp,
                                        int udpPort,
                                        bool rtpHeader) = 0;
    // virtual status_t        configureTargetFormat(AV_FORMAT_CONFIG &targetFmt) = 0;
    virtual status_t        configureTargetFormat( // video
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
                                            int a_bitrate) = 0;
    virtual status_t        registerStatusHandler(sp<AHandler> handlerId) = 0;
     // for RVSD
    virtual status_t        setTranscodeVOWriteback(const int32_t flag) = 0;
    virtual status_t        setTranscodeOSDStatus(const int32_t flag) = 0;
    virtual status_t        setLayerOrderValue(const uint32_t value) = 0;
    virtual status_t        setPlaySpeedValue(const int32_t nSpeed) = 0;
    virtual status_t        initCheck() = 0;
    virtual void            SetContentMode(/*RTK_PLAYER_CONTENTMODE_e*/ int contentMode) = 0;
    virtual int             checkLoadMedia() = 0;
    virtual int             getState() = 0;
    virtual int             SetVideoWindow(int x, int y, int width, int height) = 0;
    virtual int             VideoShow() = 0;
    virtual int             VideoHide() = 0;
    virtual void            setVideoDisapper(bool disapper) = 0;
    //===== RTK added above==============================
};

// ----------------------------------------------------------------------------

class BnMediaPlayer: public BnInterface<IMediaPlayer>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif // ANDROID_IMEDIAPLAYER_H

/*
**
** Copyright 2008, The Android Open Source Project
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

#include <arpa/inet.h>
#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>

#include <media/AudioResamplerPublic.h>
#include <media/AVSyncSettings.h>

#include <media/IDataSource.h>
#include <media/IMediaHTTPService.h>
#include <media/IMediaPlayer.h>
#include <media/IStreamSource.h>

#include <gui/IGraphicBufferProducer.h>
#include <utils/String8.h>

//#include <transcode/RtkTranscodeCommon.h>
#include <media/stagefright/foundation/AHandler.h>

namespace android {

enum {
    DISCONNECT = IBinder::FIRST_CALL_TRANSACTION,
    SET_DATA_SOURCE_URL,
    SET_DATA_SOURCE_FD,
    SET_DATA_SOURCE_STREAM,
    SET_DATA_SOURCE_CALLBACK,
    PREPARE_ASYNC,
    START,
    STOP,
    IS_PLAYING,
    SET_PLAYBACK_SETTINGS,
    GET_PLAYBACK_SETTINGS,
    SET_SYNC_SETTINGS,
    GET_SYNC_SETTINGS,
    PAUSE,
    SEEK_TO,
    GET_CURRENT_POSITION,
    GET_VIDEO_INFO,
    GET_DURATION,
    RESET,
    SET_AUDIO_STREAM_TYPE,
    SET_LOOPING,
    SET_VOLUME,
    INVOKE,
    SET_METADATA_FILTER,
    GET_METADATA,
    SET_AUX_EFFECT_SEND_LEVEL,
    ATTACH_AUX_EFFECT,
    SET_VIDEO_SURFACETEXTURE,
    SET_PARAMETER,
    GET_PARAMETER,
    SET_RETRANSMIT_ENDPOINT,
    GET_RETRANSMIT_ENDPOINT,
    SET_NEXT_PLAYER,
#ifdef USE_RT_MEDIA_PLAYER
    USE_RT_MEDIAPLAYER,
    FAST_FORWARD,
    FAST_REWIND,
    GET_SUBTITLE_INFO,
    SET_SUBTITLE_INFO,
    SET_AUTO_SCAN_EXT_SUBTITLE,
    SET_EXT_SUBTITLE_PATH,
    GET_AUDIO_TRACK_INFO,
    SET_AUDIO_TRACK_INFO,
    SET_GET_NAV_PROPERTY,
    DELIVER_NAVI_CONTROL_CMD,
    SET_START_TIME,
    // for transcode
    SET_TRANSCODE_MODE,
    START_TRANSCODE,
    PAUSE_TRANSCODE,
    STOP_TRANSCODE,
    SET_OUTPUT_FORMAT,
    CONFIGURE_TARGET_FORMAT,
    REGISTER_STATUS_HANDLER,
    // for RVSD
    SET_TRANSCODE_VO_WRITEBACK,
    SET_TRANSCODE_OSD_STATUS,
    SET_LAYER_ORDER_VALUE,
    SET_PLAY_SPEED_VALUE,
    INIT_CHECK,
    SET_CONTENT_MODE,
    CHECK_LOAD_MEDIA,
    GET_STATE,
    SET_VIDEO_WINDOW,
    VIDEO_SHOW,
    VIDEO_HIDE,
    SET_VIDEO_DISAPPER,
#endif
};

class BpMediaPlayer: public BpInterface<IMediaPlayer>
{
public:
    BpMediaPlayer(const sp<IBinder>& impl)
        : BpInterface<IMediaPlayer>(impl)
    {
    }

    // disconnect from media player service
    void disconnect()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(DISCONNECT, data, &reply);
    }

    status_t setDataSource(
            const sp<IMediaHTTPService> &httpService,
            const char* url,
            const KeyedVector<String8, String8>* headers)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(httpService != NULL);
        if (httpService != NULL) {
            data.writeStrongBinder(IInterface::asBinder(httpService));
        }
        data.writeCString(url);
        if (headers == NULL) {
            data.writeInt32(0);
        } else {
            // serialize the headers
            data.writeInt32(headers->size());
            for (size_t i = 0; i < headers->size(); ++i) {
                data.writeString8(headers->keyAt(i));
                data.writeString8(headers->valueAt(i));
            }
        }
        remote()->transact(SET_DATA_SOURCE_URL, data, &reply);
        return reply.readInt32();
    }

    status_t setDataSource(int fd, int64_t offset, int64_t length) {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeFileDescriptor(fd);
        data.writeInt64(offset);
        data.writeInt64(length);
        remote()->transact(SET_DATA_SOURCE_FD, data, &reply);
        return reply.readInt32();
    }

    status_t setDataSource(const sp<IStreamSource> &source) {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(source));
        remote()->transact(SET_DATA_SOURCE_STREAM, data, &reply);
        return reply.readInt32();
    }

    status_t setDataSource(const sp<IDataSource> &source) {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(source));
        remote()->transact(SET_DATA_SOURCE_CALLBACK, data, &reply);
        return reply.readInt32();
    }

    // pass the buffered IGraphicBufferProducer to the media player service
    status_t setVideoSurfaceTexture(const sp<IGraphicBufferProducer>& bufferProducer)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        sp<IBinder> b(IInterface::asBinder(bufferProducer));
        data.writeStrongBinder(b);
        remote()->transact(SET_VIDEO_SURFACETEXTURE, data, &reply);
        return reply.readInt32();
    }

    status_t prepareAsync()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(PREPARE_ASYNC, data, &reply);
        return reply.readInt32();
    }

    status_t start()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(START, data, &reply);
        return reply.readInt32();
    }

    status_t stop()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(STOP, data, &reply);
        return reply.readInt32();
    }

    status_t isPlaying(bool* state)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(IS_PLAYING, data, &reply);
        *state = reply.readInt32();
        return reply.readInt32();
    }

    status_t setPlaybackSettings(const AudioPlaybackRate& rate)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeFloat(rate.mSpeed);
        data.writeFloat(rate.mPitch);
        data.writeInt32((int32_t)rate.mFallbackMode);
        data.writeInt32((int32_t)rate.mStretchMode);
        remote()->transact(SET_PLAYBACK_SETTINGS, data, &reply);
        return reply.readInt32();
    }

    status_t getPlaybackSettings(AudioPlaybackRate* rate /* nonnull */)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(GET_PLAYBACK_SETTINGS, data, &reply);
        status_t err = reply.readInt32();
        if (err == OK) {
            *rate = AUDIO_PLAYBACK_RATE_DEFAULT;
            rate->mSpeed = reply.readFloat();
            rate->mPitch = reply.readFloat();
            rate->mFallbackMode = (AudioTimestretchFallbackMode)reply.readInt32();
            rate->mStretchMode = (AudioTimestretchStretchMode)reply.readInt32();
        }
        return err;
    }

    status_t setSyncSettings(const AVSyncSettings& sync, float videoFpsHint)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32((int32_t)sync.mSource);
        data.writeInt32((int32_t)sync.mAudioAdjustMode);
        data.writeFloat(sync.mTolerance);
        data.writeFloat(videoFpsHint);
        remote()->transact(SET_SYNC_SETTINGS, data, &reply);
        return reply.readInt32();
    }

    status_t getSyncSettings(AVSyncSettings* sync /* nonnull */, float* videoFps /* nonnull */)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(GET_SYNC_SETTINGS, data, &reply);
        status_t err = reply.readInt32();
        if (err == OK) {
            AVSyncSettings settings;
            settings.mSource = (AVSyncSource)reply.readInt32();
            settings.mAudioAdjustMode = (AVSyncAudioAdjustMode)reply.readInt32();
            settings.mTolerance = reply.readFloat();
            *sync = settings;
            *videoFps = reply.readFloat();
        }
        return err;
    }

    status_t pause()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(PAUSE, data, &reply);
        return reply.readInt32();
    }

    status_t seekTo(int msec)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(msec);
        remote()->transact(SEEK_TO, data, &reply);
        return reply.readInt32();
    }

    status_t getCurrentPosition(int* msec)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(GET_CURRENT_POSITION, data, &reply);
        *msec = reply.readInt32();
        return reply.readInt32();
    }

    status_t getDuration(int* msec)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(GET_DURATION, data, &reply);
        *msec = reply.readInt32();
        return reply.readInt32();
    }

    status_t reset()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(RESET, data, &reply);
        return reply.readInt32();
    }

    status_t setAudioStreamType(audio_stream_type_t stream)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32((int32_t) stream);
        remote()->transact(SET_AUDIO_STREAM_TYPE, data, &reply);
        return reply.readInt32();
    }

    status_t setLooping(int loop)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(loop);
        remote()->transact(SET_LOOPING, data, &reply);
        return reply.readInt32();
    }

    status_t setVolume(float leftVolume, float rightVolume)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeFloat(leftVolume);
        data.writeFloat(rightVolume);
        remote()->transact(SET_VOLUME, data, &reply);
        return reply.readInt32();
    }

    status_t invoke(const Parcel& request, Parcel *reply)
    {
        // Avoid doing any extra copy. The interface descriptor should
        // have been set by MediaPlayer.java.
        return remote()->transact(INVOKE, request, reply);
    }

    status_t setMetadataFilter(const Parcel& request)
    {
        Parcel reply;
        // Avoid doing any extra copy of the request. The interface
        // descriptor should have been set by MediaPlayer.java.
        remote()->transact(SET_METADATA_FILTER, request, &reply);
        return reply.readInt32();
    }

    status_t getMetadata(bool update_only, bool apply_filter, Parcel *reply)
    {
        Parcel request;
        request.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        // TODO: Burning 2 ints for 2 boolean. Should probably use flags in an int here.
        request.writeInt32(update_only);
        request.writeInt32(apply_filter);
        remote()->transact(GET_METADATA, request, reply);
        return reply->readInt32();
    }

    status_t setAuxEffectSendLevel(float level)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeFloat(level);
        remote()->transact(SET_AUX_EFFECT_SEND_LEVEL, data, &reply);
        return reply.readInt32();
    }

    status_t attachAuxEffect(int effectId)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(effectId);
        remote()->transact(ATTACH_AUX_EFFECT, data, &reply);
        return reply.readInt32();
    }

    status_t setParameter(int key, const Parcel& request)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(key);
        if (request.dataSize() > 0) {
            data.appendFrom(const_cast<Parcel *>(&request), 0, request.dataSize());
        }
        remote()->transact(SET_PARAMETER, data, &reply);
        return reply.readInt32();
    }

    status_t getParameter(int key, Parcel *reply)
    {
        Parcel data;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(key);
        return remote()->transact(GET_PARAMETER, data, reply);
    }

    status_t setRetransmitEndpoint(const struct sockaddr_in* endpoint)
    {
        Parcel data, reply;
        status_t err;

        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        if (NULL != endpoint) {
            data.writeInt32(sizeof(*endpoint));
            data.write(endpoint, sizeof(*endpoint));
        } else {
            data.writeInt32(0);
        }

        err = remote()->transact(SET_RETRANSMIT_ENDPOINT, data, &reply);
        if (OK != err) {
            return err;
        }
        return reply.readInt32();
    }

    status_t setNextPlayer(const sp<IMediaPlayer>& player) {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        sp<IBinder> b(IInterface::asBinder(player));
        data.writeStrongBinder(b);
        remote()->transact(SET_NEXT_PLAYER, data, &reply);
        return reply.readInt32();
    }

    status_t getRetransmitEndpoint(struct sockaddr_in* endpoint)
    {
        Parcel data, reply;
        status_t err;

        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        err = remote()->transact(GET_RETRANSMIT_ENDPOINT, data, &reply);

        if ((OK != err) || (OK != (err = reply.readInt32()))) {
            return err;
        }

        data.read(endpoint, sizeof(*endpoint));

        return err;
    }

#ifdef USE_RT_MEDIA_PLAYER
    status_t useRTMediaPlayer(int forceType)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(forceType);
        remote()->transact(USE_RT_MEDIAPLAYER, data, &reply);
        return reply.readInt32();
    }

    status_t fastforward(int speed)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(speed);
        remote()->transact(FAST_FORWARD, data, &reply);
        return reply.readInt32();
    }

    status_t fastrewind(int speed)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(speed);
        remote()->transact(FAST_REWIND, data, &reply);
        return reply.readInt32();
    }

    int *getSubtitleInfo(int streamNum)
    {
        Parcel data, reply;
        int32_t tmp_add;
        static int glSubtitleInfo[5];

        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(streamNum);
        remote()->transact(GET_SUBTITLE_INFO, data, &reply);

        reply.readInt32(&tmp_add);
        glSubtitleInfo[0]=(int)tmp_add;
        for(int i=0;i<glSubtitleInfo[0];i++)
        {
            reply.readInt32(&tmp_add);
            glSubtitleInfo[i+1]=(int)tmp_add;
        }
        ALOGI("[%s,%s,%d] m_subtitleInfo = %d,%d,%d,%d,%d\n",__FILE__,__func__,__LINE__, glSubtitleInfo[0],glSubtitleInfo[1],glSubtitleInfo[2],glSubtitleInfo[3],glSubtitleInfo[4]);
        return glSubtitleInfo;
    }

    status_t        setSubtitleInfo(int streamNum,int enable,int textEncoding,int textColor,int fontSize, int syncTime, int offset)
    {
        Parcel data, reply;
        ALOGI("[%s,%s,%d] streamNum= %d,enable= %d,textEncoding= %d,textColor= %d,fontSize= %d\n",__FILE__,__func__,__LINE__,streamNum,enable,textEncoding,textColor,fontSize);
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(streamNum);
        data.writeInt32(enable);
        data.writeInt32(textEncoding);
        data.writeInt32(textColor);
        data.writeInt32(fontSize);
        data.writeInt32(syncTime);
        data.writeInt32(offset);
        remote()->transact(SET_SUBTITLE_INFO, data, &reply);
        return reply.readInt32();
    }

    status_t setAutoScanExtSubtitle(int autoScan)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(autoScan);
        remote()->transact(SET_AUTO_SCAN_EXT_SUBTITLE, data, &reply);
        return reply.readInt32();
    }

    status_t setExtSubtitlePath(const char *path)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeCString(path);
        remote()->transact(SET_EXT_SUBTITLE_PATH, data, &reply);
        return reply.readInt32();
    }

    int           *getAudioTrackInfo(int streamNum)
    {
        Parcel data, reply;
        int32_t tmp_add;
        static int glAudioTrackInfo[8];

        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(streamNum);
        remote()->transact(GET_AUDIO_TRACK_INFO, data, &reply);

        reply.readInt32(&tmp_add);
        glAudioTrackInfo[0]=(int)tmp_add;
        for(int i=0;i<glAudioTrackInfo[0];i++)
        {
            reply.readInt32(&tmp_add);
            glAudioTrackInfo[i+1]=(int)tmp_add;
        }
        ALOGI("[%s,%s,%d] m_AudioTrackInfo = %d,%d,%d,%d,%d,%d,%d,%d\n",__FILE__,__func__,__LINE__,glAudioTrackInfo[0],glAudioTrackInfo[1],glAudioTrackInfo[2],glAudioTrackInfo[3],glAudioTrackInfo[4],glAudioTrackInfo[5],glAudioTrackInfo[6],glAudioTrackInfo[7]);
        return glAudioTrackInfo;
    }

    status_t        setAudioTrackInfo(int streamNum)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(streamNum);
        remote()->transact(SET_AUDIO_TRACK_INFO, data, &reply);
        return reply.readInt32();
    }

    status_t    execSetGetNavProperty(int propertyID,int inDataSize,int outDataOffset,unsigned long *phyAddr)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(propertyID);
        data.writeInt32(inDataSize);
        data.writeInt32(outDataOffset);
        remote()->transact(SET_GET_NAV_PROPERTY, data, &reply);
        *phyAddr=(unsigned long)reply.readInt32();
        return reply.readInt32();
    }

    status_t    deliverNaviControlCmd(int action, int status)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(action);
        data.writeInt32(status);
        remote()->transact(DELIVER_NAVI_CONTROL_CMD, data, &reply);
        return reply.readInt32();
    }

    status_t    setStartTime(int msec)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(msec);
        remote()->transact(SET_START_TIME, data, &reply);
        return reply.readInt32();
    }

    status_t getVideoInfo(int* width, int *height)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(GET_VIDEO_INFO, data, &reply);
        *width = reply.readInt32();
        *height = reply.readInt32();
        return reply.readInt32();
    }

     // for transcode
    status_t    setTranscodeMode(bool bEnable)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(static_cast<int>(bEnable));
        remote()->transact(SET_TRANSCODE_MODE, data, &reply);
        return reply.readInt32();
    }

    status_t    startTranscode()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(START_TRANSCODE, data, &reply);
        return reply.readInt32();
    }

    status_t    pauseTranscode()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(PAUSE_TRANSCODE, data, &reply);
        return reply.readInt32();
    }

    status_t    stopTranscode()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(STOP_TRANSCODE, data, &reply);
        return reply.readInt32();
    }

    status_t    setOutputFormat(int outputType,
                                int outputFormat,
                                int fileDesc,
                                const char *fileName,
                                sp<IMemory> &ashmemData,
                                sp<IMemory> &ashmemInfo,
                                const char *udpIp,
                                int udpPort,
                                bool rtpHeader)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        ALOGV("setOutputFormat outputType:%d",outputType);
        // outputType
        data.writeInt32(outputType);
        ALOGV("setOutputFormat outputFormat:%d",outputFormat);
        // outputFormat
        data.writeInt32(outputFormat);
        // fileDesc
        if(outputType == OUTPUT_FD)
        {
            data.writeDupFileDescriptor(fileDesc);
        }
        if(fileName == NULL)
            fileName = "";

        ALOGV("setOutputFormat fileName: %s",fileName);
        int len = strlen(fileName)+1;
        // fileName
        data.writeInt32(len);
        data.write(fileName,len);

        // TODO: ashmem review correctness
        sp<IBinder> bData(ashmemData->asBinder(ashmemData));
        sp<IBinder> bInfo(ashmemInfo->asBinder(ashmemInfo));
        data.writeStrongBinder(bData);
        data.writeStrongBinder(bInfo);

        if(udpIp == NULL)
            udpIp = "";
        // udpIp
        len = strlen(udpIp)+1;
        data.writeInt32(len);
        data.write(udpIp,len);
        // udpPort
        data.writeInt32(udpPort);
        // rtpHeader
        data.writeInt32(rtpHeader);
        remote()->transact(SET_OUTPUT_FORMAT, data, &reply);
        return reply.readInt32();
    }

    status_t    configureTargetFormat(  // video
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
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        // video
        data.writeInt32(width);
        data.writeInt32(height);
        data.writeInt32(fps);
        data.writeInt32(v_bitrate);
        data.writeInt32(iframeInterval);
        data.writeInt32(rateControl);
        data.writeInt32(aspectRatio);
        data.writeInt32(interlace);
        data.writeInt32(shareWBbuf);
        // audio
        data.writeInt32(channelCount);
        data.writeInt32(channelMode);
        data.writeInt32(sampleRate);
        data.writeInt32(a_bitrate);

        remote()->transact(CONFIGURE_TARGET_FORMAT, data, &reply);
        return reply.readInt32();
    }

    status_t    registerStatusHandler(sp<AHandler> handlerId)
    {
        ALOGV("handlerId:%d", handlerId->id());
        return INVALID_OPERATION;
    }

     // for RVSD
    status_t    setTranscodeVOWriteback(const int32_t flag)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(flag);
        remote()->transact(SET_TRANSCODE_VO_WRITEBACK, data, &reply);
        return reply.readInt32();
    }

    status_t    setTranscodeOSDStatus(const int32_t flag)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(flag);
        remote()->transact(SET_TRANSCODE_OSD_STATUS, data, &reply);
        return reply.readInt32();
    }

    status_t    setLayerOrderValue(const uint32_t value)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeUint32(value);
        remote()->transact(SET_LAYER_ORDER_VALUE, data, &reply);
        return reply.readInt32();
    }

    status_t    setPlaySpeedValue(const int32_t nSpeed)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(nSpeed);
        remote()->transact(SET_PLAY_SPEED_VALUE, data, &reply);
        return reply.readInt32();

    }

    status_t    initCheck()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(INIT_CHECK, data, &reply);
        return reply.readInt32();
    }

    void        SetContentMode(/*RTK_PLAYER_CONTENTMODE_e*/ int contentMode)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(contentMode);
        remote()->transact(SET_CONTENT_MODE, data, &reply);
        return;
    }

    int         checkLoadMedia()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(CHECK_LOAD_MEDIA, data, &reply);
        return reply.readInt32();
    }

    int         getState()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(GET_STATE, data, &reply);
        return reply.readInt32();
    }

    int         SetVideoWindow(int x, int y, int width, int height)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(x);
        data.writeInt32(y);
        data.writeInt32(width);
        data.writeInt32(height);
        remote()->transact(SET_VIDEO_WINDOW, data, &reply);
        return reply.readInt32();
    }

    int         VideoShow()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(VIDEO_SHOW, data, &reply);
        return reply.readInt32();
    }

    int         VideoHide()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        remote()->transact(VIDEO_HIDE, data, &reply);
        return reply.readInt32();
    }

    void        setVideoDisapper(bool disapper)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IMediaPlayer::getInterfaceDescriptor());
        data.writeInt32(disapper);
        remote()->transact(SET_VIDEO_DISAPPER, data, &reply);
        return;
    }

#endif // USE_RT_MEDIA_PLAYER
};

IMPLEMENT_META_INTERFACE(MediaPlayer, "android.media.IMediaPlayer");

// ----------------------------------------------------------------------

status_t BnMediaPlayer::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
        case DISCONNECT: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            disconnect();
            return NO_ERROR;
        } break;
        case SET_DATA_SOURCE_URL: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);

            sp<IMediaHTTPService> httpService;
            if (data.readInt32()) {
                httpService =
                    interface_cast<IMediaHTTPService>(data.readStrongBinder());
            }

            const char* url = data.readCString();
            if (url == NULL) {
                reply->writeInt32(BAD_VALUE);
                return NO_ERROR;
            }
            KeyedVector<String8, String8> headers;
            int32_t numHeaders = data.readInt32();
            for (int i = 0; i < numHeaders; ++i) {
                String8 key = data.readString8();
                String8 value = data.readString8();
                headers.add(key, value);
            }
            reply->writeInt32(setDataSource(
                        httpService, url, numHeaders > 0 ? &headers : NULL));
            return NO_ERROR;
        } break;
        case SET_DATA_SOURCE_FD: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int fd = data.readFileDescriptor();
            int64_t offset = data.readInt64();
            int64_t length = data.readInt64();
            reply->writeInt32(setDataSource(fd, offset, length));
            return NO_ERROR;
        }
        case SET_DATA_SOURCE_STREAM: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            sp<IStreamSource> source =
                interface_cast<IStreamSource>(data.readStrongBinder());
            if (source == NULL) {
                reply->writeInt32(BAD_VALUE);
            } else {
                reply->writeInt32(setDataSource(source));
            }
            return NO_ERROR;
        }
        case SET_DATA_SOURCE_CALLBACK: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            sp<IDataSource> source =
                interface_cast<IDataSource>(data.readStrongBinder());
            if (source == NULL) {
                reply->writeInt32(BAD_VALUE);
            } else {
                reply->writeInt32(setDataSource(source));
            }
            return NO_ERROR;
        }
        case SET_VIDEO_SURFACETEXTURE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            sp<IGraphicBufferProducer> bufferProducer =
                    interface_cast<IGraphicBufferProducer>(data.readStrongBinder());
            reply->writeInt32(setVideoSurfaceTexture(bufferProducer));
            return NO_ERROR;
        } break;
        case PREPARE_ASYNC: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(prepareAsync());
            return NO_ERROR;
        } break;
        case START: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(start());
            return NO_ERROR;
        } break;
        case STOP: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(stop());
            return NO_ERROR;
        } break;
        case IS_PLAYING: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            bool state;
            status_t ret = isPlaying(&state);
            reply->writeInt32(state);
            reply->writeInt32(ret);
            return NO_ERROR;
        } break;
        case SET_PLAYBACK_SETTINGS: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            AudioPlaybackRate rate = AUDIO_PLAYBACK_RATE_DEFAULT;
            rate.mSpeed = data.readFloat();
            rate.mPitch = data.readFloat();
            rate.mFallbackMode = (AudioTimestretchFallbackMode)data.readInt32();
            rate.mStretchMode = (AudioTimestretchStretchMode)data.readInt32();
            reply->writeInt32(setPlaybackSettings(rate));
            return NO_ERROR;
        } break;
        case GET_PLAYBACK_SETTINGS: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            AudioPlaybackRate rate = AUDIO_PLAYBACK_RATE_DEFAULT;
            status_t err = getPlaybackSettings(&rate);
            reply->writeInt32(err);
            if (err == OK) {
                reply->writeFloat(rate.mSpeed);
                reply->writeFloat(rate.mPitch);
                reply->writeInt32((int32_t)rate.mFallbackMode);
                reply->writeInt32((int32_t)rate.mStretchMode);
            }
            return NO_ERROR;
        } break;
        case SET_SYNC_SETTINGS: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            AVSyncSettings sync;
            sync.mSource = (AVSyncSource)data.readInt32();
            sync.mAudioAdjustMode = (AVSyncAudioAdjustMode)data.readInt32();
            sync.mTolerance = data.readFloat();
            float videoFpsHint = data.readFloat();
            reply->writeInt32(setSyncSettings(sync, videoFpsHint));
            return NO_ERROR;
        } break;
        case GET_SYNC_SETTINGS: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            AVSyncSettings sync;
            float videoFps;
            status_t err = getSyncSettings(&sync, &videoFps);
            reply->writeInt32(err);
            if (err == OK) {
                reply->writeInt32((int32_t)sync.mSource);
                reply->writeInt32((int32_t)sync.mAudioAdjustMode);
                reply->writeFloat(sync.mTolerance);
                reply->writeFloat(videoFps);
            }
            return NO_ERROR;
        } break;
        case PAUSE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(pause());
            return NO_ERROR;
        } break;
        case SEEK_TO: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(seekTo(data.readInt32()));
            return NO_ERROR;
        } break;
        case GET_CURRENT_POSITION: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int msec = 0;
            status_t ret = getCurrentPosition(&msec);
            reply->writeInt32(msec);
            reply->writeInt32(ret);
            return NO_ERROR;
        } break;
        case GET_VIDEO_INFO: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int width = 0;
            int height = 0;
            status_t ret = getVideoInfo(&width, &height);
            reply->writeInt32(width);
            reply->writeInt32(height);
            reply->writeInt32(ret);
            return NO_ERROR;
        } break;
        case GET_DURATION: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int msec = 0;
            status_t ret = getDuration(&msec);
            reply->writeInt32(msec);
            reply->writeInt32(ret);
            return NO_ERROR;
        } break;
        case RESET: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(reset());
            return NO_ERROR;
        } break;
        case SET_AUDIO_STREAM_TYPE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(setAudioStreamType((audio_stream_type_t) data.readInt32()));
            return NO_ERROR;
        } break;
        case SET_LOOPING: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(setLooping(data.readInt32()));
            return NO_ERROR;
        } break;
        case SET_VOLUME: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            float leftVolume = data.readFloat();
            float rightVolume = data.readFloat();
            reply->writeInt32(setVolume(leftVolume, rightVolume));
            return NO_ERROR;
        } break;
        case INVOKE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            status_t result = invoke(data, reply);
            return result;
        } break;
        case SET_METADATA_FILTER: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(setMetadataFilter(data));
            return NO_ERROR;
        } break;
        case GET_METADATA: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            bool update_only = static_cast<bool>(data.readInt32());
            bool apply_filter = static_cast<bool>(data.readInt32());
            const status_t retcode = getMetadata(update_only, apply_filter, reply);
            reply->setDataPosition(0);
            reply->writeInt32(retcode);
            reply->setDataPosition(0);
            return NO_ERROR;
        } break;
        case SET_AUX_EFFECT_SEND_LEVEL: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(setAuxEffectSendLevel(data.readFloat()));
            return NO_ERROR;
        } break;
        case ATTACH_AUX_EFFECT: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(attachAuxEffect(data.readInt32()));
            return NO_ERROR;
        } break;
        case SET_PARAMETER: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int key = data.readInt32();

            Parcel request;
            if (data.dataAvail() > 0) {
                request.appendFrom(
                        const_cast<Parcel *>(&data), data.dataPosition(), data.dataAvail());
            }
            request.setDataPosition(0);
            reply->writeInt32(setParameter(key, request));
            return NO_ERROR;
        } break;
        case GET_PARAMETER: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            return getParameter(data.readInt32(), reply);
        } break;
        case SET_RETRANSMIT_ENDPOINT: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);

            struct sockaddr_in endpoint;
            memset(&endpoint, 0, sizeof(endpoint));
            int amt = data.readInt32();
            if (amt == sizeof(endpoint)) {
                data.read(&endpoint, sizeof(struct sockaddr_in));
                reply->writeInt32(setRetransmitEndpoint(&endpoint));
            } else {
                reply->writeInt32(setRetransmitEndpoint(NULL));
            }

            return NO_ERROR;
        } break;
        case GET_RETRANSMIT_ENDPOINT: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);

            struct sockaddr_in endpoint;
            memset(&endpoint, 0, sizeof(endpoint));
            status_t res = getRetransmitEndpoint(&endpoint);

            reply->writeInt32(res);
            reply->write(&endpoint, sizeof(endpoint));

            return NO_ERROR;
        } break;
        case SET_NEXT_PLAYER: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(setNextPlayer(interface_cast<IMediaPlayer>(data.readStrongBinder())));

            return NO_ERROR;
        } break;
#ifdef USE_RT_MEDIA_PLAYER
        case USE_RT_MEDIAPLAYER: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(useRTMediaPlayer(data.readInt32()));
            return NO_ERROR;
        } break;
        case FAST_FORWARD: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(fastforward(data.readInt32()));
            return NO_ERROR;
        } break;
        case FAST_REWIND: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(fastrewind(data.readInt32()));
            return NO_ERROR;
        } break;
        case GET_SUBTITLE_INFO: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int *res=getSubtitleInfo(data.readInt32());
            ALOGI("[%s,%s,%d] m_subtitleInfo = %d,%d,%d,%d,%d\n",__FILE__,__func__,__LINE__,res[0],res[1],res[2],res[3],res[4]);
            reply->writeInt32((int32_t)res[0]);
            for(int i=0;i<res[0];i++)
                reply->writeInt32((int32_t)res[i+1]);
            return NO_ERROR;
        } break;
        case SET_SUBTITLE_INFO: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
#if 1
            int streamNum=(int)data.readInt32();
            int enable=(int)data.readInt32();
            int textEncoding=(int)data.readInt32();
            int textColor=(int)data.readInt32();
            int fontSize=(int)data.readInt32();
            int syncTime=(int)data.readInt32();
            int offset=(int)data.readInt32();
            ALOGI("[%s,%s,%d] streamNum= %d,enable= %d,textEncoding= %d,textColor= %d,fontSize= %d\n",__FILE__,__func__,__LINE__,streamNum,enable,textEncoding,textColor,fontSize);
            reply->writeInt32(setSubtitleInfo(streamNum,enable,textEncoding,textColor,fontSize,syncTime,offset));
#else
            reply->writeInt32(setSubtitleInfo((int)data.readInt32(),(int)data.readInt32(),(int)data.readInt32(),(int)data.readInt32(),(int)data.readInt32()));
#endif
            ALOGI("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
            return NO_ERROR;
        } break;
        case SET_AUTO_SCAN_EXT_SUBTITLE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(setAutoScanExtSubtitle(data.readInt32()));
            return NO_ERROR;
        } break;
        case SET_EXT_SUBTITLE_PATH: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            const char* path = data.readCString();
            reply->writeInt32(setExtSubtitlePath(path));
            return NO_ERROR;
        } break;
        case GET_AUDIO_TRACK_INFO: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int *res=getAudioTrackInfo((int)data.readInt32());
            ALOGI("[%s,%s,%d] m_AudioTrackInfo = %d,%d,%d,%d,%d,%d,%d,%d\n",__FILE__,__func__,__LINE__,res[0],res[1],res[2],res[3],res[4],res[5],res[6],res[7]);
            reply->writeInt32((int32_t)res[0]);
            for(int i=0;i<res[0];i++)
                reply->writeInt32((int32_t)res[i+1]);
            return NO_ERROR;
        } break;
        case SET_AUDIO_TRACK_INFO: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(setAudioTrackInfo((int)data.readInt32()));
            ALOGI("[%s,%s,%d]\n",__FILE__,__func__,__LINE__);
            return NO_ERROR;
        } break;
        case SET_GET_NAV_PROPERTY: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int propertyID=(int)data.readInt32();
            int inDataSize=(int)data.readInt32();
            int outDataOffset=(int)data.readInt32();
            unsigned long phyAddr=0;
            status_t ret = execSetGetNavProperty(propertyID,inDataSize,outDataOffset,&phyAddr);
            reply->writeInt32((int)phyAddr);
            reply->writeInt32(ret);
            ALOGI("[%s,%s,%d] propertyID=%d,inDataSize=%d,outDataOffset=%d,phyAddr=0x%x\n",__FILE__,__func__,__LINE__,
                (int)propertyID,(int)inDataSize,outDataOffset,(int)phyAddr);
            return NO_ERROR;
        } break;
        case DELIVER_NAVI_CONTROL_CMD: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int action = (int)data.readInt32();
            int status = (int)data.readInt32();
            status_t ret = deliverNaviControlCmd(action, status);
            reply->writeInt32(ret);
            ALOGI("[%s,%s,%d] action=%d,status=%d\n",__FILE__,__func__,__LINE__, action, status);
            return NO_ERROR;
        } break;
        case SET_START_TIME: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int msec = (int)data.readInt32();
            status_t ret = setStartTime(msec);
            reply->writeInt32(ret);
            ALOGI("[%s,%s,%d] msec=%d\n",__FILE__,__func__,__LINE__, msec);
            return NO_ERROR;
        } break;
     // for transcode
        case SET_TRANSCODE_MODE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            bool bEnable = (bool)data.readInt32();
            status_t ret = setTranscodeMode(bEnable);
            reply->writeInt32(ret);
            ALOGI("[%s,%s,%d] bEnable=%d\n",__FILE__,__func__,__LINE__, bEnable);
            return NO_ERROR;
        } break;
        case START_TRANSCODE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(startTranscode());
            return NO_ERROR;
        } break;
        case PAUSE_TRANSCODE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(pauseTranscode());
            return NO_ERROR;
        } break;
        case STOP_TRANSCODE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(stopTranscode());
            return NO_ERROR;
        } break;
        case SET_OUTPUT_FORMAT: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            // declare local variable
            int outputType = 0;
            int outputFormat = 0;
            int fileDesc = 0;
            const char *fileName;
            sp<IMemory> ashmemData;
            sp<IMemory> ashmemInfo;
            const char *udpIp;
            int udpPort = 0;
            bool rtpHeader = false;

            // outputType
            outputType = data.readInt32();
            // outputFormat
            outputFormat = data.readInt32();
            // fileDesc
            if(outputType == OUTPUT_FD)
            {
                fileDesc = data.readFileDescriptor();
                ALOGV("SET_OUTPUT_FORMAT read fd %d",fileDesc);
            }
            // fileName
            int len = data.readInt32();
            char *fn = (char *)malloc(len);
            data.read(fn,len);
            fileName = fn;
            ALOGV("SET_OUTPUT_FORMAT fileName %s",fileName);
            // TODO: ashmem review later
            ashmemData = interface_cast<IMemory>(data.readStrongBinder());
            ashmemInfo = interface_cast<IMemory>(data.readStrongBinder());
            // udpIp
            len = data.readInt32();
            char *ip = (char *)malloc(len);
            data.read(ip,len);
            udpIp = ip;
            // udpPort
            udpPort = data.readInt32();
            // rtpHeader
            rtpHeader = (bool)data.readInt32();
            // do function call
            reply->writeInt32(setOutputFormat(outputType,
                                            outputFormat,
                                            fileDesc,
                                            fileName,
                                            ashmemData,
                                            ashmemInfo,
                                            udpIp,
                                            udpPort,
                                            rtpHeader));
            // free memory
            free(fn);
            free(ip);
            return NO_ERROR;
        } break;
        case CONFIGURE_TARGET_FORMAT: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            // video
            int width = data.readInt32();
            int height = data.readInt32();
            int fps = data.readInt32();
            int v_bitrate = data.readInt32();
            int iframeInterval = data.readInt32();
            int rateControl = data.readInt32();
            int aspectRatio = data.readInt32();
            int interlace = data.readInt32();
            int shareWBbuf = data.readInt32();
            // audio
            int channelCount = data.readInt32();
            int channelMode = data.readInt32();
            int sampleRate = data.readInt32();
            int a_bitrate = data.readInt32();
            reply->writeInt32(configureTargetFormat(// video
                                            width,
                                            height,
                                            fps,
                                            v_bitrate,
                                            iframeInterval,
                                            rateControl,
                                            aspectRatio,
                                            interlace,
                                            shareWBbuf,
                                            // audio
                                            channelCount,
                                            channelMode,
                                            sampleRate,
                                            a_bitrate));
            return NO_ERROR;
        } break;
        case REGISTER_STATUS_HANDLER: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            //FIXME: is there any ways to pass AHandler?
            //reply->writeInt32(registerStatusHandler(data.readInt32()));
            return NO_ERROR;
        } break;
        // for RVSD
        case SET_TRANSCODE_VO_WRITEBACK: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int32_t flag = static_cast<int32_t>(data.readInt32());
            reply->writeInt32(setTranscodeVOWriteback(flag));
            return NO_ERROR;
        } break;
        case SET_TRANSCODE_OSD_STATUS: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int32_t flag = static_cast<int32_t>(data.readInt32());
            reply->writeInt32(setTranscodeOSDStatus(flag));
            return NO_ERROR;
        } break;
        case SET_LAYER_ORDER_VALUE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            uint32_t value = static_cast<uint32_t>(data.readUint32());
            reply->writeInt32(setLayerOrderValue(value));
            return NO_ERROR;
        } break;
        case SET_PLAY_SPEED_VALUE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int32_t nSpeed = static_cast<int32_t>(data.readInt32());
            reply->writeInt32(setPlaySpeedValue(nSpeed));
            return NO_ERROR;
        } break;
        case INIT_CHECK: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(initCheck());
            return NO_ERROR;
        } break;
        case SET_CONTENT_MODE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int contentMode = static_cast<int>(data.readInt32());
            SetContentMode(contentMode);
            return NO_ERROR;
        } break;
        case CHECK_LOAD_MEDIA: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(checkLoadMedia());
            return NO_ERROR;
        } break;
        case GET_STATE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(getState());
            return NO_ERROR;
        } break;
        case SET_VIDEO_WINDOW: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            int x = static_cast<int>(data.readInt32());
            int y = static_cast<int>(data.readInt32());
            int width = static_cast<int>(data.readInt32());
            int height = static_cast<int>(data.readInt32());
            reply->writeInt32(SetVideoWindow(x,y,width,height));
            return NO_ERROR;
        } break;
        case VIDEO_SHOW: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(VideoShow());
            return NO_ERROR;
        } break;
        case VIDEO_HIDE: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            reply->writeInt32(VideoHide());
            return NO_ERROR;
        } break;
        case SET_VIDEO_DISAPPER: {
            CHECK_INTERFACE(IMediaPlayer, data, reply);
            bool disapper = static_cast<bool>(data.readInt32());
            setVideoDisapper(disapper);
            return NO_ERROR;
        } break;
#endif // USE_RT_MEDIA_PLAYER
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

// ----------------------------------------------------------------------------

} // namespace android

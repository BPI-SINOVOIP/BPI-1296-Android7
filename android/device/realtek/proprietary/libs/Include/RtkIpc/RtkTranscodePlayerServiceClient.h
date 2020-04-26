#ifndef RTKTRANSCODEPLAYERSERVICECLIENT_H
#define RTKTRANSCODEPLAYERSERVICECLIENT_H

#include "RtkIpc/IRtkTranscodePlayerServiceClient.h"
#include "RtkIpc/IPlayerListener.h"
#include "RtkIpc/RtkTranscodePlayerService.h"
#include "RtkTranscodePlayer.h"

namespace android {

class RtkTranscodePlayerServiceClient: public BnRtkTranscodePlayerServiceClient,
                                        private IBinder::DeathRecipient
{
public:
    RtkTranscodePlayerServiceClient(int onlyTrack, sp<RtkTranscodePlayerService>& service);
    ~RtkTranscodePlayerServiceClient();

    status_t initiate(int transcodeType = TranscodeTypeFile,
                output_format outputFormat = OUTPUT_FORMAT_MPEG2TS); //outputFormat is already obsolete, use setOutputFormat() instead.
    status_t setDataSource(const char *path);
    status_t setDataSource(int32_t fileId, int64_t offset, int64_t length);
    status_t setCameraSource(/*int cameraId, String16 &clientName,*/ const sp<::android::hardware::ICamera>& camera);
    status_t setAudioSource(int audioSource);
    status_t setSurface(const sp<IGraphicBufferProducer> &bufferProducer);
    status_t setOutputFormat(RtkTranscodeConf &config);
    status_t stop();
    bool EOSReached();
    status_t setPlayback(bool video, bool audio);
    status_t setTranscode(bool transcode);
    status_t setDebug(bool controlDebug);
    status_t setDecode();
    bool getTranscodeEnabled();
    sp<IMemory> getCameraSnapshot(CAMERA_SNAPSHOT *snapshot);
    String8 getCameraParameters();
    status_t configureTargetFormat(const AV_FORMAT_CONFIG &config);
    status_t resetTargetFormat(const AV_FORMAT_CONFIG &config);
    status_t configureRxPreviewResolution(int32_t width, int32_t height);
    status_t notifyHDMIRxAudioHotplug(int32_t state);
    void destoryBnObj();
    void release();

    virtual void binderDied(const wp<IBinder>& who);

protected:
    void setListener(sp<IPlayerListener> &l);

private:
    sp<RtkTranscodePlayer> mPlayer;
    wp<RtkTranscodePlayerService> mService;
};

}; // namespace android

#endif // RTKTRANSCODEPLAYERSERVICE_H

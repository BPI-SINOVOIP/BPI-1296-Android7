#ifndef IRTKTRANSCODEPLAYERSERVICECLIENT_H
#define IRTKTRANSCODEPLAYERSERVICECLIENT_H

#include <camera/android/hardware/ICamera.h>
#include <gui/IGraphicBufferProducer.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <media/mediarecorder.h>
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/AMessage.h>
#include "RtkTranscodeCommon.h"
#include "RtkIpc/IPlayerListener.h"

#define STRING_BUFFER_LENGTH    4096

namespace android{

class IRtkTranscodePlayerServiceClient: public IInterface
{
public:
	DECLARE_META_INTERFACE(RtkTranscodePlayerServiceClient);

    virtual status_t initiate(int transcodeType = TranscodeTypeFile,
                            output_format outputFormat = OUTPUT_FORMAT_MPEG2TS) = 0; //outputFormat is already obsolete, use setOutputFormat() instead.
    virtual status_t setDataSource(const char *path) = 0;
    virtual status_t setDataSource(int32_t fileId, int64_t offset, int64_t length) = 0;
    /* new api added for binder client/server architecture
     * rtktranscodeplayerservice has no permission to open camera device,
     * so client side should have camera permission, and open device for service
     * then finally, pass the camera handle to service side
     */
    virtual status_t setCameraSource(/*int cameraId, String16 &clientName,*/ const sp<::android::hardware::ICamera>& camera) = 0;
    virtual status_t setAudioSource(int audioSource) = 0;
    virtual status_t setSurface(const sp<IGraphicBufferProducer> &bufferProducer) = 0;
    virtual status_t setOutputFormat(RtkTranscodeConf &config) = 0;
    virtual status_t stop() = 0;
    virtual bool EOSReached() = 0;
    virtual status_t setPlayback(bool video, bool audio) = 0;
    virtual status_t setTranscode(bool transcode) = 0;
    virtual status_t setDebug(bool controlDebug) = 0;
    virtual status_t setDecode() = 0;
    virtual bool getTranscodeEnabled() = 0;
    virtual sp<IMemory> getCameraSnapshot(CAMERA_SNAPSHOT *snapshot) = 0;
    virtual String8 getCameraParameters() = 0;
    virtual status_t configureTargetFormat(const AV_FORMAT_CONFIG &config) = 0;
    virtual status_t resetTargetFormat(const AV_FORMAT_CONFIG &config) = 0;
    virtual status_t configureRxPreviewResolution(int32_t width, int32_t height) = 0;
    virtual status_t notifyHDMIRxAudioHotplug(int32_t state) = 0;
    virtual void destoryBnObj() = 0;
    virtual void release() = 0;

    void registerStatusHandler(sp<AHandler> id) { 
        mStatusId = id;
        mListener->registerStatusHandler(id);
    };

    //void registerStatusHandler(ALooper::handler_id id) { mStatusId = id; };
protected:
    virtual void setListener(sp<IPlayerListener> &l) = 0;
    sp<IPlayerListener> mListener;
    sp<AHandler> mStatusId;
};

// BnInterface.
class BnRtkTranscodePlayerServiceClient: public BnInterface<IRtkTranscodePlayerServiceClient>
{
public:

	virtual status_t onTransact(
		uint32_t code,
		const Parcel& data,
		Parcel* reply,
		uint32_t flags = 0);

    char mDataSource[STRING_BUFFER_LENGTH];
    char mFileName[STRING_BUFFER_LENGTH];
    char mUdpIp[STRING_BUFFER_LENGTH];
};

}; // namespace android

#endif // IRTKTRANSCODEPLAYERSERVICECLIENT_H


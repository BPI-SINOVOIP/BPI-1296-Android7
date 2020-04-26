#ifndef _HDMIRXAUDIOREADER_H_
#define _HDMIRXAUDIOREADER_H_

#include <utils/Log.h>
#include <OSAL_RTK.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <Errors.h>
#include <cutils/properties.h>
#include "AudioRPCBaseDS.h"
#include "RPCBaseDS.h"
#include "AudioRPC_Agent.h"
#include "AudioRPC_System.h"
#include "SVCFunc.h"
#include "RPCProxy.h"
#include "hresult.h"
#include "AudioInbandAPI.h"
#include "audio_common.h"

#define AIN_CHANNEL_NUM     2
#define RINGBUFFER_SIZE     (64*1024)
#define MAX_AUDIO_SIZE      8192 //Audio encoder max buffer size is 8192

namespace android {

class HDMIRxAudioReader
{

public:
    HDMIRxAudioReader();
    ~HDMIRxAudioReader();

    // basic apis
    void start();
    void stop();

private:

    // I think capture allocator is not used
    OSAL_RTKALLOCATOR *mCaptureAllocator;
    OSAL_RTKALLOCATOR *mPassthroughAllocator[AIN_CHANNEL_NUM];
    OSAL_RTKALLOCATOR *mAIInAllocator[AIN_CHANNEL_NUM];

    long mAudioInId;
    long mAudioOutId;
    long mPinID;
    long mAoutPinId;    //AO flash pin id

    bool    mRxReady;
    bool    mRxLPCM;
    int32_t mSrcSampleRate;
    int32_t mChannelCount;
    int32_t mSampleRate;
    int32_t mSampleSize;
    int32_t mAudioSource;

    status_t retrieveHdmiRxAduioInfo();
    status_t prepare();
    status_t createAIInstance();
    status_t destroyAIInstance();
    //status_t getAOInstance();
    status_t destroyAudioInstance(long instanceId);
    status_t stopAudioRender(long instanceId);
    status_t pauseAudioInstance(long instanceId);
    status_t stopAudioInstance(long instanceId);

    status_t getAOFlashPin();
    status_t releaseAOFlashPin();

    // for 6.0 API
    status_t getAOPinId();
    status_t releaseAOPinId(long instanceId, long pinId);

    status_t initRingBuffer();
    status_t uninitRingBuffer();

    // for 6.0 API
    status_t initPCMRingBuffer();
    status_t uninitPCMRingBuffer();
    status_t runAudioInstance(long instanceId);

    status_t configADC();
    status_t setupAudioRedirect();
    status_t enableAudioInput();
    status_t disableAudioInput();
    //status_t startAudioRender();
    status_t startAudioRender(long instanceId);
    status_t stopAudioRender();
    // for 6.0 API
    status_t createAudioInstance(uint32_t instanceType, long *intanceId);
    status_t getAOInstance();
};

} // namespace android

#endif // _HDMIRXAUDIOREADER_H_

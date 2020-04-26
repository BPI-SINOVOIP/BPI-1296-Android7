/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _CAMERAAGENT_H_
#define _CAMERAAGENT_H_

#include <binder/IMemory.h>

#include "RtkTranscodeCommon.h"

namespace android {

struct ABuffer;
struct ALooper;

struct RtkFilter;

struct CameraAgent : public RtkFilter
{
public:
    CameraAgent();
    CameraAgent(bool useScreenRecord);

    enum {
        /* Player --> Agent */
        kWhatDebugBuffers = 0x200,
        kWhatStop = WHAT_STOP,

        /* CameraReader --> Agent */
        kWhatDrainVideoBuffer = 0x210,
        kWhatEndOfVideoStream,
        kWhatDrainVideoHandle,

        /* AudioRecordReader --> Agent */
        kWhatDrainAudioBuffer = 0x220,
        kWhatEndOfAudioStream,

        /* PostProcessor --> Agent */
        kWhatDrainBufferDone = 0x230,
    };

protected:
    virtual ~CameraAgent();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum {
        UNINITIALIZED,
    };

    bool mInputWriteback;
    int mState;

    bool mHasVideo;
    bool mHasAudio;
    unsigned int mVideoCnt;
    unsigned int mAudioCnt;

    void sendToSink(uint32_t sourceIndex, const sp<AMessage> &msg, const sp<ABuffer> &buf);
    void checkEOS();
    void resetVariables();
    DISALLOW_EVIL_CONSTRUCTORS(CameraAgent);
};

} // namespace android

#endif //_CAMERAAGENT_H_

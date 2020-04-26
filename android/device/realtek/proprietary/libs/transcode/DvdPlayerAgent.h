/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _DVDPLAYERAGENT_H_
#define _DVDPLAYERAGENT_H_

#include <binder/IMemory.h>

#include "RtkTranscodeCommon.h"

namespace android {

struct ABuffer;
struct ALooper;

struct RtkFilter;

struct DvdPlayerAgent : public RtkFilter
{
public:
    DvdPlayerAgent();

    enum {
        /* Player --> Agent */
        kWhatDebugBuffers = 0x200,
        kWhatStop = WHAT_STOP,

        /* RVSD --> Agent */
        kWhatDrainVideoBuffer = 0x210,
        kWhatEndOfVideoStream,
        kWhatDrainVideoHandle,

        /* AudioReader --> Agent */
        kWhatDrainAudioBuffer = 0x220,
        kWhatEndOfAudioStream,

        /* PostProcessor --> Agent */
        kWhatDrainBufferDone = 0x230,

        /* RVSD --> Agent */
        kWhatGetWriteBackBuffer = 0x240,
        kWhatGetWriteBackBufferDone,
    };

protected:
    virtual ~DvdPlayerAgent();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum {
        UNINITIALIZED,
    };

    int mState;

    bool mVideoEOS;
    bool mAudioEOS;
    bool mHasVideo;
    bool mHasAudio;

    unsigned int mVideoCnt;
    unsigned int mAudioCnt;

    void sendToSink(uint32_t sourceIndex, const sp<AMessage> &msg, const sp<ABuffer> &buf);
    void checkEOS();
    void resetVariables();
    DISALLOW_EVIL_CONSTRUCTORS(DvdPlayerAgent);

    unsigned long *mWBvirtaddr;
    unsigned long  mWBphysaddr;
    uint32_t mWBSize;
    uint32_t mWBbufnum;
    bool mkWhtAlcWBBufFlg;
};

} // namespace android
#endif /* end of not define _DVDPLAYERAGENT_H_ */

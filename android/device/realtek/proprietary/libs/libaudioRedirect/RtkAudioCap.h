/* 
 * Copyright (c) 2014 Realtek Semiconductor Corp. 
 */

#ifndef _AUDIOCAP_H
#define _AUDIOCAP_H

#include <OSAL_RTK.h>
#include "RtkAudioRender.h"
#include "RtkBypassRender.h"
//#include "RtkAudioCommon.h"

namespace android {

#define AUIDO_SAMPLE_RATE   48000
#define KARAOKE_CHANNELS    2
#define NUMBER_OF_CHANNELS  KARAOKE_CHANNELS
#define BT_VSDB_DELAY_TIME  505
#define USB_VSDB_DELAY_TIME 100

struct FRAME
{
    unsigned char *bufferAddr;
    unsigned long bufferSize;
    unsigned long frameSize;
    long long timeStamp;
};

class RtkAudioCap : public AHandler
{
public:
    RtkAudioCap();

    enum {
        kWhatPrepare = 0x2000,
        kWhatAudioRead,
        kWhatStop,
        kWhatFlush,
        kWhatDelay
    };

protected:
    virtual ~RtkAudioCap();
    virtual void onMessageReceived(const sp <AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        PREPARED,
        STARTED,
        STOPPED,
    };

    enum CAP_State
    {
        CAP_STATE_WAIT,
        CAP_STATE_GET_FRAME,
        CAP_STATE_EOS,
        CAP_STATE_ERROR = -1
    };

    enum DEVICE_CONNECTION
    {
        DEVICE_CONNECT_USB,
        DEVICE_CONNECT_BT
    };

    State mState;
    int32_t mAOAgentId;
    int32_t mChannels;
    int32_t mSampleRate;
    int32_t mDelay;
    int32_t mDeviceMode;
    FRAME mAudioFrame;

    OSAL_RTKALLOCATOR* mAOcapBuffer[NUMBER_OF_CHANNELS];
    unsigned char *mAOcapLower[NUMBER_OF_CHANNELS];
    unsigned char *mAOcapUpper[NUMBER_OF_CHANNELS];

    status_t prepare();
    status_t startCap();
    status_t flush();
    int32_t getFrame(FRAME *frame);
    status_t stopCap();
    status_t setDelay();

    DISALLOW_EVIL_CONSTRUCTORS(RtkAudioCap);
};

} // namespace android

#endif //_AUDIOCAP_H

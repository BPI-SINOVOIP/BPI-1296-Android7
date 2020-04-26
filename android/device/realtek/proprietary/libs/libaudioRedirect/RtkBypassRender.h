/* 
 * Copyright (c) 2014 Realtek Semiconductor Corp. 
 */

#ifndef _BYPASSRENDER_H
#define _BYPASSRENDER_H

#include "RtkAudioCommon.h"

namespace android {

#define MAX_AUDIO_SOUND_CARDS 8
#define BYTES_PER_SAMPLE      2

class RtkBypassRender : public AHandler
{
public:
    RtkBypassRender();

    enum {
        kWhatStartBypass = 0x4000,
        kWhatBufferBypass,
        kWhatCardMute,
        kWhatCardUnMute,
        kWhatStopBypass
    };

protected:
    virtual ~RtkBypassRender();
    virtual void onMessageReceived(const sp <AMessage> &msg);

private:
    enum PCM_State {
        PCM_UNINITIALIZED,
        PCM_PREPARED,
        PCM_STARTED,
        PCM_STOPPED,
    };

    PCM_State mState;

    struct cardManager {
        struct pcm* mPcmHandle;
        int32_t mRate;
        //int32_t mChannels;
        int32_t mMute;
        int32_t mAvail;
        struct resampler_itfe *mResampler;
    };

    cardManager mAudioCards[MAX_AUDIO_SOUND_CARDS];

    int32_t mInputChannels;
    int32_t mInputSampleRate;

    enum {
        DEVICE_CHECK,
        DEVICE_CONNECTED
    };

    status_t checkAvailCard(int32_t card, int32_t connectState);
    status_t getPcmHandle(int32_t card);
    void doStop(int32_t card);
    void releaseCard(int32_t card);
    void stopPcmRender();

    DISALLOW_EVIL_CONSTRUCTORS(RtkBypassRender);
};

} // namespace android

#endif //_BYPASSRENDER_H
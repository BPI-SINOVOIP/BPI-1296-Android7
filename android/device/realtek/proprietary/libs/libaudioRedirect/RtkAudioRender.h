/* 
 * Copyright (c) 2014 Realtek Semiconductor Corp. 
 */

#ifndef _AUDIORENDER_H
#define _AUDIORENDER_H

#include "RtkAudioCommon.h"

namespace android {

class RtkAudioRender : public AHandler
{
public:
    RtkAudioRender();

    enum {
        kWhatStart = 0x3000,
        kWhatFlushBuffer,
        kWhatQueueBuffer,
        kWhatStop
    };

protected:
    virtual ~RtkAudioRender();
    virtual void onMessageReceived(const sp <AMessage> &msg);

private:
    enum AO_State {
        AO_UNINITIALIZED,
        AO_STARTED,
        AO_STOPPED,
    };

    AO_State mState;
    int32_t mChannels;
    int32_t mSampleRate;

    sp<AudioTrack> mAudioTrack;
    List< sp<ABuffer> > mAudioQueue;
    Mutex mAQueueLock;

    void setupAudioTrack();
    static void audioCallback(int event, void *user, void *info);
    void renderAudioCb(int event, void *info);

    DISALLOW_EVIL_CONSTRUCTORS(RtkAudioRender);
};

} // namespace android

#endif //_AUDIORENDER_H

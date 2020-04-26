/* 
 * Copyright (c) 2014 Realtek Semiconductor Corp. 
 */

#ifndef _AUDIOREDIRECT_H
#define _AUDIOREDIRECT_H

namespace android {

struct ABuffer;
struct ALooper;
struct AHandler;

class RtkAudioCap;

class RtkAudioRedirect : public AHandler
{
public:
    RtkAudioRedirect();

    status_t startAudioCap(sp<AHandler> handleId);
    status_t stopAudioCap(sp<AHandler> handleId);
    status_t startAudioRender(sp<AHandler> handleId);
    status_t flushAudioRender(sp<AHandler> handleId);
    status_t stopAudioRender(sp<AHandler> handleId);
    status_t setAudioMode(sp<AHandler> handleId, int mode);

    status_t startBypassRender(sp<AHandler> handleId);
    status_t cardMute(sp<AHandler> handleId, int card);
    status_t cardUnMute(sp<AHandler> handleId, int card);
    status_t stopBypassRender(sp<AHandler> handleId);

    enum {
        kWhatRedirectStartCap = 0x1000,
        kWhatRedirectStopCap,
        kWhatRedirectStartRender,
        kWhatRedirectFlushRender,
        kWhatRedirectStopRender,
        kWhatRedirectSetMode,
        kWhatRedirectStartBypass,
        kWhatRedirectMute,
        kWhatRedirectUnMute,
        kWhatRedirectStopBypass,
    };

protected:
    virtual ~RtkAudioRedirect();
    virtual void onMessageReceived(const sp <AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        STARTED,
        STOPPED
    };

    State mState;
    State mRenderState;
    sp<RtkAudioCap> mAudioCap;
    sp<ALooper> mAudioCapLooper;
    int32_t mAudioMode;

    status_t startCap();
    status_t stopCap();
    status_t startRender();
    status_t flushRender();
    status_t stopRender();
    status_t setMode();
    status_t startBypass();
    status_t bypassCardMute(int32_t card);
    status_t bypassCardUnMute(int32_t card);
    status_t stopBypass();
    
    DISALLOW_EVIL_CONSTRUCTORS(RtkAudioRedirect);
};

} // namespace android

#endif //_AUDIOREDIRECT_H

/* 
 * Copyright (c) 2014 Realtek Semiconductor Corp. 
 */

#ifndef _AUDIOREDIRECTSERVICE_H
#define _AUDIOREDIRECTSERVICE_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

struct ABuffer;
struct ALooper;
struct AHandler;

class RtkAudioRedirect;

class AudioRedirectService: public BBinder {
public:
    AudioRedirectService();
    virtual ~AudioRedirectService(); 
 
    static void instantiate();
    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags=0);
private:
    status_t startAudioCap();
    status_t stopAudioCap();
    status_t startAudioRender();
    status_t flushAudioRender();
    status_t stopAudioRender();
    status_t setAudioMode(uint32_t mode);

    sp<RtkAudioRedirect> mAudioRedirect;
    sp<ALooper> mAudioRedirectLooper;
    uint32_t mRenderClient;
    uint32_t mState;

    mutable Mutex mLock;
};

} // namespace android

#endif //_AUDIOREDIRECTSERVICE_H

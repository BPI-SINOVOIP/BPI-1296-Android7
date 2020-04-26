/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _AUDIOSOURCEFILTER_H_
#define _AUDIOSOURCEFILTER_H_

#include "RtkTranscodeCommon.h"
#include "RtkFilter.h"

namespace android {

struct ALooper;
struct AHandler;

class AudioSourceFilter : public RtkFilter
{
public:
    AudioSourceFilter() { };
    enum {
        /* Player --> AudioSourceFilter */
        kWhatSetAudioSource = 0x500,
        kWhatConfigure,
        kWhatPrepare,
        kWhatSetPlayback,
        kWhatSetEncode,
        kWhatStart,
        kWhatNotifyHDMIRxAudioHotplug,
        kWhatStop = WHAT_STOP,
    };

protected:
    DISALLOW_EVIL_CONSTRUCTORS(AudioSourceFilter);
};

} // namespace android

#endif //_AUDIOSOURCEFILTER_H_

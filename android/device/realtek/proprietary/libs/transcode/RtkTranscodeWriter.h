/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */

#ifndef __RTKTRANSCODEWRITER_H__
#define __RTKTRANSCODEWRITER_H__

#include <binder/IMemory.h>

#include "RtkTranscodeCommon.h"

namespace android {

struct ABuffer;
struct ALooper;
struct AHandler;
struct MediaWriter;
struct MediaSource;
class MetaData;

struct RtkFilter;

struct RtkTranscodeWriter : public RtkFilter
{
public:
    RtkTranscodeWriter();

    enum {
        /* Self */

        /* Encoder --> Writer */
        kWhatNewSource = 0x400,         //add new output source
        kWhatStart,
        kWhatFormatUpdate,              //notify output format metadata
        kWhatDataArrives,               //notify output data arrives
        kWhatStop = WHAT_STOP,

        /* Player --> Writer */
        kWhatSetOutputFormat = 0x410,
    };

protected:
    virtual ~RtkTranscodeWriter();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        CONFIGURED,
        PREPARED,
        STARTED,
        STOPPED,
    };

    State mState;

    class WriterSource;

    const sp<const AHandler>     mStatusHandler;
    const sp<const AHandler>     mSourceHandler;

    RtkTranscodeConf        *mOutputConfig;

    sp<MediaWriter>         mWriter;

    Vector< sp<WriterSource> >  mSourceList;
    void resetVariables();
    status_t onSetupMuxer();
    void onFormatUpdate(const sp<AMessage> &msg);

    DISALLOW_EVIL_CONSTRUCTORS(RtkTranscodeWriter);
};

} // namespace android

#endif //__RTKTRANSCODEWRITER_H__

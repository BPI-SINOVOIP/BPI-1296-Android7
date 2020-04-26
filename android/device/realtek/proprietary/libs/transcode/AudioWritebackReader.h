/*
 * Copyright (c) 2017 Realtek Semiconductor Corp.
 */
#ifndef _AUDIOWRITEBACKREADER_H_
#define _AUDIOWRITEBACKREADER_H_

#include <utils/Log.h>
#include <OSAL_RTK.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "AudioRPCBaseDS.h"
#include "RPCBaseDS.h"
#include "AudioRPC_Agent.h"
#include "AudioRPC_System.h"
#include "SVCFunc.h"
#include "RPCProxy.h"
#include "hresult.h"
#include "AudioInbandAPI.h"
#include "audio_common.h"
#include "RtkTranscodeCommon.h"
#include "AudioReader.h"

namespace android {

#define AUDIO_OUTPUT_PIN 2
#define NUMBER_OF_CHANNELS  2

struct ALooper;
struct AHandler;

class AudioWritebackReader : public RtkFilter
{
public:
    AudioWritebackReader();
    //void setDecoderAgentId(uint32_t id) { mDecoderAgentId = id; };

    enum {
        /* Player --> AudioWritebackReader */
        kWhatConfigure = 0x500,
        kWhatPrepare,
        kWhatAudioRead,
        kWhatStop = WHAT_STOP,
    };

protected:
    virtual ~AudioWritebackReader();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        CONFIGURED,
        PREPARED,
        STARTED,
        STOPPED,
    };

    enum ReaderState
    {
        READER_STATE_WAIT,
        READER_STATE_GET_FRAME,
        READER_STATE_EOS,
        READER_STATE_ERROR = -1
    };

    State mState;

    uint32_t mAOAgentId;
    AUDIO_CONFIG mAudioConfig;
    FRAME mAudioFrame;
    AUDIO_INFO_PCM_FORMAT mPCM_Format;

    OSAL_RTKALLOCATOR* mReaderDataBuffer[AUDIO_OUTPUT_PIN];
    unsigned char *mDataLower[AUDIO_OUTPUT_PIN];
    unsigned char *mDataUpper[AUDIO_OUTPUT_PIN];

    OSAL_RTKALLOCATOR* mReaderPtsBuffer;
    unsigned char *mPtsLower;
    unsigned char *mPtsUpper;
    void prepare();
    status_t read();
    void stopRead();
    int32_t getFrame(FRAME *frame);

    DISALLOW_EVIL_CONSTRUCTORS(AudioWritebackReader);
};

} // namespace android

#endif //_AUDIOWRITEBACKREADER_H_

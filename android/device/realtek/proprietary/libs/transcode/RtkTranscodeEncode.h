/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _RTK_TRANSCODE_ENCODE_H_
#define _RTK_TRANSCODE_ENCODE_H_
#include <binder/IMemory.h>

#include "RtkTranscodeCommon.h"
#include <media/stagefright/ACodec.h>

namespace android {

struct ABuffer;
struct ALooper;
struct ACodec;
struct MediaWriter;
struct MediaSource;
class MetaData;

struct RtkFilter;

struct RtkTranscodeEncode : public RtkFilter
{
public:
    RtkTranscodeEncode();
    status_t start();
    status_t stop();
    status_t reset();
    bool EOSReached();
    status_t stopTranscoding();

    enum {
        /* Self */
        kWhatStop = WHAT_STOP,

        /* Player --> Encoder */
        kWhatSetOutputFile = 0x310,
        kWhatDebugBuffers,

        /* Decoder --> Encoder */
        kWhatFillBufferDone = 0x320,
        kWhatStartTranscoding,          /* start transcode by dynamic start or initial */
        kWhatStopTranscoding,           /* stop transcode by dynamic stop */
        kWhatSetupEncoder,              /* created encoder */

        /* ACodec --> Encoder */
        kWhatACodecNotify = 0x330,

        /* Writer --> Encoder */
        kWhatDrainBufferDone = 0x340,
    };

protected:
    virtual ~RtkTranscodeEncode();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        PREPARED,
        STOPPED,
        STARTED
    };

    struct CodecState
    {
        sp<ACodec> mCodec;

        sp<ACodec::PortDescription> mInputPortDesc;
        sp<ACodec::PortDescription> mOutputPortDesc;

        uint32_t mCodecType;
        sp<MetaData> mOutputFormat;
        int64_t mLastPts;
        sp<ALooper> mLooper;
        int32_t sink;
        Statistics mStat;
    };

    int32_t mWidth;
    int32_t mHeight;
    int32_t mStride;
    int32_t mSliceHeight;
    int32_t mInterlaceMode;

    int32_t mSampleRate;
    int32_t mChannelCount;

    bool mEOSReached;
    bool mTranscode;
    bool mDebug;
    uint32_t mMemberCount;
    uint32_t mReplyId;

    State mState;

    uint32_t mNumFramesWritten;
    Mutex mAQueLock;

    sp<MediaWriter> mWriter;

    KeyedVector<size_t, CodecState> mACodecState;

    KeyedVector< int32_t, sp<AMessage> > mVideoMap;
    KeyedVector< int32_t, sp<AMessage> > mAudioMap;

    int64_t mStartTimeRealUs;

    status_t onStop();
    status_t onDebugBuffers();
    status_t onStartTranscoding();
    void onSetupEncoder(const sp<AMessage> &msg);
    void createEncoder(int32_t codecType, const sp<AMessage> &srcFormat);
    status_t onSetupMuxer();
    void onStopTranscoding();
    void StopWriter();
    void FlushACodec();
    void ShutdownACodec();
    void StopACodecBuffer();

    void onFillThisBufferStart(const sp<AMessage> &msg);
    void onFillThisBufferDone(const sp<AMessage> &msg);
    void onDrainThisBuffer(const sp<AMessage> &msg);
    void onOutputFormatChanged(const sp<AMessage> &msg);

    void resetVariables();

    DISALLOW_EVIL_CONSTRUCTORS(RtkTranscodeEncode);
};

} // namespace android
#endif /* end of not define _RTK_TRANSCODE_ENCODE_H_ */

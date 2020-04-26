/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _RTK_TRANSCODE_DECODE_H_
#define _RTK_TRANSCODE_DECODE_H_

#include <binder/IMemory.h>

#include "RtkTranscodeCommon.h"

namespace android {

struct ABuffer;
struct ALooper;
struct AudioTrack;
struct IGraphicBufferProducer;
struct ACodec;
struct NativeWindowWrapper;
struct MediaSource;

class MediaExtractor;
class MetaData;

struct RtkFilter;

struct RtkTranscodeDecode : public RtkFilter
{
public:
    RtkTranscodeDecode();

    enum {
        /* Player --> Decoder */
        kWhatSetPlayback = 0x200,
        kWhatDebugBuffers,
        kWhatCreateDecoder,
        kWhatSetSurface = WHAT_SET_SURFACE,
        kWhatStop = WHAT_STOP,
        //Note: max 16 value
        
        /* Decoder --> TranscodeManager */
        kWhatDrainBufferDone = 0x220,

        /* Reader --> Decoder */
        kWhatFillBufferDone = 0x230,

        /* ACodec --> Decoder */
        kWhatACodecNotify = 0x240,          /* Notification from ACodec */

        /* Self */
        //kWhatStopEOS = 0x250,               /* Detect EOS or Err from ACodec */

        
    };

protected:
    virtual ~RtkTranscodeDecode();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        PREPARED,
        STOPPING,
        STOPPED,
        STARTED
    };

    struct CodecState
    {
        sp<ACodec> mCodec;
        size_t mTrackIndex;
        uint32_t mCodecType;
        int64_t mLastPts;
        sp<MetaData> mFormat;
        sp<ALooper> mLooper;
        Statistics mStat;
    };

    bool mDebug;
    bool mRenderVideo;
    bool mRenderAudio;

    uint32_t mMemberCount;
    uint32_t mDecodeDone;
    int32_t mWidth;
    int32_t mHeight;

    State mState;
    sp<Surface> mNativeWindow;

    sp<AudioTrack> mAudioTrack;
    List< sp<ABuffer> > mAudioQueue;

    uint32_t mNumFramesWritten;
    Mutex mAQueLock;

    Vector<CodecState *> mACodecState;
    KeyedVector< int32_t, sp<AMessage> > mOutputVideoMap;
    KeyedVector< int32_t, sp<AMessage> > mOutputAudioMap;
    KeyedVector< int32_t, sp<AMessage> > mInputAudioMap;
    KeyedVector< int32_t, sp<AMessage> > mInputVideoMap;

    int64_t mStartTimeRealUs;

    status_t onStart();
    status_t onOutputFormatChanged(size_t trackIndex, CodecState *state);
    status_t onDebugBuffers();
    void onStopEOS(int err);
    void StopACodecBuffer();
    void continueStop();

    status_t setupDecoder(size_t trackIndex, sp<AMessage> &format, uint32_t sourceType);
    status_t onCreateDecoder(const sp<AMessage> &msg);

    void onDrainThisBuffer(const sp<AMessage> &msg);
    void onOutputFormatChanged(const sp<AMessage> &msg);

    void resetVariables();
    void onFillThisBuffer(const sp<AMessage> &msg);
    void onDecodeAudioDone(const sp<AMessage> &msg);
    static void audioCallback(int event, void *user, void *info);
    void renderAudioCb(int event, void *info);
    size_t renderAudio(uint8_t *data, size_t size);
    void StopAudioRender();
    void releaseDecoderOutputBuffer();

    DISALLOW_EVIL_CONSTRUCTORS(RtkTranscodeDecode);
};

} // namespace android
#endif /* end of not define _RTK_TRANSCODE_DECODE_H_ */

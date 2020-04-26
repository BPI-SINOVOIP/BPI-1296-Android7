/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _RTK_TRANSCODE_FLOW_MANAGER_H_
#define _RTK_TRANSCODE_FLOW_MANAGER_H_

#include <binder/IMemory.h>
#include <hardware/gralloc.h>

#include "RtkIpc/ISeService.h"
#include "RtkTranscodeCommon.h"
#include <audio_utils/resampler.h>
#include <hardware/HardwareAPI.h>
#include <native_handle.h>

namespace android {

struct ABuffer;
struct ALooper;
struct AudioTrack;
struct IGraphicBufferProducer;

class MediaExtractor;
class MetaData;

struct RtkFilter;

struct RtkTranscodePostProcessor : public RtkFilter
{
public:
    RtkTranscodePostProcessor(bool doScaler = false);
    status_t start();
    status_t stop();
    status_t reset();

    enum {
        /* Player --> PostProcessor */
        kWhatDebugBuffers = 0x600,
        kWhatStartTranscoding,
        kWhatStopTranscoding,
        kWhatConfigure,
        kWhatFreeAllocBuffer,
        //Note: max 16 value
        
        /* Decoder --> PostProcessor */
        kWhatDrainBuffer = 0x610,
        kWhatAllocWriteBackBuffer,

        /* Encoder --> PostProcessor */
        kWhatFillBuffer = 0x620,
        kWhatBufferCount,

        /* Self */
    };

protected:
    virtual ~RtkTranscodePostProcessor();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        PREPARED,
        STOPPING,
        STOPPED,
        STARTED
    };

    int32_t mWidth;
    int32_t mHeight;
    int32_t mYUVSize;
    int32_t mBufferCount;
    int32_t mInterlaceMode;

    typedef struct
    {
        unsigned long physaddr;
        unsigned long *virtaddr;
        uint32_t bufferId;
        uint32_t size;
        uint32_t bufferHandle;
        int64_t pts;
        uint32_t aspect_h;
        uint32_t aspect_l;
        uint32_t vui_frame_rate;
        unsigned char *pLockaddr;
    }VideoBuffer ;
    KeyedVector<unsigned int, VideoBuffer*> mBufferQueue;

    bool mTranscode;
    bool mDebug;

    uint32_t mMemberCount;
    uint32_t mDecodeDone;

    State mState;
#ifdef SUPPORT_SE
    bool mCheckSE;
    unsigned long mSeHandle;
    //sp<ISeService> mSeService;
#endif
    bool mScaler;

    int32_t mIonFd;

    uint32_t mNumFramesWritten;

    struct resampler_itfe *mResampler;
    AV_FORMAT_CONFIG mTargetFormat;

    List< sp<AMessage> > mVideoDecodeOutputMessage;
    List< sp<AMessage> > mVideoEncodeInputMessage;
    List< sp<AMessage> > mAudioDecodeOutputMessage;
    List< sp<AMessage> > mAudioEncodeInputMessage;

#ifdef SUPPORT_SE
    List< VideoBuffer * > mVideoRawDataEmptyBuffer;
    List< VideoBuffer * > mVideoRawDataFullBuffer;
    KeyedVector< int32_t, VideoBuffer*> mVideoFillBufferMap;
    int64_t mTotalTime;
    int32_t mTotalCount;
#endif /* end of SUPPORT_SE */

    KeyedVector< int32_t, sp<AMessage> > mVideoRawDataMap;

    int64_t mStartTimeRealUs;
    status_t onStart();
    status_t onDebugBuffers();
    void onStopEOS(int err);
    void continueStop();

    void onDrainThisBuffer(const sp<AMessage> &msg);

    void resetVariables();

    void onDecodeVideoDone(const sp<AMessage> &msg);
    void onDecodeAudioDone(const sp<AMessage> &msg);

    void onFillEncoderAudioData(const sp<AMessage> &msg);
    void onFillEncoderVideoData(const sp<AMessage> &msg);

    void onStopTranscoding();
    void onSetupTranscoding();

#ifdef SUPPORT_SE
    bool onCheckVideoYUVtoSeScaler(int32_t whereComeFrom);
    bool onCheckVideoBufferToEncode(int32_t whereComeFrom);
    void releaseDecoderOutputBuffer(const sp<AMessage> &msg, VideoBuffer *scalerBuffer);
    void onFreeVideoBuffer(bool freeAll);
#endif /* end of SUPPORT_SE */
    bool onCheckAudioResample(const sp<ABuffer>&srcBuffer, sp<ABuffer>*destBuffer);
    void onCheckVideoMessage(int32_t whereComeFrom);
    void onCheckAudioMessage(int32_t whereComeFrom);
    void onFillThisBufferByDecoder(const sp<AMessage> &msg);
    void releaseDecoderOutputBuffer(const sp<AMessage> &msg, sp<AMessage> &reply);
    status_t onCreateGraphBuffer(const sp<AMessage> &msg);

    DISALLOW_EVIL_CONSTRUCTORS(RtkTranscodePostProcessor);
    #define WBINBAND_ENTRYNUM 10
    bool mEnbWBBufferFlg;
    bool mkWhtAlcWBBufFlg;
    unsigned long *mWBvirtaddr;
    unsigned long mWBphysaddr;
    uint32_t mWBSize;
    uint32_t mWBionHandle;
    uint32_t mWBbufnum;
    void onFreeVideoBufferForWB(bool freeAll);
    status_t onCreateGraphBufferForWBBuffer(const sp<AMessage> &msg, int width, int height);
};

} // namespace android
#endif /* end of not define _RTK_TRANSCODE_FLOW_MANAGER_H_ */

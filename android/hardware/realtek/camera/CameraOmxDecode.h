/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _RTK_CAMERA_OMX_DECODE_H_
#define _RTK_CAMERA_OMX_DECODE_H_

#include <utils/Log.h>
#include <utils/KeyedVector.h>
#include <utils/Statistics.h>
#include <utils/Timers.h>
#include <binder/IMemory.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>

#include <hardware/camera.h>

#define DEBUG_INTERVAL
#ifdef DEBUG_INTERVAL
#define INTERVAL(cond, min, tprev)  do{ \
        nsecs_t t = systemTime(); \
        nsecs_t interval = t - tprev; \
        if((cond) && (interval > min)) ALOGD("\033[0;33m%d: %.3f - %.3f = %.3fms\033[m", __LINE__, t/1000000.0, tprev/1000000.0, interval/1000000.0); \
        tprev = t; \
    }while(0)
#else
#define INTERVAL(tprev)  do{ }while(0)
#endif
//#define DEBUG_CODEC_BUFFERS
#ifdef DEBUG_CODEC_BUFFERS
#define DBGBUF(flag, ...)     do{ if((((unsigned int)flag) & debugBufferFlag) == ((unsigned int)flag)) ALOGD(__VA_ARGS__); }while(0)
#else
#define DBGBUF(flag, ...)     do{ }while(0)
#endif

#define FROM_ACODEC 0x0
#define FROM_CAMERA 0x1
#define CHECK_COMEFROM(a)  ((a == FROM_ACODEC) ? "Decode" : "Camera")

/* For Encoder, Decoder Used */
#define ABUFFER_META_TIMESTAMP      "timeUs"
#define ABUFFER_ACODE_BUFFER        "buffer"
#define AMESSAGE_ACODE_REPLY        "reply"
#define AMESSAGE_ACODE_ERR_STATUS   "err"
#define AMESSAGE_CHECK_FROM         "from"
#define AMESSAGE_ACODE_RENDER       "render"
#define AMESSAGE_ENCODE_TYPE        "type"
#define AMESSAGE_OBJ_META           "meta"
#define AMESSAGE_POINTER_BUFFER_ID  "buffer-id"
#define AMESSAGE_RESPONSE_REPLYID   "replyID"
#define AMESSAGE_ACODE_WHAT         "what"
#define AMESSAGE_SET_TRANSCODE      "transcode"
#define AMESSAGE_SET_RENDER_VIDEO   "video"
#define AMESSAGE_SET_DEBUG          "debug"
#define AMESSAGE_CONFIG             "config"
#define AMESSAGE_SINK               "sink"
#define AMESSAGE_MSG                "msg"
#define AMESSAGE_READER_SOURCE      "format"
#define AMESSAGE_ONLY_ONE_NOT       "only"
#define AMESSAGE_CHECK_SOURCE_DONE  "Allf"      // All source finished
#define AMESSAGE_PHYS_ADDR          "phys"
#define AMESSAGE_VIRT_ADDR          "virt"
#define AMESSAGE_SIZE               "size"
#define AMESSAGE_AGENT              "agent"

/* For Setup decoder / encoder and FormatChange */
#define AMESSAGE_MIME               "mime"
#define AMESAAGE_CREATE_ENCODER     "encoder"
#define AMESSAGE_BITRATE            "bitrate"
#define AMESSAGE_VIDEO_MSG          "vmsg"
#define AMESSAGE_VIDEO_METADATA     "vMeta"
#define AMESSAGE_VIDEO_TRACK_INDEX  "vtrack"
#define AMESSAGE_VIDEO_INDEX_TYPE   "vtype"
/* Video Parameter */
#define AMESSAGE_VIDEO_WIDTH        "width"
#define AMESSAGE_VIDEO_HEIGHT       "height"
#define AMESSAGE_VIDEO_STRIDE       "stride"    // Actually, It is described output width
#define AMESSAGE_VIDEO_SLICE_HEIGHT "slice-height"    // Actually, It is described output height
#define AMESSAGE_VIDEO_COLOR_FORMAT "color-format"
#define AMESSAGE_VIDEO_CROP         "crop"
#define AMESSAGE_VIDEO_FRAMERATE    "frame-rate"
#define AMESSAGE_VIDEO_INTERLACED   "interlaced"
#define AMESSAGE_VIDEO_VOWRITE_BACK "YUV422"
#define AMESSAGE_VIDEO_ASPECTRATIO_HIGH "arh"
#define AMESSAGE_VIDEO_ASPECTRATIO_LOW  "arl"

/* Got Decoder Buffer_Handle_t */
#define AMESSAGE_VIDEO_BUFFER_HANDLE_T  "handle_t"

#define ALIGN(x, y)     ((x + y - 1) & ~(y - 1))
#define ALIGN16(x)      ALIGN(x, 16)

namespace android {

struct ABuffer;
struct ALooper;
struct ACodec;

class MetaData;

struct CameraDecode : public AHandler
{
public:
    CameraDecode(int pixfmt, ANativeWindow * nativeWindow, int width, int height, int size);

    enum {
        /* Camerahardware --> CameraOmxDecode */
        kWhatStop = 0x200,
        kWhatStart,
        kWhatDebugBuffers,
        kWhatSetSurface,
        kWhatCreateDecoder,
        
        kWhatDecodeBuffer = 0x220,                /* CamearHardware send buffer to decode */
        kWhatGetBuffer,                           /* CamearHardware get buffer from decode */
        kWhatNoMoreData,                          /* CameraHardware want do stopPreview */
        /* ACodec --> CameraOmxDecode */
        kWhatACodecNotify = 0x230,                /* Notification from ACodec */


        //kWhatStopEOS = 0x250,               /* Detect EOS or Err from ACodec */
    };
    int32_t mCountDown;

protected:
    virtual ~CameraDecode();

    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum State {
        UNINITIALIZED,
        STOPPING,
        STOPPED,
        STARTED
    };

    int32_t mType;
    sp<ACodec> mCodec;
    sp<ALooper> mLooper;
    sp<MetaData> mFormat;
    Statistics mStat;
    sp<AMessage> mStopMsg; 
    ANativeWindow *mNativeWindow;

    int32_t mWidth;
    int32_t mHeight;
    int32_t mSize;
    unsigned long mSeHandle;

    State mState;

    uint32_t mNumFramesWritten;

    List< sp<AMessage> > mFromCameraInputMessage;
    List< sp<AMessage> > mFromCameraOutputMessage;
    List< sp<AMessage> > mFromACodecInputMessage;
    List< sp<AMessage> > mFromACodecOutputMessage;

    int64_t mStartTimeRealUs;

    status_t onStart();
    void onStop();
    void StopACodecBuffer();

    status_t createDecoder();

    void onFillThisBuffer(const sp<AMessage> &msg);
    void onDrainThisBuffer(const sp<AMessage> &msg);
    void onOutputFormatChanged(const sp<AMessage> &msg);
    void onCheckInputMessage(const sp<AMessage> &msg, int comeFrom);
    void onCheckOutputMessage(const sp<AMessage> &msg, int comeFrom);
    void onHandleBufferReturn(const sp<AMessage> &msg, bool option);

    void releaseDecoderBuffer();

    DISALLOW_EVIL_CONSTRUCTORS(CameraDecode);
};

static inline status_t PostAndAwaitResponse(const sp<AMessage> &msg, sp<AMessage> *response)
{
    status_t err = msg->postAndAwaitResponse(response);

    if (err != OK)
        return err;
    if (!(*response)->findInt32(AMESSAGE_ACODE_ERR_STATUS, &err))
        err = OK;
    return err;
}

static inline void ReplyResponse(const sp<AMessage> &msg, const sp<AMessage> &response)
{
    sp<AReplyToken> replyID;
    CHECK(msg->senderAwaitsResponse(&replyID));
    response->postReply(replyID);
}

static inline void ReplyErrorStatus(const sp<AMessage> &msg, int32_t err)
{
    sp<AMessage> response = new AMessage;
    response->setInt32(AMESSAGE_ACODE_ERR_STATUS, err);
    ReplyResponse(msg, response);
}

} // namespace android
#endif /* end of not define _RTK_CAMERA_OMX_DECODE_H_ */

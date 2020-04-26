/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef __RTK_TRANSCODE_COMMON_H__H_

#define __RTK_TRANSCODE_COMMON_H__H_

#include <utils/Log.h>
#include <utils/KeyedVector.h>
#include <utils/Statistics.h>
#include <utils/Timers.h>
#include <utils/Trace.h>
#include <binder/IMemory.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>

#undef ATRACE_TAG
#define ATRACE_TAG ATRACE_TAG_VIDEO

#define CREATE_ENCODER
#define SUPPORT_SE
#ifdef SUPPORT_SE
#define USED_SCALER
#define USE_SE_SERVICE
#endif /* end of SUPPORT_SE */

#define DTRANSCODE_VOWRITE_BACK_SHARE_BUFFER 0x08

//#define DEBUG_INTERVAL
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

#define UNUSED(x)           (void)(x)
#define ISVIDEO(state)      (state->mCodecType & CODEC_VIDEO)
#define ISAUDIO(state)      (state->mCodecType & CODEC_AUDIO)
#define ISDECODER(state)    (state->mCodecType & CODEC_DECODER)
#define ISENCODER(state)    (state->mCodecType & CODEC_ENCODER)
#define CODECTYPE(state)    ISVIDEO(state) ? (ISENCODER(state) ? "VEnc" : "VDec") : (ISENCODER(state) ? "AEnc" : "ADec")
#define CHECK_TYPE_VIDEO(member)      (member & CODEC_VIDEO)
#define CHECK_TYPE_AUDIO(member)      (member & CODEC_AUDIO)
#define CHECK_TYPE_DECODER(member)    (member & CODEC_DECODER)
#define CHECK_TYPE_ENCODER(member)    (member & CODEC_ENCODER)
#define CHECK_TYPE_WRITER(member)     (member & CODEC_WRITER)
#define CHECK_TYPE_READER(member)     (member & CODEC_READER)
#define CONTROLSTATUS(x)    x ? "Enable" : "Disable"
#define MEDIATYPE(x)        CHECK_TYPE_VIDEO(x) ? "V" : "A"
#define COMPONENT(x)        CHECK_TYPE_DECODER(x) ? "Dec" : CHECK_TYPE_ENCODER(x) ? "Enc" : CHECK_TYPE_READER(x) ? "Src" : "Wri"
#define CODECATTR(x)        CHECK_TYPE_VIDEO(x) ? \
                                (CHECK_TYPE_DECODER(x) ? "VDec" : (CHECK_TYPE_READER(x) ? "VSrc" : (CHECK_TYPE_ENCODER(x) ? "VEnc" : "VWri"))) : \
                                (CHECK_TYPE_DECODER(x) ? "ADec" : (CHECK_TYPE_READER(x) ? "ASrc" : (CHECK_TYPE_ENCODER(x) ? "AEnc" : "AWri")))
#define CHECK_FROM_AGENT(x) (x & FROM_AGENT)
#define DATAFROM(x)         CHECK_FROM_AGENT(x) ?  "Agent" : "ACodec"

//#define DEBUG_CODEC_BUFFERS
#ifdef DEBUG_CODEC_BUFFERS
#define DBGBUF(flag, ...)     do{ if((((unsigned int)flag) & debugBufferFlag) == ((unsigned int)flag)) ALOGD(__VA_ARGS__); }while(0)
#else
#define DBGBUF(flag, ...)     do{ }while(0)
#endif


#define AMESSAGE_SOURCE_INDEX_TYPE           "source-index"
#define AMESSAGE_TRACK_INDEX_OF_SOURCE       "track-index"
/* For Encoder, Decoder Used */
#define ABUFFER_META_TIMESTAMP      "timeUs"
#define ABUFFER_ACODE_BUFFER        "buffer"
#define AMESSAGE_ACODE_REPLY        "reply"
#define AMESSAGE_ACODE_ERR_STATUS   "err"
#define AMESSAGE_CHECK_FROM         "from"
#define AMESSAGE_ACODE_RENDER       "render"
#define AMESSAGE_ENCODE_TYPE        "type"
#define AMESSAGE_OUTPUT_FORMAT      "outf"
#define AMESSAGE_OBJ_META           "meta"
#define AMESSAGE_POINTER_HANDLE     "handle"
#define AMESSAGE_POINTER_BUFFER_ID  "buffer-id"
#define AMESSAGE_RESPONSE_REPLYID   "replyID"
#define AMESSAGE_ACODE_WHAT         "what"
#define AMESSAGE_SET_TRANSCODE      "transcode"
#define AMESSAGE_SET_RENDER_VIDEO   "video"
#define AMESSAGE_SET_RENDER_AUDIO   "audio"
#define AMESSAGE_SET_DEBUG          "debug"
#define AMESSAGE_CONFIG             "config"
#define AMESSAGE_SINK               "sink"
#define AMESSAGE_MSG                "msg"
#define AMESSAGE_READER_SOURCE      "format"
#define AMESSAGE_ONLY_TRACK         "only"
#define AMESSAGE_CHECK_SOURCE_DONE  "Allf"      // All source finished
#define AMESSAGE_PHYS_ADDR          "phys"
#define AMESSAGE_VIRT_ADDR          "virt"
#define AMESSAGE_IMEMORY_PTR        "imemptr"   // added for binder interface
#define AMESSAGE_SIZE               "size"
#define AMESSAGE_AGENT              "agent"
#define AMESSAGE_WAIT_REPLY         "waitReply"
#define AMESSAGE_NATIVE_WINDOW      "native-window"
#define AMESSAGE_SURFACE            "surface"
/* For Setup decoder / encoder and FormatChange */
#define AMESSAGE_MIME               "mime"
#define AMESAAGE_CREATE_ENCODER     "encoder"
#define AMESSAGE_BITRATE            "bitrate"
#define AMESSAGE_VIDEO_MSG          "vmsg"
#define AMESSAGE_AUDIO_MSG          "amsg"
#define AMESSAGE_VIDEO_METADATA     "vMeta"
#define AMESSAGE_AUDIO_METADATA     "aMeta"
#define AMESSAGE_VIDEO_TRACK_INDEX  "vtrack"
#define AMESSAGE_AUDIO_TRACK_INDEX  "atrack"
#define AMESSAGE_VIDEO_INDEX_TYPE   "vtype"
#define AMESSAGE_AUDIO_INDEX_TYPE   "atype"
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
#define AMESSAGE_VIDEO_VUI_FRAME_RATE   "vui-frame-rate"
#define AMESSAGE_VIDEO_ENCODE_INTERLACE_MODE "kKeyEncodeToInterlace"
#define AMESSAGE_VIDEO_SHARE_WRITE_BACK_ENABLE "WBShareEnb"
#define AMESSAGE_VIDEO_WRITE_BACK_PLOCK "pLock"
#define AMESSAGE_VIDEO_WRITE_BACK_VIRT "WBvirt"
#define AMESSAGE_VIDEO_WRITE_BACK_PHYS "WBphys"
#define AMESSAGE_VIDEO_WRITE_BACK_SIZE "WBsize"
#define AMESSAGE_VIDEO_WRITE_BACK_BUFNUM "WBbufnum"
#define AMESSAGE_VIDEO_WRITE_BACK_TYPE "WBtype"
/* Audio Parameter */
#define AMESSAGE_AUDIO_CHANNELCOUNT "channel-count"
#define AMESSAGE_AUDIO_CHANNELMODE  "channel-mode"
#define AMESSAGE_AUDIO_SAMPLERATE   "sample-rate"
#define AMESSAGE_AUDIO_CHANNELMASK  "channel-mask"
#define AMESSAGE_AUDIO_SOURCE       "audio-source"
#define AMESSAGE_AUDIO_RX_HOTPLUG   "rx-audio-hotplug"
/* Camera Parameter */
#define AMESSAGE_CAMERA_TYPE        "camera-type"
#define AMESSAGE_CAMERA_ID          "camera-id"
#define AMESSAGE_CLIENT_NAME        "client-name"
#define AMESSAGE_CAMERA_SP          "camera-sp"
#define AMESSAGE_PIXELFORMAT        "pixfmt"
#define AMESSAGE_CAMERA_PARAM       "camera-param"
#define AMESSAGE_COORDINATE_X       "x"
#define AMESSAGE_COORDINATE_Y       "y"
#define AMESSAGE_OUTPUT_WIDTH       "outw"
#define AMESSAGE_OUTPUT_HEIGHT      "outh"
#define AMESSAGE_CAM_PREVIEW_WIDTH  "campw"
#define AMESSAGE_CAM_PREVIEW_HEIGHT "camph"
/* Got Decoder Buffer_Handle_t */
#define AMESSAGE_VIDEO_BUFFER_HANDLE_T  "handle_t"

#define ALIGN(x, y)     ((x + y - 1) & ~(y - 1))
#define ALIGN16(x)      ALIGN(x, 16)
#define ALIGN32(x)      ALIGN(x, 32)

namespace android {

enum {
    CODEC_AUDIO = 1 << 0,
    CODEC_VIDEO = 1 << 1,
    CODEC_SUBTITLE = 1 << 2,

    CODEC_DECODER = 1 << 8,
    CODEC_ENCODER = 1 << 9,
    CODEC_WRITER = 1 << 10,
    CODEC_READER = 1 << 11,

    BUFFER_INPUT = 1 << 16,     //buffer of input port
    BUFFER_OUTPUT = 1 << 17,    //buffer of output port
    BUFFER_SOURCE = 1 << 18,    //source of buffer
    BUFFER_SINK = 1 << 19,      //sink of buffer
    //BUFFER_DEQUEUE = 1 << 20,
    //BUFFER_ENQUEUE = 1 << 21,

    FROM_AGENT      = 1 << 25,
    FROM_ACODE      = 1 << 26,
};

enum {
    kPortIndexInput         = 0,
    kPortIndexOutput        = 1,
};

enum {
    ERROR_ALREADY_CHANGE    =   ERROR_END_OF_STREAM - 1,    // -1012
    ERROR_NO_SOURCE         =   ERROR_END_OF_STREAM - 2,    // -1013
    ERROR_OUT_OF_MEMORY     =   ERROR_END_OF_STREAM - 3,    // -1014
};

enum {
    TRANSCODE_CHNAHE = 1 << 0,
    PLAYBACK_CHANGE = 1 << 1,
};

enum {
    OUT_FMT_TS,
    OUT_FMT_MP4,
    OUT_FMT_MAX
};

#if 0
enum {
    OUTPUT_FILE = 0,
    OUTPUT_STDOUT = 1,
    OUTPUT_ASHMEM = 2,
    OUTPUT_ASHMEM_RTP = 3,
    OUTPUT_UDP = 4,
    OUTPUT_UDP_RTP = 5,
    OUTPUT_UDP_ASHMEM = 6,  //OUTPUT_RTP_BOTH
    OUTPUT_FD = 7,
    OUTPUT_MAX
};
#endif

/* For Status reference */
enum {
    /* For common */
    kWhatEndOfStream = 0x20140221,
    /* Video relation status */
    kWhatCreateVideoDecodeFailed,
    kWhatCreateVideoEncodeFailed,
    /* Audio relation status */
    kWhatCreateAudioDecodeFailed,
    kWhatCreateAudioEncodeFailed,
    /* DataSource relation status */
    kWhatCreateDataSourceFailed,
    /* Camera status failed */
    kWhatCameraError,
};

enum {
    CAMERA_FORMAT_NV21,
    CAMERA_FORMAT_ARGB,
    CAMERA_FORMAT_JPEG,
    CAMERA_FORMAT_YV12,
};

enum {
    CHANNELMODE_NONE,
    CHANNELMODE_LEFT,
    CHANNELMODE_RIGHT,
};

enum {
    TranscodeTypeFile = 0,
    TranscodeTypeDvdPlayer,
    TranscodeTypeHdmiRx,
    TranscodeTypeUsbCamera,
    TranscodeTypeMixer1,
    TranscodeTypeV1,
    TranscodeTypeMax,
};

#define WHAT_STOP           0x20140801
#define WHAT_SET_SURFACE    0x20140802

struct RtkTranscodeConf {
    int outputType;
    int outputFormat;
    int fileDesc;
    const char *fileName;
    sp<IMemory> ashmemData;
    sp<IMemory> ashmemInfo;
    const char *udpIp;
    int udpPort;
    bool rtpHeader;
};

typedef struct _VIDEO_CONFIG {
    int32_t width;
    int32_t height;
    int32_t fps;
    int32_t bitrate;  // bit/s
    int32_t iframeInterval; // seconds
    int32_t rateControl;    // Disable/ABR/CBR/ABR+skipFrame/CBR+skipFrame
    int32_t aspectRatio;    // Disable / 16:9 / 4:3
    int32_t interlace;      // encode be progressive / interlace
    int32_t shareWBbuf;     // Enable share writeback buffer.
} VIDEO_CONFIG;

typedef struct _AUDIO_CONFIG {
    int32_t channelCount;
    int32_t channelMode;
    int32_t sampleRate; // Hz
    int32_t bitrate; // bit/s
} AUDIO_CONFIG;

typedef struct _AV_FORMAT_CONFIG {
    VIDEO_CONFIG videoFmt;
    AUDIO_CONFIG audioFmt;
} AV_FORMAT_CONFIG;

typedef struct _CAMERA_SNAPSHOT {
    int format;             //input, CAMERA_FORMAT_NV21, CAMERA_FORMAT_ARGB, CAMERA_FORMAT_JPEG
    int x;                  //input
    int y;                  //input
    int width;              //input
    int height;             //input
    int outWidth;           //input
    int outHeight;          //input
    int size;               //output
    unsigned char *data;    //output
} CAMERA_SNAPSHOT;

#define IS_TRANSCODE_MODE_CHANGE(member) (member & TRANSCODE_CHNAHE)
#define IS_PLAYBACK_MODE_CHANGE(member) (member & PLAYBACK_CHANGE)
#define ENABLE_TRANSCODE_MODE_CHANGE(member) (member |= TRANSCODE_CHNAHE)
#define ENABLE_PLAYBACK_MODE_CHANGE(member) (member |= PLAYBACK_CHANGE)
#define DISABLE_TRANSCODE_MODE_CHANGE(member) (member &= ~TRANSCODE_CHNAHE)
#define DISABLE_PLAYBACK_MODE_CHANGE(member) (member &= ~PLAYBACK_CHANGE)

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

static inline void SendErrorStatus(int msgId, int32_t err, const sp<const AHandler> &receiver, int32_t sourceIndex)
{
    sp<AMessage> msg = new AMessage(msgId, receiver);
    msg->setInt32(AMESSAGE_ACODE_ERR_STATUS, err);
    msg->setInt32(AMESSAGE_SOURCE_INDEX_TYPE, sourceIndex);
    msg->post();
}

static inline status_t PostAndAwaitResponse(const sp<AMessage> &msg, sp<AMessage> *response)
{
    status_t err = msg->postAndAwaitResponse(response);

    if (err != OK)
        return err;
    if (!(*response)->findInt32(AMESSAGE_ACODE_ERR_STATUS, &err))
        err = OK;
    return err;
}

/* Don't need check response message information */
static inline status_t SendMessageAndWait(int msgId, const sp<const AHandler> &targetId)
{
    sp<AMessage> msg = new AMessage(msgId, targetId);
    sp<AMessage> response;
    return PostAndAwaitResponse(msg, &response);
}

} // namespace android

#endif /* end of not define __RTK_TRANSCODE_COMMON_H__H_ */


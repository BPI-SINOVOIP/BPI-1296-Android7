/***********************************************************************/
/* rpc_common.h
   defines for shared enums and structures between Audio/Video/System CPUs
*/

#ifndef RPC_COMMON_H
#define RPC_COMMON_H

#include <stdint.h>

#define AUDIO_RING_LIST_MAX (16)

typedef enum _tagFirmwareStatus{
  STATUS_NONE,
  STATUS_STOP,
  STATUS_PAUSE,
  STATUS_RUN
} FIRMWARE_STATUS;

typedef enum _tagRingBufferType{
  RINGBUFFER_STREAM,
  RINGBUFFER_COMMAND,
  RINGBUFFER_MESSAGE,
  RINGBUFFER_VBI,
  RINGBUFFER_PTS,
  RINGBUFFER_DTVCC,
  RINGBUFFER_STREAM1,
  RINGBUFFER_COMMAND1,
  RINGBUFFER_MESSAGE1,
  RINGBUFFER_STREAM_BL,
  RINGBUFFER_COMMAND_BL,
  RINGBUFFER_STREAM_EL,
  RINGBUFFER_COMMAND_EL,
  RINGBUFFER_STREAM_MD,
  RINGBUFFER_COMMAND_MD,
  RINGBUFFER_STREAM_SUBES,
  RINGBUFFER_COMMAND_SUBIB
} RINGBUFFER_TYPE;

typedef enum _tagAVSyncMode{
  AVSYNC_FORCED_SLAVE,
  AVSYNC_FORCED_MASTER,
  AVSYNC_AUTO_SLAVE,
  AVSYNC_AUTO_MASTER,
  AVSYNC_AUTO_MASTER_NO_SKIP,
  AVSYNC_AUTO_MASTER_CONSTANT_DELAY,   /* after getback mastership, it become force master */
  AVSYNC_EXTREME_LOW_DELAY,
  AVSYNC_AUTO_MASTER_NO_SKIP_TIMEOUT /* hold on mastership if RCD = -1 and TIMEOUT is not expired */
} AVSYNC_MODE;

typedef enum _tagOMXFrameType{
  OMXFRAME_IDLE,
  OMXFRAME_HASFRAME,
  OMXFRAME_SETED,
  OMXFRAME_DROP,
  OMXFRAME_GETINFO
}OMX_Frame_Type;

typedef enum _tagAutoMasterState{
  AUTOMASTER_NOT_MASTER,
  AUTOMASTER_IS_MASTER
} AUTOMASTER_STATE;

struct _tagMastership {
  volatile uint8_t systemMode;  /* enum AVSYNC_MODE */
  volatile uint8_t videoMode;   /* enum AVSYNC_MODE */
  volatile uint8_t audioMode;   /* enum AVSYNC_MODE */
  volatile uint8_t masterState; /* enum AUTOMASTER_STATE */
};
typedef struct _tagMastership MASTERSHIP;

/* change this structure, make sure to change the CReferenceClock definition*/
struct _tagRefClock {
  volatile int64_t   RCD;
  volatile uint32_t  RCD_ext;
  volatile int64_t   GPTSTimeout;
  volatile int64_t   videoSystemPTS;
  volatile int64_t   audioSystemPTS;
  volatile int64_t   videoRPTS;
  volatile int64_t   audioRPTS;
  volatile uint32_t  videoContext;
  volatile uint32_t  audioContext;

  MASTERSHIP         mastership;
  volatile uint32_t  videoFreeRunThreshold;
  volatile uint32_t  audioFreeRunThreshold;
  volatile int64_t   masterGPTS;      // this is the value of GPTS (hardware PTS) when master set the reference clock
  volatile int32_t   audioFullness;   // This flag will be turned on when AE's output buffer is almost full.
                                      // VE which is monitoring this flag will issue auto-pause then.
                                      // (0: still have enough output space for encoding.   1: AE request pause)
  volatile int32_t   audioPauseFlag;  // This flag will be turned on when VE decides to auto-pause.
                                      // AE which is monitoring this flag will auto-pause itself then.
                                      // (0: ignore.  1: AE have to pauseEncode immediately)

  volatile int32_t   VO_Underflow;    // (0: VO is working properly; otherwise, VO underflow)
  volatile int32_t   AO_Underflow;    // (0: AO is working properly; otherwise, AO underflow)
  volatile int32_t   videoEndOfSegment; // set to the flow EOS.eventID by VO after presenting the EOS sample
  volatile int32_t   audioEndOfSegment; // set to the flow EOS.eventID by AO after presenting the EOS sample
  volatile uint8_t   reserved[16];
};
typedef struct _tagRefClock  REFCLOCK;


/* change this structure, make sure to change the CReferenceClock definition*/
struct _tagRefClock_debug{
  volatile int64_t   RCD;
  volatile uint32_t  RCD_ext;
  volatile int64_t   GPTSTimeout;
  volatile int64_t   videoSystemPTS;
  volatile int64_t   audioSystemPTS;
  volatile int64_t   videoRPTS;
  volatile int64_t   audioRPTS;
  volatile uint32_t  videoContext;
  volatile uint32_t  audioContext;

  MASTERSHIP         mastership;
  volatile uint32_t  videoFreeRunThreshold;
  volatile uint32_t  audioFreeRunThreshold;
  volatile int64_t   masterGPTS;      // this is the value of GPTS (hardware PTS) when master set the reference clock
  volatile int32_t   audioFullness;   // This flag will be turned on when AE's output buffer is almost full.
                                      // VE which is monitoring this flag will issue auto-pause then.
                                      // (0: still have enough output space for encoding.   1: AE request pause)
  volatile int32_t   audioPauseFlag;  // This flag will be turned on when VE decides to auto-pause.
                                      // AE which is monitoring this flag will auto-pause itself then.
                                      // (0: ignore.  1: AE have to pauseEncode immediately)

  volatile int32_t   VO_Underflow;    // (0: VO is working properly; otherwise, VO underflow)
  volatile int32_t   AO_Underflow;    // (0: AO is working properly; otherwise, AO underflow)
  volatile int32_t   videoEndOfSegment; // set to the flow EOS.eventID by VO after presenting the EOS sample
  volatile int32_t   audioEndOfSegment; // set to the flow EOS.eventID by AO after presenting the EOS sample
  volatile int32_t   memorytrashAddr;
  volatile int32_t   memorytrashContext;
  volatile uint8_t   reserved[8];
};
typedef struct _tagRefClock_debug  REFCLOCK_MEMDEBUG;





// Ring Buffer header is the shared memory structure
typedef struct _tagRingBufferHeader {
  volatile uint32_t magic;   //Magic number
  volatile uint32_t beginAddr;
  volatile uint32_t size;
  volatile uint32_t bufferID;  // RINGBUFFER_TYPE, choose a type from RINGBUFFER_TYPE

  volatile uint32_t writePtr;
  volatile uint32_t numOfReadPtr;
  volatile uint32_t reserve2;  //Reserve for Red Zone
  volatile uint32_t reserve3;  //Reserve for Red Zone

  volatile uint32_t readPtr[4];

  volatile int32_t  fileOffset ;
  volatile int32_t  requestedFileOffset ;
  volatile int32_t  fileSize ;

  volatile int32_t  bSeekable ;  /* Can't be sought if data is streamed by HTTP */
/*
  FileOffset:
    the offset to the streaming file from the beginning of the file.
    It is set by system to tell FW that the current streaming is starting from ¡¥FileOffset¡¦ bytes.
    For example, the TIFF file display will set fileOffset to 0 at beginning.

  RequestedFileOffset:
    the offset to be set by video firmware, to request system to seek to other place.
    The initial is -1.When it is not equal to -1, that means FW side is requesting a new seek.

  FileSize:
    file size. At current implementation, only TIFF decode needs the fileSize,
    other decoding does not pay attention to this field

  the behavior for TIFF seek:
  At the initial value, FileOffset = 0, or at any initial offset (for example, resume from bookmark), RequestedFileOffset=-1. FileSize= file size.
  1. If FW needs to perform seek operation, FW set RequestedFileOffset to the value it need to seek.
  2. Once system see RequestedOffset is not -1, system reset the ring buffer (FW need to make sure it will not use ring buffer after request the seek), set FileOffset to the new location (the value of RequestedFileOffset), then set RequestedOffset  to -1. From now on, system will stream data from byte FileOffset of the file.
  3. FW needs to wait until RequestedOffset== -1, then check the value inside FileOffset. If FileOffset is -1, that means read out of bound.
  If system already finish the streaming before FW issue a seek, system will still continue polling.
*/

} RINGBUFFER_HEADER;

#if 0
// OMX Ring Buffer header is the shared memory structure
typedef struct _tagOMXRingBufferHeader {
  volatile uint32_t magic;   //Magic number
  volatile uint32_t beginAddr;
  volatile uint32_t size;
  volatile uint32_t bufferID;  // RINGBUFFER_TYPE, choose a type from RINGBUFFER_TYPE

  volatile uint32_t writePtr;
  volatile uint32_t numOfReadPtr;
  volatile uint32_t reserve2;
  volatile uint32_t reserve3;
  volatile uint32_t reserve4;

  volatile uint32_t readPtr[4];

  volatile uint32_t isNativeBuffer;
  volatile uint32_t state;
  volatile uint32_t omx_picW;
  volatile uint32_t omx_picH;
  volatile int32_t  omx_ptsL;
  volatile int32_t  omx_ptsH;
} OMX_RINGBUFFER_HEADER;
#else
#define OMX_RINGBUFFER_HEADER RINGBUFFER_HEADER
#endif

typedef enum _tagFatalErrorCode {

  FATALERR_GENERAL          = 0x00000000,

  /* FOR AUDIO */
  FATALERR_AUDIO            = 0x10000000,
  SPDIF_Error_Exceed_Threshold,
  AUDIO_DRY_OUT,
  UNKNOWN_FORMAT,
  UNRECOVERABLE_ERROR,
  AUDIO_DATA_RECEIVE_ERROR,
  FATALERR_AUDIO_UNSUPPORTED_SAMPLERATE,
  AUDIO_DIGITAL_OUT_ONLY,
  AUDIO_ASF_FILE_ERROR,
  FATALERR_AUDIO_PASSTHROUGH_ONLY,

  /* FOR VIDEO */
  FATALERR_VIDEO            = 0x20000000,
  FATALERR_VIDEO_VBM_CONFIG,
  FATALERR_VIDEO_MPEG2DEC_UNSUPPORTED_TRICK_PLAY,
  FATALERR_VIDEO_MPEG2DEC_UNKNOWN_FRAME_RATE,
  FATALERR_VIDEO_MPEG2DEC_UNKNOWN_START_CODE,
  FATALERR_VIDEO_MPEG2DEC_UNKNOWN_PICTURE_CODING_TYPE,
  FATALERR_VIDEO_MPEG2DEC_UNKNOWN_PICTURE_STRUCTURE,
  FATALERR_VIDEO_MPEG2DEC_UNKNOWN_EXTENSION,
  FATALERR_VIDEO_MPEG2DEC_UNSUPPORTED_RESOLUTION,
  FATALERR_VIDEO_MPEG2DEC_UNSUPPORTED_EXTENSION,
  FATALERR_VIDEO_MPEG2DEC_INVALID_MARKER_BIT,
  FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_GMC,
  FATALERR_VIDEO_MPEG4DEC_UNKNOWN_START_CODE,
  FATALERR_VIDEO_MPEG4DEC_UNKNOWN_FRAME_RATE_CODE,
  FATALERR_VIDEO_MPEG4DEC_WRONG_PIC_TYPE,
  FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_NOT_RECTANGULAR,
  FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_SPRITE,
  FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_NOT8BIT,
  FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_SCALABILITY,
  FATALERR_VIDEO_H264DEC_UNSUPPORTED_PROFILE,
  FATALERR_VIDEO_ALLOC_COMEM,
//TV code
  FATALERR_VIDEO_H264DEC_4K2K,
  FATALERR_VIDEO_HEVCDEC_4K2K,
  FATALERR_VIDEO_HEVCDEC_10BIT,
  FATALERR_VIDEO_MPEG4DEC_UNSUPPORTED_ERROR_RESILIENCE,
  FATALERR_VIDEO_MPEG4DEC_4K2K,
  FATALERR_VIDEO_VP9DEC_4K2K,
  FATALERR_VIDEO_VC1DEC_UNSUPPORTED_RESOLUTION,
  FATALERR_VIDEO_VC1DEC_UNSUPPORTED_X8INTRA,
  FATALERR_VIDEO_AVS2DEC_4K2K,
  FATALERR_VIDEO_AVS2DEC_10BIT,
// end TV code
  /* FOR SYSTEM */
  FATALERR_SYSTEM           = 0x30000000,
  FATALERR_SYSTEM_TRANSCODE_SERVER_ERROR,
  FATALERR_SYSTEM_MEDIA_REMOVED,
  FATALERR_SYSTEM_EIO,

} FATAL_ERROR_CODE;

typedef enum _tagDecoderMessageCode {

  DECODER_MESSAGE_GENERAL = 0x00000000,

  /* FOR AUDIO DECODERS */
  DECODER_MESSAGE_AUDIO_FIRST_FRAME_DECODED = 0x10000000,          /* first frame has been decoded after flushing */

  /* FOR VIDEO DECODERS */
  DECODER_MESSAGE_VIDEO_RECOVERABLE_ERROR = 0x20000000,
  DECODER_MESSAGE_VIDEO_FIRST_PICTURE_DECODED,          /* first picture has been decoded after flushing */
  DECODER_MESSAGE_VIDEO_CONTINUOUS_ERROR_FRM_DECODED,
  DECODER_MESSAGE_VIDEO_CONTINUOUS_GOOD_FRM_DECODED,
  DECODER_MESSAGE_VIDEO_FIRST_PICTURE_VO_DISPLAY,
  DECODER_MESSAGE_VIDEO_SEARCH_I_FRM_BUT_ERRORBLOCKS,

} DECODER_MESSAGE_CODE;

#endif



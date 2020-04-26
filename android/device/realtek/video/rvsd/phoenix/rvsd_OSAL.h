#ifndef _RVSD_OSAL_H_
#define _RVSD_OSAL_H_

#ifndef ANDROID
#include <stdint.h>        // for int64_t
#endif
#include <stdio.h>
#include <linux/ion.h>
#include <ion/ion.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>
#include "InbandAPI.h"
//#define RVSD_TRACE_ENABLE

#if 1

#ifdef ANDROID

#undef LOG_TAG
#define LOG_TAG "RTK_RVSD"
#include <utils/Log.h>
#define printf(fmt, args...)    ALOGD(fmt, ##args);
#define RVSDLOGAT(fmt, args...) ALOGD("[%d]" fmt, __LINE__, ##args);
//#define RVSDLOG(fmt, args...)   ALOGD(fmt, ##args);
#define RVSDLOG(fmt, args...)   RVSDLOGAT(fmt, ##args);

#else   // #ifdef ANDROID

#define RVSDLOGAT_TIME(fmt, args...) {\
    struct timeval tv;\
    gettimeofday(&tv, NULL);\
    printf("[%ld.%ld][%s.%d]" fmt, tv.tv_sec, tv.tv_usec, __FILE__, __LINE__, ##args);\
}
//#define RVSDLOG(fmt, args...)   printf(fmt, ##args);
#define RVSDLOG(fmt, args...)   RVSDLOGAT_TIME(fmt, ##args);

#endif  // #ifdef ANDROID

#else
#define RVSDLOG(fmt, args...)
#endif  // #if 1

#define RVSD_ASSERT(cond)                                     \
{                                                             \
  if (!(cond))                                                \
  {                                                           \
      RVSDLOG("ASSERT:[%s:%d]", __FUNCTION__, __LINE__);      \
  }                                                           \
}

#ifdef RVSD_TRACE_ENABLE
#define RVSD_TRACE(format, ...) printf( format, ##__VA_ARGS__);
#else
#define RVSD_TRACE(format, ...)
#endif

#define RVSD_WARNNING(format, ...) {\
    printf("[RVSD WARNNING]");\
    printf( format, ##__VA_ARGS__);}

#define RVSD_WARNNING_SEC(_sec, format, ...) \
{\
  static long long prePTS = 0;\
  long long curPTS, diffPTS;\
  curPTS = getPTS();\
  diffPTS = curPTS - prePTS;\
  if(diffPTS > 90000 * _sec)\
  {\
        printf( format, ##__VA_ARGS__);\
        prePTS = curPTS;\
  }\
}

#define RVSD_SEC(sec, pPrePTS, fmt, args...)\
{\
    long long prePTS = *((long long*)pPrePTS);\
    long long curPTS = getPTS();\
    long long diffPTS = curPTS - prePTS;\
    if (diffPTS > (90000 * sec))\
    {\
        *((long long*)pPrePTS) = curPTS;\
        printf(fmt, ##args);\
    }\
}

typedef unsigned char  BYTE;
/*
enum
{
    RVSD_FRAME_RATE_UNKNOWN = 0,
    RVSD_FRAME_RATE_23_98_FPS,
    RVSD_FRAME_RATE_24_FPS,
    RVSD_FRAME_RATE_25_FPS,
    RVSD_FRAME_RATE_29_97_FPS,
    RVSD_FRAME_RATE_30_FPS,
    RVSD_FRAME_RATE_50_FPS,
    RVSD_FRAME_RATE_59_94_FPS,
    RVSD_FRAME_RATE_60_FPS
};
*/
enum
{
    RTKExtractor_Normal    = 0x100000, /* The value must exceed Max INBAND_CMD enum, and then the decoder will see that. */
    RTKExtractor_NOP,
    RTKExtractor_WaitSeek,
};

typedef enum
{
    CMD_RECV_NONE,
    CMD_RECV_STOP,
    CMD_RECV_RUN,
    CMD_RECV_PAUSE,
    CMD_RECV_DESTROY
} CMDSTATE;

typedef enum
{
    NOTIFY_NONE,
    NOTIFY_EOS,
    NOTIFY_FATAL_ERR
} NOTIFY_DVDPLAYER_STATUS;

typedef enum
{
    DECODER_INITED,
    DECODER_NOT_INITED,
    DECODER_RUNNING
} DecoderState;

typedef enum
{
    INTERLEAVED_TOP_FIELD = 0,  /* top    field data stored in even lines of a frame buffer */
    INTERLEAVED_BOT_FIELD,      /* bottom field data stored in odd  lines of a frame buffer */
    CONSECUTIVE_TOP_FIELD,      /* top    field data stored consecutlively in all lines of a field buffer */
    CONSECUTIVE_BOT_FIELD,      /* bottom field data stored consecutlively in all lines of a field buffer */
    CONSECUTIVE_FRAME,           /* progressive frame data stored consecutlively in all lines of a frame buffer */
    INTERLEAVED_TOP_FIELD_422,  /* top    field data stored in even lines of a frame buffer */
    INTERLEAVED_BOT_FIELD_422,      /* bottom field data stored in odd  lines of a frame buffer */
    CONSECUTIVE_TOP_FIELD_422,      /* top    field data stored consecutlively in all lines of a field buffer */
    CONSECUTIVE_BOT_FIELD_422,      /* bottom field data stored consecutlively in all lines of a field buffer */
    CONSECUTIVE_FRAME_422,        /* progressive frame with 4:2:2 chroma */
    TOP_BOTTOM_FRAME,            /* top field in the 0~height/2-1, bottom field in the height/2~height-1 in the frame */
    INTERLEAVED_TOP_BOT_FIELD,   /* one frame buffer contains one top and one bot field, top field first */
    INTERLEAVED_BOT_TOP_FIELD,   /* one frame buffer contains one bot and one top field, bot field first */

    MPEG2_PIC_MODE_NOT_PROG      /*yllin: for MPEG2 check pic mode usage */

} VP_PICTURE_MODE_t ;

typedef enum
{
  VO_NV21_MASK = 0x1u<<16, //1:NV21 0:NV12

} VP_PICTURE_MODE_EXT ;

enum {
    IDLE,
    RUNNING,
    FINISH
};

typedef struct {
    unsigned char systemMode;  /* enum AVSYNC_MODE */
    unsigned char videoMode;   /* enum AVSYNC_MODE */
    unsigned char audioMode;   /* enum AVSYNC_MODE */
    unsigned char masterState; /* enum AUTOMASTER_STATE */
} MASTERSHIP_t;

typedef struct {
    long long      RCD;
    unsigned long  RCD_ext;
    long long      GPTSTimeout;
    long long      videoSystemPTS;
    long long      audioSystemPTS;
    long long      videoRPTS;
    long long      audioRPTS;
    unsigned long  videoContext;
    unsigned long  audioContext;
    MASTERSHIP_t     mastership;
    unsigned long  videoFreeRunThreshold;
    unsigned long  audioFreeRunThreshold;
    long long      masterGPTS;
    long           audioFullness;
    long           audioPauseFlag;
    long           VO_Underflow;
    long           AO_Underflow;
    long           videoEndOfSegment;
    long           audioEndOfSegment;
    unsigned char  reserved[16];
} REFCLOCK_t;

#define OFFSET_OSD_STRUCT 0               //0x06000000 OSD Struct           (SCPU Write)
#define OFFSET_VO_OSD_WIN 0x100       //0x06000100 VO_OSD_WIN           (SCPU Write)
#define OFFSET_RET_TOP_ADDR 0x1000  //0x06001000 return top_addr      (ACPU Write)
#define OFFSET_YUV_STRUCT 0x2000      //0x06002000 YUV Struct           (SCPU Write)
#define OFFSET_RET_Y_ADDR 0x3000     //0x06003000 return Y_addr        (ACPU Write)

typedef struct {
    unsigned int version;
    unsigned int mode;
    unsigned int Y_addr;
    unsigned int U_addr;
    unsigned int pLock;
    unsigned int width;
    unsigned int height;
    unsigned int Y_pitch;
    unsigned int C_pitch;
    unsigned int RPTSH;
    unsigned int RPTSL;
    unsigned int PTSH;
    unsigned int PTSL;

    /* for send two interlaced fields in the same packet,
    valid only when mode is INTERLEAVED_TOP_BOT_FIELD or INTERLEAVED_BOT_TOP_FIELD*/
    unsigned int RPTSH2;
    unsigned int RPTSL2;
    unsigned int PTSH2;
    unsigned int PTSL2;

    unsigned int context;
    unsigned int pRefClock;  /* not used now */

    unsigned int pixelAR_hor; /* pixel aspect ratio hor, not used now */
    unsigned int pixelAR_ver; /* pixel aspect ratio ver, not used now */

    unsigned int Y_addr_Right; /* for mvc */
    unsigned int U_addr_Right; /* for mvc */
    unsigned int pLock_Right; /* for mvc */
    unsigned int mvc;		  /* 1: mvc */
    unsigned int subPicOffset;/* 3D Blu-ray dependent-view sub-picture plane offset metadata as defined in BD spec sec. 9.3.3.6.
									   Valid only when Y_BufId_Right and C_BufId_Right are both valid */
    unsigned int pReceived; 		// fix bug 44329 by version 0x72746B30 'rtk0'
    unsigned int pReceived_Right;	// fix bug 44329 by version 0x72746B30 'rtk0'

    unsigned int fps;   // 'rtk1'
    unsigned int IsForceDIBobMode; // force vo use bob mode to do deinterlace, 'rtk2'.
    unsigned int lumaOffTblAddr;    // 'rtk3'
    unsigned int chromaOffTblAddr;  // 'rtk3'
    unsigned int lumaOffTblAddrR; /* for mvc, 'rtk3' */
    unsigned int chromaOffTblAddrR; /* for mvc, 'rtk3' */

    unsigned int bufBitDepth;   // 'rtk3'
    unsigned int bufFormat;     // 'rtk3', according to VO spec: 10bits Pixel Packing mode selection, "0": use 2 bytes to store 1 components. MSB justified. "1": use 4 bytes to store 3 components. LSB justified. 

    // VUI (Video Usability Information)
    unsigned int transferCharacteristics;   // 0:SDR, 1:HDR, 2:ST2084, 'rtk3'

    // Mastering display colour volume SEI, 'rtk3'
    unsigned int display_primaries_x0;
    unsigned int display_primaries_y0;
    unsigned int display_primaries_x1;
    unsigned int display_primaries_y1;
    unsigned int display_primaries_x2;
    unsigned int display_primaries_y2;
    unsigned int white_point_x;
    unsigned int white_point_y;
    unsigned int max_display_mastering_luminance;
    unsigned int min_display_mastering_luminance;

    /*for transcode interlaced feild use.*/ //'rtk4'
    unsigned int Y_addr_prev;   //'rtk4'
    unsigned int U_addr_prev;   //'rtk4'
    unsigned int Y_addr_next;   //'rtk4'
    unsigned int U_addr_next;   //'rtk4'
    unsigned int video_full_range_flag; //'rtk4'  default= 1
    unsigned int matrix_coefficients;   //'rtk4   default= 1

#if defined(RVSD_SUPPORT_TECHNICOLOR)
    /*for transcode interlaced feild use.*/  //'rtk5'
    unsigned int pLock_prev;
    unsigned int pReceived_prev;
    unsigned int pLock_next;
    unsigned int pReceived_next;

    unsigned int is_tch_video; // rtk-6
    tch_metadata technicolor_data;
#endif

   unsigned int pFrameBufferDbg;  //rtk7
   unsigned int pFrameBufferDbg_Right;
   unsigned int Y_addr_EL; //'rtk8' for dolby vision
   unsigned int U_addr_EL;
   unsigned int width_EL;
   unsigned int height_EL;
   unsigned int Y_pitch_EL;
   unsigned int C_pitch_EL;
   unsigned int lumaOffTblAddr_EL;
   unsigned int chromaOffTblAddr_EL;

   unsigned int dm_reg1_addr;
   unsigned int dm_reg1_size;
   unsigned int dm_reg2_addr;
   unsigned int dm_reg2_size;
   unsigned int dm_reg3_addr;
   unsigned int dm_reg3_size;
   unsigned int dv_lut1_addr;
   unsigned int dv_lut1_size;
   unsigned int dv_lut2_addr;
   unsigned int dv_lut2_size;
   unsigned int slice_height; //for compressed mode alignment
   unsigned int hdr_metadata_addr;//'rtk9'
   unsigned int hdr_metadata_size;//'rtk9'
   unsigned int tch_metadata_addr;//'rtk9'
   unsigned int tch_metadata_size;//'rtk9'
} YUV_STATE;

typedef struct {
    unsigned int magic; //11223344 means OK
    unsigned int state;  //0:OFF 1:ON
    unsigned int order; // 0~2 top:2
    unsigned int phyAddr;
    unsigned int Height;
    unsigned int Width;
    unsigned int attr_rgbOrder;
    unsigned int attr_type;
    unsigned int reserve[6];
} OSD_STATE;

typedef struct AspectRatioStruct
{
    unsigned short hor;
    unsigned short ver;
} AspectRatioStruct;

/* DVD-VR CCI (Copy Control Information). See DVD-VR Speccification sec. 5.5.4.2 for details */
typedef struct
{
    unsigned char CGMS:2;
    unsigned char APSTB:2;
    unsigned char source:1;
    unsigned char reserved:3;
} DVD_VR_CCI;

/* DVD-VR DCI (Display Control Information). See DVD-VR Speccification sec. 5.5.4.2 for details */
typedef struct
{
    unsigned char aspect_ratio:4;
    unsigned char subtitling_mode:2;
    unsigned char reserved:1;
    unsigned char film_camera_mode:1;
} DVD_VR_DCI;

typedef struct
{
    /* NTSC and PAL */
    unsigned int aspectRatio:4;
    unsigned int copyMode:2;
    unsigned int MV_enCopyProt:1;  /* Microvision VBI pseudo-sync pulses detection for both NTSC and PAL.
                                      1: detected, 0: undetected. */
    /* PAL Only */
    unsigned int subTitle:2;
    unsigned int filmCamera:1;     /* PAL line 23 group B b4: 0: camera mode, 1: film mode */
    /* NTSC Only */
    unsigned int APS:2;
    unsigned int pre_recorded:1;   /* NTSC line 20 Word 2 b10: 0: not analog pre-recorded medium,  1: analog pre-recorded medium */

    unsigned int MV_colour_stripe:3;  /* Types of Macrovision color stripe for NTSC:
                                         0: No color stripe, No pseudo-sync.
                                         1: Only pseudo-sync.
                                         3: pseudo-sync & color stripe on line 30, 31.
                                         7: pseudo-sync & color stripe on line 24~ 27.
                                         (If PAL, this field is always 0, so TVE could skip this.) */
    unsigned int line21_data1:8;
    unsigned int line21_data2:8;
} VIDEO_VBI_DATA;

#if defined(RVSD_VO_WRITEBACK) || defined(RVSD_USE_VO_SCALE_CMD) || defined(RVSD_USE_VO_SCALE_AUTO)
typedef struct {
    unsigned int bufferNum;
    unsigned int bufferId;
    unsigned int bufferSize;
    unsigned int addrY;
    unsigned int addrC;
    unsigned int pitchY;
    unsigned int pitchC;
    unsigned int version;//version='rtk0' has pLock, pReceived; 'rtk1' has targetFormat; 'rtk2' has width;
    unsigned int pLock;
    unsigned int pReceived;
    unsigned int targetFormat;
    //bit 0=>NV21, 1:NV21, 0:NV12;
    //bit 1=>422, 1:422, 0:420;
    //bit 2=>bit depth, 1:10 bits, 0: 8 bits;
    //bit 3=>mode_10b, 0: use 2 bytes to store 1 components. MSB justified. 1: use 4 bytes to store 3 components, LSB justified;
    unsigned int width;
    unsigned int reserved[2];
} YUV_BUFFER;
#endif


#define IPCREAD(x) IPCRead(&(x))
#define IPCWRITE(x,y) IPCWrite(&(x),y)

void rvsd_usec_sleep(unsigned int usec_delay);


int *getIOAddress(unsigned int addr);
int64_t getPTS();
void    pli_IPCCopyMemory(BYTE* src, BYTE* des, unsigned long len);
void    pli_IPCWriteULONG(BYTE* des, unsigned long data);
void    pli_IPCWriteULONGLONG(BYTE* des, unsigned long long data);
unsigned long    pli_IPCReadULONG(BYTE* src);
unsigned long long  pli_IPCReadULONGLONG(BYTE* src);
unsigned long IPCRead(unsigned long *src);
unsigned long long IPCRead(unsigned long long *src);
long IPCRead(long *src);
long long IPCRead(long long *src);

int rvsd_ion_alloc(int fd, size_t len, size_t align, unsigned int heap_mask,
                   unsigned int flags, ion_user_handle_t *handle);
#endif


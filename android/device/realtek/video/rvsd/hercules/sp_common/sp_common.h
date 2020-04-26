#ifndef VIDEO_COMMON_H
#define VIDEO_COMMON_H

#include "rvsd_def.h"
#include <string.h>
#include "../rvsd_InBandCMD.h"
#include "rpc_common.h"
#include "rvsd_VORPC.h"
#include <assert.h>
#include <pthread.h>
#include <utils/Log.h>  // for ALOGD
#include "rvsd_CommonFunc.h"
#include "rtk_ion.h"
#include "../bdsp_decoder/BDSP_Decode_def.h"

/*-----------------------------
DBG info
------------------------------*/
#define SP_DBG_VO 0
#define SP_DBG_BS_RING 0
#define SP_TRACE_ENABLE 0
#define SP_ENABLE_DBG 0
#define SP_CHECK_PHY_ADDR_RANGE 0
#define SP_CHECK_VIRT_ADDR_RANGE 0
#define SP_CHECK_VTM 0
#define SP_DVB_PRINT_DATA_EN 0
#define SP_DISABLE_SEND_OBJ_TO_VO 0
#define SP_DISABLE_SPU 0    // it may NOT work ??
#define SP_DUMP_BS 0    // NOT display pictures

#ifdef ANDROID7_HACK_AVOID_SPU
#undef SP_DISABLE_SPU
#define SP_DISABLE_SPU 1
#endif

/*-----------------------------
Main control
------------------------------*/
#define SP_USE_ION_MEMORY 1
#define SP_WORKAROUND 1
#define SP_FIXED_BUG 1
#define SP_DATA_STRUCTURE_SIZE_SYNC_WITH_VO 0
#define SP_LITTLE_ENDIAN 1
#define SP_LITTLE_ENDIAN_SPECIAL 0
#define SP_CHECK_BUG 1
#define SP_CONFIG_RESOLUTION_BY_RPC 1
#define SP_FLUSH_NEW 1 // When RVSD recieve Flush command, it will record the BS_ring-wp at that moment. Then it will update BS_ring-rp with the recorded-wp later.
#define SP_FIXED_BUG_43980 1
#define SP_FIXED_BUG_44226 1
#define SP_FIXED_BUG_44312 1
#define SP_FIXED_BUG_44226 1
#define SP_FIXED_BUG_44221 1
#define SP_FIXED_BUG_OF_DVB 1
#define SP_AVOID_HANG_IN_RPC 1
#define RPC_SUPPORT_MULTI_CALLER
#define FIXED_COVERITY

/*-----------------------------
DEFINE
------------------------------*/
#define MAX_TAG_NUM         32
#define SP_WAIT_VO_USEC 30
#define VP_DBGMASK_BDSP_DEC 0x08000000
#define VP_DBGMASK_DVBSP_DEC 0x00800000
#define VP_DBGMASK_SP_DEC 0x10000000
#define VP_DBGMASK_RPC            0x00000100      // 8
#define SP_BUSY_PRINT_SEC 3

/*-----------------------------
MACRO FUNCTION
------------------------------*/
#if SP_USE_ION_MEMORY
#define VP_malloc(a, b, c) (a)->VP_malloc_func(b)
#define VP_free(a, b)
#else
#define VP_malloc(a, b, c) malloc(b)
#define VP_free(a, b) free(a)
#endif
#define VP_memcpy memcpy
//#define VP_memblk_alloc(a, b) malloc(a)
//#define VP_memblk_request(a, b) malloc(a)
//#define VP_memblk_release(a) free(a)
//#define VP_memblk_free(a, b) free(a)
#define SP_WAIT_VO_UNLOCK(a, status) (a)->SP_WaitVOUnlock(SP_WAIT_VO_USEC, status)
#define VP_memset memset
//#define VP_malloc(a, b) malloc(a)
#define VBM_RemoveTask(a)   // NO usage
#define VTM_VALID_TASKID(a) // NO usage
#define VTM_RemoveTask(a)   // NO usage
#define VTM_ForceResume(a, b) (b)->SP_ForceResume()
#define VTM_ResumeTask(a, b, c) (c)->SP_ResumeTask(b)
#define VTM_SuspendTask(a, b, c) (c)->SP_SuspendTask(b)
#define SP_EnqueueRPCRing(a, b) (a)->SP_EnqueueRPCRing_func(b)
#if 0
#define VPMaskPrintf(level, format, ...) {\
    { ALOGD(format, ##__VA_ARGS__); }\
}
#else
#define VPMaskPrintf(...)
#endif
#define SPU_RPC_PREPARE_CLNT(A, B, C)	prepareCLNT(A, B, C)
#define SP_CHECK(a, b) {(a) = (b);}
#define SP_ToMipsUncacheAddr(a, b) (a)->SP_ToMipsUncacheAddrFunc(b, __FUNCTION__, __LINE__)
//#define SP_CHECK(a, b) {int c, d; c=(a); d=(b); if(c != d) {ROSPrintf("[SP %d %s %x %x]\n", __LINE__ ,  __FUNCTION__, c, d);}}
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define GET_BIT(x, pos) (((x) >> (pos)) & 0x1)
#define RESET_BIT(x, pos) ((x) & ~(0x1 << (pos)))
#define SET_BIT(x, pos) ((x) | (0x1 << (pos)))
#define TO_MIPS_UNCACHE_ADDR(x) (((int)(x)) | 0xa0000000)
#define CHECK_MEMORY_FOR_MIPS(x) { if(((unsigned int)(x)) >= 0x20000000) SP_WARNING("[memory error %08x for MIPS]\n", (unsigned int)(x));}
#define SP_SLEEP() usleep(25000)   // estimate 25ms ... Need to tune...

#if 1   // for android player
#define WarningPrint ALOGD
//#define Warning(fmt, args...) ALOGD("[SP Warn]%s:"fmt, __FUNCTION__, ##args);
#define Warning(format, ...) \
{\
  static long long prePTS = 0;\
  long long curPTS, diffPTS;\
  curPTS = getPTS();\
  diffPTS = curPTS - prePTS;\
  if(diffPTS > 90000)\
  {\
        ALOGD( format, ##__VA_ARGS__);\
        prePTS = curPTS;\
  }\
}
#define ROSPrintf ALOGD
#define RTK_CHECK() ALOGD("[SP CHECK @ %s %d]\n", __func__, __LINE__);   // code may have error...

#if SP_TRACE_ENABLE
//#define SP_TRACE(fmt, args...) \ALOGD("[SP]%s:", fmt, __FUNCTION__, ##args);

#define SP_TRACE(fmt, args...) \
{\
    char buf[256], temp[256];\
    sprintf(temp, fmt, ##args); \
    sprintf(buf, "[SP]%s:", __FUNCTION__); \
    strcat(buf, temp); \
    ALOGD("%s", buf); \
}



#define SP_TRACE_RPC(rpc_type, func, line) { \
        char rpc_name[10][20]  = {"Create", "InitRingBuf", "Destroy", "Stop", "Run", "Enable", "Show", "Hide", "Flush", "Config"}; \
        ALOGD("[SP] %s @ %s %d\n", rpc_name[rpc_type], func, line); }
#define SP_PRINT_RING_PTR_NOW(p) (p)->SP_PrintRingPtrNow()
#else
#define SP_TRACE(format, ...)
#define SP_TRACE_RPC(rpc_type, func, line)
#define SP_PRINT_RING_PTR_NOW(p)
#endif

#if 1
//#if SP_ENABLE_DBG
#define SP_DBG() ALOGD("[SP %s %d]\n", __FUNCTION__, __LINE__);
#define SP_DBG2 ALOGD
#else
#define SP_DBG()
#define SP_DBG2(format, ...)
#endif

#define SP_LOG ALOGD
//#define SP_WARNING(format, ...) { ALOGD("[SP WARN]"); ALOGD(format, ##__VA_ARGS__); }
#define SP_WARNING(fmt, args...)   ALOGD("[SP WARN]%s:" fmt, __FUNCTION__, ##args);
#define SP_PRINT ALOGD

#else   // #if 1   // for android player

#define WarningPrint printf
#define Warning printf
#define ROSPrintf printf
#define RTK_CHECK() printf("[SP CHECK @ %s %d]\n", __func__, __LINE__);   // code may have error...
#define SP_TRACE(format, ...) {printf("[SP]"); printf(format, ##__VA_ARGS__); }
#define SP_DBG() printf("[SP %s %d]\n", __FUNCTION__, __LINE__);
#define SP_LOG printf
#define SP_DBG2 printf
#define SP_WARNING(format, ...) { printf("[SP WARN]"); printf(format, ##__VA_ARGS__); }
#define SP_PRINT printf
#endif

#if SP_DBG_VO
#define SP_PRINT_DVB(x, y, z) SP_Print_DVB(x, y, z)
#define SP_PRINT_DVD(x, y) SP_Print_DVD(x, y)
#define SP_PRINT_WORD(a, b, c) SP_PrintWord(a, b, c)
#else
#define SP_PRINT_DVB(x, y, z)
#define SP_PRINT_DVD(x, y)
#define SP_PRINT_WORD(a, b, c)
#endif

#if SP_DBG_BS_RING   // check BS ring
#define SP_PRINT_RING_PTR(a) (a)->SP_PrintRingPtr_func()
#else
#define SP_PRINT_RING_PTR(a)
#endif

#if SP_CHECK_VIRT_ADDR_RANGE
#define SP_CheckVirtAddrValid(a, b, c, d) (a)->SP_CheckVirtAddrValidFunc(b, c, d)
#else
#define SP_CheckVirtAddrValid(a, b, c, d)
#endif

#define ASSERT(cond, msgID, format, ...)                                      \
{                                                                             \
  if (cond)                                                                   \
  {                                                                           \
    WarningPrint(format, ##__VA_ARGS__);                                      \
    assert(0);                                                                \
  }                                                                           \
}

#if SP_DVB_PRINT_DATA_EN
#define SP_DVB_PRINT_DATA(a, b, c, d) SP_DVB_PrintData(a, b, c, d)
#else
#define SP_DVB_PRINT_DATA(a, b, c, d)
#endif

/*-----------------------------
DEFINE
------------------------------*/
#define DVBSP_ON_MIPS 0

//#define ION_MEMORY_SIZE (9 * 1024 * 1024)

#define ION_FILTER_SIZE (258 * 1024)
#define ION_DVD_SIZE (0)
#define ION_DVB_SIZE (6 * 1024 * 1024)

#define VP_PACKET_DWORD_SIZE 16

#define PALETTE_SIZE 16

#define SP_MAX_PACKET_4BYTES_UNIT 60

#define VMM_SIZE_DVB_SUBTITLE (0x0200000 * 3)

#define VMM_SIZE_SP_FILTER ( 256*1024)

#define TASK_STK_SIZE_DEC 64

#define RPC_BUF_SIZE 256

#define SP_UNDERFLOW (0x80000000)
//#define TO_VIRTUAL_ADDR(x) (((x) & 0x1fffffff) | 0xa0000000)
#define TO_VIRTUAL_ADDR(x) (x)

#define VTM_INVALID_TASK           0xff

#define SP_SUCCESS                 1
#define SP_FAIL                    0

#define VTM_SEMAPHORE_REGISTRATION  0
#define VTM_SEMAPHORE_BUFFER        1
#define VTM_SEMAPHORE_INPUT         2
#define VTM_SEMAPHORE_OUTPUT        3
#define VTM_SEMAPHORE_ISR           4
#define VTM_SEMAPHORE_MEQ           5
#define VTM_SEMAPHORE_DINQ          6
#define VTM_SEMAPHORE_CMD           7
#define VTM_SEMAPHORE_POLL          8
#define VTM_SEMAPHORE_SMALLBUFFER   9

#define VP_SUCCESS                 0
#define VP_ERR_NO_MEMORY          -1
#define VP_ERR_INVALID_PIN        -2
#define VP_ERR_WRONG_ADDRESSEE    -3
#define VP_ERR_RECEIVER_OVERFLOW  -4
#define VP_ERR_VTM_OVERFLOW       -5
#define VP_ERR_WRONG_DATA         -6
#define VP_ERR_NO_SEMAPHORE       -7
#define VP_ERR_NOT_SUSPEND        -8

#define VMM_ID_BD_SUBTITLE_BUFFER   16

#define VP_STREAM_PACKET_HEAD_SIZE  (sizeof(VP_STREAM_PACKET) - (8 * (VP_PACKET_DWORD_SIZE  - 1)))
#define VP_STREAM_PACKET_SIZE(x) (sizeof(VP_STREAM_PACKET) - (8 * (VP_PACKET_DWORD_SIZE - 1) - sizeof(x)))

/* Decoder flow standardized pin ID */
#define VP_PIN_VIDEO_DECODER_COMMAND      0
#define VP_PIN_VIDEO_DECODER_INPUT        1
#define VP_PIN_VIDEO_DECODER_OUTPUT       2
#define VP_PIN_VIDEO_DECODER_TOSCALER     3

#define VMM_ID_DECODER_FILTER        1
#define VMM_ID_JPEGDEC_FILTER        2
#define VMM_ID_SP_FILTER             3
#define VMM_ID_VOUT_FILTER           4
#define VMM_ID_DVB_SP_DECODER        5
#define VMM_ID_DIVX_SP_DECODER       6
#define VMM_ID_COMEM_BUFFER          7
#define VMM_ID_VBM_CONTEXT           8
#define VMM_ID_VBM_BUFFER            9
#define VMM_ID_VOUT_STILL_FLAG      10
#define VMM_ID_VOUT_MIXER2          11
#define VMM_ID_PERF_ANALYZER        12
#define VMM_ID_DEBUG_LOG            13
#define VMM_ID_TRANSITION_FILTER    14
#define VMM_ID_THUMBNAIL_FILTER     15
#define VMM_ID_BD_SUBTITLE_BUFFER   16
#define VMM_ID_FLASH_FILTER         17
#define VMM_ID_VMIXER_FILTER        18
#define VMM_ID_COMEM_EXTRA          19
#define VMM_ID_VO_PLANE_MIXER_BUFFER 20
#define VMM_ID_DYNAMIC_STEAL_COMEM  21

#define H_FLAG_SUCCESS                  (0x10000000)
#define H_FLAG_ERROR                    (0x20000000)
#define H_FLAG_WARNNING                 (0x40000000)

#define H_TYPE_ERROR_GENERAL            ((H_FLAG_ERROR)   |     (0x00010000))
#define H_TYPE_ERROR_PIN                ((H_FLAG_ERROR)   |     (0x00020000))
#define H_TYPE_ERROR_ALLOC              ((H_FLAG_ERROR)   |     (0x00030000))
#define H_TYPE_ERROR_MEDIA_SAMPLE       ((H_FLAG_ERROR)   |     (0x00040000))
#define H_TYPE_ERROR_MEDIA_TYPE         ((H_FLAG_ERROR)   |     (0x00050000))
#define H_TYPE_ERROR_FILTER             ((H_FLAG_ERROR)   |     (0x00060000))
#define H_TYPE_ERROR_STATE              ((H_FLAG_ERROR)   |     (0x00070000))
#define H_TYPE_ERROR_ENUM_PINS          ((H_FLAG_ERROR)   |     (0x00080000))
#define H_TYPE_ERROR_ENUM_FILTERS       ((H_FLAG_ERROR)   |     (0x00090000))
#define H_TYPE_ERROR_EVENT              ((H_FLAG_ERROR)   |     (0x000a0000))

/*****************************************************************************/
/* Success codes                                                             */
/*****************************************************************************/
#define S_OK                            ((H_FLAG_SUCCESS) |     (0x00000000))
#define S_FALSE                         ((H_FLAG_SUCCESS) |     (0x00000001))

#define SW_TRAP_VBM_MEM_ERROR               0xFF01
#define SW_TRAP_MEM_HEADER_TRASH            0xFF02
#define SW_TRAP_MEM_INTRCHK_FAIL            0xFF03
#define SW_TRAP_VBM_MEM_OVERFLOW            0xFE04
#define SW_TRAP_VP_MEM_OVERFLOW             0xFE05
#define SW_TRAP_FLOW_ERROR                  0xFE06
#define SW_TRAP_INVALID_PTR                 0xFE07

/*****************************************************************************/
/* Error codes                                                               */
/*****************************************************************************/
// H_TYPE_ERROR_GENERAL
#define E_FAIL                          ((H_TYPE_ERROR_GENERAL) | (0x00000001))
#define E_GEN_POINTER_NULL              ((H_TYPE_ERROR_GENERAL) | (0x00000002))
#define E_GEN_UNEXPECTED                ((H_TYPE_ERROR_GENERAL) | (0x00000003))
#define E_GEN_MEM_ARRAY_BOUNDARY        ((H_TYPE_ERROR_GENERAL) | (0x00000004))
#define E_TIMEOUT                       ((H_TYPE_ERROR_GENERAL) | (0x00000005))
#define E_IO                            ((H_TYPE_ERROR_GENERAL) | (0x00000006))
#define E_IO_UNKNOWN_PROTOCOL           ((H_TYPE_ERROR_GENERAL) | (0x00000007))


/*-----------------------------
ENUM
------------------------------*/
enum
{
    SP_ION_FILTER_IDX = 0,
    SP_ION_DECODER_IDX,

    SP_ION_SEG_NUM
};

enum {
    ENUM_SP_EXIT_0 = 0,
    ENUM_SP_EXIT_1,
    ENUM_SP_EXIT_2,
    ENUM_SP_EXIT_3,
    ENUM_SP_EXIT_4,
    ENUM_SP_EXIT_5,
    ENUM_SP_EXIT_6,
};

enum {
    ENUM_ABNORMAL_0_PACKET = 0,
};

enum
{
    ENUM_WAIT_VO_READY_DONE = 0,
    ENUM_WAIT_VO_READY_EXIT,
    ENUM_WAIT_VO_READY_EXIT_ABNORMALLY,
};

enum SP_DVD_SEND_TO_VO_TYPE {
    SP_DVD_ICQ_TO_VO = 0,
    SP_DVD_DELIVER_TO_VO,
};

typedef enum
{
    VF_STATUS_STOP = 0,
    VF_STATUS_RUN,
    VF_STATUS_PAUSE
} VF_STATUS ;

typedef enum
{
    SP_RPC_CREATE = 0,
    SP_RPC_INIT_RING_BUF,
    SP_RPC_DESTROY,
    SP_RPC_STOP,
    SP_RPC_RUN,
    SP_RPC_ENABLE,
    SP_RPC_SHOW,
    SP_RPC_HIDE,
    SP_RPC_FLUSH,
    SP_RPC_CONFIG_RESOLUTION,
} SP_RPC_TYPE ;

typedef enum
{
    VIDEO_COMPONENT_STOP = 0,
    VIDEO_COMPONENT_RUN,
    VIDEO_COMPONENT_PAUSE,
    VIDEO_COMPONENT_CREATED
} VP_COMPONENT_STATE;

typedef enum
{
    VP_OBJ_STREAM_TYPE,
    VP_OBJ_PICTURE_TYPE,
    VP_OBJ_PICTURE_422_TYPE,
    VP_OBJ_SMBUFFER_PICTURE_TYPE,
    VP_OBJ_JPEG_PICTURE_TYPE,

    VP_MSG_EOS_TYPE,

    /* common commands */
    VP_CMD_COMMON_RUN_TYPE,
    VP_CMD_COMMON_PAUSE_TYPE,
    VP_CMD_COMMON_STOP_TYPE,
    VP_CMD_COMMON_INIT_RING_BUF_TYPE,
    VP_CMD_COMMON_CREATE_TYPE,
    VP_CMD_COMMON_DESTROY_TYPE,
    VP_CMD_COMMON_FLUSH_TYPE,

    /* decoder commands */
    VP_CMD_DEC_SET_CCBYPASS_MODE_TYPE,
    VP_CMD_DEC_SET_DNR_MODE_TYPE,

    /* VscaleToRGB commands */
    VP_CMD_VSCALE_TORGB_SETUP_TYPE,

    /* JpegDecInband commands */
    VP_CMD_JPEGDEC_INBAND_INIT_TYPE,
    VP_CMD_JPEGDEC_INBAND_INIT_ICQ_TYPE,
    VP_CMD_JPEGDEC_INBAND_SEM_TYPE,

    /* transition commands */
    VP_CMD_TRANSITION_SETUP_TYPE,

    VP_OBJ_DVD_SP_TYPE,
    VP_OBJ_DVB_SP_REGION_TYPE,
    VP_OBJ_DIVX_SP_TYPE,
    VP_OBJ_BD_SP_PAGE_TYPE,

    /* scaler commands */
    VP_CMD_SCALER_SETUP_TYPE,

    /* for switch DivX3 and MJPEG in DIVX3 component */
    VP_CMD_DIVX3_SWITCH_TO_DIVX3,
    VP_CMD_DIVX3_SWITCH_TO_MJPEG,

    /* for Scaler component in small buffer mode */
    VP_CMD_SCALER_HOR_SCALING_TYPE,

    /* smooth level for scaling */
    VP_CMD_SMOOTH_LEVEL_TYPE,

    /* thumbnail commands */
    VP_CMD_THUMBNAIL_SET_THRESHOLD_TYPE,
    VP_CMD_THUMBNAIL_SET_STARTPIC_TYPE,

    /*  image command */
    VP_CMD_IMG_DECODE_TYPE,

    /* send buffer to scaler */
    VP_OBJ_PICTURE_SCALER_START_TYPE,
    VP_OBJ_PICTURE_SCALER_BUFFER_TYPE,
    VP_OBJ_PICTURE_SCALER_END_TYPE,           /* decode success */
    VP_OBJ_PICTURE_SCALER_FAIL_TYPE,          /* Maybe decoder detects something wrong in the middle, we won't show picture */
    VP_OBJ_PICTURE_SCALER_REALPIC_TYPE,
    VP_OBJ_PICTURE_SCALER_UNSUPPORT_SIZE_TYPE,  /* memory isn't enough to decode this picture, e.g.: big progressive jpeg */

    /*flash decoder command*/
    VP_CMD_FLASH_SETOUTPUT,

    /* add all your definitions above this line */
    VP_PACKET_NUM_MAX
} VP_PACKET_TYPE ;

typedef enum
{
    ENUM_SP_NONE = 0,
    ENUM_SP_DVD_TYPE,
    ENUM_SP_DVB_TYPE,
    ENUM_SP_BD_TYPE
} SP_DECODE_TYPE;

/*-----------------------------
DATA STRUCTURE
------------------------------*/
class SP_INFO;
//typedef long HRESULT;
typedef unsigned int VP_TASK_STACK;
typedef unsigned char u_char;
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned short u_short;

typedef struct
{
    unsigned int memblk_start_addr ;
    unsigned int freeblk_start_addr ;
    unsigned int size ;
    int owner_id ;

} MEM_BLK_INFORMATION ;

typedef struct  {
    long base;
    long limit;
    long wp;
    long rp;
} SP_RINGBUF;

typedef struct
{
    short x ;        /* upper-left corner x coordinate */
    short y ;        /* upper-left corner y coordinate */
    short width ;    /* window width */
    short height ;   /* window height */

} VP_WINDOW ;

typedef struct
{
//  ROS_EVENT         *sem ;
    RINGBUFFER_HEADER *pRBH ;
} VP_CMD_COMMON_INIT_RING_BUF ;

typedef struct
{
    VP_PACKET_TYPE type ;
    unsigned char  sender ;    /* ID of the task from which the packet is sent */
    unsigned char  receiver ;  /* ID of the task to which the packet is addressed */
    unsigned short len ;       /* The total length of the header plus the struct in payLoad[] */

    unsigned long long payLoad[VP_PACKET_DWORD_SIZE - 1] ;    // 120 Bytes
} VP_STREAM_PACKET ;

typedef struct VP_BUFFER
{
    char      usage;
#ifdef VP_MEMORY_CHECK
    char      cs_status;
#endif
    void      *ptr;
#ifdef VP_MEMORY_CHECK
    int       size;
    unsigned  checksum;
    struct VP_BUFFER *next;
#endif
    unsigned char taskID ;
} VP_BUFFER;

typedef struct tagVP_COMPONENT VP_COMPONENT ;

typedef struct
{

    unsigned int  Qread ;
    unsigned int  Qwrite ;
    unsigned int  Qsize ;

    VP_STREAM_PACKET *Q ;

    int  boundPin ;
    VP_COMPONENT *boundFilter ;
} VP_IO_PIN ;

struct tagVP_COMPONENT
{
    // do not reorder the part. this part should be the same as the beginning of the filter.
    unsigned char numPins ;
    char taskID ;

    int (*Receive)     (VP_COMPONENT *, VP_STREAM_PACKET *recvPkt, int pin, int bSuspend) ;
    int (*Bind)        (VP_COMPONENT *, VP_COMPONENT *boundComp,   int thisPin, int boundPin) ;
    // end of non-reorder part

    /* component functions */
    int (*Run)        (VP_COMPONENT *) ;
    int (*Reset)      (VP_COMPONENT *) ;
    int (*Query)      (VP_COMPONENT *, int queryWhat, int arg0, int arg1) ;
    int (*Init)       (VP_COMPONENT *, void *init) ;
    int (*Destroy)    (VP_COMPONENT *) ;
    int (*ReceiveCmd) (VP_COMPONENT *, VP_STREAM_PACKET *cmd) ;
    int (*Poll)       (VP_COMPONENT *) ;

#ifdef VP_MEMORY_CHECK
    unsigned  comp_context_start;
    unsigned  comp_context_end;
#endif
    /* ROS Semaphore used betwee ProcCmd() and ReceiveCmd() */
#if SP_DATA_STRUCTURE_SIZE_SYNC_WITH_VO
    void *padding0;
    void *padding1;
#else
//  ROS_EVENT  *sem ;
//  VP_TASK_STACK *task_stack ;
#endif

    /* component data */
    char name[32] ;


    unsigned char numInPins ;
    unsigned char numOutPins ;
    int           tag;
    VP_COMPONENT_STATE  state ;

    VP_IO_PIN  *pin ;
    void       *data ;
    SP_INFO *pSpInfo;
#if SP_DATA_STRUCTURE_SIZE_SYNC_WITH_VO
#ifdef RPC_SUPPORT_MULTI_CALLER
    unsigned long sysPID;
#endif
#endif
} ;

typedef struct
{
    unsigned char  time_out ;     /* in seconds */
    unsigned char  pixel_depth ;  /* 1: 2-bit; 2: 4-bit; 3: 8-bit */
    unsigned char  st_mode ;      /* 0: block mode; 1: sequential mode */
    unsigned char  last_in_page ; /* 1 if the region is the last one in the region list of a display page */

    unsigned short start_x ;
    unsigned short start_y ;
    unsigned short width ;
    unsigned short height ;
    unsigned short pitch ;
    unsigned int   pixel_addr ;  /* absolute DRAM address of the first pixel */
    unsigned int  *CLUT ;        /* receiver must copy the CLUT contents upon receival of this object */
    VP_BUFFER     *buffer ;

    unsigned short video_width ; /* fullWidth and fullHeight for BD subtitle */
    unsigned short video_height ;
    long long      PTS ;

} VP_OBJ_DVB_SP_REGION ;

typedef struct
{
    long filter ;
    long filter_pinID ;
} Connectable_Param ;

typedef struct
{
    VP_COMPONENT *comp ;
    int comp_pinID ;
} Connectable ;


typedef struct tagFILTER FILTER ;
struct tagFILTER
{
    // do not reorder the part. this part should be the same as the beginning of the filter.
    unsigned char numPins ;
    char taskID ;

    int (*Receive)     (VP_COMPONENT *, VP_STREAM_PACKET *recvPkt, int pin,     int bSuspend) ;
    int (*Bind)        (VP_COMPONENT *, VP_COMPONENT *boundComp,   int thisPin, int boundPin) ;
    // end of non-reorder part

    HRESULT (*Run)		        (FILTER *) ;
    HRESULT (*Stop)		        (FILTER *) ;
    HRESULT (*Pause)          (FILTER *) ;
    HRESULT (*Destroy)        (SP_INFO*, FILTER *) ;
    HRESULT (*Flush)	        (SP_INFO*, FILTER *) ;
    HRESULT (*SetRefClock)    (FILTER *, long pRefClock) ;
    HRESULT (*InitRingBuffer) (SP_INFO *, FILTER *, RINGBUFFER_HEADER *ring) ;

    /* from system software's view, they command filters connect.
       However, the basic concept of connection is built between VP_COMPONENT
       We design a function to get the connectable VP_COMPONENT
       FILTER should return a connectable VP_COMPONENT according to asker filter's type.
       The selection is up to each filter's internal implementation. */
    HRESULT (*getConnectable) (Connectable_Param* self, Connectable_Param* asker, Connectable* con) ;
    HRESULT (*ProcCmd)        (FILTER *, VP_STREAM_PACKET *cmd) ;

    /* filter attribute */
    VF_STATUS      status ;
    VIDEO_VF_TYPE  type ;
    VP_COMPONENT   *des_comp; /*pointer where current filter connect to next component,
                             Note: only componet can connect each other */
    int            des_pinNum; /*pin number that connect to the destination*/
    int            tag;
    unsigned char  ringbuf_counter;
    RINGBUFFER_HEADER ringbuf[5];

#if SP_DATA_STRUCTURE_SIZE_SYNC_WITH_VO
    void *padding;
#else
//  ROS_EVENT*      rpc_sem;  /* Semaphore to prevent RPC re-entrance */
#endif
} ;

typedef struct
{
    FILTER  filter ;

    SP_STREAM_TYPE  streamType ;
    VP_COMPONENT    decComp ;
//  VO_CHANNEL     *voChannel ;

    int mtagDecoder ;

} FILTER_SPDEC ;

class SP_INFO
{
public:
    SP_INFO(void *);
    ~SP_INFO();
    MEM_BLK_INFORMATION  memblk_info[MAX_TAG_NUM] ;
    SP_RINGBUF BS_ring;
    SP_RINGBUF ICQ_ring;
    SP_RINGBUF RPC_ring;
    FILTER_SPDEC *pFilter;
    pthread_t nThread;
    VIDEO_RPC_SUBPIC_DEC_CONFIGURE nConfig;
    RINGBUFFER_HEADER *BS_HDR;  // endian change
    RINGBUFFER_HEADER *ICQ_HDR; // endian change
    int nSem;
    int bGetDestoryCmd;
    int RPC_buf[RPC_BUF_SIZE];  // for RPC_ring

    /* ION */
    ION_DATA m_ion[SP_ION_SEG_NUM];
    unsigned int ion_cur_seg;
    unsigned int ion_used_size[SP_ION_SEG_NUM];
    /* ION */
    
    unsigned char *BS_ring_base;
    unsigned char *ICQ_ring_base;
    unsigned int alwaysOnTop;
    unsigned int spFullWidth;
    unsigned int spFullHeight;
    SP_DECODE_TYPE codec_type;
    void *pVO_Filter;   // this address is big-endian, it doesn't need to do endian_change when send to VO.
    void *pRVSD;
    int   *pWaitUnlockByVO;    /* for blocking, when subtitle-decoder send decoded-object to VO. */
    int BS_ring_size;
    int ICQ_ring_size;
    int BS_fd;
    int ICQ_fd;
    int BS_base_fd;
    int ICQ_base_fd;
    int nAbnormalExit;	// bitwise operations
    int bInitRingBuf;
    int used_tag_number;
    long FlushBSWp;
    long FlushICQWp;

    // Function
#if SP_USE_ION_MEMORY
    void *VP_malloc_func(unsigned int size);
#endif
    void VP_memblk_init(void);
    int VP_memblk_request (unsigned int size, int owner_id);
    void *VP_memblk_alloc (unsigned int size, int tag_id);
    void VP_memblk_release (int tag_id);
    void SP_IONUseDefaultDecoder();
    void SP_IONReuseDecoder();
    void SP_SuspendTask(int type);
    void SP_ForceResume();
    void SP_ResumeTask(int type);
    void VP_ReleaseMem (VP_BUFFER *buf);
    void SP_UpdateICQWp();
    void SP_UpdateBSWp();
    void SP_UpdateICQRp();
    void SP_UpdateBSRp();
    void VP_memblk_free (void* ptr, int tag_id);
    void VP_memblk_reset (int tag_id);
    long SP_ToMipsUncacheAddrFunc(unsigned char *pVirt, const char *func, int line);
    unsigned char * SP_Virt2Phy(unsigned char *pVirt);
    int SP_SetVOFilter(void *filter);
    void SP_WaitVOUnlock(int usec, char *);
    void SP_EnqueueRPCRing_func(SP_RPC_TYPE rpc_type);
    int SP_ExtractBits(
        int bs_base, // unit: byte << 3
        int bs_limit,// unit: byte << 3
        int *bs_rp,   // unit: byte << 3
        int bs_wp,    // unit: byte << 3
        const int nNumBits);
    unsigned int SP_PeekBits(
        int bs_base, // unit: byte << 3
        int bs_limit,// unit: byte << 3
        int *bs_rp,   // unit: byte << 3
        int bs_wp,    // unit: byte << 3
        const int nNumBits);
    void SP_flush_ion(int tag);
    void SP_invalidate_ion(int tag);
#if SP_TRACE_ENABLE
    void SP_PrintRingPtrNow();
#endif
#if SP_CHECK_VIRT_ADDR_RANGE
    void SP_CheckVirtAddrValidFunc(unsigned char *pVirt, const char *func, int line);
#endif
#if SP_DBG_BS_RING
    void SP_PrintRingPtr_func();
#endif
protected:
private:
//    pthread_mutex_t nMutex;
//    pthread_cond_t nCond;
};


/*
typedef struct
{
  FILTER       filter ;
  VP_COMPONENT vout ;

} FILTER_VOUT ;*/

/*-----------------------------
FUNCTION PROTOTYPE
------------------------------*/
void ICQ_UpdateReadPtr (VP_INBAND_RING_BUF *pVIRB, int size);
void *ICQ_GetCmd (void *p, int p_size, VP_INBAND_RING_BUF *pVIRB, int bUpdateReadPtr);
void VP_InitPool(VP_BUFFER *pool, int pool_size, void *large_buffer, int small_buffer_size, unsigned char taskID);
VP_BUFFER*  VP_AllocateMem(VP_BUFFER *pool, int pool_size);
int SP_Decoder_Create (SP_INFO* pSpInfo, VP_COMPONENT *_this, int mtag);
int DVBSP_Decoder_Create (SP_INFO *, VP_COMPONENT *_this, int mtag);
int BDSP_Decoder_Create (SP_INFO *, VP_COMPONENT *_this, int mtag);
void VP_memblk_free (void* ptr, int tag_id);
FILTER *FILTER_SPDEC_Create(SP_INFO *pSpInfo);
int RVSD_SP_Create(SP_INFO *);
int RVSD_SP_GetRingBufFileHandler(SP_INFO *pSpInfo, int client_sockfd);
HRESULT FILTER_SPDEC_Run (FILTER *f);
HRESULT *VIDEO_RPC_SUBPIC_DEC_ToAgent_Configure_RVSD (SP_INFO *pSpInfo, VIDEO_RPC_SUBPIC_DEC_CONFIGURE *argp, RPC_STRUCT *rpc, HRESULT* retval);
HRESULT FILTER_SPDEC_Stop (FILTER *f);
HRESULT FILTER_SPDEC_Flush (SP_INFO*, FILTER *f);
HRESULT FILTER_SPDEC_Destroy (SP_INFO*, FILTER *f);
void* SP_Run_thread(void *data);
void SP_Swap4Bytes(char *p, int num);
long long SP_EndianChangeLL(long long *p);
void SP_Print_DVB(VIDEO_VO_OBJ_DVB_SP_REGION *p, int , int);
void SP_Print_DVD(VIDEO_VO_OBJ_DVD_SP *p, int);
unsigned int rvsd_vo_getVOFilterAddr(void);
unsigned int rvsd_vo_GetVOFilter(void*  p);
int rvsd_check_vo_state(void *p);
void SP_DVB_PrintData(unsigned char *pData, int pitch, int height, int);
int VIDEO_RPC_VOUT_QUERY_RVSD(SP_INFO *pSpInfo, int queryWhat, int arg0, int arg1);
void SP_PrintWord(char *name, unsigned int *p, int num);
int RVSD_SP_InitRingBuf_disable(int);
void rvsd_set_sp_null(void *p);
void VIDEO_RPC_SUBPIC_DEC_ToAgent_FlushSubPicture_RVSD(SP_INFO *pSpInfo);
void rvsd_set_sp_thread(void *p, pthread_t nThread);
void DBG_SP_DumpBS(SP_RINGBUF *pRing);
long long SP_get_master_pts(void *p);
namespace android{
int64_t GetLocalPTS(REFCLOCK_t* m_pRefClock);
}

#endif


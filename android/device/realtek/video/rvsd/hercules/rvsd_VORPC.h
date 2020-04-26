#ifndef RVSD_VORPC_H
#define RVSD_VORPC_H

#include "hresult.h"
#include "RPCBaseDS_data.h"
#include "VideoRPCBaseDS.h"
#include "VideoRPC_System.h"
#include "AudioRPC_System.h"
#include "AudioRPC_Agent.h"
#include "VideoRPC_Agent.h"
#include "OSAL_RTK.h"
#include "SVCFunc.h"
#include "RPCProxy.h"
#include "rvsd_OSAL.h"

#include <sys/mman.h>
//#define PLOCK_VERSION           (0x72746B30) //'rtk0'
//#define PLOCK_VERSION           (0x72746B31) //'rtk1' = rtk0+fps
//#define PLOCK_VERSION           (0x72746B32) //'rtk2' = rtk1+IsForceDIBobMode
//'rtk3' = rtk2+lumaOffTblAddr+chromaOffTblAddr+lumaOffTblAddrR+chromaOffTblAddrR+bufBitDepth+VUI+Mastering display colour volume SEI
//#define PLOCK_VERSION           (0x72746B33)

#if defined(RVSD_SUPPORT_TECHNICOLOR)
#define PLOCK_VERSION             (0x72746B36) //'rtk6'
#else
#define PLOCK_VERSION             (0x72746B34) //'rtk4' = rtk3+Y_addr_prev/U_addr_prev/Y_addr_next/U_addr_next/video_full_range_flag/matrix_coefficients
#endif

#define PLOCK_FREE              0
#define PLOCK_EXIST             1

#define PLOCK_BUFFER_SET_SIZE   (32) //bytes
#define PLOCK_BUFFER_SET        (2)  // 2 set of PLock buffer for sequence change
#define PLOCK_MAX_BUFFER_INDEX  (PLOCK_BUFFER_SET_SIZE*PLOCK_BUFFER_SET) //bytes  // seperate to 2 set of PLock buffer for sequence change
#define PLOCK_BUFFER_SIZE       (PLOCK_MAX_BUFFER_INDEX*2) //bytes

#define PLOCK_INIT              0xFF
#define PLOCK_QPEND             0
#define PLOCK_RECEIVED          1

#define YUV_STATE_VERSION3      (0x72746b33) //rtk3
#define YUV_V3_SIZE             (sizeof(YUV_STATE))

enum
{
    PLOCK_STATUS_INIT    = 0,       // 0  / PLOCK_RESET
    PLOCK_STATUS_QPEND   = 1,       // 0  / PLOCK_QPEND
    PLOCK_STATUS_LOCK    = 2,       // >0 / PLOCK_RESET || PLOCK_QPEND || PLOCK_RECEIVED
    PLOCK_STATUS_UNLOCK  = 3,       // 0  / PLOCK_RECEIVED
    PLOCK_STATUS_ERR	 = 4        // else
};

#define  VBI_ASPECT_RATIO_16_BY_9                7
#define  VBI_ASPECT_RATIO_4_BY_3                 8
#define  VBI_ASPECT_RATIO_14_BY_9               14

#if defined(RVSD_VO_WRITEBACK) || defined(RVSD_USE_VO_SCALE_CMD) || defined(RVSD_USE_VO_SCALE_AUTO)
//#define DUMP_VO_WRITEBACK_FRAME
#define WB_PINID                0x20140507
#define WBINBAND_VERSION        0x72746b30 //rtk0
#define WBINBAND_ENTRYNUM       10
#define WBBUFFER_MALLOC_SIZE    64*1024 //64K temporarily
//#define WRITEBACK_WIDTH         1280
#define WRITEBACK_HEIGHT        720
//#define WRITEBACK_BUFSIZE       (WRITEBACK_WIDTH * WRITEBACK_HEIGHT * 2) //writeback is 422
#define WRITEBACK_BUFNUM        6

#define SCALE_BUFFER_4K_WIDTH   1920
#define SCALE_BUFFER_4K_HEIGHT  1080
#endif

#define RVSD_ALIGN(x, y)     ((x + y - 1) & ~(y - 1))
#define RVSD_ALIGN16(x)      RVSD_ALIGN(x, 16)

#define VORPC_PRINT_DEBUG
#ifdef VORPC_PRINT_DEBUG
//#define VORPC_PRINT(fmt, args...)   printf("[RVSD_VORPC]%s:%d" fmt, __func__, __LINE__, ##args)
#define VORPC_PRINT(fmt, args...)   printf("[RVSD_VORPC]%s:" fmt, __func__, ##args)
#else
#define VORPC_PRINT(fmt, args...)
#endif

enum {
    ENUM_VO_RESET = 0,
    ENUM_VO_INIT,
    ENUM_VO_READY,
    ENUM_VO_UNINIT,
};


class RVSD_INFO ;

struct VORPCGlobal
{
    unsigned char bVOReady;
    unsigned int out_id;
    int ion_fd;
    CLNT_STRUCT clnt;
    VIDEO_RPC_VOUT_SET_MIXER_ORDER  mixer;

    OSAL_RTKALLOCATOR   *pICQRingBuffer;
    RINGBUFFER_HEADER   *m_pICQBufferHeader;
    unsigned char       *m_pICQBufferLower;
    unsigned char       *m_pICQBufferUpper;

#ifdef RVSD_VO_WRITEBACK
    OSAL_RTKALLOCATOR   *pWBRingBuffer;
    RINGBUFFER_HEADER   *m_pWBBufferHeader; //virtual address of ring buffer header
    unsigned char       *m_pWBBufferLower;  //start virtual address of ring buffer
    unsigned char       *m_pWBBufferUpper;  //end virtual address of ring buffer
    int                 wbMmapFd;
    ion_user_handle_t   wbIonHandle;
    unsigned char       *wbVirtAddr;
    unsigned long       wbPhysAddr;
    unsigned int        wbInit;             //0:uninit 1:tvsystem 2:ringbuffer 3:wbbuffers
    int                 wbWidth;
    int                 wbHeight;
    unsigned long       addrInd[WRITEBACK_BUFNUM];
    unsigned int        wbFInd;
    struct timeval      timeStamp;
    unsigned int        wbFCnt;
    int                 wbbufnum;
#endif

    unsigned char *pLock_ion_phyaddr;
    unsigned char *pLock_ion_viraddr;
    unsigned char *pReceived_ion_phyaddr;
    unsigned char *pReceived_ion_viraddr;

    int pLock_mmap_fd;
    ion_user_handle_t pLock_ion_handle;
    unsigned int pLock_ion_size;
    unsigned char pLock_exist;

    pthread_mutex_t vo_inband_mtx;
    pthread_mutex_t vo_plock_mtx;
#ifdef VIDEO_FIRMWARE_ON_SCPU
	OSAL_RTKALLOCATOR   *pPlockBuffer;
#endif

#if defined(RVSD_USE_VO_SCALE_CMD) || defined(RVSD_USE_VO_SCALE_AUTO)
    int voScaleWidth;
    int voScaleHeight;
    int voScaleBufSize;
    ion_user_handle_t voScaleBufIonHandle;
    int voScaleBufmmapFd;
    unsigned long voScaleBufPhysAddr;
    unsigned int voScaleBufIonSize;
    unsigned char *voScaleBufVirtAddr;
    unsigned char *voScaleBufPLockAddr;
#endif  // #if defined(RVSD_USE_VO_SCALE_CMD) || defined(RVSD_USE_VO_SCALE_AUTO)
};

int rvsd_vo_rpc_ready(RVSD_INFO *pRvsdInfo);
HRESULT rvsd_vo_init(RVSD_INFO *pRvsdInfo);
HRESULT rvsd_vo_initICQRingBuffer(RVSD_INFO *pRvsdInfo);
HRESULT rvsd_vo_config(RVSD_INFO *pRvsdInfo);
//HRESULT rvsd_vo_uninit(void);
HRESULT rvsd_vo_uninit(RVSD_INFO *pRvsdInfo);
unsigned int process_InbandCmd(RVSD_INFO *pRvsdInfo, int cmd_type, void *packet, unsigned int packet_size_in_bytes);
unsigned int rvsd_vo_sendInbandCmd(void* p, void *cmd_endian_swapped, unsigned int cmd_size);
void rvsd_vo_ready(RVSD_INFO *pRvsdInfo);

#ifdef RVSD_VO_WRITEBACK
HRESULT rvsd_vo_setTVSystemForDefault(RVSD_INFO *pRvsdInfo);
HRESULT rvsd_vo_setTVSystemForWriteBack(RVSD_INFO *pRvsdInfo);
HRESULT rvsd_vo_initWriteBackRingBuffer(RVSD_INFO *pRvsdInfo);
HRESULT rvsd_vo_initWriteBackBuffers(RVSD_INFO *pRvsdInfo, int width, int height);
HRESULT rvsd_vo_getWriteBackFrame(RVSD_INFO *pRvsdInfo, YUV_STATE *frame);
HRESULT rvsd_vo_SyncWriteBackBuffers(RVSD_INFO *pRvsdInfo, int width, int height);
#endif

void rvsd_vo_PLockSetStatus(RVSD_INFO *pRvsdInfo, unsigned int index, unsigned char status);
unsigned char rvsd_vo_PLockGetStatus(RVSD_INFO *pRvsdInfo, unsigned int index);
unsigned int rvsd_vo_PLockGetPhyAddr(RVSD_INFO *pRvsdInfo, unsigned int index);
unsigned int rvsd_vo_PLockGetReceivedPhyAddr(RVSD_INFO *pRvsdInfo, unsigned int index);
unsigned int rvsd_vo_PLockGetVersion();

void _dbg_check_vo_inband_queue(OSAL_RTKALLOCATOR *alloc);

#if defined(RVSD_USE_VO_SCALE_CMD) || defined(RVSD_USE_VO_SCALE_AUTO)
HRESULT rvsd_vo_allocFBForVOScale(RVSD_INFO *pRvsdInfo, int scaleWidth, int scaleHeight, int bufSize, bool bIs10bit, int nv21, int linearStride);
void rvsd_vo_turnOffVOScale(RVSD_INFO *pRvsdInfo);
bool rvsd_vo_checkVOScaleBufUnlock(RVSD_INFO *pRvsdInfo);
void rvsd_vo_freeFBForVOScale(RVSD_INFO *pRvsdInfo);
#endif  // #if defined(RVSD_USE_VO_SCALE_CMD) || defined(RVSD_USE_VO_SCALE_AUTO)


void rvsd_vo_set_plane(RVSD_INFO *p_rvsd_info, VO_VIDEO_PLANE plane);

#endif

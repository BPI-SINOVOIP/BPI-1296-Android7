#ifndef RVSD_VDECMPEG2_H
#define RVSD_VDECMPEG2_H

#include "rvsd_ringbuffer.h"
#include "rpc_common.h"

///////////////////
// Debug Contorl //
///////////////////
#define MPEG2_TRACE_ENABLE 0
#define MPEG2_PRINT_CC_RINGBUF_EN 0
#define MPEG2_CC_DUMP_DATA_EN 0

////////////
// Define //
////////////
#define MPEG2_USER_DATA_START_CODE 0x000001B2
#define MPEG2_USER_IDENTIFIER 0x47413934 // GA94
#define MPEG2_USER_DATA_START_CODE_LE 0xB2010000
#define MPEG2_USER_IDENTIFIER_LE 0x34394147 // GA94
#define USER_DATA_MIN 10
#define RTK_CC_SYNC 0x52744B63  // RtKc
#define RTK_CC_HEADER_SIZE 16
#define MPEG2_CC_REG_FRAME_MAX  20 // the maximum of VPU register frames
#define MPEG2_CC_RINGBUF_SIZE 2048
#define MPEG2_CC_FRAME_DISPLAY_ORDER 1  // transfer CC bitstream by display-order
#define VPU_CC_BUF_SIZE 512


///////////
// Macro //
///////////
#if MPEG2_TRACE_ENABLE
#define MPEG2_TRACE(fmt, args...)   ALOGD("[MPEG2]%s:" fmt, __FUNCTION__, ##args);
#else
#define MPEG2_TRACE(format, ...)
#endif
#define MPEG2_WARNING(fmt, args...)   ALOGD("[MPEG2 WARN]%s:" fmt, __FUNCTION__, ##args);
#define MPEG2_LOG(fmt, args...)   ALOGD("[MPEG2]%s:" fmt, __FUNCTION__, ##args);
#if MPEG2_CC_DUMP_DATA_EN
#define MPEG2_CC_DUMP_DATA(a, b) MPEG2_CC_DumpRingBuf(a, b)
#else
#define MPEG2_CC_DUMP_DATA(a, b) 
#endif

//////////
// enum //
//////////
enum
{
    ENUM_CC_MPGE2=1,
    ENUM_CC_H264,
    ENUM_CC_H265
};

////////////////////
// Data Structure //
////////////////////
class RVSD_MPEG2_INFO
{
public:
    RVSD_MPEG2_INFO();
    ~RVSD_MPEG2_INFO();

    // member variable
    RVSD_RINGBUF cc_ring_buf;   // share memory with AP
    RVSD_RINGBUF m_RingBuf; // local ring buffer
    int CC_fd;
    int CC_base_fd;
    RINGBUFFER_HEADER* CC_HDR;  // cc ring buffer
    unsigned char* CC_ring_base;    // cc ring buffer
    unsigned int CC_ring_size;  // cc ring buffer
    unsigned char *pBuf;
    int m_CCDecodeOrderWp[MPEG2_CC_REG_FRAME_MAX];
    unsigned int cc_frame_cnt;
    unsigned char *pVPU_cc_buf;
    char bTTAK_KO_CC;
    
    // member function
    unsigned int MPEG2_FindSyncCC(unsigned long BSbase, unsigned long BSlimit, unsigned long frameStart,unsigned long frameEnd);
#ifdef VPU_GET_CC
    unsigned int MPEG2_ProcessCC(unsigned char*, unsigned int cc_size, long long PTS, int decode_index, int display_index, int);
#else
    unsigned int MPEG2_ProcessCC(unsigned long BSbase, unsigned long BSlimit, unsigned long frameStart,unsigned long frameEnd, long long, int, int);
#endif
    inline long MPEG2_FindSyncCC_SubFunc1(RVSD_RINGBUF *pRing, unsigned long pStart, unsigned long data_size, long *search_bytelen);
    inline unsigned int MPEG2_FindSyncCC_SubFunc0(unsigned char *p, unsigned long loop_count, long *search_bytelen);
    int MPEG2_GetCCRingBuf(int client_sockfd, unsigned char *);
    void MPEG2_UpdateCCWp();
    void MPEG2_UpdateCCRp();
    int MPEG2_ProcessCC_TTAK_KO(unsigned char *pUserData/* point to "ATSC identifier" */,  int nUserDataSize);
    int H264_ProcessCC_TTAK_KO(unsigned char *pUserData,int nUserDataSize);
};
#endif  // #ifndef RVSD_VDECMPEG2_H



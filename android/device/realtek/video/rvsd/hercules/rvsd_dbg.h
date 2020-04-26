#ifndef RVSD_DBG_H
#define RVSD_DBG_H

#include "vputypes.h"
#include "rvsd_ringbuffer.h"

class RVSD_INFO;

///////////////////
// debug control //
///////////////////
#define _DBG_RVSD_TRACE_EN 0
#define _DBG_RVSD_TRACE_TERMINATE -1    // only print information in the first _DBG_RVSD_TRACE_TERMINATE frames
                                        // -1: infinite.

#define _DBG_RVSD_TRACE_FRAME_PTS_EN 0

#define _DBG_TRACE_RVSD_FLOW_EN 0

#define _DBG_DUMP_BS_EN 0   // dump file bitstream
#define DUMP_FRAME_NUM -1   // -1:infinite, others:max frames
#define MAX_DUMP_BS_BYTES 0 // maximum dump BS bytes, 0 means no limit. Only support in rvsd new flow.

#define _DBG_DUMP_SPECIFIC_FRAME_BS_EN 0
#define _DBG_DUMP_SPECIFIC_FRAME_BS 61  // start from frameIdx=0

#define _DBG_DUMP_H264_USERDATA_EN 0  // dump H264 user data

#define _DBG_UPDATE_RP_WITHOUT_DECODE_EN 0   // update rp of video-bitstream-ringBuf without decode

#define _DBG_CHECK_VO_INBAND_QUEUE_EN 0 // check VO inband-queue status

#define _DBG_GET_FB_STATUS_EN 0

/* check VPU decode frame status:
1. in WAIT_DECODING_DONE, print any return-situation
2. print any decoded error information. */
#define _DBG_CHECK_IF_DECODE_HAS_ERR 0

#define _DBG_GET_FRM_BYTE_OFFSET_EN 0 //get every frame's byte offset relative to new seg wptr. sometimes need this info to compare certain frm's behavor with PC.

#define _DBG_CHECK_SEND_VO_FRAME_COUNT 0

// Enable store YUV data in frame buffer. The implementation is copied from main_helper.c and cnm_video_helper.c.
// If HEVC 4K, the YUV data is very big and if store to /tmp, it is easy to cause system crash.
// If it intends to store in /data or usb disk, it is necessary to modify init.rc to let mediaserver process have permission to write to the target.
#define _DBG_DUMP_FRAME_BUFFER_EN 0

#define _DBG_PRINT_VPU_STATUS_EN 0

#define _DBG_DUMP_MD5 0

#define _DBG_CHECK_INIT_TO_1ST_FRAME_SPEND_TIME 0
#define MAX_CHECK_TIME_INTERVAL_NUM 20
#define MAX_CHECK_TIME_STR_LEN 32

////////////////
// Definition //
////////////////

///////////
// Macro //
///////////

#if 1
#define _DBG_AT()   ALOGD("[DBG RVSD @ %s %d]\n", __FUNCTION__, __LINE__);
#define _DBG_RVSD(fmt, args...)   ALOGD("[DBG RVSD]%s:" fmt, __FUNCTION__, ##args);
#else
#define _DBG_AT()
#define _DBG_RVSD(fmt, args...)
#endif

#if _DBG_CHECK_VO_INBAND_QUEUE_EN
#define _DBG_CHECK_VO_INBAND_QUEUE(a) _dbg_check_vo_inband_queue(a)
#else
#define _DBG_CHECK_VO_INBAND_QUEUE(a)
#endif

#define _DBG_DUMP_PIC_DONE(p) {\
    p->bDumpEn = 0;\
    p->bDumpPicDone = RVSD_ENDIAN_CHANGE(1);}

#if _DBG_UPDATE_RP_WITHOUT_DECODE_EN
#define _DBG_UPDATE_RP_WITHOUT_DECODE(pVpu, pRvsdInfo) {\
    unsigned int rp, wp, room;\
    VPU_DecGetBitstreamBuffer((pVpu)->handle, &rp, &wp, &room);\
    VPU_DecSetRdPtr((pVpu)->handle, wp, 0);\
    (pVpu)->bufferEmtpyFlag = 1;\
    (pVpu)->bufferEmtpyWPtr = wp;\
    (pVpu)->nReciPtsRPtr = (pVpu)->nReciPtsWPtr;\
    pli_IPCWriteULONG((BYTE*)&pRvsdInfo->rvsd_BSHDRBase->readPtr[0], rp);\
    VPU_DecGetBitstreamBuffer((pVpu)->handle, &rp, &wp, &room);\
    RVSD_WARNNING_SEC(1, "[々々 RVSD: _DBG_UPDATE_RP_WITHOUT_DECODE w 0x%x r 0x%x]\n", wp, rp);\
    return VDEC_BUFFER_EMPTY;\
}
#else
#define _DBG_UPDATE_RP_WITHOUT_DECODE(pVpu, pRvsdInfo)
#endif

#if _DBG_DUMP_BS_EN
#define _DBG_DUMP_BS_NO_SHARE_RINGBUF(a, b) _dbg_dump_bs_NO_share_ringbuf(a, b)
#define _DBG_DUMP_BS_WITH_SHARE_RINGBUF(a) _dbg_dump_bs_with_share_ringbuf(a)
#define _DBG_DUMP_BS_NEW(a, b)             _dbg_dump_bs_new(a, b)
#else
#define _DBG_DUMP_BS_NO_SHARE_RINGBUF(a, b)
#define _DBG_DUMP_BS_WITH_SHARE_RINGBUF(a)
#define _DBG_DUMP_BS_NEW(a, b)
#endif

#if _DBG_DUMP_SPECIFIC_FRAME_BS_EN
#define _DBG_DUMP_FRAME_BS(a) _dbg_dump_frame_bs(a)
#else
#define _DBG_DUMP_FRAME_BS(a)
#endif

#if _DBG_DUMP_H264_USERDATA_EN
#define _DBG_DUMP_H264_USERDATA(a,b)  _dbg_dump_h264_userdata(a,b)
#else
#define _DBG_DUMP_H264_USERDATA(a,b)
#endif

#if _DBG_GET_FRM_BYTE_OFFSET_EN
#define _DBG_GET_FRM_BYTE_OFFSET_INIT(a)  _dbg_get_frm_byte_offset_init(a)
#define _DBG_GET_FRM_BYTE_OFFSET(a,b)     _dbg_get_frm_byte_offset(a,b)
#else
#define _DBG_GET_FRM_BYTE_OFFSET_INIT(a)
#define _DBG_GET_FRM_BYTE_OFFSET(a,b)
#endif

#if _DBG_TRACE_RVSD_FLOW_EN
#define _TRACE_RVSD()   ALOGD("[々々 RVSD @ %s %d]\n", __FUNCTION__, __LINE__);
#define _TRACE_RVSD2(fmt, args...)   ALOGD("[々々 RVSD]%s:"fmt, __FUNCTION__, ##args);
#else   // #if _DBG_TRACE_RVSD_FLOW_EN
#define _TRACE_RVSD()
#define _TRACE_RVSD2(fmt, arg...)
#endif  // #if _DBG_TRACE_RVSD_FLOW_EN

#if _DBG_RVSD_TRACE_FRAME_PTS_EN
#define _DBG_PRINT_PTS ALOGD
#define _DBG_PRINT_PTS_FILL_BS(pVpu, paRdPtr, paWrPtr, nStreamLen, nTimeStamp) _DBG_PRINT_PTS("[々DEC: fillBS paRdPtr:0x%x, paWrPtr:0x%x, nReciPtsRPtr:%d, nReciPtsWPtr:%d, nChunkSize:%d, nTimeStamp:%f]\n" \
    , paRdPtr, paWrPtr, pVpu->nReciPtsRPtr, pVpu->nReciPtsWPtr, nStreamLen, (long long)nTimeStamp/1E6);
#define _DBG_PRINT_PTS_CALCUALTE_PTS(pVpu, curPtr, nextPtr, nTimeStamp) _DBG_PRINT_PTS("[々DEC: idx %d PtsPtr[%d(0x%x,0x%x), %d], rdPtr:0x%x, wrPtr:0x%x, nTimeStamp:%f\n",pVpu->outputInfo.indexFrameDecoded \
        , pVpu->nReciPtsRPtr, curPtr, nextPtr, pVpu->nReciPtsWPtr,rdPtr, pVpu->outputInfo.wrPtr, (long long)nTimeStamp/1E6);
#else   // #if _DBG_RVSD_TRACE_FRAME_PTS_EN
#define _DBG_PRINT_PTS(fma...)
#define _DBG_PRINT_PTS_FILL_BS(pVpu, paRdPtr, paWrPtr, nStreamLen, nTimeStamp)
#define _DBG_PRINT_PTS_CALCUALTE_PTS(pVpu, curPtr, nextPtr, nTimeStamp)
#endif  // #if _DBG_RVSD_TRACE_FRAME_PTS_EN

#if _DBG_RVSD_TRACE_EN
#define _DBG_DEC_INIT() _dbg_init()
#define _DBG_DEC_GET_BS(a) _dbg_get_BS(a)
#define _DBG_SEND_FRAME_TO_VO(a, b) _dbg_send_frame_to_vo(a, b)
#define _DBG_DEC_TRACE_FRAME(pVpu, pts) {\
    long long curPTS = getPTS(); \
    long long diffPTS; \
    long ms; \
    char pic_type[40]; \
    switch(pVpu->outputInfo.picType) \
    { \
        case PIC_TYPE_I: strcpy(pic_type, "PIC_TYPE_I"); break; \
        case PIC_TYPE_P: strcpy(pic_type, "PIC_TYPE_P"); break; \
        case PIC_TYPE_B: strcpy(pic_type, "PIC_TYPE_B"); break; \
        case PIC_TYPE_IDR: strcpy(pic_type, "PIC_TYPE_IDR"); break; \
        default: pic_type[0] = 48 + pVpu->outputInfo.picType; pic_type[1]='\0'; break; \
    } \
    diffPTS = curPTS - g_dbg_init_pts; \
    ms = diffPTS / 90; \
    if(pVpu->decodedIdx < _DBG_RVSD_TRACE_TERMINATE || _DBG_RVSD_TRACE_TERMINATE == -1) \
        {ALOGD("[々DEC:Frm idx %d disIdx %d decIdx %d picType %s PTS %lld ms %d]\n", pVpu->decodedIdx, pVpu->outputInfo.indexFrameDisplay, pVpu->outputInfo.indexFrameDecoded, pic_type, pts, ms);} \
}

#define _DBG_DEC_PRINT_PTS(pPtsInfo) {\
    long long curPTS = getPTS(); \
    long long diffPTS; \
    long ms; \
    diffPTS = curPTS - g_dbg_init_pts; \
    ms = diffPTS / 90; \
    _DBG_PRINT_PTS("[々DEC:PTS %d wp %x ptsH %x ptsL %x ms %d]\n", g_pts_count, pPtsInfo->wPtr, pPtsInfo->PTSH, pPtsInfo->PTSL, ms); \
    g_pts_count++; \
}


#define _DBG_DEC_FIRST_DECODE(handle) {\
    static long count = 1; \
    if(count) { \
        long long curPTS = getPTS(); \
        long long diffPTS; \
        long ms; \
        PhysicalAddress base, limit, wp, rp; \
        diffPTS = curPTS - g_dbg_init_pts; \
        ms = diffPTS / 90; \
        ALOGD("[DEC: first call VDecStartDecode ms %d]\n", ms); \
        RVSD_GetBSRingBufPhy(handle, &base, &limit, &wp, &rp); \
        ALOGD("[DEC: b %x l %x w %x r %x Data %x]\n", base, limit, wp, rp, wp - rp); \
        count--; \
    } \
}

#define _DBG_DEC_GET_INFO() {\
    long long curPTS = getPTS(); \
    long long diffPTS; \
    long ms; \
    diffPTS = curPTS - g_dbg_init_pts; \
    ms = diffPTS / 90; \
    ALOGD("[DEC: GetInfo ms %d]\n", ms); \
}
#define _DBG_DEC_HAS_FRAME() {\
    long long curPTS = getPTS(); \
    long long diffPTS; \
    long ms; \
    diffPTS = curPTS - g_dbg_init_pts; \
    ms = diffPTS / 90; \
    ALOGD("[DEC: HasFrame ms %d]\n", ms); \
}
#define _DBG_DEC_FAIL() {\
    long long curPTS = getPTS(); \
    long long diffPTS; \
    long ms; \
    diffPTS = curPTS - g_dbg_init_pts; \
    ms = diffPTS / 90; \
    ALOGD("[DEC: Fail ms %d]\n", ms); \
}
#define _DBG_DEC_UNDERFLOW() {\
    long long curPTS = getPTS(); \
    long long diffPTS; \
    long ms; \
    diffPTS = curPTS - g_dbg_init_pts; \
    ms = diffPTS / 90; \
    ALOGD("[DEC: Underflow ms %d]\n", ms); \
}

#else   // #if _DBG_RVSD_TRACE_EN

#define _DBG_DEC_INIT()
#define _DBG_DEC_GET_BS(a)
#define _DBG_SEND_FRAME_TO_VO(a, b)
#define _DBG_DEC_TRACE_FRAME(pVpu, pts)
#define _DBG_DEC_PRINT_PTS(pPtsInfo)
#define _DBG_DEC_FIRST_DECODE(handle)
#define _DBG_DEC_GET_INFO()
#define _DBG_DEC_HAS_FRAME()
#define _DBG_DEC_FAIL()
#define _DBG_DEC_UNDERFLOW()

#endif  // #if _DBG_RVSD_TRACE_EN

#if _DBG_CHECK_IF_DECODE_HAS_ERR
#define _DBG_DEC_ERR(decodedIdx, decodingSuccess, indexFrameDecoded, skipframeMode, numOfErrMBs) {\
    if(((decodingSuccess) & 0x1) == 0 \
        || ((indexFrameDecoded) < 0 && (skipframeMode) != SKIP_NON_REF_FRM) \
        || (numOfErrMBs))\
        printf("[々々 decodedIdx %d err (success 0x%x, idxFrmDecoded %d, numOfErrMBs %d)]\n",decodedIdx, decodingSuccess, indexFrameDecoded, numOfErrMBs);}
#define _DBG_DEC_ERR_RETURN(handle, decodedIdx, int_reason, reason) {\
    PhysicalAddress base, limit, wp, rp;\
    RVSD_GetBSRingBufPhy(handle, &base, &limit, &wp, &rp);\
    printf("[々々 decodedIdx %d return %s int_reason 0x%x(b 0x%x l 0x%x w 0x%x r 0x%x d 0x%x)]\n", decodedIdx, reason, int_reason, base, limit, wp, rp, ring_valid_data(base, limit, rp, wp));\
    }
#else
#define _DBG_DEC_ERR(decodedIdx, decodingSuccess, indexFrameDecoded, skipPicture, numOfErrMBs)
#define _DBG_DEC_ERR_RETURN(handle, decodedIdx, int_reason, reason)
#endif

#if _DBG_DUMP_FRAME_BUFFER_EN
#define _DBG_DUMP_FRAME_BUFFER(a,b,c,d)  _dbg_dump_frame_buffer(a,b,c,d)
#else
#define _DBG_DUMP_FRAME_BUFFER(a,b,c,d)
#endif

#if _DBG_CHECK_INIT_TO_1ST_FRAME_SPEND_TIME
#define _DBG_INSERT_CHECK_TIME(a,b) _dbg_insert_check_time(a,b)
#else
#define _DBG_INSERT_CHECK_TIME(a,b)
#endif

//////////
// ENUM //
//////////

typedef enum {
    ENUM_DBG_UNINIT = 0,
    ENUM_DBG_INIT,
    ENUM_DBG_GET_BS,
    ENUM_DBG_SEND_FIRST_FRAME
} ENUM_DBG_STATE;

enum
{
    ENUM_EXIT_NONE = 0,
    ENUM_ASK_EXIT,
    ENUM_EXIT_DONE
};

////////////
// STRUCT //
////////////

typedef struct  {
    int ion_fd;
    int mmap_fd;
    unsigned char* ion_virt;
    unsigned char* ion_phy;
    ion_user_handle_t ion_handle;
} DBG_ION_DATA;

typedef struct  {
    unsigned int base;
    unsigned int size;
    unsigned int rp;
    unsigned int wp;
} FIRMWARE_DUMP_LOG;

typedef struct  {
    DBG_ION_DATA nION;
    pthread_t nDBGVOThread;
    RVSD_RINGBUF nLogRingVirt;
    RVSD_RINGBUF nLogRingPhy;
    FIRMWARE_DUMP_LOG *pDumpLog;
} RVSD_DBG_VO_DUMP_DATA;

typedef struct  {
    RVSD_DBG_VO_DUMP_DATA *pData;
    int *pExit;
} DBG_THREAD_ARG;

//////////////////////
// extern variables //
//////////////////////
extern long long g_dbg_init_pts;
extern long g_dbg_send_vo_count;
extern long g_pts_count;

////////////////////////
// function prototype //
////////////////////////
void _dbg_init();
//void _dbg_get_BS(DecHandle handle);
//void _dbg_send_frame_to_vo(DecOutputInfo *displayInfo, long long outTimestamp);
void _dbg_dump_bs(unsigned char *p, unsigned int size);
void _dbg_dump_bs_NO_share_ringbuf(unsigned char *p, unsigned int size);
void* _dbg_vo_dump_pic_thread(void *data);
int capture_vo(int vo_plane);
void _dbg_test_ai(int argc, char **argv);
#if _DBG_CHECK_INIT_TO_1ST_FRAME_SPEND_TIME
void _dbg_insert_check_time(RVSD_INFO *pRvsdInfo, char* pStr);
void _dbg_show_check_time_diff(RVSD_INFO *pRvsdInfo);
#endif
int _dbg_rvsd_dump_data(char *pData, int size);
int _dbg_rvsd_dump_finish();

#endif  // #ifndef RVSD_DBG_H



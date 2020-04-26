#ifndef RVSD_VDECRM_H
#define RVSD_VDECRM_H

#include "rvsd_ringbuffer.h"

class RVSD_INFO;

///////////////////
// Debug Contorl //
///////////////////
#define RM_TRACE_EN 0
#define RM_DUMP_SEQ_HEADER 0
#define RM_DUMP_PIC_HEADER 0
#define RM_DBG_PTS_EN 0
#define RVSD_RM_DUMP_SEQ_HEADER_TO_FILE_EN 0


//////////////////
// Main Control //
//////////////////
#define CHECK_MOFTag 1
#define RM_DETERMINE_PIC_TYPE_BY_SW 0  // 1: determine pRVInfo->PicCodType by SW parsing bitstream, 0: dtermine pRVInfo->PicCodType by pVpu->outputInfo.picType
#define RM_DETERMINE_TR_BY_SW 0 // TR(temporal reference), 1:determine by SW, 0:dtermine by VPU

////////////
// Define //
////////////
#define RM_SUCCESS 1
#define RM_FAIL 0
#define RM_HX_MEDIA_VIDEO   0x5649444F
#define RM_FOURCC_RV20 0x52563230
#define RM_FOURCC_RV30 0x52563330
#define RM_FOURCC_RV40 0x52563430
#define RM_MAX_FORMATS 4
#define RM_MAX_FRAMES 2048
#define RM_MAX_SEGMENTS 4096
#define RM_SEQ_HEADER_STATIC_SIZE 	34
#define RM_PIC_HEADER_STATIC_SIZE 20
#define RM_RAW_BITSTREAM_MINOR_VERSION 128
#define RM_VIDEO_20_MAJOR_BITSTREAM_VERSION  2
#define RM_VIDEO_30_MAJOR_BITSTREAM_VERSION  3
#define RM_VIDEO_40_MAJOR_BITSTREAM_VERSION  4
#define RM_VIDEO_40_SPO_BITS_NUMRESAMPLE_IMAGES    0x00070000  /* max of 8 RPR images sizes */
#define RM_VIDEO_40_SPO_BITS_NUMRESAMPLE_IMAGES_SHIFT 16

#define RV_BITSTREAM_MINOR_VERSION 0
#define RV_BITSTREAM_MINOR_VERSION_RV8 2
#define RV_BITSTREAM_MAJOR_VERSION_RV8 3
#define RV_BITSTREAM_MAJOR_VERSION 4
#define RAW_BITSTREAM_MINOR_VERSION 128

#define RV_DEC_SUCCESS                 0

/* Slice Header syntax element errors */
#define RV_DEC_ERR_SLICE_SYNTAX        -100
#define RV_DEC_ERR_SLICE_REVERSE       -101
#define RV_DEC_ERR_PICT_SYNTAX         -200

#define RV_DEC_ERR_NO_BUFFER           -300
#define RV_DEC_ERR_SCALE_NO_BUFFER     -400
#define RV_DEC_ERR_BITSTREAM           -500
#define RV_DEC_DROP_PIC                -600

#define RV_INTRAPIC         0
#define RV_FORCED_INTRAPIC  1
#define RV_INTERPIC         2
#define RV_TRUEBPIC         3

#define TR_WRAP            256
#define TR_WRAP_RV         8192

///////////
// Macro //
///////////
#define RM_WARNING(fmt, args...)   ALOGD("[RM WARN]%s:" fmt, __FUNCTION__, ##args);

#if RM_DBG_PTS_EN
#define RM_DBG_PTS(fmt, args...)   ALOGD("[RM PTS]%s:" fmt, __FUNCTION__, ##args);
#else
#define RM_DBG_PTS(fmt, args...)
#endif

#if RM_TRACE_EN
#define RM_TRACE() ALOGD("[RM Trace @ %s %d]\n", __FUNCTION__, __LINE__)
#define RM_TRACE2(fmt, args...)   ALOGD("[RM]%s:" fmt, __FUNCTION__, ##args);
#define RM_DBG(fmt, args...)    ALOGD("[RM]%s:" fmt, __FUNCTION__, ##args);
#define RM_DBG2()    ALOGD("[RM @ %s %d]\n", __FUNCTION__, __LINE__);
#else
#define RM_TRACE()
#define RM_TRACE2(fmt, args...)
#define RM_DBG(fmt, args...)
#define RM_DBG2()
#endif

#if RM_DUMP_SEQ_HEADER
#define RV_DUMP_SEQ_HEADER(a,b) RV_DumpSeqHeader(a,b)
#else
#define RV_DUMP_SEQ_HEADER(a,b)
#endif

#if RM_DUMP_PIC_HEADER
#define RV_DUMP_PIC_HEADER(a,b) RV_DumpPicHeader(a,b)
#else
#define RV_DUMP_PIC_HEADER(a,b)
#endif

#if RVSD_RM_DUMP_SEQ_HEADER_TO_FILE_EN
#define RM_DUMP_SEQ_HEADER_TO_FILE() VDecDumpRVSeqHeader2File()
#else
#define RM_DUMP_SEQ_HEADER_TO_FILE()
#endif

#define RM_LOG ALOGD

//#define GET_LE32(p) ( *((unsigned char *)p) | *((((unsigned char *)p)+1) << 8) | *((((unsigned char *)p)+2) << 16) | *((((unsigned char *)p)+3) << 24) )
#define GET_LE32(p) (*((unsigned char *)p) | ((*(((unsigned char *)p)+1)) << 8) | ((*(((unsigned char *)p)+2)) << 16) | ((*(((unsigned char *)p)+3)) << 24))
#define GET_LE16(p) (*((unsigned char *)p) | ((*(((unsigned char *)p)+1)) << 8))

#define GET_BE32(p) (*(((unsigned char *)p)+3) | ((*(((unsigned char *)p)+2)) << 8) | ((*(((unsigned char *)p)+1)) << 16) | ((*(((unsigned char *)p))) << 24))
#define GET_BE16(p) (*(((unsigned char *)p)+1) | ((*(((unsigned char *)p))) << 8))

#if RVSD_RM_CALCULATE_PTS
#define RM_CHECK_FRM_PTS(bitstreamFormat, bIsFound) { if((bitstreamFormat) == STD_RV && (bIsFound) == false) RM_LOG("[RM: PTS NOT 1 by 1 per frame]\n"); }
#else
#define RM_CHECK_FRM_PTS(bitstreamFormat, bIsFound)
#endif

//////////
// ENUM //
//////////

typedef enum
{
    RV_FID_UNDEFINED,
    RV_FID_RGB4,
    RV_FID_CLUT8,
    RV_FID_XRGB16_1555,
    RV_FID_RGB16_565,
    RV_FID_RGB16_655,
    RV_FID_RGB16_664,
    RV_FID_RGB24,
    RV_FID_XRGB32,
    RV_FID_MPEG2V,
    RV_FID_YVU9,
    RV_FID_YUV12,
    RV_FID_IYUV,
    RV_FID_YV12,
    RV_FID_YUY2,
    RV_FID_BITMAP16,
    RV_FID_H261,
    RV_FID_H263,
    RV_FID_H263PLUS,
    RV_FID_RV89COMBO,
    RV_FID_ILVR,
    RV_FID_REALVIDEO21,
    RV_FID_REALVIDEO22,
    RV_FID_REALVIDEO30
} RV_FID;

////////////////////
// Data Structure //
////////////////////

typedef struct  {
    VIDEO_RV_FORMAT_INFO pICQFmtInfo[RM_MAX_FORMATS];
#if RVSD_RM_ENABLE
    VIDEO_RV_FRAME_INFO pICQFrmInfo[RM_MAX_FRAMES];
    VIDEO_RV_SEGMENT_INFO pICQSegInfo[RM_MAX_SEGMENTS];
#endif
    RVSD_RINGBUF nFmtRing;
    RVSD_RINGBUF nFrmRing;
    RVSD_RINGBUF nSegRing;

    // pts start
    long long initialPTS;
    unsigned int PTS_Inc;
    unsigned int initialTR;
    long long LL_CurPTS;
    long long LL_FwdPTS;
    long long LL_BwdPTS;
    unsigned int CurTR;
    unsigned int FwdTR;
    unsigned int BwdTR;
    unsigned int CurCumulativeTR;
    unsigned int FwdCumulativeTR;
    unsigned int BwdCumulativeTR;
    // pts end

    RV_FID fid;
    int PicCodType;    //@@?? the same with DV_Decode_Slice
    unsigned char bIsRV8;
    unsigned char bInitPTS;
    unsigned char is_vobu;
    unsigned char bSkipFirstFrmPTS; // from 1186, NO enqueue the PTS of the first frame.
} RVSD_RV_INFO;

//////////////
// Function //
//////////////
void VDecRVInit(RVSD_RV_INFO *pRVInfo);
void RV_GetFmtInfo(RVSD_INFO* pRvsdInfo,VIDEO_RV_FORMAT_INFO *p);
void RV_GetFrmInfo(RVSD_INFO* pRvsdInfo,VIDEO_RV_FRAME_INFO *p);
void RV_GetSegInfo(RVSD_INFO* pRvsdInfo,VIDEO_RV_SEGMENT_INFO *p, int);
void RV_DumpSeqHeader(RVSD_INFO* pRvsdInfo, unsigned char *p);
void RV_DumpPicHeader(RVSD_INFO* pRvsdInfo, unsigned char *p);
void RV_SetVOBUFlag(RVSD_INFO* pRvsdInfo);
void RV_Reset(RVSD_INFO* pRvsdInfo);
void RV_DisplayFrmPTS (RVSD_INFO* pRvsdInfo, long long *TimeStamp, int indexFrameDecoded, int indexFrameDisplay);
void RM_dump_seq_header_to_file(unsigned char *pSeqHeader);

#endif  // #ifndef RVSD_VDECRM_H



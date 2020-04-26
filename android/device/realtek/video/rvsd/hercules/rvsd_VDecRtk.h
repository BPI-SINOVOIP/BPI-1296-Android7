#ifndef __RVSD_VDEC_RTK_H__
#define __RVSD_VDEC_RTK_H__

#include <media/IMediaPlayerService.h>
#ifdef ANDROID
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/AMessage.h>

#ifndef ANDROID_6
#include <media/stagefright/NativeWindowWrapper.h>
#endif

#include <media/stagefright/MPEG2TSWriter.h>
#include <media/stagefright/MPEG4Writer.h>

#include <private/media/VideoFrame.h>

//#include <gui/SurfaceTextureClient.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <ui/DisplayInfo.h>
#else
#include "foundation/include/hexdump.h"
#include "foundation/include/ABuffer.h"
#include "foundation/include/ADebug.h"
#include "foundation/include/ALooper.h"
#include "foundation/include/AMessage.h"
#endif

#include <media/stagefright/ACodec.h>
#include <media/stagefright/AudioPlayer.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/OMXClient.h>
#include <media/mediametadataretriever.h>

#include <media/stagefright/MediaBufferGroup.h>

#include "RtkTranscodeCommon.h"

#include <utils/RefBase.h>
#include "rvsd_vdec.h"
#include "rvsd_InBandAPI.h"
#include "pthread-macro.h"
#include "rvsd_OSAL.h"
#include "rvsd_ringbuffer.h"
#include "rvsd_dbg.h"
#include "rvsd_VDecRM.h"
#include "rpc_common.h"

//#define RVSD_WRITE_HEADER_ENABLE  // disable it for DvdPlayer try to add seq/pic header

/* bug43881: VPU's frm start is unaligned with DvdPlayer's due to "prefix_nal_unit_svc" */
/*  - workaround method: when decode finish is detected (to make sure it's last frm),   */
/*                       re-send last frame and tag its pts as last pts received.       */
/*  - this workaround is NOT apply for MVC case so far. suppose mvc file won't have     */
/*    this nalu type.                                                                   */
#define BUG43881_WORKAROUND
#define BUG42723_WORKAROUND
#define BUG44230_WORKAROUND
#define BUG45376_WORKAROUND 1
#define FIXBUG_44647
/*
   work around for DHCKYLIN-61
   The decoded frame start position unmatch Wptr of pts table
   that causes the mechanism of pts searching is wrong
   (frame start + 1 = pts table.Wptr)
*/
#define BUGDHCKYLIN_61 1
//#define CHECK_READ_NEW_BS_BEFORE_HANDLE_NEW_VOBU_CMD
#define FIX_READ_NEW_BS_BEFORE_HANDLE_NEW_VOBU_CMD
#define END_Of_VOBU_ENABLED

#if defined(BUG44230_WORKAROUND)
#define WORKAROUND_FRAME_START_BUG
#endif

#if BUGDHCKYLIN_61
#define WORKAROUND_PTS_COMPARE_ISSUE 1
#endif

#define RVSD_WORKAROUND_VC1_FRM_RATE
#define RVSD_WORKAROUND_HEVC_NO_IRAP    // workaround DHCFAE-1061

//#define NEW_INBANDCMD_PROCESS

//#define SWITCH_VIDEOTYPE_RESERVE_LAST_FRAME
#ifdef SWITCH_VIDEOTYPE_RESERVE_LAST_FRAME
// MAX_GDI_IDX and MAX_REG_FRAME are copied from vpuapi.h
#define MAX_GDI_IDX             32 //31 //RTK, because 31 will causes memory overwrite when endian swap
#define MAX_REG_FRAME           MAX_GDI_IDX*2 // 2 for WTL
#include "rvsd_CommonRtk.h"
#define FIXBUG_EXTERNAL_10405_SOLUTION_1     // free frame buffer until VO display next frame buffer
//#define FIXBUG_EXTERNAL_10405_SOLUTION_2   // free frame buffer until need to reserve next frame buffer
#endif

#define CHECK_BROKEN_LINK
#define BITSTREAM_VALIDATION_ENABLED
#define PREPARSE_VIDEO_INFO_ENABLED
#define H264_DISPLAY_FLUSHED_DPB
//#define RVSD_DVDVR_DCI_CCI_CMD_ENABLE
//#define RVSD_QUALITY_LEVEL_ENABLED
//for bug#46984: in FF mode, VPU display in the wrong order due to discontinuous data.
//#define RVSD_FF_FLUSH_DPB_BY_POC_DIFF
//#define RVSD_ERR_NOTIFY_ENABLED
//#define RVSD_ROLLING_I_ENABLED
#define RVSD_MS10_MBIT_ENABLE
#define RVSD_BLOCK_DEC_THREAD_BY_PWAIT_ENABLE
//#define RVSD_DECODE_STATS_ENABLED
#define RVSD_DECODE_STATS_UPDATE_THRESHOLD    5   // seconds
#define RVSD_CONTINUOUS_FRAME_COUNT_NOTIFICATION_ENABLE
#define RVSD_SEQ_CHANGE_FREE_MOST_DPB_EN
//#define RVSD_SHOW_ION_MEDIA_HEAP_CONTENT

#define MAX_TIME_QUEUE 2048
#define MAX_VOBU_QUEUE 512
#define MAX_DVDVR_QUEUE 32

#define SEQ_HEADER_VC1              36
#define SEQ_HEADER_VC1_ADVANCED 128
#define SEQ_HEADER_DIV3         32
#define SEQ_HEADER_VP8              32

#define PIC_HEADER_VC1              8
#define PIC_HEADER_VC1_ADVANCED 4
#define PIC_HEADER_DIV3         12
#define PIC_HEADER_VP8              12

#define VP_VBI_NULL_DATA 0x8080

#define MIN_FULLNESS_FOR_CODA_DECODE         512  /* bytes */
#define MIN_FULLNESS_FOR_WAVE_DECODE         1024  /* bytes */

#ifdef BUG42723_WORKAROUND
#define VIDEO_TYPE_NONE            -1
#define VIDEO_TYPE_FRAME           CONSECUTIVE_FRAME
#define VIDEO_TYPE_FLD_FRM         100
#endif

#define ERROR_END_OF_VOBU_SEG       (ERROR_END_OF_STREAM-100)
#define ERROR_IGNORE_READ           (ERROR_END_OF_STREAM-101)

#define CALCULATEPTS_RES_NOT_FOUND                  -1
#define CALCULATEPTS_RES_IGNORE                     -2
#define CALCULATEPTS_RES_MULTI_FRM_MAP_THESAMEPTS   -3

#define PUT_BYTE(_p, _b) \
    *_p++ = (unsigned char)_b;
#define PUT_BUFFER(_p, _buf, _len) \
    memcpy(_p, _buf, _len); \
    _p += _len;
#define PUT_LE32(_p, _var) \
    *_p++ = (unsigned char)((_var)>>0);  \
    *_p++ = (unsigned char)((_var)>>8);  \
    *_p++ = (unsigned char)((_var)>>16); \
    *_p++ = (unsigned char)((_var)>>24);
#define PUT_LE16(_p, _var) \
    *_p++ = (unsigned char)((_var)>>0);  \
    *_p++ = (unsigned char)((_var)>>8);
#define PUT_BE32(_p, _var) \
    *_p++ = (unsigned char)((_var)>>24);  \
    *_p++ = (unsigned char)((_var)>>16);  \
    *_p++ = (unsigned char)((_var)>>8); \
    *_p++ = (unsigned char)((_var)>>0);
#define PUT_BE16(_p, _var) \
    *_p++ = (unsigned char)((_var)>>8);  \
    *_p++ = (unsigned char)((_var)>>0);

#define ABS(a)     (((a) < 0) ? -(a) : (a))

/* check range: start inclusive, end inclusive */
#define S_E(x, start, end) \
(   ((end) == (start) &&  ((x) == (start))) \
 || ((end)  > (start) &&  ((x) >= (start) && (x) <= (end)))  \
 || ((end)  < (start) && !((x) <  (start) && (x) >  (end))) \
)

/* check range: start exclusive, end inclusive */
#define So_E(x, start, end) \
(   ((end) == (start) &&  ((x) == (start))) \
 || ((end)  > (start) &&  ((x)  > (start) && (x) <= (end)))  \
 || ((end)  < (start) && !((x) <= (start) && (x)  > (end))) \
)

/* check range: start inclusive, end exclusive */
#define S_oE(x, start, end) \
(   ((end) == (start) &&  ((x) == (start))) \
 || ((end)  > (start) &&  ((x) >= (start) && (x)  < (end)))  \
 || ((end)  < (start) && !((x)  < (start) && (x) >= (end))) \
)

/* get the distance (in byte units) between two positions in the bitstream ring buffer range */
#define BS_DISTANCE(start, end, ringSize) \
(  (end) + ((end) >= (start) ? 0 : ringSize) - (start) )

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#define GET_QUEUE_ELEMENT_SIZE(rp,wp,size)  ((wp-rp+size)%size)
#define QUEUE_IS_FULL(rp,wp,size)  (((wp+1)%size)==rp)
#define QUEUE_IS_EMPTY(rp,wp)  (wp==rp)

#define IS_FASTFWD_MODE(decMode,speed,is4k2k,isHDMvc) (((decMode) == FASTFR_DECODE)&&(((speed) > 1)||((is4k2k)== 1)||((isHDMvc)==1)))
#define IS_FASTRWD_MODE(decMode,speed) (((decMode) == I_ONLY_DECODE)&&((speed) < 0))
#define IS_TRICKPLAY_MODE(decMode,speed,is4k2k,isHDMvc) ((IS_FASTFWD_MODE(decMode,speed,is4k2k,isHDMvc))||(IS_FASTRWD_MODE(decMode,speed))||(IS_SMOOTH_FR_MODE(decMode,speed)))
#define IS_SMOOTH_FR_MODE(decMode,speed) (((decMode) == NORMAL_DECODE)&&((speed) < 0))
// fps should be almost  23.98, 24, 24.98, 25, 29.97, 30, 50, 59.94, 60
#define FPS_IS_REASONABLE(fps) (((fps) > 23.5 && (fps) < 25.5) || ((fps) > 29.4 && (fps) < 30.5) || ((fps) > 49.5 && (fps) < 50.5) || ((fps) > 59.5 && (fps) < 60.5))

#define IS_4K(w,h)  (((w)*(h)) > (3200*1800))

/* ITU-T H.264 Table E-1: Sample Aspect Ratio */
AspectRatioStruct const H264_SampleAspectRatio[17]
= {
    {  1,  1},  /* 0: unspecified, defaults to square pixel */
    {  1,  1},  /* 1: square pixel */
    { 16, 15},  /* PAL */
    {  8,  9},  /* NTSC */
    { 16, 11},
    { 40, 33},
    { 24, 11},
    { 20, 11},
    { 32, 11},
    { 80, 33},
    { 18, 11},
    { 15, 11},
    { 64, 33},
    {160, 99},
    {  4,  3},
    {  3,  2},
    {  2,  1}
};

#ifndef ENDIAN_CHANGE32
#define ENDIAN_CHANGE32(x) (((((unsigned int)x)&0xff000000)>>24)|((((unsigned int)x)&0x00ff0000)>>8)|((((unsigned int)x)&0x0000ff00)<<8)|((((unsigned int)x)&0x000000ff)<<24))
#endif


class RVSD_INFO;

namespace android {

#define IS_VIDEO_GET_UNINIT(x) ((x)->mVideoDecState == android::ENUM_VIDEO_WAIT_TO_UNINIT ||\
    (x)->mVideoDecState == android::ENUM_VIDEO_UNINIT_DONE)

#define SET_DVDPLAYER_NOTIFY(pRvsdInfo, x)		(pRvsdInfo->rvsd_notify_dvdplayer = x)
#define RESET_DVDPLAYER_NOTIFY(pRvsdInfo)		{ pRvsdInfo->rvsd_notify_dvdplayer = NOTIFY_NONE; pRvsdInfo->rvsd_fatalErrCode = FATALERR_GENERAL; }
#define IS_DVDPLAYER_NOTIFY(pRvsdInfo, x)		(pRvsdInfo->rvsd_notify_dvdplayer == x)
#define SET_FATAL_ERROR_CODE(pRvsdInfo, x)      (pRvsdInfo->rvsd_fatalErrCode = x)

void *VDec_Rtk_decode_pthread(void *data);
void *VDec_Rtk_vo_pthread(void *data);
int VDec_UsingShareBSRingFormat(unsigned int format);

class RtkSource : public MediaSource {

public:

    RtkSource(RVSD_INFO* pRvsd_info, int width, int height, int colorFormat, int bufsize, int videoType);
    virtual sp<MetaData> getFormat();
    virtual status_t start(MetaData *params);
    virtual status_t stop();
    virtual status_t read(
        MediaBuffer **buffer, const MediaSource::ReadOptions *options);
protected:
    virtual ~RtkSource() {}

private:
    MediaBufferGroup mGroup;
    RVSD_INFO* pRvsdInfo;    
    int mWidth;
    int mHeight;
    int mColorFormat;
    size_t mSize;
    int mVideoType;
    int mMaxNumFrames;
    int mFrameRate;
    int64_t mNumFramesOutput;
    sp<MediaSource> mVideoDecoderSource;
    sp<ANativeWindow> mNativeWindow;

    RtkSource(const RtkSource &);
    RtkSource &operator=(const RtkSource &);
};

class VDecRtk : public RefBase {
public:
    VDecRtk(RVSD_INFO* pRvsd_info, pthread_t *pThreadVideo);
    int VDecInit(void);
    void VDecUninit(void);
    void UnInit(void);
    void *VDec_decode_loop(void);
    void *VDec_showVO_loop(void);
    void GetVideoInfo(long *Data, int size);
    void CheckBitstreamValidation(long bsBase, long bsSize, int* bitrate, int* type);
    int GetVideoErrorRate();
#ifdef ENABLE_ENCODE
    void configureTargetFormat(AV_FORMAT_CONFIG &targetFmt);
#ifdef ANDROID_6
    void registerAgentHandler(sp<AHandler> id) {
        mAgentId = id;
    };
#else
    void registerAgentHandler(ALooper::handler_id id) {
        mAgentId = id;
    };
#endif
#endif
    void GetVPURWPtr(PhysicalAddress *pRdPtr, PhysicalAddress *pWrPtr, unsigned int *pRoom);
    bool IsBehindMasterPts(long long pts, long long *pMasterPts);
    void *GetVpuDecHandle();
    virtual ~VDecRtk();

private:
#ifdef NEW_INBANDCMD_PROCESS
    void VDecVarInit();
#endif
    int VDecCreateVideoDecoder(void);
    void VDecDestroyVideoDecoder(void);
    int VDecHandleDecInitSequenceError(int apiErrorCode);
    void VDecSetDecConfig(void);
    int VDecFillInputBuf(unsigned int bus_address, unsigned int nStreamLen, long long nTimeStamp, bool bIsTempBuffer);
    int VDecUpdateStreamBuf(MediaBuffer *inBuffer);
    int VDecSeqInit(unsigned int, unsigned int);
    int VDecWaitDecodeDone(unsigned int, unsigned int);
    void VDecUpdateBSReadPtr(void);
    int VDecStartDecode(void);
    int VDecGetInitialInfo(void);
    int VDecDisplayFrame(bool lock);
    int VDecWriteBsBufFromBufHelper(unsigned int pChunkPhy, unsigned char *pChunk,  int nChunkSize, long long nTimeStamp);
    int VDecProcessInbandCommand(unsigned int pPrivInfoId, unsigned char *pPrivInfoData);
    int VDecBuildSeqHeader(unsigned char* pInputBuffer, unsigned int pInputBufferLen);
    int VDecBuildPicHeader(unsigned char* pInputBuffer, unsigned int pInputBufferLen);
    void VDecCheckDecodeComplete(void);
#if RVSD_RM_ENABLE
    int VDecBuildRVSeqHeader();
    int VDecBuildRVPicHeader(int);
#endif
#if RVSD_RM_CALCULATE_PTS
    void VDecRMDecAdjustTimeStamp(long long *TimeStamp, RVSD_RINGBUF *pBSRing, long FrameBeginPtr);
#endif
#if RVSD_RM_DUMP_SEQ_HEADER_TO_FILE_EN
    void VDecDumpRVSeqHeader2File(void);
#endif
    void VDecProcessInBand_Context(unsigned int pPrivInfoId, unsigned char *pPrivInfoData);
    void VDecProcessInBand_Decode(unsigned int pPrivInfoId, unsigned char *pPrivInfoData);
    void VDecProcessInBand_NewSeg(unsigned int pPrivInfoId, unsigned char *pPrivInfoData);
    void VDecProcessInBand_DVDVR_DCI_CCI(unsigned int pPrivInfoId, unsigned char *pPrivInfoData);
    void VDecProcessInBandByDecoderType(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBand_MPEG4UsabilityInfo(unsigned int pPrivInfoId, unsigned char *pPrivInfoData);
    void VDecProcessInBandCmdByVC1Dec(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBandCmdByH264Dec(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBandCmdByMPEG4Dec(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBandCmdByMPEG2Dec(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBandCmdByDIVX3Dec(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBandCmdByHEVCDec(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBandCmdByAVSDec(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBandCmdByRMDec(unsigned int pPrivInfoId, unsigned char *pPrivInfoData, int *nSize);
    void VDecProcessInBand_VOBU(unsigned int pPrivInfoId, unsigned char *pPrivInfoData);
#ifdef NEW_INBANDCMD_PROCESS
    void VDecProcessInBand_PTS(unsigned int pPrivInfoId, unsigned char *pPrivInfoData);
#endif

    int GetSkipFrmMode();
    long long GetOutputRPTS();
    int CalculateRPTS(int decMode, int speed);
    char CheckIfPairedFldFrm(void* pinfo, VP_PICTURE_MODE_t picMode);
    VP_PICTURE_MODE_t GetOutputPicMode(void* pinfo, int curIdx);
    unsigned int TrickPlayModifyTimeInc(unsigned int s_timeInc, int play_speed);
    int TrickPlayModifyTimeOutThd(int play_speed);
    void VDecMPEG4DecAdjustTimeStamp(long long *TimeStamp);
    long long VDecMPEG2DecAdjustTimeTick(long long sTimeTick, int curDispIdx, int prog_seq);
    void FlushHandler();
    long CalculateContext(unsigned int rdPtr);
    long long CalculatePTS(unsigned int rdPtr, unsigned int endPtr, int error);
    void VDecCheckVOBU(int startPtr, int endPtr, int decMode, int speed);
    void DropDisplayFrame(int frmIdx);
    int  DropAndWaitDispIFrm(int frmIdx);
    void RemoveItemFromDispQueue(unsigned int dltIdx, unsigned int* wptr, unsigned char* queue, int qsize, int element_size);
    void FillYUVData(YUV_STATE* yuv_win, void* pdisplayInfo, void* pdisplayInfoR, long long outTimestamp, long long RPTS, long long outTimestamp2, long long RPTS2, unsigned long context, int mode, AspectRatioStruct *ar, char IsReSendFrm);
    void SendOneFrameToVO(YUV_STATE* yuv_win, void* pdisplayInfo, void* pdisplayInfoR, long long outTimestamp, long long RPTS, unsigned long context, char IsReSendFrm);
    void UpdateFrameQueueInfo(int curIdx, DVD_VR_CCI cci, DVD_VR_DCI dci);
    int  FRDisplayVOBUFrame(int dispIdx);
    int SetAspectRatioInfo(AspectRatioStruct *ar, int pic_width, int pic_height);
    void UpdateMVCFrameInfo(int dispIdx);
    void CheckBrokenLink();
    int BypassDeocdedFrameToVO(void* pbypassInfo, int index);
    void FlushAndDropDPB();
    int PreSendFirstIFrame(long long nTimeStamp);
    int ReSendLastFrame();
    int H264_Decoder_ValidateBitStream(long bsBase, long bsSize);
    void FlushAndDisplayDPB(int crossVOBUType);
    int GetFramePOC(int curIdx);
    int DisplayFlushedFrameFromDPB();
    signed char CheckStillVOBU(int bsStart, int bsEnd);
    void UpdateMPEG2FrameOutPicMode(int curIdx);
    int GetDispQueueCount();
    void VDecSetErrConcealment();
    void ModifyIdxFrmDecodedByFrmDispFlag();
#if BUG45376_WORKAROUND
    int VDec_HandleBSErrOfTheLastFrame(void);
#endif
    void ScanCurPicDVDVRInbandCmd(int startPtr, int endPtr, DVD_VR_CCI *cci, DVD_VR_DCI *dci);
    VIDEO_VBI_DATA FillFrmVBIData(int index);
    void SortFrmPOCToFlushDispQ(unsigned int frmIdxInDPB);
    void SendEOSToVO();
    void VDecSetPropagationLevel();
    int CalculateFrameQualityLevel(int curIdx, int curPicType);
    void CalculateErrorRate(int curIdx);
    void H264DecErrorNotification(int curIdx);

    int CheckToFreeSequenceChangeBuffer(int checkPLock);
    int GetErrorCntDown();
    unsigned int GetHeaderFrameRate();
    int GetSkipPictureStatus(int indexFrameDisplay);

#ifdef SWITCH_VIDEOTYPE_RESERVE_LAST_FRAME
    void CheckToFreeLastFrameBuffer(int checkPLock);
#endif
    int VDecScanPLockBufferClearRtkFlag(int *pNumLock=NULL, int *pNumQpend=NULL, int *pNumClrDispFlag=NULL, int *pNumSeqChangeVoLock=NULL);
    int VDecGetDispQNum(int *pNumHoldByfq, int *pNumHoldByVo, int *pNumVoLock=NULL, int *pNumVoQpend=NULL, int *pNumClrDispFlag=NULL);
    bool VDecPrepareChangingSequence(void);
    int VDecIonAllocateFb(int ionfd, ion_user_handle_t *ionHandle, unsigned int *phyAddr, unsigned char *virtAddress, unsigned int *size, bool bScpuAcc);
    int DropAndWaitBrokenLinkDone(int frmIdx);
#ifdef RVSD_FF_FLUSH_DPB_BY_POC_DIFF
    void FlushAndDisplayDPBKeepCurFrm();
#endif

#ifdef RVSD_NEW_VIDEO_FLOW_EN
    void VDecConfigureDecode();
    int VDecScanInBandCmd(unsigned int *newPtr);
    int VDecUpdateBSWritePtr(unsigned int new_wp, bool bNotCheckSize=false);
    int VDecScanICQAndUpdateBS();
    void FlushShareICQ();
    long long AdjustPTSAfterIsFound(int error, unsigned int rdPtr, unsigned int curPtr, unsigned int nextPtr);
#endif

#ifdef ENABLE_ENCODE
    void configureAspectRatio(void* poutputInfo, VIDEO_CONFIG videoCfg, AspectRatioStruct *pixelAR);
    int VDecSendToAgent(int interlace, VP_PICTURE_MODE_t sendMode);
#ifdef ANDROID_6
    sp<AHandler> mAgentId;
#else
    ALooper::handler_id mAgentId;
#endif
    AV_FORMAT_CONFIG mTargetFormat;
    enum {
        kWhatDrainVideoBuffer = 0x210,
        kWhatEndOfVideoStream,

        kWhatGetWriteBackBuffer = 0x240,
        kWhatGetWriteBackBufferDone,
    };
#endif
#if defined(END_Of_VOBU_ENABLED)
    bool VDecPrepareChangingVobu(void);
#endif  // #if defined(END_Of_VOBU_ENABLED)
    void CheckDropFrmBeforeStartPTS();   // PHOENIX-18
    void ParseH264UserData(BYTE* pUserDataBuf,  long long);
    void H264_DecodeSEI_mvc_scalable_nesting(BYTE *pBuf, unsigned int bufSize);
    int H264_DecodeSEI_parse_CC(BYTE *pBuf, unsigned int bufSize, long long);
    void H264_DecodeSEI(BYTE *pBuf, unsigned int bufSize);
    void H264_DecodeSEI_user_data_unregistered(BYTE *pBuf, unsigned int bufSize);
    void H264_DecodeBD_offset_metadata(BYTE *pBuf, unsigned int bufSize);
    void H264Dec_GetSubPicOffset(long long PTS);
    unsigned int GetUnavailableFbCount(int *pNumHoldByDec, int *pNumHoldByfq, int *pNumHoldByVo, int *pNumVoLock=NULL, int *pNumVoQpend=NULL, int *pNumClrDispFlag=NULL);
    bool IsNoAvailableDPB(int *pNumHoldByDec, int *pNumHoldByfq, int *pNumHoldByVo, int *pNumVoLock=NULL, int *pNumVoQpend=NULL, int *pNumClrDispFlag=NULL);
    void GetLumaChromaOffsetTableAddr(int myIndex, int width, int height, unsigned int *pLumaOffTblAddr, unsigned int *pChromeOffTblAddr);
    int AllocateRegisterDecFrameBufferForHEVC();
    int AllocateRegisterDecFrameBuffer();

    int SetScaleMode(unsigned int origin_width, unsigned int origin_height);
    unsigned int CalculateScaleSize(unsigned int origin_size, unsigned int scale_size) const;

    int GetFrameAmountForScaleMode(int original_frame_amount) const;

    bool IsH264PicStructPaired(int pic_struct) const;
    bool IsActualDecodedFrame(bool bUpdateStats=false);

    long ComputeFrameRateTick() const;
    void GetBuffer(RINGBUFFER_HEADER **pp_header, unsigned char **pp_base, int *p_size);
    void SetVoPlane();

    bool IsAlignPpuBufferForCrop() const;

    int GetPpuIndex();
    void ReleasePpu(int vpu_index);
    void HoldPpu(int ppu_index);

#ifdef RVSD_CONTINUOUS_FRAME_COUNT_NOTIFICATION_ENABLE
    void CheckContinuousFrmNotification(int dispIdx);
#endif
    void insertPTSToQueue(long long nTimeStamp);
    long long getOutputPTS(int index);
    void HandleDecNoFrmAndBufFull();
#ifdef RVSD_SEQ_CHANGE_FREE_MOST_DPB_EN
    void seqChangeFreeMostIon();
#endif // #ifdef RVSD_SEQ_CHANGE_FREE_MOST_DPB_EN
    void AllocateUserData(bool userDataEnable);
    void ReleaseUserData();
    int GetUserData(void *pDecInfo, char *pBuf, unsigned int nBufSize);    // the type of pDecInfo is DecOutputInfo
    void ShowIonMediaHeap();
    void UpdateSeqInitConsumedBytes();

    void DetectMpeg4PtsOrder();

#ifdef RVSD_VO_WRITEBACK
    bool mVoWriteBack;
    long long mVoWbLastPts;
    int mVoWriteBackCnt;
#endif

    enum {
        kWhatVideoNotify                = 'vidr',
    };

    enum {
        AVCFORMAT_AVC,
        AVCFORMAT_MVC
    };

    enum {
        MPEG4FORMAT_MPEG4 = 0,
        MPEG4FORMAT_DIVX5,
        MPEG4FORMAT_XVID,
        MPEG4FORMAT_DIVX4 = 5,
        MPEG4FORMAT_SORENSON = 256
    };

    enum {
        VDEC_OK = 0,
        VDEC_NEED_MORE,
        VDEC_HAS_FRAME,
        VDEC_HAS_INFO,
        VDEC_EOS,
        VDEC_DECODE,
        VDEC_BUFFER_EMPTY,
        VDEC_NOT_HAS_RESOURCE,
        VDEC_FAIL = -1,
        VDEC_NOT_SUPPORT = -2
    };

    enum
    {
        I_FRAME_SEARCH_OFF = 0,
        IDR_FRM_SEARCH_MODE = 1,
        I_FRM_SEARCH_MODE = 2
    };

    enum
    {
        SKIP_FRM_OFF = 0,
        SKIP_EXCEPT_IDR_FRM = 1,
        SKIP_NON_REF_FRM = 2
    };

    enum
    {
        VDEC_EOS_NONE,
        VDEC_EOS_END_OF_SEGMENT,    // inband EOS event ID = -1 , no need to notify system
        VDEC_EOS_END_OF_STREAM,     // inband EOS event ID != -1, need to notify system
        VDEC_SEQ_END,               // inband SEQ_END
        VDEC_EOV                    // VIDEO_DEC_INBAND_CMD_TYPE_VOBU
    };

    enum
    {
        PTS_UNINIT = 0, //initial state,
        PTS_DEC_SYNC = 1,//got pts info at dec stage.
        PTS_DISP_SYNC = 2//got pts info at display stage.
    };

    enum
    {
        PRESEND_I_FRM_OFF  = 0,//reset state,
        PRESEND_I_FRM_WAIT = 1,//wait to send first I frm to VO
        PRESEND_I_FRM_SENT = 2//first I frm has passed to VO, wait for disp idx = 1st I frm idx.
    };

    enum
    {
        MVC_VIEWIDX_NONE = -1,
        MVC_VIEWIDX_LEFT = 0,
        MVC_VIEWIDX_RIGHT = 1
    };

    enum
    {
        LAYOUT_1GB = 0,
        LAYOUT_512MB,
        LAYOUT_256MB
    };

    enum
    {
        SCAN_ICQ_STAT_NONE,
        SCAN_ICQ_STAT_PTSQ_FULL,
        SCAN_ICQ_STAT_CNTXTQ_FULL,
        SCAN_ICQ_STAT_VOBUQ_FULL,
        SCAN_ICQ_STAT_EOS,
        SCAN_ICQ_STAT_VOBU_END,
        SCAN_ICQ_STAT_MBIT
    };

    sp<MediaSource> mVideoSource;

    void *pVpuDec;//pointer to VPU info
    void *mp_splitter;
    void *mp_hdr_parser; // point to HdrParameterParser

    RVSD_INFO *pRvsdInfo;
    bool mbFirstFrame;

    unsigned long mSeHandle;
#ifdef SWITCH_VIDEOTYPE_RESERVE_LAST_FRAME
    struct IONStruct m_pIonFb[MAX_REG_FRAME];  // for reserve last frame
    int mCheckToFreeIonFb;
#ifdef FIXBUG_EXTERNAL_10405_SOLUTION_1
    int mFrameDisplayed;
#endif
#endif
    int mIonFd;

    bool m_is_mpeg4_pts_order_detected;

    //int mem_fd;
    unsigned char* disp_base_ptr;
    pthread_t pThreadVO;
    pthread_cond_t rvsd_vo_cond;
    pthread_mutex_t rvsd_vo_mtx;
    bool bVOUninit;

    int lastDispIndex;
    pthread_mutex_t rvsd_scan_plock_mtx;
    pthread_mutex_t rvsd_dispQ_mtx;
    unsigned int nPLockBufferSet; // for sequence change

    pthread_mutex_t rvsd_reorderOutputPts_mtx;

#if _DBG_DUMP_BS_EN
    void _dbg_dump_bs_with_share_ringbuf(int nStreamLen);
    void _dbg_dump_bs_new(PhysicalAddress Ptr1, PhysicalAddress Ptr2);
#endif
#if _DBG_DUMP_SPECIFIC_FRAME_BS_EN
    void _dbg_dump_frame_bs(unsigned int frameIdx);
#endif
#if _DBG_DUMP_H264_USERDATA_EN
    void _dbg_dump_h264_userdata(char* pBuf, int nBufSize);
#endif
#if _DBG_GET_FRM_BYTE_OFFSET_EN
void _dbg_get_frm_byte_offset_init(unsigned int start_ptr);
void _dbg_get_frm_byte_offset(unsigned int newRdPtr, int ringSize);
#endif

#if _DBG_GET_FB_STATUS_EN
    int _dbg_print_fb_status();
    int _dbg_print_frmidx_in_dpb();
#endif

#if _DBG_DUMP_FRAME_BUFFER_EN
    // parameter types are: DecHandle, FrameBuffer, VpuRect, TiledMapType
    void _dbg_dump_frame_buffer(void *p_handle, void *p_dispFrame, void *p_rcDisplay, int mapType);
    // parameter types: DecOutputInfo, int
    void _dbg_dump_frame_buffer_ex(void *p_info, unsigned int dump_amount);    
#endif

#if _DBG_PRINT_VPU_STATUS_EN
    void PrintVpuStatus(Uint32 coreIdx, Uint32 productId);
#endif

#if _DBG_DUMP_MD5
    // parameter type: DecOutputInfo
    void DumpMd5(void *p_info);
#endif

    void VDecGetBitStreamBuffer(void* pVpuDec);

    bool bNotHasResource;
public:
#if BUG45376_WORKAROUND
public:
    int m_EOS_ptr;
#endif
#ifdef RVSD_VO_WRITEBACK
    int VDecGetSharedWBBuffer(void);
    int IsSharedWBBufferMode(void);
#endif

};

}  // namespace android


#endif

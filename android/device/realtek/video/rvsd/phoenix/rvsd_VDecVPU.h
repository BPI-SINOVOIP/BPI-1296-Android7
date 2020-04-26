#ifndef RVSD_RVSD_VPU_H
#define RVSD_RVSD_VPU_H

#include "rvsd_def.h"

//RTK VPU header file
#include "vpuconfig.h"
#include "vpuapi.h"
#include "vpuapifunc.h"
#include "sample/helper/main_helper.h"
#ifndef RVSD_USE_1295_VE
#include "vpuio.h"
#include "vpuhelper.h"
#endif
#include "coda9/coda9_regdefine.h"
#include "coda9/coda9_vpuconfig.h"
#ifdef RVSD_USE_1295_VE
#include "wave/wave4/wave4_regdefine.h"
#include "wave/common/common_regdefine.h"
#else
#include "wave4/wave4_regdefine.h"
#include "wave4/wave4_vpuconfig.h"
#endif
#include "vpuerror.h"

#include "rvsd_VDecRtk.h"
#include "rvsd_CommonFunc.h"

struct TimeStruct
{
	unsigned int wPtr;
	long long nTimeStamp;
};

struct ContextStruct
{
	unsigned int wPtr;
	long nContext;
};

struct OutTimeStruct
{
	long long		nTimeStamp;
	long long		nTimeTick;
};

struct FrameStruct
{
	long			  picType;
	int 			  errorBlock;
	int 			  POC;
	signed char 	  repeatFirstField;//for mpeg2 pts inc calculation
	signed char 	  mvcViewIdx;
	signed char 	  topFieldFirst;
	char			  pairedFldFrm;
	char			  stillVOBU;
	char			  reSend;
	signed short	  mvcPairIdx;
	int 			  qualityLevel;
    int               decodingSuccess;
    int               bytePosFrameStart;
    int               bytePosFrameEnd;
    int               ppuFbIndex;
    
	VP_PICTURE_MODE_t picMode;
	DVD_VR_DCI		  C_DVD_DCI;
	DVD_VR_CCI		  C_DVD_CCI;
};

struct dec_config_t
{
	int bitFormat;
	int rotAngle;
	int mirDir;
	int useRot;
	int useDering;
	int outNum;
	int checkeos;
	int mp4DeblkEnable;
	int iframeSearchEnable;
	int dynamicAllocEnable;
	int reorder;
	int mpeg4Class;
	int mapType;
	int tiled2LinearEnable;
	int seqInitMode;
	int dispMixerDirect;
	int wtlEnable;
	int wtlMode;	// 1: frame 2: field
	LowDelayInfo lowDelayInfo;

	int FrameCacheBypass   ;
	int FrameCacheBurst    ;
	int FrameCacheMerge    ;
	int FrameCacheWayShape ;
	int bLowMemory; // 0: 1GB, 1: 512MB, 2: 256MB(linux-only)
	int bEnable4KH264;
    int bEnableHEVCLinear;
} ;

struct VOBUQStruct
{
	int wPtr;
	long long RPTS;
};

struct DVDVRQStruct
{
	int 		 wPtr;
	DVD_VR_DCI	 C_DVD_DCI;
	DVD_VR_CCI	 C_DVD_CCI;
};

typedef struct
{
	unsigned long long reserved: 23;
	unsigned long long skip_nonref: 1 ;
	unsigned long long low_delay: 1;
	unsigned long long decMode: 4 ;
	unsigned long long search_I_frm: 1 ;
	unsigned long long checkVOBUPos: 1 ; //need to be reset, for vc1 vobu cmd
	unsigned long long reset_timeInc: 1;
	unsigned long long initPTS: 1;
	unsigned long long useHdrFrmRate: 1;//for mpeg2 pts inc calculation.
	unsigned long long waitDispIFrm: 1;//fix mpeg2 mosaic at the file start
	unsigned long long useVOBURPTS: 1;
	unsigned long long pts_sync: 2;//pts sync state
	unsigned long long startOneFrm: 1;//dec state is start one frm(1) or get output info already(0)
	unsigned long long startInitSeq: 1;//dec state is start init seq(1) or get initial info already(0)
	unsigned long long mvc: 1;
	unsigned long long idr_sync: 1;
	unsigned long long broken_link: 1;	// only for H.264
	unsigned long long dpb_bypass: 1;
	unsigned long long isProg: 1;
	unsigned long long useVOBU: 1;
	unsigned long long reorder_disabled: 1;
	unsigned long long stillVOBU: 1;
	unsigned long long MVCFRGetRightFrm: 1;//in FR mode, to get the right-eye frm(possible not I), disable I ONLY mode temporarily.
	unsigned long long hasDVDVRCmd: 1;//if had DVDVR_DCI_CCI cmd
	unsigned long long notifiedVOEOS: 1;
	unsigned long long waitBrokenLinkDone: 1;
	unsigned long long err_notify: 1;
	unsigned long long calRPTSByTimeTick: 1;//force calculate RPTS by accumulating TimeTick. In Normal decode only.
	unsigned long long waitFirstPTS: 1;// for new seg fake pts.
	unsigned long long applyRollingI: 1;// Intra-refresh support with recovery SEI; Rolling I.
    unsigned long long useEndOfVobu: 1;
    unsigned long long skipFrmBeforeStartPTS: 1;    // PHOENIX-18, start play from NewSeg, if frame PTS < startPTS, set skipFrmBeforeStartPTS to 1.
    unsigned long long hdr: 1;  // DHCFAE-922, if the transferCharacteristics of VUI ISNOT 0, set hdr to 1.
    unsigned long long waitPostProcessing: 1;  // PPU, 1 for skip 1st indexFrameDisplay>=0.
    unsigned long long IsMpeg4DecodeOrderPts: 1;   // DHCFAE-1428, if 1, pts table is decoded order
    unsigned long long reorderOutputPts: 1; // DHCFAE-1890, if 1, RVSD will reorder output PTS to cover incorrect container PTS table.
    unsigned long long gotDecodeInbandCmdAfterInit: 1;  // 0013087, RVSD has got INBAND_CMD_TYPE_DECODE after VDecInit, reset it to 0 in VDecInit() and set it to 1 in VDecProcessInBand_Decode().
    unsigned long long gotNewSegInbandCmdAfterInit: 1;  // 0013087, RVSD has got INBAND_CMD_TYPE_NEW_SEG after VDecInit, reset it to 0 in VDecInit() and set it to 1 in VDecProcessInBand_NewSeg().

} DECODER_STATUS ;

/* 3D BD sub-picture plane offset context: sizes must be power of 2 */
#define MAX_3DBD_SUBP_OFFSET_QUEUE_SIZE   256
#define MAX_3DBD_SUBP_GOP_PTS_QUEUE_SIZE    8
typedef struct
{
  unsigned char rdGOP;
  unsigned char wrGOP;
  unsigned char rdOffset;
  unsigned char wrOffset;
           int sequenceId;
           char lastOffset;
           char offset [MAX_3DBD_SUBP_OFFSET_QUEUE_SIZE ];
  unsigned char GOP_idx[MAX_3DBD_SUBP_GOP_PTS_QUEUE_SIZE];
      long long GOP_PTS[MAX_3DBD_SUBP_GOP_PTS_QUEUE_SIZE];

} H264_3DBD_SUBP;

typedef enum
{
    DYNAMIC_SCALE_STATE_OFF = 0,
    DYNAMIC_SCALE_STATE_OFFING,
    DYNAMIC_SCALE_STATE_ON
} DYNAMIC_SCALE_STATE;

struct PpuEntry
{
    int src_index;
    int dst_index;
    int chosen; // 1 means this ppu entry is chosen before
};

struct _DecConfigParam
{
    DecHandle handle;
    DecOpenParam decOP;
    DecInitialInfo initialInfo;
    DecOutputInfo outputInfo;
    DecParam decParam;
    SecAxiUse secAxiUse;
    MaverickCacheConfig decCacheConfig;
    DRAMConfig dramCfg;
    FrameBufferAllocInfo fbAllocInfo;
    FrameBuffer fbPPU[2];
    FrameBuffer fbUser[MAX_REG_FRAME];
    vpu_buffer_t vbStream;
    int fbWidth;
    int fbHeight;
    int fbStride;
    FrameBufferFormat fbFormat;
    FrameBufferFormat wtlFormat;
    int rotStride;
    int regFbCount;
    int regLinearFbCount;
    int seqInited;
    //VpuRect rcPrevDisp;
    int ppuEnable;
    int decodedIdx;
    int decodefinish;
    int int_reason;
    int totalNumofErrMbs;
    int needFrameBufCount;
    int instIdx;
    int coreIdx;
    int mapType;
    TiledMapConfig mapCfg;
    int chunkReuseRequired;
    FrameBuffer fbUserPPU[MAX_REG_FRAME];
    PpuEntry ppu_status[MAX_REG_FRAME];
    int preIndexFrameDisplay;
    VpuRect rcPpu;
    int lastPpuIndex2StartOneFrame;
    /** @param extradata pointer to extradata*/
    vpu_buffer_t picStream;
    unsigned char *picHeader;
    unsigned int picHeaderPhy;
    vpu_buffer_t seqStream;
    unsigned char *seqHeader;
    unsigned int seqHeaderPhy;
    /** @param extradata_size extradata size*/
    int seqHeaderSize;
    int picHeaderSize;
    int ppFbNum;

    int bufferEmtpyFlag;        // for bug 41710
    int bufferEmtpyWPtr;        // for bug 41710

    unsigned int mbitWPtr;

    unsigned int framesize;
    long long lastTimestamp;
    long long outTimestamp;
    long long timestampForCheckBeforeStartPTS;  // PHOENIX-18
#ifdef NEW_INBANDCMD_PROCESS
    long long    videoPTS;
    unsigned int videoPTSPtr;
#endif
    struct OutTimeStruct timestampQueue[MAX_TIME_QUEUE];
    struct TimeStruct    reciveTimestamp[MAX_TIME_QUEUE];
    long                 contextQueue[MAX_TIME_QUEUE];
    struct ContextStruct receiveContext[MAX_TIME_QUEUE];
    struct FrameStruct   frameQueue[MAX_REG_FRAME];
#if defined(RVSD_SUPPORT_TECHNICOLOR)
    tch_metadata *p_technicolor_data;
#endif
    unsigned int nFrameWidth;
    unsigned int nFrameHeight;
    unsigned int codecProfile;
    unsigned int nGarbageSize;
    unsigned int nFrameCnt;
    long long nTimeTick;    // PTS per frame.

    //RTK param
    bool isRTKShareBSRing;
    vpu_buffer_t tempStream;
    unsigned int tempBufferPhy;
    unsigned char *tempBuffer;
    unsigned int m_shareHDRfd;
    unsigned int m_shareBSfd;
    unsigned int m_shareRefClockfd;
    bool bIsTS; // for pts order
    unsigned int nPtsWPtr;
    unsigned int nPtsRPtr;
    unsigned int nPtsAgentRPtr;
    unsigned int nReciPtsWPtr;
    unsigned int nReciPtsRPtr;

    unsigned int nContextWPtr;
    unsigned int nContextRPtr;
    unsigned int nContextDispPtr;
    unsigned int nReciContextWPtr;
    unsigned int nReciContextRPtr;
    unsigned int videoContext;
    unsigned int videoContextPtr;

    unsigned int nSeqHeaderOffset;
    unsigned int nPicHeaderOffset;
    unsigned int nTimeoutCount;
    bool bKeepBuffer;

#ifdef WORKAROUND_FRAME_START_BUG
    unsigned int nFrameStartPtr;
    unsigned int nUseFrameStartPtr;
#endif
    //YLLIN ADD
    DECODER_STATUS status;
    long long  startPTS ;
    long long  endPTS ;
    long long  PTS ;
    long long  RPTS ;
    long long  VOBU_RPTS;
    VOBUQStruct VOBUQ[MAX_VOBU_QUEUE] ;//need to be reset,for vc1,mpeg2 vobu cmd
    unsigned int nVOBUWPtr;
    unsigned int nVOBURPtr;
    unsigned int newSegWPtr ; // for NEW_SEG, VOBU to reset rp.
    unsigned short framePTSinc ; /* out-band frame rate information */

    int        frm_timeInc;//for trick play
    int        prev_timeInc;//for trick play
    long long  fwd_frame_pts;//for mpeg4dec pts handling.
    signed char  PrevPicRepeatFirstField;//for mpeg2dec pts_inc calculation.
    unsigned int nPtsDispPtr;//for mpeg2dec pts_inc calculation.
    int          dropFrmIdx;//idx of frame buffer to be dropped
    int          waitDispIFrmIdx;
    int          prevBytePosFrameStart;
    unsigned int prevVOBUPtr;
    unsigned int nRPtsDispPtr;
    unsigned int nRPtsWPtr;
    unsigned int nRPtsRPtr;
    long long RPTSQueue[MAX_TIME_QUEUE];
    long long lastDispTimeTick;
    int skippedFrmCnt;
    int extraFbNum;
    int FRVOBUDispFrmIdx;
    int FRVOBUDispFrmIdxRight;
    int nEOSWPtr;
    int preDispIdxLeftEye;
    int refPicCnt;
    bool bBypassThisFrm;
    DecOutputInfo outputInfoBypass;
    bool bIs4k2k;
    int qualityLevel;
    char preSendFirstIFrm;
    int preSendFirstIFrmIdx;
    bool bCheckLastDispPTSSent;
    unsigned int frmIdxInDPB;
    signed char FlushDisplayQueue[32];
    int FlushDisplayQueueWP;
    int FlushDisplayQueueRP;
    int preMVCViewIdx;
    int displayPrevVOBULastFrmIdx;
    int crossVOBUType;
    int stillVOBUStartPtr;//assume only 1 still vobu in one segment.
    int stillVOBUEndPtr;// progressive I-picture in a DVD still-VOBU
    bool bIsHDMvc;
    bool bIsDispQcntInTransition;
    int dispQCntInTransition;
    int frameDisplayFlag;
    unsigned int frmIdxInDPBAfterIDR;
    int brokenLinkDoneFrmIdx;
    int errorCntDown;
    int propagationLevel;
    int frmIdxInDPBNoReorder;
    long long prevTimestamp;
    int gotNewSeg;  // bug 45822
    int FRVOBUMatchIFrame;    // bug 48361
    int displayedSkipPictureIdx;
    int displayedSkipPictureCnt;
    int bSkipCheckFrmMode;

    DVDVRQStruct DVDVRQ[MAX_DVDVR_QUEUE];
    unsigned int nDVDVRQWPtr;
    unsigned int nDVDVRQRPtr;

    H264_3DBD_SUBP subPic3DBD;

    //for error rate
    unsigned int errFrmCnt;
    unsigned int totalFrmCnt;
    int errFrmRate;//0~100

#ifdef BUG42723_WORKAROUND
    int videoStructure;
    char forceSendFldMode;  // only for H.264
    int fldRepeatCnt;
#endif
    //DVD_VR_CCI   cci;
    int                 seqChangeRequest;
    int                 seqChangedStreamEndFlag;
    PhysicalAddress     seqChangedRdPtr;
    PhysicalAddress     seqChangedWrPtr;
    bool                bKeepDecode;
    bool                bSeqChangeDone;
    int                 prevSeqRegFbCount;
    DecGetFramebufInfo  prevFbInfo;
    struct IONStruct pIonFb[MAX_REG_FRAME];
    struct IONStruct pIonPreFb[MAX_REG_FRAME];
    struct IONStruct pIonFbPPU[MAX_REG_FRAME];
    struct IONStruct pIonPreFbPPU[MAX_REG_FRAME];
    struct IONStruct pIonSeqHeader;
    struct IONStruct pIonPicHeader;
    struct IONStruct pIonTempBuffer;
    AspectRatioStruct agentPixelAR;

    bool bResetWave4;
    bool bResetCoda9;

#ifdef RVSD_NEW_VIDEO_FLOW_EN
    unsigned int newSegRdPtr; // rdPtr after seg init.
    unsigned int ICQData[200];//in VC1 case, may get icq data size of 200*sizeof(int ) bytes(seq info). ref 1185.
#endif

#ifdef RVSD_FF_FLUSH_DPB_BY_POC_DIFF
    int prevDecodedPOC;
#endif

    unsigned short notificationLevel;
    unsigned short continuousFrmCntForNotification;
    int continuousErrFrmCnt;    // -1 means disable calculation, >=0 means calculating frm count
    int continuousGoodFrmCnt;   // -1 means disable calculation, >=0 means calculating frm count

#ifdef RVSD_ERR_NOTIFY_ENABLED
    unsigned short errNoticeTH ;
    unsigned short errNoticeSpan ;
    unsigned short errNoticeCntDown;

    unsigned short errNoticeQuotient;
    unsigned short errNoticeFrameCnt;
    unsigned int   errNoticeMask;
    unsigned int   errNoticeQueue[10];
#endif

	frame_queue_item_t *disp_queue;
	frame_queue_item_t *fbQ;

	struct dec_config_t s_dec_config;
    VpuReportConfig_t reportCfg;

    // move from static variables
    long long preRcvPTS;

    unsigned int bDumpOpenByVDecInit;
#if _DBG_DUMP_BS_EN
    unsigned int bDumpOpen;
    unsigned int dumpSerial;
    char dumpFileName[256];
    unsigned char *pDumpBuf;
    int totalDumpBytes;
#endif
    // debug
    long long decBSEmptyLogCtrlPTS;
    long long newBSNotEnoughLogCtrlPTS;
    long long seqInitBSEmptyLogCtrlPTS;
    long long decMinus1LogCtrlPTS;

#if _DBG_CHECK_SEND_VO_FRAME_COUNT
    long long sendFrameCountLogCtrlPTS;
    unsigned int sendFrameCount;
    long long sendFrameCountStartPTS;
    long long sendFrameCountEndPTS;
    unsigned int sendFrameCountAll;
#endif

#if _DBG_DUMP_FRAME_BUFFER_EN
    unsigned int bDumpFbOpen;
    unsigned int dumpFbFileSerial;
    char dumpFbFileName[4][256];
#endif
    unsigned int dumpFbSerial;

#if _DBG_DUMP_MD5
    unsigned int md5_file_count;
    bool is_open_md5;
#endif

    int lastPOC;
    long long lastPTS;
    long long previousPts;
    VP_PICTURE_MODE_t lastSendMode;

    PhysicalAddress lastVobuWPtr;
    unsigned long long vobuOffsetFromNewSeg;

    unsigned int minFeedingDataSize;

    PhysicalAddress lastVpuBsBufRdPtr;
    PhysicalAddress lastVpuBsBufWrPtr;
    unsigned int    lastVpuBsBufAvailSpace;
    unsigned int    lastFbNumOfQpend;   // only updated in VDecScanPLockBufferClearRtkFlag()
    unsigned int    lastFbNumOfLock;    // only updated in VDecScanPLockBufferClearRtkFlag()

    unsigned int outputinfoSN;
    unsigned int decodedFrmNum;
    unsigned int displayFrmNum;

#if defined(RVSD_USE_VO_SCALE_CMD)
    DYNAMIC_SCALE_STATE dynamicScaleState;
    //bool testDynamicScaleOn;
    //bool testDynamicScaleOff;
#endif  // #if defined(RVSD_USE_VO_SCALE_CMD)

    int             lumaTblSize;
    int             chromaTblSize;
    PhysicalAddress initBeginRingRp;                    // set it in handle ENUM_VIDEO_WAIT_TO_INIT.
    PhysicalAddress seqInitDoneRingRp;                  // reset it to 0 before VPU_DecIssueSeqInit(), set it after VPU_DecCompleteSeqInit() succeed.
    PhysicalAddress lastSeqInitProcessingVpuRp;         // set initial value before VPU_DecIssueSeqInit(), update it when check int_reason.
    unsigned int    totSeqInitConsumedBytes;            // update it when check int_reason.
    bool            bCheckPtsInSeqInitData;             // set it to true after VPU_DecCompleteSeqInit() succeed, set it to false when PTS inband cmd doesn't belong to seq init consumed bytes.
    unsigned int    ptsWpToAccuBytes;                   // calculate every PTS inband cmd wp to a linear accumulated bytes.
    unsigned long   lastPtsWpInSeqInitConsumedBytes;    // wp of the last drop(belongs to seq init consumed bytes) PTS inband cmd.
    long long       lastRcvPTSInSeqInitConsumedBytes;   // PTS of the last drop(belongs to seq init consumed bytes) PTS inband cmd.
    bool            bCheckCtxtInSeqInitData;
    unsigned int    ctxtWpToAccuBytes;
    unsigned long   lastCtxtWpInSeqInitConsumedBytes;
    bool            bNewSegDone;                        // INBAND_CMD_TYPE_NEW_SEG is received and handled in VDecRtk::VDecScanInBandCmd(). Only bNewSegDone==true, then start decode frame.

#ifdef RVSD_WORKAROUND_HEVC_NO_IRAP
    bool            checkHEVCNoIRAP;    // in VDecInit(), if HEVC, set it to true. If check done, set it to false.
    bool            isHEVCNoIRAP;       // if true, use skip frame mode 0 to search I frame. isHEVCNoIRAP is check while starting playback and flush won't change isHEVCNoIRAP.
    int             skipOutputInfoCnt;
    bool            hasRecoveryPointSEI;    // has HEVC recovery point SEI or not
#endif  // #ifdef RVSD_WORKAROUND_HEVC_NO_IRAP

#ifdef RVSD_DECODE_STATS_ENABLED
    DecodeStats     stats;
    long long       updateStatsPTS;
#endif

    // User data
    bool            userDataEnable;
    int             userDataReportMode;
    int             userDataBufSize;
    vpu_buffer_t    userDataBuf;

    bool            bSkipDue2ErrBlock; // bSkipDue2ErrBlock is for RTCEZ-143
} ;

int RVSD_GetBSRingBufVirt(vpu_buffer_t *vbStream, DecHandle handle, unsigned long *base, unsigned long *limit, unsigned long *wp, unsigned long *rp);
int RVSD_GetBSRingBufPhy(DecHandle handle, PhysicalAddress *base, PhysicalAddress *limit, PhysicalAddress *wp, PhysicalAddress *rp);
void RVSD_RtkSetVpu(SecAxiUse *p, int coreIdx);
unsigned long RVSD_BSRingPhy2Virt(vpu_buffer_t *vbStream, DecHandle handle, PhysicalAddress phyAddr);

#endif

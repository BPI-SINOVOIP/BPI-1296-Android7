#ifndef RVSD_MAIN_H
#define RVSD_MAIN_H

#include "rvsd_def.h"
#include "RtkTranscodeCommon.h"

#ifdef ANDROID
#include "rvsd_VDecACodec.h"
#include "rvsd_ADecACodec.h"
#include "rvsd_VDecOMX.h"
#include <gui/Surface.h>
#endif
#include "rvsd_CommonRtk.h"
#include "rvsd_VDecRtk.h"
#ifdef RVSD_JDEC_ENABLE
#include "rvsd_JDecRtk.h"
#endif

#include <utils/Mutex.h>
#ifdef SPU_DECODE_ENABLE
#include "sp_common/sp_common.h"
#endif
#include "rvsd_VDecRM.h"
#include "rvsd_VORPC.h"
#include "rvsd_VDecMpeg2.h"
#include "rvsd_CommonFunc.h"

using namespace android;

#ifdef RVSD_NEW_VIDEO_FLOW_EN
enum RING_CONTENT_TYPE
{
	CONTENT_BS,
	CONTENT_ICQ
};
#endif

class RVSD;
/**
 * It is a singleton class.
 */

class RVSD_INFO {

    friend class RVSD;

public:
    //static RVSD_INFO* getInstance();
    void setVideoSurfaceTexture();
    void setListener(RVSD *listener) {
        mListener = listener;
    };
    void notifyListener(int msg, int ext1 = 0, int ext2 = 0);
    void setVideoDecState(android::ENUM_VIDEO_DEC_STATUS status);

protected:
    //static void releaseInstance();

private:
    RVSD_INFO(int handle);
    ~RVSD_INFO();

    RVSD_INFO(RVSD_INFO const&);
    RVSD_INFO& operator= (RVSD_INFO const&);

public:
    int mHandle;
#ifdef ANDROID
    android::sp<android::VDecOMX> mVDecOMX;
    android::sp<android::ADecACodec> mADecACodec;
    android::sp<android::VDecACodec> mVDecACodec;
#endif
#ifdef ANDROID_6
    android::sp<android::AHandler> mAgentId;
#else
    android::ALooper::handler_id mAgentId;
#endif
    int32_t mTranscodeVOWriteBackFlag;
    android::sp<android::VDecRtk> mVDecRtk;
#ifdef RVSD_JDEC_ENABLE
	android::sp<android::JDecRtk> mJDecRtk;
#endif

    AV_FORMAT_CONFIG mTargetFormat;

    int bUseFork;

    pthread_t mRvsd_tid;

    // for ACodec and RTK API //
    int use_codec_type;
    int bFlush;
    int bExitLoopThread;
    int mVideoType;
    unsigned int bRotateEnable;
    unsigned int nRotateAngle;
#ifdef ANDROID
    android::sp<android::Surface> mSurface;
#endif
    volatile android::ENUM_VIDEO_DEC_STATUS mVideoDecState;

    /* global variables start */
    bool gStopSocket;
    int nEnableOSD;
    bool bLibMode;
    unsigned int LayerOrder;
    int g_play_speed;
    int g_disable_reorder;
    int g_set_errConcealment;
    int g_qualityLevel;
    int set_propagationLevel;
    int propagationLevel;
#ifdef SWITCH_VIDEOTYPE_RESERVE_LAST_FRAME
    int g_set_reserve_last_frame;
#endif
    int g_set_dpb_bypass;
    int g_set_preSendFirstIFrm;
    int g_disable_chk_broken_link;
    int notificationLevel;
    // if continuous error frame count >= this, notify DECODER_MESSAGE_VIDEO_CONTINUOUS_ERROR_FRM_DECODED to AP.
    // if continuous good frame count >= this, notify DECODER_MESSAGE_VIDEO_CONTINUOUS_GOOD_FRM_DECODED to AP.
    int g_continuousFrmCntForNotification;
    volatile CMDSTATE rvsd_cmd_state;
    volatile NOTIFY_DVDPLAYER_STATUS rvsd_notify_dvdplayer;
    volatile unsigned int rvsd_fatalErrCode;    // refer to FATAL_ERROR_CODE in rpc_common.h

    /* video variables */
    unsigned long rvsd_BSringPhyaddr;
    int rvsd_BSHDRSize;
    int rvsd_ipc_BSsharefd;
    int rvsd_ipc_BSHDRsharefd;
    unsigned long rvsd_BSHDRAddr;
    int rvsd_BSringSize;
    RINGBUFFER_HEADER *rvsd_BSHDRBase;
    unsigned char *rvsd_BSringBase;

#ifdef RVSD_NEW_VIDEO_FLOW_EN
	unsigned long rvsd_ICQringPhyaddr;
	int rvsd_ICQHDRSize;
	int rvsd_ipc_ICQsharefd;
	int rvsd_ipc_ICQHDRsharefd;
	unsigned long rvsd_ICQHDRAddr;
	int rvsd_ICQringSize;
	RINGBUFFER_HEADER *rvsd_ICQHDRBase;
	void * rvsd_ICQringBase;
	bool bCheckCreateCodecHandler;
	VP_INBAND_RING_BUF S_ICQ[1];
#endif
    rvsdRingBuffer_t rvsdICQRingBuffer;

    int rvsd_vdec_doFlush;

    /* audio variables */
    unsigned long rvsd_BSringPhyaddr_audio;
    int rvsd_BSHDRSize_audio;
    int rvsd_ipc_BSsharefd_audio;
    int rvsd_ipc_BSHDRsharefd_audio;
    unsigned long rvsd_BSHDRAddr_audio;
    int rvsd_BSringSize_audio;
    RINGBUFFER_HEADER *rvsd_BSHDRBase_audio;
    unsigned char *rvsd_BSringBase_audio;

    DecoderState decState_audio;
    int rvsd_adec_audioType;
    int rvsd_adec_audioSampleRate;
    int rvsd_adec_audioChannels;
    volatile unsigned int rvsd_audio_decoder_status;
    volatile unsigned int rvsd_adecInit_status;
    volatile unsigned int rvsd_adecUninit_status;
    rvsdRingBuffer_t rvsdICQRingBuffer_audio;

    int rvsd_adec_doFlush;
    int rvsd_adec_doInit;
    int rvsd_adec_doUninit;
    int rvsd_wait_audio_decode_done;

    AUDIO_DEC_NEW_FORMAT audioCmd;

    /* VIDEO_DECODE_OMX */
    int rvsd_last_vdec_uninit_finish;
    unsigned int rvsd_vdec_rcnts;
    unsigned int rvsd_vdec_wcnts;
    int rvsd_vdec_ridx;
    int rvsd_vdec_widx;
    DecoderState decState;
    int rvsd_vdec_videoType;
    volatile unsigned int rvsd_decoder_status;
    volatile unsigned int rvsd_vdecInit_status;
    volatile unsigned int rvsd_vdecUninit_status;
    int rvsd_vdec_doInit;
    int rvsd_vdec_doUninit;
    int rvsd_wait_decode_done;

    pthread_t thread_video;
    pthread_t thread_audio;
    pthread_cond_t rvsd_vdec_cond;
    pthread_mutex_t rvsd_vdec_mtx;
    pthread_cond_t rvsd_adec_cond;
    pthread_mutex_t rvsd_adec_mtx;
    pthread_mutex_t PQMutex;   // protect this structure
    pthread_mutex_t APQMutex;
    pthread_mutex_t refClock_mtx;

    REFCLOCK_t *rvsd_RefClock;
    VORPCGlobal *pVORPC;
    int rvsd_RefClockSize;
    int rvsd_ipc_RefClocksharefd;

    int PQcounts;
    int APQcounts;
    /* the queue(PQroot) store the following inband-command:
    1. INBAND_CMD_TYPE_PTS
    2. INBAND_CMD_TYPE_EOS
    3. INBAND_CMD_TYPE_SEQ_END */
    PtsQueue_t *PQroot;
    PtsQueue_t *PQtail;
    PtsQueue_t *APQroot;
    PtsQueue_t *APQtail;

#ifndef RVSD_RPC_RVSD_HANDLE
    unsigned int g_VOInstanceID;
#endif
    /* global variables end */

    // for ACodec and RTK API //

#ifdef SPU_DECODE_ENABLE
    SP_INFO *mSpInfo;
    pthread_t mSPThread;
#endif

#if (RVSD_RM_ENABLE) || (RVSD_RM_CALCULATE_PTS)
    /* Why RVSD_RV_INFO not placed in VDecRtk ??
    Because RVSD will receive inband command VIDEO_RV_INBAND_CMD_TYPE_FORMAT_INFO before creating VDecRtk(get first PTS). */
    RVSD_RV_INFO m_RV_Info;
#endif

    /* Why RVSD_MPEG2_INFO not placed in VDecRtk ??
    Because RVSD will receive inband-command about mpeg2 before creating VDecRtk(get first PTS). */
    RVSD_MPEG2_INFO m_Mpeg2Info;

    unsigned char *pVpuH264UserDataBuf;

    int mVOState;
    int decode_mode;
    int uninitByRecvSocketFail;

    bool bIs4k2k;
    bool bIsHDMvc;

    int scale_width;
    int scale_height;

#if defined(RVSD_USE_VO_SCALE_CMD)
    bool bTurnOnDynamicScale;
    bool bTurnOffDynamicScale;
    int dynamicScaleWidth;
    int dynamicScaleHeight;
#endif  // #if defined(RVSD_USE_VO_SCALE_CMD)

    // move from static variables
    int timeout_cnt;
    int just_got_newSeg;
    unsigned int newSeg_wPtr;
    long long preRcvPTS;

    // debug
    long long pollTimeoutLogCtrolPTS;
    long long decLoopChkLogCtrlPTS;

    unsigned int totAllocIonSizeOfFb;

#if _DBG_CHECK_INIT_TO_1ST_FRAME_SPEND_TIME
    bool bCheck1stData;
    struct timeval checkTime[MAX_CHECK_TIME_INTERVAL_NUM];
    char checkTimeStr[MAX_CHECK_TIME_INTERVAL_NUM][MAX_CHECK_TIME_STR_LEN];
    int checkTimeSN;
    long seqInitStartRdPtr;
    long seqInitDoneRdPtr;
    int firstIFrameSize;
#endif // #if _DBG_CHECK_INIT_TO_1ST_FRAME_SPEND_TIME

    int vpuBsMode;

private:
    //static android::Mutex _mlock;
    //static RVSD_INFO* mInstance;
    RVSD *mListener;
};

void rvsd_init(RVSD_INFO *pRvsdInfo);
void rvsd_create_codec_handler(RVSD_INFO *pRvsdInfo);
void rvsd_delete_codec_handler(RVSD_INFO *pRvsdInfo);

int rvsd_run(void *rvsd, int codec_type = -1);
bool rvsd_terminate(void *rvsd);

extern const char *RVSD_SOCKET_NAME;
//void setVideoSurfaceTexture(const sp<IGraphicBufferProducer>& bufferProducer);

#endif


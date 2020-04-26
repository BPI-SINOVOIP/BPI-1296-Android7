#ifndef __RVSD_JDEC_RTK_H__
#define __RVSD_JDEC_RTK_H__

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
#include "rvsd_InBandAPI.h"
#include "pthread-macro.h"
#include "rvsd_OSAL.h"
#include "rvsd_ringbuffer.h"
#include "rvsd_dbg.h"

#define RTK_RVSD_RUNNING    "rtk.rvsd.running"

#define JPU_MAX_TIME_QUEUE 256

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

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#define IS_FASTFWD_MODE(decMode,speed,is4k2k,isHDMvc) (((decMode) == FASTFR_DECODE)&&(((speed) > 1)||((is4k2k)== 1)||((isHDMvc)==1)))
#define IS_FASTRWD_MODE(decMode,speed) (((decMode) == I_ONLY_DECODE)&&((speed) < 0))
#define IS_TRICKPLAY_MODE(decMode,speed,is4k2k,isHDMvc) ((IS_FASTFWD_MODE(decMode,speed,is4k2k,isHDMvc))||(IS_FASTRWD_MODE(decMode,speed))||(IS_SMOOTH_FR_MODE(decMode,speed)))
#define IS_SMOOTH_FR_MODE(decMode,speed) (((decMode) == NORMAL_DECODE)&&((speed) < 0))

// fps should be almost  23.98, 24, 24.98, 25, 29.97, 30, 50, 59.94, 60
#define FPS_IS_REASONABLE(fps) (((fps) > 23.5 && (fps) < 25.5) || ((fps) > 29.4 && (fps) < 30.5) || ((fps) > 49.5 && (fps) < 50.5) || ((fps) > 59.5 && (fps) < 60.5))

class RVSD_INFO;

namespace android {

#define IS_VIDEO_GET_UNINIT(x) ((x)->mVideoDecState == android::ENUM_VIDEO_WAIT_TO_UNINIT ||\
    (x)->mVideoDecState == android::ENUM_VIDEO_UNINIT_DONE)

#define SET_DVDPLAYER_NOTIFY(pRvsdInfo, x)		(pRvsdInfo->rvsd_notify_dvdplayer = x)
#define RESET_DVDPLAYER_NOTIFY(pRvsdInfo)		(pRvsdInfo->rvsd_notify_dvdplayer = NOTIFY_NONE)
#define IS_DVDPLAYER_NOTIFY(pRvsdInfo, x)		(pRvsdInfo->rvsd_notify_dvdplayer == x)

void *JDec_Rtk_decode_pthread(void *data);
void *JDec_Rtk_vo_pthread(void *data);

#define GET_BS_VIR_ADDR(pRvsdInfo, phyaddr)    ((int)phyaddr -(int)(pRvsdInfo->rvsd_BSringPhyaddr-(unsigned long)pRvsdInfo->rvsd_BSringBase))

class RtkJDECSource : public MediaSource {

public:
    RtkJDECSource(RVSD_INFO* pRvsd_info, int width, int height, int colorFormat, int bufsize, int videoType);
    virtual sp<MetaData> getFormat();
    virtual status_t start(MetaData *params);
    virtual status_t stop();
    virtual status_t read(
        MediaBuffer **buffer, const MediaSource::ReadOptions *options);
protected:
    virtual ~RtkJDECSource() {}

private:
    RVSD_INFO* pRvsdInfo;
    MediaBufferGroup mGroup;
    int mWidth;
	int mHeight;
    int mColorFormat;
    size_t mSize;
    int mVideoType;
    int mMaxNumFrames;
    int mFrameRate;
    int64_t mNumFramesOutput;
    sp<MediaSource> mVideoDecoderSource;
    //sp<AwesomeRenderer> mVideoRendererOut;
    sp<ANativeWindow> mNativeWindow;

    RtkJDECSource(const RtkJDECSource &);
    RtkJDECSource &operator=(const RtkJDECSource &);
};

class JDecRtk : public RefBase {
public:
    JDecRtk(RVSD_INFO* pRvsd_info, pthread_t *pThreadVideo);
    int JDecInit(void);
    void JDecUninit(void);
    void UnInit(void);
    void *JDec_DecodeLoop(void);
    void *JDec_ShowVOLoop(void);
    void JDecGetVideoInfo(long *Data, int size);
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
    virtual ~JDecRtk();

private:

	void JDecErrHandler(void);	
	void JDecUpdateBSReadPtr(unsigned int rp);

	void JDecUpdatePTSWritePtr(int64_t timeUs, int64_t timeDuration, unsigned int pre_wPtr);
	long long JDecUpdatePTSReadPtr(void);

	void JDecUpdateShowVOPTSWritePtr(int64_t timeUs);
	long long JDecUpdateShowVOReadPtr(void);

	int JDecUpdateStreamBuf(MediaBuffer *inBuffer);
    int JDecDisplayFrame(void);	
	int JDecGetDispQNum(void);
    int JDecScanPLockBufferClearRtkFlag(void);
	void JDecCalculateRPTS(long long pts);

    void JDecFlushHandler();
	void JDecFillYUVData(YUV_STATE* yuv_win, unsigned int dispFramePhyAddr, int indexFrameDisplay, long long outTimestamp, long long RPTS);
	void JDecFormat422To420(unsigned int dispFramePhyAddr);
	void JDecSendOneFrameToVO(YUV_STATE* yuv_win, void * pdisplayInfo, long long outTimestamp, long long RPTS);
    void JDecSendEOSToVO();
#ifdef RVSD_NEW_VIDEO_FLOW_EN
	void JDecScanICQInit();
	int JDecScanICQAndUpdateBS();
	int JDecUpdateDecStream(unsigned int setPtr);
	int JDecScanInBandCmd(unsigned int *newWPtr);
	void FlushShareICQ();
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
    };
#endif

    enum {
        kWhatVideoNotify = 'vidr',
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
		SCAN_ICQ_PTS_RECEIVED,
        SCAN_ICQ_STAT_PTSQ_FULL,
        SCAN_ICQ_STAT_CNTXTQ_FULL,
        SCAN_ICQ_STAT_VOBUQ_FULL,
        SCAN_ICQ_STAT_EOS,
        SCAN_ICQ_STAT_VOBU_END
    };

    sp<MediaSource> mVideoSource;

	void *pJpuDec;//pointer to JPU info

    RVSD_INFO *pRvsdInfo;

    bool mbFirstFrame;
    bool bVOUninit;

    unsigned long mSeHandle;
    int mIonFd;

    pthread_t pThreadVO;
    pthread_cond_t rvsd_vo_cond;
    pthread_mutex_t rvsd_vo_mtx;
    pthread_mutex_t rvsd_vo_plock_mtx;
    unsigned int nPLockBufferSet; // for sequence change

#if _DBG_DUMP_BS_EN
//    void _dbg_dump_bs_with_share_ringbuf(int nStreamLen);
#endif
#if _DBG_DUMP_SPECIFIC_FRAME_BS_EN
//    void _dbg_dump_frame_bs(unsigned int frameIdx);
#endif

public:
    int m_EOS_ptr;
};

}  // namespace android

#endif

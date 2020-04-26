#ifndef __RVSD_VDEC_ACODEC_H__
#define __RVSD_VDEC_ACODEC_H__

#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <media/IMediaPlayerService.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/AMessage.h>

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

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/MediaBufferGroup.h>

#include <utils/RefBase.h>
#include <utils/Log.h>

#include "rvsd_vdec.h"
#include "rvsd_InBandAPI.h"
#include "pthread-macro.h"
#include "rvsd_OSAL.h"

#ifdef RVSD_ACODEC_FLOW_SIDEBAND_ENABLED
#include <gui/SidebandClient.h>
#include <gui/SidebandControl.h>
#include <utils/NativeHandle.h>
#endif

#if defined RVSD_RPC_VO_ACODEC || defined RVSD_NEW_VIDEO_FLOW_EN
#include "rvsd_VORPC.h"
#define VP9_MAX_SUPPROT_RESOLUTION	640*360//(width*height)
#define VO_BUFFER_NUM	PLOCK_BUFFER_SET_SIZE//PLOCK_MAX_BUFFER_INDEX //64->32, 512MB version memory limitation
#define ACODEC_VO_BUFFER_SIZE   VP9_MAX_SUPPROT_RESOLUTION*3>>1//Y(width*height)+CbCr(width*height/2)
#define NO_AVAILABLE_BUFFER	-1
#define RVSD_ACODEC_AVSYNC_RENDER_PTS_OFFSET 0.2
#define RVSD_ACODEC_AVSYNC_RENDER_DROP_THRESHOLD_LEVEL_1 2
#define RVSD_ACODEC_AVSYNC_RENDER_DROP_THRESHOLD_LEVEL_2 5
#define RVSD_ACODEC_FRAMERATE_IS_REASONABLE(fps) (((fps) > 23.5 && (fps) < 60.5))
#define RVSD_ACODEC_AVDIFF_HI_BOUND   0.07
#define RVSD_ACODEC_AVDIFF_LOW_BOUND   -0.2

struct AcodecShowVOGlobal
{
    //memory for VO
    unsigned char *acodec_ion_phyaddr[VO_BUFFER_NUM];
    unsigned char *acodec_ion_viraddr[VO_BUFFER_NUM];
    int acodec_ion_fd[VO_BUFFER_NUM];
    int acodec_mmap_fd[VO_BUFFER_NUM];
    ion_user_handle_t acodec_ion_handle[VO_BUFFER_NUM];

    //decoder info
    int32_t mDec_nFrameWidth;
    int32_t mDec_nFrameHeight;
    int32_t mDec_nStride;

    int32_t mDec_nWhatFillThisBuffer;
    int32_t mDec_nWhatDrainThisBuffer;
    int32_t mDec_nSetRenderMsg;
    int32_t mDec_nFrameRate;

    //pts info
    long long nPreTimeStamp;
    long long nTimeTick;
    long long rpts;

    float preRenderPTS_f;

    //workaround
    int32_t FirstFrameAfterSeek;
    int32_t RenderCnt;
#ifdef RVSD_NEW_VIDEO_FLOW_EN
    unsigned int ICQData[200];
    pthread_mutex_t rvsd_get_icq_mtx;  
#endif

};
#endif

//#define RVSD_ACODEC_DEBUG_TRACE
#ifdef RVSD_ACODEC_DEBUG_TRACE
#define RVSD_ACODEC_DBG_TRACE(fmt, args...) ALOGD(fmt, ##args)
#else
#define RVSD_ACODEC_DBG_TRACE(fmt, args...)
#endif

#define EOS_END_OF_SEGMENT (ERROR_END_OF_STREAM-1)
#define ERROR_PTS_CMD_SIZE_ZERO (ERROR_END_OF_STREAM-2)

class RVSD_INFO;

namespace android {


#define IS_VIDEO_GET_UNINIT(x) ((x)->mVideoDecState == android::ENUM_VIDEO_WAIT_TO_UNINIT ||\
    (x)->mVideoDecState == android::ENUM_VIDEO_UNINIT_DONE)

#define IS_VIDEO_PROCESS_UNINIT(x) ((x)->mVideoDecState == android::ENUM_VIDEO_WAIT_TO_UNINIT ||\
    (x)->mVideoDecState == android::ENUM_VIDEO_READY_TO_UNINIT)

void *VDec_ACodec_pthread(void *data);

struct VORender : public AHandler {
    VORender(RVSD_INFO* pRvsd_info);
    void startAsync();
    void stopAsync();
    void SetStopStatus(ENUM_SERVICE_STATUS arg) {
        mStop = arg;
    }
    void queueBuffer(
        const sp<ABuffer> &buffer,
        const sp<AMessage> &notifyConsumed);
    ENUM_SERVICE_STATUS GetStopStatus() {
        return mStop;
    }

#ifdef RVSD_RPC_VO_ACODEC
    void CalculateRPTS(long long pts);
    void FillYUVData(YUV_STATE* yuv_win, unsigned int index, long long pts);
    void CbCrInterleave(unsigned char* addr, unsigned int byteSize);
    int SearchOneFreePLockBuffer(void);
    void SendOneFrameToVO(unsigned char* p, unsigned int size, long long pts);
#endif

protected:
    virtual ~VORender();
    virtual void onMessageReceived(const sp<AMessage> &msg);
private:
    enum {
        kWhatStart             = 'strt',
        kWhatStop              = 'stop',
        kWhatCodecNotify       = 'noti',
        kWhatSeek              = 'seek',
        kWhatVideoNotify       = 'vidr',
        kWhatRendererNotify    = 'renN',
        kWhatRenderVOBuffer    = 'rvob',
        kWhatQueueVO	       = 'qvoN',
        kWhatEOS               = 'eos ',
    };

    struct QueueEntry {
        sp<ABuffer> mBuffer;
        sp<AMessage> mNotifyConsumed;
        size_t mOffset;
        status_t mFinalResult;
    };
    List<QueueEntry> mVideoQueue;
//    sp<AMessage> mNotify;
//    sp<Surface> mSurface;
//    bool mRenderToSurface;
//    sp<ACodec> mCodec;
//    sp<MediaSource> mSource;
    enum SeekState {
        SEEK_NONE,
        SEEK_FLUSHING,
        SEEK_FLUSH_COMPLETED,
    };
    SeekState mSeekState;
    int64_t mSeekTimeUs;

    int64_t vRCD;
    void onFillThisBuffer(const sp<AMessage> &msg);
    ENUM_SERVICE_STATUS mStop;
    RVSD_INFO *pRvsdInfo;
};

struct VDecController : public AHandler {
#ifdef ANDROID_6
    VDecController(RVSD_INFO* pRvsd_info, const sp<Surface> &surface, const sp<MediaSource> &source, sp<AHandler> VOid);
#else
    VDecController(RVSD_INFO* pRvsd_info, const sp<Surface> &surface, const sp<MediaSource> &source, ALooper::handler_id VOid);
#endif
    void startAsync();
    void stopAsync();
    void signalFlush();
    void SetStopStatus(ENUM_SERVICE_STATUS arg) {
        mStop = arg;
    }
    ENUM_SERVICE_STATUS GetStopStatus() {
        return mStop;
    }

#ifdef RVSD_RPC_VO_ACODEC
    int GetFreePLockBufferNum(void);
#endif

protected:
    virtual ~VDecController();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum {
        kWhatStart             = 'strt',
        kWhatStop              = 'stop',
        kWhatCodecNotify       = 'noti',
        kWhatSeek              = 'seek',
        kWhatVideoNotify       = 'vidr',
        kWhatRendererNotify    = 'renN',
        kWhatQueueVO	       = 'qvoN',
        kWhatEOS               = 'eos ',
    };

    struct QueueEntry {
        sp<ABuffer> mBuffer;
        sp<AMessage> mNotifyConsumed;
        size_t mOffset;
        status_t mFinalResult;
    };

    sp<ALooper> mDecodeLooper;
    sp<Surface> mSurface;
    sp<MediaSource> mSource;
    sp<ACodec> mCodec;
    sp<AMessage> mNotify;
#ifdef ANDROID_6
    sp<AHandler> mVOid;
#else
    ALooper::handler_id mVOid;
#endif
    Vector<sp<ABuffer> > mCSD;
    size_t mCSDIndex;

    MediaBuffer *mLeftOverBuffer;
    status_t mFinalResult;

    int64_t mStartTimeUs;
    int32_t mNumOutputBuffersReceived;
    int64_t mTotalBytesReceived;

    enum SeekState {
        SEEK_NONE,
        SEEK_FLUSHING,
        SEEK_FLUSH_COMPLETED,
    };
    SeekState mSeekState;
    int64_t mSeekTimeUs;

//    int64_t vRCD;

    sp<AMessage> makeFormat(const sp<MetaData> &meta);
#ifdef RVSD_NEW_VIDEO_FLOW_EN
	void FlushShareICQ(void);
#endif
    void onFillThisBuffer(const sp<AMessage> &msg);

    ENUM_SERVICE_STATUS mStop;

    RVSD_INFO* pRvsdInfo;
    bool mStreamEOS;
};

class ACodecSource : public MediaSource {

public:

    ACodecSource(RVSD_INFO* pRvsd_info, int width, int height, int colorFormat, int bufsize, int videoType);
    virtual sp<MetaData> getFormat();
    virtual status_t start(MetaData *params);
    virtual status_t stop();
    virtual status_t read(
        MediaBuffer **buffer, const MediaSource::ReadOptions *options);
protected:
    virtual ~ACodecSource() {}

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
    sp<ANativeWindow> mNativeWindow;

    ACodecSource(const ACodecSource &);
    ACodecSource &operator=(const ACodecSource &);
};

class VDecACodec : public RefBase {
public:
    VDecACodec(RVSD_INFO* pRvsd_info, pthread_t *pThreadVideo);
    void VDecInit(void);
    void VDecUninit(void);
    void UnInit(void);
    void * VDec_decode_loop(void);

    status_t setVideoSurfaceTexture(const sp<Surface>& surface);
    void connectNativeWindow();
    void disconnectNativeWindow();
#ifdef RVSD_RPC_VO_ACODEC
    void showVOinit(void);
    void showVOuninit(void);
#endif
    void GetVideoInfo(long *Data, int size);

    virtual ~VDecACodec();

private:

    RVSD_INFO *pRvsdInfo;

    //void connectNativeWindow();
    //void disconnectNativeWindow();

    enum {
        kWhatVideoNotify                = 'vidr',
    };

    sp<MediaSource> mVideoDecoder;
    sp<MediaSource> mVideoSource;

    sp<Surface> mSurface;

    sp<ALooper> mVDecLooper;
    sp<VDecController> mVDecController;
    sp<ALooper> mVORenderLooper;
    sp<VORender> mVORender;

    int mSignalFlush;
    ENUM_SERVICE_STATUS mVDecController_stop;

#ifdef RVSD_ACODEC_FLOW_SIDEBAND_ENABLED
    sp<SidebandClient>  SB_client;
    sp<SidebandControl> SB_control;
    native_handle_t * mNativeHandle = NULL;
    sp<NativeHandle> mSourceHandle;
    sp <Surface> SF_surface;
#endif

};

}  // namespace android

#endif

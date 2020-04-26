#ifndef __RVSD_VDEC_OMX_H__
#define __RVSD_VDEC_OMX_H__

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <media/IMediaPlayerService.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/AudioPlayer.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/OMXClient.h>
#include <media/mediametadataretriever.h>

#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/MPEG2TSWriter.h>
#include <media/stagefright/MPEG4Writer.h>

#include <private/media/VideoFrame.h>

//#include <gui/SurfaceTextureClient.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
//#include <gui/SurfaceControl.h>
#include <gui/ISurfaceComposer.h>
#include <ui/DisplayInfo.h>

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/MediaBufferGroup.h>

#include <utils/RefBase.h>

#include "rvsd_vdec.h"
#include "rvsd_InBandAPI.h"
#include "pthread-macro.h"
#include "rvsd_OSAL.h"

class RVSD_INFO;

namespace android {
void *VDec_decode_pthread(void *data);

class DummySource : public MediaSource {

public:
    int isEOS;
    int isReading;

    DummySource(RVSD_INFO* pRvsd_info,int width, int height, int colorFormat, int bufsize, int videoType);
    virtual sp<MetaData> getFormat();
    virtual status_t start(MetaData *params);
    virtual status_t stop();
    virtual status_t read(MediaBuffer **buffer, const MediaSource::ReadOptions *options);
protected:
    virtual ~DummySource() {pRvsdInfo = NULL;}

private:
    MediaBufferGroup mGroup;
    RVSD_INFO *pRvsdInfo;
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
    DummySource(const DummySource &);
    DummySource &operator=(const DummySource &);
};

class VDecOMX : public RefBase {
public:
    VDecOMX(RVSD_INFO* pRvsd_info,pthread_t *pThreadVideo);
    void VDecInit(void);
    void VDecUninit(void);
    void * VDec_decode_loop(void);

    void OMX_Connect(void);
    void OMX_Disconnect(void);


//protected:
    virtual ~VDecOMX();

private:
    sp<MediaSource> mVideoDecoder;
    sp<MediaSource> mediaSource;
    sp<MediaSource> mVideoSource;
    OMXClient mClient;
    sp<ANativeWindow> gSurface;

    sp<SurfaceComposerClient> composerClient;
    sp<SurfaceControl> control;
    //sp<AwesomeRenderer> mVideoRenderer; //FIXME: AwesomeRenderer no longer exists

    RVSD_INFO *pRvsdInfo;
};

}  // namespace android

#endif

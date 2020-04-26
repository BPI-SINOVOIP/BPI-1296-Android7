/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
#ifndef _CAMERAREADER_H_
#define _CAMERAREADER_H_

#include <utils/Log.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <utils/List.h>
#include <media/stagefright/MediaBuffer.h>
#include <gui/BufferItemConsumer.h>
#include <binder/MemoryBase.h>
#include <libRTKAllocator/include/hardware/GrallocWrapper.h>

#include "RtkTranscodeCommon.h"

namespace android {


struct ALooper;
struct AHandler;
class Statistics;
class Camera;
class CameraParameters;

class CameraReader : public RtkFilter
{
public:
    CameraReader();

    enum {
        /* Player --> CameraReader */
        kWhatSetCameraSource = 0x700,
        kWhatSetCameraSource2,
        kWhatConfigure,
        kWhatPrepare,
        kWhatSetPlayback,
        kWhatSetEncode,
        kWhatStart,
        kWhatGetSnapshot,
        kWhatGetCameraParam,
        kWhatSetEncodeCleanup, // added for buffer queue mode
        kWhatConfigurePreview,
        kWhatStop = WHAT_STOP,
        kWhatSetSurface = WHAT_SET_SURFACE,

        /* Agent --> CameraReader */
        kWhatDrainBufferDone = 0x710,
        kWhatDrainBufferHandleDone,

        /* CameraReader --> CameraReader */
        kWhatReadBufferQueue = 0x800,
        kWhatResetTargetFormat,
    };

    status_t setPlayerHandler(AHandler *pHandler) {
        mPlayerHandler = pHandler;
        return OK;
    }

protected:
    virtual ~CameraReader();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    friend struct CameraReaderListener;

    enum State {
        UNINITIALIZED,
        WAIT_SURFACE,
        INITIALIZED,
        CONFIGURED,
        PREPARED,
        STARTED,
        STOPPED,
    };

    State mState;

    bool mPreviewCamera;
    bool mEncodeCamera;
    bool mHdmiRx;

    String16 mClientName;
    int mCameraId;
    int mSrcHdmiRx;
    int mSrcReady;
    int mSrcProgressive;
    int mSrcColor;
    int mSrcWidth;
    int mSrcHeight;
    int mSrcPreviewWidth;
    int mSrcPreviewHeight;
    int mSrcFps;
    unsigned int mFrameCount;
    unsigned int mEncodeCount;
    int mTargetWidth;
    int mTargetHeight;
    int mTargetFps;

    // for VIDEO_BUFFER_MODE_BUFFER_QUEUE record
    GrallocWrapper *pGrallocWrapper;
    int64_t mGlitchDurationThresholdUs;
    int32_t mNumGlitches;
    int mVideoBufferMode;
    //sp<MetaData> mMeta;

    static const size_t kConsumerBufferCount = 8;
    static const nsecs_t kMemoryBaseAvailableTimeoutNs = 200000000; // 200ms

    /**
     * The class for listening to BufferQueue's onFrameAvailable. This is used to receive video
     * buffers in VIDEO_BUFFER_MODE_BUFFER_QUEUE mode. When a frame is available,
     * CameraSource::processBufferQueueFrame() will be called.
     */
    class BufferQueueListener : public Thread,  public BufferItemConsumer::FrameAvailableListener {
    public:
        BufferQueueListener(const sp<BufferItemConsumer> &consumer,
                const sp<CameraReader> &cameraReader);
        virtual void onFrameAvailable(const BufferItem& item);
        virtual bool threadLoop();
        virtual void onFirstRef() {
            initFrameCounter();
        }
    private:
        static const nsecs_t kFrameAvailableTimeout = 50000000; // 50ms

        sp<BufferItemConsumer> mConsumer;
        sp<CameraReader> mCameraReader;

        Mutex mLock;
        Condition mFrameAvailableSignal;
        bool mFrameAvailable;
        void initFrameCounter();
        int64_t mFrameReceived;
        int32_t mDivide;
    };

    Mutex mLock;
    sp<BufferItemConsumer> mVideoBufferConsumer;
    sp<IGraphicBufferProducer> mVideoBufferProducer;
    sp<IMemoryHeap> mMemoryHeapBase;
    List<sp<IMemory>> mMemoryBases;
    KeyedVector<native_handle_t*, BufferItem> mReceivedBufferItemMap;
    sp<BufferQueueListener> mBufferQueueListener;
    int32_t mEncoderDataSpace;
    int32_t mNumInputBuffers;
    Condition mMemoryBaseAvailableCond;
    Condition mFrameCompleteCondition;
    List<sp<IMemory> > mFramesReceived;
    List<sp<IMemory> > mFramesBeingEncoded;
    List<int64_t> mFrameTimes;
    Condition mFrameAvailableCondition;
    int64_t mStartTimeUs;
    int64_t mFirstFrameTimeUs;
    int64_t mLastFrameTimestampUs;
    int64_t mTimeBetweenFrameCaptureUs;
    int32_t mNumFramesDropped;
    int32_t mNumFramesEncoded;
    int32_t mNumFramesReceived;
    // end VIDEO_BUFFER_MODE_BUFFER_QUEUE members


    sp<Camera> mCamera;
    sp<Surface> mSurface;
    CameraParameters mParameters;
    sp<IMemory> mSnapshotBuffer;
    pthread_mutex_t mSnapMtx;
    pthread_cond_t mSnapCnd;
    CAMERA_SNAPSHOT mSnapParam;
    Statistics mDropStat;
    sp<AHandler> mPlayerHandler;

    status_t setPreviewStatus(bool preview);
    status_t setPlayback(int previewCamera);
    status_t setEncode(int encodeCamera);
    status_t setEncodeCleanup(int encodeCamera);
    status_t start();
    status_t stop();
    status_t prepare();
    status_t getSnapshot(const sp<AMessage> &msg, sp<AMessage> &response);
    status_t retrieveHdmiRxVideoInfo();

    void dataCallback(int32_t msgType, const sp<IMemory> &data, camera_frame_metadata_t * metadata);
    void dataCallbackTimestamp(int64_t timestampUs, int32_t msgType, const sp<IMemory> &data);
    void handleCallbackTimestamp(int64_t timestampUs, native_handle_t* handle);

    // Following are the APIs used in VIDEO_BUFFER_MODE_BUFFER_QUEUE
    void initBufferQueueControlVariables();
    status_t initBufferQueue(uint32_t width, uint32_t height, uint32_t format,
                  android_dataspace dataSpace, uint32_t bufferCount);

    status_t reconfigureCamera(
            uint32_t videoWidth,
            uint32_t videoHeight);

    void createVideoBufferMemoryHeap(size_t size, uint32_t bufferCount);
    virtual void processBufferQueueFrame(BufferItem& buffer);
    bool shouldSkipFrameLocked(int64_t timestampUs);
    status_t read();
    virtual void releaseRecordingFrame(const sp<IMemory>& frame);
    virtual void releaseRecordingFrameHandle(native_handle_t* handle);
    void releaseQueuedFrames();
    void releaseOneRecordingFrame(const sp<IMemory>& frame);
    void handleBufferQueueFrame(int64_t timestampUs, native_handle_t *handle);
    void getPhyAddr(buffer_handle_t *buffer, uint32_t *phyAddr);
    void returnBufferQueue(native_handle_t *pHandle);
    void returnAllBufferQueue();
    status_t resetBufferQueueStuff();
    void clearVideoBufferMemoryHeap();

    DISALLOW_EVIL_CONSTRUCTORS(CameraReader);
};

} // namespace android

#endif //_CAMERAREADER_H_

/*
 * Copyright 2015 Realtek Semiconductor Corp.
 */
#ifndef _HDMIHWSIDEBAND_H_
#define _HDMIHWSIDEBAND_H_

#include <String8.h>
#include <utils/threads.h>
#include <ui/GraphicBufferAllocator.h>
#include <ui/GraphicBufferMapper.h>
#include <gui/SidebandClient.h>
#include <gui/SidebandControl.h>
#include <gui/Surface.h>
#include <gui/BufferQueue.h>
#include <gui/IConsumerListener.h>
#include <gui/IProducerListener.h>
#include <hardware/gralloc.h>
#include <cutils/properties.h>
#include <android/native_window.h>
#include <libRTKAllocator/include/hardware/GrallocWrapper.h>
#include "TvInputHw.h"
#include "HdmiMonitor.h"
#include "SidebandBuf.h"
#include "V4L2DeviceSideband.h"
#include "HDMIRxAudioReader.h"

class HdmiHwSideband : public TvInputHw
{
public:
    HdmiHwSideband(int device_id, tv_stream_t *stream, uint32_t w , uint32_t h);
    ~HdmiHwSideband();
    // no function..
    virtual int requestCapture(buffer_handle_t buffer, uint32_t seq);
    virtual int cancelCapture(uint32_t seq);

    // Sideband APIs
    bool createSideband(tv_stream_t *stream);
    void closeSideband();

    void releaseBuffers();
    void stopStreamingThreads();

    // thread entries
    int sidebandThread();
    int v4l2Thread();

    // Queue access api, protected by mutex
    BufInfo *sbDqDisplayQueue();
    void sbQCaptureQueue(BufInfo *pInfo);
    BufInfo *v4l2DqCaptureQueue();
    void v4l2QDisplayQueue(BufInfo *pInfo);

    // helper apis
    BufInfo *findBufInfoViaANB(ANativeWindowBuffer *pANB);

    void saveRawToFile(BufInfo *pBuf);

private:
    // Sideband control variables
    sp<SidebandClient> mClient;
    sp<SidebandControl> mControl;

    int mDeviceId;      // un-used now, but keep it
    int mStreamId;      // un-used now, but keep it
    int mStreamType;    // un-used now, but keep it

    uint32_t mW;
    uint32_t mH;

    uint32_t mSidebandThreadLogCnt;
    uint32_t mV4L2ThreadLogCnt;

    int mV4L2PixelFormat;
    int mHalPixelFormat;

    tv_stream_t         *mStreamConfig;
    HdmiMonitor         *mMonitor;
    V4L2DeviceSideband  *mDev;

    // Sideband control variables
    int mBufInfoCount;
    sp<ANativeWindow> mANW;

    BufInfo *mPBufInfos;

    // ION related
    const struct hw_module_t *mGrallocModule;
    alloc_device_t *mAllocDev;
    GrallocWrapper      *pGrallocWrapper;

    // Sideband thread 
    class SidebandThread : public Thread {
        HdmiHwSideband* mHw;
    public:
        SidebandThread(HdmiHwSideband* hw)
            : Thread(false), mHw(hw) { }

        virtual ~SidebandThread();

        virtual void onFirstRef() {
            run("SidebandThread", PRIORITY_URGENT_DISPLAY);
        }
        virtual bool threadLoop() {
            int rst = mHw->sidebandThread();
            return (rst>=0)? true:false;
        }
    };

    // V4L2 thread
    class V4L2Thread : public Thread {
        HdmiHwSideband* mHw;
    public:
        V4L2Thread(HdmiHwSideband* hw)
            : Thread(false), mHw(hw) { }

        virtual ~V4L2Thread();

        virtual void onFirstRef() {
            run("V4L2Thread", PRIORITY_DISPLAY);
        }
        virtual bool threadLoop() {
            int rst = mHw->v4l2Thread();
            return (rst>=0)? true:false;
        }
    };

    sp<SidebandThread> mSidebandThread;
    sp<V4L2Thread> mV4l2Thread;

    bool mSidebandThreadAlive;
    bool mV4L2ThreadAlive;

    // lock to protect multiple thread access
    mutable Mutex mDisplayLock;
    mutable Mutex mCaptureLock;
    mutable Mutex mFlowLock;

    Condition mFrameAvailableSignal;
    bool mFrameAvailable;

    // vector to keep buffer infos
    Vector<BufInfo *> mDisplayQueue;
    Vector<BufInfo *> mCaptureQueue;
    int mSidebandBufQueueCnt;
    int mMinUndequeueBufCnt;

    // audio helper class
    HDMIRxAudioReader *mAudioReader;

    int mSaveRawCount;
    nsecs_t calcFrameInterval(nsecs_t timestamp);
};

#endif //_HDMIHWSIDEBAND_H_

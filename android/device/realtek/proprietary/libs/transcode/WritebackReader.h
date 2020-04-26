/*
 * Copyright (c) 2017 Realtek Semiconductor Corp.
 */
#ifndef _WRITEBACKREADER_H_
#define _WRITEBACKREADER_H_

#include <utils/Log.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <include/RTKScreenRecord.h>

#include "RtkTranscodeCommon.h"

#define WB_BUF_NUM 6

namespace android {

struct ALooper;
struct AHandler;
class Statistics;
class Camera;
class CameraParameters;

class WritebackReader : public RtkFilter
{
public:
    WritebackReader();

    enum {
        /* Player --> WritebackReader */
        kWhatSetCameraSource = 0x700,
        kWhatSetCameraSource2,
        kWhatConfigure,
        kWhatPrepare,
        kWhatSetPlayback,
        kWhatSetEncode,
        kWhatStart,
        kWhatGetSnapshot,
        kWhatGetCameraParam,
        kWhatConfigurePreview,
        kWhatStop = WHAT_STOP,
        kWhatSetSurface = WHAT_SET_SURFACE,

        /* Agent --> WritebackReader */
        kWhatDrainBufferDone = 0x710,

        /* Player --> WritebackReader */
        kWhatGetWritebackFrame = 0x720,
    };

    status_t setPlayerHandler(AHandler *pHandler) {
        mPlayerHandler = pHandler;
        return OK;
    }

protected:
    virtual ~WritebackReader();
    virtual void onMessageReceived(const sp<AMessage> &msg);

    // Callback called when still camera raw data is available.
    virtual void dataCallback(int32_t msgType, const sp<IMemory> &data);
    virtual void dataCallbackTimestamp(int64_t timestampUs, int32_t msgType, const sp<IMemory> &data);
    virtual bool getWritebackVideoFrame();

private:
    friend class WritebackCallbackHandler;

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

    class CaptureThread : public Thread{
        WritebackReader* mWritebackReader;
        public:
            CaptureThread(WritebackReader* wb)
                : Thread(false), mWritebackReader(wb) {}
            virtual void onFirstRef(){
                run("TranscodeWritebackCaptureThread", PRIORITY_DISPLAY);
            }
            virtual bool threadLoop(){
                mWritebackReader->captureThread();
                // loop until we need to quit
                return true;
            }
    };
    status_t captureThread();
    sp<CaptureThread>       mCaptureThread;
    bool mCaptureStopped;

    bool mPreviewCamera;
    bool mEncodeCamera;
    bool mHdmiRx;
    bool mFirstTime;

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
    int mDivide;
    unsigned int mFrameReceived;
    sp<Camera> mCamera;
    sp<Surface> mSurface;
    CameraParameters mParameters;
    sp<IMemory> mSnapshotBuffer;
    pthread_mutex_t mSnapMtx;
    pthread_cond_t mSnapCnd;
    CAMERA_SNAPSHOT mSnapParam;
    Statistics mDropStat;
    sp<AHandler> mPlayerHandler;
    ScreenRecordService* mRecordService;

    int                 mWritebackType;
    int                 mWbBufCount;
    Vector<SRBuffer*>   mWritebackBufferArray;
    mutable Mutex       mUnusedLock;
    nsecs_t             prevTs;

    status_t setPreviewStatus(bool preview);
    status_t start();
    status_t stop();
    status_t getSnapshot(const sp<AMessage> &msg, sp<AMessage> &response);
    status_t retrieveHdmiRxVideoInfo();
    void dumpWbFrame(void *buf, int cnt);

    DISALLOW_EVIL_CONSTRUCTORS(WritebackReader);
};

} // namespace android

#endif //_WRITEBACKREADER_H_

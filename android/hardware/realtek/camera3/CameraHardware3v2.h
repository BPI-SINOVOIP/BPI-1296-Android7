#ifndef ANDROID_CAMERA_HARDWARE_3_V2
#define ANDROID_CAMERA_HARDWARE_3_V2

#include <utils/Mutex.h>
#include "CameraHardware3Base.h"
#include "V4L2Device2Common.h"
#include "FormatConvertHelper.h"

using namespace androidcamera3;

namespace android {

class CameraHardware3v2 : public CameraHardware3Base,
                            public V4L2Device2Callback {

    /**
     * Thread to return capture result and call notify
     */
    class ShutterThread : public Thread {
    public:
        ShutterThread(CameraHardware3v2 *parent);
        ~ShutterThread();
        void                    signal();
        void                    stop();
    private:

        virtual bool            threadLoop();

        enum {
            INPUT_QUEUE,
            OUTPUT_QUEUE,
            NO_QUEUE,
        } mQueueIndex;

        Mutex                   mThreadLock;
        Condition               mSignal;
        CameraHardware3v2*      mParent;
        bool                    mRunning;
    };

public:
    CameraHardware3v2(int camId,
        int v4l2Id,
        int facing);

    ~CameraHardware3v2();

    status_t connectCamera(/*hw_device_t** device*/);
    status_t closeCamera();
    status_t configureStreams(camera3_stream_configuration_t *streamList);
    status_t processCaptureRequest(const Request &r);
    status_t flush();

    void submitRequest(Request *r);
    void submitErrorRequest(Request *r);

    /**
     * V4L2Device2Callback
     */
    void returnRequest(Request *r);

private:
    Mutex                       mLock;
    V4L2Device2Common*          mDevice;
    bool                        mFlushed;
    V4L2Params                  mV4L2Params;
    //int32_t                   mErrorFrames;
    int32_t                     mResultFrameNumber;
    int32_t                     mRequestFrameNumber;

    nsecs_t                     mPrevSystemTime;
    nsecs_t                     mPrevPTS;

    Mutex                       mOutputRequestQueueLock;
    Vector<Request *>           mOutputRequestQueue;

    Mutex                       mInputRequestQueueLock;
    Vector<Request *>           mInputRequestQueue;

    Mutex                       mFrameNumberQueueLock;
    Vector<int32_t>             mFrameNumberQueue;

    sp<ShutterThread>           mShutterThread;
    FormatConvertHelper         *mFormatConvertHelper;

    void setV4L2Device(V4L2Params *params);

    bool queueCaptureRequest(const Request &r);
    void reportFailedRequest(const Request &r);

    void queueFinishedOutputRequest(Request *r);
    void dequeueOutputRequest();

    void queueReprocessRequest(const Request &r);
    void dequeueReprocessRequest();

    void queueFrameNumber(int32_t num);
    void dequeueFrameNumber();

    void signalDeviceIdle() { mStatus = STATUS_READY; }

    // implement it for CTS only.
    void submitReprocessRequest(Request *r);

    void removeShutterThread();
}; // class CameraHardware3v2

}; // namespace android

#endif // ANDROID_CAMERA_HARDWARE_3_V2

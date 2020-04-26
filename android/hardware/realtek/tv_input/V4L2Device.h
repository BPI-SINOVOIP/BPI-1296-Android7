/*
 * Copyright 2015 Realtek Semiconductor Corp.
 */
#ifndef _V4L2DEVICE_H_
#define _V4L2DEVICE_H_

#include <linux/videodev2.h>
#include <system/window.h>
#include <hardware/hardware.h>
// #define USE_RT_ION //for gralloc.h
#include <hardware/gralloc.h>
#include <utils/SortedVector.h>
#include <utils/threads.h>
#include <utils/Mutex.h>

using android::SortedVector;
using android::Thread;
using android::Mutex;
using android::sp;

enum {
    REQ_CAPTURE,
    REQ_CANCEL,
};

class CaptureThread;

struct CaptureRequest
{
    CaptureRequest() : buffer(NULL), seq(0), status(REQ_CAPTURE) { };
    CaptureRequest(buffer_handle_t buffer, uint32_t seq) : buffer(buffer), seq(seq), status(REQ_CAPTURE) { };
    bool operator < (const CaptureRequest &rhs) const { return (seq < rhs.seq); }

    buffer_handle_t buffer;
    uint32_t seq;
    int status;
};

typedef void (*callback_t)(void *hw, buffer_handle_t buffer, uint32_t seq, int result);

class V4L2Device {
public:
    V4L2Device();
    ~V4L2Device();
    int init(const char *device, int width, int height, int pixelformat, int numBuffers);
    int uninit();
    int registerCallback(void *hw, callback_t cb);
    int requestCapture(buffer_handle_t buffer, uint32_t seq);
    int cancelCapture(uint32_t seq);
    int captureThread();

protected:

private:
    callback_t                      mCb;
    void                            *mHw;
    int                             mFd;
    unsigned long                   mSeHandle;
    const struct hw_module_t        *mGrallocModule;
    bool                            mStopCapture;
    sp<CaptureThread>               mCaptureThread;

    SortedVector<CaptureRequest>    mRequestQueue;
    Mutex                           mQLock;
};

class CaptureThread : public Thread {
public:
    CaptureThread(V4L2Device *dev) : Thread(false), mDevice(dev) { }
    virtual void onFirstRef() { run("CaptureThread", android::PRIORITY_DEFAULT); }
    virtual bool threadLoop() { mDevice->captureThread(); return true; }

private:
    V4L2Device *mDevice;
};

#endif //_V4L2DEVICE_H_

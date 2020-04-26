/*
 * Copyright 2015 Realtek Semiconductor Corp.
 */
#ifndef _V4L2DEVICE_SIDEBAND_H_
#define _V4L2DEVICE_SIDEBAND_H_

#include <linux/videodev2.h>
#include <system/window.h>
#include <hardware/hardware.h>
#include <hardware/gralloc.h>
#include <utils/SortedVector.h>
#include <utils/threads.h>
#include <utils/Mutex.h>
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

#include "SidebandBuf.h"
#include "HdmiMonitor.h"

using namespace android;

struct vdIn {
    struct v4l2_capability cap;
    struct v4l2_format format;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers rb;
    bool isStreaming;
    bool isInited;
    int fps;
    int width;
    int height;
    int formatIn;
    int framesizeIn;
    char devName[32];
};

// TODO: un-used now, might need it in the future
struct vdOut {
    unsigned char *data;
    int size;
    int format;
    int index;
    nsecs_t timestamp;
};

class V4L2DeviceSideband {
public:
    V4L2DeviceSideband();
    ~V4L2DeviceSideband();

    int init(const char *device,
            int width,
            int height,
            int pixelformat,
            int numBuffers,
            BufInfo *pBufInfos,
            HdmiMonitor *monitor);

    int uninit();
    int startStreaming();
    int stopStreaming();

    // basic API opened by V4L2Device
    bool captureNewFrame(BufInfo *pInfo);
    bool getFrame(BufInfo **ppInfo);
    void closeDev();

private:

    void                            *mHw;
    int                             mFd;
    const struct hw_module_t        *mGrallocModule;
    BufInfo                         *mPBufInfos;
    int                             mBufferCount;
    struct vdIn                     videoIn;
};

#endif //_V4L2DEVICE_SIDEBAND_H_

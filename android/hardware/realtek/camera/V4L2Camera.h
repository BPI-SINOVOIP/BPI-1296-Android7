/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      Author: Niels Keeman <nielskeeman@gmail.com>
 *
 */

#ifndef _V4L2CAMERA_H
#define _V4L2CAMERA_H

#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <utils/List.h>
#include <utils/Vector.h>
#include <utils/KeyedVector.h>
#include <linux/videodev2.h>
#include <hardware/camera.h>

#define WB_BUFFER 14 // buffer for VOWB and previewThread [MUST BE EVEN NUMBER]
#define CAPTURE_BUFFER 6 // buffer for V4L2 and captureThread

/* VIDIOC_MIPI_IN_TYPE */
struct v4l2_mipi_in_type {
    __u32 input_type; /* 0 MIPI ; 1:HDMI */
} __attribute__ ((packed));

#define VIDIOC_QUERY_MIPI_IN_TYPE  _IOWR('V', BASE_VIDIOC_PRIVATE+0, struct v4l2_mipi_in_type)
#define VIDIOC_SET_MIPI_IN_TYPE  _IOWR('V', BASE_VIDIOC_PRIVATE+1, struct v4l2_mipi_in_type)

namespace android {

class String8;

struct Res {
    int width;
    int height;

    Res() {
        width = 0;
        height = 0;
    }

    Res(int w, int h) {
        width = w;
        height = h;
    }

    bool operator< (const Res &rhs) const {
        if(width < rhs.width)
            return true;
        if(width == rhs.width && height < rhs.height)
            return true;
        return false;
    }
};

struct vdIn {
    struct v4l2_capability cap;
    struct v4l2_format format;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers rb;
    Vector<void *> mem;
    bool isStreaming;
    int fps;
    int width;
    int height;
    int formatIn;
    int framesizeIn;
    char devName[32];
};

struct vdOut {
    unsigned char *data;
    int size;
    int format;
    int index;
    nsecs_t timestamp;
};

struct VBuffer {
    void *data;
    size_t size;
};

class V4L2Camera {

public:
    V4L2Camera();
    ~V4L2Camera();

    int Open (const char *device);
    int Setup(int width, int height, int pixelformat, int fps = -1);
    void Close ();

    int Init (Vector<VBuffer> &buffers, bool vowbRunning);
    int Init (int numBuffers);
    void Uninit ();

    int StartStreaming ();
    int StopStreaming ();
    bool isStreaming();

    int queryCapabilities(int format);

    String8 getFormatList();
    String8 enumAndGetFormatList();
    String8 getSizeList();
    String8 getFpsList();
    String8 getFpsRangeList();

    String8 getFormat(size_t idx, int &fmt);
    String8 getSize(size_t idx, int &width, int &height);
    bool getMinSize(int &width, int &height);
    String8 getFps(size_t idx, int &fps);
    //String8 getFpsRange(size_t idx, int &bfps, int &efps);
    String8 getFpsRange();
    void getFullFpsRange(int *fpsmin, int *fpsmax);

    int GrabPreviewFrame (struct vdOut **frame);
    void ReleasePreviewFrame (int index);
    sp<IMemory> GrabRawFrame ();
    camera_memory_t*   GrabJpegFrame (camera_request_memory   mRequestMemory);

	int isFormatSupported(int fmt);
	int isSizeSupported(int widht, int height);
	int getClosestFps(int width, int height, int pixelformat, int fps);
    int QueryBufferSize(int BufferCount);

private:
    int fd;

    int nQueued;
    int nDequeued;
    struct vdIn videoIn;
	struct vdOut videoOut;
	KeyedVector<int, String8> mFormatList;
	KeyedVector<Res, String8> mSizeList;
	KeyedVector<int, String8> mFpsList;

	KeyedVector<int, v4l2_buffer> mCapturedQueue;
	Mutex mLock;

    enum v4l2_memory          mMemoryType;

    int enumPixelFormat();
    int enumFrameSize(int fmt);
    int enumFrameRate(int fmt, int width, int height);
#ifdef HAVE_JPEG
    int saveYUYVtoJPEG (unsigned char *inputBuffer, int width, int height, FILE *file, int quality);
#endif
    void convert(unsigned char *buf, unsigned char *rgb, int width, int height);
    void yuv_to_rgb16(unsigned char y, unsigned char u, unsigned char v, unsigned char *rgb);

};

}; // namespace android

#endif

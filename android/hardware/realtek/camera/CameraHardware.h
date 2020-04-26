/*
**
** Copyright 2009, The Android-x86 Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** Author: Niels Keeman <nielskeeman@gmail.com>
**
*/

#ifndef ANDROID_HARDWARE_CAMERA_HARDWARE_H
#define ANDROID_HARDWARE_CAMERA_HARDWARE_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <linux/videodev2.h>
#include <utils/threads.h>
#include <utils/Vector.h>
#include <utils/Log.h>
#include <utils/threads.h>
#include <utils/Statistics.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <camera/CameraParameters.h>
//#include <media/IEncodeBuffer.h>
#include <hardware/camera.h>
#include "MetadataBuffer.h"
#include "V4L2Camera.h"
#include "CameraOmxDecode.h"
#include "HdcpMonitor.h"
#include "CameraVoWriteBack.h"

namespace android {

#define USE_USERPTR_MODE //need to allocate contiguous buffer for V4L2 device
#define HAVE_SE_ENGINE
//#define CALLBACK_ALIGNED_BUFFER
//#define CALLBACK_UNALIGNED_NV12_BUFFER
#define USE_OMX_DECODEC
//#define HDMIRX_AVSYNC
#ifdef HDMIRX_AVSYNC
#define NB_BUFFER 20
#else
#define NB_BUFFER 8
#endif

struct ALooper;
struct CameraDecode;

enum {
    USED_BY_NOBODY = 1 << 0,
    USED_BY_DEVICE = 1 << 1,
    USED_BY_PREVIEW = 1 << 2,
    USED_BY_APP = 1 << 3,
    USED_BY_VO = 1 << 4,
};

typedef struct {
    int             index;
    camera_memory_t *mem;
    void            *meta;
    buffer_handle_t *handle;
    int64_t         timestamp;
    void            *data;
    unsigned long   phys;
    size_t          size;
    unsigned int    usage;
} FrameInfo;

typedef struct _private_rtk_data
{
    unsigned int        yuv_mode;
    unsigned long long  pts[2];
    int                 delay_mode;
    int                 delay_depth;
    unsigned int        IsForceDIBobMode; //force deinterlace bob mode
    int                 init_frame;
    int                 deintflag; // 0:AUTODEINT 1:FORCEDEINT 2:FORCEPROGRESSIVE
    int                 ssid;
    unsigned int        lumaOffTblAddr;     /* legacy */
    unsigned int        chromaOffTblAddr;   /* legacy */
    unsigned int        lumaOffTblAddrR;    /* legacy */
    unsigned int        chromaOffTblAddrR;  /* legacy */
    unsigned int        bufBitDepth;
    unsigned int        bufFormat;
    unsigned int        transferCharacteristics;
    unsigned int        display_primaries_x0;
    unsigned int        display_primaries_y0;
    unsigned int        display_primaries_x1;
    unsigned int        display_primaries_y1;
    unsigned int        display_primaries_x2;
    unsigned int        display_primaries_y2;
    unsigned int        white_point_x;
    unsigned int        white_point_y;
    unsigned int        max_display_mastering_luminance;
    unsigned int        min_display_mastering_luminance;
    unsigned int        display_width;
    unsigned int        display_height;
    unsigned int        hdmi_flag; // bit[0] : HDCP
    unsigned long long  hdmi_hdcp_key;
    unsigned long long  hdmi_hdcp_key_response;
    unsigned int        reserve[64-35];
} private_rtk_data;

typedef struct {
    int             index;
    struct vdOut    videoOut;
} FramePair;

typedef struct {
    int             idx0; // top field
    int             idx1; // bottom field
} VoFramePair;

enum {
    COLOR_RGB,
    COLOR_YUV444,
    COLOR_YUV422,
    COLOR_UNKNOWN,
};

typedef struct {
    bool            hdmirx;     //true:hdmirx  false:mipi camera
    bool            status;
    bool            interlaced;
    int             width;
    int             height;
    int             color;
    int             fps;
} HdmiRxInfo;

class CameraHardware
{
public:
    virtual status_t    startPreview();
    virtual void        setCallbacks(camera_notify_callback notify_cb,
                                     camera_data_callback data_cb,
                                     camera_data_timestamp_callback data_cb_timestamp,
                                     camera_request_memory get_memory,
                                     void* arg);
    /**
     * Enable a message, or set of messages.
     */
    virtual void        enableMsgType(int32_t msgType);

    virtual int setupPrevieWindow(int width, int height);
    virtual int setPreviewWindow( struct preview_stream_ops *window);
    /**
     * Disable a message, or a set of messages.
     */
    virtual void        disableMsgType(int32_t msgType);

    /**
     * Query whether a message, or a set of messages, is enabled.
     * Note that this is operates as an AND, if any of the messages
     * queried are off, this will return false.
     */
    virtual bool        msgTypeEnabled(int32_t msgType);

    virtual void        stopPreview();
    virtual bool        previewEnabled();

    virtual status_t    startRecording();
    virtual void        stopRecording();
    virtual bool        recordingEnabled();
    virtual void        releaseRecordingFrame(const void* opaque);

    virtual status_t    autoFocus();
    virtual status_t    cancelAutoFocus();
    virtual status_t    takePicture();
    virtual status_t    cancelPicture();
    virtual status_t    dump(int fd, const Vector<String16>& args) const;
    virtual status_t    setParameters(const CameraParameters& params);
    virtual CameraParameters  getParameters() const;
    virtual void        release();
    virtual status_t    sendCommand(int32_t cmd, int32_t arg1, int32_t arg2);
    virtual int         storeMetaDataInBuffers(int enable);

                        CameraHardware(int cameraId);

    virtual             ~CameraHardware();

    virtual bool        Get_HDMI_RX_Video_Info(const char* pszFindName, char* pszResult, int nCbSize);
    virtual void        getHdmiRxVideoInfo();

private:


    static const int kBufferCount = 4;

    class PreviewThread : public Thread {
        CameraHardware* mHardware;
    public:
        PreviewThread(CameraHardware* hw)
            : Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("CameraPreviewThread", PRIORITY_URGENT_DISPLAY);
        }
        virtual bool threadLoop() {
            mHardware->previewThread();
            // loop until we need to quit
            return true;
        }
    };

    class CallbackThread : public Thread {
        CameraHardware* mHardware;
    public:
        CallbackThread(CameraHardware* hw)
            : Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("CameraCallbackThread", PRIORITY_DISPLAY);
        }
        virtual bool threadLoop() {
            mHardware->callbackThread();
            // loop until we need to quit
            return true;
        }
    };

    class CaptureThread : public Thread {
        CameraHardware* mHardware;
    public:
        CaptureThread(CameraHardware* hw)
            : Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("CameraCaptureThread", PRIORITY_DISPLAY);
        }
        virtual bool threadLoop() {
            mHardware->captureThread();
            // loop until we need to quit
            return true;
        }
    };

    class NwDequeueThread : public Thread {
        CameraHardware* mHardware;
    public:
        NwDequeueThread(CameraHardware* hw)
            : Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("NwDequeueThread", PRIORITY_DISPLAY);
        }
        virtual bool threadLoop() {
            mHardware->nwDequeueThread();
            // loop until we need to quit
            return true;
        }
    };

    class RestartFlowThread : public Thread {
        CameraHardware* mHardware;
    public:
        RestartFlowThread(CameraHardware* hw)
            : Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("RestartFlowThread", PRIORITY_DISPLAY);
        }
        virtual bool threadLoop() {
            return mHardware->restartPreviewFlow();
        }
    };

    class CancelFlowThread : public Thread {
        CameraHardware* mHardware;
    public:
        CancelFlowThread(CameraHardware* hw)
            : Thread(false), mHardware(hw) { }
        virtual void onFirstRef() {
            run("CancelFlowThread", PRIORITY_DISPLAY);
        }
        virtual bool threadLoop() {
            mHardware->stopPreview();
            return false;
        }
    };

    void initDefaultParameters();
    bool initHeapLocked();

    int previewThread();
    int callbackThread();
    int captureThread();
    int nwDequeueThread();

    static int beginAutoFocusThread(void *cookie);
    int autoFocusThread();

    static int beginPictureThread(void *cookie);
    int pictureThread();

    //bool adjustPreviewDimensions();
    //bool isPreviewTime();
    int restartStreaming(int width, int height, int pixfmt, int fps);

    status_t runPreviewFlow();
    void cancelPreviewFlow();
    status_t restartPreviewFlow();

    void        sendNotify(int32_t msgType, int32_t ext1, int32_t ext2);
    void        sendError(int32_t reason);

    int convertVideoFormat(int srcFmt, int dstFmt, unsigned char *srcBuf, unsigned char *dstBuf, size_t srcSize, int width, int height, int aligned, int cropX = 0, int cropY = 0, int cropWidth = 0, int cropHeight = 0, int outWidth = 0, int outHeight = 0);
    bool AddExifToJpeg(camera_memory_t *pSrcJpg, camera_memory_t *pThumbnailJpg, camera_memory_t **ppResultJpg);
    bool MakeJpegThumbnail(uint32_t srcPhyBuffer, camera_memory_t** ppJpeg);
#ifdef HAVE_SE_ENGINE
    bool ScaleYuv(uint32_t srcPhyBuffer, camera_memory_t** ppYuv, int iScaleWidthTo, int iScaleHeightTo);
    bool ScaleAndNv1221(uint32_t srcPhyBuffer, camera_memory_t** ppYuv, int iScaleWidthTo, int iScaleHeightTo);
    bool se_init(alloc_device_t  **ppAllocDev);
    void se_uninit(alloc_device_t  **ppAllocDev);
    bool se_Create420spPhyBuffer(alloc_device_t  *pAllocDev, int iWidth, int iHeight, int format, buffer_handle_t *pBufferHandle, uint32_t *pPhyBufferHandle);
    bool se_DestroyPhyBuffer(alloc_device_t  *pAllocDev, buffer_handle_t pBufferHandle);
    bool se_PhyBuf2VirBuf(int iWidth, int iHeight, buffer_handle_t pBufferHandle, void **pvirtaddr);
    void se_nv1221(uint32_t Srcphys, size_t *pSize);
    void se_nv1221(uint32_t Srcphys, uint32_t Dstphys, int iPicture_NoAlign_Width, int iPicture_NoAlign_Height);
#endif
    mutable Mutex           mLock;
    preview_stream_ops_t*   mNativeWindow;

    int                     mMinor;
    CameraParameters        mParameters;

    int                     mPreviewFrameWidth;
    int                     mPreviewFrameHeight;
    int                     mPreviewFrameRate;
    int                     mSrcFormat;             //format used to capture from V4L2 device
    int                     mNativeWindowFormat;    //the format used by native window
    int                     mPreviewFormat;         //the preview format callback to application
    int                     mVideoFormat;           //the video format callback to application
    int                     mIsMjpegAvi1Format;
    sp<PreviewThread>       mPreviewThread;
    sp<CaptureThread>       mCaptureThread;
    sp<CallbackThread>      mCallbackThread;
    sp<NwDequeueThread>     mNwDequeueThread;

    bool                    mPreviewEnabled;
    bool                    mPreviewStopped;
    bool                    mCallbackStopped;
    bool                    mCaptureStopped;
    bool                    mNwDequeueStopped;
    bool                    mTakingPicture;
    bool                    mRecordRunning;
    bool                    mPreviewPending;
    bool                    mHdcpStatus;
    HdcpMonitor             *mMonitor;
    bool                    mQueueCancelBuffer;
    int64_t                 mHdcpTimeSend;
    int64_t                 mHdcpTimeResponse;

    V4L2Camera              camera;

    camera_request_memory   mRequestMemory;
    camera_notify_callback  mNotifyFn;
    camera_data_callback    mDataFn;
    camera_data_timestamp_callback mTimestampFn;

    void*                   mUser;
    int32_t                 mMsgEnabled;

    bool                    mStoreMetaDataInBuffers;
    bool                    mIntermediateMode;      //true if we need to copy/convert from v4l2 buffer
    KeyedVector<const void *, int> mMetaMap;        //mapping metadata struct to buffer index
    KeyedVector<buffer_handle_t *, int> mHandleMap; //mapping native buffer handle to buffer index
    List<int>               mUnusedQueue;           //list of unused frame index
    mutable Mutex           mUnusedLock;            //lock to protect UnusedQueue
    Vector<FrameInfo *>     mFrameArray;            //array to keep all video frame info
    int                     mEnqueueCount;          //# buffers enqueue to native window
    int                     mMinUndequeued;         //min # undequeued buffers in native window
    unsigned long           mPreviewCount;          //# buffers sends to preview
#ifdef USE_USERPTR_MODE
    Vector<void *>          mCameraQueue;       //array to keep all intermediate v4l2 buffers
#endif
#ifdef USE_OMX_DECODEC
    sp<ALooper>             mDecoderLooper;
    sp<CameraDecode>        mDecode;
#endif /* end of USE_OMX_DECODEC */
    List<FramePair>         mReadyQueue;            //list of caputre done index
    List<VoFramePair>       mVoReadyQueue;          //list of caputre done index
    List<struct vdOut>      mWaitQueue;             //list of caputre done index
    Statistics              mPreviewStat;
    Statistics              mCbPreviewStat;
    mutable Mutex           mReadyLock;
    mutable Mutex           mWaitLock;
    bool                    mQueLenReached;
    Statistics mCaptureStat;

    bool                    mMipiDevice;
    HdmiRxInfo              mHdmiRxInfo;

#ifdef HAVE_SE_ENGINE
    unsigned long           mSeHandle;
    alloc_device_t          *mBufAlignRemove_pAllocDev;
#endif

    const struct hw_module_t *mGrallocModule;

    int                     mDumpFd;
    int                     mDumpCnt;

    int                     mSnapX;
    int                     mSnapY;
    int                     mSnapWidth;
    int                     mSnapHeight;
    int                     mSnapOutWidth;
    int                     mSnapOutHeight;

    double                  mExifGpsLatitude;
    double                  mExifGpsLongitude;
    double                  mExifGpsAltitude;
    char                    mExifGpsProcessingMethod[100];
    int                     mExifGpsTimeStampYear;
    int                     mExifGpsTimeStampMonth;
    int                     mExifGpsTimeStampDay;
    int                     mExifGpsTimeStampHour;
    int                     mExifGpsTimeStampMin;
    int                     mExifGpsTimeStampSec;

    sp<CameraVoWb>          mVoWb;
    sp<ALooper>             mVoWbLooper;
    bool                    isVoWbRunning;
};

}; // namespace android

#endif

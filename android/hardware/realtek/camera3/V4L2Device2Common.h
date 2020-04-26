#ifndef _CAMERA_HAL3_V4L2DEVICE2_COMMON_
#define _CAMERA_HAL3_V4L2DEVICE2_COMMON_

#include <libRTKAllocator/include/hardware/GrallocWrapper.h>
#include <libRTKAllocator/include/allocator/IonAllocator.h>
#include "V4L2Common.h"
#include "Base.h"
#include "V4L2Device2Callback.h"
#include "FormatConvertHelper.h"

#define FCC2ASCII(fmt)  ((char *)(&fmt))[0], ((char *)(&fmt))[1], ((char *)(&fmt))[2], ((char *)(&fmt))[3]

#define V4L2_SLOT_COUNT     8
#define STR_BUF_SIZE        64

using namespace android;

/* RequestQueue compare function */
static inline int compare_request(
        android::Request *const * r0,
        android::Request *const * r1)
{
    uint32_t f0 = (*r0)->frameNumber;
    uint32_t f1 = (*r1)->frameNumber;

    ALOGV("[%s] f0 %d f1 %d",__FUNCTION__,f0,f1);

    return (f0 > f1);
}

static inline int compare_request2(
        const android::Request *r0,
        const android::Request *r1)
{
    ALOGV("[CAMSORT][%s] f0 %d f1 %d",__FUNCTION__,r0->frameNumber,r1->frameNumber);
    return (r0->frameNumber > r1->frameNumber);
}

namespace androidcamera3 {

typedef struct _V4L2_PARAMS {
    int w;
    int h;
    int fps;
    int v4l2format;
} V4L2Params;

/**
 * This class defines the basic prototype of the functions that a
 * V4L2Device2 should support.
 */
class V4L2Device2Common {

public:
    V4L2Device2Common(
        int v4l2Id,
        V4L2Params *params,
        V4L2Device2Callback *callback,
        int maxBufferCount,
        GrallocWrapper *pGrallocWrapper,
        FormatConvertHelper *pConvertHelper,
        bool forceAllocator,
        bool usePhyAddr);

    virtual ~V4L2Device2Common();

    V4L2Params*         getParams() { return &mParams; }
    status_t            init();
    bool                isStreaming();

    // virtual functions
    virtual status_t    start() = 0;
    virtual void        flush(bool reportError) = 0;

    virtual void        dumpDeviceStatus() = 0;
    virtual int32_t     getOccupiedRequestCount() = 0;

    virtual bool        enqueueRequest(const Request& r) = 0;
    virtual bool        dequeueRequest(Request *r) = 0;

    // some commonly shared variables.
protected:
    int                 mV4L2Id;
    int                 mFd;
    struct vdIn         videoIn;
    V4L2Params          mParams;
    int32_t             mHalPixelFormat;
    V4L2Device2Callback *mCallback;
    FormatConvertHelper *mFormatConvertHelper;
    GrallocWrapper      *mGrallocWrapper;
    int                 mMaxBufferCount;
    uint64_t            mRequestCount;
    bool                mForceUseAllocatorBuf;
    bool                mUsePhyAddr;
    bool                mCheckHDCP;
    char                mDeviceStr[STR_BUF_SIZE];

    /**
     * Queue to record request sent from hal
     */
    Mutex               mRequestQueueLock;
    Vector<Request>     mRequestQueue;

    status_t            startStreaming();
    status_t            stopStreaming();

    void                closeDev();

    void                getBufferCbCr(
                            buffer_handle_t *buffer,
                            int w,
                            int h,
                            void **ppVPtr,
                            void **ppCb,
                            void **ppCr);

    void                getBufferPhyAddr(
                            buffer_handle_t *buffer,
                            uint32_t *phyAddr);
};

}; // namespace androidcamera3

#endif

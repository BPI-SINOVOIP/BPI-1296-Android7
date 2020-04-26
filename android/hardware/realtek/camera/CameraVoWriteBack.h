/*
 * Copyright (c) 2016 Realtek Semiconductor Corp.
 */
#ifndef _RTK_CAMERA_VO_WRITEBACK_H_
#define _RTK_CAMERA_VO_WRITEBACK_H_

#include <utils/Log.h>
#include <utils/KeyedVector.h>
#include <utils/Statistics.h>
#include <utils/Timers.h>
#include <binder/IMemory.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>

#include <hardware/camera.h>
#include <VOWriteBack.h>
#include "CameraOmxDecode.h"

#define FROM_VO 0x0
#define FROM_CAMERA 0x1
#define CHECK_COMEFROM(a)  ((a == FROM_VO) ? "VO" : "Camera")

#define ALIGN(x, y)     ((x + y - 1) & ~(y - 1))
#define ALIGN16(x)      ALIGN(x, 16)

namespace android {

struct ABuffer;
struct ALooper;
struct ACodec;

class MetaData;

struct CameraVoWb : public AHandler
{
public:
    CameraVoWb(ANativeWindow * nativeWindow, int width, int height, int size);
    int             GetCameraVoWbState() {return mState;};
    int             GetCameraVoWbReleaseFb() {Mutex::Autolock flock(mFinishLock); return fb_release;};
    unsigned int    GetpLockphyAddr(int idx , int fieldidx , int dFBCount, int dIsInterlaceFrm);
    int             GetCameraVoWbAvailBufCnt() {return mVoAvailBufCnt;};

    enum {
        /* Camerahardware --> CameraVoWriteBack */
        kWhatStart = 0x1,
        kWhatStop,
        kWhatAddWriteBackBuffers,
        kWhatLockWriteBackBuffers,
        kWhatClearWriteBackBuffers,
        kWhatPrepareBuffer,
        kWhatProcessCmd,
        kWhatGetOutputBuffer,               /* CamearHardware get buffer from VO readyQueue */
        //kWhatNoMoreData,                  /* CameraHardware want do stopPreview */
    };

protected:
    virtual                 ~CameraVoWb();
    virtual void            onMessageReceived(const sp<AMessage> &msg);

private:
    int32_t                 mType;
    int                     mState;
    sp<ALooper>             mLooper;
    sp<AMessage>            mStopMsg; 
    ANativeWindow           *mNativeWindow;

    int32_t                 mWidth;
    int32_t                 mHeight;
    int32_t                 mBufferSize;
    volatile int            mVoAvailBufCnt;
    int32_t                 fb_release;         // for deinterlace mode
    List<unsigned long>     mBufferQueue;       // Queue for all buffers that can be used by VOWB (store address)
    List<int>               mReadyQueue;        // Queue for buffer which is ready to be returned to cameraHAL (store index)
    VOWriteBack*            mVOWriteBackHandle;

    mutable Mutex           mReadyLock;
    mutable Mutex           mPrepareLock;
    mutable Mutex           mFinishLock;

    bool                    onStop();
    void                    PrintQueue(int whichQueue);
    int                     FindIdxFromAddr(unsigned long addr);
    unsigned long           GetWriteBackFrame();

    typedef struct {
        int32_t index;
        int32_t prev_index;
        int32_t pLockAddr;
        unsigned int yAddr;
        unsigned int uAddr;
    } PrevFrameInfo;
    PrevFrameInfo           prev;

    enum{
        PRINT_READY_QUEUE = 0,
        PRINT_FINISH_QUEUE,
    };
    
    enum{
        CAMERA_VO_STATE_UNINITIALIZED = 0x1,
        CAMERA_VO_STATE_READY,
        CAMERA_VO_STATE_RUNNING,
        CAMERA_VO_STATE_FIRST_RECEIVED,
        CAMERA_VO_STATE_STOPPED,
    };
};

static inline void ReplyVOResponse(const sp<AMessage> &msg, const sp<AMessage> &response)
{
    sp<AReplyToken> replyID;
    CHECK(msg->senderAwaitsResponse(&replyID));
    response->postReply(replyID);
}

} // namespace android
#endif /* end of not define _RTK_CAMERA_VO_WRITEBACK_H_ */

#ifndef RTK_SCREENRECORD_DEF_H
#define RTK_SCREENRECORD_DEF_H

#include <stdlib.h>
#include <utils/Vector.h>
#include <utils/List.h>
#include <utils/threads.h>

#ifdef __cplusplus

class SRBuffer_priv;
class ScreenRecordService_priv;

class SRBuffer{
public:
    enum State {
        INIT,
        DEQUEUED,   // In VO
        QUEUED,     // In ready queue waiting for acquired by application
        ACQUIRED,
        PROTECTED,
    };
    SRBuffer(int share_fd = -1, int size = 0, int idx = -1);
    virtual ~SRBuffer();
    unsigned long           GetPhyAddr();
    virtual void *          GetVirAddr();
    virtual int             GetShareFd();
    virtual void            Sync();
    virtual void            Flush();
    virtual void            Invalidate();
    virtual void            SetBufferBlack(bool is422);
    virtual unsigned long   GetSize();
    virtual int             GetState()      {return mState;};
    virtual int             GetIndex()      {return mIndex;};
    virtual bool            IsDequeued()    {return (GetState() == DEQUEUED ) ? true : false;};
    virtual bool            IsQueued()      {return (GetState() == QUEUED   ) ? true : false;};
    virtual bool            IsAcquired()    {return (GetState() == ACQUIRED ) ? true : false;};
    virtual bool            IsProtected()   {return (GetState() == PROTECTED) ? true : false;};
    virtual uint64_t        GetTimeStamp()  {return mTimestamp;};
    virtual int             GetHalFormat()  {return mHalFormat;};

private:
    friend class ScreenRecordService;
    void                    CreateBuffer(int share_fd, int size);
    enum State              mState;
    int                     mIndex;
    int                     mHalFormat;
    unsigned long long      mTimestamp;
    SRBuffer_priv*          mPriv;
};

class ScreenRecordService {
public:
    enum{
        SCREEN_RECORD_DEFAULT_USAGE     = 0x0U,         // Default settings: nv12, 420, 8 bit
        SCREEN_RECORD_FMT_NV21          = 0x1U << 0,    // Default: NV12
        SCREEN_RECORD_COLOR_422         = 0x1U << 1,    // Default: 420
        SCREEN_RECORD_BIT_DEPTH_10      = 0x1U << 2,    // Default: 8 bit
        SCREEN_RECORD_LSB_JUSTIFIED     = 0x1U << 3,    // Defulat: MSB justified
    };
    enum{ // Reference from AudioRPCBaseDS_data.h
       WRITEBACK_TYPE_BACKGROUND_V1 = 0,
       WRITEBACK_TYPE_BACKGROUND_V2 = 1,
       WRITEBACK_TYPE_VSYNC_MIXER1 = 2,
       WRITEBACK_TYPE_VSYNC_MIXER2 = 3,
       WRITEBACK_TYPE_VSYNC_V1 = 4,
    };

    ScreenRecordService(int hal_format, int width, int height, int type, uint32_t usage);
    ScreenRecordService(int hal_format, int width, int height, int type, uint32_t usage, bool captureOnly);
    virtual ~ScreenRecordService();

    virtual bool            ConfigSettings();
    virtual SRBuffer *      CreateBuffer(int share_fd = -1);
    virtual void            DeleteBuffer(SRBuffer * buffer);
    virtual int             GetBufferCount();
    virtual int             GetCaptureFps();
    virtual SRBuffer *      AcquiredBuffer(); // Acquire one frame, must be called right after FreeBuffer
    virtual int             FreeBuffer(SRBuffer * buffer);
    virtual int             FreeAllBuffer();

private:
    class WritebackThread : public android::Thread{
        ScreenRecordService* mService;
    public:
        WritebackThread(ScreenRecordService* service)
            : Thread(false), mService(service) { }
        virtual void onFirstRef(){
            run("ScreenRecordWritebackThread", android::PRIORITY_DISPLAY);
        }
        virtual bool threadLoop(){
            mService->writebackThread();
            // loop until we need to quit
            return true;
        }
    };

    int                                     writebackThread();
    int                                     GetDequeueBufferCnt();
    SRBuffer *                              GetWritebackFrameBuffer();
    void                                    SetWritebackFormat(int type, uint32_t usage);
    void                                    SetProtectBuffer();

    ScreenRecordService_priv *              mPriv;
    android::Vector<SRBuffer *>             mBufArray;
    android::List<SRBuffer *>               mReadyQueue;
    android::sp<WritebackThread>            mWritebackThread;
    mutable android::Mutex                  mReadyLock;
    bool                                    mWritebackStopped;
    bool                                    mCaptureOnly;
    bool                                    mIsProtect;
    int                                     mHalFormat;
    int                                     mWidth;
    int                                     mHeight;
    int                                     mWritebackType;
    uint32_t                                mUsage;
    int                                     mLastValidIndex;
    bool                                    mFwState;
    bool                                    mDelayWriteback;
};
#else
typedef struct SRBuffer SRBuffer;
typedef struct ScreenRecordService ScreenRecordService;
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* End of RTK_SCREENRECORD_DEF_H */

#include <YUV_STATE.h>
#include <VOWriteBack.h>

class ScreenRecordService_priv{
public:
    ScreenRecordService_priv(unsigned int bufCnt, int widht, int height, int type, uint32_t usage);
    virtual ~ScreenRecordService_priv();
    virtual bool            SyncBuffer(int idx, unsigned long phys);
    virtual bool            FreeBuffer(unsigned long phys);
    virtual unsigned int    GetWritebackBuffer();
    virtual bool            IsWritebackReady() {return (mWriteback == NULL) ? false : true;};
    virtual bool            CheckFwState() {return mFwState;};

private:
    VOWriteBack*    mWriteback;
    int             mWidth;
    int             mHeight;
    int             mType;
    int             mUsage;
    bool            mFwState; /* become false if fw get error */
};

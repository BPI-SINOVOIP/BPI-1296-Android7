#ifndef __RTK_HWC_VO_WINDOW_API_SIDEBAND_H_
#define __RTK_HWC_VO_WINDOW_API_SIDEBAND_H_
#include "VOWindowBase.h"

#include <gui/SidebandClient.h>
#include <gui/SidebandControl.h>
#include <gui/Surface.h>

#define RESOURCE_UPDATE_DELAY_NS 1000000000
class VOWindowSideband : public VOWindowBase {
public:
    VOWindowSideband();
    virtual ~VOWindowSideband();
    virtual int updateWindow(void);

    virtual int setSidebandStream(native_handle_t * handle);
    virtual void releaseSidebandStream(void);
    virtual void passToRender(void);
    virtual void stopToRender(void);
    virtual void dump(android::String8& buf, const char* prefix);
    virtual void clearState(void);
    virtual void DisplayInfoEvent(int cmd, int ext);
    virtual bool getHDCPInfo();
private:
    native_handle_t * mSideband;
    android::sp<android::SidebandClient>  mClient;
    android::sp<android::SidebandControl> mControl;

    pthread_mutex_t mLock;

    bool mHidden;
    bool mClear;
    int64_t mResourceTime;
    hwc_rect_t mActiveSourceCrop, mActiveDisplayFrame;
};
#endif /* End of __RTK_HWC_VO_WINDOW_API_SIDEBAND_H_ */

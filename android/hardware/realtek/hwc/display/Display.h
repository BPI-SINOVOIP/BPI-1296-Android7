#ifndef __RTK_HWC_DISPLAY_H_
#define __RTK_HWC_DISPLAY_H_
#include <hwc_utils.h>
#include <volayer/VOLayerBase.h>
#include <vsync/Vsync.h>
#include <arbiter/ResourceArbiter.h>
#include <power/Power.h>
#include "DisplayInfo.h"
#include <vowindow/VOWindowBase.h>
#include <property/HWCProperty.h>
#include <vowindow/VOMixerServer.h>
#include <pthread.h>
#include <hdcp/HDCPInfo.h>

class Display;
class VOWindowBase;

struct super_cb {
    Display* (*getPrimaryDisplay)  (void * data);
    Display* (*getExternalDisplay) (void * data);
    Display* (*getVirtualDisplay)  (void * data);
};

class Display : public DisplayInfo,
    virtual public Vsync::Client,
    virtual public Power::Client,
    virtual public HDCPInfo::Client,
    virtual public HWCProperty
{
public:
    Display(Vsync * vsync, unsigned int display_type = HWC_DISPLAY_PRIMARY);
    virtual ~Display();

    virtual void    setResourceArbiter(ResourceArbiter * service);
    virtual void    setPower(Power * service);
    virtual void    setVOMixer(VOMixerServer * service);

    virtual int     prepare         (hwc_display_contents_1_t * display_contents);
    virtual int     set             (hwc_display_contents_1_t * display_contents);
    virtual int     eventControl    (int event, int enable);
    virtual int     setPowerMode    (int mode);
    virtual int     blank           (int blank);
    virtual void    registerProcs   (hwc_procs_t const* procs);
    virtual int     getDisplayConfigs(uint32_t* configs, size_t* numConfigs);
    virtual int     getDisplayAttributes(uint32_t config, const uint32_t* attributes, int32_t* values);
    virtual void    dump            (android::String8& buf, const char* prefix);
    virtual int     getFirstHwOverlay (void) {return mFirstHwOverlay;};
    virtual void    setSuperCallback(struct super_cb * cb, void * data) {mSuperCb = cb; mSuperData = data;};
    virtual bool    checkSkipLayer(int total_layer, int layer);

    /* VsyncListener */
    virtual void    VsyncEvent(void);

private:
    void getVideoStandard(void);
    void windowTask(void); // in vsync event loop.

    Vsync *                 mVsync;
    const hwc_procs_t *     mProcs;
    bool                    mVsyncEnable;
    int                     mPowerMode;
    int                     mBlank;
    bool                    mOnlyVOLayer;
    VOWindowBase *          mDisplayWindow;
    int64_t                 mDisplayHideTimestampUs;
    int                     mVideoStandard;
    pthread_mutex_t         mLayerLock;

    android::Vector<VOLayerBase*> mLayerList;

    int                     mFirstHwOverlay;
    struct super_cb *       mSuperCb;
    void *                  mSuperData;
};

#endif /* End of __RTK_HWC_DISPLAY_H_ */

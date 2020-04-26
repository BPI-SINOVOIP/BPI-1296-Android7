#ifndef __RTK_HWC_LAYER_SIDEBAND_H_
#define __RTK_HWC_LAYER_SIDEBAND_H_
#include "VOLayerBase.h"
#include <vowindow/VOWindowSideband.h>
#include <property/HWCProperty.h>

class VOLayerSideband : public VOLayerBase,
    virtual public VOWindowSideband,
    virtual ResourceArbiter::Client,
    virtual HWCProperty
{
public:
    VOLayerSideband();
    virtual ~VOLayerSideband();
    virtual bool supportHwcLayer(hwc_layer_1_t * hwc_layer);
    virtual void prepare();
    virtual void set();
    virtual void dump(android::String8& buf, const char* prefix);

    virtual void setDisplayInfo(DisplayInfo * info) {VOWindowSideband::setDisplayInfo(info);};
    virtual void setResourceArbiter(ResourceArbiter * service) {
        ResourceArbiter::Client::setResourceArbiter(service);
    };

    virtual int ResourceEvent(int /*notify*/) {return 0;};

    virtual void VsyncEvent(void);
    virtual bool getHDCPInfo(void);
private:
    const native_handle_t * mSidebandStream;

};
#endif /* End of __RTK_HWC_LAYER_SIDEBAND_H_ */

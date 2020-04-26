#ifndef SIDEBAND_WINDOWAPI_H
#define SIDEBAND_WINDOWAPI_H

#include <utils/RefBase.h>

#include <ui/Region.h>

//#include <private/gui/LayerState.h>

namespace android {
class SidebandWindowAPI :public virtual RefBase{
public:
    enum flags_t {
        HIDDEN          = 0x1U <<   0,  //layer_state_t::eLayerHidden, /* 0x01    |     1:hide 0:shwo */
        OPAQUE          = 0x1U <<   1,  //layer_state_t::eLayerOpaque, /* 0x02 */
        REFRESH         = 0x1U <<   16,
        FRAME_READY     = 0x1U <<   17,
        FLUSH           = 0x1U <<   18,
    };
    SidebandWindowAPI() { }
    virtual ~SidebandWindowAPI() { }

    virtual status_t setCrop(const Rect& crop)                  = 0;
    virtual status_t setPosition(float x, float y)              = 0;
    virtual status_t setSize(uint32_t w, uint32_t h)            = 0;
    virtual status_t setLayer(int32_t z)                        = 0;
    virtual status_t setFlags(uint32_t flags, uint32_t mask)    = 0;
    virtual status_t setAlpha(float alpha)                      = 0;
    virtual void dump(String8& /*result*/, const char* /*prefix*/) const {}
};

}; // namespace android
#endif /* End of SIDEBAND_WINDOWAPI_H */

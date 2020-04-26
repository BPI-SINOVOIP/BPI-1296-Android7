#include "IRtkHDMICallback.h"

using namespace android;

class NativeRtkHDMICallback : public BnRtkHDMICallback {
public:
    NativeRtkHDMICallback();
    virtual void onHandleEvent(int32_t event);
};

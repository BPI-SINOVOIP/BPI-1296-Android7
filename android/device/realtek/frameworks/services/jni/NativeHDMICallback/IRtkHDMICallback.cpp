/**
 * Realtek HDMI callback native implementation
 */
#define LOG_TAG "RtkHDMICallback-native"

#include "IRtkHDMICallback.h"

#include <utils/Log.h>
#include <binder/Parcel.h>
#include <utils/String8.h>

#include <private/binder/Static.h>

namespace android {

// ----------------------------------------------------------------------

class BpRtkHDMICallback : public BpInterface<IRtkHDMICallback>
{
public:
    BpRtkHDMICallback(const sp<IBinder>& impl)
        : BpInterface<IRtkHDMICallback>(impl)
    {
        ALOGD("Create BpRtkHDMICallback");
    }

    virtual void onHandleEvent(int32_t event) {
        ALOGD("%s event:%d",__FUNCTION__,event);
        Parcel data, reply;
        data.writeInterfaceToken(IRtkHDMICallback::getInterfaceDescriptor());
        data.writeInt32(event);
        remote()->transact(ON_HANDLE_EVENT_TRANSACTION, data, &reply);
    }
};

IMPLEMENT_META_INTERFACE(RtkHDMICallback, "com.realtek.hardware.IRtkHDMICallback");

// ----------------------------------------------------------------------

status_t BnRtkHDMICallback::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{

    ALOGD("Call BnRtkHDMICallback onTransact");
    switch(code) {
        case ON_HANDLE_EVENT_TRANSACTION: {
            ALOGD("ON_HANDLE_EVENT_TRANSACTION called");
            CHECK_INTERFACE(IRtkHDMICallback, data, reply);
            int32_t event = data.readInt32();
            onHandleEvent(event);
            reply->writeNoException();
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}; // namespace android

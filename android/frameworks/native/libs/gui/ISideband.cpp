#define LOG_TAG "Sideband"

#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>
#include <binder/IMemory.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <gui/BitTube.h>
#include <gui/ISideband.h>
#include <gui/IGraphicBufferProducer.h>

#include <private/gui/LayerState.h>
#include <utils/Log.h>

// ---------------------------------------------------------------------------

namespace android {


class BpSideband : public BpInterface<ISideband>
{
public:
    BpSideband(const sp<IBinder>& impl)
        : BpInterface<ISideband>(impl)
    {
    }

    virtual ~BpSideband();

    virtual sp<ISidebandClient> createConnection()
    {
        uint32_t n;
        Parcel data, reply;
        data.writeInterfaceToken(ISideband::getInterfaceDescriptor());
        remote()->transact(BnSideband::CREATE_CONNECTION, data, &reply);
        return interface_cast<ISidebandClient>(reply.readStrongBinder());
    }

};

// Out-of-line virtual method definition to trigger vtable emission in this
// translation unit (see clang warning -Wweak-vtables)
BpSideband::~BpSideband() {}

IMPLEMENT_META_INTERFACE(Sideband, "android.ui.ISideband");

// ----------------------------------------------------------------------

status_t BnSideband::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
        case CREATE_CONNECTION: {
            CHECK_INTERFACE(ISideband, data, reply);
            sp<IBinder> b = IInterface::asBinder(createConnection());
            reply->writeStrongBinder(b);
            return NO_ERROR;
        }
        default: {
            return BBinder::onTransact(code, data, reply, flags);
        }
    }
}

// ----------------------------------------------------------------------------

};

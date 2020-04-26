// Realtek HDMICallback in native
#ifndef RTK_HDMI_CALLBACK_H
#define RTK_HDMI_CALLBACK_H

#include <binder/IInterface.h>

#define EVENT_HDMI_PLUGIN       (1)
#define EVENT_HDMI_PLUGOUT      (2)
#define EVENT_HDCP_CHANGED      (3)

namespace android {

// ----------------------------------------------------------------------

class IRtkHDMICallback : public IInterface
{
public:
    DECLARE_META_INTERFACE(RtkHDMICallback);

    virtual void onHandleEvent(int32_t event) = 0;

    enum {
        ON_HANDLE_EVENT_TRANSACTION = IBinder::FIRST_CALL_TRANSACTION
    };
};

// ----------------------------------------------------------------------

class BnRtkHDMICallback : public BnInterface<IRtkHDMICallback>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

// ----------------------------------------------------------------------

}; // namespace android

#endif // RTK_HDMI_CALLBACK_H


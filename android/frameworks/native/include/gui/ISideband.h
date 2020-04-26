#ifndef RTK_GUI_ISIDEBAND_H
#define RTK_GUI_ISIDEBAND_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/RefBase.h>
#include <utils/Errors.h>
#include <utils/Timers.h>
#include <utils/Vector.h>

#include <binder/IInterface.h>

#include <ui/PixelFormat.h>

#include <gui/ISidebandClient.h>

namespace android {

class ISideband: public IInterface {
    public:
        DECLARE_META_INTERFACE(Sideband);
        /* create connection with sideband, requires
         * TODO ACCESS_SURFACE_FLINGER "permission" ??
         */
        virtual sp<ISidebandClient> createConnection() = 0;
};


class BnSideband: public BnInterface<ISideband> {
    public:
        enum {
            // Note: BOOT_FINISHED must remain this value, it is called from
            // Java by ActivityManagerService.
            BOOT_FINISHED = IBinder::FIRST_CALL_TRANSACTION,
            CREATE_CONNECTION,
        };

        virtual status_t onTransact(uint32_t code, const Parcel& data,
                Parcel* reply, uint32_t flags = 0);
};


}; // namespace android

#endif /* End of RTK_GUI_ISIDEBAND_H */

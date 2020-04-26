#ifndef SIDEBAND_REFCLOCK_H
#define SIDEBAND_REFCLOCK_H

//#include <stdint.h>
//#include <sys/types.h>

//#include <utils/RefBase.h>
//#include <utils/String8.h>
//#include <utils/Timers.h>

#include "Sideband.h"

namespace android {

#define MAX_SIDEBAND_INSTANCE 4
#define GET_HWSYNC_THRESHOLD  3000000000

class SBRefClock : public RefBase {
public:
    SBRefClock();
    virtual ~SBRefClock();

    status_t requestHwSync(const sp<Client>& client, int32_t* resource);
    status_t getHwSyncByResource(const sp<Client>& client, int32_t resource, int32_t* audioHwSync);
    status_t destroyHwSyncByResource(const sp<Client>& client, int32_t resource);

private:
    status_t allocHwSyc(int32_t* resource);

    int64_t mRequestTime;
    int mRefClockfd;
    int32_t mResource;
    int mClientCount;
};

}; // namespace android
#endif /* End of SIDEBAND_REFCLOCK_H */

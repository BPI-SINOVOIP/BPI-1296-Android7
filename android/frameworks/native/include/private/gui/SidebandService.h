#ifndef REALTEK_PRIVATE_SIDEBAND_SERVICE_H
#define REALTEK_PRIVATE_SIDEBAND_SERVICE_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/Singleton.h>
#include <utils/StrongPointer.h>


namespace android {

// ---------------------------------------------------------------------------

class IMemoryHeap;
class ISideband;

// ---------------------------------------------------------------------------

// This holds our connection to the sidebacd service (i.e. Sideband).
// If the remote side goes away, we will re-establish the connection.
// Users of this class should not retain the value from
// getComposerService() for an extended period.
//
// (It's not clear that using Singleton is useful here anymore.)
class SidebandService : public Singleton<SidebandService>
{
    sp<ISideband> mSidebandService;
    sp<IBinder::DeathRecipient> mDeathObserver;
    Mutex mLock;

    SidebandService();
    void connectLocked();
    void sidebandServiceDied();
    friend class Singleton<SidebandService>;
public:

    // Get a connection to the Composer Service.  This will block until
    // a connection is established.
    static sp<ISideband> getSidebandService();
};

// ---------------------------------------------------------------------------
}; // namespace android

#endif // REALTEK_PRIVATE_SIDEBAND_SERVICE_H

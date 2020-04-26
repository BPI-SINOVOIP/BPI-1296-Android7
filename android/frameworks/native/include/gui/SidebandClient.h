#ifndef REALTEK_GUI_SIDEBAND_CLIENT_H
#define REALTEK_GUI_SIDEBAND_CLIENT_H

#include <stdint.h>
#include <sys/types.h>

#include <binder/IBinder.h>
#include <binder/IMemory.h>

#include <utils/RefBase.h>
#include <utils/Singleton.h>
#include <utils/SortedVector.h>
#include <utils/threads.h>

#include <ui/FrameStats.h>
#include <ui/PixelFormat.h>

#include <gui/SidebandControl.h>
#include <cutils/native_handle.h>

namespace android {

// ---------------------------------------------------------------------------

class Composer;
class ISidebandClient;
class IGraphicBufferProducer;
class Region;

// ---------------------------------------------------------------------------

class SidebandClient : public RefBase
{
    // TODO friend class Composer;
public:
    enum {
        NORMAL_SIDEBAND = 0x534231,
        HACK_SIDEBAND = 0x534232
    };
                SidebandClient();
    virtual     ~SidebandClient();

    // Always make sure we could initialize
    status_t    initCheck() const;

    // Return the connection of this client
    sp<IBinder> connection() const;

    // Forcibly remove connection before all references have gone away.
    void        dispose();

    // TODO what about function name? callback when the composer is dies
    status_t linkToSidebandDeath(const sp<IBinder::DeathRecipient>& recipient,
            void* cookie = NULL, uint32_t flags = 0);

    // ------------------------------------------------------------------------
    // sideband creation / destruction

    //! Create a sideband
    sp<SidebandControl> createSurface(
            const String8& name,// name of the sideband
            uint32_t w,         // width in pixel
            uint32_t h,         // height in pixel
            PixelFormat format, // pixel-format desired
            uint32_t flags = 0  // usage flags
    );

    status_t    destroySurface(const sp<IBinder>& id);

    status_t    createSidebandStream(const sp<IBinder>& id, native_handle_t ** handle, bool hack=false);
    status_t    deleteSidebandStream(const sp<IBinder>& id, native_handle_t * handle);
    static status_t checkSidebandStream(const native_handle_t *handle, int id);
    sp<SidebandControl>    binderSurface(native_handle_t * handle);

    //sp<SidebandControl>    binderSurface(uint64_t key /*const sp <IBinder>& id*/);


    status_t    hide(const sp<IBinder>& id);
    status_t    show(const sp<IBinder>& id);
    status_t    setFlags(const sp<IBinder>& id, uint32_t flags, uint32_t mask);
    status_t    setLayer(const sp<IBinder>& id, int32_t layer);
    status_t    requestHwSync(int32_t* resource);
    status_t    getHwSyncByResource(int32_t resource, int32_t* audioHwSync);
    status_t    destroyHwSyncByResource(int32_t resource);
    status_t    setAudioHwSync(const sp<IBinder>& id, int32_t audioHwSync);
    status_t    setAlpha(const sp<IBinder>& id, float alpha=1.0f);
    status_t    setPosition(const sp<IBinder>& id, float x, float y);
    status_t    setSize(const sp<IBinder>& id, uint32_t w, uint32_t h);
    status_t    setCrop(const sp<IBinder>& id, const Rect& crop);

    status_t    setResource(const sp<IBinder>& id, uint64_t resource);
    status_t    getKey(const sp<IBinder>& id, native_handle_t * handle, uint64_t * key);
    status_t    getHDCPInfo(const sp<IBinder>& id, bool *info);
private:
    virtual void onFirstRef();
    mutable     Mutex                       mLock;
    status_t                                mStatus;
    sp<ISidebandClient>                     mClient;
    // TODO Composer&                       mComposer;
};

// ---------------------------------------------------------------------------
}; // namespace android

#endif // REALTEK_GUI_SIDEBAND_CLIENT_H

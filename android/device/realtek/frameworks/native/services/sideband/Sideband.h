#ifndef SIDEBAND_SERVICE_H
#define SIDEBAND_SERVICE_H
/* include */
#include <stdint.h>
#include <sys/types.h>

#include <cutils/compiler.h>

#include <utils/Atomic.h>
#include <utils/Errors.h>
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/SortedVector.h>
#include <utils/threads.h>

#include <binder/IMemory.h>

#include <ui/PixelFormat.h>
#include <ui/mat4.h>

#include <gui/ISideband.h>
#include <gui/ISidebandClient.h>


#include <private/gui/LayerState.h>

#include "SBLayer.h"
#include "SBRefClock.h"

namespace android {

class Client;
class SBLayer;
class SBRefClock;

class Sideband :    public BnSideband,
                    private IBinder::DeathRecipient
{
public:

    static char const* getServiceName() ANDROID_API { return "Sideband"; }

    Sideband() ANDROID_API;

    // must be called before clients can connect
    void init() ANDROID_API;


private:

    friend class Client;
    friend class SidebandLayer;
    virtual ~Sideband();

    /* ------------------------------------------------------------------------
     * IBinder interface
     */
    virtual status_t onTransact(uint32_t code, const Parcel& data,
        Parcel* reply, uint32_t flags);

    virtual status_t dump(int fd, const Vector<String16>& args);

    /* ------------------------------------------------------------------------
     * ISideband interface
     */
    virtual sp<ISidebandClient> createConnection();

    /* ------------------------------------------------------------------------
     * DeathRecipient interface
     */
    virtual void binderDied(const wp<IBinder>& who);

    /* ------------------------------------------------------------------------
     * RefBase interface
     */
    virtual void onFirstRef();


    /* ------------------------------------------------------------------------
     * Layer management
     */
    status_t createLayer(const String8& name, const sp<Client>& client,
            uint32_t w, uint32_t h, PixelFormat format, uint32_t flags,
            sp<IBinder>* handle, sp<IGraphicBufferProducer>* gbp);

    status_t getBinderKey(const sp<Client>& client, const sp<IBinder>& handle, uint64_t * key);

    status_t binderLayer(const sp<Client>& client, uint64_t key, sp<IBinder>* handle/*const sp<IBinder>& handle*/, sp<IGraphicBufferProducer>* gbp);
    // called when all clients have released all their references to
    // this layer meaning it is entirely safe to destroy all
    // resources associated to this layer.
    status_t onLayerDestroyed(const wp<SidebandLayer>& layer);

    status_t removeLayer(const sp<SidebandLayer>& layer);

    status_t requestHwSync(const sp<Client>& client, int32_t* resource);

    status_t getHwSyncByResource(const sp<Client>& client, int32_t resource, int32_t* audioHwSync);

    status_t destroyHwSyncByResource(const sp<Client>& client, int32_t resource);

    class LayerVector : public SortedVector< sp<SidebandLayer> > {
        public:
            LayerVector();
            LayerVector(const LayerVector& rhs);
    };

    struct State {
        LayerVector layersSortedByZ;
    };

    mutable Mutex mStateLock;
    State mCurrentState;

    sp<SBRefClock> mSBRefClock;
};


}; // namespace android

#endif /* End of SIDEBAND_SERVICE_H */

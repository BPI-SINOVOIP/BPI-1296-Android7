#ifndef RTK_SB_CLIENT_H
#define RTK_SB_CLIENT_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <utils/KeyedVector.h>
#include <utils/Mutex.h>

#include <gui/ISidebandClient.h>

namespace android {


class SidebandLayer;
class Sideband;


class Client : public BnSidebandClient
{
public:
        Client(const sp<Sideband>& sideband);
        ~Client();

    status_t initCheck() const;

    void attachLayer(const sp<IBinder>& handle, const sp<SidebandLayer>& layer);
    void detachLayer(const SidebandLayer* layer);

    sp<SidebandLayer> getLayerUser(const sp<IBinder>& handle) const;

private:
    virtual status_t createSurface(
            const String8& name,
            uint32_t w, uint32_t h,PixelFormat format, uint32_t flags,
            sp<IBinder>* handle,
            sp<IGraphicBufferProducer>* gbp);

    virtual status_t destroySurface(const sp<IBinder>& handle);

    virtual status_t getBinderKey(const sp<IBinder>& handle, uint64_t * key);
    virtual status_t binderSurface(uint64_t key, sp<IBinder>* handle/*const sp<IBinder>& handle*/, sp<IGraphicBufferProducer>* gbp);

    virtual status_t setCrop(const sp<IBinder>& handle, const Rect& crop);
    virtual status_t setPosition(const sp<IBinder>& handle, float x, float y);
    virtual status_t setSize(const sp<IBinder>& handle, uint32_t w, uint32_t h);
    virtual status_t setLayer(const sp<IBinder>& handle, int32_t z);
    virtual status_t setFlags(const sp<IBinder>& id, uint32_t flags, uint32_t mask);
    virtual status_t setAlpha(const sp<IBinder>& id, float alpha);
    virtual status_t requestHwSync(/*const sp<IBinder>& handle,*/ int32_t* resource);
    virtual status_t getHwSyncByResource(/*const sp<IBinder>& handle,*/ int32_t resource, int32_t* audioHwSync);
    virtual status_t destroyHwSyncByResource(/*const sp<IBinder>& handle,*/ int32_t resource);

    virtual status_t setAudioHwSync(const sp<IBinder>& handle, int32_t audioHwSync);
    virtual status_t setResource(const sp<IBinder>& handle, uint64_t resource);
    virtual status_t getHDCPInfo(const sp<IBinder>& handle, bool *info);
    virtual status_t onTransact(
        uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags);

    // constant
    sp<Sideband> mSideband;
    // thread-safe
    mutable Mutex mLock;
    DefaultKeyedVector< wp<IBinder>, wp<SidebandLayer> > mLayers;
};

}; // namespace android

#endif /* End of RTK_SB_CLIENT_H */

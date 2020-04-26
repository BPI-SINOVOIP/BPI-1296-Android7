#ifndef RTK_GUI_ISIDEBAND_CLIENT_H
#define RTK_GUI_ISIDEBAND_CLIENT_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <utils/RefBase.h>

#include <binder/IInterface.h>
#include <ui/Rect.h>
#include <ui/PixelFormat.h>

namespace android {

class IGraphicBufferProducer;

class ISidebandClient : public IInterface
{
    public:
        DECLARE_META_INTERFACE(SidebandClient);
        virtual status_t createSurface(
                const String8& name, uint32_t w, uint32_t h,
                PixelFormat format, uint32_t flags,
                sp<IBinder>* handle,
                sp<IGraphicBufferProducer>* gbp) = 0;

        virtual status_t destroySurface(const sp<IBinder>& handle) = 0;
        virtual status_t getBinderKey(const sp<IBinder>& handle, uint64_t * key) = 0;
        virtual status_t binderSurface(uint64_t key, sp<IBinder>* handle/*const sp<IBinder>& handle*/, sp<IGraphicBufferProducer>* gbp) = 0;

        virtual status_t    setFlags(const sp<IBinder>& handle, uint32_t flags, uint32_t mask) = 0;
        virtual status_t    setLayer(const sp<IBinder>& handle, int32_t layer) = 0;
        virtual status_t    requestHwSync(int32_t* resource) = 0;
        virtual status_t    getHwSyncByResource(int32_t resource, int32_t* audioHwSync) = 0;
        virtual status_t    destroyHwSyncByResource(int32_t resource) = 0;
        virtual status_t    setAudioHwSync(const sp<IBinder>& handle, int32_t audioHwSync) = 0;
        virtual status_t    setAlpha(const sp<IBinder>& handle, float alpha=1.0f) = 0;
        virtual status_t    setPosition(const sp<IBinder>& handle, float x, float y) = 0;
        virtual status_t    setSize(const sp<IBinder>& handle, uint32_t w, uint32_t h) = 0;
        virtual status_t    setCrop(const sp<IBinder>& handle, const Rect& crop) = 0;
        virtual status_t    setResource(const sp<IBinder>& handle, uint64_t resource) = 0;
        virtual status_t    getHDCPInfo(const sp<IBinder>& handle, bool *info) = 0;
};


class BnSidebandClient: public BnInterface<ISidebandClient> {
    public:
        virtual status_t onTransact(uint32_t code, const Parcel& data,
                Parcel* reply, uint32_t flags = 0);
};


}; // namespace android

#endif /* End of RTK_GUI_ISIDEBAND_CLIENT_H */

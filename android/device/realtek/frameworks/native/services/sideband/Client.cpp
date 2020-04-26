#include <stdint.h>
#include <sys/types.h>

#include <binder/PermissionCache.h>
#include <binder/IPCThreadState.h>

#include <private/android_filesystem_config.h>

#include "Client.h"
#include "SBLayer.h"
#include "Sideband.h"

namespace android {

Client::Client(const sp<Sideband>& sideband)
    : mSideband(sideband)
{
}

Client::~Client() {
    Mutex::Autolock _l(mLock);
    const size_t count = mLayers.size();
    ALOGV("[%s][+] count:%d", __FUNCTION__, (int)count);
    for (size_t i=0 ; i<count ; i++) {
        sp<SidebandLayer> layer(mLayers.valueAt(i).promote());
        if (layer != NULL)
            layer->ClientRemove(this);
    }
    ALOGV("[%s][-]", __FUNCTION__);
}

status_t Client::initCheck() const {
    return NO_ERROR;
}

void Client::attachLayer(const sp<IBinder>& handle, const sp<SidebandLayer>& layer)
{
    Mutex::Autolock _l(mLock);
    mLayers.add(handle, layer);
}

void Client::detachLayer(const SidebandLayer* layer) {
    Mutex::Autolock _l(mLock);
    const size_t count = mLayers.size();
    for (size_t i=0 ; i<count ; i++) {
        if (mLayers.valueAt(i) == layer) {
            mLayers.removeItemsAt(i, 1);
            break;
        }
    }

}

sp<SidebandLayer> Client::getLayerUser(const sp<IBinder>& handle) const {
    Mutex::Autolock _l(mLock);
    sp<SidebandLayer> lbc;
    wp<SidebandLayer> layer(mLayers.valueFor(handle));
    if (layer != NULL) {
        lbc = layer.promote();
        ALOGE_IF(lbc==0, "getSidebandLayerUser(name=%p) is dead", handle.get());
    }
    return lbc;
}


status_t Client::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
     return BnSidebandClient::onTransact(code, data, reply, flags);
}


status_t Client::createSurface(
        const String8& name,
        uint32_t w, uint32_t h, PixelFormat format, uint32_t flags,
        sp<IBinder>* handle,
        sp<IGraphicBufferProducer>* gbp)
{
    return mSideband->createLayer(name, this, w, h, format, flags,
            handle, gbp);
}

status_t Client::destroySurface(const sp<IBinder>& handle) {
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    detachLayer(layer.get());
    return layer->ClientRemove(this);
}

status_t Client::getBinderKey(const sp<IBinder>& handle, uint64_t * key) {
    ALOGV("[%s]",__FUNCTION__);
    return mSideband->getBinderKey(this, handle, key);
}

status_t Client::binderSurface(uint64_t key, sp<IBinder>* handle, sp<IGraphicBufferProducer>* gbp) {
    return mSideband->binderLayer(this, key, handle, gbp);
}

status_t Client::setCrop(const sp<IBinder>& handle, const Rect& crop) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    return layer->setCrop(crop);
}

status_t Client::setPosition(const sp<IBinder>& handle, float x, float y) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    return layer->setPosition(x, y);
}

status_t Client::setSize(const sp<IBinder>& handle, uint32_t w, uint32_t h) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    return layer->setSize(w, h);
}

status_t Client::requestHwSync(/*const sp<IBinder>& handle,*/ int32_t* resource) {
    ALOGV("[%s]",__FUNCTION__);
    return mSideband->requestHwSync(this, resource);
}

status_t Client::getHwSyncByResource(/*const sp<IBinder>& handle,*/ int32_t resource, int32_t* audioHwSync) {
    ALOGV("[%s]",__FUNCTION__);
    return mSideband->getHwSyncByResource(this, resource, audioHwSync);
}

status_t Client::destroyHwSyncByResource(/*const sp<IBinder>& handle,*/ int32_t resource) {
    ALOGV("[%s]",__FUNCTION__);
    return mSideband->destroyHwSyncByResource(this, resource);
}

status_t Client::setAudioHwSync(const sp<IBinder>& handle, int32_t audioHwSync) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    return layer->setAudioHwSync(audioHwSync);
}

status_t Client::setLayer(const sp<IBinder>& handle, int32_t z) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    return layer->setLayer(z);
}

status_t Client::setFlags(const sp<IBinder>& handle, uint32_t flags, uint32_t mask) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    return layer->setFlags(flags, mask);
}

status_t Client::setAlpha(const sp<IBinder>& handle, float alpha) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    return layer->setAlpha(alpha);
}

status_t Client::setResource(const sp<IBinder>& handle, uint64_t resource) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL)
        return NAME_NOT_FOUND;
    return layer->setResource(resource);
}

status_t Client::getHDCPInfo(const sp<IBinder>& handle, bool *info) {
    ALOGV("[%s]",__FUNCTION__);
    sp<SidebandLayer> layer = getLayerUser(handle);
    if (layer == NULL){
        *info = false;
        return NAME_NOT_FOUND;
    }
    return layer->getHDCPInfo(info);
}

}; // namespace android

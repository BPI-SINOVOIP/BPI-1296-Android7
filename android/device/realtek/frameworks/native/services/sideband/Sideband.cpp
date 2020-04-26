
#define ATRACE_TAG ATRACE_TAG_GRAPHICS
#include <stdint.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>
#include <dlfcn.h>
#include <inttypes.h>
#include <stdatomic.h>

#include <EGL/egl.h>

#include <cutils/log.h>
#include <cutils/properties.h>

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/MemoryHeapBase.h>
#include <binder/PermissionCache.h>

#include <gui/BitTube.h>
#include <gui/BufferQueue.h>
#include <gui/GuiConfig.h>
#include <gui/IDisplayEventConnection.h>
#include <gui/Surface.h>
#include <gui/GraphicBufferAlloc.h>

#include <ui/GraphicBufferAllocator.h>
#include <ui/PixelFormat.h>
#include <ui/UiConfig.h>

#include <utils/misc.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <utils/StopWatch.h>
#include <utils/Trace.h>

#include <private/android_filesystem_config.h>
#include <private/gui/SyncFeatures.h>

#include <cutils/compiler.h>

#include "Sideband.h"
#include "SBLayer.h"
#include "SBConsumer.h"
namespace android {

#if 0
const String16 sHardwareTest("android.permission.HARDWARE_TEST");
const String16 sAccessSurfaceFlinger("android.permission.ACCESS_SURFACE_FLINGER");
const String16 sReadFramebuffer("android.permission.READ_FRAME_BUFFER");
#endif
const String16 sDumpPermission("android.permission.DUMP");

Sideband::Sideband()
    :mSBRefClock(NULL)
{
}

Sideband::~Sideband() {
    ALOGI("[%s %d]\n", __FUNCTION__, __LINE__);
    if (mSBRefClock != NULL) {
       mSBRefClock.clear();
       mSBRefClock = NULL;
    }
}

void Sideband::init() {
}

void Sideband::onFirstRef() {
}

void Sideband::binderDied(const wp<IBinder>& /* who */) {
    ALOGI("[%s] Nothing TODO", __FUNCTION__);
}

sp<ISidebandClient> Sideband::createConnection() {
    sp<ISidebandClient> bclient;
    sp<Client> client(new Client(this));
    status_t err = client->initCheck();
    if (err == NO_ERROR) {
        bclient = client;
    }
    return bclient;
}

status_t Sideband::createLayer(const String8& name, const sp<Client>& client,
            uint32_t /* w */, uint32_t /* h */, PixelFormat /* format */, uint32_t flags,
            sp<IBinder>* handle, sp<IGraphicBufferProducer>* gbp) {
    Mutex::Autolock _l(mStateLock);
    const size_t count = mCurrentState.layersSortedByZ.size();
    if (count >= 1)
        return status_t(-1);

    sp <SidebandLayer> layer = new SidebandLayer(this, client, name, flags);
    layer->setKey((uint64_t) layer.get() ^ (uint64_t) this);
    *handle = layer->getHandle(client, SidebandLayer::CreateType);
    *gbp = layer->getProducer();
    client->attachLayer(*handle, layer);
    mCurrentState.layersSortedByZ.add(layer);
    return NO_ERROR;
}

status_t Sideband::getBinderKey(const sp<Client>& client, const sp<IBinder>& handle, uint64_t * key) {
    Mutex::Autolock _l(mStateLock);
    sp <SidebandLayer> l(client->getLayerUser(handle));
    sp <SidebandLayer> lbc;
    const size_t count = mCurrentState.layersSortedByZ.size();
    for (size_t i=0 ; i<count ; i++) {
        lbc = mCurrentState.layersSortedByZ.itemAt(i);
        if (lbc == l) {
            *key = lbc->getKey();
            ALOGI("[%s] key:0x%llx", __FUNCTION__, (long long)*key);
            return NO_ERROR;
        }
    }
    ALOGE("[%s] Key is not found!", __FUNCTION__);
    return status_t(-1);
}

status_t Sideband::binderLayer(const sp<Client>& client, uint64_t key, sp<IBinder>* handle, sp<IGraphicBufferProducer>* gbp) {
    Mutex::Autolock _l(mStateLock);
    status_t err = NO_ERROR;
    sp <SidebandLayer> lbc;
    const size_t count = mCurrentState.layersSortedByZ.size();
    ALOGV("[%s] count:%d", __FUNCTION__, (int)count);
    for (size_t i=0 ; i<count ; i++) {
        lbc = mCurrentState.layersSortedByZ.itemAt(i);
        if (lbc != NULL && lbc->getKey() == key) {
            *handle = lbc->getHandle(client, SidebandLayer::BinderType);
            *gbp = lbc->getProducer();
            client->attachLayer(*handle, lbc);
            ALOGI("[%s] return OK", __FUNCTION__);
            return NO_ERROR;
        }
    }
    ALOGE("[%s] Layer is not found! (key:0x%llx)", __FUNCTION__, (long long)key);
    for (size_t i=0 ; i<count ; i++) {
        lbc = mCurrentState.layersSortedByZ.itemAt(i);
        if(lbc.get())
            ALOGW("[%s] Slot[%d] key:0x%llx", __FUNCTION__, (int)i, (long long)lbc->getKey());
    }
    return status_t(-1);
}

status_t Sideband::onLayerDestroyed(const wp<SidebandLayer>& layer) {
    status_t err = NO_ERROR;
    sp<SidebandLayer> l(layer.promote());
    if (l != NULL) {
        err = removeLayer(l);
        ALOGE_IF(err<0 && err != NAME_NOT_FOUND,
                "error removing layer=%p (%s)", l.get(), strerror(-err));
    }
    return err;
}

status_t Sideband::removeLayer(const sp<SidebandLayer>& layer) {
    ssize_t index = -1;
    if (layer != NULL)
        layer->onRemoved();

    Mutex::Autolock _l(mStateLock);
    index = mCurrentState.layersSortedByZ.remove(layer);
    return status_t(index);
}

status_t Sideband::requestHwSync(const sp<Client>& client, int32_t* resource) {
    ALOGI("[%s %d] client %p\n", __FUNCTION__, __LINE__, client.get());
    status_t err = NO_ERROR;
    if (mSBRefClock == NULL) {
        mSBRefClock = new SBRefClock();
    }
    err = mSBRefClock->requestHwSync(client, resource);
    return err;
}

status_t Sideband::getHwSyncByResource(const sp<Client>& client, int32_t resource, int32_t* audioHwSync) {
    ALOGI("[%s %d] client %p resource %d\n", __FUNCTION__, __LINE__, client.get(), resource);
    status_t err = NO_ERROR;
    if (mSBRefClock == NULL) {
        return status_t(-1);
    }
    err = mSBRefClock->getHwSyncByResource(client, resource, audioHwSync);
    return err;
}

status_t Sideband::destroyHwSyncByResource(const sp<Client>& client, int32_t resource) {
    ALOGI("[%s %d] client %p resource %d\n", __FUNCTION__, __LINE__, client.get(), resource);
    status_t err = NO_ERROR;
    if (mSBRefClock == NULL) {
        return status_t(-1);
    }
    err = mSBRefClock->destroyHwSyncByResource(client, resource);
    return err;
}

status_t Sideband::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
        case CREATE_CONNECTION:
        {
#if 0
            //TODO::
            // codes that require permission check
            IPCThreadState* ipc = IPCThreadState::self();
            const int pid = ipc->getCallingPid();
            const int uid = ipc->getCallingUid();
            if ((uid != AID_GRAPHICS) &&
                    !PermissionCache::checkPermission(sAccessSurfaceFlinger, pid, uid)) {
                ALOGE("Permission Denial: "
                        "can't access SurfaceFlinger pid=%d, uid=%d", pid, uid);
                return PERMISSION_DENIED;
            }
#endif
            break;
        }
    }

    status_t err = BnSideband::onTransact(code, data, reply, flags);
    if (err == UNKNOWN_TRANSACTION || err == PERMISSION_DENIED) {
        ALOGE("[%s] BnSurfaceComposer::onTransact return %d", __FUNCTION__, err);
    }
    return err;
}

status_t Sideband::dump(int fd, const Vector<String16>& /*args*/)
{
    String8 result;
    const IPCThreadState* ipc = IPCThreadState::self();
    const int pid = ipc->getCallingPid();
    const int uid = ipc->getCallingUid();
    if ((uid != AID_SHELL)
            && !PermissionCache::checkPermission(sDumpPermission, pid, uid)) {
        result.appendFormat("Permission Denial: "
                "can't dump Sideband from pid=%d, uid=%d\n", pid, uid);
    } else {
        Mutex::Autolock _l(mStateLock);
        const size_t count = mCurrentState.layersSortedByZ.size();
        sp <SidebandLayer> lbc;
        result.appendFormat("Sideband: %p count:%d\n", this, (int)count);
        for (size_t i=0 ; i<count ; i++) {
            lbc = mCurrentState.layersSortedByZ.itemAt(i);
            if (lbc.get())
                lbc->dump(result, " |  ");
        }
        if (count)
            result.append(" +------\n");
    }
    write(fd, result.string(), result.size());
    return NO_ERROR;
}

// ---------------------------------------------------------------------------

Sideband::LayerVector::LayerVector() {
}

Sideband::LayerVector::LayerVector(const LayerVector& rhs)
    : SortedVector<sp<SidebandLayer> >(rhs) {
}

// ---------------------------------------------------------------------------

}; // namespace android

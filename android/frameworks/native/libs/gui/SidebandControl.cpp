#define LOG_TAG "SidebandControl"

#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <android/native_window.h>

#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/threads.h>

#include <binder/IPCThreadState.h>

#include <ui/DisplayInfo.h>
#include <ui/GraphicBuffer.h>
#include <ui/Rect.h>

#include <gui/ISideband.h>
#include <gui/Surface.h>
#include <gui/SidebandClient.h>
#include <gui/SidebandControl.h>

namespace android {

// ============================================================================
//  SidebandControl
// ============================================================================

SidebandControl::SidebandControl(
        const sp<SidebandClient>& client,
        const sp<IBinder>& handle,
        const sp<IGraphicBufferProducer>& gbp)
    : mClient(client), mHandle(handle), mGraphicBufferProducer(gbp), mSidebandStream(NULL), mHackSideband(false)
{
}

SidebandControl::~SidebandControl()
{
    destroy();
}

void SidebandControl::destroy()
{
    if (mSidebandStream != NULL) {
        mClient->deleteSidebandStream(mHandle, mSidebandStream);
        mSidebandStream = NULL;
    }
    if (isValid()) {
        mClient->destroySurface(mHandle);
    }
    // clear all references and trigger an IPC now, to make sure things
    // happen without delay, since these resources are quite heavy.
    mClient.clear();
    mHandle.clear();
    mGraphicBufferProducer.clear();
    IPCThreadState::self()->flushCommands();
}

#if 0
void SurfaceControl::clear()
{
    // here, the window manager tells us explicitly that we should destroy
    // the surface's resource. Soon after this call, it will also release
    // its last reference (which will call the dtor); however, it is possible
    // that a client living in the same process still holds references which
    // would delay the call to the dtor -- that is why we need this explicit
    // "clear()" call.
    destroy();
}
#endif

bool SidebandControl::isSameSurface(
        const sp<SidebandControl>& lhs, const sp<SidebandControl>& rhs)
{
    if (lhs == 0 || rhs == 0)
        return false;
    return lhs->mHandle == rhs->mHandle;
}

status_t SidebandControl::requestHwSync(int32_t* resource) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->requestHwSync(resource);
}

status_t SidebandControl::getHwSyncByResource(int32_t resource, int32_t* audioHwSync) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->getHwSyncByResource(resource, audioHwSync);
}

status_t SidebandControl::destroyHwSyncByResource(int32_t resource) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->destroyHwSyncByResource(resource);
}

status_t SidebandControl::setAudioHwSync(int32_t audioHwSync) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->setAudioHwSync(mHandle, audioHwSync);
}

status_t SidebandControl::setLayer(int32_t layer) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->setLayer(mHandle, layer);
}
status_t SidebandControl::setPosition(float x, float y) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->setPosition(mHandle, x, y);
}
status_t SidebandControl::setSize(uint32_t w, uint32_t h) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->setSize(mHandle, w, h);
}
status_t SidebandControl::hide() {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->hide(mHandle);
}
status_t SidebandControl::show() {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->show(mHandle);
}

status_t SidebandControl::refresh() {
    status_t err = validate();
    if (err < 0) return err;
    return setFlags(REFRESH,REFRESH);
}

status_t SidebandControl::setFlags(uint32_t flags, uint32_t mask) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->setFlags(mHandle, flags, mask);
}

status_t SidebandControl::setAlpha(float alpha) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->setAlpha(mHandle, alpha);
}

status_t SidebandControl::setCrop(const Rect& crop) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->setCrop(mHandle, crop);
}
status_t SidebandControl::setResource(uint64_t resource) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->setResource(mHandle, resource);
}

status_t SidebandControl::getHDCPInfo(bool *info) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->getHDCPInfo(mHandle, info);
}

void SidebandControl::setSBHint(bool hack)
{
   mHackSideband = hack;
}

status_t SidebandControl::getSidebandStream(native_handle_t ** handle) {
    status_t err = validate();
    if (err < 0) return err;
    if (mSidebandStream == NULL) {
        err = createSidebandStream(&mSidebandStream);
        if (err != NO_ERROR)
            return err;
    }

    if (mSidebandStream == NULL)
        return status_t(-1);

    *handle = mSidebandStream;
    return NO_ERROR;
}


status_t SidebandControl::getKey(uint64_t * key) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->getKey(mHandle, mSidebandStream, key);
}

status_t SidebandControl::createSidebandStream(native_handle_t ** handle) {
    status_t err = validate();
    if (err < 0) return err;
    return mClient->createSidebandStream(mHandle, handle, mHackSideband);
}

status_t SidebandControl::setSidebandStream(native_handle_t * handle) {
    status_t err = validate();
    if (err < 0) return err;
    if (mSidebandStream != NULL) {
        ALOGW("[%s] change mSidebandStream %p => %p", __FUNCTION__, mSidebandStream, handle);
        mClient->deleteSidebandStream(mHandle, mSidebandStream);
    }
    mSidebandStream = handle;
    return NO_ERROR;
}

status_t SidebandControl::validate() const
{
    if (mHandle==0 || mClient==0) {
        ALOGE("invalid handle (%p) or client (%p)",
                mHandle.get(), mClient.get());
        return NO_INIT;
    }
    return NO_ERROR;
}

sp<Surface> SidebandControl::getSurface() const
{
    Mutex::Autolock _l(mLock);
    if (mSurfaceData == 0) {
        // This surface is always consumed by SurfaceFlinger, so the
        // producerControlledByApp value doesn't matter; using false.
        mSurfaceData = new Surface(mGraphicBufferProducer, false);
    }
    return mSurfaceData;
}

// ----------------------------------------------------------------------------
}; // namespace android

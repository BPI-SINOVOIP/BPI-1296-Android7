/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "SidebandClient"

#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/Singleton.h>
#include <utils/SortedVector.h>
#include <utils/String8.h>
#include <utils/threads.h>

#include <binder/IMemory.h>
#include <binder/IServiceManager.h>

#include <gui/CpuConsumer.h>
#include <gui/IGraphicBufferProducer.h>
#include <gui/ISideband.h>
#include <gui/ISidebandClient.h>
#include <gui/SidebandClient.h>

#include <private/gui/SidebandService.h>
#include <private/gui/LayerState.h>

namespace android {
// ---------------------------------------------------------------------------

ANDROID_SINGLETON_STATIC_INSTANCE(SidebandService);

SidebandService::SidebandService()
: Singleton<SidebandService>() {
    Mutex::Autolock _l(mLock);
    connectLocked();
}

void SidebandService::connectLocked() {
    const String16 name("Sideband");
    while (getService(name, &mSidebandService) != NO_ERROR) {
        usleep(250000);
    }
    assert(mSidebandService != NULL);

    // Create the death listener.
    class DeathObserver : public IBinder::DeathRecipient {
        SidebandService& mSidebandService;
        virtual void binderDied(const wp<IBinder>& who) {
            ALOGW("SidebandService remote (sideband) died [%p]",
                  who.unsafe_get());
            mSidebandService.sidebandServiceDied();
        }
     public:
        DeathObserver(SidebandService& mgr) : mSidebandService(mgr) { }
    };

    mDeathObserver = new DeathObserver(*const_cast<SidebandService*>(this));
    IInterface::asBinder(mSidebandService)->linkToDeath(mDeathObserver);
}

/*static*/ sp<ISideband> SidebandService::getSidebandService() {
    SidebandService& instance = SidebandService::getInstance();
    Mutex::Autolock _l(instance.mLock);
    if (instance.mSidebandService == NULL) {
        SidebandService::getInstance().connectLocked();
        assert(instance.mSidebandService != NULL);
        ALOGD("SidebandService reconnected");
    }
    return instance.mSidebandService;
}

void SidebandService::sidebandServiceDied()
{
    Mutex::Autolock _l(mLock);
    mSidebandService = NULL;
    mDeathObserver = NULL;
}

// ---------------------------------------------------------------------------

SidebandClient::SidebandClient()
    : mStatus(NO_INIT)/* TODO , mComposer(Composer::getInstance())*/
{
}

void SidebandClient::onFirstRef() {
    sp<ISideband> sm(SidebandService::getSidebandService());
    if (sm != 0) {
        // call Sideband.createConnection, return Client.cpp
        sp<ISidebandClient> conn = sm->createConnection();
        if (conn != 0) {
            mClient = conn;
            mStatus = NO_ERROR;
        }
    }
}

SidebandClient::~SidebandClient() {
    dispose();
}

status_t SidebandClient::initCheck() const {
    return mStatus;
}

sp<IBinder> SidebandClient::connection() const {
    return (mClient != 0) ? IInterface::asBinder(mClient) : 0;
}

status_t SidebandClient::linkToSidebandDeath(
        const sp<IBinder::DeathRecipient>& recipient,
        void* cookie, uint32_t flags) {
    sp<ISideband> sm(SidebandService::getSidebandService());
    return IInterface::asBinder(sm)->linkToDeath(recipient, cookie, flags);
}

void SidebandClient::dispose() {
    // this can be called more than once.
    sp<ISidebandClient> client;
    Mutex::Autolock _lm(mLock);
    if (mClient != 0) {
        client = mClient; // hold ref while lock is held
        mClient.clear();
    }
    mStatus = NO_INIT;
}

sp<SidebandControl> SidebandClient::createSurface(
        const String8& name,
        uint32_t w,
        uint32_t h,
        PixelFormat format,
        uint32_t flags)
{
    sp<SidebandControl> sb;
    if (mStatus == NO_ERROR) {
        sp<IBinder> handle;
        sp<IGraphicBufferProducer> gbp;
        status_t err = mClient->createSurface(name, w, h, format, flags,
                &handle, &gbp);
        ALOGE_IF(err, "SidebandClient::createSurface error %s", strerror(-err));
        if (err == NO_ERROR) {
            sb = new SidebandControl(this, handle, gbp);
        }
    }
    return sb;
}

status_t SidebandClient::destroySurface(const sp<IBinder>& sid) {
    if (mStatus != NO_ERROR)
        return mStatus;
    status_t err = mClient->destroySurface(sid);
    return err;
}

status_t SidebandClient::createSidebandStream(const sp<IBinder>& id, native_handle_t ** handle, bool hack) {
    uint64_t key;
    if (mStatus != NO_ERROR)
        return mStatus;

    status_t err = mClient->getBinderKey(id, &key);
    if (err!=NO_ERROR)
        return err;

    const int numFds = 0;
    const int numInts = 3;/* uint64_t key */

    native_handle* h = native_handle_create(numFds, numInts);
    if (!h)
        return NO_MEMORY;

    memcpy(h->data + numFds, &key, numInts*sizeof(int));
    if(hack)
        h->data[numFds + 2] = HACK_SIDEBAND;
    else
        h->data[numFds + 2] = NORMAL_SIDEBAND;

    *handle = h;

    return err;
}

status_t SidebandClient::checkSidebandStream(const native_handle_t *handle, int ID) {
    if(handle == NULL)
       return BAD_VALUE;

    if(handle->data[handle->numFds + 2] == ID)
        return NO_ERROR;
    else
        return BAD_TYPE;

}

status_t SidebandClient::deleteSidebandStream(const sp<IBinder>& /*id*/, native_handle_t * /*handle*/) {
    if (mStatus != NO_ERROR)
        return mStatus;
#if 1
    return NO_ERROR;
#else
    return native_handle_delete(handle);
#endif
}

sp<SidebandControl> SidebandClient::binderSurface(native_handle_t * h) {
    uint64_t key;
    sp<SidebandControl> sb;
    sp<IBinder> handle;
    sp<IGraphicBufferProducer> gbp;
    memcpy(&key, h->data + h->numFds, 2*sizeof(int));
    if (mStatus == NO_ERROR) {
        status_t err = mClient->binderSurface(key/*sid*/, &handle, &gbp);
        if (err == NO_ERROR) {
            sb = new SidebandControl(this, handle, gbp);
            if (sb.get() != NULL)
                sb->setSidebandStream(h);
        }
    }
    return sb;
}

status_t SidebandClient::getKey(const sp<IBinder>& id, native_handle_t * h, uint64_t * key) {
    if (mStatus != NO_ERROR)
        return mStatus;
    if (key == NULL)
        return status_t(-1);
    if (h) {
        memcpy(key, h->data + h->numFds, 2*sizeof(int));
        return NO_ERROR;
    }
    return mClient->getBinderKey(id, key);
}

status_t SidebandClient::setCrop(const sp<IBinder>& id, const Rect& crop) {
    if (mStatus == NO_ERROR) {
        return mClient->setCrop(id, crop);
    }
    return mStatus;
}

status_t SidebandClient::setPosition(const sp<IBinder>& id, float x, float y) {
    if (mStatus == NO_ERROR) {
        return mClient->setPosition(id, x, y);
    }
    return mStatus;
}

status_t SidebandClient::setSize(const sp<IBinder>& id, uint32_t w, uint32_t h) {
    if (mStatus == NO_ERROR) {
        return mClient->setSize(id, w, h);
    }
    return mStatus;
}

status_t SidebandClient::setLayer(const sp<IBinder>& id, int32_t z) {
    if (mStatus == NO_ERROR) {
        return mClient->setLayer(id, z);
    }
    return mStatus;
}

status_t SidebandClient::requestHwSync(int32_t* resource) {
    if (mStatus == NO_ERROR) {
        return mClient->requestHwSync(resource);
    }
    return mStatus;
}

status_t SidebandClient::getHwSyncByResource(int32_t resource, int32_t* audioHwSync) {
    if (mStatus == NO_ERROR) {
        return mClient->getHwSyncByResource(resource, audioHwSync);
    }
    return mStatus;
}

status_t SidebandClient::destroyHwSyncByResource(int32_t resource) {
    if (mStatus == NO_ERROR) {
        return mClient->destroyHwSyncByResource(resource);
    }
    return mStatus;
}

status_t SidebandClient::setAudioHwSync(const sp<IBinder>& id, int32_t audioHwSync) {
    if (mStatus == NO_ERROR) {
        return mClient->setAudioHwSync(id, audioHwSync);
    }
    return mStatus;
}

status_t SidebandClient::hide(const sp<IBinder>& id) {
    if (mStatus == NO_ERROR) {
        return mClient->setFlags(id,
                layer_state_t::eLayerHidden,
                layer_state_t::eLayerHidden);
    }
    return mStatus;
}

status_t SidebandClient::show(const sp<IBinder>& id) {
    if (mStatus == NO_ERROR) {
        return mClient->setFlags(id,
                0,
                layer_state_t::eLayerHidden);
    }
    return mStatus;
}

status_t SidebandClient::setFlags(const sp<IBinder>& id, uint32_t flags,
        uint32_t mask) {
    if (mStatus == NO_ERROR) {
        return mClient->setFlags(id, flags, mask);
    }
    return mStatus;
}

status_t SidebandClient::setResource(const sp<IBinder>& id, uint64_t resource) {
    if (mStatus == NO_ERROR) {
        return mClient->setResource(id, resource);
    }
    return mStatus;
}

status_t SidebandClient::setAlpha(const sp<IBinder>& id, float alpha) {
    if (mStatus == NO_ERROR) {
        return mClient->setAlpha(id, alpha);
    }
    return mStatus;
}

status_t SidebandClient::getHDCPInfo(const sp<IBinder>& id, bool *info) {
    if (mStatus == NO_ERROR) {
        return mClient->getHDCPInfo(id, info);
    }
    return false;
}
// ----------------------------------------------------------------------------
}; // namespace android

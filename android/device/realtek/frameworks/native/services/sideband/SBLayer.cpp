
#define ATRACE_TAG ATRACE_TAG_GRAPHICS
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <math.h>

#include <cutils/compiler.h>
#include <cutils/native_handle.h>
#include <cutils/properties.h>

#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/NativeHandle.h>
#include <utils/StopWatch.h>
#include <utils/Trace.h>

#include <ui/GraphicBuffer.h>
#include <ui/PixelFormat.h>

#include <gui/BufferItem.h>
#include <gui/Surface.h>

#include "SBLayer.h"
#include "SBConsumer.h"

namespace android {

// ---------------------------------------------------------------------------

struct SidebandLayer::LooperThread : public Thread {
    LooperThread(SidebandLayer * layer)
        : mLayer(layer),
        mThreadId(NULL) {
    }

    virtual status_t readyToRun() {
        mThreadId = androidGetThreadId();
        return Thread::readyToRun();
    }       

    virtual bool threadLoop() {
        return mLayer->loop();
    }           

    bool isCurrentThread() const {
        return mThreadId == androidGetThreadId();
    }           

    protected:      
    virtual ~LooperThread() {}

    private:        
    SidebandLayer *         mLayer;
    android_thread_id_t     mThreadId;

};  

// ---------------------------------------------------------------------------

SidebandLayer::SidebandLayer(Sideband* sideband, const sp<Client>& client,
                const String8& name, uint32_t /* flags */)
    :   mSideband(sideband),
        mName(name),
        mFormat(PIXEL_FORMAT_NONE),
        mFlags(/*flags*/ 0),
        mKey(-1ULL)
{
            ClientRegister(client, CreateType);

}

void SidebandLayer::onFirstRef() {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);
    mProducer = producer;

#if 0
    char value[PROPERTY_VALUE_MAX]={0};
    if (property_get("rtk.feature-tunneled-playback", value, NULL)
                && (!strcmp("1", value) || !strcasecmp("true", value)))
        mConsumer = new SidebandConsumer(consumer, 0 /* consumerUsage */, 3 /*bufferCount*/);
    else
        mConsumer = new SidebandConsumer(consumer, 0 /* consumerUsage */, 1 /*bufferCount*/);
#else
    mConsumer = new SidebandConsumer(consumer, 0 /* consumerUsage */, 3 /*bufferCount*/);
#endif
    mConsumer->setFrameAvailableListener(this);
    mConsumer->setName(mName);
    mConsumer->setKey((uint64_t)this);
    //mConsumer->setDefaultMaxBufferCount(3);

    mWindow = mConsumer->getWindowAPI();
    mThread = new LooperThread(this);
    mThread->run(getName().string()); //priority ??

}

status_t SidebandLayer::setKey(uint64_t key) {
    mKey = key;
    if (mConsumer == NULL) {
        ALOGE("[%s] mConsumer is not found", __FUNCTION__);
        return status_t(-1);
    }
    mConsumer->setKey(key);
    return NO_ERROR;
}

SidebandLayer::~SidebandLayer() {
    
    mFlags |= END_OF_RENDER;
    if (mThread != NULL) {
        mThread->requestExit();
        mThread->requestExitAndWait();
        mThread.clear();
    }
}

sp<IBinder> SidebandLayer::getHandle(const sp<Client>& client, enum ClientType type) {
    Mutex::Autolock _l(mLock);

    class Handle : public BBinder, public LayerCleaner {
    public:
        Handle(const sp<Sideband>& sideband, const sp<SidebandLayer>& layer, const sp<Client>& client)
            : LayerCleaner(sideband, layer, client) {}
    };
    ClientRegister(client, type);
    return new Handle(mSideband, this, client);
}

void SidebandLayer::onRemoved() {
    ALOGI("[%s] %p", __FUNCTION__, this);
    mFlags |= END_OF_RENDER;
    const BufferItem item;
    onFrameAvailable(item);
    if (mThread != NULL) {
        mThread->requestExit();
        mThread->requestExitAndWait();
        mThread.clear();
    }
    mConsumer->onRemoved();
}

void SidebandLayer::dump(String8& result, const char* prefix) const {
    String8 mPrefix(prefix);
    result.appendFormat("%s", mPrefix.string());
    result.appendFormat("SBLayer %p (%s) mFlags:0x%x mKey:0x%16llx\n",
            this, getName().string(), mFlags, (long long)mKey);

    mPrefix.append("    ");
    {
        Mutex::Autolock _l(mClientLock);
        for (size_t i=0;i<mClientList.size();i++) {
            sp <Client> client_l (mClientList.valueAt(i).promote());
            result.appendFormat("%s  ", mPrefix.string());
            result.appendFormat(
                    "mClient[%d] %p\n",
                    (int)i, client_l.get());
        }
    }
    if (mConsumer != NULL)
        mConsumer->dump(result, mPrefix.string());

#if 0
    sp<SidebandWindowAPI> window(mWindow.promote());
    if (window != NULL)
        window->dump(result, mPrefix.string());
#endif

}

// ---------------------------------------------------------------------------
status_t SidebandLayer::ClientRegister(const sp<Client>& client, enum ClientType type) {
    ALOGV("[%s] list size:%d client:%p", __FUNCTION__, (int)mClientList.size(), client.get());
    if (client.get() == NULL)
        return status_t(-1);

    {
        Mutex::Autolock _l(mClientLock);
        bool found = false;
        for (size_t i=0;i<mClientList.size();i++) {
            sp <Client> client_l (mClientList.valueAt(i).promote());
            if (client_l != NULL && client_l.get() == client.get()) {
                found = true;
                break;
            }
        }
        if (!found)
            mClientList.add(type, client);
    }
    return NO_ERROR;
}

status_t SidebandLayer::ClientRemove(const sp<Client>& client) {
    ALOGV("[%s] list size:%d client:%p", __FUNCTION__, (int)mClientList.size(), client.get());
    if (mClientList.size() == 0)
        return NO_ERROR;

    bool needToRemove = false;
    {
        Mutex::Autolock _l(mClientLock);

        bool removeing;
        do {
            removeing = false;
            for (size_t i=0;i<mClientList.size();i++) {
                sp <Client> client_l (mClientList.valueAt(i).promote());
                if (client_l == NULL) {
                    mClientList.removeItemsAt(i,1);
                    removeing = true;
                    break;
                }
            }
        } while (removeing);

        if (mClientList.size() == 0)
            return NO_ERROR;

        for (size_t i=0;i<mClientList.size();i++) {
            sp <Client> client_l (mClientList.valueAt(i).promote());
            ALOGV("[%s] mClientList.valueAt(%d):%p client:%p", __FUNCTION__, (int)i, client_l.get(), client.get());
            if (client_l != NULL && client_l.get() == client.get()) {
                mClientList.removeItemsAt(i,1);
                break;
            }
        }

        if (mClientList.size() == 0)
            needToRemove = true;

        if (!needToRemove) {
            bool CreateTypeIsEmpty = true;
            for (size_t i=0;i<mClientList.size();i++) {
                if (mClientList.keyAt(i) == CreateType) {
                    CreateTypeIsEmpty = false;
                    break;
                }
            }
            if (CreateTypeIsEmpty)
                needToRemove = true;
        }
    }
    if (needToRemove) {
        if (mClientList.size() != 0) {
            for (size_t i=0;i<mClientList.size();i++) {
                sp <Client> client_l (mClientList.valueAt(i).promote());
                if (client_l != NULL)
                    client_l->detachLayer(this);
            }
        }
        mSideband->onLayerDestroyed(this);
    }
    return NO_ERROR;
}
// ---------------------------------------------------------------------------

SidebandLayer::LayerCleaner::LayerCleaner(const sp<Sideband>& sideband,
        const sp<SidebandLayer>& layer,
        const sp<Client>& client)
    : mSideband(sideband), mLayer(layer), mClient(client), mpClient(client.get()) {
}

SidebandLayer::LayerCleaner::~LayerCleaner() {
    ALOGV("[%s][+]", __FUNCTION__);
    sp<SidebandLayer> l (mLayer.promote());
    sp<Client> c (mClient.promote());
    if (l != NULL && c != NULL)
            l->ClientRemove(c);
    ALOGV("[%s][-]", __FUNCTION__);
}

// ---------------------------------------------------------------------------

void SidebandLayer::onFrameAvailable(const BufferItem& /* item */) {
    Mutex::Autolock l(mLock);
    if (!mRenderingFrameAvailable) {
        mRenderingFrameAvailable = true;
        mRenderingFrameAvailableSignal.signal();
    }
}

void SidebandLayer::onFrameReplaced(const BufferItem& /* item */) {
    ALOGV("[%s] [%s]", getName().string(), __FUNCTION__);
}

void SidebandLayer::onSidebandStreamChanged() {
    ALOGV("[%s] [%s]", getName().string(), __FUNCTION__);
}

status_t SidebandLayer::setResource(uint64_t resource) {
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    ALOGV("[%s] [%s]", getName().string(), __FUNCTION__);
    const BufferItem item;
    mConsumer->setResource(resource);
    onFrameAvailable(item);
    return NO_ERROR;
}

void SidebandLayer::onProducerNotify(const int notify, const int ext) {
    ALOGI("[%s] [%s] notify:%d ext:%d", getName().string(), __FUNCTION__,
            notify, ext);
    switch (notify) {
        case BUFFER_COUNT:
            mConsumer->setBufferCount(ext);
            break;
        case BUFFER_RELEASING:
            mConsumer->setFreeze();
            break;
    }
}

bool SidebandLayer::loop() {
    if (mFlags & END_OF_RENDER)
        return false;

    status_t res;
    {
        static const nsecs_t kWaitDuration = 100000000; // 100ms
        Mutex::Autolock l(mLock);
        while (!mRenderingFrameAvailable) {
            res = mRenderingFrameAvailableSignal.waitRelative(
                    mLock, kWaitDuration);
#if 0
            if (res == TIMED_OUT) return true;
#endif
        }
        mRenderingFrameAvailable = false;
    }

    if (mFlags & END_OF_RENDER)
        return false;

    res = mConsumer->processRenderingFrame();
    if (res == OK)
        mRenderingFrameAvailable = true;

    return true;
}

// ---------------------------------------------------------------------------
status_t SidebandLayer::setAudioHwSync(int32_t audioHwSync) {
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    return mConsumer->setAudioHwSync(audioHwSync);
}

status_t SidebandLayer::getHDCPInfo(bool *info){
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    return mConsumer->getHDCPInfo(info);
}
// ---------------------------------------------------------------------------

status_t SidebandLayer::setCrop(const Rect& crop) {
    sp<SidebandWindowAPI> window(mWindow.promote());
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    if (window == NULL)
        return status_t(-1);
    return window->setCrop(crop);
}
status_t SidebandLayer::setPosition(float x, float y) {
    sp<SidebandWindowAPI> window(mWindow.promote());
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    if (window == NULL)
        return status_t(-1);
    return window->setPosition(x, y);
}
status_t SidebandLayer::setSize(uint32_t w, uint32_t h) {
    sp<SidebandWindowAPI> window(mWindow.promote());
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    if (window == NULL)
        return status_t(-1);
    return window->setSize(w, h);
}
status_t SidebandLayer::setLayer(int32_t z) {
    sp<SidebandWindowAPI> window(mWindow.promote());
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    if (window == NULL)
        return status_t(-1);
    return window->setLayer(z);
}
status_t SidebandLayer::setFlags(uint32_t flags, uint32_t mask) {
    sp<SidebandWindowAPI> window(mWindow.promote());
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    if (window == NULL)
        return status_t(-1);
    return window->setFlags(flags, mask);
}
status_t SidebandLayer::setAlpha(float alpha) {
    sp<SidebandWindowAPI> window(mWindow.promote());
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    if (window == NULL)
        return status_t(-1);
    return window->setAlpha(alpha);
}

// ---------------------------------------------------------------------------

} // namespace android

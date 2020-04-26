#ifndef SIDEBAND_LAYER_H
#define SIDEBAND_LAYER_H

#include <stdint.h>
#include <sys/types.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <utils/RefBase.h>
#include <utils/String8.h>
#include <utils/Timers.h>
#include <utils/Thread.h>

#include <ui/FrameStats.h>
#include <ui/GraphicBuffer.h>
#include <ui/PixelFormat.h>
#include <ui/Region.h>

#include <gui/ISurfaceComposerClient.h>

#include <private/gui/LayerState.h>

#include "SBConsumer.h"
#include "Client.h"
#include "Sideband.h"

namespace android {

class Client;
class Sideband;

class SidebandLayer: public SidebandConsumer::FrameAvailableListener,
    public SidebandWindowAPI
{
public:
    enum ClientType {
        CreateType,
        BinderType,
    };

    SidebandLayer(Sideband* sideband, const sp<Client>& client,
            const String8& name, uint32_t flags);
    ~SidebandLayer();
    sp<IBinder>                 getHandle(const sp<Client>& client, enum ClientType type);
    sp<IGraphicBufferProducer>  getProducer()   const { return mProducer;};
    const String8&              getName()       const { return mName;};
    void                        onRemoved();
    void                        dump(String8& result, const char* prefix) const;

    /* SidebandWindowAPI */
    virtual status_t setCrop(const Rect& crop);
    virtual status_t setPosition(float x, float y);
    virtual status_t setSize(uint32_t w, uint32_t h);
    virtual status_t setLayer(int32_t z);
    virtual status_t setFlags(uint32_t flags, uint32_t mask);
    virtual status_t setAlpha(float alpha);
    virtual status_t setResource(uint64_t resource);

    virtual status_t setAudioHwSync(int32_t audioHwSync);
    virtual status_t getHDCPInfo(bool *info);
    virtual status_t ClientRegister(const sp<Client>& client, enum ClientType type);
    virtual status_t ClientRemove(const sp<Client>& client);

    virtual status_t setKey(uint64_t key);
    virtual uint64_t getKey(void) {return mKey;};
protected:
    sp<Sideband>                mSideband;
    virtual void                onFirstRef();
    /*
     * Trivial class, used to ensure that mSideband->onLayerDestroyed(mLayer)
     * is called.
     */
    class LayerCleaner {
    private:
        sp<Sideband>                mSideband;
        wp<SidebandLayer>           mLayer;
        wp<Client>                  mClient;
        Client *                    mpClient;
    protected:
        ~LayerCleaner();
    public:
        LayerCleaner(const sp<Sideband>& sideband, const sp<SidebandLayer>& layer, const sp<Client>& client);
    };

private:
    /*
     * Interface implementation for SidebandConsumer::FrameAvailableListener
     */
    virtual void onFrameAvailable(const BufferItem& item);
    virtual void onFrameReplaced(const BufferItem& item);
    virtual void onSidebandStreamChanged();
    virtual void onProducerNotify(const int notify, const int ext);

    enum flags_t {
        FIRST_RENDER            = 1U <<  0,
        END_OF_RENDER           = 1U <<  1,
    };

    /*
     * constant
     */
    sp<SidebandConsumer>        mConsumer;
    sp<IGraphicBufferProducer>  mProducer;
    String8                     mName;
    PixelFormat                 mFormat;

    volatile uint32_t           mFlags;
    volatile uint64_t           mKey;

    /* protected by mLock */
    mutable Mutex               mLock;

    bool loop();
    struct LooperThread;
    sp<LooperThread>            mThread;

    Condition                   mRenderingFrameAvailableSignal;
    bool                        mRenderingFrameAvailable;
    wp<SidebandWindowAPI>       mWindow;

    DefaultKeyedVector<enum ClientType, wp<Client>> mClientList;
    mutable Mutex               mClientLock;
};

}; // namespace android
#endif /* End of SIDEBAND_CONSUMER_H */

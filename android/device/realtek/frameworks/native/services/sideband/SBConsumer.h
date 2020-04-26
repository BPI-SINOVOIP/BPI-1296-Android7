#ifndef SIDEBAND_CONSUMER_H
#define SIDEBAND_CONSUMER_H

#include <gui/BufferItemConsumer.h>

#include "Display/VOLayer/VOLayer.h"
#include "WindowAPI.h"

namespace android {

class SidebandConsumer: public BufferItemConsumer
{
public:
    typedef BufferItemConsumer::FrameAvailableListener FrameAvailableListener;

    SidebandConsumer(const sp<IGraphicBufferConsumer>& consumer, uint32_t consumerUsage,
            int bufferCount = BufferItemConsumer::DEFAULT_MAX_BUFFERS, bool controlledByApp = false);

    virtual ~SidebandConsumer();

    void                            onRemoved();

    virtual status_t                processRenderingFrame();

    virtual wp<SidebandWindowAPI>   getWindowAPI() {return mVOLayer;};

    virtual status_t                setAudioHwSync(int audioHwSync);
    virtual void                    setBufferCount(int count);
    virtual void                    setFreeze(void);
    virtual status_t                setResource(uint64_t resource);
    virtual status_t                setKey(uint64_t key);
    void                            dump(String8& result, const char* prefix) const;
    virtual status_t                getHDCPInfo(bool *info);
private:
    enum flags_t {
        FIRST_RENDER            = 1U <<  0,
        END_OF_RENDER           = 1U <<  1,
    };
    volatile uint32_t           mFlags;
    sp <VOLayer>                mVOLayer;
    mutable Mutex               mSBLock;
    volatile uint64_t           mResource;
    volatile uint64_t           mKey;
    bool                        mUseHwSync;
    bool                        mSkipFrameWhenNotReady;
    int                         mSkipFrameCnt;
};

}; // namespace android
#endif /* End of SIDEBAND_CONSUMER_H */

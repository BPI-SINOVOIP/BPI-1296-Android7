#ifndef RTK_GUI_SIDEBAND_CONTROL_H
#define RTK_GUI_SIDEBAND_CONTROL_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/threads.h>

#include <ui/PixelFormat.h>

#include <gui/ISidebandClient.h>

namespace android {

// ---------------------------------------------------------------------------

class IGraphicBufferProducer;
class Surface;
class SidebandClient;

// ---------------------------------------------------------------------------

class SidebandControl : public RefBase
{
    public:
        enum {
            HIDDEN          = 0x1U <<   0,  //layer_state_t::eLayerHidden, /* 0x01    |     1:hide 0:shwo */
            OPAQUE          = 0x1U <<   1,  //layer_state_t::eLayerOpaque, /* 0x02 */
            REFRESH         = 0x1U <<   16,
            FLUSH           = 0x1U <<   18,
        };
        static bool isValid(const sp<SidebandControl>& surface) {
            return (surface != 0) && surface->isValid();
        }

        bool isValid() {
            return mHandle!=0 && mClient!=0;
        }

        static bool isSameSurface(
                const sp<SidebandControl>& lhs, const sp<SidebandControl>& rhs);

        status_t    getSidebandStream(native_handle_t ** handle);
        void        setSBHint(bool hack);
        status_t    setLayer(int32_t layer);
        status_t    setPosition(float x, float y);
        status_t    setSize(uint32_t w, uint32_t h);
        status_t    hide();
        status_t    show();
        status_t    refresh();
        status_t    setFlags(uint32_t flags, uint32_t mask);
        status_t    setAlpha(float alpha=1.0f);
        status_t    setCrop(const Rect& crop);

        status_t    setResource(uint64_t resource);

        status_t    requestHwSync(int32_t* resource);
        status_t    getHwSyncByResource(int32_t resource, int32_t* audioHwSync);
        status_t    destroyHwSyncByResource(int32_t resource);
        status_t    setAudioHwSync(int32_t audioHwSync);

        sp<Surface> getSurface() const;

        status_t    getKey(uint64_t * key);
        status_t    getHDCPInfo(bool *info);
    private:
        // can't be copied
        SidebandControl& operator = (SidebandControl& rhs);
        SidebandControl(const SidebandControl& rhs);

        friend class SidebandClient;
        friend class Surface;

        status_t    setSidebandStream(native_handle_t * handle);
        status_t    createSidebandStream(native_handle_t ** handle);
        SidebandControl(
                const sp<SidebandClient>& client,
                const sp<IBinder>& handle,
                const sp<IGraphicBufferProducer>& gbp);

        ~SidebandControl();

        status_t validate() const;
        void destroy();

        sp<SidebandClient>          mClient;
        sp<IBinder>                 mHandle;
        sp<IGraphicBufferProducer>  mGraphicBufferProducer;
        mutable Mutex               mLock;
        mutable sp<Surface>         mSurfaceData;

        native_handle_t *           mSidebandStream;
        bool                        mHackSideband;
};

}; // namespace android

#endif /* End of RTK_GUI_SIDEBAND_CONTROL_H */

#ifndef __RTK_SB_VOLAYER_H__
#define __RTK_SB_VOLAYER_H__

#include <utils/RefBase.h>
#include <utils/String8.h>
#include <utils/Mutex.h>

#include <ui/Region.h>

#include <hardware/gralloc.h>

#include "hwlayer/video/VideoLayer.h"
#include <VOWindowRPC.h>
#include <WindowAPI.h>
#include "hwlayer/utils/GraphicBufferInfo.h"


namespace android {

class VOLayer : /*public RefBase,*/ public SidebandWindowAPI {
public:

    enum layer_type {
         LAYER_V1   = VO_VIDEO_PLANE_V1,
         LAYER_V2   = VO_VIDEO_PLANE_V2,
#if 0    /* NOT READY */
         LAYER_SUB1 = VO_VIDEO_PLANE_SUB1,
         LAYER_OSD1 = VO_VIDEO_PLANE_OSD1,
         LAYER_OSD2 = VO_VIDEO_PLANE_OSD2,
         LAYER_WIN1 = VO_VIDEO_PLANE_WIN1,
         LAYER_WIN2 = VO_VIDEO_PLANE_WIN2,
         LAYER_WIN3 = VO_VIDEO_PLANE_WIN3,
         LAYER_WIN4 = VO_VIDEO_PLANE_WIN4,
         LAYER_WIN5 = VO_VIDEO_PLANE_WIN5,
#endif
         LAYER_NONE = VO_VIDEO_PLANE_NONE,
         LAYER_NULL,
    };

    VOLayer();
    ~VOLayer();

    /* Render to VO */
    status_t                    setLayerType(enum layer_type type);
    status_t                    setBufferCount(int BufferCnt = 8);
    status_t                    render(buffer_handle_t handle, int *pFenceFd);
    status_t                    validate(buffer_handle_t handle);
    status_t                    freeze(bool enable = true);
    status_t                    setAudioHwSync(int audioHwSync);

    virtual void dump(String8& result, const char* prefix) const;

    /* SidebandWindowAPI */
    virtual status_t                    setCrop(const Rect& crop);
    virtual status_t                    setPosition(float x, float y);
    virtual status_t                    setSize(uint32_t w, uint32_t h);
    virtual status_t                    setLayer(int32_t z);
    virtual status_t                    setFlags(uint32_t flags, uint32_t mask);
    virtual status_t                    setAlpha(float alpha);
    virtual bool getHDCPInfo(void);
private:
    enum flags_t {
        FLUSH                   = android::SidebandWindowAPI::FLUSH,
        FIRST_RENDER            = 1U <<  0,
        END_OF_RENDER           = 1U <<  1,
    };
    volatile uint32_t           mFlags;
    sp <VideoLayer>             mHWLayer;
    sp <VOWindowRPC>            mWindow;
    enum layer_type             mLayerType;
    int                         mBufferCnt;
    int                         mAudioHwSync;
    mutable Mutex               mLock;

    GraphicBufferInfo *         mGBInfo;
    GraphicBufferInfo::Client * mGBInfoClient;
};

}; // namespace android
#endif /* End of __RTK_SB_VOLAYER_H__ */

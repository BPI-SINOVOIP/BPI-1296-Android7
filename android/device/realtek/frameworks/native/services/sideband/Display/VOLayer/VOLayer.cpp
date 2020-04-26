
#include "VOLayer.h"
namespace android {

#define HWC_PROPERTY_PLANE_V1                       0
#define HWC_PROPERTY_PLANE_V2                       1
#define HWC_PROPERTY_LINEAR_PLAVE                   "rtk.hwc.linear.plane" /* 0:V1 1:V2 other:V1 */
#define HWC_PROPERTY_LINEAR_PLAVE_EN                "rtk.hwc.linear.plane.en" /* 0:disable 1:enable */

static int get_hwc_linear_plane(void) {
    char value[PROPERTY_VALUE_MAX];
    if (property_get(HWC_PROPERTY_LINEAR_PLAVE, value, "0") > 0)
        return atoi(value);
    return -1;
}

static bool get_hwc_linear_plane_enable(void) {
    char value[PROPERTY_VALUE_MAX];
    int en = 0;
    if (property_get(HWC_PROPERTY_LINEAR_PLAVE_EN, value, "0") > 0)
        en = atoi(value);
    return (en == 1) ? true : false;
}

VOLayer::VOLayer()
    : mFlags(FIRST_RENDER),
    mLayerType(LAYER_NULL),
    mBufferCnt(8),
    mAudioHwSync(-1)
{
    mHWLayer = new VideoLayer();
    mWindow = new VOWindowRPC();

    mGBInfo = new GraphicBufferInfo();
    mGBInfoClient = new GraphicBufferInfo::Client();
    mGBInfoClient->setGraphicBufferInfo(mGBInfo);
}

VOLayer::~VOLayer() {
    mWindow->setFlags(HIDDEN|REFRESH,HIDDEN|REFRESH);
    mHWLayer->disconnect();
    mHWLayer.clear();
    mWindow.clear();
    if (mAudioHwSync >= 0) {
        ion_close(mAudioHwSync);
        mAudioHwSync = -1;
    }
    mGBInfoClient->setGraphicBufferInfo();
    delete mGBInfoClient;
    delete mGBInfo;
}

status_t VOLayer::setLayerType(enum layer_type type) {
    if (mFlags & FIRST_RENDER) {
        mLayerType = type;
    }
    return NO_ERROR;
}

status_t VOLayer::setBufferCount(int BufferCnt) {
    if (mFlags & FIRST_RENDER) {
        mBufferCnt = BufferCnt;
    }
    return NO_ERROR;
}

status_t VOLayer::render(buffer_handle_t handle, int *pFenceFd) {
    Mutex::Autolock l(mLock);
    int ret;
    if (mFlags & FIRST_RENDER) {
        if (get_hwc_linear_plane_enable()) {
            /*
             * [DHCKYLIN-1154]
             * For hardware limitations(A01), the compressed data sent to V2 layer, others sent V1 layer.
             */
            unsigned int format;
            mGBInfoClient->setBufferHandle(handle);

            /* default: mLayerType is linear type */
            mLayerType = (get_hwc_linear_plane() == HWC_PROPERTY_PLANE_V1) ? LAYER_V1 : LAYER_V2;

            if (mGBInfoClient->getFormat(&format) == 0) {
                switch (format) {
                    case HAL_PIXEL_FORMAT_RTK_P8_COMPRESSED:
                    case HAL_PIXEL_FORMAT_RTK_P10_COMPRESSED:
                    case HAL_PIXEL_FORMAT_RTK_W32_H32_P8_COMPRESSED:
                    case HAL_PIXEL_FORMAT_RTK_W32_H32_P10_COMPRESSED:
                        mLayerType = (get_hwc_linear_plane() == HWC_PROPERTY_PLANE_V1) ? LAYER_V2 : LAYER_V1;
                        break;
                    default:
                        break;
                }
            }
            mGBInfoClient->clearBufferHandle();
        }
        ret = mHWLayer->connect((enum VO_VIDEO_PLANE) mLayerType, mBufferCnt, mAudioHwSync);
        if (ret)
            return status_t(ret);
        mWindow->setPlane((enum VO_VIDEO_PLANE) mLayerType);
        mFlags &= ~FIRST_RENDER;
        mWindow->setFlags(VOWindowRPC::FRAME_READY,VOWindowRPC::FRAME_READY);
    }

    ret = mHWLayer->render(handle, pFenceFd);
    return status_t(ret);
}

status_t VOLayer::validate(buffer_handle_t handle) {
    int ret = mHWLayer->validate(handle);
    return status_t(ret);
}

void VOLayer::dump(String8& result, const char* prefix) const {
    String8 mPrefix(prefix);
    result.appendFormat("%s", mPrefix.string());
    result.appendFormat("VOLayer %p mFlags:0x%x mLayerType:%d mBufferCnt:%d mAudioHwSync:%d\n",
            this, mFlags, mLayerType, mBufferCnt, mAudioHwSync);

    mPrefix.append("    ");

    if (mWindow != NULL)
        mWindow->dump(result, mPrefix.string());

    if (mHWLayer != NULL)
        mHWLayer->dump(result, mPrefix.string());
}

status_t VOLayer::freeze(bool enable) {
    Mutex::Autolock l(mLock);
    if (mFlags & FIRST_RENDER) return status_t(-1);
    int ret = mHWLayer->freeze(enable);
    return status_t(ret);
}

status_t VOLayer::setAudioHwSync(int audioHwSync) {
    if (mAudioHwSync > 0) {
        if (mHWLayer->isConnect())
            close(audioHwSync);
        else {
            close(mAudioHwSync);
            mAudioHwSync = audioHwSync;
        }
    } else
        mAudioHwSync = audioHwSync;
    return NO_ERROR;
}

status_t VOLayer::setCrop(const Rect& crop) {
    int ret = mWindow->setCrop(crop.left, crop.top, crop.right, crop.bottom);
    return status_t(ret);
}

status_t VOLayer::setPosition(float x, float y) {
    int ret = mWindow->setPosition(x, y);
    return status_t(ret);
}

status_t VOLayer::setSize(uint32_t w, uint32_t h) {
    int ret = mWindow->setSize(w, h);
    return status_t(ret);
}

status_t VOLayer::setLayer(int32_t z) {
    int ret = mWindow->setLayer(z);
    return status_t(ret);
}

status_t VOLayer::setFlags(uint32_t flags, uint32_t mask) {
    if ((flags & FLUSH) && (mask & FLUSH)) {
        mHWLayer->freeze(true);
        flags &= ~FLUSH;
        mask &= ~FLUSH;
        if (mask == 0)
            return 0;
    }
    int ret = mWindow->setFlags(flags, mask);
    return status_t(ret);
}

status_t VOLayer::setAlpha(float alpha) {
    int ret = mWindow->setAlpha(alpha);
    return status_t(ret);
}

bool VOLayer::getHDCPInfo(void) {
    return mHWLayer->getHDCPInfo();
}

}; // namespace android

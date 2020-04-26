
#include <cutils/properties.h>
#include "SBConsumer.h"
namespace android {

static bool disable_skip_frame (void) {
    char value[PROPERTY_VALUE_MAX]={0};
    if (property_get("persist.rtk.sb.skip.dsb", value, NULL)
            && (!strcmp("1", value) || !strcasecmp("true", value)))
        return true;
    return false;
}

SidebandConsumer::SidebandConsumer(const sp<IGraphicBufferConsumer>& consumer, uint32_t consumerUsage, int bufferCount, bool controlledByApp)
    : BufferItemConsumer(consumer, consumerUsage, bufferCount, controlledByApp),
    mFlags(0),
    mVOLayer(NULL),
    mResource(0),
    mKey(0),
    mUseHwSync(false),
    mSkipFrameCnt(0)
{
    mSkipFrameWhenNotReady = !disable_skip_frame();
    mVOLayer = new VOLayer();
    mVOLayer->setLayerType(VOLayer::LAYER_V1);
    mVOLayer->setBufferCount(4);
}

SidebandConsumer::~SidebandConsumer() {
    mVOLayer.clear();
}

void SidebandConsumer::onRemoved() {
    {
        mFlags |= END_OF_RENDER;
        Mutex::Autolock l(mSBLock);
        mVOLayer->freeze();
    }
    ALOGI("[%s] => abandon()", __FUNCTION__);
    abandon();
}

void SidebandConsumer::dump(String8& result, const char* prefix) const {
    String8 mPrefix(prefix);
    result.appendFormat("%s", mPrefix.string());
    result.appendFormat("SBConsumer %p mFlags:0x%x mResource:0x%llx mKey:0x%llx mUseHwSync:%d mSkipFrameCnt:%d\n",
            this, mFlags, (long long)mResource, (long long)mKey,
            mUseHwSync, mSkipFrameCnt);

    mPrefix.append("    ");
            
    BufferItemConsumer::dump(result, mPrefix.string());
    if (mVOLayer != NULL)
        mVOLayer->dump(result, mPrefix.string());
}

status_t SidebandConsumer::setAudioHwSync(int audioHwSync) {
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    mUseHwSync = true;
    return mVOLayer->setAudioHwSync(audioHwSync);
}

void SidebandConsumer::setBufferCount(int count) {
    if (mFlags & END_OF_RENDER) return;
    mVOLayer->setBufferCount(count);
}

void SidebandConsumer::setFreeze() {
    if (mFlags & END_OF_RENDER) return;
    ALOGV("[%s] [+]", __FUNCTION__);
    mVOLayer->freeze();
    ALOGV("[%s] [-]", __FUNCTION__);
}

status_t SidebandConsumer::setResource(uint64_t resource) {
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    mResource = resource;
    return NO_ERROR;
}

status_t SidebandConsumer::setKey(uint64_t key) {
    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;
    mKey = key;
    return NO_ERROR;
}

status_t SidebandConsumer::processRenderingFrame() {
    status_t res;
    bool resourceIsReady = true;
    /*BufferItemConsumer::*/BufferItem item;

    Mutex::Autolock l(mSBLock);

    if (mFlags & END_OF_RENDER) return UNKNOWN_ERROR;


    int64_t nowNs = systemTime(CLOCK_REALTIME);
    int64_t resourceNs = mResource ^ mKey;

    if ((nowNs - resourceNs) > 3000000000/* && mResource != 0*/) {
        ALOGW("[%s] [%s] Resource is not ready!   wait HWC to connect!...... resourceNs:%lld nowNs:%lld",
                mName.string(), __FUNCTION__, (long long)resourceNs, (long long)nowNs);

        if (mUseHwSync)
            return UNKNOWN_ERROR;

        if (!mSkipFrameWhenNotReady)
            return UNKNOWN_ERROR;

        resourceIsReady = false;
    }

    //ALOGV("[%s] [%s]", mName.string(), __FUNCTION__);

    if (mVOLayer == NULL) {
        ALOGE("[%s] mVOLayer is null!!", __FUNCTION__);
        return UNKNOWN_ERROR;
    }

    res = acquireBuffer(&item, 0/*presentWhen*/);

    if (res != OK || item.mGraphicBuffer == NULL) {
        ALOGV("[%s] [%s]", mName.string(), __FUNCTION__);
        return UNKNOWN_ERROR;
    }

    int fenceFd = -1;

    if (mFlags & END_OF_RENDER) {
        ALOGW("[%s] SKIP RENDER! (EOS)", __FUNCTION__);
    } else if (!resourceIsReady) {
        mSkipFrameCnt++;
        ALOGW("[%s] SKIP RENDER! (Resource is not ready) skip count : %d", __FUNCTION__, mSkipFrameCnt);
    } else {
        mVOLayer->render(item.mGraphicBuffer->handle, &fenceFd);
    }

    if (fenceFd == -1) {
        ALOGE("[%s] [%s]", mName.string(), __FUNCTION__);
        //releaseBuffer(item, NULL);
        //releaseBuffer(item, new Fence(-1));
        releaseBufferLocked(item.mSlot, item.mGraphicBuffer, EGL_NO_DISPLAY, EGL_NO_SYNC_KHR);
    } else {
        releaseBuffer(item, new Fence(dup(fenceFd)));
    }
    
    if (mResource == 0)
        mResource++;

    return OK;
}

status_t SidebandConsumer::getHDCPInfo(bool *info) {
    *info = mVOLayer->getHDCPInfo();
    return NO_ERROR;
}

}; // namespace android

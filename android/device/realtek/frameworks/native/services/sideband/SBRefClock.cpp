#include <SBRefClock.h>

namespace android {

SBRefClock::SBRefClock()
    :mRequestTime(0),
     mRefClockfd(-1),
     mResource(-1),
     mClientCount(0)
{
    ALOGI("[%s %d]\n", __FUNCTION__, __LINE__);
}

SBRefClock::~SBRefClock()
{
    ALOGI("[%s %d]\n", __FUNCTION__, __LINE__);
    if (mRefClockfd >= 0)
        ion_close(mRefClockfd);
    mRefClockfd = -1;
    mRequestTime = 0;
    mClientCount = 0;
}

status_t SBRefClock::allocHwSyc(int32_t* resource)
{
    if (mRefClockfd > 0) {
        *resource = mRefClockfd;
        return NO_ERROR;
    }

    int ionFd = ion_open();
    if (ionFd < 0) {
        ALOGD("[%s %d] ion_open failed", __FUNCTION__, __LINE__);
    }

    ion_user_handle_t ionHandle;
    int ret = ion_alloc(ionFd, sizeof(REFCLOCK), getpagesize(), RTK_PHOENIX_ION_HEAP_AUDIO_MASK,
            ION_FLAG_NONCACHED |ION_FLAG_SCPUACC | ION_FLAG_ACPUACC, &(ionHandle));
    if (ret < 0) {
        ALOGE("[%s %d] fail to allocate ion buffer", __FUNCTION__, __LINE__);
    }

    ret = ion_share(ionFd, ionHandle, &mRefClockfd);
    if (ret < 0) {
        ALOGE("[%s %d] ion share error", __FUNCTION__, __LINE__);
    }

    *resource = mRefClockfd;
    ALOGI("[%s %d] resource %d", __FUNCTION__, __LINE__, *resource);

    ion_free(ionFd, ionHandle);
    ion_close(ionFd);
    return NO_ERROR;
}

status_t SBRefClock::requestHwSync(const sp<Client>& client, int32_t* resource)
{
    ALOGI("[%s %d]", __FUNCTION__, __LINE__);
    mRequestTime = systemTime(CLOCK_REALTIME);
    ALOGI("client %p mRequestTime %lld\n", client.get(), mRequestTime);
    status_t err = allocHwSyc(resource);

    return err;
}

status_t SBRefClock::getHwSyncByResource(const sp<Client>& /*client*/, int32_t resource, int32_t* audioHwSync)
{
    if (resource == mRefClockfd) {
        int64_t nowNs = systemTime(CLOCK_REALTIME);
        ALOGI("nowNs %lld mRequestTime %lld diff %lld", nowNs, mRequestTime, nowNs - mRequestTime);
        if (nowNs - mRequestTime > GET_HWSYNC_THRESHOLD)
            ALOGW("%s %d used the resource too late ", __FUNCTION__, __LINE__);
        *audioHwSync = mRefClockfd;
        mClientCount++;
        ALOGI("[%s %d] mClientCount %d", __FUNCTION__, __LINE__, mClientCount);
    } else {
        ALOGE("%s %d not found audioHwSync", __FUNCTION__, __LINE__);
    }
    return NO_ERROR;
}

status_t SBRefClock::destroyHwSyncByResource(const sp<Client>& /*client*/, int32_t resource)
{
    ALOGI("[%s %d] resource %d\n", __FUNCTION__, __LINE__, resource);

    if (resource == mRefClockfd)
        mClientCount--;
    else
        ALOGE("%s %d not found resource %d\n", __FUNCTION__, __LINE__, resource);

    if (mClientCount == 0) {
        ALOGI("[%s %d] resource %d ion close\n", __FUNCTION__, __LINE__, resource);
        if (mRefClockfd >= 0)
            ion_close(mRefClockfd);
        mRefClockfd = -1;
        mRequestTime = 0;
        mClientCount = 0;
    }
    return NO_ERROR;
}

}

#include "SWFence.h"

#include <sync/sync.h>
#ifdef ANDROID_VERSION_5_0_0
#include <sw_sync.h>
#else
#include <sync/sw_sync.h>
#endif

void SWFence::Client::dump(android::String8& buf, const char* prefix) {
    if (prefix != NULL)
        buf.appendFormat("%s", prefix);
    buf.appendFormat("Fence[fd=%d,context=%d]", mFenceFd, mContext);
    if (prefix != NULL)
        buf.append("\n");
}
SWFence::SWFence() {
    mLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    mClientLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mLock);
    mSync_timeline_fd   = sw_sync_timeline_create();
    mFenceIndex = 1;
    pthread_mutex_unlock(&mLock);
}

SWFence::~SWFence() {
    while (mClientList.size() > 0) {
        Client * client = mClientList.itemAt(0);
        removeClient(client);
#if 1 // debug
        if (client == mClientList.itemAt(0))
            ALOGE("[%s] ERROR! client=%p", __func__, client);
#endif
    }
#if 0
    sw_sync_timeline_inc(mSync_timeline_fd, 3);
#endif
    usleep(100);
    pthread_mutex_lock(&mLock);
    close(mSync_timeline_fd);
    pthread_mutex_unlock(&mLock);
}

int SWFence::acquiredFenceFd() {
    int retFenceFd = InvalidFd;
    pthread_mutex_lock(&mLock);
    retFenceFd = sw_sync_fence_create(mSync_timeline_fd, "SWFence", mFenceIndex);
    if (retFenceFd != InvalidFd)
        mFenceIndex++;
    pthread_mutex_unlock(&mLock);
    return retFenceFd;
}

void SWFence::releaseFenceFd(int fenceFd) {
    pthread_mutex_lock(&mLock);
    if (fenceFd != InvalidFd) {
        if (sw_sync_timeline_inc(mSync_timeline_fd, 1) < 0)
            ALOGE("[%s] can't increment sync obj. fd:%d mFenceIndex:%d", __FUNCTION__, fenceFd, mFenceIndex);
        close(fenceFd);
    }
    pthread_mutex_unlock(&mLock);
}

void SWFence::registerClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (!found) {
        if (client != NULL && client->mFenceFd == InvalidFd) {
            mClientList.add(client);
            client->mService = this;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void SWFence::removeClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            if (client->mFenceFd != InvalidFd) {
                releaseFenceFd(client->mFenceFd);
                client->mFenceFd = InvalidFd;
            }
            mClientList.removeAt(i);
            client->mService = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void SWFence::acquiredFenceFd(Client * client) {
    pthread_mutex_lock(&mClientLock);
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (found) {
        client->mContext = mFenceIndex;
        client->mFenceFd = acquiredFenceFd();
    }
    pthread_mutex_unlock(&mClientLock);
}

int SWFence::releaseFenceFd(Client * client) {
    int ret = -1;
    pthread_mutex_lock(&mClientLock);
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (found) {
        if (client->mFenceFd != InvalidFd) {
            releaseFenceFd(client->mFenceFd);
            client->mFenceFd = InvalidFd;
        }
        ret = 0;
    }
    pthread_mutex_unlock(&mClientLock);
    return ret;
}

#include "GraphicBufferInfo.h"

GraphicBufferInfo::Client::Client()
    : mLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mReleaseLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mService(NULL),
    mHandle(NULL),
    mState(BufferFree),
    mViraddr(MAP_FAILED),
    mViraddrSize(0),
    mIonBuffer(NULL),
    mExtBuffer(NULL),
    mShareBuffer(NULL)
{
    memset((void *) &mYUVData, 0, sizeof(mYUVData));
    memset((void *) &mPrivData, 0, sizeof(mPrivData));
}

void GraphicBufferInfo::Client::setBufferHandle(buffer_handle_t handle) {
    pthread_mutex_lock(&mLock);
    if (mService)
        mService->setBufferHandle(this, handle);
    else
        mHandle = handle;

    if (mState == Client::BufferGralloc && mYUVData.share_fd > 0)
        mShareBuffer = new IonBuffer(IonBuffer::ImportBuffer_lock, (long long) mYUVData.share_fd);

    pthread_mutex_unlock(&mLock);
}

void GraphicBufferInfo::Client::setExtBuffer(ExtBuffer * buffer) {
    pthread_mutex_lock(&mLock);
    if (mService)
        mService->setExtBuffer(this, buffer);
    else
        mExtBuffer = buffer;
    pthread_mutex_unlock(&mLock);
}

int GraphicBufferInfo::Client::validate(buffer_handle_t handle) {
    int ret = -1;
    pthread_mutex_lock(&mLock);
    if (mService)
        ret = mService->validate(this, handle);
    pthread_mutex_unlock(&mLock);
    return ret;
}

unsigned long GraphicBufferInfo::Client::getOffset(unsigned int slot) {
    unsigned long offset = 0;
    if (mState == BufferGralloc || mState == BufferReplica) {

#if 0
        if ((mYUVData.format & HAL_PIXEL_FORMAT_RTK_P10_MAGIC_MASK) == HAL_PIXEL_FORMAT_RTK_P10_MAGIC) {
            if (slot < RTK_P10_MAX_COMPRESSED_PLANE)
                offset = mPrivData.p10_compressed_offset[slot];
        } else {
#endif
            if (slot == 0) {
                offset = 0;
            } else if (slot == 1) {
                switch (mYUVData.format) {
                    case HAL_PIXEL_FORMAT_RTK_W16_H8_YCbCr_420_SP:
                    case HAL_PIXEL_FORMAT_RTK_W16_H8_YCrCb_420_SP:
                        offset = (GRALLOC_ALIGN(mYUVData.height,  8) * mYUVData.stride);
                        break;
                    case HAL_PIXEL_FORMAT_RTK_W16_H32_YCbCr_420_SP:
                    case HAL_PIXEL_FORMAT_RTK_W32_H32_YCbCr_420_SP:
                    case HAL_PIXEL_FORMAT_RTK_W16_H32_YCrCb_420_SP:
                    case HAL_PIXEL_FORMAT_RTK_W32_H32_YCrCb_420_SP:
                        offset = (GRALLOC_ALIGN(mYUVData.height, 32) * mYUVData.stride);
                        break;
                    case HAL_PIXEL_FORMAT_RTK_P8_COMPRESSED:
                    case HAL_PIXEL_FORMAT_RTK_P10_COMPRESSED:
                        offset = (mYUVData.height * mYUVData.stride);
                        break;
                    case HAL_PIXEL_FORMAT_RTK_W32_H32_P8_COMPRESSED:
                    case HAL_PIXEL_FORMAT_RTK_W32_H32_P10_COMPRESSED:
                        offset = (GRALLOC_ALIGN(mYUVData.height,32) * mYUVData.stride);
                        break;
                    default:
                        offset = (GRALLOC_ALIGN(mYUVData.height, 16) * mYUVData.stride);
                        break;
                }
            }
#if 0
        }
#endif
    }
    return offset;
}

void * GraphicBufferInfo::Client::getPhyAddr(unsigned int slot, long long * pBufferSize) {
    pthread_mutex_lock(&mLock);
    long long retSize = 0;
    void * retAddr = NULL;

    if (mState == BufferGralloc) {
        retSize = (long long) mYUVData.size;
        retAddr = (void *) (mYUVData.phyAddr & -1UL);
    } else if (mState == BufferReplica) {
        if (mIonBuffer != NULL) {
            retSize = mIonBuffer->getSize();
            retAddr = mIonBuffer->getPhyAddr();
        }
    }

    if (retAddr) {
        retAddr = (void *) (((unsigned long) retAddr )+ getOffset(slot));
    }

    if (pBufferSize)
        *pBufferSize = retSize;
    pthread_mutex_unlock(&mLock);

    return retAddr;
}

void * GraphicBufferInfo::Client::getVirAddr(long long * pBufferSize) {
    pthread_mutex_lock(&mLock);
    long long retSize = 0;
    void * retAddr = NULL;

    if (mState == BufferGralloc) {
        if (mViraddr ==  MAP_FAILED && mShareBuffer != NULL) {
            mViraddr = mmap(NULL, mYUVData.size, PROT_READ | PROT_WRITE, MAP_SHARED, mShareBuffer->getShareFd(), 0);
            if (mViraddr != MAP_FAILED)
                mViraddrSize = mYUVData.size;
        }
        if (mViraddr != MAP_FAILED) {
            retSize = mViraddrSize;
            retAddr = mViraddr;
        } else
            ALOGE("[%s] MAP_FAILED! (ion:share_fd = %d , size = %d)",
                    __FUNCTION__, mYUVData.share_fd, mYUVData.size);
    } else if (mState == BufferReplica) {
        if (mIonBuffer != NULL) {
            retSize = mIonBuffer->getSize();
            retAddr = mIonBuffer->getVirAddr();
        }
    }

    if (pBufferSize)
        *pBufferSize = retSize;
    pthread_mutex_unlock(&mLock);

    if (retAddr == NULL || retSize == 0)
        ALOGW("[%s] return %p (size=%lld , mState = 0x%x)", __FUNCTION__, retAddr, retSize, mState);

    return retAddr;
}

int GraphicBufferInfo::Client::getFormat(unsigned int * pFormat) {
    if (pFormat == NULL)
        goto err;
    if (isExtraBuffer() || isFreedBuffer())
        goto err;

    *pFormat = mYUVData.format;

    return 0;
err:
    return -1;
}

void GraphicBufferInfo::Client::clearBufferHandle() {
    pthread_mutex_lock(&mReleaseLock);
    pthread_mutex_lock(&mLock);
    if (mService != NULL && mService->clearBufferHandle(this) != 0)
        ALOGE("[%s] service->clearBufferHandle return error! (mService:%p mClient:%p)",
                __FUNCTION__, mService, this);
    else
        mState = BufferFree;

    if (mState == BufferFree) {
        mHandle = NULL;
        if (mViraddr != MAP_FAILED) {
            munmap(mViraddr, mViraddrSize);
            mViraddr = MAP_FAILED;
            mViraddrSize = 0;
        }
        if (mIonBuffer) {
            delete mIonBuffer;
            mIonBuffer = NULL;
        }
        if (mExtBuffer) {
            delete mExtBuffer;
            mExtBuffer = NULL;
        }
        if (mShareBuffer) {
            delete mShareBuffer;
            mShareBuffer = NULL;
        }
        memset(&mYUVData, 0, sizeof(mYUVData));
        memset(&mPrivData, 0, sizeof(mPrivData));
    }
    pthread_mutex_unlock(&mLock);
    pthread_mutex_unlock(&mReleaseLock);
}

void GraphicBufferInfo::Client::dump(android::String8& buf, const char* prefix) {
    if (prefix != NULL)
        buf.appendFormat("%s", prefix);
    buf.appendFormat("GBInfo[state=%x]", mState);
    if (prefix != NULL)
        buf.append("\n");
}


/****************************************************************************************************/

GraphicBufferInfo::GraphicBufferInfo()
    : mLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER),
    mClientLock((pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER)
{
    while (mClientList.size() > 0) {
        Client * client = mClientList.itemAt(0);
        removeClient(client);
#if 1 // debug
        if (client == mClientList.itemAt(0))
            ALOGE("[%s] ERROR! client=%p", __func__, client);
#endif
    }
    hw_get_module(GRALLOC_HARDWARE_MODULE_ID, &mGrallocModule);
}

GraphicBufferInfo::~GraphicBufferInfo() {
}

void GraphicBufferInfo::registerClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (!found) {
        if (client != NULL) {
            mClientList.add(client);
            client->mService = this;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void GraphicBufferInfo::removeClient(Client * client) {
    pthread_mutex_lock(&mClientLock);
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            mClientList.removeAt(i);
            client->mService = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&mClientLock);
}

void GraphicBufferInfo::setBufferHandle(Client * client, buffer_handle_t handle) {
    pthread_mutex_lock(&mClientLock);
    mali_gralloc_private_module_t * privateModule = (mali_gralloc_private_module_t *) mGrallocModule;
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (found) {
        int ret = privateModule->GetYUVData((struct private_module_t const *) privateModule,
                handle,
                (void *) &client->mYUVData);

        if (ret == 0) {
            privateModule->GetPrivData((struct private_module_t const *) privateModule,
                    handle,
                    (void *) &client->mPrivData, sizeof(client->mPrivData));
            client->mHandle = handle;
            client->mState = Client::BufferGralloc;
        } else
            ALOGE("[%s] GetYUVData return ERROR!!! (%d)", __FUNCTION__, ret);
        
    } else
        ALOGE("[%s] Client(%p) is not found!", __FUNCTION__, client);
    pthread_mutex_unlock(&mClientLock);
}

int GraphicBufferInfo::validate(Client * client, buffer_handle_t handle) {
    int ret = -1;
    pthread_mutex_lock(&mClientLock);
    mali_gralloc_private_module_t * privateModule = (mali_gralloc_private_module_t *) mGrallocModule;
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (found) {
        ret = privateModule->Validate((struct private_module_t const *) privateModule, handle);
    } else
        ALOGE("[%s] Client(%p) is not found!", __FUNCTION__, client);
    pthread_mutex_unlock(&mClientLock);
    return ret;
}

void GraphicBufferInfo::setExtBuffer(Client * client, Client::ExtBuffer * buffer) {
    pthread_mutex_lock(&mClientLock);
    mali_gralloc_private_module_t * privateModule = (mali_gralloc_private_module_t *) mGrallocModule;
    bool found = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == client) {
            found = true;
            break;
        }
    }
    if (found) {
        client->mExtBuffer = buffer;
        client->mState = Client::BufferExtra;
    } else
        ALOGE("[%s] Client(%p) is not found!", __FUNCTION__, client);
    pthread_mutex_unlock(&mClientLock);
}

int GraphicBufferInfo::clearBufferHandle(Client * client) {
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
        ret = 0;
        client->mState = Client::BufferFree;
    }
    pthread_mutex_unlock(&mClientLock);
    return ret;
}

int GraphicBufferInfo::replicaCopy(Client * des, Client * src) {
    int ret = -1;
    pthread_mutex_lock(&mClientLock);
    bool found_des = false, found_src = false;
    for (size_t i=0;i<mClientList.size();i++) {
        if (mClientList.itemAt(i) == des)
            found_des = true;
        if (mClientList.itemAt(i) == src)
            found_src = true;
        if (found_des && found_src)
            break;
    }

    if (found_des && found_src) {
        src->lockBuffer();
        des->lockBuffer();
        if (    des->mState == Client::BufferFree   &&
                src->mState != Client::BufferFree   ) {
            if (src->mState & Client::BufferExtra) {
                if (des->mExtBuffer != NULL)
                    delete des->mExtBuffer;
                des->mExtBuffer = src->mExtBuffer->replica();
                des->mState = Client::BufferReplica | Client::BufferExtra;
                ret = 0;
            } else {
                {
                    memcpy((void *) &des->mYUVData, (void *) &src->mYUVData, sizeof(des->mYUVData));
                    memcpy((void *) &des->mPrivData, (void *) &src->mPrivData, sizeof(des->mPrivData));
                    ret = 0;
                }
                if (ret == 0) {
                    long long copySize;
                    void * srcAddr = src->getVirAddr(&copySize);
                    if (srcAddr) {
                        des->mIonBuffer = new IonBuffer(IonBuffer::AllocBuffer, copySize);
                        if (des->mIonBuffer->getVirAddr()) {
                            memcpy(des->mIonBuffer->getVirAddr(), srcAddr, copySize);
                            des->mState = Client::BufferReplica;
                            ret = 0;
                        } else {
                            delete des->mIonBuffer;
                            des->mIonBuffer = NULL;
                            ret = -1;
                            ALOGW("[%s] create IonBuffer failed! (copySize = %lld)", __FUNCTION__, copySize);
                        }
                    } else
                        ret = -1;
                }
            }
        }
        src->unlockBuffer();
        des->unlockBuffer();
    }
    pthread_mutex_unlock(&mClientLock);
    return ret;
}


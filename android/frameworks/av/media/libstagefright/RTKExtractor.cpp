/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "RTKExtractor"
#include <utils/Log.h>

#include "include/RTKExtractor.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <utils/String8.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/Utils.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/FileSource.h>

#include <media/stagefright/RTKFileSource.h>
#include <dlfcn.h>

#include <utils/Errors.h>

//#define DISABLE_RTKEXT 1

/* The extractor lifetime is short - just long enough to get
 * the media sources constructed - so the shared lib needs to remain open
 * beyond the lifetime of the extractor.  So keep the handle as a global
 * rather than a member of the extractor
 */
void *rtkVendorLibHandle = NULL;

namespace android {

RTKFileSource::RTKFileSource(const sp<DataSource> &source)
    : mFd(-1),
      mOffset(0),
      mLength(-1),
      mDecryptHandle(NULL),
      mDrmManagerClient(NULL),
      mDrmBufOffset(0),
      mDrmBufSize(0),
      mDrmBuf(NULL) {
      
    ALOGV("%s:%d  source->RTK_EXT = %d  mFd=%d\n", __func__, __LINE__, source->RTK_EXT, mFd);
}


RTKFileSource::RTKFileSource(const char *filename)
    : mFd(-1),
      mOffset(0),
      mLength(-1),
      mDecryptHandle(NULL),
      mDrmManagerClient(NULL),
      mDrmBufOffset(0),
      mDrmBufSize(0),
      mDrmBuf(NULL) {
    ALOGV("%s:%s:%d filename=%s\n", __FILE__, __func__, __LINE__, filename);

    mFd = open(filename, O_LARGEFILE | O_RDONLY);

    if (mFd >= 0) {
        mLength = lseek64(mFd, 0, SEEK_END);
        RTK_EXT = 1;
    } else {
        ALOGE("Failed to open file '%s'. (%s)", filename, strerror(errno));
    }

}

RTKFileSource::RTKFileSource(int fd, int64_t offset, int64_t length)
    : mFd(fd),
      mOffset(offset),
      mLength(length),
      mDecryptHandle(NULL),
      mDrmManagerClient(NULL),
      mDrmBufOffset(0),
      mDrmBufSize(0),
      mDrmBuf(NULL) {
    CHECK(offset >= 0);
    CHECK(length >= 0);

    //int load_status = -1;

#ifdef DISABLE_RTKEXT
    RTK_EXT = 0;
#else
    RTK_EXT = 1;
#endif
    ALOGV("%s:%d  new mFd = %d,   orig fd=%d  ", __func__, __LINE__, mFd, fd);
}

RTKFileSource::~RTKFileSource() {
    ALOGV("%s:%s:%d  RTK_EXT=0x%x mFd=%d ", __FILE__, __func__, __LINE__, RTK_EXT, mFd);
    if (RTK_EXT)
        RTK_EXT = 0;

    if (mFd >= 0) {
        ::close(mFd);
        mFd = -1;
    }

    if (mDrmBuf != NULL) {
        delete[] mDrmBuf;
        mDrmBuf = NULL;
    }

    if (mDecryptHandle != NULL) {
        // To release mDecryptHandle
        CHECK(mDrmManagerClient);
        mDrmManagerClient->closeDecryptSession(mDecryptHandle);
        mDecryptHandle = NULL;
    }

    if (mDrmManagerClient != NULL) {
        delete mDrmManagerClient;
        mDrmManagerClient = NULL;
    }
}

void RTKFileSource::read_block(void)
{
    //int buftype = -1;
    ALOGV("%s:%s:%d bf m_pSource->ReadFrame() ...\n", __FILE__, __func__, __LINE__);
//    m_pSource->ReadFrame(&buftype);
    ALOGV("%s:%s:%d af m_pSource->ReadFrame() ...\n", __FILE__, __func__, __LINE__);
}

int RTKFileSource::getfd(void)
{
    ALOGV("%s:%s:%d mFd=%d ", __FILE__, __func__, __LINE__, mFd);
    return mFd;
}

status_t RTKFileSource::initCheck() const {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return mFd >= 0 ? OK : NO_INIT;
}

ssize_t RTKFileSource::readAt(off64_t offset, void *data, size_t size) {
//    ALOGD("%s:%s:%d mFd=%d mLength=%d size=%d offset=%d\n", __FILE__, __func__, __LINE__, (int)mFd, (int)mLength, (int)size, (int)offset);
    if (mFd < 0) {
        return NO_INIT;
    }

    Mutex::Autolock autoLock(mLock);

    if (mLength >= 0) {
        if (offset >= mLength) {
            return 0;  // read beyond EOF.
        }
        int64_t numAvailable = mLength - offset;
        if ((int64_t)size > numAvailable) {
            size = numAvailable;
        }
    }

    if (mDecryptHandle != NULL && DecryptApiType::CONTAINER_BASED
            == mDecryptHandle->decryptApiType) {
        return readAtDRM(offset, data, size);
    } else {
        off64_t result = lseek64(mFd, offset + mOffset, SEEK_SET);
        if (result == -1) {
            ALOGE("seek to %lld failed, mFd=%d", (long long)offset + mOffset, mFd);
            return UNKNOWN_ERROR;
        }
        return ::read(mFd, data, size);
    }
}

status_t RTKFileSource::getSize(off64_t *size) {
    //ALOGD("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    Mutex::Autolock autoLock(mLock);

    if (mFd < 0)
        return NO_INIT;

    if (mLength >= 0) {
        *size = mLength;
        return OK;
    }

    *size = lseek64(mFd, 0, SEEK_END);

    return OK;
}

sp<DecryptHandle> RTKFileSource::DrmInitialization(const char *mime) {
    if (mDrmManagerClient == NULL) {
        mDrmManagerClient = new DrmManagerClient();
    }

    if (mDrmManagerClient == NULL) {
        return NULL;
    }

    if (mDecryptHandle == NULL) {
        mDecryptHandle = mDrmManagerClient->openDecryptSession(
                             mFd, mOffset, mLength, mime);
    }

    if (mDecryptHandle == NULL) {
        delete mDrmManagerClient;
        mDrmManagerClient = NULL;
    }

    return mDecryptHandle;
}

void RTKFileSource::getDrmInfo(sp<DecryptHandle> &handle, DrmManagerClient **client) {
    handle = mDecryptHandle;

    *client = mDrmManagerClient;
}

ssize_t RTKFileSource::readAtDRM(off64_t offset, void *data, size_t size) {
    size_t DRM_CACHE_SIZE = 1024;
    if (mDrmBuf == NULL) {
        mDrmBuf = new unsigned char[DRM_CACHE_SIZE];
    }

    if (mDrmBuf != NULL && mDrmBufSize > 0 && (offset + mOffset) >= mDrmBufOffset
            && (long)(offset + mOffset + size) <= (long)(mDrmBufOffset + mDrmBufSize)) {
        /* Use buffered data */
        memcpy(data, (void*)(mDrmBuf+(offset+mOffset-mDrmBufOffset)), size);
        return size;
    } else if (size <= DRM_CACHE_SIZE) {
        /* Buffer new data */
        mDrmBufOffset =  offset + mOffset;
        mDrmBufSize = mDrmManagerClient->pread(mDecryptHandle, mDrmBuf,
                                               DRM_CACHE_SIZE, offset + mOffset);
        if (mDrmBufSize > 0) {
            int64_t dataRead = 0;
            dataRead = (int)size > (int)mDrmBufSize ? mDrmBufSize : size;
            memcpy(data, (void*)mDrmBuf, dataRead);
            return dataRead;
        } else {
            return mDrmBufSize;
        }
    } else {
        /* Too big chunk to cache. Call DRM directly */
        return mDrmManagerClient->pread(mDecryptHandle, data, size, offset + mOffset);
    }
}


static Mutex gRTKutex;

RTKExtractor::RTKExtractor(const sp<DataSource> &source)
    : mDataSource(source)
{
    Mutex::Autolock autoLock(gRTKutex);
    ALOGV("%s:%s:%d   DBG!!!\n", __FILE__, __func__, __LINE__);

    if (!getVendorLibHandle()) {
        return;
    }

    typedef RTKLoadableExtractor *(*GetInstanceFunc)(sp<DataSource>);

    GetInstanceFunc getInstanceFunc =
        (GetInstanceFunc) dlsym(rtkVendorLibHandle,
                            "_ZN7android13RTKExt_CreateENS_2spINS_10DataSourceEEE");

    if (getInstanceFunc) {
        mImpl = (*getInstanceFunc)(source);
        ALOGV("%s:%d dbg 01 af mImpl = static_cast<RTKLoadableExtractor *>(pInst)\n", __func__, __LINE__);

        //CHECK(mImpl != NULL);
        if (mImpl == NULL)
            ALOGW("\033[0;35m#### %d mImpl is NULL ###\033[m", __LINE__);
    } else {
        ALOGE("Failed to locate GetInstance in libRTKExtractor.so");
    }
    ALOGV("%s:%d bf call mImpl->countTracks()\n", __func__, __LINE__);// mImpl->countTracks());

}

static void init_routine()
{
ALOGD("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    rtkVendorLibHandle = dlopen("libRTKExtractor.so", RTLD_NOW);
    
    if (rtkVendorLibHandle == NULL) {
ALOGD("%s:%s:%d af dlopen rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    }

    if (rtkVendorLibHandle == NULL) {
        ALOGE("Failed to open libRTKExtractor.so");
    }
}

bool RTKExtractor::getVendorLibHandle()
{
    static pthread_once_t sOnceControl = PTHREAD_ONCE_INIT;
    pthread_once(&sOnceControl, init_routine);

    return rtkVendorLibHandle != NULL;
}

RTKExtractor::~RTKExtractor() {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
}

size_t RTKExtractor::countTracks() {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return (mImpl != NULL) ? mImpl->countTracks() : 0;
}

void RTKExtractor::FlowStop(void) {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    if (mImpl != NULL)
        mImpl->FlowStop();
}

sp<IMediaSource> RTKExtractor::getTrack(size_t index) {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p index=%d\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle, (int)index);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrack(index);
}

sp<MetaData> RTKExtractor::getTrackMetaData(size_t index, uint32_t flags) {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p index=%d flags=0x%x\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle, (int)index, flags);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrackMetaData(index, flags);
}

sp<MetaData> RTKExtractor::getMetaData() {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getMetaData();
}

size_t  RTKExtractor::approxDataRemaining(status_t *finalStatus) {
    if (mImpl == NULL) {
        return 0;
    }
    return mImpl->approxDataRemaining(finalStatus);
}

int64_t RTKExtractor::getCachedDurationUs(status_t *finalStatus) {
    //ALOGD("%s:%s:%d rtkVendorLibHandle=0x%x\n", __FILE__, __func__, __LINE__, (unsigned int)rtkVendorLibHandle);
    if (mImpl == NULL) {
        return 0;
    }

    return mImpl->getCachedDurationUs(finalStatus);
}

status_t RTKExtractor::getEstimatedBandwidthKbps(int32_t *kbps) {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getEstimatedBandwidthKbps(kbps);
}


void RTKExtractor::setAdaptiveStreamingMode(bool adaptive) {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setAdaptiveStreamingMode(adaptive);
    }
}

void RTKExtractor::setCryptoPluginMode(bool cryptoPluginMode) {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setCryptoPluginMode(cryptoPluginMode);
    }
}

void RTKExtractor::setUID(uid_t uid) {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setUID(uid);
    }
}

status_t RTKExtractor::getError() {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getError();
}

void RTKExtractor::setError(status_t err) {
    ALOGV("%s:%s:%d rtkVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setError(err);
    }
}

bool SniffRTK(
    const sp<DataSource> &source, String8 *mimeType, float *confidence,
    sp<AMessage> *) {

    Mutex::Autolock autoLock(gRTKutex);
    if (!RTKExtractor::getVendorLibHandle()) {
        return false;
    }

    typedef RTKLoadableExtractor *(*SnifferFunc)(const sp<DataSource>&);
    SnifferFunc snifferFunc =
        (SnifferFunc) dlsym(rtkVendorLibHandle,
                                "_ZN7android15RTKExt_IdentifyERKNS_2spINS_10DataSourceEEE");

    if (snifferFunc) {
        if ((*snifferFunc)(source)) 
        {
            *mimeType = MEDIA_MIMETYPE_CONTAINER_RTK;
            *confidence = 10.0f;
            return true;
        }
        else
        {
            ALOGE("Format not support in libRTKExtractor.so");            
            source->RTK_EXT = 0;
        }
    } else {
        ALOGE("Probe function not found in libRTKExtractor.so");
    }

    return false;
}

} //namespace android


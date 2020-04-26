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
#define LOG_TAG "RTKAIFFExtractor"
#include <utils/Log.h>

#include "include/RTKAIFFExtractor.h"
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

#include <dlfcn.h>

#include <utils/Errors.h>

/* The extractor lifetime is short - just long enough to get
 * the media sources constructed - so the shared lib needs to remain open
 * beyond the lifetime of the extractor.  So keep the handle as a global
 * rather than a member of the extractor
 */
void *rtkAIFFVendorLibHandle = NULL;

#define ARRANGE_ENDIAN_32(dat) ( (((dat) & 0xff000000 ) >> 24 ) | (((dat) & 0x00ff0000 ) >> 8 ) | (((dat) & 0x0000ff00 ) << 8 ) | (((dat) & 0x000000ff ) << 24 ) )
/* File formats */
#define AIFF_TYPE_IFF ARRANGE_ENDIAN_32(0x464F524D)
#define AIFF_TYPE_AIFF ARRANGE_ENDIAN_32(0x41494646)
#define AIFF_TYPE_AIFC ARRANGE_ENDIAN_32(0x41494643)
typedef struct s_IFFHeader
{
    uint32_t hid ;
    uint32_t len ;
    uint32_t fid ;
} IFFHeader ;

namespace android {

static Mutex gRTKAIFFutex;

RTKAIFFExtractor::RTKAIFFExtractor(const sp<DataSource> &source)
    : mDataSource(source)
{
    Mutex::Autolock autoLock(gRTKAIFFutex);
    ALOGV("%s:%s:%d   DBG!!!\n", __FILE__, __func__, __LINE__);

    if (!getVendorLibHandle()) {
        return;
    }

    typedef RTKAIFFLoadableExtractor *(*GetInstanceFunc)(sp<DataSource>);

    GetInstanceFunc getInstanceFunc =
        (GetInstanceFunc) dlsym(rtkAIFFVendorLibHandle, "_ZN7android17RTKAIFFExt_CreateENS_2spINS_10DataSourceEEE");

    if (getInstanceFunc) {
        mImpl = (*getInstanceFunc)(source);
        ALOGV("%s:%d dbg 01 af mImpl = static_cast<RTKAIFFLoadableExtractor *>(pInst)\n", __func__, __LINE__);

        if (mImpl == NULL)
            ALOGW("\033[0;35m#### %d mImpl is NULL ###\033[m", __LINE__);
    } else {
        ALOGE("Failed to locate GetInstance in libRTKAIFFExtractor.so");
    }
}

static void init_routine()
{
    ALOGD("%s:%s:%d rtkAIFFVendorLibHandle=%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    rtkAIFFVendorLibHandle = dlopen("libRTKAIFFExtractor.so", RTLD_NOW);

    if (rtkAIFFVendorLibHandle == NULL) {
        ALOGD("%s:%s:%d af dlopen rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    }

    if (rtkAIFFVendorLibHandle == NULL) {
        ALOGE("Failed to open libRTKAIFFExtractor.so");
    }
}

bool RTKAIFFExtractor::getVendorLibHandle()
{
    static pthread_once_t sOnceControl = PTHREAD_ONCE_INIT;
    pthread_once(&sOnceControl, init_routine);

    return rtkAIFFVendorLibHandle != NULL;
}

RTKAIFFExtractor::~RTKAIFFExtractor() {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
}

size_t RTKAIFFExtractor::countTracks() {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return (mImpl != NULL) ? mImpl->countTracks() : 0;
}

void RTKAIFFExtractor::FlowStop(void) {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    if (mImpl != NULL)
        mImpl->FlowStop();
}

sp<IMediaSource> RTKAIFFExtractor::getTrack(size_t index) {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p index=%d\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle, (int)index);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrack(index);
}

sp<MetaData> RTKAIFFExtractor::getTrackMetaData(size_t index, uint32_t flags) {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p index=%d flags=0x%x\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle, (int)index, flags);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrackMetaData(index, flags);
}

sp<MetaData> RTKAIFFExtractor::getMetaData() {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getMetaData();
}

size_t  RTKAIFFExtractor::approxDataRemaining(status_t *finalStatus) {
    if (mImpl == NULL) {
        return 0;
    }
    return mImpl->approxDataRemaining(finalStatus);
}

int64_t RTKAIFFExtractor::getCachedDurationUs(status_t *finalStatus) {
    //ALOGD("%s:%s:%d rtkAIFFVendorLibHandle=0x%x\n", __FILE__, __func__, __LINE__, (unsigned int)rtkAIFFVendorLibHandle);
    if (mImpl == NULL) {
        return 0;
    }

    return mImpl->getCachedDurationUs(finalStatus);
}

status_t RTKAIFFExtractor::getEstimatedBandwidthKbps(int32_t *kbps) {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getEstimatedBandwidthKbps(kbps);
}

void RTKAIFFExtractor::setAdaptiveStreamingMode(bool adaptive) {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setAdaptiveStreamingMode(adaptive);
    }
}

void RTKAIFFExtractor::setCryptoPluginMode(bool cryptoPluginMode) {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setCryptoPluginMode(cryptoPluginMode);
    }
}

void RTKAIFFExtractor::setUID(uid_t uid) {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setUID(uid);
    }
}

status_t RTKAIFFExtractor::getError() {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getError();
}

void RTKAIFFExtractor::setError(status_t err) {
    ALOGV("%s:%s:%d rtkAIFFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkAIFFVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setError(err);
    }
}

bool SniffRTKAIFF(const sp<DataSource> &source, String8 *mimeType, float *confidence, sp<AMessage> *) {
    IFFHeader hdr;

    if (source->readAt(0, (unsigned char*)&hdr, sizeof(IFFHeader)) < (ssize_t)sizeof(IFFHeader)) {
        return false;
    }

    switch (hdr.hid)
    {
        case AIFF_TYPE_IFF:
            /* Continue reading the IFF header */
            ///* Check the format type (AIFF or AIFC)*/
            switch (hdr.fid)
            {
                case AIFF_TYPE_AIFF:
                    break;
                case AIFF_TYPE_AIFC:
                    break;
                default:
                    return false;
            }
            break;
        default:
            return false;
    }
    mimeType->setTo(MEDIA_MIMETYPE_CONTAINER_AIFF);
    *confidence = 0.68;

    ALOGD("\033[0;36m [%d]  set MEDIA_MIMETYPE_CONTAINER_AIFF    return true\033[m", __LINE__);
    return true;
}

} //namespace android


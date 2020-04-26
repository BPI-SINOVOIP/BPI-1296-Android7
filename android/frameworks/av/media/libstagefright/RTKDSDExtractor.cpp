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
#define LOG_TAG "RTKDSDExtractor"
#include <utils/Log.h>

#include "include/RTKDSDExtractor.h"
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
void *rtkDSDVendorLibHandle = NULL;

namespace android {

static Mutex gRTKDSDutex;

RTKDSDExtractor::RTKDSDExtractor(const sp<DataSource> &source)
    : mDataSource(source)
{
    Mutex::Autolock autoLock(gRTKDSDutex);
    ALOGV("%s:%s:%d   DBG!!!\n", __FILE__, __func__, __LINE__);

    if (!getVendorLibHandle()) {
        return;
    }

    typedef RTKDSDLoadableExtractor *(*GetInstanceFunc)(sp<DataSource>);

    GetInstanceFunc getInstanceFunc =
        (GetInstanceFunc) dlsym(rtkDSDVendorLibHandle, "_ZN7android16RTKDSDExt_CreateENS_2spINS_10DataSourceEEE");

    if (getInstanceFunc) {
        mImpl = (*getInstanceFunc)(source);
        ALOGV("%s:%d dbg 01 af mImpl = static_cast<RTKDSDLoadableExtractor *>(pInst)\n", __func__, __LINE__);

        if (mImpl == NULL)
            ALOGW("\033[0;35m#### %d mImpl is NULL ###\033[m", __LINE__);
    } else {
        ALOGE("Failed to locate GetInstance in libRTKDSDExtractor.so");
    }
}

static void init_routine()
{
    ALOGD("%s:%s:%d rtkDSDVendorLibHandle=%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    rtkDSDVendorLibHandle = dlopen("libRTKDSDExtractor.so", RTLD_NOW);

    if (rtkDSDVendorLibHandle == NULL) {
        ALOGD("%s:%s:%d af dlopen rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    }

    if (rtkDSDVendorLibHandle == NULL) {
        ALOGE("Failed to open libRTKDSDExtractor.so");
    }
}

bool RTKDSDExtractor::getVendorLibHandle()
{
    static pthread_once_t sOnceControl = PTHREAD_ONCE_INIT;
    pthread_once(&sOnceControl, init_routine);

    return rtkDSDVendorLibHandle != NULL;
}

RTKDSDExtractor::~RTKDSDExtractor() {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
}

size_t RTKDSDExtractor::countTracks() {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return (mImpl != NULL) ? mImpl->countTracks() : 0;
}

void RTKDSDExtractor::FlowStop(void) {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    if (mImpl != NULL)
        mImpl->FlowStop();
}

sp<IMediaSource> RTKDSDExtractor::getTrack(size_t index) {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p index=%d\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle, (int)index);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrack(index);
}

sp<MetaData> RTKDSDExtractor::getTrackMetaData(size_t index, uint32_t flags) {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p index=%d flags=0x%x\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle, (int)index, flags);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrackMetaData(index, flags);
}

sp<MetaData> RTKDSDExtractor::getMetaData() {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getMetaData();
}

size_t  RTKDSDExtractor::approxDataRemaining(status_t *finalStatus) {
    if (mImpl == NULL) {
        return 0;
    }
    return mImpl->approxDataRemaining(finalStatus);
}

int64_t RTKDSDExtractor::getCachedDurationUs(status_t *finalStatus) {
    //ALOGD("%s:%s:%d rtkDSDVendorLibHandle=0x%x\n", __FILE__, __func__, __LINE__, (unsigned int)rtkDSDVendorLibHandle);
    if (mImpl == NULL) {
        return 0;
    }

    return mImpl->getCachedDurationUs(finalStatus);
}

status_t RTKDSDExtractor::getEstimatedBandwidthKbps(int32_t *kbps) {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getEstimatedBandwidthKbps(kbps);
}

void RTKDSDExtractor::setAdaptiveStreamingMode(bool adaptive) {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setAdaptiveStreamingMode(adaptive);
    }
}

void RTKDSDExtractor::setCryptoPluginMode(bool cryptoPluginMode) {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setCryptoPluginMode(cryptoPluginMode);
    }
}

void RTKDSDExtractor::setUID(uid_t uid) {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setUID(uid);
    }
}

status_t RTKDSDExtractor::getError() {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getError();
}

void RTKDSDExtractor::setError(status_t err) {
    ALOGV("%s:%s:%d rtkDSDVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkDSDVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setError(err);
    }
}

bool SniffRTKDSD(const sp<DataSource> &source, String8 *mimeType, float *confidence, sp<AMessage> *) {
    char tmp[8] = {0};

    if (source->readAt(0, tmp, 8) < 8) {
        return false;
    }
    //ALOGD("\033[0;36m [%d]  tmp[0 ~ 7] = 0x%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X \033[m", __LINE__, tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7]);
    if (!(tmp[0] == 0x44 && tmp[1] == 0x53 && tmp[2] == 0x44 && tmp[3] == 0x20))
        return false;

    mimeType->setTo(MEDIA_MIMETYPE_AUDIO_DSD);
    *confidence = 0.68;

    //ALOGD("\033[0;36m [%d]  set MEDIA_MIMETYPE_AUDIO_DSD    return true\033[m", __LINE__);
    return true;
}

} //namespace android


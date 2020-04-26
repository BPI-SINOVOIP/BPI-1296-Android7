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
#define LOG_TAG "RTKASFExtractor"
#include <utils/Log.h>

#include "include/RTKASFExtractor.h"
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
void *rtkASFVendorLibHandle = NULL;

namespace android {

static Mutex gRTKASFutex;

RTKASFExtractor::RTKASFExtractor(const sp<DataSource> &source)
    : mDataSource(source)
{
    Mutex::Autolock autoLock(gRTKASFutex);
    ALOGV("%s:%s:%d   DBG!!!\n", __FILE__, __func__, __LINE__);

    if (!getVendorLibHandle()) {
        return;
    }

    typedef RTKASFLoadableExtractor *(*GetInstanceFunc)(sp<DataSource>);

    GetInstanceFunc getInstanceFunc =
        (GetInstanceFunc) dlsym(rtkASFVendorLibHandle, "_ZN7android16RTKASFExt_CreateENS_2spINS_10DataSourceEEE");

    if (getInstanceFunc) {
        mImpl = (*getInstanceFunc)(source);
        ALOGV("%s:%d dbg 01 af mImpl = static_cast<RTKASFLoadableExtractor *>(pInst)\n", __func__, __LINE__);

        if (mImpl == NULL)
            ALOGW("\033[0;35m#### %d mImpl is NULL ###\033[m", __LINE__);
    } else {
        ALOGE("Failed to locate GetInstance in libRTKASFExtractor.so");
    }
}

static void init_routine()
{
    ALOGD("%s:%s:%d rtkASFVendorLibHandle=%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    rtkASFVendorLibHandle = dlopen("libRTKASFExtractor.so", RTLD_NOW);

    if (rtkASFVendorLibHandle == NULL) {
        ALOGD("%s:%s:%d af dlopen rtkASFVendorLibHandle=%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    }

    if (rtkASFVendorLibHandle == NULL) {
        ALOGE("Failed to open libRTKASFExtractor.so");
    }
}

bool RTKASFExtractor::getVendorLibHandle()
{
    static pthread_once_t sOnceControl = PTHREAD_ONCE_INIT;
    pthread_once(&sOnceControl, init_routine);

    return rtkASFVendorLibHandle != NULL;
}

RTKASFExtractor::~RTKASFExtractor() {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
}

size_t RTKASFExtractor::countTracks() {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return (mImpl != NULL) ? mImpl->countTracks() : 0;
}

void RTKASFExtractor::FlowStop(void) {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    if (mImpl != NULL)
        mImpl->FlowStop();
}

sp<IMediaSource> RTKASFExtractor::getTrack(size_t index) {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p index=%d\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle, (int)index);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrack(index);
}

sp<MetaData> RTKASFExtractor::getTrackMetaData(size_t index, uint32_t flags) {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p index=%d flags=0x%x\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle, (int)index, flags);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrackMetaData(index, flags);
}

sp<MetaData> RTKASFExtractor::getMetaData() {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getMetaData();
}

size_t  RTKASFExtractor::approxDataRemaining(status_t *finalStatus) {
    if (mImpl == NULL) {
        return 0;
    }
    return mImpl->approxDataRemaining(finalStatus);
}

int64_t RTKASFExtractor::getCachedDurationUs(status_t *finalStatus) {
    //ALOGD("%s:%s:%d rtkASFVendorLibHandle=0x%x\n", __FILE__, __func__, __LINE__, (unsigned int)rtkASFVendorLibHandle);
    if (mImpl == NULL) {
        return 0;
    }

    return mImpl->getCachedDurationUs(finalStatus);
}

status_t RTKASFExtractor::getEstimatedBandwidthKbps(int32_t *kbps) {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getEstimatedBandwidthKbps(kbps);
}

void RTKASFExtractor::setAdaptiveStreamingMode(bool adaptive) {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setAdaptiveStreamingMode(adaptive);
    }
}

void RTKASFExtractor::setCryptoPluginMode(bool cryptoPluginMode) {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setCryptoPluginMode(cryptoPluginMode);
    }
}

void RTKASFExtractor::setUID(uid_t uid) {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setUID(uid);
    }
}

status_t RTKASFExtractor::getError() {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getError();
}

void RTKASFExtractor::setError(status_t err) {
    ALOGV("%s:%s:%d rtkASFVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkASFVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setError(err);
    }
}

bool SniffRTKASF(const sp<DataSource> &source, String8 *mimeType, float *confidence, sp<AMessage> *) {
    char tmp[16];
    char asf_header_object[16] = {0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C};

    if (source->readAt(0, tmp, 16) < 16) {
        return false;
    }

    if (!memcmp((void *)&tmp[0], (void *)&asf_header_object[0], 16)) {
        mimeType->setTo(MEDIA_MIMETYPE_CONTAINER_WMV);
        *confidence = 0.68;

        //ALOGD("\033[0;36m [%d]  set MEDIA_MIMETYPE_CONTAINER_WMV    return true\033[m", __LINE__);
        return true;
    }

    return false;
}

} //namespace android


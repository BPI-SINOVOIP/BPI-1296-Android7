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
#define LOG_TAG "RTKFLVExtractor"
#include <utils/Log.h>

#include "include/RTKFLVExtractor.h"
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
void *rtkFLVVendorLibHandle = NULL;

namespace android {

static Mutex gRTKFLVutex;

RTKFLVExtractor::RTKFLVExtractor(const sp<DataSource> &source)
    : mDataSource(source)
{
    Mutex::Autolock autoLock(gRTKFLVutex);
    ALOGV("%s:%s:%d   DBG!!!\n", __FILE__, __func__, __LINE__);

    if (!getVendorLibHandle()) {
        return;
    }

    typedef RTKFLVLoadableExtractor *(*GetInstanceFunc)(sp<DataSource>);

    GetInstanceFunc getInstanceFunc =
        (GetInstanceFunc) dlsym(rtkFLVVendorLibHandle, "_ZN7android16RTKFLVExt_CreateENS_2spINS_10DataSourceEEE");

    if (getInstanceFunc) {
        mImpl = (*getInstanceFunc)(source);
        ALOGV("%s:%d dbg 01 af mImpl = static_cast<RTKFLVLoadableExtractor *>(pInst)\n", __func__, __LINE__);

        if (mImpl == NULL)
            ALOGW("\033[0;35m#### %d mImpl is NULL ###\033[m", __LINE__);
    } else {
        ALOGE("Failed to locate GetInstance in libRTKFLVExtractor.so");
    }
}

static void init_routine()
{
    ALOGD("%s:%s:%d rtkFLVVendorLibHandle=%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    rtkFLVVendorLibHandle = dlopen("libRTKFLVExtractor.so", RTLD_NOW);

    if (rtkFLVVendorLibHandle == NULL) {
        ALOGD("%s:%s:%d af dlopen rtkFLVVendorLibHandle=%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    }

    if (rtkFLVVendorLibHandle == NULL) {
        ALOGE("Failed to open libRTKFLVExtractor.so");
    }
}

bool RTKFLVExtractor::getVendorLibHandle()
{
    static pthread_once_t sOnceControl = PTHREAD_ONCE_INIT;
    pthread_once(&sOnceControl, init_routine);

    return rtkFLVVendorLibHandle != NULL;
}

RTKFLVExtractor::~RTKFLVExtractor() {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
}

size_t RTKFLVExtractor::countTracks() {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return (mImpl != NULL) ? mImpl->countTracks() : 0;
}

void RTKFLVExtractor::FlowStop(void) {
    ALOGV("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    if (mImpl != NULL)
        mImpl->FlowStop();
}

sp<IMediaSource> RTKFLVExtractor::getTrack(size_t index) {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p index=%d\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle, (int)index);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrack(index);
}

sp<MetaData> RTKFLVExtractor::getTrackMetaData(size_t index, uint32_t flags) {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p index=%d flags=0x%x\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle, (int)index, flags);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrackMetaData(index, flags);
}

sp<MetaData> RTKFLVExtractor::getMetaData() {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getMetaData();
}

size_t  RTKFLVExtractor::approxDataRemaining(status_t *finalStatus) {
    if (mImpl == NULL) {
        return 0;
    }
    return mImpl->approxDataRemaining(finalStatus);
}

int64_t RTKFLVExtractor::getCachedDurationUs(status_t *finalStatus) {
    //ALOGD("%s:%s:%d rtkFLVVendorLibHandle=0x%x\n", __FILE__, __func__, __LINE__, (unsigned int)rtkFLVVendorLibHandle);
    if (mImpl == NULL) {
        return 0;
    }

    return mImpl->getCachedDurationUs(finalStatus);
}

status_t RTKFLVExtractor::getEstimatedBandwidthKbps(int32_t *kbps) {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getEstimatedBandwidthKbps(kbps);
}

void RTKFLVExtractor::setAdaptiveStreamingMode(bool adaptive) {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setAdaptiveStreamingMode(adaptive);
    }
}

void RTKFLVExtractor::setCryptoPluginMode(bool cryptoPluginMode) {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setCryptoPluginMode(cryptoPluginMode);
    }
}

void RTKFLVExtractor::setUID(uid_t uid) {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setUID(uid);
    }
}

status_t RTKFLVExtractor::getError() {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getError();
}

void RTKFLVExtractor::setError(status_t err) {
    ALOGV("%s:%s:%d rtkFLVVendorLibHandle=0x%p\n", __FILE__, __func__, __LINE__, rtkFLVVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setError(err);
    }
}

bool SniffRTKFLV(const sp<DataSource> &source, String8 *mimeType, float *confidence, sp<AMessage> *) {
    char tmp[16];
    char flv_header[13] = {0x46, 0x4C, 0x56, 0x01, 0x05, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00};
    int i = 0;

    if (source->readAt(0, tmp, 16) < 16) {
        return false;
    }

    for (i = 0; i < 13; i++)
    {
        if (i == 4)
        {
            // tmp[4] = flags==0 ? AUDIO_FLAG | VIDEO_FLAG : flags;  AUDIO_FLAG:int=4;  VIDEO_FLAG:int=1;
            if (!((tmp[i] == 0) || (flv_header[i] & tmp[i])))
            {
                //ALOGD("\033[0;36m [%d] Not FLV format, tmp[%d]= 0x%.2X\033[m", __LINE__, i, tmp[i]);
                return false;
            }
        }
        else
        {
            if (tmp[i] != flv_header[i])
            {
                //ALOGD("\033[0;36m [%d] Not FLV format, tmp[%d]= 0x%.2X   flv_header[%d]= 0x%.2X\033[m", __LINE__, i, tmp[i], i, flv_header[i]);
                return false;
            }
        }
    }
    mimeType->setTo(MEDIA_MIMETYPE_CONTAINER_FLV);
    *confidence = 0.88;

    //ALOGD("\033[0;36m [%d]  set MEDIA_MIMETYPE_CONTAINER_FLV    return true\033[m", __LINE__);
    return true;
}

} //namespace android


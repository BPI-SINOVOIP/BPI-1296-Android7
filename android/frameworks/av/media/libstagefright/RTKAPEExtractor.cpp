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
#define LOG_TAG "RTKAPEExtractor"
#include <utils/Log.h>

#include "include/RTKAPEExtractor.h"
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
void *rtkAPEVendorLibHandle = NULL;

namespace android {

static Mutex gRTKAPEutex;

static bool Resync(const sp<DataSource> &source, int *post_id3_pos) {

    #define ANALYSIS_BUF_LEN	(1024*10)//	8KB
    unsigned char analysisBuf[ANALYSIS_BUF_LEN];
    if (source->readAt(0, analysisBuf, ANALYSIS_BUF_LEN) < ANALYSIS_BUF_LEN) {
        ALOGV("%s:%d Readsize is less than ANALYSIS_BUF_LEN:%d ", __func__, __LINE__, ANALYSIS_BUF_LEN);
        return false;
    }

    // here we try to skip ID3v2 Tag.
    int nJunkBytes=0;
    unsigned char cID3v2Header[10];
    memcpy(cID3v2Header,&(analysisBuf[0]),10);

    // calculate bytes of ID3v2 tag:
    if(cID3v2Header[0]=='I'&&cID3v2Header[1]=='D'&&cID3v2Header[2]=='3')
    {
        unsigned int nSyncSafeLength = 0;
        nSyncSafeLength = (cID3v2Header[6] & 127) << 21;
        nSyncSafeLength += (cID3v2Header[7] & 127) << 14;
        nSyncSafeLength += (cID3v2Header[8] & 127) << 7;
        nSyncSafeLength += (cID3v2Header[9] & 127);

        if (cID3v2Header[5] & 16)
        {
            nJunkBytes = nSyncSafeLength + 20;
        }
        else
        {
            nJunkBytes = nSyncSafeLength + 10;
        }
    }
    ALOGV("%s:%d nJunkBytes:%d ", __func__, __LINE__, nJunkBytes);
    *post_id3_pos = nJunkBytes;
    return true;
}

RTKAPEExtractor::RTKAPEExtractor(const sp<DataSource> &source)
    : mDataSource(source)
{
    Mutex::Autolock autoLock(gRTKAPEutex);
    ALOGV("%s:%d DBG!!!", __func__, __LINE__);

    if (!getVendorLibHandle()) {
        return;
    }

    typedef RTKAPELoadableExtractor *(*GetInstanceFunc)(sp<DataSource>);

    GetInstanceFunc getInstanceFunc =
        (GetInstanceFunc) dlsym(rtkAPEVendorLibHandle, "_ZN7android16RTKAPEExt_CreateENS_2spINS_10DataSourceEEE");

    if (getInstanceFunc) {
        mImpl = (*getInstanceFunc)(source);
        ALOGV("%s:%d dbg 01 af mImpl = static_cast<RTKAPELoadableExtractor *>(pInst)", __func__, __LINE__);

        if (mImpl == NULL)
            ALOGW("\033[0;35m#### %d mImpl is NULL ###\033[m", __LINE__);
    } else {
        ALOGE("Failed to locate GetInstance in libRTKAPEExtractor.so");
    }
}

static void init_routine()
{
    ALOGD("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    rtkAPEVendorLibHandle = dlopen("libRTKAPEExtractor.so", RTLD_NOW);

    if (rtkAPEVendorLibHandle == NULL) {
        ALOGD("%s:%d af dlopen rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    }

    if (rtkAPEVendorLibHandle == NULL) {
        ALOGE("Failed to open libRTKAPEExtractor.so");
    }
}

bool RTKAPEExtractor::getVendorLibHandle()
{
    static pthread_once_t sOnceControl = PTHREAD_ONCE_INIT;
    pthread_once(&sOnceControl, init_routine);

    return rtkAPEVendorLibHandle != NULL;
}

RTKAPEExtractor::~RTKAPEExtractor() {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
}

size_t RTKAPEExtractor::countTracks() {
    ALOGV("%s:%d", __func__, __LINE__);
    return (mImpl != NULL) ? mImpl->countTracks() : 0;
}

void RTKAPEExtractor::FlowStop(void) {
    ALOGV("%s:%d", __func__, __LINE__);
    if (mImpl != NULL)
        mImpl->FlowStop();
}

sp<IMediaSource> RTKAPEExtractor::getTrack(size_t index) {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p index=%d", __func__, __LINE__, rtkAPEVendorLibHandle, (int)index);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrack(index);
}

sp<MetaData> RTKAPEExtractor::getTrackMetaData(size_t index, uint32_t flags) {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p index=%d flags=0x%x", __func__, __LINE__, rtkAPEVendorLibHandle, (int)index, flags);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getTrackMetaData(index, flags);
}

sp<MetaData> RTKAPEExtractor::getMetaData() {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    if (mImpl == NULL) {
        return NULL;
    }
    return mImpl->getMetaData();
}

size_t  RTKAPEExtractor::approxDataRemaining(status_t *finalStatus) {
    if (mImpl == NULL) {
        return 0;
    }
    return mImpl->approxDataRemaining(finalStatus);
}

int64_t RTKAPEExtractor::getCachedDurationUs(status_t *finalStatus) {
    //ALOGD("%s:%d rtkAPEVendorLibHandle=0x%x", __func__, __LINE__, (unsigned int)rtkAPEVendorLibHandle);
    if (mImpl == NULL) {
        return 0;
    }

    return mImpl->getCachedDurationUs(finalStatus);
}

status_t RTKAPEExtractor::getEstimatedBandwidthKbps(int32_t *kbps) {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getEstimatedBandwidthKbps(kbps);
}

void RTKAPEExtractor::setAdaptiveStreamingMode(bool adaptive) {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setAdaptiveStreamingMode(adaptive);
    }
}

void RTKAPEExtractor::setCryptoPluginMode(bool cryptoPluginMode) {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setCryptoPluginMode(cryptoPluginMode);
    }
}

void RTKAPEExtractor::setUID(uid_t uid) {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setUID(uid);
    }
}

status_t RTKAPEExtractor::getError() {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    if (mImpl == NULL) {
        return UNKNOWN_ERROR;
    }

    return mImpl->getError();
}

void RTKAPEExtractor::setError(status_t err) {
    ALOGV("%s:%d rtkAPEVendorLibHandle=%p", __func__, __LINE__, rtkAPEVendorLibHandle);
    if (mImpl != NULL) {
        mImpl->setError(err);
    }
}

bool SniffRTKAPE(const sp<DataSource> &source, String8 *mimeType, float *confidence, sp<AMessage> *) {
    int nJunkBytes = 0;
    Resync(source, &nJunkBytes);

    char tmp[4] = {0};
    if (source->readAt(nJunkBytes, tmp, 4) < 4) {
        return false;
    }
    ALOGV("\033[0;36m[%d]  tmp[0 ~ 3] = 0x%.2X %.2X %.2X %.2X\033[m", __LINE__, tmp[0], tmp[1], tmp[2], tmp[3]);
    if (!(tmp[0] == 0x4D && tmp[1] == 0x41 && tmp[2] == 0x43 && tmp[3] == 0x20))
        return false;

    mimeType->setTo(MEDIA_MIMETYPE_AUDIO_APE);
    *confidence = 0.68;

    //ALOGD("\033[0;36m [%d]  set MEDIA_MIMETYPE_AUDIO_APE    return true\033[m", __LINE__);
    return true;
}

} //namespace android


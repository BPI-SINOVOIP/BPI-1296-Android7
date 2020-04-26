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
#define LOG_TAG "RTKHEIFExtractor"
#include <utils/Log.h>

#include "include/RTKHEIFExtractor.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>

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
void *rtkHEIFVendorLibHandle = NULL;

namespace android
{

static Mutex gRTKHEIFutex;

RTKHEIFExtractor::RTKHEIFExtractor(const sp<DataSource> &source)
    : mDataSource(source)
{
    Mutex::Autolock autoLock(gRTKHEIFutex);
    ALOGV("%s:%d DBG!!!", __func__, __LINE__);

    if (!getVendorLibHandle())
    {
        return;
    }

    typedef RTKHEIFLoadableExtractor *(*GetInstanceFunc)(sp<DataSource>);

    GetInstanceFunc getInstanceFunc =
        (GetInstanceFunc) dlsym(rtkHEIFVendorLibHandle, "_ZN7android17RTKHEIFExt_CreateENS_2spINS_10DataSourceEEE");

    if (getInstanceFunc)
    {
        mImpl = (*getInstanceFunc)(source);
        ALOGV("%s:%d dbg 01 af mImpl = static_cast<RTKHEIFLoadableExtractor *>(pInst)", __func__, __LINE__);

        if (mImpl == NULL)
            ALOGW("\033[0;35m#### %d mImpl is NULL ###\033[m", __LINE__);
    }
    else
    {
        ALOGE("Failed to locate GetInstance in libRTKHEIFExtractor.so");
    }
}

static void init_routine()
{
    ALOGD("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    rtkHEIFVendorLibHandle = dlopen("libRTKHEIFExtractor.so", RTLD_NOW);

    if (rtkHEIFVendorLibHandle == NULL)
    {
        ALOGD("%s:%d af dlopen rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    }

    if (rtkHEIFVendorLibHandle == NULL)
    {
        ALOGE("Failed to open libRTKHEIFExtractor.so");
    }
}

bool RTKHEIFExtractor::getVendorLibHandle()
{
    static pthread_once_t sOnceControl = PTHREAD_ONCE_INIT;
    pthread_once(&sOnceControl, init_routine);

    return rtkHEIFVendorLibHandle != NULL;
}

RTKHEIFExtractor::~RTKHEIFExtractor()
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
}

size_t RTKHEIFExtractor::countTracks()
{
    ALOGV("%s:%d", __func__, __LINE__);
    return (mImpl != NULL) ? mImpl->countTracks() : 0;
}

void RTKHEIFExtractor::FlowStop(void)
{
    ALOGV("%s:%d", __func__, __LINE__);
    if (mImpl != NULL)
        mImpl->FlowStop();
}

sp<IMediaSource> RTKHEIFExtractor::getTrack(size_t index)
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p index=%d", __func__, __LINE__, rtkHEIFVendorLibHandle, (int)index);
    if (mImpl == NULL)
    {
        return NULL;
    }
    return mImpl->getTrack(index);
}

sp<MetaData> RTKHEIFExtractor::getTrackMetaData(size_t index, uint32_t flags)
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p index=%d flags=0x%x", __func__, __LINE__, rtkHEIFVendorLibHandle, (int)index, flags);
    if (mImpl == NULL)
    {
        return NULL;
    }
    return mImpl->getTrackMetaData(index, flags);
}

sp<MetaData> RTKHEIFExtractor::getMetaData()
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    if (mImpl == NULL)
    {
        return NULL;
    }
    return mImpl->getMetaData();
}

size_t  RTKHEIFExtractor::approxDataRemaining(status_t *finalStatus)
{
    if (mImpl == NULL)
    {
        return 0;
    }
    return mImpl->approxDataRemaining(finalStatus);
}

int64_t RTKHEIFExtractor::getCachedDurationUs(status_t *finalStatus)
{
    //ALOGD("%s:%d rtkHEIFVendorLibHandle=0x%x", __func__, __LINE__, (unsigned int)rtkHEIFVendorLibHandle);
    if (mImpl == NULL)
    {
        return 0;
    }

    return mImpl->getCachedDurationUs(finalStatus);
}

status_t RTKHEIFExtractor::getEstimatedBandwidthKbps(int32_t *kbps)
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    if (mImpl == NULL)
    {
        return UNKNOWN_ERROR;
    }

    return mImpl->getEstimatedBandwidthKbps(kbps);
}

void RTKHEIFExtractor::setAdaptiveStreamingMode(bool adaptive)
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    if (mImpl != NULL)
    {
        mImpl->setAdaptiveStreamingMode(adaptive);
    }
}

void RTKHEIFExtractor::setCryptoPluginMode(bool cryptoPluginMode)
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    if (mImpl != NULL)
    {
        mImpl->setCryptoPluginMode(cryptoPluginMode);
    }
}

void RTKHEIFExtractor::setUID(uid_t uid)
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    if (mImpl != NULL)
    {
        mImpl->setUID(uid);
    }
}

status_t RTKHEIFExtractor::getError()
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    if (mImpl == NULL)
    {
        return UNKNOWN_ERROR;
    }

    return mImpl->getError();
}

void RTKHEIFExtractor::setError(status_t err)
{
    ALOGV("%s:%d rtkHEIFVendorLibHandle=%p", __func__, __LINE__, rtkHEIFVendorLibHandle);
    if (mImpl != NULL)
    {
        mImpl->setError(err);
    }
}

static void MakeFourCCString(uint32_t x, char *s)
{
    s[0] = x >> 24;
    s[1] = (x >> 16) & 0xff;
    s[2] = (x >> 8) & 0xff;
    s[3] = x & 0xff;
    s[4] = '\0';
}

static bool isCompatibleBrand(uint32_t fourcc)
{
    static const uint32_t kCompatibleBrands[] =
    {
        //image
        FOURCC('h', 'e', 'i', 'c'),
        FOURCC('h', 'e', 'i', 'x'),
        FOURCC('m', 'i', 'f', '1'),
    };

    for (size_t i = 0; i < sizeof(kCompatibleBrands) / sizeof(kCompatibleBrands[0]); ++i)
    {
        if (kCompatibleBrands[i] == fourcc)
        {
            char brand_char[5];
            MakeFourCCString(fourcc, brand_char);
            ALOGV("CompatibleBrand: \"%s\"", brand_char);
            return true;
        }
    }
    return false;
}

static bool isSequenceBrand(uint32_t fourcc)
{
    static const uint32_t kCompatibleBrands[] =
    {
        //sequence
        FOURCC('m', 's', 'f', '1'),
    };

    for (size_t i = 0; i < sizeof(kCompatibleBrands) / sizeof(kCompatibleBrands[0]); ++i)
    {
        if (kCompatibleBrands[i] == fourcc)
        {
            char brand_char[5];
            MakeFourCCString(fourcc, brand_char);
            ALOGV("SequenceBrand: \"%s\"", brand_char);
            return true;
        }
    }
    return false;
}

// Attempt to actually parse the 'ftyp' atom and determine if a suitable
// compatible brand is present.
// Also try to identify where this file's metadata ends
// (end of the 'moov' atom) and report it to the caller as part of
// the metadata.
static bool BetterSniffHEIF(
    const sp<DataSource> &source, String8 *mimeType, float *confidence)
{
    // We scan up to 128 bytes to identify this file as an MP4.
    off64_t kMaxScanOffset = 128ll;

    off64_t offset = 0ll;
    bool foundGoodFileType = false;
    off64_t moovAtomEndOffset = -1ll;
    bool done = false;

    if (source->getSize((off64_t *)&kMaxScanOffset) != OK)
    {
        kMaxScanOffset = 128;
    }

    while (!done && offset < kMaxScanOffset)
    {
        uint32_t chunk[2];
        if (source->readAt(offset, chunk, 8) < 8)
        {
            return false;
        }

        uint64_t chunkSize = ntohl(chunk[0]);
        uint32_t chunkType = ntohl(chunk[1]);
        off64_t chunkDataOffset = offset + 8;

        if (chunkSize == 1)
        {
            if (source->readAt(offset + 8, &chunkSize, 8) < 8)
            {
                return false;
            }

            chunkSize = ntoh64(chunkSize);
            chunkDataOffset += 8;

            if (chunkSize < 16)
            {
                // The smallest valid chunk is 16 bytes long in this case.
                return false;
            }

        }
        else if (chunkSize < 8)
        {
            // The smallest valid chunk is 8 bytes long.
            return false;
        }

        // (data_offset - offset) is either 8 or 16
        off64_t chunkDataSize = chunkSize - (chunkDataOffset - offset);
        if (chunkDataSize < 0)
        {
            ALOGE("b/23540914");
            return ERROR_MALFORMED;
        }

        char chunkstring[5];
        MakeFourCCString(chunkType, chunkstring);
        ALOGV("saw chunk type %s, size %" PRIu64 " @ %lld", chunkstring, chunkSize, (long long)offset);
        switch (chunkType)
        {
            case FOURCC('f', 't', 'y', 'p'):
            {
                if (chunkDataSize < 8)
                {
                    return false;
                }

                uint32_t numCompatibleBrands = (chunkDataSize - 8) / 4;
                for (size_t i = 0; i < numCompatibleBrands + 2; ++i)
                {
                    if (i == 1)
                    {
                        // Skip this index, it refers to the minorVersion,
                        // not a brand.
                        continue;
                    }

                    uint32_t brand;
                    if (source->readAt(
                                chunkDataOffset + 4 * i, &brand, 4) < 4)
                    {
                        return false;
                    }

                    brand = ntohl(brand);
                    if (isSequenceBrand(brand))
                    {
                        foundGoodFileType = false;
                        break;
                    }

                    if (isCompatibleBrand(brand))
                    {
                        foundGoodFileType = true;
//                        break;
                    }
                }

                if (!foundGoodFileType)
                {
                    return false;
                }

                break;
            }

            case FOURCC('m', 'o', 'o', 'v'):
            {
                moovAtomEndOffset = offset + chunkSize;

                done = true;
                break;
            }

            default:
                break;
        }

        offset += chunkSize;
    }
    if (!foundGoodFileType)
    {
        return false;
    }

    *mimeType = MEDIA_MIMETYPE_CONTAINER_HEIF;
    *confidence = 0.6f;
    return true;
}

bool SniffRTKHEIF(const sp<DataSource> &source, String8 *mimeType, float *confidence, sp<AMessage> *)
{
    if (BetterSniffHEIF(source, mimeType, confidence))
    {
        return true;
    }
    return false;
}

} //namespace android


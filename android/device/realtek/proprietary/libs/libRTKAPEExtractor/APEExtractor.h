/*
 * Copyright (C) 2009 The Android Open Source Project
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

#ifndef APE_EXTRACTOR_H_

#define APE_EXTRACTOR_H_

#include <utils/Errors.h>
#include <media/IMediaSource.h>
#include <media/stagefright/MediaExtractor.h>

// a light-weight class to parse APE header:
#include "APEHeaderManager.h"

namespace android {

struct AMessage;
class DataSource;
class String8;

typedef struct _APEFORMATEX {
    uint16_t numberOfChannels;
    uint16_t bitsPerSample;
    double samplingRate;
} APEFORMATEX;

typedef struct _APEFILEINFO {
    long long dataStart;
    long long dataSize;
    float ratioValue;
} APEFILEINFO;

class APEExtractor : public MediaExtractor {
public:
    // Extractor assumes ownership of "source".
    APEExtractor(const sp<DataSource> &source);

    virtual size_t countTracks();
    virtual sp<IMediaSource> getTrack(size_t index);
    virtual sp<MetaData> getTrackMetaData(size_t index, uint32_t flags);
    virtual sp<MetaData> getMetaData();
    APEHeaderManager *pHeaderManager;

private:
    /* start of ape used*/
    #define APE_IO_SIZE        (18*1024) // 36K bytes
    #define INSANE_COMPRESS_LEVEL  5000

    APEFORMATEX mAudioFormat;
    APEFILEINFO mAudioFileInfo;
    /* end of ape used*/

    status_t mInitCheck;
    sp<DataSource> mDataSource;
    sp<MetaData> mMeta;

    APEExtractor(const APEExtractor &);
    APEExtractor &operator=(const APEExtractor &);
};

}  // namespace android

#endif  // APE_EXTRACTOR_H_

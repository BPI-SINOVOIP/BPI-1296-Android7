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

#ifndef DSD_EXTRACTOR_H_

#define DSD_EXTRACTOR_H_

#include <utils/Errors.h>
#include <media/IMediaSource.h>
#include <media/stagefright/MediaExtractor.h>

namespace android {

struct AMessage;
class DataSource;
class String8;

class DSDExtractor : public MediaExtractor {
public:
    // Extractor assumes ownership of "source".
    DSDExtractor(const sp<DataSource> &source);

    virtual size_t countTracks();
    virtual sp<IMediaSource> getTrack(size_t index);
    virtual sp<MetaData> getTrackMetaData(size_t index, uint32_t flags);
    virtual sp<MetaData> getMetaData();

private:

    long long currFilePos;
    long long dataSize;
    long long dataStart;
    off64_t streamTotalBytes;
    status_t mInitCheck;
    long long Metadata_start_pos;
    uint32_t Channel_Type;
    uint32_t Channel_num;
    uint32_t Sampling_frequency;
    uint32_t Bits_per_sample;
    long long Sample_count;
    uint32_t Block_size_per_channel;

    sp<DataSource> mDataSource;
    sp<MetaData> mMeta;

    DSDExtractor(const DSDExtractor &);
    DSDExtractor &operator=(const DSDExtractor &);
};

}  // namespace android

#endif  // DSD_EXTRACTOR_H_

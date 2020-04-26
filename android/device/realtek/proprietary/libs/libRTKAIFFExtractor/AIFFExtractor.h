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

#ifndef AIFF_EXTRACTOR_H_

#define AIFF_EXTRACTOR_H_

#include <utils/Errors.h>
#include <media/IMediaSource.h>
#include <media/stagefright/MediaExtractor.h>

namespace android {

struct AMessage;
class DataSource;
class String8;

class AIFFExtractor : public MediaExtractor {
public:
    // Extractor assumes ownership of "source".
    AIFFExtractor(const sp<DataSource> &source);

    virtual size_t countTracks();
    virtual sp<IMediaSource> getTrack(size_t index);
    virtual sp<MetaData> getTrackMetaData(size_t index, uint32_t flags);

    virtual sp<MetaData> getMetaData();
    status_t find_iff_chunk(uint32_t chunk, uint32_t * length);
    double ieee754_read_extended(uint8_t* in);

private:
    /* start of aiff used*/
    /* === Endian-related stuff === */
    #define ARRANGE_ENDIAN_16(dat) ( (((dat) & 0xff00 ) >> 8 ) | (((dat) & 0x00ff ) << 8 ) )
    #define ARRANGE_ENDIAN_32(dat) ( (((dat) & 0xff000000 ) >> 24 ) | (((dat) & 0x00ff0000 ) >> 8 ) | (((dat) & 0x0000ff00 ) << 8 ) | (((dat) & 0x000000ff ) << 24 ) )
    /* File formats */
    #define AIFF_TYPE_IFF ARRANGE_ENDIAN_32(0x464F524D)
    #define AIFF_TYPE_AIFF ARRANGE_ENDIAN_32(0x41494646)
    #define AIFF_TYPE_AIFC ARRANGE_ENDIAN_32(0x41494643)

    /* OSTypes */
    #define AIFF_NAME	 0x4e414d45
    #define AIFF_AUTH	 0x41555448
    #define AIFF_COPY 0x28632920
    #define AIFF_ANNO 0x414e4e4f
    #define AIFF_FORM 0x464f524d
    #define AIFF_AIFF 0x41494646
    #define AIFF_AIFC 0x41494643
    #define AIFF_FVER 0x46564552
    #define AIFF_COMM 0x434f4d4d
    #define AIFF_SSND 0x53534e44
    #define AIFF_MARK 0x4d41524b
    #define AIFF_INST 0x494e5354
    #define AIFF_COMT 0x434f4d54

    /* Audio formats */
    #define AUDIO_FORMAT_LPCM ARRANGE_ENDIAN_32(0x4E4F4E45) //'NONE'
    #define AUDIO_FORMAT_ACE2 ARRANGE_ENDIAN_32(0x41434532) //'ACE2'
    #define AUDIO_FORMAT_ACE8 ARRANGE_ENDIAN_32(0x41434538) //'ACE8'
    #define AUDIO_FORMAT_MAC3 ARRANGE_ENDIAN_32(0x4d414333) //'MAC3'
    #define AUDIO_FORMAT_MAC6 ARRANGE_ENDIAN_32(0x4d414336) //'MAC6'
    #define AUDIO_FORMAT_FL32 ARRANGE_ENDIAN_32(0x464c3332) //PCM 32-bits floating point
    #define AUDIO_FORMAT_fl32 ARRANGE_ENDIAN_32(0x666c3332)
    #define AUDIO_FORMAT_FL64 ARRANGE_ENDIAN_32(0x464c3634) //PCM 64-bits floating point
    #define AUDIO_FORMAT_fl64 ARRANGE_ENDIAN_32(0x666c3634)
    #define AUDIO_FORMAT_ima4 ARRANGE_ENDIAN_32(0x696D6134) //IMA 4:1 (one kind of ADPCM)
    #define AUDIO_FORMAT_UNKNOWN 0xFFFFFFFF

    typedef struct s_IFFHeader
    {
        uint32_t hid ;
        uint32_t len ;
        uint32_t fid ;
    } IFFHeader ;

    typedef  struct s_IFFChunk
    {
        uint32_t id ;
        uint32_t len ;
    }  IFFChunk ;

    /* WARNING: this struct is not alignment-safe! */
    struct s_AIFFCommon
    {
        uint16_t numChannels;
        uint32_t numSampleFrames;
        uint16_t sampleSize;
        uint8_t  sampleRate[10]; /* Motorola 80-bit extended */
    }__attribute__ ((packed));

    typedef struct _AIFFFORMATEX {
        uint16_t numberOfChannels;
        uint32_t numSampleFrames;
        uint16_t bitsPerSample;
        double samplingRate;
    } AIFFFORMATEX, *PAIFFFORMATE;

    typedef struct s_AIFFSound
    {
        uint32_t offset;
        uint32_t blockSize;
    } SoundChunk;

    long long currFilePos;
    long long dataSize;
    long long dataStart;
    off64_t streamTotalBytes;
    typedef struct s_AIFFCommon CommonChunk;
    AIFFFORMATEX aiffformat;
    bool isAIFC;
    /* end of aiff used*/
    status_t mInitCheck;

    sp<DataSource> mDataSource;
    sp<MetaData> mMeta;

    AIFFExtractor(const AIFFExtractor &);
    AIFFExtractor &operator=(const AIFFExtractor &);
};

}  // namespace android

#endif  // AIFF_EXTRACTOR_H_

/*
 * Copyright (C) 2011 The Android Open Source Project
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

#ifndef AVI_EXTRACTOR_H_

#define AVI_EXTRACTOR_H_

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaSource.h>
#include <utils/Vector.h>

namespace android {

struct AVIExtractor : public MediaExtractor {
    AVIExtractor(const sp<DataSource> &dataSource);

    virtual size_t countTracks();

    virtual sp<IMediaSource> getTrack(size_t index);

    virtual sp<MetaData> getTrackMetaData(
            size_t index, uint32_t flags);

    virtual sp<MetaData> getMetaData();
    virtual const char * name() { return "AVIExtractor"; }

protected:
    virtual ~AVIExtractor();

private:
    struct AVISource;
    struct MP3Splitter;

#ifdef RTK_PLATFORM
    struct SampleInfo {
        uint64_t mOffset;
        uint32_t mSize;
        bool mIsKey;
    };
#else
    struct SampleInfo {
        uint32_t mOffset;
        bool mIsKey;
    };
#endif

    struct Track {
        sp<MetaData> mMeta;
        Vector<SampleInfo> mSamples;
        uint32_t mRate;
        uint32_t mScale;

        // If bytes per sample == 0, each chunk represents a single sample,
        // otherwise each chunk should me a multiple of bytes-per-sample in
        // size.
        uint32_t mBytesPerSample;

        enum Kind {
            AUDIO,
            VIDEO,
            OTHER

        } mKind;

        size_t mNumSyncSamples;
        size_t mThumbnailSampleSize;
        ssize_t mThumbnailSampleIndex;
        size_t mMaxSampleSize;

        // If mBytesPerSample > 0:
        double mAvgChunkSize;
        size_t mFirstChunkSize;
#ifdef RTK_PLATFORM
        typedef struct _WAVEFORMATEX {
            uint16_t wFormatTag;
            uint16_t nChannels;
            uint32_t nSamplesPerSec;
            uint32_t nAvgBytesPerSec;
            uint16_t nBlockAlign;
            uint16_t wBitsPerSample;
            uint16_t cbSize;
            uint16_t reserved;
        } WAVEFORMATEX;
        WAVEFORMATEX waveformHeader;
        uint32_t mStart;

        // 0: not VC1, 1: WVC1, 2: WMV3
        uint32_t mVC1Type;
#endif
    };

    sp<DataSource> mDataSource;
    status_t mInitCheck;
    Vector<Track> mTracks;

    off64_t mMovieOffset;
    bool mFoundIndex;
    bool mOffsetsAreAbsolute;

    ssize_t parseChunk(off64_t offset, off64_t size, int depth = 0);
    status_t parseStreamHeader(off64_t offset, size_t size);
    status_t parseStreamFormat(off64_t offset, size_t size);
    status_t parseIndex(off64_t offset, size_t size);

    status_t parseHeaders();

    status_t getSampleInfo(
            size_t trackIndex, size_t sampleIndex,
            off64_t *offset, size_t *size, bool *isKey,
            int64_t *sampleTimeUs);

    status_t getSampleTime(
            size_t trackIndex, size_t sampleIndex, int64_t *sampleTimeUs);

#ifdef RTK_PLATFORM
    status_t getSampleIndexAtTime(
            size_t trackIndex,
            int64_t timeUs, MediaSource::ReadOptions::SeekMode mode,
            size_t *sampleIndex);
#else
    status_t getSampleIndexAtTime(
            size_t trackIndex,
            int64_t timeUs, MediaSource::ReadOptions::SeekMode mode,
            size_t *sampleIndex) const;
#endif

#ifdef RTK_PLATFORM
    sp<ABuffer> MakeMPEG4VideoCodecSpecificData(const sp<ABuffer> &config);
#endif
    status_t addMPEG4CodecSpecificData(size_t trackIndex);
    status_t addH264CodecSpecificData(size_t trackIndex);

    static bool IsCorrectChunkType(
        ssize_t trackIndex, Track::Kind kind, uint32_t chunkType);

    DISALLOW_EVIL_CONSTRUCTORS(AVIExtractor);

#ifdef RTK_PLATFORM
    // index type codes
    #define AVI_INDEX_OF_INDEXES       0x00
    #define AVI_INDEX_OF_CHUNKS        0x01
    #define AVI_INDEX_OF_TIMED_CHUNKS  0x02
    #define AVI_INDEX_OF_SUB_2FIELD    0x03
    #define AVI_INDEX_IS_DATA          0x80

    typedef enum {
        NO_INDEX,
        MOVI_LIST_BASED,
        FILE_START_BASED,
        AVI2_SINGLE_INDEX,
        AVI2_TWO_TIERED_INDEX
    } idx1Type;

    #define ckidMAINAVIHEADER FOURCC('a', 'v', 'i', 'h')
    typedef struct _avimainheader {
        uint32_t  fcc;                    // 'avih'
        uint32_t  cb;                     // size of this structure -8
        uint32_t  dwMicroSecPerFrame;     // frame display rate (or 0L)
        uint32_t  dwMaxBytesPerSec;       // max. transfer rate
        uint32_t  dwPaddingGranularity;   // pad to multiples of this size; normally 2K.
        uint32_t  dwFlags;                // the ever-present flags
        #define AVIF_HASINDEX        0x00000010 // Index at end of file?
        #define AVIF_MUSTUSEINDEX    0x00000020
        #define AVIF_ISINTERLEAVED   0x00000100
        #define AVIF_TRUSTCKTYPE     0x00000800 // Use CKType to find key frames
        #define AVIF_WASCAPTUREFILE  0x00010000
        #define AVIF_COPYRIGHTED     0x00020000
        uint32_t  dwTotalFrames;          // # frames in first movi list
        uint32_t  dwInitialFrames;
        uint32_t  dwStreams;
        uint32_t  dwSuggestedBufferSize;
        uint32_t  dwWidth;
        uint32_t  dwHeight;
        uint32_t  dwReserved[4];
    } AVIMAINHEADER;

    #define ckidAVISUPERINDEX FCC('indx')
    typedef struct _avisuperindex_entry {
        uint64_t    qwOffset;     // 64 bit offset to sub index chunk
        uint32_t    dwSize;       // 32 bit size of sub index chunk
        uint32_t    dwDuration;   // time span of subindex chunk (in stream ticks)
    } AVISUPERINDEX_ENTRY;

    typedef struct _avisuperindex {
        uint32_t    fcc;               // 'indx'
        uint32_t    cb;                // size of this structure
        uint16_t    wLongsPerEntry;    // ==4
        uint8_t     bIndexSubType;     // ==0 (frame index) or AVI_INDEX_SUB_2FIELD
        uint8_t     bIndexType;        // ==AVI_INDEX_OF_INDEXES
        uint32_t    nEntriesInUse;     // offset of next unused entry in aIndex
        uint32_t    dwChunkId;         // chunk ID of chunks being indexed, (i.e. RGB8)
        uint32_t    dwReserved[3];     // must be 0
        AVISUPERINDEX_ENTRY *aIndex;
    } AVISUPERINDEX;

    typedef struct _avistdindex_entry {
        uint32_t dwOffset; // 32 bit offset to data (points to data, not riff header)
        uint32_t dwSize;    // 31 bit size of data (does not include size of riff header), bit 31 is deltaframe bit
    } AVISTDINDEX_ENTRY;
    #define AVISTDINDEX_DELTAFRAME ( 0x80000000) // Delta frames have the high bit set
    #define AVISTDINDEX_SIZEMASK   (~0x80000000)

    typedef struct _avistdindex {
        uint32_t   fcc;               // 'indx' or '##ix'
        uint32_t   cb;                // size of this structure
        uint16_t   wLongsPerEntry;    // ==2
        uint8_t    bIndexSubType;     // ==0
        uint8_t    bIndexType;        // ==AVI_INDEX_OF_CHUNKS
        uint32_t   nEntriesInUse;     // offset of next unused entry in aIndex
        uint32_t   dwChunkId;         // chunk ID of chunks being indexed, (i.e. RGB8)
        //uint64_t   qwBaseOffset;     // base offset that all index intries are relative to
        uint32_t   qwBaseOffset1;
        uint32_t   qwBaseOffset2;
        uint32_t    dwReserved_3;      // must be 0
        //AVISTDINDEX_ENTRY aIndex[NUMINDEX(2)];
    } AVISTDINDEX;

    AVIMAINHEADER    mainHeader;
    AVISUPERINDEX    *avi2SuperIndex;
    status_t ReadavihInfo(off64_t offset, size_t size);
    status_t ReadindxInfo(off64_t offset, size_t size);
    status_t Build_AVS_index(off64_t offset, size_t size);
    idx1Type indexType;
    int64_t audioFrameCount;
    int64_t FiledurationUs;
    bool isSend_MPEG4_ESDS;
#endif
};

class String8;
struct AMessage;

bool SniffAVI(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *);

}  // namespace android

#endif  // AVI_EXTRACTOR_H_

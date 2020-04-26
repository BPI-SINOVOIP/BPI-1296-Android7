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
#define LOG_TAG "MatroskaExtractor"
#include <utils/Log.h>

#include "MatroskaExtractor.h"
#include "avc_utils.h"

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AUtils.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ColorUtils.h>
#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/Utils.h>
#include <utils/String8.h>

#include <inttypes.h>

#ifdef RTK_PLATFORM
#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |     \
                          (((uint32_t)(ch4) & 0xFF00) << 8) |    \
                          (((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
                          (((uint32_t)(ch4) & 0xFF000000) >> 24))
#endif

namespace android {

struct DataSourceReader : public mkvparser::IMkvReader {
    DataSourceReader(const sp<DataSource> &source)
        : mSource(source) {
    }

    virtual int Read(long long position, long length, unsigned char* buffer) {
        CHECK(position >= 0);
        CHECK(length >= 0);

        if (length == 0) {
            return 0;
        }

        ssize_t n = mSource->readAt(position, buffer, length);

        if (n <= 0) {
            return -1;
        }

        return 0;
    }

    virtual int Length(long long* total, long long* available) {
        off64_t size;
        if (mSource->getSize(&size) != OK) {
            *total = -1;
            *available = (long long)((1ull << 63) - 1);

            return 0;
        }

        if (total) {
            *total = size;
        }

        if (available) {
            *available = size;
        }

        return 0;
    }

private:
    sp<DataSource> mSource;

    DataSourceReader(const DataSourceReader &);
    DataSourceReader &operator=(const DataSourceReader &);
};

////////////////////////////////////////////////////////////////////////////////

struct BlockIterator {
    BlockIterator(MatroskaExtractor *extractor, unsigned long trackNum, unsigned long index);

    bool eos() const;

    void advance();
    void reset();

    void seek(
            int64_t seekTimeUs, bool isAudio,
            int64_t *actualFrameTimeUs);

    const mkvparser::Block *block() const;
    int64_t blockTimeUs() const;

private:
    MatroskaExtractor *mExtractor;
    long long mTrackNum;
    unsigned long mIndex;

    const mkvparser::Cluster *mCluster;
    const mkvparser::BlockEntry *mBlockEntry;
    long mBlockEntryIndex;

    void advance_l();

    BlockIterator(const BlockIterator &);
    BlockIterator &operator=(const BlockIterator &);
};

struct MatroskaSource : public MediaSource {
    MatroskaSource(
            const sp<MatroskaExtractor> &extractor, size_t index);

    virtual status_t start(MetaData *params);
    virtual status_t stop();

    virtual sp<MetaData> getFormat();

    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options);

protected:
    virtual ~MatroskaSource();

private:
    enum Type {
        AVC,
        AAC,
        HEVC,
#ifdef RTK_PLATFORM
        AC3,
        MP3,
        DTS,
        DDHD,
#endif
        OTHER
    };

    sp<MatroskaExtractor> mExtractor;
    size_t mTrackIndex;
    Type mType;
    bool mIsAudio;
    BlockIterator mBlockIter;
    ssize_t mNALSizeLen;  // for type AVC

    List<MediaBuffer *> mPendingFrames;

    status_t advance();

    status_t setWebmBlockCryptoInfo(MediaBuffer *mbuf);
    status_t readBlock();
    void clearPendingFrames();

    MatroskaSource(const MatroskaSource &);
    MatroskaSource &operator=(const MatroskaSource &);

#ifdef RTK_PLATFORM
#define ZERO_NAN 0
#define FrameRate_23 23.976
#define FrameRate_24 24
#define FrameRate_29 29.97
#define FrameRate_30 30
#define FrameRate_50 50
#define FrameRate_59 59.94
#define FrameRate_60 60
#define MAX_NAN 100
    struct strTimeUs {
        int64_t mTimeUs;
    };

    static int Sortarray(const strTimeUs *a, const strTimeUs *b);
    double calculateFrameRate();
    MediaBuffer * AudioAddPrefixSyncWord(MediaBuffer *, const char *stSyncWord, int32_t dsize);
    int nVideoHeaderStripSize; // for type AVC or HEVC
    bool IsDTSHDAudio();
#endif
};

const mkvparser::Track* MatroskaExtractor::TrackInfo::getTrack() const {
    return mExtractor->mSegment->GetTracks()->GetTrackByNumber(mTrackNum);
}

// This function does exactly the same as mkvparser::Cues::Find, except that it
// searches in our own track based vectors. We should not need this once mkvparser
// adds the same functionality.
const mkvparser::CuePoint::TrackPosition *MatroskaExtractor::TrackInfo::find(
        long long timeNs) const {
    ALOGV("mCuePoints.size %zu", mCuePoints.size());
    if (mCuePoints.empty()) {
        return NULL;
    }

    const mkvparser::CuePoint* cp = mCuePoints.itemAt(0);
    const mkvparser::Track* track = getTrack();
    if (timeNs <= cp->GetTime(mExtractor->mSegment)) {
        return cp->Find(track);
    }

    // Binary searches through relevant cues; assumes cues are ordered by timecode.
    // If we do detect out-of-order cues, return NULL.
    size_t lo = 0;
    size_t hi = mCuePoints.size();
    while (lo < hi) {
        const size_t mid = lo + (hi - lo) / 2;
        const mkvparser::CuePoint* const midCp = mCuePoints.itemAt(mid);
        const long long cueTimeNs = midCp->GetTime(mExtractor->mSegment);
        if (cueTimeNs <= timeNs) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    if (lo == 0) {
        return NULL;
    }

    cp = mCuePoints.itemAt(lo - 1);
    if (cp->GetTime(mExtractor->mSegment) > timeNs) {
        return NULL;
    }

    return cp->Find(track);
}

MatroskaSource::MatroskaSource(
        const sp<MatroskaExtractor> &extractor, size_t index)
    : mExtractor(extractor),
      mTrackIndex(index),
      mType(OTHER),
      mIsAudio(false),
      mBlockIter(mExtractor.get(),
                 mExtractor->mTracks.itemAt(index).mTrackNum,
                 index),
      mNALSizeLen(-1) {
    sp<MetaData> meta = mExtractor->mTracks.itemAt(index).mMeta;

    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));
    mIsAudio = !strncasecmp("audio/", mime, 6);

    if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)
#ifdef RTK_PLATFORM
            &&extractor->mIsVFWFOURCC == 0
#endif
            ) {
        mType = AVC;

        uint32_t dummy;
        const uint8_t *avcc;
        size_t avccSize;
        int32_t nalSizeLen = 0;
        if (meta->findInt32(kKeyNalLengthSize, &nalSizeLen)) {
            if (nalSizeLen >= 0 && nalSizeLen <= 4) {
                mNALSizeLen = nalSizeLen;
            }
        } else if (meta->findData(kKeyAVCC, &dummy, (const void **)&avcc, &avccSize)
                && avccSize >= 5u) {
            mNALSizeLen = 1 + (avcc[4] & 3);
            ALOGV("mNALSizeLen = %zd", mNALSizeLen);
        } else {
            ALOGE("No mNALSizeLen");
        }
    } else if (!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC)) {
        mType = AAC;
    }
#ifdef RTK_PLATFORM
    else if(!strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC_MAIN)) {
        mType = AAC;
    }
    else if(!strcasecmp(mime,MEDIA_MIMETYPE_VIDEO_HEVC))
    {
        uint32_t type;
        const void *data;
        size_t size;
        if(meta->findData(kKeyHVCC, &type, &data, &size))
        {
            mType = HEVC;
            const uint8_t *ptr = (const uint8_t *)data;
            CHECK(size >= 7);
            CHECK((unsigned)ptr[0] <= 1u );  // configurationVersion == 1 or 0
            mNALSizeLen = 1 + (ptr[14 + 7] & 3);
        }
    }
    else if(!strcasecmp(mime,MEDIA_MIMETYPE_AUDIO_AC3)) {
        mType = AC3;
    }
    else if(!strcasecmp(mime,MEDIA_MIMETYPE_AUDIO_MPEG))
    {
        mType = MP3;
    }
    else if(!strcasecmp(mime,MEDIA_MIMETYPE_AUDIO_DTS))
    {
        if(mIsAudio)
        {
            if (IsDTSHDAudio())
                meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_DTS_HD);
        }

        mType = DTS;
    }
    else if(!strcasecmp(mime,MEDIA_MIMETYPE_AUDIO_MLP))
    {
        mType = DDHD;
    }

    if (mType == AVC || mType == HEVC)
    {
        nVideoHeaderStripSize = mExtractor->mTracks.itemAt(index).mContentCompSettingLen;
        ALOGD("\033[0;32m[%d] mNALSizeLen= %d,  nVideoHeaderStripSize= %d\033[m\n", __LINE__, (int)mNALSizeLen, nVideoHeaderStripSize);
        if (mNALSizeLen != 4)
            ALOGD("\033[0;31m[%d] mNALSizeLen= %d is not correct,  need to replace by nVideoHeaderStripSize= %d\033[m\n", __LINE__, (int)mNALSizeLen, nVideoHeaderStripSize);
        else
            nVideoHeaderStripSize = 0;
    }
    else
        nVideoHeaderStripSize = 0;

    if(!mIsAudio)
    {
        float slFramerate = 0;
        meta->findFloat(kKeyFrameRateFloat, &slFramerate);
        if(slFramerate <= 0)
        {
            ALOGI("[%s][%d] kKeyFrameRate ::%f need check frame rate by calculating frame count/timeUs.\n",__FUNCTION__,__LINE__,slFramerate);
            slFramerate = calculateFrameRate();

            if(slFramerate > 0 && slFramerate < 100)
            {
                meta->setFloat(kKeyFrameRateFloat, slFramerate);
                meta->setInt32(kKeyFrameRate, (int)slFramerate);
            }
        }
    }
#endif

}

#ifdef RTK_PLATFORM
int MatroskaSource::Sortarray(const strTimeUs *a, const strTimeUs *b)
{
    if (a->mTimeUs <= b->mTimeUs)
        return -1;

    return 1;
}

double MatroskaSource::calculateFrameRate()
{
    MediaBuffer *buffer = NULL;
    double dFramerate = 0;
    int64_t timeUs,StarttimeUs = -1;
    int32_t dFrameCount =0;
    int32_t dReadBit = 0;
    int64_t difftimeUs = 0;
    int32_t dIterCount = 0;

#define DEF_FRAME_RATE_NUMBER 9
#define FRAME_COUNR_NUM 100
#define READ_BIT_NUM 5*1000*1000
#define DIFF_TIME_US_NUM 10*1000000
#define ITER_COUNT_NUM 500

    double FRarray[DEF_FRAME_RATE_NUMBER]={ZERO_NAN,FrameRate_23,FrameRate_24,FrameRate_29,FrameRate_30,FrameRate_50,FrameRate_59,FrameRate_60,MAX_NAN};
    int64_t timeTick=0,PreTimeUs=0;
    Vector<strTimeUs> vTimeUsVec;

    while(dFrameCount < FRAME_COUNR_NUM && dReadBit < READ_BIT_NUM && difftimeUs < DIFF_TIME_US_NUM && dIterCount++ < ITER_COUNT_NUM)
    {
        read(&buffer,NULL);
        if(buffer == NULL)
            continue;

        buffer->meta_data()->findInt64(kKeyTime, &timeUs);
        strTimeUs item;
        item.mTimeUs = timeUs;
        vTimeUsVec.push(item);

        if(StarttimeUs == -1 && timeUs > 0)
            StarttimeUs = timeUs;

        dReadBit += buffer->range_length();
        difftimeUs = timeUs - StarttimeUs;
        buffer->release();
    }

    vTimeUsVec.sort(Sortarray);
    dFrameCount = 0;
    PreTimeUs = 0;
    size_t index = 0;
    for (index = 0; index < vTimeUsVec.size(); index++)
    {
        const strTimeUs &item =vTimeUsVec[index];

        timeTick = item.mTimeUs - PreTimeUs;
        PreTimeUs = item.mTimeUs;
        if(timeTick < 16000 || timeTick > 100000)
            continue;
        dFrameCount++;
    }
    vTimeUsVec.clear();
    dFramerate = dFrameCount * 1E6 / difftimeUs;
    int32_t i=0;
    int32_t dMinDis = 1000, dIdx =-1,dDis = 0;
    for(i=0;i<DEF_FRAME_RATE_NUMBER;i++)
    {
       dDis = (FRarray[i] > dFramerate)?(FRarray[i] - dFramerate):(dFramerate - FRarray[i]);
       if(dMinDis > dDis)
       {
           dMinDis = dDis;
           dIdx = i;
       }
    }
    dFramerate = FRarray[dIdx];
    ALOGI("[%s][%d]dFrameCount::%d, dReadBit:%d, difftimeUs::%fs, dFramerate::%f.\n",__FUNCTION__,__LINE__,
            dFrameCount, dReadBit , (double)difftimeUs/1E6, dFramerate);
    return dFramerate;
}

bool MatroskaSource::IsDTSHDAudio() {
    // identify it is DTS-HD or not
    MediaBuffer *mbuf = NULL;
    size_t packet_size = 0, pos = 0, frame_size = 0;
    char *checkDTSformat = NULL;
    int word_mode = 0; //define bitstream
    int le_mode = 0; //define endian mode
    bool findDTSAudio = false;
    uint32_t msyncWord = 0;

    read(&mbuf,NULL);
    if (mbuf == NULL)
        return false;
    else
    {
        packet_size = mbuf->size();
        checkDTSformat = (char *)mbuf->data();

        while (pos < packet_size)
        {
            msyncWord = checkDTSformat[pos]<<24 | checkDTSformat[pos+1]<<16 | checkDTSformat[pos+2]<<8 | checkDTSformat[pos+3];
            if (msyncWord == 0x64582025) // EXSS
            {
                //ALOGD("\033[0;32m[%d] mbuf->size()= %d   pos= %d   return true\033[m\n", __LINE__, (int)mbuf->size(), pos);
                mbuf->release();
                return true;
            }

            switch(msyncWord)
            {
                //14 bits LE
                case 0xFF1F00E8:
                    //make sure frame type is DTS
                    if ((checkDTSformat[pos+4] & 0xF0) != 0xF0 || checkDTSformat[pos+5] != 0x07)
                        return false; // wrong DTS start code
                    word_mode = 0;
                    le_mode = 1;
                    findDTSAudio = true;
                    break;
                //14 bit BE
                case 0x1FFFE800:
                    //make sure frame type is DTS
                    if (checkDTSformat[pos+4] != 0x07 || (checkDTSformat[pos+5] & 0xF0) != 0xF0)
                        return false; // wrong DTS start code
                    word_mode =0;
                    le_mode = 0;
                    findDTSAudio = true;
                    break;
                //16 bits LE
                case 0xFE7F0180:
                case 0x72F81F4E:
                    word_mode = 1;
                    le_mode = 1;
                    findDTSAudio = true;
                    break;
                // 16 bits BE
                case 0x5A5A5A5A://XCH
                case 0x1D95F262://X96K
                case 0x64582025://EXSS
                case 0x7FFE8001:
                    word_mode = 1;
                    le_mode = 0;
                    findDTSAudio = true;
                    break;
                default:
                    findDTSAudio = false;
                    break;
            }
            //ALOGD("\033[0;32m [%d]  word_mode= %d, le_mode= %d   findDTSAudio= %d\033[m\n", __LINE__, word_mode, le_mode, findDTSAudio);

            if (findDTSAudio)
            {
                if(word_mode){
                    frame_size = (checkDTSformat[pos+5-le_mode] & 0x03) << 12 | checkDTSformat[pos+6+le_mode] << 4| checkDTSformat[pos+7-le_mode] >> 4;
                    ++frame_size;
                }else{
                    frame_size = (checkDTSformat[pos+6+le_mode] & 0x03) << 12 | checkDTSformat[pos+7-le_mode] << 4| (checkDTSformat[pos+8+le_mode] & 0x3F) >> 2;
                    ++frame_size;
                }
                //ALOGD("\033[0;32m [%d]  frame_size= %d\033[m\n", __LINE__, (int)frame_size);
                pos += frame_size;
            }
            else
                break;
        }
        //ALOGD("\033[0;32m[%d] mbuf->size()= %d   pos= %d     return false....\033[m\n", __LINE__, (int)mbuf->size(), pos);
        mbuf->release();
        return false;
    }
}
#endif

MatroskaSource::~MatroskaSource() {
    clearPendingFrames();
}

status_t MatroskaSource::start(MetaData * /* params */) {
    if (mType == AVC && mNALSizeLen < 0) {
        return ERROR_MALFORMED;
    }

    mBlockIter.reset();

    return OK;
}

status_t MatroskaSource::stop() {
    clearPendingFrames();

    return OK;
}

sp<MetaData> MatroskaSource::getFormat() {
    return mExtractor->mTracks.itemAt(mTrackIndex).mMeta;
}

////////////////////////////////////////////////////////////////////////////////

BlockIterator::BlockIterator(
        MatroskaExtractor *extractor, unsigned long trackNum, unsigned long index)
    : mExtractor(extractor),
      mTrackNum(trackNum),
      mIndex(index),
      mCluster(NULL),
      mBlockEntry(NULL),
      mBlockEntryIndex(0) {
    reset();
}

bool BlockIterator::eos() const {
    return mCluster == NULL || mCluster->EOS();
}

void BlockIterator::advance() {
    Mutex::Autolock autoLock(mExtractor->mLock);
    advance_l();
}

void BlockIterator::advance_l() {
    for (;;) {
        long res = mCluster->GetEntry(mBlockEntryIndex, mBlockEntry);
        ALOGV("GetEntry returned %ld", res);

        long long pos;
        long len;
        if (res < 0) {
            // Need to parse this cluster some more

            CHECK_EQ(res, mkvparser::E_BUFFER_NOT_FULL);

            res = mCluster->Parse(pos, len);
            ALOGV("Parse returned %ld", res);

            if (res < 0) {
                // I/O error

                ALOGE("Cluster::Parse returned result %ld", res);

                mCluster = NULL;
                break;
            }

            continue;
        } else if (res == 0) {
            // We're done with this cluster

            const mkvparser::Cluster *nextCluster;
            res = mExtractor->mSegment->ParseNext(
                    mCluster, nextCluster, pos, len);
            ALOGV("ParseNext returned %ld", res);

            if (res != 0) {
                // EOF or error

                mCluster = NULL;
                break;
            }

            CHECK_EQ(res, 0);
            CHECK(nextCluster != NULL);
            CHECK(!nextCluster->EOS());

            mCluster = nextCluster;

            res = mCluster->Parse(pos, len);
            ALOGV("Parse (2) returned %ld", res);
            CHECK_GE(res, 0);

            mBlockEntryIndex = 0;
            continue;
        }

        CHECK(mBlockEntry != NULL);
        CHECK(mBlockEntry->GetBlock() != NULL);
        ++mBlockEntryIndex;

        if (mBlockEntry->GetBlock()->GetTrackNumber() == mTrackNum) {
            break;
        }
    }
}

void BlockIterator::reset() {
    Mutex::Autolock autoLock(mExtractor->mLock);

    mCluster = mExtractor->mSegment->GetFirst();
    mBlockEntry = NULL;
    mBlockEntryIndex = 0;

    do {
        advance_l();
    } while (!eos() && block()->GetTrackNumber() != mTrackNum);
}

void BlockIterator::seek(
        int64_t seekTimeUs, bool isAudio,
        int64_t *actualFrameTimeUs) {
    Mutex::Autolock autoLock(mExtractor->mLock);

    *actualFrameTimeUs = -1ll;

    const int64_t seekTimeNs = seekTimeUs * 1000ll - mExtractor->mSeekPreRollNs;

    mkvparser::Segment* const pSegment = mExtractor->mSegment;

    // Special case the 0 seek to avoid loading Cues when the application
    // extraneously seeks to 0 before playing.
    if (seekTimeNs <= 0) {
        ALOGV("Seek to beginning: %" PRId64, seekTimeUs);
        mCluster = pSegment->GetFirst();
        mBlockEntryIndex = 0;
        do {
            advance_l();
        } while (!eos() && block()->GetTrackNumber() != mTrackNum);
        return;
    }

    ALOGV("Seeking to: %" PRId64, seekTimeUs);

    // If the Cues have not been located then find them.
    const mkvparser::Cues* pCues = pSegment->GetCues();
    const mkvparser::SeekHead* pSH = pSegment->GetSeekHead();
    if (!pCues && pSH) {
        const size_t count = pSH->GetCount();
        const mkvparser::SeekHead::Entry* pEntry;
        ALOGV("No Cues yet");

        for (size_t index = 0; index < count; index++) {
            pEntry = pSH->GetEntry(index);

            if (pEntry->id == 0x0C53BB6B) { // Cues ID
                long len; long long pos;
                pSegment->ParseCues(pEntry->pos, pos, len);
                pCues = pSegment->GetCues();
                ALOGV("Cues found");
                break;
            }
        }

        if (!pCues) {
            ALOGE("No Cues in file");
#ifdef RTK_PLATFORM
            if (seekTimeNs < pSegment->GetLast()->GetTime()) {
                mCluster = pSegment->FindCluster(seekTimeNs);
            } else {
                mCluster = pSegment->FindCluster(pSegment->GetLast()->GetTime() - 10000000000ll);
            }
            for(;;) {
                advance_l();
                if (eos()) break;
                if (isAudio || block()->IsKey()) {
                    // Accept the first key frame
                    int64_t frameTimeUs = (block()->GetTime(mCluster) + 500LL) / 1000LL;
                    if (!isAudio || frameTimeUs >= seekTimeUs) {
                        *actualFrameTimeUs = frameTimeUs;
                        ALOGV("Requested seek point: %" PRId64 " actual: %" PRId64,
                              seekTimeUs, *actualFrameTimeUs);
                        break;
                    }
                }
            }
#endif
            return;
        }
    }
    else if (!pSH) {
        ALOGE("No SeekHead");
        return;
    }

    const mkvparser::CuePoint* pCP;
    mkvparser::Tracks const *pTracks = pSegment->GetTracks();
    while (!pCues->DoneParsing()) {
        pCues->LoadCuePoint();
        pCP = pCues->GetLast();
        CHECK(pCP);

        size_t trackCount = mExtractor->mTracks.size();
        for (size_t index = 0; index < trackCount; ++index) {
            MatroskaExtractor::TrackInfo& track = mExtractor->mTracks.editItemAt(index);
            const mkvparser::Track *pTrack = pTracks->GetTrackByNumber(track.mTrackNum);
            if (pTrack && pTrack->GetType() == 1 && pCP->Find(pTrack)) { // VIDEO_TRACK
                track.mCuePoints.push_back(pCP);
            }
        }

        if (pCP->GetTime(pSegment) >= seekTimeNs) {
            ALOGV("Parsed past relevant Cue");
            break;
        }
    }

    const mkvparser::CuePoint::TrackPosition *pTP = NULL;
    const mkvparser::Track *thisTrack = pTracks->GetTrackByNumber(mTrackNum);
    if (thisTrack->GetType() == 1) { // video
        MatroskaExtractor::TrackInfo& track = mExtractor->mTracks.editItemAt(mIndex);
        pTP = track.find(seekTimeNs);
    } else {
        // The Cue index is built around video keyframes
        unsigned long int trackCount = pTracks->GetTracksCount();
        for (size_t index = 0; index < trackCount; ++index) {
            const mkvparser::Track *pTrack = pTracks->GetTrackByIndex(index);
            if (pTrack && pTrack->GetType() == 1 && pCues->Find(seekTimeNs, pTrack, pCP, pTP)) {
                ALOGV("Video track located at %zu", index);
                break;
            }
        }
    }


    // Always *search* based on the video track, but finalize based on mTrackNum
    if (!pTP) {
        ALOGE("Did not locate the video track for seeking");
        return;
    }

    mCluster = pSegment->FindOrPreloadCluster(pTP->m_pos);

    CHECK(mCluster);
    CHECK(!mCluster->EOS());

    // mBlockEntryIndex starts at 0 but m_block starts at 1
    CHECK_GT(pTP->m_block, 0);
    mBlockEntryIndex = pTP->m_block - 1;

    for (;;) {
        advance_l();

        if (eos()) break;

        if (isAudio || block()->IsKey()) {
            // Accept the first key frame
            int64_t frameTimeUs = (block()->GetTime(mCluster) + 500LL) / 1000LL;
            if (thisTrack->GetType() == 1 || frameTimeUs >= seekTimeUs) {
                *actualFrameTimeUs = frameTimeUs;
                ALOGV("Requested seek point: %" PRId64 " actual: %" PRId64,
                      seekTimeUs, *actualFrameTimeUs);
                break;
            }
        }
    }
}

const mkvparser::Block *BlockIterator::block() const {
    CHECK(!eos());

    return mBlockEntry->GetBlock();
}

int64_t BlockIterator::blockTimeUs() const {
    if (mCluster == NULL || mBlockEntry == NULL) {
        return -1;
    }
    return (mBlockEntry->GetBlock()->GetTime(mCluster) + 500ll) / 1000ll;
}

////////////////////////////////////////////////////////////////////////////////

static unsigned U24_AT(const uint8_t *ptr) {
    return ptr[0] << 16 | ptr[1] << 8 | ptr[2];
}

void MatroskaSource::clearPendingFrames() {
    while (!mPendingFrames.empty()) {
        MediaBuffer *frame = *mPendingFrames.begin();
        mPendingFrames.erase(mPendingFrames.begin());

        frame->release();
        frame = NULL;
    }
}
#ifdef RTK_PLATFORM
MediaBuffer *MatroskaSource::AudioAddPrefixSyncWord(MediaBuffer * mbuf, const char *stSyncWord, int32_t dsize)
{
    MediaBuffer *mbuf2 = new MediaBuffer(mbuf->size() + dsize);
    unsigned char *pBuf = (unsigned char *)mbuf2->data();
    memcpy(pBuf, stSyncWord, dsize);
    memcpy((pBuf + dsize), mbuf->data(), mbuf->size());
    mbuf->release();
    mbuf = mbuf2;
    return mbuf;
}
#endif

status_t MatroskaSource::setWebmBlockCryptoInfo(MediaBuffer *mbuf) {
    if (mbuf->range_length() < 1 || mbuf->range_length() - 1 > INT32_MAX) {
        // 1-byte signal
        return ERROR_MALFORMED;
    }

    const uint8_t *data = (const uint8_t *)mbuf->data() + mbuf->range_offset();
    bool blockEncrypted = data[0] & 0x1;
    if (blockEncrypted && mbuf->range_length() < 9) {
        // 1-byte signal + 8-byte IV
        return ERROR_MALFORMED;
    }

    sp<MetaData> meta = mbuf->meta_data();
    if (blockEncrypted) {
        /*
         *  0                   1                   2                   3
         *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
         *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         *  |  Signal Byte  |                                               |
         *  +-+-+-+-+-+-+-+-+             IV                                |
         *  |                                                               |
         *  |               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         *  |               |                                               |
         *  |-+-+-+-+-+-+-+-+                                               |
         *  :               Bytes 1..N of encrypted frame                   :
         *  |                                                               |
         *  |                                                               |
         *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         */
        int32_t plainSizes[] = { 0 };
        int32_t encryptedSizes[] = { static_cast<int32_t>(mbuf->range_length() - 9) };
        uint8_t ctrCounter[16] = { 0 };
        uint32_t type;
        const uint8_t *keyId;
        size_t keyIdSize;
        sp<MetaData> trackMeta = mExtractor->mTracks.itemAt(mTrackIndex).mMeta;
        CHECK(trackMeta->findData(kKeyCryptoKey, &type, (const void **)&keyId, &keyIdSize));
        meta->setData(kKeyCryptoKey, 0, keyId, keyIdSize);
        memcpy(ctrCounter, data + 1, 8);
        meta->setData(kKeyCryptoIV, 0, ctrCounter, 16);
        meta->setData(kKeyPlainSizes, 0, plainSizes, sizeof(plainSizes));
        meta->setData(kKeyEncryptedSizes, 0, encryptedSizes, sizeof(encryptedSizes));
        mbuf->set_range(9, mbuf->range_length() - 9);
    } else {
        /*
         *  0                   1                   2                   3
         *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
         *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         *  |  Signal Byte  |                                               |
         *  +-+-+-+-+-+-+-+-+                                               |
         *  :               Bytes 1..N of unencrypted frame                 :
         *  |                                                               |
         *  |                                                               |
         *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         */
        int32_t plainSizes[] = { static_cast<int32_t>(mbuf->range_length() - 1) };
        int32_t encryptedSizes[] = { 0 };
        meta->setData(kKeyPlainSizes, 0, plainSizes, sizeof(plainSizes));
        meta->setData(kKeyEncryptedSizes, 0, encryptedSizes, sizeof(encryptedSizes));
        mbuf->set_range(1, mbuf->range_length() - 1);
    }

    return OK;
}

status_t MatroskaSource::readBlock() {
    CHECK(mPendingFrames.empty());

    if (mBlockIter.eos()) {
        return ERROR_END_OF_STREAM;
    }
#ifdef RTK_PLATFORM
    if(mIsAudio && mType == DDHD)//DDHD need to buffer more frame data
    {
        long targetBuffersize = 16384;
        long tmpBufferSize = 0;
        const mkvparser::Block *block = NULL;
        int64_t timeUs = mBlockIter.blockTimeUs();
        MediaBuffer *mbuf = new MediaBuffer(targetBuffersize);
        while(tmpBufferSize <= targetBuffersize)
        {
            if(mBlockIter.eos())
            {
                if(tmpBufferSize != 0)
                {
                    mbuf->set_range(0, tmpBufferSize);
                    mPendingFrames.push_back(mbuf);
                    return OK;
                }
                else
                {
                    return ERROR_END_OF_STREAM;
                }
            }
            block = mBlockIter.block();
            for (int i = 0; i < block->GetFrameCount(); ++i) {
                const mkvparser::Block::Frame &frame = block->GetFrame(i);
                if(tmpBufferSize + frame.len > targetBuffersize)
                {
                    mbuf->set_range(0, tmpBufferSize);
                    mPendingFrames.push_back(mbuf);
                    return OK;
                }
                if(tmpBufferSize == 0)
                {
                    mbuf->meta_data()->setInt64(kKeyTime, timeUs);
                    mbuf->meta_data()->setInt32(kKeyIsSyncFrame, block->IsKey());
                }
                long n = frame.Read(mExtractor->mReader, (unsigned char *)(mbuf->data())+tmpBufferSize);
                tmpBufferSize += frame.len;
                if (n != 0) {
                    mPendingFrames.clear();
                    mBlockIter.advance();
                    return ERROR_IO;
                }
            }
            mBlockIter.advance();
        }
        ALOGW("Should not get in here !!");
        return OK;
    }
    else
    {
#endif
        const mkvparser::Block *block = mBlockIter.block();

        int64_t timeUs = mBlockIter.blockTimeUs();
        for (int i = 0; i < block->GetFrameCount(); ++i) {
            const mkvparser::Block::Frame &frame = block->GetFrame(i);
            MediaBuffer *mbuf = new MediaBuffer(frame.len);
            mbuf->meta_data()->setInt64(kKeyTime, timeUs);
        mbuf->meta_data()->setInt32(kKeyIsSyncFrame, block->IsKey());

        status_t err = frame.Read(mExtractor->mReader, static_cast<uint8_t *>(mbuf->data()));
        if (err == OK
                && mExtractor->mIsWebm
                && mExtractor->mTracks.itemAt(mTrackIndex).mEncrypted) {
            err = setWebmBlockCryptoInfo(mbuf);
        }

#ifdef RTK_PLATFORM
        if(mIsAudio && mType == DTS)
        {
            const char str1[4]={0x7F,0xFE,0x80,0x01};
            int32_t dsize = 4;
            if(strncmp((const char *)mbuf->data(), str1, dsize) != 0)
                mbuf = AudioAddPrefixSyncWord(mbuf, str1, dsize);
        }

        if(mIsAudio && mType == AC3)
        {
            const char str1[2]={0x0B,0x77};
            int32_t dsize = 2;
            //check AC3 data has sync word 0x0b77 or not.
            if(strncmp((const char *)mbuf->data(), str1, dsize) != 0)
                mbuf = AudioAddPrefixSyncWord(mbuf, str1, dsize);
        }

        if(mIsAudio && mType == MP3)
        {
            unsigned char * pBuf =(unsigned char *) mbuf->data();
            const char str1[2]={0xFF,0xF0};
            int32_t dsize = 1;
            //add sync word on first two bytes. 0xff 0xfX
            if(strncmp((const char *)mbuf->data(), str1, dsize) != 0 && (pBuf[0]&&0xF0 == 0xF0))
            {
                mbuf = AudioAddPrefixSyncWord(mbuf, str1, dsize);
            }else if(pBuf[0] != 0xFF && (pBuf[1]&&0xF0 != 0xF0))
            {
                ALOGV("[%s][%d]WARNING:MP3 has no syncword:0xfffx ::%2x%2x\n",__FUNCTION__,__LINE__, pBuf[0], pBuf[1]);
            }

        }

        mbuf->meta_data()->setInt64(kKeyTime, timeUs);
        mbuf->meta_data()->setInt32(kKeyIsSyncFrame, block->IsKey());
#endif

        if (err != OK) {
            mPendingFrames.clear();

            mBlockIter.advance();
            mbuf->release();
            return err;
        }

        mPendingFrames.push_back(mbuf);
    }

    mBlockIter.advance();

    return OK;
#ifdef RTK_PLATFORM
    }
#endif
}

status_t MatroskaSource::read(
        MediaBuffer **out, const ReadOptions *options) {
    *out = NULL;

    int64_t targetSampleTimeUs = -1ll;

    int64_t seekTimeUs;
    ReadOptions::SeekMode mode;
    if (options && options->getSeekTo(&seekTimeUs, &mode)
            && !mExtractor->isLiveStreaming()) {
        clearPendingFrames();

        // The audio we want is located by using the Cues to seek the video
        // stream to find the target Cluster then iterating to finalize for
        // audio.
        int64_t actualFrameTimeUs;
        mBlockIter.seek(seekTimeUs, mIsAudio, &actualFrameTimeUs);

        if (mode == ReadOptions::SEEK_CLOSEST) {
            targetSampleTimeUs = actualFrameTimeUs;
        }
    }

    while (mPendingFrames.empty()) {
        status_t err = readBlock();

        if (err != OK) {
            clearPendingFrames();

            return err;
        }
    }

    MediaBuffer *frame = *mPendingFrames.begin();
    mPendingFrames.erase(mPendingFrames.begin());

    if ((mType != AVC && mType != HEVC) || mNALSizeLen == 0) {
        if (targetSampleTimeUs >= 0ll) {
            frame->meta_data()->setInt64(
                    kKeyTargetTime, targetSampleTimeUs);
        }

        *out = frame;

        return OK;
    }

    // Each input frame contains one or more NAL fragments, each fragment
    // is prefixed by mNALSizeLen bytes giving the fragment length,
    // followed by a corresponding number of bytes containing the fragment.
    // We output all these fragments into a single large buffer separated
    // by startcodes (0x00 0x00 0x00 0x01).
    //
    // When mNALSizeLen is 0, we assume the data is already in the format
    // desired.

    const uint8_t *srcPtr =
        (const uint8_t *)frame->data() + frame->range_offset();

    size_t srcSize = frame->range_length();

    size_t dstSize = 0;
    MediaBuffer *buffer = NULL;
    uint8_t *dstPtr = NULL;

    for (int32_t pass = 0; pass < 2; ++pass) {
        size_t srcOffset = 0;
        size_t dstOffset = 0;
#ifdef RTK_PLATFORM
        bool isFirstNAL = true;
#endif
        while (srcOffset + mNALSizeLen <= srcSize) {
            size_t NALsize;
#ifdef RTK_PLATFORM
            if (nVideoHeaderStripSize && isFirstNAL)
            {
                switch (nVideoHeaderStripSize) {
                    case 1: NALsize = U24_AT(srcPtr + srcOffset); break;
                    case 2: NALsize = U16_AT(srcPtr + srcOffset); break;
                    default:
                        TRESPASS();
                }
            }
            else // else of if (nVideoHeaderStripSize && isFirstNAL)
            {
#endif
            switch (mNALSizeLen) {
                case 1: NALsize = srcPtr[srcOffset]; break;
                case 2: NALsize = U16_AT(srcPtr + srcOffset); break;
                case 3: NALsize = U24_AT(srcPtr + srcOffset); break;
                case 4: NALsize = U32_AT(srcPtr + srcOffset); break;
                default:
                    TRESPASS();
            }
#ifdef RTK_PLATFORM
            } // end of if (nVideoHeaderStripSize && isFirstNAL)
            //ALOGD("\033[0;32m[%d] srcOffset= %d, mNALSizeLen= %d, srcSize= %d, NALsize= %d, isFirstNAL= %d, pass= %d\033[m\n", __LINE__, (int)srcOffset, (int)mNALSizeLen, (int)srcSize, (int)NALsize, isFirstNAL, pass);
            //ALOGD("\033[0;32m[%d] NALsize= %d  srcPtr[0 ~ 3]= 0x%.2X %.2X %.2X %.2X \033[m\n", __LINE__, (int)NALsize, srcPtr[srcOffset], srcPtr[srcOffset+1], srcPtr[srcOffset+2], srcPtr[srcOffset+3]);
#endif

            if (srcOffset + mNALSizeLen + NALsize <= srcOffset + mNALSizeLen) {
                frame->release();
                frame = NULL;

                return ERROR_MALFORMED;
            } else if (srcOffset + mNALSizeLen + NALsize > srcSize) {
#ifdef RTK_PLATFORM
                if (nVideoHeaderStripSize != 0)
                {
                    if ((nVideoHeaderStripSize == 1) && (srcOffset + 3 + NALsize > srcSize))
                        break;
                    else if ((nVideoHeaderStripSize == 2) && (srcOffset + 2 + NALsize > srcSize))
                        break;
                }
                else
#endif
                break;
            }

            if (pass == 1) {
                memcpy(&dstPtr[dstOffset], "\x00\x00\x00\x01", 4);

                if (frame != buffer) {
#ifdef RTK_PLATFORM
                    if (nVideoHeaderStripSize != 0 && isFirstNAL)
                    {
                        if (nVideoHeaderStripSize == 1)
                            memcpy(&dstPtr[dstOffset + 4], &srcPtr[srcOffset + 3], NALsize);
                        else if (nVideoHeaderStripSize == 2)
                            memcpy(&dstPtr[dstOffset + 4], &srcPtr[srcOffset + 2], NALsize);
                    }
                    else
#endif
                        memcpy(&dstPtr[dstOffset + 4],
                                &srcPtr[srcOffset + mNALSizeLen],
                                NALsize);
                }
            }

            dstOffset += 4;  // 0x00 00 00 01
            dstOffset += NALsize;

#ifdef RTK_PLATFORM
            if (nVideoHeaderStripSize != 0 && isFirstNAL)
            {
                if (nVideoHeaderStripSize == 1)
                    srcOffset += 3 + NALsize;
                else if (nVideoHeaderStripSize == 2)
                    srcOffset += 2 + NALsize;
                isFirstNAL = false;
            }
            else
#endif
            srcOffset += mNALSizeLen + NALsize;
        }

        if (srcOffset < srcSize) {
            // There were trailing bytes or not enough data to complete
            // a fragment.

            frame->release();
            frame = NULL;

            return ERROR_MALFORMED;
        }

        if (pass == 0) {
            dstSize = dstOffset;

            if (dstSize == srcSize && mNALSizeLen == 4) {
                // In this special case we can re-use the input buffer by substituting
                // each 4-byte nal size with a 4-byte start code
                buffer = frame;
            } else {
                buffer = new MediaBuffer(dstSize);
            }

            int64_t timeUs;
            CHECK(frame->meta_data()->findInt64(kKeyTime, &timeUs));
            int32_t isSync;
            CHECK(frame->meta_data()->findInt32(kKeyIsSyncFrame, &isSync));

            buffer->meta_data()->setInt64(kKeyTime, timeUs);
            buffer->meta_data()->setInt32(kKeyIsSyncFrame, isSync);

            dstPtr = (uint8_t *)buffer->data();
        }
    }

    if (frame != buffer) {
        frame->release();
        frame = NULL;
    }

    if (targetSampleTimeUs >= 0ll) {
        buffer->meta_data()->setInt64(
                kKeyTargetTime, targetSampleTimeUs);
    }

    *out = buffer;

    return OK;
}

////////////////////////////////////////////////////////////////////////////////

MatroskaExtractor::MatroskaExtractor(const sp<DataSource> &source)
    : mDataSource(source),
      mReader(new DataSourceReader(mDataSource)),
      mSegment(NULL),
      mExtractedThumbnails(false),
      mIsWebm(false),
      mSeekPreRollNs(0) {
    off64_t size;
    mIsLiveStreaming =
        (mDataSource->flags()
            & (DataSource::kWantsPrefetching
                | DataSource::kIsCachingDataSource))
        && mDataSource->getSize(&size) != OK;

    mkvparser::EBMLHeader ebmlHeader;
    long long pos;
    if (ebmlHeader.Parse(mReader, pos) < 0) {
        return;
    }

    if (ebmlHeader.m_docType && !strcmp("webm", ebmlHeader.m_docType)) {
        mIsWebm = true;
    }

    long long ret =
        mkvparser::Segment::CreateInstance(mReader, pos, mSegment);

    if (ret) {
        CHECK(mSegment == NULL);
        return;
    }

    // from mkvparser::Segment::Load(), but stop at first cluster
    ret = mSegment->ParseHeaders();
#ifdef RTK_PLATFORM
    // to determine whether the file contains cue.
    // if the file has no cue, load all cluster for seek function.
    bool isHasCues = true;
    const mkvparser::Cues* mCues = mSegment->GetCues();
    const mkvparser::SeekHead* mSH = mSegment->GetSeekHead();
    if (!mCues && mSH) {
        const size_t count = mSH->GetCount();
        const mkvparser::SeekHead::Entry* mEntry;
        ALOGV("No Cues yet");
        for (size_t index = 0; index < count; index++) {
            mEntry = mSH->GetEntry(index);

            if (mEntry->id == 0x0C53BB6B) { // Cues ID
                long len; long long pos;
                mSegment->ParseCues(mEntry->pos, pos, len);
                mCues = mSegment->GetCues();
                ALOGV("Cues found");
                break;
            }
        }

        if (!mCues) {
            ALOGE("No Cues in file");
            isHasCues = false;
        }
    }
    if (isHasCues) {
        ALOGD("Have Cues in file");
        if (ret == 0) {
            long len;
            ret = mSegment->LoadCluster(pos, len);
            if (ret >= 1) {
                // no more clusters
                ret = 0;
            }
         } else if (ret > 0) {
            ret = mkvparser::E_BUFFER_NOT_FULL;
         }
    } else {
        ALOGD("Load all clusters, may waste some sec");
        ret = mSegment->Load();
    }
#else
    if (ret == 0) {
        long len;
        ret = mSegment->LoadCluster(pos, len);
        if (ret >= 1) {
            // no more clusters
            ret = 0;
        }
    } else if (ret > 0) {
        ret = mkvparser::E_BUFFER_NOT_FULL;
    }
#endif
    if (ret < 0) {
        ALOGW("Corrupt %s source: %s", mIsWebm ? "webm" : "matroska",
                uriDebugString(mDataSource->getUri()).c_str());
        delete mSegment;
        mSegment = NULL;
        return;
    }

#if 0
    const mkvparser::SegmentInfo *info = mSegment->GetInfo();
    ALOGI("muxing app: %s, writing app: %s",
         info->GetMuxingAppAsUTF8(),
         info->GetWritingAppAsUTF8());
#endif
#ifdef RTK_PLATFORM
    mIsVFWFOURCC = 0;
#endif

    addTracks();
}

MatroskaExtractor::~MatroskaExtractor() {
    delete mSegment;
    mSegment = NULL;

    delete mReader;
    mReader = NULL;
}

size_t MatroskaExtractor::countTracks() {
    return mTracks.size();
}

sp<IMediaSource> MatroskaExtractor::getTrack(size_t index) {
    if (index >= mTracks.size()) {
        return NULL;
    }

    return new MatroskaSource(this, index);
}

sp<MetaData> MatroskaExtractor::getTrackMetaData(
        size_t index, uint32_t flags) {
    if (index >= mTracks.size()) {
        return NULL;
    }

    if ((flags & kIncludeExtensiveMetaData) && !mExtractedThumbnails
            && !isLiveStreaming()) {
        findThumbnails();
        mExtractedThumbnails = true;
    }

    return mTracks.itemAt(index).mMeta;
}

bool MatroskaExtractor::isLiveStreaming() const {
    return mIsLiveStreaming;
}

static int bytesForSize(size_t size) {
    // use at most 28 bits (4 times 7)
    CHECK(size <= 0xfffffff);

    if (size > 0x1fffff) {
        return 4;
    } else if (size > 0x3fff) {
        return 3;
    } else if (size > 0x7f) {
        return 2;
    }
    return 1;
}

static void storeSize(uint8_t *data, size_t &idx, size_t size) {
    int numBytes = bytesForSize(size);
    idx += numBytes;

    data += idx;
    size_t next = 0;
    while (numBytes--) {
        *--data = (size & 0x7f) | next;
        size >>= 7;
        next = 0x80;
    }
}

static void addESDSFromCodecPrivate(
        const sp<MetaData> &meta,
        bool isAudio, const void *priv, size_t privSize) {

    int privSizeBytesRequired = bytesForSize(privSize);
    int esdsSize2 = 14 + privSizeBytesRequired + privSize;
    int esdsSize2BytesRequired = bytesForSize(esdsSize2);
    int esdsSize1 = 4 + esdsSize2BytesRequired + esdsSize2;
    int esdsSize1BytesRequired = bytesForSize(esdsSize1);
    size_t esdsSize = 1 + esdsSize1BytesRequired + esdsSize1;
    uint8_t *esds = new uint8_t[esdsSize];

    size_t idx = 0;
    esds[idx++] = 0x03;
    storeSize(esds, idx, esdsSize1);
    esds[idx++] = 0x00; // ES_ID
    esds[idx++] = 0x00; // ES_ID
    esds[idx++] = 0x00; // streamDependenceFlag, URL_Flag, OCRstreamFlag
    esds[idx++] = 0x04;
    storeSize(esds, idx, esdsSize2);
    esds[idx++] = isAudio ? 0x40   // Audio ISO/IEC 14496-3
                          : 0x20;  // Visual ISO/IEC 14496-2
    for (int i = 0; i < 12; i++) {
        esds[idx++] = 0x00;
    }
    esds[idx++] = 0x05;
    storeSize(esds, idx, privSize);
    memcpy(esds + idx, priv, privSize);

    meta->setData(kKeyESDS, 0, esds, esdsSize);

    delete[] esds;
    esds = NULL;
}

status_t addVorbisCodecInfo(
        const sp<MetaData> &meta,
        const void *_codecPrivate, size_t codecPrivateSize) {
    // hexdump(_codecPrivate, codecPrivateSize);

    if (codecPrivateSize < 1) {
        return ERROR_MALFORMED;
    }

    const uint8_t *codecPrivate = (const uint8_t *)_codecPrivate;

    if (codecPrivate[0] != 0x02) {
        return ERROR_MALFORMED;
    }

    // codecInfo starts with two lengths, len1 and len2, that are
    // "Xiph-style-lacing encoded"...

    size_t offset = 1;
    size_t len1 = 0;
    while (offset < codecPrivateSize && codecPrivate[offset] == 0xff) {
        if (len1 > (SIZE_MAX - 0xff)) {
            return ERROR_MALFORMED; // would overflow
        }
        len1 += 0xff;
        ++offset;
    }
    if (offset >= codecPrivateSize) {
        return ERROR_MALFORMED;
    }
    if (len1 > (SIZE_MAX - codecPrivate[offset])) {
        return ERROR_MALFORMED; // would overflow
    }
    len1 += codecPrivate[offset++];

    size_t len2 = 0;
    while (offset < codecPrivateSize && codecPrivate[offset] == 0xff) {
        if (len2 > (SIZE_MAX - 0xff)) {
            return ERROR_MALFORMED; // would overflow
        }
        len2 += 0xff;
        ++offset;
    }
    if (offset >= codecPrivateSize) {
        return ERROR_MALFORMED;
    }
    if (len2 > (SIZE_MAX - codecPrivate[offset])) {
        return ERROR_MALFORMED; // would overflow
    }
    len2 += codecPrivate[offset++];

    if (len1 > SIZE_MAX - len2 || offset > SIZE_MAX - (len1 + len2) ||
            codecPrivateSize < offset + len1 + len2) {
        return ERROR_MALFORMED;
    }

    if (codecPrivate[offset] != 0x01) {
        return ERROR_MALFORMED;
    }
    meta->setData(kKeyVorbisInfo, 0, &codecPrivate[offset], len1);

    offset += len1;
    if (codecPrivate[offset] != 0x03) {
        return ERROR_MALFORMED;
    }

    offset += len2;
    if (codecPrivate[offset] != 0x05) {
        return ERROR_MALFORMED;
    }

    meta->setData(
            kKeyVorbisBooks, 0, &codecPrivate[offset],
            codecPrivateSize - offset);

    return OK;
}

status_t MatroskaExtractor::synthesizeAVCC(TrackInfo *trackInfo, size_t index) {
    BlockIterator iter(this, trackInfo->mTrackNum, index);
    if (iter.eos()) {
        return ERROR_MALFORMED;
    }

    const mkvparser::Block *block = iter.block();
    if (block->GetFrameCount() <= 0) {
        return ERROR_MALFORMED;
    }

    const mkvparser::Block::Frame &frame = block->GetFrame(0);
    sp<ABuffer> abuf = new ABuffer(frame.len);
    long n = frame.Read(mReader, abuf->data());
    if (n != 0) {
        return ERROR_MALFORMED;
    }

    sp<MetaData> avcMeta = MakeAVCCodecSpecificData(abuf);
    if (avcMeta == NULL) {
        return ERROR_MALFORMED;
    }

    // Override the synthesized nal length size, which is arbitrary
    avcMeta->setInt32(kKeyNalLengthSize, 0);
    trackInfo->mMeta = avcMeta;
    return OK;
}

static inline bool isValidInt32ColourValue(long long value) {
    return value != mkvparser::Colour::kValueNotPresent
            && value >= INT32_MIN
            && value <= INT32_MAX;
}

static inline bool isValidUint16ColourValue(long long value) {
    return value != mkvparser::Colour::kValueNotPresent
            && value >= 0
            && value <= UINT16_MAX;
}

static inline bool isValidPrimary(const mkvparser::PrimaryChromaticity *primary) {
    return primary != NULL && primary->x >= 0 && primary->x <= 1
             && primary->y >= 0 && primary->y <= 1;
}

void MatroskaExtractor::getColorInformation(
        const mkvparser::VideoTrack *vtrack, sp<MetaData> &meta) {
    const mkvparser::Colour *color = vtrack->GetColour();
    if (color == NULL) {
        return;
    }

    // Color Aspects
    {
        int32_t primaries = 2; // ISO unspecified
        int32_t transfer = 2; // ISO unspecified
        int32_t coeffs = 2; // ISO unspecified
        bool fullRange = false; // default
        bool rangeSpecified = false;

        if (isValidInt32ColourValue(color->primaries)) {
            primaries = color->primaries;
        }
        if (isValidInt32ColourValue(color->transfer_characteristics)) {
            transfer = color->transfer_characteristics;
        }
        if (isValidInt32ColourValue(color->matrix_coefficients)) {
            coeffs = color->matrix_coefficients;
        }
        if (color->range != mkvparser::Colour::kValueNotPresent
                && color->range != 0 /* MKV unspecified */) {
            // We only support MKV broadcast range (== limited) and full range.
            // We treat all other value as the default limited range.
            fullRange = color->range == 2 /* MKV fullRange */;
            rangeSpecified = true;
        }

        ColorAspects aspects;
        ColorUtils::convertIsoColorAspectsToCodecAspects(
                primaries, transfer, coeffs, fullRange, aspects);
        meta->setInt32(kKeyColorPrimaries, aspects.mPrimaries);
        meta->setInt32(kKeyTransferFunction, aspects.mTransfer);
        meta->setInt32(kKeyColorMatrix, aspects.mMatrixCoeffs);
        meta->setInt32(
                kKeyColorRange, rangeSpecified ? aspects.mRange : ColorAspects::RangeUnspecified);
    }

    // HDR Static Info
    {
        HDRStaticInfo info, nullInfo; // nullInfo is a fully unspecified static info
        memset(&info, 0, sizeof(info));
        memset(&nullInfo, 0, sizeof(nullInfo));
        if (isValidUint16ColourValue(color->max_cll)) {
            info.sType1.mMaxContentLightLevel = color->max_cll;
        }
        if (isValidUint16ColourValue(color->max_fall)) {
            info.sType1.mMaxFrameAverageLightLevel = color->max_fall;
        }
        const mkvparser::MasteringMetadata *mastering = color->mastering_metadata;
        if (mastering != NULL) {
            // Convert matroska values to HDRStaticInfo equivalent values for each fully specified
            // group. See CTA-681.3 section 3.2.1 for more info.
            if (mastering->luminance_max >= 0.5 && mastering->luminance_max < 65535.5) {
                info.sType1.mMaxDisplayLuminance = (uint16_t)(mastering->luminance_max + 0.5);
            }
            if (mastering->luminance_min >= 0.00005 && mastering->luminance_min < 6.55355) {
                // HDRStaticInfo Type1 stores min luminance scaled 10000:1
                info.sType1.mMinDisplayLuminance =
                    (uint16_t)(10000 * mastering->luminance_min + 0.5);
            }
            // HDRStaticInfo Type1 stores primaries scaled 50000:1
            if (isValidPrimary(mastering->white_point)) {
                info.sType1.mW.x = (uint16_t)(50000 * mastering->white_point->x + 0.5);
                info.sType1.mW.y = (uint16_t)(50000 * mastering->white_point->y + 0.5);
            }
            if (isValidPrimary(mastering->r) && isValidPrimary(mastering->g)
                    && isValidPrimary(mastering->b)) {
                info.sType1.mR.x = (uint16_t)(50000 * mastering->r->x + 0.5);
                info.sType1.mR.y = (uint16_t)(50000 * mastering->r->y + 0.5);
                info.sType1.mG.x = (uint16_t)(50000 * mastering->g->x + 0.5);
                info.sType1.mG.y = (uint16_t)(50000 * mastering->g->y + 0.5);
                info.sType1.mB.x = (uint16_t)(50000 * mastering->b->x + 0.5);
                info.sType1.mB.y = (uint16_t)(50000 * mastering->b->y + 0.5);
            }
        }
        // Only advertise static info if at least one of the groups have been specified.
        if (memcmp(&info, &nullInfo, sizeof(info)) != 0) {
            info.mID = HDRStaticInfo::kType1;
            meta->setData(kKeyHdrStaticInfo, 'hdrS', &info, sizeof(info));
        }
    }
}

void MatroskaExtractor::addTracks() {
    const mkvparser::Tracks *tracks = mSegment->GetTracks();

#ifdef RTK_PLATFORM
    int needstoppb = 0;
#endif
    for (size_t index = 0; index < tracks->GetTracksCount(); ++index) {
        const mkvparser::Track *track = tracks->GetTrackByIndex(index);
#ifdef RTK_PLATFORM
        int ContentCompSettingsSize = 0;
#endif

        if (track == NULL) {
            // Apparently this is currently valid (if unexpected) behaviour
            // of the mkv parser lib.
            continue;
        }

        const char *const codecID = track->GetCodecId();
        ALOGV("codec id = %s", codecID);
        ALOGV("codec name = %s", track->GetCodecNameAsUTF8());

        if (codecID == NULL) {
            ALOGW("unknown codecID is not supported.");
            continue;
        }

        size_t codecPrivateSize;
        const unsigned char *codecPrivate =
            track->GetCodecPrivate(codecPrivateSize);

        enum { VIDEO_TRACK = 1, AUDIO_TRACK = 2 };

        sp<MetaData> meta = new MetaData;

        status_t err = OK;
#ifdef RTK_PLATFORM
        ContentCompSettingsSize = track->m_ContentCompSettings_len;
        //ALOGD("\033[0;32m [%d]  codecID= %s   m_ContentCompSettings_len= %lld\033[m\n", __LINE__, codecID, track->m_ContentCompSettings_len);
#endif
        switch (track->GetType()) {
            case VIDEO_TRACK:
            {
                const mkvparser::VideoTrack *vtrack =
                    static_cast<const mkvparser::VideoTrack *>(track);

                if (!strcmp("V_MPEG4/ISO/AVC", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVC);
                    meta->setData(kKeyAVCC, 0, codecPrivate, codecPrivateSize);
#ifdef RTK_PLATFORM // for debug
                    ALOGD("\033[0;32m[%d] Set AVC private data, codecPrivateSize= %d\033[m\n", __LINE__, (int)codecPrivateSize);
                    //int k=0;
                    //for (k=0; k< (int)codecPrivateSize; k++)
                    //    ALOGD("\033[0;36m [%d]  codecPrivate[%d]= 0x%.2X\033[m\n", __LINE__, k, codecPrivate[k]);
#endif
                } else if (!strcmp("V_MPEG4/ISO/ASP", codecID)) {
                    if (codecPrivateSize > 0) {
                        meta->setCString(
                                kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_MPEG4);
                        addESDSFromCodecPrivate(
                                meta, false, codecPrivate, codecPrivateSize);
                    } else {
                        ALOGW("%s is detected, but does not have configuration.",
                                codecID);
                        continue;
                    }
                } else if (!strcmp("V_VP8", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_VP8);
                } else if (!strcmp("V_VP9", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_VP9);
                    if (codecPrivateSize > 0) {
                      // 'csd-0' for VP9 is the Blob of Codec Private data as
                      // specified in http://www.webmproject.org/vp9/profiles/.
                      meta->setData(
                              kKeyVp9CodecPrivate, 0, codecPrivate,
                              codecPrivateSize);
                    }
                }
#ifdef RTK_PLATFORM
                else if(!strcmp("V_MPEG2", codecID))
                {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_MPEG2);
                }
                else if(!strcmp("V_MJPEG", codecID))
                {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_MJPEG);
                }
                else if (!strcmp("V_REAL/RV40", codecID))
                {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_RV40);
                }
                else if (!strcmp("V_MPEGH/ISO/HEVC", codecID)) {
                    ALOGV("[%s][%d] V_MPEGH/ISO/HEVC %zu \n" ,__FUNCTION__,__LINE__,codecPrivateSize);
//                    hexdump(codecPrivate ,codecPrivateSize );
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_HEVC);
                    if(codecPrivateSize >= 7)
                    {
                        meta->setData(kKeyHVCC, 0, codecPrivate, codecPrivateSize);
                    }else if(codecPrivateSize == 0)
                    {
                        ALOGI("[%s][%d] V_MPEGH/ISO/HEVC codecPrivateSize:: %zu \n" ,__FUNCTION__,__LINE__,codecPrivateSize);
                    }else
                        continue;
                }
                else if(!strcmp("V_MS/VFW/FOURCC",codecID)){
                    ALOGV("[%s][%d] V_MS/VFW/FOURCC::  %zu \n" ,__FUNCTION__,__LINE__,codecPrivateSize);
//                    hexdump(codecPrivate ,codecPrivateSize );
                    BITMAPINFOHEADER *pVideoFormat = (BITMAPINFOHEADER *)codecPrivate;

                    switch(pVideoFormat->biCompression)
                    {
                        case FCC('DIV3'):
                        case FCC('MP43'):
                            meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_DIVX3);
                            needstoppb = 1;
                            break;
                        case FCC('avc1'):
                        case FCC('X264'):
                        case FCC('H264'):
                        case FCC('AVC1'):
                        case FCC('h264'):
                            meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVC);
                            mIsVFWFOURCC = 1;
                            break;
                        case FCC('DIV4'):
                        case FCC('DIVX'):
                        case FCC('DX50'):
                        case FCC('divx'):
                            needstoppb = 1;
                        case FCC('M4S2'):
                        case FCC('MP4S'):
                        case FCC('MPG4'):
                        case FCC('XVID'):
                        case FCC('xvid'):
                        case FCC('FMP4'):
                        case FCC('RMP4'):
                        case FCC('MP4V'):
                            meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_MPEG4);
                            break;
                        case FCC('MPEG'):
                        case FCC('MPG2'):
                        case FCC('MPG1'):
                            meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_MPEG2);
                            addESDSFromCodecPrivate(
                                    meta, false, codecPrivate, codecPrivateSize);
                            break;
                        case FCC('WMVA'):
                        case FCC('WVC1'):
                            meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_WVC1);
                            break;
                        case FCC('H263'):
                            meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_H263);
                            break;
                        default:
                            ALOGW("%s %x is not supported.", codecID,pVideoFormat->biCompression);
                            continue;
                    }

                }
#endif
                else {
                    ALOGW("%s is not supported.", codecID);
                    continue;
                }

                meta->setInt32(kKeyWidth, vtrack->GetWidth());
                meta->setInt32(kKeyHeight, vtrack->GetHeight());
#ifdef RTK_PLATFORM
                if(vtrack->GetFrameRate() > 0)
                    meta->setFloat(kKeyFrameRateFloat,(float)vtrack->GetFrameRate());
#endif

                getColorInformation(vtrack, meta);

                break;
            }

            case AUDIO_TRACK:
            {
                const mkvparser::AudioTrack *atrack =
                    static_cast<const mkvparser::AudioTrack *>(track);

                if (!strcmp("A_AAC", codecID)) {
#ifdef RTK_PLATFORM
                    int objectTypeIndex = (int)((codecPrivate[0] >> 3) & 0x1F);
                    if (objectTypeIndex == 1) {
                        meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AAC_MAIN);
                    } else {
                        meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AAC);
                    }
#else
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AAC);
#endif
                    CHECK(codecPrivateSize >= 2);

                    addESDSFromCodecPrivate(
                            meta, true, codecPrivate, codecPrivateSize);
                } else if (!strcmp("A_VORBIS", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_VORBIS);

                    err = addVorbisCodecInfo(
                            meta, codecPrivate, codecPrivateSize);
                } else if (!strcmp("A_OPUS", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_OPUS);
                    meta->setData(kKeyOpusHeader, 0, codecPrivate, codecPrivateSize);
                    meta->setInt64(kKeyOpusCodecDelay, track->GetCodecDelay());
                    meta->setInt64(kKeyOpusSeekPreRoll, track->GetSeekPreRoll());
                    mSeekPreRollNs = track->GetSeekPreRoll();
                } else if (!strcmp("A_MPEG/L3", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MPEG);
                } else if (!strcmp("A_FLAC", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_FLAC);
                    addESDSFromCodecPrivate(
                            meta, true, codecPrivate, codecPrivateSize);
                }
#ifdef RTK_PLATFORM
                else if (!strcmp("A_MPEG/L2", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_II);
                }else if (!strcmp("A_AC3", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AC3);
                } else if (!strcmp("A_DTS", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_DTS);
                } else if (!strcmp("A_EAC3", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AC3);
                } else if(!strcmp("A_TRUEHD", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_MLP);
                } else if(!strcmp("A_PCM/INT/LIT", codecID)) {
                    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_RAW);
                }
#endif
                else {
                    ALOGW("%s is not supported.", codecID);
                    continue;
                }

                meta->setInt32(kKeySampleRate, (int32_t)atrack->GetSamplingRate());
                meta->setInt32(kKeyChannelCount, (int32_t)atrack->GetChannels());
                break;
            }

            default:
                continue;
        }

        if (err != OK) {
            ALOGE("skipping track, codec specific data was malformed.");
            continue;
        }

        long long durationNs = mSegment->GetDuration();
        meta->setInt64(kKeyDuration, (durationNs + 500) / 1000);

        mTracks.push();
        size_t n = mTracks.size() - 1;
        TrackInfo *trackInfo = &mTracks.editItemAt(n);
        trackInfo->mTrackNum = track->GetNumber();
        trackInfo->mMeta = meta;
#ifdef RTK_PLATFORM
        trackInfo->mContentCompSettingLen = ContentCompSettingsSize;
#endif
        trackInfo->mExtractor = this;

        trackInfo->mEncrypted = false;
        for(size_t i = 0; i < track->GetContentEncodingCount() && !trackInfo->mEncrypted; i++) {
            const mkvparser::ContentEncoding *encoding = track->GetContentEncodingByIndex(i);
            for(size_t j = 0; j < encoding->GetEncryptionCount(); j++) {
                const mkvparser::ContentEncoding::ContentEncryption *encryption;
                encryption = encoding->GetEncryptionByIndex(j);
                meta->setData(kKeyCryptoKey, 0, encryption->key_id, encryption->key_id_len);
                trackInfo->mEncrypted = true;
                break;
            }
        }

        if (!strcmp("V_MPEG4/ISO/AVC", codecID) && codecPrivateSize == 0) {
            // Attempt to recover from AVC track without codec private data
            err = synthesizeAVCC(trackInfo, n);
            if (err != OK) {
                mTracks.pop();
            }
        }
        if(needstoppb == 1)
        {
            meta.clear();
            meta = new MetaData;
            trackInfo->mMeta = meta;
            trackInfo->mTrackNum = 0;
            mTracks.clear();
        }
    }
}

void MatroskaExtractor::findThumbnails() {
    for (size_t i = 0; i < mTracks.size(); ++i) {
        TrackInfo *info = &mTracks.editItemAt(i);

        const char *mime;
        CHECK(info->mMeta->findCString(kKeyMIMEType, &mime));

        if (strncasecmp(mime, "video/", 6)) {
            continue;
        }

        BlockIterator iter(this, info->mTrackNum, i);
        int32_t j = 0;
        int64_t thumbnailTimeUs = 0;
        size_t maxBlockSize = 0;
        while (!iter.eos() && j < 20) {
            if (iter.block()->IsKey()) {
                ++j;

                size_t blockSize = 0;
                for (int k = 0; k < iter.block()->GetFrameCount(); ++k) {
                    blockSize += iter.block()->GetFrame(k).len;
                }

                if (blockSize > maxBlockSize) {
                    maxBlockSize = blockSize;
                    thumbnailTimeUs = iter.blockTimeUs();
                }
            }
            iter.advance();
        }
        info->mMeta->setInt64(kKeyThumbnailTime, thumbnailTimeUs);
    }
}

sp<MetaData> MatroskaExtractor::getMetaData() {
    sp<MetaData> meta = new MetaData;

    meta->setCString(
            kKeyMIMEType,
            mIsWebm ? "video/webm" : MEDIA_MIMETYPE_CONTAINER_MATROSKA);

    return meta;
}

uint32_t MatroskaExtractor::flags() const {
    uint32_t x = CAN_PAUSE;
    if (!isLiveStreaming()) {
        x |= CAN_SEEK_BACKWARD | CAN_SEEK_FORWARD | CAN_SEEK;
    }

    return x;
}

bool SniffMatroska(
        const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *) {
    DataSourceReader reader(source);
    mkvparser::EBMLHeader ebmlHeader;
    long long pos;
    if (ebmlHeader.Parse(&reader, pos) < 0) {
        return false;
    }

    mimeType->setTo(MEDIA_MIMETYPE_CONTAINER_MATROSKA);
    *confidence = 0.6;

    return true;
}

}  // namespace android

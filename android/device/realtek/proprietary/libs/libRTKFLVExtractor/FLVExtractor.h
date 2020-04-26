#ifndef FLV_EXTRACTOR_H_

#define FLV_EXTRACTOR_H_

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/IMediaSource.h>
#include <utils/Vector.h>

namespace android {

struct FLVExtractor : public MediaExtractor {
    FLVExtractor(const sp<DataSource> &dataSource);

    virtual size_t countTracks();

    virtual sp<IMediaSource> getTrack(size_t index);

    virtual sp<MetaData> getTrackMetaData(
            size_t index, uint32_t flags);

    virtual sp<MetaData> getMetaData();

protected:
    virtual ~FLVExtractor();

private:
    struct FLVSource;

    struct Track {
        sp<MetaData> mMeta;

        enum Kind {
            AUDIO,
            VIDEO,
            OTHER
        } mKind;

        size_t mMaxSampleSize;

        // 0: not H26x, 1: H264, 2: H265
        uint32_t mIsH26X;

        // 0: not AAC, 1: AAC
        uint32_t mIsAAC;
    };

    /*  FLV use start*/
    //return code
    #define RET_OK 0
    #define RET_FAIL -1
    #define RET_IGNORE -2
    #define RET_WAIT -3
    #define FLV_MAX_STRING_LEN 512

    #define FLV_TYPE_NUMBER 0x00
    #define FLV_TYPE_BOOL 0x01
    #define FLV_TYPE_STRING 0x02
    #define FLV_TYPE_OBJECT 0x03
    #define FLV_TYPE_MOVIECLIP 0x04
    #define FLV_TYPE_NULL 0x05
    #define FLV_TYPE_UNDEF 0x06
    #define FLV_TYPE_REFERENCE 0x07
    #define FLV_TYPE_ECMAARRAY 0x08
    #define FLV_TYPE_STRICTARRAY 0x0a
    #define FLV_TYPE_DATE 0x0b
    #define FLV_TYPE_LONGSTRING 0x0c

    typedef struct _IdxEntry
    {
        uint64_t pos;
        uint64_t timecode;
    }IdxEntry;

    /*--------------------------------------------------------------
        FLV file format stuff
    ---------------------------------------------------------------*/
    long long streamTotalBytes;
    long long currReadPosition;
    bool bAudioExist;
    bool bVideoExist;
    int64_t tagStart; //position of the first audio/video tag
    uint64_t TimecodeStart; // timecode of the first block
    unsigned int fileDuration; //in miliseconds
    double mbitRate;

    char *mAudioCodecPrivate; //Private data only known to the codec.
    unsigned int mAudioCodecPrivateLen; //length of private data
    char *mVideoCodecPrivate; //Private data only known to the codec.
    unsigned int mVideoCodecPrivateLen; //length of private data

    /*To Make seek quicker
    every 10 seconds will be a KeyFrame Index*/
    IdxEntry *keyFrameCache;
    unsigned int nKeyFrame;
    unsigned int nKeyFrameMax;
    bool bCreate_Index_Done;
    bool bVP6Alpha;
    unsigned int VideoMaxFrameSize;
    unsigned int AudioMaxFrameSize;

    unsigned int VideoWidth;
    unsigned int VideoHeight;
    unsigned int nalu_length_size;
    double VideoFPS;
    bool bHttpFlv; // for streaming
    bool bHttpVOD;
    long long LastSearchIOffset;

    #define getbits mp_getbits
    #define find_min(a, b) ((a) <= (b) ? (a) : (b))
    unsigned char mp_getbits(unsigned char *buffer, unsigned int from, unsigned char len);
    unsigned int read_golomb(unsigned char *buffer, unsigned int *init);
    int read_golomb_s(unsigned char *buffer, unsigned int *init);
    int find_avc1_resolution(unsigned char *in_buf, unsigned int *w, unsigned int *h);

    ssize_t flvReadBytes(off64_t offset, void* pBuffer, size_t size);
    int flvReadUINT(unsigned long long *retInteger, unsigned int len);
    int flvParseString(char *str);
    int flvReadDouble(double *retDouble);
    int flvParseVariableEnd();
    int flvParseLongString(char *str);
    int flvParseDate();
    int flvParseObjectEnd();
    int flvParseObject(long long tagTail);

    int flvParseValue(char *valName, long long tagTail);
    int flvSkip(unsigned int len);
    int flvReadTimecode(unsigned int *retInteger);
    int flvParseAMF(long long tagTail);
    int flvGetFileDuration();
    int flvReadNextTag(long long tagpos, char *tagtype, unsigned long long *taglen, unsigned int *tagtime, char *isKey);
    int flvParseAVformat();
    status_t flvParseHeader();
    /*  FLV use end*/

    sp<DataSource> mDataSource;
    status_t mInitCheck;
    Vector<Track> mTracks;


    /*--------------------------------------------------------------
        IO stuff
    ---------------------------------------------------------------*/
    long long LastVideoReadOffset;
    long long LastAudioReadOffset;

    /*--------------------------------------------------------------
        Playback information
    ---------------------------------------------------------------*/
    bool isSeekVideoOrAudio;

    status_t flv_playback_atTime(
            size_t trackIndex,
            int64_t timeUs, IMediaSource::ReadOptions::SeekMode mode,
            long long *seektofileoffset);
};

class String8;
struct AMessage;

}  // namespace android

#endif  // ASF_EXTRACTOR_H_

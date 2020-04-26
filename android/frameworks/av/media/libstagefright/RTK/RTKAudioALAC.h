#ifndef RTK_AUDIO_ALAC_H
#define RTK_AUDIO_ALAC_H

#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/MediaBufferGroup.h>

namespace android 
{

////////////
// define //
////////////
#define TARGET_RT_LITTLE_ENDIAN 1
#define kMinCAFFPacketTableHeaderSize 24
#define kMaxBERSize 5
#define OFFSET_TABLE_LEN    300
#define RTK_ALAC_HEADER "caff"
#define RTK_ALAC_HEADER_LEN 4
#define RTK_ALAC_MAX_BUF 65536

// for audio firmware
#define ALAC_SYNCH          0x75786A70
#define ALAC_SYNCH_ASCII    "uxjp"
#define ALAC_HEADER_SIZE    0x8     // synch_word(4 bytes) + frame_length(4 bytes)
    
///////////
// macro //
///////////
#define BSWAP16(x) (((x << 8) | ((x >> 8) & 0x00ff)))
#define BSWAP32(x) (((x << 24) | ((x << 8) & 0x00ff0000) | ((x >> 8) & 0x0000ff00) | ((x >> 24) & 0x000000ff)))
#define BSWAP64(x) ((((int64_t)x << 56) | (((int64_t)x << 40) & 0x00ff000000000000LL) | \
                    (((int64_t)x << 24) & 0x0000ff0000000000LL) | (((int64_t)x << 8) & 0x000000ff00000000LL) | \
                    (((int64_t)x >> 8) & 0x00000000ff000000LL) | (((int64_t)x >> 24) & 0x0000000000ff0000LL) | \
                    (((int64_t)x >> 40) & 0x000000000000ff00LL) | (((int64_t)x >> 56) & 0x00000000000000ffLL)))
#define RequireAction(condition, action) if (!(condition)) { action }

//////////
// enum //
//////////
enum
{
    kALACFormatAppleLossless = 'alac',
    kALACFormatLinearPCM = 'lpcm'
};

enum
{
	kALACCodecFormat		= 'alac',
	kALACVersion			= 0,
	kALACCompatibleVersion	= kALACVersion,
	kALACDefaultFrameSize	= 4096
};

enum 
{
    kALAC_UnimplementedError   = -4,
    kALAC_FileNotFoundError    = -43,
    kALAC_ParamError           = -50,
    kALAC_MemFullError         = -108
};

enum
{
    ALAC_noErr = 0
};

typedef double alac_float64_t;

typedef struct
{
    uint32_t          mFileType;			// 'caff'
    uint16_t			mFileVersion;		//initial revision set to 1
    uint16_t			mFileFlags;			//initial revision set to 0
} port_CAFFileHeader;

// Every file MUST have this chunk. It MUST be the first chunk in the file
typedef struct 
{
    double mSampleRate;
    uint32_t  mFormatID;
    uint32_t  mFormatFlags;
    uint32_t  mBytesPerPacket;
    uint32_t  mFramesPerPacket;
    uint32_t  mChannelsPerFrame;
    uint32_t  mBitsPerChannel;
} port_CAFAudioDescription;

struct AudioFormatDescription
{
    alac_float64_t mSampleRate;
    uint32_t  mFormatID;
    uint32_t  mFormatFlags;
    uint32_t  mBytesPerPacket;
    uint32_t  mFramesPerPacket;
    uint32_t  mBytesPerFrame;
    uint32_t  mChannelsPerFrame;
    uint32_t  mBitsPerChannel;
    uint32_t  mReserved;
};
typedef struct AudioFormatDescription  AudioFormatDescription;

typedef struct ALACSpecificConfig
{
	uint32_t				frameLength;
	uint8_t					compatibleVersion;
	uint8_t					bitDepth;							// max 32
	uint8_t					pb;									// 0 <= pb <= 255
	uint8_t					mb;
	uint8_t					kb;
	uint8_t					numChannels;
	uint16_t				maxRun;
	uint32_t				maxFrameBytes;
	uint32_t				avgBitRate;
	uint32_t				sampleRate;

} ALACSpecificConfig;

class ALACSource : public MediaSource {
public:
    ALACSource(const sp<DataSource> &source,
              const sp<MetaData> &meta,
              const off64_t *offset_table,
              const off64_t *pkt_size_offset_table,
              size_t offset_table_length,
              off64_t mInputDataPos,
              off64_t mPacketTablePos,
              unsigned int us);

    virtual status_t start(MetaData *params = NULL);
    virtual status_t stop();

    virtual sp<MetaData> getFormat();

    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options = NULL);

protected:
    virtual ~ALACSource();

private:
    sp<DataSource> mDataSource;
    sp<MetaData> mMeta;

    off64_t mOffset;
    off64_t mTblOffset;
    off64_t mInputDataPos;
    off64_t mPacketTablePos;
    int64_t mCurrentTimeUs;
    bool mStarted;
    MediaBufferGroup *mGroup;
    off64_t mOffsetTable[OFFSET_TABLE_LEN];
    off64_t mPktSizeOffsetTable[OFFSET_TABLE_LEN];
    size_t mOffsetTableLength;
    unsigned int us_per_frame;
    unsigned int mFrameCnt;

    ALACSource(const ALACSource &);
    ALACSource &operator=(const ALACSource &);
};

class RTK_ALAC
{
public:
    AudioFormatDescription inputFormat;
    ALACSpecificConfig mConfig;
    unsigned int nNumCh;
    unsigned int nSampleRate;
    unsigned int us_per_frame;
    sp<DataSource> mDataSource;
    off64_t mOffset;
    int32_t mInputDataPos;
    int32_t mInputDataSize;
    int32_t mPacketTablePos;
    int32_t mThePacketTableSize;
    size_t mTotalFrames;

    size_t mOffsetTableLength;  // for seek
    off64_t mOffsetTable[OFFSET_TABLE_LEN]; // for seek
    off64_t mPktSizeOffsetTable[OFFSET_TABLE_LEN]; // for seek
    
    // member function
    RTK_ALAC(sp<DataSource> source);
    ~RTK_ALAC();
    bool GetCAFFdescFormat(/*FILE * inputFile,*/ AudioFormatDescription * theInputFormat);
    void ALAC_init();
    bool FindCAFFDataStart(/*FILE * inputFile, */int32_t * dataPos, int32_t * dataSize);
    uint32_t GetMagicCookieSizeFromCAFFkuki(/*FILE * inputFile*/);
    int32_t GetMagicCookieFromCAFFkuki(/*FILE * inputFile, */uint8_t * outMagicCookie, uint32_t * ioMagicCookieSize);
    void PrintCookie(ALACSpecificConfig *pCookie);
    int32_t Internal_Init( void * inMagicCookie, uint32_t inMagicCookieSize );
    int32_t FindCAFFPacketTableStart(/*FILE * inputFile, */int32_t * paktPos, int32_t * paktSize);
    int64_t EstimateDuration();

};

uint32_t ReadBERInteger(uint8_t * theInputBuffer, int32_t * ioNumBytes);
uint16_t Swap16BtoN(uint16_t inUInt16);
uint32_t Swap32BtoN(uint32_t inUInt32);
double SwapFloat64BtoN(double in);

}   // namespace android {

#endif



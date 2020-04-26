#define LOG_TAG "RTKAudioALAC"
#include <utils/Log.h>

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <utils/String8.h>

#include "RTKAudioALAC.h"

namespace android {

#ifdef TRACE_EN
#define TRACE_PRINT(fmt, args...)   ALOGD("@ %d %s %d:"fmt,gettid(), __FUNCTION__, __LINE__, ##args);
#else
#define TRACE_PRINT(fmt, args...)   do{ }while(0)
#endif

uint16_t Swap16BtoN(uint16_t inUInt16)
{
#if TARGET_RT_LITTLE_ENDIAN
    return BSWAP16(inUInt16);
#else
    return inUInt16;
#endif
}

uint32_t Swap32BtoN(uint32_t inUInt32)
{
#if TARGET_RT_LITTLE_ENDIAN
    return BSWAP32(inUInt32);
#else
    return inUInt32;
#endif
}

double SwapFloat64BtoN(double in)
{
#if TARGET_RT_LITTLE_ENDIAN
	union {
		double f;
		int64_t i;
	} x;
	x.f = in;	
	x.i = BSWAP64(x.i);
	return x.f;
#else
	return in;
#endif
}


bool RTK_ALAC::GetCAFFdescFormat(/*FILE * inputFile,*/ AudioFormatDescription * theInputFormat)
{
    bool done = false;
    uint32_t theChunkSize = 0, theChunkType = 0;
    uint8_t theReadBuffer[32];

//    fseek(inputFile, 4, SEEK_CUR); // skip 4 bytes
    mOffset = 8;

    while (!done)
    {
//        fread(theReadBuffer, 1, 4, inputFile);
        mDataSource->readAt(mOffset, theReadBuffer, 4);
        mOffset += 4;
        theChunkType = ((int32_t)(theReadBuffer[0]) << 24) + ((int32_t)(theReadBuffer[1]) << 16) + ((int32_t)(theReadBuffer[2]) << 8) + theReadBuffer[3];
        switch (theChunkType)
        {
            case 'desc':
//                fseek(inputFile, 8, SEEK_CUR); // skip 8 bytes
//                fread(theReadBuffer, 1, sizeof(port_CAFAudioDescription), inputFile);
                mOffset += 8;
                mDataSource->readAt(mOffset, theReadBuffer, sizeof(port_CAFAudioDescription));
                mOffset += sizeof(port_CAFAudioDescription);
                theInputFormat->mFormatID = Swap32BtoN(((port_CAFAudioDescription *)(theReadBuffer))->mFormatID);
                theInputFormat->mChannelsPerFrame = Swap32BtoN(((port_CAFAudioDescription *)(theReadBuffer))->mChannelsPerFrame);
                theInputFormat->mSampleRate = SwapFloat64BtoN(((port_CAFAudioDescription *)(theReadBuffer))->mSampleRate);
                theInputFormat->mBitsPerChannel = Swap32BtoN(((port_CAFAudioDescription *)(theReadBuffer))->mBitsPerChannel);
                theInputFormat->mFormatFlags = Swap32BtoN(((port_CAFAudioDescription *)(theReadBuffer))->mFormatFlags);
                theInputFormat->mBytesPerPacket = Swap32BtoN(((port_CAFAudioDescription *)(theReadBuffer))->mBytesPerPacket);
                if (theInputFormat->mFormatID == kALACFormatAppleLossless)
                {
                    theInputFormat->mBytesPerFrame = 0;
                }
                else
                {
					theInputFormat->mBytesPerFrame = theInputFormat->mBytesPerPacket;
					if ((theInputFormat->mFormatFlags & 0x02) == 0x02)
					{
						theInputFormat->mFormatFlags &= 0xfffffffc;
					}
					else
					{
						theInputFormat->mFormatFlags |= 0x02;
					}

                }
                theInputFormat->mFramesPerPacket = Swap32BtoN(((port_CAFAudioDescription *)(theReadBuffer))->mFramesPerPacket);
                theInputFormat->mReserved = 0;
                done = true;
                break;
            default:
                // read the size and skip
//                fread(theReadBuffer, 1, 8, inputFile);
                mDataSource->readAt(mOffset, theReadBuffer, 8);
                mOffset += 8;
                theChunkSize = ((int32_t)(theReadBuffer[4]) << 24) + ((int32_t)(theReadBuffer[5]) << 16) + ((int32_t)(theReadBuffer[6]) << 8) + theReadBuffer[7];
//                fseek(inputFile, theChunkSize, SEEK_CUR);
                mOffset += theChunkSize;
                break;
        }
    }
    return done;
}

bool RTK_ALAC::FindCAFFDataStart(/*FILE * inputFile, */int32_t * dataPos, int32_t * dataSize)
{
    bool done = false;
    int32_t bytesRead = 8;
    uint32_t chunkType = 0, chunkSize = 0;
    uint8_t theBuffer[12];
    off64_t Offset;
    
//    fseek(inputFile, bytesRead, SEEK_SET); // start at 8!
    Offset = bytesRead;
    while (!done && bytesRead > 0) // no file size here
    {
//        bytesRead = fread(theBuffer, 1, 12, inputFile);
        bytesRead = mDataSource->readAt(Offset, theBuffer, 12);
        Offset += 12;
        chunkType = ((int32_t)(theBuffer[0]) << 24) + ((int32_t)(theBuffer[1]) << 16) + ((int32_t)(theBuffer[2]) << 8) + theBuffer[3];
        switch(chunkType)
        {
            case 'data':
//                *dataPos = ftell(inputFile) + sizeof(uint32_t); // skip the edits
                *dataPos = Offset + sizeof(uint32_t);
                // big endian size
                *dataSize = ((int32_t)(theBuffer[8]) << 24) + ((int32_t)(theBuffer[9]) << 16) + ((int32_t)(theBuffer[10]) << 8) + theBuffer[11];
                *dataSize -= 4; // the edits are included in the size
                done = true;
                break;
            default:
                chunkSize = ((int32_t)(theBuffer[8]) << 24) + ((int32_t)(theBuffer[9]) << 16) + ((int32_t)(theBuffer[10]) << 8) + theBuffer[11];
//                fseek(inputFile, chunkSize, SEEK_CUR);
                Offset += chunkSize;
                break;
        }
    }
    return done;
}

uint32_t RTK_ALAC::GetMagicCookieSizeFromCAFFkuki(/*FILE * inputFile*/)
{
    // returns to the current absolute position within the file
//    int32_t currentPosition = ftell(inputFile); // record the current position
    off64_t currentPosition = mOffset; // record the current position
    uint8_t theReadBuffer[sizeof(ALACSpecificConfig)];
    uint32_t chunkType = 0, chunkSize = 0;
    bool done = false;
    int32_t bytesRead = sizeof(port_CAFFileHeader);
    uint32_t theCookieSize = 0;
    
//    fseek(inputFile, bytesRead, SEEK_SET); // start at 8!
    mOffset = bytesRead;
    while (!done && bytesRead > 0) // no file size here
    {
//        bytesRead = fread(theReadBuffer, 1, 12, inputFile);
        bytesRead = mDataSource->readAt(mOffset, theReadBuffer, 12);
        mOffset += 12;

        chunkType = ((int32_t)(theReadBuffer[0]) << 24) + ((int32_t)(theReadBuffer[1]) << 16) + ((int32_t)(theReadBuffer[2]) << 8) + theReadBuffer[3];
        switch(chunkType)
        {                    
            case 'kuki':
            {
                theCookieSize = theReadBuffer[11];
                done = true;
                break;
            }
            default:
                chunkSize = ((int32_t)(theReadBuffer[8]) << 24) + ((int32_t)(theReadBuffer[9]) << 16) + ((int32_t)(theReadBuffer[10]) << 8) + theReadBuffer[11];
//                fseek(inputFile, chunkSize, SEEK_CUR);
                mOffset += chunkSize;
                break;
        }
    }
    
//    fseek(inputFile, currentPosition, SEEK_SET); // start at 0
    mOffset = currentPosition;
    
    if (!done) return -1;
    
    return theCookieSize;
    
}

// gets the kuki chunk from a caff file
int32_t RTK_ALAC::GetMagicCookieFromCAFFkuki(/*FILE * inputFile, */uint8_t * outMagicCookie, uint32_t * ioMagicCookieSize)
{
    // returns to the current absolute position within the file
//    int32_t currentPosition = ftell(inputFile); // record the current position
    off64_t currentPosition = mOffset; // record the current position
    uint8_t theReadBuffer[12];
    uint32_t chunkType = 0, chunkSize = 0;
    bool done = false, cookieFound = false;
    int32_t bytesRead = sizeof(port_CAFFileHeader);
    uint32_t theStoredCookieSize = 0;
    
//    fseek(inputFile, bytesRead, SEEK_SET); // start at 8!
    mOffset = bytesRead;
    while (!done && bytesRead > 0) // no file size here
    {
//        bytesRead = fread(theReadBuffer, 1, 12, inputFile);
        bytesRead = mDataSource->readAt(mOffset, theReadBuffer, 12);
        mOffset += 12;
        chunkType = ((int32_t)(theReadBuffer[0]) << 24) + ((int32_t)(theReadBuffer[1]) << 16) + ((int32_t)(theReadBuffer[2]) << 8) + theReadBuffer[3];
        switch(chunkType)
        {                    
            case 'kuki':
            {
                theStoredCookieSize = theReadBuffer[11];
                if (*ioMagicCookieSize >= theStoredCookieSize)
                {
//                    fread(outMagicCookie, 1, theStoredCookieSize, inputFile);
                    mDataSource->readAt(mOffset, outMagicCookie, theStoredCookieSize);
                    mOffset += theStoredCookieSize;
                    *ioMagicCookieSize = theStoredCookieSize;
                    cookieFound = true;
                }
                else
                {
                    *ioMagicCookieSize = 0;
                }
                done = true;
                break;
            }
            default:
                chunkSize = ((int32_t)(theReadBuffer[8]) << 24) + ((int32_t)(theReadBuffer[9]) << 16) + ((int32_t)(theReadBuffer[10]) << 8) + theReadBuffer[11];
//                fseek(inputFile, chunkSize, SEEK_CUR);
                mOffset += chunkSize;
                break;
        }
    }
    
//    fseek(inputFile, currentPosition, SEEK_SET); // start at 0
    mOffset = currentPosition;    

    if (!done || !cookieFound) return -1;
    
    return 0;
}

void RTK_ALAC::PrintCookie(ALACSpecificConfig *pCookie)
{
    printf("\n\n\n ====== ALAC cookie start ========\n");
    printf("[framelength %d]\n", pCookie->frameLength);
    printf("[compatibleVersion %d]\n", pCookie->compatibleVersion);
    printf("[bitDepth %d]\n", pCookie->bitDepth);
    printf("[pb %d]\n", pCookie->pb);
    printf("[mb %d]\n", pCookie->mb);
    printf("[kb %d]\n", pCookie->kb);
    printf("[numChannels %d]\n", pCookie->numChannels);
    printf("[maxRun %d]\n", pCookie->maxRun);
    printf("[maxFrameBytes %d]\n", pCookie->maxFrameBytes);
    printf("[avgBitRate %d]\n", pCookie->avgBitRate);
    printf("[sampleRate %d]\n", pCookie->sampleRate);
    printf("====== ALAC cookie end ========\n\n\n\n");
}

/*
	Init()
	- initialize the decoder with the given configuration
*/
int32_t RTK_ALAC::Internal_Init( void * inMagicCookie, uint32_t inMagicCookieSize )
{
	int32_t		status = ALAC_noErr;
    ALACSpecificConfig theConfig;
    uint8_t * theActualCookie = (uint8_t *)inMagicCookie;
    uint32_t theCookieBytesRemaining = inMagicCookieSize;

    // For historical reasons the decoder needs to be resilient to magic cookies vended by older encoders.
    // As specified in the ALACMagicCookieDescription.txt document, there may be additional data encapsulating 
    // the ALACSpecificConfig. This would consist of format ('frma') and 'alac' atoms which preceed the
    // ALACSpecificConfig. 
    // See ALACMagicCookieDescription.txt for additional documentation concerning the 'magic cookie'
    
    // skip format ('frma') atom if present
    if (theActualCookie[4] == 'f' && theActualCookie[5] == 'r' && theActualCookie[6] == 'm' && theActualCookie[7] == 'a')
    {
        theActualCookie += 12;
        theCookieBytesRemaining -= 12;
    }
    
    // skip 'alac' atom header if present
    if (theActualCookie[4] == 'a' && theActualCookie[5] == 'l' && theActualCookie[6] == 'a' && theActualCookie[7] == 'c')
    {
        theActualCookie += 12;
        theCookieBytesRemaining -= 12;
    }

    // read the ALACSpecificConfig
    if (theCookieBytesRemaining >= sizeof(ALACSpecificConfig))
    {
        theConfig.frameLength = Swap32BtoN(((ALACSpecificConfig *)theActualCookie)->frameLength);
        theConfig.compatibleVersion = ((ALACSpecificConfig *)theActualCookie)->compatibleVersion;
        theConfig.bitDepth = ((ALACSpecificConfig *)theActualCookie)->bitDepth;
        theConfig.pb = ((ALACSpecificConfig *)theActualCookie)->pb;
        theConfig.mb = ((ALACSpecificConfig *)theActualCookie)->mb;
        theConfig.kb = ((ALACSpecificConfig *)theActualCookie)->kb;
        theConfig.numChannels = ((ALACSpecificConfig *)theActualCookie)->numChannels;
        theConfig.maxRun = Swap16BtoN(((ALACSpecificConfig *)theActualCookie)->maxRun);
        theConfig.maxFrameBytes = Swap32BtoN(((ALACSpecificConfig *)theActualCookie)->maxFrameBytes);
        theConfig.avgBitRate = Swap32BtoN(((ALACSpecificConfig *)theActualCookie)->avgBitRate);
        theConfig.sampleRate = Swap32BtoN(((ALACSpecificConfig *)theActualCookie)->sampleRate);

        mConfig = theConfig;

        PrintCookie(&mConfig);

        RequireAction( mConfig.compatibleVersion <= kALACVersion, return kALAC_ParamError; );
     }
    else
    {
        status = kALAC_ParamError;
    }

    // skip to Channel Layout Info
    // theActualCookie += sizeof(ALACSpecificConfig);
    
    // Currently, the Channel Layout Info portion of the magic cookie (as defined in the 
    // ALACMagicCookieDescription.txt document) is unused by the decoder. 
    
//Exit:
	return status;
}

int32_t RTK_ALAC::FindCAFFPacketTableStart(/*FILE * inputFile, */int32_t * paktPos, int32_t * paktSize)
{
    // returns the absolute position within the file
//    int32_t currentPosition = ftell(inputFile); // record the current position
    off64_t currentPosition = mOffset; // record the current position
    uint8_t theReadBuffer[12];
    uint32_t chunkType = 0, chunkSize = 0;
    bool done = false;
    int32_t bytesRead = 8;
    
//    fseek(inputFile, bytesRead, SEEK_SET); // start at 8!
    mOffset = bytesRead;
    while (!done && bytesRead > 0) // no file size here
    {
//        bytesRead = fread(theReadBuffer, 1, 12, inputFile);
        bytesRead = mDataSource->readAt(mOffset, theReadBuffer, 12);
        mOffset += 12;
        chunkType = ((int32_t)(theReadBuffer[0]) << 24) + ((int32_t)(theReadBuffer[1]) << 16) + ((int32_t)(theReadBuffer[2]) << 8) + theReadBuffer[3];
        switch(chunkType)
        {
            case 'pakt':
//                *paktPos = ftell(inputFile) + kMinCAFFPacketTableHeaderSize;
                *paktPos = mOffset + kMinCAFFPacketTableHeaderSize;
                // big endian size
                *paktSize = ((int32_t)(theReadBuffer[8]) << 24) + ((int32_t)(theReadBuffer[9]) << 16) + ((int32_t)(theReadBuffer[10]) << 8) + theReadBuffer[11];
                done = true;
                break;
            default:
                chunkSize = ((int32_t)(theReadBuffer[8]) << 24) + ((int32_t)(theReadBuffer[9]) << 16) + ((int32_t)(theReadBuffer[10]) << 8) + theReadBuffer[11];
//                fseek(inputFile, chunkSize, SEEK_CUR);
                mOffset += chunkSize;
                break;
        }
    }
    
//    fseek(inputFile, currentPosition, SEEK_SET); // start at 0
    mOffset = currentPosition;
    
    return 0;
    
}

uint32_t ReadBERInteger(uint8_t * theInputBuffer, int32_t * ioNumBytes)
{
	uint32_t theAnswer = 0;
	uint8_t theData;
	int32_t size = 0;
    do
	{
		theData = theInputBuffer[size];
		theAnswer = (theAnswer << 7) | (theData & 0x7F);
		if (++size > 5)
		{
			size = 0xFFFFFFFF;
			return 0;
		}
	}
	while(((theData & 0x80) != 0) && (size <= *ioNumBytes));
    
    *ioNumBytes = size;
	
	return theAnswer;
}

int64_t RTK_ALAC::EstimateDuration()
{
#define MAX_FRAME_BYTES 5120
    int32_t numBytes = 0, frmBytes = 0;
    off64_t currentPosition = mOffset; // record the current position
    unsigned char theReadBuffer[MAX_FRAME_BYTES];
    int32_t theInputPacketBytes;
    int32_t back_up = mPacketTablePos;
    int64_t ret = 0;
    unsigned int frames_per_second = 1000000 / us_per_frame;

    mTotalFrames = 0;
    mOffsetTableLength = 0;
    while(1)
    {
        numBytes = mDataSource->readAt(mPacketTablePos, theReadBuffer, kMaxBERSize);
        theInputPacketBytes = ReadBERInteger(theReadBuffer, &numBytes);
        if(theInputPacketBytes > MAX_FRAME_BYTES)
            ALOGD("[WARN frm too large %x @ %s %d]\n", theInputPacketBytes, __FUNCTION__, __LINE__);

        frmBytes = mDataSource->readAt(mOffset, theReadBuffer, theInputPacketBytes);

        if(theInputPacketBytes <= 0 || theInputPacketBytes != frmBytes)
            break;

        // for seek
        if ((mTotalFrames % frames_per_second == 0) && (mTotalFrames / frames_per_second < OFFSET_TABLE_LEN)) {
            CHECK_EQ(mOffsetTableLength, mTotalFrames / frames_per_second);
            mPktSizeOffsetTable[mOffsetTableLength] = mPacketTablePos;
            mOffsetTable[mOffsetTableLength] = mOffset;
            mOffsetTableLength ++;
        }

        mPacketTablePos += numBytes;
        mOffset += theInputPacketBytes;
        ret += us_per_frame;
        mTotalFrames++;
    }
    
    mPacketTablePos = back_up;
    mOffset = currentPosition;

    return ret;
}

void RTK_ALAC::ALAC_init()
{
    uint32_t theMagicCookieSize = 0;
    uint8_t* theMagicCookie = NULL;

    TRACE_PRINT("[+]\n");
    GetCAFFdescFormat(&inputFormat);

    FindCAFFDataStart(&mInputDataPos, &mInputDataSize);
    TRACE_PRINT("[ALAC data pos %x size %x]\n", mInputDataPos, mInputDataSize);
    mOffset = mInputDataPos;

    // We need to get the cookie from the file
    theMagicCookieSize = GetMagicCookieSizeFromCAFFkuki();
    theMagicCookie = (uint8_t *)calloc(theMagicCookieSize, 1);
    GetMagicCookieFromCAFFkuki(theMagicCookie, &theMagicCookieSize);

    // While we don't  have a use for this here, if you were using arbitrary channel layouts, you'd need to run the following check:
    Internal_Init(theMagicCookie, theMagicCookieSize);

    free(theMagicCookie);

    // We do have to get the packet size from the packet table
    FindCAFFPacketTableStart(&mPacketTablePos, &mThePacketTableSize);
    TRACE_PRINT("[ALAC PktTbl pos %x size %x]\n", mPacketTablePos, mThePacketTableSize);

    us_per_frame = (1000000 * mConfig.frameLength) / mConfig.sampleRate;

    TRACE_PRINT("[-]\n");
}

RTK_ALAC::RTK_ALAC(sp<DataSource> source)
:mDataSource(source)
{
    TRACE_PRINT("\n");
}

RTK_ALAC::~RTK_ALAC()
{
    TRACE_PRINT("\n");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ALACSource::ALACSource(
        const sp<DataSource> &source, const sp<MetaData> &meta,
        const off64_t *offset_table, const off64_t *pkt_size_offset_table, size_t offset_table_length, off64_t InputDataPos,
        off64_t PacketTablePos, unsigned int us)
    : mDataSource(source),
      mMeta(meta),
      mInputDataPos(InputDataPos),
      mPacketTablePos(PacketTablePos),
      mCurrentTimeUs(0),
      mStarted(false),
      mGroup(NULL),
      mOffsetTableLength(offset_table_length),
      us_per_frame(us)
{
    TRACE_PRINT("[+]\n");
    TRACE_PRINT("[PacketTablePos %x InputDataPos %x mInputDataPos %x]\n", PacketTablePos, InputDataPos, mInputDataPos);
    if (mOffsetTableLength > 0 && mOffsetTableLength <= OFFSET_TABLE_LEN) 
    {
        memcpy ((char*)mOffsetTable, (char*)offset_table, sizeof(off64_t) * mOffsetTableLength);
        memcpy((char *)mPktSizeOffsetTable, (char *)pkt_size_offset_table, sizeof(off64_t) * mOffsetTableLength);
    }
    mFrameCnt = 0;
    TRACE_PRINT("[-]\n");
}

ALACSource::~ALACSource() {
    if (mStarted) {
        stop();
    }
}

status_t ALACSource::start(MetaData * /*params*/) {
    TRACE_PRINT("[+]\n");
    CHECK(!mStarted);

    mOffset = mInputDataPos; 
    mTblOffset = mPacketTablePos;
    mCurrentTimeUs = 0;
    mGroup = new MediaBufferGroup;
    mGroup->add_buffer(new MediaBuffer(RTK_ALAC_MAX_BUF));
    mStarted = true;

    TRACE_PRINT("[mOffset %x mTblOffset %x]\n", mOffset, mTblOffset);
    TRACE_PRINT("[-]\n");
    return OK;
}

status_t ALACSource::stop() {
    TRACE_PRINT("[+]\n");
    CHECK(mStarted);

    delete mGroup;
    mGroup = NULL;

    mStarted = false;
    TRACE_PRINT("[-]\n");
    return OK;
}

sp<MetaData> ALACSource::getFormat() {
    return mMeta;
}

status_t ALACSource::read(
        MediaBuffer **out, const ReadOptions *options) 
{
    int64_t seekTimeUs;
    ReadOptions::SeekMode mode;
    int32_t numBytes = 0;
    unsigned char theReadBuffer [kMaxBERSize];
    int32_t theInputPacketBytes;
    int32_t frmBytes = 0;
    unsigned char *pBuf, *pTemp;
    
    TRACE_PRINT("read\n");

    *out = NULL;
    if (options && options->getSeekTo(&seekTimeUs, &mode)) {
        TRACE_PRINT("read seek\n");
        int64_t seekFrame = seekTimeUs / us_per_frame;
        unsigned int frames_per_second = 1000000 / us_per_frame;
        mCurrentTimeUs = seekFrame * us_per_frame;

        int index = seekFrame / frames_per_second;
        if (index >= (ssize_t)mOffsetTableLength) {
            index = mOffsetTableLength - 1;
        }

        mTblOffset = mPktSizeOffsetTable[index];
        mOffset = mOffsetTable[index];

        for (int i = 0; i< seekFrame - index * frames_per_second; i++) {
            numBytes = mDataSource->readAt(mTblOffset, theReadBuffer, kMaxBERSize);
            theInputPacketBytes = ReadBERInteger(theReadBuffer, &numBytes);
            mTblOffset += numBytes;
            mOffset += theInputPacketBytes;
        }
    }

    MediaBuffer *buffer;
    status_t err = mGroup->acquire_buffer(&buffer);
    if (err != OK) {
        return err;
    }
    
    numBytes = mDataSource->readAt(mTblOffset, theReadBuffer, kMaxBERSize);
    theInputPacketBytes = ReadBERInteger(theReadBuffer, &numBytes);

    // get buffer
    pBuf = (unsigned char *)buffer->data();

    // copy ALAC sync 
    memcpy(pBuf, ALAC_SYNCH_ASCII, 4);
    pTemp = (unsigned char *)&theInputPacketBytes;
    
    // copy packet length
    memcpy(pBuf + 4, pTemp, 4);
    frmBytes = mDataSource->readAt(mOffset, (void *)(pBuf + ALAC_HEADER_SIZE), theInputPacketBytes);

    if(theInputPacketBytes <= 0 || theInputPacketBytes != frmBytes)
    {
        ALOGD("WARN EOS %x %x @ %s\n", theInputPacketBytes, frmBytes, __FUNCTION__);
        buffer->release();
        buffer = NULL;

        return ERROR_IO;
    }
        
    buffer->set_range(0, ALAC_HEADER_SIZE + theInputPacketBytes);
    buffer->meta_data()->setInt64(kKeyTime, mCurrentTimeUs);
    buffer->meta_data()->setInt32(kKeyIsSyncFrame, 1);

    mTblOffset += numBytes;
    mOffset += theInputPacketBytes;
    mCurrentTimeUs += us_per_frame;
    mFrameCnt++;

    *out = buffer;
    return OK;
}

}   // namespace android {

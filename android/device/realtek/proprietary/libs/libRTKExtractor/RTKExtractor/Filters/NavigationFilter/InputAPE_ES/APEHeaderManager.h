/*
 * APEHeaderManager.h
 *
 *  Created on: 2012/5/16
 *      Author: bruce_huang
 */

#ifndef APEHEADERMANAGER_H_
#define APEHEADERMANAGER_H_

typedef struct _APE_DESCRIPTOR {
	char   cID[4];                             // should equal 'MAC '
	unsigned short nVersion;                           // version number * 1000 (3.81 = 3810) (remember that 4-byte alignment causes this to take 4-bytes)
	unsigned short padding;
	unsigned int nDescriptorBytes;                   // the number of descriptor bytes (allows later expansion of this header)
	unsigned int nHeaderBytes;                       // the number of header APE_HEADER bytes
	unsigned int nSeekTableBytes;                    // the number of bytes of the seek table
	unsigned int nHeaderDataBytes;                   // the number of header data bytes (from original file)
	unsigned int nAPEFrameDataBytes;                 // the number of bytes of APE frame data
	unsigned int nAPEFrameDataBytesHigh;             // the high order number of APE frame data bytes
	unsigned int nTerminatingDataBytes;              // the terminating data of the file (not including tag data)
	char  cFileMD5[16];                       // the MD5 hash of the file (see notes for usage... it's a littly tricky)
}APEDescriptor;

typedef struct _APE_HEADER{
	unsigned short nCompressionLevel;
	unsigned short nFormatFlags;
	unsigned int nBlocksPerFrame;
	unsigned int nFinalFrameBlocks;
	unsigned int nTotalFrames;
	unsigned short bitsPerSample;
	unsigned short channels;
	unsigned int sampleRate;
}APEHeader;

/*****************************************************************************************
APE header structure for old APE files (3.97 and earlier)
*****************************************************************************************/
typedef struct _APE_HEADER_OLD{
	char cID[4];                            // should equal 'MAC '
	unsigned short nVersion;                        // version number * 1000 (3.81 = 3810)
	unsigned short nCompressionLevel;               // the compression level
	unsigned short nFormatFlags;                    // any format flags (for future use)
	unsigned short nChannels;                       // the number of channels (1 or 2)
	unsigned int nSampleRate;                     // the sample rate (typically 44100)
	unsigned int nHeaderBytes;                    // the bytes after the MAC header that compose the WAV header
	unsigned int nTerminatingBytes;               // the bytes after that raw data (for extended info)
	unsigned int nTotalFrames;                    // the number of frames in the file
	unsigned int nFinalFrameBlocks;               // the number of samples in the final frame
}APEHeaderOld;

// collect information of APE file, interface between inside and outside.
typedef struct _APE_FILE_INFO {
    int nVersion;                                   // file version number * 1000 (3.93 = 3930)
    int nCompressionLevel;                          // the compression level
    int nFormatFlags;                               // format flags
    int nTotalFrames;                               // the total number frames (frames are used internally)
    int nBlocksPerFrame;                            // the samples in a frame (frames are used internally)
    int nFinalFrameBlocks;                          // the number of samples in the final frame
    int nChannels;                                  // audio channels
    int nSampleRate;                                // audio samples per second
    int nBitsPerSample;                             // audio bits per sample
    int nBytesPerSample;                            // audio bytes per sample
    int nBlockAlign;                                // audio block align (channels * bytes per sample)
    int nWAVHeaderBytes;                            // header bytes of the original WAV
    int nWAVDataBytes;                              // data bytes of the original WAV
    int nWAVTerminatingBytes;                       // terminating bytes of the original WAV
    int nWAVTotalBytes;                             // total bytes of the original WAV
    //int nAPETotalBytes;                             // total bytes of the APE file
    int nTotalBlocks;                               // the total number audio blocks
    int nLengthMS;                                  // the length in milliseconds
    //int nAverageBitrate;                            // the kbps (i.e. 637 kpbs)
    int nDecompressedBitrate;                       // the kbps of the decompressed audio (i.e. 1440 kpbs for CD audio)
    int nJunkHeaderBytes;                           // used for ID3v2, etc.
    int nSeekTableElements;                         // the number of elements in the seek table(s)
	int nMD5Invalid;                                // whether the MD5 is valid

	unsigned int *spSeekByteTable;              	// the seek table (byte)
	unsigned char *spSeekBitTable;        			// the seek table (bits -- legacy)
	unsigned char *spWaveHeaderData;      			// the pre-audio header data
}APEInfo;


typedef struct _WAVE_HEADER
{
    // RIFF header
    char cRIFFHeader[4];
    unsigned int nRIFFBytes;

    // data type
    char cDataTypeID[4];

    // wave format
    char cFormatHeader[4];
    unsigned int nFormatBytes;

    unsigned short nFormatTag;
    unsigned short nChannels;
    unsigned int nSamplesPerSec;
    unsigned int nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short nBitsPerSample;

    // data chunk header
    char cDataHeader[4];
    unsigned int nDataBytes;
}WaveHeader;

class APEHeaderManager {

public:
	APEHeaderManager();
	~APEHeaderManager();
public:
	APEInfo m_info;
	bool analysis(char *buf,int bufSize);
	int getSeekBytes(int nBaseFrame);
	int getSeekBits(int nBaseFrame);
	int calculateRequestBufSize(char *buf,int bufSize);
	//int getTotalFrames(){ return m_info.nSeekTableElements; }
private:
	char *pData;
	int dataSize;
	int junkHeaderBytes;
	APEDescriptor m_des;
	APEHeader m_header;
	APEHeaderOld m_header_old;
	bool parseDescriptor();
	bool parseHeader();
	bool parseHeaderSimple();
	char *calculateJunkBytes(char *buf, int bufSize);
	// maybe more...
};

#endif /* APEHEADERMANAGER_H_ */

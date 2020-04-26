#ifndef __RM_PARSER_H__
#define __RM_PARSER_H__

#include <hresult.h>
#include <StreamClass/EType.h>
#include <Filters/PrivateInfo.h>
#include <FlowEvent.h>
#include <InbandAPI.h>
#include <Util.h>
#include "../../../../../common/IPC/include/rpc_common.h"
#include "InputRM.h"
#include "rm_util.h"
#include "rm_reader.h"

#define RM_BUILD_INDEX	


#define RM_FILE_HEADER_OBJECT      0x2E524D46   /* '.RMF' */
#define RM_PROPERTIES_OBJECT       0x50524F50   /* 'PROP' */
#define RM_MEDIA_PROPERTIES_OBJECT 0x4D445052   /* 'MDPR' */
#define RM_CONTENT_OBJECT          0x434F4E54   /* 'CONT' */
#define RM_DATA_OBJECT             0x44415441   /* 'DATA' */
#define RM_INDEX_OBJECT            0x494E4458   /* 'INDX' */
#define RM_MULTIHEADER_OBJECT      0x4D4C5449   /* 'MLTI' */

#define RM_AUDIO_CODEC_COOK			0x636F6F6B
#define RM_AUDIO_CODEC_RAAC			0x72616163
#define RM_AUDIO_CODEC_RACP			0x72616370
#define RM_AUDIO_CODEC_LSD			0x4C53443A
#define RM_AUDIO_CODEC_ATRC			0x61747263
#define RM_AUDIO_CODEC_SIPR			0x73697072
#define RM_AUDIO_CODEC_DNET			0x646E6574	//ac3


#define NUM_OF_AUDIO_ARRIVAL_FRAME_FLAG	1800	//bug 23740. There are 13980 frames per super block


#define RM_DEINTERLEAVE_BUFFER_SIZE		(16*RM_BYTE_ALIGN_FOR_ODIRECT_OPEN)

//#define RM_PARSER_GENERIC_HDR_SIZE       10
#define RM_PARSER_DATA_CHUNK_HEADER_SIZE   18	//sizeof(rm_generic_hdr_t) + numPacket(4) + nextDataHeader
//#define RM_PARSER_INDEX_HEADER_SIZE      20

#define RM_HAS_MULTI_DATA_CHUNK(_pRM) (_pRM->dataChunk.totalNum > 0)

#ifdef RDDEBUG
	#define RM_DEBUG_INFO(x)    { printf x; }
	#define RM_WARNNING(x)    	{ printf x; }
	#define RM_PTS_INFO(x)    	{ printf x; }
	#define RM_HEADER_INFO(x)   { printf x; }
	#define RM_COMMAND(x)	    { printf x; }
	#define RM_DEBUG_INDEX(x)   //{ printf x; }
#else
	#define RM_DEBUG_INFO(x)    
	#define RM_WARNNING(x)    	
	#define RM_PTS_INFO(x)   
	#define RM_HEADER_INFO(x) 	
	#define RM_COMMAND(x)
	#define RM_DEBUG_INDEX(x)
#endif

//====================================================
#define RM_FREE_AND_NULL_OUT_PTR(ptr)	\
	if(ptr != NULL) {					\
		free(ptr);						\
		ptr = 0;						\
	}									\
//====================================================


#define IS_AT_NORMAL_SPEED(_speed)			(_speed == 256)

#ifndef DONTFIXBUG16110
	#define IS_RM_SMOOTH_TRICK_PLAY_ENABLE(_speed) (APCFG_SMOOTH_TRICK_PLAY && (IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)) && _speed > 0)
#else
	#define IS_RM_SMOOTH_TRICK_PLAY_ENABLE(_speed) (APCFG_SMOOTH_TRICK_PLAY && (IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)))
#endif

#define	STATUS_RM_AT_PAUSE_STATE				(1L << 0)
#define	STATUS_RM_AT_RUN_STATE					(1L << 1)
#define	STATUS_RM_AT_STOP_STATE					(1L << 2)
#define	STATUS_RM_SHOULD_SEND_SEG_END			(1L << 3)    
#define STATUS_RM_CREATE_VIDEO_FRAME			(1L << 4)
#define	STATUS_RM_SHOULD_SEND_STOP				(1L << 5)    
#define	STATUS_RM_WAIT_PRESENTATION_FINISH		(1L << 6)    
#define	STATUS_RM_PRESENTATION_FINISHED			(1L << 7)    
#define	STATUS_RM_USE_ODIRECT_OPEN				(1L << 8)
#define STATUS_RM_SEND_PTS						(1L << 9)
#define STATUS_RM_SEND_AUDIO_OPAQUE_DATA		(1L << 10)
#define STATUS_RM_AUDIO_KEYFRAME_FOUND			(1L << 11)
#define STATUS_RM_VIDEO_KEYFRAME_FOUND			(1L << 12)
#define STATUS_RM_READ_ERROR_JUMP				(1L << 13)
#define STATUS_RM_BUILD_VIDEO_INDEX_COMPLETE	(1L << 14)
#define STATUS_RM_BUILD_AUDIO_INDEX_COMPLETE	(1L << 15)
#define STATUS_RM_DROP_BITSTREAM				(1L << 16)
#define STATUS_RM_DONT_RESET_DROP_BITSTREAM		(1L << 17)
#define STATUS_RM_REWIND_IS_PENDING				(1L << 18)
#define STATUS_RM_CONTINUOUS_MEM_FOR_DEINTERLEAVE	(1L << 19)

#define	CMD_RM_FLUSH				(1L << 1)    
#define	CMD_RM_RESET_SPEED			(1L << 2)
#define CMD_RM_RESET_REPEAT_MODE    (1L << 3)
#define	CMD_RM_AUDIO_FORMAT			(1L << 4)    
#define	CMD_RM_NEW_SEGMENT			(1L << 5)
#define	CMD_RM_DECODE_INIT			(1L << 6)
#define	CMD_RM_BLEND				(1L << 7)
#define	CMD_RM_DECODE				(1L << 8)
#define CMD_RM_VIDEO_FORMAT			(1L << 9)
#define CMD_RM_VIDEO_FRAME_INFO		(1L << 10)
#define CMD_RM_VOBU_INFO			(1L << 11)
#define CMD_RM_BITSTREAM_ERROR		(1L << 12)
#define CMD_RM_VIDEO_FRAME_SEGMENT	(1L << 13)// segment info if present should be sent before frame info
#define	CMD_RM_END_OF_SEGMENT		(1L << 14)
#define	CMD_RM_STOP					(1L << 15)
#define CMD_RM_AUDIO_DECODE			(1L << 16)

/* the result of each reading */
typedef enum {
	/* error case */
	RM_READ_ERROR 		= IOPLUGIN_ERROR_GENERIC, 	//-1
	RM_READ_END 		= IOPLUGIN_ERROR_EOF,		//-2
	RM_READ_ERROR_BUSY 	= IOPLUGIN_ERROR_BUSY, 		//-3
	RM_READ_NO_DEVICE 	= IOPLUGIN_ERROR_NO_DEVICE,	//-4
	RM_READ_EIO			= IOPLUGIN_ERROR_EIO,		//-8
  
  	/* normal case */
	RM_READ_DATA 		= (RM_BUFSIZE+1),
	RM_READ_COMMAND 	= (RM_BUFSIZE+2),		
	RM_READ_AGAIN 		= (RM_BUFSIZE+3),
	RM_READ_NONE 		= (RM_BUFSIZE+4),
	RM_READ_ERROR_JUMP	= (RM_BUFSIZE+5),

} rm_read_status_t;

/* the result of play at time */
typedef enum {
	
	RM_TIME_SEEK_SEARCH_BACKWARD 	= -2,
	RM_TIME_SEEK_FAIL 				= 0,
	RM_TIME_SEEK_OK 				= 1,
	RM_TIME_SEEK_SEARCH_FORWARD 	= 2,
	
} rm_time_seek_status_t;

/* all rm headers start with this header */
typedef struct rm_generic_hdr
{
    unsigned long id;          	/* unique identifier for this header */
    unsigned long size;        	/* size of header in file */
    unsigned short version;     /* struct version */
} __attribute__((packed)) rm_generic_hdr_t;

/* rm file header -- all real media files start with this header */
typedef struct rm_file_hdr
{
    
    unsigned long fileVersion; /* revision number for realmedia file */
    unsigned long numHeaders;  /* num headers in file (including this one) */
    
} __attribute__((packed)) rm_file_hdr_t;


/* rm properties header -- summarizes data for entire clip */
typedef struct rm_properties_hdr
{
    
    //unsigned long maxBitRate; 	/* maximum bit rate of clip */
    //unsigned long avgBitRate; 	/* average bit rate of clip */
    //unsigned long maxPacketSize; 	/* max length of packet in clip (bytes) */
    //unsigned long avgPacketSize; 	/* averge length of packet in clip (bytes) */
    //unsigned long numPackets;     /* total packets in clip (all streams) */
    unsigned long duration;     	/* duration of clip (milliseconds) */
    //unsigned long preroll;      	/* preroll time (milliseconds) */
    unsigned long indexOffset; 		/* offset to beginning of index data. zero means no index chunks in this file */
    unsigned long dataOffset;  		/* offset to beginning of packet data */
    unsigned short numStreams;  	/* total number of streams in clip */
    //unsigned short flags;        	/* various and sundry */
    
}__attribute__((packed)) rm_properties_hdr_t;

/***********************************************************************/
/* rm data chunk header -- mark the start of the data chunk */
typedef struct rm_data_chunk_hdr
{
	unsigned long size;				/* the size of the data chunk in bytes */
	unsigned long nextDataHeader;		/* offset from start of file to the next data chunk. */
	
}rm_data_chunk_hdr_t;

typedef struct rm_data_chunk
{
	int size;
	
	int inUsedNum;	//number of field of offset in used.
	int totalNum;  //total number of field of offset. 0 meas only one data chunk in this file and offset is pRM->propertyHeader.dataOffset
	long* offset;		//offset of each data chunk starting from beginning of file
	
} rm_data_chunk_t;

/***********************************************************************/

/* rm audio codec-specific information -- one for each stream */
typedef struct rm_audio_opaque
{
	unsigned short flavorIdx;			/* codec flavor index */
	unsigned short sampleSize;			/* number of bits per audio sample*/
	unsigned short interleaveFactor;	/* the number of packet(blocks) over which interleaving is performed(a superblock) */
	unsigned short interleaveBlockSize; /* the size of each block (packet) */
	unsigned long  bitRate;				/* bit rate per second of this substream */		
	unsigned long  sampleRate;
	unsigned short frameSize;			/* codec frame size : size, in bytes, of each encoded frame of audio for this substream. */
	unsigned short numChannels;			/* number of channels in this audio substream */
	unsigned long  interleaverID;		/* a 4CC that spcifies the interleaver for this substream */
	unsigned long  codecID;				/* a 4CC that identifies the codec for this substream */
	unsigned long  codecExtraSize;		/* the size of codec opaque data */
	unsigned char* pCodecExtraData;		/* codec-specific opaque data of length codecExtraSize */
	unsigned char  streamType;			/* it is significant only when codec type is RAAC */
	unsigned char  isInterleaved;		/* specify whether or not interleaving is used on this substream */
	unsigned char  hasInterleavePattern;/* non-zero means a user defined interleave pattern is used if this substream is interleaved */
	unsigned short* pInterleavePattern; /* a user defined interleave pattern */
	unsigned short* pDeinterleavePattern;/* the de-interleave pattern is transformed from interleave pattern which is used to organize audio packet */
	
	int 		   	framesPerSuperBlock;/* number of frames in each superblock */
	int 			superblockSize;		/* size, in byte, of each superblock */
}rm_audio_opaque_t;


/* rm name value header -- implements name/value pairs */
typedef struct rm_name_value_map
{
    char   			nameLen; 	/* length of name field */
    char*  			pName;   	/* name of property */
    unsigned long 	type;     	/* type of property */
    unsigned short 	valueLen; 	/* length of value field */
    unsigned char*  pValue;     /* value of property */
}rm_name_value_map_t;

/* rm logical stream header -- used for surestream */
typedef struct rm_logical_opaque
{
    unsigned short  numPhyStreams;     /* number of actual streams */
    unsigned short* pPhyStreamNum;     /* array of stream numbers */
    unsigned long* 	pDataOffsets;      /* array of offsets to stream packets */
    unsigned short  numRules;          /* number of ASM rules */
    unsigned short* pRuleStreamMap;    /* maps rules to streams */
    unsigned short  numProps;          /* number of name value properties */
    rm_name_value_map_t* pProps; 	   /* array of name value properties */
}rm_logical_opaque_t;

typedef struct rm_video_opaque
{
	
    unsigned short     numRules; 		/* number of rules */
	unsigned short     numSubStreams; 	/* number of substreams */
	unsigned short	   activeSubStream;
	unsigned short*	   pRule2SubStream;	/* mapping of ASM rule number to substream */
	VIDEORMFORMAT*     pSubStreamHdr;	/* video format (extracted from codec-specific opaque data) of each substream */
} rm_video_opaque_t;

/* rm media properties header -- stream info, one for each stream */
typedef struct rm_media_props_hdr
{
    
    unsigned short streamNum;		/* stream number */
    unsigned long maxBitRate;		/* maximum bit rate of stream */
    unsigned long avgBitRate;		/* average bit rate of stream */
    unsigned long maxPacketSize;	/* max length of packet in stream (bytes) */
    unsigned long avgPacketSize;	/* averge length of packet in stream (bytes) */
    unsigned long startTime;		/* start time of stream -- clip relative */
    unsigned long preroll;			/* preroll time (milliseconds) */
    unsigned long duration;			/* duration of stream (milliseconds) */
    //BYTE   streamNameSize;        /* length of stream_name field */
    //char*  streamName;            /* string name of stream */
    //BYTE   mimeTypeSize;          /* length of mime_type field */
    //char*  mimeType;              /* MIME type of stream */
    //unsigned long typeSpecSize;   /* length of type_spec field */
    //BYTE*  typeSpec;              /* type specific data buffer */
    
    enum {RM_AUDIO = 0,	RM_VIDEO = 1, RM_LOGICAL, RM_EVENT, RM_UNKNOWN} streamMediaType; /* the type of this substream */
    
    /* the data structure of opaque data is dependent on streamMediaType */
    union {	rm_audio_opaque_t 	audio;
    		rm_video_opaque_t   video;
    		rm_logical_opaque_t logical;	/* logical stream structure */
    };
    
}__attribute__((packed)) rm_media_props_hdr_t;


typedef struct
{ 
  unsigned int	SequenceNum;	/* specify a sequence index for each frame */
  unsigned int	NumSegments;	/* the field num_packets in frame header */
  
} rv_frame_info_t;

/* Frame type enum */
typedef enum
{
    FrameTypePartial 		= 0,
    FrameTypeWhole 			= 1,
    FrameTypeLastPartial 	= 2,
    FrameTypeMultiple		= 3
} RVFrameType;

/* Struct which holds frame header info */
typedef struct rv_frame_hdr
{
    RVFrameType   eType;				/* specify how to interpret the frame data */
    unsigned long packetNum;			/* the index within the set of partial-frame packets for this frame */
    unsigned long numPackets;			/* the number of partial-frame packets */
    unsigned long frameSize;			/* size, in bytes, of the entire frame */
    unsigned long partialFrameSize;		/* size, in bytes, of the partial-frame. for WholeFrame and MultipleFrame, it is equal to frame size */
    unsigned long partialFrameOffset;	/* in partial frame packets, it specifies the offset within the entire frame */
    unsigned long timestamp;			// in million seconds
    unsigned long sequenceNum;			/* specify a sequence index for each frame */	
    
    unsigned long headerOffset;
    unsigned char bBrokenUpByUs;		/* indicate the encoder broke up the frame into partial frames */
} rv_frame_hdr_t;

/* information we should send to video */
typedef struct rv_frame
{
    VIDEORMFRAMEINFO	videoFrameInfo;		/* inband command FrameInfo */
    VIDEORMSEGMENT		videoSegmentInfo;	/* inband command SegmentInfo */
	unsigned long   	timestamp;			/* pts = timestamp * 90 */
    unsigned long		frameSize;			/* payload size */
    unsigned long		numSegments;
    unsigned char   	bBrokenUpByUs;
} rv_frame_t;

/* Struct which holds packet header info */
typedef struct rm_packet_header
{
    unsigned long  firstFrameOffset;	/* offset from start of file to the first frame of this packet */
    unsigned short version;             /* struct version */
    unsigned short packetLen;			/* the length of the packet in bytes */
    unsigned short streamNum;           /* used to associate data packet with their associated Media Properties Header */
    unsigned long  timestamp;           /* packet timestamp in milliseconds */
    unsigned short asmRule;				
    unsigned char  flags;               /* keyframe, etc. */
        
}rm_packet_header_t;

/* information used to quickly find a packet of a particular time stamp */
typedef struct rm_index_record
{
	unsigned long timestamp;	/* in milliseconds */
	unsigned long offset;		/* the offset from the start of the file at which this packet can be found. */
	
}rm_index_record_t;

/* only one index chunk per stream */
typedef struct rm_index_chunk
{
	unsigned long 	 	numIndices;		/* number of index records in the index chunk */
	unsigned long		maxIndexNum;	/* The max num of record can be stored in pIndexRecord.
										 * Only used when we want to build index table
										 */
	rm_index_record_t*  pIndexRecord;
}rm_index_chunk_t;




typedef struct _tagrm_input_plugin_t {

	/******** headers defined in real media document ********/
	
    rm_file_hdr_t			fileHeader;
    rm_properties_hdr_t 	propertyHeader;
    rm_media_props_hdr_t* 	pMediaPropsHeader;
    rm_data_chunk_t			dataChunk;	//current reference data chunk
    										
    rm_packet_header_t		packetHdr;
    rm_index_chunk_t*		pIndexChunk;
    
    /********* information used to streamming *********/
    
    rv_frame_t				currVideoFrame;
   
    
    int 					numMediaPropsHdrsAlloc;
    int						numMediaPropsHdrs;
    int						numLogicalStreamHdrs;
    
    unsigned char bHasRelativeTimeStamps;
    unsigned long	ulZeroTimeOffset;
    
    int						reservedPayloadSize;	// the len of the remaining payload not sent in the previous run
    unsigned short 			audioStreamNum;			// the number of audio stream which is playing back now. 0xFFFF means no audio stream in this file.
    unsigned short 			videoStreamNum;			// the number of video stream which is playing back now. 0xFFFF means no video stream in this file.
    
    
    
    int						ABRepeatStartMilliSec; 	//for AB repeat (point A) in milli second
    int64_t					ABRepeatEndPTS;			//for AB repeat (point B) in PTS 
    
    /********* information used to FF/FR *********/
    
    int					speed;				// the speed of source filter
    
    int					deltaPTS;			
    int					scanningSecStride;
    int					currReadTime;		/* in milliseconds, */
    
    
    int					DeliverDuration;	/* how many data in ms will be delivered in each scanning step */
    
    /******** information used for playback status *****/					
    
    //int	        		presentationTime;   	/* playback elapsed time */   
    int					timeOutCounter;			
    int					totalDuration;			// duration of clip in milliseconds
    
    int64_t				lastPresentedPTS;		/* the latest PTS updated from video or audio */
    int64_t				lastDemuxPTS;			// the lastest PTS sent to video or audio
    int64_t				tickSincePresented;		// the PTS tick since the lastest presented PTS updated
    int64_t         	lastReferenceClock;		// the lastest reference clock
    //int64_t			firstPresentedPTS;		/* the first PTS updated from video or audio */   
    
    /******** used for inband commands ********/
    
    long                fatalErrCode;
    long 				statusFlag; 
	long				cmdFlag;
	int64_t				decodeInitCMD;
	VIDEOMIXERBLEND 	blendCMD;
	VIDEODECODECOMMAND	decodeCMD;
	VIDEODVDVOBUINFO    vobuInfo;
	AUDIOFORMAT 		audioFormat;
	VIDEORMFORMAT 		videoFormat;
	NAV_REPEAT_MODE 	repeatMode;
	AUDIODECODECOMMAND  audioDecCMD;
    long				vRelatePTS;
    long				aRelatePTS;
    
    /******** used for io *********************/
    char*				mediaPath;
	osal_mutex_t*		ioMutex;
         
    rm_reader_t			reader;
    
    /******** used for interleaved audio *********************/
    unsigned char*		audioDeInterleaveBuffer;
	unsigned char		audioArrivalFrameFlag[NUM_OF_AUDIO_ARRIVAL_FRAME_FLAG];
    int					ptsOfAudioSuperBlock;
    int					audioDeliverFrameIdx ;
    int					currAudioFrameIndex;
    
    /******** used for event post *********************/
    
    /*unsigned int        eventId;
    unsigned char*      eventData;
    unsigned int        eventSize;*/

    
} rm_input_plugin_t;

int rm_reader_get_last_index_timestamp(rm_input_plugin_t *pRM, RM_INDEX_TYPE indexType);
rm_time_seek_status_t rm_reader_play_at_time(rm_input_plugin_t *pRM, int milliSec);
int rm_reader_replay(rm_input_plugin_t *pRM);
void rm_reader_flush(rm_input_plugin_t *pRM);
HRESULT rm_reader_read(rm_input_plugin_t *pRM, NAVBUF* pBuffer);
void rm_reader_close(rm_reader_t* pReader);
int rm_reader_open(rm_reader_t* pReader, unsigned char* path, NAVIOINFO* pIOInfo);

int rm_parser_read_all_headers(rm_input_plugin_t* pRM, bool* pbContinue);
void rm_parser_cleanup_all_media_props_hdrs(rm_input_plugin_t* pRM);

void rm_data_cleanup_index_chunk(rm_input_plugin_t* pRM);
long long rm_data_binary_search(rm_index_chunk_t* pIndexChunk, unsigned long timestamp, int forward, int* pTimestamp);
int rm_data_build_data_chunk_table(rm_input_plugin_t* pRM, unsigned long firstDataChunkSize, bool* pbContinue);
int rm_data_build_index_table(rm_input_plugin_t* pRM, bool* pbContinue);
int rm_data_decide_streaming_mode(rm_input_plugin_t *pRM);

int rm_depack_get_packet(rm_input_plugin_t* pRM, NAVBUF* pBuffer);

#endif //__RM_PARSER_H__

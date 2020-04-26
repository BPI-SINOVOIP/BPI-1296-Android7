#ifndef __NRD_PARSE__
#define __NRD_PARSE__

#include <hresult.h>
#include <StreamClass/EType.h>
#include <Filters/PrivateInfo.h>
#include <InbandAPI.h>
#include <Util.h>
#include <FlowEvent.h>
#include "InputNRD.h"
#include "NRDGUID.h"
#include "../../../../../common/IPC/include/rpc_common.h"

#ifdef SUPPORT_WMDRM_PD
#include "drmcommon.h"
#include "drmutilities.h"
#include "drmcrt.h"
#include "drmcontextsizes.h"
#include "drmmanager.h"
#include "oemimpl.h"
#include "drmcmdlnpars.h"
#elif defined SUPPORT_PLAYREADY
#include <drmcommon.h>
#include <drmutilities.h>
#include <drmcrt.h>
#include <drmcontextsizes.h>
#include <drmmanager.h>
#include <drmcmdlnpars.h>
#endif

#define NRD_DEBUG
#ifdef NRD_DEBUG
#define PDEBUG(fmt, args...) printf(fmt, ## args)
#else
#define PDEBUG(fmt, args...)  /*empty macro */
#endif



#define MIN_NRD_HEADER_OBJECT_SIZE	24	// sizeof(NRD_GUID) + sizeof(long long)
#define NRD_DATA_OBJECT_HEADER_SIZE 50	//Data Object GUID(16) + Object Size (8) + File ID(16) + Total Data Packets(8) + Reversed (2) = 50 bytes

#define NRD_BYTE_ALIGN_FOR_ODIRECT_OPEN	4096
#define NRD_PRESENTATION_TIME_OUT_AUDIO_ONLY	 5
#define NRD_PRESENTATION_TIME_OUT       	     200

#define NRD_CURR_STREAM_INDEX_VIDEO	0
#define NRD_CURR_STREAM_INDEX_VIDEO1	1
#define NRD_CURR_STREAM_INDEX_VIDEO2	2
#define NRD_CURR_STREAM_INDEX_VIDEO3	3
#define NRD_CURR_STREAM_INDEX_VIDEO4	4
#define NRD_CURR_STREAM_INDEX_VIDEO5	5
#define NRD_CURR_STREAM_INDEX_AUDIO	6
#define NRD_CURR_STREAM_INDEX_AUDIO1	7
#define NRD_CURR_STREAM_INDEX_AUDIO2	8
#define NRD_CURR_STREAM_INDEX_AUDIO3	9
#define NRD_CURR_STREAM_INDEX_AUDIO4	10
#define NRD_CURR_STREAM_INDEX_AUDIO5	11
#define NRD_CURR_STREAM_INDEX_AUDIO6	12
#define NRD_CURR_STREAM_INDEX_AUDIO7	13
#define NRD_CURR_STREAM_INDEX_AUDIO8	14
#define NRD_CURR_STREAM_INDEX_AUDIO9	15
#define NRD_CURR_STREAM_INDEX_UNKNOWN   16

#define NRD_FWD_NORMAL_SPEED  256
#define NRD_FWD_IONLY_SPEED   (30*256)
#define NRD_RWD_IONLY_SPEED   0
#define NRD_DVIX3_IFRAME_REPRESENT  (0-99)

#define	STATUS_NRD_AT_PAUSE_STATE		(1L << 0)
#define	STATUS_NRD_AT_RUN_STATE			(1L << 1)
#define	STATUS_NRD_AT_STOP_STATE		(1L << 2)
#define	STATUS_NRD_USE_ODIRECT_OPEN		(1L << 3)
#define	STATUS_NRD_SHOULD_SEND_SEQ_END		(1L << 4)
#define	STATUS_NRD_SHOULD_SEND_SEG_END		(1L << 5)
#define	STATUS_NRD_SHOULD_SEND_STOP		(1L << 6)
#define	STATUS_NRD_HAS_VIDEO_STREAM		(1L << 7)
#define	STATUS_NRD_HAS_AUDIO_STREAM		(1L << 8)
#define	STATUS_NRD_DEVICE_REMOVED		(1L << 9)
#define	STATUS_NRD_WAIT_PRESENTATION_FINISH	(1L << 10)
#define	STATUS_NRD_PRESENTATION_FINISHED	(1L << 11)
#define STATUS_NRD_HAS_SIMPLE_INDEX             (1L << 12)
#define STATUS_NRD_I_SCAN_MODE                  (1L << 13)
#define STATUS_NRD_SHOULD_JUMP_NEXT_I           (1L << 14)
#define STATUS_NRD_TRICKPLAY_MODE               (1L << 15)
#define STATUS_NRD_I_FRAME_FOUND                (1L << 16)
#define STATUS_NRD_SMOOTH_TRICKPLAY             (1L << 17)
#define STATUS_NRD_PLAYWITHNEWHEADER		(1L << 18)
#define	STATUS_NRD_LOADMEDIA_READY		(1L << 19) 
#define	STATUS_NRD_DATA_OBJECT_READY		(1L << 20) 
#define	STATUS_NRD_BUF_INIT_READY		(1L << 21) 
#define	STATUS_NRD_SHOULD_FILL_BUFFER0		(1L << 22) 
#define	STATUS_NRD_SHOULD_FILL_BUFFER1		(1L << 23) 


#define CMD_NRD_SET_AUDIO_FORMAT		(1L << 0)
#define	CMD_NRD_NEW_SEGMENT			(1L << 1)
#define	CMD_NRD_DECODE_INIT			(1L << 2)
#define	CMD_NRD_BLEND				(1L << 3)
#define	CMD_NRD_DECODE				(1L << 4)
#define	CMD_NRD_AUDIO_MEDIA_PROPERTY		(1L << 5)
#define	CMD_NRD_PRESENTATION_TIME		(1L << 6)
#define	CMD_NRD_END_OF_SEQUENCE			(1L << 7)
#define	CMD_NRD_END_OF_SEGMENT			(1L << 8)
#define	CMD_NRD_STOP				(1L << 9)
#define CMD_NRD_POST_EVENT			(1L << 10)
#define CMD_NRD_RESET_SPEED			(1L << 11)
#define CMD_NRD_FLUSH				(1L << 12)
#define CMD_NRD_AUDIO_WMAPRO_DRC		(1L << 13)
#define CMD_NRD_AUDIO_WMAPRO_DOWNMIX_MATRIX	(1L << 14)
#define CMD_NRD_SET_VIDEO_FORMAT                (1L << 15)
#define CMD_NRD_PENDING_DCI_CCI                 (1L << 16)



// flags for wFormatTag field of WAVEFORMAT 
#define	WMA_FORMAT_PCM      	0x0001
#define	WMA_FORMAT_ADPCM	0x0002 
#define WMA_FORMAT_MPEGLAYER3 	0x0055 
#define WMA_FORMAT_WMAPRO       0x0162
#define WMA_FORMAT_G726_ADPCM   0x0045


// flags for ompressionID field for _bitmap_info_header_t 
#define WMV_FORMAT_WMV1         0x31564D57
#define WMV_FORMAT_WMV2         0x32564D57
#define WMV_FORMAT_WMV3         0x33564D57
#define WMV_FORMAT_MP4S         0x5334504D
#define WMV_FORMAT_M4S2         0x3253344D
#define WMV_FORMAT_MP43         0x3334504D
#define WMV_FORMAT_WVC1         0x31435657
#define WMV_FORMAT_WMVA         0x41564D57
// dvix4
#define WMV_FORMAT_DVIX         0x58564944


#define DRM_KEY_LENGTH          15

/* nrd_read_payloads() return status */
typedef enum {
	NRD_READ_ERROR = IOPLUGIN_ERROR_GENERIC, 		//-1
	NRD_READ_END =  IOPLUGIN_ERROR_EOF,			//-2
	NRD_READ_ERROR_BUSY = IOPLUGIN_ERROR_BUSY, 	//-3
	NRD_NO_DEVICE = IOPLUGIN_ERROR_NO_DEVICE,		//-4
	NRD_READ_AGAIN = -5,
	NRD_READ_BITSTREAM_ERROR = 0,
	NRD_READ_DATA = 1,
	NRD_READ_COMMAND = 2,		
	//NRD_READ_AGAIN = 3,  
	NRD_READ_HEADER_ERROR = 4
} nrd_read_status_t;

typedef enum {
	NRD_LICENSE_INIT,
	NRD_LICENSE_GET_SUCCESS,
	NRD_LICENSE_PROC_SUCCESS,
	NRD_LICENSE_FAIL,
	NRD_LICENSE_FAIL_SEND,
	NRD_LICENSE_UNKNOWN
} ENUM_NRD_LICENSE_STATUS;

//struct for getting header info from DPI
typedef struct _nrd_AudioAttributes
{
	uint32_t streamID_;
	uint32_t formatID_;
	uint32_t numberOfChannels_;
	uint32_t samplesPerSecond_;
	uint32_t bytesPerSecond_;
	uint32_t blockAlignment_;
	uint32_t bitsPerSample_;
	bool     encrypted_;
	uint32_t codecSpecificDataSize_;
	unsigned char*	 codecSpecificData_;
} nrd_AudioAttributes;

typedef struct _nrd_VideoAttributes
{
	uint32_t streamID_;
	uint32_t formatID_;
	uint32_t imageWidth_;
	uint32_t imageHeight_;
	uint32_t bitsPerPixel_;
	uint32_t pixelAspectRatioX_;
	uint32_t pixelAspectRatioY_;
	bool     encrypted_;
	uint32_t codecSpecificDataSize_;
	unsigned char*	 codecSpecificData_;
} nrd_VideoAttributes;

typedef struct _nrd_StreamAttributes
{
	uint32_t         streamIndex_;
	int64_t          ptsOffset_;
	nrd_AudioAttributes* audioAttributes_;
	nrd_VideoAttributes* videoAttributes_;
} nrd_StreamAttributes;

//////////////////////////////////
// Trick Play related structure //
typedef struct _nrd_count_jump_res {
	int      bSuccess;
	int      bUseIndex;    
	long long dest;    
}nrd_count_jump_res_t;


typedef struct _nrd_smooth_tp_index {
	int      startIdx;
	int      interval;        
}nrd_smooth_tp_index_t; 

typedef struct _nrd_jump_collector {
	int      bNeedToJump;
	long long jumpPos;
} nrd_jump_collector_t;

///////////////////////////////////////////////



typedef struct _nrd_pinpong_buffer_st {
	unsigned char*   	myBuffer; 
	int                 totalReadBytes;
}nrd_pinpong_buffer_t;



typedef struct _nrd_sub_header
{
	NRD_GUID 	GUID;	
	long long 	objectSize;
}__attribute__((packed)) nrd_sub_header_t;


typedef struct _header_object
{
	NRD_GUID 	GUID;	
	long long 	objectSize;		
	long		numHeaders;			// number of objects within the HEADER OBJECT, not including THIS one
	char		reserved1;			// NRD parser may safely ignore THIS value
	char		reserved2;			// must be set to 0x02

} __attribute__((packed)) header_object_t;


typedef struct _file_properties_object
{
	NRD_GUID	fileID;
	long long	fileSize;				// the size of the entire file
	long long	creationDate;			// the date of the initial creation of the file
	long long	dataPacketsCount;		// number of Data Packet within the Data Object.
	long long	playDuration;			// the time need to play THIS file in 100-nanosec units.
	long long	sendDuration;			// the time need to send THIS file in 100-nanosec units.
	long long 	preroll;				// the time to buffer data before playing THIS file
	struct {

		unsigned long broadcastFlag : 1;			// bit 0 : broadcast flag
		unsigned long seekableFlag : 1;			// bit 1 : seekable flag
		unsigned long reserved : 30;
	} __attribute__((packed)) flags;					
	long		minDataPacketSize;	// the minimum Data Packet size in bytes
	long		maxDataPacketSize;	// the maximun Data Packet size in bytes
	long		maxBitRate;			// the maximum instantaneous bit rate in bits per second for THIS file


} __attribute__((packed)) file_properties_object_t;

typedef struct _stream_properties_object
{
	NRD_GUID	streamType;
	NRD_GUID	errCorrType;		// audio stream : NRD_Audio+Spread, the others : NRD_No_Error_Correction
	long long	timeOffset;					// the presentation time offset of the stream in 100-nanosec units
	long		typeDataLen;
	long		errCorrDataLen;
	struct {
		unsigned char streamNum : 7;
		unsigned char reserved : 8;
		unsigned char encryptedContentFlag : 1;
	}__attribute__((packed)) flags;	
	//short flags;					
	long		reserved;


} __attribute__((packed)) stream_properties_object_t;


typedef struct _header_extension_object
{
	NRD_GUID	reserved1;
	short		reserved2;
	long		extensionDataSize;

} __attribute__((packed)) header_extension_object_t;

typedef struct _audio_media_data
{
	short			codecID;			// unigue ID of the codec used to encode audio
	short			numChannel;			
	long			samplesPerSec;
	long			avgBtyesPerSec;
	short			blockAlign;
	short			bitsPerSample;
	short			specificDataSize;
	//unsigned char*	specificData;
} __attribute__((packed)) audio_media_data_t;

typedef struct _video_media_data
{
	long			imageWidth;
	long 			imageHeight;
	char			reservedFlag;
	short 			formatDataSize;
	//unsigned char*	formatData;
} __attribute__((packed)) video_media_data_t;

typedef struct _bitmap_info_header
{
	long            formatDataSize;
	long            imageWidth;
	long            imageHeight;
	short           reserved;
	short           bitsPerPixelCnt;
	long            compressionID;
	long            imageSize;
	long            horizontalPixels;
	long            verticalPixels;
	long            colorsUsedCnt;
	long            importantColorsCnt;    
} __attribute__((packed)) _bitmap_info_header_t;


typedef struct _presentation_time
{
	long PTS;
	long deltaPTS;

} __attribute__((packed)) presentation_time_t;

typedef struct _error_correct_data
{


	unsigned char errCorrDataLen : 4; 	// Error Correction Data Length
	unsigned char opaqueDataPresent : 1;			// Opaque Data Present
	unsigned char errCorrLenType : 2;	// Error Correction Length Type
	unsigned char errCorrPresent : 1;			// Error Correction Present 


} __attribute__((packed)) error_correct_data_t ;

typedef struct _payload_extension
{
	NRD_GUID 	GUID;
	short		extDataSize;
	long		extSysInfoLen;	// Extension System Info Length
	struct _payload_extension* next;	// pointer to the next payload extension system

} __attribute__((packed)) payload_extension_t;

typedef struct _extended_stream_properties
{
	long long 	startTime;		// the presentation time of the first object
	long long 	endTime;		// the presentation time of the last object
	long 		dataBitrate1;	// 
	long 		bufferSize1;
	long		bufferFullness1;
	long		dataBitrate2;	// Alternate Data Bitrate
	long		bufferSize2;	// Alternate Buffer Size
	long		bufferFullness2;	// Alternate Initial Buffer Fullness
	long		maxObjectSize;		// max size of the largest sample in the data packet
	long		flags;
	short		streamNum;
	short		streamLanguateIDidx;	// Stream Language ID index
	long long	avgTimePerFrame;
	short		streamNameCount;
	short		payloadExtSysCount;		// Payload Extension System Count
	struct { 
		short	LangIDidx;		// Language ID index
		short	streamNameLen;
	}__attribute__((packed)) streamNames;

	payload_extension_t* payloadExtSystems;

} __attribute__((packed)) extended_stream_properties_t;

typedef struct _media_properites
{
	unsigned char	isAudio;		// 0: THIS entry is the properites of video stream; 
	// otherwise, it is audio's;
	union {
		audio_media_data_t	audio;
		video_media_data_t	video;
	};

	unsigned char*	mediaData;	// if Stream Type is NRD_Audio_Media, it is specificData of Audio media data
	// otherwise, it is formatData of Video media data
} __attribute__((packed)) media_properties_t;

typedef struct _payload_parsing_info
{
	/*-----------------------------------
	  payload parsing information fields
	  -------------------------------------*/
	struct {

		unsigned char multiplePayloadsPresent : 1; 	// Multiple Payloads Present
		unsigned char sequenceType : 2;		// Sequence Type
		unsigned char paddingLenType : 2;	// Padding Length Type
		unsigned char packetLenType : 2;	// Packet Length Type
		unsigned char errorCorrPresent : 1;		  	// Error Correction Present 

	} __attribute__((packed)) lenTypeFlag;

	//char	propertyFlags;  
	struct {

		unsigned char dataLenType : 2;		// Replicated Data Length Type
		unsigned char offsetLenType : 2;	// Offset into Media Object Length Type
		unsigned char objectNumType : 2;	// Media Object Number Length Type
		unsigned char streamNumType : 2;	// Stream Number Length Type
	} __attribute__((packed)) propertyFlags;

	long	packetLen;		// the length of the data packet
	long	paddingLen;		// the length of the padding at the end of the data packet
	long	sendTime;		// the send time of the Data Packet
	short	duration;		// duration of the Data Packet

	/*-----------------------------------
	  payload  fields
	  -------------------------------------*/

	struct {

		unsigned char	numPayloads : 6;		// Number of Payloads
		unsigned char	payloadLenType : 2;		// Payload Length Type	
	} __attribute__((packed)) payloadFlags;

	struct {
		unsigned char streamNum : 7;	// the number of the stream
		unsigned char isKeyFrame : 1;			// Key frame bit
	} __attribute__((packed)) streamNumFlags;

	union {
		long					mediaObjectOffset;	// non-compressed Payloads
		presentation_time_t 	presentationTime;	// Compressed Payloads
	};

	/*----------------------------------------
	  the followings are not standard  fields
	  ------------------------------------------*/

	long	packetHeaderLen;	// the length of Payload parsing information
	long	payloadHeaderLen;	
	long	replicatedDataLen;	
	long	totalSubPayloadSize;	// for compressed payload data, THIS field is total subpayload data size
	// for multiple payloads single payload data, it is the size of one payload data
	long    packetTotalReadBytes;	// count if each packet really match to maxDatapacketSize								
	long    mediaObjectSize;									
	long	pts;													

} __attribute__((packed)) payload_parsing_info_t;

typedef struct _idx_specifier
{
	short streamNum;
	short indexType;

} __attribute__((packed)) idx_specifier_t;

typedef struct _idx_block
{
	long	    entry_count;
	long long*  blockPosition;	// long long blockPosition[specifiersCount]
	long*		offset;			// long offset[entry_count][specifiersCount]

} __attribute__((packed)) idx_block_t;

typedef struct _index_object
{
	long	timeInterval;		// Index Entry Time Interval
	short	specifiersCount;	// Index Specifiers Count
	long	blocksCount;		// Index Blocks Count

	idx_specifier_t* 	idxSpecifers;
	idx_block_t* 		idxBlocks;

} __attribute__((packed)) index_object_t;


typedef struct _s_idx_block_t
{
	int     packetNumber;
	short   packetCount;
} __attribute__((packed)) s_idx_block_t;

typedef struct _simple_index_object
{    
	long long	timeInterval;
	int         packetCount;
	int         idxCount;
	s_idx_block_t*  idxEntryTable;
} __attribute__((packed)) simple_index_object_t;

typedef struct _metadata_description
{
	short 			streamNum;
	unsigned char*  pName;
	unsigned char*  pData;
	short 			nameLen;
	int 			dataLen;
	short			dataType;

} metadata_description_t;

typedef struct _metadata_object
{
	int recordCount;
	metadata_description_t* pDescRecord;
} metadata_t;

#define NRD_BUFSIZE        (64 * 1024) // 64K bytes
#define NRD_CMDBUFSIZE      256
#define MAX_STREAM_NUM		127

typedef struct _tagnrd_input_plugin {

	/*--------------------------------------------------------------
	  Medium information
	  ---------------------------------------------------------------*/

	file_properties_object_t	  FileProperties;	
	media_properties_t*			  nrdStream[MAX_STREAM_NUM]; // array index is (streamNum - 1)
	extended_stream_properties_t* extStreamProperties[MAX_STREAM_NUM]; // array index is (streamNum - 1)
	metadata_t					  metaData;
	payload_parsing_info_t		  payloadInfo;
	index_object_t*				  idxObject;
	simple_index_object_t*        sidxObject;

	/*--------------------------------------------------------------
	  Playback information
	  ---------------------------------------------------------------*/
	int					  timeOutCounter;
	int					  reservedPayloadSize;	// the len of the remaining payload not sent in the previous run
	char				  curStreamNum[NRD_CURR_STREAM_INDEX_UNKNOWN+1];		// currStreamNum[0] : the number of current video stream
	// currStreamNum[1] : the number of current audio stream
	// currStreamNum[2] : the number of current audio stream
	// currStreamNum[3] : the number of remembering unknown stream
	long long			  dataObjectStart;		// start address of the Data Object
	long long			  dataObjectEnd;			// end address of the Data Object
	// long long			  curReadAddr;			// current read position
	int64_t				  lastPresentedPTS;
	int64_t 			  presentationTime;
	unsigned long		  prevPTS;
	NAV_REPEAT_MODE 	  repeatMode;
	nrd_jump_collector_t  jumpCollector;

	/*--------------------------------------------------------------
	  Inband command stuff
	  ---------------------------------------------------------------*/

	long 				statusFlag; 
	long				cmdFlag;
	int64_t				decodeInitCMD;
	ENUM_MEDIA_TYPE     videoType[5];              // VC-1 or MPEG4
	VIDEOMIXERBLEND 	blendCMD;
	VIDEODECODECOMMAND	decodeCMD;	
	VIDEOFORMAT         videoFormat[5];
	VIDEODIVX4          videoFormatDivx[5];
	VIDEOVC1SEQINFO     videoFormatVC1[5];
	AUDIOFORMAT 		audioFormat;	
	WMAPRODRC			DRCSettings;
	WMAPRODOWNMIXMATRIX	downMixMatrix;
	NAVLOADFAILURE      errorLoadMsg;

	/*--------------------------------------------------------------
	  WMV stuff
	  ---------------------------------------------------------------*/
	int64_t             prevVideoSendPTS;       // for preventing send duplicated PTS
	char                bIsFirstVideoFrame;     // in MPEG4, first video frame has to send sequence header
	char                bIsCompressPayload;
	int                 curAudioStreamNum;      // for mutiple audio channel
	int                 curVideoStreamChannel;      // for mutiple video channel
	int                 bIsVideoUseDefaultStream; // use default video stream num or the first packet
	int64_t             ptsOffset;              // when pts too big, minus the offset..
	bool                bNotHonorComPressPayload; // compress payload happens error before, used to decide
	// when payload type(compressed or not) need to honor previous packet
	bool                bCompressPayloadParsing;	                                              


	/*--------------------------------------------------------------
	  Trick Play stuff
	  ---------------------------------------------------------------*/
	int                 speed;
	int                 skip;
	int                 idxOfIndex;            // trick play's index reference (has index table)
	int                 fakeIndexJumpOffset;   // trick play's offset reference (no index table)
	int                 latestIFrameSize;
	int                 latestidxOfIndexNumber;	
	unsigned int        contextReport;
	unsigned int        context;


	/*--------------------------------------------------------------
	  Smooth Trick Play stuff
	  ---------------------------------------------------------------*/
	nrd_smooth_tp_index_t smoothIdx;
	long long           smoothPlayEndPos;


	/*--------------------------------------------------------------
	  event stuff
	  ---------------------------------------------------------------*/
	long                fatalErrCode;	
	unsigned int        eventId;
	bool                bSendStop;


	/*--------------------------------------------------------------
	  IO stuff
	  ---------------------------------------------------------------*/
	long long			  currFilePos;
	int					  currReadOffset;
	int                   payloadFilePos;   // used in compressed payload, error happens sometimes
	int					  totalReadBytes;
	long long             fileTotalSize;
	long long             currFileReadPos;

	unsigned char*   	  buffer; 		
	unsigned char*        cmdBuffer;
	nrd_pinpong_buffer_t  bufferManager[2];  // ping-pong buffer
	void*				  fileHandler;
	IOPLUGIN* 			  ioPlugin;
	osal_mutex_t*		  ioMutex;
	bool                  isStreaming;
	NAVIOINFO             m_ioInfo;
	char *                mediaPath;

	/*-------------------------------------------------------------
	  DRM stuff 
	  -------------------------------------------------------------*/
	int                   drmVersionNumber; // we only support DRM V2 header now
	// if only V1 exist, return "Invalid File"
	unsigned char         *drmKeyArray; // pli continuous memory. 
	unsigned char         *drmKeyPtr;
	VIDEODVDDCICCI        DCICCI;


	//for A-B repeat added by baili_feng
	int     abStartSec;
	int     abStartFrame;
	int     abEndSec;
	int     abEndFrame;

	nrd_StreamAttributes nrd_sa;
	nrd_AudioAttributes nrd_aa;
	nrd_VideoAttributes nrd_va;

        int nrd_AudioAttributeNotReady;
#ifdef SUPPORT_WMDRM_PD
	DRM_DWORD                     extendedContentEncryptionDataCount;
	DRM_BYTE                      *pExtendedContentEncryptionData;
	DRM_MANAGER_CONTEXT           drmManager;
	DRM_MANAGER_DECRYPT_CONTEXT   drmDecryptContext;
	DRM_CONST_STRING              DrmTmpLicenseStoreName;  
	DRM_CONST_STRING              DrmFlashLicenseStoreName;  
	DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS    drmOutputLEvels;  
	pthread_t                     m_thread_id_drm;
	ENUM_NRD_LICENSE_STATUS       licenseStatus;
#elif defined SUPPORT_PLAYREADY
	DRM_DWORD                     extendedContentEncryptionDataCount;
	DRM_BYTE                      *pExtendedContentEncryptionData;
	DRM_APP_CONTEXT               drmManager;
	DRM_DECRYPT_CONTEXT           drmDecryptContext;
	DRM_CONST_STRING              DrmTmpLicenseStoreName;  
	DRM_CONST_STRING              DrmFlashLicenseStoreName;  
	DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS    drmOutputLEvels;  
	pthread_t                     m_thread_id_drm;
	ENUM_NRD_LICENSE_STATUS       licenseStatus;
#endif

} nrd_input_plugin_t;


HRESULT nrd_read_header_objects(nrd_input_plugin_t* THIS);
int nrd_read_data_packets(nrd_input_plugin_t* THIS);
HRESULT nrd_send_command(nrd_input_plugin_t *THIS, NAVBUF* pBuffer);
nrd_read_status_t nrd_read_payloads(nrd_input_plugin_t* THIS, NAVBUF* pBuffer);
#endif //__NRD_PARSE__


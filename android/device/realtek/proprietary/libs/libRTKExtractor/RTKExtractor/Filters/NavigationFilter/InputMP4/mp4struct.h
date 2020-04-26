#ifndef __MP4_STRUCT__
#define __MP4_STRUCT__

#include <hresult.h>
#include <StreamClass/EType.h>//define "MEDIASUBTYPE_MP4"
#include <Filters/PrivateInfo.h>//define "AUDIOFORMAT"
#include <InbandAPI.h>
#include <Util.h>

#ifndef __MP4_ALOG__
#define __MP4_ALOG__

//#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define LOG_TAG "RTKInputMP4"
#include <utils/Log.h> 
#define printf(fmt, args...) ALOGE("%s(%d): " fmt, __FUNCTION__, __LINE__, ##args)
#define DBGP(fmt, args...) ALOGE("%s(%d): " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define printf(fmt, args...)
#define DBGP(fmt, args...)
#endif

#endif

#include "InputMP4.h"
//#include "MP4FID.h"
#include "FlowEvent.h"
#include "mp4ffint.h"
#define MIN_MP4_HEADER_OBJECT_SIZE	  24	// sizeof(MP4_GUID) + sizeof(long long)
#define MP4_DATA_OBJECT_HEADER_SIZE	  50
#define MP4_PRESENTATION_TIME_OUT	    15
#define MP4_PRESENTATION_TIME_OUT_LIMIT  100
#define MP4_MIN_SCAN_SPEED		        (15*256)
#define MP4_NORMAL_SPEED		          256
#define MP4_INITIAL_FORWARD_SCAN_PTS	1000000
#define MP4_INITIAL_REVERSE_SCAN_PTS	100000000000000
#define MP4_FWD_IONLY_SPEED           512
#define MP4_RWD_IONLY_SPEED           0

#define	STATUS_MP4_AT_PAUSE_STATE			(1L << 0)
#define	STATUS_MP4_AT_RUN_STATE				(1L << 1)
#define	STATUS_MP4_AT_STOP_STATE			(1L << 2)
#define	STATUS_MP4_AT_PARSER_MODE			(1L << 3)
#define	STATUS_MP4_SHOULD_SEND_SEQ_END		(1L << 4)    
#define	STATUS_MP4_SHOULD_SEND_SEG_END		(1L << 5)    
#define	STATUS_MP4_SHOULD_SEND_STOP			(1L << 6)    
#define	STATUS_MP4_HAS_VIDEO_STREAM			(1L << 7)    
#define	STATUS_MP4_HAS_AUDIO_STREAM			(1L << 8)    
#define	STATUS_MP4_USE_ODIRECT_OPEN			(1L << 9)  
#define	STATUS_MP4_SHOULD_SET_INITIAL_PTS   (1L << 10)    
#define	STATUS_MP4_PRESENTATION_FINISHED	  (1L << 11)
#define STATUS_MP4_WAIT_PRESENTATION_FINISH (1L << 12) 

//#define CMD_MP4_SET_FORMAT_MP4			(1L << 0)
#define CMD_MP4_SET_AUDIO_FORMAT		(1L << 0)	// Ivan*
#define	CMD_MP4_NEW_SEGMENT				(1L << 1)
#define	CMD_MP4_DECODE_INIT				(1L << 2)
#define	CMD_MP4_BLEND					(1L << 3)
#define	CMD_MP4_DECODE					(1L << 4)
#define	CMD_MP4_AUDIO_VORBIS_DES	(1L << 5)
#define	CMD_MP4_AVSYNC_CHANGE	(1L << 6)
#define	CMD_MP4_END_OF_SEQUENCE			(1L << 7)
#define	CMD_MP4_END_OF_SEGMENT			(1L << 8)
#define	CMD_MP4_STOP					(1L << 9)
#define	CMD_MP4_SET_FORMAT_MP4HE		(1L << 10)

#define	CMD_MP4_POST_EVENT				(1L << 13)
#define	CMD_MP4_RESET_REPEAT_MODE		(1L << 14)
#define	CMD_MP4_FLUSH					(1L << 15)    
#define	CMD_MP4_RESET_SPEED				(1L << 16) 
#define CMD_MP4_SET_VIDEO_FORMAT		(1L << 17)	// Ivan+
#define CMD_MP4_SEND_VIDEO_DES		    (1L << 18)	// yllin+
#define	CMD_MP4_READ_ERROR				(1L << 19)
#define   CMD_MP4_SEND_SUBTITLE                   (1L << 20)//RAYsubtitle
#define CMD_MP4_SPIC_FLUSH                   (1L << 21)//RAYsubtitle
#define CMD_MP4_SPIC_CONFIG                   (1L << 22)//RAYsubtitle
#define CMD_MP4_SPIC_FORMAT                   (1L << 23)//RAYsubtitle
#define CMD_MP4_SPU_PALETTE                   (1L << 24)//RAYsubtitle
#define CMD_MP4_AUDIO_FLUSH                   (1L << 25)//RAY flush audio
#define CMD_MP4_SEND_VBOU                   (1L << 26)//RAY flush audio
#define CMD_MP4_SET_VIDEO_USABILITY_INFO                   (1L << 27)//RAY set video us
#define  CMD_MP4_FLUSH_AND_STOP    (1L << 28)//RAY set video us
//=== Ivan+ =====================================================
#define MP4_STSZ_SIZE        (2048) 
#define MP4_STSC_SIZE        (2048) 
#define MP4_STCO_SIZE        (2048) 

#define MP4_IO_SIZE			(64 * 1024)
#define MP4_IO_ALIGN_MASK	0xfffffffffffff000LL
#define MP4_IO_ALIGN_CHECK	0xfff
#define MP4_AVSYNC_MAXPTS (1*90000LL)
#define MP4_AVSYNC_MINCUKS 2
//===============================================================

#define MP4_BUFSIZE			(64 * 1024) // 32K bytes
#define MP4_TXTBUFSIZE		(4 * 1024) //

#define MP4_BYTE_ALIGN_FOR_ODIRECT_OPEN	2048
#define MP4_IOERROR_CNT_LIMIT           15
#ifdef TOOLCHAIN344
#undef MP4_IO_SIZE
#undef MP4_BUFSIZE
#undef MP4_IO_ALIGN_MASK
#undef MP4_IO_ALIGN_CHECK
#define MP4_IO_SIZE			(64 * 1024)
#define MP4_BUFSIZE			(64 * 1024) // 32K bytes
#define MP4_IO_ALIGN_MASK	0xfffffffffffff000LL
#define MP4_IO_ALIGN_CHECK	0xfff
#endif
/* read_payloads() return status */
typedef enum {
	MP4_READ_DATA,
	MP4_READ_END,
	MP4_READ_COMMAND,		
	MP4_READ_AGAIN,
	MP4_READ_HEADER,
} mp4_read_status_t;

typedef struct _mp4stream
{
	long long file_address;	// Ivan+
	unsigned long byte_used;
	unsigned long byte_left;
//	unsigned char* bufferNonCached;
	unsigned char* data_pool;
} mp4stream_t;

typedef struct _riffchunk{
	uint32_t fcc;
	uint32_t  cb;
} RIFFCHUNK, * LPRIFFCHUNK;

#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |    \
				 (((uint32_t)(ch4) & 0xFF00) << 8) |    \
				 (((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
				 (((uint32_t)(ch4) & 0xFF000000) >> 24))

//===============================================================

typedef struct _tagmp4_input_plugin {

/*--------------------------------------------------------------
    	Medium information
---------------------------------------------------------------*/

//    file_properties_object_t	  FileProperties;	
//    payload_parsing_info_t	  payloadInfo;
//    index_object_t*		  idxObject;
    
/*--------------------------------------------------------------
    	Playback information
---------------------------------------------------------------*/
	int64_t				presentationTime;
	int					timeOutCounter;	
	int64_t				lastPresentedPTS;
	NAV_REPEAT_MODE	repeatMode;
	mp4ff_t				mp4ff;              // mp4 file information
	int					NextDeliverType;	// Ivan*
/*--------------------------------------------------------------
		Inband command stuff
---------------------------------------------------------------*/
    
	long				statusFlag; 
	long				cmdFlag;
	int64_t				decodeInitCMD;
	NAVMEDIAINFO		media;				// Ivan*
	VIDEOMIXERBLEND		blendCMD;
	VIDEODECODECOMMAND	decodeCMD;    
	AUDIOFORMAT			audioFormat;
	VIDEOFORMAT			videoFormat;		// Ivan*
	VIDEODIVX4			videoFormat2;		// Ivan*
	int64_t				i64VideoLastPTS;	// neco+
	int64_t				i64AudioLastPTS;	// neco+
	int64_t				lastSentPTS;
	int64_t       currSystemPTS;     // yllin+	
/*--------------------------------------------------------------
		IO stuff
---------------------------------------------------------------*/
	NAVIOINFO			ioInfo;//ABRAY
	int64_t			streamTotalBytes;//ABRAY
	unsigned char*		buffer;
	//unsigned char*	bufferNonCached;    
	void*				fileHandler;
	IOPLUGIN*			ioPlugin;
	osal_mutex_t*		ioMutex;
      
	unsigned int		eventId;
	unsigned char*		eventData;
	unsigned int		eventSize;         
	long				fatalErrCode;     
	mp4stream_t			mp4stream; 
	bool        bIsSeekAudioBySample;
	
	//set speed
	int         m_speed;
	int         m_skip;
	int         idxToSkip;
	int         extraWait;
	
  unsigned char bIsSeekEnd; 
  unsigned char bDirectIO; 
  unsigned long videoTrackCnt;
  unsigned long audioTrackCnt;  
  unsigned long IOErrorCnt;
  unsigned char AudioEOS;
  unsigned char VideoEOS;
  
#ifdef ENABLE_CMOV  
  unsigned char bIsHasCMOV;
  unsigned char *moov_buf;
  unsigned char *cmov_buf;  
#endif
  FILE* dumphandler;//RAY ++
  int subtrackidx;
  bool bSubtitleSync;
  int m_spicOnOff;
  SPUCONFIG	spuConfig;
  EventSpicFrame m_spicFrame;
  int total_subtracks;
  char *txtBuffer;
  char *ptxtBuffer;
  int vorbisheadersendcnt;//for vorbis
  int bseekbwd;
  int frametosent;
  int abEndSec;
  int abEndFrame;
  int	abStartSec; 
  int	abStartFrame;
  bool* pbLoadmediaContinue;
  VIDEODVDVOBUINFO m_vobuinfo;
  NAVLOADFAILURE* m_pFailure;
  int64_t totalread;
  int64_t apts;
  int64_t vpts;
  bool clipend;
  unsigned char*		DecryptBuffer;
#ifdef ENABLE_FILELIST_PLAYBACK
  int m_numfiles;
  int m_curFile;
  int m_loading;
  long long m_ptsbase;
#endif
  bool b_mUsedInband;
} mp4_input_plugin_t;

unsigned int get_sample_rate(const unsigned char sr_index);
unsigned char get_sr_index(const int samplerate);

#endif //__MP4_STRUCT__



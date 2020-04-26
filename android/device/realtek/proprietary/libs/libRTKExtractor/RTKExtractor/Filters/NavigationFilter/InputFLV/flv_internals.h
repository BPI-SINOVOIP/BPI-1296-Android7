#ifndef __FLV_INPUT_PLUGIN_INTERNAL_HEADER__
#define __FLV_INPUT_PLUGIN_INTERNAL_HEADER__

#include <OSAL/Types.h>
#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/PrivateInfo.h>
#include <FlowEvent.h>
#include <StreamClass/EType.h>
#include <stdio.h> //for debug
#include <sys/time.h>
#include "utility.h"
#include "mmreg.h"

#undef RDDEBUG

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

#define FLV_IO_SIZE			(128*1024)

#define FLV_IO_ALIGN_MASK	0xfffffffffffff000LL
#define FLV_IO_ALIGN_UNIT	(~FLV_IO_ALIGN_MASK)

#define FLV_CMD_SIZE		(256)

#define FLV_BUFSIZE 		(128*1024)
//#define FLV_BUFSIZE 		(256*1024)

#define FWD_IONLY_SPEED 256
#define RWD_IONLY_SPEED 0

/* private table definition */
#define PENDING_AUDIO_FORMAT	  0x00000001
#define PENDING_VIDEO_FORMAT	  0x00000002
#define PENDING_SPIC_FORMAT		  0x00000004
#define PENDING_AUDIO_AAC_FORMAT  0x00100008
#define PENDING_AUDIO_WMA_TWEAK	  0x00000010
#define PENDING_DIVX3_TWEAK		  0x00000020
#define PENDING_AUDIO_AAC_TWEAK   0x00000040
#define PENDING_VIDEO_H264_HEADER 0x00000080



#define PENDING_AUDIO_FLUSH		0x00000100
#define PENDING_VIDEO_FLUSH		0x00000200
#define PENDING_SPIC_FLUSH		0x00000400
#define PENDING_ALL_FLUSH		0x00000800
#define PENDING_SPIC_CONFIG		0x00001000
#define PENDING_RESET_SPEED		0x00002000
#define PENDING_VIDEO_END_SEQ	0x00004000
#define PENDING_DCI_CCI			0x00008000
#define PENDING_EOS_RDY			0x00010000
#define PENDING_NOT_AUTH_USER	0x01000000
#define PENDING_RENTAL_EXPIRED	0x02000000
#define PENDING_REQUEST_RENTAL	0x04000000
#define PENDING_WAIT_APPROVE	0x08000000
#define PENDING_NOT_AUTHORIZED	0x10000000
#define PENDING_READ_ERROR		0x20000000
#define PENDING_SPIC_FRAME      0x40000000
#define PENDING_STOP_RDY		0x80000000

#define STATE_RUNNING			0x00000001
#define STATE_PAUSED			0x00000002
#define STATE_INITIALIZED		0x00000004
#define STATE_HEADER_PARSED		0x00000008
#define STATE_MENU_RESUME		0x00010000
#define STATE_STOP_RESUME		0x00020000
#define STATE_IO_ERROR			0x00040000
#define STATE_IO_READ_ERROR		0x00080000

#define	CMD_FLV_NEW_SEGMENT			(1L << 0)
#define	CMD_FLV_DECODE_INIT			(1L << 1)
#define	CMD_FLV_BLEND				(1L << 2)
#define	CMD_FLV_DECODE				(1L << 3)
#define	CMD_FLV_VIDEO_FORMAT		(1L << 4)
#define CMD_FLV_END_OF_SEGMENT      (1L << 7)
#define CMD_FLV_STOP                (1L << 8)
#define CMD_FLV_MEDIA_REMOVED		(1L << 9)
#define CMD_FLV_AVSYNC_CHANGE		(1L << 10)
#define CMD_FLV_VIDEO_MPEG4_USABILITY_INFO 		(1L << 11)
#define CMD_FLV_LOAD_NEXTCLIP		(1L << 12)

//#define FLV_RETRIAL_LIMIT	10
#define FLV_IOERROR_LIMIT	64

#define FLV_TYPE_NUMBER      0x00
#define FLV_TYPE_BOOL        0x01
#define FLV_TYPE_STRING      0x02
#define FLV_TYPE_OBJECT      0x03
#define FLV_TYPE_MOVIECLIP   0x04
#define FLV_TYPE_NULL        0x05
#define FLV_TYPE_UNDEF       0x06
#define FLV_TYPE_REFERENCE   0x07
#define FLV_TYPE_ECMAARRAY   0x08
#define FLV_TYPE_STRICTARRAY 0x0a
#define FLV_TYPE_DATE        0x0b
#define FLV_TYPE_LONGSTRING  0x0c

#ifndef IS_SET
		#define IS_SET(x,y)		((x) & (y))
		#define ClearFlag(x,y)	((x) &= ~(y))
		#define SetFlag(x,y)	((x) |= (y))
#endif

//#define ENABLE_DEBUG
#include <utils/Log.h>
#ifdef ENABLE_DEBUG
#define LOG_NDEBUG 0
//#define RTKDBG(fmt, args...) LOGE("%s(%d): " fmt, __FUNCTION__, __LINE__, ##args)
#define printf(fmt, args...) ALOGE("%s(%d): " fmt, __FUNCTION__, __LINE__, ##args)
#define FLVDEBUG(fmt, args...) ALOGE("%s(%d): " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define printf(fmt, args...)
#define FLVDEBUG(fmt,args...)
//#define LOG_NDEBUG 0
#endif
//#define FLVDEBUG(fmt,args...)  printf(fmt,##args)
//#define FLVDEBUG(fmt,args...)

//return code
#define RET_OK     0
#define RET_FAIL   -1
#define RET_IGNORE -2
#define RET_WAIT   -3

typedef uint64_t      UINT;
typedef int64_t       INT;
typedef double        FLOAT;

typedef struct __TrackEntry
{
	unsigned char mTrackType;

	union
	{
		struct
		{
			unsigned int  mInterlaced:1;
			unsigned char mAspectRatioType;
			unsigned int  mPixelWidth;
			unsigned int  mPixelHeight;
			double        mFrameRate;
		}Video;

		struct
		{
			unsigned int  mSamplingFreq;
			unsigned char mChannels;
			unsigned char mBitDepth;
		}Audio;
	}Media;
}TrackEntry;

typedef struct _Frame
{
  UINT  startTime;
  UINT  framePos;
  unsigned int frameLen;
  unsigned int bKeyFrame;
}Frame;

/*
typedef struct _FrameList
{
	Frame *pHeader;
	unsigned int nFrames;
	unsigned int nUnread;
}FrameList;
*/


typedef struct _IdxEntry
{
	UINT pos;
	UINT timecode;
}IdxEntry;

#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |    \
				 (((uint32_t)(ch4) & 0xFF00) << 8) |    \
				 (((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
				 (((uint32_t)(ch4) & 0xFF000000) >> 24))

typedef enum {
	SEEK_FORWARD,
	SEEK_BACKWARD,
} seekDirection;

typedef enum {
	AUDIO_PACKET,
	VIDEO_PACKET,
	OTHERS,
} BlockType;

typedef enum {
	Init,
	Recalc,
} calcType;



/* plugin instance object */

typedef struct _tagInputFLV_t {
	/*--------------------------------------------------------------
		FLV file format stuff
	---------------------------------------------------------------*/
	int64_t streamTotalBytes;

	int64_t	currReadPosition;
	int64_t tagStart;         //position of the first audio/video tag

	//add by yunfeng_han for sending H264 Video(need to send H264 start code 0x0000001 first)
	int64_t	lastReadPosition;
	char    *mAudioCodecPrivate;     //Private data only known to the codec.
	unsigned int mAudioCodecPrivateLen;  //length of private data

	char    *mVideoCodecPrivate;     //Private data only known to the codec.
	unsigned int mVideoCodecPrivateLen;  //length of private data


	//used only in H264 Video add by yunfeng_han
	//used in H264 Video 0 means to send startcode  1 means to send nalu unit
	unsigned int  bH264Sync;
	//used in H264 Video 0 means last frame last frame was sent completely otherwise means how much data not sent in last frame
	unsigned long  nH264FrameLeft;
	//used in H264 Video  means how much data has been sent in last frame
	unsigned long  nH264AlreadyRead;
	unsigned int   nalu_length_size;
	unsigned char *	pPayloadData;
	unsigned int	bFirstFrameUnit;//indacate if we have to send pts
	unsigned int    bNaluDataLeft;
	int64_t        	current_h264_pts;
	int				bissendingH264;


	//Video/Audio format
	TrackEntry AudioTrack;
	TrackEntry VideoTrack;
	char       bAudioExist;
	char       bVideoExist;


	int			bAACSync;

	UINT         TimecodeStart;	 // timecode of the first block
	unsigned int fileDuration;   //in miliseconds
//	UINT FileEnd;             // end position of file
/*
	//Store frame positions in a BlockGroup
	FrameList videoFrameList;
	FrameList audioFrameList;
*/

	Frame videoFrame;
	Frame audioFrame;

	Frame currentFrame;

	unsigned int  currTrackType;

	/*To Make seek quicker
	every 10 seconds will be a KeyFrame Index
	*/
	IdxEntry *keyFrameCache;
	unsigned int nKeyFrame;
	unsigned int nKeyFrameMax;
	int			bCreate_Index_Done;
	// for Gfile mode
	char     bGfile;

	//add by yunfeng_han
	unsigned long long CurrentTagPos; //if current read pos is EOF,CurrentTagPos will be the last tag pos

	//for A-B repeat
	UINT          abStart;
	UINT          abEnd;
	unsigned int  bABReset;

	bool		  bVP6Alpha;

	//temporary use
	unsigned int totalVideoLen;
	unsigned int totalAudioLen;
	FILE            *pAudioFile;
	FILE            *pVideoFile;

	PRESENTATION_POSITIONS	currPlaybackPosition;
	int64_t			currVideoSystemPTS;
	int64_t			prevVideoSystemPTS;
	int64_t			currRefClock;
    int64_t         FRStopRefClock;

	/*--------------------------------------------------------------
		Playback control
	---------------------------------------------------------------*/
	int				mEvent;
	int				mState;

	bool			bAudioSeek;
	bool			bVideoSeek;

	bool			bAudioTerm;
	bool			bVideoTerm;

	bool			bVideoEndSeq;
	bool			bStreamEnd;

	NAV_REPEAT_MODE  repeatMode;

	AUDIOFORMAT audioFormat;
	VIDEOFORMAT videoFormat;
	VIDEODIVX4	videoFormat2;
	int64_t		audio_preload_pts;

	NAVMEDIAINFO	media;

	int64_t			audioPTS;
	int64_t			videoPTS;

	int64_t			AACPTS;
	int				AACFrameLeft;
	int  			ReadFrameLen;
	unsigned char*  pAACPayloadData;

	int64_t			initVideoPTS;

	VIDEODVDDCICCI	DCICCI;

	/* for speed control, I-Frame Scan Only */
	int				m_speed;
	int				m_skip;

	/*--------------------------------------------------------------
		Inband command stuff
	---------------------------------------------------------------*/

    long 				statusFlag;
	long				cmdFlag;
	int64_t				decodeInitCMD;
	VIDEOMIXERBLEND 	blendCMD;
	VIDEODECODECOMMAND	decodeCMD;
	NAV_AVSYNC_MODE		AVSyncMode;

	/*--------------------------------------------------------------
		IO stuff
	---------------------------------------------------------------*/
	NAVIOINFO 			ioInfo;
	int64_t 			ioOffset;
	bool				bDirectIO;
	int					ioErrorCnt;

    int 				initialOffset;
	void*				fileHandler;

	unsigned char		*cmdBuffer;

	unsigned char   	*buffer;
	long long 			bufferStart;
	int					bufferSize;

	unsigned char		*ioBuffer;
	long long			ioBufferStart;
	int					ioBufferSize;

	IOPLUGIN* 			ioPlugin;
    osal_mutex_t*		ioMutex;
	osal_mutex_t        m_readMutex;
	long fatalErrCode;

	VIDEOMPEG4USABILITYINFO mp4info;

	double        mbitRate;

#if AP_CONFIG_WITH(APCFG_SMOOTH_TRICK_PLAY)
  	 bool 	bShowNextGOP;
	bool	bGOPSentIframe;
	int		nGOPSkipTotal;
	int 	nFramesGOPSent;
	int		nFramesNeedSkip;
	int 	nFramesSkip;
#endif

	NAVLOADFAILURE* m_pFailure;

	//add this flag to check if read_block has entered prohibitedChannelMask
	// if 0 means never enter ,means this file is small enough no need to create index to FF/FR/SEEK
	//if 1 need to create index to FF/FR Seek
	int bEnterprohibitedChannelMask;

	bool bLoadMediaDone;
	int eos_handling_count;
	bool bFlvLive;
}InputFLV_t;


unsigned int flvGetFilePos(InputFLV_t *pFLV);
inline int flvSkip(InputFLV_t *pFLV, unsigned int len);
inline char flvReadByte(InputFLV_t *pFLV, char *charOut);
char flvReadBytes(InputFLV_t *pFLV, void *buffer, unsigned int len);
inline char flvReadUINT(InputFLV_t *pFLV, unsigned long long *retInteger, unsigned int len);
inline char flvReadDouble(InputFLV_t *pFLV, double *retDouble);
inline char flvReadTimecode(InputFLV_t *pFLV, unsigned int *retInteger);
int flvParseLongString(InputFLV_t *pFLV, char *str);
int flvParseString(InputFLV_t *pFLV, char *str);
int flvParseDate(InputFLV_t *pFLV);
int flvParseVariableEnd(InputFLV_t *pFLV);
int flvParseVariable(InputFLV_t *pFLV, unsigned int tagTail);
int flvParseValue(InputFLV_t *pFLV, char *valName, unsigned int tagTail);
int flvParseObjectEnd(InputFLV_t *pFLV);
int flvParseObject(InputFLV_t *pFLV, unsigned int tagTail);
int flvParseAMF(InputFLV_t *pFLV, unsigned int tagTail);
int flvPrevIFrame(InputFLV_t *pFLV, int *retPTS);
int flvNextIFrame(InputFLV_t *pFLV, int *retPTS);
int flvSeekTo(InputFLV_t *pFLV, int mseconds, int *retPTS,int direction);
int flvParseTags(InputFLV_t *pFLV);
int flvParseAVformat(InputFLV_t *pFLV,bool* pbContinue);
int flvParseHeader(InputFLV_t *pFLV,bool* pbContinue);
int flvGetFileDuration(InputFLV_t *pFLV);


#endif //__FLV_INPUT_PLUGIN_INTERNAL_HEADER__


#ifndef __RTP_PARSE__
#define __RTP_PARSE__

#include <hresult.h>
#include <StreamClass/EType.h>
#include <Filters/PrivateInfo.h>
#include <Filters/NavigationFilter/NavDef.h>
#include <InbandAPI.h>
#include <Util.h>
#include <FlowEvent.h>
#include "InputRTP.h"
#include "utility.h"
#include "../../../../../common/IPC/include/rpc_common.h"


#define	STATUS_RTP_AT_PAUSE_STATE			(1L << 0)
#define	STATUS_RTP_AT_RUN_STATE				(1L << 1)
#define	STATUS_RTP_AT_STOP_STATE			(1L << 2)
#define	STATUS_RTP_USE_ODIRECT_OPEN			(1L << 3)
#define	STATUS_RTP_SHOULD_SEND_SEQ_END		(1L << 4)    
#define	STATUS_RTP_SHOULD_SEND_SEG_END		(1L << 5)    
#define	STATUS_RTP_SHOULD_SEND_STOP			(1L << 6)    
#define	STATUS_RTP_HAS_VIDEO_STREAM			(1L << 7)    
#define	STATUS_RTP_HAS_AUDIO_STREAM			(1L << 8)    
#define	STATUS_RTP_DEVICE_REMOVED			(1L << 9) 
#define	STATUS_RTP_INITIAL_PTS_SENT			(1L << 10) 


#define 	CMD_RTP_SET_AUDIO_FORMAT			(1L << 0)
#define	CMD_RTP_NEW_SEGMENT				(1L << 1)
#define	CMD_RTP_DECODE_INIT				(1L << 2)
#define	CMD_RTP_BLEND						(1L << 3)
#define	CMD_RTP_DECODE					(1L << 4)
#define	CMD_RTP_AUDIO_MEDIA_REMOVED		(1L << 5)
#define	CMD_RTP_PRESENTATION_TIME			(1L << 6)
#define	CMD_RTP_END_OF_SEQUENCE			(1L << 7)
#define	CMD_RTP_END_OF_SEGMENT			(1L << 8)
#define	CMD_RTP_STOP						(1L << 9)
#define 	CMD_RTP_POST_EVENT				(1L << 10)
#define 	CMD_RTP_SET_VIDEO_FORMAT			(1L << 11)
#define 	CMD_PENDING_AUDIO_AAC_TWEAK     	(1L << 12)
#define 	CMD_RTP_SEND_VIDEO_CONFIG			(1L << 13)
#define 	CMD_RTP_PENDING_ALL_FLUSH			(1L << 14)


#define RTP_BUFSIZE						(256*1024) // 64K bytes

#ifndef true 
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef IS_SET
	#define IS_SET(x,y)		((x) & (y))
	#define ClearFlag(x,y)	((x) &= ~(y))
	#define SetFlag(x,y)	((x) |= (y))
#endif

typedef struct _tagrtp_input_plugin {

    int64_t streamTotalBytes;

	unsigned char *buffer;
	long long bufferStart;
	int bufferSize;

	/*--------------------------------------------------------------
    	Medium information
	---------------------------------------------------------------*/
	AUDIOFORMAT	audioFormat;
	int64_t		baseAudioPTS;
    
    /*--------------------------------------------------------------
    	Playback information
	---------------------------------------------------------------*/
	PRESENTATION_POSITIONS	currPlaybackPosition;
    
	/*--------------------------------------------------------------
		Inband command stuff
	---------------------------------------------------------------*/
    long 				statusFlag; 
	long				cmdFlag;
	int64_t				decodeInitCMD;
	VIDEOMIXERBLEND 	blendCMD;
	VIDEODECODECOMMAND	decodeCMD;

	/*--------------------------------------------------------------
		event stuff
	---------------------------------------------------------------*/
	long                fatalErrCode;	
	unsigned int        eventId;
    NAV_REPEAT_MODE  	repeatMode;

	/*--------------------------------------------------------------
		IO stuff
	---------------------------------------------------------------*/
	NAVIOINFO 			ioInfo;
	bool				bDirectIO;

	int64_t				audioPTS;
	int64_t				videoPTS;
	int64_t				spicPTS;

	void*				fileHandler;
	IOPLUGIN* 			ioPlugin;
    osal_mutex_t*		ioMutex;

	FILE *	pAudioFile;
	FILE *	pVideoFile;
	FILE *	pSubFile;

	int					bAACSync;
	VIDEOFORMAT videoFormat;
	VIDEODIVX4	videoFormat2;

	char rtsppath[512];

} rtp_input_plugin_t;

#endif //__RTP_PARSE__

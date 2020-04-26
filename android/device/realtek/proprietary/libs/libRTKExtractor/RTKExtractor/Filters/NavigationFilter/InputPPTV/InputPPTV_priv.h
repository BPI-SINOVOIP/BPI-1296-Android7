#ifndef __INPUT_AV_PRIV_H__
#define __INPUT_AV_PRIV_H__

#include <hresult.h>
#include <StreamClass/EType.h>
#include <Filters/PrivateInfo.h>
#include <Filters/NavigationFilter/NavPlugins.h>
#include <InbandAPI.h>
#include <Util.h>
#include <FlowEvent.h>
#include "../../../../../common/IPC/include/rpc_common.h"


#define	STATUS_AT_PAUSE_STATE		(1L << 0)
#define	STATUS_AT_RUN_STATE			(1L << 1)
#define	STATUS_AT_STOP_STATE		(1L << 2)
#define	STATUS_USE_ODIRECT_OPEN		(1L << 3)
#define	STATUS_SHOULD_SEND_SEQ_END	(1L << 4)    
#define	STATUS_SHOULD_SEND_SEG_END	(1L << 5)    
#define	STATUS_SHOULD_SEND_STOP		(1L << 6)    
#define	STATUS_HAS_VIDEO_STREAM		(1L << 7)    
#define	STATUS_HAS_AUDIO_STREAM		(1L << 8)    
#define	STATUS_DEVICE_REMOVED		(1L << 9) 
#define	STATUS_INITIAL_PTS_SENT		(1L << 10) 
#define	STATUS_FLUSH_BUFFER         (1L << 15) 


#define CMD_SET_AUDIO_FORMAT		(1L << 0)
#define	CMD_NEW_SEGMENT				(1L << 1)
#define	CMD_DECODE_INIT				(1L << 2)
#define	CMD_BLEND					(1L << 3)
#define	CMD_DECODE					(1L << 4)
#define	CMD_AUDIO_MEDIA_REMOVED		(1L << 5)
#define	CMD_PRESENTATION_TIME		0//(1L << 6)
#define	CMD_END_OF_SEQUENCE			(1L << 7)
#define	CMD_END_OF_SEGMENT			(1L << 8)
#define	CMD_STOP					(1L << 9)
#define CMD_POST_EVENT				0//(1L << 10)
#define CMD_SET_VIDEO_FORMAT		(1L << 11)
#define CMD_PENDING_AUDIO_AAC_TWEAK (1L << 12)
#define CMD_SEND_VIDEO_CONFIG		(1L << 13)
#define CMD_MPEG4_DIVX4				(1L << 14)
#define	CMD_FLUSH_AUDIO         	(1L << 15) 


#define AV_BUFSIZE                  (640*1024) // 64K bytes
#define V_BUFSIZE                   (630*1024)  // 4K bytes
#define A_BUFSIZE                   (10*1024)  // 4K bytes

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

typedef struct {

    int64_t streamTotalBytes;

	unsigned char *buffer;
	long long PayloadSize;
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
	VIDEOFORMAT         videoFormat;
    
	bool				bIsDIVX4;

    /*--------------------------------------------------------------
    Video Buffer
	---------------------------------------------------------------*/
	unsigned char*      pVideoData;
	unsigned long       VideoDataSize;
	int64_t     		VideoDataPTS;
	
	/*--------------------------------------------------------------
    Audio Buffer
	---------------------------------------------------------------*/
	unsigned char*      pAudioData;
	unsigned long       AudioDataSize;
	int64_t     		AudioDataPTS;
	int64_t     		AudioPTS;
	int64_t             AudioPTSBase;
	unsigned long       AudioFrameCount;

	int					videoSubStreamFd;	// > 0 means video data comes from sub stream
	int					audioSubStreamFd;	// > 0 means audio data comes from sub stream	
} av_input_plugin_t;


//////////////////////////////////////////////////////////////////////////////////

#define INIT_NAV_EVENT(pbuff, event_id, event_data, event_size)        do { (pbuff)->type = NAVBUF_EVENT;		\
																			(pbuff)->event.eventId   = event_id;   \
                                                                           (pbuff)->event.eventSize = event_size; \
                                                                           (pbuff)->event.eventData = (unsigned char *)event_data;    \
                                                                       } while(0)

#define INIT_NAV_NONE(pbuff)                                           do {(pbuff)->type   = NAVBUF_NONE; } while(0)
                
#define INIT_NAV_WAIT(pbuff, ms)                                       do {(pbuff)->type   = NAVBUF_WAIT;   \
                                                                           (pbuff)->wait.waitTime = ms; \
                                                                       } while(0)

                    
#define INIT_NAV_DATA(pbuff, id, data, len, pts)                       do {(pbuff)->type                       = NAVBUF_DATA;   \
                                                                           (pbuff)->bDoNotInterruptWithUserCmd = 0; \
                                                                           (pbuff)->data.payloadData           = (unsigned char *)data; \
                                                                           (pbuff)->data.payloadSize           = len; \
                                                                           (pbuff)->data.infoId                = id;  \
                                                                           (pbuff)->data.channelIndex          = 0;          \
                                                                           (pbuff)->data.startAddress          = 0xFFFFFFFF;  \
                                                                           (pbuff)->data.pts                   = pts;  \
                                                                       } while(0)

//////////////////////////////////////////////////////////////////////////////////

#define av_warning(fmt, args...)    printf("[InputPPTV] Warning," fmt, ## args)        
#define av_info(fmt, args...)       printf("[InputPPTV] Info," fmt, ## args)        
#define av_dbg(fmt, args...)        printf("[InputPPTV] Dbg," fmt, ## args)    

#define PRINT_VIDEO_INFO                        
#define PRINT_AUDIO_INFO                        

#ifdef PRINT_VIDEO_INFO                        
#define video_info(fmt, args...)    av_info(fmt, ## args)            	
#else
#define video_info(fmt, args...)
#endif   

#ifdef PRINT_AUDIO_INFO                        
#define audio_info(fmt, args...)    av_info(fmt, ## args)            	
#else
#define audio_info(fmt, args...)
#endif   

#endif //__INPUTPRIV_H__

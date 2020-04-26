
#ifndef __C_MPEG_INPUT_PLUGIN_H__
#define __C_MPEG_INPUT_PLUGIN_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>	/* for strncasecmp */

#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/PrivateInfo.h>
#include <Filters/NavigationFilter/NavPlugins.h>
#include <Util.h>

///////////////////////////////////////////////////////////////

#define MPEG_BYTE_ALIGN_FOR_ODIRECT_OPEN	4096	//for devices with sector size 4k(refer to bug 18414)            
#define MPEG_BUFCNT       		2
#define MPEG_BUFSIZE       		(16*MPEG_BYTE_ALIGN_FOR_ODIRECT_OPEN)
#define MPEG_READ_UNITSIZE 		(2*MPEG_BYTE_ALIGN_FOR_ODIRECT_OPEN)

#define MPEG_MAX_AUDIO_STREAMS			8
#define MPEG_MAX_SPU_STREAMS			32

///////////////////////////////////////////////////////////////

#ifdef RDDEBUG
	#define MPEG_DEBUG(x) 		{ printf x; }
	#define MPEG_INFO(x)    	{ printf x; }
	#define MPEG_WARNING(x)		{ printf x; }
#else
	#define MPEG_DEBUG(x)
	#define MPEG_INFO(x)   
	#define MPEG_WARNING(x) 
#endif

///////////////////////////////////////////////////////////////

#define	STATUS_MPEG_AT_PAUSE_STATE				(1L << 0)
#define	STATUS_MPEG_AT_RUN_STATE				(1L << 1)
#define	STATUS_MPEG_AT_STOP_STATE				(1L << 2)
#define	STATUS_MPEG_AT_PARSER_MODE				(1L << 3)
#define	STATUS_MPEG_SHOULD_SEND_SEQ_END			(1L << 4)    
#define	STATUS_MPEG_SHOULD_SEND_SEG_END			(1L << 5)    
#define	STATUS_MPEG_SHOULD_SEND_STOP			(1L << 6)    
#define	STATUS_MPEG_WAIT_PRESENTATION_FINISH	(1L << 7)    
#define	STATUS_MPEG_USE_ODIRECT_OPEN			(1L << 8)  
#define	STATUS_MPEG_STILL						(1L << 9)
#define STATUS_MPEG_CANCEL_AB_REPEAT			(1L << 10)


#define	CMD_MPEG_FLUSH					(1L << 0)
#define	CMD_MPEG_AUDIO_TARGET			(1L << 1)
#define	CMD_MPEG_NEW_SEGMENT			(1L << 2)
#define	CMD_MPEG_DECODE_INIT			(1L << 3)
#define	CMD_MPEG_BLEND					(1L << 4)
#define	CMD_MPEG_DECODE					(1L << 5)
#define CMD_MPEG_ERROR_CONCEAL_LEVEL	(1L << 6)
#define CMD_MPEG_RESET_SPEED			(1L << 7)
#define	CMD_MPEG_VOBU_INFO				(1L << 8)
#define	CMD_MPEG_RESET_REPEAT_MODE		(1L << 9)
#define	CMD_MPEG_END_OF_SEQUENCE		(1L << 10)
#define	CMD_MPEG_END_OF_SEGMENT			(1L << 11)
#define	CMD_MPEG_STOP					(1L << 12)
#define CMD_MPEG_VIDEO_TARGET           (1L << 13)
#define CMD_MPEG_FLUSH_AUDIO			(1L << 14)
#define CMD_MPEG_POST_EVENT				(1L << 15)
#define CMD_MPEG_DCI_CCI				(1L << 16)
#define CMD_MPEG_UPDATE_STC_OFFSET      (1L << 17)
#define CMD_MPEG_BITSTREAM_ERROR	    (1L << 18)
#define CMD_MPEG_UNSUPPORTED_FORMAT	    (1L << 19)
#define CMD_MPEG_SPU_TARGET				(1L << 20)
#define CMD_MPEG_SPU_CONFIG				(1L << 21)
#define CMD_MPEG_SPU_ON_OFF				(1L << 22)
#define CMD_MPEG_FLUSH_SPU				(1L << 23)
#define CMD_MPEG_APS_STATUS_CHANGED		(1L << 24)

//////////////////////////////////////////////////////////////////////////


typedef struct _tagmpeg_input_plugin {

    int					readUnitSize;
    int 				initialOffset;
    int 				bIsRIFF_CDXA;
  	int					totalTime;
    int					speed;	
  	int					skip;
  	int					currContext;
  	int					concealmentLevel[2];	// [0] for normal speed, [1] for trick play
  	int					deltaPTS;
  	int					amountToRead;
  	int					amountToSkip;
  	int					readBytes;
  	int					waitCount;
  	int					currAspectRatio;
  	int					bytePerSec;
  	int					demuxErrorCount;
  	int					APS;
  	
  	int					seqPlayStartSec;
  	int64_t				seqPlayEndPos;
  	
#ifdef MPEG_USE_REF_CLOCK     
    int64_t         	baseTickTimer;      // reset when stop  
    int64_t         	lastLocalPTS;       // last updated local PTS
#endif    
	int64_t				lastPresentedPTS;
	int64_t				tickSincePresented;
	int64_t         	lastReferenceClock;
   // int64_t 			evaluatedLocalPTS;
	int64_t 			lastDemuxPTS;
    int64_t 			fileBeginPTS;           // when positive, the PTS based ELTM method is used
    int64_t				streamTotalBytes;
    int64_t				currPosition;		// the current playback position, not reading position
    int64_t				currReadPosition;	// the current reading position
    int64_t             stcOffset;
    int64_t             lastOffsetPTS;
    
    unsigned int		muxRate;
    NAV_REPEAT_MODE 	repeatMode;
#ifndef  NOTFIXBUG15246
    ENUM_MEDIA_TYPE 	videoType;
    int					video_stream_id;
#else    
	bool				use_mpeg_4;
	int                 mpeg_4_video_stream_id;
#endif	
	bool				bMPEG1;
	struct
	{
		int				stream_id;
		ENUM_MEDIA_TYPE type;
		int				Channels;
		int				SampleRate;
		int				BitsPerSample;
		int				BitRate;
	} audio_streams[MPEG_MAX_AUDIO_STREAMS];
	
	int					num_of_audio_streams;
	int					curr_audio_stream_index;
    
    int 				spu_stream_ids[MPEG_MAX_SPU_STREAMS];
    int					num_of_spu_streams;
    int					curr_spu_stream_index;
    
    int                 bufferingSecondsLeftPrepare;
    int                 bufferingSecondsTotal;

    long 				statusFlag; 
	long				cmdFlag;
	int64_t				decodeInitCMD;
	VIDEOMIXERBLEND 	blendCMD;
	VIDEODECODECOMMAND	decodeCMD;
	VIDEODVDVOBUINFO    vobuInfo;
	DEMUXTARGET    		spuTarget;
	DEMUXTARGET    		audioTarget;
	DEMUXTARGET			videoTarget;
	VIDEODVDDCICCI		dciCCI;
	SPUCONFIG			spuConfig;
	int					spuOnOff;
	
    unsigned int        eventId;
    unsigned char*      eventData;
    unsigned int        eventSize;

	unsigned char		bIOSeekable;
	
#ifdef MPEG_USE_AIO	
	int                 aioIndex;		
	unsigned char*   	buffer_pli;
#endif
	
	char*				mediaPath;
	unsigned char*   	buffer;
	void*				fileHandler;
	IOPLUGIN* 			ioPlugin;
    osal_mutex_t*		ioMutex;
    long                fatalErrCode;
    
    //for external subtitle (bug 31318)
    int64_t             firstPTS;
    int64_t             elapsedPTS;
    
} mpeg_input_plugin_t;


void check_if_mpeg1(mpeg_input_plugin_t *this, NAVIOINFO* pIoInfo, int read_bytes, NAVBUF* pEvent);
bool estimate_bit_rate(mpeg_input_plugin_t *this, NAVIOINFO* pIoInfo, int read_bytes, bool* pbContinue);

#ifdef ENABLE_SPU_IN_MPEG_PLUGIN
void mpeg_plugin_check_insert_spu_stream(void* pInstance, int id);
#endif

void parse_program_stream_map(mpeg_input_plugin_t* this);

#endif //__C_MPEG_INPUT_PLUGIN_H__

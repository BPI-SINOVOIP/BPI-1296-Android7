#ifndef __WAVE_PARSE__
#define __WAVE_PARSE__

#include <hresult.h>
#include <StreamClass/EType.h>
#include <Filters/PrivateInfo.h>
#include <InbandAPI.h>
#include <Util.h>
#include <FlowEvent.h>
#include "InputWAVE.h"
#include "wavriff.h"
#include "mmreg.h"
#include "../../../../../common/IPC/include/rpc_common.h"
#include "ksmedia.h"

#define	STATUS_WAVE_AT_PAUSE_STATE				(1L << 0)
#define	STATUS_WAVE_AT_RUN_STATE				(1L << 1)
#define	STATUS_WAVE_AT_STOP_STATE				(1L << 2)
#define	STATUS_WAVE_USE_ODIRECT_OPEN			(1L << 3)
#define	STATUS_WAVE_SHOULD_SEND_SEQ_END			(1L << 4)    
#define	STATUS_WAVE_SHOULD_SEND_SEG_END			(1L << 5)    
#define	STATUS_WAVE_SHOULD_SEND_STOP			(1L << 6)    
#define	STATUS_WAVE_HAS_VIDEO_STREAM			(1L << 7)    
#define	STATUS_WAVE_HAS_AUDIO_STREAM			(1L << 8)    
#define	STATUS_WAVE_DEVICE_REMOVED				(1L << 9) 
#define	STATUS_WAVE_INITIAL_PTS_SENT			(1L << 10) 
#define	STATUS_WAVE_PRESENTATION_FINISHED		(1L << 11)
#define	STATUS_WAVE_WAIT_PRESENTATION_FINISH	(1L << 12)
#define STATUS_WAVE_CANCEL_AB_REPEAT			(1L << 13)

#define CMD_WAVE_SET_AUDIO_FORMAT		(1L << 0)
#define	CMD_WAVE_NEW_SEGMENT			(1L << 1)
#define	CMD_WAVE_DECODE_INIT			(1L << 2)
#define	CMD_WAVE_BLEND					(1L << 3)
#define	CMD_WAVE_DECODE					(1L << 4)
#define	CMD_WAVE_AUDIO_MEDIA_REMOVED	(1L << 5)
#define	CMD_WAVE_PRESENTATION_TIME		(1L << 6)
#define	CMD_WAVE_END_OF_SEQUENCE		(1L << 7)
#define	CMD_WAVE_END_OF_SEGMENT			(1L << 8)
#define	CMD_WAVE_STOP					(1L << 9)
#define CMD_WAVE_POST_EVENT				(1L << 10)
#define CMD_WAVE_RESET_SPEED			(1L << 11)
#define CMD_WAVE_FLUSH					(1L << 12)

#define WAVE_BUFSIZE        (16*1024) // 16K bytes
#define WAVE_IO_SIZE		(32*1024) // 32K bytes
#define WAVE_IO_ALIGN_MASK	0xfffffffffffff000LL
#define WAVE_IO_UNIT		(~WAVE_IO_ALIGN_MASK)

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

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

typedef struct _tagwave_input_plugin {

    int64_t streamTotalBytes;

	unsigned char *buffer;
	long long bufferStart;
	int bufferSize;

	/*--------------------------------------------------------------
    	Medium information
	---------------------------------------------------------------*/
	WAVEFORMATEX waveformat;
	AUDIOFORMAT	audioFormat;
	int64_t		baseAudioPTS;
	int64_t		lastPresentedPTS;
    WAVEFORMATEXTENSIBLE extension;
    
    /*--------------------------------------------------------------
    	Playback information
	---------------------------------------------------------------*/
	PRESENTATION_POSITIONS	currPlaybackPosition;
	int						timeOutCounter;
    
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
	int64_t 			ioOffset;
	bool				bDirectIO;
	bool				bDTSCD;

	int64_t				currFilePos;
	int64_t				fileSize;
	int64_t				dataStart;
	int64_t				dataLength;
	int64_t             dataEnd;
	
	unsigned char		*ioBuffer;
	unsigned char		*ioBufferNonCached;
	long long			ioBufferStart;
	int					ioBufferSize;

	void*				fileHandler;
	IOPLUGIN* 			ioPlugin;
    osal_mutex_t*		ioMutex;

	bool				bBigEndianDTS;
	
	int                 segPlayAinSec;
	int64_t             segPlayBinPos;
} wave_input_plugin_t;

#endif //__WAVE_PARSE__

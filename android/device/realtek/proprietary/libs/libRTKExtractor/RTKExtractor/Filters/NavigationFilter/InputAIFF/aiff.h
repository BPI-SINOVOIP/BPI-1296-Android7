#ifndef __AIFF_PARSE__
#define __AIFF_PARSE__

#include <hresult.h>
#include <StreamClass/EType.h>
#include <Filters/PrivateInfo.h>
#include <InbandAPI.h>
#include <Util.h>
#include <FlowEvent.h>
#include "InputAIFF.h"
//#include "aiffriff.h"
//#include "mmreg.h"
#include "../../../../../common/IPC/include/rpc_common.h"


#define	STATUS_AIFF_AT_PAUSE_STATE				(1L << 0)
#define	STATUS_AIFF_AT_RUN_STATE				(1L << 1)
#define	STATUS_AIFF_AT_STOP_STATE				(1L << 2)
#define	STATUS_AIFF_USE_ODIRECT_OPEN			(1L << 3)
#define	STATUS_AIFF_SHOULD_SEND_SEQ_END			(1L << 4)    
#define	STATUS_AIFF_SHOULD_SEND_SEG_END			(1L << 5)    
#define	STATUS_AIFF_SHOULD_SEND_STOP			(1L << 6)    
#define	STATUS_AIFF_HAS_VIDEO_STREAM			(1L << 7)    
#define	STATUS_AIFF_HAS_AUDIO_STREAM			(1L << 8)    
#define	STATUS_AIFF_DEVICE_REMOVED				(1L << 9) 
#define	STATUS_AIFF_INITIAL_PTS_SENT			(1L << 10) 
#define STATUS_AIFF_SSND_REACHED				(1L << 11) 
#define STATUS_AIFF_FILETYPE_AIFC				(1L << 12) 

#define CMD_AIFF_SET_AUDIO_FORMAT		(1L << 0)
#define	CMD_AIFF_NEW_SEGMENT			(1L << 1)
#define	CMD_AIFF_DECODE_INIT			(1L << 2)
#define	CMD_AIFF_BLEND					(1L << 3)
#define	CMD_AIFF_DECODE					(1L << 4)
#define	CMD_AIFF_AUDIO_MEDIA_REMOVED	(1L << 5)
#define	CMD_AIFF_PRESENTATION_TIME		(1L << 6)
#define	CMD_AIFF_END_OF_SEQUENCE		(1L << 7)
#define	CMD_AIFF_END_OF_SEGMENT			(1L << 8)
#define	CMD_AIFF_STOP					(1L << 9)
#define CMD_AIFF_POST_EVENT				(1L << 10)

#define AIFF_BUFSIZE        (16*1024) // 16K bytes
#define AIFF_IO_SIZE		(32*1024) // 32K bytes
#define AIFF_IO_ALIGN_MASK	0xfffffffffffff000LL
#define AIFF_IO_UNIT		(~AIFF_IO_ALIGN_MASK)

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))



/* === Endian-related stuff === */
#define ARRANGE_ENDIAN_16(dat) ( (((dat) & 0xff00 ) >> 8 ) | (((dat) & 0x00ff ) << 8 ) )
#define ARRANGE_ENDIAN_32(dat) ( (((dat) & 0xff000000 ) >> 24 ) | (((dat) & 0x00ff0000 ) >> 8 ) | (((dat) & 0x0000ff00 ) << 8 ) | (((dat) & 0x000000ff ) << 24 ) )

/*#ifdef WORDS_BIGENDIAN
# define ARRANGE_BE16(dat) (dat)
# define ARRANGE_BE32(dat) (dat)
# define ARRANGE_LE16(dat) ARRANGE_ENDIAN_16(dat)
# define ARRANGE_LE32(dat) ARRANGE_ENDIAN_32(dat)
#else
*/
# define ARRANGE_BE16(dat) ARRANGE_ENDIAN_16(dat)
# define ARRANGE_BE32(dat) ARRANGE_ENDIAN_32(dat)
# define ARRANGE_LE16(dat) (dat)
# define ARRANGE_LE32(dat) (dat)
//#endif /* WORDS_BIGENDIAN */




 /* OSTypes */
#define AIFF_NAME		0x4e414d45
#define AIFF_AUTH		0x41555448
#define AIFF_COPY		0x28632920
#define AIFF_ANNO		0x414e4e4f
#define AIFF_FORM 		0x464f524d
#define AIFF_AIFF 		0x41494646
#define AIFF_AIFC 		0x41494643
#define AIFF_FVER 		0x46564552
#define AIFF_COMM 		0x434f4d4d
#define AIFF_SSND 		0x53534e44
#define AIFF_MARK 		0x4d41524b
#define AIFF_INST 		0x494e5354
#define AIFF_COMT 		0x434f4d54


/* == Supported formats == */

/* File formats */
#define AIFF_TYPE_IFF  ARRANGE_BE32(0x464F524D)
#define AIFF_TYPE_AIFF ARRANGE_BE32(0x41494646)
#define AIFF_TYPE_AIFC ARRANGE_BE32(0x41494643)

/* Audio encoding formats */
/*
#define AUDIO_FORMAT_LPCM  ARRANGE_BE32(0x4E4F4E45)
#define AUDIO_FORMAT_lpcm  ARRANGE_BE32(0x6C70636D)
#define AUDIO_FORMAT_twos  ARRANGE_BE32(0x74776F73)
#define AUDIO_FORMAT_sowt  ARRANGE_LE32(0x74776F73)
#define AUDIO_FORMAT_ULAW  ARRANGE_BE32(0x554C4157)
#define AUDIO_FORMAT_ulaw  ARRANGE_BE32(0x756C6177)
#define AUDIO_FORMAT_ALAW  ARRANGE_BE32(0x414C4157)
#define AUDIO_FORMAT_alaw  ARRANGE_BE32(0x616C6177)
#define AUDIO_FORMAT_UNKNOWN 0xFFFFFFFF
*/

#define AUDIO_FORMAT_LPCM  ARRANGE_BE32(0x4E4F4E45) //'NONE'
#define AUDIO_FORMAT_ACE2  ARRANGE_BE32(0x41434532) //'ACE2'
#define AUDIO_FORMAT_ACE8  ARRANGE_BE32(0x41434538)	//'ACE8'
#define AUDIO_FORMAT_MAC3  ARRANGE_BE32(0x4d414333) //'MAC3'
#define AUDIO_FORMAT_MAC6  ARRANGE_BE32(0x4d414336) //'MAC6'
#define AUDIO_FORMAT_FL32  ARRANGE_BE32(0x464c3332)	//PCM 32-bits floating point
#define AUDIO_FORMAT_fl32  ARRANGE_BE32(0x666c3332)
#define AUDIO_FORMAT_FL64  ARRANGE_BE32(0x464c3634)	//PCM 64-bits floating point
#define AUDIO_FORMAT_fl64  ARRANGE_BE32(0x666c3634)
#define AUDIO_FORMAT_ima4  ARRANGE_BE32(0x696D6134) //IMA 4:1 (one kind of ADPCM)
#define AUDIO_FORMAT_UNKNOWN 0xFFFFFFFF


/* Play modes */
#define kModeNoLooping			0
#define kModeForwardLooping		1
#define kModeForwardBackwardLooping	2

/* == Typedefs == */
typedef uint32_t IFFType ;
typedef uint8_t iext ;
typedef uint16_t MarkerId ;


struct s_Loop
{
	int16_t playMode ;
	uint64_t beginLoop ;
	uint64_t endLoop ;
} ;

struct s_Instrument
{
	int8_t baseNote ;
	int8_t detune ;
	int8_t lowNote ;
	int8_t highNote ;
	int8_t lowVelocity ;
	int8_t highVelocity ;
	int16_t gain ;
	struct s_Loop sustainLoop ;
	struct s_Loop releaseLoop ;
} ;
typedef struct s_Instrument Instrument ;


typedef struct s_IFFHeader
{
	IFFType hid ;
	uint32_t len ;
	IFFType fid ;
} IFFHeader ;
  

typedef  struct s_IFFChunk
{
	IFFType id ;
	uint32_t len ;
}  IFFChunk ;


/*
 * WARNING: this struct is not alignment-safe!
 */
 struct s_AIFFCommon
{	
	uint16_t numChannels ;
	uint32_t numSampleFrames ;
	uint16_t sampleSize ;
	uint8_t  sampleRate[10] ; /* Motorola 80-bit extended */
}__attribute__ ((packed));

typedef struct s_AIFFCommon CommonChunk ;

typedef struct s_AIFFSound
{
	uint32_t offset ;
	uint32_t blockSize ;
} SoundChunk ;


struct s_Marker
{
	MarkerId id ;
	uint32_t position ;
	uint8_t markerNameLen ;
	char markerName ;
} ;
typedef struct s_Marker Marker ;

struct s_AIFFMarker
{
	uint16_t numMarkers ;
	char markers ;
} ;
typedef struct s_AIFFMarker MarkerChunk ;

struct s_AIFFLoop
{
	int16_t playMode ;
	MarkerId beginLoop ;
	MarkerId endLoop ;
	uint16_t garbage ; /* not read (size=6 bytes) */
} ;
typedef struct s_AIFFLoop AIFFLoop ;


struct s_Comment
{
	uint32_t timeStamp ;
	MarkerId marker ;
	uint16_t count ;
	char text ;
} ;
typedef struct s_Comment Comment ;

struct s_AIFFComment
{
	uint16_t numComments ;
	char comments ;
} ;
typedef struct s_AIFFComment CommentChunk ;

typedef struct _AIFFFORMATEX {

  	uint16_t numberOfChannels ;
	uint32_t numSampleFrames ;
	uint16_t bitsPerSample ;
	double   samplingRate;
} AIFFFORMATEX, *PAIFFFORMATE;

///////////////////////////////////////////////////////////////////////////////


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

typedef struct _tagaiff_input_plugin {

    int64_t streamTotalBytes;

	unsigned char *buffer;
	long long bufferStart;
	int bufferSize;

	/*--------------------------------------------------------------
    	Medium information
	---------------------------------------------------------------*/
	//WAVEFORMATEX waveformat;

	
	AIFFFORMATEX aiffformat; 
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
	int64_t 			ioOffset;
	bool				bDirectIO;
	bool				bDTSCD;

	int64_t				currFilePos;
	int64_t				fileSize;
	int64_t				dataStart;
	int64_t				dataSize;

	unsigned char		*ioBuffer;
	unsigned char		*ioBufferNonCached;
	long long			ioBufferStart;
	int					ioBufferSize;

	void*				fileHandler;
	IOPLUGIN* 			ioPlugin;
    osal_mutex_t*		ioMutex;

} aiff_input_plugin_t;

#endif //__aiff_PARSE__

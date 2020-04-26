#include <Filters/NavigationFilter/NavPlugins.h>

#define VORBIS_BYTE_ALIGN_FOR_ODIRECT_OPEN   4096
#define VORBIS_PRESENTATION_TIME_OUT	        15

#define VORBIS_BUFSIZE                       (64 * 1024) // 64K bytes

#define	STATUS_VORBIS_AT_PAUSE_STATE           (1L << 0)
#define	STATUS_VORBIS_AT_RUN_STATE             (1L << 1)
#define	STATUS_VORBIS_AT_STOP_STATE            (1L << 2)
#define	STATUS_VORBIS_SHOULD_SEND_SEG_END      (1L << 3)    
#define	STATUS_VORBIS_SHOULD_SEND_STOP         (1L << 4)   
#define	STATUS_VORBIS_USE_ODIRECT_OPEN         (1L << 5)  
#define	STATUS_VORBIS_SHOULD_SET_INITIAL_PTS   (1L << 6)    
#define	STATUS_VORBIS_WAIT_PRESENTATION_FINISH (1L << 7)
#define	STATUS_VORBIS_PRESENTATION_FINISHED		(1L << 8)  
#define STATUS_VORBIS_SHOULD_SEND_REMAIN_DATA		(1L << 9)
#define STATUS_VORBIS_SHOULD_SEND_VORBIS_HEADER		(1L << 10)
#define STATUS_VORBIS_SHOULD_SEND_INITIAL_PTS		(1L << 11)

#define CMD_VORBIS_SET_AUDIO_FORMAT          (1L << 0)
#define	CMD_VORBIS_NEW_SEGMENT               (1L << 1)
#define	CMD_VORBIS_DECODE_INIT               (1L << 2)
#define	CMD_VORBIS_BLEND                     (1L << 3)
#define	CMD_VORBIS_DECODE                    (1L << 4)
#define CMD_VORBIS_POST_EVENT				 (1L << 5)
#define	CMD_VORBIS_PRESENTATION_TIME         (1L << 6)
#define	CMD_VORBIS_END_OF_SEGMENT            (1L << 7)
#define	CMD_VORBIS_STOP                      (1L << 8)
#define	CMD_VORBIS_RESET_REPEAT_MODE         (1L << 9)
#define	CMD_VORBIS_FLUSH	              			(1L << 10)    
#define	CMD_VORBIS_RESET_SPEED		           	(1L << 11) 
#define	CMD_VORBIS_SET_AUDIO_TARGET	(1L <<13)


#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |    \
		(((uint32_t)(ch4) & 0xFF00) << 8) |    \
		(((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
		(((uint32_t)(ch4) & 0xFF000000) >> 24))

typedef enum{
	VORBIS_READ_DATA,
	VORBIS_READ_END,
	VORBIS_READ_COMMAND,
	VORBIS_READ_AGAIN,
	VORBIS_READ_FAIL,
	VORBIS_READ_ERROR,
} vorbis_read_status_t;

typedef enum{
	VORBIS_PLAY_NORMAL
} vorbis_play_mode; 
typedef struct _vorbisstream
{
	unsigned long  byte_used;
	unsigned long  byte_left;
	unsigned char* bufferNonCached;
	unsigned char* data_pool;
} vorbisstream_t;

typedef struct _oggvorbheader
{
	int pktsize[3];
	int count;
	int totalsize;
	unsigned char* buf;

	int isFilled;
} oggvorbheader_t;

typedef struct _vorbisInfo
{
	ENUM_MEDIA_TYPE mediaType;
	unsigned int version;
	unsigned int channels;
	unsigned int samplerate;
	unsigned int bitrate;
	unsigned int bitsPerSample;
	int bSelected;

	oggvorbheader_t	vorbHeader;
} vorbis_Info;

typedef struct _oggpkt
{
	int bop;//begin of packet
	int eop;//end of packet
	bool isSend;//if has been sent
} oggpkt_t;

typedef struct _oggheader
{
	uint8_t HeaderType;
	uint64_t GranulePosition;
	uint32_t BitstreamSerialNumber;
	uint32_t PageSequenceNumber;
	uint8_t PageSegments;

	oggpkt_t packetTable[255];
	int packetCount;
} oggheader_t;

typedef struct _ogglangcode
{
	char* code;
	char* name;
	int len;
} ogglangcode_t;

typedef struct _tagogg_input_plugin
{

	/* Playback information */
	int					reservedPayloadSize;    // the len of the remaining payload not sent in the previous run
	int64_t				presentationTime;
	int					audioContext;           // for display time
	long long			streamTotalBytes;       // for PLayAtTime
	unsigned long		prevPTS;
	int					timeOutCounter;
	int64_t				lastPresentedPTS;	
	int64_t				audioSystemPTS;
	NAV_REPEAT_MODE		repeatMode;
    NAV_PLAYBACK_MODE	playbackMode;

	ENUM_MEDIA_TYPE		audioType;
	/* Inband command stuff */
	long				statusFlag;
	long				cmdFlag;
	int64_t				decodeInitCMD;
	VIDEOMIXERBLEND		blendCMD;
	VIDEODECODECOMMAND	decodeCMD;
	AUDIOFORMAT			audioFormat;


	/* IO stuff */
	unsigned char*     buffer;
	unsigned char*     bufferNonCached;
	unsigned char*     remainbuffer;
	unsigned char*     remainbufferNonCached;
	int                remainpayloadSize;

	void*              fileHandler;
	IOPLUGIN*          ioPlugin;
	osal_mutex_t*      ioMutex;
	unsigned int       eventId;
	unsigned char*     eventData;
	unsigned int       eventSize;         
	long               fatalErrCode;
	vorbisstream_t        oggstream;

	/* VORBIS file info */
	unsigned long      fileEndAddress;
	int                version;
	int                packetSize;
	int                packetNumber;
	int                streamSerialNumber;
	bool               fileEnd;

	vorbis_Info		vorbisinfo;
	/* The granule position (little endian) of an ogg page containing
	 * audio packets is the absolute pcm position in the stream
	 * after the last packet ending on this page is processed.
	 */
	unsigned int       granulePos_high;
	unsigned int       lastGranulePos_high;
	int                nonsyncPayloadSize;
	bool               resync_flg;
	bool               readErrorFlg;

	int		audioStreamId;
	int64_t		timeUnit;//timeUnit == 90000/fps

	uint32_t	biCompression;
	int		duration_in_seconds;
	NAVIOINFO	ioInfo;
	int		m_speed;
	PRESENTATION_POSITIONS  currPlaybackPosition;
	vorbis_play_mode   playMode;

	oggheader_t	pageHeader;
	int		needContinue;

	int		isVorbHeaderSaved;
	int64_t		currRefClock;
	NAVMEDIAINFO*	pMediaInfo;
	bool		bDirectIO;


	AUDIOVORBIS        audioVorbis;
	int                payloadSize;
	int                vorbisPktCnt;
} Sndaogg_input_plugin_t;

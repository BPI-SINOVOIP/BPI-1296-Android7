#include <Filters/NavigationFilter/NavPlugins.h>


#define OGG_BYTE_ALIGN_FOR_ODIRECT_OPEN   4096
#define OGG_PRESENTATION_TIME_OUT	        15

#define OGG_BUFSIZE                       (64 * 1024) // 64K bytes

#define	STATUS_OGG_AT_PAUSE_STATE           (1L << 0)
#define	STATUS_OGG_AT_RUN_STATE             (1L << 1)
#define	STATUS_OGG_AT_STOP_STATE            (1L << 2)
#define	STATUS_OGG_AT_PARSER_MODE           (1L << 3)
#define	STATUS_OGG_SHOULD_SEND_SEQ_END      (1L << 4)    
#define	STATUS_OGG_SHOULD_SEND_SEG_END      (1L << 5)    
#define	STATUS_OGG_SHOULD_SEND_STOP         (1L << 6)    
#define	STATUS_OGG_HAS_VIDEO_STREAM         (1L << 7)    
#define	STATUS_OGG_HAS_AUDIO_STREAM         (1L << 8)    
#define	STATUS_OGG_USE_ODIRECT_OPEN         (1L << 9)  
#define	STATUS_OGG_SHOULD_SET_INITIAL_PTS   (1L << 10)    
#define	STATUS_OGG_PRESENTATION_FINISHED		(1L << 11)  
//added by baili_feng
#define STATUS_OGG_AUDIO_SWITCH				(1L << 12)
#define STATUS_OGG_SPIC_SWITCH				(1L << 13)
#define STATUS_OGG_AUDIO_SYNC				(1L << 14)
#define STATUS_OGG_VIDEO_SYNC				(1L << 15)
#define STATUS_OGG_SPIC_SYNC				(1L << 16)
#define STATUS_OGG_SHOULD_AUDIO_RESYNC			(1L << 17)
#define STATUS_OGG_SHOULD_SEND_REMAIN_DATA		(1L << 18)
#define STATUS_OGG_SHOULD_SEND_AUDIO_HEADER		(1L << 19)
#define STATUS_OGG_SHOULD_SEND_INITIAL_PTS		(1L << 20)

#define CMD_OGG_SET_AUDIO_FORMAT          (1L << 0)
#define	CMD_OGG_NEW_SEGMENT               (1L << 1)
#define	CMD_OGG_DECODE_INIT               (1L << 2)
#define	CMD_OGG_BLEND                     (1L << 3)
#define	CMD_OGG_DECODE                    (1L << 4)
#define	CMD_OGG_AUDIO_MEDIA_PROPERTY      (1L << 5)
#define	CMD_OGG_PRESENTATION_TIME         (1L << 6)
#define	CMD_OGG_END_OF_SEQUENCE           (1L << 7)
#define	CMD_OGG_END_OF_SEGMENT            (1L << 8)
#define	CMD_OGG_STOP                      (1L << 9)
#define	CMD_OGG_SET_FORMAT_OGGHE          (1L << 10)
#define	CMD_OGG_RESET_REPEAT_MODE         (1L << 11)
#define CMD_OGG_SEND_FRAME_IDX            (1L << 12)
#define	CMD_OGG_POST_EVENT                (1L << 13)
#define	CMD_OGG_RESYNC_TO_HEADER          (1L << 14)
#define	CMD_OGG_FLUSH	              			(1L << 15)    
#define	CMD_OGG_RESET_SPEED		           	(1L << 16) 
#define CMD_OGG_SET_VIDEO_FORMAT			(1L << 17)
#define CMD_OGG_SEND_SUBTITLE				(1L << 18)
#define	CMD_OGG_AUDIO_FLUSH	              		(1L << 19)    
#define CMD_OGG_SPIC_FLUSH				(1L << 20)
#define CMD_OGG_AVSYNC_CHANGE				(1L << 21)
#define CMD_OGG_VIDEO_USABILITY_INFO			(1L << 22)


//added by baili_feng
#define FWD_IONLY_SPEED 256
#define RWD_IONLY_SPEED 0

//added by baili_feng
#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |    \
		(((uint32_t)(ch4) & 0xFF00) << 8) |    \
		(((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
		(((uint32_t)(ch4) & 0xFF000000) >> 24))
#define OGG_SUBTEXT_SIZE	1024

typedef enum{
	OGG_READ_DATA,
	OGG_READ_END,
	OGG_READ_COMMAND,
	OGG_READ_AGAIN,
	OGG_READ_FAIL,
	OGG_READ_ERROR,
} ogg_read_status_t;

//added by baili_feng
typedef enum
{
	OGG_TRACK_VIDEO,
	OGG_TRACK_AUDIO,
	OGG_TRACK_SUBPIC,
	OGG_TRACK_SUBTXT,
	OGG_TRACK_OTHERS,
} ogg_track_type;

typedef enum
{
	OGG_PLAY_NORMAL,
	OGG_PLAY_PLAYATTIME,
	OGG_PLAY_FWD_I,
	OGG_PLAY_RWD_I,
	OGG_PLAY_SMOOTH,
} ogg_play_mode;

typedef struct _oggstream
{
	unsigned long  byte_used;
	unsigned long  byte_left;
	unsigned char* bufferNonCached;
	unsigned char* data_pool;
} oggstream_t;

//added by baili_feng
typedef struct _RIFFCHUNK
{
	uint32_t fcc;
	uint32_t cb;
} RIFFCHUNK;

typedef struct _RESUME_STATE
{
	int64_t offset;
	int	videoTrack;
	int	audioTrack;
	int	subtxtTrack;
} RESUME_STATE;
typedef struct _oggvorbheader
{
	int pktsize[3];
	uint64_t granule[3];
	int count;
	int off;
	unsigned char* buf;

	int isFilled;
} oggvorbheader_t;

typedef struct _oggtrack
{
	int trackId;
	int trackType;//ogg_track_type
	unsigned int fourCC;
	ENUM_MEDIA_TYPE mediaType;
	unsigned int langCode;
	unsigned int rate;
	unsigned int channels;
	unsigned int bitsPerSample;
	int bSelected;

	oggvorbheader_t	vorbHeader;
} oggtrack_t;

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

typedef struct _oggframe
{
	uint64_t pos;//position in file
	uint16_t off;//offset in page
} oggframe_t;

typedef struct _ogglangcode
{
	char* code;
	char* name;
	int len;
} ogglangcode_t;

typedef struct _oggchapter
{
	unsigned int sec;
} oggchapter_t;

typedef struct _tagogg_input_plugin
{
	/* Medium information */
	// file_properties_object_t    FileProperties;	
	// payload_parsing_info_t      payloadInfo;
	// index_object_t*             idxObject;


	/* Playback information */
	int                reservedPayloadSize;    // the len of the remaining payload not sent in the previous run
	long long	         dataObjectStart;        // start address of the Data Object
	long long	         dataObjectEnd;          // end address of the Data Object*/
	int64_t 	         presentationTime;
	int                audioContext;           // for display time
	uint64_t          currentReadPosition;    // for PlayAtTime
	//long long          streamTotalBytes;       // for PLayAtTime
	unsigned long      prevPTS;
	int					       timeOutCounter;
	int64_t				     lastPresentedPTS;	
	NAV_REPEAT_MODE    repeatMode;
	long long          SendFrame;              // num of frame data that has been send


	/* Inband command stuff */
	long               statusFlag;
	long               cmdFlag;
	int64_t            decodeInitCMD;
	VIDEOMIXERBLEND    blendCMD;
	VIDEODECODECOMMAND decodeCMD;
	AUDIOFORMAT        audioFormat;


	/* IO stuff */
	//unsigned char      buffer[OGG_BUFSIZE];
	unsigned char*     buffer;
	unsigned char*     bufferNonCached;
	//added by baili_feng
	unsigned char*     remainbuffer;
	unsigned char*     remainbufferNonCached;
	int                remainpayloadSize;
	unsigned char*     indexbuffer;
	unsigned char*     indexbufferNonCached;

	void*              fileHandler;
	IOPLUGIN*          ioPlugin;
	osal_mutex_t*      ioMutex;
	unsigned int       eventId;
	unsigned char*     eventData;
	unsigned int       eventSize;         
	long               fatalErrCode;
	oggstream_t        oggstream;

	/* OGG file info */
	unsigned long      fileEndAddress;
	int                version;
	int                packetSize;
	int                packetNumber;
	int                streamSerialNumber;
	bool               fileEnd;

	/* The granule position (little endian) of an ogg page containing
	 * audio packets is the absolute pcm position in the stream
	 * after the last packet ending on this page is processed.
	 */
	unsigned int       granulePos_high;
	unsigned int       lastGranulePos_high;
//	unsigned int       fileTotalBytes;
	int                nonsyncPayloadSize;
	bool               resync_flg;
	bool               readErrorFlg;

	//added by baili_feng
	
	VIDEOUSABILITYINFO	videoUsabilityInfo;
	VIDEOMPEG4USABILITYINFO mp4info;
	VIDEOFORMAT	videoFormat;
	VIDEODIVX4	videoFormat2;
	EventSpicFrame  spicFrame;
	int		audioStreamId;
	int		pageSegments;
	int64_t		timeUnit;//timeUnit == 90000/fps
	oggtrack_t**	pTracks;
	int		nTracks;
	oggtrack_t*	pVideoTrack;
	oggtrack_t*	pAudioTrack;
	oggtrack_t*	pSpicTrack;
	oggtrack_t*	pSubtxtTrack;
	uint32_t	biCompression;
	int		duration_in_seconds;
	char*		subText;
	int		subTextPos;
	NAVIOINFO	ioInfo;
	int64_t		fileTotalBytes;
	int		m_speed;
	PRESENTATION_POSITIONS  currPlaybackPosition;
	int64_t                 currVideoSystemPTS;

	oggframe_t*	keyFrameIndex;
	oggstream_t	oggindexstream;

	unsigned int	lastKeyGranule;
	oggheader_t	pageHeader;
	int		isLastKey;
	int		isIndexFilled;
	int		isIndexFillOK;
	uint64_t	indexOff;
	ogg_play_mode	playMode;
	uint64_t	resync_off;
	int		needContinue;
	int		lastKey;

	int		isVorbHeaderSaved;
	int64_t		currRefClock;
	int64_t		videoPTS;
	NAVMEDIAINFO*	pMediaInfo;

	oggchapter_t*	pChapters;
	int		nChapters;
	int		sizeChapters;

	uint32_t	lastVideoStreamNum;
	uint32_t	lastVideoPageSeq;
	int		lastVideoPkt;

	uint32_t	lastAudioStreamNum;
	uint32_t	lastAudioPageSeq;
	int		lastAudioPkt;
	bool		lastAudioReadPayloadFlg;

	uint32_t	lastSpicStreamNum;
	uint32_t	lastSpicPageSeq;

	uint32_t	switchVideoStreamNum;
	uint32_t	switchVideoPageSeq;
	int		switchVideoPkt;

	uint32_t	switchAudioStreamNum;
	uint32_t	switchAudioPageSeq;
	int		switchAudioPkt;
	bool		switchAudioReadPayloadFlg;

	uint32_t	switchSpicStreamNum;
	uint32_t	switchSpicPageSeq;

	int64_t		switchAudioPTS;
	int		switchAudioOff;

	int64_t		switchSpicPTS;
	unsigned char*	div3buffer;
	int		div3size;
	int64_t		div3PTS;
	unsigned char*	vorbisbuffer;
	int		vorbissize;
	int64_t		vorbisPTS;
	int		bDIVX3Sync;
	RIFFCHUNK	riffChunk;
	bool		bDirectIO;

	//for A-B repeat
	int     abStartSec;
	int     abStartFrame;
	int     abEndSec;
	int     abEndFrame;

	NAV_AVSYNC_MODE         AVSyncMode;
#if 1
	AUDIOVORBIS        audioVorbis;
#else
	/* The following data must be in this specific order and contiguous ! */
	int                vorbisPktType;
	unsigned int       backShift;
	int                cmdHeaderType;
	int                cmdHeaderSize;
	AUDIOVORBISPKTINFO vorbisPacketListInfo[255];
	/* Contiguous end */
#endif

	bool               readPayloadFlg;
	int                payloadSize;

	int                vorbisPktCnt;

	bool		keyFlag;
	bool		needSyncToKey;
} ogg_input_plugin_t;


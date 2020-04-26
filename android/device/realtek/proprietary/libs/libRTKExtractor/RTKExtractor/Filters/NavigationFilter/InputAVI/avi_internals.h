#ifndef __AVI_INPUT_PLUGIN_INTERNAL_HEADER__
#define __AVI_INPUT_PLUGIN_INTERNAL_HEADER__

#include "aviriff.h"
#include "mmreg.h"
#include "divx_ex.h"
#include "utility.h"
#include "asf.h"
#include "ogg.h"
#if defined(DIVX_ULTRA)
	#include "DivxMenuControllerTypes.h"
	#include "DivXMenuControllerAPI.h"
#endif
#include <OSAL/Types.h>

//#define SHOW_IO_PERFORMANCE
//#define DRY_RUN
#undef RDDEBUG

#if defined(RDDEBUG)
  #define DP(type, fmt...) printf(fmt)
#else
  #define DP(type, fmt...)
#endif

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
	#define AVI_IO_SIZE			(64*1024)
	#define AVI_IO_ALIGN_MASK	0xffffffffffff000LL
	#define AVI_IO_ALIGN_UNIT	(~AVI_IO_ALIGN_MASK)

	#define AVI_CMD_SIZE		(64*1024)

	#define AVI_BUFSIZE 		((128*1024) * 4)
	#define AVI_IDXSIZE			((128*1024) * 4)
	#define CONGEST_ACCESS_BYTE	((32*1024) * 4)
	#define CONGEST_IDX_NUM		((32*1024/16) * 4)
#else
	#define AVI_IO_SIZE			(32*1024)
	#define AVI_IO_ALIGN_MASK	0xfffffffffffff000LL
	#define AVI_IO_ALIGN_UNIT	(~AVI_IO_ALIGN_MASK)

	#define AVI_CMD_SIZE		(64*1024)

	#define AVI_BUFSIZE 		(128*1024)
	#define AVI_IDXSIZE			(128*1024)
	#define CONGEST_ACCESS_BYTE	(32*1024)
	#define CONGEST_IDX_NUM		(32*1024/16)
#endif

#define AVI_FMT_PARSER_SIZE	(32*1024)

#define AUDIO_PRELOAD_PTS       90000

#define AVI_RETRIAL_LIMIT	10
#define AVI_IOERROR_LIMIT	64

//for index of index
#define INDEXING_SIZE           3*60
#define INDEX_PTS_DISTANCE      60*90000
#define MAX_AVI_STREAMS        17


/*DivX ultra define junk*/
#define MENU_MEMORY_POOL_SIZE (40*1024)
#define MENU_INFO_CHUNK_SIZE  (400*1024)
#define ALIGMENT              8

#define FWD_IONLY_SPEED 256
#define RWD_IONLY_SPEED 0

/* private table definition */
#define PENDING_AUDIO_WMA_TWEAK			(1L << 0)
#define PENDING_AUDIO_AAC_TWEAK 		(1L << 1)
#define PENDING_AUDIO_VORBIS_INIT_TWEAK	(1L << 2)
#define PENDING_AUDIO_VORBIS_TWEAK		(1L << 3)
#define PENDING_DIVX3_TWEAK				(1L << 4)
#define PENDING_SPIC_FORMAT				(1L << 5)
#define PENDING_SPIC_CONFIG				(1L << 6)
#define PENDING_RESET_SPEED				(1L << 7)
#define PENDING_VIDEO_END_SEQ			(1L << 8)
#define PENDING_DCI_CCI					(1L << 9)
#define PENDING_EOS_RDY					(1L << 10)
#define PENDING_NOT_AUTH_USER			(1L << 11)
#define PENDING_RENTAL_EXPIRED			(1L << 12)
#define PENDING_REQUEST_RENTAL			(1L << 13)
#define PENDING_WAIT_APPROVE			(1L << 14)
#define PENDING_NOT_AUTHORIZED			(1L << 15)
#define PENDING_READ_ERROR				(1L << 16)
#define PENDING_DIVX_SUBTITLE_TWEAK		(1L << 17)
#define PENDING_UPDATE_AUDIO_POSITION	(1L << 18)

#define STATE_RUNNING			(1L << 0)
#define STATE_PAUSED			(1L << 1)
#define STATE_HEADER_PARSED		(1L << 2)
#define STATE_MENU_RESUME		(1L << 3)
#define STATE_STOP_RESUME		(1L << 4)
#define STATE_IO_ERROR			(1L << 5)
#define STATE_IO_READ_ERROR		(1L << 6)
#define STATE_GET_CHUNK_HEADER	(1L << 7)
#define STATE_BUILDING_INDEX	(1L << 8)
#define STATE_PARTIAL_INDEX		(1L << 9)
#define STATE_FORCE_SEQUENTIAL	(1L << 10)

#define DRM_ENCRYPT_DETECTED	(1L << 0)
#define DRM_RDY_TO_SEND			(1L << 1)

#define CMD_AVI_AUDIO_FLUSH			(1L << 0)
#define CMD_AVI_SPIC_FLUSH			(1L << 1)
#define CMD_AVI_ALL_FLUSH			(1L << 2)
#define	CMD_AVI_NEW_SEGMENT			(1L << 3)
#define CMD_AVI_VIDEO_FORMAT		(1L << 4)
#define CMD_AVI_VIDEO_EXTRA_DATA	(1L << 5)
#define	CMD_AVI_DECODE_INIT			(1L << 6)
#define	CMD_AVI_BLEND				(1L << 7)
#define	CMD_AVI_DECODE				(1L << 8)
#define CMD_AVI_END_OF_SEGMENT		(1L << 9)
#define CMD_AVI_STOP				(1L << 10)
#define CMD_AVI_MEDIA_REMOVED		(1L << 11)
#define CMD_AVI_AUDIO_FORMAT		(1L << 12)
#define CMD_AVI_AUDIO_DECODE		(1L << 13)
#define CMD_AVI_AVSYNC_CHANGE		(1L << 14)
#define CMD_AVI_VIDEO_MPEG4_USABILITY_INFO (1L << 15)
#define CMD_AVI_VIDEO_USABILITY_INFO (1L << 16)

#ifndef IS_SET
	#define IS_SET(x,y)		((x) & (y))
	#define ClearFlag(x,y)	((x) &= ~(y))
	#define SetFlag(x,y)	((x) |= (y))
#endif

typedef enum {
	NO_INDEX,
	MOVI_LIST_BASED,
	FILE_START_BASED,
	AVI2_SINGLE_INDEX,
	AVI2_TWO_TIERED_INDEX
} idx1Type;

typedef enum {
	SEEK_FORWARD,
	SEEK_BACKWARD,
} seekDirection;

typedef enum {
	AUDIO_PACKET,
	VIDEO_PACKET,
	SPIC_PACKET,
	OTHERS,
} chunkType;

typedef enum {
	Init,
	Recalc,
} calcType;

typedef enum {
	UNIDENTIFIED,
	AUDIO_STREAM,
	VIDEO_STREAM,
	SUBTITLE_STREAM,
} STREAMTYPE;

typedef struct _tagAVIndexing {
  STREAMTYPE streamType;
  char isVBR;
  uint32_t totalLen;
  int64_t idxPos[INDEXING_SIZE];
  int64_t idxPts[INDEXING_SIZE];
  int64_t frameCount[INDEXING_SIZE];
  int64_t chunkCount[INDEXING_SIZE];
} AVIndexing;

typedef struct _tagAVIndexingBuf{
  struct _avioldindex_entry indexEntry[CONGEST_IDX_NUM];
}AVIndexingBuf;


typedef struct _tagDRMParams {
	uint8_t 		rentalMessageFlag;
	uint8_t 		useLimit;
	uint8_t 		useCount;

	uint32_t		leftCount;

	uint8_t			acptbSignal;
	uint8_t			cgmsaSignal;
	uint8_t			digitalProtectionSignal;
	uint8_t			ict;
} DRMPARAMS;

typedef struct _tagDRMInfoChunk {
	uint16_t	frameKeyIndex __attribute__((packed));
	uint32_t	encryptionOffset __attribute__((packed));
	uint32_t	encryptionLength __attribute__((packed));
} DRMINFOCHUNK;

typedef struct _tagStreamNameInfo {
	STREAMTYPE 	streamType;
	char		languageTag[2];
	char		countryTag[2];
	char		typeTag[2];
	bool		formatParserIdentified;
} STREAMNAMEINFO;

typedef struct _tagAVIDescriptor {
	int64_t			hdrlOffset;
	unsigned int	hdrlSize;
    int64_t    		moviOffset; // stream data offset
	unsigned int	moviSize;
    int64_t    		idx1Offset; // index data offset
	unsigned int	idx1Size;
    AVIMAINHEADER   mainHeader;
    AVISTREAMHEADER *streamHeader;
    WAVEFORMATEX    *waveformHeader;
	BITMAPINFO		*videoBitmapInfo;
	STREAMNAMEINFO	*streamNameInfo;
	STRD_CHUNK		*drmChunk;
	AVISUPERINDEX	*avi2SuperIndex;
	AVIEXTHEADER	dmlHeader;

	idx1Type		indexType;
} AVIDescriptor;

typedef struct _tagNavState {
	int	version;
	int frameCount;
	int audioStreamIdx;
	int spicStreamIdx;
} AVINavState;

/* plugin instance object */

typedef struct _tagInputAVI_t {
	/*--------------------------------------------------------------
		Inband command stuff
	---------------------------------------------------------------*/
#ifdef SHOW_IO_PERFORMANCE
	int64_t abs_start;
	int64_t audio_bytes;
	int64_t video_bytes;
#endif
		
    int64_t streamTotalBytes;
    void* pParentInstance;

	int64_t			currReadPosition;

	bool			bIsSplitted;
	bool			bIsPTSSent;
	unsigned int	remainSize;

	PRESENTATION_POSITIONS	currPlaybackPosition;
	int64_t			currVideoSystemPTS;
	int64_t			prevVideoSystemPTS;
	int64_t			currRefClock;

	int				audioStreamIdx;
	int				videoStreamIdx;
	int				spicStreamIdx;

	uint32_t		audioIdxPosOffset;	// offset for accessing idx1 
	uint32_t		videoIdxPosOffset; 	// offset for accessing idx1
	uint32_t		spicIdxPosOffset;	// offset for accessing idx1
	uint32_t		firstvideoIdxPosOffset;

	/* for OpenDML Super Index 'indx' access index */
	uint32_t		audioAVI2_SprIdx;
	uint32_t		videoAVI2_SprIdx;
	uint32_t		spicAVI2_SprIdx;

	/* for OpenDML Standard Index index */
	uint32_t		audioAVI2_StdIdx;
	uint32_t		videoAVI2_StdIdx;
	uint32_t		spicAVI2_StdIdx;

	/* for OpenDML IndexEntry */
	AVISTDINDEX		aSTDINDEX;
	AVISTDINDEX 	vSTDINDEX;
	AVISTDINDEX 	sSTDINDEX;

	int				aSTDINDEX_from;
	int				aSTDINDEX_size;

	int				vSTDINDEX_from;
	int				vSTDINDEX_size;

	int				sSTDINDEX_from;
	int				sSTDINDEX_size;

	/* for easing audio PTS calculation *tricky* */
	int64_t			audioBasePTS;

	int				mEvent;
	int				mState;

	bool 			bIsType1DV;

	bool			bExistAVIX;
	bool			bModeAVIX;

	bool			bAudioSeek;
	bool			bVideoSeek;
	bool			bSpicSeek;

	bool			bAudioTerm;
	bool			bVideoTerm;
	bool			bSpicTerm;

	bool			bVideoEndSeq;
	bool			bStreamEnd;

	bool			bIsDRM;
	bool			decrypt_flag;
	
	AUDIODECODECOMMAND audioDecodeCmd;

	bool			bAudioSplitTransmit;
	bool			bAudioTransmitRestBytes;
	unsigned int	audioRestBytes;
	int64_t			audioReadPosition;
	
	DRMINFOCHUNK	drmInfoChunk;

	DRMPARAMS		drmParams;

	void 			*drmContext;

    NAV_REPEAT_MODE  repeatMode;

    uint32_t		chunkSize;
	int64_t			chunkOffset;

	AUDIOFORMAT audioFormat;
	VIDEOFORMAT videoFormat;
	VIDEODIVX4	videoFormat2;
#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
	VIDEOVC1SEQINFO videoFormat3;
#endif

	VIDEOUSABILITYINFO videousabilityinfo;
	VIDEOMPEG4USABILITYINFO mp4info;

	SPUCONFIG	spuConfig;
	int64_t		audio_preload_pts;

	AVIDescriptor	fileInfo;

	chunkType		currentChunkType;

	NAVMEDIAINFO	media;

	int64_t			audioPTS;
	int64_t			videoPTS;
	int64_t			spicPTS;

	int64_t			initVideoPTS;

	int32_t			videoFrameCount;
	int64_t			audioFrameCount;
	int32_t			audioChunkCount;

	uint32_t		audioChunkSize;

	VIDEODVDDCICCI	DCICCI;

	/* for speed control, I-Frame Scan Only */
	bool			m_ISCAN_rate;
	int				m_speed;
	int				m_skip;

	/* for spic */
	int				m_spicOnOff;

	/* for DivX DRM */
	NAVDIVXDRMINFO	drmStatus;

	/* for Language Preference feature */
	bool m_bLangPreference;
	bool m_bUpdateLangPreference;
	NAVLANGPREFERENCE m_langPreference;

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

	unsigned char		*decryptBuffer;

	unsigned char		*ioBuffer;
	long long			ioBufferStart;
	int					ioBufferSize;

	unsigned char		*idxBufferAudio;
	int					idxBufferAudioStart; // idxBuffer (base: idx1Offset)
	int					idxBufferAudioSize;  // for super mini sized idx1 chunk

	unsigned char		*idxBufferVideo;
	int					idxBufferVideoStart; // idxBuffer (base: idx1Offset)
	int					idxBufferVideoSize;  // for super mini sized idx1 chunk

	unsigned char		*idxBufferSpic;
	int					idxBufferSpicStart; // idxBuffer (base: idx1Offset)
	int					idxBufferSpicSize;  // for super mini sized idx1 chunk

	unsigned char		*fmtParserBuffer;
	int					fmtParserBufferSize;

	unsigned long 		fmtParserBufferPhyAddr;

	IOPLUGIN* 			ioPlugin;
    osal_mutex_t*		ioMutex;
	osal_mutex_t		m_readMutex;

	RIFFCHUNK			matchChunk;	// for byte-to-byte search
	unsigned int		matchReceivedBytes;

	RIFFCHUNK			tmpChunk; 			// for DivX3 Video/DivX Subtitle only

	char 				spicTimeInfo[27];
	int					spicTimeInfoReceivedBytes;

	//for A-B repeat
	int	abStartSec; 
	int	abStartFrame;
	int	abEndSec;
	int	abEndFrame;

  //trick play optimization storage (index of index)
  char hasIndexOfIndex;
  AVIndexing superIndexing[MAX_AVI_STREAMS];
  int64_t superIndexing_frameCount[MAX_AVI_STREAMS];
  int64_t superIndexing_chunkCount[MAX_AVI_STREAMS];
  int64_t superIndexing_trackPTS[MAX_AVI_STREAMS];
  int64_t superIndexing_indexLastPTS[MAX_AVI_STREAMS];
  
  int superIndexing_ptr;

 // DivX Ultra Junk
  char hasMenu;   //flag to show whether current DivX file has menu or not
  char isReverseFourCC; //flag to show whether current DivX chunk is reverse fourCC or not
  unsigned char *menuInfoChunk;
  unsigned char *menuMemoryPool;
#if defined(DIVX_ULTRA)
  DMCData *dmcData;
  DMCMediaTrackList *playList;
  DMCPlaybackType playbackType;
#endif
  int menuVideoStartFrame;
  int menuVideoEndFrame;
  int mainFeatureFrame;
  bool mainFeatureResume;
  int mainFeatureResumeID;

 // DivX Ultra Information buffer
  int currTitleId;
  int currChapterNum;
  int currChapterTotalNum;
  int currChapterStartFrame; 
  int currChapterEndFrame;

 // bad content workaround
  struct _avioldindex_entry currIndex;

  long fatalErrCode;

  // vorbis support
  OGG_STATUS_DESCRIPTOR oggVorbis;
  unsigned char *oggVorbisPartialBuffer;
  size_t oggVorbisPartialBufferSize;

  int64_t oggPTS;

#if AP_CONFIG_WITH(APCFG_SMOOTH_TRICK_PLAY)
#define CONSEQUENCE_FRAME_COUNT 15
  int32_t forwardBeginFrame;
  int32_t forwardSentFrame;
  bool bForwardGetIFrame;
#endif

#if defined(DIVX_PLUS)
  unsigned int repeatChapter;
#endif

	FILE*	pAudioFile;
	FILE*	pVideoFile;
	bool b_mUsedInband;
} InputAVI_t;

#endif

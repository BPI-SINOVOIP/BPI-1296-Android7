#ifndef __APE_H__
#define __APE_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include <hresult.h>
#include <StreamClass/EType.h>
#include <Filters/PrivateInfo.h>
#include <InbandAPI.h>
#include <Util.h>
#include <FlowEvent.h>
#include "../../../../../common/IPC/include/rpc_common.h"

// a light-weight class to parse APE header:
#include "APEHeaderManager.h"

#define BLOCKS_PER_DECODE               9216

#define STATUS_APE_AT_PAUSE_STATE              (1L << 0)
#define STATUS_APE_AT_RUN_STATE                (1L << 1)
#define STATUS_APE_AT_STOP_STATE               (1L << 2)
#define STATUS_APE_USE_ODIRECT_OPEN            (1L << 3)
#define STATUS_APE_SHOULD_SEND_SEQ_END         (1L << 4)
#define STATUS_APE_SHOULD_SEND_SEG_END         (1L << 5)
#define STATUS_APE_SHOULD_SEND_STOP            (1L << 6)
#define STATUS_APE_HAS_VIDEO_STREAM            (1L << 7)
#define STATUS_APE_HAS_AUDIO_STREAM            (1L << 8)
#define STATUS_APE_DEVICE_REMOVED              (1L << 9)
#define STATUS_APE_INITIAL_PTS_SENT            (1L << 10)

#define CMD_APE_SET_AUDIO_FORMAT       (1L << 0)
#define CMD_APE_NEW_SEGMENT            (1L << 1)
#define CMD_APE_DECODE_INIT            (1L << 2)
#define CMD_APE_BLEND                  (1L << 3)
#define CMD_APE_DECODE                 (1L << 4)
#define CMD_APE_AUDIO_MEDIA_REMOVED    (1L << 5)
#define CMD_APE_PRESENTATION_TIME      (1L << 6)
#define CMD_APE_END_OF_SEQUENCE        (1L << 7)
#define CMD_APE_END_OF_SEGMENT         (1L << 8)
#define CMD_APE_STOP                   (1L << 9)
#define CMD_APE_POST_EVENT             (1L << 10)
#define CMD_APE_SKIP_BITS              (1L << 11)

//#define APE_BUFSIZE        (12*1024) // 12K bytes
//#define APE_IO_SIZE        (24*1024) // 24K bytes
//#define APE_BUFSIZE        (18*1024) // 18K bytes
#define APE_IO_SIZE        (18*1024) // 36K bytes
#define APE_IO_ALIGN_MASK  0xfffffffffffff000LL
#define APE_IO_UNIT        (~APE_IO_ALIGN_MASK)

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |     \
                  (((uint32_t)(ch4) & 0xFF00) << 8) |    \
                  (((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
                  (((uint32_t)(ch4) & 0xFF000000) >> 24))

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef IS_SET
    #define IS_SET(x,y)     ((x) & (y))
    #define ClearFlag(x,y)  ((x) &= ~(y))
    #define SetFlag(x,y)    ((x) |= (y))
#endif

#define	ANALYSIS_BUF_LEN	(1024*8)//	8KB

typedef struct _tagape_input_plugin {

    /* IO */
    int64_t streamTotalBytes;
//    long long bufferStart;
//    int bufferSize;

    /* PCM file info */
    AUDIOFORMAT audioFormat;
    int64_t     baseAudioPTS;

    /* Playback information */
    PRESENTATION_POSITIONS  currPlaybackPosition;

    /* Inband command stuff */
    long                statusFlag;
    long                cmdFlag;
    int64_t             decodeInitCMD;
    VIDEOMIXERBLEND     blendCMD;
    AUDIODECODECOMMAND	decodeCMD;
    APESKIPBITSINFO     skipBitsCMD;
    //VIDEODECODECOMMAND  decodeCMD;
    
    /* event stuff */
    long                fatalErrCode;
    unsigned int        eventId;
    NAV_REPEAT_MODE     repeatMode;

    /* IO stuff */
    NAVIOINFO           ioInfo;
    int64_t             ioOffset;
    bool                bDirectIO;
    bool                bDTSCD;

    int64_t             currFilePos;
    int64_t             fileSize;
    int64_t             dataStart;

    unsigned char       *ioBuffer;
    unsigned char       *ioBufferNonCached;
    long long           ioBufferStart;
    int                 ioBufferSize;

    void*               fileHandler;
    IOPLUGIN*           ioPlugin;
    osal_mutex_t*       ioMutex;

    APEHeaderManager *pHeaderManager;

	bool bIsSeeked; // This variable is used to store the state, then we can move the read pointer to suitable place in _read_block(). 

	int ape_titleNum;
	int ape_track_start;
	int ape_track_end;

	int ape_total_time;

	int ape_startFrameIdx;
	int ape_endFrameIdx;
	bool bRepeat;

	int seekReminder;
	bool bHasSeekReminder;

	int decodeReminder;
	bool bHasDecodeReminder;
	bool doFirstBlockRead;

	bool bWaitNewSegComplete;
	float ratioValue;
	int previousAudioSystemPTS;
	volatile int currentAudioSystemPTS;

	int playAtTimeOffset;
	int seekTimeOffset;

	// a flag to let InputAPE aware a playAtTime event is just set.
	bool bPlayAtTimeFlag;
	int adjustRound;

	unsigned char analysisBuf[ANALYSIS_BUF_LEN];
	volatile bool bEOTFlag; 	// EOT: end of track
	volatile bool bEOTSetFlag;	// EOT flag has been set

	// for old ape file version, we should let audio firmware know how many bits should be skipped.
	int skipBytes;
	int skipBits;
	int skipFramIdx;

} ape_input_plugin_t;     

#endif /* __APE_H__ */

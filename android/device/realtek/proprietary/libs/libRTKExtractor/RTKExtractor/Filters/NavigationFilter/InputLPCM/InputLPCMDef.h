#ifndef __LPCM_H__
#define __LPCM_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include <hresult.h>
#include <StreamClass/EType.h>
#include <Filters/PrivateInfo.h>
#include <InbandAPI.h>
#include <Util.h>
#include <FlowEvent.h>
#include "../../../../../common/IPC/include/rpc_common.h"

#define STATUS_LPCM_AT_PAUSE_STATE              (1L << 0)
#define STATUS_LPCM_AT_RUN_STATE                (1L << 1)
#define STATUS_LPCM_AT_STOP_STATE               (1L << 2)
#define STATUS_LPCM_USE_ODIRECT_OPEN            (1L << 3)
#define STATUS_LPCM_SHOULD_SEND_SEQ_END         (1L << 4)
#define STATUS_LPCM_SHOULD_SEND_SEG_END         (1L << 5)
#define STATUS_LPCM_SHOULD_SEND_STOP            (1L << 6)
#define STATUS_LPCM_HAS_VIDEO_STREAM            (1L << 7)
#define STATUS_LPCM_HAS_AUDIO_STREAM            (1L << 8)
#define STATUS_LPCM_DEVICE_REMOVED              (1L << 9)
#define STATUS_LPCM_INITIAL_PTS_SENT            (1L << 10)

#define CMD_LPCM_SET_AUDIO_FORMAT       (1L << 0)
#define CMD_LPCM_NEW_SEGMENT            (1L << 1)
#define CMD_LPCM_DECODE_INIT            (1L << 2)
#define CMD_LPCM_BLEND                  (1L << 3)
#define CMD_LPCM_DECODE                 (1L << 4)
#define CMD_LPCM_AUDIO_MEDIA_REMOVED    (1L << 5)
#define CMD_LPCM_PRESENTATION_TIME      (1L << 6)
#define CMD_LPCM_END_OF_SEQUENCE        (1L << 7)
#define CMD_LPCM_END_OF_SEGMENT         (1L << 8)
#define CMD_LPCM_STOP                   (1L << 9)
#define CMD_LPCM_POST_EVENT             (1L << 10)

#define LPCM_BUFSIZE        (16*1024) // 16K bytes
#define LPCM_IO_SIZE        (32*1024) // 32K bytes
#define LPCM_IO_ALIGN_MASK  0xfffffffffffff000LL
#define LPCM_IO_UNIT        (~LPCM_IO_ALIGN_MASK)

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

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


typedef struct _taglpcm_input_plugin {

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
    VIDEODECODECOMMAND  decodeCMD;
    
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

} lpcm_input_plugin_t;     

#endif /* __LPCM_H__ */

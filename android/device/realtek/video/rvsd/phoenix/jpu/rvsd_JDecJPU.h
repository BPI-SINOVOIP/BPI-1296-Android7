#ifndef RVSD_RVSD_JPU_H
#define RVSD_RVSD_JPU_H

#include <OMX_Types.h>
#include <OMX_Core.h>

#include "rvsd_def.h"
#include "jputypes.h"

//RTK CNM header file
#include "jpuconfig.h"
#include "jpuapi.h"
#include "jpuapifunc.h"
#include "regdefine.h"

#include "jpuhelper.h"
#include "jpulog.h"

#define USE_ION_ALLOC_FB

#ifdef USE_ION_ALLOC_FB
#include "ion/ion.h"
#include <sys/mman.h>
#endif

#define RVSD_SUPPORT_SE
#ifdef RVSD_SUPPORT_SE
#include "RtkIpc/SeOp.h"
#endif

#define MCU_SIZE 16
#define MCU_MAX_COUNT (8100)
#define DEFAULT_STREAMBUFFER_SIZE       2*1024*1024//(4*1024*1024)
#define DEFAULT_WIDTH					MAX_MJPG_PIC_WIDTH
#define DEFAULT_HEIGHT					MAX_MJPG_PIC_HEIGHT
#define EXTRA_FRAME_BUFFER_NUM		3
#define JPU_DEC_TIMEOUT				100
#define MAX_DISP_QUEUE_BUFFER               31
#define JPU_MAX_TIME_QUEUE 256
#define JPU_WAIT_TIME_OUT 10

#define FORMAT_TRANS_BUFFER_NUM	1//for format 422->420

struct TimeStruct
{
	unsigned int wPtr;
	long long nTimeStamp;
};

typedef enum CODEC_STATE
{
	CODEC_NEED_MORE,
	CODEC_HAS_FRAME,
	CODEC_HAS_INFO,
	CODEC_OK,
	CODEC_WAIT_SEEK,
	CODEC_NOT_HAS_RESOURCE,
	CODEC_DECODE_FINISH,
	CODEC_DECODE,
	CODEC_BUFFER_EMPTY,
	CODEC_ERROR_HW_TIMEOUT = -1,
	CODEC_ERROR_HW_BUS_ERROR = -2,
	CODEC_ERROR_SYS = -3,
	CODEC_ERROR_DWL = -4,
	CODEC_ERROR_UNSPECIFIED = -5,
	CODEC_ERROR_STREAM = -6,
	CODEC_ERROR_INVALID_ARGUMENT = -7,
	CODEC_ERROR_NOT_INITIALIZED = -8,
	CODEC_ERROR_INITFAIL = -9,
	CODEC_ERROR_HW_RESERVED = -10,
	CODEC_ERROR_MEMFAIL = -11,
	CODEC_ERROR_STREAM_NOT_SUPPORTED = -12,
	CODEC_ERROR_FORMAT_NOT_SUPPORTED = -13,
	CODEC_FAIL,
	CODEC_FILL_NONE
} CODEC_STATE;

typedef enum JPEG_DEC_STATE
{
    JPEG_PARSE_HEADERS,
    JPEG_DECODE
} JPEG_DEC_STATE;

typedef struct {
    JpgDecOutputInfo*  buffer;
    int             size;
    int             count;
    int             front;
    int             rear;
} jpeg_frame_queue_item_t;

typedef struct
{
	unsigned int reserved: 2;
	unsigned int skip_nonref: 1 ;
	unsigned int low_delay: 1;
	unsigned int decMode: 4 ;
	unsigned int search_I_frm: 1 ;
	unsigned int checkVOBUPos: 1 ; //need to be reset, for vc1 vobu cmd
	unsigned int reset_timeInc: 1;
	unsigned int initPTS: 1;
	unsigned int useHdrFrmRate: 1;//for mpeg2 pts inc calculation.
	unsigned int waitDispIFrm: 1;//fix mpeg2 mosaic at the file start
	unsigned int useVOBURPTS: 1;
	unsigned int pts_sync: 2;//pts sync state
	unsigned int startOneFrm: 1;//dec state is start one frm(1) or get output info already(0)
	unsigned int startInitSeq: 1;//dec state is start init seq(1) or get initial info already(0)
	unsigned int mvc: 1;
	unsigned int idr_sync: 1;
	unsigned int broken_link: 1;	// only for H.264
	unsigned int dpb_bypass: 1;
	unsigned int isProg: 1;
	unsigned int useVOBU: 1;
	unsigned int reorder_disabled: 1;
	unsigned int stillVOBU: 1;
	unsigned int MVCFRGetRightFrm: 1;//in FR mode, to get the right-eye frm(possible not I), disable I ONLY mode temporarily.
	unsigned int hasDVDVRCmd: 1;//if had DVDVR_DCI_CCI cmd
	unsigned int notifiedVOEOS: 1;
	unsigned int waitBrokenLinkDone: 1;
	unsigned int err_notify: 1;
} JDEC_DECODER_STATUS ;

typedef struct dec_config_t
{
    int bitFormat;
    int rotAngle;
    int mirDir;
    int useRot;
    int useDering;
    int outNum;
    int checkeos;
    int reorder;
    int seqInitMode;
    int dispMixerDirect;
    int bEnChrIntlv;

    //ROI
    int roiEnable;
    int roiWidth;
    int roiHeight;
    int roiOffsetX;
    int roiOffsetY;

    //packed
    int packedFormat;

    int usePartialMode;
    int partialBufNum; //[2 ~ 4]
} dec_config_t;

typedef struct CODEC_JPEG
{
    JpgDecHandle handle;
    JpgDecOpenParam decOP;
    JpgDecInitialInfo initialInfo;
    JpgDecOutputInfo outputInfo;
    JpgDecParam decParam;
    FrameBuffer frameBuf[MAX_FRAME];
    jpu_buffer_t vb_base;
    jpu_buffer_t vbStream;
    int fbWidth;
    int fbHeight;
    int fbStride;
    int fbFormat;
    int rotStride;
    int rotEnable;
    int regFbCount;
    int seqInited;
    int frameIdx;
    int ppIdx;
    int decodefinish;
    int int_reason;
    int totalNumofErrMbs;
    int needFrameBufCount;
    int regFrameBufCount;
    int instIdx;
    int mapType;
    int partPosIdx;
    int partBufIdx;
    int partMaxIdx;
    int partialHeight;
    int iHorScaleMode;
    int iVerScaleMode;

    OMX_BOOL bWritedData;
    OMX_S32 framebufSize;
#ifdef USE_ION_ALLOC_FB
    struct IONStruct pIonFb[MAX_FRAME + FORMAT_TRANS_BUFFER_NUM];
    struct IONStruct pIonDispFrame;
    Int32 mIonFd;
#endif
    unsigned long mSeHandle;
    jpeg_frame_queue_item_t *disp_queue;
    OMX_BOOL bWaitInputFlushDone;
    OMX_BOOL bWaitOutputFlushDone;
    OMX_U32 nPtsWPtr;
    OMX_U32 nPtsRPtr;
    OMX_S32 startOneFrm;
    OMX_TICKS timestampQueue[JPU_MAX_TIME_QUEUE];
    OMX_TICKS outputTimestamp;
    OMX_S32 framebuf420Size;

    OMX_S32 nTimeoutCount;

	//below rvsd add ...
	long long outTimestamp;
	long long nPreTimeStamp;
	long long nTimeTick;
    long long rpts;
    unsigned int nReciPtsWPtr;
    unsigned int nReciPtsRPtr;
    struct TimeStruct	DecReciveTimestamp[JPU_MAX_TIME_QUEUE];

    int nFormatTransBufferCount;
	int nFormatTransBufferIndex;

#ifdef RVSD_NEW_VIDEO_FLOW_EN
	unsigned int newSegRdPtr;
	unsigned int ICQData[200];

	unsigned int prePTSICQReceiveWP;
	unsigned int preUpdateDecBSWP;//preRcvBSWP;
	long long preRcvPTS;
#endif

	//flow control condition flag
    bool bKeepBuffer;	
	bool bUpdateBsRP;
	bool bUpdatePtsRP;
	int bufferEmtpyFlag;
    int bufferEmtpyWPtr;

    JDEC_DECODER_STATUS status;
	dec_config_t s_dec_config;

} CODEC_JPEG;


//function
int jpeg_frame_queue_count(jpeg_frame_queue_item_t* queue);
int jpeg_frame_queue_enqueue(jpeg_frame_queue_item_t* queue, JpgDecOutputInfo data);
int jpeg_frame_queue_dequeue(jpeg_frame_queue_item_t* queue, JpgDecOutputInfo* data);
int jpeg_frame_queue_dequeue_all(jpeg_frame_queue_item_t* queue);

//OMX_ERRORTYPE CreateImageDecoder(CODEC_JPEG *pJpu, unsigned long rvsd_BSringSize, unsigned long rvsd_BSringPhyaddr, unsigned long rvsd_BSringBase);
OMX_ERRORTYPE CreateImageDecoder(CODEC_JPEG *pJpu);
void DestroyImageDecoder(CODEC_JPEG *pJpu);
void SetJDecConfig(CODEC_JPEG *pJpu);
CODEC_STATE JDecStartDecode(CODEC_JPEG *pJpu);
CODEC_STATE JDecGetInfoRegisterFrameBuffer(CODEC_JPEG *pJpu);
#endif

#ifndef CC_DECODER_API_H
#define CC_DECODER_API_H

#include <IPC/rpc_common.h>
#include <Platform_Lib/Graphics/DirectGraphics.h>

#define SUBTITLE_TYPE_ATSC_CC 	1	// ATSC_CC
#ifdef USE_1080P_UI
#define	MEM_WIDTH               1920	// PVR
#define MEM_HEIGHT              1080	// PVR
#define	MEM_OBJ_HEIGHT          72	// PVR
#else
#define	MEM_WIDTH               1280	// PVR
#define MEM_HEIGHT              720	// PVR
#define	MEM_OBJ_HEIGHT          48	// PVR
#endif

/*typedef struct _RECTANGLE{
	UINT16	x;
	UINT16	y;
	UINT16	width;
	UINT16	height;
}RECT;*/

typedef struct _CC_RECTANGLE{
	unsigned short	x;
	unsigned short	y;
	unsigned short	width;
	unsigned short	height;
} CC_RECT;


typedef	struct	_CC_HANDLE {
	long				cc_id;
	unsigned char		used;
	long				videoDecoderID;
	RINGBUFFER_HEADER*	pBufHeader;
	long				bufAddrDiff;
	SURFACEHANDLE		hSurface[2];
} CC_HANDLE;

typedef struct _CC_ACTIVE_NO {
	unsigned long long		hasServices;			// from service1 to service63, 0 for no atsc cc
													// bit 1 = service 1, bit 63 = service 63
	short					hasNtscModes;			// from cc1 to cc4 , text1 to text 4 , 0 for no ntsc cc
													// bit 1 ~ 4 = cc1 ~ cc4 , bit 5 ~ 8 = text1 ~ text4
} CC_ACTIVE_NO;

typedef enum {
	CC_FONT_SIZE_SMALL,
	CC_FONT_SIZE_MEDIUM,
	CC_FONT_SIZE_LARGE,
	CC_FONT_SIZE_DEFAULT
} CC_FONT_SIZE;

typedef enum {
	CC_FONT_COLOR_WHITE,
	CC_FONT_COLOR_YELLOW,
	CC_FONT_COLOR_GREEN,
	CC_FONT_COLOR_BLACK,
	CC_FONT_COLOR_RED,
	CC_FONT_COLOR_DEFAULT
} CC_FONT_COLOR;

typedef enum {
	CC_BG_ALPHA_FULL,
	CC_BG_ALPHA_ZERO,
	CC_BG_ALPHA_DEFAULT
} CC_BG_ALPHA;

//retuen value is CC Decoder id
long CCDecoderCreate();

//STC_ERR_CODE stc_init(UINT8 layer)
/* return value = 0 means fail. */
char CCDecoderInit(long id);

//STC_ERR_CODE stc_terminate(void)
char CCDecoderDestroy(long id);


/*STC_ERR_CODE stc_setSource(BOOL layer, SUBTITLE_TYPE type, UINT16 vPid,
							UINT16 sPid, SUBTITLE_TPMOD tpMod)*/

char CCDecoderSetSource(long id, int type, short vPid, short sPid, long tpMod);

//STC_ERR_CODE stc_show(BOOL layer, BOOL show)
char CCDecoderShow(long id, char bEnable);

//STC_ERR_CODE stc_setVideoRect(BOOL   layer, RECT     ori_rect, RECT cur_rect)
char CCDecoderSetVideoRect(long id, CC_RECT* pOriRect, CC_RECT* pOutputRect);

//STC_ERR_CODE stc_enable(BOOL layer, BOOL enable)
char CCDecoderEnable(long id, char bEnable);

//stc_pause(BOOT layer, BOOL pause)
char CCDecoderPause(long id, char bPause);

//void Ds_CC_ChangeServiceLayer(CC_SERVLAYER servLayer)
char CCDecoderChangeServiceLayer(long id, char servLayer);

//void	Ds_CC_ChangeCaptionType(CC_CAPTION_TYPE ccType)
char CCDecoderChangeCaptionType(long id, char ccType);

//stc_flush(BOOL layer)
char CCPinFlush(long id);

//stc_shareBufAddr(struct by PVR)
/*// Ring Buffer header is the shared memory structure
typedef struct _tagRingBufferHeader {
  unsigned long magic;   //Magic number
  unsigned long beginAddr;
  unsigned long size;
  unsigned long bufferID;  // RINGBUFFER_TYPE, choose a type from RINGBUFFER_TYPE

  unsigned long  writePtr;
  unsigned long  numOfReadPtr;
  unsigned long  reserve2;  //Reserve for Red Zone
  unsigned long  reserve3;  //Reserve for Red Zone

  unsigned long readPtr[4];
} RINGBUFFER_HEADER;
*/
char CCPinSetRingBuffer(long id, RINGBUFFER_HEADER* pBufHeader, long diff);

char CCDecoderSetUIHandle(long m_id, SURFACEHANDLE hSurface1, SURFACEHANDLE hSurface2);

char CCDecoderGetUIHandle(long id, CC_HANDLE** HdlAddr);

char CCDecoderSetInterHandle(long id, SURFACEHANDLE hSmallSurf1, SURFACEHANDLE hSmallSurf2,
	                    			  SURFACEHANDLE hSmallSurf3, SURFACEHANDLE hSmallSurf4);

char  CCDecoderGetServiceNo(long id, CC_ACTIVE_NO *servNo);
char CCDecoderSetHandlerID(long id);

char CCDecoderSetCharSize(CC_FONT_SIZE size);
char CCDecoderSetCharColor(CC_FONT_COLOR color);
char CCDecoderSetBgAlpha(CC_BG_ALPHA alpha);
char CCDecoderGetCharAttr(CC_FONT_SIZE *size, CC_FONT_COLOR *color);

#endif


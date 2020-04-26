#ifndef TT_DECODER_API_H
#define TT_DECODER_API_H

#include <Platform_Lib/Graphics/DirectGraphics.h>

typedef enum _TT_DECODER_MODE_{

	ENUM_TT_MODE_NORMAL						= 0,	/*TT_MODE_NORMAL = 0*/
	ENUM_TT_MODE_SUBTITLE 					= 1,	/*TT_MODE_SUBTITLE = 1*/
	ENUM_TT_MODE_ONLY_VBI_OUTPUT 			= 2,	/*TT_MODE_ONLY_VBI_OUTPUT =2*/
	ENUM_TT_MODE_ALPHA_SCREEN 				= 3,	/*TT_MODE_ALPHA_SCREEN = 3*/
	ENUM_TT_MODE_NORMAL_WITH_VBI_OUTPUT 	= 4,	/*TT_MODE_NORMAL_WITH_VBI_OUTPUT = 4*/
	ENUM_TT_MODE_SUBTITLE_WITH_VBI_OUTPUT 	= 5,	/*TT_MODE_SUBTITLE_WITH_VBI_OUTPUT = 5*/
	
	ENUM_TT_MODE_UNDER_RUN 					= 0x80,	/*TT_MODE_UNDER_RUN = 0x80*/	
	

} ENUM_TT_DECODER_MODE;

typedef enum _TT_DECODER_USER_ACTION {

	ENUM_TT_NEXT_PAGE = 0,		//TT_NEXT_PAGE
	ENUM_TT_PREVIOUS_PAGE,		//TT_PREVIOUS_PAGE
	ENUM_TT_NEXT_SUBPAGE,		//TT_NEXT_SUBPAGE
	ENUM_TT_PREVIOUS_SUBPAGE,	//TT_PREVIOUS_SUBPAGE
	ENUM_TT_LINK1,				//TT_LINK1
	ENUM_TT_LINK2,				//TT_LINK2
	ENUM_TT_LINK3,				//TT_LINK3
	ENUM_TT_LINK4				//TT_LINK4

} ENUM_TT_DECODER_USER_ACTION;

typedef enum _TT_DECODER_NATIONAL_CHARSET {
	ENUM_TT_NATIONAL_CHARSET_0 = 0,
	ENUM_TT_NATIONAL_CHARSET_1,
	ENUM_TT_NATIONAL_CHARSET_2,
	ENUM_TT_NATIONAL_CHARSET_3,
	ENUM_TT_NATIONAL_CHARSET_4,
	ENUM_TT_NATIONAL_CHARSET_5,
	ENUM_TT_NATIONAL_CHARSET_6,
	ENUM_TT_NATIONAL_CHARSET_7,
	ENUM_TT_NATIONAL_CHARSET_8,
	ENUM_TT_NATIONAL_CHARSET_9,
	ENUM_TT_NATIONAL_CHARSET_10,
} ENUM_TT_DECODER_NATIONAL_CHARSET;

typedef struct _TT_DECODER_USER_PAGE {

	unsigned char		magNum;
	unsigned char		pageTens;
	unsigned char		pageUnits;
	unsigned int		subpageNum;

} TT_DECODER_USER_PAGE;

typedef struct _TT_DECODER_OUTPUT_REGION{
	int	x;	///< x
	int	y;	///< y
	int	w;	///< width
	int	h;	///< height
} TT_DECODER_OUTPUT_REGION;

// return value is TT decoder id
////long TTDecoderCreate();
long TTDecoderCreate(int cacheSize);


/* return value = 0 means fail. */


//TTApi_DeInit(m_id)
char TTDecoderDestroy(long id);

//BOOL TTApi_SetFontPath(UINT8* fontPath)
char TTDecoderSetFontPath(long id, const unsigned char* fontPath);

//BOOL TTApi_ShowScreen(BOOL enable);
char TTDecoderShowScreen(long id, char bEnable); 

char TTDecoderSetUserNation(long id, unsigned int userNation);
//BOOL TTApi_SetSubtitleMagazineAndPageNumber(UINT8 MagazineNumber, UINT8 PageNumber)
//char TTDecoderSetSubtitleMagAndPageNum(long id, char MagazineNumber, char PageNumber);

//BOOL TTApi_SetOutPutRegion(int pos_x, int pos_y, int pos_width, int pos_height, int pos_isScaling);
//char TTDecoderSetOutPutRegion(long id, int X, int Y, int Width, int Height, int isScaling);

//BOOL TTApi_Enable(BOOL enable, UINT8 modeDecision);
//char TTDecoderEnable(long id, char bEnable, ENUM_TT_DECODER_MODE mode);
char TTDecoderEnable(
	long id, 
	ENUM_TT_DECODER_MODE mode,
	TT_DECODER_OUTPUT_REGION* pRegion,
	unsigned char magazineNumber, unsigned char pageNumber
);//if ENUM_TT_MODE_NORMAL, always set Page:100 (ie. magazineNumber = 1, pageNumber = 00).

char TTDecoderDisable(long id);

char TTPinFlush(long id);
char TTInputPinSetRingBuffer(long id, RINGBUFFER_HEADER* pBufferHeader, long diff);
char TTOutputPinSetRingBuffer(long id, RINGBUFFER_HEADER* pBufferHeader, long diff);

//BOOL TTApi_SetUserLabel(char *src);
char TTDecoderSetUserLabel(long id, char* label);

//BOOL TTApi_ShowUserLabel(BOOL show);
char TTDecoderShowUserLabel(long id, char bShow);

//BOOL TTApi_SetUserPageAddr(TT_PAGE_ADDRESS pageAddr);
char TTDecoderSetUserPageAddr(long id, TT_DECODER_USER_PAGE* pPageAddr);

//BOOL TTApi_SetUserAction(TT_USER_ACTION userAction);
char TTDecoderSetUserAction(long id, ENUM_TT_DECODER_USER_ACTION action);

char TTDecoderSetUIHandle(long m_id, SURFACEHANDLE hSurface1, SURFACEHANDLE hSurface2);
char TTDecoderUnSetUIHandle(long m_id);
#endif

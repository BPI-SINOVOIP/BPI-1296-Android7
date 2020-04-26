/**
 * @file DirectGraphics.h
 * @brief 2D Graphics API
 * @defgroup DirectGraphicsInterface Direct Graphics Interface
 * @defgroup DisplayInterface Display Interface
*/
/**
 * @mainpage Introduction
 *
 * This document describes basic drawing API for visual UI interface design on Mars / Jupiter system. The 2D Graphics API complies two parts, Direct Graphics and GDI API. Direct Graphics is the hardware accelerated Graphic APIs supported by Mars and Jupiter system. The GDI API is the software library that supports basic drawing functions and objects.
 * \n For Window API, please refer to Venus Window Programming API.
 *
 * @section Scope Scope
 * This document covers the introduction and the application interfaces of Direct Graphics. It also mention about how to setup and use the GDI library.
 * @par Remarks
 * <b>Real-Time Design :</b>Use Direct Graphics interface will significantly increase the performance of the system compare to use the GDI API.
 * \n<b>User Interface Design :</b>Refer to User Interface material in the SRS and supplement with any design considerations not mentioned there. You should discuss the expected effectiveness of your design.
 *
 * @section References References
 * .../branch_src_sharedMemory_integration/Include/Platform_Lib/Graphics/DirectGrphics.h
 * \n Operating System Abstraction Layer (OSAL)
 *
 * @page DeploymentDiagram  Deployment Diagram
 * @image rtf GraphicInterfaceUncovered.PNG "Graphic Interface Uncovered"
 * Figure demonstrates the DVD+VR recording flow under StreamClass architecture. The Flow Manager is a class that serves as a gateway between Filters and Application.
 *
 */
#ifndef DIRECT_GRAPHICS_H
#define DIRECT_GRAPHICS_H

#include <OSAL.h>

#ifdef WITH_SW_OSD_SCALER
#include "Platform_Lib/Graphics/DrawScale.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_HANDLE ((void *)0)

typedef void * SURFACEHANDLE;

/**
 * @typedef
 */
typedef void * HANDLE;	///< HANDLE of object

typedef struct rect_t {
    int x;
    int y;
    int w;
    int h;
} RECTANGLE;

typedef struct {
    int32_t x;
    int32_t y;
} MYPOINT;

typedef struct {
    int32_t w;
    int32_t h;
} MYSHIFT;

typedef struct {
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
} MYRECT;

typedef union {
    int clipOn;
    int shiftOn;
    MYRECT clipRect;
    MYSHIFT shift;
} CONFIG_PARAM;

typedef enum {
    FLAG_CONFIG_SET_CLIP_ON_OFF,
    FLAG_CONFIG_GET_CLIP_ON_OFF,
    FLAG_CONFIG_SET_SHIFT_ON_OFF,
    FLAG_CONFIG_GET_SHIFT_ON_OFF,
    FLAG_CONFIG_SET_CLIP_RECT,
    FLAG_CONFIG_GET_CLIP_RECT,
    FLAG_CONFIG_SET_SHIFT_OFFSET,
    FLAG_CONFIG_GET_SHIFT_OFFSET
} ENUM_CONFIG_FLAG;

typedef enum {
    CONFIG_SOURCE,
    CONFIG_DESTINATION
} ENUM_CONFIG_TARGET;

typedef enum _STRETCH_COEFF_MODE
{
	SCALING_MODE_AUTO = 0,				// count stretch coefficient automatically
	SCALING_MODE_BY_USER = 1			// using stretch coefficient by user define

} STRETCH_COEFF_MODE;

#if 0
#ifndef MICROWINDOWS
#include <simulator/DG_drawimage.h>
#define HANDLE void *
#else
#include <simulator/windows.h>
#include <simulator/device.h>
#endif
#endif
/**
 * @enum PIXEL_FORMAT
 */
typedef enum _PIXELFORMAT
{
    Format_8,           ///< 332
    Format_16,          ///< 565
    Format_32,          ///< 8888 ARGB
    Format_32_888A,			///< 8888 RGBA
    Format_2_IDX,
    Format_4_IDX,
    Format_8_IDX,
    Format_1555,
    Format_4444_ARGB,
    Format_4444_YUVA,
    Format_8888_YUVA,
    Format_5551,
    Format_4444_RGBA,
    Format_YUV420,
    Format_YUV422,
    Format_YUV444,
    NO_OF_PIXEL_FORMAT
} PIXEL_FORMAT;

typedef enum
{
  VOUT_GRAPHIC_OSD=0,
  VOUT_GRAPHIC_SUB1=1,
  VOUT_GRAPHIC_SUB2=2,
  VOUT_GRAPHIC_OSD1=0,	//+JT - saturn's OSD1
  VOUT_GRAPHIC_OSD2=2	//+JT - saturn's OSD2
} VOUT_GRAPHIC_PLANE;

#define SE_VO_SYNC(_plane_) ((((_plane_)+1) << 24)) //+JT - NOTE: output of macro is _plane_ pluse 1, then 24bit left-shift.
#define IS_OSD_SYNC(_sync_) ((((_sync_) >> 24) & (0xFF)) == (VOUT_GRAPHIC_OSD+1)) //+JT - plus 1 before checking AS SE_VO_SYNC() increases 1 for every plane. 
#define IS_OSD1_SYNC(_sync_) ((((_sync_) >> 24) & (0xFF)) == (VOUT_GRAPHIC_OSD1+1))
#define IS_OSD2_SYNC(_sync_) ((((_sync_) >> 24) & (0xFF)) == (VOUT_GRAPHIC_OSD2+1))
#define IS_SUB1_SYNC(_sync_) ((((_sync_) >> 24) & (0xFF)) == (VOUT_GRAPHIC_SUB1+1))



typedef enum _vout_window_stacking
{
    VOUT_WINDOW_MIDDLE = 0,
    VOUT_WINDOW_UPPER  = 1,
    VOUT_WINDOW_LOWER  = 2
} VOUT_WINDOW_STACKING;

#ifdef USE_DFB_SUPPORT
#define RTK_OSD1_LAYER_ID	(DLID_PRIMARY)
#ifdef ENABLE_DFB_LAYER_OSD2
#define RTK_OSD2_LAYER_ID	(DLID_PRIMARY+1)
#define RTK_SUB1_LAYER_ID	(DLID_PRIMARY+2)
#else
#define RTK_OSD2_LAYER_ID	(DLID_PRIMARY)
#define RTK_SUB1_LAYER_ID	(DLID_PRIMARY+1)
#endif
#endif


#ifdef WITH_SW_OSD_SCALER
#define SIZE_OF_PREALLOCATED_GRAPHIC_MEMORY (0x100000*8)//+200*1024)
#define SIZE_OF_PREALLOCATED_GRAPHIC_MEMORY2 (0x100000*4)
#else
#define SIZE_OF_PREALLOCATED_GRAPHIC_MEMORY (0x100000*4)
#define SIZE_OF_PREALLOCATED_GRAPHIC_MEMORY2 0
#endif

#define MAKE_ARGB8888(a, r, g, b) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#define MAKE_ARGB0565(a, r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))
#define MAKE_ARGB0332(a, r, g, b) (((r) & 0xE0) | (((g) & 0xE0) >> 3) | (((b) & 0xC0) >> 6))

#define MAKE_RGB8(r, g, b)  MAKE_ARGB0332(0xFF, r, g, b)
#define MAKE_RGB16(r, g, b) MAKE_ARGB0565(0xFF, r, g, b)

#define MAKE_ARGB(a, r, g, b) MAKE_ARGB8888(a, r, g, b)

typedef unsigned int COLORDEF;

#ifdef COLOR_FORMAT_32

	#define Color_Format Format_32
	//#define MAKE_RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
	#define MAKE_RGB(r, g, b) MAKE_ARGB(0xFF, r, g, b)
	#define COLOR_FORMAT_BPP 32

#elif defined COLOR_FORMAT_8

	#define Color_Format Format_8
	//#define MAKE_RGB(r, g, b) MAKE_ARGB0332(a, r, g, b)
	#define MAKE_RGB MAKE_RGB8
	#define COLOR_FORMAT_BPP 8

#else //COLOR_FORMAT_16

	#define Color_Format Format_16
	//#define MAKE_RGB(r, g, b) MAKE_ARGB0565(a, r, g, b)
	#define MAKE_RGB MAKE_RGB16
	#define COLOR_FORMAT_BPP 16

#endif

#define  COLOR_KEY_A 0
#define  COLOR_KEY_R 255
#define  COLOR_KEY_G 0
#define  COLOR_KEY_B 255

#ifdef CASABLANCA
  #ifdef COLOR_FORMAT_32
    #define  RESERVED_COLOR_KEY  MAKE_ARGB(COLOR_KEY_A, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)
    #define  RESERVED_COLOR_RGB      Color(COLOR_KEY_A, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)
  #else
    #define  RESERVED_COLOR_KEY  MAKE_RGB(COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)
    #define  RESERVED_COLOR_RGB     Color(COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)
  #endif
#else
  #ifdef COLOR_FORMAT_32
    #define  RESERVED_COLOR_KEY  MAKE_ARGB(COLOR_KEY_A, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)
    #define  RESERVED_COLOR_RGB      Color(COLOR_KEY_A, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)
  #else
    #define  RESERVED_COLOR_KEY  MAKE_RGB(0x08, 0x08, 0x10)
    #define  RESERVED_COLOR_RGB     Color(0x08, 0x08, 0x10)
  #endif
#endif

/**
 * @enum ALPHAMODE
 */
typedef enum _ALPHAMODE
{
    Alpha_None,							///< none Alpha mode
    Alpha_Constant,					///< constant Alpha mode
    Alpha_SrcMajor,					///< source Alpha major mode
    Alpha_DecMajor,					///< destination Alpha major mode
    Alpha_Const_Src,
    Alpha_Const_SrcInverse,
#if IS_CHIP(SATURN)
	/* Add new alpha mode to match dfb.*/
	Alpha_Clear,
	Alpha_Src,
	Alpha_Src_Over,
	Alpha_Dst_Over,
	Alpha_Src_In,
	Alpha_Dst_In,
	Alpha_Src_Out,
	Alpha_Dst_Out,
	Alpha_Src_Atop,
	Alpha_Dst_Atop,
	Alpha_Add,
	Alpha_Xor,
	Alpha_Dst,
	Alpha_Disable,
#endif

} ALPHA_MODE;

typedef enum _ALPHA_SEL
{
    Alpha_Sel_Const,
    Alpha_Sel_ConstInverse,
    Alpha_Sel_Des,
    Alpha_Sel_DesInverse,
    Alpha_Sel_Src,
    Alpha_Sel_SrcInverse,
    Alpha_Sel_Const_Value_0,
    Alpha_Sel_Const_Value_1,
} ALPHA_SEL;

/**
 * @enum COLORKEY_MODE
 */
typedef enum _COLORKEYMODE
{
    ColorKey_Src,	///< source color key
    ColorKey_Des,	///< destination color key
    ColorKey_None,	///<none color key
    ColorKey_Src_Des,
} COLORKEY_MODE;

typedef enum _START_CONER
{
    CONER_TOP_LEFT = 0x0,
    CONER_TOP_RIGHT = 0x1,
    CONER_BOTTOM_LEFT = 0x2,
    CONER_BOTTOM_RIGHT = 0x3
} START_CORNER;

typedef enum _ROTATEMODE
{
	ROTATE_90,
	ROTATE_180,
	ROTATE_270,
	ROTATE_DISABLE,
} ROTATE_MODE;

//definitions for handling DFB specific memory request
#define BIT_FREE_SURFMEM	 	(1<<0)
#define BIT_FREE_HANDLE 		(1<<31)


/**
 * @name
 * @par CommonRasterOperationCodes
 * The following list shows some common raster operation codes:
 */
//@{
#define BLACKNESS   1		///< Fills the destination rectangle using the color associated with index 0 in the physical palette. (This color is black for the default physical palette.)
#define DSTINVERT   2		///< Inverts the destination rectangle.
#define MERGECOPY   3		///< Merges the colors of the source rectangle with the specified pattern by using the Boolean AND operator.
#define MERGEPAINT  4		///< Merges the colors of the inverted source rectangle with the colors of the destination rectangle by using the Boolean OR operator.
#define NOTSRCCOPY  5		///< Copies the inverted source rectangle to the destination.
#define NOTSRCERASE 6		///< Combines the colors of the source and destination rectangles by using the Boolean OR operator and then inverts the resultant color.
#define PATCOPY     7		///< Copies the specified pattern into the destination bitmap.
#define PATINVERT   8		///< Combines the colors of the specified pattern with the colors of the destination rectangle by using the Boolean XOR operator.
#define PATPAINT    9		///< Combines the colors of the pattern with the colors of the inverted source rectangle by using the Boolean OR operator. The result of this operation is combined with the colors of the destination rectangle by using the Boolean OR operator.
#define SRCAND      10	///< Combines the colors of the source and destination rectangles by using the Boolean AND operator.
#define SRCCOPY     11	///< Copies the source rectangle directly to the destination rectangle.
#define SRCERASE    12	///< Combines the inverted colors of the destination rectangle with the colors of the source rectangle by using the Boolean AND operator.
#define SRCINVERT   13	///< Combines the colors of the source and destination rectangles by using the Boolean XOR operator.
#define SRCPAINT    14	///< Combines the colors of the source and destination rectangles by using the Boolean OR operator.
#define WHITENESS   15	///< Fills the destination rectangle using the color associated with index 1 in the physical palette. This color is white for the default physical palette.
//@}

/**
 * @struct _SURFACEDESC
 * Surface Descriptor
 */
typedef struct _SURFACEDESC {
    uint32_t            dwSize;							///< Size of the structure, in bytes. This member must be initialized before the structure is used.
    uint32_t            dwFlags;						///< control flags of surface description
    uint32_t            dwHeight;						///< Height of the surface to be created, in pixels.
    uint32_t            dwWidth;						///< Width of the surface to be created, in pixels.
    uint32_t            lPitch;							///< Distance, in bytes, to the start of next line.
    uint32_t            dwColorKey;
    uint32_t            dwBackBufferCount;	///< Not used.
    uint32_t            dwRefreshRate;			///< Refresh rate (used when the display mode is described). The value of 0 indicates an adapter default.
    uint32_t            dwAlphaBitDepth;		///< bit depth of Alpha
    uint32_t            PhyAddr;
    void                *lpSurface;					///< Address of the associated surface memory. It has to be 4 page boundary. The size of one memory page depends on SDRAM type. Use getSdramPageSize() to get the size;
    void                *lpSurfaceCached;
    PIXEL_FORMAT        pixelFormat;				///< 8 bits, 16 bits or 32 bits. see PIXEL_FORMAT
    int                 storageMode;            //0 :  block mode, 1 : sequential mode, 2 : de/compressed mode
	MYRECT				srcClipRect;			//turn on clip rectangle to constraint area while DG APIs used
	MYRECT				dstClipRect;			//turn on clip rectangle to constraint area while DG APIs used
	MYSHIFT	    		srcShift;
	MYSHIFT    			dstShift;
    int                srcClipOn;
    int                dstClipOn;
    int                srcShiftOn;
    int                dstShiftOn;
#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
    uint32_t           dwCompressedBitstreamBufAddr[4];
    uint32_t           dwCompressedBitstreamLen[4];
#endif
    bool                bBigEndian;
    //bool               bUseGraphicRegion;
	uint32_t 			type;					//+JT - 20101004 -'DFBS' for directfb surface, others are DG native surface
} SURFACEDESC;

//=================================================================================================
typedef enum _SRC_CLR_TRANSFORM_MODE // Saturn Sepc, page 33, Command Bitblit, Word4, field: src_clr_transform
{
    SRC_CLR_TRANSFORM_DISABLE = 0,               //Disable source color transform
    SRC_CLR_TRANSFORM_ENABLE_FOR_ALPHA_ONLY = 1, //Enable source color transform for alpha only
    SRC_CLR_TRANSFORM_ENABLE_FOR_ALL = 2         //Enable source color transform for all components(RGBA)
} SOURCE_COLOR_TRANSFORM_MODE;

//=================================================================================================
typedef enum _DFB_CLR_BLEND_MODE // Saturn Sepc, page 33, Command Bitblit, Word4, field: fb_sel_srcclr_blend & fb_sel_destclr_blend
{
    DFB_BLEND_CONST_0 = 0,                       //Constant Value 0
    DFB_BLEND_CONST_1 = 1,                       //Constant Value 1
    DFB_BLEND_ALPHA_DEST = 2,                    //Select dest alpha (i.e. dest(A) ¡V destination pixel by pixel alpha)   SEINFO_DFB_BLEND_ALPHA_1_DEST = 3,                  //Select (1-dest(A))
    DFB_BLEND_ALPHA_SRC = 4,                     //Select pixel by pixel src alpha (i.e. src(A))
    DFB_BLEND_ALPHA_1_SRC = 5,                   //Select (1-src(A))
    DFB_BLEND_RGB_SRC = 6,                       //Select source color src(RGB)
    DFB_BLEND_RGB_1_SRC = 7,                     //Select (1-src(RGB))
    DFB_BLEND_RGB_DEST = 8,                      //Select destination color dest(RGB)
    DFB_BLEND_RGB_1_DEST = 9,                    //Select (1-dest(RGB))
    DFB_BLEND_MIN_ALPHA_SRC_1_DEST = 10,         //Select min(src(A), 1-dest(A))
 } DFB_CLR_BLEND_MODE;

typedef struct COLOR64 {
    uint16_t alpha;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} COLOR64;

typedef enum _SYNC_VO_MODE {
    SYNC_VO_DISABLE=0,
    SYNC_VO_ENABLE=1,
} SYNC_VO_MODE;

//=================================================================================================
typedef enum _MIRRORMODE
{
    MIRROR_X = 1,                                //The Definition of Mirror Mode: X Only
    MIRROR_Y = 2,                                //The Definition of Mirror Mode: Y Only
    MIRROR_XY = 3                                //The Definition of Mirror Mode: X and Y
} MIRRORMODE;

typedef struct _dfbdesc {
    VOUT_GRAPHIC_PLANE    vout_plane;
    RECTANGLE             window_rect;
    VOUT_WINDOW_STACKING  window_stack;
    int                   alpha_mode;
} DFBDESC;


int
DG_PixelEndianSwap(PIXEL_FORMAT pixelFormat, int color);

bool
DG_IsGraphicPlaneInUse(VOUT_GRAPHIC_PLANE plane);

HRESULT
DG_ShowGraphicPlane(VOUT_GRAPHIC_PLANE plane);

HRESULT
DG_HideGraphicPlane(VOUT_GRAPHIC_PLANE plane);

/**
 * @ingroup DisplayInterface
 * @brief  method to set display area
 *
 * @param[in]		displayHandle	Specifies the handle to display area
 * @param[in]		nXDest 		Specifies the logical x-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nYDest 		Specifies the logical y-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nWidth 		Specifies the logical width of the source and destination rectangles.
 * @param[in]		nHeight 	Specifies the logical height of the source and the destination rectangles.
 * @param[in]		handle		Handle to the working buffer, which is the source.
 * @param[in]		nXSrc 		Specifies the logical x-coordinate of the upper-left corner of the source rectangle.
 * @param[in]		nYSrc 		Specifies the logical y-coordinate of the upper-left corner of the source rectangle.
 * @param[in]		alphaMode Only Alpha_None, Alpha_Constant and Alpha_SrcMajor are supported.
 * \n				              Choose none if no alpha blending is needed.
 * \n                     "If choose constant alpha. Set the alpha value in srcA."
 * \n                     "   out = src *srcA + des * (1- srcA)  // Same operation apply to all the channels (RGBA)"
 * \n                     "If choose source alpha as major, and operate on per pixel alpha. This operation only applicable on 32 bits format."
 * \n                     "   out = src *srcA_pixel + des * (1-srcA_pixel) // Same operation apply to all the channels (RGBA)"
 * @param[in]		srcA					source alpha value.
 * @param[in]		colorKeyMode	Only ColorKey_Src and CloroKey_None are supported.
 * @param[in]		colorKey			Color key value. Depends on color format, alpha channel in 32 bits mode will be ignored when doing the key comparison.
 * @see ALPHA_MODE, COLORKEY_MODE
 * @retval S_OK
 * @retval S_FALSE
 *
 */
HRESULT
DG_DisplayArea   (  HANDLE handleDisplay,
				    int nXDest,
                    int nYDest,
                    int nWidth,
                    int nHeight,
                    SURFACEHANDLE handle,
                    int nXSrc,
                    int nYSrc,
                    ALPHA_MODE  alphaMode,
                    uint8_t srcA,
                    COLORKEY_MODE  colorKeyMode,
                    unsigned long colorKey);

HRESULT
DG_DisplayAreaBatch( HANDLE handleDisplay,
				     int nXDest,
                     int nYDest,
                     int nWidth,
                     int nHeight,
                     SURFACEHANDLE handle,
                     int nXSrc,
                     int nYSrc,
                     ALPHA_MODE  alphaMode,
                     uint8_t srcA,
                     COLORKEY_MODE  colorKeyMode,
                     unsigned long colorKey);

HANDLE
DG_GetGraphicDisplayHandle(VOUT_GRAPHIC_PLANE plane); //0 : OSD, 1 : SUB1, 2 : SUB2

/**
 * @ingroup DisplayInterface
 * @brief  method to query display handle
 *
 * @return display handle, if return value is NULL, it means no more display hardware available.
 *
 * @par Remarks
 * You need to get a valid handle before subsequent call to display functions. You can use the same handle for multiple display function calling.
 *
 */
HANDLE
DG_GetDisplayHandle(void);
HANDLE
DG_GetDisplaySurfaceInfo(HANDLE handleDisplay);

/**
 * @ingroup DisplayInterface
 * @brief  method to release display handle
 *
 * @param[in]		displayHandle	Specifies the handle to display area
 * @retval S_OK
 * @retval S_FALSE
 *
 */
HRESULT
DG_ReleaseDisplayHandle (HANDLE handleDisplay);

HRESULT
DG_ReleaseDisplayHandleBatch (HANDLE handleDisplay);

HRESULT
DG_SetCLUT(SURFACEHANDLE hSurface, uint32_t *pCLUT, int numEntries);

HRESULT
DG_BindPalette(HANDLE hDisplay, HANDLE hPalette);

HRESULT
DG_FreePaletteHandle(HANDLE hPalette);

HRESULT
DG_SetPaletteContent(HANDLE hPalette, char *pal_table, int entry_num);

HANDLE
DG_GetPaletteHandle(void);

HANDLE
DG_CreateCmdQueue(void);

void
DG_DrawEllipse(SURFACEHANDLE handle, int xc, int yc, int rx, int ry, COLORDEF color);

void
DrawHollowRectangle(SURFACEHANDLE hSurface, int x, int y, int w, int h, int thick, COLORDEF color);

HRESULT
DG_DrawIntersectRectangle( SURFACEHANDLE handleDes,
				  MYRECT *p_src_rect,
				  MYRECT *p_clip_rect,
                  int srcColor);

int
DG_PixelConvert(PIXEL_FORMAT srcFormat,
				  int srcColor,
				  PIXEL_FORMAT dstFormat);

/**
 * @ingroup DirectGraphicsInterface
 * @brief  method to draw pixels on the surface
 *
 * @param[in]		handleDes	Handle to the destination drawing surface.
 * @param[in]		nXDest 		Specifies the logical x-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nYDest 		Specifies the logical y-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		srcColor 	The srcColor represents the native color value of the source constant color. Function will take lower 8 bits of srcColor if destination surface is a 8 bits color plane. Same concept apply to other color depth destination surface with the lower n bits as the source constant color value.
 * @retval S_OK
 * @retval S_FALSE
 *
 * @par Remarks
 * The DG_DrawPixel will queue the drawing command into the hardware queue. Depends on the queue size, the DG_DrawPixel might not be the best way to draw thousands of pixels. For large bit map drawing, it is prefer to call DG_Lock, then directly access the surface using direct memory access.
 *
 */
HRESULT
DG_DrawPixel ( SURFACEHANDLE handleDes,
                int nXDest,
                int nYDest,
                int srcColor );

/**
 * @ingroup DirectGraphicsInterface
 * @brief  method to unlock the surface
 *
 * @param[in]		handleDes	Handle to the destination drawing surface.
 * @retval S_OK
 * @retval S_FALSE
 *
 * @par Remarks
 * Unlock the surface. A surface can be locked once before calling the unlock.
 *
 */
HRESULT
DG_Unlock(SURFACEHANDLE  handleDes);

/**
 * @ingroup DirectGraphicsInterface
 * @brief  method to lock the surface for direct memory access
 *
 * @param[in]		handleDes	Handle to the destination drawing surface.
 * @param[in]		nXDest 		Specifies the logical x-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nYDest 		Specifies the logical y-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nWidth 		Specifies the logical width of the source and destination rectangles.
 * @param[in]		nHeight 	Specifies the logical height of the source and the destination rectangles.
 * @param[in]		dwFlags		Reserved
 * @retval S_OK
 * @retval S_FALSE
 *
 * @par Remarks
 * Lock the surface for direct memory access. A surface can be locked once before calling the unlock. The Lock function will guarantee the completion of hardware acceleration before the return of the Lock function. All functions prefix with DG_ are hardware accelerated functions, which are executed asynchronously to the main program. In order to do direct access to the surface, one must make sure the previously issued DG functions have been executed, to prevent overwritten by DG function again.
 *
 */
HRESULT
DG_Lock(SURFACEHANDLE handleDes,
        int nXDest,
        int nYDest,
        int nWidth,
        int nHeight,
        int dwFlags );

HRESULT
DG_WaitComplete(SURFACEHANDLE handleDes);

HRESULT
DG_SetAsDefaultGraphicRegion(SURFACEHANDLE handleDes);

//+JT
HRESULT DG_MemZoneLock(unsigned long phys_start, unsigned long length, uint32_t* pZoneIdx);
void DG_MemZoneUnlock(uint32_t zoneIdx);

HRESULT DG_switchSPDomination(int mode);
bool DG_querySPDomination(void);

//-JT

#ifdef __cplusplus
/**
 * @ingroup DirectGraphicsInterface
 * @brief  method to provid simple opaque rectangle drawing on the destination surface
 *
 * @param[in]		handleDes	Handle to the destination drawing surface.
 * @param[in]		nXDest 		Specifies the logical x-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nYDest 		Specifies the logical y-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nWidth 		Specifies the logical width of the source and destination rectangles.
 * @param[in]		nHeight 	Specifies the logical height of the source and the destination rectangles.
 * @param[in]		srcColor 	This field take effect when handleSrc equal to 0, otherwise this field will be ignored. The srcColor represents the native color value of the source constant color. Function will take lower 8 bits of srcColor if destination surface is a 8 bits color plane. Same concept apply to other color depth destination surface with the lower n bits as the source constant color value.
 * @retval S_OK
 * @retval S_FALSE
 *
 * @par Remarks
 * <b>Drawing Lines :</b> Use DG_DrawRectangle to draw line. While drawing horizontal line, set nHeight to 1. When drawing vertical lines, set nWidth to 1. Only horizontal line and vertical line are supported by the hardware. Use GDI functions to draw other slop lines.
 * @note
 * We use DG_AlphaBlt to implement the DrawRectangle by setting srcHandle to 0, and set the source color to srcColor. To enable alpha feature or color key feature, use DG_AlphaBlt directly.
 * @see DG_AlphaBlt
 *
 */
HRESULT
DG_DrawRectangle( SURFACEHANDLE handleDes,
                  int nXDest,
                  int nYDest,
                  int nWidth,
                  int nHeight,
                  int srcColor,
                  HANDLE hQueue = 0);

/**
 * @ingroup DirectGraphicsInterface
 * @brief  method bitblt
 *
 * @param[in]		handleDes	Handle to the destination drawing surface.
 * @param[in]		nXDest 		Specifies the logical x-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nYDest 		Specifies the logical y-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nWidth 		Specifies the logical width of the source and destination rectangles.
 * @param[in]		nHeight 	Specifies the logical height of the source and the destination rectangles.
 * @param[in]		handleSrc	Handle to the source drawing surface.
 * @param[in]		srcColor 	This field take effect when handleSrc equal to 0, otherwise this field will be ignored. The srcColor represents the native color value of the source constant color. Function will take lower 8 bits of srcColor if destination surface is a 8 bits color plane. Same concept apply to other color depth destination surface with the lower n bits as the source constant color value.
 * @param[in]		nXSrc 		Specifies the logical x-coordinate of the upper-left corner of the source rectangle.
 * @param[in]		nYSrc 		Specifies the logical y-coordinate of the upper-left corner of the source rectangle.
 * @param[in]		ropCode 	Specifies a raster-operation code. These codes define how the color data for the source rectangle is to be combined with the color data for the destination rectangle to achieve the final color.
 * 				@see CommonRasterOperationCodes
 * @retval S_OK
 * @retval S_FALSE
 *
 */
HRESULT
DG_BitBlt(  SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
            int srcColor,
            int nXSrc,
            int nYSrc,
            int ropCode,
            HANDLE hQueue = 0,
            START_CORNER corner = CONER_TOP_LEFT);

/**
	do se bitblit without tearing if operated surface is on VO displaying.
	See macro SE_VO_SYNC() before using.
**/
HRESULT
DG_BitBltEx(  SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
            int srcColor,
            int nXSrc,
            int nYSrc,
            int ropCode,
            HANDLE hQueue = 0,
            START_CORNER corner = CONER_TOP_LEFT,
            uint32_t vo_sync = 0);


/**
 * @ingroup DirectGraphicsInterface
 * @brief  method bitblt with Alpha
 *
 * @param[in]		handleDes	Handle to the destination drawing surface.
 * @param[in]		nXDest		Specifies the logical x-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nYDest 		Specifies the logical y-coordinate of the upper-left corner of the destination rectangle.
 * @param[in]		nWidth 		Specifies the logical width of the source and destination rectangles.
 * @param[in]		nHeight 	Specifies the logical height of the source and the destination rectangles.
 * @param[in]		handleSrc	Handle to the source drawing surface. If handleSrc equal to 0, the source is a constant color.
 * @param[in]		srcColor 	This field take effect when handleSrc equal to 0, otherwise this field will be ignored. The srcColor represents the native color value of the source constant color. Function will take lower 8 bits of srcColor if destination surface is a 8 bits color plane. Same concept apply to other color depth destination surface with the lower n bits as the source constant color value.
 * @param[in]		nXSrc 		Specifies the logical x-coordinate of the upper-left corner of the source rectangle.
 * @param[in]		nYSrc 		Specifies the logical y-coordinate of the upper-left corner of the source rectangle.
 * @param[in]		alphaMode	Choose none if no alpha blending is needed. If choose constant alpha. Set the alpha value in both srcA and desA. out = src *srcA + des * desA  // Same operation apply to all the channels (RGBA) If choose source alpha as major, and operate on per pixel alpha. This operation only applicable on 32 bits format. out = src *srcA_pixel + des * (1-srcA_pixel) // Same operation apply to all the channels (RGBA) If choose destination alpha as major, and operate on per pixel alpha. This operation only applicable on 32 bits format. out = src *(1-desA_pixel) + des * desA_pixel) // Same operation apply to all the channels (RGBA)
 * @param[in]		srcA 			source alpha value.
 * @param[in]		desA 			destination alpha value.
 * @param[in]		isForcedestAlpha	set to true if want to set the value of destination alpha channel after blt operation to become a constant value.
 * @param[in]		forcedDesA		This parameter contains the forced destination alpha channel value.
 * @param[in]		colorKeyMode	Choose between source color key, destination color key or no color key applied. If choose source color key, the source pixels that match the color key value will be transparent on the destination. On the other hand, if destination key is chose, the destination pixels that match the destination color key will be replaced by corresponding source pixels. When color key is enabled, color key operations suppress alpha operations.
 * @param[in]		colorKey			Color key value. Depends on color format, alpha channel in 32 bits mode will be ignored when doing the key comparison.
 * @see ALPHA_MODE, COLORKEY_MODE
 * @retval S_OK
 * @retval S_FALSE
 *
 */
HRESULT
DG_AlphaBlt(SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
			int srcColor,
            int nXSrc,
            int nYSrc,
            ALPHA_MODE  alphaMode,
            uint8_t srcA,
		    uint8_t desA,
            int isForceDestAlpha,
            uint8_t forcedDesA,
            COLORKEY_MODE  colorKeyMode,
            unsigned long colorKey,
            HANDLE hQueue = 0,
            START_CORNER corner = CONER_TOP_LEFT);

HRESULT
DG_AlphaBltEx(SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
			int srcColor,
            int nXSrc,
            int nYSrc,
            ALPHA_MODE  alphaMode,
            uint8_t srcA,
		    uint8_t desA,
            int isForceDestAlpha,
            uint8_t forcedDesA,
            COLORKEY_MODE  colorKeyMode,
            unsigned long colorKey,
            HANDLE hQueue = 0,
            START_CORNER corner = CONER_TOP_LEFT,
            void *pExParam = NULL);

/*****************************************************************************************************************************
DFBSurfaceBlittingFlags
DSBLIT_NOFX 	 			0x00000000 	 	uses none of the effects
DSBLIT_BLEND_ALPHACHANNEL 	0x00000001 	 	enables blending and uses alphachannel from source
DSBLIT_BLEND_COLORALPHA 	0x00000002 	 	enables blending and uses alpha value from color
DSBLIT_COLORIZE 	 		0x00000004 	 	modulates source color with the color's r/g/b values
DSBLIT_SRC_PREMULTIPLY 	 	0x00000020 	 	modulates the source color with the (modulated) source alpha
DSBLIT_DST_PREMULTIPLY 		0x00000040 	 	modulates the dest. color with the dest. alpha
DSBLIT_DEMULTIPLY 	 		0x00000080 	 	divides the color by the alpha before writing the data to the destination
DSBLIT_SRC_PREMULTCOLOR 	0x00000200 	 	modulates the source color with the color alpha
DSBLIT_XOR 	 				0x00000400 	 	bitwise xor the destination pixels with the source pixels after premultiplication
****************************************************************************************************************************/
HRESULT
DG_AlphaBltEx2(SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
			int srcColor,
            int nXSrc,
            int nYSrc,
            ALPHA_SEL srcAlphaSel,
            uint8_t srcA,
            ALPHA_SEL desAlphaSel,
		    uint8_t desA,
            int isForceDestAlpha,
            uint8_t forcedDesA,
            COLORKEY_MODE  colorKeyMode,
            unsigned long colorKey,
            HANDLE hQueue = 0,
            START_CORNER corner = CONER_TOP_LEFT,
            SYNC_VO_MODE syncMode = SYNC_VO_DISABLE,
            SOURCE_COLOR_TRANSFORM_MODE srcColorTransformMode = SRC_CLR_TRANSFORM_DISABLE,
            uint64_t add = 0,
            uint64_t mul = 0,
            bool enableSrcColorTransparency = false,
            int DFBSurfaceBlittingFlags = 0,
            DFB_CLR_BLEND_MODE srcColorBlendMode = DFB_BLEND_ALPHA_SRC,
            DFB_CLR_BLEND_MODE dstColorBlendMode = DFB_BLEND_ALPHA_1_SRC);

HRESULT
DG_AlphaBltEx3(SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
			int srcColor,
            int nXSrc,
            int nYSrc,
            ALPHA_MODE  alphaMode,
            uint8_t srcA,
		    uint8_t desA,
            int isForceDestAlpha,
            uint8_t forcedDesA,
            COLORKEY_MODE  colorKeyMode,
            unsigned long colorKey,
			unsigned long dst_colorkey,
            HANDLE hQueue = 0,
            START_CORNER corner = CONER_TOP_LEFT,
            void *pExParam = NULL);

#else
HRESULT
DG_DrawRectangle( SURFACEHANDLE handleDes,
                  int nXDest,
                  int nYDest,
                  int nWidth,
                  int nHeight,
                  int srcColor,
                  HANDLE hQueue);

HRESULT
DG_BitBlt(  SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
            int srcColor,
            int nXSrc,
            int nYSrc,
            int ropCode,
            HANDLE hQueue,
            START_CORNER corner);

HRESULT
DG_BitBltEx(  SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
            int srcColor,
            int nXSrc,
            int nYSrc,
            int ropCode,
            HANDLE hQueue,
            START_CORNER corner,
            uint32_t vo_sync);


HRESULT
DG_AlphaBlt(SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
			int srcColor,
            int nXSrc,
            int nYSrc,
            ALPHA_MODE  alphaMode,
            uint8_t srcA,
		    uint8_t desA,
            int isForceDestAlpha,
            uint8_t forcedDesA,
            COLORKEY_MODE  colorKeyMode,
            unsigned long colorKey,
            HANDLE hQueue,
            START_CORNER corner);

HRESULT
DG_AlphaBltEx(SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
			int srcColor,
            int nXSrc,
            int nYSrc,
            ALPHA_MODE  alphaMode,
            uint8_t srcA,
		    uint8_t desA,
            int isForceDestAlpha,
            uint8_t forcedDesA,
            COLORKEY_MODE  colorKeyMode,
            unsigned long colorKey,
            HANDLE hQueue,
            START_CORNER corner,
            void *pExParam);

HRESULT
DG_AlphaBltEx2(SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
			int srcColor,
            int nXSrc,
            int nYSrc,
            ALPHA_SEL srcAlphaSel,
            uint8_t srcA,
            ALPHA_SEL desAlphaSel,
		    uint8_t desA,
            int isForceDestAlpha,
            uint8_t forcedDesA,
            COLORKEY_MODE  colorKeyMode,
            unsigned long colorKey,
            HANDLE hQueue,
            START_CORNER corner,
            SYNC_VO_MODE syncMode,
            SOURCE_COLOR_TRANSFORM_MODE srcColorTransformMode,
            uint64_t add,
            uint64_t mul,
            bool enableSrcColorTransparency,
            int DFBSurfaceBlittingFlags,
            DFB_CLR_BLEND_MODE srcColorBlendMode,
            DFB_CLR_BLEND_MODE dstColorBlendMode);

HRESULT
DG_AlphaBltEx3(SURFACEHANDLE handleDes,
            int nXDest,
            int nYDest,
            int nWidth,
            int nHeight,
            SURFACEHANDLE handleSrc,
			int srcColor,
            int nXSrc,
            int nYSrc,
            ALPHA_MODE  alphaMode,
            uint8_t srcA,
		    uint8_t desA,
            int isForceDestAlpha,
            uint8_t forcedDesA,
            COLORKEY_MODE  colorKeyMode,
            unsigned long colorKey,
			unsigned long dst_colorkey,
            HANDLE hQueue,
            START_CORNER corner,
            void *pExParam);

#endif

HRESULT
DG_Rotate(SURFACEHANDLE handleDst,
		uint32_t nXDest,
		uint32_t nYDest,
		SURFACEHANDLE handleSrc,
		uint32_t nXSrc,
		uint32_t nYSrc,
		uint32_t nWidth,
		uint32_t nHeight,
		ROTATE_MODE rotate_mode,
		HANDLE hQueue);

HRESULT
DG_SetColorKey(SURFACEHANDLE handle, unsigned long dwColorKey);

HRESULT
DG_GetColorKey(SURFACEHANDLE handle, unsigned long *lpColorKey);

int
DG_GetSerialNumber(SURFACEHANDLE handle);

//HRESULT
//DG_GetDC(SURFACEHANDLE handle, HDC *phdc);
//
//HRESULT
//DG_ReleaseDC(SURFACEHANDLE handle, HDC hdc);
//
/**
 * @ingroup DirectGraphicsInterface
 * @brief  method retrieves the descriptor of drawing surface.
 *
 * @param[in]	handle	Handle to the drawing surface.
 * @param[out]	pDesc	Pointer to a SURFACEDESC structure that will retrieve the information for the drawing object.
 * @retval S_OK
 * @retval S_FALSE
 *
 */
HRESULT
DG_GetSurfaceDesc(SURFACEHANDLE handle, SURFACEDESC *pDesc);
HRESULT
DG_GetSurfaceRowDataSize(SURFACEHANDLE handle, int *iRet);
void*
DG_GetSurfaceRowDataPointer(SURFACEHANDLE handle);


//==============================================================================
// Only for AbstractAP::m_hSurface on DirecrFB.
//==============================================================================
HRESULT
DG_DirectFB_DumpMainWindowSurface(void);

HRESULT
DG_DirectFB_CloseMainWindowSurface(void);

HRESULT
DG_DirectFB_SetMainLayerColorkey(bool bEnable);

HRESULT
DG_DirectFB_SetMainLayerCursor(bool bEnable);

HRESULT
DG_DirectFB_SetMainLayerOpacity(bool bEnable);

#ifdef __cplusplus
HRESULT
DG_DirectFB_ShowMainWindow(RECTANGLE *rect=NULL);
#else
HRESULT
DG_DirectFB_ShowMainWindow(RECTANGLE *rect);
#endif

HRESULT
DG_DirectFB_HideMainWindow(void);

SURFACEHANDLE
DG_DirectFB_CreateMainWindowSurface(SURFACEDESC *pDesc, DFBDESC *pDfbDesc);
//==============================================================================


//==============================================================================
// For All DirecrFB Display Windows except AbstractAP::m_hSurface
//==============================================================================
HRESULT
DG_DirectFB_CloseWindowSurface(int index);

HRESULT
DG_DirectFB_MoveWindowTo(int index, int x, int y);

#ifdef __cplusplus
HRESULT
DG_DirectFB_ShowWindow(int index, RECTANGLE *rect=NULL);
#else
HRESULT
DG_DirectFB_ShowWindow(int index, RECTANGLE *rect);
#endif

SURFACEHANDLE
DG_DirectFB_CreateWindowSurface(int *pIndex, SURFACEDESC *pDesc, DFBDESC *pDfbDesc);
//==============================================================================


//==============================================================================
// Only for layer handle controlling on DirecrFB.
//==============================================================================
void
DG_DirectFB_ShowDisplay(VOUT_GRAPHIC_PLANE plane);

void
DG_DirectFB_HideDisplay(VOUT_GRAPHIC_PLANE plane);
//==============================================================================

void
DG_DirectFB_setMemMaxLimit(unsigned long size);


HRESULT
DG_CloseSurface(SURFACEHANDLE handle);

SURFACEHANDLE
DG_CreateSurfaceEx(SURFACEDESC *pDesc,bool bAllocMem, bool bUseGraphicRegion);

SURFACEHANDLE
DG_CreateSurfaceEx1(SURFACEDESC *pDesc,bool bAllocMem, bool bUseGraphicRegion, bool bAllocFromCache);

SURFACEHANDLE
DG_CreateSurfaceEx2(SURFACEDESC *pDesc);

SURFACEHANDLE
DG_CreateSurfaceEx3(SURFACEDESC *pDesc, bool bAllocMem, bool bUseGraphicRegion, bool bAllocFromCache, bool bBigEndian);

/**
 * @ingroup DirectGraphicsInterface
 * @brief  method creates a drawing surface.
 *
 * @param[in]	pDesc		Pointer to a SURFACEDESC structure that contains the information necessary to create the drawing object.
 * @return The handle to the created surface.
 *
 * @par Remarks
 * The lPitch and lpSurface members are output values when calling the DG_GetSurfaceDesc method. When creating surfaces from existing memory or updating surface characteristics, these members are input values that describe the pitch and location of memory allocated by the calling application for use by 2D graphics library. The library does not attempt to manage or free memory allocated by the application.
 */
SURFACEHANDLE
DG_CreateSurface(SURFACEDESC *pDesc);


SURFACEHANDLE
DG_ModifySurface(SURFACEHANDLE hSurface, SURFACEDESC *pDesc);
int
DG_GetGraphicCacheMemoryMaxSize();

HRESULT
DG_DrawText( SURFACEHANDLE handleDes,
            	  int nXDest,
            	  int nYDest,
            	  int nWidth,
            	  int nHeight,
             	  int color_fg,
             	  int color_bg,
		          char *string,
				  int size,
		          char charset,
		          int FLAG );

/* In-core color palette structure*/
typedef struct {
    uint32_t c1 : 8;
    uint32_t c2 : 8;
	uint32_t c3 : 8;
    uint32_t a  : 8;
} PALENTRY;

typedef struct {
    bool    use_header_wh;  // 28
    int     dst_height;     // 27:16
    int     dst_width;      // 15:4
    int     effect_type;    // 3:0
} EFFECTDRAWHDR, *PEFFECTDRAWHDR;    //  ErinC // bitmap header for drawing effect

typedef enum {
    IMG_TYPE_BMP = 0,
    IMG_TYPE_PNG = 1,
} ENUM_IMG_TYPE;

typedef struct {
    int     width;
    int     height;
    int     bpp;
    int     BPP;
    int     pitch;
    int     fd;
    int     imagebits_pos[4];
    char    *filename;
    int     ori_width;
    int     ori_height;
    SURFACEHANDLE hSurface;
    //int cache_start_x;
    //int cache_start_y;
    EFFECTDRAWHDR effect_draw;  // ErinC
    /* realtek only */
    int    isRtkProprietary;
    ENUM_IMG_TYPE img_type;
} DGIMAGEHDR, *PDGIMAGEHDR;


HRESULT
DG_GetBmpPalette(char *filename, PALENTRY *palette);

HRESULT
DG_GetBmpHeader(char *filename, PDGIMAGEHDR *ppimage);

HRESULT
DG_FreeBmpHeader(PDGIMAGEHDR pimage);

HRESULT
DG_DrawBitmap(SURFACEHANDLE handleDes, uint32_t x, uint32_t y, PDGIMAGEHDR pimage);

HRESULT
DG_DrawBitmapFile(SURFACEHANDLE handleDes, uint32_t x, uint32_t y, char *filename);

HRESULT
DG_DrawBitmapEx(SURFACEHANDLE hSurface,
               uint32_t x, uint32_t y, uint32_t w, uint32_t h,
               PDGIMAGEHDR pImageHdr,
               COLORKEY_MODE colorKeyMode, uint32_t colorKey);
HRESULT
DG_DrawBitmapWithEffect(SURFACEHANDLE hSurface,
                        PDGIMAGEHDR pImageHdr,                              //  img header
                        COLORKEY_MODE colorKeyMode, uint32_t colorKey,
                        int effectType,int dst_w,int dst_h,int bgColor);
HRESULT
DG_DrawBitmapScale(SURFACEHANDLE hSurface,
               uint32_t x, uint32_t y, uint32_t w, uint32_t h,
               PDGIMAGEHDR pImageHdr,
               COLORKEY_MODE colorKeyMode, uint32_t colorKey,uint32_t dstW, uint32_t dstH);

HRESULT
DG_DrawAlphaBitmap(SURFACEHANDLE hSurface,
                   uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                   PDGIMAGEHDR pImageHdr,
                   ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA);

HRESULT
DG_DrawBitmapFile(SURFACEHANDLE handleDes, uint32_t x, uint32_t y, char *filename);

HRESULT
DG_DrawBitmapEx1(SURFACEHANDLE hSurface,
                 uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                 PDGIMAGEHDR pImageHdr,
                 uint32_t sx, uint32_t sy, uint32_t sw, uint32_t sh,
                 COLORKEY_MODE colorKeyMode, uint32_t colorKey,
                 ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA);

void
DG_FreeDrawBitmapBuffer(void);

HRESULT
DG_CommitOsdWin(VOUT_GRAPHIC_PLANE plane);

#if AP_CONFIG_WITH(APCFG_USE_PNG_IMAGE)
#define _FAST_PNG

HRESULT
DG_GetPngHeader(char *filename, PDGIMAGEHDR *ppimage);

HRESULT
DG_GetPngHeader_int(char *filename, PDGIMAGEHDR *ppimage, bool *pbInterlaced);

HRESULT
DG_FreePngHeader(PDGIMAGEHDR pimage);
HRESULT
DG_DrawPng(SURFACEHANDLE handleDes, uint32_t x, uint32_t y, PDGIMAGEHDR pimage, bool *pbIsCancel);

HRESULT
DG_DrawPngFile(SURFACEHANDLE handleDes, uint32_t x, uint32_t y, char *filename, bool *pbIsCancel);

HRESULT
DG_DrawPngFileEx1(SURFACEHANDLE handleDes, char *filename,
                 uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                 bool bCalNewSize,
                 // in percentage of image width & height
                 uint32_t sx_per, uint32_t sy_per, uint32_t sw_per, uint32_t sh_per,
                 COLORKEY_MODE colorKeyMode, uint32_t colorKey,
                 ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
                 bool *pbIsCancel);
//+JT - 20110208
HRESULT
DG_DrawFastPngDataEx(SURFACEHANDLE handleDes, unsigned char *pBuffer, unsigned int size,
                 uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                 bool bCalNewSize,
                 // in percentage of image width & height
                 uint32_t sx_per, uint32_t sy_per, uint32_t sw_per, uint32_t sh_per,
                 COLORKEY_MODE colorKeyMode, uint32_t colorKey,
                 ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
                 bool *pbIsCancel);
//-JT

HRESULT
DG_DrawPngFileEx1_resol( SURFACEHANDLE handleDes, char *filename,
                        uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                        bool bCalNewSize,
                        uint32_t sx_per, uint32_t sy_per, uint32_t sw_per, uint32_t sh_per,
                        COLORKEY_MODE colorKeyMode, uint32_t colorKey,
                        ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
                        int *pImg_width, int *pImg_height,
                        bool *pbIsCancel);

HRESULT
DG_DrawPngFileEx1_resol_swap( SURFACEHANDLE handleDes, char *filename,
	                        uint32_t x, uint32_t y, uint32_t w, uint32_t h,
	                        bool bCalNewSize,
	                        uint32_t sx_per, uint32_t sy_per, uint32_t sw_per, uint32_t sh_per,
	                        COLORKEY_MODE colorKeyMode, uint32_t colorKey,
	                        ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
	                        int *pImg_width, int *pImg_height,
	                        bool *pbIsCancel,
	                        bool *pbSwap);

HRESULT
DG_DrawPngEx(SURFACEHANDLE hSurface,
               uint32_t x, uint32_t y, uint32_t w, uint32_t h,
               PDGIMAGEHDR pImageHdr,
               COLORKEY_MODE colorKeyMode, uint32_t colorKey,
               bool *pbIsCancel);
HRESULT
DG_DrawPngScale(SURFACEHANDLE hSurface,
               uint32_t x, uint32_t y, uint32_t w, uint32_t h,
               PDGIMAGEHDR pImageHdr,
               COLORKEY_MODE colorKeyMode, uint32_t colorKey,uint32_t dstW, uint32_t dstH,
               bool *pbIsCancel);

HRESULT
DG_DrawAlphaPng(SURFACEHANDLE hSurface,
                   uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                   PDGIMAGEHDR pImageHdr,
                   ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
                   bool *pbIsCancel);

HRESULT
DG_DrawPngEx1(SURFACEHANDLE hSurface,
                 uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                 PDGIMAGEHDR pImageHdr,
                 uint32_t sx, uint32_t sy, uint32_t sw, uint32_t sh,
                 COLORKEY_MODE colorKeyMode, uint32_t colorKey,
                 ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
                 bool *pbIsCancel);

HRESULT
DG_GetFastPngDec(HANDLE *phFastPngDec);

HRESULT
DG_FreeFastPngDec(HANDLE hFastPngDec);

HRESULT
DG_GetFastPngHeader(char *filename, PDGIMAGEHDR *ppimage, HANDLE hFastPngDec);

HRESULT
DG_GetFastPngMemHeader(unsigned char *pBuffer, unsigned int size, PDGIMAGEHDR *ppimage, HANDLE hFastPngDec);

HRESULT
DG_FreeFastPngHeader(PDGIMAGEHDR pimage, HANDLE hFastPngDec);

#ifdef _FAST_PNG
HRESULT
DG_DrawFastPngFileEx1(SURFACEHANDLE handleDes, char *filename,
                 uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                 bool bCalNewSize,
                 // in percentage of image width & height
                 uint32_t sx_per, uint32_t sy_per, uint32_t sw_per, uint32_t sh_per,
                 COLORKEY_MODE colorKeyMode, uint32_t colorKey,
                 ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
                 bool *pbIsCancel);

HRESULT
DG_DrawFastPngFileEx1_resol( SURFACEHANDLE handleDes, char *filename,
                        uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                        bool bCalNewSize,
                        uint32_t sx_per, uint32_t sy_per, uint32_t sw_per, uint32_t sh_per,
                        COLORKEY_MODE colorKeyMode, uint32_t colorKey,
                        ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
                        int *pImg_width, int *pImg_height,
                        bool *pbIsCancel);

HRESULT
DG_DrawFastPngFileEx1_resol_swap( SURFACEHANDLE handleDes, char *filename,
	                        uint32_t x, uint32_t y, uint32_t w, uint32_t h,
	                        bool bCalNewSize,
	                        uint32_t sx_per, uint32_t sy_per, uint32_t sw_per, uint32_t sh_per,
	                        COLORKEY_MODE colorKeyMode, uint32_t colorKey,
	                        ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
	                        int *pImg_width, int *pImg_height,
	                        bool *pbIsCancel,
	                        bool *pbSwap);
#endif
HRESULT
DG_DrawFastPngCommon(SURFACEHANDLE hSurface,
                    uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                    bool bCalNewSize,
                    PDGIMAGEHDR pImageHdr,
                    uint32_t sx, uint32_t sy, uint32_t sw, uint32_t sh,
                    COLORKEY_MODE colorKeyMode, uint32_t colorKey, ALPHA_MODE alphaMode, uint8_t srcA, uint8_t dstA,
                    bool *pbIsCancel, HANDLE hFastPngDec, bool isSwapEndian);

#endif





//IsInRect(MYRECT *pRect, MYPOINT *pPt)
#define IsInsideRect(pRect, a, b) (((pRect)->x <= (a)) && \
                                    (((pRect)->x + (pRect)->w) > (a)) && \
								    ((pRect)->y <= (b)) && \
								    (((pRect)->y + (pRect)->h) > (b)))

MYRECT
GetIntersectRect(MYRECT *pOriRect, MYRECT *pClipRect);

HRESULT
DG_ConfigSurface(SURFACEHANDLE handle, ENUM_CONFIG_TARGET target, ENUM_CONFIG_FLAG flag, CONFIG_PARAM *configParam);

void DG_Init(void);
void DG_UnInit(void);

void DG_Warm_Init(void);
void DG_Warm_Uninit(void);

void DG_SetDirectFBZoneCount(int cnt);


#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
HANDLE
DG_RequestQueue(void);

HRESULT
DG_ReleaseQueue(HANDLE hQueue);

HRESULT
DG_WaitVSync(HANDLE hQueue, int count);

HRESULT
DG_WaitCmdDone(HANDLE hQueue);

///copy from block-memory to sequence-memory or sequence-memory to block-memory
HRESULT DG_CopyBlt( SURFACEHANDLE handleDest,
                    uint32_t nXDest,
                    uint32_t nYDest,
                    uint32_t nWidth,
                    uint32_t nHeight,
                    SURFACEHANDLE handleSrc,
                    uint32_t nXSrc,
                    uint32_t nYSrc);

HRESULT DG_SwapCopyBlt( SURFACEHANDLE handleDest,
                    uint32_t nXDest,
                    uint32_t nYDest,
                    uint32_t nWidth,
                    uint32_t nHeight,
                    SURFACEHANDLE handleSrc,
                    uint32_t nXSrc,
                    uint32_t nYSrc);

#ifdef __cplusplus
HRESULT
DG_Stretch(
            SURFACEHANDLE hSurfaceDes,
            int      iXDes,
            int      iYDes,
            int      iWDes,	// 0 means 4096
            int      iHDes, // 0 means 4096
            SURFACEHANDLE hSurfaceSrc,
            int      iXSrc,
            int      iYSrc,
            int      iWSrc,
            int      iHSrc,
            COLORKEY_MODE byColorKeySelect,
            unsigned long		iColorKey,
            bool          		bAlphaEnable,
            HANDLE 				hQueue = 0,
            STRETCH_COEFF_MODE	StretchMode = SCALING_MODE_AUTO,
            uint8_t				iVerticalScalingOffset = 0,
            int					iVerticalTapNumber = 2);

HRESULT
DG_FormatConv(  SURFACEHANDLE handleDes,
	            int nXDest,
	            int nYDest,
	            int nWidth,
	            int nHeight,
	            SURFACEHANDLE handleSrc,
	            int nXSrc,
	            int nYSrc,
	            bool bIsSrcColorKey,
	            bool bIsConstAlpha,
	            uint8_t desA,
		        HANDLE hQueue = 0,
		        uint32_t colorkey = 0);

/*
HRESULT
DG_StretchBlt(
            SURFACEHANDLE hSurfaceDes,
            int      iXDes,
            int      iYDes,
            int      iWDes, // 0 means 4096
            int      iHDes, // 0 means 4096
            SURFACEHANDLE hSurfaceSrc,
            int      iXSrc,
            int      iYSrc,
            int      iWSrc,
            int      iHSrc,
            COLORKEY_MODE byColorKeySelect,
            unsigned long       iColorKey,
            bool                bAlphaEnable,
            HANDLE              hQueue = 0,
            STRETCH_COEFF_MODE  StretchMode = SCALING_MODE_AUTO,
            uint8_t             iVerticalScalingOffset = 0,
            int                 iVerticalTapNumber = 2);
*/

HRESULT
DG_Mirror( SURFACEHANDLE handleDes,
                    uint32_t nXDest,
                    uint32_t nYDest,
                    uint32_t nWidth,
                    uint32_t nHeight,
                    SURFACEHANDLE handleSrc,
                    uint32_t nXSrc,
                    uint32_t nYSrc,
                    MIRRORMODE mirrorDirection,
                    HANDLE hQueue = 0);

#if IS_CHIP(SATURN)
// function to do Stretch + AlphaBlt:
HRESULT DG_AlphaStretchEx( SURFACEHANDLE hSurface,
						 int dstX, int dstY, int dstW, int dstH,
						 SURFACEHANDLE hImgSurface,
						 int srcX, int srcY, int srcW, int srcH,
						 SURFACEHANDLE hAlphaStretchSurface,	// tmp surface for doing stretch
						 int stretchX, int stretchY, int stretchW, int stretchH, // area of hAlphaStretchSurface
						 ALPHA_MODE alphaMode,
						 uint8_t srcA, uint8_t desA,
						 HANDLE hQueue = 0,
						 COLORKEY_MODE byColorKeySelect = ColorKey_Src,
						 unsigned long iColorKey = RESERVED_COLOR_KEY,
						 int isForceDestAlpha = true,
						 uint8_t forcedDesA = 0xFF,
						 STRETCH_COEFF_MODE	StretchMode = SCALING_MODE_AUTO,
			             uint8_t iVerticalScalingOffset = 0,
			             int iVerticalTapNumber = 2,
						 void *pExParam = NULL);

HRESULT DG_AlphaStretchEx2( SURFACEHANDLE hSurface,
						 int dstX, int dstY, int dstW, int dstH,
						 SURFACEHANDLE hImgSurface,
						 int srcX, int srcY, int srcW, int srcH,
						 SURFACEHANDLE hAlphaStretchSurface,	// tmp surface for doing stretch
						 int stretchX, int stretchY, int stretchW, int stretchH, // area of hAlphaStretchSurface
						 ALPHA_MODE alphaMode,
						 uint8_t srcA, uint8_t desA,
						 HANDLE hQueue = 0,
						 COLORKEY_MODE byColorKeySelect = ColorKey_Src,
						 unsigned long iColorKey = RESERVED_COLOR_KEY,
						 unsigned long dst_colorkey = RESERVED_COLOR_KEY,
						 int isForceDestAlpha = true,
						 uint8_t forcedDesA = 0xFF,
						 STRETCH_COEFF_MODE	StretchMode = SCALING_MODE_AUTO,
			             uint8_t iVerticalScalingOffset = 0,
			             int iVerticalTapNumber = 2,
						 void *pExParam = NULL);

#endif

// function to do Stretch + AlphaBlt:
HRESULT DG_AlphaStretch( SURFACEHANDLE hSurface,
						 int dstX, int dstY, int dstW, int dstH,
						 SURFACEHANDLE hImgSurface,
						 int srcX, int srcY, int srcW, int srcH,
						 SURFACEHANDLE hAlphaStretchSurface,	// tmp surface for doing stretch
						 int stretchX, int stretchY, int stretchW, int stretchH, // area of hAlphaStretchSurface
						 ALPHA_MODE alphaMode,
						 uint8_t srcA, uint8_t desA,
						 HANDLE hQueue = 0,
						 COLORKEY_MODE byColorKeySelect = ColorKey_Src,
						 unsigned long iColorKey = RESERVED_COLOR_KEY,
						 int isForceDestAlpha = true,
						 uint8_t forcedDesA = 0xFF,
						 STRETCH_COEFF_MODE	StretchMode = SCALING_MODE_AUTO,
			             uint8_t iVerticalScalingOffset = 0,
			             int iVerticalTapNumber = 2);

#else
HRESULT
DG_Stretch(
            SURFACEHANDLE hSurfaceDes,
            int      iXDes,
            int      iYDes,
            int      iWDes, // 0 means 4096
            int      iHDes, // 0 means 4096
            SURFACEHANDLE hSurfaceSrc,
            int      iXSrc,
            int      iYSrc,
            int      iWSrc,
            int      iHSrc,
            COLORKEY_MODE byColorKeySelect,
            unsigned long		iColorKey,
            bool          		bAlphaEnable,
            HANDLE 				hQueue,
            STRETCH_COEFF_MODE	StretchMode,
            uint8_t				iVerticalScalingOffset,
            int					iVerticalTapNumber);

HRESULT
DG_FormatConv(  SURFACEHANDLE handleDes,
	            int nXDest,
	            int nYDest,
	            int nWidth,
	            int nHeight,
	            SURFACEHANDLE handleSrc,
	            int nXSrc,
	            int nYSrc,
	            bool bIsSrcColorKey,
	            bool bIsConstAlpha,
	            uint8_t desA,
		        HANDLE hQueue,
		        uint32_t colorkey);
HRESULT
DG_Mirror( SURFACEHANDLE handleDes,
                    uint32_t nXDest,
                    uint32_t nYDest,
                    uint32_t nWidth,
                    uint32_t nHeight,
                    SURFACEHANDLE handleSrc,
                    uint32_t nXSrc,
                    uint32_t nYSrc,
                    MIRRORMODE mirrorDirection,
                    HANDLE hQueue);

#if IS_CHIP(SATURN)
// function to do Stretch + AlphaBlt:
HRESULT DG_AlphaStretchEx( SURFACEHANDLE hSurface,
						 int dstX, int dstY, int dstW, int dstH,
						 SURFACEHANDLE hImgSurface,
						 int srcX, int srcY, int srcW, int srcH,
						 SURFACEHANDLE hAlphaStretchSurface,	// tmp surface for doing stretch
						 int stretchX, int stretchY, int stretchW, int stretchH, // area of hAlphaStretchSurface
						 ALPHA_MODE alphaMode,
						 uint8_t srcA, uint8_t desA,
						 HANDLE hQueue,
						 COLORKEY_MODE byColorKeySelect,
						 unsigned long iColorKey,
						 int isForceDestAlpha,
						 uint8_t forcedDesA,
						 STRETCH_COEFF_MODE	StretchMode,
			             uint8_t iVerticalScalingOffset,
			             int iVerticalTapNumber,
						 void *pExParam);

HRESULT DG_AlphaStretchEx2( SURFACEHANDLE hSurface,
						 int dstX, int dstY, int dstW, int dstH,
						 SURFACEHANDLE hImgSurface,
						 int srcX, int srcY, int srcW, int srcH,
						 SURFACEHANDLE hAlphaStretchSurface,	// tmp surface for doing stretch
						 int stretchX, int stretchY, int stretchW, int stretchH, // area of hAlphaStretchSurface
						 ALPHA_MODE alphaMode,
						 uint8_t srcA, uint8_t desA,
						 HANDLE hQueue,
						 COLORKEY_MODE byColorKeySelect,
						 unsigned long iColorKey,
						 unsigned long dst_colorkey,
						 int isForceDestAlpha,
						 uint8_t forcedDesA,
						 STRETCH_COEFF_MODE	StretchMode,
			             uint8_t iVerticalScalingOffset,
			             int iVerticalTapNumber,
						 void *pExParam);

#endif
// function to do Stretch + AlphaBlt:
HRESULT DG_AlphaStretch( SURFACEHANDLE hSurface,
						 int dstX, int dstY, int dstW, int dstH,
						 SURFACEHANDLE hImgSurface,
						 int srcX, int srcY, int srcW, int srcH,
						 SURFACEHANDLE hAlphaStretchSurface,	// tmp surface for doing stretch
						 int stretchX, int stretchY, int stretchW, int stretchH, // area of hAlphaStretchSurface
						 ALPHA_MODE alphaMode,
						 uint8_t srcA, uint8_t desA,
						 HANDLE hQueue,
						 COLORKEY_MODE byColorKeySelect,
						 unsigned long iColorKey,
						 int isForceDestAlpha,
						 uint8_t forcedDesA,
						 STRETCH_COEFF_MODE	StretchMode,
			             uint8_t iVerticalScalingOffset,
			             int iVerticalTapNumber);
#endif

HRESULT
DG_SurfaceFelics(SURFACEHANDLE cmpHandle,
                   int nWidth,
                   int nHeight,
                   SURFACEHANDLE uncmpHandle,
                   int nXuncmp,
                   int nYuncmp,
                   bool mode, 			// 0: decompress, 1: compress
                   int threshold);

SURFACEHANDLE
DG_GetGraphicDisplaySurface(VOUT_GRAPHIC_PLANE plane);

//====================================================================================
// Animation
//====================================================================================

// structure of array:
typedef struct ui_animation_path {
        int			x;
        int			y;
        int			w;
        int			h;
        uint8_t		alpha;
        uint32_t	delay;                    // unit : V Sync
} ui_animation_path;

// animation type:
typedef enum _animation_ty{

    TYPE_ANI_UNIT_SPEED,
    TYPE_ANI_TYPE_NUM,
} ENUM_ANIMATION_TYPE;

#define	ANI_BHV_NONE		0x00000000
#define ANI_BHV_BG_A_BLT	0x00000001
#define ANI_BHV_OBJ_STRH	0x00000100
#define ANI_BHV_OBJ_A_BLT	0x00000200

// function to produce the array for animation (user need to alloc the memory of *pPath):
HRESULT DG_MakeAnimationArray(int s_x, int s_y, int s_w, int s_h, uint8_t s_A,	// start position
							  int e_x, int e_y, int e_w, int e_h, uint8_t e_A,	// end position
							  ENUM_ANIMATION_TYPE ani_type,	// animation type
							  int step,						// array size, must >= 1
							  ui_animation_path *pPath,
							  int delay);					// unit : V Sync


// function to do the animation:
HRESULT DG_MovingBlt(uint32_t behavior,
					 SURFACEHANDLE hSurface,
		             SURFACEHANDLE hBGSurface,
		             int off_x,	// the offset in x-axis that the origin of background relative to the origin of the hSurface
		             int off_y,	// the offset in y-axis that the origin of background relative to the origin of the hSurface
		             SURFACEHANDLE hObjSurface,
		             int obj_x,	// the x-offset of the object of the hObjSurface
		             int obj_y,	// the y-offset of the object of the hObjSurface
		             int obj_w,	// the width of the object of the hObjSurface
		             int obj_h,	// the height of the object of the hObjSurface
		             SURFACEHANDLE hAlphaStretchSurface, // surface for doing alpha-stretch
		             int stretchX, // x-offset of the stretch area in the AlphaStretchSurface
		             int stretchY, // y-offset of the stretch area in the AlphaStretchSurface
		             int stretchW, // width of the stretch area in the AlphaStretchSurface
		             int stretchH, // height of the stretch area in the AlphaStretchSurface
		             int minX,	// the min X-coordinates of display area of hSurface
		             int minY,	// the min Y-coordinates of display area of hSurface
		             int maxX,	// the max X-coordinates of display area of hSurface
		             int maxY,	// the max Y-coordinates of display area of hSurface
		             int step,
		             ui_animation_path *pPath,
		             HANDLE hQueue);

//====================================================================================
#define RATIO_MAX	7.9999
int CarryToIntByRatioDevide(int SrcInt, float ratio);

#endif	//IS_CHIP(MARS)

void
DG_PreAllocGraphicMemory(void);

void
DG_FreePreAllocGraphicMemory(void);

void
DG_listGraphicsMemory(size_t size);

//+JT - 20100115 - for DirectFB using
void *
DG_allocGraphicMemory(size_t size, BYTE** nonCachedAddr, unsigned long *pPhyAddr);

void
DG_freeGraphicMemory(void *mem_addr);

void* DG_GetDfbFusionObjMagic(SURFACEHANDLE handleDes);

#define USE_GRAPHIC_REGION	false //false or true

//+

#ifdef __cplusplus
}
#endif
#endif //DIRECT_GRAPHICS_H

//-----------------------------------------------------------------------------
// SysSurfaceCommon.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef SYSSURFACECOMMON_H
#define SYSSURFACECOMMON_H

namespace SonicSurface 
{
	//byte ordering signifies format info
	//B1 - type
	//B2 - subtype
	//B3 - true bitdepth
	//B4 - N/A

	#define kFormatTypeMask		0xFF000000
	#define	kFormatSubTypeMask	0x00FF0000
	#define kFormatBitDepth		0x0000FF00

	#define	kFormatIndexed		0x01000000
	#define	kFormat16BitRGB		0x02000000
	#define kFormat24BitRGB		0x04000000
	#define kFormat32BitRGB		0x08000000
	#define kFormat16BitYUV		0x10000000
	#define kFormat12BitPlanerYUV	0x20000000
	#define kFormat32BitPlanerYUV	0x40000000
	#define kFormatLuminance	0x80000000

	enum SurfaceFormat 
	{
		eFormat_Unknown	= 0,							//  something for the base class to return  
	    
		eFormat_1MonochromePixelFormat	= 0x01010100,   //  1 bit indexed 
	    eFormat_2IndexedPixelFormat		= 0x01020200,	//  2 bit indexed 
	    eFormat_4IndexedPixelFormat		= 0x01040400,   //  4 bit indexed 
	    eFormat_8IndexedPixelFormat		= 0x01080800,   //  8 bit indexed 

	    eFormat_16BE555PixelFormat		= 0x02001000,   //  16 bit Big End rgb 555 (Mac) 
	    eFormat_16LE555PixelFormat		= 0x02011000,	//  16 bit LE rgb 555 (PC) 
	    eFormat_16LE5551PixelFormat		= 0x02021000,	//  16 bit LE rgb 5551 
	    eFormat_16BE565PixelFormat		= 0x02041000,	//  16 bit BE rgb 565 
	    eFormat_16LE565PixelFormat		= 0x02081000,	//  16 bit LE rgb 565 

		eFormat_24RGBPixelFormat		= 0x04001800,   //  24 bit rgb (Mac)
	    eFormat_24BGRPixelFormat		= 0x04011800,	//  24 bit bgr (Windows)

	    eFormat_32ARGBPixelFormat		= 0x08002000,   //  32 bit argb (Mac)
		eFormat_32BGRAPixelFormat		= 0x08012000,	//  32 bit bgra (Windows) 
		eFormat_32BGRPixelFormat		= 0x08022000,	//  32 bit bgra (Windows, alpha ignored) 
	    eFormat_32ABGRPixelFormat		= 0x08042000,	//  32 bit abgr
	    eFormat_32RGBAPixelFormat		= 0x08082000,	//  32 bit rgba
	    
		eFormat_YUY2PixelFormat			= 0x10001000,  	//  YUV 4:2:2 byte ordering Y0,U0,Y1,V0  
	    eFormat_UYVYPixelFormat			= 0x10011000,	//  YUV 4:2:2 byte ordering	U0,Y0,V0,Y1
	    
	    eFormat_YV12PixelFormat			= 0x20010C00,	//  YUV 4:2:0 byte Planer Y plane followed by 2x2 V and U planes
	    eFormat_IYUVPixelFormat			= 0x20020C00,	//  YUV 4:2:0 byte Planer Y plane followed by 2x2 U and V planes
	    eFormat_NV12PixelFormat			= 0x20040C00,	//  YUV 4:2:0 byte Planer Y plane followed by 2x2 Interleaved U and V planes. NVidia motion compensation buffer format.  NVidia and ATI.
	    eFormat_NV24PixelFormat			= 0x20041800,	//  YUV 4:2:0 byte Planer Y plane followed by 2x2 Interleaved U and V planes. NVidia motion compensation buffer format.  NVidia and ATI.
	    eFormat_IMC1PixelFormat			= 0x20080C00,	//  As YV12 except the U and V planes each have the same stride as the Y plane.  Intel.
	    eFormat_IMC2PixelFormat			= 0x200a0C00,	//  Similar to IMC1 except that the U and V lines are interleaved at half stride boundaries.  Intel.
	    eFormat_IMC3PixelFormat			= 0x200c0C00,	//  As IMC1 except that U and V are swapped.  Intel.
	    eFormat_IMC4PixelFormat			= 0x200e0C00,	//  As IMC2 except that U and V are swapped.  Intel.

		eFormat_AYUVPixelFormat			= 0x40002000,  	//  AYUV 4:4:4 YUV format with 8 bit samples for each component along with an 8 bit alpha blend value per pixel. Component ordering is A Y U V.

		eFormat_8ALuminPixelFormat		= 0x80000800,	//  8 bit A4L4
		eFormat_16ALuminPixelFormat		= 0x80011000,	//  16 bit A8L8

	};

	//how does this Surface get initialized?
	enum SurfaceUse
	{
		eUse_New		= 0,		//allocate surface, fill to default
		eUse_Copy		= 1,		//allocate surface, copy contents from provided ptr
		eUse_OwnPassed	= 2,		//take ownership of another ptr, no allocation
		eUse_Reference	= 3			//reference another set of bits, no allocation or ownership
	};

	// Pixel aspect ratio
	enum SurfaceAspectRatio
	{
		kNoAspectRatio					= 0,		// undefined, or don't care
		kSquareAspectRatio				= 1,
		kNTSCAspectRatio				= 2,
		kPALAspectRatio					= 3,
		kHalfWidthNTSCAspectRatio		= 4,
		kHalfWidthPALAspectRatio		= 5,
		k16x9NTSCAspectRatio			= 6,
		k16x9PALAspectRatio				= 7,
		kSVCDNTSCAspectRatio			= 8,
		kSVCDPALAspectRatio				= 9,
		k221x1NTSCAspectRatio			= 10,
		k221x1PALAspectRatio			= 11,
		kDVBPALAspectRatio				= 12,
        kDVBNTSCAspectRatio             = 13,
		kHDAnamorphic960x1080			= 14,
		kHDAnamorphic1280x1080			= 15,
		kHDAnamorphic1440x1080			= 16,
		k16x9SqrAspectRatio				= 17
	};

	enum TransferMode
	{
		eSrcCopy			= 0,
		eSrcCopyVFlip		= 1,
		eSrcCopyRotate90	= 2, // rotate 90 degrees clockwise
		eSrcCopyRotate180	= 3,
		eSrcCopyRotate270	= 4,

		eMode_Last			= 5
	};

	enum TransferBlendMode
	{
		eBlendSrcOver = 0,				// (default) source alpha is used to blend source color with destination color
		eBlendSrcCopy = 1,				// source color (and alpha) overwrites destination color
	};

	enum TransferQuality
	{
		eNotSpecified		= 0,		// quality not specified
		eDefault			= 1,		// use system default

		eLowQuality			= 2,		// actual quality levels, lower quality generally equals higher speed
		eHiQuality			= 3,
		eBilinear			= 4,
		eBicubic			= 5,
		eNearestNeighbor	= 6,
		eHiQualityBilinear	= 7,
		eHiQualityBicubic	= 8,

		eQuality_Last		= 9,

// **CodeWizzard** - Possible Violation:  User Defined item 740  - When using enum, the values of each member should be explicitly declared
		eHighestSpeed = eLowQuality,
		eHighestQuality = eHiQualityBicubic
	};

	enum SurfaceColorRange
	{
		KNoColorRange  = 0,
		KColorRange0_255  =1,
		KColorRange16_235 = 2

	};
}	// namespace


#endif

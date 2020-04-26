//-----------------------------------------------------------------------------
// BaseSurfaceUtils.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef BASESURFACEUTILS_H
#define BASESURFACEUTILS_H


#include "SysSurfaceCommon.h"
#include "PiPoint.h"
#include "PiRect.h"
//#include "SysSurfaceForward.h"
#include "ColorPalette.h"
#include "SysSurface.h"
//#include "FilePath.h"

#ifndef REALTEK_MODIFY
#include "SonicText.h"
namespace SonicSurfaceText
{
	extern const SonicText invalidUse;
	extern const SonicText unimplemented;
	extern const SonicText badReferenceCount;
	extern const SonicText badDSInterface;
	extern const SonicText noMatchingColor;
}
#endif

namespace SonicSurface
{
//_____________________________________________________________________________


	UInt32			ConvertSurfaceFormatToBPP(SonicSurface::SurfaceFormat format);
	SonicSurface::SurfaceFormat ConvertBPPToSurfaceFormat(UInt32 depth);

	bool IsSurfaceFormatRGB(SonicSurface::SurfaceFormat format);
	bool IsSurfaceFormatYUV(SonicSurface::SurfaceFormat format);
	bool IsSurfaceFormatYUVPlaner(SonicSurface::SurfaceFormat format);

	void			GetPixelAspectRatio(SurfaceAspectRatio eAspectRatio, PiFPoint &outPixelAspectRatio);
	SurfaceAspectRatio	DeterminePixelAspectRatioBySize(SInt32 width, SInt32 height);

	PiSize			CalculateAspectRatioCorrection(const PiSize& srcSize, SurfaceAspectRatio srcAspectRatio,
													const PiSize& dstSize, SurfaceAspectRatio dstAspectRatio, UInt32 rotation);
	PiSize			CalculateAspectRatioCorrection(const PiSize& srcSize, const PiFPoint& srcPixelAspectRatio, 
													const PiSize& dstSize, const PiFPoint& dstPixelAspectRatio, UInt32 rotation);
	PiRect			DetermineLetterBoxSize(const PiSize &srcSize, const PiSize &dstSize);

	//SysSurface *RotateImage(const SysSurface& sourceSurface, UInt32 rotation);
	//SysSurface *ZoomImage(const SysSurface& sourceSurface, SInt32 sourceWidth, SInt32 sourceHeight);
	//SysSurface *CenterImage(const SysSurface& sourceSurface, SInt32 sourceWidth, SInt32 sourceHeight, PiRGBColor marginColor);
	//SysSurface *StretchImage(const SysSurface& sourceSurface, SInt32 sourceWidth, SInt32 sourceHeight);
	//SysSurface *LetterBoxImage(const SysSurface& srcSurface, const PiSize &dstSize, SurfaceAspectRatio destAspectRatio, 
	//							PiRGBColor const &bgColor);

	//UInt16			PixelValueStandardDeviation(SysSurface &rSurface);

	//void			SwapFields(SysSurface &dstSurface, SysSurface const &srcSurface);
	//void			DeInterlace(safe_ptr<SysSurface> &srcSurface);

    //void GenerateNumberedImageFiles( FilePath baseDir, UInt32 imageCount, PiSize size, PiRGBColor backgroundColor, PiRGBColor textColor );
}


#endif	//BASESURFACEUTILS_H

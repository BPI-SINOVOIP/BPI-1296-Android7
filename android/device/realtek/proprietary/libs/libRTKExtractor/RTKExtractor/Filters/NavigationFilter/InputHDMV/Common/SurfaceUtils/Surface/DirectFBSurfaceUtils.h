//-----------------------------------------------------------------------------
// DirectFBSurfaceUtils.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef DirectFB_SURFACEUTILS_H
#define DirectFB_SURFACEUTILS_H
#if defined (_DIRECTFB)
#include "SysSurfaceCommon.h"
#include "directfb.h"
/*#include "BaseSurfaceUtils.h"
//#include "FileSpec.h"

namespace SonicSurface 
{
	class DirectFBSurface;
}*/

namespace SonicSurface
{
	// to and from Media Samples
//	SonicSurface::DirectFBSurface*	CreateSurfaceFromMediaSample(IMediaSample* pSample, AM_MEDIA_TYPE *pmt);

//	SonicSurface::SurfaceFormat GetMediaTypeSurfaceFormat(AM_MEDIA_TYPE& mt);

	//pixel format conversions
	SonicSurface::SurfaceFormat	ConvertPixelFormat(DFBSurfacePixelFormat format);
	DFBSurfacePixelFormat ConvertPixelFormat(SonicSurface::SurfaceFormat format); 
#if 0 
	//bool LoadSurfaceFromFile(DirectFBSurface &rDest, const FileSpec& rFilename);
	bool GetImageInfoFromFileInMemory(const void *pFileBuffer, size_t bufferSize, PiSize& srcSize, SurfaceFormat& srcFormat);

	//debugging
#if defined (_DEBUG) || defined (DEBUG)
	void DumpSurfaceToFile(SonicSurface::DirectFBSurface const &rSrc, char const * const pComment = 0);
#else
	// just does nothing in release builds...
	void DumpSurfaceToFile(SonicSurface::DirectFBSurface const &, char const * const = 0);
#endif

	enum FileDumpTypes
	{
		eDump_PNG	= 0,
		eDump_BMP	= 1,
		eDump_JPG	= 2,
		eDump_GIF	= 3,
		eDump_TIFF	= 4
	};
	void WriteSurfaceToFile(DirectFBSurface &rSrc, FileSpec const &fsToUse, FileDumpTypes dumpType);
#endif
}
#endif
#endif	//DirectFB_SURFACEUTILS_H


//-----------------------------------------------------------------------------
// ImageRef.h
//
// ImageRef class definitions.
// Images can be passed to the Renderer as an actual surface, or as a file buffer reference (file buffer pointer and buffer size)
//
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef IMAGEREF_H
#define IMAGEREF_H

//#include "RendererTypes.h"
#include "ref_ptr.h"
#include "SysSurface.h"
#include "SysHDSurface.h"

//
// Image Reference Classes
//
class RENDERER_EXPORT BaseImageRef
{
public:
	BaseImageRef() { }
	virtual ~BaseImageRef() { }

	virtual bool IsFile() { return false; }
	virtual bool IsSurface() { return false; }
	virtual bool IsIndexedSurface() { return false; }
};

// Compressed image file loaded into memory (e.g. PNG, JPG, MNG)
// Renderer is responsible for allocating destination surface/texture and decoding image as necessary
// If the destination surface/texture is lost for some reason, the renderer will automatically try to rebuild it using the specified file buffer.
// NOTE: Although the renderer does NOT own the file buffer, it is assuming for now that the file buffer will always be valid (never deleted out from underneath it)
class RENDERER_EXPORT FileImageRef : public BaseImageRef
{
public:
	void const *mpFileBuffer; // pointer to file in memory
	size_t		mFileBufferSize; // size of file buffer
	UInt32		mFrame; // zero based frame number for multi-frame images (such as layered PSD files)

	FileImageRef() : BaseImageRef(), mpFileBuffer(0), mFileBufferSize(0), mFrame(0)  { }
	FileImageRef(void const *pFileBuffer, size_t fileSize) : BaseImageRef(), mpFileBuffer(pFileBuffer), mFileBufferSize(fileSize), mFrame(0) { }
	virtual ~FileImageRef() { }

	virtual bool IsFile() { return true; }
};

// User scripts may dynamically allocate surfaces and render into them as desired -- passing the resulting surface as a sprite to be composited normally
// NOTE: The renderer supplies a CreateSurface() function to allow users the ability to generate a new surface;
// users can then use the common SurfaceUtils to render or blit into the surface.
class RENDERER_EXPORT SurfaceImageRef : public BaseImageRef
{
public:
	ref_ptr<SonicSurface::SysHDSurface> mpSurface;

	SurfaceImageRef() : BaseImageRef(), mpSurface(0) { }
	SurfaceImageRef(const ref_ptr<SonicSurface::SysHDSurface> pSurface) : BaseImageRef(), mpSurface(pSurface) { }
	virtual ~SurfaceImageRef() { mpSurface = 0; }

	virtual bool IsSurface() { return true; }
};

// In cases where the Renderer doesn't natively support 8 bit indexed surfaces, users can pass an old style 8 bit indexed SysSurface
// which the Renderer will convert to a native format that it does support while still allowing palette changes
// NOTE: Users can check for native 8 bit support either by calling BaseRenderer::SupportsIndexedSurfaces()
class RENDERER_EXPORT IndexedSurfaceImageRef : public BaseImageRef
{
public:
	ref_ptr<SonicSurface::SysSurface> mpIndexedSurface;

	IndexedSurfaceImageRef() : BaseImageRef(), mpIndexedSurface(0) { }
	IndexedSurfaceImageRef(const ref_ptr<SonicSurface::SysSurface> pSurface) : BaseImageRef(), mpIndexedSurface(pSurface) { }
	virtual ~IndexedSurfaceImageRef() { mpIndexedSurface = 0; }

	virtual bool IsIndexedSurface() { return true; }
};

RENDERER_EXPORT SInt32 CompareImageRef(BaseImageRef *p1, BaseImageRef *p2);

#endif	// IMAGEREF_H

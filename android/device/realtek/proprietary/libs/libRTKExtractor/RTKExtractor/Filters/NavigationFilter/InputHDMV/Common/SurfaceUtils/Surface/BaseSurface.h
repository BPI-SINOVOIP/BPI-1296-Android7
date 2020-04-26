//-----------------------------------------------------------------------------
// BaseSurface.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef BASESURFACE_H
#define BASESURFACE_H

#include "SysSurfaceCommon.h"
#include "PiRect.h"
#include "ColorPalette.h"
#include "safe_ptr.h"

class PiFPoint;
class FileSpec;

namespace SonicSurface 
{

class PiRGBColor;

class BaseSurface
{
public:
	BaseSurface();
	BaseSurface(SInt32 width, SInt32 height, SInt32 colorDepth, 
						void* pBits = 0, SInt32 stride = 0, SurfaceUse use = eUse_New);
	BaseSurface(SInt32 width, SInt32 height, SurfaceFormat surfFormat, 
						void* pBits = 0, SInt32 stride = 0, SurfaceUse use = eUse_New);
	virtual ~BaseSurface();

	//assign & copy constructor. Be sure to properly use!
	BaseSurface(BaseSurface const &rhs);
	BaseSurface& operator=(BaseSurface const &rhs);

	//manual initialization
	virtual void	Initialize(SInt32 width, SInt32 height, SInt32 colorDepth, 
									void* pBits = 0, SInt32 stride = 0, SurfaceUse use = eUse_New);
	virtual void	Initialize(SInt32 width, SInt32 height, SurfaceFormat surfFormat, 
									void* pBits = 0, SInt32 stride = 0, SurfaceUse use = eUse_New);

	// Save surface to file.  Format will vary by platform
	virtual bool	WriteToFile(FileSpec const &) const;

	// queries
			UInt32	GetWidth() const				{ return(mActiveWidth); }
			UInt32	GetHeight() const				{ return(mActiveHeight); }
			UInt32	GetDepth() const				{ return(mBitDepth); }	// returns in bpp
			SInt32	GetScanLineStride() const		{ return(mActiveStride); }
			SurfaceFormat GetPixelFormat() const		{ return(mSurfaceFormat); }
			void	GetRGBAPos(SInt8 &rPos, SInt8 &gPos, SInt8 &bPos, SInt8 &aPos) const;
			SInt8	GetAlphaPos() const;
			bool	IsTopDown() const;
			void	SetIsTopDown(bool bTopDown);
	virtual bool	IsAlphaPremultiplied() const	{ return false; }

	// get at surface data
	virtual void  LockPixels() const;
    virtual void  UnlockPixels() const;
	virtual bool  IsLocked() const;

	// once locked, you can actually manipulate things
	virtual void*	GetScanLine(SInt32 row);
    virtual const void* GetConstScanLine(SInt32 row) const;
	// but be careful when accessing BufferPtr vs. ScanLine
	virtual char*	GetBufferPtr();
	virtual const char*	GetConstBufferPtr() const;

	//pixel-aspect ratio
			void	GetPixelAspectRatio(PiFPoint& ptOut) const;
			void	SetPixelAspectRatio(SurfaceAspectRatio eAspectRatio);
			void	SetPixelAspectRatio(const PiFPoint& ptIn);
			void	SetPixelAspectRatio(const BaseSurface& fromSurface);
			void	SetNoPixelAspectRatio();
			bool	HasPixelAspectRatio() const;

	//clip rect
			void	ClipEmptyRects(bool clipEmptyRects) { mClipEmptyRects = clipEmptyRects; }
			const PiRect& GetClippingRect() const	{ return(mClipRect); }
	virtual void	SetClippingRect(const PiRect& r);
	virtual void	ResetClippingRect();

	virtual	void	Clear();
	virtual void	Clear(UInt8 idx);
	virtual void	Clear(const PiRGBColor &color) = 0;
	virtual void	FillAlphaChannel(UInt8 alpha);
			void	ConvertTransparencyToColor(UInt8 alphaThreshold, PiRGBColor const &color);

			void	GetOptimizedPalette(ColorPalette& palette, UInt32 numColors) const;
			void	GetOptimizedPalette(ColorUsageList& palette, UInt32 numColors) const;
	virtual void	GetPalette(ColorPalette& palette) const;
			UInt32	GetPaletteEntryCount() const;
	virtual void	SetPalette(const ColorPalette& palette, bool remapPixels);
	virtual void	SetPalette(const ColorUsageList& palette, bool remapPixels);

	//this is a clip rect that actually switches all calls to use a subsection of the surface
	virtual void	SetActiveRect(PiRect& activeRect);
	virtual void	ResetActiveRect();

protected:
	virtual void	ResetSurface();
	virtual UInt32	GetBufferSize();
			void	InitPalette();

protected:
	SurfaceFormat			mSurfaceFormat;

	mutable SInt32			mLockCount;
	SInt32					mWidth;
	SInt32					mHeight;

	//presumes constant stride & depth (no variable compression)
	SInt32					mStride;		//a negative stride indicates vertically flipped
	SInt32					mBitDepth;		//ease-of-use bitdepth

	char*					mpBuffer;		//ptr to row 0 of the bits - not-allocated
	safe_ptr_array<char>	mBuffer;		//bit-bucket of pixels - only if we own them, may be null

	float					mPixelAspectRatio_X;
	float					mPixelAspectRatio_Y;
	bool					mHasPixelAspectRatio;

	PiRect					mClipRect;		//clipping Rect
	bool					mClipEmptyRects;

	ColorPalette			mPalette;

	//since we might play games by adjusting the stride and start point of the rasters
	//(e.g. to crop to a subrect on the surface) - we actually have some variables for holding
	//the current "active" width/height/stride/buffer ptr
	SInt32					mActiveWidth;
	SInt32					mActiveHeight;
	SInt32					mActiveStride;		//a negative stride indicates vertically flipped
	char*					mpActiveBuffer;		//ptr to row 0 of the bits - not-allocated
	PiPoint					mActiveStartPt;		//start x,y for subrect


private:
	void	CopySurfaceToSelf(BaseSurface const &rhs);

};



}


#endif	//WINBLITTER_H

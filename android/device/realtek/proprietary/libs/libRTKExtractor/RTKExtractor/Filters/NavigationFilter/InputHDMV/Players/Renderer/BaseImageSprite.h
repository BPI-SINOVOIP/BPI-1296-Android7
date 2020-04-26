//-----------------------------------------------------------------------------
// BaseImageSprite.h
//
// Base Image Sprite class declarations for use by HD Renderer
//
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef BASE_IMAGE_SPRITE_H
#define BASE_IMAGE_SPRITE_H


#include "BaseSprite.h"
#include "ref_ptr.h"
#include "ImageRef.h"
#include "SysHDSurface.h" 

// Forward declarations
class ImageRefCacheObject;

#define kSpriteSrcRectChanged			0x0100
#define kSpriteImageChanged				0x0200
#define kSpriteDisplayRectChanged		0x0400
#define kSpriteDisplayFlagsChanged		0x0800
#define kSpritePaletteChanged			0x1000

// General sprite attributes, as well as source image ref and source image crop rect
class ImageSpriteState : public BaseSpriteState
{
public:
	// Constructor
	ImageSpriteState() : BaseSpriteState(), mpSrcImageRef(0), mpSrcRect(0), mDisplayRect(), mFlipHorizontally(false), mFlipVertically(false), mRepeat(false), mPaletteIndex(-1) { }

	// Accessor functions
	void SetSourceImage(const ref_ptr<BaseImageRef>& pSrcImageRef)
	{
		if ( (mpSrcImageRef.get() == 0 && pSrcImageRef.get() != 0) ||
			 (pSrcImageRef.get() == 0 && mpSrcImageRef.get() != 0) ||
			 (mpSrcImageRef.get() != 0 && pSrcImageRef.get() != 0 && CompareImageRef(mpSrcImageRef.get(), pSrcImageRef.get()) != 0)
			)
		{ 
			mpSrcImageRef = pSrcImageRef;
			mDirtyMask |= kSpriteImageChanged;
		}
	}

	void SetSrcRect(ref_ptr<PiRect>& pSrcRect)
	{
		if ( (mpSrcRect.get() == 0 && pSrcRect.get() != 0) ||
			(pSrcRect.get() == 0 && mpSrcRect.get() != 0) ||
			(pSrcRect.get() != 0 && mpSrcRect.get() != 0  && *mpSrcRect != *pSrcRect)
			)
		{
			mpSrcRect = pSrcRect;
			mDirtyMask |= kSpriteSrcRectChanged;
		}
	}

	void SetDisplayRect(const PiRect& displayRect)
	{
		if (mDisplayRect != displayRect)
		{
			mDisplayRect = displayRect;
			mDirtyMask |= kSpriteDisplayRectChanged;
		}
	}

	void SetFlipHorizontally(bool flip) { if (mFlipHorizontally != flip) { mFlipHorizontally = flip; mDirtyMask |= kSpriteDisplayFlagsChanged; } }
	void SetFlipVertically(bool flip) { if (mFlipVertically != flip) { mFlipVertically = flip; mDirtyMask |= kSpriteDisplayFlagsChanged; } }
	void SetRepeatMode(bool repeat) { if (mRepeat != repeat) { mRepeat = repeat; mDirtyMask |= kSpriteDisplayFlagsChanged; } }
	void SetPaletteIndex(SInt32 paletteIndex) { if (mPaletteIndex != paletteIndex) { mPaletteIndex = paletteIndex; mDirtyMask |= kSpritePaletteChanged; } }

	ref_ptr<BaseImageRef> GetSourceImage() const { return (mpSrcImageRef); }
	ref_ptr<PiRect> GetSrcRect() const { return (mpSrcRect); }
	PiRect GetDisplayRect() const { return (mDisplayRect); }
	bool GetFlipHorizontally() const { return (mFlipHorizontally); }
	bool GetFlipVertically() const { return (mFlipVertically); }
	bool GetRepeatMode() const { return (mRepeat); }
	SInt32 GetPaletteIndex() const { return (mPaletteIndex); }

protected:
	// Data
	ref_ptr<BaseImageRef> mpSrcImageRef; // source image reference (either compressed file buffer or user-created surface)
	ref_ptr<PiRect> mpSrcRect; // Portion of source image to use (NULL to use full image)

	// User can specify optional display sub rect relative to the destination rect within which the source image should be displayed
	// If the display sub rect is not specified, renderer scales the source image to fill the destination rect
	PiRect mDisplayRect;

	bool mFlipHorizontally, mFlipVertically; // true to flip image horizontally and/or vertically
	bool mRepeat; // true to tile image both horizontally and vertically

	SInt32 mPaletteIndex; // Global palette index to use (or -1 for no palette)
};

class BaseImageSprite : public BaseSprite
{
public:

	// Constructor
	BaseImageSprite()
		: BaseSprite(), mpImage(0), mpCacheAccessor(0)
	{
		// On startup, set logical state to active; when the sprite is first committed, it will automatically copy the state across
		mStates[eLogicalStateSet].SetActive();
	}
	virtual ~BaseImageSprite() {}

	// General accessor functions
	bool GetVisibility(ESpriteStateIndex stateIndex) const { return mStates[stateIndex].GetVisibility(); }
	UInt32 GetZDepth(ESpriteStateIndex stateIndex) const { return mStates[stateIndex].GetZDepth(); }
	PiRect GetDstRect(ESpriteStateIndex stateIndex) const { return mStates[stateIndex].GetDstRect(); }
	UInt8 GetOpacity(ESpriteStateIndex stateIndex) const { return mStates[stateIndex].GetOpacity(); }
	UInt32 GetCanvas(ESpriteStateIndex stateIndex) const { return mStates[stateIndex].GetCanvas(); }
	void GetBorders(ESpriteStateIndex stateIndex, BorderInfo *pBorders) const { mStates[stateIndex].GetBorders(pBorders); }
	SonicSurface::PiRGBColor GetBackgroundColor(ESpriteStateIndex stateIndex) const { return mStates[stateIndex].GetBackgroundColor(); }

	ref_ptr<BaseImageRef> GetSourceImage(ESpriteStateIndex stateIndex) const { return (mStates[stateIndex].GetSourceImage()); }
	ref_ptr<PiRect> GetSrcRect(ESpriteStateIndex stateIndex) const { return (mStates[stateIndex].GetSrcRect()); }
	PiRect GetDisplayRect(ESpriteStateIndex stateIndex) const { return (mStates[stateIndex].GetDisplayRect()); }
	bool GetFlipHorizontally(ESpriteStateIndex stateIndex) const { return (mStates[stateIndex].GetFlipHorizontally()); }
	bool GetFlipVertically(ESpriteStateIndex stateIndex) const { return (mStates[stateIndex].GetFlipVertically()); }
	bool GetRepeatMode(ESpriteStateIndex stateIndex) const { return (mStates[stateIndex].GetRepeatMode()); }
	SInt32 GetPaletteIndex(ESpriteStateIndex stateIndex) const { return (mStates[stateIndex].GetPaletteIndex()); }

	// Application functions
	void SetVisibility(bool flag) { mStates[eLogicalStateSet].SetVisibility(flag); }
	void SetZDepth(UInt32 depth) { /*assert(depth >= kStartingSpriteZDepth);*/ mStates[eLogicalStateSet].SetZDepth(depth); }
	void SetDstRect(const PiRect& dstRect) { mStates[eLogicalStateSet].SetDstRect(dstRect); }
	void SetOpacity(UInt8 opacity) { mStates[eLogicalStateSet].SetOpacity(opacity); }
	void SetCanvas(UInt32 canvas) { mStates[eLogicalStateSet].SetCanvas(canvas); }
	void SetBorder(EBorderIndex index, const BorderInfo& info) { mStates[eLogicalStateSet].SetBorder(index, info); }
	void SetBackgroundColor(const SonicSurface::PiRGBColor& color) { mStates[eLogicalStateSet].SetBackgroundColor(color); }

	void SetSourceImage(ref_ptr<BaseImageRef>& pImageRef) { mStates[eLogicalStateSet].SetSourceImage(pImageRef); }
	void SetSrcRect(ref_ptr<PiRect>& pSrcRect) { mStates[eLogicalStateSet].SetSrcRect(pSrcRect); }
	void SetDisplayRect(const PiRect& displayRect) { mStates[eLogicalStateSet].SetDisplayRect(displayRect); }
	void SetFlipHorizontally(bool flip) { mStates[eLogicalStateSet].SetFlipHorizontally(flip); }
	void SetFlipVertically(bool flip) { mStates[eLogicalStateSet].SetFlipVertically(flip); }
	void SetRepeatMode(bool repeat) { mStates[eLogicalStateSet].SetRepeatMode(repeat); }
	void SetPaletteIndex(SInt32 paletteIndex) { mStates[eLogicalStateSet].SetPaletteIndex(paletteIndex); }

	virtual bool IsDirty() { return (mStates[eLogicalStateSet].GetDirtyMask() != 0); }
	virtual void MarkToBeDestroyed() { mStates[eLogicalStateSet].MarkToBeDestroyed(); }

	// used for video frames coming from external a/p (SMP)
	virtual bool ReserveForVideo(bool bReserve) {return bReserve; /* overriden by WinImageSprite*/}

	// Renderer functions
	virtual bool IsImage() { return true; }
	ESpriteState GetState() const { return (mStates[eCommittedStateSet].GetState()); }
	UInt32 GetDirtyMask() const { return (mStates[eCommittedStateSet].GetDirtyMask()); }
	void ClearDirtyMask() { mStates[eLogicalStateSet].ClearDirtyMask(); mStates[eCommittedStateSet].ClearDirtyMask(); }
	virtual void Commit() { mStates[eCommittedStateSet] = mStates[eLogicalStateSet]; }

	void SetImageToRender(const ref_ptr<SonicSurface::SysHDSurface>& pImage) { mpImage = pImage; }
	ref_ptr<SonicSurface::SysHDSurface> GetImageToRender() { return (mpImage); }

#if 0
	void SetImageRefCacheObject(ref_ptr<ImageRefCacheObject> pAccessor);
#else
	//void SetImageRefCacheObject(ref_ptr<ImageRefCacheObject> const & pAccessor);
#endif

protected:
	ImageSpriteState mStates[2];

	ref_ptr<SonicSurface::SysHDSurface> mpImage; // Renderer builds platform specific surface/texture from source image reference
#if 0
	ref_ptr<ImageRefCacheObject> mpCacheAccessor; // Prevent image ref cache from purging cache objects in use by active sprites
#else
	void * mpCacheAccessor;						 // Prevent image ref cache from purging cache objects in use by active sprites
#endif
};

#endif	// BASE_IMAGE_SPRITE_H

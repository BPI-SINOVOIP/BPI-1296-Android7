//-----------------------------------------------------------------------------
// BaseSprite.h
//
// Base Sprite class declarations for use by HD Renderer
//
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef BASE_SPRITE_H
#define BASE_SPRITE_H


#include "PiRect.h"
#include "PiRGBColor.h"
#include <assert.h>

// Public enums for simplicity sake
enum EBorderIndex { eLeftBorder = 0, eTopBorder, eRightBorder, eBottomBorder, eMaxBorders };
enum ESpriteState { eInactiveSprite, eActiveSprite, eDeadSprite };

// Starting Z Depth for sprites
#define kDVDPlaybackModeMinSpriteZDepth	1000
#define kStartingSpriteZDepth			kDVDPlaybackModeMinSpriteZDepth + 50

// Sprite dirty bit flags
#define kSpriteVisibilityChanged		0x0001
#define kSpriteZDepthChanged			0x0002
#define kSpriteDstRectChanged			0x0004
#define kSpriteOpacityChanged			0x0008
#define kSpriteBordersChanged			0x0010
#define kSpriteBackgroundColorChanged	0x0020
#define kSpriteCanvasChanged			0x0040
#define kSpriteStateChanged				0x0080

// Sprite elements may have 4 separately defined border frames -- each with its own width and/or color
class BorderInfo
{
public:
	// Data
	UInt32 mWidth; // If width is zero, don't draw border
	SonicSurface::PiRGBColor mColor;

	// Constructors
	BorderInfo() : mWidth(0), mColor() { }
	BorderInfo(UInt32 width, const SonicSurface::PiRGBColor& color) : mWidth(width), mColor(color) { }
};

bool operator==(const BorderInfo& info1, const BorderInfo& info2);
bool operator!=(const BorderInfo& info1, const BorderInfo& info2);

// General sprite attributes, including a dirty mask of what's been changed
class BaseSpriteState
{
public:

	// Constructor
	BaseSpriteState() : mVisibility(true), mZDepth(0), mDstRect(), mOpacity(255), mCanvas(0), mBackgroundColor(0,0,0,0), mState(eInactiveSprite), mDirtyMask(0) { }

	// Accessor functions
	void SetVisibility(bool flag)			{ if (mVisibility != flag) { mVisibility = flag; mDirtyMask |= kSpriteVisibilityChanged; } }
	void SetZDepth(UInt32 zDepth)			{ if (mZDepth != zDepth) { mZDepth = zDepth; mDirtyMask |= kSpriteZDepthChanged; } }
	void SetDstRect(const PiRect& dstRect)	{ if (mDstRect != dstRect) { mDstRect = dstRect; mDirtyMask |= kSpriteDstRectChanged; } }
	void SetOpacity(UInt8 opacity)			{ if (mOpacity != opacity) { mOpacity = opacity; mDirtyMask |= kSpriteOpacityChanged; } }
	void SetCanvas(UInt32 canvas)			{ if (mCanvas != canvas) { mCanvas = canvas; mDirtyMask |= kSpriteCanvasChanged; } }
	void SetBorder(EBorderIndex index, const BorderInfo& info) { if (mBorders[index] != info) { mBorders[index] = info; mDirtyMask |= kSpriteBordersChanged; } }
	void SetBackgroundColor(const SonicSurface::PiRGBColor& color) { if (mBackgroundColor != color) { mBackgroundColor = color; mDirtyMask |= kSpriteBackgroundColorChanged; } }

	bool GetVisibility() const { return (mVisibility); }
	UInt32 GetZDepth() const { return (mZDepth); }
	PiRect GetDstRect() const { return (mDstRect); }
	UInt8 GetOpacity() const { return (mOpacity); }
	UInt32 GetCanvas() const { return (mCanvas); }
	void GetBorders(BorderInfo *pBorders) const 
	{
		assert(pBorders);
		for (UInt32 i = 0; i < eMaxBorders; ++i)
		{
			pBorders[i] = mBorders[i];
		}
	}
	SonicSurface::PiRGBColor GetBackgroundColor() const { return (mBackgroundColor); }

	void SetActive() { if (mState != eActiveSprite) { mState = eActiveSprite; mDirtyMask |= kSpriteStateChanged; } }
	void MarkToBeDestroyed() { if (mState != eDeadSprite) { mState = eDeadSprite;  mDirtyMask |= kSpriteStateChanged; } }
	ESpriteState GetState() const { return (mState); }

	UInt32 GetDirtyMask() const { return (mDirtyMask); }
	void ClearDirtyMask() { mDirtyMask = 0; }

protected:
	// Data
	bool		mVisibility; // render if true, else hidden
	UInt32		mZDepth; // z order bottom to front (e.g. zero is background)
	PiRect		mDstRect; // position, width and height
	UInt8		mOpacity; // Transparency (0 is fully transparent, 255 is fully opaque)
	UInt32		mCanvas; // Canvas the sprite is associated with (defaults to 0 for main Renderer window)

	BorderInfo mBorders[eMaxBorders];

	SonicSurface::PiRGBColor mBackgroundColor;
	
	ESpriteState mState;

	UInt32		mDirtyMask;

};

// Base sprite class declaration
// NOTE: In order to maintain sync lock, all sprites have separate logical and committed state sets
// Applications operate on the logical state set, while the Renderer operates off the current committed state set
class BaseSprite
{
public:
	BaseSprite() { }
	virtual ~BaseSprite() { }

	// General accessor functions
	enum ESpriteStateIndex { eLogicalStateSet, eCommittedStateSet };
	virtual bool GetVisibility(ESpriteStateIndex /*stateIndex*/) const { return false; };
	virtual UInt32 GetZDepth(ESpriteStateIndex /*stateIndex*/) const { return 0; };
	virtual PiRect GetDstRect(ESpriteStateIndex /*stateIndex*/) const { return PiRect(); };
	virtual UInt8 GetOpacity(ESpriteStateIndex /*stateIndex*/) const { return 0; };
	virtual UInt32 GetCanvas(ESpriteStateIndex /*stateIndex*/) const { return 0; };
	virtual void GetBorders(ESpriteStateIndex /*stateIndex*/, BorderInfo * /*pBorders*/) const { };
	virtual SonicSurface::PiRGBColor GetBackgroundColor(ESpriteStateIndex /*stateIndex*/) const { return SonicSurface::PiRGBColor(); };

	// Application functions -- operate on the logical states
	virtual void SetVisibility(bool /*flag*/) { assert(false); };
	virtual void SetZDepth(UInt32 /*depth*/) { assert(false); };
	virtual void SetDstRect(const PiRect& /*dstRect*/) { assert(false); };
	virtual void SetOpacity(UInt8 /*opacity*/) { assert(false); };
	virtual void SetCanvas(UInt32 /*canvas*/) { assert(false); };
	virtual void SetBorder(EBorderIndex /*index*/, const BorderInfo& /*info*/) { assert(false); };
	virtual void SetBackgroundColor(const SonicSurface::PiRGBColor& /*color*/) { assert(false); };

	virtual bool IsDirty() { assert(false); return false; }
	virtual void MarkToBeDestroyed() { assert(false); };

	// Renderer functions
	virtual bool IsAudio() { return false; }		// OK to dynamic_cast<BaseAudioSprite *>
	virtual bool IsGraph() { return false; }		// OK to dynamic_cast<BaseGraphSprite *>
	virtual bool IsImage() { return false; }		// OK to dynamic_cast<BaseImageSprite *>
	virtual bool IsText()  { return false; }		// OK to dynamic_cast<BaseTextSprite *>
	virtual bool IsVideo() { return false; }		// OK to dynamic_cast<BaseVideoSprite *>
	virtual ESpriteState GetState() const { assert(false); return eInactiveSprite; };
	virtual UInt32 GetDirtyMask() const { assert(false); return 0; };
	virtual void ClearDirtyMask() { assert(false); }
	virtual void Commit() { assert(false); };

};

#endif	// BASE_SPRITE_H

//-----------------------------------------------------------------------------
// CanvasManager.h
// Copyright (c) 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __CanvasManager_h__
#define __CanvasManager_h__

// Common
#include "ref_ptr.h"
class PiRect;
class PiSize;

// HDRenderer
class BaseRenderer;

class CanvasManager
{
public:
	CanvasManager(ref_ptr<BaseRenderer> inRenderer, UInt32 inRendererCanvas);
	virtual ~CanvasManager();

public:
	virtual SonicError ApplyBDJVideoSettings();
	virtual SonicError ConfigureKeepResolution(bool bKeepResolution, UInt16 uiBaseResWidth, UInt16 uiBaseResHeight, bool bUseScreenRect, 
		float screenRectX, float screenRectY, float screenRectWidth, float screenRectHeight);
	virtual UInt32 GetRendererCanvas() const { return mRendererCanvas; }
	virtual SonicError HideVideo();
	virtual SonicError NotifyScaling();
	virtual SonicError ResetVideoSettings();
	virtual SonicError SetVideoTransformation(float offsetX, float offsetY, float scaleX, float scaleY);
	virtual SonicError ShowFullScreenVideo(bool inClearBackground);
	virtual SonicError SwitchResolution(UInt32 inWidth, UInt32 inHeight, UInt32 inARx, UInt32 inARy);
	virtual SonicError UpdateVideoSize(UInt32 inSizeX, UInt32 inSizeY, UInt32 inAspectX, UInt32 inAspectY);
	virtual SonicError ResetAudioMixer();
	virtual bool       IsPrimaryVideoFullScreen(void);

protected:
	void GetVideoDisplaySize(PiSize& outSize, bool& outMaintainAR);
	void GetVideoDisplaySize(const PiSize& inNativeSize, PiSize& outSize, bool& outMaintainAR);

protected:
	ref_ptr<BaseRenderer> mRenderer;
	UInt32 mRendererCanvas;

	bool mScalingInProgress;

	float mTransformOffsetX;
	float mTransformOffsetY;
	float mTransformScaleX;
	float mTransformScaleY;

	bool mCanvasMatchesVideo;
	UInt32 mVideoSizeX; // The current video size reported from the renderer
	UInt32 mVideoSizeY; // The current video size reported from the renderer
	UInt32 mVideoARX; // The current video aspect ratio reported from the renderer
	UInt32 mVideoARY; // The current video aspect ratio reported from the renderer
};

#endif // __CanvasManager_h_

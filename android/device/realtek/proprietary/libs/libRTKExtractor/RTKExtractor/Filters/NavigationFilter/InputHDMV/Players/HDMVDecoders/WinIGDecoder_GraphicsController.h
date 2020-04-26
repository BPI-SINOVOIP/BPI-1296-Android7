//-----------------------------------------------------------------------------
// WinIGDecoder_GraphicsController.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINIGDECODER_GRAPHICSCONTROLLER_H__
#define __WINIGDECODER_GRAPHICSCONTROLLER_H__

#include "IGDecoder_GraphicsController.h"
#include "WinHDMVDecoder_DisplayFrameUtils.h"
#include "BDROM_PTSAccess.h"

class DisplayLayer;


//__________________________________________________________________________________________________________________________________________________
// WinIGDecoder_GraphicsController
// The Windows IG Decoder's GC
//

class WinIGDecoder_GraphicsController : public IGDecoder_GraphicsController
{
public:
	WinIGDecoder_GraphicsController();
	virtual ~WinIGDecoder_GraphicsController();

	void Init(ref_ptr<BaseRenderer> pRenderer, ref_ptr<BDROM_SoundPlayer> pSoundPlayer,
								  IGDecoder_CompositionBuffer * pCompositionBuffer, IGDecoder_DecodedDataBuffer * pDecodedDataBuffer,
								  DisplayLayer * pDisplayLayer, ref_ptr<BDROM_PTSAccess> pPtsAccess);
	virtual void Reset(void);

	virtual void SetRenderDeviceState(DeviceState renderDeviceState);

	virtual void Flush(void);

protected:
	// overridden functions
	virtual void ButtonStatesChanged(IGButtonStatesChangeReason btnStateChangeReason, void * pInfo = 0);

	virtual void RendererPrepare(void)									{ }
	virtual void RendererCommit(void)									{ }

public:
	virtual bool SetupPageButtons(void);

	virtual bool RenderPage(void);										// first display of a page. returns false if page was not fully rendered
	virtual void AnimatePage(void);										// Animates buttons in the active page
	bool RenderOrAnimatePage(bool bUpdateAnimation);					// displays active page. Used for first display of page, page animations too.
	virtual bool RenderPageEffects(UInt32 uiPageId, IGPageEffectType effectType);	// displays In/Out effects of a page.
	void RemoveLastFrame(void);											// removes last frame sent to display. last frame is undefined after this.
	virtual void ClearDisplay(void);									// removes active page, if any, from display. Active page is undefined after this.
	void RemoveAllFrames(void);											// removes all frames from display (incl queued). last frame is undefined after this.

private:
	WinIGDecoder_GraphicsController(const WinIGDecoder_GraphicsController&); // no copying
	const WinIGDecoder_GraphicsController& operator=(const WinIGDecoder_GraphicsController&); // no copying

private:
	DisplayLayer		*	m_pDisplayLayer;

	GCFrameInfo				m_LastFrame;			// last from that IG decoder displayed
	bool					m_bAbortFrameWait;		// flag to abort the 'wait for frame' loop in composition rendering functions
//	bool					m_bPageNeedsUpdation;	// indicates that current page needs to be displayed again (e.g., new button selected by user)
};

#endif // __WINIGDECODER_GRAPHICSCONTROLLER_H__

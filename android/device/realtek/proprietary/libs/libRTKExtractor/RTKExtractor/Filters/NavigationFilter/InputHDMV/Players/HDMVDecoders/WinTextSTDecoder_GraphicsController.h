//-----------------------------------------------------------------------------
// WinTextSTDecoder_GraphicsController.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINTEXTSTDECODER_GRAPHICSCONTROLLER_H__
#define __WINTEXTSTDECODER_GRAPHICSCONTROLLER_H__

#include "TextSTDecoder_GraphicsController.h"
#include "WinHDMVDecoder_DisplayFrameUtils.h"
#include "BDROM_PTSAccess.h"

class DisplayLayer;


//__________________________________________________________________________________________________________________________________________________
// WinTextSTDecoder_GraphicsController
// Windows TextST Decoder's GC
//

class WinTextSTDecoder_GraphicsController : public TextSTDecoder_GraphicsController
{
public:
	WinTextSTDecoder_GraphicsController();
	~WinTextSTDecoder_GraphicsController();

	void Init(ref_ptr<BaseRenderer> pRenderer,
				TextSTDecoder_CompositionBuffer * pCompositionBuffer,
				TextSTDecoder_DecodedDataBuffer * pDecodedDataBuffer,
				DisplayLayer * pDisplayLayer,
				ref_ptr<BDROM_PTSAccess> pPtsAccess);
	void Reset(void);

	virtual void SetRenderDeviceState(DeviceState renderDeviceState);

	// Event processing
	//
public:
	virtual void Flush(void);
	virtual void Stop(void);

	// Composition rendering
	//
protected:
	//virtual void ProcessTimerEvent(void);

	void RemoveLastFrame(void);											// removes last frame sent to display. last frame is undefined after this.
	void RemoveAllFrames(void);											// removes all frames from display (incl queued). last frame is undefined after this.
	bool CreateDPSBitmaps(txt_dialog_presentation_segment * pDPS, txt_dialog_style_segment * pDSS, UInt32 regionId);		// builds bitmap surfaces
	bool DrawText(txt_dialog_style_segment * pDSS, txt_dialog_presentation_segment * pDPS, UInt32 rgnId, SonicSurface::SysHDSurface * pSurface);
	void DestroyDPSBitmaps(txt_dialog_presentation_segment * pDPS);		// destroys bitmap surfaces associated with a DPS

	virtual void RendererPrepare(void)									{ }
	virtual void RendererCommit(void)									{ }

	virtual bool RenderSubtitle(UInt32 DPSnum, HDMVStreamPlaylistLocation & playlistLoc);
	virtual void RemoveSubtitle(UInt32 DPSnum);
	virtual void EnableSubtitle(bool bEnable);
	virtual void ClearDisplay(void)										{ RemoveAllFrames(); }

protected:
	DisplayLayer		*	m_pDisplayLayer;
	GCFrameInfo				m_LastFrame;			// last from that TextST decoder displayed
	bool					m_bAbortFrameWait;		// flag to abort the 'wait for frame' loop in composition rendering functions

private:
	WinTextSTDecoder_GraphicsController(const WinTextSTDecoder_GraphicsController&); // no copying
	const WinTextSTDecoder_GraphicsController& operator=(const WinTextSTDecoder_GraphicsController&); // no copying
};

#endif // __WINTEXTSTDECODER_GRAPHICSCONTROLLER_H__

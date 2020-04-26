//-----------------------------------------------------------------------------
// TextSTDecoder_GraphicsController.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __TEXTSTDECODER_GRAPHICSCONTROLLER_H__
#define __TEXTSTDECODER_GRAPHICSCONTROLLER_H__

#include "ref_ptr.h"
#include "SonicMutex.h"
#include <vector>
#include "BaseRenderer.h"
#include "HDMVDecoder_GraphicsController.h"
#include "HDMVDecoder_Types_Internal.h"
#include "BDROMIndex.h"
#include "BDROM_PTSAccess.h"

using namespace BDROM_HDMVDecoder;

class TextSTDecoder_CodedDataBuffer;
class TextSTDecoder_CompositionBuffer;
class TextSTDecoder_DecodedDataBuffer;


//__________________________________________________________________________________________________________________________________________________
// TextST Decoder GC Events
//

const GCEventCode kGCEventBase_TextST					= 0x04;

const GCEventCode kGCEvent_SelectUserStyle				= MAKE_GC_EVENTCODE(kGCEventBase_TextST, 0);
const GCEventCode kGCEvent_DialogPresSegmentDecoded		= MAKE_GC_EVENTCODE(kGCEventBase_TextST, 1);
const GCEventCode kGCEvent_TextSTDisplay				= MAKE_GC_EVENTCODE(kGCEventBase_TextST, 2);

//__________________________________________________________________________________________________________________________________________________
// DPS Display Info
// Info (like start and end PTS) of a DPS
//

struct DPSDisplayInfo
{
public:
	DPSDisplayInfo() : mNum(kDPSNum_Invalid), mStartStreamPts90k(0), mEndStreamPts90k(0)	{}

	void Reset(void)	{ mNum = kDPSNum_Invalid; mStartStreamPts90k = mEndStreamPts90k = 0; }
	bool IsValid(void)	{ return (mNum != kDPSNum_Invalid); }

public:
	UInt32			mNum;					// DPS Number (1 based)
	TimeValue		mStartStreamPts90k;		// Start PTS of DPS. This is a 90KHz value extracted from text stream.
	TimeValue		mEndStreamPts90k;		// End PTS of DPS. This is a 90KHz value extracted from text stream.
};


//__________________________________________________________________________________________________________________________________________________
// TextSTDecoder_GraphicsController
// The Text Subtitle Decoder's GC
//

class TextSTDecoder_GraphicsController : public HDMVDecoder_GraphicsController
{
public:
	TextSTDecoder_GraphicsController();
	virtual ~TextSTDecoder_GraphicsController();

	void Init(ref_ptr<BaseRenderer> pRenderer,
				  TextSTDecoder_CompositionBuffer * pCompositionBuffer,
				  TextSTDecoder_DecodedDataBuffer * pDecodedDataBuffer,
				  ref_ptr<BDROM_PTSAccess> pPtsAccess);
	virtual void Reset(void);

	void SetWorkingRegionStyle(const txt_dialog_style_segment & pDSS, UInt32 regionId, UInt32 regionStyleId);
	void SetRenderDeviceState(DeviceState renderDeviceState);

	// primary video size
	void SetVideoSize(UInt32 width, UInt32 height);

	// BDROM TextST Decoder API
	//
public:
	void SelectUserStyleNumber(UInt32 uiStyleNumber);
	void EnableDisplay(bool bOn);

	// Event processing
	//
public:
	void StartOfStream(void);
	void EndOfStream(void);
	virtual void Flush(void);
	void CompositionSegmentDecoded(UInt32 uiCSid);
	void SetSyncOffsetPTS(SInt64 pts);

protected:
	typedef enum tagIGKey
	{
		kIGKey_Up,
		kIGKey_Down,
		kIGKey_Left,
		kIGKey_Right
	} IGKey;

	void TimerEvent(bool bForceUpdate = false);
	void ProcessEvent(void);

	virtual bool GetCurrentTS(Time90K & ts90kTimestamp);			// returns current timestamp clock value
	UInt32 GetSleepDuration(void);

	void ProcessSelectUserStyle(UInt32 uiStyleNumber);
	void ProcessEnableDisplay(bool bOn);

	void ProcessCompositionSegmentDecoded(UInt32 uiCSid);
	virtual void ProcessTimerEvent(bool bForceUpdate = false);
	void ProcessRateChange(double dRate);

	// Composition rendering
	//
protected:
	void SetNextDPS(UInt32 DPSnum);
	virtual bool RenderSubtitle(UInt32 DPSnum, HDMVStreamPlaylistLocation & playlistLoc);
	virtual void RemoveSubtitle(UInt32 DPSnum);
	virtual void ClearDisplay(void)		{}
	void GetRegionBoundingRect(UInt8 regionId, UInt16 & l, UInt16 & t, UInt16 & r, UInt16 & b);
	virtual void EnableSubtitle(bool bEnable);
	void		 ApplyUserStyle(const txt_dialog_style_segment & pDSS, UInt32 rgnId, UInt32 rgnStyleId);

private:
	TextSTDecoder_GraphicsController(const TextSTDecoder_GraphicsController&); // no copying
	const TextSTDecoder_GraphicsController& operator=(const TextSTDecoder_GraphicsController&); // no copying

protected:
	typedef enum tagGCState
	{
		kGCState_Idle,				// Idle. No rendering possible
		kGCState_Reset,				// Reset and ready
		kGCState_SubtitleDisplay	// Subtitle being decoded and displayed (if display is enabled)
	} GCState;

protected:
	GCState					m_State;			// current state

	DPSDisplayInfo			m_PrevDPS;			// last DPS that was displayed
	DPSDisplayInfo			m_CurrDPS;			// curr DPS on display
	DPSDisplayInfo			m_NextDPS;			// next DPS to display

	UInt32					m_uiLastDPSnum;		// DPS Number (1 based) of the last DPS added to the composition buffer

	bool					m_bDisplayOn;		// display state
	UInt32					m_UserStyleNumber;	// User Style Number to use

	txt_region_style		m_workingStyle[2];	// two active region style are created from DSS::region_style, user_style_control and inline style

	SonicMutex				m_mtxPropChange;
	PiRect					m_VideoSize;				// size of Primary video
	bool					m_bVideoSizeChangePending;	// indicate if size has changed
	PiRect					m_NewVideoSize;				// new size of Primary video
};

#endif // __TEXTSTDECODER_GRAPHICSCONTROLLER_H__

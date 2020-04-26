//-----------------------------------------------------------------------------
// IGDecoder_GraphicsController.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __IGDECODER_GRAPHICSCONTROLLER_H__
#define __IGDECODER_GRAPHICSCONTROLLER_H__

#include "ref_ptr.h"
//#include "SonicThread.h"
//#include "SonicSemaphore.h"
#include <vector>
#include "BaseRenderer.h"
#include "HDMVDecoder_GraphicsController.h"
#include "HDMVDecoder_Types_Internal.h"
//#include "BDROMIndex.h"
#include "BDROM_SoundPlayer.h"
#include "BDROM_PTSAccess.h"

using namespace BDROM_HDMVDecoder;

class IGDecoder_CodedDataBuffer;
class IGDecoder_CompositionBuffer;
class IGDecoder_DecodedDataBuffer;


//__________________________________________________________________________________________________________________________________________________
// IG Decoder GC Events
//

const GCEventCode kGCEventBase_IG						= 0x02;

const GCEventCode kGCEvent_ButtonUp						= MAKE_GC_EVENTCODE(kGCEventBase_IG, 0);
const GCEventCode kGCEvent_ButtonDown					= MAKE_GC_EVENTCODE(kGCEventBase_IG, 1);
const GCEventCode kGCEvent_ButtonLeft					= MAKE_GC_EVENTCODE(kGCEventBase_IG, 2);
const GCEventCode kGCEvent_ButtonRight					= MAKE_GC_EVENTCODE(kGCEventBase_IG, 3);
const GCEventCode kGCEvent_ButtonActivate				= MAKE_GC_EVENTCODE(kGCEventBase_IG, 4);
const GCEventCode kGCEvent_ButtonSelect					= MAKE_GC_EVENTCODE(kGCEventBase_IG, 5);	// param 1: button num
const GCEventCode kGCEvent_ButtonSelectActivate			= MAKE_GC_EVENTCODE(kGCEventBase_IG, 6);	// param 1: button num
const GCEventCode kGCEvent_PopUpOn						= MAKE_GC_EVENTCODE(kGCEventBase_IG, 7);
const GCEventCode kGCEvent_PopUpOff						= MAKE_GC_EVENTCODE(kGCEventBase_IG, 8);
const GCEventCode kGCEvent_SetButtonPage				= MAKE_GC_EVENTCODE(kGCEventBase_IG, 9);	// param 1: page_id, param 2: curr page out effects state, param 3: button_id
const GCEventCode kGCEvent_BtnCmdsExecuted				= MAKE_GC_EVENTCODE(kGCEventBase_IG, 10);
const GCEventCode kGCEvent_EnableButton					= MAKE_GC_EVENTCODE(kGCEventBase_IG, 11);
const GCEventCode kGCEvent_DisableButton				= MAKE_GC_EVENTCODE(kGCEventBase_IG, 12);


//__________________________________________________________________________________________________________________________________________________
// ICS Display & Page Timing Data
//

class IGCompSegDisplay
{
public:
	IGCompSegDisplay()
		: m_bValid(false)
		, m_PageId(kPageId_Invalid)
		, m_PageVersion(kPageVersion_Invalid)
		, m_CompSegId(kCompSegId_Invalid)
		, m_CompSegPts90k(0)
		, m_CompSegTimeoutPts90k(0)
		, m_SelectionTimeoutPts90k(0)
		, m_UserTimeoutDuration90k(0)
		, m_UserTimeoutPlaylistPts90k(0)
	{}

	void Clear(void)
	{
		m_bValid = false;
		m_PageId = kPageId_Invalid;
		m_PageVersion = kPageVersion_Invalid;
		m_CompSegId = kCompSegId_Invalid;
		m_CompSegPts90k = m_CompSegTimeoutPts90k = m_SelectionTimeoutPts90k = 0;
		m_UserTimeoutDuration90k = m_UserTimeoutPlaylistPts90k = 0;
	}

	void SetValidity(bool bValid)			{ m_bValid = bValid; }
	bool IsValid(void)						{ return m_bValid; }

public:
	bool		m_bValid;					// true if this structure is valid

	UInt8		m_PageId;					// ID of Page
	UInt32		m_PageVersion;				// Page Version
	UInt32		m_CompSegId;				// ID of ICS containing the Page
	TimeValue	m_CompSegPts90k;			// ICS PTS
	TimeValue	m_CompSegTimeoutPts90k;		// ICS composition_timeout_pts. 0 if no timeout.  (valid only if StreamModel is Muxed)
	TimeValue	m_SelectionTimeoutPts90k;	// ICS selection_timeout_pts. 0 if no timeout.  (valid only if StreamModel is Muxed)
	TimeValue	m_UserTimeoutDuration90k;	// user_timeout_duration. 0 if invalid.
	TimeValue	m_UserTimeoutPlaylistPts90k;// User Timeout Playlist PTS (aka Media Time). 0 if no timeout. Must be updated after each user operation.
};

//__________________________________________________________________________________________________________________________________________________
// IGDecoder_GraphicsController
// The IG Decoder's GC
//

class IGDecoder_GraphicsController : public HDMVDecoder_GraphicsController
{
public:
	IGDecoder_GraphicsController();
	virtual ~IGDecoder_GraphicsController();

	void Init(ref_ptr<BaseRenderer> pRenderer, ref_ptr<BDROM_SoundPlayer> pSoundPlayer,
								  IGDecoder_CompositionBuffer * pCompositionBuffer, IGDecoder_DecodedDataBuffer * pDecodedDataBuffer,
								  ref_ptr<BDROM_PTSAccess> pPtsAccess);
	virtual void Reset(void);

	virtual void SetRenderDeviceState(DeviceState renderDeviceState);

	// user action processing
	//
public:
	void MoveUpSelectedButton();
	void MoveDownSelectedButton();
	void MoveLeftSelectedButton();
	void MoveRightSelectedButton();
	void SelectButton(UInt32 uiButtonId);
	void ActivateButton();
	void SelectButtonAndActivate(UInt32 uiButtonId);
	bool IsPopUpOn(void);
	void PopUpOn();
	void PopUpOff();
	void SetButtonPage(bool bSetPage, UInt8 uiPageId, bool bCurrPageOutEffect, bool bSetButton, UInt16 uiButtonId);
	void BtnCmdsExecuted(ButtonCmdsExecutionInfo & btnCmdsExecInfo);
	void EnableButton(UInt32 uiButtonId);
	void DisableButton(UInt32 uiButtonId);

	SonicError MousePosition(SInt32 x, SInt32 y, bool & bOnClickableButton);
	SonicError MouseClick(SInt32 x, SInt32 y);

	int GetTotalButtonNumOfCurPage() { return (m_pActivePage ? m_pActivePage->mnumber_of_BOGs : 0); } 
	// Event processing
	//
public:
	void CompositionSegmentDecoded(UInt32 uiCSid);
	void SetStreamModel(IGStreamModel streamModel)						{ m_StreamModel = streamModel; }
	void SetUserInterfaceModel(IGUserInterfaceModel userInterfaceModel)	{ m_UserInterfaceModel = userInterfaceModel; }
	virtual void Flush(void);

#ifdef REALTEK_MODIFY_ONLYONESEG
	void StartOfStream() {m_bEndOfStream = false;}
	void EndOfStream() 
	{
		SonicAutoCriticalSection Lock(&m_csLock);
		m_bEndOfStream = true; ProcessTimerEvent();
	}
#endif
	
protected:
	typedef enum tagIGKey
	{
		kIGKey_Up,
		kIGKey_Down,
		kIGKey_Left,
		kIGKey_Right
	} IGKey;

	typedef enum tagIGButtonStatesChangeReason
	{
		kBtnStatesChange_ButtonNavigation,
		kBtnStatesChange_ButtonActivate,
		kBtnStatesChange_ButtonSelectOnly
	} IGButtonStatesChangeReason;

	typedef enum tagIGPageEffectType
	{
		kIGPageEffectType_In,			// In Effects for Page
		kIGPageEffectType_Out,			// Out Effects for Page
	} IGPageEffectType;

	void TimerEvent(bool bForceUpdate = false);
	virtual void ProcessEvent(void);

	virtual bool GetCurrentTS(Time90K & ts90kTimestamp);			// returns current timestamp clock value
	UInt32 GetSleepDuration(void);

	void ProcessCompositionSegmentDecoded(UInt32 uiCSid);
	virtual void ProcessTimerEvent(bool bForceUpdate = false);
	void ProcessButtonNavigation(IGKey direction);
	virtual void ButtonStatesChanged(IGButtonStatesChangeReason btnStateChangeReason, void * pInfo = 0);
	void ProcessButtonActivate(void);
	void ProcessButtonSelect(UInt32 uiBtnId);
	void ProcessButtonSelectAndActivate(UInt32 uiBtnId);
	virtual void ProcessSetButtonPage(bool bSetPage, UInt8 uiPageId, bool bCurrPageOutEffect, bool bSetButton, UInt16 uiButtonId);
	void ProcessBtnCmdsExecuted(ButtonCmdsExecutionInfo & btnCmdsExecInfo);
	void ProcessPopUpOnOff(bool bPopUpOn);
	void ProcessEnableButton(UInt32 uiButtonId);
	void ProcessDisableButton(UInt32 uiButtonId);
	SonicError ProcessMouseOperation(SInt32 x, SInt32 y, bool bClicked, bool & bOnClickableButton);
	void ProcessRateChange(double dRate);
	void ProcessFakeMouseMove();


	// button functions
	//
	void ParseNavigationCommand(ig_navigation_command & igNavCmdBytes, BDROM::navigation_command & navCmd);
	/*
	void GetBOGBoundingRect(UInt16 bogId, UInt16 & l, UInt16 & t, UInt16 & r, UInt16 & b);
	*/

	// Composition rendering
	//
protected:
	void PerformDefaultPSR10Updation(void);								// updates PSR10 based on logic on sec 5.9.8 of the spec
	virtual bool SwitchPage(UInt8 uiPageId, bool bCurrPageOutEffect, UInt16 uiButtonId);
	virtual bool SetPage(UInt8 uiPageId, UInt16 uiButtonId);
	virtual bool SetupPage(UInt8 uiPageId, UInt16 uiButtonId);
	virtual bool SetupPageButtons(void);

	void SetButtonStateSelected(UInt16 selBtnId);						// selects btn, updates PSR and current page members

	virtual bool RenderPage(void);										// first display of a page. returns false if page was not fully rendered
	virtual void AnimatePage(void);										// Animates buttons in the active page
	virtual bool RenderPageEffects(UInt32 uiPageId, IGPageEffectType effectType);	// displays In/Out effects of a page.
	void SetButtonState(ig_button * pBtn, IGButtonState newState, bool bForceBtnDisplayUpdate = false);		// changes button state and updates associated sprites
	virtual void RemoveActivePage(bool bClearDisplay, bool bNextPageAvailable);	// removes active page, if any, from display. Active page is undefined after this.
	virtual void ClearDisplay(void);									// clear IG display
	bool ButtonSelectOnly(UInt32 uiBtnId);								// selects button, but does not activate it. Returns true if a new btn is selected
	bool AutoActivateButton(void);										// will process any auto_activate button in page or specified one (RenderPage will not do this)
	void ProcessSelectionTimeout(void);
	void ProcessUserTimeout(void);
	void UpdateUserTimeoutCount(void);

	void SendPageEvents(bool bPageDisplayed);							// sends active page displayed and UOPs or 'no menu' and 'no UOPs' messages

	void BuildMenuPageInfo(void);										// updates our MenuPageButtonInfo for current page.
	void SetMenuPageInfoUsability(bool bUsable);						// sets usability of MenuPageButtonInfo for mouse input
	void UpdateMenuPageInfoBtnState(UInt16 buttonId, IGButtonState btnState);	// updates button info in current MenuPageButtonInfo.

	void FillNextICS(UInt32 nextICSid);

private:
	IGDecoder_GraphicsController(const IGDecoder_GraphicsController&); // no copying
	const IGDecoder_GraphicsController& operator=(const IGDecoder_GraphicsController&); // no copying

protected:
	typedef enum tagGCState
	{
		kGCState_Idle,				// Idle. No rendering possible
		kGCState_Reset,				// Reset and ready

		kGCState_NewPage,			// There is a new page to be set/switched to. do any out-effect of old page and in-effects of new page (= kGCState_PageStart)
		kGCState_PageSet,			// active page set, but no display update yet
		kGCState_PageDisplayed,		// first rendering of active page done. Any further animations, updates will continue to be done
	} GCState;

protected:
	IGStreamModel			m_StreamModel;			// IG stream type - Muxed or Not.  (Muxed can mean either in Main Path or in Out-of-Mux Subpath)
	IGUserInterfaceModel	m_UserInterfaceModel;	// User Interface Model

	UInt8					m_uiNextActivePageId;	// page to make active next
	UInt8					m_uiActivePageId;		// active page ID
	UInt32					m_uiActivePageICSid;	// ICS that contains active page ID
	ig_page *				m_pActivePage;			// pointer to active page
	UInt32					m_LastButtonCmdSetId;	// unique ID set for the previous set of button commands;

	ref_ptr<BDROM_SoundPlayer>	m_pSoundPlayer;		// BDROM Nav's Sound Player

	UInt32					m_uiVideoFrameRate;		// current video frame rate (frames per second)

	GCState					m_State;				// current state
	bool					m_bNextTSValid;			// true if m_nextTS is a valid value
	TimeValue				m_nextTS;				// time at which next operation (e.g., ICS rendering) should be done. This is a 90KHz value.

	IGCompSegDisplay		m_NextICS;
	IGCompSegDisplay		m_CurrICS;
	IGCompSegDisplay		m_PrevICS;
	std::vector<UInt32>		m_vecNextICSid;

	bool					m_bPopUpOn;				// PopUps display state, i.e., whether or not IG/PopUp should be displayed
	bool					m_bIGDisplayed;			// Current state of IG Display

	SonicCriticalSection 	m_csPageInfoLock;		// Lock to serialize access to Page Info structure
	MenuPageButtonInfo	*	m_pButtonInfo;			// Current Page's button info (used for handling mouse events)

	PiRect					m_VideoSize;			// size of video that this IG is associaed with.

	SInt32				    m_lastXValid;		    // previous x position of mouse ptr that was valid
	SInt32				    m_lastYValid;			// previous y position of mouse ptr that was valid
	SInt32				    m_lastXPending;			// previous x position of mouse ptr that was pending
	SInt32				    m_lastYPending;			// previous y position of mouse ptr that was pending
	bool                    m_lastMouseOpPending;   // Indicate whether the previous mouse operation is performed 	    
#ifdef REALTEK_MODIFY_ONLYONESEG
	bool                    m_bEndOfStream;
#endif	
};

#endif // __IGDECODER_GRAPHICSCONTROLLER_H__

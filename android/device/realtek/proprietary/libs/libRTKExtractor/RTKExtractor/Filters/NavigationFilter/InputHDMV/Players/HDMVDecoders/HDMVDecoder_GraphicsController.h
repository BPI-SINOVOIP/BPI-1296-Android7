//-----------------------------------------------------------------------------
// HDMVDecoder_GraphicsController.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_GRAPHICSCONTROLLER_H__
#define __HDMVDECODER_GRAPHICSCONTROLLER_H__

#include "ref_ptr.h"
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	#include "ThreadPriority.h"
#else
	#include "SonicThread.h"
#endif	
#include "SonicSemaphore.h"
#include "SonicCriticalSection.h"
#include <vector>
#include "BaseRenderer.h"				
#include "NotificationEventManager.h"
#include "SonicPBCE_Registers.h"
#include "HDMVDecoder_Types_Internal.h"
#include "BDROM_PTSAccess.h"
#include "HDMVDecoders_DeviceLost.h"

using namespace BDROM_HDMVDecoder;

class HDMVDecoder_CompositionBuffer;
class HDMVDecoder_DecodedDataBuffer;


//__________________________________________________________________________________________________________________________________________________
// Common GC Events
// GC Events codes are made up of a 8 bit Event base OR-ed with a 24 Event Number.
// Event Base for common events ia 0x1. Derived classes can define their own Event base to create unique messages
//

typedef UInt32 GCEventCode;

const GCEventCode kGCEventBase_Common					= 0x01;
#define MAKE_GC_EVENTCODE(base, evt)					((base << 24) | evt);

const GCEventCode kGCEvent_None							= MAKE_GC_EVENTCODE(kGCEventBase_Common, 0);
const GCEventCode kGCEvent_Stop							= MAKE_GC_EVENTCODE(kGCEventBase_Common, 1);
const GCEventCode kGCEvent_CompositionSegmentDecoded	= MAKE_GC_EVENTCODE(kGCEventBase_Common, 2);
const GCEventCode kGCEvent_Timer						= MAKE_GC_EVENTCODE(kGCEventBase_Common, 3);
const GCEventCode kGCEvent_RateChange					= MAKE_GC_EVENTCODE(kGCEventBase_Common, 4);

struct GCEvent
{
public:
	GCEvent()
	{
		mEventCode = kGCEvent_None;
	}

	GCEvent(GCEventCode evtCode)
	{
		mEventCode = evtCode;
	}

public:
	GCEventCode		mEventCode;

	union
	{
		// for SetButtonPage cmd
		struct
		{
			bool			mbSetPage;
			UInt8			mPageId;
			bool			mbCurrPageOutEffects;
			bool			mbSetButton;
			UInt16			mButtonId;
		} SetButtonPage;

		// for EnableButton, DisableButton, SelectButton, SelectAndActivateButton
		struct
		{
			UInt16			mButtonId;
		} ButtonAction;

		// for BtnCmdsExecuted.
		struct
		{
			ButtonCmdsExecutionInfo		mBtnCmdsExecInfo;
		} ButtonCmd;

		// for PopUpOn/PopUpOff/PopUpToggle, PG Display state
		struct
		{
			bool			mDisplayOn;
			bool			mForceUpdate;	// Used with TimerEvent to force update of current display (if display is on) even if nothing has changed
		} Display;

		// for SetRate
		struct
		{
			double			mRate;
		} Rate;

		// For SegmentDecoded
		struct
		{
			UInt32			mCSid;					// composition_segment id (PCS id / ICS id)
		} SegDecode;

		// for SelectUserStyleNumber
		struct
		{
			UInt32			mStyleNumber;
		} TextStyle;
	} params;
};


//__________________________________________________________________________________________________________________________________________________
// HDMVDecoder_GraphicsController
// The GC base class for PG/IG decoder GC
//
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
class HDMVDecoder_GraphicsController
#else
class HDMVDecoder_GraphicsController : public SonicThread
#endif	
{
public:
	HDMVDecoder_GraphicsController();
	virtual ~HDMVDecoder_GraphicsController();

	void Init(ref_ptr<BaseRenderer> pRenderer,
									HDMVDecoder_CompositionBuffer * pCompositionBuffer,
									HDMVDecoder_DecodedDataBuffer * pDecodedDataBuffer,
									Priority priority,
									ref_ptr<BDROM_PTSAccess> pPtsAccess);

	void SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager);
	void SetRegisters(ref_ptr<BDROM_SonicPBCE_Registers> pRegisters);

	void SendEvent(BDROM_Event event);

	virtual void Reset(void);
	virtual void SetRenderDeviceState(DeviceState renderDeviceState);

	// stream properties
	virtual void SetApplicationType(BDROM_ApplicationType appType)		{ m_ApplicationType = appType; }
	virtual void SetSyncOffsetPTS(SInt64 pts)							{ m_SyncOffsetPTS = pts; }
	virtual void SetCharacterSet(BDROM_CharSet charSet)					{ m_CharSet = charSet; }

	// Thread and GC control
	//
public:
	SonicError ThreadExit(void);
	SonicError ThreadStart(void);

#ifdef 	REALTEK_MODIFY_DISABLE_SONICTHREAD
	static void ThreadEntry(void* pInstance);
#else	
	SonicError ThreadStop(void);
	
protected:
	SonicError Run(void);

#endif	

protected:
	void Process(void);
	virtual void ProcessEvent(void);

	// Event processing
	//
public:
	virtual void CompositionSegmentDecoded(UInt32 uiCSid)				= 0;	// call from SGP telling GC that a composition segment was decoded
	virtual void Play(void);
	virtual void Pause(void);
	virtual void Stop(void);
	virtual void Flush(void);
	virtual void SetRate(double dRate);
	void SetMetadata(HDMVPrivateMetadata & metadata);
protected:
	virtual void TimerEvent(bool bForceUpdate = false)					= 0;	// Generates event to wake up thread
	TimeValue StreamPtsToPlaylistPts(TimeValue encodedPts90k);
	virtual bool GetCurrentTS(Time90K & ts90kTimestamp);						// returns current timestamp clock value
	bool GetCurrentPlaylistPts(HDMVStreamPlaylistLocation & loc);				// returns current location in main path
	virtual UInt32 GetSleepDuration(void)								= 0;	// returns duration to sleep (if at all) before next timer event is to be generated
	virtual void ProcessCompositionSegmentDecoded(UInt32 uiCSid)		= 0;
	virtual void ProcessTimerEvent(bool bForceUpdate = false)			= 0;	// Processes next event or whatever else is necessary
	virtual void ProcessRateChange(double dRate)						= 0;
	void ProcessMetadata(TimeValue playlistPts90k);

protected:
	UInt32 GetPSR(UInt32 uiPSR);
	void   SetPSR(UInt32 uiPSR, UInt32 uiValue);
	bool   CropAndWindow(bool bCrop, hdmv_cropping_rectangle * pCropRect,
						 bool bWnd, hdmv_window * pWndRect,
						 PiRect & rawRect, PiRect & objSrcRect, PiPoint & objDstPos);

	virtual void RendererPrepare(void)									{ m_pRenderer->Prepare(); }
	virtual void RendererCommit(void)									{ m_pRenderer->Commit(); }

protected:
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	char*								m_ThreadName;				// Name of this thread
	Priority 							m_Priority;
	CThread*							m_pThread;
#else	
	std::string							m_ThreadName				// Name of this thread
#endif
	ref_ptr<BaseRenderer>				m_pRenderer;
	DeviceState							m_RenderDeviceState;		// state of render device. determines if we can render or not

	ref_ptr<BDROM_PTSAccess>			m_pPtsAccess;				// object for PTS retrieval

	HDMVDecoder_CompositionBuffer	*	m_pCompositionBuffer;
	HDMVDecoder_DecodedDataBuffer	*	m_pDecodedDataBuffer;

	bool								m_bProcess;
	bool								m_bExit;

	SonicCriticalSection 				m_csQueueLock;				///< Lock to serialize internal queue access
	SonicCriticalSection 				m_csLock;					///< Lock to serialize threads accessing/modifying intenal data

	SonicSemaphore						m_semEventLock;				///< Signals the availability of a new event
	SonicSemaphore						m_semEventExecLock;			///< Signals that the current event has been fully executed.
	GCEvent								m_CurrEvent;
	std::vector<GCEvent>				m_EventQueue;
	SInt32								m_siTimerEventsPending;

	ref_ptr<NotificationEventManager>	m_pNEManager;
	ref_ptr<BDROM_SonicPBCE_Registers>	m_pRegisters;

	BDROM_ApplicationType				m_ApplicationType;			// application type of clip providing stream data
	SInt64								m_SyncOffsetPTS;			// offset PTS to be added to Stream PTS to sync this stream with main path
	BDROM_CharSet						m_CharSet;					// character set (if any) associated with stream data

	double								m_dRate;					///< Current playback rate.
	double								m_dPendingRate;				///< New rate to change to when a rate change event is received

	bool								m_bStreamExtentValid;		///< m_StreamExtent valid or not
	HDMVStreamExtent					m_StreamExtent;				///< current extent of stream
	bool								m_bStartLocationValid;		///< m_StartLocation valid or not
	HDMVStreamPlaylistLocation			m_StartLocation;			///< Start location of playback (updated on non-seamless transitions only)

	HDMVPrivateMetadataList				m_PendingMetadataList;		///< list of metadata (e.g. new stream extent) that is to be applied in the future

private:
	HDMVDecoder_GraphicsController(const HDMVDecoder_GraphicsController&); // no copying
	const HDMVDecoder_GraphicsController& operator=(const HDMVDecoder_GraphicsController&); // no copying
};

#endif // __HDMVDECODER_GRAPHICSCONTROLLER_H__

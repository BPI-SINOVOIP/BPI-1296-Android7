//-----------------------------------------------------------------------------
// HDMVDecoder_StreamGraphicsProc.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_STREAMGRAPHICSPROC_H__
#define __HDMVDECODER_STREAMGRAPHICSPROC_H__

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
#include "BDROM_NavigatorTypes.h"
#include "HDMVDecoder_EB.h"
#include "HDMVDecoder_Types_Internal.h"
#include "BDROM_NavigatorTypes.h"
#include "BDROM_PTSAccess.h"


using namespace BDROM_HDMVDecoder;

class HDMVDecoder_CodedDataBuffer;
class HDMVDecoder_CompositionBuffer;
class HDMVDecoder_DecodedDataBuffer;
class HDMVDecoder_GraphicsController;


//__________________________________________________________________________________________________________________________________________________
// Common SGP Events
// SGP Events codes are made up of a 8 bit Event base OR-ed with a 24 Event Number.
// Event Base for common events ia 0x1. Derived classes can define their own Event base to create unique messages
//

typedef UInt32 SGPEventCode;

const SGPEventCode kSGPEventBase_Common					= 0x01;
#define MAKE_SGP_EVENTCODE(base, evt)					((base << 24) | evt);

const SGPEventCode kSGPEvent_None						= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 0);
const SGPEventCode kSGPEvent_Stop						= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 1);
const SGPEventCode kSGPEvent_SegmentReceived			= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 2);
const SGPEventCode kSGPEvent_Timer						= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 3);
const SGPEventCode kSGPEvent_StartOfStream				= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 4);
const SGPEventCode kSGPEvent_EndOfStream				= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 5);
const SGPEventCode kSGPEvent_Flush						= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 6);
const SGPEventCode kSGPEvent_RateChange					= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 7);
const SGPEventCode kSGPEvent_EnableDisplay				= MAKE_SGP_EVENTCODE(kSGPEventBase_Common, 8);

struct SGPEvent
{
public:
	SGPEvent()
	{
		mEventCode = kSGPEvent_None;
	}

	SGPEvent(SGPEventCode evtCode)
	{
		mEventCode = evtCode;
	}

public:
	SGPEventCode	mEventCode;

	union
	{
		// for enable/display Display state
		struct
		{
			bool			mDisplayOn;
		} Display;

		// for SetRate
		struct
		{
			double			mRate;
		} Rate;
	} params;
};

//__________________________________________________________________________________________________________________________________________________
// HDMVDecoder_StreamGraphicsProcessor
// The SGP base class for PG/IG decoder SGP
//
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
class HDMVDecoder_StreamGraphicsProcessor
#else
class HDMVDecoder_StreamGraphicsProcessor : public SonicThread
#endif	
{
public:
	HDMVDecoder_StreamGraphicsProcessor();
	virtual ~HDMVDecoder_StreamGraphicsProcessor();

	void Init(ref_ptr<BaseRenderer> pRenderer,
										HDMVDecoder_CodedDataBuffer * pCodedDataBuffer,
										HDMVDecoder_CompositionBuffer * pCompositionBuffer,
										HDMVDecoder_DecodedDataBuffer * pDecodedDataBuffer,
										HDMVDecoder_GraphicsController * pGraphicsController,
										Priority priority,
										ref_ptr<BDROM_PTSAccess> pPtsAccess);
	virtual void Reset(void);

	// stream properties
	virtual void SetApplicationType(BDROM_ApplicationType appType)		{ m_ApplicationType = appType; }
	virtual void SetSyncOffsetPTS(SInt64 pts)							{ m_SyncOffsetPTS = pts; }
	virtual void SetCharacterSet(BDROM_CharSet charSet)					{ m_CharSet = charSet; }

	// Thread and SGP control
	//
public:
	SonicError ThreadExit(void);
	SonicError ThreadStart(void);
	
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD	
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
	virtual void SegmentReceived(void)					= 0;		// HDMV Graphics segment received
	virtual void StartOfStream(void)					= 0;		// Start of a new data stream. called before any data is received
	virtual void EndOfStream(void)						= 0;		// End of stream. called after all data is received
	virtual void Play(void);
	virtual void Pause(void);
	virtual void Stop(void);
	virtual void Flush(void);
	virtual void SetRate(double dRate);
	virtual void SetStartLocation(HDMVStreamPlaylistLocation & startLocation);
protected:
	virtual void TimerEvent(void)						= 0;
	TimeValue StreamPtsToPlaylistPts(TimeValue encodedPts90k);
	TimeValue PlaylistPtsToStreamPts(TimeValue playlistPts90k);
	virtual bool GetCurrentTS(Time90K & ts90kTimestamp);			// returns current timestamp clock value
	        bool GetCurrentPlaylistPts(HDMVStreamPlaylistLocation & loc);	// returns current location in main path
	virtual UInt32 GetSleepDuration(void)				= 0;		// returns duration to sleep (if at all) before another timer event is to be generated
	virtual void ProcessSegmentReceived(void)			= 0;
	virtual void ProcessTimerEvent(void)				= 0;
	virtual void ProcessStartOfStream(void)				= 0;
	virtual void ProcessEndOfStream(void)				= 0;
	virtual void ProcessFlush(void)						= 0;
	virtual void ProcessRateChange(double dRate)		= 0;

	// stream parsing
	//
protected:
	void ParseSegmentDescriptor(hdmv_segment_descriptor * pSegDesc, HDMVDec_EBPtr & ebPtr);
	void ParseVideoDescriptor(hdmv_video_descriptor * pVidDesc, HDMVDec_EBPtr & ebPtr);
	void ParsePaletteDefSegment(hdmv_palette_definition_segment * pPDS, HDMVDec_EBPtr & ebPtr);
	void ParseCompositionDescriptor(hdmv_composition_descriptor * pCompDesc, HDMVDec_EBPtr & ebPtr);
	void ParseSequenceDescriptor(hdmv_sequence_descriptor * pSeqDesc, HDMVDec_EBPtr & ebPtr);
	void ParseWindow(hdmv_window * pWindow, HDMVDec_EBPtr & ebPtr);
	void ParseCompositionObject(hdmv_composition_object * pCompObj, HDMVDec_EBPtr & ebPtr);
	void ParseObjectDefinitionSegment(hdmv_object_definition_segment * pODS, HDMVDec_EBPtr & ebPtr);
	void ParseObjectData(hdmv_object * pObject, HDMVDec_EBPtr & ebPtr);

	virtual void DecodeObjectData(hdmv_object * pObject, HDMVDec_EBPtr & ebPtr);

protected:
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	char*						m_ThreadName;				// Name of this thread
	Priority					m_Priority;
	CThread*					m_pThread;
#else	
	std::string					m_ThreadName				// Name of this thread
#endif

	ref_ptr<BaseRenderer>		m_pRenderer;

	ref_ptr<BDROM_PTSAccess>	m_pPtsAccess;				// object for PTS retrieval

	HDMVDecoder_CodedDataBuffer		*	m_pCodedDataBuffer;
	HDMVDecoder_CompositionBuffer	*	m_pCompositionBuffer;
	HDMVDecoder_DecodedDataBuffer	*	m_pDecodedDataBuffer;
	HDMVDecoder_GraphicsController	*	m_pGraphicsController;

	bool						m_bProcess;
	bool						m_bExit;

	SonicCriticalSection 		m_csQueueLock;				///< Lock to serialize internal queue access
	SonicCriticalSection 		m_csLock;					///< Lock to serialize threads accessing/modifying intenal data

	SonicSemaphore				m_semEventLock;
	SGPEvent					m_CurrEvent;
	std::vector<SGPEvent>		m_EventQueue;
	SInt32						m_siTimerEventsPending;

	bool						m_bTimerEventPending;
	HDMVDec_EBSegment			m_CurrSeg;
	HDMVDec_EBPtr				m_EBPtr;

	BDROM_ApplicationType		m_ApplicationType;			// application type of clip providing stream data
	SInt64						m_SyncOffsetPTS;			// offset PTS to be added to encoded PTS to sync this stream with main video.
	BDROM_CharSet				m_CharSet;					// character set (if any) associated with stream data

	double						m_dRate;					///< Current playback rate.
	double						m_dPendingRate;				///< New rate to change to when a rate change event is received

	HDMVStreamExtent			m_StreamExtent;				///< current extent of stream
	HDMVStreamPlaylistLocation	m_StartLocation;			///< Start location of playback (updated on non-seamless transitions only)

private:
	HDMVDecoder_StreamGraphicsProcessor(const HDMVDecoder_StreamGraphicsProcessor&); // no copying
	const HDMVDecoder_StreamGraphicsProcessor& operator=(const HDMVDecoder_StreamGraphicsProcessor&); // no copying
};

#endif // __HDMVDECODER_STREAMGRAPHICSPROC_H__

//-----------------------------------------------------------------------------
// PTSCallbackHandler.h
// Copyright (c) 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __PTSCALLBACKHANDLER_H_
#define __PTSCALLBACKHANDLER_H_

#include "ref_ptr.h"
#include "SonicCriticalSection.h"
#include "SonicSemaphore.h"
#include "SonicMutex.h"
#include "NotificationEventManager.h"
#include "TimeValue.h"
#include "BDROMPlaylist.h"
#include "BaseRenderer.h"


#include "ThreadPriority.h"

#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	#include <OSAL.h>
#endif
//__________________________________________________________________________________________________________________________________________________
// Defines, Macros, conditional includes, ...
//

//--------------------------------------------------------------------------
// Compile symbols
//



namespace BDROM
{

//__________________________________________________________________________________________________________________________________________________
// Playlist PTS data
//

//--------------------------------------------------------------------------
// PlayitemPtsMap: Stores 1 PlayItem's PTS info
//
class PlayitemPtsMap
{
public:
	PlayitemPtsMap() : m_InPts90k(0), m_OutPts90k(0), m_InMediaTime90k(0)	{}
	~PlayitemPtsMap()	{}

	void Clear(void)		{ m_InPts90k = m_OutPts90k = 0; m_InMediaTime90k = 0; }

	void SetInPts45k(const Time90K inPts45k)				{ m_InPts90k = (TimeValue)inPts45k << 1; }
	void SetOutPts45k(const Time90K outPts45k)				{ m_OutPts90k = (TimeValue)outPts45k << 1; }
	void SetInMediaTime45k(const TimeValue inMediaTime45k)	{ m_InMediaTime90k = (TimeValue)inMediaTime45k << 1; }

	void SetInPts90k(const TimeValue inPts90k)				{ m_InPts90k = inPts90k; }
	void SetOutPts90k(const TimeValue outPts90k)			{ m_OutPts90k = outPts90k; }
	void SetInMediaTime90k(const TimeValue inMediaTime90k)	{ m_InMediaTime90k = inMediaTime90k; }


	TimeValue GetDuration45k(void)			{ return (m_OutPts90k - m_InPts90k) >> 1; }
	TimeValue GetInPts45k(void)				{ return m_InPts90k >> 1; }
	TimeValue GetOutPts45k(void)			{ return m_OutPts90k >> 1; }
	TimeValue GetInMediaTime45k(void)		{ return m_InMediaTime90k >> 1; }

	TimeValue GetDuration90k(void)			{ return (m_OutPts90k - m_InPts90k); }
	TimeValue GetInPts90k(void)				{ return m_InPts90k; }
	TimeValue GetOutPts90k(void)			{ return m_OutPts90k; }
	TimeValue GetInMediaTime90k(void)		{ return m_InMediaTime90k; }

protected:
	TimeValue	m_InPts90k;
	TimeValue	m_OutPts90k;
	TimeValue	m_InMediaTime90k;		// InMediaTime[n] = Sum Of (Duration[0] ... Duration[n-1]). All times in 45K
};


//--------------------------------------------------------------------------
// PlaylistPtsMap: Stores PlayList's PTS info
//
class PlaylistPtsMap
{
public:
	PlaylistPtsMap();
	~PlaylistPtsMap();

	void Clear(void);
	bool CreatePtsMap(BDROM::PlayList_mpls * pPlaylistMpls);

	UInt32           GetNumPlayitems(void);
	TimeValue        GetPlaylistDuration45k(void);
	TimeValue        GetPlaylistDuration90k(void);
	PlayitemPtsMap * GetPlayitemPtsMap(UInt32 playitemId);
	bool             GetPlayitemPtsMap(UInt32 playitemId, PlayitemPtsMap & playitemData);

protected:
	bool SetNumPlayitems(UInt32 numPlayitems);

protected:
	UInt32				m_NumPlayitems;
	PlayitemPtsMap	*	m_pPlayitemPts;
};


//__________________________________________________________________________________________________________________________________________________
// PTS Callback Notifier
// Thread that sends PTS events to Nav (PBCE and BD-J module)
//
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
class PTSCallbackNotifier
#else
class PTSCallbackNotifier: public SonicThread
#endif	
{
public:
	PTSCallbackNotifier();
	~PTSCallbackNotifier();
	void Init(ref_ptr<NotificationEventManager> pNEManager);
	void DeInit(void);

	
#ifdef 	REALTEK_MODIFY_DISABLE_SONICTHREAD
	Priority GetPriority(void)		{ return Priority_Normal; }
	SonicError Run(void);

	static void ThreadEntry(void* pInstance);
#else	
	virtual Priority GetPriority(void)		{ return Priority_Normal; }
	virtual SonicError Run(void);

#endif

public:
	void SendTimePulseEvent(void);
	void SendMediaTimeEvent(Time90K mediaTime45k);

protected:
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	char*								m_ThreadName;
#else
	std::string							m_ThreadName;				// Name of this thread
#endif
	ref_ptr<NotificationEventManager>			m_pNEManager;

	bool								m_bExit;
	SonicSemaphore						m_semNewEvent;
	SonicCriticalSection				m_csEventProcLock;			// CritSec held by run() when processing an event

	SonicMutex							m_mtxEvents;				// Mutex to be held when accessing following data members
	bool								m_bTimePulsePending;		// Time Pulse Event to be sent
	bool								m_bMediaTimePending;		// Media Time Event to be sent
	Time90K								m_MediaTime45k;				// Media Time for the Media Time Event

#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	CThread* 								m_pThread;
#endif
};


//__________________________________________________________________________________________________________________________________________________
// PTS Callback Handler
// Processes PTS sent by HDR. Uses PTSCallbackNotifier to send events
//
#ifdef 	REALTEK_MODIFY_DISABLE_SONICTHREAD
class PTSCallbackHandler: public PTSCallback
#else
class PTSCallbackHandler: public PTSCallback, public SonicThread
#endif	
{
public:
	PTSCallbackHandler();
	~PTSCallbackHandler();
	void Init(ref_ptr<BaseRenderer> pRenderer, ref_ptr<NotificationEventManager> pNEManager);
	void DeInit(void);

	virtual Priority GetPriority(void)		{ return Priority_Highest; }
	virtual SonicError Run(void);


#ifdef 	REALTEK_MODIFY_DISABLE_SONICTHREAD
	static void ThreadEntry(void* pInstance);
#endif

	// PTS Callback interface
public:
    virtual void NewPTS(TimeValue const & platformPts, TimeValue const & encodedPts, bool bInterpolated);

	// PTS Callback Handler setup and query
public:
	// SetPlaylistPtsMap: Sets new playlist map for the handler
	void SetPlaylistPtsMap(ref_ptr<PlaylistPtsMap> pPlaylistPtsMap);

	// SetMinPTSDiff: Sets min difference between new and curr pts (in 45 KHz). Anything lesser & interpolated will be ignored.
	void SetMinPTSDiff(UInt32 ptsDiff45k);

	// SetTimePulseFrequency: Sets frequency of TimePulse events generated
	void SetTimePulseFrequency(UInt32 pulsesPerSec);

	// StartHandler: Provides a starting point for the PTSCallbackHandler. Must be called after every jump (or after StopHandler call)
	void StartHandler(UInt32 playitemId, Time90K startTime45k, double dRate);

	// StopHandler: Stops handling PTSs. Makes curr pts invalid.
	void StopHandler(void);

	// SetRate: Updates rate info
	void SetRate(double dRate);


	// GetPlaylistPTS: Gets current PTS (90 KHz) as a PTS in playlist timeline (aka Media Time)
	bool GetPlaylistPTS(TimeValue & playlistPts90k);
	bool GetPlaylistPTS(TimeValue & playlistPts90k, TimeValue & platformPts90k);

	// GetPTS: Gets current playitem Id and PTS (90 KHz) within the playitem
	bool GetPTS(UInt32 & currPlayitemId, TimeValue & currPts90k);

	// GetPTS: Gets current playitem Id, PTS (90 KHz), and Platform PTS within the playitem
	bool GetPTS(UInt32 & currPlayitemId, TimeValue & currPts90k, TimeValue & currPlatformPts);


	typedef enum tagPTSCallbackState
	{
		kPCS_Stopped,		// waiting for a start call. No Valid PTS available. All PTS callbacks will be ignored. No events will be generated.
		kPCS_Starting,		// Inital PTS provided. Processing PTS Callbacks (a few initial callbacks may contain invalid values)
		kPCS_Running,		// First valid PTS Callback received.
		kPCS_Done,			// End of playlist. Last PTS is still valid. All PTS callbacks will be ignored. No events will be generated.
	} PTSCallbackState;

	
	void ForceStartHandler();
	PTSCallbackState GetCallbackHanderState();

public:
	// Additional APIs
	//
	
	// PiP Position API
	void ClearPiPMetadata(void)
	{
		m_mtxPiPMetadata.lock();
		m_bPendingPiPMetadata = false;
		m_mtxPiPMetadata.unlock();
	}
	void SetPiPMetadata(const TimeValue metadataPlaylistPts90k, const PiRect & destRect, const bool bFullScreen, const int lumaKey)
	{
		m_mtxPiPMetadata.lock();
		m_PiPMetadataPlaylistPts90k = metadataPlaylistPts90k;
		m_PiPDestRect = destRect;
		m_bPiPFullScreen = bFullScreen;
		m_PiPLumaKey = lumaKey;
		m_bPendingPiPMetadata = true;
		m_mtxPiPMetadata.unlock();
	}
	bool IsPiPMetadataPending(void)
	{
		m_mtxPiPMetadata.lock();
		bool bPending = m_bPendingPiPMetadata;
		m_mtxPiPMetadata.unlock();
		return bPending;
	}

private:

	typedef enum tagPTSCallbackEvent
	{
		kPCE_NewPts,		// New Pts event
		//kPCE_NewState,		// State change event
		kPCE_Exit,			// exit thread
	} PTSCallbackEvent;

	struct HandlerPts
	{
		HandlerPts() : m_PlatformPts(0), m_EncodedPts90k(0), m_bInterpolated(true), m_PlayitemId(0xFFFF)
		{
		}

		void Clear(void)
		{
			m_PlatformPts = m_EncodedPts90k = 0;
			m_bInterpolated = true;
			m_PlayitemId = 0xFFFF;
		}

		TimeValue							m_PlatformPts;
		TimeValue							m_EncodedPts90k;
		bool								m_bInterpolated;
		UInt32								m_PlayitemId;			// playitem ID that the current Pts falls in. 0xFFFF is invalid.
	};

protected:
	void ProcessPts(HandlerPts & newPts);
	void UpdatePts(HandlerPts & newPts, bool bForceSendTimePulse = false, bool bForceSendMediaTime = false);
	void UpdatePlayitemId(HandlerPts & newPts);

protected:
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	char*								m_ThreadName;
#else	
	std::string							m_ThreadName;				// Name of this thread
#endif		
	ref_ptr<BaseRenderer>						m_pRenderer;
	ref_ptr<NotificationEventManager>	m_pNEManager;

	PTSCallbackNotifier				*	m_pNotifier;

	bool								m_bExit;
	SonicSemaphore						m_semNewEvent;
	SonicCriticalSection				m_csEventProcLock;			// CritSec held by run() when processing an event

	SonicMutex							m_mtxState;					// mutex to be held when updating any of the following HandlerState members.
	PTSCallbackState					m_State;
	BDROM_PlayDirection					m_Direction;
	bool								m_bScanning;
	TimeValue							m_MinPtsDiff90k;
	TimeValue							m_TimePulsePtsDiff90k;
	ref_ptr<PlaylistPtsMap>				m_pPlaylistPtsMap;
	TimeValue							m_PlatformPtsfreq;			// Platform PTS clock frequency
	double								m_90kToPlatformPtsScaler;	// scaler to conver a 90 Khz PTS to a Platform PTS

	SonicMutex							m_mtxPts;					// mutex to be held when updating any of the following HandlerPts members.
	HandlerPts							m_CurrPts;					// current PTS. This is what is reported by GetPts()
	TimeValue							m_PlaylistPts90k;			// 90K Playlist PTS (aka Media Time). Valid when m_CurrPts is valid.
	HandlerPts							m_LastRealPts;				// last 'current PTS' that was not interpolated 
	HandlerPts							m_LatestPts;				// latest PTS data received by PTSCallback.
	HandlerPts							m_StartPts;					// first pts received in 'Starting' state - used only for debugging purposes
	TimeValue							m_tvForceStartPts;			// trigger time (compared against encoded pts) if hit before a non-interpolated pts will force start.

	// PiP Positioning
	SonicMutex							m_mtxPiPMetadata;			// mutex to be held when updating any of the following HandlerState members.
	bool								m_bPendingPiPMetadata;		// true if there is a pending PiP Position to be applied
	TimeValue							m_PiPMetadataPlaylistPts90k;// PTS (in Main Path time) at which to apply next PiP Position
	PiRect								m_PiPDestRect;				// new PiP dest rect
	bool								m_bPiPFullScreen;			// true if PiP should be made fullscreen
	int									m_PiPLumaKey;				// luma Key value

#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	CThread* 								m_pThread;
#endif
};

}; // namespace BDROM

#endif // __PTSCALLBACKHANDLER_H_
	

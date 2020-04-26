//-----------------------------------------------------------------------------
// BDROM_StreamControl.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROM_STREAMCONTROL_H_
#define __BDROM_STREAMCONTROL_H_

#include "ref_ptr.h"
#include "BaseRenderer.h"
#include "typedesc.h"
#include "BDROM_NavigatorTypes.h"
#include "BDROM_NavigatorTypes_Internal.h"
#include "BDROM_PTSAccess.h"
#include "SonicSemaphore.h"
#include "SonicMutex.h"
#include "PTSCallbackHandler.h"

#ifndef REALTEK_MODIFY
#include "BDROM_Settings.h"
#endif
#include "HDMVDecoder_Types.h"
#include "SonicPBCE_Registers.h"

#if defined(WIN32)
#include "IEncryptedStreamNotifyCallback.h"
#endif

class rt_stream;
class HDMVDecoders_PacketSink;
class PlaybackControlEngine;

//__________________________________________________________________________________________________________________________________________________
// Defines, Macros, conditional includes, ...
//

//--------------------------------------------------------------------------
// Compile symbols
//

//--------------------------------------------------------------------------
// conditional header file inclusions
//

//__________________________________________________________________________________________________________________________________________________
// Destinatinon IDs to use with HDRenderer
// (use these for HDR APIs that take an ID param
//

const UInt32 kHDRPrimaryVideoId				= 0;
const UInt32 kHDRPrimaryAudioId				= 1;
const UInt32 kHDRSecondaryVideoId			= 2;
const UInt32 kHDRSecondaryAudioId			= 3;

//__________________________________________________________________________________________________________________________________________________
// Stream Source and Destinations types
//

const UInt32 kMaxStreamSources				= 4;
const UInt32 kMaxStreamDestinations			= 7;
const UInt32 kMaxRoutes						= 7;


//--------------------------------------------------------------------------
// Source Types
//
typedef enum tagBDROM_StreamSourceType
{
	// valid
	kBDROM_StreamSourceType__Main			= 0,		///< Main Path stream (Primary Video)
	kBDROM_StreamSourceType__Sub			= 1,		///< Sub Path stream (Secondary Video (PiP), or Primary Audio for browsable slideshow)
	kBDROM_StreamSourceType__Preload_IG		= 2,		///< Sub Path stream for preloading IG
	kBDROM_StreamSourceType__Preload_Text	= 3,		///< Sub Path stream for preloading Text

	// all
	kBDROM_StreamSourceType__All			= 0xFE,		///< All sources

	// invalid
	kBDROM_StreamSourceType__Invalid		= 0xFF,		///< Invalid source
} BDROM_StreamSourceType;

#define VALID_SOURCE_TYPE(src)				((src != kBDROM_StreamSourceType__Invalid) && (src <= kBDROM_StreamSourceType__Preload_Text))

//--------------------------------------------------------------------------
// Destination Types
//
typedef enum tagBDROM_StreamDestType
{
	// Pins
	kBDROM_StreamDestType__PrimaryVideo		= 0,	///< Video Pin 1 (Primary Video)
	kBDROM_StreamDestType__PrimaryAudio		= 1,	///< Audio Pin 1 (Primary Audio)
	kBDROM_StreamDestType__SecondaryVideo	= 2,	///< Video Pin 2 (Secondary Video / PiP)
	kBDROM_StreamDestType__SecondaryAudio	= 3,	///< Audio Pin 2 (Secondary Audio)

	// Packet Sinks
	kBDROM_StreamDestType__IG				= 4,	///< IG Decoder
	kBDROM_StreamDestType__PG				= 5,	///< PG / PiP PG Decoder
	kBDROM_StreamDestType__TextST			= 6,	///< TextST / PiP TextST Decoder

	// all
	kBDROM_StreamDestType__All				= 0xFE,	///< All Destinations

	// invalid
	kBDROM_StreamDestType__Invalid			= 0xFF,	///< Invalid Destination
} BDROM_StreamDestType;

#define VALID_DEST_TYPE(dest)				((dest != kBDROM_StreamDestType__Invalid) && (dest <= kBDROM_StreamDestType__TextST))
#define IS_PIN_DEST(dest)					((dest >= kBDROM_StreamDestType__PrimaryVideo) && (dest <= kBDROM_StreamDestType__SecondaryAudio))
#define IS_PACKET_SINK_DEST(dest)			((dest >= kBDROM_StreamDestType__IG) && (dest <= kBDROM_StreamDestType__TextST))
#define IS_VIDEO_DEST(dest)					((dest == kBDROM_StreamDestType__PrimaryVideo)   || \
											 (dest == kBDROM_StreamDestType__SecondaryVideo))
#define IS_GRAPHICS_DEST(dest)				((dest == kBDROM_StreamDestType__IG)             || \
											 (dest == kBDROM_StreamDestType__PG)             || \
											 (dest == kBDROM_StreamDestType__TextST))

//--------------------------------------------------------------------------
// PTS Source Types
//
typedef enum tagBDROM_PTSSource
{
	kBDROM_PTSSource__Unknown,					///< Invalid value. Used only for initialization
	kBDROM_PTSSource__HDR,						///< Get PTS from HDR
	kBDROM_PTSSource__MainPathPTSClock			///< Get PTS from MainPath PTS Clock.
} BDROM_PTSSource;

//__________________________________________________________________________________________________________________________________________________
// Stream Source and Destinations
//

//--------------------------------------------------------------------------
// Source
//
struct BDROM_StreamSource
{
public:
	BDROM_StreamSource()
		: mType(kBDROM_StreamSourceType__Invalid)
		, mId(0xFFFF)
		, mRTSource(0)
	{
	}

public:
	BDROM_StreamSourceType	mType;				///< Source type
	UInt32					mId;				///< Id used to register with the Renderer
	rt_stream *				mRTSource;			///< rt_stream pointer to source
};

//--------------------------------------------------------------------------
// Destination
//
struct BDROM_StreamDest
{
public:
	BDROM_StreamDest()
		: mType(kBDROM_StreamDestType__Invalid)
		, mId(0xFFFF)
		, mStrmCodingType(kBDROM_StreamCodingType_Unknown)
		, mbFmtChangePending(false)
		, mpPacketSink(0)
		
	{
	}

	~BDROM_StreamDest()
	{
		switch (mType)
		{
		case kBDROM_StreamDestType__PrimaryVideo:
		case kBDROM_StreamDestType__SecondaryVideo:
			if (mpVideoDesc != 0)
				delete mpVideoDesc;
			break;
		case kBDROM_StreamDestType__PrimaryAudio:
		case kBDROM_StreamDestType__SecondaryAudio:
			if (mpAudioDesc != 0)
				delete mpAudioDesc;
			break;
#ifdef REALTEK_MODIFY
		default:
			break;
#endif			
		}
	}

public:
	BDROM_StreamDestType	mType;				///< Destination type
	UInt32					mId;				///< Id used to register with the Renderer (if dest type is a Pin)
	BDROM_StreamCodingTypes	mStrmCodingType;	///< stream_coding_type. Mainly used to distinguish between PG and TextST
	bool					mbFmtChangePending;	///< Flag to indicate that the format change is pending.

	union
	{
	HDMVDecoders_PacketSink	*	mpPacketSink;	///< Packet Sink pointer if destination is a Packet Sink. 0 if destination is a Pin.
	AUDIO_STREAM_DESC		*	mpAudioDesc;	///< STREAM_DESC for an Audio Output Pin
	VIDEO_STREAM_DESC		*	mpVideoDesc;	///< STREAM_DESC for a Video Output Pin
	};
};


//__________________________________________________________________________________________________________________________________________________
// Stream Routing
//

//--------------------------------------------------------------------------
// Stream Route
//
struct BDROM_StreamRoute
{

	/*
	Status of a route:
	 (a) 'Active'/'Inactive'        : Active if this route is in use. This usually means that data is being sent over this
	                                  route. For Preload streams, it means that data has been sent over this route for the current
	                                  playlst.
	 (b) 'Connected'/'Disconnected' : Connected if source is connected to destination. Disconnected if source is not connected
	                                  to destination.

	 A Route can be:
	  - Inactive & Disconnected: A route that is not in use during playlist playback.
	  - Inactive & Connected   : A route marked inactive during a transition but still connected. Such routes will
	                             typically be disconnected DisconnectInactiveRoutes() after transition has been setup
	  - Active & Connected     : A route in use during playlist playback (like Primary Video)
	  - Active & Disconnected  : A route in use during playlist playback but disconnected from HDR. e.g. Preloaded IG/TextST
	                             which has been disconnected for performance reasons since they no longer stream data once
								 playlist playback begins
	*/

public:
	BDROM_StreamRoute()			{ Clear(); }

	// Clear: Marks route 'InActive' and 'Disconnected'
	void Clear(void)
	{
		mSourceType = kBDROM_StreamSourceType__Invalid;
		mDestType   = kBDROM_StreamDestType__Invalid;
	 	mPID        = BDROM_NULL_PACKET_PID;
		mbConnected = false;
		mbActive    = false;
	}

	// Activate: only marks the route as 'Active'
	void Activate(void)			{ mbActive = true; }
	// Deactivate: only marks the route as 'InActive'.
	void Deactivate(void)		{ mbActive = false; }

	// IsActive: returns true if route is 'Active'
	bool IsActive(void)			{ return mbActive; }

	// Connect: Marks route as 'Connected' and updates route information
	void Connect(BDROM_StreamSourceType sourceType, BDROM_StreamDestType destType, UInt16 uiPID)
	{
		mSourceType = sourceType;
		mDestType   = destType;
	 	mPID        = uiPID;
		mbConnected = true;
	}
	// Disconnect: Marks the route as 'Disconnected'
	void Disconnect(void)
	{
		mbConnected = false;
	}

	// IsConnected: returns true if route is 'Connected'
	bool IsConnected(void)		{ return mbConnected; }

	bool IsEqual(BDROM_StreamSourceType sourceType, BDROM_StreamDestType destType, UInt16 uiPID)
	{
		if (uiPID != BDROM_NULL_PACKET_PID)
			return ((mSourceType == sourceType) && (mDestType == destType) && (mPID == uiPID));
		else
			return (mPID == BDROM_NULL_PACKET_PID);
	}

public:
	BDROM_StreamSourceType	mSourceType;
	BDROM_StreamDestType	mDestType;
	UInt16					mPID;

	bool					mbConnected;	///< 'Connected'/'Disconnected' status of this route.
	bool					mbActive;		///< 'Active'/'Inactive' status of this route.
};

//__________________________________________________________________________________________________________________________________________________
// Clocks
//

//--------------------------------------------------------------------------
// Stream Clock
//
struct BDROM_StreamClock
{
public:
	BDROM_StreamClock()
		: mbActive(false)
		, mSourceType(kBDROM_StreamSourceType__Invalid)
		, mClockId((EBDROMPrecisionClock)0)
		
	{
	}

	~BDROM_StreamClock()
	{
	}

	void SetActive(void)					{ mbActive = true; }
	void SetInactive(void)					{ mbActive = false; }
	bool IsActive(void)						{ return mbActive; }

public:
	bool										mbActive;		///< Indicates that the clock needs to be started (when playback/streaming is started)
	BDROM_StreamSourceType						mSourceType;
	EBDROMPrecisionClock						mClockId;
	TimeValue									mtvPauseTime;	///< System clock time at which this clock was paused. Valid only if clock is in Paused state
};


//__________________________________________________________________________________________________________________________________________________
/// @class	PreRollSwitch
/// @brief	Pre-roll switch (thread that is used only for turn on/off pre-roll). Used only by BDROM_StreamControl
///
#if defined(WIN32)
class PreRollSwitch : SonicThread
{
public:
	PreRollSwitch();
	~PreRollSwitch();
	bool Init(ref_ptr<BaseRenderer> pRenderer);
	void DeInit(void);
	virtual Priority GetPriority(void)		{ return Priority_RealTime; }
	virtual SonicError Run(void);

	// PreRoll functions
	//
	void BeginPreRoll(void);
	void EndPreRoll(void);
	bool InPreRoll(void);

private:
	typedef enum tagPreRollSwitchEvent
	{
		kPRS_Invalid,
		kPRS_BeginPreRoll,	// start of pre-roll
		kPRS_EndPreRoll,	// end of pre-roll
		kPRS_Exit,			// exit thread
	} PreRollSwitchEvent;

private:
	ref_ptr<BaseRenderer>	m_pRenderer;
	std::string				m_ThreadName;				// Name of this thread

	SonicCriticalSection 	m_csLock;
	bool					m_bExit;

	SonicCriticalSection	m_csNewEventLock;
	PreRollSwitchEvent		m_CurrEvent;
	SonicSemaphore			m_semNewEvent;
	SonicSemaphore			m_semEventDone;
};
#endif //defined(WIN32)


//__________________________________________________________________________________________________________________________________________________
// EndOfPlaybackCallback
//

//--------------------------------------------------------------------------
// @class BaseEndOfPlaybackCallback
// @brief Base implementation of End-of-Playback callback for a destination.
// This base version does not register for callbacks - always assumes end-of-playback on end-of-stream
//
class BaseEndOfPlaybackCallback
{
public:
	BaseEndOfPlaybackCallback();
	virtual ~BaseEndOfPlaybackCallback();

	// Init
	void Init(ref_ptr<BaseRenderer> pRenderer, BDROM_StreamDestType destType);

	// Clears class data
	void Clear(void);

	// Sets a callback (with provided callback user data)
	virtual void SetCallback(UInt32 callbackData);

	// Clears any previously set callback 
	virtual void ClearCallback(void);

	// indicates that End-Of-Stream condition (all data send to demux) for dest type has been reached
	// (End Of Playback callbacks received before this are discarded)
	virtual void SetEndOfStream(void);

	// end-of-playback callback received. Dereived classes must call this when they receive a callback
	void EndOfPlaybackCallbackRcvd(UInt32 callbackData);

	// checks if end-of-playback condition (for a given destinatino) is met by testing received callbacks against expected ones
	virtual bool IsEndOfPlayback(void);

protected:
	SonicCriticalSection 	m_csLock;

	ref_ptr<BaseRenderer>	m_pRenderer;		// pointer to HD Renderer
	BDROM_StreamDestType	m_DestType;			// dest type that this callback instance represents


	UInt32					m_CallbackData;		// unique id that can be set as user data for callback (to weed out older/cancelled callbacks)
	bool					m_bEndOfStream;		// End-of-Stream: all data sent
	bool					m_bCallbackRcvd;	// only first m_NumDests are valid and each index matches dest in m_Dests. 
};

//--------------------------------------------------------------------------
// @class WinEndOfPlaybackCallback
// @brief Windows implementation of End-of-Playback callback. This implements the necessary interface to be registered as a callback
//
#if defined(WIN32)

class WinEndOfPlaybackCallback : public BaseEndOfPlaybackCallback, public IEncryptedStreamNotifyCallback
{
public:
	WinEndOfPlaybackCallback();
	~WinEndOfPlaybackCallback();

	// Sets a callback (with provided callback user data)
	virtual void SetCallback(UInt32 callbackData);

	// Clears any previously set callback 
	virtual void ClearCallback(void);

	// indicates that End-Of-Stream condition (all data send to demux) for dest type has been reached
	// (End Of Playback callbacks received before this are discarded)
	virtual void SetEndOfStream(void);

	// IEncryptedStreamNotifyCallback interface implementation
    STDMETHODIMP EncryptedStreamCallback(ENCRYPTED_STREAM_NOTIFY_CODE notifyCode, size_t value1, size_t value2, BYTE* pData, size_t nDataBytes);
};
#else
class EndOfPlaybackCallback : public BaseEndOfPlaybackCallback
{
public:
	EndOfPlaybackCallback();
	~EndOfPlaybackCallback();

	// Sets a callback (with provided callback user data)
	virtual void SetCallback(UInt32 callbackData);

	// Clears any previously set callback 
	virtual void ClearCallback(void);

	// indicates that End-Of-Stream condition (all data send to demux) for dest type has been reached
	// (End Of Playback callbacks received before this are discarded)
	virtual void SetEndOfStream(void);

	// IEncryptedStreamNotifyCallback interface implementation
    virtual void EncryptedStreamCallback(int value1, int value2);
};

#endif


//__________________________________________________________________________________________________________________________________________________
// Stream Control
//

//--------------------------------------------------------------------------
/// @class BDROM_StreamControl
/// @brief	Stream controller that tracks sources and destinations, manages stream routing, adding stream metadata and managing title clocks
///
class BDROM_StreamControl : public BDROM_PTSAccess
{
public:
#ifdef REALTEK_MODIFY
	BDROM_StreamControl(UInt32 uiNumClocks, ref_ptr<BaseRenderer> pRenderer, void* pSettings, bool bSKBDisc = false, bool bBDPlusDisc = false);
#else	
	BDROM_StreamControl(UInt32 uiNumClocks, ref_ptr<BaseRenderer> pRenderer, ref_ptr<BDROM_Settings> pSettings, bool bSKBDisc = false, bool bBDPlusDisc = false);
#endif	
	virtual ~BDROM_StreamControl();
private:
	BDROM_StreamControl();

	// register inputs, outputs, clocks and then Startup
public:
	SonicError RegisterSource(BDROM_StreamSourceType sourceType, ref_ptr<rt_stream> rtSource, UInt32 uiSrcReadUnitSize, UInt32 uiNumReadBuffers);
	SonicError RegisterDestination(BDROM_StreamDestType destType, STREAM_DESC * streamDesc, BDROM_StreamCodingTypes strmCodingType = kBDROM_StreamCodingType_Unknown);
	SonicError RegisterDestination(BDROM_StreamDestType destType, HDMVDecoders_PacketSink * packetSink, BDROM_StreamCodingTypes strmCodingType = kBDROM_StreamCodingType_Unknown);
	SonicError RegisterClock(BDROM_StreamSourceType sourceType, EBDROMPrecisionClock clockId);
	SonicError RegisterMasterClock(EBDROMPrecisionClock clockId, bool bCanReset = false);
	SonicError UnregisterSource(BDROM_StreamSourceType sourceType);
	SonicError Startup(void);
	
	SonicError GetDestinationId(BDROM_StreamDestType destType, UInt32 & destId);

	// Send MediaEvent to HDR
	SonicError MediaEvent(EMediaEventType mediaEventType, const UInt32 mediaEventDataSize = 0, void * pMediaEventData = 0);

	// clock control
public:
	void PauseOn(BDROM_StreamSourceType sourceType);
	void PauseOff(BDROM_StreamSourceType sourceType);

	SonicError StillOn();
	SonicError StillOff();

	// device lost handling
	void DeviceLost(void);
	void DeviceNew(void);

	// individual clock control
	bool SetClockTime(EBDROMPrecisionClock clockId, TimeValue tvTime);
	bool GetClockTime(EBDROMPrecisionClock clockId, TimeValue & tvTime);
	bool ResumeClock(EBDROMPrecisionClock clockId);
	bool PauseClock(EBDROMPrecisionClock clockId);
	bool StopClock(EBDROMPrecisionClock clockId);

	// PTS
	ref_ptr<BDROM::PTSCallbackHandler> GetPTSCallbackHandler(void);
	SonicError SetPTSSource(BDROM_PTSSource PtsSource);
	void SetRegisters(ref_ptr<BDROM_SonicPBCE_Registers> pRegisters)			{ m_pRegisters = pRegisters; }
	void SetPlaylistPtsMap(ref_ptr<BDROM::PlaylistPtsMap> pPlaylistPtsMap)		{ m_pPlaylistPtsMap = pPlaylistPtsMap; }

	// BDROM_PTSAccess Interface override
	virtual bool GetCurrentPTS(BDROM_PTSType ptsType, Time90K & ts90kCurrentPTS);		// returns PTS as a 90 KHz value
	virtual bool GetCurrentPlaylistPTS(TimeValue & playlistPts90k);
	virtual bool GetCurrentPlaylistPTS(TimeValue & playlistPts90k, TimeValue & platformPts90k);
protected:
	bool GetCurrentMainPathPTS(TimeValue & encodedPts90k, TimeValue & platformPts, UInt32 & playitemId);
	bool GetCurrentMainPathPTS_NoPTSCallback(TimeValue & encodedPts90k, TimeValue & platformPts, UInt32 & playitemId);
	bool GetCurrentASyncSubPathPTS(TimeValue & pts90k);
	bool GetCurrentASyncIGPTS(TimeValue & pts90k);

	// routing, change output format
public:
	virtual void StartStreaming(void);
	virtual void StopStreaming(void);
	void       TransitionBegin(void);
	void       TransitionEnd(void);

	// Pre-Roll:
	// * Call IsPreRollEnabled() before performing any pre-roll related tasks.
	// * BeginPreRoll() and EndPreRoll() should not be called from within the thread that pulls data (will deadlock if called)
	//
	bool       IsPreRollEnabled(void)						{ return m_bPreRollEnabled; }
	void       BeginPreRoll(void);
	void       EndPreRoll(void);
	bool       InPreRoll(void);

	bool       IsEndOfPlayback(BDROM_StreamDestType destType);
	bool       IsEndOfPlayback(BDROM_StreamSourceType sourceType);

	SonicError Flush(BDROM_StreamSourceType sourceType, BDROM_StreamDestType destType = kBDROM_StreamDestType__All);
	SonicError Flush(BDROM_StreamDestType destType);

	SonicError Stop(BDROM_StreamDestType destType);

	SonicError ActivateRoute(BDROM_StreamSourceType sourceType, BDROM_StreamDestType destType, UInt16 uiPID);
	SonicError DestinationOnOff(BDROM_StreamDestType destType, bool bOn);
	SonicError SetDestinationFormat(BDROM_StreamDestType destType, STREAM_DESC * pStreamDesc);
	SonicError SetDestinationFormat(BDROM_StreamDestType destType, BDROM_StreamCodingTypes strmCodingType);

	SonicError DeactivateRoute(BDROM_StreamSourceType sourceType, BDROM_StreamDestType destType, bool bForce = false);
	SonicError DeactivateRoutes(BDROM_StreamSourceType sourceType);
	SonicError DeactivateRoute(BDROM_StreamDestType destType, bool bForce = false);

	// unregisters packet sink for preload source type, but leaves route info unchanged.
	void       DetachPreloadSource(BDROM_StreamSourceType sourceType);

	bool       IsActive(void);

private:
	void       DisconnectInactiveRoutes(void);

	UInt32     GetNumConnectedPinDests(BDROM_StreamSourceType sourceType);
	UInt32     GetNumConnectedPktSinkDests(BDROM_StreamSourceType sourceType);


	// stream properties
public:
	void       SetPlayListStart(TimeValue tvPlayListStartElapsedTime);
	void       SetPlayListEnd(void);
	SonicError SetSegmentStart(BDROM_StreamSourceType sourceType);
	SonicError SetSegmentOffsetPTS(BDROM_StreamSourceType sourceType, SInt64 siTimeOffset);
	SonicError SetSourceApplicationType(BDROM_StreamSourceType sourceType, BDROM_ApplicationType applType);
	SonicError SetDestinationSyncOffset(BDROM_StreamDestType destType, SInt64 siTimeOffset);
	SonicError SetDestinationCharacterSet(BDROM_StreamDestType destType, BDROM_CharSet charSet);
	SonicError SetSegmentStartTime(BDROM_StreamSourceType sourceType, TimeValue tvSegStart, bool bSeamlessSegmentStart = false);
	
	//REALTEK_MODIFY
	SonicError SetDataDiscontinuity(BDROM_StreamSourceType sourceType, BDROM_StreamDestType destType = kBDROM_StreamDestType__All, long long ptsIn90K = -1);
	
	SonicError SetSegmentEnd(BDROM_StreamSourceType sourceType, bool bEndOfMenu = false);
	SonicError SetSkipRange(BDROM_StreamSourceType sourceType, bool bFromHead, UInt32 uiNumBytesToSkip);
	SonicError SetSampleDecryptionMetadata(BDROM_StreamSourceType sourceType,
										   UInt32 spn = BDROM_SAMPLEMD_INVALID_SPN,
										   SInt32 clipId = BDROM_SAMPLEMD_INVALID_CLIP_ID,
										   SInt32 playitemId = BDROM_SAMPLEMD_INVALID_PLAYITEM_ID);
	SonicError SetSPSegmentId(BDROM_StreamSourceType sourceType, UInt32 uiSPSegmentId);
    void       SetDesiredRate(double dRate, bool bResetIgTxtDecoderRate = false);		// sets desired rate
    SonicError SetRate(BDROM_StreamSourceType sourceType, double dRate);	// sets actual rate
    SonicError GetMaxPlayRate(double & dMaxPlayRate);
    double     GetDesiredRate(void);										// gets desired rate
    double     GetRate(void);												// get actual rate
	SonicError SetStartTime(BDROM_StreamSourceType sourceType, UInt64 pts90kStart, UInt32 numPIDs = 0, UInt32 * pPIDs = 0);
	SonicError SetStopTime(BDROM_StreamSourceType sourceType, UInt64 pts90kStop, UInt32 numPIDs = 0, UInt32 * pPIDs = 0);
	SonicError SetStartTimeReset(BDROM_StreamSourceType sourceType, UInt32 numPIDs = 0, UInt32 * pPIDs = 0);
	SonicError SetStopTimeReset(BDROM_StreamSourceType sourceType, UInt32 numPIDs = 0, UInt32 * pPIDs = 0);
	SonicError SetHDMVStreamExtent(BDROM_StreamSourceType sourceType, BDROM_StreamDestType destType, HDMVStreamExtent & hdmvStrmExtent);
	SonicError SetMapTime(BDROM_StreamSourceType sourceType);
	SonicError ConfigureLPCMDownMixCoeffs(BDROM_StreamSourceType sourceType, UInt16 pid, UInt8 * coeffs);
	SonicError GetScanFPS(UInt32 & nScanFPS);
	SonicError SetScanFPS(UInt32 nScanFPS);
	SonicError SetMaxPlayRate(double dMaxPlayRate);
	bool       IsScanningRate(double dblRate);
	bool       IsScanning(void);

	// display layer/plane
public:
	SonicError ClearDisplayPlane(BDROM_StreamDestType videoDestType);

	// decryption control
public:
	SonicError SetSampleMetadata(void)			{ return 0; }			

	// validity checkes
private:
	bool SourceExists(BDROM_StreamSourceType sourceType)
	{
		return (VALID_SOURCE_TYPE(sourceType) && ((UInt32)sourceType < kMaxStreamSources) && (VALID_SOURCE_TYPE(m_Sources[sourceType].mType)));
	}

	bool DestExists(BDROM_StreamDestType destType)
	{
		return (VALID_DEST_TYPE(destType) && ((UInt32)destType < kMaxStreamDestinations) && (VALID_DEST_TYPE(m_Dests[destType].mType)));
	}

	bool RouteExists(BDROM_StreamDestType destType)
	{
		return (((UInt32)destType < kMaxStreamDestinations) && (m_Routes[destType].mDestType != kBDROM_StreamDestType__Invalid));
	}

	bool IsPinDestination(BDROM_StreamDestType destType)
	{
		return ((destType >= kBDROM_StreamDestType__PrimaryVideo) && (destType <= kBDROM_StreamDestType__SecondaryAudio));
	}

	bool IsPktSinkDestination(BDROM_StreamDestType destType)
	{
		return ((destType >= kBDROM_StreamDestType__IG) && (destType <= kBDROM_StreamDestType__TextST));
	}

public:
	bool IsConnected(BDROM_StreamSourceType sourceType, BDROM_StreamDestType destType)
	{
		return (SourceExists(sourceType) && DestExists(destType) && (m_Routes[destType].mSourceType == sourceType));
	}

	// data members
protected:
	ref_ptr<BaseRenderer>		m_pRenderer;						///< pointer to HD Renderer

#ifdef REALTEK_MODIFY
	void*						m_pSettings;
#else
	ref_ptr<BDROM_Settings>		m_pSettings;
#endif	
	bool						m_bPreRollEnabled;					///< flag that indicates if PreRoll is to be enabled (one time setup via constructor)
	bool						m_bUsePtsCallback;					///< flag to indicate if PTS Callbacks should be used (one time setup via constructor)
	bool						m_bSKBDisc;							///< flag to indicate if disc has SKBs. Used for setting EMD_BD_DECRYPT_METADATA metadata.
	bool						m_bBDPlusDisc;						///< flag to indicate if disc has BD+. Used for setting EMD_BD_DECRYPT_METADATA metadata.

	bool						m_bStreaming;						///< True when data is being streamed. Used to manage Route activation/deactivation.
	bool						m_bInTransition;					///< True when a transition is being setup. Used to manage Route activation/deactivation.
	BDROM_StreamRoute			m_Routes[kMaxStreamDestinations];	///< All possible routes (in same order of destinations)
	SonicCriticalSection		m_csRoutingLock;					///< to serialize routing, flushing, metadata
	SonicCriticalSection		m_csClockLock;						///< to serialize clock access for PTS
	ref_ptr<BDROM::PTSCallbackHandler>	m_PtsCallback;

private:
	UInt32						m_uiMaxClocks;						///< Max clocks to support
	UInt32						m_uiNumClocks;						///< Number of clocks registered

	UInt32						m_uiNextSourceId;					///< Next source Id to be used when adding a stream to the Renderer
	UInt32						m_uiNextDestId;						///< Next destination Id to be used when adding a Pin output to the Renderer

	BDROM_StreamSource			m_Sources[kMaxStreamDestinations];	///< All possible sources
	BDROM_StreamDest			m_Dests[kMaxStreamDestinations];	///< All possible destinations
	BDROM_StreamClock *			m_pClocks;							///< List of registered clocks
	EBDROMPrecisionClock		m_MasterClockId;					///< Id Master clock. It never stops

	bool						m_bPlayListStartPending;			///< true if SetPlayListStart() has been called, its first segment has not yet started
	TimeValue					m_tvPlayListStartElapsedTime;		///< Elapsed time of the playlist start location

    double						m_dDesiredRate;						///< Desired playback speed
    double						m_dRate;							///< Actual playback speed
    double						m_dMaxPlayRate;						///< the max forward playback speed that the video decoder supports
	UInt32						m_nScanFPS;							///< number of frames per second to display in scan modes

	BDROM_PTSSource				m_PTSSource;						///< Source for getting current PTS
	bool						m_bSyncMainPathPTSClock;			///< true if Main Path PTS clock must be updated with PTS from HDR on next call to GetCurrentPTS()
	Time90K						m_ts90kPTSofRateChange;				///< PTS at which last rate change occured

#if defined(WIN32)
	PreRollSwitch			*	m_pPreRollSwitch;					///< object to pause/play graph using a separate thread
#endif
	SonicMutex					m_mtxState;							///< mutex to access state info (like pre-roll state)
	bool						m_bInPreRoll;						///< flag to indicate if player is in a pre-roll state

	UInt32						m_EndOfPlaybackId;					///< unique id set for each set of end-of-playack callbacks
	BaseEndOfPlaybackCallback * m_pEndOfPlaybackCallbacks[kMaxStreamDestinations];	///< End-of-playback callbacks for each destination

	ref_ptr<BDROM_SonicPBCE_Registers>	m_pRegisters;				///< PSRs and GPRs. Needed for getting current playitem Id, curr PTS, PlaylistPts
	ref_ptr<BDROM::PlaylistPtsMap>		m_pPlaylistPtsMap;			///< PTS Map. Needed for getting PlaylistPts.
};

#endif
	

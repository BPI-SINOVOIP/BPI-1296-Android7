//-----------------------------------------------------------------------------
// SonicPBCE_Paths.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SONICPBCE_PATHS_H__
#define __SONICPBCE_PATHS_H__

#include "ref_ptr.h"
#include "BDROM_NavigatorTypes.h"
#include "BDROMNavigationCache.h"
#include "SonicPBCE_EventSink.h"
#include "SonicPBCE_Clips.h"

//class BDROM_SonicPBCE_Clip;
class BDROM_SonicPBCE_ClipManager;
class BDROM_Timer;
class BDROM_TimerManager;


// PBCE_ItemState
// State of a PlayItem/SubPlayItem
//
typedef enum tagPBCE_ItemState
{
	// Typical state changes:
	//	Idle --> Opened --> Started --> Stopped -> Idle
	//

	kPBCE_ItemState__Idle,		// Not yet initialized
	kPBCE_ItemState__Opened,	// Initialized and ready for reading & playback
	kPBCE_ItemState__Started,	// Item started Playing current [Sub]PlayItem (includes paused/scan/still playback states)
	kPBCE_ItemState__Stopped	// Item has been stopped (because playback stopped). this is usually done before resetting item
} PBCE_ItemState;


// BDROM_SonicPBCE_Item
// PlayItem/SubPlayItem base class
//
class BDROM_SonicPBCE_Item
{
public:
	BDROM_SonicPBCE_Item(BDROM_SonicPBCE_Registers * pRegisters, BDROM_SonicPBCE_ClipManager * pClipManager, PBCE_ItemType itemType);
	virtual ~BDROM_SonicPBCE_Item();

	// Reset/Cleanup
	//

	virtual void Reset(void);
	
	// Item state
	//
	PBCE_ItemState GetState(void)					{ return m_State; }
	bool IsProvidingData(void);

	// Init to be done when opening an item
	//

	// Set playlist id, playlist info, and path type
	virtual void SetPlayListId(UInt32 uiPlayListId)								{ m_uiPlayListId = uiPlayListId; }
	virtual void SetPlayListInfo(ref_ptr<BDROM::PlayList_mpls> pPlayList_mpls)	{ m_pPlayList_mpls = pPlayList_mpls; assert(m_uiPlayListId != BDROM_INVALID_PLAYLIST_ID);}
	virtual void SetPathType(PBCE_PathType pathType)							{ m_PathType = pathType; }
	virtual void SetStartOffsetIntoItem(Time90K ts45kOffsetIntoItem)			{ m_ts45kOffsetIntoItem = ts45kOffsetIntoItem; }

	// Init/configuration to be done before starting item
	//

	// caching of PSRs (for seamlessly connected items or items that are being read, but not yet being played)
	void CachePrimaryAudioStrmPSR(UInt32 uiPSRVal)		{ m_PrimaryAudioStrmNum_PSR = uiPSRVal; m_bPrimaryAudioStrmNum_PSRcached = true; }
	void CachePGTextSTStrmPSR(UInt32 uiPSRVal)			{ m_PGTextStrmNum_PSR = uiPSRVal; m_bPGTextStrmNum_PSRcached = true; }
	void CacheIGStrmPSR(UInt32 uiPSRVal)				{ m_IGStrmNum_PSR = uiPSRVal; m_bIGStrmNum_PSRcached = true; }
	void CacheSecondaryStrmPSR(UInt32 uiPSRVal)			{ m_SecondaryStrmNum_PSR = uiPSRVal; m_bSecondaryStrmNum_PSRcached = true; }

	// item repetition
	void SetRepeatable(bool bRepeatable)				{ m_bRepeatable = bRepeatable; }

	// seamless connectivity
	void SetSeamlessStart(bool bSeamlessStart)			{ m_bSeamlessStart = bSeamlessStart; }
	void SetSeamlessStop(bool bSeamlessStop)			{ m_bSeamlessStop  = bSeamlessStop; }
	void SetStartOffsetPTS(SInt64 ts45kOffsetPTS)		{ m_ts45kStartOffsetPTS = ts45kOffsetPTS; }
	void ClipIsPlaying(void)							{ if (m_pClip) m_pClip->ClipIsPlaying(); }
	bool IsClipEnded(void)								{ if (m_pClip) return m_pClip->IsClipEnded(); return true;}

	// clip open/close
	virtual SonicError OpenClip(BDROM_PlayDirection direction, double dSpeed)		= 0;
	virtual SonicError CloseClip(void)												= 0;

	// stream selection (stream selection can be done even during playback), and setting PIDs
	// - In SelectStream: uiStreamNum 0 means no stream
	virtual SonicError SelectStream(BDROM_StreamType streamType, UInt32 uiStreamNum, bool bFlushOld = false) = 0;
	virtual SonicError StreamOnOff(BDROM_StreamType streamType, bool bOn, bool bIsText = false)	= 0;
	void SetPID(BDROM_StreamType streamType, UInt16 uiPID);

	// Get item properties
	//

	// Get playlist id, and path type
	UInt32 GetPlayListId(void)									{ return m_uiPlayListId; }
	PBCE_PathType GetPathType(void)								{ return m_PathType; }

	// get IN/OUT time info (as 45 Khz PTS values and as Elapsed Time values), and start offset (as 45 Khz PTS values)
	virtual Time90K GetInTime(void)								= 0;
	virtual Time90K GetOutTime(void)							= 0;
	Time90K GetInElapsedTime(void)								{ return m_ts45kInElapsedTime;					}
	Time90K GetOutElapsedTime(void)								{ return m_ts45kOutElapsedTime;					}
	virtual Time90K GetStartOffsetIntoItem(void)				{ return m_ts45kOffsetIntoItem; }

	// item repetition
	bool IsRepeatable(void)										{ return m_bRepeatable; }

	// connection
	bool   GetSeamlessStart(void)								{ return m_bSeamlessStart; }
	bool   GetSeamlessStop(void)								{ return m_bSeamlessStop; }
	SInt64 GetStartOffsetPTS(void)								{ return m_ts45kStartOffsetPTS; }

	// get still info
	virtual UInt32 GetStillType(void)							{ return kBDROM_StillType__None; }
	virtual UInt32 GetStillDuration(void)						{ return 0; }

	// get clip's application type
	BDROM_ApplicationType GetClipApplicationType(void);

	// get cached PSRs
	bool IsPrimaryAudioStrmPSRCached(void)						{ return m_bPrimaryAudioStrmNum_PSRcached; }
	bool IsPGTextSTStrmPSRPSRCached(void)						{ return m_bPGTextStrmNum_PSRcached; }
	bool IsIGStrmPSRCached(void)								{ return m_bIGStrmNum_PSRcached; }
	bool IsSecondaryStrmPSRCached(void)							{ return m_bSecondaryStrmNum_PSRcached; }
	UInt32 GetCachedPrimaryAudioStrmPSR(void)
	{
		assert(m_bPrimaryAudioStrmNum_PSRcached);
		return m_bPrimaryAudioStrmNum_PSRcached ? m_PrimaryAudioStrmNum_PSR : m_pRegisters->GetPSR(BDROM_PSRidx__PrimaryAudio_StrmNum);
	}
	UInt32 GetCachedPGTextSTStrmPSR(void)
	{
		assert(m_bPGTextStrmNum_PSRcached);
		return m_bPGTextStrmNum_PSRcached ? m_PGTextStrmNum_PSR : m_pRegisters->GetPSR(BDROM_PSRidx__PGText_StrmNum);
	}
	UInt32 GetCachedIGStrmPSR(void)
	{
		assert(m_bIGStrmNum_PSRcached);
		return m_bIGStrmNum_PSRcached ? m_IGStrmNum_PSR : m_pRegisters->GetPSR(BDROM_PSRidx__IG_StrmNum);
	}
	UInt32 GetCachedSecondaryStrmPSR(void)
	{
		assert(m_bSecondaryStrmNum_PSRcached);
		return m_bSecondaryStrmNum_PSRcached ? m_SecondaryStrmNum_PSR : m_pRegisters->GetPSR(BDROM_PSRidx__Secondary_StrmNum);
	}

	// angle
	virtual UInt32 GetNumberOfAngles(void)						{ return 1; }
	virtual UInt32 GetCurrentAngle(void)						{ return m_CurrAngleNum; }

	// get attributes and PIDs
	SonicError GetStreamAttributes(UInt16 uiPID, BDROM_Stream & streamAttr);
	SonicError GetCurrentVideoAttributes(BDROM_VideoAttributes & vatr);
	SonicError GetCurrentAudioAttributes(BDROM_AudioAttributes & aatr);
	UInt16 GetPID(BDROM_StreamType streamType);

	// Playback control
	//

	virtual SonicError Start(void)				{ return 0; }
	virtual SonicError Stop(void)				{ return 0; }
	virtual bool IsStarted(void)				{ return m_State == kPBCE_ItemState__Started; }
	virtual Time90K GetStartPTS(void)			= 0;			// returns start PTS (90KHz) of PlayItem/SubPlayItem

	virtual SonicError PauseReading(void)		{ return 0; }
	virtual SonicError ResumeReading(void)		{ return 0; }

	virtual SonicError PauseOn(void)			{ return 0; }
	virtual SonicError PauseOff(void)			{ return 0; }

	virtual SonicError SetRate(BDROM_PlayDirection direction, double dSpeed)			= 0;
	virtual bool       IsRateChangeAllowed(void);

	virtual SonicError TimeSearch(Time90K pts90kOffset)		= 0;

	virtual SonicError Repeat(void)							= 0;

protected:
	PBCE_ItemType					m_ItemType;				// Item Type: PlayItem or SubPlayItem
	PBCE_PathType					m_PathType;				// Type of path (Main-path or a type of Sub-path)
	BDROM_SonicPBCE_Registers *		m_pRegisters;
	BDROM_SonicPBCE_ClipManager *	m_pClipManager;

	PBCE_ItemState					m_State;				// Item State

	UInt32							m_uiPlayListId;			// Current PlayList
	ref_ptr<BDROM::PlayList_mpls>	m_pPlayList_mpls;		// PlayList info

	bool							m_bRepeatable;			// true if item is to be repeated

	bool							m_bSeamlessStart;		// true if the start of this item is to be seamless (with prev item)
	bool							m_bSeamlessStop;		// true if end of this item is to be seamless (with next item)
	SInt64							m_ts45kStartOffsetPTS;	// PTS (45kHz) to be added to stream PTS to make its start seamless or sync Async PiP to main path

	BDROM_SonicPBCE_Clip *			m_pClip;				// Ptr to clip. Valid as long as clip is being used for reading.
	ref_ptr<BDROM::ClipInfo_clpi>	m_pClipInfo_clpi;		// Ptr to clip info. Valid as long as item is valid (i.e. is playing or reading)

	UInt16							m_PrimaryVideo_PID;		// PID of primary video being played from this item. NULL_PID if none is.
	UInt16							m_PrimaryAudio_PID;		// PID of primary audio being played from this item. NULL_PID if none is.
	UInt16							m_IG_PID;				// PID of IG being played from this item. NULL_PID if none is.
	UInt16							m_PGTextST_PID;			// PID of PG/TextST being played from this item. NULL_PID if none is.
	UInt16							m_SecondaryVideo_PID;	// PID of secondary video being played from this item. NULL_PID if none is.
	UInt16							m_SecondaryAudio_PID;	// PID of secondary audio being played from this item. NULL_PID if none is.

	UInt32							m_CurrAngleNum;			// current angle

	// caching of PSRs associated with streams. Used for seamless connection conditions. PSRs are
	// cached when the next item is loaded, and retrieved to update PSRs when elapsed time reaches
	// next item's IN time.
	bool							m_bPrimaryAudioStrmNum_PSRcached;
	UInt32							m_PrimaryAudioStrmNum_PSR;
	bool							m_bPGTextStrmNum_PSRcached;
	UInt32							m_PGTextStrmNum_PSR;
	bool							m_bIGStrmNum_PSRcached;
	UInt32							m_IGStrmNum_PSR;
	bool							m_bSecondaryStrmNum_PSRcached;
	UInt32							m_SecondaryStrmNum_PSR;

	// In and Out elapsed time are in the playlist time for PlayLists with Sequential playback type, and in playitem time for random/shuffle
	Time90K							m_ts45kInElapsedTime;	// Elapsed Time (as 45 KHz value) that this [Sub]PlayItem's IN_time corresponds to.
	Time90K							m_ts45kOutElapsedTime;	// Elapsed Time (as 45 KHz value) that this [Sub]PlayItem's OUT_time corresponds to.

	Time90K							m_ts45kOffsetIntoItem;	// A start offset (if item is not starting from item begining). used only when loading clip
};


// BDROM_SonicPBCE_PlayItem
// A BDROM PlayItem
//
class BDROM_SonicPBCE_PlayItem : public BDROM_SonicPBCE_Item
{
public:
	BDROM_SonicPBCE_PlayItem(BDROM_SonicPBCE_Registers * pRegisters, BDROM_SonicPBCE_ClipManager * pClipManager);
	~BDROM_SonicPBCE_PlayItem();

	// Reset/Cleanup
	//

	void Reset(void);
	
	// Init to be done when opening an item
	//

	// set playitem info
	void SetPlayItemId(UInt32 uiPlayItemId)		{ m_uiPlayItemId = uiPlayItemId; }

	// clip open/close
	virtual SonicError OpenClip(BDROM_PlayDirection direction, double dSpeed);
	virtual SonicError CloseClip(void);

	// stream selection (stream selection can be done even during playback)
	// - In SelectStream: uiStreamNum 0 means no stream
	virtual SonicError SelectStream(BDROM_StreamType streamType, UInt32 uiStreamNum, bool bFlushOld = false);
	virtual SonicError StreamOnOff(BDROM_StreamType streamType, bool bOn, bool bIsText = false);

	// Get item properties
	//

	// get playitem info
	UInt32 GetPlayItemId(void)					{ return m_uiPlayItemId; }

	// get IN/OUT time info (as 45 Khz values)
	Time90K GetInTime(void)						{ return (Time90K)m_pPlayList_mpls->mPlayList.mPlayItem[m_uiPlayItemId+1].mIN_time; }
	Time90K GetOutTime(void)					{ return (Time90K)m_pPlayList_mpls->mPlayList.mPlayItem[m_uiPlayItemId+1].mOUT_time; }

	// get still info
	UInt32 GetStillType(void)					{ return (BDROM_StillType)(UInt32)m_pPlayList_mpls->mPlayList.mPlayItem[m_uiPlayItemId+1].mstill_mode; }
	UInt32 GetStillDuration(void)				{ return (UInt32)m_pPlayList_mpls->mPlayList.mPlayItem[m_uiPlayItemId+1].mstill_time; }

	// angle
	UInt32 GetNumberOfAngles(void);

	// Playback control
	//

	virtual SonicError Start(void);
	virtual SonicError Stop(void);
	virtual Time90K GetStartPTS(void)			{ return (m_pPlayList_mpls->mPlayList.mPlayItem[m_uiPlayItemId+1].mIN_time << 1); }

	virtual SonicError PauseReading(void);
	virtual SonicError ResumeReading(void);

	virtual SonicError PauseOn(void);
	virtual SonicError PauseOff(void);
	virtual SonicError SetRate(BDROM_PlayDirection direction, double dSpeed);

	virtual SonicError TimeSearch(Time90K pts90kOffset);

	// PlayItems can't be repeated
	virtual SonicError Repeat(void)				{ assert(false); return kBDROMNavStreamErrorNotSupported; }

	SonicError ChangeAngle(UInt32 uiAngleNumber);

public:
	UInt32						m_uiPlayItemId;
};


// BDROM_SonicPBCE_SubPlayItem
// A BDROM SubPlayItem
//
class BDROM_SonicPBCE_SubPlayItem : public BDROM_SonicPBCE_Item
{
public:
	BDROM_SonicPBCE_SubPlayItem(BDROM_SonicPBCE_Registers * pRegisters, BDROM_SonicPBCE_ClipManager * pClipManager);
	~BDROM_SonicPBCE_SubPlayItem();

	// Reset/Cleanup
	//

	void Reset(void);
	
	// Init to be done when opening an item
	//

	// Set SubPath ID, SubPlayItem Id, and Sync PlayItem Id
	void SetSubPathType(BDROM_SubPathType subPathType)	{ m_SubPathType = subPathType; }
	void SetSubPathId(UInt32 uiSubPathId)				{ m_uiSubPathId = uiSubPathId; }
	void SetSubPlayItemId(UInt32 uiSubPlayItemId)		{ m_uiSubPlayItemId = uiSubPlayItemId; }
	void SetSubClipId(UInt32 uiSubClipId)				{ m_uiSubClipId = uiSubClipId; }
	void SetSyncPlayItemId(UInt32 uiSyncPlayItemId)		{ m_uiSyncPlayItemId = uiSyncPlayItemId; }
	void SetSyncSubplayitemOutWithPlayitemOut(bool bSync)	{ m_bSyncSubplayitemOutWithPlayitemOut = bSync; }
	void SetSyncSubplayitemInWithPlayitemIn(bool bSync)		{ m_bSyncSubplayitemInWithPlayitemIn = bSync; }

	// clip open/close
	virtual SonicError OpenClip(BDROM_PlayDirection direction, double dSpeed);
	virtual SonicError CloseClip(void);

	// stream selection (stream selection can be done even during playback)
	// - In SelectStream: uiStreamNum 0 means no stream
	virtual SonicError SelectStream(BDROM_StreamType streamType, UInt32 uiStreamNum, bool bFlushOld = false);
	virtual SonicError StreamOnOff(BDROM_StreamType streamType, bool bOn, bool bIsText = false)	{ return 0; }	

	// Get item properties
	//

	// Set SubPath ID, SubPlayItem Id, and Sync PlayItem Id
	BDROM_SubPathType GetSubPathType(void)	{ return m_SubPathType; }
	UInt32 GetSubPathId(void)				{ return m_uiSubPathId; }
	UInt32 GetSubPlayItemId(void)			{ return m_uiSubPlayItemId; }
	UInt32 GetSubClipId(void)				{ return m_uiSubClipId; }
	UInt32 GetSyncPlayItemId(void)			{ return m_uiSyncPlayItemId; }
	bool   GetSyncSubplayitemOutWithPlayitemOut(void)	{ return m_bSyncSubplayitemOutWithPlayitemOut; }
	bool   GetSyncSubplayitemInWithPlayitemIn(void)		{ return m_bSyncSubplayitemInWithPlayitemIn; }

	// get IN/OUT time info
	Time90K GetInTime(void);
	Time90K GetOutTime(void);
    Time90K GetSyncStartTimeOfPlayItem(void);

	virtual SonicError Start(void);
	virtual SonicError Stop(void);
	virtual SonicError PauseReading(void);
	virtual SonicError ResumeReading(void);
	virtual Time90K GetStartPTS(void)			{ return 0; }	
	virtual SonicError PauseOn(void)			{ return 0; }	
	virtual SonicError PauseOff(void)			{ return 0; }	
	virtual SonicError SetRate(BDROM_PlayDirection direction, double dSpeed);

	SonicError TimeSearch(Time90K pts90kOffset);

	virtual SonicError Repeat(void);

public:
	BDROM_SubPathType			m_SubPathType;			// subpath that this item is part of
	UInt32						m_uiSubPathId;
	UInt32						m_uiSubPlayItemId;
	UInt32						m_uiSubClipId;
	UInt32						m_uiSyncPlayItemId;

	// CL0512: may actually need to set both start and end sync playitem ids. 
	//       this is for a case where playitem_id n and n+1 are seamless connected and need to load paths with same id ...

protected:
	// Flags to indicate if this subplayitem's IN/OUT time syncs with playitem's IN/OUT time.
	// Valid only for Sync ES, Sync PiP (incl. just Secondary Audio)
	// This can be used by PBCE to syhcnronize the processing end-of-clip/end-of-item conditions in main and sub paths
	//
	bool						m_bSyncSubplayitemOutWithPlayitemOut;
	bool						m_bSyncSubplayitemInWithPlayitemIn;
};


// PBCE_PathState
// State of a playback Path
//
typedef enum tagPBCE_PathState
{
	// Typical state changes:
	//	Idle --> ItemStart --> Playing --> ClipEnd -->  [ Still ] --> ItemEnd / Playing (for seamless connections)
	//

	kPBCE_PathState__Idle,		// Not yet initialized
	kPBCE_PathState__ItemStart,	// Initialized and ready for playback (but not currently playing) of current [Sub]PlayItem
	kPBCE_PathState__Playing,	// Playing current [Sub]PlayItem
	kPBCE_PathState__Paused,	// Paused
	kPBCE_PathState__ClipEnd,	// At End of [Sub]PlayItem's clip (but before any Still)		
	kPBCE_PathState__Still,		// In a Still state
	kPBCE_PathState__ItemEnd	// At End of [Sub]PlayItem. It is time to unload the clip
} PBCE_PathState;


// BDROM_SonicPBCE_Path
// A BDROM Playback Path
//
class BDROM_SonicPBCE_Path
{
public:
	BDROM_SonicPBCE_Path(BDROM_SonicPBCE_Registers * pRegisters, BDROM_SonicPBCE_ClipManager * pClipManager, PBCE_PathType pathType,
						 BDROM_SonicPBCE_EventSink * pEventSink, BDROM_StreamControl * pStreamControl);
	virtual ~BDROM_SonicPBCE_Path();

	// Reset
	//

	virtual void Reset(void);

	// Init for Path setup
	//

	void SetPlayListId(UInt32 uiPlayListId)								{ m_uiPlayListId = uiPlayListId; }
	void SetPlayListInfo(ref_ptr<BDROM::PlayList_mpls> pPlayList_mpls)	{ m_pPlayList_mpls = pPlayList_mpls; assert(m_uiPlayListId != BDROM_INVALID_PLAYLIST_ID);}
	void SetPathType(PBCE_PathType pathType)							{ m_PathType = pathType; }

	// unloads all items in path, but does not reset path info
	void Unload(void);

	// Get Path Info, and info from the items in it
	//

	// path state
	PBCE_PathState GetState(void)					{ return m_State; }
	bool IsProvidingData(void);

	// items in path
	// CurrentItem is the one that is playing
	// NexItem exists only at seamless item transitions (the next item is the one being read while the current item is playing)
	virtual BDROM_SonicPBCE_Item * GetCurrentItem(void)		{ return m_pCurrItem; }
	virtual BDROM_SonicPBCE_Item * GetNextItem(void)		{ return m_pNextItem; }

	// Playback control
	//

	virtual SonicError Stop(void)					= 0;
	virtual SonicError Start(void)					= 0;
	virtual bool IsStarted(void)					{ return (m_pCurrItem ? m_pCurrItem->IsStarted() : false); }

	virtual SonicError PauseReading(void)			= 0;
	virtual SonicError ResumeReading(void)			= 0;

	virtual SonicError PauseOn(void)				= 0;
	virtual SonicError PauseOff(void)				= 0;
	virtual bool IsPaused(void)						= 0;

	virtual SonicError StillOn(void)				= 0;
    virtual SonicError StillOff(void)				= 0;

	virtual SonicError SetRate(BDROM_PlayDirection direction, double dSpeed)			= 0;
	virtual bool       IsRateChangeAllowed(void)	= 0;

	// repeats current item
	virtual SonicError RepeatItem(void)				= 0;

	// Stream changes during playback
	//

	virtual SonicError ChangeStream(BDROM_StreamType streamType, UInt32 uiStreamNum)	= 0;		// uiStreamNum 0 means no stream
	virtual SonicError TurnStreamOnOff(BDROM_StreamType streamType, bool bOn)			= 0;

	// Playback Event Processsing
	//

	virtual void ElapsedTime(Time90K ts45kElapsedTime)				= 0;
	virtual void EndOfClip(void)									= 0;
	virtual void StillTimerElapsed(void)							= 0;
	virtual void EndOfItem(void)									= 0;	// unload/close current item
	virtual void AdvanceItem(void)									= 0;	// advance to next item

	virtual void SetPendingEndOfItem(bool pending)					= 0;
	virtual bool GetPendingEndOfItem()								= 0;
	virtual void SetPendingEndOfClip(bool pending)					= 0;
	virtual bool GetPendingEndOfClip()								= 0;

protected:
	// Item Management
	//
	virtual BDROM_SonicPBCE_Item * AllocateItem(void)				= 0;
	virtual void FreeItem(BDROM_SonicPBCE_Item * pItem)				= 0;
	
protected:
	BDROM_SonicPBCE_Registers *		m_pRegisters;
	BDROM_SonicPBCE_ClipManager *	m_pClipManager;
	BDROM_SonicPBCE_EventSink *		m_pEventSink;
	BDROM_StreamControl *			m_pStreamControl;

	PBCE_PathState					m_State;
	PBCE_PathState					m_OldState;

	UInt32							m_uiPlayListId;
	ref_ptr<BDROM::PlayList_mpls>	m_pPlayList_mpls;
	PBCE_PathType					m_PathType;			// Path type of this path. Once set, it will never be reset to 'unknown'

	BDROM_SonicPBCE_Item *			m_pCurrItem;		// currently playing Item
	BDROM_SonicPBCE_Item *			m_pNextItem;		// next Item to be played (and is being currently read). Valid only at seamless connection points

	bool							m_EndOfItemPending;	// status of pending end of item for subpath
	bool							m_EndOfClipPending;	// status of pending end of clip for subpath
};


// BDROM_SonicPBCE_MainPath
// SonicPlaybackControlEngine's Main-Path
//
class BDROM_SonicPBCE_MainPath : public BDROM_SonicPBCE_Path
{
public:
	BDROM_SonicPBCE_MainPath(BDROM_SonicPBCE_Registers * pRegisters, BDROM_SonicPBCE_ClipManager * pClipManager, BDROM_SonicPBCE_EventSink * pEventSink,
							 BDROM_TimerManager * pTimerManager, BDROM_StreamControl * pStreamControl);
	~BDROM_SonicPBCE_MainPath();

	// Reset
	//
	void Reset(void);

	// PlayItem Init
	//

	SonicError LoadPlayItem(UInt32 uiPlayItemId, Time90K ts45kOffsetIntoPlayItem = 0);
	SonicError LoadNextPlayItem(UInt32 uiPlayItemId);

	// Get Path Info, and info from the items in it
	//

	// items in path
	BDROM_SonicPBCE_PlayItem * GetItem(UInt32 uiPlayItemId);

	UInt32 GetNumberOfAngles(void);

	// Playback Control
	//

	virtual SonicError Start(void);
	virtual SonicError Stop(void);

	virtual SonicError PauseReading(void);
	virtual SonicError ResumeReading(void);

	virtual SonicError PauseOn(void);
	virtual SonicError PauseOff(void);
	virtual bool       IsPaused(void);

	virtual SonicError StillOn(void);
    virtual SonicError StillOff(void);
	virtual bool       IsStillOn(void);

	virtual SonicError SetRate(BDROM_PlayDirection direction, double dSpeed);
	virtual bool       IsRateChangeAllowed(void);
	virtual bool	   IsScanning(void);

	SonicError ChangeAngle(UInt32 uiAngleNumber);

	// repeats current item
	virtual SonicError RepeatItem(void)							{ return kBDROMNavStreamErrorNotSupported; }

	// Stream changes during playback
	//

	virtual SonicError ChangeStream(BDROM_StreamType streamType, UInt32 uiStreamNum);		// uiStreamNum 0 means no stream
	virtual SonicError TurnStreamOnOff(BDROM_StreamType streamType, bool bOn);

	// Playback Event Processsing
	//

	virtual void ElapsedTime(Time90K ts45kElapsedTime);
	virtual void EndOfClip(void);
	virtual void StillTimerElapsed(void);
	virtual void EndOfItem(void);								// unload/close current item
	virtual void AdvanceItem(void);								// advance to next item

	virtual void SetPendingEndOfItem(bool pending);
	virtual bool GetPendingEndOfItem();
	virtual void SetPendingEndOfClip(bool pending);
	virtual bool GetPendingEndOfClip();

protected:
	// Item Management
	//

	virtual BDROM_SonicPBCE_Item * AllocateItem(void);
	virtual void FreeItem(BDROM_SonicPBCE_Item * pItem);
	
protected:
	BDROM_TimerManager *			m_pTimerManager;
	ref_ptr<BDROM_Timer>			m_pTimedStillTimer;

	BDROM_SonicPBCE_PlayItem *		m_PlayItems[2];			// array of playitems
};


// BDROM_SonicPBCE_SubPath
// SonicPlaybackControlEngine's Sub-Path
//
class BDROM_SonicPBCE_SubPath : public BDROM_SonicPBCE_Path
{
public:
	BDROM_SonicPBCE_SubPath(BDROM_SonicPBCE_Registers * pRegisters, BDROM_SonicPBCE_ClipManager * pClipManager, BDROM_SonicPBCE_EventSink * pEventSink,
							BDROM_StreamControl * pStreamControl);
	~BDROM_SonicPBCE_SubPath();

	// Reset
	//

	void Reset(void);

	// SubPlayItem Init
	//

	SonicError LoadSubPlayItem(UInt32 uiSubPathId, UInt32 uiSubPlayItemId, UInt32 uiSubClipId, UInt32 uiSyncPlayItemId, Time90K ts45kOffsetIntoPlayItem = 0);
	SonicError LoadNextSubPlayItem(UInt32 uiSubPathId, UInt32 uiSubPlayItemId, UInt32 uiSubClipId, UInt32 uiSyncPlayItemId);

	// CL0512 m_STCOffset is never used.
    SInt64 GetSTCOffset(void) { return m_STCOffset; }
    void   SetSTCOffset(SInt64 startTime) { m_STCOffset = startTime; } 

	// Get Path Info, and info from the items in it
	//

	// items in path
	BDROM_SonicPBCE_SubPlayItem * GetItem(UInt32 uiSubPathId, UInt32 uiSubClipId);
	BDROM_SonicPBCE_SubPlayItem * GetItem(UInt32 uiSubPathId, UInt32 uiSubClipId, UInt32 uiSubPlayItemId);

	// Playback Control
	//

	virtual SonicError Start(void);
	virtual SonicError Stop(void);

	virtual SonicError PauseReading(void);
	virtual SonicError ResumeReading(void);

	// Since SubPaths don't pause independent of the MainPath, PauseOn and PauseOff return without doing anything,
	// and IsPaused() always returns false.
	// The PlayListManager should use the main path for pause operations
	//
	virtual SonicError PauseOn(void)			{ return kBDROMNavStreamErrorNoErr; }
	virtual SonicError PauseOff(void)			{ return kBDROMNavStreamErrorNoErr; }
	virtual bool IsPaused(void)					{ return false; }

	// Since SubPaths don't have a Still associated with them, StillOn and StilOff return without doing anything.
	//
	virtual SonicError StillOn(void)			{ return kBDROMNavStreamErrorNoErr; }
    virtual SonicError StillOff(void)			{ return kBDROMNavStreamErrorNoErr; }

	// since sub-path rate is the same as main-path rate, GetRate returns 'not available.' PlayListManager should get this
	// rate from the main-path
	//
	virtual SonicError SetRate(BDROM_PlayDirection direction, double dSpeed);
	virtual bool       IsRateChangeAllowed(void);

	// repeats current item
	virtual SonicError RepeatItem(void);

	// Stream changes during playback
	//

	// CL0512: Does change stream even apply to a SubPath 
	virtual SonicError ChangeStream(BDROM_StreamType streamType, UInt32 uiStreamNum)	{/* streamType; uiStreamNum; */return 0; }	
	virtual SonicError TurnStreamOnOff(BDROM_StreamType streamType, bool bOn)			{ /*streamType; bOn;*/ return 0; }	

	// Playback Event Processsing
	//

	virtual void ElapsedTime(Time90K ts45kElapsedTime);
	virtual void EndOfClip(void);
	virtual void StillTimerElapsed(void)							{ }	
	virtual void EndOfItem(void);									// unload/close current item
	virtual void AdvanceItem(void);									// advance to next item
	virtual void SetPendingEndOfItem(bool pending);
	virtual bool GetPendingEndOfItem();
	virtual void SetPendingEndOfClip(bool pending);
	virtual bool GetPendingEndOfClip();
				
protected:
	// Item Management
	//

	virtual BDROM_SonicPBCE_Item * AllocateItem(void);
	virtual void FreeItem(BDROM_SonicPBCE_Item * pItem);
	
protected:
	BDROM_SonicPBCE_SubPlayItem	*	m_SubPlayItems[2];			// array of subplayitems

    SInt64                          m_STCOffset;                // async PiP start time(90k), see spec 8.11.1.3
};

#endif

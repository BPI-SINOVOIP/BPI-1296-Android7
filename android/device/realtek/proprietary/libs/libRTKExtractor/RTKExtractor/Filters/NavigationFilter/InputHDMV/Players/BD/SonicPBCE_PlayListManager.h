//-----------------------------------------------------------------------------
// SonicPBCE_PlayListManager.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SONICPBCE_PLAYLISTMANAGER_H__
#define __SONICPBCE_PLAYLISTMANAGER_H__

#include "ref_ptr.h"
#include "SonicCriticalSection.h"
#include "SonicSemaphore.h"
#include "BaseRenderer.h"
#include "BDROM_NavigatorTypes.h"
#include "BDROM_NavigatorTypes_Internal.h"
#include "NotificationEventManager.h"
#include "BDROMNavigationCache.h"
#include "SonicPBCE_EventSink.h"
#include "SonicPBCE_Registers.h"
#include "BDROM_PlayList_PipMetaData.h"
#ifdef REALTEK_MODIFY
	#include <OSAL.h>
#else
	#include "BDROM_Settings.h"
#endif

class BDROM_SonicPBCE_SubPath;
class BDROM_SonicPBCE_MainPath;
class BDROM_SonicPBCE_ClipManager;
class BDROM_SonicPBCE_Clip;
class BDROM_StreamControl;
class BDROM_Timer;
class BDROM_TimerManager;

class BDROM_PacketSink;
namespace BDROM
{
	class pip_metadata;
}
class BDROM_SonicPBCE_Path;


//-------------------------------------------------------------------------------------------------------
// PlayList Manager
//

// BDROM_PlayListState
// State of a PlayList playback
//
typedef enum tagPBCE_PlayListState
{
	kPBCE_PlayListState__Idle,		// Not yet initialized
	kPBCE_PlayListState__Ready,		// Initialized and ready for playback (but not currently playing)
	kPBCE_PlayListState__Active		// Playing
} PBCE_PlayListState;

#define kPBCE_PLAYITEM_ID_UNDEFINED			0xFFFF			///< Undefined playitem_id
#define kPBCE_MARK_ID_UNDEFINED				0xFFFF			///< Undefined playlistmark_id

// PLMGRThread_Event
/// @brief	PlayList Manager Event
///
enum PLMGRThread_Action
{
	kPLMGRThread_Action__None,
	kPLMGRThread_Action__ElapsedTime,			// elapsed time
	kPLMGRThread_Action__StillTimerElapsed,		// Still Timer has elapsed
	kPLMGRThread_Action__EndOfClip,				// a clip/subclip (in any path) has been completely read
	kPLMGRThread_Action__EndOfItem,				// [Sub]PlayItem end has been reached
	kPLMGRThread_Action__PSRChanged,			// a PSR has been changed and requires more actions to be taken. 'uiPSRNum' has PSR that changed 
	kPLMGRThread_Action__PreRollDataRead,		// MainPath clip has read enough data for preroll (or has reached end of clip before preroll amount is read) 
	kPLMGRThread_Action__DeviceNew,				// New Device (after a device lost condition)
	kPLMGRThread_Action__Exit					// thread must exit
};

// PLMGR_PlayItemShuffleHistory
/// @brief PlayItem shuffle history for the current playlist
///
struct PLMGR_PlayItemShuffleHistory
{
	PLMGR_PlayItemShuffleHistory()				{	Reset(); }

	void Reset(void)							{	mPlayCount = 0; }
	void Add(UInt16 uiPlayItemId)				{	assert(mPlayCount < BDROM_MAX_PL_PLAYITEMS); mPlayItemIds[mPlayCount] = uiPlayItemId; IncPlayCount(); }
	void IncPlayCount(void)						{	
													mPlayCount++; 
												}
	bool IsPresent(UInt16 uiPlayItemId)			{	for (UInt16 i = 0; i < mPlayCount; i++)
													{
														if (mPlayItemIds[i] == uiPlayItemId)
															return true;
													}
													return false;

												}

	UInt16 PlayCount(void)						{	return mPlayCount; }

private:
	UInt16		mPlayItemIds[BDROM_MAX_PL_PLAYITEMS];	///< history of playitem ids
	UInt16		mPlayCount;								///< number of items in history
};

// PLMGR_ResumeInfo
/// @brief PlayItem resume info that cann't be stored in backup PSRs
///
struct PLMGR_ResumeInfo
{
public:
	PLMGR_ResumeInfo()					{	Clear(); }

	void Clear(void)					{
											m_bValid = false;
											m_PlayItemShuffleHistory.Reset();
										}
	void Store(PLMGR_PlayItemShuffleHistory & playItemShuffleHistory,
			   UInt32 titleNum, UInt32 chapterNum, UInt32 playListId, UInt32 playItemId, UInt32 presentationTime,
			   UInt32 selectedButton, UInt32 menuPageId, UInt32 userStyleNum, bool bPlaying)
										{
											m_PlayItemShuffleHistory = playItemShuffleHistory;
											m_Title_Num = titleNum;
											m_Chapter_Num = chapterNum;
											m_PlayList_Id = playListId;
											m_PlayItem_Id = playItemId;
											m_PresentationTime = presentationTime;
											m_SelectedButton = selectedButton;
											m_MenuPage_Id = menuPageId;
											m_UserStyle_Num = userStyleNum;
											m_bPlaying = bPlaying;
											m_bValid = true;
										}

public:
	bool								m_bValid;

	// backup registers (meant to be used mainly for debugging)
	UInt32								m_Title_Num;
	UInt32								m_Chapter_Num;
	UInt32								m_PlayList_Id;
	UInt32								m_PlayItem_Id;
	UInt32								m_PresentationTime;
	UInt32								m_SelectedButton;
	UInt32								m_MenuPage_Id;
	UInt32								m_UserStyle_Num;

	PLMGR_PlayItemShuffleHistory		m_PlayItemShuffleHistory;

	bool								m_bPlaying;					// If PBCE was playing when it was suspended
};


// PLMGR_StreamNumberInfo
/// @brief	Information about the a stream number
///
struct PLMGR_StreamNumberInfo
{
	PLMGR_StreamNumberInfo()		{ Reset(); }
	~PLMGR_StreamNumberInfo()		{ }

	void Reset(void)
	{
		mStreamType = kBDROM_StreamType_Unknown;
		mStreamNum = kBDROM_StreamNum__None;
		mbPresentationOn = false;
		mStreamEntryType = (UInt8)-1;
		mbInMainPath = false;
		mPlayItemId = BDROM_INVALID_PLAYITEM_ID;
		mSubPathId = BDROM_INVALID_PATH_ID;
		mSubClipId = BDROM_INVALID_SUBCLIP_ID;
		mPSRVal = 0;
	}

public:
	// stream info
	//
	BDROM_StreamType			mStreamType;			// Stream type. Can be 'unknown' if not in use.
	UInt32						mStreamNum;				// Stream Number. kBDROM_StreamNum__None if no stream
	bool						mbPresentationOn;		// Indicates is stream is to be presented or not (e.g. PG loaded but not being presented)

	// stream source info
	//
	UInt8						mStreamEntryType;		// Stream entry type, providing context to the four items below.
	bool						mbInMainPath;			// true if stream is part of main path, and false if it is in a subpath
	UInt32						mPlayItemId;			// PlayItem id / Sync PlayItem Id (in case of subpath)
	// if stream is coming from a sub path
	UInt32						mSubPathId;				// SubPath id
	UInt32						mSubClipId;				// SubClip id

	// PSR info
	UInt32						mPSRVal;				// the value that should be set to appropriate PSR
};

// PLMGRThread_Event
/// @brief	PlayList Manager Event
///
struct PLMGRThread_Event
{
	// members set by function sending an event
	//
	PLMGRThread_Action		mAction;			// action/event to perform

	union
	{
		BDROM_StreamSourceType	mStrmSrcType;		// clip's stream source type (used by EndOfClip event)
		PBCE_PathType			mPathType;			// path type (used by EndOfItem event)
		Time90K					mts45kElapsedTime;	// elapsed time (used by ElapsedTime event)
		UInt8					muiPSRNum;			// a PSR number
	} param1;

	union
	{
		bool				mbMainPath;			// indicates if main-path or sub-path associated with clip ended (used by EndOfClip event)
		UInt32				muiOldPSRVal;		// Old PSR value (used by kPLMGRThread_Action__PSRChanged evebt)
	} param2;

	// members set by the processing function
	//
	SonicError				mRtnCode;				// success/error code.

	PLMGRThread_Event()	{ mAction = kPLMGRThread_Action__None; }
};

// PLMGRThread_EventHistory
/// @brief	Stores history of last events (parameters sent with last event of each type that the PBCE generates), so that playlist manager can
///         optimize event sending by send events only when there is a change in the params for each event type
///
struct PLMGRThread_EventHistory
{
	UInt32				mPlayListStart_PLNum;		// kBDROM_EC_PL__PlayListStart: PlayList Number sent with last event
	UInt32				mPlayItemStart_PINum;		// kBDROM_EC_PL__PlayItemStart: PlayItem Number sent with last event
	UInt32				mChapterStart_ChapterNum;	// kBDROM_EC_TT__ChapterStart : Chapter Number sent with last event
	UInt32				mPlaylistMark_MarkNum;		// kBDROM_EC_PL__PlaybackMark : Mark Number sent with last event
	UInt32				mPlaylistTime_MediaTime;	// kBDROM_EC_TM__PlaylistTime : media time sent with last event
	UInt32				mAngleChange_AngleNum;		// kBDROM_EC_PL__AngleChange  : Angle number sent with last event
	bool				mStillOnOff_bStillOn;		// kBDROM_EC_PL__StillOn/Off  : 'true' if StillOn was sent. Set to false after StillOff is sent / non-seamless branch
	BDROM_PlayDirection	mRateChange_Direction;		// kBDROM_EC_PL__RateChange   : The Direction send with last event
	UInt32				mRateChange_Speed;			// kBDROM_EC_PL__RateChange   : The 'speed * 1000' value sent with last event
	UInt32				mPrimAudioChange_StrmNum;	// kBDROM_EC_PL__PrimaryAudioChange     : Stream Number sent with last event
	UInt32				mSecAudioChange_StrmNum;	// kBDROM_EC_PL__SecondaryAudioChange   : Stream Number sent with last event
	UInt32				mSecAudioChange_DispState;	// kBDROM_EC_PL__SecondaryAudioChange   : Presentation State sent with last event
	UInt32				mSecVideoChange_StrmNum;	// kBDROM_EC_PL__SecondaryVideoChange   : Stream Number sent with last event
	UInt32				mSecVideoChange_DispState;	// kBDROM_EC_PL__SecondaryAudioChange   : Presentation State sent with last event
	UInt32				mPGTextSTChange_StrmNum;	// kBDROM_EC_PL__PGTextSTChange         : Stream Number sent with last event
	UInt32				mPGTextSTChange_DispState;	// kBDROM_EC_PL__PGTextSTChange         : Presentation State sent with last event
	UInt32				mSecAudioAvailbl_NumStrms;	// kBDROM_EC_PL__SecondaryAudioAvailable: Number of available audio streams sent with last event.
	UInt32				mSecAudioAvailbl_SelBasis;	// kBDROM_EC_PL__SecondaryAudioAvailable: Secondary Audio stream selection basis sent with last event.
	UInt32				mSecVideoAvailbl_NumStrms;	// kBDROM_EC_PL__SecondaryVideoAvailable: Number of available video streams sent with last event.
	bool				mPopUpAvailbl_Available;	// kBDROM_EC_IG__PopUpMenuAvailable     : true if a PopUp available event was sent
	UInt32				mAnglesAvailbl_NumAngles;	// kBDROM_EC_PL__AnglesAvailable        : Number of angles sent with last event
	UInt32				mSubtitleAvailbl_NumStrms;	// mSubtitleAvailbl_NumStrms			: Number of subtitle streams available
	UInt32				mPiPSubtitleAvailbl_NumStrms;	// mPiPSubtitleAvailbl_NumStrms			: Number of PiPsubtitle streams available

	PLMGRThread_EventHistory()			{ Reset(); }
	void Reset(void)
	{
		mPlayListStart_PLNum = BDROM_INVALID_PLAYLIST_NUM;  mPlayItemStart_PINum = BDROM_INVALID_PLAYITEM_NUM;  mChapterStart_ChapterNum = BDROM_INVALID_CHAPTER_NUM; mPlaylistMark_MarkNum = BDROM_INVALID_MARK_NUM;
		mPlaylistTime_MediaTime = BDROM_INVALID_MEDIA_TIME;  mAngleChange_AngleNum = BDROM_INVALID_ANGLE_NUM;  mStillOnOff_bStillOn = false;
		mRateChange_Direction = kBDROM_UnknownPlayDirection; mRateChange_Speed = 0;
		mPrimAudioChange_StrmNum = BDROM_PRIMARY_AUDIO_STREAM_NONE;
		mSecAudioChange_StrmNum = BDROM_SECONDARY_AUDIO_STREAM_NONE; mSecAudioChange_DispState = 0;
		mSecVideoChange_StrmNum = BDROM_SECONDARY_VIDEO_STREAM_NONE; mSecVideoChange_DispState = 0;
		mPGTextSTChange_StrmNum = BDROM_PGTEXTST_STREAM_NONE; mPGTextSTChange_DispState = 0;
		mSecAudioAvailbl_NumStrms = 0; mSecAudioAvailbl_SelBasis = 0;  mSecVideoAvailbl_NumStrms = 0;
		mSubtitleAvailbl_NumStrms = 0; mPiPSubtitleAvailbl_NumStrms = 0;
		mPopUpAvailbl_Available = false;   mAnglesAvailbl_NumAngles = 0;
	}
};


// PBCE_FlushReason
// enumeration of reasons to flush data
//
typedef enum tagPBCE_FlushReason
{
	kPBCE_FlushReason__StopPlayback,			// flush because playlist playback is being stopped
	kPBCE_FlushReason__PlayListEnd,				// flush because playlist has ended normally
	kPBCE_FlushReason__NonSeamlessJump,			// flush because a non-seamless jump is about to be performed
	kPBCE_FlushReason__StartScan,				// flush because a scan operation is about to start
} PBCE_FlushReason;


// BDROM_PBCE_PlayListManager
// PlayList Manager class
//
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
class BDROM_PBCE_PlayListManager : public BDROM_SonicPBCE_EventSink
#else
class BDROM_PBCE_PlayListManager : public BDROM_SonicPBCE_EventSink, public SonicThread
#endif
{
public:
	BDROM_PBCE_PlayListManager(	ref_ptr<BaseRenderer> pRenderer,
								ref_ptr<BDROM_SonicPBCE_Registers> pBDROMRegisters,
								ref_ptr<BDROM_StreamControl> pStreamControl,
								ref_ptr<BDROM_TimerManager> pTimerManager,
								BDROM_DiscType discType,
							#ifdef REALTEK_MODIFY	
								void* pSettings);
							#else
								ref_ptr<BDROM_Settings> pSettings);
							#endif
									
	virtual ~BDROM_PBCE_PlayListManager();

	// PLMGR thread
	SonicError Run(void);
	
#ifdef 	REALTEK_MODIFY_DISABLE_SONICTHREAD
	static void ThreadEntry(void* pInstance);
#endif
	
private:
	SonicError PLMGRThread_Start(void);
	SonicError PLMGRThread_Run(void);
	SonicError PLMGRThread_Exit(void);

public:
	// Init or re-init
	SonicError Init(ref_ptr<BDROM::BDROM_NavigationCache> pNavCache);
	void       SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager);
	void       SetTitleType(BDROM_ObjectType objType, BDROM_TitleType titleType);

	ref_ptr<BDROM_SonicPBCE_Registers> GetRegisters(void)								{ return m_pRegisters; }
	SonicError EnableInteractiveGraphics(bool bEnable);
	UInt32 GetCurrentPlayListId(void)													{ return m_pRegisters->GetPSR(BDROM_PSRidx__PlayList_Id); }
	UInt32 GetCurrentPlayItemId(void)													{ return m_pRegisters->GetPSR(BDROM_PSRidx__PlayItem_Id); }

public:
	//---------------------------------------------------------------------------
	// Public API of the PlayListManager

	// special handling for Device Lost
	//
	// DeviceLost: pause nav, flush decoders (but pause Async paths), and store current elapsed time info
	SonicError DeviceLost(void);
	// DeviceNew: Timesearch to device lost time, and begin playback
	SonicError DeviceNew(void);

	// start/stop
	SonicError Stop(void);

	// Sends a MediaEvent to HDR
	SonicError MediaEvent(EMediaEventType type, const UInt32 dataSize = 0, void * pData = 0);

	// register access (only for external modules, like HDMV Module, to access, change registers)
	void GetGPR(UInt32 uiGPRNum, UInt32 &uiGPRValue);
	void SetGPR(UInt32 uiGPRNum, UInt32 uiGPRValue);
	void GetPSR(UInt8 uiPSRNum, UInt32 &uiPSRValue);
	SonicError SetPSR(UInt8 uiPSRNum, UInt32 uiPSRValue, bool bSetByBDJxlet = false);

	// For HDMV Module to process SetStream Nav command 
	// that set values for the Primary audio stream, PG TextST Stream, Angle and IG stream
	void SetPSRForPrimaryAudioStream(UInt32 uiStreamNum);
	void SetPSRForPGTextSTStream(bool bSetStrm, UInt32 uiStreamNum, UInt8 disp_s_flag);
	void SetPSRForIGStream(UInt32 uiStreamNum);
	void SetPSRForAngle(UInt32 uiAngleNum);

	// For HDMV Module to process SetSecondryStream Nav command 
	// that set values for the Secondary video, Secondary audio and PiP PG TextST Stream to be presented
	void SetPSRForPiPPGTextSTStream(bool bSetStrm, UInt32 uiStreamNum, UInt8 disp_s_flag, UInt8 PIP_PG_textST_valid_flag);
	void SetPSRForSecondaryVideoStream(bool bSetStrmNum,UInt32 uiStreamNum,bool bSetVideoSize,UInt32 uiVideoSize,UInt8 disp_v_flag);
	void SetPSRForSecondaryAudioStream(bool bSetStrm, UInt32 uiStreamNum, UInt8 disp_a_flag);

	// start and end playlist playback
	SonicError StartPlayList(UInt32 uiPlayListId, double bRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	SonicError StartPlayListAtPlayItem(UInt32 uiPlayListId, UInt32 uiPlayItemId = 0, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	SonicError StartPlayListAtMark(UInt32 uiPlayListId, UInt32 uiPlayListMarkId = 0, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	SonicError StartPlayListAtTime(UInt32 uiPlayListId, DVDTimeStampType tsTargetTime, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	SonicError TerminatePlayList(bool bDiscardResumeInfo = true, bool bInternalToPBCE = true);
	SonicError SuspendPlayList(void);
	SonicError ResumePlayList(void);
	void       DiscardResumeInfo(void);
	SonicError SetStopTime(DVDTimeStampType tsStopTime);
	SonicError GetStopTime(DVDTimeStampType &tsStopTime);

	// get playlist, playitem info
	SonicError GetPlayListPlaybackType(UInt32 uiPlayListId, BDROM_PlayList_PlaybackType & playlistPlaybackType);

	// error handling
	void FatalError(UInt32 majorCode = 0xFFFFFFFF , UInt32 minorCode = 0xFFFFFFFF );

	// navigate within current playlist
	SonicError TimeSearch(DVDTimeStampType tsTargetTime, double dRate = 1.0);
	SonicError JumpToMark(UInt32 uiPlayListMarkId, bool isUserOPeration = false);
	SonicError JumpToPlayItem(UInt32 uiPlayItemId);
    SonicError ForwardPlay(double dSpeed);
    SonicError BackwardPlay(double dSpeed);
	SonicError ForwardSkip(BDROM_MarkType uiMarkType = kBDROM_MarkType_ChapterMark);
	SonicError BackwardSkip(BDROM_MarkType uiMarkType = kBDROM_MarkType_ChapterMark, bool bOverride = false, bool bSkipOverMark = false);
	SonicError ChapterSearch(UInt32 uiChapterNum);
	
	SonicError PauseOn(void);
	bool       IsPaused(void);
	bool       IsStillOn(void);
	bool	   IsScanning(void);
	SonicError PauseOff(void);

	// Elapsed Time notification
	void SetElapsedTime(Time90K ts45kElapsedTime);

	// functions to get current status/location
    SonicError GetRate(BDROM_PlayDirection & direction, double & dRate);
	SonicError GetNumberOfChapters(UInt32 & uiNumChapters);
	SonicError GetCurrentChapter(UInt32 & uiCurrChapterNum);
	SonicError GetCurrentApplicationType(BDROM_ApplicationType & applType);
	SonicError GetChapterStartTime(UInt32 uiChapterNum, TimeValue & ts45kStartTime);
	SonicError GetChapterDuration(UInt32 uiChapterNum, TimeValue & ts45kDuration);
	SonicError GetCurrentElapsedTime(DVDTimeStampType & tsPlaybackElapsedTime);
	SonicError GetCurrentElapsedTime(UInt32 &tsPlaybackElapsedTime45k);
	SonicError GetCurrentPlayListDuration(DVDTimeStampType & titleDuration);
	SonicError GetCurrentPlayListDuration(TimeValue & ts45kTitleDuration);
	SonicError GetPlayListDuration(UInt32 uiPlayListId, DVDTimeStampType & titleDuration);
	SonicError GetPlayListDuration(UInt32 uiPlayListId, TimeValue & titleDuration);
	SonicError GetPlayItemDuration(UInt32 uiPlayItemNum, TimeValue & ts45kDuration);
	SonicError GetPlayItemStartTime(UInt32 uiPlayItemNum, TimeValue & ts45kPlayItemStartTime);
	SonicError GetNumberOfPlayItems(UInt32 uiPlayListNum, UInt32 & uiNumPlayItems);
	SonicError GetNumberOfSubPlayItems(UInt32 uiPlayListNum, UInt32 & uiNumSubPlayItems);
	SonicError GetNumberOfMarks(UInt32 uiPlayListNum, BDROM_MarkType markType, UInt32 & uiNumberOfMarks);
	SonicError GetMarkInfo(UInt32 uiPlayListNum, UInt32 uiMarkNum, BDROM_MarkType markType, BDROM_PL_MarkInfo & markInfo);
	SonicError GetCurrentLocation(BDROM_Location & currLocation);
	SonicError GetCurrentMarkNum(BDROM_MarkType markType, UInt32 & uiMarkNum);
	SonicError GetPlayListUOPsMask(UInt64 & UOPMask);

	// functions to get any stream's properties
	SonicError PLMGR_GetCurrentStream(BDROM_StreamType streamType, UInt32 & uiStreamsAvailable, UInt32 & uiCurrentStream);
	SonicError PLMGR_GetStreamAttributes(BDROM_StreamType streamType, UInt32 uiStreamNum, BDROM_Stream & streamAttr);

	// Stream Selection

	// Primary Audio and PG/TextST
	SonicError PrimaryAudioStreamChange(UInt32 audioStreamNum);
	SonicError PGTextSTStreamChange(UInt32 uiStreamNum, bool bChangeOnly = false);
	SonicError PGTextSTEnableDisable(bool bEnable);	// For either primary or PiP subtitles, whichever is the "current" stream

				
	// Secondary Video and Secondary Audio
	SonicError SecondaryVideoEnableDisable(bool bEnable, bool bEnableDisableSecondaryAudioToo = true);
	SonicError SecondaryVideoStreamChange(UInt32 uiStreamNum, bool bSelectSecondaryAudioToo = true);
	SonicError SecondaryVideoForceFullScaleChange(bool bForceFullScale);
	SonicError SecondaryAudioEnableDisable(bool bEnable);
	SonicError SecondaryAudioStreamChange(UInt32 uiStreamNum);

	// PiP PG/TextST stream change
	SonicError PiPPGTextSTStreamChange(UInt32 uiStreamNum);

	// angle selection
	SonicError GetCurrentAngle(UInt32 & uiNumberOfAngles, UInt32 & uiCurrentAngle);
	SonicError SelectAngle(UInt8 uiAngleNumber);

	// Still switch
    SonicError StillOn();
    SonicError StillOff();
	void StillTimerElapsed(void);

	// query information
	BDROM_SonicPBCE_SubPath * GetSubPath(UInt32 uiSubPathId, UInt32 uiSubClipEntryId);
	SonicError GetVideoAttributes(BDROM_StreamType, BDROM_VideoAttributes & vatr);
	SonicError GetAudioAttributes(UInt32 uiStreamNum, BDROM_StreamType, BDROM_AudioAttributes & aatr);
//	SonicError GetPGTextSTAttributes(UInt32 uiStreamNum, BDROM_PGTextSTAttributes & pgtxtatr);
//	SonicError GetIGAttributes(UInt32 uiStreamNum, BDROM_StreamType, BDROM_IGAttributes & igatr);
	SonicError IsSecondaryVideoDisplayed(bool & bIsPiPDisplayed);
	SonicError IsSecondaryVideoEnabled(bool & bIsEnabled);
	bool       IsPiPAvailable(void);
	bool       PlaylistHasPiP(UInt32 playlistId);

	// preferences
	void SetPreferredAudioLanguage(const char * pLangCode);
	void SetPreferredMenuLanguage(const char * pLangCode);
	void SetPreferredPGTextSTLanguage(const char * pLangCode);
	void SetAudioCapability(UInt32 uiLPCM192, UInt32 uiLPCM48and96, UInt32 uiDDpIndep, UInt32 uiDDpDep,
							UInt32 uiDTSHDCore, UInt32 uiDTSHDExtn, UInt32 uiDolbyDigital, UInt32 uiDolbyMLP);
	void SetParentalLevel(UInt8 uiParentalLevel);
	void SetPlayerProfile(UInt8 uiPlayerProfile);
	void SetParentalLevelChangeByBDJxletAllowed(bool bAllowed)			{ m_bParentalLvlPSRChangeByBDJxletAllowed = bAllowed; }
	void SetCountryCodeChangeByBDJxletAllowed(bool bAllowed)			{ m_bCountryCodePSRChangeByBDJxletAllowed = bAllowed; }
	void SetLanguageCodeChangeByBDJxletAllowed(bool bAllowed)			{ m_bLangPSRChangeByBDJxletAllowed = bAllowed; }
	void AuthorizeParentalLevelChange(bool bGranted);

	//---------------------------------------------------------------------------------
	// API for breaking and continuing playback - For Emulator use only

	SonicError Break(void);
	SonicError Continue(void);

public:
	// ----------------------------------------------------------------------
	// BDROM_SonicPBCE_EventSink Interface implementation
	//
	virtual void EndOfClip(PBCE_PathType pathType, bool bMainPath);
	virtual void PreRollDataRead(void);
	virtual void EndOfItem(PBCE_PathType pathType);
	virtual void SendEvent(BDROM_Event & evt);

			//--------------------------------------------------------------------------
			///REALTEK_MODIFY
			/// For user operations like TitleSearch, MenuCall, etc, 
			///	We want to abort StartPaths() so that HDMV_Module can enter
			///	its own critical section. And make sure it is followed by TerminatePlayList().
			///
			void PreTerminatePlayList() { m_bInTerminatePlaylist = true; }
			void SendEventTotalChapterNum();
			void SendEventChapterTimeInfo(UInt32 chapterNum);
			void SendEventCurrentPlaylistDuration();
			void SendStreamStatus(BDROM_StreamType streamType, BDROM_EventCode eventCode, int numStreams);
				
			
protected:
	// SubPathDelayLoadInfo:
	// Stores info about subpaths that need to be loaded during playback of current playitem
	//
	class SubPathDelayLoadInfo
	{
	public:
		class StreamLoadInfo
		{
		public:
			StreamLoadInfo()
				: m_bLoad(false)
				, m_SyncStartPlayItemId(BDROM_INVALID_PLAYITEM_ID)
				, m_SyncStartPTS45k(0)
				, m_SubPlayitemId(BDROM_INVALID_PLAYITEM_ID)
			{
			}

			void Clear(void)
			{
				m_bLoad = false;
				m_StreamNumInfo.Reset();
				m_SyncStartPlayItemId = BDROM_INVALID_PLAYITEM_ID;
				m_SyncStartPTS45k = 0;
				m_SubPlayitemId = BDROM_INVALID_PLAYITEM_ID;
			}

			void Init(PLMGR_StreamNumberInfo streamNumInfo, UInt32 syncStartPlayitemId, UInt32 syncStartPTS45k, UInt32 subPlayitemId)
			{
				m_StreamNumInfo = streamNumInfo;
				m_SyncStartPlayItemId = syncStartPlayitemId;
				m_SyncStartPTS45k = syncStartPTS45k;
				m_SubPlayitemId = subPlayitemId;
				m_bLoad = true;
			}
		public:
			bool					m_bLoad;
			PLMGR_StreamNumberInfo	m_StreamNumInfo;
			UInt32					m_SyncStartPlayItemId;
			UInt32					m_SyncStartPTS45k;
			UInt32					m_SubPlayitemId;
		};

	public:
		void Clear(void)
		{
			m_bValid = false;
			m_SecondaryVideo.Clear();
			m_SecondaryAudio.Clear();
			m_PGTextST.Clear();
			m_IG.Clear();
		}

		void InitPath(BDROM_StreamType streamType, PLMGR_StreamNumberInfo streamNumInfo,
					  UInt32 syncStartPlayitemId, UInt32 syncStartPTS45k, UInt32 subPlayitemId)
		{
			switch (streamType)
			{
			case kBDROM_StreamType_SecondaryVideo:
				m_SecondaryVideo.Init(streamNumInfo, syncStartPlayitemId, syncStartPTS45k, subPlayitemId);
				break;
			case kBDROM_StreamType_SecondaryAudio:
				m_SecondaryAudio.Init(streamNumInfo, syncStartPlayitemId, syncStartPTS45k, subPlayitemId);
				break;
			case kBDROM_StreamType_PiP_PGTextST:
			case kBDROM_StreamType_PGTextST:
				m_PGTextST.Init(streamNumInfo, syncStartPlayitemId, syncStartPTS45k, subPlayitemId);
				break;
			case kBDROM_StreamType_IG:
				m_IG.Init(streamNumInfo, syncStartPlayitemId, syncStartPTS45k, subPlayitemId);
				break;
		#ifdef REALTEK_MODIFY
			default:
				break;
		#endif
			}

			m_bValid = true;
		}

		void ClearPath(BDROM_StreamType streamType)
		{
			switch (streamType)
			{
			case kBDROM_StreamType_SecondaryVideo:
				m_SecondaryVideo.Clear();
				break;
			case kBDROM_StreamType_SecondaryAudio:
				m_SecondaryAudio.Clear();
				break;
			case kBDROM_StreamType_PiP_PGTextST:
			case kBDROM_StreamType_PGTextST:
				m_PGTextST.Clear();
				break;
			case kBDROM_StreamType_IG:
				m_IG.Clear();
				break;
		#ifdef REALTEK_MODIFY
			default:
				break;
		#endif	
			}

			if ((!m_SecondaryVideo.m_bLoad) && (!m_SecondaryAudio.m_bLoad) && (!m_PGTextST.m_bLoad) && (!m_IG.m_bLoad))
				m_bValid = false;
		}
	public:
		bool			m_bValid;

		StreamLoadInfo	m_SecondaryVideo;

		StreamLoadInfo	m_SecondaryAudio;
		StreamLoadInfo	m_PGTextST;

		StreamLoadInfo	m_IG;
	};


private:
	//---------------------------------------------------------------------------
	// Internal functions of the PlayListManager

	void Reset(void);
	void ResetPlaybackRate(bool bResetIgTxtDecoderRate = false);
	SonicError ResetVideoResolution(void);

	// Playlist, playItem loading
	SonicError LoadPlayListAtMark(UInt32 uiPlayListMarkId, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	SonicError LoadPlayListAtPlayItem(UInt32 uiPlayItemId, UInt32 ui45kTimeOffset = 0, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	SonicError LoadPlayItem(UInt32 uiPlayItemId, Time90K ts45kOffsetIntoPlayItem = 0);
	ref_ptr<BDROM::PlayList_mpls> LoadPlayListInfoFile(UInt32 uiPlayListId);
	void PlayListInfoFileLoaded(UInt32 uiPlayListId);
	SonicError GetFirstPlayItemId(UInt32 & uiPlayItemId);
	SonicError SetFirstPlayItemId(UInt32 uiPlayItemId);
	SonicError GetNextPlayItemId(UInt32 & uiPlayItemId, UInt32 uiLastLoadedPlayItemId = 0xFFFFFFFF);
	void       SetPathPlayListInfo(UInt32 uiPlayListId, ref_ptr<BDROM::PlayList_mpls> pPlayList_mpls);

	// gets ref_ptr to PlaylistInfo without changing player state.
	ref_ptr<BDROM::PlayList_mpls> GetPlaylistMpls(UInt32 playlistId);

	// for seamless connectivity
	SonicError LoadNextPlayItem(UInt32 uiPlayItemId);

	// updating state, registers, event generation
	void AtPlaylistStart(void);
	void AtPlayItemStart(UInt32 uiPlayItemId, Time90K timeOffset);
	void UpdateChapterNum(void);
	void UpdatePSR(BDROM_RegPSR_Index PSRidx, UInt32 uiPSRVal);
	void SendMediaSelectEvent(UInt32 mediaSelectTestFlags);

	// flags for passing params to SendMediaSelectEvent
	// individual:
	static const UInt32 mstfPrimaryVideo		= 0x01;
	static const UInt32 mstfPrimaryAudio		= 0x02;
	static const UInt32 mstfSecondaryVideo		= 0x04;
	static const UInt32 mstfSecondaryAudio		= 0x08;
	static const UInt32 mstfPrimarySubtitle		= 0x10;
	static const UInt32 mstfPiPSubtitle			= 0x20;
	// combos:
	static const UInt32 mstfAllStreams			= 0x3F;
	static const UInt32 mstfSubtitle			= 0x30;

	// get info
	SonicError GetMarkLocation(UInt32 uiPlayListMarkId, UInt32 & uiMarkPlayItemId, Time90K & ts45kOffsetIntoMarkPlayItem);
    SonicError GetTimeLocation(Time90K pts45KhzTargetTime, UInt32 & uiPlayItemId, Time90K & ts45kOffsetIntoPlayItem);
    SonicError GetPiPTimeLocation(Time90K pts45KhzTargetTime, UInt32 & uiSubPlayItemId, Time90K & ts45kOffsetIntoSubPlayItem);
	SonicError GetChapterMarkId(UInt32 uiChapterNum, UInt32 & uiChapterMarkId);
	SonicError GetChapterNum(UInt32 uiPlayItemId, Time90K ts45kPlayItemTime, UInt32 & uiChapterNum);
	SonicError GetMarkNum(UInt32 uiPlayItemId, Time90K ts45kPlayItemTime, BDROM_MarkType markType, UInt32 & uiMarkNum);
	SonicError GetPlaybackMarkId(UInt32 uiPlayItemId, Time90K ts45kPlayItemTime, UInt32 & uiMarkID);
	SonicError GetMarkId(UInt32 uiMarkNum, BDROM_MarkType markType, UInt32 & uiMarkId);
	bool       IsBrowsableSlideshow(void);		// is current playlist a browsable slideshow

	// Starting and stopping paths
	SonicError StartPaths(void														);
	SonicError StopPaths(bool bStopPreloadPaths = true);
	SonicError PauseOnPaths(void);
	SonicError PauseOffPaths(void);
	SonicError PauseReadingInPaths(bool bPausePrimaryAudioForBrowsableSlideshow = true);
	SonicError ResumeReadingInPaths(void);
	void       FlushPaths(PBCE_FlushReason flushReason);
	void       StartPtsCallbackHandler(UInt32 uiPlayitemId, Time90K startTime45k, double dRate);
	void       StopPtsCallbackHandler(void);
	void       ForceStartPtsCallbackHandler();
	void       BeginPreRoll(void);
	void       EndPreRoll(void);
	bool       IsAsyncSubpathActive(void);

	// Font loading/unloading
	SonicError LoadFonts(void);
	SonicError UnloadFonts(void);

	// end of clip/playitem/subplayitem processing, still timer and elapsed time processing
	void ProcessElapsedTime(Time90K ts45kElapsedTime);
	void ProcessStillTimerElapsed(void);
	void ProcessEndOfClip(PBCE_PathType pathType, bool bMainPath);
	void ProcessEndOfItem(PBCE_PathType pathType);
	void ProcessPSRChanged(UInt8 uiPSRNum, UInt32 uiOldPSRValue);
	bool ValidateSetPSR(UInt8 uiPSRNum, UInt32 & uiPSRValue, bool & bActive);
	bool IsSettingPSR(UInt8 uiPSRNum);
	bool IsStatusPSR(UInt8 uiPSRNum);
	bool GetPermissionForSettingPSRChangeByBDJxlet(UInt8 uiPSRNum, UInt32 uiPSRValue);

	void ProcessEndOfPlayItemClip(void);
	void ProcessEndOfSubPlayItemClip(PBCE_PathType pathType);
	void ProcessEndOfPlayItem(void);
	void ProcessEndOfSubPlayItem(PBCE_PathType pathType, bool bStart);

	// functions to get any stream's properties
	SonicError SetStreamDefaultAttributes(BDROM_StreamType streamType, BDROM_Stream & streamAttr);
	SonicError GetCurrentStream(BDROM_StreamType streamType, UInt32 & uiStreamsAvailable, UInt32 & uiCurrentStream);
	SonicError GetStreamAttributes(BDROM_StreamType streamType, UInt32 uiStreamNum, BDROM_Stream & streamAttr);
	SonicError GetStreamAttributes(BDROM::STN_table * pSTNTable, BDROM_StreamType streamType, UInt32 uiStreamNum, BDROM_Stream & streamAttr);
	void       GetStreamNumberInfo(BDROM_StreamType streamType, UInt32 streamNumber, PLMGR_StreamNumberInfo & streamNumInfo);
	SonicError GetSubPathStreamAttributes(UInt32 uiSubPathId, UInt32 uiSubClipId, UInt32 uiPID, BDROM_Stream & streamAttr);

	//-------------------------------
	// Stream Selection and changing
	//


	// Primary Video
	//

	// for initially selecting
	void ChoosePrimaryVideoStream(UInt32 uiPlayItemId, PLMGR_StreamNumberInfo & streamNumInfo);
	SonicError LoadPrimaryVideoStream(const PLMGR_StreamNumberInfo & streamNumInfo);
	SonicError SelectPrimaryVideoStream(UInt32 uiPlayItemId);

	// Primary Audio
	//

	// for initially selecting
	void ChoosePrimaryAudioStream(UInt32 uiPlayItemId, PLMGR_StreamNumberInfo & streamNumInfo);
	SonicError LoadPrimaryAudioStream(const PLMGR_StreamNumberInfo & streamNumInfo, bool bUpdatePSRs);
	SonicError SelectPrimaryAudioStream(UInt32 uiPlayItemId, bool bUpdatePSRs);

	// for changing
	SonicError ChangePrimaryAudioStream(UInt32 uiStreamNum, bool bUpdatePSR, bool bSendEvent);

	// common
	bool       IsAudioPlaybackCapable(BDROM_Stream & streamAttr, bool bSpecCheck);
	bool       IsLanguageCodeMatchWithPlayerSetting(BDROM_Stream & streamAttr);
	bool       IsLanguageCodeMatch(BDROM_Stream & streamAttr, const BDROM_Stream& src_attr);
	bool       IsAudioSurroundCapable(BDROM_Stream & streamAttr);
	SonicError AutoSelectNewPrimaryAudioStream(UInt32 & uiStreamNum);


	// IG
	//

	// for initially selecting
	void ChooseIGStream(UInt32 uiPlayItemId, PLMGR_StreamNumberInfo & streamNumInfo);
	SonicError LoadIGStream(const PLMGR_StreamNumberInfo & streamNumInfo, bool bUpdatePSRs, bool bFlush);
	SonicError SelectIGStream(UInt32 uiPlayItemId, bool bUpdatePSRs);

	// for changing during playback
	SonicError ChangeIGStream(UInt32 uiStreamNum);

	// PG/TextST
	//

	// for initially selecting
	void ChoosePGTextSTStream(UInt32 uiPlayItemId, BDROM_StreamType streamType, PLMGR_StreamNumberInfo & streamNumInfo);
	SonicError LoadPGTextSTStream(const PLMGR_StreamNumberInfo & streamNumInfo, Time90K ts45kOffsetIntoPlayItem, bool bUpdatePSRs);
	SonicError LoadPGTextSTSubPath(const PLMGR_StreamNumberInfo & streamNumInfo, UInt32 uiSubPlayItemId, Time90K ts45kOffsetIntoItem, bool bUpdatePSRs);
	SonicError SelectPGTextSTStream(UInt32 uiPlayItemId, Time90K ts45kOffsetIntoPlayItem, bool bUpdatePSRs);
	BDROM_SonicPBCE_SubPath * GetTargetPGTextSTSubPath(const PLMGR_StreamNumberInfo & streamNumInfo);

	// for changing, and enabling/disabling
	SonicError ChangePGTextSTStream(UInt32 uiStreamNum, BDROM_StreamType streamType, bool bForceEnable, bool bUpdatePSR, bool bSendEvent);
	SonicError CheckPGTextSTStream(UInt32 uiStreamNum, BDROM_StreamType streamType, UInt32 & uiCheckedStreamNum);
	SonicError EnablePGTextSTStream(bool bEnable, bool bUpdatePSR, bool bSendEvent);

	BDROM_SonicPBCE_Path * GetPGTextSTPath(void);
	BDROM_SonicPBCE_Path * GetPiPPGTextSTPath(void);

	// Secondary Video
	//
	// for initially selecting
	void ChooseSecondaryVideoStream(UInt32 uiPlayItemId, PLMGR_StreamNumberInfo & streamNumInfo);
	SonicError LoadSecondaryVideoStream(UInt32 uiPlayItemId, Time90K ts45kOffsetIntoPlayItem, const PLMGR_StreamNumberInfo & streamNumInfo, bool bUpdatePSRs);
	SonicError LoadSecondaryVideoSubPath(const PLMGR_StreamNumberInfo & streamNumInfo, UInt32 uiSubPlayItemId, Time90K ts45kOffsetIntoPlayItem, bool bUpdatePSRs);
	SonicError SelectSecondaryVideoStream(UInt32 uiPlayItemId, Time90K ts45kOffsetIntoPlayItem, bool bUpdatePSRs);
	SonicError BeginConfigurePiPSetting(UInt32 uiStreamNum, BDROM_SonicPBCE_Path * pSecVidSubPath, UInt32 uiPlayItemId, Time90K ts45kMainPathPTS);
	void       ContinueConfigurePiPSetting(UInt32 uiPlayItemId, Time90K ts45kMainPathPTS, Time90K ts45kAsyncPathPTS);

	// for changing, and enabling/disabling
	SonicError SecondaryVideoStreamChangeInternal(UInt32 uiStreamNum, bool bForceEnableVideo = true, bool bForceEnableAudioToo = true);
	SonicError ChangeSecondaryVideoStream(UInt32 uiStreamNum, bool bForceEnableVideo, bool bForceEnableAudioToo, bool bUpdatePSR, bool bSendEvent);

	// common
	bool       IsProfile_1dot1_Or_2(void);
	bool       IsVideoPlaybackCapable(BDROM_Stream & streamAttr);
	SonicError GetPiPScaledSize(BDROM_VideoFormat primVidFmt, BDROM_VideoAspectRatio primVidAspectRatio, BDROM_VideoFormat secVidFmt, BDROM_VideoAspectRatio secVidAspectRatio, UInt8 pipScale,
								bool bForceFullScreen, UInt32 & pipWidth, UInt32 & pipHeight, UInt32 & pipX, UInt32 & pipY, bool & bPillarBox);
	bool       IsPiPDisplayed(void);
	BDROM_SonicPBCE_Path * GetSecondaryVideoPath(void);

	// Secondary Audio
	//

	// for initially selecting
	void ChooseSecondaryAudioStream(UInt32 uiPlayItemId, PLMGR_StreamNumberInfo & streamNumInfo);
	SonicError LoadSecondaryAudioStream(UInt32 uiPlayItemId, Time90K ts45kOffsetIntoPlayItem, const PLMGR_StreamNumberInfo & streamNumInfo, bool bUpdatePSRs);
	SonicError LoadSecondaryAudioSubPath(const PLMGR_StreamNumberInfo & streamNumInfo, UInt32 uiSubPlayItemId, Time90K ts45kOffsetIntoItem, bool bUpdatePSRs);
	SonicError SelectSecondaryAudioStream(UInt32 uiPlayItemId, Time90K ts45kOffsetIntoPlayItem, bool bUpdatePSRs);

	// for changing, and enabling/disabling
	SonicError SecondaryAudioStreamChangeInternal(UInt32 secAudVidPSR, UInt32 uiStreamNum);
	SonicError EnableDisableSecondaryAudio(bool bEnable);
	SonicError ChangeSecondaryAudioStream(UInt32 uiStreamNum, bool bForceEnable, bool bUpdatePSR, bool bSendEvent);

	// common
	bool       IsSecondaryAudioDisplayed(void);
	BDROM_SonicPBCE_Path * GetSecondaryAudioPath(void);


	//-------------------------------
	// Angle Change/Selection
	//

	SonicError ChangeAngle(UInt32 uiAngleNumber);		// switches current angle in a multi-angle stream

	// preferences
	bool AskForParentalLevelChangeAuthorization(UInt32 newPtlLvl);


	//-------------------------------
	// special handling for Device Lost
	//
	void ProcessDeviceNew(void);

	// checks for availability of clips for a give playitem/subplayitem
	//
	bool ClipsAvailable(UInt32 playitemId);
	bool ClipsAvailable(UInt32 subPathId, UInt32 subPlayitemId);
	bool ClipsAvailable(std::string & clipName);
private:
	ref_ptr<BDROM::BDROM_NavigationCache>			m_pNavCache;
	ref_ptr<NotificationEventManager>				m_pNEManager;
	ref_ptr<BDROM_StreamControl>					m_pStreamControl;
	BDROM_DiscType									m_discType;
	
#ifdef REALTEK_MODIFY	
	void*											m_pSettings;
#else	
	ref_ptr<BDROM_Settings>							m_pSettings;
#endif
	ref_ptr<BDROM::PTSCallbackHandler>				m_PtsCallback;


	// outputs
	UInt32											m_uiNumOutputStreams;		// number of rt_stream outputs from the PBCE

	// current state, location
	PBCE_PlayListState								m_State;
	BDROM_RandomGen									m_RndPlayItemIdGen;			///< random playitem id generator
	PLMGRThread_EventHistory						m_EventHistory;				// history of previous event params
	volatile bool									m_bInTerminatePlaylist;		// true for the duration of TerminatePlaylist call

	// Current Title Information
	BDROM_ObjectType								m_ObjectType;				///< Current FirstPlay/TopMenu/Title Object Type
	BDROM_TitleType									m_TitleType;				///< Current FirstPlay/TopMenu/Title Playback type
	PLMGR_ResumeInfo								m_ResumeInfo;				///< extra resume info

	// Current PlayList Information
	ref_ptr<BDROM::PlayList_mpls>					m_pPlayList_mpls;
	PLMGR_PlayItemShuffleHistory					m_PlayItemShuffleHistory;

	// Current PlayItem Information
	UInt32											m_uiPlayItemId;
	bool											m_bIGEnabled;
	std::vector<BDROM_FontDesciption>				m_FontDescList;				// list of loaded fonts (for TextST subpath)

	// Current PiP & its PiP Metadata
	BDROM_SubPathType								m_PiPType;					// Current PiP type (None / InMux-Sync / OutOfMux-Sync / OutOfMux-Async) 
	BDROM::pip_metadata								m_pip_metadata;				// PiP metadata for whole playlist
	UInt32											m_PiPMetadataBlockIdx;		// Index (0 based) of curr metadata block in use
	UInt32											m_PiPMetadataBlockEntryIdx;	// Index (0 based) of curr metadata block entry in curr metatdata block.
	bool											m_bPiPForceFullScaleChange;	// flag used to tell pip metadata handler to assert full forced full scale change.

	// Next PlayItem Information (valid only for the duration of seamless playitem transitions)
	// <nothing here yet>

	// Main-Path and Sub-Path members
	BDROM_SonicPBCE_MainPath *						m_pMainPath;				// Main Path
	BDROM_SonicPBCE_SubPath  *						m_pSubPath_InMux;			// In-Mux Sub-Path (i.e. Mux-ed in with MainPath)
	BDROM_SonicPBCE_SubPath  *						m_pSubPath_OutofMux;		// Out-Of-Mux Sub-Path (i.e. not Mux-ed in with MainPath)
	BDROM_SonicPBCE_SubPath  *						m_pSubPath_Preload_Txt;		// Text Preload Sub-Path
	BDROM_SonicPBCE_SubPath  *						m_pSubPath_Preload_IG;		// IG Preload Sub-Path

	SubPathDelayLoadInfo							m_SubPathDelayLoadInfo;		// sub path 'delay-load' info

	// Registers
	ref_ptr<BDROM_SonicPBCE_Registers>				m_pRegisters;

	// flags related to controlling BDJ xlet initiated Setting PSRs change
	bool											m_bParentalLvlPSRChangeByBDJxletAllowed;
	bool											m_bLangPSRChangeByBDJxletAllowed;
	bool											m_bCountryCodePSRChangeByBDJxletAllowed;
	bool											m_bParentalLvlChangeReqGranted;
	BDROMNavEvent									m_ParentlLvlAuthEvent;

	// clips
	ref_ptr<SonicCriticalSection>					m_pcsRead;					///< Serializes Nav file opening/reading and AV File reading functions
	BDROM_SonicPBCE_ClipManager *					m_pClipManager;

	// Renderer, Decoders
	ref_ptr<BaseRenderer>							m_pRenderer;				

	// PLMGR Thread, thread synchronization, events, event queue
	bool											m_bPLMGRThreadExit;			///< flag to indicate if PLMGR thread should exit
	SonicCriticalSection 							m_csApiLock;				///< Lock to serialize threads accessing/modifying intenal data
	PLMGRThread_Event								m_CurrEvent;				///< Event to be processed by PlayList Manager thread
	std::vector<PLMGRThread_Event>					m_EventQueue;				///< Event queue of the PLMGR thread
	SonicCriticalSection 							m_csEventQueueLock;			///< lock to access event queue and m_ElapsedTimeEvent
	SonicSemaphore									m_semEventsAvailable;		///< semaphore for event availability

	ref_ptr<BDROM_TimerManager>						m_pTimerManager;

	// Device Lost info. Used for recovering from Device Lost
	bool											m_bDeviceLost;					// true if device has been lost
	Time90K											m_ts45kDeviceLostOffsetTime;	// offset into current item at which we lost device in current playitem
	UInt32											m_DeviceLostPlayItemId;			// playitem at which we lost the device
	DVDTimeStampType								m_tsDeviceLostElapsedTime;		// Elapsed time at which we lost device. could be 0 for non-seq playlists

	// For Break/Continue
	bool											m_bInBreak;					// true for the duration between Break() and Continue()
	bool											m_bEndPrerollOnContinue;	// true if PreRoll should be ended when Continue() is called.

	// pause
	UInt32											m_PauseStartTimeMs;

	// stop
	Time90K											m_tsPlaybackStopTime;

protected:
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	CThread* 										m_pThread;
#endif
};

#endif

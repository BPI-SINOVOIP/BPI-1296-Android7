//-----------------------------------------------------------------------------
// BDROM_Navigator.h
// Copyright (c) 2000 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------


#ifndef __BD_ROM_NAVIGATOR_H__
#define __BD_ROM_NAVIGATOR_H__

#include "BDROM_NavigatorTypes.h"
#include "ModuleManager.h"
#include "UserEventManager.h"

class CanvasManager;
class PlaybackControlEngine;
class BaseRenderer;
class PresentationGraphicsDecoder;
class InteractiveGraphicsDecoder;
class TextSubtitleDecoder;
class BDROM_SoundPlayer;
class BDROM_TimerManager;
#ifdef ENABLE_BDJ_TITLE
	class BDROM::VFSManager;
#endif		
class BDROM_Settings;


class BDROM_Navigator
{
public:
	BDROM_Navigator(ref_ptr<BaseRenderer>pRenderer,
					UInt32 rendererCanvas,
					ref_ptr<BDROM::BDROM_NavigationCache> pNavCache,
					ref_ptr<CanvasManager> pCanvasManager,
					ref_ptr<PlaybackControlEngine> pPlaybackEngine,
					PresentationGraphicsDecoder * pPGDecoder,
					InteractiveGraphicsDecoder * pIGDecoder, 
					TextSubtitleDecoder * pTextSTDecoder,
					ref_ptr<BD_FS_Layer> pLayer,
					BD_FS_DeviceHandle BDROMDeviceHandle,
					ref_ptr<BDROM_TimerManager> pTimerManager,
					ref_ptr<BDROM_SoundPlayer> pSoundPlayer,
				#ifdef ENABLE_BDJ_TITLE	
					ref_ptr<BDROM::VFSManager> pVFSManager,
				#else
					void* pVFSManager,
				#endif		
				#ifdef REALTEK_MODIFY		
					void* pSettings = 0);
				#else
					ref_ptr<BDROM_Settings> pSettings = 0);
				#endif
				
	~BDROM_Navigator();

	ref_ptr<ModuleManager>    GetModuleManager()		{ return m_pModuleManager; }
	ref_ptr<UserEventManager> GetUserEventManager()		{ return m_pUserEventManager; }
	ref_ptr<HDMV_Module>      GetHDMVModule(void)		{ return m_pModuleManager->GetHDMVModule(); }
#ifdef ENABLE_BDJ_TITLE		
	ref_ptr<BDJ_Module>       GetBDJModule(void)		{ return m_pModuleManager->GetBDJModule(); }
#endif
	void SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager);

	// Setup optional callback to occur when an HDMV Navi Command is executed
	void SetNaviCommandCallback(NaviCommandCallback* callback) { GetHDMVModule()->SetNaviCommandCallback(callback); }

#ifndef REALTEK_MODIFY_DISABLE_PLAYBACKINFO_CALLBACK
	// Setup optional callback to occur when BDROM Playback information is available
	void SetPlaybackInfoCallback(BDROM_PlaybackInfo::PlaybackInfoCallback* callback) { BDROM_PlaybackInfo::SetPlaybackInfoCallback(callback) }
#endif
private:
	ref_ptr<UserEventManager>				m_pUserEventManager;
	ref_ptr<ModuleManager>					m_pModuleManager;
	ref_ptr<BDROM_Module>					m_pHDMV_Module;
	ref_ptr<BDROM_Module>					m_pBDJ_Module;
	ref_ptr<BDROM_SoundPlayer>				m_pSoundPlayer;
	ref_ptr<CanvasManager>					m_pCanvasManager;
	ref_ptr<PlaybackControlEngine>			m_pPlaybackEngine;
	ref_ptr<BaseRenderer>					m_pRenderer;
	ref_ptr<BDROM::BDROM_NavigationCache>	m_pNavCache;
	ref_ptr<NotificationEventManager>		m_pNEManager;


	InteractiveGraphicsDecoder *			m_pIGDecoder;
	PresentationGraphicsDecoder *			m_pPGDecoder;
	ref_ptr<BD_FS_Layer>					m_pLayer;
	BD_FS_DeviceHandle						m_BDROMDeviceHandle;

public:
	// special handling for Device Lost
	//
	// DeviceLost: pause nav, flush decoders (but pause Async paths), and store current elapsed time info
	SonicError DeviceLost(void);
	// DeviceNew: Timesearch to device lost time, and begin playback
	SonicError DeviceNew(void);


	SonicError GetUserEventResponseTable(BDROM_UserEventResponseTable & userEventResponseTable);

	// Title Control User Operations
	SonicError MenuCall(void);
	SonicError TitleSearch(UInt32 uiTitleNum);
	SonicError Resume(void);
	SonicError Play(void);
	SonicError Stop(bool bResumable = false, bool bForceStop =false);
    SonicError SetRate(double dRate);
    double     GetRate(void);
	
	// Playback Control User Operations
	SonicError ChapterSearch(UInt32 uiChapterNum);
	SonicError TimeSearch(UInt32 pts45Khz);
	SonicError ForwardSkip(void);
	SonicError BackwardSkip(bool bOverride = false, bool bSkipOverChapterMark = false);
	SonicError PauseOn(void);
	SonicError PauseOff(void);
	SonicError StillOff(void);
	SonicError ForwardPlay(double dSpeed);
	SonicError BackwardPlay(double dSpeed);
	SonicError AudioChange(UInt8 audioStreamNum);
	SonicError PGTextSTChange(UInt8 PGTextSTStreamNum);
	SonicError PGTextSTOnOff(bool bDisplay);
	SonicError AngleChange(UInt8 AngleNum);

    SonicError EnableSecondaryVideo(bool bOn);
    SonicError SelectSecondaryVideo(UInt8 secondaryVideoNumber);
    SonicError EnableSecondaryAudio(bool bOn);
    SonicError SelectSecondaryAudio(UInt8 secondaryAudioNumber);
    SonicError SelectPiPPGTextST(UInt8 pipPgText);

	// Player Settting User Operations
	SonicError SelectMenuLanguage(UInt8 Language);

	// TextSubtitle
	SonicError SetSubtitleStyle(UInt32 uiStyleId);
	SonicError GetNumberOfSubtitleStyles(UInt32 &numberOfSTStyles);

	// Interactive User Operations
	SonicError MoveUpSelectedButton(void);
	SonicError MoveDownSelectedButton(void);
	SonicError MoveLeftSelectedButton(void);
	SonicError MoveRightSelectedButton(void);
	SonicError SelectButton(UInt32 uiButtonNumber);
	SonicError ActivateButton(void);
	SonicError SelectButtonAndActivate(UInt32 uiButtonNum);
	SonicError IsPopUpAvailable(bool & bAvailable)								{ return m_pModuleManager->IsPopUpAvailable(bAvailable); }
	SonicError PopUpOn(void);
	SonicError PopUpOff(void);
	bool       IsPopUpOn(void)													{ return m_pModuleManager->IsPopUpOn(); }
	SonicError MousePosition(SInt32 x, SInt32 y, bool & bOnClickableButton);
	SonicError MouseClick(SInt32 x, SInt32 y);

	SonicError KeyEvent(BDROM_KeyEventCode keyEvent, BDROM_KeyState keyState);

	// replace this with flexible notifications
	SonicError ProcessStreamDone(){return m_pModuleManager->ProcessStreamDone();};

	// get information
	BDROM_PlayerState GetPlayerState(void)										{ return m_pModuleManager->GetPlayerState(); }
	bool       IsPaused(void) 													{ return (GetPlayerState() == kBDROM_PlayerState__Pause); }
	bool       IsStill(void) 													{ return (GetPlayerState() == kBDROM_PlayerState__Still); }
	bool       IsScanning(void) 												{ return (GetPlayerState() == kBDROM_PlayerState__Scanning); }

	SonicError GetDiscId(BDROMDiscId & discId)									{ return m_pModuleManager->GetDiscId(discId); }
	SonicError GetNumberOfTitles(UInt32 & uiNumTitles)							{ return m_pModuleManager->GetNumberOfTitles(uiNumTitles); }
	SonicError GetNumberOfTitles(UInt32 & uiNumTitles,
								 bool & bHasFirstPlayback, bool & bHasTopMenu)	{ return m_pModuleManager->GetNumberOfTitles(uiNumTitles, bHasFirstPlayback, bHasTopMenu); }
	SonicError GetTitleType(UInt32 uiTitleNum,
							BDROM_ObjectType & objType,
							BDROM_TitleType & titleType)						{ return m_pModuleManager->GetTitleType(uiTitleNum, objType, titleType); }
	SonicError GetTitleAccessType(UInt32 uiTitleNum,
								  BDROM_TitleAccessType & accessType)			{ return m_pModuleManager->GetTitleAccessType(uiTitleNum, accessType); }
	bool       IsInteractiveTitle(UInt32 uiTitleNum)							{ return m_pModuleManager->IsInteractiveTitle(uiTitleNum); }
	bool	   IsTitleAccessAllowed(UInt32 uiTitleNum)							{ return m_pModuleManager->IsTitleAccessAllowed(uiTitleNum); }
	SonicError IsResumePossible(bool & bPossible)								{ return m_pModuleManager->IsResumePossible(bPossible); }

	SonicError GetCurrentTitleType(BDROM_TitleType & titleType)					{ return m_pModuleManager->GetCurrentTitleType(titleType); }
	SonicError GetNumberOfChapters(UInt32 & uiNumChapters)						{ return m_pPlaybackEngine->GetNumberOfChapters(uiNumChapters); }
	SonicError GetCurrentElapsedTime(DVDTimeStampType &tsPlaybackElapsedTime)	{ return m_pPlaybackEngine->GetMediaTime(tsPlaybackElapsedTime); }
	SonicError GetCurrentTitleDuration(DVDTimeStampType & titleDuration)		{ return m_pModuleManager->GetCurrentTitleDuration(titleDuration); }
	SonicError GetCurrentAngle(UInt32 & uiAnglesAvailable, UInt32 & uiCurrentAngle);
	SonicError GetCurrentTitleNum(UInt16 & uiTitleNum)							{ return m_pModuleManager->GetCurrentTitleNum(uiTitleNum); }
	SonicError GetCurrentChapterNum(UInt32 & uiChapterNum)						{ return m_pPlaybackEngine->GetCurrentChapterNum(uiChapterNum); }
	SonicError GetCurrentApplicationType(BDROM_ApplicationType & applType)		{ return m_pPlaybackEngine->GetCurrentApplicationType(applType); }
	SonicError GetChapterStartTime(UInt32 uiChapterNum, DVDTimeStampType & startTime);
	SonicError GetChapterDuration(UInt32 uiChapterNum, DVDTimeStampType & duration);
	SonicError GetCurrentPlaylistDuration(DVDTimeStampType & duration);
	SonicError GetCurrentPlayitemDuration(DVDTimeStampType & duration);
	SonicError GetCurrentTextSubTitleStyle(UInt8 & uiStylesAvailable, UInt8 & uiCurrentStyle);
	bool       IsPiPSubtitleMode()                                                  { return m_pPlaybackEngine->IsPiPSubtitleMode();}
	bool       IsPiPDisplayed()                                                     { return m_pPlaybackEngine->IsPiPDisplayed();}

	// get any stream's properties
	SonicError GetCurrentStream(BDROM_StreamType streamType,
							UInt32 & uiStreamsAvailable,
							UInt32 & uiCurrentStream)					{ return m_pPlaybackEngine->GetCurrentStream(streamType, uiStreamsAvailable, uiCurrentStream); }
	SonicError GetCurrentStreamAttributes(BDROM_StreamType streamType,
							BDROM_Stream & streamAttr)					{ return m_pPlaybackEngine->GetCurrentStreamAttributes(streamType, streamAttr); }
	SonicError GetStreamAttributes(BDROM_StreamType streamType,
							UInt32 uiStream,
							BDROM_Stream & streamAttr)					{ return m_pPlaybackEngine->GetStreamAttributes(streamType, uiStream, streamAttr); }

	// Set Preferences
	void SetRegion(UInt32 uiRgnCode);
	void SetCountry(UInt32 uiCountryCode);
	void SetPreferredAudioLanguage(const char * pLangCode);
	void SetPreferredMenuLanguage(const char * pLangCode);
	void SetPreferredPGTextSTLanguage(const char * pLangCode);
	void SetAudioCapability(UInt32 uiLPCM192, UInt32 uiLPCM48and96,
							UInt32 uiDDpIndep, UInt32 uiDDpDep,
							UInt32 uiDTSHDCore, UInt32 uiDTSHDExtn,
							UInt32 uiDolbyDigital, UInt32 uiDolbyMLP);
	void SetAudioCapability(UInt32 uiAudioCaps);
	void SetParentalLevel(UInt8 uiParentalLevel);
	void SetPlayerProfile(BDROM_PlayerProfile uiPlayerProfile);
	void SetParentalLevelChangeByBDJxletAllowed(bool bAllowed)		{ m_pPlaybackEngine->SetParentalLevelChangeByBDJxletAllowed(bAllowed); }
	void SetCountryCodeChangeByBDJxletAllowed(bool bAllowed)		{ m_pPlaybackEngine->SetCountryCodeChangeByBDJxletAllowed(bAllowed); }
	void SetLanguageCodeChangeByBDJxletAllowed(bool bAllowed)		{ m_pPlaybackEngine->SetLanguageCodeChangeByBDJxletAllowed(bAllowed); }
	void AuthorizeParentalLevelChange(bool bGranted)				{ m_pPlaybackEngine->AuthorizeParentalLevelChange(bGranted); }

	// Get Preferences
	void GetRegion(UInt32 & uiRgnCode);
	void GetCountry(UInt32 & uiCountryCode);
	void GetPreferredAudioLanguage(char * pLangCode);
	void GetPreferredMenuLanguage(char * pLangCode);
	void GetPreferredPGTextSTLanguage(char * pLangCode);
	void GetAudioCapability(UInt32 & uiAudioCaps);
	void GetParentalLevel(UInt8 & uiParentalLevel);
	void GetPlayerProfile(BDROM_PlayerProfile & uiPlayerProfile);
	void GetPlayerVersion(UInt16 & uiPlayerVersion);

	UInt32 GetPSR(UInt32 psrNum)									{ UInt32 v = 0xCCCCCCCC; m_pPlaybackEngine->GetPSR((UInt8)psrNum, v); return v; }
	UInt32 GetGPR(UInt32 gprNum)									{ UInt32 v = 0xCCCCCCCC; m_pPlaybackEngine->GetGPR(gprNum, v); return v; }

	static void GetVideoSize(BDROM_VideoAttributes&, UInt32& uiHeight, UInt32& uiWidth);
    void SetElapsedTimeStatus(UInt32 uiTitleNum);
	
	// DiscEject: Terminates playback but leaves BDJ Disc-unbound Apps running.
	SonicError DiscEject(void);

	// DiscInsert: Loads new disc (only when new BDJ Disc-unbound Apps are running).
	SonicError DiscInsert(void);


	//---------------------------------------------------------------------------------
	// API for breaking and continuing playback - For Emulator use only

	// Break: Pauses playback.
	SonicError Break(void);

	// Continue: Resumes playback
	SonicError Continue(void);


#ifdef REALTEK_MODIFY_RESUMEPLAY
	SonicError ResumePlay(int titleNum, int chapterNum, int time, int plNum = -1, int audioStreamNum =-1, int spuStreamNum =-1)
	{
		return m_pModuleManager->ResumePlay(titleNum, chapterNum, time, plNum, audioStreamNum, spuStreamNum);
	}
#endif


protected:
	void SendUserOperationMaskedEvent(UInt32 uiIndex);

private:
};


#endif

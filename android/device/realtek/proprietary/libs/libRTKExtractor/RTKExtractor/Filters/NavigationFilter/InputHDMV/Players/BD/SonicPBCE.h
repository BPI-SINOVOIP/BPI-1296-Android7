//-----------------------------------------------------------------------------
// SonicPBCE.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SONICPBCE_H__
#define __SONICPBCE_H__

#include "CanvasManager.h"
#include "PlaybackControlEngine.h"
#include "SonicPBCE_CommonTypes.h"
#include "BaseRenderer.h"
#include "SonicPBCE_Registers.h"
#include "SonicPBCE_PlayListManager.h"

class BDROM_SonicPBCE_Clip;
class BDROM_StreamControl;
class BDROM_TimerManager;
class TextSubtitleDecoder;

// SonicPlaybackControlEngine
// Sonic's BDROM PlaybackControlEngine implementation
//
class SonicPlaybackControlEngine : public PlaybackControlEngine, public NotificationEventSink
{
public:
	SonicPlaybackControlEngine(	ref_ptr<BaseRenderer> pRenderer,
								ref_ptr<BDROM_SonicPBCE_Registers> pBDROMRegisters,
								ref_ptr<BDROM_StreamControl> pStreamControl,
								ref_ptr<BDROM_TimerManager> pTimerManager,
								TextSubtitleDecoder* pTextSTDecoder,
								BDROM_DiscType discType,
							#ifdef REALTEK_MODIFY
								void*					pSettings,
							#else	
								ref_ptr<BDROM_Settings> pSettings,
							#endif	
								ref_ptr<CanvasManager> pCanvasManager);
	virtual ~SonicPlaybackControlEngine();

public:
	//-------------------------------------------------------------------------------------------------------
	// PlaybackControlEngine Interface implementation/overrides
	//

	virtual SonicError SetBDROMDirectory(ref_ptr<BDROM::BDROM_NavigationCache> pNavCache);
	virtual void       SetNotificationEventManager(ref_ptr<NotificationEventManager> pNEManager);
	virtual void       SetTitleType(BDROM_ObjectType objType, BDROM_TitleType titleType)			{ m_pPlayListManager->SetTitleType(objType, titleType); } 

	ref_ptr<BDROM_SonicPBCE_Registers> GetRegisters(void);

	// special handling for Device Lost
	//
	// DeviceLost: pause nav, flush decoders (but pause Async paths), and store current elapsed time info
	SonicError DeviceLost(void)																		{ return m_pPlayListManager->DeviceLost(); }
	// DeviceNew: Timesearch to device lost time, and begin playback
	SonicError DeviceNew(void)																		{ return m_pPlayListManager->DeviceNew(); }

	virtual SonicError EnableInteractiveGraphics(bool bEnable);

	virtual SonicError ForwardPlay(UInt32 uiSpeed);
	virtual SonicError BackwardPlay (UInt32 uiSpeed);
	virtual SonicError PauseOn(void);
	virtual bool       IsPaused(void)									{ return m_pPlayListManager->IsPaused(); }
	virtual bool       IsStillOn(void)									{ return m_pPlayListManager->IsStillOn(); }
	virtual SonicError PauseOff(void); 
	virtual SonicError StillOn();
	virtual SonicError StillOff();
	virtual bool	   IsScanning()										{ return m_pPlayListManager->IsScanning(); }
	virtual SonicError Step(SInt32 siNumFrames);
	virtual SonicError Stop(void)										{ return m_pPlayListManager->Stop(); }

	// Sends a MediaEvent to HDR
	// - 'type' param is actually a EMediaEventType enum from BaseRenderer.h.  The reason why BaseRenderer.h is not included
	//   is because it causes D3D header to be included...
	virtual SonicError MediaEvent(UInt32 type, const UInt32 dataSize=0, void * pData=0)	{ return m_pPlayListManager->MediaEvent((EMediaEventType)type, dataSize, pData); }

	// elapsed time
	virtual void SetElapsedTime(Time90K ts45kElapsedTime);

	virtual SonicError PlayPL(UInt32 uiPlayListId, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	virtual SonicError PlayPLatPI(UInt32 uiPlayListId, UInt32 uiPlayItemId, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	virtual SonicError LinkPI(UInt32 uiPlayItemId);
	virtual SonicError PlayPLatMK(UInt32 uiPlayListId, UInt32 uiPlayListMarkId, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED); 
	virtual SonicError LinkMK(UInt32 uiPlayListMarkId); 
	virtual SonicError PlayPLatTime(UInt32 uiPlayListId, UInt32 ts45k, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	virtual SonicError TimeSearch(UInt32 ts45k, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	virtual SonicError ForwardSkip(UInt8 uiMarkType, UInt32 &uiChapterNumber);
	virtual SonicError BackwardSkip(UInt8 uiMarkType, UInt32 &uiChapterNumber, bool bOverride = false, bool bSkipOverMark = false);
	virtual SonicError Replay(void);
	virtual SonicError TerminatePlaylist(bool bDiscardResumeInfo = true);
	virtual SonicError SuspendPlayList(void);
	virtual SonicError ResumePlayList(void);
	virtual void       DiscardResumeInfo(void)							{ m_pPlayListManager->DiscardResumeInfo(); }
	virtual SonicError SelectPrimaryAudio(UInt8 uiStreamNum); 
	virtual SonicError SelectPGTextST(UInt32 uiStreamNum, bool bChangeOnly = false);
	virtual SonicError PGTextSTOnOff(bool bDisplay);
	virtual SonicError SelectAngle(UInt8 uiAngleNumber); 
    virtual SonicError EnableSecondaryVideo(bool bOn);
    virtual SonicError SelectSecondaryVideo(UInt8 secondaryVideoNumber);
    virtual SonicError EnableSecondaryAudio(bool bOn);
    virtual SonicError SelectSecondaryAudio(UInt8 secondaryAudioNumber);
    virtual SonicError SelectPiPPGTextST(UInt8 pipPgText);
	virtual SonicError ChapterSearch(UInt32 uiChapterNum);

	// registers
	virtual SonicError GetGPR(UInt32 uiGPRNum, UInt32 &uiGPRValue);
	virtual SonicError SetGPR(UInt32 uiGPRNum, UInt32 uiGPRValue);
	virtual SonicError GetPSR(UInt8 uiPSRNum, UInt32 &uiPSRValue);
	virtual SonicError SetPSR(UInt8 uiPSRNum, UInt32 uiPSRValue, bool bSetByBDJxlet = false);
	virtual SonicError ClearBackupRegisters();
	virtual void SetPSRForPrimaryAudioStream(UInt32 uiStreamNum);
	virtual void SetPSRForPGTextSTStream(bool bSetStrm, UInt32 uiStreamNum, UInt8 disp_s_flag);
	virtual void SetPSRForIGStream(UInt32 uiStreamNum);
	virtual void SetPSRForAngle(UInt32 uiAngleNum);
	virtual void SetPSRForSecondaryVideoStream(bool bSetStrmNum,UInt32 uiStreamNum,bool bSetVideoSize,UInt32 uiVideoSize,UInt8 disp_v_flag);
	virtual void SetPSRForSecondaryAudioStream(bool bSetStrm, UInt32 uiStreamNum, UInt8 disp_a_flag);
	virtual void SetPSRForPiPPGTextSTStream(bool bSetStrm, UInt32 uiStreamNum, UInt8 disp_s_flag, UInt8 PIP_PG_textST_valid_flag);

	// get info
	virtual SonicError GetPlayListPlaybackType(UInt32 uiPlayListId, BDROM_PlayList_PlaybackType & playlistPlaybackType)
						{
							return m_pPlayListManager->GetPlayListPlaybackType(uiPlayListId, playlistPlaybackType);
						} 
	virtual SonicError GetNumberOfPlayItems(UInt32 uiPlayListID, UInt32& uiNumPlayItems);
	virtual SonicError GetPlayListUOPsMask(UInt64 &UOPMask);
	virtual SonicError GetPlayListDuration(UInt32 uiPlayListId,  TimeValue & tsPlaybackDuration45k);
	virtual SonicError GetPlayItemDuration(UInt32 uiPlayListNum, UInt32 & ts45k);
 	virtual SonicError GetNumMarks(UInt32 uiPlayListNum, BDROM_MarkType markType, UInt32& uiNumMarks);
	virtual SonicError GetMarkInfo(UInt32 uiPlayListNum, UInt32 uiMarkNum, BDROM_MarkType markType, BDROM_PL_MarkInfo& markInfo);
	virtual SonicError GetCurrentLocation(BDROM_Location& currLocation);
	virtual SonicError GetCurrentPlayList(UInt32& uiCurrPlayList);
	virtual SonicError GetCurrentPlayItem(UInt32& uiCurrPlayItem);
	virtual SonicError GetCurrentMark(UInt32& uiCurrMarkNum);
	virtual SonicError GetCurrentAngle(UInt32 & uiAnglesAvailable, UInt32 & uiCurrentAngle);
    virtual SonicError GetCurrentChapter(UInt32 & uiChapterNum);
	virtual	SonicError GetNumberOfChapters(UInt32 & uiNumChapters);
	virtual SonicError GetCurrentChapterNum(UInt32 & uiChapterNum)						{ return m_pPlayListManager->GetCurrentChapter(uiChapterNum); }
	virtual SonicError GetCurrentApplicationType(BDROM_ApplicationType & applType)		{ return m_pPlayListManager->GetCurrentApplicationType(applType); }
	virtual SonicError GetChapterStartTime(UInt32 uiChapterNum,
										   TimeValue & ts45kStartTime)					{ return m_pPlayListManager->GetChapterStartTime(uiChapterNum, ts45kStartTime); }
	virtual SonicError GetChapterDuration(UInt32 uiChapterNum,
										  TimeValue & ts45kDuration)					{ return m_pPlayListManager->GetChapterDuration(uiChapterNum, ts45kDuration); }

	// preferences
	void SetPreferredAudioLanguage(const char * pLangCode);
	void SetPreferredMenuLanguage(const char * pLangCode);
	void SetPreferredPGTextSTLanguage(const char * pLangCode);
	void SetAudioCapability(UInt32 uiLPCM192, UInt32 uiLPCM48and96, UInt32 uiDDpIndep, UInt32 uiDDpDep,
							UInt32 uiDTSHDCore, UInt32 uiDTSHDExtn, UInt32 uiDolbyDigital, UInt32 uiDolbyMLP);
	void SetParentalLevel(UInt8 uiParentalLevel);
	void SetPlayerProfile(UInt8 uiPlayerProfile);
	void SetParentalLevelChangeByBDJxletAllowed(bool bAllowed)		{ m_pPlayListManager->SetParentalLevelChangeByBDJxletAllowed(bAllowed); }
	void SetCountryCodeChangeByBDJxletAllowed(bool bAllowed)		{ m_pPlayListManager->SetCountryCodeChangeByBDJxletAllowed(bAllowed); }
	void SetLanguageCodeChangeByBDJxletAllowed(bool bAllowed)		{ m_pPlayListManager->SetLanguageCodeChangeByBDJxletAllowed(bAllowed); }
	void AuthorizeParentalLevelChange(bool bGranted)				{ m_pPlayListManager->AuthorizeParentalLevelChange(bGranted); }

	// disc id (and organization id)
	virtual SonicError GetDiscId(BDROMDiscId & discId);

	// functions to get any stream's properties
	virtual SonicError GetCurrentStream(BDROM_StreamType streamType, UInt32 & uiStreamsAvailable, UInt32 & uiCurrentStream);
	virtual SonicError GetCurrentStreamAttributes(BDROM_StreamType streamType, BDROM_Stream & streamAttr);
	virtual SonicError GetStreamAttributes(BDROM_StreamType streamType, UInt32 uiStream, BDROM_Stream & streamAttr);

	// playlist information
	virtual SonicError GetCurrentPlayListDuration(UInt32 & playlistDuration45k);

 	// Subtitle Control
	virtual SonicError SetSubtitleMode(bool bSubtitleOn);
 	virtual SonicError SetSubtitleStyle(UInt32 subtitleStyle);
	virtual SonicError GetNumberOfSubtitleStyles(UInt32 &numberOfSTStyles);

	// PiP control
	virtual bool       IsPiPSubtitleMode(void);
	virtual SonicError SetPiPSubtitleMode(bool bSubtitleOn);
	virtual bool       IsPiPDisplayed(void);
	virtual bool	   IsPiPEnabled(void);
	virtual bool       IsPiPAvailable(void);
	virtual UInt32	   GetNumberOfPiPStreams(void);
	virtual bool       IsPiPSynchedDuringTrickPlay(void);
	virtual void       EnablePiPMetadataProcessing(bool /*bEnable*/);
	virtual bool       IsPiPMetadataProcessingEnabled(void)	;
	virtual SonicError StartAsyncPiP(void);
	virtual SonicError StopAsyncPiP(void);
	virtual SonicError GetAsyncPiPMediaTime(UInt32 & /*ts45kPlaybackElapsedTime*/);
	virtual bool       PlaylistHasPiP(UInt32 playlistId)			{ return m_pPlayListManager->PlaylistHasPiP(playlistId); }

	virtual SonicError GetCurrentAudio(UInt32& uiStreamsAvailable, UInt32& uiCurrentStream);
	virtual SonicError GetSecondaryAudio(UInt32& uiStreamsAvailable, UInt32& uiCurrentStream);
	virtual SonicError GetCurrentAudioAttributes(BDROM_AudioAttributes & aatr);
	virtual SonicError GetAudioAttributes(UInt32 uiStream, BDROM_AudioAttributes & aatr);
	virtual SonicError GetCurrentAngle(UInt8 &uiNumberOfAngles, UInt8 &uiCurrentAngle);
	virtual SonicError GetCurrentVideoAttributes(BDROM_VideoAttributes & vatr);
	virtual SonicError GetCurrentStreamPIDs(UInt32& uiVideoPid, UInt32& uiPrimaryAudioPid, UInt32& uiSecondaryAudioPid, UInt32& uiPGPid, UInt32& uiIGPid, UInt32& uiTextPID);
	virtual SonicError GetCurrentPGStream(UInt16 &uiNumberOfPGStreams, UInt16 &uiCurrentStream);
	virtual SonicError GetCurrentIGStream(UInt8 &uiNumberOfIGStreams, UInt8 &uiCurrentStream);
	virtual SonicError GetPGStreamAttributes(UInt8 uiStreamNumber, BDROM_GraphicsAttributes &bdattr);
	virtual SonicError GetIGStreamAttributes(UInt8 uiStreamNumber, BDROM_GraphicsAttributes &bdattr);
	
	virtual SonicError SetPGPacketCallback(PGPacketCallback pCallback);
	virtual SonicError SetIGPacketCallback(IGPacketCallback pCallback);
	virtual SonicError PlaySound(UInt32 sound_id);
	virtual SonicError SetNVTimer(bool bMobjIDIsImmediate, UInt16 MobjID_or_GPR,  bool bTimerValueIsImmediate, UInt16 TimerValue_or_GPR);
	virtual SonicError IsVideoFullScreen(bool & fullScreen);
	virtual SonicError GetActiveVideoArea(UInt32 &left, UInt32 &top, UInt32 &right, UInt32 &bottom);
	virtual SonicError GetActiveVideoAreaOnScreen(UInt32 &left, UInt32 &top, UInt32 &right, UInt32 &bottom) ;
	virtual SonicError GetClipRegion(UInt32 &left, UInt32 &top, UInt32 &right, UInt32 &bottom) ;
	virtual SonicError GetHorizontalScalingFactors(float* pScaleFactorArray, size_t arraySize) ;
	virtual SonicError GetInputVideoSize(UInt32 & uiHeight, UInt32 & uiWidth);

	virtual SonicError GetPositioningCapability(POS_CAP& pc) ;
	virtual SonicError GetTotalVideoArea(UInt32 &left, UInt32 &top, UInt32 &right, UInt32 &bottom);
	virtual SonicError GetTotalVideoAreaOnScreen(UInt32 &left, UInt32 &top, UInt32 &right, UInt32 &bottom) ;
	virtual SonicError GetVerticalScalingFactors(float* pScaleFactorArray, UInt32 arraySize) ;
	virtual SonicError GetVideoSize(UInt32 &uiHeight, UInt32 &uiWidth) ;
	virtual SonicError SetClipRegion(UInt32 left, UInt32 top, UInt32 Right, UInt32 bottom) ;
	virtual SonicError SupportsArbitraryHorizontalScaling(bool &bSupported,  float &flMinScale, float &flMaxScale) ;
	virtual SonicError SupportsArbitraryVerticalScaling(bool &bSupported, float &lMinScale, float &flMaxScale) ;
	virtual SonicError SupportsClipping(bool &bSupported);
	virtual SonicError IsPanAndScan(bool& bIsPanScan) ;
	virtual SonicError CanScaleSourceToDestination(UInt32 sXIn, UInt32 sYIn, UInt32 sWIn, UInt32 sHIn, 
												   UInt32 dXIn, UInt32 dYIn, UInt32 dWIn, UInt32 dHIn,
												   UInt32 &sXOut,UInt32 &sYOut,UInt32 &sWOut,UInt32 &sHOut, 
												   UInt32 &dXOut,UInt32 &dYOut,UInt32 &dWOut,UInt32 &dHOut);
	virtual SonicError ScaleSourceToDestination(UInt32 sX, UInt32 sY, UInt32 sW, UInt32 sH, UInt32 dX, UInt32 dY, UInt32 dW, UInt32 dH);
	virtual SonicError SetScalingFactors(float horizontalScalingFactor, float verticalScalingFactor) ;
	virtual SonicError GetScalingFactors(float &horizontalScalingFactor, float &verticalScalingFactor);
	virtual SonicError GetVideoPosition(UInt32 &x, UInt32 &y) ;
	virtual SonicError SetVideoPosition(UInt32 x, UInt32 y) ;
	virtual SonicError SendEvent(BDROM_Event event);
	virtual SonicError ConfigureKeepResolution(bool bKeepResolution, UInt16 uiBaseResWidth, UInt16 uiBaseResHeight, bool bUseScreenRect, 
												float screenRectX, float screenRectY, float screenRectWidth, float screenRectHeight);
	virtual SonicError GetKeepResolution(bool &bKeepResolution);
	virtual SonicError SetFullScreenMode(bool bMode);
	virtual SonicError ResetVideoSettings();

	// org.bluray.media
	virtual SonicError GetCurrentLanguage(BDROM_StreamType streamType, UInt32& uiLanguage);
	virtual SonicError ListAvailableLanguages(BDROM_StreamType streamType, UInt32 *uiNumOfLanguages, UInt32 *uiLanguageArray);	
	virtual	SonicError SelectLanguage(BDROM_StreamType streamType, UInt32 uiLanguage);
	virtual SonicError SelectDefaultLanguage(BDROM_StreamType streamType, UInt32 &uiLanguage);

	// org.bluray.ti
	virtual SonicError GetPlayListInfo(UInt32 uiPlayListId, BDROM_PL_PLInfo &playListInfo);

	// com.sonic.javax.media
	virtual SonicError GetRate(BDROM_PlayDirection &direction, UInt32 &uiSpeed);
	virtual SonicError GetDuration(UInt32 &tsPlaybackDuration45k);
	virtual SonicError GetMediaTime(UInt32 &tsPlaybackElapsedTime45k);
	virtual SonicError SetStopTime(UInt32 tsStopTime45k);
	virtual SonicError GetStopTime(UInt32 &tsStopTime45k);

	virtual SonicError GetAACSVolumeID(ref_ptr_array<UInt8> &ID); // returns 16 bytes
	virtual SonicError GetAACSMediaSerialNum(ref_ptr_array<UInt8> &PMSN); // returns 16 bytes
	virtual SonicError GetAACSDeviceBindingID(ref_ptr_array<UInt8> &devID); // returns 16 bytes
	virtual SonicError GetAACSContentCertID(ref_ptr_array<UInt8> &certID); // returns 6 bytes
	virtual SonicError GetAACSNonce(UInt32 titleID, ref_ptr_array<UInt8> &nonce); // returns 16 bytes
	virtual SonicError SetAACSPermission(UInt32 titleID, ref_ptr_array<UInt8> perm);
	virtual bool CheckAACSPermission(UInt32 titleID);
	virtual bool IsAACSPermissionCacheable();

	
	// OBSOLETE VERSIONS that use DVDTimeStampType -- Deprecated
	virtual SonicError GetDuration(DVDTimeStampType &tsPlaybackDuration);
	virtual SonicError GetMediaTime(DVDTimeStampType &tsPlaybackElapsedTime);
	virtual SonicError SetStopTime(DVDTimeStampType tsStopTime);
	virtual SonicError GetStopTime(DVDTimeStampType &tsStopTime);
	virtual SonicError PlayPLatTime(UInt32 ulPlayListNum, DVDTimeStampType ts, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	virtual SonicError TimeSearch(DVDTimeStampType ts, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED);
	virtual SonicError GetPlayListDuration(UInt32 uiPlayListNum,  DVDTimeStampType & ts);
	virtual SonicError GetPlayItemDuration(UInt32 uiPlayListNum, DVDTimeStampType & ts);
	virtual SonicError GetCurrentPlayListDuration(DVDTimeStampType & /*playlistDuration*/);

	//---------------------------------------------------------------------------------
	// API for breaking and continuing playback - For Emulator use only

	// Break: Pauses playback.
	virtual SonicError Break(void)							{ return m_pPlayListManager->Break(); }

	// Continue: Resumes playback
	virtual SonicError Continue(void)						{ return m_pPlayListManager->Continue(); }

	//-------------------------------------------------------------------------------------------------------
	// NotificationEventSink interface implementation
	//
public:
	virtual SonicError NotifyEvent(BDROM_Event event);

public: // -------------- Dell Demo DS Filter requires this ----------------
	void GetCurrentStreamPIDs(UInt32 & uiAudioPid, UInt32 & uiVideoPid);

private:
	//-------------------------------------------------------------------------------------------------------
	// Private member functions
	//
//	bool LoadMainPathClipInfoFile(UInt32 uiPlayItemId, UInt32 uiAngleId);
//	bool LoadSubPathClipInfoFile(UInt32 uiSubPathId, UInt32 uiSubPlayItemId, UInt32 uiSubClipId);

private:
	//-------------------------------------------------------------------------------------------------------
	// Data members
	//

	ref_ptr<BDROM::BDROM_NavigationCache>			m_pNavCache;
	ref_ptr<NotificationEventManager>				m_pNEManager;
	ref_ptr<BaseRenderer>							m_pRenderer;
	ref_ptr<BDROM_PBCE_PlayListManager>				m_pPlayListManager;
	TextSubtitleDecoder*							m_pTextSTDecoder;
	ref_ptr<BDROM_StreamControl>					m_pStreamControl;
#ifdef REALTEK_MODIFY
	void*											m_pClipRect;
#else	
	ref_ptr<BaseClipRect>							m_pClipRect;
#endif	
	ref_ptr<CanvasManager>							m_pCanvasManager;
	bool											m_KeepResolution;
};

#endif

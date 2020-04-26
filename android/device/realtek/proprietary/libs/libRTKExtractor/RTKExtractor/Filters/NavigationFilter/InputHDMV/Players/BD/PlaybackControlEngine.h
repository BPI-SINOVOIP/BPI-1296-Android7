//-----------------------------------------------------------------------------
// PlaybackControlEngine.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __PLAYBACKCONTROLENGINE_H__
#define __PLAYBACKCONTROLENGINE_H__

#include "DVDnUtilities.h" // for DVDTimeStampType
#include "CommonTypes.h"	// for virtual SonicError;
#include "BDROM_NavigatorTypes.h"
#include "NotificationEventManager.h"
#include "BDROMNavigationCache.h"
#include "TimeValue.h"

typedef enum tagPOS_CAP
{
	POS_CAP_FULL,
	POS_CAP_FULL_IF_ENTIRE_VIDEO_ON_SCREEN,
	POS_CAP_FULL_EVEN_LINES,
	POS_CAP_FULL_EVEN_LINES_IF_ENTIRE_VIDEO_ON_SCREEN,
	POS_CAP_OTHER
}POS_CAP;

// callbacks
typedef SInt32 (*PGPacketCallback)(void* pData, size_t size);
typedef SInt32 (*IGPacketCallback)(void* pData, size_t size);

// default implementation choices (mutually exclusive) for the Playback Control Engine interface
//#define PLAYBACKCTRLENGINE_IFACE_IMPL				{ return 0; }
#define PLAYBACKCTRLENGINE_IFACE_IMPL				= 0;
//#define PLAYBACKCTRLENGINE_IFACE_IMPL				{ assert(false); }

class PiSize;

class PlaybackControlEngine
{
public:
	PlaybackControlEngine()				{ }
	virtual ~PlaybackControlEngine()	{ }

public:
	// Init/Setup
	virtual SonicError SetBDROMDirectory(ref_ptr<BDROM::BDROM_NavigationCache> /*pNavCache*/)									PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual void       SetNotificationEventManager(ref_ptr<NotificationEventManager> /*pNEManager*/)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual void       SetTitleType(BDROM_ObjectType objType, BDROM_TitleType titleType)										{ }

	// special handling for Device Lost
	//
	// DeviceLost: pause nav, flush decoders (but pause Async paths), and store current elapsed time info
	virtual SonicError DeviceLost(void)																							{ return -1; }
	// DeviceNew: Timesearch to device lost time, and begin playback
	virtual SonicError DeviceNew(void)																							{ return -1; }

	// IG Decode control (to be set by Module Manager or current Module when switching modules). Can be called only when Nav is in stop state
	virtual SonicError EnableInteractiveGraphics(bool /*bEnable*/)																PLAYBACKCTRLENGINE_IFACE_IMPL

	virtual SonicError ForwardPlay(UInt32 /*uiSpeed*/)																			PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError BackwardPlay (UInt32 /*uiSpeed*/)																		PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError PauseOn(void)																							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError PauseOff(void)																							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError StillOn(void)																							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual bool       IsPaused(void)																							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual bool       IsStillOn(void)																							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual bool       IsScanning(void)																							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError StillOff(void)																							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError Step(SInt32 /*siNumFrames*/)																				PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError Stop(void)																								{ return 0; }

	// Sends a MediaEvent to HDR
	// - 'type' param is actually a EMediaEventType enum from BaseRenderer.h.  The reason why BaseRenderer.h is not included
	//   is because it causes D3D header to be included...
	virtual SonicError MediaEvent(UInt32 type, const UInt32 dataSize = 0, void * pData = 0)										{ return 0; }

	virtual SonicError PlayPL(UInt32 /*uiPlayListId*/, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError PlayPLatPI(UInt32 /*uiPlayListId*/, UInt32 /*uiPlayItemId*/, 
									double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED)												PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError LinkPI(UInt32 /*uiPlayItemId*/)																			PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError PlayPLatMK(UInt32 /*uiPlayListId*/, UInt32 /*uiPlayListMarkId*/, 
									double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED)												PLAYBACKCTRLENGINE_IFACE_IMPL 
	virtual SonicError LinkMK(UInt32 /*uiPlayListMarkId*/)																		PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError PlayPLatTime(UInt32 /*uiPlayListId*/, UInt32 /*ts45k*/, 
									double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED)												PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError TimeSearch(UInt32 /*ts45k*/, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED)								PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError ForwardSkip(UInt8 /*uiMarkType*/, UInt32 &/*uiChapterNumber*/)											PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError BackwardSkip(UInt8 /*uiMarkType*/, UInt32 &/*uiChapterNumber*/,
									bool bOverride = false, bool bSkipOverMark = false)											PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError Replay(void)																								PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError TerminatePlaylist(bool bDiscardResumeInfo = true)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SuspendPlayList(void)																					PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError ResumePlayList(void)																						PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual void       DiscardResumeInfo(void)																					{ }
	virtual SonicError SelectPrimaryAudio(UInt8 /*uiStreamNum*/)																PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SelectPGTextST(UInt32 /*uiStreamNum*/, bool bChangeOnly = false)											PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError PGTextSTOnOff(bool bDisplay)																				{ return -1; }
	virtual SonicError SelectAngle(UInt8 /*uiAngleNumber*/)																		PLAYBACKCTRLENGINE_IFACE_IMPL 
	virtual SonicError EnableSecondaryVideo(bool /*bOn*/)																		PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SelectSecondaryVideo(UInt8 /*secondaryVideoNumber*/)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError EnableSecondaryAudio(bool /*bOn*/)																		PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SelectSecondaryAudio(UInt8 /*secondaryAudioNumber*/)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SelectPiPPGTextST(UInt8 /*pipPgText*/)																	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError ChapterSearch(UInt32 /*uiChapterNum*/)																	PLAYBACKCTRLENGINE_IFACE_IMPL	
	virtual SonicError GetNumberOfChapters(UInt32 & /*uiNumChapters*/)															PLAYBACKCTRLENGINE_IFACE_IMPL

	// Registers
	virtual SonicError GetGPR(UInt32 /*uiGPRNum*/, UInt32 & /*uiGPRValue*/)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetGPR(UInt32 /*uiGPRNum*/, UInt32 /*uiGPRValue*/)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetPSR(UInt8 /*uiPSRNum*/, UInt32 & /*uiPSRValue*/)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetPSR(UInt8 /*uiPSRNum*/, UInt32 /*uiPSRValue*/, bool bSetByBDJxlet = false)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError ClearBackupRegisters()																					PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual void SetPSRForPrimaryAudioStream(UInt32 /*uiStreamNum*/)															{}
	virtual void SetPSRForPGTextSTStream(bool /*bSetStrm*/, UInt32 /*uiStreamNum*/, UInt8 /*disp_s_flag*/)						{}
	virtual void SetPSRForIGStream(UInt32 /*uiStreamNum*/)																		{}
	virtual void SetPSRForAngle(UInt32 /*uiAngleNum*/)																			{}
	virtual void SetPSRForSecondaryVideoStream(bool /*bSetStrmNum*/,UInt32 /*uiStreamNum*/,bool /*bSetVideoSize*/,UInt32 /*uiVideoSize*/,UInt8 /*disp_v_flag*/){}
	virtual void SetPSRForSecondaryAudioStream(bool /*bSetStrm*/, UInt32 /*uiStreamNum*/, UInt8 /*disp_a_flag*/)				{}
	virtual void SetPSRForPiPPGTextSTStream(bool /*bSetStrm*/, UInt32 /*uiStreamNum*/, UInt8 /*disp_s_flag*/, UInt8 /*PIP_PG_textST_valid_flag*/){}

	// get info
	virtual SonicError GetPlayListPlaybackType(UInt32 uiPlayListId, BDROM_PlayList_PlaybackType & playlistPlaybackType)			{ return -1; }
	virtual SonicError GetNumberOfPlayItems(UInt32 /*uiPlayListID*/, UInt32 & /*uiNumPlayItems*/)								PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetPlayListUOPsMask(UInt64 & /*UOPMask*/)																PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetPlayListDuration(UInt32 /*uiPlayListNum*/,  TimeValue & /*ts45k*/)									PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetPlayItemDuration(UInt32 /*uiPlayListNum*/, UInt32 & /*ts45k*/)										PLAYBACKCTRLENGINE_IFACE_IMPL
 	virtual SonicError GetNumMarks(UInt32 /*uiPlayListNum*/, BDROM_MarkType /*markType*/, UInt32 & /*uiNumMarks*/)											PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetMarkInfo(UInt32 /*uiPlayListNum*/, UInt32 /*uiMarkNum*/, BDROM_MarkType /*markType*/, BDROM_PL_MarkInfo & /*markInfo*/)			PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentLocation(BDROM_Location & /*currLocation*/)													PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentPlayList(UInt32 & /*uiCurrPlayList*/)															PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentPlayItem(UInt32 & /*uiCurrPlayItem*/)															PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentMark(UInt32 & /*uiCurrMarkNum*/)																PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentAngle(UInt32 & /*uiAnglesAvailable*/, UInt32 & /*uiCurrentAngle*/)								{ return -1; }
	virtual SonicError GetCurrentChapterNum(UInt32 & /*uiChapterNum*/)															{ return -1; }
	virtual SonicError GetCurrentApplicationType(BDROM_ApplicationType & /*applType*/)											{ return -1; }
	virtual SonicError GetChapterStartTime(UInt32 /*uiChapterNum*/, TimeValue & /*ts45kStartTime*/)								{ return -1; }
	virtual SonicError GetChapterDuration(UInt32 /*uiChapterNum*/, TimeValue & /*ts45kDuration*/)								{ return -1; }

	// preferences
	virtual void SetPreferredAudioLanguage(const char * pLangCode)																{ }
	virtual void SetPreferredMenuLanguage(const char * pLangCode)																{ }
	virtual void SetPreferredPGTextSTLanguage(const char * pLangCode)															{ }
	virtual void SetAudioCapability(UInt32 uiLPCM192, UInt32 uiLPCM48and96, UInt32 uiDDpIndep, UInt32 uiDDpDep,
									UInt32 uiDTSHDCore, UInt32 uiDTSHDExtn, UInt32 uiDolbyDigital, UInt32 uiDolbyMLP)			{ }
	virtual void SetParentalLevel(UInt8 uiParentalLevel)																		{ }
	virtual void SetPlayerProfile(UInt8 uiPlayerProfile)																		{ }
	virtual void SetParentalLevelChangeByBDJxletAllowed(bool bAllowed)															{ }
	virtual void SetCountryCodeChangeByBDJxletAllowed(bool bAllowed)															{ }
	virtual void SetLanguageCodeChangeByBDJxletAllowed(bool bAllowed)															{ }
	virtual void AuthorizeParentalLevelChange(bool bGranted)																	{ }

	// elapsed time
	virtual void SetElapsedTime(Time90K /*ts45kElapsedTime */)																	{ }

	// disc id
	virtual SonicError GetDiscId(BDROMDiscId & /*discId*/)																		PLAYBACKCTRLENGINE_IFACE_IMPL

	// functions to get any stream's properties
	virtual SonicError GetCurrentStream(BDROM_StreamType /*streamType*/, UInt32 & /*uiStreamsAvailable*/, UInt32 & /*uiCurrentStream*/)		PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentStreamAttributes(BDROM_StreamType /*streamType*/, BDROM_Stream & /*streamAttr*/)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetStreamAttributes(BDROM_StreamType /*streamType*/, UInt32 /*uiStream*/, BDROM_Stream & /*streamAttr*/)				PLAYBACKCTRLENGINE_IFACE_IMPL

	// playlist info
	virtual SonicError GetCurrentPlayListDuration(UInt32 & /*45kplaylistDuration*/)										PLAYBACKCTRLENGINE_IFACE_IMPL

	// PiP control
	//
	virtual bool       IsPiPSubtitleMode(void)																					PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetPiPSubtitleMode(bool /*bSubtitleOn*/)																	PLAYBACKCTRLENGINE_IFACE_IMPL

	// IsPiPDisplayed: Returns true if Secondary Video is being displayed
	virtual bool       IsPiPDisplayed(void)																						PLAYBACKCTRLENGINE_IFACE_IMPL
	// IsPiPEnabled: Returns true if Secondary Video is enabled for display
	virtual bool       IsPiPEnabled(void)																						PLAYBACKCTRLENGINE_IFACE_IMPL
	// IsPiPAvailable: Returns true if Secondary Video is being available
	virtual bool       IsPiPAvailable(void)																						PLAYBACKCTRLENGINE_IFACE_IMPL
	// GetNumberOfPiPStreams: Returns a number of Secondary Video streams
	virtual UInt32	   GetNumberOfPiPStreams(void)																				PLAYBACKCTRLENGINE_IFACE_IMPL

	// returns true if playlist has PiP
	virtual bool       PlaylistHasPiP(UInt32 /*playlistId*/)																	{ return false; }

	// IsPiPSynchedDuringTrickPlay: Returns true if Secondary Video playback is synched to primary video during trick play.
	// Returns false if secondary video is either not displayed during trick play or the secondary video frames that are displayed
	// are not synched to the primary video frames displayed during trick play.
	virtual bool       IsPiPSynchedDuringTrickPlay(void)																		PLAYBACKCTRLENGINE_IFACE_IMPL
	// EnablePiPMetadataProcessing: Enables/Disables PiP Metadata processing
	virtual void       EnablePiPMetadataProcessing(bool /*bEnable*/)															PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual bool       IsPiPMetadataProcessingEnabled(void)																		PLAYBACKCTRLENGINE_IFACE_IMPL

	// StartAsyncPiP: Starts playback of selected Async PiP stream. If no stream is selected (i.e., as per PSR), an Async
	// PiP stream is selected.
	virtual SonicError StartAsyncPiP(void)																						PLAYBACKCTRLENGINE_IFACE_IMPL

	// StopAsyncPiP: Stops playback of current Async PiP stream
	virtual SonicError StopAsyncPiP(void)																						PLAYBACKCTRLENGINE_IFACE_IMPL

	// GetAsyncPiPMediaTime: Retrieves Async PiP's media time (elapsed time as a 45 KHz value)
	virtual SonicError GetAsyncPiPMediaTime(UInt32 & /*ts45kPlaybackElapsedTime*/)												PLAYBACKCTRLENGINE_IFACE_IMPL

	// Subtitle Control
	virtual SonicError SetSubtitleMode(bool /*bSubtitleOn*/)																	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetSubtitleStyle(UInt32 subtitleStyle)																	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetNumberOfSubtitleStyles(UInt32 &numberOfSTStyles)														PLAYBACKCTRLENGINE_IFACE_IMPL

	virtual SonicError GetCurrentAudio(UInt32 & /*uiStreamsAvailable*/, UInt32 & /*uiCurrentStream*/)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetSecondaryAudio(UInt32 & /*uiStreamsAvailable*/, UInt32 & /*uiCurrentStream*/)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentAudioAttributes(BDROM_AudioAttributes & /*aatr*/)												PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetAudioAttributes(UInt32 /*uiStream*/, BDROM_AudioAttributes & /*aatr*/)								PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentAngle(UInt8 & /*uiNumberOfAngles*/, UInt8 & /*uiCurrentAngle*/)								PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentVideoAttributes(BDROM_VideoAttributes & /*vatr*/)												PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentStreamPIDs(UInt32 & /*uiVideoPid*/, UInt32 & /*uiPrimaryAudioPid*/,
											UInt32 & /*uiSecondaryAudioPid*/, UInt32 & /*uiPGPid*/,
											UInt32 & /*uiIGPid*/, UInt32 & /*uiTextPID*/)										PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentPGStream(UInt16 & /*uiNumberOfPGStreams*/, UInt16 & /*uiCurrentStream*/)						PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentIGStream(UInt8 & /*uiNumberOfIGStreams*/, UInt8 & /*uiCurrentStream*/)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetPGStreamAttributes(UInt8 /*uiStreamNumber*/, BDROM_GraphicsAttributes & /*bdattr*/)					PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetIGStreamAttributes(UInt8 /*uiStreamNumber*/, BDROM_GraphicsAttributes & /*bdattr*/)					PLAYBACKCTRLENGINE_IFACE_IMPL
	
	virtual SonicError SetPGPacketCallback(PGPacketCallback /*pCallback*/)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetIGPacketCallback(IGPacketCallback /*pCallback*/)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError PlaySound(UInt32 /*sound_id*/)																			PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetNVTimer(bool /*bMobjIDIsImmediate*/, UInt16 /*MobjID_or_GPR*/,  bool /*bTimerValueIsImmediate*/,
								UInt16 /*TimerValue_or_GPR*/)																	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError IsVideoFullScreen(bool & /*fullScreen*/)																	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetActiveVideoArea(UInt32 & /*left*/, UInt32 & /*top*/, UInt32 & /*right*/, UInt32 & /*bottom*/)			PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetActiveVideoAreaOnScreen(UInt32 & /*left*/, UInt32 & /*top*/, UInt32 & /*right*/, UInt32 & /*bottom*/)	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetClipRegion(UInt32 & /*left*/, UInt32 & /*top*/, UInt32 & /*right*/, UInt32 & /*bottom*/)				PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetHorizontalScalingFactors(float * /*pScaleFactorArray*/, size_t /*arraySize*/)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetInputVideoSize(UInt32 & /*uiHeight*/, UInt32 & /*uiWidth*/)											PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetPositioningCapability(POS_CAP & /*pc*/)																PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetTotalVideoArea(UInt32 & /*left*/, UInt32 & /*top*/, UInt32 & /*right*/, UInt32 & /*bottom*/)			PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetTotalVideoAreaOnScreen(UInt32 & /*left*/, UInt32 & /*top*/, UInt32 & /*right*/, UInt32 & /*bottom*/)	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetVerticalScalingFactors(float * /*pScaleFactorArray*/, UInt32 /*arraySize*/)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetVideoSize(UInt32 & /*uiHeight*/, UInt32 & /*uiWidth*/)												PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetClipRegion(UInt32 /*left*/, UInt32 /*top*/, UInt32 /*Right*/, UInt32 /*bottom*/)						PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SupportsArbitraryHorizontalScaling(bool & /*bSupported*/,  float & /*flMinScale*/, float & /*flMaxScale*/)	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SupportsArbitraryVerticalScaling(bool & /*bSupported*/, float & /*lMinScale*/, float & /*flMaxScale*/)	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SupportsClipping(bool & bSupported)																		PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError IsPanAndScan(bool &bIsPanScan)																			PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError CanScaleSourceToDestination(UInt32 /*sXIn*/,   UInt32 /*sYIn*/,   UInt32 /*sWIn*/,   UInt32 /*sHIn*/,
												   UInt32 /*dXIn*/,   UInt32 /*dYIn*/,   UInt32 /*dWIn*/,   UInt32 /*dHIn*/,
												   UInt32 &/*sXOut*/, UInt32 &/*sYOut*/, UInt32 &/*sWOut*/, UInt32 &/*sHOut*/,
												   UInt32 &/*dXOut*/, UInt32 &/*dYOut*/, UInt32 &/*dWOut*/, UInt32 &/*dHOut*/)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError ScaleSourceToDestination(UInt32 /*sX*/, UInt32 /*sY*/, UInt32 /*sW*/, UInt32 /*sH*/, UInt32 /*dX*/, UInt32 /*dY*/, UInt32 /*dW*/, UInt32 /*dH*/)		PLAYBACKCTRLENGINE_IFACE_IMPL

	virtual SonicError SetScalingFactors(float /*horizontalScalingFactor*/, float /*verticalScalingFactor*/)					PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetScalingFactors(float & /*horizontalScalingFactor*/, float & /*verticalScalingFactor*/)				PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetVideoPosition(UInt32 & /*x*/, UInt32 & /*y*/)															PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetVideoPosition(UInt32 /*x*/, UInt32 /*y*/)																PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError ConfigureKeepResolution(bool /*bKeepResolution*/, UInt16 /*uiBaseResWidth*/, UInt16 /*uiBaseResHeight*/, bool /*bUseScreenRect*/, 
												float /*screenRectX*/, float /*screenRectY*/, float /*screenRectWidth*/, float /*screenRectHeight*/)	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetKeepResolution(bool &/*bKeepResolution*/)																PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetFullScreenMode(bool /*bMode*/)																		PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError ResetVideoSettings()																						PLAYBACKCTRLENGINE_IFACE_IMPL

	// org.bluray.media
	virtual SonicError GetCurrentLanguage(BDROM_StreamType streamType, UInt32& /*uiLanguage*/)									PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError ListAvailableLanguages(BDROM_StreamType streamType, UInt32 * /*uiNumOfLanguages*/, 
													UInt32 * /*uiLanguageArray*/ )												PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual	SonicError SelectLanguage(BDROM_StreamType streamType, UInt32 /*uiLanguage*/)										PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SelectDefaultLanguage(BDROM_StreamType streamType, UInt32 & /*uiLanguage*/)								PLAYBACKCTRLENGINE_IFACE_IMPL

	// org.bluray.ti
	virtual SonicError GetPlayListInfo(UInt32 /*uiPlayListId*/, BDROM_PL_PLInfo &/*playListInfo*/)								PLAYBACKCTRLENGINE_IFACE_IMPL

	// com.sonic.javax.media
	virtual SonicError GetRate(BDROM_PlayDirection & /*direction */, UInt32 & /*uiSpeed */ )									PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetDuration(UInt32 & /*ts45kPlaybackDuration*/)													PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetMediaTime(UInt32 & /*ts45kPlaybackElapsedTime */)												PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetStopTime(UInt32 /* ts45kStopTime */)															PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetStopTime(UInt32 & /* ts45kStopTime */ )														PLAYBACKCTRLENGINE_IFACE_IMPL

	// OBSOLETE VERSIONS that use DVDTimeStampType -- Deprecated
	virtual SonicError GetDuration(DVDTimeStampType & /*tsPlaybackDuration */)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetMediaTime(DVDTimeStampType & /*tsPlaybackElapsedTime*/)													PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError SetStopTime(DVDTimeStampType /*tsStopTime*/)																	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetStopTime(DVDTimeStampType & /*tsStopTime*/)																PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError PlayPLatTime(UInt32 ulPlayListNum, DVDTimeStampType /*ts*/, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED)	PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError TimeSearch(DVDTimeStampType /*ts*/, double dRate = BDROM_NORMAL_FORWARD_PLAY_SPEED)							PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetPlayListDuration(UInt32 uiPlayListNum,  DVDTimeStampType & /*ts*/)										PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetPlayItemDuration(UInt32 uiPlayListNum, DVDTimeStampType & /*ts*/)											PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetCurrentPlayListDuration(DVDTimeStampType & /*playlistDuration*/)										PLAYBACKCTRLENGINE_IFACE_IMPL

	virtual SonicError GetAACSVolumeID(ref_ptr_array<UInt8> &ID)																PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetAACSMediaSerialNum(ref_ptr_array<UInt8> &PMSN)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetAACSDeviceBindingID(ref_ptr_array<UInt8> &devID)														PLAYBACKCTRLENGINE_IFACE_IMPL 
	virtual SonicError GetAACSContentCertID(ref_ptr_array<UInt8> &certID)														PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual SonicError GetAACSNonce(UInt32 titleID, ref_ptr_array<UInt8> &nonce)												PLAYBACKCTRLENGINE_IFACE_IMPL 
	virtual SonicError SetAACSPermission(UInt32 titleID, ref_ptr_array<UInt8> perm)												PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual bool CheckAACSPermission(UInt32 titleID)																		PLAYBACKCTRLENGINE_IFACE_IMPL
	virtual bool IsAACSPermissionCacheable()																					PLAYBACKCTRLENGINE_IFACE_IMPL

	//---------------------------------------------------------------------------------
	// API for breaking and continuing playback - For Emulator use only

	// Break: Pauses playback.
	virtual SonicError Break(void)																							{ return -1; }

	// Continue: Resumes playback
	virtual SonicError Continue(void)																						{ return -1; }
};

#endif

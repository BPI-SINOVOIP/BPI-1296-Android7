//-----------------------------------------------------------------------------
// BDROM_NavInfoUtils.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROM_NAVINFOUTILS_H__
#define __BDROM_NAVINFOUTILS_H__

#include "TimeValue.h"

#include "BDROMPlaylist.h"
#include "BDROMClipInfo.h"
#include "BDROMCPI.h"

#include "BDROM_NavigatorTypes.h"
#include "SonicPBCE_CommonTypes.h"
#include "BDROM_NavigatorTypes_Internal.h"
#include "BDROM_NavigatorErrors.h"

#include "TimeValue.h"
#include "DVDnUtilities.h"

namespace BDROM_NavInfoUtils
{

//_____________________________________________________________________________________________________________________________________________
// Extacting info from PlayList file
//

// GetNumberOfPlayListMarks
//	Gets number of PlayList Marks in playlist.
// Parameters:
//	pPlayListMpls		[ in] : PlayList info.
// Returns:
//	Number of PlayItem Marks
//
/*
UInt32 GetNumberOfPlayListMarks(BDROM::PlayList_mpls * pPlayListMpls)
{
	assert(pPlayListMpls);
	return pPlayListMpls->mPlayListMark.mnumber_of_PlayListMarks;
}
*/

// GetNumberOfPlayItems
//	Gets number of PlayItems in playlist.
// Parameters:
//	pPlayListMpls		[ in] : PlayList info.
// Returns:
//	Number of PlayItems
//
/*
UInt32 GetNumberOfPlayItems(BDROM::PlayList_mpls * pPlayListMpls)
{
	assert(pPlayListMpls);
	return m_pPlayList_mpls->mPlayList.mnumber_of_PlayItems;
}
*/

// GetPlayItemElapsedTimes
//	Gets Start and End Elapsed time (as 45 Khz values) of a PlayItem.
//	For a PlayList with Sequential playback type, these are elapsed times in PlayList timeline.
//	For a PlayList with Random/Shuffle playback type, these are elapsed times in PlayItem timeline.
// Parameters:
//	pPlayListMpls		[ in] : PlayList info.
//	uiPlayItemId		[ in] : PlayItem Id.
//	ts45kOutElapsedTime	[out] : Elapsed Time that corresponds to IN_time of PlayItem.
//	ts45kOutElapsedTime	[out] : Elapsed Time that corresponds to OUT_time of PlayItem.
//
SonicError GetPlayItemElapsedTimes(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiPlayItemId, Time90K & ts45kInElapsedTime, Time90K & ts45kOutElapsedTime);

// GetStreamAttributes
//	Gets attributes of a stream in the STN_table for a given stream type and stream number.
// Parameters:
//	pPlayItemInfo	[ in] : PlayItem info.
//	streamType		[ in] : Stream Type.
//	streamEntryType	[ in] : stream_entry_type (playitem / subplayitem) of stream to find.
//	uiStreamNum		[ in] : Stream Number.
//	ppStrmEntry		[out] : Ptr to stream_entry ptr. pointer to stream_entry of stream is returned in this.
//	streamType		[out] : Ptr to stream_attributes ptr. pointer to stream_attributes of stream is returned in this.
//
SonicError GetStreamAttributes(BDROM::PlayItem * pPlayItemInfo, BDROM_StreamType streamType, BDROM_StreamEntryType streamEntryType, UInt32 uiStreamNum,
							   BDROM::stream_entry ** ppStrmEntry, BDROM::stream_attributes ** ppStrmAttr);

// GetSubPlayItemId
//	Gets id of SubPlayItem that matches the specified path id, start playitem id, time offset
// Parameters:
//	uiSubPathId				[ in] : Subpath_id of SubPath to load
//	uiStartPlayItemId		[ in] : playitem_id of PlayItem in MainPath to that subplayitem must map to
//                                 (i.e, it is not necessarily the sync_playitem_id of subplayitem)
//	ts45kOffsetIntoPlayItem	[ in] : 45Khz offset into PlayItem (specified by uiSyncPlayItemId) to which subpath must match
//	uiSubPlayItemId			[out] : Matching SubPlayItem id.
//
SonicError GetSubPlayItemId(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiSubPathId, UInt32 uiStartPlayItemId, Time90K ts45kOffsetIntoPlayItem,
							UInt32 & uiSubPlayItemId);

// GetNextSubPlayItemId
//	Gets id of the next SubPlayItem that matches the specified path id, start playitem id, time offset
// Parameters:
//	uiSubPathId				[ in] : Subpath_id of SubPath to load
//	uiStartPlayItemId		[ in] : playitem_id of PlayItem in MainPath to that subplayitem must map to
//                                 (i.e, it is not necessarily the sync_playitem_id of subplayitem)
//	ts45kOffsetIntoPlayItem	[ in] : 45Khz offset into PlayItem (specified by uiSyncPlayItemId) to which subpath must match
//	uiSubPlayItemId			[out] : Matching SubPlayItem id.
//
SonicError GetNextSubPlayItemId(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiSubPathId, UInt32 uiStartPlayItemId, Time90K ts45kOffsetIntoPlayItem,
								UInt32 & uiSubPlayItemId);

// GetSubPlayItemPlayItems
//	Gets Start and End PlayItems of a SubPlayItem.
//	For a PlayList with Sequential playback type, the start playitem is always sync_playitem_id, but the end playitem is calculated based on subplayitem duration.
//	For a PlayList with Random/Shuffle playback type, these are the same (always sync_playitem_id).
// Parameters:
//	pPlayListMpls		[ in] : PlayList info.
//	uiSubPathId			[ in] : SubPath Id.
//	uiSubPlayItemId		[ in] : SubPlayItem Id.
//	uiStartPlayItemId	[out] : Start PlayItem of subplayitem.
//	uiEndPlayItemId		[out] : End PlayItem of subplayitem.
//
SonicError GetSubPlayItemPlayItems(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiSubPathId, UInt32 uiSubPlayItemId,
								   UInt32 & uiStartPlayItemId, UInt32 & uiEndPlayItemId);

// IsConnectionSeamless
//	Determines if connection from a specified playitem to the next is seamless
// Parameters:
//	pPlayListMpls	[ in] : PlayList info.
//	uiPlayItemId	[ in] : Id of playitem whose connection with next playitem is to to be tested.
//
bool IsConnectionWithNextSeamless(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiPlayItemId);

// IsConnectionSeamless
//	Determines if connection from a specified sub-playitem to the next is seamless
// Parameters:
//	pPlayListMpls	[ in] : PlayList info.
//	uiSubPathId		[ in] : Id of subpath that contains the subplayitem whose connection with next playitem is to to be tested.
//	uiSubPlayItemId	[ in] : Id of subplayitem whose connection with next playitem is to to be tested.
//
bool IsConnectionWithNextSeamless(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiSubPathId, UInt32 uiSubPlayItemId);

// IsPiPPGTextSTAndSecVideoComboAllowed
//	Determines if specified PG/TextST stream is selectable for specified secondary video stream.
// Parameters:
//	pPlayListMpls				[ in] : PlayList info.
//	uiPlayItemId				[ in] : Id of playitem whose STN_table is to be examined.
//	uiPiPPGTextSTStreamNum		[ in] : PiP PG/TextST stream number.
//	uiSecondaryVideoStreamNum	[ in] : Secopndary Video stream number.
//
bool IsPiPPGTextSTAndSecVideoComboAllowed(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiPlayItemId,
										  UInt32 uiPiPPGTextSTStreamNum, UInt32 uiSecondaryVideoStreamNum);

// IsSecAudioAndPrimaryAudioComboAllowed
//	Determines if specified Secondary Audio stream is selectable for specified Primary Audio stream.
// Parameters:
//	pPlayListMpls				[ in] : PlayList info.
//	uiPlayItemId				[ in] : Id of playitem whose STN_table is to be examined.
//	uiSecAudioStreamNum			[ in] : Secondary Audio stream number.
//	uiPrimaryAudioStreamNum		[ in] : Primary Audio stream number.
//
bool IsSecAudioAndPrimaryAudioComboAllowed(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiPlayItemId,
										   UInt32 uiSecAudioStreamNum, UInt32 uiPrimaryAudioStreamNum);

// IsSecAudioAndSecVideoComboAllowed
//	Determines if specified Secondary Audio stream is selectable for specified Secondary Video stream.
// Parameters:
//	pPlayListMpls				[ in] : PlayList info.
//	uiPlayItemId				[ in] : Id of playitem whose STN_table is to be examined.
//	uiSecAudioStreamNum			[ in] : Secondary Audio stream number.
//	uiSecondaryVideoStreamNum	[ in] : Secondary Video stream number.
//
bool IsSecAudioAndSecVideoComboAllowed(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiPlayItemId,
									   UInt32 uiSecAudioStreamNum, UInt32 uiSecondaryVideoStreamNum);

// IsAsyncPiP
//	Determines if specified secondary video stream is in an Async PiP.
// Parameters:
//	pPlayListMpls	[ in] : PlayList info.
//	uiPlayItemId	[ in] : Id of playitem whose STN_table is to be examined.
//	uiStreamNum		[ in] : Secopndary Video stream number.
//
bool IsAsyncPiP(BDROM::PlayList_mpls * pPlayListMpls, UInt32 uiPlayItemId, UInt32 uiStreamNum);

// PlayitemLocationToPlaylistTime
//	Converts a Playitem Location (playitem_id and PTS in that playitem) to a Playlist time (aka media time)
// Parameters:
//	pPlayListMpls	[ in] : PlayList info.
//	playitemId		[ in] : playitem Id.
//	playitemPts45k	[ in] : 45KHz PTS in playitem (in IN_time - OUT_time range)
//	playlistPts45k	[out] : 45KHz Playlist time (aka media time)
//
bool PlayitemLocationToPlaylistTime(BDROM::PlayList_mpls * pPlayListMpls, UInt32 playitemId, UInt32 playitemPts45k, TimeValue & playlistPts45k);

// PlaylistTimeToPlayitemLocation
//	Converts a Playlist time (aka media time) to a Playitem Location (playitem_id and PTS in that playitem)
// Parameters:
//	pPlayListMpls	[ in] : PlayList info.
//	playlistPts45k	[ in] : 45KHz Playlist time (aka media time)
//	playitemId		[out] : playitem Id.
//	playitemPts45k	[out] : 45KHz PTS in playitem (in IN_time - OUT_time range)
//
bool PlaylistTimeToPlayitemLocation(BDROM::PlayList_mpls * pPlayListMpls, TimeValue playlistPts45k, UInt32 & playitemId, UInt32 & playitemPts45k);


//_____________________________________________________________________________________________________________________________________________
// Extacting info from ClipInfo file
//

// GetApplicationType
//	Gets Application Type from ClipInfo
// Parameters:
//	pClipInfoClpi	[ in] : Clip info.
// Returns:
//	Application Type
BDROM_ApplicationType GetApplicationType(BDROM::ClipInfo_clpi * pClipInfoClpi);

// GetSPNofPTS
//	Gets SPN associated with a PTS
// Parameters:
//	pts90kTime			[ in] : PTS in 90KHz
//	bAlignToLowerEntry	[ in] : indicates which EP_map entry to get spn from if PTS falls between 2 entries
//	pClipInfo			[ in] : Ptr to ClipInfo that contains the EP_map
//	uiATCSeqIdx			[ in] : ATC Sequence Index (1 based)
//	uiSTCSeqIdx			[ in] : STC Sequence Index (1 based)
//	pStreamPIDEntry		[ in] : Ptr to EP_map info for a stream.
//	pEPMapForOneStream	[ in] : Ptr to EP_map entries for a stream. Must correspond to stream that pStreamPIDEntry is for.
//	spn					[out] : SPN that corresponds to input PTS
//
SonicError GetSPNofPTS(Time90K pts90kTime, bool bAlignToLowerEntry, BDROM::ClipInfo_clpi * pClipInfo,
						BDROM::stream_PID_entry * pStreamPIDEntry, BDROM::EP_map_for_one_stream * pEPMapForOneStream,
						UInt32 uiATCSeqIdx, UInt32 uiSTCSeqIdx, UInt32 & spn);

// GetStreamAttributes
//	Gets attributes of a stream in ClipInfo for a given PID.
// Parameters:
//	pClipInfoClpi	[ in] : Clip info.
//	uiPID			[ in] : PID.
//	streamAttr		[out] : Stream attributes.
//
SonicError GetStreamAttributes(BDROM::ClipInfo_clpi * pClipInfoClpi, UInt16 uiPID, BDROM_Stream & streamAttr);

//-------------------------------------------------------
// Convert45kToDVDTimestamp
//	Converts from a numeric time value to a timestamp object.
//
void Convert45kToDVDTimestamp(TimeValue & ts45kDuration, DVDTimeStampType & duration);

//-------------------------------------------------------
// Convert45kToDVDTimestamp
//	Converts from a numeric time value to a timestamp object.
//
void Convert45kToDVDTimestamp(UInt32 & ts45kDuration, DVDTimeStampType & duration);

// ExtractVideoAttributes
//	Extracts Video Attributes from StreamInfo in Clip's ProgramInfo
// Parameters:
//	pStreamInfo		[ in] : StreamInfo from a ProgramInfo.
//	pVideoAttr		[out] : Video attributes.
//
inline void ExtractVideoAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_VideoAttributes * pVideoAttr)
{
	assert(pStreamInfo);
	assert(pVideoAttr);
	pVideoAttr->codingType  = (BDROM_StreamCodingTypes)((UInt32)pStreamInfo->mStreamCodingInfo.mstream_coding_type);
	pVideoAttr->videoFormat = (BDROM_VideoFormat)      ((UInt32)pStreamInfo->mStreamCodingInfo.mvideo_format_table.mvideo_format);
	pVideoAttr->aspectRatio = (BDROM_VideoAspectRatio) ((UInt32)pStreamInfo->mStreamCodingInfo.mvideo_format_table.maspect_ratio);
	pVideoAttr->frameRate   = (BDROM_VideoFrameRate)   ((UInt32)pStreamInfo->mStreamCodingInfo.mvideo_format_table.mframe_rate);
	pVideoAttr->bLine21Data = (pStreamInfo->mStreamCodingInfo.mvideo_format_table.mcc_flag == 1);
}

// ExtractAudioAttributes
//	Extracts Audio Attributes from StreamInfo in Clip's ProgramInfo
// Parameters:
//	pStreamInfo		[ in] : StreamInfo from a ProgramInfo.
//	pAudioAttr		[out] : Audio attributes.
//
inline void ExtractAudioAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_AudioAttributes * pAudioAttr)
{
	assert(pStreamInfo);
	assert(pAudioAttr);
	pAudioAttr->codingType       = (BDROM_StreamCodingTypes)    ((UInt32)pStreamInfo->mStreamCodingInfo.mstream_coding_type);
	pAudioAttr->samplingFreq     = (BDROM_AudioSamplingFreq)    ((UInt32)pStreamInfo->mStreamCodingInfo.maudio_format_table.msampling_frequency);
	pAudioAttr->presentationType = (BDROM_AudioPresentationType)((UInt32)pStreamInfo->mStreamCodingInfo.maudio_format_table.maudio_presentation_type);
	pAudioAttr->language[0]      = pStreamInfo->mStreamCodingInfo.maudio_format_table.maudio_language_code[1];
	pAudioAttr->language[1]      = pStreamInfo->mStreamCodingInfo.maudio_format_table.maudio_language_code[2];
	pAudioAttr->language[2]      = pStreamInfo->mStreamCodingInfo.maudio_format_table.maudio_language_code[3];
#if 1
	// let demux determine correct channel info
	pAudioAttr->audioChannels    = kBDROM_AudioChannels_Unknown;
#else
	if (pAudioAttr->presentationType == BDROM_AudioPres_SingleMono)
		pAudioAttr->audioChannels    = kBDROM_AudioChannels_1;
	else if (pAudioAttr->presentationType == BDROM_AudioPres_Stereo)
		pAudioAttr->audioChannels    = kBDROM_AudioChannels_2;
	else
		pAudioAttr->audioChannels    = kBDROM_AudioChannels_Unknown;
#endif
	pAudioAttr->bitsPerSample    = BDROM_AUDIO_BITS_PER_SAMPLE_NOINFO;
}

// ExtractPGAttributes
//	Extracts PG Attributes from StreamInfo in Clip's ProgramInfo
// Parameters:
//	pStreamInfo		[ in] : StreamInfo from a ProgramInfo.
//	pGraphicsAttr	[out] : PG attributes.
//
inline void ExtractPGAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_GraphicsAttributes * pGraphicsAttr)
{
	assert(pStreamInfo);
	assert(pGraphicsAttr);
	pGraphicsAttr->language[0] = pStreamInfo->mStreamCodingInfo.mPG_format_table.mPG_language_code[1];
	pGraphicsAttr->language[1] = pStreamInfo->mStreamCodingInfo.mPG_format_table.mPG_language_code[2];
	pGraphicsAttr->language[2] = pStreamInfo->mStreamCodingInfo.mPG_format_table.mPG_language_code[3];
}

// ExtractIGAttributes
//	Extracts IG Attributes from StreamInfo in Clip's ProgramInfo
// Parameters:
//	pStreamInfo		[ in] : StreamInfo from a ProgramInfo.
//	pGraphicsAttr	[out] : IG attributes.
//
inline void ExtractIGAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_GraphicsAttributes * pGraphicsAttr)
{
	assert(pStreamInfo);
	assert(pGraphicsAttr);
	pGraphicsAttr->language[0] = pStreamInfo->mStreamCodingInfo.mIG_format_table.mIG_language_code[1];
	pGraphicsAttr->language[1] = pStreamInfo->mStreamCodingInfo.mIG_format_table.mIG_language_code[2];
	pGraphicsAttr->language[2] = pStreamInfo->mStreamCodingInfo.mIG_format_table.mIG_language_code[3];
}

// ExtractTextSTAttributes
//	Extracts TextST Attributes from StreamInfo in Clip's ProgramInfo
// Parameters:
//	pStreamInfo		[ in] : StreamInfo from a ProgramInfo.
//	pGraphicsAttr	[out] : TextST attributes.
//
inline void ExtractTextSTAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_GraphicsAttributes * pGraphicsAttr)
{
	assert(pStreamInfo);
	assert(pGraphicsAttr);
	pGraphicsAttr->charCode    = (BDROM_CharSet)(UInt32)pStreamInfo->mStreamCodingInfo.mtextST_format_table.mcharacter_code;
	pGraphicsAttr->language[0] = pStreamInfo->mStreamCodingInfo.mtextST_format_table.mtextST_language_code[1];
	pGraphicsAttr->language[1] = pStreamInfo->mStreamCodingInfo.mtextST_format_table.mtextST_language_code[2];
	pGraphicsAttr->language[2] = pStreamInfo->mStreamCodingInfo.mtextST_format_table.mtextST_language_code[3];
}

// GetNumberOfFonts
//	Gets Number of Fonts (associated with a TextST clip file)
// Parameters:
//	pClipInfoClpi	[ in] : Clip info.
// Returns:
//	Number of Fonts
//
inline UInt32 GetNumberOfFonts(BDROM::ClipInfo_clpi * pClipInfoClpi)
{
	assert(pClipInfoClpi);
	assert(pClipInfoClpi->mClipInfo.mapplication_type == kBDROM_ApplType__SubPath_TextSubtitle);

	return pClipInfoClpi->mClipInfo.mnumber_of_font_files;
}

// GetFontName
//	Gets the name of a Font
// Parameters:
//	pClipInfoClpi	[ in] : Clip info.
//	uiFontId		[ in] : Font Id (0 based)
//	FontDesc		[out] : Font Descriptor. Font name field of the descriptor is filled in when function returns successfully.
// Returns:
//	Number of Fonts
//
inline SonicError GetFontName(BDROM::ClipInfo_clpi * pClipInfoClpi, UInt32 uiFontId, BDROM_FontDesciption & fontDesc)
{
	assert(pClipInfoClpi);
	assert(pClipInfoClpi->mClipInfo.mapplication_type == kBDROM_ApplType__SubPath_TextSubtitle);

	if (pClipInfoClpi->mClipInfo.mnumber_of_font_files > uiFontId)
	{
		fontDesc.mFontId = uiFontId;
		for (UInt32 i = 0; i < 5; i++)
			fontDesc.mFontFile[i] = pClipInfoClpi->mClipInfo.mfont_files[uiFontId+1].mfont_file_name[i+1];
		fontDesc.mFontFile[5] = '\0';
		return kBDROMNavStreamErrorNoErr;
	}
	else
	{
		return kBDROMNavStreamErrorNotAvailable;
	}
}

};

#endif // __BDROM_NAVINFOUTILS_H__

#ifndef _SI_CH_API_H_
#define _SI_CH_API_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
	#include "siChannel.h"
	#include "siFixedBugList.h"
#else
	#include <si3/siTypes.h>
	#include <si3/siChannel.h>
#endif



BOOL SiChApi_PhyChExist(TPMOD tpmod, UINT32 frequency, UINT32 phyChIndex);
BOOL SiChApi_VirChExist(TPMOD tpmod, SI_CHANNEL *channel);

BOOL SiChApi_ResetAllUserAudioIndex(TPMOD tpmod);

#ifdef DVR
BOOL SiChApi_ResetAllUserSubtitleIndex(TPMOD tpmod);
BOOL SiChApi_ResetAllUserTeletextIndex(TPMOD tpmod);
#endif
BOOL SiChApi_SetChStatus(TPMOD tpmod, SI_CHANNEL_STATUS status);
BOOL SiChApi_SetChAudio(TPMOD tpmod, UINT16 majorNum, UINT16 minorNum, UINT32 frequency, UINT16 audioPid);

#ifdef DVR
BOOL SiChApi_SetChSubtitle(TPMOD tpmod, UINT16 majorNum, UINT16 minorNum, UINT32 frequency, UINT16 subtitlePid);
BOOL SiChApi_SetChTeletext(TPMOD tpmod, UINT16 majorNum, UINT16 minorNum, UINT32 frequency, UINT16 teletextPid);
#endif

BOOL SiChApi_SetChModulation(TPMOD tpmod, UINT32 frequency, UINT16 majorNum, UINT16 minorNum, SI_MODULATION modulation);
BOOL SiChApi_GetChModulation(TPMOD tpmod, UINT32 frequency, UINT16 majorNum, UINT16 minorNum, SI_MODULATION *modulation);

BOOL SiChApi_IsChannelFiltered(TPMOD tpmod, UINT16 majorNum, UINT16 minorNum, UINT32 frequency, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetChInfo(SI_CHANNEL *channel, TPMOD tpmod, UINT16 majorNum, UINT16 minorNum, UINT32 frequency);
BOOL SiChApi_GetChIndexList(SI_CHANNEL_INDEX_LIST *chIndexList, TPMOD tpmod, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetChInfoByIndex(SI_CHANNEL *channel, TPMOD tpmod, UINT16 chIndex);
BOOL SiChApi_GetFirstChInFreq(SI_CHANNEL *channel, TPMOD tpmod, UINT32 frequency, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetFirstCh(SI_CHANNEL *channel, TPMOD tpmod, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetLastCh(SI_CHANNEL *channel, TPMOD tpmod, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetCurCh(SI_CHANNEL *channel, TPMOD tpmod);
BOOL SiChApi_GetNextCh(SI_CHANNEL *channel, TPMOD tpmod, UINT16 majorChNum, UINT16 minorChNum, UINT32 frequency, SI_CHANNEL_FILTER filter, BOOL autoWrap);
BOOL SiChApi_GetPrevCh(SI_CHANNEL *channel, TPMOD tpmod, UINT16 majorChNum, UINT16 minorChNum, UINT32 frequency, SI_CHANNEL_FILTER filter, BOOL autoWrap);
BOOL SiChApi_GetList(SI_CHANNEL_LIST *channelList, TPMOD tpmod);
BOOL SiChApi_GetListEx(SI_CHANNEL_LIST *channelList, TPMOD tpmod, CHLIST_PARAM param, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetSpecialList(SI_CHANNEL_LIST *channelList, TPMOD tpmod, SPECIAL_CH_PARAM param);
BOOL SiChApi_GetSpecialListEx(SI_CHANNEL_LIST *channelList, TPMOD tpmod, SPECIAL_CH_PARAM param, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetSpecialCount(int *count, TPMOD tpmod, SI_SERVICE serviceType, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetListInFreqEx(SI_CHANNEL_LIST *channelList, TPMOD tpmod, int phyChIndex, int maxCount, SI_CHANNEL_FILTER filter);
BOOL SiChApi_GetChInputListEx(SI_CHANNEL_LIST *channelList, TPMOD tpmod, UINT16 majorNum, UINT16 minorNum, SI_CHANNEL_FILTER filter);

BOOL SiChApi_SavePhyChToFileEx(TPMOD tpmod,UINT8 *fileName, UINT16 majorNum, UINT16 minorNum, UINT32 frequency);
BOOL SiChApi_LoadPhyChFromFile(TPMOD tpmod, UINT8 *fileName);
BOOL SiChApi_SavePhyChToMemoryEx(TPMOD tpmod, UINT8 **buf, UINT32 *bufLen, UINT16 majorNum, UINT16 minorNum, UINT32 frequency, BOOL saveSingleVirCh);
BOOL SiChApi_LoadPhyChFromMemory(TPMOD tpmod, UINT8 *buf, UINT32 bufLen);

BOOL SiChApi_InitFavCh(TPMOD tpmod);
BOOL SiChApi_SaveFavChFile(TPMOD tpmod);
BOOL SiChApi_LoadFavChFile(TPMOD tpmod);
BOOL SiChApi_ResetFavChList(TPMOD tpmod, UINT16 favListId);
BOOL SiChApi_ResetAllFavChList(TPMOD tpmod);
BOOL SiChApi_SetFavChListName(TPMOD tpmod, UINT16 favListId, UINT8 *listName);
BOOL SiChApi_GetFavChListName(TPMOD tpmod, UINT16 favListId, UINT8 *listName);
BOOL SiChApi_AddFavCh(TPMOD tpmod, UINT16 favListId, SI_SHORT_CHANNEL *channel);
BOOL SiChApi_RemoveFavCh(TPMOD tpmod, UINT16 favListId, SI_SHORT_CHANNEL *channel);
BOOL SiChApi_GetFavChList(TPMOD tpmod, SI_CHANNEL_LIST *chList, FAV_LIST_PARAM param);
BOOL SiChApi_GetFavChCount(TPMOD tpmod, int *count, int favListId);
void SiChApi_SyncFavChList(TPMOD tpmod, SI_PHYSICAL_CHANNEL *phyCh);

BOOL SiChApi_SaveUserChList(TPMOD tpmod);
BOOL SiChApi_LoadUserChList(TPMOD tpmod);
BOOL SiChApi_SetUserId(TPMOD tpmod, UINT8 userId, BOOL forceSet);
BOOL SiChApi_GetUserId(TPMOD tpmod, UINT8 *userId);
BOOL SiChApi_ResetUserId(TPMOD tpmod);


#ifdef ENABLE_SDDS
BOOL SiChApi_GetDataChannelList(TPMOD tpmod, SI_CHANNEL_LIST *channelList, UINT32 frequency);
#endif

#ifdef ENABLE_EAM
// ====== EAM Related
BOOL SiChApi_GetFrequencyByChNum(TPMOD tpmod, UINT16 majorChNum, UINT16 minorChNum, UINT32* frequency);
#endif

#ifdef ATSC_SI_FIXBUG16880
BOOL SiChApi_ValidateSiState(const UINT8* pBuf, UINT32 bufLen);
#endif

#endif


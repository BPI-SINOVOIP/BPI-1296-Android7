/*=======================================================================
 * Copyright c				Realtek Semiconductor Corporation, 2007     *
 * All rights reserved.                                                 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *
 * Module		: SI4
 *
 * Author		: ss@realtek.com.tw
 * Date			: 2007/04/23
 *
 *=======================================================================*/
/**
	@file
	@brief SI API

	@author ss@realtek.com.tw
	@date 2007/04/03
*/

// [ss] [20070821] because we integrate it with pvr
//#ifndef _SI_API_H_
//#define _SI_API_H_

#ifndef _SI4_API_H_
#define _SI4_API_H_


//------------------------------------------------------------------------------
//
//	Include Files
//
//------------------------------------------------------------------------------

#include <system/si4/api/SI_ApiMacros.h>


#include <system/si4/SITypes.h>
#include <system/si4/util/siMessage.h> // for SiMessage_Get_Wapper


//------------------------------------------------------------------------------
//
//	Functions
//
//------------------------------------------------------------------------------

/**
	@brief create an si handle
	@param ppSI pointer to an si handle
	@param mode si running mode (playing, recoding, livepause_playback...)
	@return ppSI

	@warning since si handle is a pointer to struct SI,so ppSI is a pointer to pointer to struct SI
*/
ErrCode SI_New
(
	SI									**ppSI,
	SI_MODE								mode
);

/**
	@brief destroy an si handle
	@param ppSI pointer to an si handle

	@warning since si handle is a pointer to struct SI,so ppSI is a pointer to pointer to struct SI
*/
ErrCode SI_Destroy
(
	SI									**ppSI
);

/**
	@brief reset si
	@param pSI si handle
*/
ErrCode SI_Reset
(
	SI									*pSI
);

/**
	@brief set si running mode (playing, recoding, livepause_playback...)
	@param pSI si handle
	@param mode si mode
*/
ErrCode SI_SetMode
(
	SI									*pSI,
	SI_MODE								mode
);

#ifdef FEATURE_010
ErrCode SI_SetTimeOffset(SI *pSI, unsigned char isManual, int timeOffset);
ErrCode SI_GetTimeOffset(SI *pSI, int* retTimeOffset);
#endif

/**
	@brief set region
	@param regionSpec region
*/
ErrCode SI_SetRegion
(
	SI									*pSI,
	SI_SPEC								regionSpec
);

ErrCode SI_Set_IrregularLCN
(
	SI							*pSI,
	unsigned int 					lcn_start
);

/**
	@brief use in pvr to connect with navigation filter
	@param tp is a handle of tp interface
*/
ErrCode SI_Set_TP
(
	SI									*pSI,
	void								*tp
);

/**
	@brief get tp interface handle
	@param ppTP pointer to a tp handle
*/
ErrCode SI_Get_TP
(
	SI									*pSI,
	void								**ppTP
);

/**
	@brief disconnect from navigation filter
*/
ErrCode SI_Unset_TP
(
	SI									*pSI
);

/**
	@brief reset tp. on pvr system, this function reset the tp interface
	@warning for some design issues, this function doesn't remove pid filter
*/
ErrCode SI_Reset_TP
(
	SI									*pSI
);

ErrCode SI_Reset_Full_TP
(
	SI		*pSI
);

ErrCode SI_Reset_TP_Buffer
(
	SI		*pSI
);

/**
	@brief start a thread to collect/maintain section data
*/
ErrCode SI_Start
(
	SI									*pSI
);

/**
	@brief stop collecting section
*/
ErrCode SI_Stop
(
	SI									*pSI
);

/**
	@brief stop all siAgent tasks
*/
ErrCode SI_StopAgentAllTask	// Fix the bug that after auto scan start, cancel auto scan immediately, siAgent still run auto scan tasks.
(
	SI									*pSI
);

// Channel

/**
	@brief initialize to do channel scan
*/
ErrCode SI_AutoScanInit
(
	SI									*pSI
);

/**
	@brief set si states for autoscan,etc..

*/
ErrCode SI_SetState
(
	SI							*pSI,
	SI_STATE					st
);

ErrCode SI_ResetState
(
	SI							*pSI,
	SI_STATE					st
);

ErrCode SIMessage_Get_Wapper
(
	SI_MSG_QUEUE *mq,
	SI_MESSAGE *pMessage,
	unsigned int *pData
);

/**
	@brief scan channel in specific frequency and bandwith
*/
ErrCode SI_ScanChannel
(
	SI									*pSI,
	unsigned int						frequency,
	int									offset,
	unsigned int						bandwidth,
	unsigned int						quality
);

/**
	@brief set channel by specified ch, this function is usually used with
	SI_GetChannelByChannelNumber();
*/
ErrCode SI_SetCurrentChannel
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch
);

/**
	@brief record current channel, used in pvr
*/
ErrCode SI_RecordCurrentChannel
(
	SI									*pSI
);

/**
	@brief memcpy current channel to *ch
	@param ch pointer to struct SI_CHANNEL_EXT
*/
ErrCode SI_GetCurrentChannel
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch
);

/**
	@brief get next channel, skipping the channels we don't want
	@param filterOutFlag to filter out the channels we don't need by
	specific flags, e.g. (SI_CHANNEL_FLAG_IS_DELETED|SI_CHANNEL_FLAG_IS_SCRAMBLED)
	@param queryStrategy SI_CH_QUERY_STRATEGY_LINEAR and SI_CH_QUERY_STRATEGY_LOOP
	@param ch the pointer we memcpy to
*/
ErrCode SI_GetNextChannel
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	SI_CH_QUERY_STRATEGY				queryStrategy,
	SI_CHANNEL_EXT						*ch
);

/**
	@brief get previous channel, skipping the channels we don't want
	@param filterOutFlag to filter out the channels we don't need by
	specific flags, e.g. (SI_CHANNEL_FLAG_IS_DELETED|SI_CHANNEL_FLAG_IS_SCRAMBLED)
	@param queryStrategy SI_CH_QUERY_STRATEGY_LINEAR and SI_CH_QUERY_STRATEGY_LOOP
	@param ch the pointer we memcpy to
*/
ErrCode SI_GetPreviousChannel
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	SI_CH_QUERY_STRATEGY				queryStrategy,
	SI_CHANNEL_EXT						*ch
);

/**
	@brief get last channel we set
	@param ch the pointer we memcpy to
*/
ErrCode SI_GetHistoryChannel
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch
);

/**
	@brief get the first channel in channel list, skipping the channels we don't want
	@param filterOutFlag to filter out the channels we don't need by
	specific flags, e.g. (SI_CHANNEL_FLAG_IS_DELETED|SI_CHANNEL_FLAG_IS_SCRAMBLED)
	@param ch the pointer we memcpy to
*/
ErrCode SI_GetFirstChannel
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	SI_CHANNEL_EXT						*ch
);

/**
	@brief get the last channel in channel list, skipping the channels we don't want
	@param filterOutFlag to filter out the channels we don't need by
	specific flags, e.g. (SI_CHANNEL_FLAG_IS_DELETED|SI_CHANNEL_FLAG_IS_SCRAMBLED)
	@param ch the pointer we memcpy to
*/
ErrCode SI_GetLastChannel
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	SI_CHANNEL_EXT						*ch
);

/**
	@brief get the first channel in specific frequency, skipping the channels we don't want
	@param filterOutFlag to filter out the channels we don't need by
	specific flags, e.g. (SI_CHANNEL_FLAG_IS_DELETED|SI_CHANNEL_FLAG_IS_SCRAMBLED)
	@param ch the pointer we memcpy to
*/
ErrCode SI_GetFirstChannelWithinFrequency
(
	SI									*pSI,
	unsigned int						frequency,
	SI_CHANNEL_FLAG						filterOutFlag,
	SI_CHANNEL_EXT						*ch
);

/**
	@brief get channel by logical channel number
	@param logicalChNum logical channel number, can be used as channels identity
*/
ErrCode SI_GetChannelByChannelNumber
(
	SI									*pSI,
	unsigned short						logicalChNum,
	SI_CHANNEL_EXT						*ch
);

ErrCode SI_GetNextChannelByChNumber
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	unsigned short						logicalChNum,
	SI_CHANNEL_EXT						*ch
);

ErrCode SI_GetPrevChannelByChNumber
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	unsigned short						logicalChNum,
	SI_CHANNEL_EXT						*ch
);

// channel mgr


/**
	@brief get channel list, skipping the channels we don't ask for
	@param filterOutFlag to filter out the channels we don't need by
	specific flags, e.g. (SI_CHANNEL_FLAG_IS_DELETED|SI_CHANNEL_FLAG_IS_SCRAMBLED)
	@param startChannelNumber logical channel number of the datum channel
	@param startChannelIndex if you don't know the logical channel number, you
	can use channel index as a parameter, channel index is a sequential number indicate
	channels' order.
	@param datumChannelOffset specifiy the parameter channel you set by startChannelNumber
	or StartChannelIndex is ?-th channel in the channel list you ask for
	@param maxChannelNum max channel you want
	@param ppChannelList pointer to pointer to channel list

	example:

	if the channel list is : "1, 2, 5(deleted), 6, 13, 18(deleted), 19, 20"

	GetChannelList(si, xxx, FILTER(0), startLCN(2), startChIndex(0), datumOffset(0), maxNum(3), ppChList)
	will get "2, 5, 6"

	GetChannelList(si, xxx, FILTER(0), startLCN(2), startChIndex(3), datumOffset(2), maxNum(5), ppChList)
	will get "1, 2, 5, 6"

	GetChannelList(si, xxx, FILTER(deleted), startLCN(0), startChIndex(3), datumOffset(1), maxNum(5), ppChList)
	will get "6, 13, 19, 20"
*/
ErrCode SI_GetChannelList
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	unsigned int						startChannelNumber,
	unsigned int						startChannelIndex,
	unsigned int						datumChannelOffset,
	unsigned int						maxChannelNum,
	SI_CHANNEL_LIST						**ppChannelList
);

/**
	@brief free channel list returned from SI_GetChannelList()
*/
ErrCode SI_FreeChannelList
(
	SI_CHANNEL_LIST						**ppChannelList
);


ErrCode SI_GetChannelListEx
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	SI_CH_LIST_PARAM 					*param,
	SI_CHANNEL_LIST_EX					**ppChannelList
);

/**
	@brief free channel list returned from SI_GetChannelList()
*/
ErrCode SI_FreeChannelListEx
(
	SI_CHANNEL_LIST_EX						**ppChannelList
);


/**
	@brief todo
*/
ErrCode SI_GetChannelListByStr
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag
);

/**
	@brief todo
*/
ErrCode SI_GetChannelListInFrequency
(
	SI									*pSI,
	unsigned int						frequency,
	SI_CHANNEL_FLAG						filterOutFlag,
	unsigned int						maxChannelNum,
	SI_CHANNEL_LIST						**ppChannelList
);

/**
	@brief  get ch object by query different fields
*/
ErrCode SI_GetChannel_Query
(
	SI					*pSI,
	ChannelQueryItem		*qq,
	SI_CHANNEL_EXT 		*ch
);


/**
	@brief get channel count in channel list, ignoring filtered-out channels
	@param filterOutFlag to filter out the channels we don't need by
	specific flags, e.g. (SI_CHANNEL_FLAG_IS_DELETED|SI_CHANNEL_FLAG_IS_SCRAMBLED)
*/
ErrCode SI_GetChannelCount
(
	SI									*pSI,
	SI_CHANNEL_FLAG						filterOutFlag,
	unsigned int						*cnt
);

ErrCode SI_GetFrequencyCount(
	SI									*pSI,
	unsigned int						*cnt
);

ErrCode SI_GetFrequencyByIndex(
	SI									*pSI,
	unsigned int 						index,
	unsigned int						*frequency
);

/**
	@brief get channel count in frequency, ignoring filtered-out channels
	@param filterOutFlag to filter out the channels we don't need by
	specific flags, e.g. (SI_CHANNEL_FLAG_IS_DELETED|SI_CHANNEL_FLAG_IS_SCRAMBLED)
*/
ErrCode SI_GetChannelCountWithinFrequency
(
	SI									*pSI,
	unsigned int						frequency,
	SI_CHANNEL_FLAG						filterOutFlag,
	unsigned int						*cnt
);

// channel info

/**
	@brief set/unset locked flag of a channel in channel list
	@param ch you can use a local copy of ch struct to set/unset locked flag
	@param frequency if ch==NULL, it will find ch by frequency and programNumber
	@param isLocked TRUE / FALSE
*/
ErrCode SI_SetChannelIsLocked
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								isLocked
);

/**
	@brief get locked flag of a channel in channel list
	@param ch you can use a local copy of ch struct to get locked flag
	@param frequency if ch==NULL, it will find ch by frequency and programNumber
	@return pIsLocked TRUE / FALSE
*/
ErrCode SI_GetChannelIsLocked
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								*pIsLocked
);

/**
	@brief set/unset deleted flag of a channel in channel list
	@param ch you can use a local copy of ch struct to set/unset deleted flag
	@param frequency if ch==NULL, it will find ch by frequency and programNumber
	@param isDeleted TRUE / FALSE
*/
ErrCode SI_SetChannelIsDeleted
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								isDeleted
);

/**
	@brief get deleted flag of a channel in channel list
	@param ch you can use a local copy of ch struct to get deleted flag
	@param frequency if ch==NULL, it will find ch by frequency and programNumber
	@return isDeleted TRUE / FALSE
*/
ErrCode SI_GetChannelIsDeleted
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								*isDeleted
);

/**
	@brief set/unset hidden flag of a channel in channel list
	@param ch you can use a local copy of ch struct to set/unset deleted flag
	@param frequency if ch==NULL, it will find ch by frequency and programNumber
	@param isHidden TRUE / FALSE
*/
ErrCode SI_SetChannelIsHidden
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								isHidden
);

/**
	@brief get hidden flag of a channel in channel list
	@param ch you can use a local copy of ch struct to get deleted flag
	@param frequency if ch==NULL, it will find ch by frequency and programNumber
	@return isHidden TRUE / FALSE
*/
ErrCode SI_GetChannelIsHidden
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								*isHidden
);

/**
	@brief set/unset favorite flag of a channel in channel list
	@param ch you can use a local copy of ch struct to set/unset favorite flag
	@param frequency if ch==NULL, it will find ch by frequency and programNumber
	@param isFavorite TRUE / FALSE
*/
ErrCode SI_SetChannelIsFavorite
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								isFavorite
);

/**
	@brief get favorite flag of a channel in channel list
	@param ch you can use a local copy of ch struct to get favorite flag
	@param frequency if ch==NULL, it will find ch by frequency and programNumber
	@return isFavorite TRUE / FALSE
*/
ErrCode SI_GetChannelIsFavorite
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								*isFavorite
);

/**
	@brief set audio
	@param pid if you know the pid, use the pid
	@param langCode if you know the langCode but pid, set pid = 0, use langCode
	@param index if you don't know pid and langcode, just set them 0 and use index
*/
ErrCode SI_SetAudio
(
	SI									*pSI,
	unsigned short						pid,
	SI_LANG								langCode,
	unsigned short						index
);

/**
 	@brief todo
*/
ErrCode SI_EditChannelName
(
	SI									*pSI,
	SI_CHANNEL_EXT						*ch,
	unsigned int						frequency,
	unsigned short						programNumber,
	unsigned int						channelNameLen,
	unsigned char						*channelName
);

/**
	@brief sort channel list by channel number, or channel name
	@param sortingStrategy SI_CH_SORTING_STRATEGY_LCN, SI_CH_SORTING_STRATEGY_CHNAME
*/
ErrCode SI_SortChannel
(
	SI									*pSI,
	SI_CH_SORTING_STRATEGY				sortingStrategy
);

/**
	@brief move channel

	example:

	move 5 to 2 in channel list "1 2 3 4 5 6", ====> "1 5 2 3 4 6"
*/
ErrCode SI_MoveChannel
(
	SI									*pSI,
	SI_CHANNEL_EXT						*dstCh,
	unsigned int						dstChLCN,
	SI_CHANNEL_EXT						*srcCh,
	unsigned int						srcChLCN
);

/**
	@brief save channel list to fileName
	@param fileName file path
*/
ErrCode SI_SaveChannelMgr
(
	SI									*pSI,
	char								*fileName
);

/**
	@brief restore channel list from file
	@param fileName file path
*/
ErrCode SI_RestoreChannelMgr
(
	SI									*pSI,
	unsigned char						*fileName
);

/**
	@brief restore database from channel manager when boot up

	called after SI_RestoreChannelMgr()
*/
ErrCode SI_RestoreDatabase
(
	SI									*pSI
);

// EPG, Program

/**
	@brief get EPG
	@param logicalChNum logical channel number
	@param startTime time_t param
	@param endtime time_t param
	@return epg pointer to pointer to struct SI_EPG

	allow user to "query epg of ch 7, from 5:00 to 9:00, maximal 6 program infos"
*/
ErrCode SI_GetEPG
(
	SI									*pSI,
	unsigned short						logicalChNum,
	time_t								startTime,
	time_t								endTime,
	unsigned char						maxProgramNum,
	SI_EPG								**epg
);

/**
	@brief destroy epg returned from SI_GetEPG()
*/
ErrCode SI_DestroyEPG
(
	SI_EPG								**epg
);

/**
	@brief get current program (event) from following eit
	@return pProgram
*/
ErrCode SI_GetCurrentProgram
(
	SI									*pSI,
	SI_PROGRAM							*pProgram
);

/**
	@brief get next program (event) from following eit
	@return pProgram
*/
ErrCode SI_GetNextProgram
(
	SI									*pSI,
	SI_PROGRAM							*pProgram
);

ErrCode SI_GetLivePauseProgramRating
(
	SI									*pSI,
	unsigned char						*pRating
);

// Others

/**
	@brief get frequency table from file, this will be influenced by region setting
*/
ErrCode SI_GetFrequencyTableFromFile
(
	SI									*pSI
);

/**
	@brief todo
*/
ErrCode SI_GetCurrentFrequency
(
	SI									*pSI
);

/**
	@brief free frequency table returned from SI_GetFrequencyTableFromFile()
*/
ErrCode SI_FreeFrequencyTable
(
	SI									*pSI
);

/**
	@brief get localtime from tot/tdt
	@return currentTime time_t
*/
ErrCode SI_GetSourceTime
(
	SI									*pSI,
	time_t								*currentTime
);

ErrCode SI_GetLocalTime
(
	SI									*pSI,
	time_t								*currentTime
);

/**
	@brief set teletext
	@param pid if you know the pid, use the pid
	@param langCode if you know the langCode but pid, set pid = 0, use langCode
	@param index if you don't know pid and langcode, just set them 0 and use index
*/
ErrCode SI_SetTeletext
(
	SI									*pSI,
	unsigned short						pid,
	unsigned int						langCode,
	unsigned short						index
);

/**
	@brief remove teletext
*/
ErrCode SI_RemoveTeletext
(
	SI									*pSI
);

/**
	@brief set audio
	@param pid if you know the pid, use the pid
	@param langCode if you know the langCode but pid, set pid = 0, use langCode
	@param index if you don't know pid and langcode, just set them 0 and use index
*/
ErrCode SI_SetSubtitle
(
	SI									*pSI,
	unsigned short						pid,
	unsigned int						langCode,
	unsigned short 						index
);

/**
	@brief remove subtitle
*/
ErrCode SI_RemoveSubtitle
(
	SI									*pSI
);

/**
	@brief set teletext subtitle
	@param pid if you know the pid, use the pid
	@param langCode if you know the langCode but pid, set pid = 0, use langCode
	@param index if you don't know pid and langcode, just set them 0 and use index
*/
ErrCode SI_SetTTXSubtitle
(
	SI									*pSI,
	unsigned short						pid,
	unsigned int						langCode,
	unsigned short 						index
);

/**
	@brief remove teletext subtitle
*/
ErrCode SI_RemoveTTXSubtitle
(
	SI									*pSI
);

ErrCode SI_RenewChannelList
(
	SI									*pSI
);

ErrCode SI_UpdateChannelList_Append
(
	SI									*pSI
);

ErrCode si_isInSIMode
(
	SI *pSI,
	SI_MODE si_mode
);

// #ifdef FIXBUG_17588
ErrCode SI_ResetTimeOffset(SI *pSI);


/**
	@brief used by navigation filter in livepause state
*/
ErrCode SI_PVRInterface_SetChannelState(SI *si);

// call out


ErrCode SI_PVRInterface_PostEvent(SI *si, SI_MESSAGE msg);

// call in

/**
	@brief used by navigation to store packets to si queue
*/
int SI_PVRInterface_StorePackets(unsigned int si, unsigned char * buf, unsigned int bufLen);

/**
	@brief used by navigation filter in livepause state
*/
int SI_PVRInterface_StoreSiState(unsigned int si, unsigned char * pStateData, unsigned int bytes);
int SI_PVRInterface_CheckSiState(SI* si, unsigned char *pStateData, unsigned int bytes);


#if AP_CONFIG_WITH(APCFG_USE_MHEG)
    ErrCode SI_Cabot_AddSectionFilter(SI* si, void* sf, SF_HANDLE *sfHandle);
    ErrCode SI_Cabot_RemoveSectionFilter(SI* si, SF_HANDLE *sfHandle);
    ErrCode SI_Cabot_Register_PMTChangedCallback(SI* si, void *cb);
    ErrCode SI_Cabot_Deregister_PMTChangedCallback(SI* si, void *cb);
#endif//AP_CONFIG_WITH(APCFG_USE_MHEG)


//ENABLE_DVB_OTA_DOWNLOAD is used at application class level
#ifdef ENABLE_DVB_OTA_DOWNLOAD
#define SI_SSU_MAX_DL_FILE_LEN          (256)

#define SI_SSU_MAX_SW_ITEM               (5)
#define SI_SSU_MAX_REASON_LEN        (28)
#define SI_SSU_SW_VERSION_LEN          (4)
#define SI_SSU_MAX_MODEL_NAME_LEN          (256)

typedef struct  _SI_SSU_INIT_PARAM
{
	unsigned int devOui;
	unsigned short devHwModel;
	unsigned short devHwVersion;
	unsigned short devSwModel;
	unsigned short devSwVersion;
	//unsigned short modelVersion;
	//char modelName[SI_SSU_MAX_MODEL_NAME_LEN];
	char				dlFilePath[SI_SSU_MAX_DL_FILE_LEN];
} SI_SSU_INIT_PARAM;


typedef struct _SI_SSU_SCAN_PARAM
{
	int 	frequency;
	int	bandwidth;
	int 	offset;


} SI_SSU_SCAN_PARAM;

typedef enum
{
	SI_SSU_STATUS_UNDEFINED =0,
	//SI_SSU_SWINFO_SEARCH,
	SI_SSU_SWINFO_NOT_FOUND,
	SI_SSU_SWINFO_JUMP,		//check targetFreq, if not zero, need to switch to targetFreq
	SI_SSU_SWINFO_READY,
	//SI_SSU_SW_DOWNLOAD_START,
	SI_SSU_SW_VERIFY_ERROR,
	SI_SSU_SW_DOWNLOAD_ERROR,
	SI_SSU_SW_DOWNLOAD_UPDATE,
	SI_SSU_SW_VERIFY_UPDATE,	//verifying crc
	SI_SSU_SW_DOWNLOAD_OK,

} SI_SSU_STATUS_CODE;


typedef struct _SI_SSU_SCAN_INFO
{
	int targetFreq;

} SI_SSU_SCAN_INFO;

typedef struct _SI_SSU_DL_INFO
{
	int progress;

} SI_SSU_DL_INFO;


typedef struct _SI_SSU_STATUS
{
	SI_SSU_STATUS_CODE status;

	union {
		SI_SSU_DL_INFO dlInfo;
		SI_SSU_SCAN_INFO scanInfo;
	};

} SI_SSU_STATUS;

//SI_SSU_PROFILE must be the same with SSU_PROFILE of ssuDtr.h
//only support simple now
typedef enum
{
	SI_SSU_PROPRIETARY_PROFILE      = 0x0000,
	SI_SSU_SIMPLE_PROFILE           = 0x0001,
	SI_SSU_ENHANCED_PROFILE         = 0x0002,
	SI_SSU_ENHANCED_PROFILE_WITH_RC = 0x0003,
	SI_SSU_UNKNOWN_PROFILE          = 0x0004
} SI_SSU_PROFILE ;

typedef struct _SI_SSU_SW_INFO
{
	int			frequency;
	int			bandwidth;
	int 			freq_offset;
	int			channelNumber;
	SI_SSU_PROFILE		profile;
	unsigned short	programNum;
	unsigned short	ssuEsPid;
	unsigned int          diiTransactionId;
	unsigned char		version[SI_SSU_SW_VERSION_LEN];
	unsigned char		updateReasonIso636[3];
	unsigned char		updateReasonText[SI_SSU_MAX_REASON_LEN];
	//for pvr
	//char				dlFilePath[SI_SSU_MAX_DL_FILE_LEN];
} SI_SSU_SW_INFO;


typedef struct _SI_SSU_SW_LIST
{
	unsigned int		count;
	SI_SSU_SW_INFO	list[SI_SSU_MAX_SW_ITEM];
} SI_SSU_SW_LIST;

ErrCode SI_SSU_Init(SI *pSI, SI_SSU_INIT_PARAM* param);
ErrCode SI_SSU_DeInit(SI *pSI);
ErrCode SI_SSU_Scan(SI *pSI,  SI_SSU_SCAN_PARAM* param);
ErrCode SI_SSU_GetSwList(SI_SSU_SW_LIST* swList);
ErrCode SI_SSU_Download(SI *pSI, SI_SSU_SW_INFO* sw_info);


#endif//ENABLE_DVB_OTA_DOWNLOAD

// ifdef FEATURE_002
ErrCode SI_GetPDDSignaling(SI *pSI, SI_CHANNEL_EXT *ch,
							unsigned char privateDataSpecifier[4],
							unsigned char customerPrivateDescriptorTag,
							unsigned char pdd[258]);

#ifdef DVB_S	// shanghai-dvbs
typedef enum
{
	SI_DVBS_SCAN_NONE					= 0,
	SI_DVBS_SCAN_ALL					= 0x01,		// scan for all satellites
	SI_DVBS_SCAN_SAT					= 0x02,		// scan for all transponders with indicated satellite
	SI_DVBS_SCAN_SAT_TP					= 0x03,		// scan for indicated satellite and transponder
} SI_DVBS_SCAN_TYPE;

#define SI_DVBS_INVALID_VALUE			0xFFFFFFFF

typedef struct
{
	SI_DVBS_SCAN_TYPE	scan_type;			// scan type
	unsigned int		frequency;			// scan frequency, this value use to distinguish transponder
	unsigned int		bandwidth;			// scan bandwidth, this value use to distinguish satellites
	int					channel_count;		// found channel count
	int					scan_flag;
} SI_DVBS_SCAN_INFO;

ErrCode SI_DvbSxScanInit
(
	SI					*pSI,
	SI_DVBS_SCAN_TYPE	scan_type,
	unsigned int		frequency,
	unsigned int		bandwidth
);

ErrCode SI_DvbSxScanFoundChannelCount
(
	SI					*pSI,
	int					*channel_count
);

#endif // end of DVB_S	// shanghai-dvbs

ErrCode SI_Set_EPG_LanguagePreference(SI *pSI, SI_LANG preference);

char* SI_GetPMTSection(SI *pSI, int *pLen);

/**
	@brief Set the disable LCNs mode for ITA/1287: Test that the receiver correctly orders services when LCN detection is disabled 
	@param bValue ON/OFF
*/
ErrCode SI_SetDisableLCNs
(
	SI									*pSI,
	BOOL								bValue
);
ErrCode SI_SetUserConfirmForChAddDelete	// bug 26588, Evora iSuite v2.3, GBR/394
(
	SI									*pSI,
	BOOL								bValue
);

void SI_SetChAlterMode	// bug 26588, Evora iSuite v2.3, GBR/394
(
	SI* pSI, 
	SI_FLAG chAlterMode
);

/**
	@brief get current network name from following nit
	@return pNetwork
*/
ErrCode SI_GetCurrentNetwork
(
	SI									*pSI,
	SI_NETWORK							*pNetwork
);

/**
	@brief Get DTCP descriptor from PMT specified by programNumber. programNumber is the programNumber of SI_CHANNEL_EXT.
	@return SI_GetDTCP() will fill all the fields of pDtcp which is malloced by caller.
*/
ErrCode SI_GetDTCP(	// bug 28532
	SI *pSI, 
	unsigned short programNumber, 
	SI_DTCP *pDtcp
);

#endif	//_SI4_API_H_



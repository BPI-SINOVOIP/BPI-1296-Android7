
#ifndef _SI_CH_H
#define _SI_CH_H

typedef enum _SI_CHANNEL_MERGE_STRATEGY
{
   SI_CH_MERGE_LIST_APPEND 	= 0x00000001,	// append new services to list tail
   SI_CH_MERGE_LIST_RESORT		= 0x00000002,	// append new services to list tail &  resort all
   SI_CH_MERGE_DUP_DEL		= 0x00000004, 	//delete duplicate one (new one)
   SI_CH_MERGE_DUP_REASSIGN	= 0x00000008, 	//assign a new lcn(or userNum) to this service
   										//DUP_DEL, DUP_REASSIGN may use same flag..
} SI_CHANNEL_MERGE_STRATEGY;

struct SI_CHANNEL_MGR
{
	unsigned int						channelNum;
	SI_CHANNEL_EXT						*chList;

	SI_CHANNEL_EXT						*currentChannel;
	SI_CHANNEL_EXT						*historyChannel;

	SI_CH_SORTING_STRATEGY				sortingStrategy;
	unsigned int 							lastLCN;		//largest lcn value
	unsigned int 						lastTvLCN;		// fix bug 25547, Evora iSuite v2.2, SWE/1364
	unsigned int						lastRadioLCN;	// fix bug 25547, Evora iSuite v2.2, SWE/1364
	unsigned int 							countryOtherLCN; //lcn for other country
	SI_CHANNEL_MERGE_STRATEGY			mergeStrategy;
	SI_SERVICE_TYPE						serviceType;
	unsigned int 							isUseIrregularLCN;//
	unsigned int 							iIrregularLCN;//
	
	SI_LOGICAL_CH_NUM					prevUserChNum; //for check new<->disappeared ch
		
	pthread_mutex_t						mutex;
};


#define SI_CHANNEL_MAX_CH_NUM_PER_TS 256
typedef struct _SI_CHANNEL_COLLECTION_IN_TS
{
	unsigned int							channelNum;
	struct SI_CHANNEL_EXT*				channels[SI_CHANNEL_MAX_CH_NUM_PER_TS];
} SI_CHANNEL_COLLECTION_IN_TS;



typedef enum _SI_CHANNEL_DUP_STATUS
{
	SI_CH_DUP_NONE,		//not a dup service
	SI_CH_DUP_WHOLE,	//SI_CHANEL_DUP_LCN + SI_CHANEL_DUP_DVB
	SI_CH_DUP_DVB,		// (onid, tsid, sid) is same, but lcn is different
	SI_CH_DUP_LCN,			//only lcn is same
	SI_CH_DUP_LCN_LOW,	// fix bug 25646, SWE/451
} SI_CHANNEL_DUP_STATUS;

//------------------------------------------------------------------------------
//
//	Include Files
//
//------------------------------------------------------------------------------
#include <system/si4/channel/SI_ChannelMacros.h>


//------------------------------------------------------------------------------
//
//	Functions
//
//------------------------------------------------------------------------------
ErrCode SI_Channel_New
(
	SI_CHANNEL_MGR						**ppChannelMgr
);

ErrCode SI_Channel_Destroy
(
	SI_CHANNEL_MGR						**ppChannelMgr
);

ErrCode SI_Channel_Reset
(
	SI_CHANNEL_MGR						*pChannelMgr
);

ErrCode SI_Channel_Lock
(
	SI_CHANNEL_MGR						*pChannelMgr
);

ErrCode SI_Channel_Unlock
(
	SI_CHANNEL_MGR						*pChannelMgr
);

ErrCode SI_Channel_Update_LastLCN
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_DATABASE							*pDatabase,
	SI_SPEC 								spec,
	unsigned int 							updateNumbers
);


ErrCode SI_Channel_SetupChList
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_DATABASE							*pDatabase,
	SI_SPEC								spec
);

ErrCode SI_Channel_UpdatePartialChList
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_DATABASE							*pDatabase,
	unsigned int						frequency
);

ErrCode SI_Channel_UpdatePartialChListInCurTS
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_DATABASE							*pDatabase,
	unsigned int						frequency,
	SI_TID								tid
);

ErrCode SI_Channel_UpdateChList_Append
(
	SI_CHANNEL_MGR					*pChannelMgr,
	SI_DATABASE						*pDatabase,
	struct SI_DATABASE_TS_NODE 			*ts
);


ErrCode SI_Channel_UpdatePartialChListInVC
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_DATABASE							*pDatabase,
	unsigned int						frequency,
	unsigned short						programNumber,
	SI_TID								tid,
	BOOL								allowAddingChannel
);

ErrCode SI_Channel_Sort
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_CH_SORTING_STRATEGY				sortingStrategy
);

ErrCode SI_Channel_MoveChannel
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_CHANNEL_EXT						*dstCh,
	unsigned int						dstChLCN,
	SI_CHANNEL_EXT						*srcCh,
	unsigned int						srcChLCN
);

ErrCode SI_Channel_GetSortingStrategy
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_CH_SORTING_STRATEGY				*sortingStrategy
);

ErrCode SI_Channel_GetSetupServiceType
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_SERVICE_TYPE						*serviceType
);

ErrCode SI_Channel_GetChannelList
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_CHANNEL_FLAG						filterOutFlag,
	unsigned int						startLogicalChNum,
	unsigned int						startChannelIndex,
	unsigned int						datumChannelOffset,
	unsigned int						maxChannelNum,
	SI_CHANNEL_LIST						**ppChannelList
);

ErrCode SI_Channel_GetChannelListWithinFreq
(
	SI_CHANNEL_MGR						*pChannelMgr,
	unsigned int						frequency,
	SI_CHANNEL_FLAG						filterOutFlag,
	unsigned int						maxChannelNum,
	SI_CHANNEL_LIST						**ppChannelList
);

ErrCode SI_Channel_GetChannel_By_Service
(
	SI_CHANNEL_MGR					*pChannelMgr,
	unsigned short						onid,
	unsigned short						tsid,
	unsigned short 					serviceID,
	SI_CHANNEL_EXT					*targetCh
);

ErrCode SI_Channel_GetChannel_By_LCN
(
	SI									*pSI,
	unsigned short						logicalChNum,
	SI_CHANNEL_EXT						*ch
);

ErrCode SI_Channel_GetChannel_By_Query
(
	SI_CHANNEL_MGR				*pChannelMgr,
	ChannelQueryItem				*qq,
	SI_CHANNEL_EXT 				*targetCh
);

ErrCode SI_Channel_FreeChannelList
(
	SI_CHANNEL_LIST						**ppChannelList
);

ErrCode SI_Channel_SetChannelFlag
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_CHANNEL_EXT						*pChannel,
	unsigned int							logicalChNum,
	unsigned int							frequency,
	unsigned int							programNumber,
	SI_CHANNEL_FLAG						setFlag,
	SI_CHANNEL_FLAG						clrFlag
);

ErrCode SI_Channel_Convert
(
	unsigned int				srcChVer,
	unsigned char						*srcCh,
	SI_CHANNEL_EXT				*retCh
);


ErrCode SI_Channel_Update_Global_User_ONID
(
	unsigned short 						userONID
);

// si if for fix bug 25491, Evora iSuite v2.2
ErrCode si_channel_util_isShow(SI *si, SI_SERVICE_TYPE serv, unsigned int vCount, unsigned int aCount, SI_SPEC spec, unsigned short lcn);


ErrCode SI_Channel_GetChannelList_ExtFull
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_CHANNEL_FLAG						filterOutFlag,
	SI_CH_LIST_PARAM 					*param,
	SI_CHANNEL_LIST_EX					**ppChannelList
);


#if AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)
ErrCode SI_Channel_GetChannelList_Ext
(
	SI_CHANNEL_MGR						*pChannelMgr,
	SI_CHANNEL_FLAG						filterOutFlag,
	SI_CHANNEL_LIST_EXT						**ppChannelList
);

ErrCode SI_Channel_FreeChannelList_Ext
(
	SI_CHANNEL_LIST_EXT					**ppChannelList
);


#endif//AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)


#ifdef DVB_S	// shanghai-dvbs
ErrCode SI_DvbSx_Channel_Remove(
	SI_CHANNEL_MGR		*pChannelMgr,
	unsigned int		frequency,
	unsigned int		bandwidth
);

ErrCode SI_DvbSx_Channel_UpdatePartialChList(
	SI_CHANNEL_MGR		*pChannelMgr,
	SI_DATABASE			*pDatabase,
	unsigned int		frequency,
	unsigned int		bandwidth
);
#endif // DVB_S	// shanghai-dvbs

#endif//_SI_CH_H


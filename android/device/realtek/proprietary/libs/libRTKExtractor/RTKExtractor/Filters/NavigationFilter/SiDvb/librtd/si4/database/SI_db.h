
#ifndef SI_DB_H
#define SI_DB_H

#include "SITypes_local.h"

struct SI_DATABASE_VC_NODE
{
	SI_DATABASE_VC_NODE					*next;
	SI_DATABASE_VC_NODE					*previous;

//	ss: for future use
//	unsigned int						onid;

	unsigned short						programNumber;

	unsigned char						lastScheduleEITTID;

	//SI_SERVICE_TYPE						serviceType;

	BOOL								gotPMT;			//always set it if receive pmt(don't care it's version is same or not)
	BOOL								gotEITPresent;
	BOOL								gotEITFollowing;

	BOOL								newVersionPMT;

	SI_DATABASE_SECTION					*pmt;
	SI_DATABASE_SECTION					*eitPresent;
	SI_DATABASE_SECTION					*eitFollowing;

	BOOL								newVC;	//set to 1 if it's new in PAT

	SI_DATABASE_SCHEDULE_EIT_S			*scheduleEITss[SI_DB_SCHEDULEEIT_NUM];
};


struct SI_DATABASE_TS_NODE
{
	SI_DATABASE_TS_NODE					*next;
	SI_DATABASE_TS_NODE					*previous;

//	ss: for future use
	unsigned int						onid;
	unsigned int						quality;

	unsigned int						frequency;
	int									offset;
	unsigned int						bandwidth;
	unsigned short						tsID;
	unsigned int						tsIndex; 	// 1. 0 means this ts node is invalid(used for all si project)
												// 2. index of this ts node, for quickly access this node
												//it can replace frequency(for AP) in future (only used in gemStar now)

	unsigned int						vcNum;
	SI_DATABASE_VC_NODE					*vcList;
	SI_DATABASE_VC_NODE					*currentVC;

	BOOL								gotPAT;
	BOOL								gotNIT;
	BOOL								gotSDT;
	BOOL								gotTDT;
	BOOL								gotTOT;

	SI_DATABASE_SECTION					*pat;

#ifdef ADD_MULTISEC
	#define MAX_SECTION_NUM		256
	SI_DATABASE_SECTION					*nit[MAX_SECTION_NUM];
#else
	SI_DATABASE_SECTION					*nit;
#endif

#ifdef ADD_MULTISEC
	SI_DATABASE_SECTION					*sdt[MAX_SECTION_NUM];
#else
	SI_DATABASE_SECTION					*sdt;
#endif
	SI_DATABASE_SECTION					*tdt;
	SI_DATABASE_SECTION					*tot;
};

struct SI_DATABASE
{

	SI_DATABASE_TS_NODE					*tsList;
	SI_DATABASE_TS_NODE					*currentTS;
	struct SI							*si;
	pthread_mutex_t						mutex;

	unsigned int								tsCount;

	#if AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)

	LOGICAL_FREQ_INDEX freqIndex;

	#endif//

};


//------------------------------------------------------------------------------
//
//	Include Files
//
//------------------------------------------------------------------------------

#include <system/si4/database/SI_DatabaseMacros.h>


#define SI_DB_INVALID_TS_ONID (0xffff)

//------------------------------------------------------------------------------
//
//	Functions
//
//------------------------------------------------------------------------------
//-----------------------------------------------------
// Control
//-----------------------------------------------------
ErrCode SI_Database_New
(
	SI_DATABASE							**ppDatabase,
	SI									*si
);

ErrCode SI_Database_Destroy
(
	SI_DATABASE							**ppDatabase
);

ErrCode SI_Database_Clean
(
	SI_DATABASE							*ppDatabase
);


ErrCode SI_Database_Reset
(
	SI_DATABASE							*pDatabase
);


ErrCode SI_Database_AddTSNode
(
	SI_DATABASE							*pDatabase,
	unsigned int						frequency,
	int									offset,
	unsigned int						bandwidth,
	unsigned int						quality,
	SI_DATABASE_TS_NODE					**ppTS
);

ErrCode SI_Database_AddVCNode
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	unsigned short						programNumber
);

ErrCode SI_Database_SetCurrentTSNode
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency
);

ErrCode SI_Database_GetCurrentTSNodeInfo
(
	SI_DATABASE							*pDatabase,
	unsigned int						*frequency,
	unsigned short						*tsID,
	unsigned int						*vcNum
);

ErrCode SI_Database_SetCurrentVCNode
(
	SI_DATABASE							*pDatabase,
	unsigned int						frequency,
	unsigned short						programNumber
);

ErrCode SI_Database_GetCurrentVCNodeInfo
(
	SI_DATABASE							*pDatabase,
	unsigned int						*pFrequency,
	unsigned short						*pTSID,
	unsigned short						*pProgramNumber
);

ErrCode SI_Database_InsertSection
(
	SI_DATABASE							*pDatabase,
	unsigned int						sectionLength,
	unsigned char						*sectionRawBuffer,
	unsigned int 						pid
);

ErrCode SI_Database_RemoveSection
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	SI_DATABASE_VC_NODE					*vc,
	unsigned short						programNumber,
	SI_TID								tid,
	unsigned int						eitPresentFollowing
);

ErrCode SI_Database_SetSectionReceiveFlag
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	SI_DATABASE_VC_NODE					*vc,
	unsigned short						programNumber,
	SI_TID								tid,
	unsigned int						eitPresentFollowing,
	BOOL								flag
);

ErrCode SI_Database_GetSectionReceiveFlag
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	SI_DATABASE_VC_NODE					*vc,
	unsigned short						programNumber,
	SI_TID								tid,
	unsigned int						eitPresentFollowing,
	BOOL								*flag
);

ErrCode SI_Database_GetSectionAddr
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	SI_DATABASE_VC_NODE					*vc,
	unsigned short						programNumber,
	SI_TID								tid,
	unsigned int						eitPresentFollowing,
	SI_DATABASE_SECTION					**ppSectionAddr
#ifdef ADD_MULTISEC
	,int secNum
#endif
);

ErrCode SI_Database_GetSectionCRC
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	SI_DATABASE_VC_NODE					*vc,
	unsigned short						programNumber,
	SI_TID								tid,
	unsigned int						eitPresentFollowing,
	unsigned int						*crc
);

ErrCode SI_Database_GetSectionVersion
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	SI_DATABASE_VC_NODE					*vc,
	unsigned short						programNumber,
	SI_TID								tid,
	unsigned int						eitPresentFollowing,
	unsigned char						*version
);

ErrCode SI_Database_GetDescriptorAddrBySection
(
	SI_DATABASE_SECTION					*pSectionAddr,
	SI_DESCRIPTOR_TAG					tag,
	BOOL								specifyLoop,
	unsigned int						ref,
	unsigned char						**ppDescriptorAddr
);

ErrCode SI_Database_GetDescriptorAddrByAddr
(
	unsigned char						*addr,
	unsigned int						descsLen,
	SI_DESCRIPTOR_TAG					tag,
	unsigned char						**ppDescriptorAddr
);

//-----------------------------------------------------
// For Si Agent
//-----------------------------------------------------
ErrCode SI_Database_GetProgramInfoCollection
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	SI_DATABASE_PROGRAM_INFO_COLLECTION		**ppProgramInfos
);

ErrCode SI_Database_FreeProgramInfoCollection
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_PROGRAM_INFO_COLLECTION		**ppProgramInfos
);

ErrCode SI_Database_CheckProgramInfoVaild
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	unsigned short						programNumber,
	unsigned short						pid,
	BOOL								*isValid
);

ErrCode SI_Database_CheckProgramNumberVaild
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	unsigned short						programNumber,
	BOOL								*isValid
);

ErrCode SI_Database_CheckTSNeedToDoScan
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	BOOL								*need
);

//-----------------------------------------------------
// Query
//-----------------------------------------------------

#if 0
// serviceType is not set currently
ErrCode SI_Database_IsUserService
(
	SI_DATABASE						*pDatabase,
	SI_DATABASE_TS_NODE			*ts,
	SI_DATABASE_VC_NODE			*vc,
	SI_SERVICE_TYPE*				serviceType
);


ErrCode SI_Database_GetCurrentEvent
(
	SI_DATABASE							*pDatabase
);

ErrCode SI_Database_GetNextEvent
(
	SI_DATABASE							*pDatabase
);

ErrCode SI_Database_DestroyEvent
(
	SI_DATABASE							*pDatabase
);

ErrCode SI_Database_GetTimeDurationOfCurrentEvent
(
	SI_DATABASE							*pDatabase
);

ErrCode SI_Database_GetTimeDurationOfNextEvent
(
	SI_DATABASE							*pDatabase
);

ErrCode SI_Database_GetEventsByParameters
(
	SI_DATABASE							*pDatabase
);

ErrCode SI_Database_GetProgramNumbersByFrequency
(
	SI_DATABASE							*pDatabase
);

ErrCode SI_Database_GetPMTPidsInPAT
(
	SI_DATABASE							*pDatabase
);

#endif//0

ErrCode SI_Database_GetChannelInfo
(
	// inputs
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int						frequency,
	SI_DATABASE_VC_NODE					*vc,
	unsigned short						programNumber,

	// outputs
	unsigned short						*pLogicalChNum,
	unsigned short						*pOnid,

	unsigned int						*pOriChannelNameLen,
	unsigned char						*pOriChannelNameBufAddr,

	unsigned short						*pPcrPid,

	SI_CHANNEL_SUBTITLE_LIST			*pSubtitlesList,
	SI_CHANNEL_TT_LIST					*pTeletextsList,
	SI_CHANNEL_TT_LIST					*pTTXSubtitlesList,

	unsigned char						*pVideoNum,
	SI_CHANNEL_VIDEO_INFO				*pVideosBufAddr,

	SI_CHANNEL_AUDIO_LIST				*pAudiosList,

	SI_SERVICE_TYPE						*pServiceType,
	SI_CHANNEL_FLAG						*pChannelFlags
);

ErrCode SI_Database_GetProgram
(
	SI_DATABASE							*pDatabase,
	unsigned int						eitPresentFollowing,
	SI_SPEC								spec,
	SI_PROGRAM							*pProgram
);

ErrCode SI_Database_GetCurrentRating
(
	SI_DATABASE							*pDatabase,
	unsigned char						*pRating
);

ErrCode SI_Database_GetEPG
(
	SI_DATABASE							*pDatabase,
	unsigned int						frequency,
	unsigned short						programNumber,
	time_t								startTime,
	time_t								endTime,
	unsigned char						maxProgramNum,
	SI_SPEC								spec,
	SI_EPG								**ppEpg
);

ErrCode SI_Database_DestroyEPG
(
	SI_EPG								**ppEpg
);

ErrCode SI_Database_GetTimeInSI
(
	SI_DATABASE							*pDatabase,
	time_t								*siTime
);

ErrCode SI_Database_ResetTSNode
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int 						frequency
);

ErrCode SI_Database_CleanTSNode
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned int 						frequency
);

ErrCode SI_Database_CleanVCNode
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_VC_NODE					*vc,
	unsigned int						frequency,
	unsigned short						programNumber
);

ErrCode SI_Database_RemoveAllEITInVC
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_VC_NODE					*vc,
	unsigned int						frequency,
	unsigned short						programNumber
);


ErrCode SI_Database_Get_Freq_BY_Network
(SI_DATABASE		*pDatabase,
  unsigned short  		onID,
  unsigned short		tsID,
  unsigned int			*freq
);

ErrCode SI_Database_Get_TSID_BY_Freq
( SI_DATABASE		*pDatabase,
  unsigned int			freq,
  unsigned short		*tsID
);

//util function for inner sidb(SI_DatabaseSection)
ErrCode si_database_get_onid
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned short						*pOnid
//#ifdef FIX_DTV_LABS_HK_ISSUE_21 // fix bug 25485 (ESP/4) , 25487 (ESP/1355), 25489 (SWD/1664)
	,unsigned short						*pNid
//#endif
);

unsigned long si_database_check_crc(unsigned char *data, int len);

ErrCode SI_Database_GetLPCMInfo
(
	SI_DATABASE					*pDatabase,
	int 							target_idx,
	void 						*si_lpcm_info
);



ErrCode SI_DatabaseQuery_GetDataBroadCastIDCollection
(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned short 					programNumber,
	SSU_ES_INFO_T					**ppEsInfos
);



ErrCode SI_DatabaseQuery_FreeDataBroadCastIDCollection
(
	SI_DATABASE						*pDatabase,
	SSU_ES_INFO_T					**ppEsInfos
);

// No sample stream to test currently, just commit code for usage later
ErrCode si_database_get_T2_delivery_sys_descriptor(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts,
	unsigned char						*plp_id,
	unsigned short						*T2_system_id
);

// No sample stream to test currently, just commit code for usage later
ErrCode si_database_get_target_region_descriptor(
	SI_DATABASE							*pDatabase,
	SI_DATABASE_TS_NODE					*ts
);

ErrCode SI_Database_GetNetwork
(
	SI_DATABASE					*pDatabase,
	SI_NETWORK					*pNetwork
);

#endif//SI_DB_H


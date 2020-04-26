
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siFunctionWrapper.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/15
 *
 *=======================================================================*/


#ifndef _SIFUNCTIONWRAPPER_H_
#define _SIFUNCTIONWRAPPER_H_



//#include <rtd_types.h>
//#include <system/sidb2/siInterface/siInterfaceHeader.h>

	#include "siAgentObject.h"

//#include <tp/demux_common.h>
#include <system/si4/SITypes.h>


#if 0
void SiAgtFW_CreateSectionBuffer( X_SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid );
void SiAgtFW_DeleteSectionBuffer( X_SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid );

void SiAgtFW_EnablePidFilter( X_SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid );
void SiAgtFW_DisablePidFilter( X_SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid );

BOOL SiAgtFW_CheckPatExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckAs4DayEitExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned char tid );
/*
BOOL SiAgtFW_CheckPmtExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
BOOL SiAgtFW_CheckTotExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckTdtExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckActualSdtExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckActualNitExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckAllBatExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckApEitExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
BOOL SiAgtFW_CheckAfEitExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
*/

BOOL SiAgtFW_CheckPatReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckPmtReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
BOOL SiAgtFW_CheckTotReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckTdtReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckActualSdtReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckActualNitReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckAllBatReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckApEitReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
BOOL SiAgtFW_CheckAfEitReceive( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

void SiAgtFW_ClearFlagReceivePat( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceivePmt( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_ClearFlagReceiveTot( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveTdt( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveActualSdt( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveActualNit( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveAllBat( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveApEit( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_ClearFlagReceiveAfEit( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

unsigned int SiAgtFW_GetPatCrc( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned int SiAgtFW_GetPmtCrc( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned int SiAgtFW_GetActualNitCrc( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned int SiAgtFW_GetActualSdtCrc( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned int SiAgtFW_GetApEitCrc( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned int SiAgtFW_GetAfEitCrc( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

void SiAgtFW_CleanTsNode( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelAllBat( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelTot( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelTdt( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelANit( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelASdt( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelApEit( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_DelAfEit( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_DelAs4DayEit( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned char tid );
void SiAgtFW_DelPmt( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_DelPat( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelAllVc( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

unsigned char SiAgtFW_GetANitVersion( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned char SiAgtFW_GetASdtVersion( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned char SiAgtFW_GetApEitVersion( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned char SiAgtFW_GetAfEitVersion( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned char SiAgtFW_GetPatVersion( X_SIAGENT_OBJECT_P pSiAgentObj,TABLE_TREE_TS_LAYER_P tsAddr );
unsigned char SiAgtFW_GetPmtVersion( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

PROGRAMS_INFO_P SiAgtFW_GetProgramsInfo( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_FreeProgramsInfo( X_SIAGENT_OBJECT_P pSiAgentObj, PROGRAMS_INFO_P* ppProgramsInfo );

BOOL SiAgtFW_IsAsEitTheLatestVersion( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

BOOL SiAgtFW_CheckAnyAsEitExist( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

unsigned short SiAgtFW_GetPmtPid( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned short SiAgtFW_GetTsidFromPat( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

BOOL SiAgtFW_CheckProgramInfoValid( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber, unsigned short pid );
BOOL SiAgtFW_CheckProgramNumberValid( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

BOOL SiAgtFW_CheckTsNeedScanCh( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

void SiAgtFW_MaintainDatabase( X_SIAGENT_OBJECT_P pSiAgentObj );

TABLE_TREE_TS_LAYER_P SiAgtFW_CreateTsNode( X_SIAGENT_OBJECT_P pSiAgentObj, unsigned int  frequency );
void SiAgtFW_ResetBeanPacker( X_SIAGENT_OBJECT_P pSiAgentObj );
void SiAgtFW_SetCurrentTSNode( X_SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );


#else
void SiAgtFW_CreateSectionBuffer( SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid );
void SiAgtFW_DeleteSectionBuffer( SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid );

void SiAgtFW_EnablePidFilter( SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid );
void SiAgtFW_DisablePidFilter( SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid );

BOOL SiAgtFW_CheckPatExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckAs4DayEitExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned char tid );
/*
BOOL SiAgtFW_CheckPmtExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
BOOL SiAgtFW_CheckTotExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckTdtExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckActualSdtExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckActualNitExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckAllBatExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckApEitExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
BOOL SiAgtFW_CheckAfEitExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
*/

BOOL SiAgtFW_CheckPatReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckPmtReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
BOOL SiAgtFW_CheckTotReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckTdtReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckActualSdtReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckActualNitReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckAllBatReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
BOOL SiAgtFW_CheckApEitReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
BOOL SiAgtFW_CheckAfEitReceive( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

void SiAgtFW_ClearFlagReceivePat( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceivePmt( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_ClearFlagReceiveTot( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveTdt( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveActualSdt( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveActualNit( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveAllBat( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_ClearFlagReceiveApEit( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_ClearFlagReceiveAfEit( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

unsigned int SiAgtFW_GetPatCrc( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned int SiAgtFW_GetPmtCrc( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned int SiAgtFW_GetActualNitCrc( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned int SiAgtFW_GetActualSdtCrc( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned int SiAgtFW_GetApEitCrc( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned int SiAgtFW_GetAfEitCrc( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

void SiAgtFW_CleanTsNode( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelAllBat( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelTot( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelTdt( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelANit( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelASdt( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelApEit( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_DelAfEit( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_DelAs4DayEit( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned char tid );
void SiAgtFW_DelPmt( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
void SiAgtFW_DelPat( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_DelAllVc( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

unsigned char SiAgtFW_GetANitVersion( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned char SiAgtFW_GetASdtVersion( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
unsigned char SiAgtFW_GetApEitVersion( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned char SiAgtFW_GetAfEitVersion( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned char SiAgtFW_GetPatVersion( SIAGENT_OBJECT_P pSiAgentObj,TABLE_TREE_TS_LAYER_P tsAddr );
unsigned char SiAgtFW_GetPmtVersion( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

PROGRAMS_INFO_P SiAgtFW_GetProgramsInfo( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
void SiAgtFW_FreeProgramsInfo( SIAGENT_OBJECT_P pSiAgentObj, PROGRAMS_INFO_P* ppProgramsInfo );

SSU_ES_INFO_P SiAgtFW_GetDataBroadCastIDInfo( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber);
void SiAgtFW_FreeDataBroadCastIDInfo( SIAGENT_OBJECT_P pSiAgentObj, SSU_ES_INFO_T **ppSsuInfo );


BOOL SiAgtFW_IsAsEitTheLatestVersion( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

BOOL SiAgtFW_CheckAnyAsEitExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

unsigned short SiAgtFW_GetPmtPid( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
unsigned short SiAgtFW_GetTsidFromPat( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

BOOL SiAgtFW_CheckProgramInfoValid( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber, unsigned short pid );
BOOL SiAgtFW_CheckProgramNumberValid( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

BOOL SiAgtFW_CheckTsNeedScanCh( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

void SiAgtFW_MaintainDatabase( SIAGENT_OBJECT_P pSiAgentObj );

TABLE_TREE_TS_LAYER_P SiAgtFW_CreateTsNode( SIAGENT_OBJECT_P pSiAgentObj, unsigned int  frequency );
void SiAgtFW_ResetBeanPacker( SIAGENT_OBJECT_P pSiAgentObj );
void SiAgtFW_SetCurrentTSNode( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

unsigned long SiAgtFW_CheckSectionCrc( unsigned char* data,  int len);
#endif//0


#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE

void SsuDTR_EnableReceiveDsi(void);
void SsuDTR_DisableReceiveDsi(void);
void SsuDTR_EnableReceiveDii(void);
void SsuDTR_DisableReceiveDii(void);
void SsuDTR_EnableReceiveDdb(void);
void SsuDTR_DisableReceiveDdb(void);
void SsuDTR_EnableReceiveUnt(void);
void SsuDTR_DisableReceiveUnt(void);

bool SsuDTR_CheckSpDsiExist(void);
bool SsuDTR_CheckEpUntExist(void);
bool SsuDTR_SpHaveCompatiblePlatform(void);
bool SsuDTR_SpHaveCompatiblePlatformDii(void);
bool SsuDTR_SpHaveCompatiblePlatformDdb(void);
bool SsuDTR_EpHaveCompatiblePlatform(void);

void SsuDTR_SetDtrInfoReadyFlag(void);
void SsuDTR_ClearDtrInfo(void);
void SsuDTR_InitSsuDtr( TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber, UINT16 ssuEsPid, SSU_PROFILE profile );

SSU_DII_TRANSACTION_ID_INFO_P SsuDTR_SpGetCompatiblePlatformDiiInfo(void);
SSU_DTR_DDB_INFO_P SsuDTR_SpGetCompatiblePlatformDdbInfo(void);

bool SsuDTR_CheckSpDiiExist( UINT32 diiTransactionId );
bool SsuDTR_CheckSpDdbExist( UINT32 ddbDownloadId );
bool SsuDCR_CheckDsiExist(void);
bool SsuDCR_CheckDiiExist(void);
bool SsuDCR_CheckAllBlocksExist(void);

SSU_ES_INFO_P SsuDTR_GenVcSsuEsList(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber );
void SsuDTR_FreeVcSsuEsList(SIAGENT_OBJECT_P pSiAgentObj, SSU_ES_INFO_P pEsInfo);

void SsuDCR_ClearSsuDcr(void);
int SsuDCR_InitSsuDcr(SIAGENT_OBJECT_P pSiAgentObj, SSU_SW_LOCATION_P pSsuSwLocation );
bool SsuDCR_CheckSsuEsPidValid(void);

void SsuDCR_EnableReceiveDsi(void) ;
void SsuDCR_DisableReceiveDsi(void);
void SsuDCR_EnableReceiveDii(void) ;
void SsuDCR_DisableReceiveDii(void);
void SsuDCR_EnableReceiveDdb(void) ;
void SsuDCR_DisableReceiveDdb(void);
void SsuDCR_FinishFileDownload(void);

UINT16 SsuDCR_GetSsuEsPid(void);

bool SsuDCR_ConfirmPlatformCompatibility( UINT32 diiTransactionId ) ;
UINT32 SsuDCR_GetDiiTransactionId(void);

UINT8 SsuDCR_GetDiiVersion(void);
UINT32 SsuDCR_GetDiiCrc(void);

UINT8 SsuDCR_GetModuleIdMsb(void);
UINT32 SsuDCR_GetDdbCountForTimeoutDetect(void);

UINT32 SsuDCR_GetCollectDdbProgressPercent(void);
UINT32 SsuDCR_GetModuleNumber(void);
UINT32 SsuDCR_GetBlockNumber( UINT32 moduleNumber );
void SsuDCR_ResetCrc32( UINT32 *pTempCrc32Result );
bool SsuDCR_CheckModuleHasCrc( UINT32 moduleNumber );
UINT32 SsuDCR_GetModuleCrc( UINT32 moduleNumber );
void SsuDCR_UpdateModuleCrcFlag( UINT32 moduleNumber, bool flag );
void SsuDCR_UpdateModulePassCheckFlag( UINT32 moduleNumber, bool flag );
void SsuDCR_UpdateCheckedBlockNum( UINT32 moduleNumber, UINT32 checkedBlockNum );
void SsuDCR_UpdateGroupPassCheckFlag( bool flag );
UINT8* SsuDCR_GetBlockRawData( UINT32 moduleNumber, UINT32 blockNumber );
UINT32 SsuDCR_GetBlockSize( UINT32 moduleNumber, UINT32 blockNumber );

BLOCK_CHECKED_STATE SsuDCR_CheckBlockCrc( UINT8* blockRawData, UINT32 blockSize, UINT32 byteNumber, UINT32* pBlockDataCheckedLocation, UINT32* pTempCrc32Result );
bool SsuDCR_CheckModuleCrc( UINT32 moduleCrc32, UINT32* pTempCrc32Result );

UINT32 SsuDCR_GetCheckCrcProgressPercent(void);

void SsuLIB_EnableMvdMemAccess( void );
void SsuLIB_DisableMvdMemAccess( void );
void SsuLIB_InitMvdMemHeapPartition( void );
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE


#ifdef _SIFUNCTIONWRAPPER_C_
    #define _SIFUNCTIONWRAPPER_C_DECLARE_



    #define _DEBUG_FUNCTION_WRAPPER_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #if 0 //def _DEBUG_FUNCTION_WRAPPER_
           #define FUNC_WRAPPER_DBG_MSG(x, format, args...) { \
                    switch(x) { \
                    case DBG_MSG_LEVEL_INFO   : printf("\033[1;32m%s() \033[m", __FUNCTION__); \
                        printf("[INFO][%4d]: ", __LINE__); break; \
                    case DBG_MSG_LEVEL_WARNING: printf("\033[1;33m%s() \033[m", __FUNCTION__); \
                        printf("[WARNING][%4d]: ", __LINE__); break; \
                    case DBG_MSG_LEVEL_ERROR  : printf("\033[1;31m%s() \033[m", __FUNCTION__); \
                        printf("[ERROR][%4d]: ", __LINE__); break; \
                    default: break; } \
                    printf(format, ##args); \
                    printf("\n"); \
                   }
    #else  //_DEBUG_FUNCTION_WRAPPER_
           #define FUNC_WRAPPER_DBG_MSG(x, format, args...)
    #endif //_DEBUG_FUNCTION_WRAPPER_


#else //_SIFUNCTIONWRAPPER_C_
    #define _SIFUNCTIONWRAPPER_C_DECLARE_ extern

#endif //_SIFUNCTIONWRAPPER_C_




#endif //_SIFUNCTIONWRAPPER_H_




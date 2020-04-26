
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuDtr.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/29
 *
 *=======================================================================*/


#ifndef _SSUDTR_H_
#define _SSUDTR_H_


#include <string.h>
#include "ssuUnt.h"
#include "ssuDdb.h"
#include "ssuDii.h"
#include "ssuDsi.h"



struct SSU_DTR_ ;
typedef struct SSU_DTR_       SSU_DTR_T, *SSU_DTR_P ;



//SSU SW Location
struct SSU_SW_LOCATION_ {
 SSU_PROFILE profile ;
 TABLE_TREE_TS_LAYER_P tsAddr ;
 UINT16 programNumber ;

 //Enhanced Profile
 //struct {
 UINT16 assocTag ;
 UINT8  subgroupTag[5] ;
 //};
 
 //Simple Profile
 //struct {
 UINT16 ssuEsPid ;
 UINT32 diiTransactionId ;
 //};

};
typedef struct SSU_SW_LOCATION_       SSU_SW_LOCATION_T, *SSU_SW_LOCATION_P ;

/*
struct PMT_ES_RECORD_ {
 UINT8        streamType;
 UINT16       esPid;
};
typedef struct PMT_ES_RECORD_       PMT_ES_RECORD_T, *PMT_ES_RECORD_P;

struct PMT_ES_INFO_ {
 INT32              infoNum;
 PMT_ES_RECORD_T    info[0];
};
typedef struct PMT_ES_INFO_       PMT_ES_INFO_T, *PMT_ES_INFO_P;

struct DESCRIPTOR_OBJECT_ {
 INT32      dataLen;
 UINT8      data[0];
};
typedef struct DESCRIPTOR_OBJECT_       DESCRIPTOR_OBJECT_T, *DESCRIPTOR_OBJECT_P;

struct DESCRIPTOR_INFO_ {
 INT32        num;
 UINT8        tag[0];
};
typedef struct DESCRIPTOR_INFO_     DESCRIPTOR_INFO_T, *DESCRIPTOR_INFO_P;
*/

#define TID_SSU_UNT 0x4B
#define TID_SSU_DXI 0x3B
#define TID_SSU_DDB 0x3C

//DTR Query Struct

#if 0
struct SSU_ES_RECORD_ {
 SSU_PROFILE profile ;
 UINT32 oui ;
 UINT16 pid ;
};
typedef struct SSU_ES_RECORD_       SSU_ES_RECORD_T, *SSU_ES_RECORD_P ;

struct SSU_ES_INFO_ {
 INT32 ssuEsNumber ;
 SSU_ES_RECORD_T es[0] ;
};
typedef struct SSU_ES_INFO_       SSU_ES_INFO_T, *SSU_ES_INFO_P ;
#endif//0

struct SSU_DII_TRANSACTION_ID_INFO_ {
 UINT32  diiTransactionIdNumber ;
 UINT32  diiTransactionIds[0] ;
};
typedef struct SSU_DII_TRANSACTION_ID_INFO_       SSU_DII_TRANSACTION_ID_INFO_T, *SSU_DII_TRANSACTION_ID_INFO_P ;

struct SSU_DTR_DDB_RECORD_ {
 UINT32  ddbDownloadId ;
 UINT8   moduleIdMsb   ;
};
typedef struct SSU_DTR_DDB_RECORD_       SSU_DTR_DDB_RECORD_T, *SSU_DTR_DDB_RECORD_P ;

struct SSU_DTR_DDB_INFO_ {
 UINT32  ddbNumber ;
 SSU_DTR_DDB_RECORD_T  ddbs[0] ;
};
typedef struct SSU_DTR_DDB_INFO_       SSU_DTR_DDB_INFO_T, *SSU_DTR_DDB_INFO_P ;

struct SSU_DTR_INFO_ {
 SSU_PROFILE profile ;
 UINT32 swPackNum ;
 UINT32 swPackIndex[0] ;
};
typedef struct SSU_DTR_INFO_       SSU_DTR_INFO_T, *SSU_DTR_INFO_P ;

//DTR Object Struct

struct SSU_DTR_METHOD_ {
 void  (*ClearDtrInfo)(SSU_DTR_P);
 void  (*EnableReceiveUnt)(SSU_DTR_P) ;
 void  (*DisableReceiveUnt)(SSU_DTR_P);
 void  (*EnableReceiveDsi)(SSU_DTR_P) ;
 void  (*DisableReceiveDsi)(SSU_DTR_P);
 void  (*EnableReceiveDii)(SSU_DTR_P) ;
 void  (*DisableReceiveDii)(SSU_DTR_P);
 void  (*EnableReceiveDdb)(SSU_DTR_P) ;
 void  (*DisableReceiveDdb)(SSU_DTR_P);
 void  (*InitSsuDtr)(SSU_DTR_P, TABLE_TREE_TS_LAYER_P, UINT16, UINT16, SSU_PROFILE ) ;
 bool  (*CheckEpUntExist)(SSU_DTR_P) ;
 bool  (*CheckSpDsiExist)(SSU_DTR_P) ;
 bool  (*CheckSpDiiExist)(SSU_DTR_P, UINT32) ;
 bool  (*CheckSpDdbExist)(SSU_DTR_P, UINT32) ;
 void  (*MountSsuSections)(SSU_DTR_P, UINT16, UINT8*, INT32) ;
 void  (*InitDtrObj)(SSU_DTR_P);
 bool  (*SpHaveCompatiblePlatform)(SSU_DTR_P) ;//for simple profile
 SSU_DII_TRANSACTION_ID_INFO_P (*SpGetCompatiblePlatformDiiInfo)(SSU_DTR_P) ;
 bool  (*SpHaveCompatiblePlatformDii)(SSU_DTR_P) ;//for simple-profile
 SSU_DTR_DDB_INFO_P (*SpGetCompatiblePlatformDdbInfo)(SSU_DTR_P) ;
 bool  (*SpHaveCompatiblePlatformDdb)(SSU_DTR_P) ;//for simple-profile
 void  (*SetDtrInfoReadyFlag)(SSU_DTR_P) ;
 bool  (*EpHaveCompatiblePlatform)(SSU_DTR_P) ;//for Enhanced-profile
 bool  (*GetDtrInfo)( SSU_DTR_P, SSU_DTR_INFO_P* );
 bool  (*GetSwLocation)( SSU_DTR_P, UINT32, SSU_SW_LOCATION_P* );
 bool  (*GetSwUpdateReason)( SSU_DTR_P, UINT32, SW_UPDATE_REASON_P* );
 bool  (*GetEpSsuEventName)( SSU_DTR_P, UINT32, SSU_EVENT_NAME_P* );
 bool  (*GetEpSsuEventText)( SSU_DTR_P, UINT32, SSU_EVENT_TEXT_P* );
 bool  (*GetEpUpdateSwSchedule)( SSU_DTR_P, UINT32, SSU_SCHEDULE_P* );
 bool  (*GetEpUpdateSwInfo)( SSU_DTR_P, UINT32, SSU_UPDATE_SW_INFO_P* );
};
typedef struct SSU_DTR_METHOD_       SSU_DTR_METHOD_T, *SSU_DTR_METHOD_P ;

struct SSU_DTR_SP_GROUP_ {
 //DII
 UINT32           diiTransactionId ;
 SSU_DII_P        dii ;
 //DDB
 UINT32           ddbDownloadId ;
 UINT8            ddbModuleIdMsb ;
 SSU_DDB_P        ddb ;
};
typedef struct SSU_DTR_SP_GROUP_       SSU_DTR_SP_GROUP_T, *SSU_DTR_SP_GROUP_P ;

struct SSU_DTR_SP_GROUPS_ {
 UINT32 groupNumber ;
 SSU_DTR_SP_GROUP_T groups[0] ;
};
typedef struct SSU_DTR_SP_GROUPS_       SSU_DTR_SP_GROUPS_T, *SSU_DTR_SP_GROUPS_P ;

struct SSU_DTR_ {

 SSU_DTR_METHOD_P method ;

 TABLE_TREE_TS_LAYER_P tsAddr ;
 UINT16 programNumber ;
 UINT16 ssuEsPid ;
 SSU_PROFILE profile ;
 bool flagDtrInfoReady ;

 //Enhanced Profile
 struct {
 //UNT
 bool             flagDtrCanReceiveUnt ;
 SSU_UNT_P        unt ;
 };

 //Simple Profile
 struct {
 //DSI
 bool                flagDtrCanReceiveDsi ;
 SSU_DSI_P           dsi ;
 //DII & DDB
 bool                flagDtrCanReceiveDii ;
 bool                flagDtrCanReceiveDdb ;
 SSU_DTR_SP_GROUPS_P pSpGroups ;
 };

};



//SSU DTR METHODS
void  SsuDtr_ClearDtrInfo(SSU_DTR_P ssuDtr);
void  SsuDtr_EnableReceiveUnt(SSU_DTR_P ssuDtr) ;
void  SsuDtr_DisableReceiveUnt(SSU_DTR_P ssuDtr);
void  SsuDtr_EnableReceiveDsi(SSU_DTR_P ssuDtr) ;
void  SsuDtr_DisableReceiveDsi(SSU_DTR_P ssuDtr);
void  SsuDtr_EnableReceiveDii(SSU_DTR_P ssuDtr) ;
void  SsuDtr_DisableReceiveDii(SSU_DTR_P ssuDtr);
void  SsuDtr_EnableReceiveDdb(SSU_DTR_P ssuDtr) ;
void  SsuDtr_DisableReceiveDdb(SSU_DTR_P ssuDtr);
void  SsuDtr_InitSsuDtr(SSU_DTR_P ssuDtr, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber, UINT16 ssuEsPid, SSU_PROFILE profile );
bool  SsuDtr_CheckEpUntExist(SSU_DTR_P ssuDtr) ;
bool  SsuDtr_CheckSpDsiExist(SSU_DTR_P ssuDtr) ;
bool  SsuDtr_CheckSpDiiExist(SSU_DTR_P ssuDtr, UINT32 diiTransactionId) ;
bool  SsuDtr_CheckSpDdbExist(SSU_DTR_P ssuDtr, UINT32 ddbDownloadId) ;
void  SsuDtr_MountSsuSections(SSU_DTR_P ssuDtr, UINT16 pid, UINT8* secRawData, INT32 secRawDataLen );
void  SsuDtr_InitDtrObj(SSU_DTR_P ssuDtr);
bool  SsuDtr_SpHaveCompatiblePlatform( SSU_DTR_P ssuDtr ) ;//for simple-profile
SSU_DII_TRANSACTION_ID_INFO_P SsuDtr_SpGetCompatiblePlatformDiiInfo( SSU_DTR_P ssuDtr ) ;
bool  SsuDtr_SpHaveCompatiblePlatformDii( SSU_DTR_P ssuDtr ) ;//for simple-profile
SSU_DTR_DDB_INFO_P SsuDtr_SpGetCompatiblePlatformDdbInfo( SSU_DTR_P ssuDtr ) ;
bool  SsuDtr_SpHaveCompatiblePlatformDdb( SSU_DTR_P ssuDtr ) ;//for simple-profile
void  SsuDtr_SetDtrInfoReadyFlag( SSU_DTR_P ssuDtr ) ;
bool  SsuDtr_EpHaveCompatiblePlatform( SSU_DTR_P ssuDtr ) ;//for Enhanced-profile
bool  SsuDtr_GetDtrInfo( SSU_DTR_P ssuDtr, SSU_DTR_INFO_P *ppGetDtrInfo );
bool  SsuDtr_GetSwLocation( SSU_DTR_P ssuDtr, UINT32 swPackIndex, SSU_SW_LOCATION_P *ppGetSwLocation );
bool  SsuDtr_GetSwUpdateReason( SSU_DTR_P ssuDtr, UINT32 swPackIndex, SW_UPDATE_REASON_P *ppGetSwUpdateReason );
bool  SsuDtr_GetEpSsuEventName( SSU_DTR_P ssuDtr, UINT32 swPackIndex, SSU_EVENT_NAME_P *ppGetSsuEventName );
bool  SsuDtr_GetEpSsuEventText( SSU_DTR_P ssuDtr, UINT32 swPackIndex, SSU_EVENT_TEXT_P *ppGetSsuEventText );
bool  SsuDtr_GetEpUpdateSwSchedule( SSU_DTR_P ssuDtr, UINT32 swPackIndex, SSU_SCHEDULE_P *ppGetUpdateSwSchedule );
bool  SsuDtr_GetEpUpdateSwInfo( SSU_DTR_P ssuDtr, UINT32 swPackIndex, SSU_UPDATE_SW_INFO_P *ppGetUpdateSwInfo );



SSU_ES_INFO_P SsuDtr_GenVcSsuEsList(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber );
void SsuDtr_FreeVcSsuEsList(SIAGENT_OBJECT_P pSiAgentObj, SSU_ES_INFO_T** pEsInfo);


    #if 1

	#define  SSUDTR_MALLOC(size)   SI_AGENT_MALLOC((size))

	#define  SSUDTR_FREE(p)	SI_AGENT_FREE((p))
		
	#if 0	
	    #include <memCheck.h>

	    #define  SSUDTR_MALLOC(size)   Memcheck_Alloc(size)

	    #define  SSUDTR_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
	#endif//0
    #else

    #define SSUDTR_MALLOC(size)   malloc(size)

    #define SSUDTR_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif


#ifdef _SSUDTR_C_
    #define _SSUDTR_C_DECLARE_



    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_DTR_
           #define SSU_DTR_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SSU_DTR_
           #define SSU_DTR_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_DTR_


#else //_SSUDTR_C_
    #define _SSUDTR_C_DECLARE_ extern

#endif //_SSUDTR_C_

void SsuDtr_InitDtrMethod(void);
_SSUDTR_C_DECLARE_ SSU_DTR_METHOD_T ssuDtrMethod ;

_SSUDTR_C_DECLARE_ SSU_DTR_T ssuDtrObj ;
_SSUDTR_C_DECLARE_ SSU_DTR_P pSsuDtrObj ;

#if 0
_SSUDTR_C_DECLARE_ UINT32 deviceOui        ;
_SSUDTR_C_DECLARE_ UINT16 deviceSwModel    ;
_SSUDTR_C_DECLARE_ UINT16 deviceSwVersion  ;
_SSUDTR_C_DECLARE_ UINT16 deviceHwModel    ;
_SSUDTR_C_DECLARE_ UINT16 deviceHwVersion  ;
#endif//0

#endif //_SSUDTR_H_





/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuDsi.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/29
 *
 *=======================================================================*/


#ifndef _SSUDSI_H_
#define _SSUDSI_H_


#include "ssuFunctionWrapper.h"
#include <string.h>


struct SSU_DSI_ ;
typedef struct SSU_DSI_       SSU_DSI_T, *SSU_DSI_P ;


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

//DSI Query Struct

struct SW_UPDATE_REASON_ {
 UINT8  iso639[3];
 UINT32 textLen ;
 UINT8  text[0] ;
};
typedef struct SW_UPDATE_REASON_       SW_UPDATE_REASON_T, *SW_UPDATE_REASON_P,
                                         SSU_EVENT_NAME_T,   *SSU_EVENT_NAME_P,
                                         SSU_EVENT_TEXT_T,   *SSU_EVENT_TEXT_P;

struct SSU_DSI_GROUP_RECORD_ {
 UINT32 transactionId ;
};
typedef struct SSU_DSI_GROUP_RECORD_       SSU_DSI_GROUP_RECORD_T, *SSU_DSI_GROUP_RECORD_P ;

struct SSU_DSI_GROUP_INFO_ {
 UINT32 gpNumber ;
 SSU_DSI_GROUP_RECORD_T gpRecords[0] ;
};
typedef struct SSU_DSI_GROUP_INFO_       SSU_DSI_GROUP_INFO_T, *SSU_DSI_GROUP_INFO_P ;

struct PLATFORM_DESC_OBJ_ {
 UINT8  descType ;
 UINT32 oui ;
 UINT16 model ;
 UINT16 version ;
 UINT8  subDescCount ;
};
typedef struct PLATFORM_DESC_OBJ_       PLATFORM_DESC_OBJ_T, *PLATFORM_DESC_OBJ_P ;

struct PLATFORM_DESC_OBJS_ {
 UINT32 descNumber ;
 PLATFORM_DESC_OBJ_T desc[0] ;
};
typedef struct PLATFORM_DESC_OBJS_       PLATFORM_DESC_OBJS_T, *PLATFORM_DESC_OBJS_P ;

struct PLATFORM_COMPATIBILITY_REPORT_ {
 UINT32 platformNumber ;
 bool   report[0] ;
};
typedef struct PLATFORM_COMPATIBILITY_REPORT_       PLATFORM_COMPATIBILITY_REPORT_T, *PLATFORM_COMPATIBILITY_REPORT_P ;


//DSI Object Struct

struct SSU_DSI_METHOD_ {
 UINT8  (*GetTableID)(SSU_DSI_P) ;
 UINT8  (*GetTableVersion)(SSU_DSI_P) ;
 UINT32 (*GetSectionLength)(SSU_DSI_P) ;
 UINT32 (*GetSectionCrc)(SSU_DSI_P) ;
 UINT32 (*GetNumberOfGroups)(SSU_DSI_P) ;
 SSU_DSI_GROUP_INFO_P (*GetGroupInfo)(SSU_DSI_P) ;
 PLATFORM_DESC_OBJS_P (*GetGpPlatformDescObjs)(SSU_DSI_P, UINT32) ;
 PLATFORM_DESC_OBJS_P (*GetGpPlatformSubDescObjs)(SSU_DSI_P, UINT32, UINT32) ;
 PLATFORM_COMPATIBILITY_REPORT_P (*GetPlatformCompatibility)(SSU_DSI_P) ;//for simple profile
 bool (*ConfirmPlatformCompatibility)(SSU_DSI_P, UINT32) ;//for DCR
 DESCRIPTOR_INFO_P (*GetGpInfoDescInfo)(SSU_DSI_P, UINT32) ;
 DESCRIPTOR_OBJECT_P (*GetGpInfoDesc)(SSU_DSI_P, UINT32, UINT32) ;
 UINT32 (*GetEpDiiTransactionId)(SSU_DSI_P, UINT8*) ;
 bool (*GetSwUpdateReason)( SSU_DSI_P, UINT32, SW_UPDATE_REASON_P* );
};
typedef struct SSU_DSI_METHOD_       SSU_DSI_METHOD_T, *SSU_DSI_METHOD_P ;

struct SSU_DSI_ {
 SSU_DSI_METHOD_P method ;
 UINT32 tableRawDataLen ;
 UINT8  tableRawData[0] ;
};

//SSU DSI METHODS
UINT8  SsuDsi_GetTableID(SSU_DSI_P ssuDsi) ;
UINT8  SsuDsi_GetTableVersion(SSU_DSI_P ssuDsi) ;
UINT32 SsuDsi_GetSectionLength(SSU_DSI_P ssuDsi) ;
UINT32 SsuDsi_GetSectionCrc(SSU_DSI_P ssuDsi) ;
UINT32 SsuDsi_GetNumberOfGroups(SSU_DSI_P ssuDsi) ;
SSU_DSI_GROUP_INFO_P SsuDsi_GetGroupInfo(SSU_DSI_P ssuDsi) ;
PLATFORM_DESC_OBJS_P SsuDsi_GetGpPlatformDescObjs( SSU_DSI_P ssuDsi, UINT32 gpNum ) ;
PLATFORM_DESC_OBJS_P SsuDsi_GetGpPlatformSubDescObjs( SSU_DSI_P ssuDsi, UINT32 gpNum, UINT32 descNum ) ;
PLATFORM_COMPATIBILITY_REPORT_P SsuDsi_GetPlatformCompatibility( SSU_DSI_P ssuDsi ) ;//for simple-profile DTR
bool SsuDsi_ConfirmPlatformCompatibility( SSU_DSI_P ssuDsi, UINT32 gpNum ) ;//for DCR
DESCRIPTOR_INFO_P SsuDsi_GetGpInfoDescInfo(SSU_DSI_P ssuDsi, UINT32 gpNum) ;
DESCRIPTOR_OBJECT_P SsuDsi_GetGpInfoDesc(SSU_DSI_P ssuDsi, UINT32 gpNum, UINT32 descNum) ;
UINT32 SsuDsi_GetEpDiiTransactionId( SSU_DSI_P ssuDsi, UINT8* pSubgroupTag );//UINT8  subgroupTag[5]
bool SsuDsi_GetSwUpdateReason( SSU_DSI_P ssuDsi, UINT32 swPackIndex, SW_UPDATE_REASON_P *ppGetSwUpdateReason );


SSU_DSI_P SsuDsi_CreateSsuDsiObj( UINT8* secRawData, INT32 secRawDataLen );
bool SsuDsi_CheckSsuDsiSyntax( UINT8* secRawData, INT32 secRawDataLen );


#ifdef _SSUDSI_C_
    #define _SSUDSI_C_DECLARE_

    #if 1

	#define SSUDSI_MALLOC(size)   SI_AGENT_MALLOC((size))
	#define SSUDSI_FREE(p)		SI_AGENT_FREE((p))
	
		#if 0
	    #include <memCheck.h>

	    #define SSUDSI_MALLOC(size)   Memcheck_Alloc(size)

	    #define SSUDSI_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
		#endif//0
		
    #else

    #define SSUDSI_MALLOC(size)   malloc(size)

    #define SSUDSI_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif


    #define _DEBUG_SSU_DSI_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_DSI_
           #define SSU_DSI_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SSU_DSI_
           #define SSU_DSI_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_DSI_


#else //_SSUDSI_C_
    #define _SSUDSI_C_DECLARE_ extern

#endif //_SSUDSI_C_


_SSUDSI_C_DECLARE_ UINT32 deviceOui        ;
_SSUDSI_C_DECLARE_ UINT16 deviceSwModel    ;
_SSUDSI_C_DECLARE_ UINT16 deviceSwVersion  ;
_SSUDSI_C_DECLARE_ UINT16 deviceHwModel    ;
_SSUDSI_C_DECLARE_ UINT16 deviceHwVersion  ;

void SsuDsi_InitDsiMethod(void);
_SSUDSI_C_DECLARE_ SSU_DSI_METHOD_T ssuDsiMethod ;

#endif //_SSUDSI_H_




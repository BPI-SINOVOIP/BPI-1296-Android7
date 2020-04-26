
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuUnt.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/29
 *
 *=======================================================================*/


#ifndef _SSUUNT_H_
#define _SSUUNT_H_



#include <string.h>
#include "ssuFunctionWrapper.h"
#include "ssuDsi.h"

struct SSU_UNT_ ;
typedef struct SSU_UNT_       SSU_UNT_T, *SSU_UNT_P ;


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

//UNT Query Struct

typedef enum {
  SSU_SCHEDULE_TIME_UNIT_SECOND   = 0,
  SSU_SCHEDULE_TIME_UNIT_MINUTE   = 1,
  SSU_SCHEDULE_TIME_UNIT_HOUR     = 2,
  SSU_SCHEDULE_TIME_UNIT_DAY      = 3
} SSU_SCHEDULE_TIME_UNIT ;

struct SSU_SCHEDULE_ {
 UINT8 startDateTime[5];
 UINT8 endDateTime[5];
 bool finalAvailability ;
 bool periodicityFlag ;
 SSU_SCHEDULE_TIME_UNIT periodUnit ;
 SSU_SCHEDULE_TIME_UNIT durationUnit ;
 SSU_SCHEDULE_TIME_UNIT estimatedCycleTimeUnit ;
 UINT8 period ;
 UINT8 duration ;
 UINT8 estimatedCycleTime ;
};
typedef struct SSU_SCHEDULE_       SSU_SCHEDULE_T, *SSU_SCHEDULE_P ;


typedef enum {
  SSU_UPDATE_FLAG_MANUAL    = 0,
  SSU_UPDATE_FLAG_AUTO      = 1,
  SSU_UPDATE_FLAG_RESERVE1  = 2,
  SSU_UPDATE_FLAG_RESERVE2  = 3
} SSU_UPDATE_FLAG ;

typedef enum {
  SSU_UPDATE_METHOD_IMMEDIATE     =  0,
  SSU_UPDATE_METHOD_IRD_AVAILABLE =  1,
  SSU_UPDATE_METHOD_NEXT_RESTART  =  2,
  SSU_UPDATE_METHOD_RESERVE1      =  3,
  SSU_UPDATE_METHOD_RESERVE2      =  4,
  SSU_UPDATE_METHOD_RESERVE3      =  5,
  SSU_UPDATE_METHOD_RESERVE4      =  6,
  SSU_UPDATE_METHOD_RESERVE5      =  7,
  SSU_UPDATE_METHOD_PRIVATE1      =  8,
  SSU_UPDATE_METHOD_PRIVATE2      =  9,
  SSU_UPDATE_METHOD_PRIVATE3      = 10,
  SSU_UPDATE_METHOD_PRIVATE4      = 11,
  SSU_UPDATE_METHOD_PRIVATE5      = 12,
  SSU_UPDATE_METHOD_PRIVATE6      = 13,
  SSU_UPDATE_METHOD_PRIVATE7      = 14,
  SSU_UPDATE_METHOD_RESERVE6      = 15
} SSU_UPDATE_METHOD ;

struct SSU_UPDATE_SW_INFO_ {
 SSU_UPDATE_FLAG updateFlag ;
 SSU_UPDATE_METHOD updateMethod ;
 UINT8 updatePriority :4 ;
};
typedef struct SSU_UPDATE_SW_INFO_       SSU_UPDATE_SW_INFO_T, *SSU_UPDATE_SW_INFO_P ;


struct TARGET_REPORT_ {
 UINT32 targetDescNumber ;
 bool   report[0] ;
};
typedef struct TARGET_REPORT_       TARGET_REPORT_T, *TARGET_REPORT_P ;

struct TARGET_LOOP_REPORT_ {
 UINT32 targetLoopNumber ;
 bool   report[0] ;
};
typedef struct TARGET_LOOP_REPORT_       TARGET_LOOP_REPORT_T, *TARGET_LOOP_REPORT_P ;



//UNT Object Struct

struct SSU_UNT_METHOD_ {
 UINT8  (*GetTableID)(SSU_UNT_P) ;
 UINT8  (*GetTableVersion)(SSU_UNT_P) ;
 UINT32 (*GetSectionLength)(SSU_UNT_P) ;
 UINT32 (*GetSectionCrc)(SSU_UNT_P) ;
 UINT32 (*GetOui)(SSU_UNT_P) ;
 UINT8  (*GetActionType)(SSU_UNT_P) ;
 UINT32 (*GetPlatformLoopNumber)(SSU_UNT_P) ;
 UINT32 (*GetTargetLoopNumber)(SSU_UNT_P, UINT32) ;
 DESCRIPTOR_INFO_P (*GetCommonDescInfo)(SSU_UNT_P) ;
 DESCRIPTOR_INFO_P (*GetTargetDescInfo)(SSU_UNT_P, UINT32, UINT32) ;
 DESCRIPTOR_INFO_P (*GetOperationalDescInfo)(SSU_UNT_P, UINT32, UINT32) ;
 DESCRIPTOR_OBJECT_P (*GetCommonDesc)(SSU_UNT_P, UINT32) ;
 DESCRIPTOR_OBJECT_P (*GetTargetDesc)(SSU_UNT_P, UINT32, UINT32, UINT32) ;
 DESCRIPTOR_OBJECT_P (*GetOperationalDesc)(SSU_UNT_P, UINT32, UINT32, UINT32) ;
 PLATFORM_COMPATIBILITY_REPORT_P (*GetPlatformCompatibility)(SSU_UNT_P) ;
 TARGET_REPORT_P (*GetTargetReport)(SSU_UNT_P, UINT32, UINT32) ;
 TARGET_LOOP_REPORT_P (*GetTargetLoopReport)(SSU_UNT_P, UINT32) ;
 PLATFORM_DESC_OBJS_P (*GetGpPlatformDescObjs)(SSU_UNT_P, UINT32) ;
 bool (*GetSwPackSubGroupTag)( SSU_UNT_P, UINT32, UINT8** ) ;
 bool (*GetSwPackAssocTag)( SSU_UNT_P, UINT32, UINT16* ) ;
 bool (*GetSwUpdateReason)( SSU_UNT_P, UINT32, SW_UPDATE_REASON_P* );
 bool (*GetEpSsuEventName)( SSU_UNT_P, UINT32, SSU_EVENT_NAME_P* );
 bool (*GetEpSsuEventText)( SSU_UNT_P, UINT32, SSU_EVENT_TEXT_P* );
 bool (*GetEpUpdateSwSchedule)( SSU_UNT_P, UINT32, SSU_SCHEDULE_P* );
 bool (*GetEpUpdateSwInfo)( SSU_UNT_P, UINT32, SSU_UPDATE_SW_INFO_P* );
};
typedef struct SSU_UNT_METHOD_       SSU_UNT_METHOD_T, *SSU_UNT_METHOD_P ;

struct SSU_UNT_ {
 SSU_UNT_METHOD_P method ;
 UINT32 tableRawDataLen ;
 UINT8  tableRawData[0] ;
};



//SSU UNT METHODS
UINT8  SsuUnt_GetTableID(SSU_UNT_P ssuUnt) ;
UINT8  SsuUnt_GetTableVersion(SSU_UNT_P ssuUnt) ;
UINT32 SsuUnt_GetSectionLength(SSU_UNT_P ssuUnt) ;
UINT32 SsuUnt_GetSectionCrc(SSU_UNT_P ssuUnt) ;
UINT32 SsuUnt_GetOui(SSU_UNT_P ssuUnt) ;
UINT8  SsuUnt_GetActionType(SSU_UNT_P ssuUnt) ;
UINT32 SsuUnt_GetPlatformLoopNumber(SSU_UNT_P ssuUnt) ;
UINT32 SsuUnt_GetTargetLoopNumber(SSU_UNT_P ssuUnt, UINT32 platformNum) ;
DESCRIPTOR_INFO_P SsuUnt_GetCommonDescInfo(SSU_UNT_P ssuUnt) ;
DESCRIPTOR_INFO_P SsuUnt_GetTargetDescInfo(SSU_UNT_P ssuUnt, UINT32 platformNum, UINT32 targetNum) ;
DESCRIPTOR_INFO_P SsuUnt_GetOperationalDescInfo(SSU_UNT_P ssuUnt, UINT32 platformNum, UINT32 targetNum) ;
DESCRIPTOR_OBJECT_P SsuUnt_GetCommonDesc(SSU_UNT_P ssuUnt, UINT32 descNum) ;
DESCRIPTOR_OBJECT_P SsuUnt_GetTargetDesc(SSU_UNT_P ssuUnt, UINT32 platformNum, UINT32 targetNum, UINT32 descNum) ;
DESCRIPTOR_OBJECT_P SsuUnt_GetOperationalDesc(SSU_UNT_P ssuUnt, UINT32 platformNum, UINT32 targetNum, UINT32 descNum) ;
PLATFORM_COMPATIBILITY_REPORT_P SsuUnt_GetPlatformCompatibility( SSU_UNT_P ssuUnt ) ;
TARGET_REPORT_P SsuUnt_GetTargetReport( SSU_UNT_P ssuUnt, UINT32 platformNum, UINT32 targetNum ) ;
TARGET_LOOP_REPORT_P SsuUnt_GetTargetLoopReport( SSU_UNT_P ssuUnt, UINT32 platformNum ) ;
PLATFORM_DESC_OBJS_P SsuUnt_GetGpPlatformDescObjs( SSU_UNT_P ssuUnt, UINT32 platformNum ) ;
bool SsuUnt_GetSwPackSubGroupTag( SSU_UNT_P ssuUnt, UINT32 swPackIndex, UINT8 **ppGetSubGroupTag ) ;
bool SsuUnt_GetSwPackAssocTag( SSU_UNT_P ssuUnt, UINT32 swPackIndex, UINT16 *pGetAssocTag ) ;
bool SsuUnt_GetSwUpdateReason( SSU_UNT_P ssuUnt, UINT32 swPackIndex, SW_UPDATE_REASON_P *ppGetSwUpdateReason );
bool SsuUnt_GetEpSsuEventName( SSU_UNT_P ssuUnt, UINT32 swPackIndex, SSU_EVENT_NAME_P *ppGetSsuEventName );
bool SsuUnt_GetEpSsuEventText( SSU_UNT_P ssuUnt, UINT32 swPackIndex, SSU_EVENT_TEXT_P *ppGetSsuEventText );
bool SsuUnt_GetEpUpdateSwSchedule( SSU_UNT_P ssuUnt, UINT32 swPackIndex, SSU_SCHEDULE_P *ppGetUpdateSwSchedule );
bool SsuUnt_GetEpUpdateSwInfo( SSU_UNT_P ssuUnt, UINT32 swPackIndex, SSU_UPDATE_SW_INFO_P *ppGetUpdateSwInfo );


SSU_UNT_P SsuUnt_CreateSsuUntObj( UINT8* secRawData, INT32 secRawDataLen );
bool SsuUnt_CheckSsuUntSyntax( UINT8* secRawData, INT32 secRawDataLen );
UINT32 SsuUnt_GetSecRawDataOui(UINT8* secRawData) ;
UINT8  SsuUnt_GetSecRawDataActionType(UINT8* secRawData) ;


#ifdef _SSUUNT_C_
    #define _SSUUNT_C_DECLARE_

    #if 1

	#define  SSUUNT_MALLOC(size)   SI_AGENT_MALLOC((size))

	#define  SSUUNT_FREE(p)	SI_AGENT_FREE((p))
		
	#if 0	
	    #include <memCheck.h>

	    #define  SSUUNT_MALLOC(size)   Memcheck_Alloc(size)

	    #define  SSUUNT_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
	#endif//0
    #else

    #define SSUUNT_MALLOC(size)   malloc(size)

    #define SSUUNT_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif


    #define _DEBUG_SSU_UNT_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_UNT_
           #define SSU_UNT_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SSU_UNT_
           #define SSU_UNT_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_UNT_


#else //_SSUUNT_C_
    #define _SSUUNT_C_DECLARE_ extern

#endif //_SSUUNT_C_



typedef UINT8 (*CB_FP_CHECK_TARGET_DESC)(DESCRIPTOR_OBJECT_P);

_SSUUNT_C_DECLARE_ CB_FP_CHECK_TARGET_DESC CB_CheckSmartCardDescTarget   ;
_SSUUNT_C_DECLARE_ CB_FP_CHECK_TARGET_DESC CB_CheckMacAddrDescTarget     ;
_SSUUNT_C_DECLARE_ CB_FP_CHECK_TARGET_DESC CB_CheckSerialNumDescTarget   ;
_SSUUNT_C_DECLARE_ CB_FP_CHECK_TARGET_DESC CB_CheckIpAddrDescTarget      ;
_SSUUNT_C_DECLARE_ CB_FP_CHECK_TARGET_DESC CB_CheckIpv6AddrDescTarget    ;
_SSUUNT_C_DECLARE_ CB_FP_CHECK_TARGET_DESC CB_CheckPrivateDataDescTarget ;

void SsuUnt_InitUntMethod(void);
_SSUUNT_C_DECLARE_ SSU_UNT_METHOD_T ssuUntMethod ;

#endif //_SSUUNT_H_




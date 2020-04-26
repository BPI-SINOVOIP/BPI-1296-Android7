
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siAgentMainTask.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/11
 *
 *=======================================================================*/


#ifndef _SIAGENTMAINTASK_H_
#define _SIAGENTMAINTASK_H_

//#include <rtd_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "siAgentUtil.h"
//#include <system/sidb2/siInterface/siInterfaceHeader.h>
#include <system/si4/SITypes.h>
#include "siAgentVirSecFilterMgr.h"

///kjw cccccccccccccc
#include "siAgentTypes.h"

//#include "siFunctionWrapper.h"


#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
	#include "ssu/ssuDcr.h"
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

	
#ifndef SI_TYPE_TS_LAYER
#define SI_TYPE_TS_LAYER
//typedef	SI_DATABASE_TS_NODE *TABLE_TREE_TS_LAYER_P;
#endif


struct SIAGENT_OBJECT_ ;


/////////////////////Filter Task/////////////////////


typedef enum {
  FILTER_TASK_STATE_TO_DO        = 1,
  FILTER_TASK_STATE_DOING        = 2,
  FILTER_TASK_STATE_DONE         = 3
} FILTER_TASK_STATE ;

typedef enum {
  FILTER_TASK_TYPE_TIMEOUT_AUTO_CLOSED        = 1,
  FILTER_TASK_TYPE_RECEIVE_AUTO_CLOSED        = 2,
  FILTER_TASK_TYPE_OPEN_CONTINUOUSLY          = 3
} FILTER_TASK_TYPE ;

typedef enum {
  CHK_TABLE_RECEIVE_FUNC_PAT              = 1,
  CHK_TABLE_RECEIVE_FUNC_PMT              = 2,
  CHK_TABLE_RECEIVE_FUNC_TOT              = 3,
  CHK_TABLE_RECEIVE_FUNC_TDT              = 4,
  CHK_TABLE_RECEIVE_FUNC_A_SDT            = 5,
  CHK_TABLE_RECEIVE_FUNC_A_NIT            = 6,
  CHK_TABLE_RECEIVE_FUNC_ALL_BAT          = 7,
  CHK_TABLE_RECEIVE_FUNC_AP_EIT           = 8,
  CHK_TABLE_RECEIVE_FUNC_AF_EIT           = 9,
  CHK_TABLE_EXIST_FUNC_AS4DAY_EIT         = 10,

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
	CHK_TABLE_EXIST_FUNC_DTR_EP_UNT 		= 13,
	CHK_TABLE_EXIST_FUNC_DTR_SP_DSI 		= 14,
	CHK_TABLE_EXIST_FUNC_DTR_SP_DII 		= 15,
	CHK_TABLE_EXIST_FUNC_DTR_SP_DDB 		= 16,
	CHK_TABLE_EXIST_FUNC_DCR_DSI			= 17,
	CHK_TABLE_EXIST_FUNC_DCR_DII			= 18,
	CHK_TABLE_EXIST_FUNC_DCR_ALL_BLKS		= 19
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE
  
} CHK_TABLE_RECEIVE_FUNCTION ;


struct CHK_TS_TABLE_RECEIVE_ARG_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
};
typedef struct CHK_TS_TABLE_RECEIVE_ARG_     CHK_PAT_RECEIVE_ARG_T,     *CHK_PAT_RECEIVE_ARG_P,
                                             CHK_TOT_RECEIVE_ARG_T,     *CHK_TOT_RECEIVE_ARG_P,
                                             CHK_TDT_RECEIVE_ARG_T,     *CHK_TDT_RECEIVE_ARG_P,
                                             CHK_A_SDT_RECEIVE_ARG_T,   *CHK_A_SDT_RECEIVE_ARG_P,
                                             CHK_A_NIT_RECEIVE_ARG_T,   *CHK_A_NIT_RECEIVE_ARG_P,
                                             CHK_ALL_BAT_RECEIVE_ARG_T, *CHK_ALL_BAT_RECEIVE_ARG_P ;

struct CHK_VC_TABLE_RECEIVE_ARG_ {
  TABLE_TREE_TS_LAYER_P  tsAddr        ;
  unsigned short                 programNumber ;
};
typedef struct CHK_VC_TABLE_RECEIVE_ARG_     CHK_PMT_RECEIVE_ARG_T,     *CHK_PMT_RECEIVE_ARG_P,
                                             CHK_AP_EIT_RECEIVE_ARG_T,  *CHK_AP_EIT_RECEIVE_ARG_P,
                                             CHK_AF_EIT_RECEIVE_ARG_T,  *CHK_AF_EIT_RECEIVE_ARG_P ;

struct CHK_AS4DAY_EIT_EXIST_ARG_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  unsigned char                  tid    ;
};
typedef struct CHK_AS4DAY_EIT_EXIST_ARG_     CHK_AS4DAY_EIT_EXIST_ARG_T,     *CHK_AS4DAY_EIT_EXIST_ARG_P ;


#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
	struct CHK_SSU_TABLE_VOID_ARG_ {
      //void
	};
	typedef struct CHK_SSU_TABLE_VOID_ARG_       CHK_DTR_SP_DSI_EXIST_ARG_T,     *CHK_DTR_SP_DSI_EXIST_ARG_P,
                                                 CHK_DTR_EP_UNT_EXIST_ARG_T,     *CHK_DTR_EP_UNT_EXIST_ARG_P,
                                                 CHK_DCR_DSI_EXIST_ARG_T,        *CHK_DCR_DSI_EXIST_ARG_P,
                                                 CHK_DCR_DII_EXIST_ARG_T,        *CHK_DCR_DII_EXIST_ARG_P,
                                                 CHK_DCR_ALL_BLKS_EXIST_ARG_T,   *CHK_DCR_ALL_BLKS_EXIST_ARG_P ;

	struct CHK_DTR_SP_DII_EXIST_ARG_ {
      UINT32 diiTransactionId ;
    };
    typedef struct CHK_DTR_SP_DII_EXIST_ARG_     CHK_DTR_SP_DII_EXIST_ARG_T,     *CHK_DTR_SP_DII_EXIST_ARG_P ;

    struct CHK_DTR_SP_DDB_EXIST_ARG_ {
      UINT32 ddbDownloadId ;
    };
    typedef struct CHK_DTR_SP_DDB_EXIST_ARG_     CHK_DTR_SP_DDB_EXIST_ARG_T,     *CHK_DTR_SP_DDB_EXIST_ARG_P ;


#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE


typedef enum {
  FILTER_TASK_CLOSED_REASON_TIMEOUT     = 1,
  FILTER_TASK_CLOSED_REASON_RECEIVED    = 2
} FILTER_TASK_CLOSED_REASON ;


struct TYPE_TIMEOUT_AUTO_CLOSED_ {
  time_t endingTime ;//Target Time
  FILTER_TASK_CLOSED_REASON closedReason ;
};
typedef struct TYPE_TIMEOUT_AUTO_CLOSED_     TYPE_TIMEOUT_AUTO_CLOSED_T,     *TYPE_TIMEOUT_AUTO_CLOSED_P ;
struct TYPE_RECEIVE_AUTO_CLOSED_ {
  //Empty Struct
};
typedef struct TYPE_RECEIVE_AUTO_CLOSED_     TYPE_RECEIVE_AUTO_CLOSED_T,     *TYPE_RECEIVE_AUTO_CLOSED_P ;
struct TYPE_OPEN_CONTINUOUSLY_ {
  //Empty Struct
};
typedef struct TYPE_OPEN_CONTINUOUSLY_     TYPE_OPEN_CONTINUOUSLY_T,     *TYPE_OPEN_CONTINUOUSLY_P ;


struct FILTER_TASK_ {
  FILTER_TASK_STATE filterTaskState ;
  FILTER_TASK_TYPE  filterTaskType  ;

  union {
    TYPE_TIMEOUT_AUTO_CLOSED_T  typeTimeoutClosed    ;
    TYPE_RECEIVE_AUTO_CLOSED_T  typeReceiveClosed    ;
    TYPE_OPEN_CONTINUOUSLY_T    typeOpenContinuously ;
  };

  CHK_TABLE_RECEIVE_FUNCTION chkTableReceiveFunction ;
  union {
    CHK_PAT_RECEIVE_ARG_T         argChkPat       ;
    CHK_TOT_RECEIVE_ARG_T         argChkTot       ;
    CHK_TDT_RECEIVE_ARG_T         argChkTdt       ;
    CHK_A_SDT_RECEIVE_ARG_T       argChkASdt      ;
    CHK_A_NIT_RECEIVE_ARG_T       argChkANit      ;
    CHK_ALL_BAT_RECEIVE_ARG_T     argChkAllBat    ;
    CHK_PMT_RECEIVE_ARG_T         argChkPmt       ;
    CHK_AP_EIT_RECEIVE_ARG_T      argChkApEit     ;
    CHK_AF_EIT_RECEIVE_ARG_T      argChkAfEit     ;
    CHK_AS4DAY_EIT_EXIST_ARG_T    argChkAs4DayEit ;

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
	CHK_DTR_SP_DSI_EXIST_ARG_T	  argChkDtrSpDsi ;
	CHK_DTR_EP_UNT_EXIST_ARG_T	  argChkDtrEpUnt ;
	CHK_DCR_DSI_EXIST_ARG_T 	  argChkDcrDsi	 ;
	CHK_DCR_DII_EXIST_ARG_T 	  argChkDcrDii	 ;
	CHK_DCR_ALL_BLKS_EXIST_ARG_T  argChkDcrAllBlks ;
	CHK_DTR_SP_DII_EXIST_ARG_T	  argChkDtrSpDii ;
	CHK_DTR_SP_DDB_EXIST_ARG_T	  argChkDtrSpDdb ;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

  };//only for FILTER_TASK_TYPE_TIMEOUT_AUTO_CLOSED and FILTER_TASK_TYPE_RECEIVE_AUTO_CLOSED

  SEC_FILTER_FUNCTION secFilterFunction ;
  union {
    ARG_GET_TABLE_T       argGetTable         ;
    ARG_MONITOR_PAT_T     argMonitorPat       ;
    ARG_GET_PMT_T         argGetPmt           ;
    ARG_MONITOR_PMT_T     argMonitorPmt       ;
    ARG_MONITOR_TABLE_T   argMonitorTable     ;
    ARG_GET_APF_EIT_T     argGetApfEit        ;
    ARG_MONITOR_APF_EIT_T argMonitorApfEit    ;
#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
	ARG_GET_DXI_T         argGetDxi;
	ARG_MONITOR_DXI_T     argMonitorDxi;
	ARG_GET_SSU_DDB_T     argGetSsuDdb;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

  };
};
typedef struct FILTER_TASK_       FILTER_TASK_T, *FILTER_TASK_P ;


struct FILTER_TASK_POOL_ {
  time_t    targetCurrentTime  ;
  unsigned short    maxSecFilterNumber ;
  unsigned short    totalFilterTaskNumber ;
  unsigned short    todoFilterTaskNumber  ;
  unsigned short    doingFilterTaskNumber ;
  unsigned short    doneFilterTaskNumber  ;
  FILTER_TASK_T   filterTask[0]   ;//filterTask[n]
};
typedef struct FILTER_TASK_POOL_       FILTER_TASK_POOL_T, *FILTER_TASK_POOL_P ;


/////////////////////Main Task/////////////////////

struct MAIN_TASK_ARG_CHG_CH_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  unsigned short  programNumber ;
};
typedef struct MAIN_TASK_ARG_CHG_CH_       MAIN_TASK_ARG_CHG_CH_T,     *MAIN_TASK_ARG_CHG_CH_P,
                                           MAIN_TASK_ARG_RECORD_PRG_T, *MAIN_TASK_ARG_RECORD_PRG_P ;
struct MAIN_TASK_ARG_SCAN_CH_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
};
typedef struct MAIN_TASK_ARG_SCAN_CH_       MAIN_TASK_ARG_SCAN_CH_T,  *MAIN_TASK_ARG_SCAN_CH_P,
                                            MAIN_TASK_ARG_SCAN_PRG_T, *MAIN_TASK_ARG_SCAN_PRG_P ;
struct MAIN_TASK_ARG_STOP_ {
  //Empty Struct
};
typedef struct MAIN_TASK_ARG_STOP_       MAIN_TASK_ARG_STOP_T, *MAIN_TASK_ARG_STOP_P ;


#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE

struct MAIN_TASK_ARG_SCAN_SSU_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  PROGRAMS_INFO_P        pProgramsInfo ;
  UINT32                 pmtLoopIndex ;
  SSU_ES_INFO_P          pSsuEsInfo ;
  UINT32                 esLoopIndex ;
};
typedef struct MAIN_TASK_ARG_SCAN_SSU_       MAIN_TASK_ARG_SCAN_SSU_T, *MAIN_TASK_ARG_SCAN_SSU_P ;
struct MAIN_TASK_ARG_SSU_DCR_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  UINT16  programNumber ;//unused: for align with MAIN_TASK_ARG_CHGCH_SSUDCR_T(dcr sub task can get right value).
  SSU_SW_LOCATION_T ssuSwLocation ;
};
typedef struct MAIN_TASK_ARG_SSU_DCR_       MAIN_TASK_ARG_SSU_DCR_T, *MAIN_TASK_ARG_SSU_DCR_P ;
struct MAIN_TASK_ARG_CHGCH_SSUDCR_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  UINT16  programNumber ;
  SSU_SW_LOCATION_T ssuSwLocation ;
};
typedef struct MAIN_TASK_ARG_CHGCH_SSUDCR_       MAIN_TASK_ARG_CHGCH_SSUDCR_T, *MAIN_TASK_ARG_CHGCH_SSUDCR_P ;

#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE


struct MAIN_TASK_ {
  struct MAIN_TASK_  *fd ;
  struct MAIN_TASK_  *bk ;
  unsigned int stateNumber ;
  unsigned int lastStateNumber ;
  void (*StateMachineFunction)( struct SIAGENT_OBJECT_ *,struct MAIN_TASK_  *) ;
  FILTER_TASK_POOL_P pFilterTaskPool ;
  BOOL                   lastTaskIsChgCh ;
  TABLE_TREE_TS_LAYER_P  lastTsAddr ;

  union {
    MAIN_TASK_ARG_CHG_CH_T       argChangeCh ;
    MAIN_TASK_ARG_SCAN_CH_T      argScanCh   ;
    MAIN_TASK_ARG_STOP_T         argStop     ;
    MAIN_TASK_ARG_RECORD_PRG_T   argRecordPrg ;
    MAIN_TASK_ARG_SCAN_PRG_T     argScanPrg ;
#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
    MAIN_TASK_ARG_SCAN_SSU_T     argScanSsu;
    MAIN_TASK_ARG_SSU_DCR_T      argSsuDcr ;
    MAIN_TASK_ARG_CHGCH_SSUDCR_T argChgChSsuDcr ;
#endif//#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE

  };

};
typedef struct MAIN_TASK_       MAIN_TASK_T, *MAIN_TASK_P ;


struct MAIN_TASK_INFO_ {
  MAIN_TASK_P            pExecMainTask ;
  BOOL                   lastTaskIsChgCh ;
  TABLE_TREE_TS_LAYER_P  lastTsAddr ;
};
typedef struct MAIN_TASK_INFO_       MAIN_TASK_INFO_T, *MAIN_TASK_INFO_P ;



struct MAIN_TASK_HD_ {
  struct MAIN_TASK_HD_  *fd ;
  struct MAIN_TASK_HD_  *bk ;
};
typedef struct MAIN_TASK_HD_       MAIN_TASK_HD_T, *MAIN_TASK_HD_P ;


#ifdef _SIAGENTMAINTASK_C_
    #define _SIAGENTMAINTASK_C_DECLARE_




    #if 0
       #include <memCheck.h>
       #define MAINTASK_MALLOC(size)     Memcheck_Alloc(size)
       #define MAINTASK_FREE(p)          do{                   \
                                            Memcheck_Free(p) ; \
                                            (p) = NULL ;       \
                                           }while(0)
    #else
       #define MAINTASK_MALLOC(size)     malloc(size)
       #define MAINTASK_FREE(p)          do{             \
                                            free(p) ;    \
                                            (p) = NULL ; \
                                           }while(0)
    #endif


    #define _DEBUG_MAIN_TASK_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #if 0 //def _DEBUG_MAIN_TASK_
           #define MAIN_TASK_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_MAIN_TASK_
           #define MAIN_TASK_DBG_MSG(x, format, args...)
    #endif //_DEBUG_MAIN_TASK_


#else //_SIAGENTMAINTASK_C_
    #define _SIAGENTMAINTASK_C_DECLARE_ extern

#endif //_SIAGENTMAINTASK_C_




#endif //_SIAGENTMAINTASK_H_










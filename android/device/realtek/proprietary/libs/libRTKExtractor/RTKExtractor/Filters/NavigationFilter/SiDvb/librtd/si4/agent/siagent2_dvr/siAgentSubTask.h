
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siAgentSubTask.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/11
 *
 *=======================================================================*/


#ifndef _SIAGENTSUBTASK_H_
#define _SIAGENTSUBTASK_H_

//#include <rtd_types.h>
#include "siAgentMainTask.h"


struct SUB_TASK_ARG_CHG_CH_PAT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  unsigned short  tsid ;
  BOOL    patReceive ;
  unsigned char   patVersion ;
  unsigned int  patCrc ;
  unsigned short  programNumber ;
  unsigned short  pmtPid ;
};
typedef struct SUB_TASK_ARG_CHG_CH_PAT_       SUB_TASK_ARG_CHG_CH_PAT_T, *SUB_TASK_ARG_CHG_CH_PAT_P ;
struct SUB_TASK_ARG_CHG_CH_PMT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  unsigned short  programNumber ;
  BOOL    pmtReceive ;
  unsigned char   pmtVersion ;
  unsigned int  pmtCrc ;
};
typedef struct SUB_TASK_ARG_CHG_CH_PMT_       SUB_TASK_ARG_CHG_CH_PMT_T, *SUB_TASK_ARG_CHG_CH_PMT_P ;
struct SUB_TASK_ARG_CHG_CH_BAT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  BOOL    allBatsReceive ;
};
typedef struct SUB_TASK_ARG_CHG_CH_BAT_       SUB_TASK_ARG_CHG_CH_BAT_T, *SUB_TASK_ARG_CHG_CH_BAT_P ;
struct SUB_TASK_ARG_CHG_CH_A_NIT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  BOOL    aNitReceive ;
  unsigned char   aNitVersion ;
  unsigned int  aNitCrc ;
};
typedef struct SUB_TASK_ARG_CHG_CH_A_NIT_       SUB_TASK_ARG_CHG_CH_A_NIT_T, *SUB_TASK_ARG_CHG_CH_A_NIT_P ;
struct SUB_TASK_ARG_CHG_CH_A_SDT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  BOOL    aSdtReceive ;
  unsigned char   aSdtVersion ;
  unsigned int  aSdtCrc ;
};
typedef struct SUB_TASK_ARG_CHG_CH_A_SDT_       SUB_TASK_ARG_CHG_CH_A_SDT_T, *SUB_TASK_ARG_CHG_CH_A_SDT_P ;
struct SUB_TASK_ARG_CHG_CH_TDT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  BOOL    tdtReceive ;
};
typedef struct SUB_TASK_ARG_CHG_CH_TDT_       SUB_TASK_ARG_CHG_CH_TDT_T, *SUB_TASK_ARG_CHG_CH_TDT_P ;
struct SUB_TASK_ARG_CHG_CH_TOT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  BOOL    totReceive ;
};
typedef struct SUB_TASK_ARG_CHG_CH_TOT_       SUB_TASK_ARG_CHG_CH_TOT_T, *SUB_TASK_ARG_CHG_CH_TOT_P ;
struct SUB_TASK_ARG_CHG_CH_AP_EIT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  unsigned short  programNumber ;
  BOOL    apEitReceive ;
  unsigned char   apEitVersion ;
  unsigned int  apEitCrc ;
};
typedef struct SUB_TASK_ARG_CHG_CH_AP_EIT_       SUB_TASK_ARG_CHG_CH_AP_EIT_T, *SUB_TASK_ARG_CHG_CH_AP_EIT_P ;
struct SUB_TASK_ARG_CHG_CH_AF_EIT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  unsigned short  programNumber ;
  BOOL    afEitReceive ;
  unsigned char   afEitVersion ;
  unsigned int  afEitCrc ;
};
typedef struct SUB_TASK_ARG_CHG_CH_AF_EIT_       SUB_TASK_ARG_CHG_CH_AF_EIT_T, *SUB_TASK_ARG_CHG_CH_AF_EIT_P ;
struct SUB_TASK_ARG_CHG_CH_AS_EIT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  BOOL    asEitExist[16] ;
  time_t  tdtAndTotTimeout ;
};
typedef struct SUB_TASK_ARG_CHG_CH_AS_EIT_       SUB_TASK_ARG_CHG_CH_AS_EIT_T, *SUB_TASK_ARG_CHG_CH_AS_EIT_P ;
struct SUB_TASK_ARG_STOP_ {
  //Empty Struct
};
typedef struct SUB_TASK_ARG_STOP_       SUB_TASK_ARG_STOP_T, *SUB_TASK_ARG_STOP_P ;
struct SUB_TASK_ARG_TS_CHG_GET_PMTS_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
};
typedef struct SUB_TASK_ARG_TS_CHG_GET_PMTS_     SUB_TASK_ARG_TS_CHG_GET_PMTS_T, *SUB_TASK_ARG_TS_CHG_GET_PMTS_P ;
struct SUB_TASK_ARG_BKGD_GET_PMT_ {
  TABLE_TREE_TS_LAYER_P  tsAddr ;
  unsigned short  programNumber ;
  BOOL    pmtReceive ;
  unsigned char   pmtVersion ;
  unsigned int  pmtCrc ;
};
typedef struct SUB_TASK_ARG_BKGD_GET_PMT_       SUB_TASK_ARG_BKGD_GET_PMT_T, *SUB_TASK_ARG_BKGD_GET_PMT_P ;

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE

struct SUB_TASK_ARG_BKGD_SSU_DTR_ {
      TABLE_TREE_TS_LAYER_P  tsAddr ;
      UINT16  programNumber ;
      INT32   forLoopIndex ;
      SSU_ES_INFO_T ssuEsInfo ;
    };
typedef struct SUB_TASK_ARG_BKGD_SSU_DTR_       SUB_TASK_ARG_BKGD_SSU_DTR_T, *SUB_TASK_ARG_BKGD_SSU_DTR_P ;
	
struct SUB_TASK_ARG_SSU_DCR_ {
      SSU_SW_LOCATION_T ssuSwLocation ;
      time_t sectionTimeout ;//for PAT,DDB
      UINT32 receivedDdbCountForTimeoutDetect ;
      bool patReceive ;
      bool pmtReceive ;
      bool dsiReceive ;
      bool diiReceive ;
      UINT8 diiVersion ;
      UINT32 diiCrc ;
      UINT32 diiTransactionId ;
      bool allBlocksReceive ;
      UINT32 progressBarPercent ;
      //for check module crc32
      UINT32 moduleCrc32 ;
      UINT32 tempCrc32Result ;
      UINT32 moduleNumber ;
      UINT32 moduleLoopIndex ;
      UINT32 blockNumber ;
      UINT32 blockLoopIndex ;
      UINT32 blockSize ;
      UINT32 blockDataCheckedLocation ;
      UINT8* pBlockRawData ;
    };
    typedef struct SUB_TASK_ARG_SSU_DCR_       SUB_TASK_ARG_SSU_DCR_T, *SUB_TASK_ARG_SSU_DCR_P ;
    struct SUB_TASK_ARG_SSUDCR_PAT_ {
      TABLE_TREE_TS_LAYER_P  tsAddr ;
    };
    typedef struct SUB_TASK_ARG_SSUDCR_PAT_       SUB_TASK_ARG_SSUDCR_PAT_T, *SUB_TASK_ARG_SSUDCR_PAT_P ;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

typedef enum {
  SUB_TASK_QUEUE_PAT        = 0,
  SUB_TASK_QUEUE_PMT,
  SUB_TASK_QUEUE_TOT,
  SUB_TASK_QUEUE_TDT,
  SUB_TASK_QUEUE_A_NIT,
  SUB_TASK_QUEUE_A_SDT,
  SUB_TASK_QUEUE_AP_EIT,
  SUB_TASK_QUEUE_AF_EIT,
  SUB_TASK_QUEUE_AS_EIT,
  SUB_TASK_QUEUE_BAT,
  SUB_TASK_QUEUE_SSU_DTR,
  SUB_TASK_QUEUE_SSU_DCR,
  SUB_TASK_QUEUE_NUMBER,
} SUB_TASK_QUEUE_INDEX ;

struct SUB_TASK_ {
  struct SUB_TASK_  *fd ;
  struct SUB_TASK_  *bk ;
  unsigned int stateNumber ;
  unsigned int lastStateNumber ;
  void (*StateMachineFunction)( struct SIAGENT_OBJECT_ *, SUB_TASK_QUEUE_INDEX, struct SUB_TASK_  *) ;
  FILTER_TASK_POOL_P pFilterTaskPool ;
  union {
    SUB_TASK_ARG_CHG_CH_PAT_T       argChgChPat   ;
    SUB_TASK_ARG_CHG_CH_PMT_T       argChgChPmt   ;
    SUB_TASK_ARG_CHG_CH_BAT_T       argChgChBat   ;
    SUB_TASK_ARG_CHG_CH_A_NIT_T     argChgChANit  ;
    SUB_TASK_ARG_CHG_CH_A_SDT_T     argChgChASdt  ;
    SUB_TASK_ARG_CHG_CH_TDT_T       argChgChTdt   ;
    SUB_TASK_ARG_CHG_CH_TOT_T       argChgChTot   ;
    SUB_TASK_ARG_CHG_CH_AP_EIT_T    argChgChApEit ;
    SUB_TASK_ARG_CHG_CH_AF_EIT_T    argChgChAfEit ;
    SUB_TASK_ARG_CHG_CH_AS_EIT_T    argChgChAsEit ;
    SUB_TASK_ARG_STOP_T             argStop       ;
    SUB_TASK_ARG_TS_CHG_GET_PMTS_T  argTsChgGetPmts ;
    SUB_TASK_ARG_BKGD_GET_PMT_T     argBkgdGetPmts ;
#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
	SUB_TASK_ARG_BKGD_SSU_DTR_T     argBkgdSsuDtr ;
	SUB_TASK_ARG_SSU_DCR_T          argSsuDcr ;
	SUB_TASK_ARG_SSUDCR_PAT_T       argSsuDcrPat ;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE
  };
};
typedef struct SUB_TASK_       SUB_TASK_T, *SUB_TASK_P ;


struct SUB_TASK_HD_ {
  struct SUB_TASK_HD_  *fd ;
  struct SUB_TASK_HD_  *bk ;
};
typedef struct SUB_TASK_HD_       SUB_TASK_HD_T, *SUB_TASK_HD_P ;


#ifdef _SIAGENTSUBTASK_C_
    #define _SIAGENTSUBTASK_C_DECLARE_



    #if 0
       #include <memCheck.h>
       #define SUBTASK_MALLOC(size)     Memcheck_Alloc(size)
       #define SUBTASK_FREE(p)          do{                   \
                                           Memcheck_Free(p) ; \
                                           (p) = NULL ;       \
                                          }while(0)
    #else
       #define SUBTASK_MALLOC(size)     malloc(size)
       #define SUBTASK_FREE(p)          do{             \
                                           free(p) ;    \
                                           (p) = NULL ; \
                                          }while(0)
    #endif


    #define _DEBUG_SUB_TASK_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #if 0 //def _DEBUG_SUB_TASK_
           #define SUB_TASK_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SUB_TASK_
           #define SUB_TASK_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SUB_TASK_


#else //_SIAGENTSUBTASK_C_
    #define _SIAGENTSUBTASK_C_DECLARE_ extern

#endif //_SIAGENTSUBTASK_C_




#endif //_SIAGENTSUBTASK_H_










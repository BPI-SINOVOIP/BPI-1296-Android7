
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siAgentStateMachine.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/16
 *
 *=======================================================================*/


#ifndef _SIAGENTSTATEMACHINE_H_
#define _SIAGENTSTATEMACHINE_H_

//#include <rtd_types.h>
#include "siAgentFilterTaskExecutor.h"



#define STOP_EXEC_TASK     0xFFFF
#define START_EXEC_TASK    0x0000
#define NO_PREVIOUS_STATE 0x10000

typedef enum {
  SM_MAIN_SCANCH_ST_START           = 0x0000,
  SM_MAIN_SCANCH_ST_WAIT_GET_PAT    = 0x0001,
  SM_MAIN_SCANCH_ST_WAIT_GET_TABLES = 0x0002,
  SM_MAIN_SCANCH_ST_END             = 0xFFFF
} SM_MAIN_SCANCH_STATE ;

typedef enum {
  SM_MAIN_STOP_ST_START           = 0x0000,
  SM_MAIN_STOP_ST_END             = 0xFFFF
} SM_MAIN_STOP_STATE ;

typedef enum {
  SM_SUB_STOP_ST_START           = 0x0000,
  SM_SUB_STOP_ST_END             = 0xFFFF
} SM_SUB_STOP_STATE ;

typedef enum {
  SM_MAIN_CHGCH_ST_START           = 0x0000,
  SM_MAIN_CHGCH_ST_THE_SAME_TS     = 0x0001,
  SM_MAIN_CHGCH_ST_DIFFERENT_TS    = 0x0002,
  SM_MAIN_CHGCH_ST_END             = 0xFFFF
} SM_MAIN_CHGCH_STATE ;

typedef enum {
  SM_MAIN_SCANPRG_ST_START           = 0x0000,
  SM_MAIN_SCANPRG_ST_WAIT_GET_PAT    = 0x0001,
  SM_MAIN_SCANPRG_ST_WAIT_GET_PMTS   = 0x0002,
  SM_MAIN_SCANPRG_ST_END             = 0xFFFF
} SM_MAIN_SCANPRG_STATE ;

typedef enum {
  SM_MAIN_RECORDPRG_ST_START           = 0x0000,
  SM_MAIN_RECORDPRG_ST_THE_SAME_TS     = 0x0001,
  SM_MAIN_RECORDPRG_ST_DIFFERENT_TS    = 0x0002,
  SM_MAIN_RECORDPRG_ST_END             = 0xFFFF
} SM_MAIN_RECORDPRG_STATE ;

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
    typedef enum {
      SM_MAIN_SSUDCR_ST_START           = 0x0000,
      SM_MAIN_SSUDCR_ST_END             = 0xFFFF
    } SM_MAIN_SSUDCR_STATE ;

    typedef enum {
      SM_MAIN_CHGCHSSUDCR_ST_START           = 0x0000,
      SM_MAIN_CHGCHSSUDCR_ST_THE_SAME_TS     = 0x0001,
      SM_MAIN_CHGCHSSUDCR_ST_DIFFERENT_TS    = 0x0002,
      SM_MAIN_CHGCHSSUDCR_ST_END             = 0xFFFF
    } SM_MAIN_CHGCHSSUDCR_STATE ;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE


typedef enum {
  SM_SUB_CHGCH_BAT_ST_START           = 0x0000,
  SM_SUB_CHGCH_BAT_ST_WAIT_GET_BAT    = 0x0001,
  SM_SUB_CHGCH_BAT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_BAT_STATE ;


typedef enum {
  SM_SUB_CHGCH_TOT_ST_START           = 0x0000,
  SM_SUB_CHGCH_TOT_ST_WAIT_GET_TOT    = 0x0001,
  SM_SUB_CHGCH_TOT_ST_DEL_TDT         = 0x0002,
  SM_SUB_CHGCH_TOT_ST_KEEP_GET_TOT    = 0x0003,
  SM_SUB_CHGCH_TOT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_TOT_STATE ;


typedef enum {
  SM_SUB_CHGCH_TDT_ST_START           = 0x0000,
  SM_SUB_CHGCH_TDT_ST_WAIT_GET_TDT    = 0x0001,
  SM_SUB_CHGCH_TDT_ST_KEEP_GET_TDT    = 0x0002,
  SM_SUB_CHGCH_TDT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_TDT_STATE ;


typedef enum {
  SM_SUB_CHGCH_ANIT_ST_START           = 0x0000,
  SM_SUB_CHGCH_ANIT_ST_WAIT_GET_ANIT   = 0x0001,
  SM_SUB_CHGCH_ANIT_ST_MONITOR_ANIT    = 0x0002,
  SM_SUB_CHGCH_ANIT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_ANIT_STATE ;

typedef enum {
  SM_SUB_CHGCH_ASDT_ST_START           = 0x0000,
  SM_SUB_CHGCH_ASDT_ST_WAIT_GET_ASDT   = 0x0001,
  SM_SUB_CHGCH_ASDT_ST_MONITOR_ASDT    = 0x0002,
  SM_SUB_CHGCH_ASDT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_ASDT_STATE ;



typedef enum {
  SM_SUB_CHGCH_APEIT_ST_START           = 0x0000,
  SM_SUB_CHGCH_APEIT_ST_WAIT_GET_APEIT  = 0x0001,
  SM_SUB_CHGCH_APEIT_ST_MONITOR_APEIT   = 0x0002,
  SM_SUB_CHGCH_APEIT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_APEIT_STATE ;


typedef enum {
  SM_SUB_CHGCH_AFEIT_ST_START           = 0x0000,
  SM_SUB_CHGCH_AFEIT_ST_WAIT_GET_AFEIT  = 0x0001,
  SM_SUB_CHGCH_AFEIT_ST_MONITOR_AFEIT   = 0x0002,
  SM_SUB_CHGCH_AFEIT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_AFEIT_STATE ;


typedef enum {
  SM_SUB_CHGCH_ASEIT_ST_START           = 0x0000,
  SM_SUB_CHGCH_ASEIT_ST_GET_ASEIT       = 0x0001,
  SM_SUB_CHGCH_ASEIT_ST_WAIT_GET_ASEIT  = 0x0002,
  SM_SUB_CHGCH_ASEIT_ST_WAIT_TXT        = 0x0003,
  SM_SUB_CHGCH_ASEIT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_ASEIT_STATE ;


typedef enum {
  SM_SUB_CHGCH_PMT_ST_START           = 0x0000,
  SM_SUB_CHGCH_PMT_ST_WAIT_GET_PAT    = 0x0001,
  SM_SUB_CHGCH_PMT_ST_WAIT_GET_PMT    = 0x0002,
  SM_SUB_CHGCH_PMT_ST_MONITOR_PMT     = 0x0003,
  SM_SUB_CHGCH_PMT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_PMT_STATE ;

typedef enum {
  SM_SUB_CHGCH_PAT_ST_START           = 0x0000,
  SM_SUB_CHGCH_PAT_ST_WAIT_GET_PAT    = 0x0001,
  SM_SUB_CHGCH_PAT_ST_MONITOR_PAT     = 0x0002,
  SM_SUB_CHGCH_PAT_ST_TS_CHANGED_1    = 0x0003,
  SM_SUB_CHGCH_PAT_ST_TS_CHANGED_2    = 0x0004,
  SM_SUB_CHGCH_PAT_ST_PAT_VER_CHG     = 0x0005,
  SM_SUB_CHGCH_PAT_ST_END             = 0xFFFF
} SM_SUB_CHGCH_PAT_STATE ;

typedef enum {
  SM_SUB_TSCHG_PMT_ST_START           = 0x0000,
  SM_SUB_TSCHG_PMT_ST_WAIT_GETPMTS    = 0x0001,
  SM_SUB_TSCHG_PMT_ST_END             = 0xFFFF
} SM_SUB_TSCHG_PMT_STATE ;


#ifdef _SUPPORT_CABOT_MHEG5_
      typedef enum {
        SM_SUB_BKGD_PMT_ST_START           = 0x0000,
        SM_SUB_BKGD_PMT_ST_WAIT_GET_PAT    = 0x0001,
        SM_SUB_BKGD_PMT_ST_WAIT_GET_PMT    = 0x0002,
        SM_SUB_BKGD_PMT_ST_MONITOR_PMTS    = 0x0003,
        SM_SUB_BKGD_PMT_ST_WAIT_GET_PMTS   = 0x0004,
        SM_SUB_BKGD_PMT_ST_END             = 0xFFFF
      } SM_SUB_BKGD_PMT_STATE ;
#else //_SUPPORT_CABOT_MHEG5_
typedef enum {
  SM_SUB_BKGD_PMT_ST_START           = 0x0000,
  SM_SUB_BKGD_PMT_ST_WAIT_GET_PAT    = 0x0001,
  SM_SUB_BKGD_PMT_ST_WAIT_GET_PMT    = 0x0002,
  SM_SUB_BKGD_PMT_ST_WAIT_GET_PMTS   = 0x0003,
  SM_SUB_BKGD_PMT_ST_MONITOR_PMT     = 0x0004,
  SM_SUB_BKGD_PMT_ST_END             = 0xFFFF
} SM_SUB_BKGD_PMT_STATE ;
#endif //_SUPPORT_CABOT_MHEG5_
	  
#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
  typedef enum {
      SM_SUB_SSU_DTR_ST_START           = 0x0000,//Reset For-Loop Index
      SM_SUB_SSU_DTR_ST_SEARCH_LOOP     = 0x0001,//Search SSU Service For-Loop
      SM_SUB_SSU_DTR_ST_FILTER_DSI      = 0x0002,//Simple Profile Step 4-1
      SM_SUB_SSU_DTR_ST_FILTER_DII      = 0x0003,//Simple Profile Step 4-2
      SM_SUB_SSU_DTR_ST_FILTER_DDB      = 0x0004,//Simple Profile Step 4-3
      SM_SUB_SSU_DTR_ST_SAVE_SP_SW_INFO = 0x0005,//Simple Profile Step 4-4
      SM_SUB_SSU_DTR_ST_FILTER_UNT      = 0x0006,//Enhanced Profile Step 2-1
      SM_SUB_SSU_DTR_ST_SAVE_EP_SW_INFO = 0x0007,//Enhanced Profile Step 2-2
      SM_SUB_SSU_DTR_ST_END             = 0xFFFF
    } SM_SUB_SSU_DTR_STATE ;

    typedef enum {
      SM_MAIN_SCANSSU_ST_START           = 0x0000,
      SM_MAIN_SCANSSU_ST_WAIT_GET_PAT    = 0x0001,
      SM_MAIN_SCANSSU_ST_PMT_LOOP        = 0x0002,
      SM_MAIN_SCANSSU_ST_WAIT_GET_PMT    = 0x0003,
      SM_MAIN_SCANSSU_ST_SEARCH_SSU_LOOP = 0x0004,//Search SSU Service For-Loop
      SM_MAIN_SCANSSU_ST_FILTER_DSI      = 0x0005,//Simple Profile Step 4-1
      SM_MAIN_SCANSSU_ST_FILTER_DII      = 0x0006,//Simple Profile Step 4-2
      SM_MAIN_SCANSSU_ST_FILTER_DDB      = 0x0007,//Simple Profile Step 4-3
      SM_MAIN_SCANSSU_ST_SAVE_SP_SW_INFO = 0x0008,//Simple Profile Step 4-4
      SM_MAIN_SCANSSU_ST_FILTER_UNT      = 0x0009,//Enhanced Profile Step 2-1
      SM_MAIN_SCANSSU_ST_SAVE_EP_SW_INFO = 0x000A,//Enhanced Profile Step 2-2
      SM_MAIN_SCANSSU_ST_END             = 0xFFFF
    } SM_MAIN_SCANSSU_STATE ;

    typedef enum {
      SM_SUB_SSU_DCR_ST_START           = 0x0000,
      SM_SUB_SSU_DCR_ST_WAIT_GET_PAT    = 0x0001,
      SM_SUB_SSU_DCR_ST_WAIT_GET_PMT    = 0x0002,
      SM_SUB_SSU_DCR_ST_WAIT_GET_DSI    = 0x0003,
      SM_SUB_SSU_DCR_ST_WAIT_GET_DII    = 0x0004,
      SM_SUB_SSU_DCR_ST_WAIT_GET_DDB    = 0x0005,
      SM_SUB_SSU_DCR_ST_MODULE_CHK_LOOP = 0x0006,
      SM_SUB_SSU_DCR_ST_BLOCK_CHK_LOOP  = 0x0007,
      SM_SUB_SSU_DCR_ST_CHK_CRC         = 0x0008,
      SM_SUB_SSU_DCR_ST_END             = 0xFFFF
    } SM_SUB_SSU_DCR_STATE ;

    typedef enum {
      SM_SUB_SSUDCR_PAT_ST_START           = 0x0000,
      SM_SUB_SSUDCR_PAT_ST_WAIT_GET_PAT    = 0x0001,
      SM_SUB_SSUDCR_PAT_ST_END             = 0xFFFF
    } SM_SUB_SSUDCR_PAT_STATE ;

#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

MAIN_TASK_P SiAgent_SM_GenMainTask_Stop(void) ;
MAIN_TASK_P SiAgent_SM_GenMainTask_ScanCh( TABLE_TREE_TS_LAYER_P tsAddr ) ;
MAIN_TASK_P SiAgent_SM_GenMainTask_ChgCh( TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
MAIN_TASK_P SiAgent_SM_GenMainTask_ScanPrg( TABLE_TREE_TS_LAYER_P tsAddr ) ;
MAIN_TASK_P SiAgent_SM_GenMainTask_RecordPrg( TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
MAIN_TASK_P SiAgent_SM_GenMainTask_ScanSsu( TABLE_TREE_TS_LAYER_P tsAddr );
MAIN_TASK_P SiAgent_SM_GenMainTask_SsuDcr( TABLE_TREE_TS_LAYER_P tsAddr, SSU_SW_LOCATION_P pSsuSwLocation );
MAIN_TASK_P SiAgent_SM_GenMainTask_ChgChSsuDcr( TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber, SSU_SW_LOCATION_P pSsuSwLocation );
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

SUB_TASK_P SiAgent_SM_GenSubTask_Stop(void) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChPat(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChPmt(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChBat(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChANit(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChASdt(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChTot(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChTdt(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChAsEit(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChApEit(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_ChgChAfEit(MAIN_TASK_P pMainTask) ;

SUB_TASK_P SiAgent_SM_GenSubTask_TsChgGetPmts(MAIN_TASK_P pMainTask) ;
SUB_TASK_P SiAgent_SM_GenSubTask_TsChgGetPmts_CurChDisappear(MAIN_TASK_P pMainTask) ;

SUB_TASK_P SiAgent_SM_GenSubTask_BkgdGetPmts(MAIN_TASK_P pMainTask) ;

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
SUB_TASK_P SiAgent_SM_GenSubTask_BkgdSsuDtr( MAIN_TASK_P pMainTask, SSU_ES_INFO_P pSsuEsInfo ) ;
SUB_TASK_P SiAgent_SM_GenSubTask_SsuDcr( MAIN_TASK_P pMainTask );
SUB_TASK_P SiAgent_SM_GenSubTask_SsuDcrPat( MAIN_TASK_P pMainTask ) ;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

void SiAgent_SM_InsertSubTask( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask ) ;


void SiAgent_SM_Main_ScanCh( SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask) ;
void SiAgent_SM_Main_ChangeCh( SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask) ;
void SiAgent_SM_Main_Stop( SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask) ;
void SiAgent_SM_Main_ScanPrg( SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask) ;
void SiAgent_SM_Main_RecordPrg( SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask) ;

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
    void SiAgent_SM_Main_ScanSsu(SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask);
    void SiAgent_SM_Main_ChgChSsuDecode(SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask) ;
    void SiAgent_SM_Main_SsuDecode(SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask) ;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

void SiAgent_SM_Sub_Stop( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChPat( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChPmt( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChTot( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChTdt( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChANit( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChASdt( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChApEit( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChAfEit( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChAsEit( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_ChgChBat( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_TsChgGetPmts( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_TsChgGetPmts_CurChDisappear( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_BkgdGetPmts( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
void SiAgent_SM_Sub_BkgdSsuDtr(SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
void SiAgent_SM_Sub_SsuDcr(SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask);
void SiAgent_SM_Sub_SsuDecodePat(SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex, SUB_TASK_P pSubTask) ;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

 //#ifdef _DEBUG_SIAGENT_MEM_
 #if 1

		#define SM_MALLOC(size)  SI_AGENT_MALLOC(size)

		#define SM_FREE(p) 	SI_AGENT_FREE((p))

		#if 0
	    #include <memCheck.h>

	    #define SM_MALLOC(size)  Memcheck_Alloc(size)

	    #define SM_FREE(p)      do{                  \
	                               Memcheck_Free(p); \
	                               (p) = NULL ;      \
	                              }while(0)
		#endif//0
    #else

		#if 0//for debug...
   		#define SM_MALLOC(size)   malloc(size)

    		#define SM_FREE(p)         do{               \
                                  free(p) ;      \
                                  (p) = NULL ;   \
                                 }while(0)
		#endif//

#endif//_DEBUG_SIAGENT_MEM_

#ifdef _SIAGENTSTATEMACHINE_C_
    #define _SIAGENTSTATEMACHINE_C_DECLARE_


    #define _DEBUG_SIAGENT_SM_




    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _SI_DEBUG_SIAGENT_SM_DETAIL_
           #define SM_DBG_MSG(x, format, args...) { \
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
           #define MAIN_SM_STATE_MSG(pAgent, pMT, str) { \
                    if( pMT->stateNumber != pMT->lastStateNumber ) \
                      { \
                       SM_DBG_MSG(DBG_MSG_LEVEL_INFO, "[SiAgent-%s]:Main State Machine [task %s] enter state %d.",pAgent->agentObjectName , str, pMT->stateNumber); \
                       pMT->lastStateNumber = pMT->stateNumber ; \
                      } \
                   }
           #define SUB_SM_STATE_MSG(pAgent, sm, pST, str) { \
                    if( pST->stateNumber != pST->lastStateNumber ) \
                      { \
                       SM_DBG_MSG(DBG_MSG_LEVEL_INFO, "[SiAgent-%s]:Sub State Machine %d [task %s] enter state %d.",pAgent->agentObjectName , sm, str, pST->stateNumber); \
                       pST->lastStateNumber = pST->stateNumber ; \
                      } \
                   }
           #define MAIN_TASK_ENDING_MSG(pAgent, pMT, str) { \
                    if( (STOP_EXEC_TASK != pMT->lastStateNumber) && (STOP_EXEC_TASK == pMT->stateNumber) ) \
                      { \
                       SM_DBG_MSG(DBG_MSG_LEVEL_INFO, "[SiAgent-%s]:Main State Machine [task %s] enter task ending state.",pAgent->agentObjectName ,str); \
                      } \
                   }
           #define SUB_TASK_ENDING_MSG(pAgent, sm, pST, str) { \
                    if( (STOP_EXEC_TASK != pST->lastStateNumber) && (STOP_EXEC_TASK == pST->stateNumber) ) \
                      { \
                       SM_DBG_MSG(DBG_MSG_LEVEL_INFO, "[SiAgent-%s]:Sub State Machine %d [task %s] enter task ending state.",pAgent->agentObjectName , sm, str); \
                      } \
                   }
    #else
           #define SM_DBG_MSG(x, format, args...)
           #define MAIN_SM_STATE_MSG(pAgent, pMT, str)
           #define SUB_SM_STATE_MSG(pAgent, sm, pST, str)
           #define MAIN_TASK_ENDING_MSG(pAgent, pMT, str)
           #define SUB_TASK_ENDING_MSG(pAgent, sm, pST, str)
    #endif



#else //_SIAGENTSTATEMACHINE_C_
    #define _SIAGENTSTATEMACHINE_C_DECLARE_ extern

#endif //_SIAGENTSTATEMACHINE_C_






#endif //_SIAGENTSTATEMACHINE_H_



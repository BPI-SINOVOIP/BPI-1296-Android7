
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siAgentFilterTaskExecutor.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/15
 *
 *=======================================================================*/


#ifndef _SIAGENTFILTERTASKEXECUTOR_H_
#define _SIAGENTFILTERTASKEXECUTOR_H_

//#include <rtd_types.h>

#include "siAgentObject.h"
#include "siFunctionWrapper.h"



#define ACTUAL_SCHEDULE_EIT_TID_BASE 0x50

// [ss] [20070830] : sec + 1 will be better
#define TIMEOUT_SEC(sec)   (sec + 1)


void SiAgent_UpdateFilterTaskState( SIAGENT_OBJECT_P pSiAgentObj, FILTER_TASK_POOL_P pFilterTaskPool );
void SiAgent_ExecuteFilterTask( SIAGENT_OBJECT_P pSiAgentObj, FILTER_TASK_POOL_P pFilterTaskPool );

void SiAgent_StopAllFilterTasks( SIAGENT_OBJECT_P pSiAgentObj, FILTER_TASK_POOL_P pFilterTaskPool );
void SiAgent_StopOneFilterTask( SIAGENT_OBJECT_P pSiAgentObj, FILTER_TASK_POOL_P pFilterTaskPool, unsigned short filterTaskNumber );

FILTER_TASK_POOL_P SiAgent_GenScanChGetPatFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
FILTER_TASK_POOL_P SiAgent_GenScanChGetTablesFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, PROGRAMS_INFO_P pProgramsInfo );

FILTER_TASK_POOL_P SiAgent_GenScanPrgGetPmtsFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, PROGRAMS_INFO_P pProgramsInfo );

FILTER_TASK_POOL_P SiAgent_GenChgChGetPatFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
FILTER_TASK_POOL_P SiAgent_GenChgChMonitorPatFtp( SIAGENT_OBJECT_P pSiAgentObj, unsigned short  tsid, unsigned char version );
FILTER_TASK_POOL_P SiAgent_GenChgChGetPmtFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short pid, unsigned short programNumber );
FILTER_TASK_POOL_P SiAgent_GenChgChMonitorPmtFtp( SIAGENT_OBJECT_P pSiAgentObj, unsigned short pid, unsigned short programNumber, unsigned char version );
FILTER_TASK_POOL_P SiAgent_GenChgChGetANitFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
FILTER_TASK_POOL_P SiAgent_GenChgChMonitorANitFtp( SIAGENT_OBJECT_P pSiAgentObj, unsigned char version );
FILTER_TASK_POOL_P SiAgent_GenChgChGetASdtFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
FILTER_TASK_POOL_P SiAgent_GenChgChMonitorASdtFtp( SIAGENT_OBJECT_P pSiAgentObj, unsigned char version );
FILTER_TASK_POOL_P SiAgent_GenChgChGetTdtFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
FILTER_TASK_POOL_P SiAgent_GenChgChGetTotFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
FILTER_TASK_POOL_P SiAgent_GenChgChGetBatFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
FILTER_TASK_POOL_P SiAgent_GenChgChGetApEitFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
FILTER_TASK_POOL_P SiAgent_GenChgChMonitorApEitFtp( SIAGENT_OBJECT_P pSiAgentObj, unsigned short programNumber, unsigned char version );
FILTER_TASK_POOL_P SiAgent_GenChgChGetAfEitFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short programNumber );
FILTER_TASK_POOL_P SiAgent_GenChgChMonitorAfEitFtp( SIAGENT_OBJECT_P pSiAgentObj, unsigned short programNumber, unsigned char version );
FILTER_TASK_POOL_P SiAgent_GenChgChGetAsEitFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

FILTER_TASK_POOL_P SiAgent_GenTsChgGetPmtsFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, PROGRAMS_INFO_P pProgramsInfo );

FILTER_TASK_POOL_P SiAgent_GenBackgroundGetPmtsFtp( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, unsigned short pid, unsigned short programNumber, unsigned char version, PROGRAMS_INFO_P pProgramsInfo );

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
FILTER_TASK_POOL_P SiAgent_GenChgChGetDtrUntFtp(SIAGENT_OBJECT_P pSiAgentObj, UINT16 pid, UINT32 oui );
FILTER_TASK_POOL_P SiAgent_GenChgChGetDtrDsiFtp(SIAGENT_OBJECT_P pSiAgentObj, UINT16 pid );
FILTER_TASK_POOL_P SiAgent_GenChgChGetDtrDiisFtp(SIAGENT_OBJECT_P pSiAgentObj, UINT16 pid, SSU_DII_TRANSACTION_ID_INFO_P pDiiTransactionIdInfo );
FILTER_TASK_POOL_P SiAgent_GenChgChGetDtrDdbsFtp(SIAGENT_OBJECT_P pSiAgentObj, UINT16 pid, SSU_DTR_DDB_INFO_P pDtrDdbInfo );

FILTER_TASK_POOL_P SiAgent_GenScanSsuGetPmtFtp(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 pid, UINT16 programNumber );

FILTER_TASK_POOL_P SiAgent_GenSsuDcrGetDiiFtp(SIAGENT_OBJECT_P pSiAgentObj, UINT16 pid, UINT32 diiTransactionId );
FILTER_TASK_POOL_P SiAgent_GenSsuDcrGetDsiFtp(SIAGENT_OBJECT_P pSiAgentObj, UINT16 pid );
FILTER_TASK_POOL_P SiAgent_GenSsuDcrGetDdbFtp(SIAGENT_OBJECT_P pSiAgentObj, UINT16 pid, UINT8 moduleIdMsb, UINT32 diiTransactionId );
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE


//#ifdef _DEBUG_SIAGENT_MEM_
#if 1

	#define FTE_MALLOC(size)  SI_AGENT_MALLOC(size)
 	#define FTE_FREE(p)   SI_AGENT_FREE((p))

	#if 0
	    #include <memCheck.h>

	    #define FTE_MALLOC(size)  Memcheck_Alloc(size)

	    #define FTE_FREE(p)     do{                  \
	                               Memcheck_Free(p); \
	                               (p) = NULL ;      \
	                              }while(0)
	#endif//0

    #else
	    #define FTE_MALLOC(size)   malloc(size)

	    #define FTE_FREE(p)        do{               \
	                                  free(p) ;      \
	                                  (p) = NULL ;   \
	                                 }while(0)
#endif//_DEBUG_SIAGENT_MEM_


#ifdef _SIAGENTFILTERTASKEXECUTOR_C_
    #define _SIAGENTFILTERTASKEXECUTOR_C_DECLARE_


    #define _DEBUG_SIAGENT_FTE_



    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #if 0 //_DEBUG_SIAGENT_FTE_
           #define FTE_DBG_MSG(x, format, args...) { \
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
    #else
           #define FTE_DBG_MSG(x, format, args...)
    #endif




    #define PID_PAT  0x0000
    #define PID_NIT  0x0010
    #define PID_SDT  0x0011
    #define PID_BAT  0x0011
    #define PID_TDT  0x0014
    #define PID_TOT  0x0014
    #define PID_EIT  0x0012


	#define TID_PAT         0x00
	#define TID_CAT 		0x01
	#define TID_PMT 		0x02
	#define TID_DXI 		0x3B
	#define TID_DDB 		0x3C
    #define TID_ACTUAL_NIT  0x40
    #define TID_ACTUAL_SDT  0x42
    #define TID_BAT         0x4A
	#define TID_UNT 		0x4B

    #define TID_ACTUAL_PRESENT_EIT   0x4E
    #define TID_ACTUAL_FOLLOWING_EIT 0x4E

	#define TID_TDT 		0x70
	#define TID_TOT 		0x73

    

    #define PRESENT_EIT_SECTION_NUMBER   0x00
    #define FOLLOWING_EIT_SECTION_NUMBER 0x01

	#define ACTION_TYPE_SSU 0x01
	#define DSI_TRANSACTION_ID_SUB_FIELD_ID 		0x00


    BOOL SiAgent_FTE_CheckTableReceive( SIAGENT_OBJECT_P pSiAgentObj, FILTER_TASK_P pFilterTask );
    void SiAgent_FTE_DisableSecFilter( SIAGENT_OBJECT_P pSiAgentObj, FILTER_TASK_P pFilterTask );
    void SiAgent_FTE_EnableSecFilter( SIAGENT_OBJECT_P pSiAgentObj, FILTER_TASK_P pFilterTask );

#else //_SIAGENTFILTERTASKEXECUTOR_C_
    #define _SIAGENTFILTERTASKEXECUTOR_C_DECLARE_ extern

#endif //_SIAGENTFILTERTASKEXECUTOR_C_



#define PID_NULL_PACKET  (0x1FFF)


#define NIT_PROGRAM_NUMBER (0x0000)



#endif //_SIAGENTFILTERTASKEXECUTOR_H_



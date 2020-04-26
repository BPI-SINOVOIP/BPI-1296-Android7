
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siAgentObject.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/10
 *
 *=======================================================================*/


#ifndef _SIAGENTOBJECT_H_
#define _SIAGENTOBJECT_H_

//#include <rtd_types.h>
#include "siAgentSubTask.h"
//#include <tp/demux_common.h>
//#include <system/sidb2/siInterface/siInterfaceHeader.h>
#include "siAgentVirSecFilterMgr.h"
#include "siAgentTypes.h"

//kjw:include following file for memory leak check(SI_AGENT_MALLOC)
//_DEBUG_SIAGENT_MEM_
#include "../SI_AgentMacros.h"


#ifdef USE_NEW_CF
#include <CFramework.h>
#else
#include <system/CFramework/CFramework.h>
#endif
//end of mem check


#include <pthread.h>


#define SIDB_HANDLE      SI_DATABASE*
#define SI_HANDLE      SI*
#define DEMUXER_HANDLE   void*

//struct SIAGENT_OBJECT_ ;
//typedef struct SIAGENT_OBJECT_       SIAGENT_OBJECT_T, *SIAGENT_OBJECT_P ;

/////////////////////   Callback Function   ///////////////////////////

typedef void *(*SI_AGENT_CBF)(SI_HANDLE, void*);
//unsigned int: agentObjectId
//void* : information

typedef enum
{
  SI_AGENT_CBF_TYPE_SCAN_CHANNEL,
  SI_AGENT_CBF_TYPE_STREAM_CHANGE_BEGIN,
  SI_AGENT_CBF_TYPE_STREAM_CHANGE_END,
  SI_AGENT_CBF_TYPE_MONITOR_PAT,
  SI_AGENT_CBF_TYPE_MONITOR_PMT,
  SI_AGENT_CBF_TYPE_MONITOR_NIT,
  SI_AGENT_CBF_TYPE_MONITOR_SDT,
  SI_AGENT_CBF_TYPE_MONITOR_EIT_PRESENT,
  SI_AGENT_CBF_TYPE_MONITOR_EIT_FOLLOWING,
  SI_AGENT_CBF_TYPE_MONITOR_EIT_SCHEDULE,
  SI_AGENT_CBF_TYPE_MONITOR_TIME,
  SI_AGENT_CBF_TYPE_FILE_RESET_POSITION,

	//ssu
  SI_AGENT_CBF_TYPE_SCAN_SSU_DONE,
  SI_AGENT_CBF_TYPE_FOUND_OAD_SW,
  SI_AGENT_CBF_TYPE_OAD_STOP,
  SI_AGENT_CBF_TYPE_UPDATE_SW_DOWNLOAD_PROGRESS,
  SI_AGENT_CBF_TYPE_UPDATE_SW_VERIFY_PROGRESS,
  SI_AGENT_CBF_TYPE_OAD_DONE,
  
  SI_AGENT_CBF_TYPE_NUM__

} SI_AGENT_CBF_TYPE;

///////////////////// siAgent ///////////////////////////
//Private Method:
void        SiAgentObj_ExecMainTaskSM( SIAGENT_OBJECT_P pSiAgentObj );
void        SiAgentObj_ExecSubTaskSM( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex );
BOOL        SiAgentObj_CheckSubTaskInQueue( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex );
SUB_TASK_P  SiAgentObj_GetSubTaskFromQueue( SIAGENT_OBJECT_P pSiAgentObj, SUB_TASK_QUEUE_INDEX subTaskQueueIndex );
void        SiAgentObj_CheckMainTaskInQueue( SIAGENT_OBJECT_P pSiAgentObj, BOOL *pResult );
void        SiAgentObj_GetMainTaskFromQueue( SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P *ppMainTask );
//Public Method:
SIDB_HANDLE SiAgentObj_GetSidbHandle( SIAGENT_OBJECT_P pSiAgentObj );
unsigned int      SiAgentObj_GetObjectId( SIAGENT_OBJECT_P pSiAgentObj );
void        SiAgentObj_Run( SIAGENT_OBJECT_P pSiAgentObj );
void        SiAgentObj_CheckAllSmIdle( SIAGENT_OBJECT_P pSiAgentObj, BOOL *pResult );
void        SiAgentObj_InsertMainTaskIntoQueue( SIAGENT_OBJECT_P pSiAgentObj, MAIN_TASK_P pMainTask );
BOOL        SiAgentObj_StopAllTask( SIAGENT_OBJECT_P pSiAgentObj );
BOOL        SiAgentObj_RegisterCBF( SIAGENT_OBJECT_P pSiAgentObj, SI_AGENT_CBF cbf, SI_AGENT_CBF_TYPE type );

struct SIAGENT_OBJECT_ {

  //Private Method:
  void (*ExecMainTaskSM)( SIAGENT_OBJECT_P );
  void (*ExecSubTaskSM)( SIAGENT_OBJECT_P , SUB_TASK_QUEUE_INDEX );
  BOOL (*CheckSubTaskInQueue)( SIAGENT_OBJECT_P , SUB_TASK_QUEUE_INDEX );
  SUB_TASK_P (*GetSubTaskFromQueue)( SIAGENT_OBJECT_P , SUB_TASK_QUEUE_INDEX );
  void (*CheckMainTaskInQueue)( SIAGENT_OBJECT_P , BOOL* );
  void (*GetMainTaskFromQueue)( SIAGENT_OBJECT_P , MAIN_TASK_P* );

  //Public Method:
  SIDB_HANDLE (*GetSidbHandle)( SIAGENT_OBJECT_P );
  unsigned int (*GetObjectId)( SIAGENT_OBJECT_P );
  void (*Run)( SIAGENT_OBJECT_P );
  void (*CheckAllSmIdle)( SIAGENT_OBJECT_P , BOOL* );
  void (*InsertMainTaskIntoQueue)( SIAGENT_OBJECT_P , MAIN_TASK_P );
  BOOL (*StopAllTask)( SIAGENT_OBJECT_P );
  BOOL (*RegisterCBF)( SIAGENT_OBJECT_P , SI_AGENT_CBF , SI_AGENT_CBF_TYPE );

  //Callback Function:
  SI_AGENT_CBF siAgentCBFs[SI_AGENT_CBF_TYPE_NUM__] ;

  //Main Task
  MAIN_TASK_HD_T     mainTaskQueue ;
  MAIN_TASK_INFO_T   execMainTaskInfo ;

  //Sub Task
  SUB_TASK_HD_T      subTaskQueue[SUB_TASK_QUEUE_NUMBER] ;
  SUB_TASK_P         pExecSubTask[SUB_TASK_QUEUE_NUMBER] ;

  //VSFM(Virtual Section Filter Manager)
  PID_LAYER_HD_T     pidLinkList ;

  //siAgent Object's ID and Name
  unsigned int agentObjectId ;//siAgent Object Address(for AP Identify)
  char   agentObjectName[16] ;//siAgent Object Name(for Print Debug Message)

  //SI Handle
  SIDB_HANDLE handleSidb;
  SI_HANDLE si;

  //Demuxer Handle
  DEMUXER_HANDLE  handleDemuxer ;

	#ifdef _EXPORT_SECTION_FILTER_MODULE
//	pthread_rwlock_t vsfmRWLock; //r/w mutex is not available

	pthread_mutex_t vsfmMutex;
	//co-operate with pidLinkList
	SF_HANDLE_POOL sfHandlePool;//store handles of each section filter

	#endif//_EXPORT_SECTION_FILTER_MODULE

	CB_PMT_PAYLOAD cbPMTPayload;	// bug 26759

    //For RtkExtractor
    pthread_mutex_t mTpModelMutex     ; // not used now
    pthread_mutex_t mSiAgentTaskMutex;
};
//typedef struct SIAGENT_OBJECT_       SIAGENT_OBJECT_T, *SIAGENT_OBJECT_P ;


SIAGENT_OBJECT_P SiAgent_CreateSiAgentObject( char*          pAgentObjectName,//max. 15 chars + '\0'
                                              SI_HANDLE    si,
                                              SIDB_HANDLE    handleSidb,
                                              DEMUXER_HANDLE handleDemuxer );

void SiAgent_DeleteSiAgentObject( SIAGENT_OBJECT_P pSiAgentObj );


#ifdef REMOVE_GLOBAL_VARIABLE
#define  CHECK_MAIN_TASK_IN_QUEUE(pAgent, pResult) {\
          pthread_mutex_lock(&pAgent->mSiAgentTaskMutex);\
          pAgent->CheckMainTaskInQueue(pAgent, pResult);\
          pthread_mutex_unlock(&pAgent->mSiAgentTaskMutex);\
          SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME);\
         }

#define  GET_MAIN_TASK_FROM_QUEUE(pAgent, ppMainTask) {\
          pthread_mutex_lock(&pAgent->mSiAgentTaskMutex);\
          pAgent->GetMainTaskFromQueue(pAgent, ppMainTask );\
          pthread_mutex_unlock(&pAgent->mSiAgentTaskMutex);\
          SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME);\
         }

#define  CHECK_ALL_SM_IDLE(pAgent, pResult) {\
          pthread_mutex_lock(&pAgent->mSiAgentTaskMutex);\
          pAgent->CheckAllSmIdle(pAgent, pResult);\
          pthread_mutex_unlock(&pAgent->mSiAgentTaskMutex);\
          SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME);\
         }

#define  INSERT_MAIN_TASK_INTO_QUEUE(pAgent, pMainTask) {\
          pthread_mutex_lock(&pAgent->mSiAgentTaskMutex);\
          pAgent->InsertMainTaskIntoQueue(pAgent, pMainTask);\
          pthread_mutex_unlock(&pAgent->mSiAgentTaskMutex);\
          SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME);\
         }
#else /* else of REMOVE_GLOBAL_VARIABLE */
#define  CHECK_MAIN_TASK_IN_QUEUE(pAgent, pResult) {\
          pthread_mutex_lock(&siAgentTaskMutex);\
          pAgent->CheckMainTaskInQueue(pAgent, pResult);\
          pthread_mutex_unlock(&siAgentTaskMutex);\
          SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME);\
         }

#define  GET_MAIN_TASK_FROM_QUEUE(pAgent, ppMainTask) {\
          pthread_mutex_lock(&siAgentTaskMutex);\
          pAgent->GetMainTaskFromQueue(pAgent, ppMainTask );\
          pthread_mutex_unlock(&siAgentTaskMutex);\
          SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME);\
         }

#define  CHECK_ALL_SM_IDLE(pAgent, pResult) {\
          pthread_mutex_lock(&siAgentTaskMutex);\
          pAgent->CheckAllSmIdle(pAgent, pResult);\
          pthread_mutex_unlock(&siAgentTaskMutex);\
          SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME);\
         }

#define  INSERT_MAIN_TASK_INTO_QUEUE(pAgent, pMainTask) {\
          pthread_mutex_lock(&siAgentTaskMutex);\
          pAgent->InsertMainTaskIntoQueue(pAgent, pMainTask);\
          pthread_mutex_unlock(&siAgentTaskMutex);\
          SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME);\
         }
#endif /* end of REMOVE_GLOBAL_VARIABLE */

//#ifdef _DEBUG_SIAGENT_MEM_
#if 1
	 #define AGENTOBJ_MALLOC(size)     SI_AGENT_MALLOC(size)
       #define AGENTOBJ_FREE(p)          SI_AGENT_FREE((p))

	#if 0
	 #include <memCheck.h>
       #define AGENTOBJ_MALLOC(size)     Memcheck_Alloc(size)
       #define AGENTOBJ_FREE(p)          do{                   \
                                            Memcheck_Free(p) ; \
                                            (p) = NULL ;       \
                                           }while(0)
	#endif//0

    #else
       #define AGENTOBJ_MALLOC(size)     malloc(size)
       #define AGENTOBJ_FREE(p)          do{             \
                                            free(p) ;    \
                                            (p) = NULL ; \
                                           }while(0)
#endif//_DEBUG_SIAGENT_MEM_

#ifdef _SIAGENTOBJECT_C_
    #define _SIAGENTOBJECT_C_DECLARE_




    //#define _DEBUG_SIAGENT_OBJECT_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SIAGENT_OBJECT_
           #define SIAGENT_OBJ_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SIAGENT_OBJECT_
           #define SIAGENT_OBJ_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SIAGENT_OBJECT_


#else //_SIAGENTOBJECT_C_
    #define _SIAGENTOBJECT_C_DECLARE_ extern

#endif //_SIAGENTOBJECT_C_




#endif //_SIAGENTOBJECT_H_










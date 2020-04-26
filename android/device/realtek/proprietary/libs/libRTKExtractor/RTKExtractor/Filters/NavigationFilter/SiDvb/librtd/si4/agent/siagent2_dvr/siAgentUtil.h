
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siAgentUtil.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/22
 *
 *=======================================================================*/


#ifndef _SIAGENTUTIL_H_
#define _SIAGENTUTIL_H_

//#include <rtd_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <tp/demux_common.h>
#include <pthread.h>
#include <time.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef BOOL
#define BOOL int
#endif


#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE

typedef enum {
  OAD_ERR_NONE  = 0,
  OAD_ERR_SCAN_NONE,
  OAD_ERR_SCAN_FIND,
//below are all download error
  OAD_ERR_NO_PAT,
  OAD_ERR_NO_PMT,
  OAD_ERR_NO_SSU_ES,
  OAD_ERR_NO_DSI,
  OAD_ERR_NO_GROUP,
  OAD_ERR_NOT_COMPATIBLE,
  OAD_ERR_NO_DII,
  OAD_ERR_DII_VER_CHG,
  OAD_ERR_DDB_TIMEOUT,
  OAD_ERR_FILTER_DDB_DII_VER_CHG,
  OAD_ERR_MODULE_CRC32_ERROR,
  OAD_ERR_NO_TARGET_FILE,		//can't write to target file
  OAD_ERR_READ_FILE,
} OAD_ERROR_CODE ;


#endif//#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE


//SIAGENT_THREAD_SLEEP(unsigned int ms)
#define SIAGENT_THREAD_SLEEP(ms) {\
         struct timespec delay ;\
         delay.tv_sec = ms / 1000 ;\
         delay.tv_nsec = ((ms % 1000)*1000000) ;\
         nanosleep(&delay,NULL) ;\
        }

void  SiAgtUtil_HexDump(unsigned char *ptr, unsigned int len , void* address);
int SiAgtUtil_kbhit(void);

time_t SiAgent_Time(void);


#define SIAGENT_THREAD_SLEEP_TIME 1
#ifdef REMOVE_GLOBAL_VARIABLE
//void SiAgtUtil_SiAgentTaskMutexInit(SIAGENT_OBJECT_P pSiAgentObj);
//void SiAgtUtil_SiAgentTaskMutexDestroy(SIAGENT_OBJECT_P pSiAgentObj);
void SiAgtUtil_TpModelMutexInit(void *agent);
void SiAgtUtil_TpModelMutexDestroy(void *agent);
void SiAgtUtil_SiAgentTaskMutexInit(void *agent);
void SiAgtUtil_SiAgentTaskMutexDestroy(void *agent);
#else /* else of REMOVE_GLOBAL_VARIABLE */
void SiAgtUtil_TpModelMutexInit(void);
void SiAgtUtil_TpModelMutexDestroy(void);
void SiAgtUtil_SiAgentTaskMutexInit(void);
void SiAgtUtil_SiAgentTaskMutexDestroy(void);
#endif /* end of REMOVE_GLOBAL_VARIABLE */


#ifdef _RUN_ON_TP_MODEL_
    #define TP_MODEL_MUTEX_LOCK() { \
             pthread_mutex_lock(&tpModelMutex);\
            }
    #define TP_MODEL_MUTEX_UNLOCK() { \
              pthread_mutex_unlock(&tpModelMutex); \
              SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME); \
            }
#else //_RUN_ON_TP_MODEL_
    #define TP_MODEL_MUTEX_LOCK()
    #define TP_MODEL_MUTEX_UNLOCK()
#endif //_RUN_ON_TP_MODEL_


#define ACCESS_SI_MUTEX_LOCK() { \
         pthread_mutex_lock(&siAgentTaskMutex);\
        }
#define ACCESS_SI_MUTEX_UNLOCK() { \
         pthread_mutex_unlock(&siAgentTaskMutex); \
         SIAGENT_THREAD_SLEEP(SIAGENT_THREAD_SLEEP_TIME); \
        }


#ifdef _SIAGENTUTIL_C_
    #define _SIAGENTUTIL_C_DECLARE_



    #if 0
       #include <memCheck.h>
       #define UTIL_MALLOC(size)     Memcheck_Alloc(size)
       #define UTIL_FREE(p)          do{                   \
                                        Memcheck_Free(p) ; \
                                        (p) = NULL ;       \
                                       }while(0)
    #else
       #define UTIL_MALLOC(size)     malloc(size)
       #define UTIL_FREE(p)          do{             \
                                        free(p) ;    \
                                        (p) = NULL ; \
                                       }while(0)
    #endif


    #define _DEBUG_SIAGENT_UTIL_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #if 0 //def _DEBUG_SIAGENT_UTIL_
           #define SIAGENT_UTIL_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SIAGENT_UTIL_
           #define SIAGENT_UTIL_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SIAGENT_UTIL_

#else //_SIAGENTUTIL_C_
    #define _SIAGENTUTIL_C_DECLARE_ extern

#endif //_SIAGENTUTIL_C_


#ifdef REMOVE_GLOBAL_VARIABLE
#else /* else of REMOVE_GLOBAL_VARIABLE */
_SIAGENTUTIL_C_DECLARE_ pthread_mutex_t tpModelMutex     ;
_SIAGENTUTIL_C_DECLARE_ pthread_mutex_t siAgentTaskMutex ;
#endif /* end of REMOVE_GLOBAL_VARIABLE */

	#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
	_SIAGENTUTIL_C_DECLARE_ unsigned int swDownloadProgress ;//Percent
	_SIAGENTUTIL_C_DECLARE_ unsigned int swVerifyProgress   ;//Percent
	_SIAGENTUTIL_C_DECLARE_ OAD_ERROR_CODE oadErrorCode ;
	#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

#endif //_SIAGENTUTIL_H_














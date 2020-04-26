#ifndef __OSAL_THREADS_H__
#define __OSAL_THREADS_H__

/* OSAL - process, thread, synchronization related definitions */

#ifdef WIN32
typedef long HRESULT;
#endif
#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Basic Data Types */
#ifdef WIN32
typedef struct tag_osal_thread_t {
    unsigned char dummy[96];
}osal_thread_t; 

typedef struct tag_osal_mutex_t {
    unsigned char dummy[32];
}osal_mutex_t;

typedef struct tag_osal_event_t {
    unsigned char dummy[80];
}osal_event_t;

typedef struct tag_osal_sem_t {
    unsigned char dummy[88];
}osal_sem_t;
#else
typedef struct tag_osal_thread_t {
    unsigned char dummy[96];
}__attribute__ ((__aligned__ (4)))osal_thread_t; 

typedef struct tag_osal_mutex_t {
    unsigned char dummy[32];
}__attribute__ ((__aligned__ (4))) osal_mutex_t;

typedef struct tag_osal_event_t {
    unsigned char dummy[80];
}__attribute__ ((__aligned__ (4))) osal_event_t;

typedef struct tag_osal_sem_t {
    unsigned char dummy[88];
}__attribute__ ((__aligned__ (4))) osal_sem_t;
#endif

/* Thread APIs */

HRESULT osal_ThreadBegin(
            void (*start_address)(void*),
            void* arglist,
            unsigned long stack_size,
            long priority,
            osal_thread_t* pThreadID);

HRESULT osal_ThreadEnd(
            osal_thread_t* pThreadID,
            long millisecondsMaxWait);

HRESULT osal_ThreadDetach(
            osal_thread_t* pThreadID);

HRESULT osal_ThreadExit(void);

HRESULT osal_ThreadCancel(
            osal_thread_t* pThreadID);

void osal_Sleep(long millisecondsSleep);

/* Synchronization APIs */

/* Mutex */

HRESULT osal_MutexCreate(
            osal_mutex_t* pMutexID);

HRESULT osal_MutexLock(
		       osal_mutex_t* pMutexID);

HRESULT osal_MutexTimedLock(
			    osal_mutex_t* pMutexID,
			    int nseconds
			    );
  
  /* function will boost the priority to wait on mutex, and return to original priority after get the mutex */
HRESULT osal_MutexLock_BoostPriority(
    osal_mutex_t* pMutexID, 
    int priority);     

HRESULT osal_MutexTryLock(
            osal_mutex_t* pMutexID);

HRESULT osal_MutexUnlock(
            osal_mutex_t* pMutexID);

HRESULT osal_MutexDestroy(
            osal_mutex_t* pMutexID);

/* Event */

HRESULT osal_EventCreate(
            osal_event_t* pEventID);

HRESULT osal_EventTest(
            osal_event_t* pEventID);

HRESULT osal_EventSet(
            osal_event_t* pEventID);

HRESULT osal_EventReset(
            osal_event_t* pEventID);

HRESULT osal_EventWait(
            osal_event_t* pEventID,
            long millisecondsTimeout);

HRESULT osal_EventDestroy(
            osal_event_t* pEventID);

/* Semaphore */

HRESULT osal_SemCreate(
            unsigned int maxCount,
            unsigned int initialCount,
            osal_sem_t* pSemID);

HRESULT osal_SemGive(
            osal_sem_t* pSemID);

HRESULT osal_SemWait(
            osal_sem_t* pSemID,
            long millisecondsTimeout);

HRESULT osal_SemTryWait(
            osal_sem_t* pSemID);

HRESULT osal_SemTest(
            osal_sem_t* pSemID);

HRESULT osal_SemSetCount(
            osal_sem_t* pSemID,
            unsigned int count);

HRESULT osal_SemDestroy(
            osal_sem_t* pSemID);

#ifdef __cplusplus
}
#endif

#endif /*__OSAL_THREADS_H__*/

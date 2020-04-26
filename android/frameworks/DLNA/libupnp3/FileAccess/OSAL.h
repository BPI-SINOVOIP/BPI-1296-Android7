#ifndef __OSAL__
#define __OSAL__

#ifndef __cplusplus
typedef int bool;
#endif

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

bool osal_MutexCreate(
            osal_mutex_t* pMutexID);

bool osal_MutexLock(
		       osal_mutex_t* pMutexID);
               
bool osal_MutexDestroy(
            osal_mutex_t* pMutexID);
#endif //!__OSAL__
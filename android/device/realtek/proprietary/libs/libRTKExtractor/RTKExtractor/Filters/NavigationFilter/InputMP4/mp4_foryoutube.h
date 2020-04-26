#ifndef ___MP4_FORYOUTUBE_H__
#define ___MP4_FORYOUTUBE_H__

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef pli_allocContinuousMemory
#undef pli_allocContinuousMemory
#endif
#define pli_allocContinuousMemory 	mp4_allocContinuousMemory

#ifdef pli_freeContinuousMemory
#undef pli_freeContinuousMemory
#endif
#define pli_freeContinuousMemory	mp4_freeContinuousMemory

#ifdef pli_flushMemory
#undef pli_flushMemory
#endif
#define pli_flushMemory 			mp4_flushMemory

#ifdef osal_MutexLock
#undef osal_MutexLock
#endif
#define osal_MutexLock	 			mp4_osal_MutexLock

#ifdef osal_MutexUnlock
#undef osal_MutexUnlock
#endif
#define osal_MutexUnlock 			mp4_osal_MutexUnlock

void *mp4_allocContinuousMemory(unsigned long size, unsigned char **nonCachedAddr, unsigned long *phyAddr);

void mp4_freeContinuousMemory(void *ptr);

void mp4_flushMemory(void *ptr, long size);

void mp4_osal_MutexLock(void *mutex);

void mp4_osal_MutexUnlock(void *mutex);

#ifdef __cplusplus
}
#endif

#endif /*___MP4_FORYOUTUBE_H__*/


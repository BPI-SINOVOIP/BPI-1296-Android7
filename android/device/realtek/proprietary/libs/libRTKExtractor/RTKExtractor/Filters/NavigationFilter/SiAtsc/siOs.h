#ifndef _SI_OS_H_
#define _SI_OS_H_

#if defined(WIN32) | defined(DVR)
	#include "rtd_types.h"
#else
	#include <rtd_types.h>
#endif




#ifdef WIN32
	#define MUTEX				HANDLE
	#define INIT_MUTEX(m, name)	m = CreateMutex(NULL, FALSE, name);
	#define LOCK_MUTEX(m)		WaitForSingleObject(m, INFINITE);
	#define UNLOCK_MUTEX(m)		ReleaseMutex(m);
	#define DEINIT_MUTEX(m)		{CloseHandle(m); m = NULL;}
#else
	#define MUTEX				pthread_mutex_t
	#define INIT_MUTEX(m, name)	pthread_mutex_init(&m, NULL);
	#define LOCK_MUTEX(m)		pthread_mutex_lock(&m);
	#define UNLOCK_MUTEX(m)		pthread_mutex_unlock(&m);
	#define DEINIT_MUTEX(m)		pthread_mutex_destroy(&m);
#endif



#endif

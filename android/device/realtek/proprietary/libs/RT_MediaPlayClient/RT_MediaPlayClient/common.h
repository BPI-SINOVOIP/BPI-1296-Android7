#ifndef RTMEDIAPLAYCLIENT_COMMON_H_
#define RTMEDIAPLAYCLIENT_COMMON_H_


//#define NO_THREAD
#ifdef NO_THREAD
	#define MUTEX_INIT(_mtx)
	#define MUTEX_DESTROY(_mtx)
	#define MUTEX_LOCK(_mtx)
	#define MUTEX_UNLOCK(_mtx)
#else
	#include <pthread.h>
	#define MUTEX_INIT(_mtx)	do{pthread_mutex_init((_mtx), NULL);}while(0)
	#define MUTEX_DESTROY(_mtx) do{pthread_mutex_destroy((_mtx));   }while(0)
	#define MUTEX_LOCK(_mtx)	do{pthread_mutex_lock((_mtx));      }while(0)
	#define MUTEX_UNLOCK(_mtx)  do{pthread_mutex_unlock((_mtx));    }while(0)
#endif

#define RTCLIENT_BITSTREAM_BUF_SIZE	(8*1024*1024)
#define RTCLIENT_COMMAND_BUF_SIZE   (256*1024)

#endif /* RTMEDIAPLAYCLIENT_COMMON_H_ */

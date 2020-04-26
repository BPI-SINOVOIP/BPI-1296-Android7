//-----------------------------------------------------------------------------
// SonicMutex.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SonicMutex_h__
#define __SonicMutex_h__

#include <OSAL.h>

class SonicMutex
{
public:
	SonicMutex() { osal_MutexCreate(&mutex);}
	~SonicMutex() { osal_MutexDestroy(&mutex);}
	

	inline void lock()		{osal_MutexLock(&mutex); }
	inline void unlock()	{osal_MutexUnlock(&mutex);}
	
private:
	osal_mutex_t	mutex;
};

#endif // __SonicMutex_h__

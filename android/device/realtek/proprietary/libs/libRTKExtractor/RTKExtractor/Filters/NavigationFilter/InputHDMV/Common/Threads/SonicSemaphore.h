//-----------------------------------------------------------------------------
// SonicSemaphore.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SonicSemaphore__h__
#define __SonicSemaphore__h__

#include <OSAL.h>

class SonicSemaphore
{

public:

	SonicSemaphore(UInt32 initialCount = 0, UInt32 maximumCount = 1)
	{
		osal_SemCreate(maximumCount, initialCount, &m_sem);
	}
	
	~SonicSemaphore()	{osal_SemDestroy(&m_sem);}

	inline void Signal()	{osal_SemGive(&m_sem);}
	
	inline bool Wait(long millisecondsTimeout = TIMES_INFINITY)	
	{
		if(millisecondsTimeout == 0)	
			return (osal_SemTryWait(&m_sem) == S_OK) ? true : false;	
		else
			return (osal_SemWait(&m_sem, millisecondsTimeout) == S_OK) ? true : false;	
			
	}
private:

	osal_sem_t	m_sem;

};

#endif // __SonicSemaphore_h__

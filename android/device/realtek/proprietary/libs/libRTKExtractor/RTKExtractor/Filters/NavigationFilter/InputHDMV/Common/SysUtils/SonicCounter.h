//-----------------------------------------------------------------------------
// SonicCounter.h
// Copyright (c) 2004-2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SonicCounter_h__
#define __SonicCounter_h__

#include "SystemHeaders.h"

//#include "atomic_operations.h"
#include "SonicCriticalSection.h"

class SonicCounter // thread-safe counter
{

public:

	// NOTE: on Linux and some versions of Windows, the results of Decrement() and Increment()
	// reflect the actual result of the operation ONLY when a value of zero is returned.
	// Other values may reflect the results of concurrent operations.

	SonicCounter(SInt32 count = 0) : m_count(count) {}
	SInt32 Count() const { return m_count; }
	SInt32 Decrement() {SonicAutoCriticalSection Lock(&m_counterLock); m_count--; return m_count; }
	SInt32 Increment() {SonicAutoCriticalSection Lock(&m_counterLock); m_count++; return m_count; }

private:
	SonicCriticalSection		m_counterLock;
	volatile SInt32 			m_count;
};

#endif //__SonicCounter_h__


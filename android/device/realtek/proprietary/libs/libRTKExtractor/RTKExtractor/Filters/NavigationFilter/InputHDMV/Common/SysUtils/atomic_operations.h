//-----------------------------------------------------------------------------
// atomic_operations.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __atomic_operations_h__
#define __atomic_operations_h__

#include "CommonTypes.h"
#include <cassert>
#include "stddef.h"

#if defined(_WIN32)
	#include <windows.h>
#elif defined(OSXPORT)
	extern "C" SInt32 IncrementAtomic(SInt32 * value); // return: value BEFORE increment
	extern "C" SInt32 DecrementAtomic(SInt32 * value); // return: value BEFORE decrement
#elif defined(__unix__) && !defined (__atomic_h__) && !defined(NO_ATOMIC_H)

	#include <asm/atomic.h>
	
	#undef  __atomic_h__
	#define __atomic_h__
#endif

#include <cassert>

namespace atomic
{

#if defined(_WIN32)

	inline SInt32 decrement(volatile SInt32 * address) { assert(((ptrdiff_t) address & 0x3) == 0); return static_cast<SInt32>(InterlockedDecrement((volatile LONG *) address)); }
	inline SInt32 increment(volatile SInt32 * address) { assert(((ptrdiff_t) address & 0x3) == 0); return static_cast<SInt32>(InterlockedIncrement((volatile LONG *) address)); }

#elif defined(OSXPORT)

	inline SInt32 decrement(volatile SInt32 * address) { assert(((ptrdiff_t) address & 0x3) == 0); return DecrementAtomic((SInt32*)address) - 1; }
	inline SInt32 increment(volatile SInt32 * address) { assert(((ptrdiff_t) address & 0x3) == 0); return IncrementAtomic((SInt32*)address) + 1; }

#elif defined(__atomic_h__)

	inline SInt32 decrement(volatile SInt32 * address) { assert(((ptrdiff_t) address & 0x3) == 0); return atomic_dec_and_test((atomic_t *) address) ? 0 : *address; }
	inline SInt32 increment(volatile SInt32 * address) { assert(((ptrdiff_t) address & 0x3) == 0); return atomic_inc_and_test((atomic_t *) address) ? 0 : *address; }

#elif defined(__GNUC__)

	inline SInt32 decrement(volatile SInt32 * address) { assert(((ptrdiff_t) address & 0x3) == 0); return static_cast<SInt32>(__sync_sub_and_fetch((int *) address)); }
	inline SInt32 increment(volatile SInt32 * address) { assert(((ptrdiff_t) address & 0x3) == 0); return static_cast<SInt32>(__sync_add_and_fetch((int *) address)); }

#else
#error no implementation
#endif

}

#endif // __atomic_operations_h__

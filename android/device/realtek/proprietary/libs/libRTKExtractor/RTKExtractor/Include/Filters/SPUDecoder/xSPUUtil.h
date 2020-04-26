#ifndef __SPU_UTIL__
#define __SPU_UTIL__

#include <assert.h>

#define SPUassert(exp) 				\
	if (0)							\
		assert(exp);				\
	else							\
		if (!(exp))					\
			printf("[%s:%d]%s() failed -------------------------------------> :(\n", __FILE__, __LINE__, __func__);	\
			
#define SPUPrintf(exp)				\
	if (0)							\
			printf exp ;			\
	else							\
			0						\

#endif


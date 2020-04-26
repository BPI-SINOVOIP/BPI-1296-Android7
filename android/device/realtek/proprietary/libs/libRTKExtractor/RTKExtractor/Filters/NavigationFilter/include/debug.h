
#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include <stdio.h>
#include <assert.h>

#define db_print(fmt, args...)    { printf("%s:%d:\t\t", __FILE__, __LINE__); printf(fmt, ## args); }
#define db_warning(fmt, args...)  { printf("Warning: %s:%d:\t\t", __FILE__, __LINE__); printf(fmt, ## args); }

#define db_assert(expr)            assert((expr))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // #ifndef __DEBUG_H__

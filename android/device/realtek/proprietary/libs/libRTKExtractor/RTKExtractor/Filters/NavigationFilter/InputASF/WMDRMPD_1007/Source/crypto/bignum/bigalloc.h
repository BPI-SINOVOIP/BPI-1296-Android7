/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
/*
    File bigalloc.h

    This file has sample versions of files which
    should be provided by the application program:

    void* bignum_alloc(size_t cblen)
    void* bignum_realloc(void * pvMem, size_t cblen)
    void  bignum_free(void *pvMem)

    These operate analogously to the standard
    malloc, realloc, free.
    Memory contents are undefined after a malloc.

    If these definitions are satisfactory, then
    #include "bigalloc.h" somewhere in
    the application program, typically near
    the main program,
*/

#ifndef BIGALLOC_H
#define BIGALLOC_H 1

#include "bigdefs.h"
#include <stdio.h>
#include <stdlib.h>

/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif



#if TARGET_OS == OS_WNT && !defined(OLD_CODE)
// the OLD_CODE test is temporary,
// until petmon gets the code to work on his laptop.
// InterlockedCompareExchangePointer is new to Whistler,
//  November, 2000

#include <windows.h>

//
// Multi-threaded memory allocation support for NT.
//

#ifdef _X86_
#define InterlockedAccess(pl) *(pl)
#define InterlockedPointerAccess(ppv) *(ppv)
#else
#define InterlockedAccess(pl) InterlockedExchangeAdd((pl), 0)
#define InterlockedPointerAccess(ppv) InterlockedExchangePointer((ppv), *(ppv))
#endif

static HANDLE l_hHeap = NULL;

LPVOID
bignum_alloc(
    const size_t cbLen)
{
    HANDLE hHeap = InterlockedPointerAccess(&l_hHeap);

    if (NULL == hHeap)
    {
        hHeap = HeapCreate(0, 0, 0);
        if (NULL == hHeap)
            return NULL;

        if (NULL != InterlockedCompareExchangePointer(&l_hHeap, hHeap, NULL))
        {
            HeapDestroy(hHeap);
            hHeap = InterlockedPointerAccess(&l_hHeap);
        }
    }

    return HeapAlloc(hHeap, HEAP_ZERO_MEMORY, cbLen);
}

LPVOID
bignum_realloc(
    LPVOID pvMem,
    const size_t cbLen)
{
    return HeapReAlloc(l_hHeap, HEAP_ZERO_MEMORY, pvMem, cbLen);
}

void
bignum_free(
    LPVOID pvMem)
{
    HeapFree(l_hHeap, 0, pvMem);
}

#elif 0    // Case not now used

void *bignum_alloc(const size_t cblen)
{
    return LocalAlloc(LMEM_FIXED, cblen);
}

// bignum_realloc presently undefined

void bignum_free(void *pvMem)
{
    LocalFree((HLOCAL)pvMem);
}

#else /* Other operating systems */

void* DRM_API bignum_alloc(const size_t cblen)
{
    return malloc(cblen);
}

void* DRM_API bignum_realloc(void *pvMem, const size_t cblen)
{
    return realloc(pvMem, cblen);
}

void DRM_API bignum_free(void *pvMem)
{
    free(pvMem);
}

#endif /* operating system */

#ifdef __cplusplus
}
#endif


#endif // BIGALLOC_H

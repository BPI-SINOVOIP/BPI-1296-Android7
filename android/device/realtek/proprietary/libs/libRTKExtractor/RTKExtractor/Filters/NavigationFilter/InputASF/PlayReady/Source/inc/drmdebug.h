/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMDEBUG_H__
#define __DRMDEBUG_H__

#include <oemdebug.h>

ENTER_PK_NAMESPACE;

/*************************************************************************
*
*   debug printf macro
*   Sample calling seq:
*
*   TRACE(("My name is %s", "DRM"));
*
*   Note the double parenthesis.
*************************************************************************/
#if DBG

    #undef TRACE
    #define TRACE(x) do { Oem_Debug_Trace((DRM_CHAR *)"TRACE at %s(%d): ", __FILE__, __LINE__); Oem_Debug_Trace x; Oem_Debug_Trace((DRM_CHAR *)"\r\n"); } while( FALSE )

    #undef TRACE_IF_FAILED
    #define TRACE_IF_FAILED(x) do { if ( DRM_FAILED( dr ) ){ TRACE(x); } } while ( FALSE )

#else  // DBG

    #ifndef TRACE
    #define TRACE(x) 
    #endif
    
    #ifndef TRACE_IF_FAILED
    #define TRACE_IF_FAILED(x) 
    #endif

#endif  // DBG


/* 
** Compile-time asserts cause PREfast warnings regarding the comparison of two constants.
** So, enable this macro only when the PREfast tool is not analyzing the code.
*/
#ifndef _PREFAST_ 
    #define DRMSIZEASSERT(x,y)  \
        {switch(0){case ((x)==(y)?0:(y)):case (y):;}} \
        {switch(0){case ((y)==(x)?0:(x)):case (x):;}}
#else
    #define DRMSIZEASSERT(x,y)
#endif


#define DRM_OFFSET_OF(struc,member)   (DRM_DWORD_PTR)&(((struc *)0)->member)
#define ENSURE_MEMBERS_ADJACENT(struc,member1,member2)  ((DRM_OFFSET_OF(struc,member1)+(DRM_DWORD)(SIZEOF(((struc *)0)->member1))) == DRM_OFFSET_OF(struc,member2))

#if DBG
    #undef DRMASSERT
    /* Call the global assert handler function. */
    #define DRMASSERT(x) Oem_Debug_Assert((x), (DRM_CHAR *)#x, (DRM_CHAR *)__FILE__, __LINE__)
    
#else
    #ifndef DRMASSERT
        #define DRMASSERT
    #endif
    #ifndef DRMCASSERT
        #define DRMCASSERT
    #endif
#endif

EXIT_PK_NAMESPACE;

#endif  /* __DRMDEBUG_H__ */


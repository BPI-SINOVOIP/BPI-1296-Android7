/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMDEBUG_H__
#define __DRMDEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************
*
*   debug printf macro
*   Sample calling seq:  -- NOTE double paranthesis for printf args. --
*   TRACE(("My name is %s\n", "DRM"));
*
*************************************************************************/
#if DBG

    #undef TRACE
    #define TRACE(x) g_TraceCallback x     

#else  // DBG

    #ifndef TRACE
    #define TRACE(x) 
    #endif

#endif  // DBG


typedef void (DRM_API *fncdrmtrace)(DRM_CHAR*, ... );
extern fncdrmtrace g_TraceCallback;

/* This type is to allow the ASSERT macro to have another implementation
	defined by the caller of DrmDebugHookAssert */
		
typedef void (DRM_API *fncdrmassert)(DRM_BOOL,DRM_CHAR*, DRM_CHAR*,DRM_INT);


/* The global current assert funtion pointer. */
extern fncdrmassert g_AssertCallback;
extern fncdrmassert g_OEMAssertCallback;

/**********************************************************************
** Function:	DrmDebugHookAssert
**
** Synopsis:	Change the global assert handler to one of the callers choosing.
**
** Arguments:	[pNewAssertFcn] -- Ptr to a function that handles asserts.
**
** Returns:		None
**
** Notes:		
**
***********************************************************************/
DRM_VOID DRM_API DrmDebugHookAssert(IN fncdrmassert pNewAssertFcn);

/**********************************************************************
** Function:	DrmDebugHookTrace
**
** Synopsis:	Change the global trace handler to one of the callers choosing.
**
** Arguments:	[pNewTraceFcn] -- Ptr to a function that handles traces.
**
** Returns:		None
**
** Notes:		
**
***********************************************************************/
DRM_VOID DRM_API DrmDebugHookTrace(IN fncdrmtrace pNewTraceFcn);

#define DRMSIZEASSERT(x,y)  \
    {switch(0){case ((x)==(y)?0:(y)):case (y):;}} \
    {switch(0){case ((y)==(x)?0:(x)):case (x):;}} \


#define DRM_OFFSET_OF(struc,member)   (DRM_DWORD_PTR)&(((struc *)0)->member)
#define ENSURE_MEMBERS_ADJACENT(struc,member1,member2)  ((DRM_OFFSET_OF(struc,member1)+(DRM_DWORD)(SIZEOF(((struc *)0)->member1))) == DRM_OFFSET_OF(struc,member2))

/* Always use compile time asserts */
#define DRMCASSERT( x ) switch(0){case 0:case (x):;}

#if DBG
    #undef DRMASSERT
    /* Call the global assert handler function. */
    #define DRMASSERT(x) g_AssertCallback((x), #x, __FILE__, __LINE__)
    
#else
    #ifndef DRMASSERT
        #define DRMASSERT
    #endif
    #ifndef DRMCASSERT
        #define DRMCASSERT
    #endif
#endif

#ifdef __cplusplus
}
#endif


#endif  /* __DRMDEBUG_H__ */


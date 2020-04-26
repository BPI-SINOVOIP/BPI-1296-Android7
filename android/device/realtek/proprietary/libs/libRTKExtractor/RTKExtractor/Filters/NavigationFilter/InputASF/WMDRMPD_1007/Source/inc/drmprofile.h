/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMPROFILE_H__
#define __DRMPROFILE_H__

#include <drmcommon.h>
#include <drmresults.h>
#include <drmtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DRM_PROFILE_USETICKCOUNT 0

/* The following defines allow individual profiling calls to be enabled or disabled */
#define DRM_PROFILE_DRIVESEEK 0
#define DRM_PROFILE_HDSGETPUT 0


#define DRM_PROFILE_BUFFER_SIZE 1024
typedef struct
{
    OEM_FILEHDL   hfile;
    DRM_BOOL      fRunning;
    DRM_DWORD     dwDepth;
    DRM_DWORD     dwNumSeeks;
    DRM_CHAR      rgchBuffer [__CB_DECL(DRM_PROFILE_BUFFER_SIZE)];
} DRM_PROFILE_CONTEXT;

typedef struct
{
#if DRM_PROFILE_USETICKCOUNT
    DRM_DWORD       dwTickCount;
#else
    DRMSYSTEMTIME   enterTime;
#endif
    DRM_DWORD       dwNumSeeks;

} DRM_SCOPE_CONTEXT;

extern DRM_PROFILE_CONTEXT g_contextPRO;

#if DRM_SUPPORT_PROFILING

DRM_RESULT DRM_API DRM_PRO_StartProfiling(void);

DRM_RESULT DRM_API DRM_PRO_StopProfiling(void);

#define DRM_PROFILING_START_IF_NOT_STARTED TRUE
#define DRM_PROFILING_DONT_START           FALSE

DRM_RESULT DRM_API DRM_PRO_EnterScope(
    IN DRM_SCOPE_CONTEXT *f_pScopeCtx,
    IN const DRM_WCHAR  *const f_pwszScope,
    IN const DRM_WCHAR  *const f_pwszComment,
    IN DRM_BOOL       f_fRunIfNot);

DRM_RESULT DRM_API DRM_PRO_LeaveScope(
    IN DRM_SCOPE_CONTEXT *f_pScopeCtx,
    IN const DRM_WCHAR  *const f_pwszScope,
    IN const DRM_WCHAR  *const f_pwszComment);

#define DRM_PROFILING_ENTER_SCOPE(s,c,r) \
    DRM_SCOPE_CONTEXT xqrK224; \
    DRM_PRO_EnterScope(&xqrK224,(const DRM_WCHAR *const)(s),(const DRM_WCHAR *const)(c),(r)); 

#define DRM_PROFILING_LEAVE_SCOPE(s,c) \
    DRM_PRO_LeaveScope(&xqrK224,(const DRM_WCHAR *const)(s),(const DRM_WCHAR *const)(c)); 

extern const DRM_WCHAR *const g_pwszEnteringFunction;
extern const DRM_WCHAR *const g_pwszLeavingFunction;

#else
#define DRM_PRO_StartProfiling()   
#define DRM_PRO_StopProfiling()
#define DRM_PRO_EnterScope(a,x,c,b)
#define DRM_PRO_LeaveScope(a,b,c)
#define DRM_PROFILING_ENTER_SCOPE(a,s,c) 
#define DRM_PROFILING_LEAVE_SCOPE(a,c) 
#endif

#ifdef __cplusplus
}
#endif

#endif      /* __XMLBUILDER_H__ */



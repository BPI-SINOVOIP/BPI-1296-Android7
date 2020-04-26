/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMPROFILE_H__
#define __OEMPROFILE_H__

ENTER_PK_NAMESPACE;

#if DRM_SUPPORT_PROFILING

#include <drmprofile.h>

DRM_API PERF_GLOBAL_CONTEXT * DRM_CALL Oem_Profiling_GetProfileContext(DRM_VOID); 

/**********************************************************************
** Function:    Oem_Profiling_QueryPerformanceCounter
** Synopsis:    Returns the value of a high performance counter if one 
**              exists on the system, otherwise it works like Oem_Clock_GetTickCount()
** Parameters:  None
** Returns:     A DRM_UINT64 with the value of the system's counter.
***********************************************************************/
DRM_API DRM_UINT64 DRM_CALL Oem_Profiling_QueryPerformanceCounter( DRM_VOID );

/**********************************************************************
** Function:    Oem_Profiling_QueryPerformanceFrequency
** Synopsis:    Returns the frequency of the high performance counter  
**              in clocks per second.
** Parameters:  None
** Returns:     A DRM_UINT64 representing the number of ticks per second
**              used by Oem_Profiling_QueryPerformanceCounter.
***********************************************************************/
DRM_API DRM_UINT64 DRM_CALL Oem_Profiling_QueryPerformanceFrequency( DRM_VOID );

#endif  /* DRM_SUPPORT_PROFILING */

EXIT_PK_NAMESPACE;

#endif /* __OEMPROFILE_H__ */

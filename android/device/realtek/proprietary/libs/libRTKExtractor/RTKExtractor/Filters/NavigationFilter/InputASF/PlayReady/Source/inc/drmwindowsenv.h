/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_WINDOWS_ENV_H__
#define __DRM_WINDOWS_ENV_H__

/*  
# Abstract:
#
# This file contains compiler directives for disabling certain Windows CE 
# compiler-time warnings and allow the PK environment to be compiled under 
# elevated warning level
#
*/

#if defined (_MSC_VER)

#if defined (WINCE)
#pragma warning( push )
#pragma warning(disable:4115) /* 'type' : named type definition in parentheses */
#pragma warning(disable:4201) /* nonstandard extension used : nameless struct/union */
#pragma warning(disable:4214) /* nonstandard extension used : bit field types other than int */
#endif

#include <windows.h> 

#if defined (WINCE)
#pragma warning( pop )
#endif

#endif /* _MSC_VER */

#endif /* __DRM_WINDOWS_ENV_H__ */





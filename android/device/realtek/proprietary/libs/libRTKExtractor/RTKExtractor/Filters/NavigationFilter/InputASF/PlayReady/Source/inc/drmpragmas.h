/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMPRAGMAS_H__
#define __DRMPRAGMAS_H__

/*  
# Abstract:
#
# This file contains compiler directives for disabling certain compiler-time warnings
# and allow the PK environment to be compiled under elevated warning level
#
# For example, Microsoft compiler can use warning level /W4
#
*/

#if defined (_MSC_VER)

/* 
** need to ignore unknown pragmas if not using prefast 
*/
#ifndef _PREFAST_
    #pragma warning(disable:4068)
#endif

/*  
** Pragmas to disable. These /W4 warnings are expected. 
*/
#pragma warning(disable:4100) /* unreferenced formal parameter */
#pragma warning(disable:4127) /* "conditional expression is constant" */
#pragma warning(disable:4244) /* conversion from 'type1' to 'type2', possible loss of data */
#pragma warning(disable:4245) /* conversion from 'type1' to 'type2', signed/unsigned mismatch */
#pragma warning(disable:4505) /* unreferenced local function has been removed */
#pragma warning(disable:4510) /* default constructor not possible for structure w/const field */
#pragma warning(disable:4512) /* assignment operator not possible for struct w/const field */
#pragma warning(disable:4610) /* struct non-instantiatable due to const field */
#pragma warning(disable:4152) /* nonstandard extension, function/data pointer conversion in expression */

/*
** some W4 warnings are not fixed in PC code
*/
#if DRM_BUILD_PROFILE == DRM_BUILD_PROFILE_PC
#pragma warning(disable:4204) /* nonstandard extension used : non-constant aggregate initializer */
#pragma warning(disable:4189) /* local variable is initialized but not referenced */
#pragma warning(disable:4706) /* assignment within conditional expression */
#endif

/* 
** amd64 and ia64 specific errors 
*/
#if defined(_M_AMD64) || defined(_M_IA64)
#pragma warning(disable:4152) /* nonstandard extension, function/data pointer conversion in expression */
#pragma warning(disable:4306) /* 'type cast' : conversion from 'type1' to 'type2' of greater size */
#pragma warning(disable:4366) /* The result of the unary operator may be unaligned */
#endif /* _M_AMD64 */

#endif /* _MSC_VER */

#endif  /* __DRMPRAGMAS_H__ */

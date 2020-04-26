/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCOMMON_H__
#define __DRMCOMMON_H__

#include "drmfeatures.h"


#define DRM_MAX_ACTIONS 10

#define VERSION_LEN          4
#define DRM_MAX_PATH         256

/*Temporary holding place for these constants */
#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifndef NO_OF
    #define NO_OF( x )  ( SIZEOF( (x) ) / SIZEOF( (x)[0] ) )
#endif

#ifndef ISODD
    #define ISODD(x) ((x)&1)
#endif

#define DRM_SAFE_INT (int)0x0fffffff
#define DRM_SAFE_UINT (unsigned int)0x0fffffff
#define DRM_UINT_MAX (unsigned int)0xffffffff
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define ROTATE_RIGHT(x, n) (((x) >> (n)) | ((x) << (32-(n))))
#define WORDSWAP(d) ( ( (d) >> 16 ) + ( (d) << 16 ) )

/* for good parameter documentation */
#define IN
#define OUT

#include "drmpragmas.h"     /* Handling of compilation warnings */
#include "drmnamespace.h"   /* Defining proper namespace (if used)                 */
#include "drmtypes.h"       /* Core DRM types */
#include "drmresults.h"     /* Return codes that DRM APIs can return */
#include "drmerr.h"

#if NO_DRM_CRT
#include <string.h>
#include <stdlib.h>
#if !defined(__arm) && !defined(__TMS320C55X__)
#include <wchar.h>
#endif
#endif

#include "drmsal.h"         /* SAL annotations */

#include "drmdebug.h"       /* Debug macros & functions */
#include "drmstrings.h"
#include "oemcommon.h"      /* Common OEM definitions */
#include "drmprofile.h"     /* Profiling definitions */
#include "drmbytemanip.h"

/*
**  When PK code is compiled with PC profile, namespace PK must be used
*/
#ifdef __cplusplus
#if DRM_BUILD_PROFILE == DRM_BUILD_PROFILE_PC
#if USE_PK_NAMESPACES
using namespace PK;
#endif /* USE_PK_NAMESPACES */
#endif /* DRM_BUILD_PROFILE */
#endif /* __cplusplus */

#endif  /* __DRMCOMMON_H__ */

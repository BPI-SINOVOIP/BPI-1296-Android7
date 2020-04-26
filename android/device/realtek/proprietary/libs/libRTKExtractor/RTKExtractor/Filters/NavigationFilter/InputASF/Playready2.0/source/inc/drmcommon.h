/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCOMMON_H__
#define __DRMCOMMON_H__

#include <drmcompiler.h>

#define DRM_MAX_ACTIONS 10

#ifndef ISODD
    #define ISODD(x) ((x)&1)
#endif

#define DRM_SAFE_INT (int)0x0fffffff
#define DRM_SAFE_UINT (unsigned int)0x0fffffff
#define DRM_UINT_MAX (unsigned int)0xffffffff
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define ROTATE_RIGHT(x, n) (((x) >> (n)) | ((x) << (32-(n))))
#define WORDSWAP(d) ( ( (d) >> 16 ) | ( (d) << 16 ) )


#define FLIP_DWORD_BYTES(_dw_) ( ((_dw_)&0xFF000000UL) >>24 | \
                                 ((_dw_)&0x00FF0000UL) >> 8 | \
                                 ((_dw_)&0x0000FF00UL) << 8 | \
                                 ((_dw_)&0x000000FFUL) <<24 )

#define FLIP_WORD_BYTES(_w_)  ( ((_w_)&0xFF00UL) >> 8 | \
                                ((_w_)&0x00FFUL) << 8 )

/* for good parameter documentation */
#define IN
#define OUT

#include "drmpragmas.h"     /* Handling of compilation warnings */
#include "drmnamespace.h"   /* Defining proper namespace (if used)                 */
#include "drmtypes.h"       /* Core DRM types */
#include "drmresults.h"     /* Return codes that DRM APIs can return */
#include "drmerr.h"


#if INCLUDE_DRMCOMMONPROFILE
#include <drmcommonprofile.h>
#endif


#include "drmsal.h"         /* SAL annotations */

#include "drmdebug.h"       /* Debug macros & functions */
#include <drmconstants.h>
#include "drmversionconstants.h"
#include "oemcommon.h"      /* Common OEM definitions */
#include "drmprofile.h"     /* Profiling definitions */
#include "drmbytemanip.h"

#ifdef __cplusplus

#if DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT
namespace PK {} /* Make sure namespace is defined before using statement */
using namespace PK;
#endif /* DRM_INCLUDE_PK_NAMESPACE_USING_STATEMENT */

#endif /* __cplusplus */

/*
** Device cert and licenses will be staged to share the same buffer
*/
#define MAX_DEVICE_CERT_SIZE          ( 36 * 1024 )
#define MAX_DEVICE_CERT_TEMPLATE_SIZE ( ( MAX_DEVICE_CERT_SIZE * 6 ) / 10 )/* 60% of max devcert */
#define DRM_MAX_HEADERDATASIZE        ( 5 * 1024 )                         /* 5K of header data  */
#define DRM_MAX_LICENSESIZE           MAX_DEVICE_CERT_SIZE                 /* 36K max license size */
#define MAX_REVOCATION_LIST_SIZE      ( 30 * 1024 )

/* The buffer should be big enough to store a single XMR license. */
#define MAX_NST_BUFFER_SIZE           1024

#endif  /* __DRMCOMMON_H__ */


/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCOMMON_H__
#define __DRMCOMMON_H__

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
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define ROTATE_RIGHT(x, n) (((x) >> (n)) | ((x) << (32-(n))))
#define WORDSWAP(d) ( ( (d) >> 16 ) + ( (d) << 16 ) )

/* for good parameter documentation */
#define IN
#define OUT

#include <limits.h>
#include "drmtypes.h"       /*  core DRM types                                      */
#include "drmresults.h"     /*  return codes that DRM APIs can return               */
#include "drmerr.h"
#include "drmdebug.h"       /*  debug macros & functions                            */
#include "drmstrings.h"

#include "oemimpl.h"
#include "drmprofile.h"

#endif  /* __DRMCOMMON_H__ */

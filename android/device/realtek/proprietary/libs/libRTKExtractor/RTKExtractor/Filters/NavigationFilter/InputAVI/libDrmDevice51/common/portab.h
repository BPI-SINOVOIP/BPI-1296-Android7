/**
** Copyright (C) 2002 DivXNetworks, all rights reserved.
**
** DivXNetworks, Inc. Proprietary & Confidential
**
** This source code and the algorithms implemented therein constitute
** confidential information and may comprise trade secrets of DivXNetworks
** or its associates, and any use thereof is subject to the terms and
** conditions of the Non-Disclosure Agreement pursuant to which this
** source code was originally received.
**
**/

/** $Id: portab.h 5093 2007-09-25 18:16:16Z jbraness $
**
**/

/*************************************************************************/

/**
 *  Copyright (C) 2001 - DivXNetworks
 *
 * Andrea Graziani (Ag)
 * Jonathan White
 *
 * DivX Advanced Research Center <darc@projectmayo.com>
 **/
/* portab.h // */

#ifndef _PORTAB_H_
#define _PORTAB_H_

#ifndef DIVXINT_NATIVE_64BIT
#define DIVXINT_NATIVE_64BIT
#endif
#include "DivXInt.h"

#define TRUE 1
#define FALSE 0

#ifdef WIN32

/* basic types
 */
#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ( (void *)0 )
#endif
#endif

#define strncasecmp strnicmp
#define strcasecmp stricmp

#include <stdlib.h>
#include <windows.h>

#define wcstok( wcs, delim, state )  wcstok( wcs, delim )

#define snprintf _snprintf
#ifndef  __cplusplus
#include <wchar.h>
#define swprintf _snwprintf
#endif

#elif defined ( ARCH_BSP_V60 )

#include <inttypes.h>
#define stricmp strcasecmp

#elif defined ( ARCH_TI_C6000 )

#include <string.h>
#define strncasecmp strncmp
#define strcasecmp strcmp

#elif defined ( __CYGWIN__ )
#include "sys/types.h"

#elif defined ( __APPLE__ )

#define stricmp strcasecmp
#define strnicmp strncasecmp

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <wchar.h>

#elif defined ( __FreeBSD__ )

#define stricmp strcasecmp
#define strnicmp strncasecmp

#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

#elif defined ( __Sigma__ ) || defined ( LINUX )

#include <inttypes.h>
#include <stddef.h>
#define stricmp strcasecmp
#define strnicmp strncasecmp

#else

#include <inttypes.h>
#define stricmp strcasecmp
#define strnicmp strncasecmp

#endif

#endif /* _PORTAB_H_ */

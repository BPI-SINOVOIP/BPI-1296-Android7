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

/** $Id: portab.h,v 1.4 2004/10/21 06:07:45 arobinson Exp $
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
// portab.h //

#ifndef _PORTAB_H_
#define _PORTAB_H_

#define TRUE 1
#define FALSE 0

#ifdef WIN32

/* basic types
*/
#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

typedef unsigned __int64 uint64_t;
typedef unsigned int uint32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned char uint8_t;
typedef __int64 int64_t;
typedef __int32 int32_t;
typedef __int16 int16_t;
typedef __int8  int8_t;

#define strncasecmp strnicmp
#define strcasecmp stricmp

#define SU_MALLOC_ALIGN_SIZE	32

#include <stdlib.h>
#include <windows.h>

#define wcstok(wcs,delim,state)  wcstok(wcs,delim)

#define snprintf _snprintf
#ifndef  __cplusplus
#include <wchar.h>
#define swprintf _snwprintf
#endif

#elif defined (TARGET_PS2)

#define int8_t char
#define uint8_t unsigned char
#define int16_t short
#define uint16_t unsigned short
#define int32_t int
#define uint32_t unsigned int 
#define	int64_t signed long		
#define	uint64_t unsigned long

#define SU_MALLOC_ALIGN_SIZE	32

#elif defined (ARCH_BSP_V60)

#include <inttypes.h>
#define stricmp strcasecmp
#define SU_MALLOC_ALIGN_SIZE	128

#elif defined (ARCH_TI_C6000)

#include <string.h>
#define strncasecmp strncmp
#define strcasecmp strcmp

#define SU_MALLOC_ALIGN_SIZE	64

#define int8_t char
#define uint8_t unsigned char
#define int16_t short
#define uint16_t unsigned short
#define int32_t int
#define uint32_t unsigned int 
#define	int64_t double
#define	uint64_t double

#elif defined (__CYGWIN__)
#include "sys/types.h"

#elif defined (__APPLE__)

#define stricmp strcasecmp
#define strnicmp strncasecmp
/*
 * #define SU_MALLOC_ALIGN_SIZE	64
 */
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <wchar.h>

#elif defined (__FreeBSD__)

#define stricmp strcasecmp
#define strnicmp strncasecmp
/*
 * #define SU_MALLOC_ALIGN_SIZE	64
 */
#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

#elif defined (LINUX)

#include <inttypes.h>

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define SU_MALLOC_ALIGN_SIZE	128

//#include <netinet/in.h>

#else 

#include <inttypes.h>
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define SU_MALLOC_ALIGN_SIZE	128

#endif 

#endif // _PORTAB_H_

/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCRT_H__
#define __DRMCRT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* String CRT functions */

DRM_BOOL  DRM_API DRMCRT_iswspace  (DRM_WCHAR wch);
DRM_BOOL  DRM_API DRMCRT_iswxdigit (DRM_WCHAR wch);
DRM_BOOL  DRM_API DRMCRT_iswdigit  (DRM_WCHAR wch);
DRM_BOOL  DRM_API DRMCRT_iswalpha  (DRM_WCHAR wch);
DRM_WCHAR DRM_API DRMCRT_towlower  (DRM_WCHAR wch);

DRM_INT DRM_API DRMCRT_memcmp(
    IN const DRM_VOID  *src1,
    IN const DRM_VOID  *src2,
    IN       DRM_DWORD  count );

DRM_VOID DRM_API DRMCRT_memset(
       OUT   DRM_VOID  *dst,
    IN       DRM_BYTE   b,
    IN       DRM_DWORD  count );

DRM_VOID DRM_API DRMCRT_memcpy(
       OUT   DRM_VOID  *dst,
    IN const DRM_VOID  *src,
    IN       DRM_DWORD  count );

DRM_VOID * DRM_API DRMCRT_memmove (
    OUT      DRM_VOID  *dst,
    IN const DRM_VOID  *src,
    IN       DRM_DWORD count );

DRM_RESULT DRM_API DRMCRT_wcsntol( 
    IN const DRM_WCHAR *pwszStringInput,
    IN       DRM_DWORD  cchStringInput,
    OUT      DRM_LONG  *plValue );

DRM_INT DRM_API DRMCRT_wcsncmp(
    IN const DRM_WCHAR *first,
    IN const DRM_WCHAR *last,
    IN       DRM_DWORD  count );

DRM_DWORD DRM_API DRMCRT_wcslen(
    IN const DRM_WCHAR *wsz );

DRM_WCHAR* DRM_API DRMCRT_wcsncpy( 
    OUT      DRM_WCHAR *wszDestination,
    IN const DRM_WCHAR *wszSource,
    IN       DRM_DWORD  cchMax );

DRM_INT DRM_API DRMCRT_strncmp (
    IN const DRM_CHAR  *first,
    IN const DRM_CHAR  *last,
    IN       DRM_DWORD  count);

DRM_INT DRM_API DRMCRT_abs( DRM_INT number );

#if NO_DRM_CRT
#include <string.h>
#include <stdlib.h>
#if !defined(__arm) && !defined(__TMS320C55X__)
#include <wchar.h>
#endif
#else
#define iswspace DRMCRT_iswspace
#define iswxdigit DRMCRT_iswxdigit
#define iswdigit DRMCRT_iswdigit
#define iswalpha DRMCRT_iswalpha
#define towlower DRMCRT_towlower
#define wcsncmp DRMCRT_wcsncmp
#define wcslen DRMCRT_wcslen
#define wcsncpy DRMCRT_wcsncpy
#define strncmp DRMCRT_strncmp
#define memmove DRMCRT_memmove
#define memcmp DRMCRT_memcmp
#define memset DRMCRT_memset
#define memcpy DRMCRT_memcpy
#define abs DRMCRT_abs
#endif

#define wcsntol DRMCRT_wcsntol

#if !defined( min )
    #define min( a, b) ( (a)<(b)?(a):(b))
#endif

#if !defined( max )
    #define max( a, b ) ( (a) < (b) ? (b):(a) )
#endif
#ifdef __cplusplus
}
#endif

#endif /* __DRMCRT_H__ */

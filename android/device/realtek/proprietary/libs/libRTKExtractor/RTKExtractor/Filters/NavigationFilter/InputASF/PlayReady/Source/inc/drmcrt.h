/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCRT_H__
#define __DRMCRT_H__

#include <drmstrsafe.h>

#if !NO_DRM_CRT
/* They want to use the DRM CRT instead of their own CRT (maybe there isn't one) */

ENTER_PK_NAMESPACE;

/* String CRT functions */

DRM_API DRM_BOOL  DRM_CALL DRMCRT_iswspace  (DRM_WCHAR wch);
DRM_API DRM_BOOL  DRM_CALL DRMCRT_iswxdigit (DRM_WCHAR wch);
DRM_API DRM_BOOL  DRM_CALL DRMCRT_iswdigit  (DRM_WCHAR wch);
DRM_API DRM_BOOL  DRM_CALL DRMCRT_iswalpha  (DRM_WCHAR wch);
DRM_API DRM_WCHAR DRM_CALL DRMCRT_towlower  (DRM_WCHAR wch);

DRM_API DRM_INT DRM_CALL DRMCRT_memcmp(
    __in_bcount( count ) const DRM_VOID  *src1,
    __in_bcount( count ) const DRM_VOID  *src2,
    IN       DRM_SIZE_T  count );

DRM_API DRM_VOID DRM_CALL DRMCRT_memset(
    __out_bcount( count ) DRM_VOID  *dst,
    IN       DRM_DWORD  b,
    IN       DRM_SIZE_T  count );

DRM_API DRM_VOID DRM_CALL DRMCRT_memcpy(
    __out_bcount( count ) DRM_VOID  *dst,
    __in_bcount( count ) const DRM_VOID  *src,
    IN       DRM_SIZE_T  count );

DRM_API DRM_VOID * DRM_CALL DRMCRT_memmove (
    __out_bcount( count ) DRM_VOID  *dst,
    __in_bcount( count ) const DRM_VOID  *src,
    IN       DRM_SIZE_T count );


DRM_API DRM_INT DRM_CALL DRMCRT_wcsncmp(
    __in_ecount( count ) const DRM_WCHAR *first,
    __in_ecount( count ) const DRM_WCHAR *last,
    IN       DRM_SIZE_T  count );

DRM_API DRM_SIZE_T DRM_CALL DRMCRT_wcslen(
    __in_z const DRM_WCHAR *wsz );

DRM_API DRM_SIZE_T DRM_CALL DRMCRT_strlen(
    __in_z const DRM_CHAR *sz );

DRM_API DRM_INT DRM_CALL DRMCRT_strncmp (
    __in_ecount( count ) const DRM_CHAR  *first,
    __in_ecount( count ) const DRM_CHAR  *last,
    IN       DRM_SIZE_T  count);

DRM_API DRM_INT DRM_CALL DRMCRT_abs( DRM_INT number );

EXIT_PK_NAMESPACE;

#else /* #ifndef NO_DRM_CRT */

/*
** The #include for all of the regular CRT headers is in drmcommon.h because drmsal.h must be included AFTER
** anything that could pull in the system's SAL annotations.
*/

#define DRMCRT_iswspace(wch)                          iswspace( wch )
#define DRMCRT_iswxdigit(wch)                         iswxdigit( wch )
#define DRMCRT_iswdigit(wch)                          iswdigit( wch )
#define DRMCRT_iswalpha(wch)                          iswalpha( wch )
#define DRMCRT_towlower(wch)                          towlower( wch )

#define DRMCRT_wcsncmp( a, b, c )                     wcsncmp( (const DRM_WCHAR_NATIVE *)a, (const DRM_WCHAR_NATIVE *)b, c )  

#define DRMCRT_wcslen( a )                            (DRM_DWORD)wcslen( (const DRM_WCHAR_NATIVE *)a  )  

#define DRMCRT_strlen( sz )                           (DRM_DWORD)strlen( sz )

#define DRMCRT_strncmp( szFirst, szLast, dwcount )    strncmp( szFirst, szLast, dwcount ) 

#define DRMCRT_memmove( dst, src, dwcount )           memmove( dst, src, dwcount )

#define DRMCRT_memcmp( src1, src2, dwcount )          memcmp( src1, src2, dwcount )

#define DRMCRT_memset( dest, bVal, dwcount )          memset( dest, bVal, dwcount )

#define DRMCRT_memcpy( dst, src, dwcount )            memcpy( dst, src, dwcount )

#define DRMCRT_abs( number )                          abs( (DRM_INT)number )

#endif /* #ifndef NO_DRM_CRT */


#if !defined( min )
    #define min( a, b ) ( (a) < (b) ? (a) : (b) )
#endif

#if !defined( max )
    #define max( a, b ) ( (a) < (b) ? (b) : (a) )
#endif



/**********************************************************************
**
** Set of DRM specific functions that complement C run time.  
*
***********************************************************************/

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL DRMCRT_AtoDWORD( 
    __in_ecount( f_cchStringInput ) const DRM_CHAR  *f_pszStringInput,  
    IN       DRM_DWORD  f_cchStringInput,
    IN       DRM_DWORD  f_base,
    OUT      DRM_DWORD *f_pdwValue );

DRM_API DRM_RESULT DRM_CALL DRMCRT_WtoDWORD( 
    __in_ecount( f_cchStringInput ) const DRM_WCHAR *pwszStringInput,  
    __in                                  DRM_DWORD  f_cchStringInput,
    __in                                  DRM_DWORD  f_base,
    __out                                 DRM_DWORD *f_pdwValue );

DRM_API DRM_RESULT DRM_CALL DRMCRT_wcsntol( 
    __in_ecount( cchStringInput ) const DRM_WCHAR *pwszStringInput,
    IN       DRM_DWORD  cchStringInput,
    OUT      DRM_LONG  *plValue );


DRM_API DRM_WCHAR* DRM_CALL DRMCRT_wcsnstr
(
   __in_ecount( cchMaxStr ) const DRM_WCHAR *pwszStr,
   IN       DRM_DWORD  cchMaxStr,
   __in_ecount( cchMaxStrSearch ) const DRM_WCHAR *pwszStrSearch, 
   IN       DRM_DWORD  cchMaxStrSearch
);

EXIT_PK_NAMESPACE;

#endif /* __DRMCRT_H__ */

/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBASE64_H__
#define __DRMBASE64_H__

ENTER_PK_NAMESPACE;

/* 
   Using decode in place will cause the decoder to place the decoded output in the 
   last *pcbDestination bytes of pwszSource->pwszString
*/
#define DRM_BASE64_DECODE_IN_PLACE 1


/* DRM normally uses a special base64 encoding.  Use this flag to get standard encoding */
#define DRM_BASE64_ENCODE_WMDRM 2


/* characters required for a binary of size cb expressed as base64*/
#define CCH_BASE64_EQUIV(cb)          ((((cb)/3)+(((cb)%3)?1:0))*4)
/* SAL does not accept modulo% operand, introducing another macro for SAL */
#define CCH_BASE64_EQUIV_SAL(cb)      ((((cb)/3)+(((cb)-((cb)/3)*3)?1:0))*4)
#define CB_BASE64_DECODE(cch)         (((cch)*3)/4)
#define CCH_BASE64_ENC_LARGE(cb)      CCH_BASE64_EQUIV((cb)+DRM_ECC160_CIPHERTEXT_LEN)
#define CCH_BASE64_ENC_LARGE_WCH(cch) CCH_BASE64_ENC_LARGE((cch)*SIZEOF(DRM_WCHAR))

DRM_API DRM_RESULT DRM_CALL DRM_B64_DecodeA(
    __in_ecount( f_pdasstrSource->m_ich + f_pdasstrSource->m_cch ) const DRM_CHAR *f_pszBase,
    IN const DRM_SUBSTRING         *f_pdasstrSource,
    IN OUT DRM_DWORD *pcbDestination, 
    __out_bcount_opt( ( dwFlags & DRM_BASE64_DECODE_IN_PLACE ) == 0 ? *pcbDestination : f_pdasstrSource->m_ich + *pcbDestination ) DRM_BYTE *pbDestination,
    IN       DRM_DWORD              dwFlags);

DRM_API DRM_RESULT DRM_CALL DRM_B64_DecodeW(
    IN const DRM_CONST_STRING      *pdstrSource,
    IN OUT   DRM_DWORD             *pcbDestination, 
    __out_bcount_opt( *pcbDestination ) DRM_BYTE *pbDestination,
    IN       DRM_DWORD              dwFlags);

DRM_API DRM_RESULT DRM_CALL DRM_B64_EncodeA( 
    __in_bcount( cbBuffer ) const DRM_BYTE  *pvBuffer,    /* input buffer */
    IN       DRM_DWORD  cbBuffer,    /* input len */
    __out_ecount_opt( *pcchEncoded ) DRM_CHAR  *pszEncoded,  /* output char */
    IN OUT   DRM_DWORD *pcchEncoded, /* output ch len */
    IN       DRM_DWORD  dwFlags );

DRM_API DRM_RESULT DRM_CALL DRM_B64_EncodeW( 
    __in_bcount( f_cbSource ) const DRM_BYTE *f_pbSource,    /* input buffer */
    IN                                   DRM_DWORD        f_cbSource,    /* input len */
    __out_ecount_opt( *f_pcchEncoded ) DRM_WCHAR *f_pwszEncoded, /* output WCHAR */
    IN OUT                               DRM_DWORD       *f_pcchEncoded, /* output ch len */
    IN                                   DRM_DWORD        f_dwFlags );

EXIT_PK_NAMESPACE;

#endif

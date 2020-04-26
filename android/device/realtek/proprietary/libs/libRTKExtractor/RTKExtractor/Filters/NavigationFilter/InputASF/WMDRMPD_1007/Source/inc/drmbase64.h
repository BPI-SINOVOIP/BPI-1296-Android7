/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBASE64_H__
#define __DRMBASE64_H__

#ifdef __cplusplus
extern "C" {
#endif
/* 
   Using decode in place will cause the decoder to place the decoded output in the 
   last *pcbDestination bytes of pwszSource->pwszString
*/
#define DRM_BASE64_DECODE_IN_PLACE 1


/* DRM normally uses a special base64 encoding.  Use this flag to get standard encoding */
#define DRM_BASE64_ENCODE_WMDRM 2


/* characters required for a binary of size cb expressed as base64*/

#define CCH_BASE64_EQUIV(cb)          ((((cb)/3)+(((cb)%3)?1:0))*4)
#define CB_BASE64_DECODE(cch)           (((cch)*3)/4)
#define CCH_BASE64_ENC_LARGE(cb)      CCH_BASE64_EQUIV(cb+PK_ENC_CIPHERTEXT_LEN)
#define CCH_BASE64_ENC_LARGE_WCH(cch) CCH_BASE64_ENC_LARGE((cch)*SIZEOF(DRM_WCHAR))

DRM_RESULT DRM_API DRM_B64_DecodeA(
    IN const DRM_CHAR              *f_pszBase,
    IN const DRM_SUBSTRING         *f_pdasstrSource,
    IN OUT   DRM_DWORD             *pcbDestination, 
       OUT   DRM_BYTE              *pbDestination,
    IN       DRM_DWORD              dwFlags);

DRM_RESULT DRM_API DRM_B64_DecodeW(
    IN const DRM_CONST_STRING      *pdstrSource,
    IN OUT   DRM_DWORD             *pcbDestination, 
       OUT   DRM_BYTE              *pbDestination,
    IN       DRM_DWORD              dwFlags);

DRM_RESULT DRM_API DRM_B64_EncodeA( 
    IN const DRM_BYTE  *pvBuffer,    /* input buffer */
    IN       DRM_DWORD  cbBuffer,    /* input len */
       OUT   DRM_CHAR  *pszEncoded,  /* output char */
    IN OUT   DRM_DWORD *pcchEncoded, /* output ch len */
    IN       DRM_DWORD  dwFlags );

DRM_RESULT DRM_API DRM_B64_EncodeW( 
    IN const DRM_BYTE  *pvBuffer,    /* input buffer */
    IN       DRM_DWORD  cbBuffer,    /* input len */
       OUT   DRM_WCHAR *pwszEncoded, /* output WCHAR */
    IN OUT   DRM_DWORD *pcchEncoded, /* output ch len */
    IN       DRM_DWORD  dwFlags );

#ifdef __cplusplus
}
#endif

#endif

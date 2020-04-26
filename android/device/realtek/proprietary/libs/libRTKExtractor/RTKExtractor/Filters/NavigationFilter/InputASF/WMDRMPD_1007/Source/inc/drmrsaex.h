/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _DRMRSAEX_H_
#define _DRMRSAEX_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#define DRM_RSA_ENC_CIPHERTEXT_LEN 128

DRM_RESULT DRM_API DrmRsaOaepDecrypt(
    IN DRM_RSA_PRIVATE_KEY *pPrivateKey,
    IN DRM_BYTE            *pbCiphertext,
    IN    DRM_DWORD            cbCiphertext,
    OUT   DRM_BYTE           **ppbPlaintext,
    OUT   DRM_DWORD           *pcbPlaintext );

DRM_RESULT DRM_API DrmRsaOaepEncrypt(
    IN DRM_RSA_PUBLIC_KEY *pPublicKey,
    IN DRM_BYTE           *pbPlaintext,
    IN       DRM_DWORD           cbPlaintext,
    OUT   DRM_BYTE          **ppbCiphertext,
    OUT   DRM_DWORD          *pcbCiphertext );

DRM_RESULT DRM_API DrmRsaSign(
       IN                                  DRM_RSA_PRIVATE_KEY *f_pkeyPrivate,
       IN                                  DRM_BYTE            *f_pbData,
       IN                                  DRM_SUBSTRING       *f_pdasstr, 
       OUT                   DRM_BYTE            **f_ppbSignature,
       OUT                                 DRM_DWORD           *f_pcbSignature);

DRM_RESULT DRM_API DrmRsaVerify(
    IN const DRM_CHAR           *f_pszBase,
    IN       DRM_SUBSTRING      *f_pdasstrData,
    IN       DRM_RSA_PUBLIC_KEY *f_ppubkey,
    IN const DRM_BYTE           *f_pbSignature,
    IN const DRM_DWORD           f_cbSignature); /* = key modulus length */

#ifdef  __cplusplus
}
#endif

#endif /* _DRMRSAEX_H_ */

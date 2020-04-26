/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMNDDEVICECERT_H
#define __DRMNDDEVICECERT_H

#include <drmcommon.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL DRM_NDCert_CreateDeviceCertificate(
    __in_opt DRM_VOID            *f_pOEMContext,
    __inout  DRM_ANSI_STRING     *f_pdastrBuffer,
    __in     DRM_DWORD            f_cbBuffer,
    __in     DRM_RSA_PRIVATE_KEY *f_poRSAModelPrivKey,   
    __in     DRM_RSA_PUBLIC_KEY  *f_poRSADevicePubKey,   
    __in     DRM_CRYPTO_CONTEXT  *f_pCryptoCtx );

DRM_API DRM_RESULT DRM_CALL DRM_NDCert_GetDeviceCertificate(
    __in_opt       DRM_VOID            *f_pOEMContext,
    __inout        DRM_ANSI_STRING     *f_pdastrNDCert,
    __in    const  DRM_DWORD            f_dwFlags,
    __inout        DRM_CRYPTO_CONTEXT  *f_pCryptoCtx );

DRM_API DRM_RESULT DRM_CALL DRM_NDCert_Certificate_Initialize(
    __in        DRM_APP_CONTEXT  *f_pAppContext,
    __in_opt    DRM_VOID         *f_pOEMContext );

EXIT_PK_NAMESPACE;

#endif /* __DRMNDDEVICECERT_H__ */


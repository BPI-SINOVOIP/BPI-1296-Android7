/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _OEMRSAOAEPPSSIMPL_H_
#define _OEMRSAOAEPPSSIMPL_H_

#include <oemrsa.h>
#include <oemrsacommon.h>
#include <drmtypes.h>
#include <oemrsaoaeppss.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL OEM_RSA_OaepDecryptImpl(
    __in const                          DRM_RSA_PRIVATE_KEY *f_pPrivateKey,
    __in const                          DRM_DWORD            f_cbCiphertext,
    __in_ecount( f_cbCiphertext ) const DRM_BYTE            *f_pbCiphertext,
    __inout                             DRM_DWORD           *f_pcbPlaintext,
    __out_ecount( *f_pcbPlaintext )     DRM_BYTE            *f_pbPlaintext,
    __in                                DRM_CRYPTO_CONTEXT  *f_pCryptoCtx
    ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL OEM_RSA_OaepEncryptImpl(
    __in_opt                               DRM_VOID           *f_pOEMContext,
    __in const                             DRM_RSA_PUBLIC_KEY *f_pPublicKey,
    __in const                             DRM_DWORD           f_cbPlaintext,
    __in_ecount( f_cbPlaintext )     const DRM_BYTE           *f_pbPlaintext,
    __inout                                DRM_DWORD          *f_pcbCiphertext,
    __out_ecount( *f_pcbCiphertext ) const DRM_BYTE           *f_pbCiphertext,
    __in                                   DRM_CRYPTO_CONTEXT *f_pCryptoCtx );

DRM_API DRM_RESULT DRM_CALL OEM_RSA_PssSignImpl(
    __in const                      DRM_RSA_PRIVATE_KEY *f_pPrivateKey,
    __in const                      DRM_DWORD            f_cbMsg,
    __in_ecount( f_cbMsg ) const    DRM_BYTE            *f_pbMsg,
    __in const                      DRM_DWORD            f_ibMsg,
    __inout                         DRM_DWORD           *f_pcbSignature,
    __out_ecount( *f_pcbSignature ) DRM_BYTE            *f_pbSignature,
    __in                            DRM_CRYPTO_CONTEXT  *f_pCryptoCtx
    );

DRM_API DRM_RESULT DRM_CALL OEM_RSA_PssVerifyImpl(
    __in const                         DRM_RSA_PUBLIC_KEY  *f_pPublicKey,
    __in                               DRM_DWORD            f_cbMsg,
    __in_ecount( f_cbMsg ) const       DRM_BYTE            *f_pbMsg,
    __in const                         DRM_DWORD            f_ibMsg,
    __in const                         DRM_DWORD            f_cbSignature,
    __in_ecount( f_cbSignature ) const DRM_BYTE            *f_pbSignature,
    __in                                DRM_CRYPTO_CONTEXT *f_pCryptoCtx
    ) DRM_NO_INLINE_ATTRIBUTE;

EXIT_PK_NAMESPACE;

#endif /* _OEMRSAOAEPPSSIMPL_H_ */


/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMWMDRMNET_H__
#define __DRMWMDRMNET_H__

#include <drmcommon.h>
#include <oemrsa.h>
#include <drmmanager.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_WMDRMNET_IsWmdrmnetSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_WMDRMNET_IsWmdrmnetUnsupported(DRM_VOID);

DRM_RESULT DRM_CALL DRM_RVK_GetWMDRMNETRevocationEntries (
    __in_bcount( f_cbRevocationData ) DRM_BYTE *f_pbRevocationData,
    IN     DRM_DWORD            f_cbRevocationData,
       OUT DRM_DWORD           *f_pcEntries,
    __deref_out_ecount( *f_pcEntries ) WMDRMNET_CRL_ENTRY **f_ppEntries) DRM_NO_INLINE_ATTRIBUTE;

DRM_RESULT DRM_RVK_UpdateWMDRMNETRevocationList(
    IN                                                                                   DRM_CRYPTO_CONTEXT          *f_pcontextCrypto,
    IN                                                                                   DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_ecount( f_pdasstrRevocationList->m_ich + f_pdasstrRevocationList->m_cch )       DRM_CHAR                    *f_rgchBase,
    IN                                                                             const DRM_SUBSTRING               *f_pdasstrRevocationList,
    __in_bcount( f_cbRevocationBuffer )                                            const DRM_BYTE                    *f_pbRevocationBuffer,
    IN                                                                                   DRM_DWORD                    f_cbRevocationBuffer,
    IN                                                                             const PUBKEY                      *f_ppubkey );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_GetWMDRMNETList(
    IN                                             DRM_CRYPTO_CONTEXT           *f_pcontextCRYP,
    IN                                             DRM_REVOCATIONSTORE_CONTEXT  *f_pContextRev,
    __out_bcount_opt( *f_pcbRevocationData ) const DRM_BYTE                     *f_pbRevocationData,
    IN  OUT                                        DRM_DWORD                    *f_pcbRevocationData,
        OUT                                        DRM_DWORD                    *f_pidSerial ) DRM_NO_INLINE_ATTRIBUTE;

DRM_RESULT DRM_RVK_VerifyWMDRMNETRevocationList(
    __in_opt DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in     DRM_CONST_STRING            *f_pdstrList,
    __out    DRM_DWORD                   *f_pdwVersion,
    __in     DRM_CRYPTO_CONTEXT          *f_pCryptoCtx ) DRM_NO_INLINE_ATTRIBUTE;

DRM_NO_INLINE DRM_RESULT DRM_RVK_UpdateWMDRMNETRevocationListDecoded(
    IN          DRM_CRYPTO_CONTEXT          *f_pcontextCrypto,
    IN          DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN          DRM_BYTE                    *f_pbRevocationList,
    IN          DRM_DWORD                    f_cbRevocationList,
    IN    const DRM_BYTE                    *f_pbRevocationBuffer,
    IN          DRM_DWORD                    f_cbRevocationBuffer,
    OUT         DRM_BOOL                    *f_pfUpdated,
    OUT         DRM_DWORD                   *f_pdwVersion );

DRM_RESULT DRM_CALL DRM_RVK_VerifyBinaryWMDRMNETSignature(
    __in_opt DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_bcount( f_cbData )
             DRM_BYTE                    *f_pbData,
    __in     DRM_DWORD                    f_cbData,
    __inout  DRM_DWORD                   *f_pidSerial,
    __in     DRM_CRYPTO_CONTEXT          *f_pCryptoCtx ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_RVK_VerifyCRLSignature(
    __in_opt                             DRM_BB_CONTEXT       *f_pcontextBBX,
    __in_opt                             DRM_SECSTORE_CONTEXT *f_pcontextSST,
    __in_opt                             DRM_DST              *f_pDatastore,
    __in_bcount( f_cbSignedBytes )       DRM_BYTE             *f_pbSignedBytes,
    __in                                 DRM_DWORD             f_cbSignedBytes,
    __in_bcount( f_cbSignature )   const DRM_BYTE             *f_pbSignature,
    __in                                 DRM_DWORD             f_cbSignature,
    __in                                 DRM_SUBSTRING         f_dasstrCertificate,
    __in                                 DRM_CRYPTO_CONTEXT   *f_pCryptoCtx ) DRM_NO_INLINE_ATTRIBUTE;

DRM_RESULT DRM_CALL DRM_XMB_RSASignAndCloseCurrNodeA(
    __inout         _XMBContextA        *f_pbContextXMLA,
    __in      const DRM_RSA_PRIVATE_KEY *f_pprivkeyRSA,
    __in            DRM_BOOL             f_fIncludeTag,
    __out           DRM_DWORD           *f_pcbSignature,
    __out_bcount    (*f_pcbSignature)  DRM_BYTE *f_pbSignature,    
    __out_ecount_opt(*f_pcchSignature) DRM_CHAR *f_pszSignature, 
    __inout         DRM_DWORD           *f_pcchSignature,
    __out_opt       DRM_SUBSTRING       *f_pdasstrXMLFragment,
    __in            DRM_CRYPTO_CONTEXT  *f_pCryptCtx);

DRM_RESULT DRM_CALL DRM_XMB_HashAndRSASignAndCloseCurrNodeA(
    __inout         _XMBContextA           *f_pbContextXMLA,
    __in            DRM_SHA1_CONTEXT       *f_pcontextSHA,
    __in      const DRM_RSA_PRIVATE_KEY    *f_pprivkeyRSA,
    __in            DRM_BOOL                f_fIncludeTag,
    __out_bcount(DRM_SHA1_DIGEST_LEN) DRM_BYTE f_rgbSHA  [__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    __out_ecount(CCH_BASE64_EQUIV_SAL( DRM_SHA1_DIGEST_LEN )) DRM_CHAR f_rgchSHA [__CB_DECL(DRM_SHA1_B64ENC_DIGEST_LEN)],
    __out           DRM_DWORD              *f_pcbSignature,
    __out_bcount(*f_pcbSignature) DRM_BYTE *f_pbSignature,
    __out           DRM_DWORD              *f_pcchSignature,
    __out_ecount(CCH_BASE64_EQUIV_SAL(DRM_RSA_CB_PRIME_MAX)) DRM_CHAR f_rgchSignature [__CB_DECL(CCH_BASE64_EQUIV(DRM_RSA_CB_PRIME_MAX))],
    __out_opt       DRM_SUBSTRING          *f_pdasstrXMLFragment,
    __in            DRM_CRYPTO_CONTEXT     *f_pCryptCtx);

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

#endif /*__DRMWMDRMNET_H__ */


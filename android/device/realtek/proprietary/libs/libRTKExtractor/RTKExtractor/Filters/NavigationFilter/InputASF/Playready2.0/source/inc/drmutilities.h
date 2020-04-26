/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_UTILITIES_H__
#define __DRM_UTILITIES_H__

#include <drmcontextsizes.h>
#include <drmutilitieslite.h>
#include <drmcertcache.h>

ENTER_PK_NAMESPACE;

#define CCH_SYMSIG_TAG_PUB_KEY ( 4 + g_dstrAttributePubkey.cchString + CCH_BASE64_EQUIV( SIZEOF( PUBKEY ) ) ) /* SPACE + pubkey="b64-encoded pubkey" */

#define CCH_SYMSIG_TAG_VERIFIEDCK ( 4 + DRM_GUID_STRING_LEN + g_dstrAttributeVerifiedCK.cchString  ) /* SPACE + ID="GUID" */

#define CCH_SYMSIG_TAG \
                        (  2 + g_dstrTagSymSig.cchString               /* <SYMSIGNATURE> */                   \
                         + CCH_BASE64_EQUIV( DRM_SHA1_DIGEST_LEN )     /* B64-encoded symmetric signature */  \
                         + 3 + g_dstrTagSymSig.cchString               /* </SYMSIGNATURE> */                  \
                         + CCH_SYMSIG_TAG_PUB_KEY                                                             \
                         + CCH_SYMSIG_TAG_VERIFIEDCK )  

/*
** The 13th bit of the GUID specifies the ckVerified status
*/
#define CKVERIFIED_BIT 0x1000
#define GET_CKVERIFIED_BIT( ckVerifiedGuid ) ( ( ckVerifiedGuid.Data1 & CKVERIFIED_BIT ) != 0 )
#define SET_CKVERIFIED_BIT( ckVerifiedGuid, fValue ) \
    ( ckVerifiedGuid.Data1 = fValue? ( ckVerifiedGuid.Data1 | CKVERIFIED_BIT ) : ( ckVerifiedGuid.Data1 & ~( DRM_DWORD )CKVERIFIED_BIT ) )                         

typedef enum DRM_ROOTPUBKEY_CATEGORY
{
    WM_DRM_ROOTPUBKEY_LICENSES = 0,
    WM_DRM_ROOTPUBKEY_LRB,   
    WM_DRM_ROOTPUBKEY_CLK,
    WM_DRM_ROOTPUBKEY_MTR   
} DRM_ROOTPUBKEY_CATEGORY;

DRM_API DRM_BOOL DRM_CALL DRM_UTL_IsCertDateOK(
    __in_opt    DRM_VOID        *pOEMContext,
    __in_bcount( 4 ) const DRM_BYTE  *f_pbDate);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_CheckCertNoCache(
    IN const CERT               *f_pcert, 
    IN const PUBKEY             *f_ppubkey, 
    IN       DRM_BOOL            f_fCheckDate, 
    IN       DRM_CRYPTO_CONTEXT *f_pcontextCRYP,
    IN       DRM_VOID            *f_pOEMContext );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_CheckCertificate(
    IN  const   CERT                    *f_pcert, 
    IN  const   PUBKEY                  *f_ppubkey, 
    IN          DRM_BOOL                 f_fCheckCertDate,   
    IN          DRM_LICEVAL_CONTEXT     *f_pcontextLEVL,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey);

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_UTL_CheckCert(
    IN const CERT                *f_pcert, 
    IN const PUBKEY              *f_ppubkey, 
    IN       DRM_BOOL             f_fCheckDate, 
    IN       DRM_LICEVAL_CONTEXT *f_pcontextLEVL ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL DRM_UTL_CheckDACCert(
    IN const CERT                *f_pcert, 
    IN const PUBKEY              *f_ppubkey, 
    IN       DRM_BOOL             f_fCheckDate, 
    IN       DRM_LICEVAL_CONTEXT *f_pcontextLEVL );

DRM_API DRM_RESULT DRM_CALL DRM_PK_EncryptLarge(
    IN                                                          DRM_VOID            *f_pOEMContext,
    IN                                                    const PUBKEY              *f_ppubkey,
    __inout_bcount( f_cbClear )                           const DRM_BYTE            *f_pbClear,
    IN                                                          DRM_DWORD            f_cbClear,
    __out_bcount( f_cbClear + DRM_ECC160_CIPHERTEXT_LEN )       DRM_BYTE            *f_pbCipher,
    IN                                                          DRM_DWORD            f_cbKeySymmetric,
    IN                                                          DRM_CRYPTO_CONTEXT  *f_pcontextCRYP );

DRM_API DRM_RESULT DRM_CALL DRM_PK_DecryptLarge( 
    IN                                                     const PRIVKEY            *f_ppubkey,
    __inout_bcount( f_cbCipher)                            const DRM_BYTE           *f_pbCipher,
    IN                                                           DRM_DWORD           f_cbCipher,
    __out_bcount( f_cbCipher - DRM_ECC160_CIPHERTEXT_LEN )       DRM_BYTE           *f_pbClear,
    IN                                                           DRM_CRYPTO_CONTEXT *f_pcontextCRYP );

DRM_API DRM_RESULT DRM_CALL DRM_UTL_VerifyXMLSignature(
    IN  const   DRM_CONST_STRING        *f_pdstrSignedData,
    IN  const   DRM_CONST_STRING        *f_pdstrSignatureXML,
    IN  const   DRM_CONST_STRING        *f_pdstrCertChainXML,
    IN          DRM_BOOL                 f_fCheckExpiry,
    IN          DRM_BOOL                 f_fCheckCertChain,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey,
    IN  OUT     DRM_LICEVAL_CONTEXT     *f_pcontextLEVL);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_VerifyXMLSignatureEx(
    IN  const   DRM_CONST_STRING        *f_pdstrSignedData,
    IN  const   DRM_CONST_STRING        *f_pdstrSignatureValue,
    IN  const   DRM_CONST_STRING        *f_pdstrCertChainXML,
    IN          DRM_BOOL                 f_fCheckExpiry,
    IN          DRM_BOOL                 f_fCheckCertChain,
    IN          DRM_ROOTPUBKEY_CATEGORY  f_eRootPubkey,
    IN  OUT     DRM_LICEVAL_CONTEXT     *f_pcontextLEVL);

DRM_API DRM_RESULT DRM_CALL DRM_UTL_BuildXMLSymmSignature(
    __in                                                  PUBKEY           *f_poPubKey,
    __in_bcount( __CB_DECL( DRM_SHA1_DIGEST_LEN ) ) const DRM_BYTE          f_rgbSymSig[ __CB_DECL( DRM_SHA1_DIGEST_LEN ) ],
    __in_ecount( f_cchBuffer )                            DRM_WCHAR        *f_pwchBuffer,
    __in                                                  DRM_DWORD         f_cchBuffer,
    __in_opt                                              DRM_GUID         *f_pCkVerifiedGUID );

EXIT_PK_NAMESPACE;

#endif /* __DRM_UTILITIES_H__ */

/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBCERTPARSER_H__
#define __DRMBCERTPARSER_H__

#include <drmtypes.h>
#include <drmbcert.h>
#include <drmblackbox.h>

ENTER_PK_NAMESPACE;

/*
** Minimal length of a certificate chain in bytes - 20
*/
#define DRM_MIN_CERT_CHAIN_LEN ( SIZEOF( DRM_DWORD )   /* Header Constant */ \
                               + SIZEOF( DRM_DWORD )   /* Version */ \
                               + SIZEOF( DRM_DWORD )   /* Length */ \
                               + SIZEOF( DRM_DWORD )   /* Flags */ \
                               + SIZEOF( DRM_DWORD ) ) /* No of certificate entries: zero for minimal cert chain */

/*
** Defines for verification of object lengths in object headers
*/
#define DRM_BCERT_SIGNATURE_OBJECT_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER )   /* Object header */ \
                                        + SIZEOF( DRM_DWORD )      /* Signature type and length */ \
                                        + SIZEOF( SIGNATURE_P256 ) /* Signature data */ \
                                        + SIZEOF( DRM_DWORD )      /* Issuer key length */ \
                                        + SIZEOF( PUBKEY_P256 ) )  /* Issuer key data */

#define DRM_BCERT_BASIC_INFO_OBJECT_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER )  /* Object header */ \
                                        + DRM_BCERT_CERT_ID_LENGTH      /* Certificate ID */ \
                                        + SIZEOF( DRM_DWORD )           /* Security level */ \
                                        + SIZEOF( DRM_DWORD )           /* Certificate Flags */ \
                                        + SIZEOF( DRM_DWORD )           /* Type */ \
                                        + SHA256_DIGEST_SIZE_IN_BYTES   /* Digest Value */ \
                                        + SIZEOF( DRM_DWORD )           /* Expiration Date */ \
                                        + DRM_BCERT_CLIENT_ID_LENGTH )  /* Client ID */

#define DRM_BCERT_DEVICE_INFO_OBJECT_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER ) /* Object header */ \
                                        + SIZEOF( DRM_DWORD )         /* Max License Size */ \
                                        + SIZEOF( DRM_DWORD )         /* Max Header Size */ \
                                        + SIZEOF( DRM_DWORD ) )       /* Max License Chain Depth */

#define DRM_BCERT_FEATURES_OBJECT_MIN_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER ) /* Object header */ \
                                          + SIZEOF( DRM_DWORD ) )   /* Number of features: zero for minimal feature info */

#define DRM_BCERT_KEY_OBJECT_MIN_LEN ( SIZEOF( DRM_BCERT_OBJECT_HEADER ) /* Object header */ \
                                     + SIZEOF( DRM_WORD )                /* Type */ \
                                     + SIZEOF( DRM_WORD )                /* Length */ \
                                     + SIZEOF( DRM_DWORD )               /* Flags */ \
                                     + ECC_P256_POINT_SIZE_IN_BYTES      /* Value */ \
                                     + SIZEOF( DRM_DWORD )               /* Number of Entries */ \
                                     + SIZEOF( DRM_DWORD ) )             /* One entry */

/*
** Minimal length of a certificate:
** Cert header, Basic Info, Feature Info with zero features,
** Key Info and Signature Info.
*/
#define DRM_MIN_CERTIFICATE_LEN ( SIZEOF( DRM_BCERT_HEADER )         \
                                + SIZEOF( DRM_BCERT_BASIC_INFO )     \
                                + DRM_BCERT_FEATURES_OBJECT_MIN_LEN  \
                                + DRM_BCERT_KEY_OBJECT_MIN_LEN       \
                                + DRM_BCERT_SIGNATURE_OBJECT_LEN )


/*
** Minimal information to keep after a single certificate is parsed,
** which is needed verify the whole certificate chain.
*/
typedef struct __tagDRM_BCertMinimalData {
    DRM_BCERT_HEADER             oCertHeader;
    DRM_BCERT_BASIC_INFO         oBasicInfo;
    DRM_BCERT_PUBKEY             oIssuerKey;
    DRM_BCERT_FEATURE_INFO       oFeatureInfo;
    DRM_BCERT_KEY_INFO           oKeyInfo;
    DRM_BCERT_SIGNATURE_INFO     oSignatureInfo;
    DRM_BCERT_SECURITY_VERSION   oSecurityVersion;
    DRM_BCERT_SECURITY_VERSION   oSecurityVersion2;
} DRM_BCERT_MINIMALDATA;


/*
** Parsing error presentation, used in validation context structure.
*/
typedef struct {
    /*
    ** Number of the cert in the chain, zero-based with leaf cert as 0
    */
    DRM_DWORD   cCertNumber;
    /*
    ** Error code
    */
    DRM_RESULT  dwResult;
} DRM_BCERT_VERIFICATIONRESULT;

#define DRM_MAX_VERIFICATIONRESULTS 10

/*
** Validation context: structure that controls certificate parser behavior.
** On exit it may contain a list of parsing errors.
*/
typedef struct {
    /*
    ** [in] The latest expiration time of the cert chain - if 0 then don't verify it
    */
    DRMFILETIME        ftExpirationTime;
    /*
    ** [in] Pointer to a public key of the cert chain root - if NULL then skip this verification
    */
    const PUBKEY_P256  *pRootPubKey;
    /*
    ** [in] Expected leaf certificate type - if DRM_BCERT_CERTTYPE_UNKNOWN then don't verify
    */
    DRM_DWORD          dwType;
    /*
    ** [in] Context for crypto operations
    */
    DRM_CRYPTO_CONTEXT *pCryptoCtx;
    /*
    ** [in] Blackbox context for certificate caching
    */
    DRM_BB_CONTEXT *pBlackboxCtx;
    /*
    ** [in] Secure store context for certificate caching
    */
    DRM_SECSTORE_CONTEXT *pSecStoreCtx;
    /*
    ** [in] Datastore context for certificate caching
    */
    DRM_DST *pDSTCtx;
    /*
    ** [in] Set into TRUE if parser should verify cert signatures up the chain
    */
    DRM_BOOL           fVerifyCertSignatures;
    /*
    ** [in] Set into FALSE if parser should ignore absense of extended data and don't verify signature over it
    */
    DRM_BOOL           fDontFailOnMissingExtData;
    /*
    ** [in] Array of key usages that must be found in a leaf certificate,
    **      otherwise error out with DRM_E_BCERT_REQUIRED_KEYUSAGE_MISSING
    */
    DRM_DWORD          rgdwRequiredKeyUsages[DRM_BCERT_MAX_KEY_USAGES];
    /*
    ** [in] Actual number of elements in rgdwRequiredKeyUsages
    */
    DRM_DWORD          cRequiredKeyUsages;
    /*
    ** [in] If set into TRUE then parser will store non-fatal parser errors in array, keep parsing
    **      and return DRM_E_BCERT_VERIFICATION_ERRORS unless this is overriden by a fatal error
    **      (i.e. error after which the cert parsing cannot continue, for instance, length overflow)
    **      If set into FALSE then parsing stops immediately after any error.
    ** By default this field is FALSE.
    */
    DRM_DWORD          fCollectErrorsInArrayAndContinue;
    /*
    ** [out] Array to store and return non-fatal verification errors.
    */
    DRM_BCERT_VERIFICATIONRESULT rgoResults[ DRM_MAX_VERIFICATIONRESULTS ];
    /*
    ** [out] Actual number of members in rgoResults array, should be <= DRM_MAX_VERIFICATIONRESULTS
    */
    DRM_DWORD                    cResults;
    /*
    ** [out] Number of a currently parsed certificate, zero-based with leaf cert as 0
    */
    DRM_DWORD                    cCurrentCert;
    /*
    ** [out] Offsets of each certificate in the chain that have been parsed.
    */
    DRM_DWORD                    rgdwCertificateOffsets[DRM_BCERT_MAX_CERTS_PER_CHAIN];
} DRM_BCERT_VERIFICATIONCONTEXT;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_BCert_InitVerificationContext(
    __in_opt                              DRMFILETIME                   *f_pftExpirationTime,
    __in_opt                        const PUBKEY_P256                   *f_pRootPubKey,
    __in                            const DRM_DWORD                      f_dwCertType,
    __in_opt                              DRM_CRYPTO_CONTEXT            *f_pCryptoCtx,
    __in                                  DRM_BOOL                       f_fVerifyCertSignatures,
    __in                                  DRM_BOOL                       f_fDontFailOnMissingExtData,
    __in_ecount_opt(f_NumKeyUsages) const DRM_DWORD                     *f_pKeyUsages,
    __in                                  DRM_DWORD                      f_NumKeyUsages,
    __in                                  DRM_BOOL                       f_fCollectErrs,
    __in_opt                              DRM_BB_CONTEXT                *f_pcontextBBX,
    __in_opt                              DRM_SECSTORE_CONTEXT          *f_pcontextSST,
    __in_opt                              DRM_DST                       *f_pDatastore,
    __inout                               DRM_BCERT_VERIFICATIONCONTEXT *f_pVerificationContext );

DRM_API DRM_RESULT DRM_CALL DRM_BCert_ParseCertificateChain(
    __in_bcount(f_cbCertData) const DRM_BYTE                      *f_pbCertData,
    __in                      const DRM_DWORD                     f_cbCertData,
    __inout                         DRM_BCERT_VERIFICATIONCONTEXT *f_pVerificationContext );

DRM_API DRM_RESULT DRM_CALL DRM_BCert_GetChainHeader(
    __in_bcount(f_cbCertData) const DRM_BYTE         *f_pbCertData,
    __in                      const DRM_DWORD        f_cbCertData,
    __inout_ecount(1)         DRM_DWORD              *f_bOffset,
    __inout_ecount(1)         DRM_BCERT_CHAIN_HEADER *f_poChain );

DRM_API DRM_RESULT DRM_CALL DRM_BCert_GetCertificate(
    __in_bcount(f_cbCertData) const DRM_BYTE        *f_pbCertData,
    __in                      const DRM_DWORD       f_cbCertData,
    __inout_ecount(1)         DRM_DWORD             *f_pbOffset,
    __inout_ecount_opt(1)     DRM_BCERT_CERTIFICATE *f_poCertificate,
    __in                      const DRM_DWORD        f_dwRequiredCertType );

DRM_API DRM_RESULT DRM_CALL DRM_BCert_GetPublicKeyByUsage(
    __in_bcount(f_cbCertData)           const DRM_BYTE    *f_pbCertData,
    __in                                const DRM_DWORD    f_cbCertData,
    __in                                const DRM_DWORD    f_dwCertIndex,
    __in                                const DRM_DWORD    f_dwKeyUsage,
    __inout_ecount(1)                   PUBKEY_P256       *f_pPubkey,
    __out_opt                           DRM_DWORD         *f_pdwKeyUsageSet );

DRM_API DRM_RESULT DRM_CALL DRM_BCert_GetPublicKey(
    __in_bcount(f_cbCertData) const DRM_BYTE      *f_pbCertData,
    __in                      const DRM_DWORD     f_cbCertData,
    __in                      const DRM_DWORD     f_dwCertIndex,
    __inout_ecount(1)         PUBKEY_P256         *f_pPubkey );

DRM_API DRM_RESULT DRM_CALL DRM_BCert_GetSecurityVersion(
    __in_bcount(f_cbCertData) const DRM_BYTE      *f_pbCertData,
    __in                      const DRM_DWORD      f_cbCertData,
    __out_ecount(1)                 DRM_DWORD     *f_pdwSecurityVersion,
    __out_ecount(1)                 DRM_DWORD     *f_pdwPlatformIdentifier );

DRM_API DRM_RESULT DRM_CALL DRM_BCert_FindObjectInCertByType(
    __in_bcount(f_cbCertData) const DRM_BYTE    *f_pbCertData,
    __in                      const DRM_DWORD   f_cbCertData,
    __inout_ecount(1)         DRM_DWORD         *f_pbOffset,
    __in                      const DRM_DWORD   f_dwObjectType );


DRM_API DRM_RESULT DRM_CALL DRM_BCert_VerifySignature(
    __in_ecount(__CB_DECL( f_cbCertificate )) const DRM_BYTE *f_pbCertificateBegin,
    __in              DRM_DWORD                f_cbCertificate,
    __in              DRM_DWORD                f_dwSignedDataLength,
    __inout_ecount(1) DRM_BCERT_SIGNATURE_INFO *f_poSignatureInfo,
    __inout_ecount(1) DRM_CRYPTO_CONTEXT       *f_pCryptoCtx,
    __in_opt          DRM_BB_CONTEXT           *f_pcontextBBX,
    __in_opt          DRM_SECSTORE_CONTEXT     *f_pcontextSST,
    __in_opt          DRM_DST                  *f_pDatastore ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL DRM_BCert_GetManufacturerStrings(
    __in_bcount(f_cbCertData) const DRM_BYTE      *f_pbCertData,
    __in                      const DRM_DWORD      f_cbCertData,
    __out_ecount(1)                 DRM_BCERT_MANUFACTURER_STRINGS *f_pManufacturerStrings );

#define DRM_BCert_GetLeafSecurityLevel(secLevel, chain) \
    NETWORKBYTES_TO_DWORD( (secLevel), (chain), SIZEOF( DRM_BCERT_CHAIN_HEADER )  \
                                              + SIZEOF( DRM_BCERT_HEADER )        \
                                              + SIZEOF( DRM_BCERT_OBJECT_HEADER ) \
                                              + SIZEOF( DRM_BCERT_CERT_ID ) );

EXIT_PK_NAMESPACE;

#endif /* __DRMBCERTPARSER_H__ */

/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBLACKBOX_H__
#define __DRMBLACKBOX_H__

#include <drmlicense.h>
#include <drmdevcert.h>
#include <oemcipher.h>
#include <oemeccp256.h>
#include <drmxmr.h>
#include <byteorder.h>
#include <drmdomainstoretypes.h>
#include <drmkeyfile.h>
#include <drmbcertparsercache.h>

ENTER_PK_NAMESPACE;

#define DRM_PK_BUFFER_MAX  DRM_ECC160_PLAINTEXT_LEN
#define DRM_MAX_DRMOLDKEYS 20
#define DRMKEYPAIRSIZE    ( SIZEOF( PUBKEY ) + SIZEOF( PRIVKEY ) )


typedef DRM_OBFUS_FIXED_ALIGN struct __tagDRM_CLIENT_INFO
{
    DRM_ANSI_STRING m_dastrClientVersion;
} DRM_CLIENT_INFO;

#define BBX_OPAQUE_DATA_SIZE 128

typedef DRM_OBFUS_FIXED_ALIGN struct __tagDRM_BB_CONTEXT
{
    /* Make sure the crypto context is the first member in this struct */
    DRM_CRYPTO_CONTEXT                   CryptoContext;
    DRM_DEVICE_CERTIFICATE_CACHED_VALUES cachedCertValues;
DRM_OBFUS_PTR_TOP
    DRM_DOMAINSTORE_CONTEXT             *pDomainStoreContext; /* Used for domain-bound licenses */
DRM_OBFUS_PTR_BTM
    DRM_BINARY_DEVICE_CERT_CACHED_VALUES cachedBCertValues;
DRM_OBFUS_PTR_TOP
    DRM_VOID                            *pOEMContext;
DRM_OBFUS_PTR_BTM
    DRM_BOOL                             fInited;

DRM_OBFUS_FILL_BYTES(4)

DRM_OBFUS_PTR_TOP
    DRM_KEYFILE_CONTEXT                  *pKeyFileContext;
DRM_OBFUS_PTR_BTM

DRM_BYTE rgbOpaqueData[ __CB_DECL(BBX_OPAQUE_DATA_SIZE) ];

DRM_OBFUS_PTR_TOP
    DRM_CLIENT_INFO                      *pClientInfo;
DRM_OBFUS_PTR_BTM
} DRM_BB_CONTEXT;


typedef enum _BB_SUPPORTED_ALGORITHMS
{
    eMSDRM_PK  = 1,  /* The key is encrypted with a ECC public key */
    eMSDRM_CK  = 2,  /* This is a chained license key algorithm. ( DES/AES encrypted ) */
    eMSDRM_SYM = 3   /* The license is symmetrically bound.  This means that is was actually rebound to a symmetric key once is was stored on the device */
} BB_SUPPORTED_ALGORITHMS;

#define LEGACY_KID_B64ENC_LEN   24
#define DRM_MAX_CHECKSUM        MAX( MAX( CHECKSUM_LENGTH, LEGACY_KID_B64ENC_LEN ), DRM_AES_CK_CHECKSUM_LENGTH )

/*
** Contains checksum data
*/
typedef DRM_OBFUS_FIXED_ALIGN struct __tagDRM_BB_CHECKSUM_CONTEXT
{
    DRM_BOOL   m_fIgnoreChecksum;
    DRM_BYTE   m_rgbChecksumData[__CB_DECL( DRM_MAX_CHECKSUM )];
    DRM_DWORD  m_cbChecksum;  /* Should be 20 bytes for normal checksum, 24 bytes for V1 header checksum */
    DRM_KID    m_KID;
DRM_OBFUS_PTR_TOP
    DRM_BYTE  *m_pbV1KID;     /* If not NULL then use the old V1 checksum algorithm */
DRM_OBFUS_PTR_BTM
} DRM_BB_CHECKSUM_CONTEXT;

/*
** Contains binding information for XML licenses
*/
typedef DRM_DWORD_ALIGN struct __tagDRM_XML_BINDING_INFO
{
    DRM_BYTE                 m_rgbContentKey[__CB_DECL(DRM_ECC160_CIPHERTEXT_LEN)];
    DRM_BYTE                 m_rgbSignature[__CB_DECL(DRM_ECC160_SIGNATURE_LEN)];
    PUBKEY                   m_oPublKey;
    PUBKEY                   m_oLsPublKey;
    DRM_DWORD                m_dwAlgorithm;
    DRM_BOOL                 m_fCKSigVerified;
} DRM_XML_BINDING_INFO;

typedef DRM_OBFUS_FIXED_ALIGN struct __tagDRM_XMR_BINDING_INFO
{
DRM_OBFUS_PTR_TOP
    DRM_XMR_CONTENT_KEY            *m_pxmrContentKey;
DRM_OBFUS_PTR_BTM
DRM_OBFUS_PTR_TOP
    DRM_XMR_OPTIMIZED_CONTENT_KEY  *m_pxmrOptimizedContentKey; /* Pointer to symmetrically optimized version of content key */
DRM_OBFUS_PTR_BTM
DRM_OBFUS_PTR_TOP
    DRM_XMR_DEVICE_KEY             *m_pxmrDeviceKey;
DRM_OBFUS_PTR_BTM
DRM_OBFUS_PTR_TOP
    DRM_XMR_ECC_DEVICE_KEY         *m_pxmrDeviceEccKey;
DRM_OBFUS_PTR_BTM
DRM_OBFUS_PTR_TOP
    DRM_XMR_SIGNATURE              *m_pxmrSignature;
DRM_OBFUS_PTR_BTM
DRM_OBFUS_PTR_TOP
    DRM_XMR_DOMAIN_ID              *m_pxmrDomainID;
DRM_OBFUS_PTR_BTM
DRM_OBFUS_PTR_TOP
    DRM_XMR_LICENSE                *m_pxmrLicense;    /* An HMAC signature of this license will be calculated and compared to m_pbSignature */
DRM_OBFUS_PTR_BTM
} DRM_XMR_BINDING_INFO;

typedef DRM_OBFUS_FIXED_ALIGN struct __tagDRM_BINDING_INFO
{
    DRM_BB_CHECKSUM_CONTEXT m_oChecksum;   /* Contains Checksum data */
    DRM_BOOL                m_fXMRLicense; /* Specifies XMR or XML license type */

DRM_OBFUS_FILL_BYTES(4)
    union
    {
        DRM_XMR_BINDING_INFO m_XMR;
        DRM_XML_BINDING_INFO m_XML;
    } m_BindingInfo;
} DRM_BINDING_INFO;


/*
** Is different from PC client's version set into 1
*/
#define DOMAIN_KEY_RECORD_VERSION 2

typedef struct _tagDRM_BB_DOMAIN_KEY_RECORD {
    /* domain private key record format version */
    DRM_DWORD dwVersion;
    /* domain private key encrypted with one of the device private keys from the keyfile */
    PRIVKEY_P256 oDomainPrivKeyEncrypted;
    /* public key matching private key used to encrypt oDomainPrivKeyEncrypted */
    PUBKEY_P256  oDevicePubKey;
} DRM_BB_DOMAIN_KEY_RECORD;


DRM_API DRM_RESULT DRM_CALL DRM_BBX_Initialize(
    __in_opt             DRM_VOID           *f_pOEMContext,
    __inout_ecount( 1 )  DRM_BB_CONTEXT     *f_pcontextBBX );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_GetClientId(
    __out_ecount( 1 )    CLIENTID          *f_pClientId,
    __inout_ecount( 1 )  DRM_BB_CONTEXT    *f_pcontextBBX );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_Shutdown(
    __inout_ecount( 1 )  DRM_BB_CONTEXT     *f_pcontextBBX );

DRM_API DRM_BOOL DRM_CALL DRM_BBX_CanBind(
    __in_ecount( f_dwChainLength ) const DRM_BINDING_INFO *f_pBindingInfo,
    __in                                 DRM_DWORD         f_dwChainLength,
    __inout_ecount( 1 )                  DRM_BB_CONTEXT   *f_pcontextBBX,
    __out_opt                            DRM_RESULT       *f_pDr );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_HashValue(
    __in_bcount( f_cbValue )  const     DRM_BYTE       *f_pbValue,
    __in                                DRM_DWORD       f_cbValue,
    __in_bcount( DRM_SHA1_DIGEST_LEN )  DRM_BYTE        f_rgbHashValue[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    __inout_ecount( 1 )                 DRM_BB_CONTEXT *f_pcontextBBX );

DRM_API DRM_BOOL DRM_CALL DRM_BBX_DecryptLicense(
    __inout_bcount( f_cbEncryptedLicense )   DRM_BYTE       *f_pbEncryptedLicense,
    __in                                     DRM_DWORD       f_cbEncryptedLicense,
    __out_bcount_opt( f_cbEncryptedLicense - DRM_ECC160_CIPHERTEXT_LEN )
                                             DRM_BYTE       *f_pbLicense,
    __inout_ecount( 1 )                      DRM_BB_CONTEXT *f_pcontextBBX,
    __in_opt                           const PUBKEY         *f_pPubKey  );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_CipherKeySetup(
    __in_ecount( f_dwChainLength ) const DRM_BINDING_INFO    *f_pBindingInfo,
    __in                                 DRM_DWORD            f_dwChainLength,
    __out_ecount( 1 )                    DRM_CIPHER_CONTEXT  *f_pCipherContext,
    __inout_ecount( 1 )                  DRM_BB_CONTEXT      *f_pcontextBBX );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_CPHR_InitDecrypt(
    __inout_ecount( 1 )                                                DRM_CIPHER_CONTEXT *f_pCipherContext,
    __in_bcount_opt( min(DRM_CPHR_CB_FOR_INITDECRYPT,f_cbData) ) const DRM_BYTE           *f_pbLast15,
    __in                                                               DRM_DWORD           f_cbData );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_CPHR_Decrypt(
    __in_ecount( 1 )            DRM_CIPHER_CONTEXT           *f_pCipherContext,
    __inout_opt                 DRM_AES_COUNTER_MODE_CONTEXT *f_pCtrContext,
    __in                        DRM_DWORD                     f_cbData,
    __inout_bcount( f_cbData )  DRM_BYTE                     *f_pbData );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_CPHR_Init(
    __out_ecount(1)                     DRM_CIPHER_CONTEXT    *f_pContext,
    __in                                DRM_SUPPORTED_CIPHERS  f_eCipherType,
    __in                                DRM_DWORD              f_cbContKey,
    __in_bcount(f_cbContKey)    const   DRM_BYTE              *f_pbContKey );

typedef enum
{
    eBBX_PRIVKEY_MACHINE               = 1,
    eBBX_PRIVKEY_RESTORELICENSESIGNER  = 2,
    eBBX_PRIVKEY_TRANSFERLICENSESIGNER = 3
} eBBX_SIGNING_KEY_TYPES;

DRM_API DRM_RESULT DRM_CALL DRM_BBX_SignData(
    __inout_ecount( 1 )                      DRM_BB_CONTEXT *f_pcontextBBX,
    __in_bcount( f_cbData )  const           DRM_BYTE       *f_pbData,
    __in                                     DRM_DWORD       f_cbData,
    __in                                     DRM_DWORD       f_ePrivKeyType,
    __out_bcount( DRM_ECC160_SIGNATURE_LEN ) DRM_BYTE        f_rgbSignature[__CB_DECL( DRM_ECC160_SIGNATURE_LEN )] );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_SymmetricSign(
    __inout_ecount( 1 )                    DRM_BB_CONTEXT *f_pcontextBBX,
    __in_bcount( f_cbData )  const         DRM_BYTE       *f_pbData,
    __in                                   DRM_DWORD       f_cbData,
    __in_bcount_opt( f_cbExtraData ) const DRM_BYTE       *f_pbExtraData,
    __in                                   DRM_DWORD       f_cbExtraData,
    __out_bcount( DRM_SHA1_DIGEST_LEN )    DRM_BYTE        f_rgbSymSig[__CB_DECL( DRM_SHA1_DIGEST_LEN )] );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_SymmetricVerify(
    __inout_ecount( 1 )                      DRM_BB_CONTEXT *f_pcontextBBX,
    __in_opt                           const PUBKEY         *f_pPubKey,
    __in_bcount( f_cbData )            const DRM_BYTE       *f_pbData,
    __in                                     DRM_DWORD       f_cbData,
    __in_bcount_opt( f_cbExtraData )   const DRM_BYTE       *f_pbExtraData,
    __in                                     DRM_DWORD       f_cbExtraData,
    __in_opt                           const DRM_SLK        *f_pslk,
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE        f_rgbSymSig[__CB_DECL( DRM_SHA1_DIGEST_LEN )] );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_AsymmetricToSymmetricKeyXML(
    __inout_ecount( 1 )      DRM_BB_CONTEXT       *f_pcontextBBX,
    __inout_ecount_opt( 1 )  DRM_BINDING_INFO     *f_pbindinfo,
    __out_ecount( 1 )        DRM_DWORD            *f_pcbSymmKey );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_AsymmetricToSymmetricKeyXMR(
    __inout_ecount( 1 )               DRM_BB_CONTEXT                *f_pcontextBBX,
    __inout_ecount_opt( 1 )           DRM_BINDING_INFO              *f_pbindinfo,
    __inout_bcount( f_cbLicense )     DRM_BYTE                      *f_pbLicense,
    __in                              DRM_DWORD                      f_cbLicense,
    __inout_ecount( 1 )               DRM_DWORD                     *f_pcbLicenseUsed,
    __inout_ecount( 1 )               DRM_STACK_ALLOCATOR_CONTEXT   *f_pStack,
    __out_ecount( 1 )                 DRM_DWORD                     *f_pcbSymmKey );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_RebindSymmetricKeyXML(
    __inout         DRM_BB_CONTEXT       *f_pcontextBBX,
    __in      const DRM_SLK              *f_pslk,
    __inout         DRM_XML_BINDING_INFO *f_pXmlBindInfo,
    __in            DRM_DWORD             f_cbSymmKey );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_RebindSymmetricKeyXMR(
    __inout         DRM_BB_CONTEXT       *f_pcontextBBX,
    __in      const DRM_SLK              *f_pslk,
    __inout         DRM_XMR_LICENSE      *f_pXmrLicense );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_RebindSLK(
    __inout_ecount( 1 )             DRM_BB_CONTEXT *f_pcontextBBX,
    __in_bcount( f_cbEncSLK ) const DRM_BYTE       *f_pbEncSLK,
    __in                            DRM_DWORD       f_cbEncSLK,
    __out_ecount( 1 )               DRM_SLK        *f_pslk );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_StoreDomainPrivateKeys(
    __inout_ecount( 1 )         DRM_BB_CONTEXT *f_poBBXContext,
    __inout_ecount( 1 )         DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    __inout_bcount( f_cbData )  DRM_BYTE *f_pbData,
    __in                        DRM_DWORD f_cbData,
    __inout_ecount( 1 )         DRM_DOMAIN_ID *f_poDomainID,
    __in_ecount( 1 )            PUBKEY_P256 *f_poECC256PubKey,
    __in_ecount( 1 )            DRM_ANSI_STRING *f_pdastrCertificate );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_EncryptDomainPrivateKey(
    __inout_ecount( 1 )         DRM_BB_CONTEXT  *f_poBBXContext,
    __in_bcount( f_pdasstrInputPrivKey->m_ich + f_pdasstrInputPrivKey->m_cch ) DRM_CHAR *f_pszBase,
    __in                const   DRM_SUBSTRING   *f_pdasstrInputPrivKey,
    __out_ecount( 1 )           PRIVKEY_P256    *f_poOutputPrivKey );

#define SYMMETRIC_ENCRYPT TRUE
#define SYMMETRIC_DECRYPT FALSE
DRM_API DRM_RESULT DRM_CALL DRM_BBX_SymmetricCipher(
    __in                                DRM_BOOL  f_fEncrypt,
    __in                                DRM_BOOL  f_fUseLegacyCrypto,
    __in_bcount_opt( f_cbKey )   const  DRM_BYTE *f_pbKey,
    __in                                DRM_DWORD f_cbKey,
    __in_bcount( f_cbData )             DRM_BYTE *f_pbDataIn,
    __out_bcount_opt( f_cbData )        DRM_BYTE *f_pbDataOut,
    __in                                DRM_DWORD f_cbData );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_SymmetricBind(
    __inout_ecount( 1 )          DRM_BB_CONTEXT *f_poBBXContext,
    __in                         DRM_BOOL        f_fEncrypt,
    __in                         DRM_BOOL        f_fUseLegacyCrypto,
    __in_bcount( f_cbData )      DRM_BYTE       *f_pbDataIn,
    __out_bcount_opt( f_cbData ) DRM_BYTE       *f_pbDataOut,
    __in                         DRM_DWORD       f_cbData );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_ECC256_Decrypt(
    __inout_ecount( 1 )          DRM_BB_CONTEXT *f_poBBXContext,
    __inout_bcount( *f_pcbData ) DRM_BYTE       *f_pbData,
    __inout_ecount( 1 )          DRM_DWORD      *f_pcbData,
    __inout_ecount( 1 )          PUBKEY_P256    *f_poECC256PubKey );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_ECC256_Sign(
    __inout_ecount( 1 )                                DRM_BB_CONTEXT *f_poBBXContext,
    __inout_bcount( f_cbData )                         const DRM_BYTE *f_pbData,
    __in                                               DRM_DWORD       f_cbData,
    __inout_ecount( 1 )                                PUBKEY_P256    *f_poECC256PubKey,
    __out_bcount( ECDSA_P256_SIGNATURE_SIZE_IN_BYTES ) DRM_BYTE        f_rgbECC256Signature[ __CB_DECL( ECDSA_P256_SIGNATURE_SIZE_IN_BYTES ) ] );

DRM_API DRM_RESULT DRM_CALL DRM_BBX_SignWithCertKeys(
    __in_opt                                          DRM_VOID                           *f_pOEMContext,
    __inout                                           DRM_CRYPTO_CONTEXT                 *f_pcontextCRYP,
    __in                                              DRM_DEVICE_KEY_TYPE                 f_eKeyType,
    __in_bcount_opt(ECC_P256_INTEGER_SIZE_IN_BYTES)   DRM_BYTE                           *f_pbPrivKey,
    __in                                              DRM_DWORD                           f_cbData,
    __in_bcount( f_cbData ) const                     DRM_BYTE                           *f_pbData,
    __inout_ecount( 1 )                               DRM_DWORD                          *f_pcbSignature,
    __out_bcount( *f_pcbSignature )                   DRM_BYTE                           *f_pbSignature);

DRM_API DRM_RESULT DRM_CALL DRM_BBX_GenerateECCKeyPair(
    __inout                             DRM_CRYPTO_CONTEXT                 *f_pcontextCRYP,
    __in                                DRM_PKCRYPTO_SUPPORTED_ALGORITHMS   f_eAlgorithm,
    __inout_ecount( 1 )                 DRM_DWORD                          *f_pcbPublicKey,
    __out_bcount( *f_pcbPublicKey )     DRM_BYTE                           *f_pbPublicKey,
    __inout_ecount( 1 )                 DRM_DWORD                          *f_pcbPrivateKey,
    __out_bcount( *f_pcbPrivateKey )    DRM_BYTE                           *f_pbPrivateKey);

DRM_API DRM_RESULT DRM_CALL DRM_BBX_KF_DecryptKey (
    __in_opt                               DRM_VOID             *f_pOEMContext,
    __in                                   DRM_ENCRYPTED_KEY    *f_pEncryptedKey,
    __in                                   DRM_DEVICE_KEY_TYPE   f_eKeyType,
    __inout_bcount_opt(*f_pcbDecryptedKey) DRM_BYTE             *f_pbDecryptedKey,
    __inout                                DRM_DWORD            *f_pcbDecryptedKey
);


DRM_API DRM_RESULT DRM_CALL DRM_BBX_KF_EncryptKey (
    __in_opt                        DRM_VOID                  *f_pOEMContext,
    __in_bcount(f_cbClearKey) const DRM_BYTE                  *f_pbClearKey,
    __in                            DRM_DWORD                  f_cbClearKey,
    __in                            DRM_DEVICE_KEY_TYPE        f_eKeyType,
    __out                           DRM_ENCRYPTED_KEY         *f_pEncryptedKey );


DRM_API DRM_RESULT DRM_CALL DRM_BBX_KF_CalculateOmac (
    __in_opt    DRM_VOID    *f_pOEMContext,
    __inout     DRM_GUID    *f_pguidAESDecryptionKeyID,
    __in        DRM_BOOL     f_fUseCurrentKey,
    __in_bcount(f_cbData) const DRM_BYTE *f_pbData,
    __in        DRM_DWORD    f_cbData,
    __inout_bcount(DRM_AES_BLOCKLEN) DRM_BYTE f_rgbSignatureBuffer[DRM_AES_BLOCKLEN]
);

/*
** Note that this API must be called in the context of the keyfile context already open
** and locked for writing.
*/

DRM_API DRM_RESULT DRM_CALL DRM_BBX_SetupSecureStoreKey(
    DRM_KEYFILE_CONTEXT                 *f_pKeyFileContext,
    DRM_VOID                            *f_pOEMContext );

EXIT_PK_NAMESPACE;

#endif /* __DRMBLACKBOX_H__ */

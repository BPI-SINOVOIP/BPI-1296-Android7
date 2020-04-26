/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBLACKBOX_H__
#define __DRMBLACKBOX_H__


#ifdef __cplusplus
extern "C" {
#endif

#define DRM_PK_BUFFER_MAX   PK_ENC_PLAINTEXT_LEN
#define DRM_MAX_DRMOLDKEYS 20
#define DRMKEYPAIRSIZE (SIZEOF( PUBKEY ) + SIZEOF( PRIVKEY )) 

typedef struct __tagDRM_BB_CONTEXT
{
    /* Make sure the crypto context is the first member in this struct */
    DRM_CRYPTO_CONTEXT                   CryptoContext;
    DRM_DEVICE_CERTIFICATE_CACHED_VALUES cachedCertValues;
    DRM_BOOL                             fInited;
} DRM_BB_CONTEXT;

typedef enum _BB_SUPPORTED_ALGORITHMS
{
    eMSDRM_PK  = 1,  /* The key is encrypted with a ECC public key */
    eMSDRM_CK  = 2,  /* This is a chained license key algorithm. ( DES encrypted ) */
    eMSDRM_SYM = 3   /* The license is symmetrically bound.  This means that is was actually rebound to a symmetric key once is was stored on the device */
} BB_SUPPORTED_ALGORITHMS;


/* The order of these entries is important, the byte arrays MUST
   be DWORD aligned, as some platforms require this. m_rgbChecksum
   has an odd number of bytes, and so must be placed at the end. */
typedef struct __tagDRM_BINDING_INFO
{
    DRM_BYTE   m_rgbContentKey[__CB_DECL(PK_ENC_CIPHERTEXT_LEN)];
    DRM_BYTE   m_rgbSignature[__CB_DECL(PK_ENC_SIGNATURE_LEN)];
    DRM_BYTE   m_rgbChecksum[__CB_DECL(CHECKSUM_LENGTH)];
    PUBKEY     m_oPublKey;
    PUBKEY     m_oLsPublKey;
    DRM_DWORD  m_dwAlgorithm;
} DRM_BINDING_INFO;

DRM_RESULT DRM_API DRM_BBX_Initialize( 
    IN OUT DRM_BB_CONTEXT     *pcontextBB );

DRM_RESULT DRM_API DRM_BBX_GetClientId(
    OUT    CLIENTID       *pClientId,
    IN OUT DRM_BB_CONTEXT *pcontextBB );

DRM_RESULT DRM_API DRM_BBX_Shutdown( 
    IN DRM_BB_CONTEXT *pcontextBB );

DRM_BOOL DRM_API DRM_BBX_CanBind( 
    IN const DRM_BINDING_INFO *rgBindingInfo ,
    IN       DRM_DWORD         dwChainLength,
    IN       DRM_BB_CONTEXT   *pcontextBB );

DRM_RESULT DRM_API DRM_BBX_HashValue(
    IN  const DRM_BYTE       *pbValue,
    IN        DRM_DWORD       cbValue,
    OUT       DRM_BYTE        rgbHashValue [__CB_DECL(SHA_DIGEST_LEN)],
    IN        DRM_BB_CONTEXT *pcontextBB );
    
DRM_BOOL DRM_API DRM_BBX_DecryptLicense(
    IN OUT DRM_BYTE       *pbEncryptedLicense,
    IN     DRM_DWORD       cbEncryptedLicense,
    OUT    DRM_BYTE       *pbLicense,
    IN     DRM_BB_CONTEXT *pcontextBB );

DRM_RESULT DRM_API DRM_BBX_CipherKeySetup( 
    IN const DRM_BINDING_INFO   *rgBindingInfo,
    IN       DRM_DWORD           dwChainLength,
    OUT      DRM_CIPHER_CONTEXT *pCipherContext,
    IN       DRM_BB_CONTEXT     *pcontextBB );

DRM_RESULT DRM_API DRM_BBX_InitDecrypt( 
    IN     DRM_CIPHER_CONTEXT *f_pcontextCipher,
    IN     DRM_BYTE           *f_pbLast15,
    IN     DRM_DWORD           f_cbData );

DRM_RESULT DRM_API DRM_BBX_Decrypt( 
    IN     DRM_DWORD           cbData,
    IN OUT DRM_BYTE           *pbData,
    IN     DRM_CIPHER_CONTEXT *pDecryptContext);

DRM_RESULT DRM_API DRM_BBX_Encrypt( 
    IN     DRM_DWORD           cbData,
    IN OUT DRM_BYTE           *pbData,
    IN     DRM_CIPHER_CONTEXT *pEncryptContext );

DRM_RESULT DRM_API DRM_BBX_LegacyCipherKeySetup( 
    IN  DRM_BINDING_INFO   *pBindInfo, 
    IN  DRM_BYTE           *pbKID,
    IN  DRM_DWORD           cbKID,
    IN  DRM_BYTE           *pbSecretData,
    IN  DRM_DWORD           cbSecretData,
    OUT DRM_CIPHER_CONTEXT *pCipherContext, 
    IN  DRM_BB_CONTEXT     *pcontextBB);


typedef enum
{
    eBBX_PRIVKEY_MACHINE               = 1,
    eBBX_PRIVKEY_RESTORELICENSESIGNER  = 2,
    eBBX_PRIVKEY_TRANSFERLICENSESIGNER = 3
} eBBX_SIGNING_KEY_TYPES;

DRM_RESULT DRM_API DRM_BBX_SignData(
    IN       DRM_BB_CONTEXT *f_pcontextBBX,
    IN const DRM_BYTE       *f_pbData,
    IN       DRM_DWORD       f_cbData,
    IN       DRM_DWORD       f_ePrivKeyType,
       OUT   DRM_BYTE        f_rgbSignature[PK_ENC_SIGNATURE_LEN] );
#if DRM_SUPPORT_SYMMETRIC_OPTIMIZATIONS

DRM_RESULT DRM_API DRM_BBX_SymmetricSign(
    IN       DRM_BB_CONTEXT *f_pcontextBBX,
    IN const DRM_BYTE       *f_pbData,
    IN       DRM_DWORD       f_cbData,
       OUT   DRM_BYTE        f_rgbSymSig[__CB_DECL( SHA_DIGEST_LEN )] );

DRM_RESULT DRM_API DRM_BBX_SymmetricVerify(
    IN       DRM_BB_CONTEXT *f_pcontextBBX,
    IN const DRM_BYTE       *f_pbData,
    IN       DRM_DWORD       f_cbData,
    IN const DRM_SLK        *f_pslk,
    IN const DRM_BYTE        f_rgbSymSig[__CB_DECL( SHA_DIGEST_LEN )] );

DRM_RESULT DRM_API DRM_BBX_AsymmetricToSymmetricKey(
    IN     DRM_BB_CONTEXT   *f_pcontextBBX,
    IN OUT DRM_BINDING_INFO *f_pbindinfo,
       OUT DRM_DWORD        *f_pcbSymmKey );

DRM_RESULT DRM_API DRM_BBX_RebindSymmetricKey(
    IN       DRM_BB_CONTEXT *f_pcontextBBX,
    IN const DRM_SLK        *f_pslk,
    IN OUT   DRM_BYTE       *f_pbSymmKey,
       OUT   DRM_DWORD       f_cbSymmKey );

DRM_RESULT DRM_API DRM_BBX_RebindSLK(
    IN       DRM_BB_CONTEXT *f_pcontextBBX,
    IN const DRM_BYTE        f_rgbEncSLK[__CB_DECL(PK_ENC_CIPHERTEXT_LEN)],
       OUT   DRM_SLK        *f_pslk );

#endif

#ifdef __cplusplus
}
#endif

#endif /* __DRMBLACKBOX_H__ */

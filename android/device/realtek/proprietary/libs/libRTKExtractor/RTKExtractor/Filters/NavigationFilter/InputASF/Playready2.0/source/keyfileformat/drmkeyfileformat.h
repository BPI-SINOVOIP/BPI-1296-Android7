/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _DRMKEYFILEFORMAT_H_
#define _DRMKEYFILEFORMAT_H_ 1

#include <drmxbbuilder.h>
#include <drmxbparser.h>

ENTER_PK_NAMESPACE;

/* TASK: Set reasonable numbers here for the buffer/stack sizes */
#define DRM_KEYFILE_PARSER_STACK_SIZE 2048
#define DRM_KEYFILE_BUILDER_STACK_SIZE 2048
#define DRM_KEYFILE_MAX_SIZE 16384

/* Must increment the count when new entry types are added, +1 for global header */
#define KF1_ENTRY_TYPE_COUNT 10
typedef enum KEYFILE_1_ENTRY_TYPES
{
    KF_ENTRY_TYPE_ESCROWED_DATA_CONTAINER           = 0x1000,
    KF_ENTRY_TYPE_SYMMETRIC_KEY_OBJECT              = 0x1001,
    KF_ENTRY_TYPE_ASYMMETRIC_KEY_OBJECT             = 0x1002,
    KF_ENTRY_TYPE_KEYHISTORY_CONTAINER              = 0x1003,
    KF_ENTRY_TYPE_KEYPAIR_OBJECT                    = 0x1004,
    KF_ENTRY_TYPE_CERTIFICATE_CHAIN_OBJECT          = 0x1005,
    KF_ENTRY_TYPE_HWID_VALUE                        = 0x1006,
    KF_ENTRY_TYPE_SECURE_STORE_KEY_OBJECT           = 0x1007,
    KF_ENTRY_TYPE_SIGNATURE_OBJECT                  = 0x1008,
} KEYFILE_1_ENTRY_TYPES;

#define KF2_ENTRY_TYPE_COUNT 12
typedef enum KEYFILE_2_ENTRY_TYPES
{
/* These objects are valid in keyfile v2 formats
** they can't be repeated otherwise there are compiler breaks
**
**  KF_ENTRY_TYPE_ESCROWED_DATA_CONTAINER           = 0x1000,
**  KF_ENTRY_TYPE_SYMMETRIC_KEY_OBJECT              = 0x1001,
**  KF_ENTRY_TYPE_ASYMMETRIC_KEY_OBJECT             = 0x1002,
**  KF_ENTRY_TYPE_HWID_VALUE                        = 0x1006,
**  KF_ENTRY_TYPE_SIGNATURE_OBJECT                  = 0x1008,
*/
    KF_ENTRY_TYPE_KEYHISTORY_CONTAINER_2            = 0x1009,
    KF_ENTRY_TYPE_KEYPAIR_OBJECT_2                  = 0x100A,
    KF_ENTRY_TYPE_SECURE_STORE_KEY_OBJECT_2         = 0x100B,
    KF_ENTRY_TYPE_PLATFORM_OBJECT                   = 0x100C,
    KF_ENTRY_TYPE_CERT_PARSER_CACHE_OBJECT          = 0x100D,
    KF_ENTRY_TYPE_CERTIFICATE_CHAIN_OBJECT_2        = 0x100E,
} KEYFILE_2_ENTRY_TYPES;

#define KF3_ENTRY_TYPE_COUNT 17
typedef enum KEYFILE_3_ENTRY_TYPES
{
/* These objects are valid in keyfile v3 formats
** they can't be repeated otherwise there are compiler breaks
**
**  KF_ENTRY_TYPE_ESCROWED_DATA_CONTAINER           = 0x1000,
**  KF_ENTRY_TYPE_SYMMETRIC_KEY_OBJECT              = 0x1001,
**  KF_ENTRY_TYPE_ASYMMETRIC_KEY_OBJECT             = 0x1002,
**  KF_ENTRY_TYPE_HWID_VALUE                        = 0x1006,
**  KF_ENTRY_TYPE_SIGNATURE_OBJECT                  = 0x1008,
**  KF_ENTRY_TYPE_KEYHISTORY_CONTAINER_2            = 0x1009,
**  KF_ENTRY_TYPE_KEYPAIR_OBJECT_2                  = 0x100A,
**  KF_ENTRY_TYPE_SECURE_STORE_KEY_OBJECT_2         = 0x100B,
**  KF_ENTRY_TYPE_PLATFORM_OBJECT                   = 0x100C,
**  KF_ENTRY_TYPE_CERT_PARSER_CACHE_OBJECT          = 0x100D,
**  KF_ENTRY_TYPE_CERTIFICATE_CHAIN_OBJECT_2        = 0x100E,
*/
    KF_ENTRY_TYPE_KEYPAIR_OBJECT_3                  = 0x100F,
    KF_ENTRY_TYPE_SECURE_STORE_KEY_OBJECT_3         = 0x1010,
    KF_ENTRY_TYPE_ESCROWED_SECURE_STORE_KEY_OBJECT  = 0x1011,
    KF_ENTRY_TYPE_ESCROWED_KEYPAIR_OBJECT           = 0x1012,
    KF_ENTRY_TYPE_DEVICE_APP_DATA                   = 0x1013,
} KEYFILE_3_ENTRY_TYPES;

typedef enum DRM_KEYFILE_CERT_TYPE
{
    KF_CERT_TYPE_INVALID    = 0,
    KF_CERT_TYPE_WMDRM      = 1,
    KF_CERT_TYPE_NDT        = 2,
    KF_CERT_TYPE_PLAYREADY  = 3,
} DRM_KEYFILE_CERT_TYPE;

typedef enum DRM_KEYFILE_ASYMMETRIC_KEY_TYPE
{
    KF_ASYMM_KEY_TYPE_INVALID = 0,
    KF_ASYMM_KEY_TYPE_ECC_256 = 1,
    KF_ASYMM_KEY_TYPE_RSA     = 2,
    KF_ASYMM_KEY_TYPE_ECC_160 = 3,
} DRM_KEYFILE_ASYMMETRIC_KEY_TYPE;

typedef enum DRM_KEYFILE_SYMMETRIC_KEY_TYPE
{
    KF_SYMM_KEY_TYPE_AES = 2 /* eDRM_AES_COUNTER_CIPHER */
} DRM_KEYFILE_SYMMETRIC_KEY_TYPE;

#define KEYFILE_PLATFORM_PC                                 (0x0001)
#define KEYFILE_PLATFORM_FIJI                               (0x0002)
#define KEYFILE_PLATFORM_WINDOWSMOBILE7                     (0x0003)
#define KEYFILE_PLATFORM_PK                                 (0x0004)

#define KEYFILE_HWID_TYPE_WPA                               (0x0001)

#define KEYFILE_SIGNATURE_TYPE_ECDSA_256_SHA_256            (0x0001)
#define KEYFILE_SIGNATURE_TYPE_OMAC_1                       (0x0002)

#define KEYFILE_VERSION_1                                   (0x00010000)
#define KEYFILE_VERSION_2                                   (0x00000002)
#define KEYFILE_VERSION_3                                   (0x00000003)
#define KEYFILE_VERSION_CURRENT                             KEYFILE_VERSION_3

typedef struct __tagKF_PLATFORM_OBJECT
{
    DRM_BOOL  fValid;
    DRM_WORD  wPlatform;
    DRM_WORD  wReserved;
} KF_PLATFORM_OBJECT;

typedef struct __tagKF_KEYPAIR_V1_OBJECT
{
    DRM_BOOL  fValid;
    struct __tagKF_KEYPAIR_V1_OBJECT *pNext;

    DRM_WORD  wCipherType;
    DRM_WORD  wKeySize;
    DRM_XB_BYTEARRAY xbbaPublicKeyData;
    DRM_UINT64 qwInitializationVector;
    DRM_XB_BYTEARRAY xbbaEncryptedPrivateKey;
} KF_KEYPAIR_V1_OBJECT;

typedef struct __tagKF_KEYPAIR_V2_OBJECT
{
    DRM_BOOL  fValid;
    struct __tagKF_KEYPAIR_V2_OBJECT *pNext;

    DRM_WORD  wCipherType;
    DRM_WORD  wKeySize;
    DRM_XB_BYTEARRAY xbbaPublicKeyData;
    DRM_XB_BYTEARRAY xbbaInitializationVector;
    DRM_XB_BYTEARRAY xbbaEncryptedPrivateKey;
    DRM_GUID  guidKeypairDecryptionKeyID;
    DRM_WORD  wKeypairDecryptionMethod;
    DRM_WORD  wReserved;
} KF_KEYPAIR_V2_OBJECT;


typedef struct __tagKF_KEYPAIR_V3_OBJECT
{
    DRM_BOOL  fValid;
    struct __tagKF_KEYPAIR_V3_OBJECT *pNext;

    DRM_WORD  wCipherType;
    DRM_WORD  wKeySize;
    DRM_XB_BYTEARRAY xbbaPublicKeyData;
    DRM_UINT64 qwInitializationVector;
    DRM_XB_BYTEARRAY xbbaEncryptedPrivateKey;
    DRM_GUID  guidKeypairDecryptionKeyID;
    DRM_WORD  wKeypairDecryptionMethod;
    DRM_WORD  wReserved;
} KF_KEYPAIR_V3_OBJECT;

typedef KF_KEYPAIR_V3_OBJECT KF_ESCROWED_KEYPAIR_OBJECT;

typedef struct __tagKF_SECURE_STORE_KEY_V1_OBJECT
{
    DRM_BOOL  fValid;
    DRM_WORD  wCipherType;
    DRM_WORD  wKeySize;
    DRM_UINT64  qwInitializationVector;
    DRM_XB_BYTEARRAY xbbaEncryptedSecureStoreKey;
} KF_SECURE_STORE_KEY_V1_OBJECT;

typedef struct __tagKF_SECURE_STORE_KEY_V2_OBJECT
{
    DRM_BOOL  fValid;
    DRM_WORD  wCipherType;
    DRM_WORD  wKeySize;
    DRM_XB_BYTEARRAY xbbaInitializationVector;
    DRM_XB_BYTEARRAY xbbaEncryptedSecureStoreKey;
    DRM_GUID  guidKeypairDecryptionKeyID;
    DRM_WORD  wKeypairDecryptionMethod;
    DRM_WORD  wReserved;
} KF_SECURE_STORE_KEY_V2_OBJECT;

typedef struct __tagKF_SECURE_STORE_KEY_V3_OBJECT
{
    DRM_BOOL  fValid;
    DRM_WORD  wCipherType;
    DRM_WORD  wKeySize;
    DRM_UINT64  qwInitializationVector;
    DRM_XB_BYTEARRAY xbbaEncryptedSecureStoreKey;
    DRM_GUID  guidKeypairDecryptionKeyID;
    DRM_WORD  wKeypairDecryptionMethod;
    DRM_WORD  wReserved;
} KF_SECURE_STORE_KEY_V3_OBJECT;

/* The escrowed SST object is identical to the SST key v3 object */
typedef KF_SECURE_STORE_KEY_V3_OBJECT KF_ESCROWED_SECURE_STORE_KEY_OBJECT;

typedef struct __tagKF_KEYHISTORY_V1
{
    DRM_BOOL                    fValid;
    KF_KEYPAIR_V1_OBJECT       *pHead;
    DRM_DWORD                   dwReserved;
} KF_KEYHISTORY_V1;

typedef struct __tagKF_KEYHISTORY_V2
{
    DRM_BOOL                    fValid;
    KF_KEYPAIR_V2_OBJECT       *pKeyPairHeadv2;
    KF_KEYPAIR_V3_OBJECT       *pKeyPairHeadv3;

} KF_KEYHISTORY_V2;


typedef struct __tagKF_SYMMETRICKEY_OBJECT
{
    DRM_BOOL  fValid;
    DRM_WORD  wCipherType;
    DRM_WORD  wKeySize;
    DRM_DWORD dwIndivEscrowKeyVersion;      /* Indiv Escrow Key Version (high 16 bits Major version, low 16 bits Minor version) */
    DRM_UINT64 qwInitializationVector;
    DRM_XB_BYTEARRAY xbbaEncryptedKey;
} KF_SYMMETRICKEY_OBJECT;

typedef struct __tagKF_ASYMMETRICKEY_OBJECT
{
    DRM_BOOL  fValid;
    DRM_WORD  wCipherType;
    DRM_WORD  wKeySize;
    DRM_XB_BYTEARRAY xbbaPublicKey;
    DRM_UINT64 qwInitializationVector;
    DRM_XB_BYTEARRAY xbbaEncryptedPrivateKey;
} KF_ASYMMETRICKEY_OBJECT;

typedef struct __tagKF_ESCROWDATA
{
    DRM_BOOL                             fValid;
    KF_SYMMETRICKEY_OBJECT               SymmetricKeyv1;
    KF_ESCROWED_SECURE_STORE_KEY_OBJECT  sstKey;
    KF_ASYMMETRICKEY_OBJECT              ASymmetricKey;
DRM_OBFUS_PTR_TOP
    KF_ESCROWED_KEYPAIR_OBJECT          *pKeyPairHead;
DRM_OBFUS_PTR_BTM
} KF_ESCROWDATA;

typedef struct __tagKF_CERTIFICATE_CHAIN_OBJECT_V1
{
    DRM_BOOL  fValid;
    struct __tagKF_CERTIFICATE_CHAIN_OBJECT_V1 *pNext;

    DRM_WORD  wCertificateType;
    DRM_XB_BYTEARRAY xbbaCertificateChain;
} KF_CERTIFICATE_CHAIN_OBJECT_V1;

typedef struct __tagKF_CERTIFICATE_CHAIN_OBJECT_V2
{
    DRM_BOOL  fValid;
    struct __tagKF_CERTIFICATE_CHAIN_OBJECT_V2 *pNext;

    DRM_DWORD  dwCertificateType;
    DRM_XB_BYTEARRAY xbbaCertificateChain;
} KF_CERTIFICATE_CHAIN_OBJECT_V2;

typedef struct __tagKF_HWID_OBJECT
{
    DRM_BOOL  fValid;
    DRM_WORD  wType;
    DRM_XB_BYTEARRAY xbbaHWID;
} KF_HWID_OBJECT;

typedef struct __tagKF_CERT_PARSER_CACHE_OBJECT
{
    DRM_BOOL  fValid;
    DRM_WORD  wVersion;
    DRM_XB_BYTEARRAY xbbaHash;
    DRM_XB_WORDLIST wlData;
} KF_CERT_PARSER_CACHE_OBJECT;

typedef struct __tagKF_SIGNATURE_OBJECT
{
    DRM_BOOL  fValid;
    DRM_XB_BYTEARRAY xbbaSigningCertificate;
    DRM_WORD  wSignatureType;
    DRM_XB_BYTEARRAY xbbaSignatureData;
} KF_SIGNATURE_OBJECT;

typedef struct __tagKF_HEADER_EXTRA_DATA_V1
{
    DRM_BOOL   fValid;
    DRM_WORD   wPlatform;
    DRM_WORD   wReserved;
    DRM_DWORD  dwLength;
    DRM_DWORD  dwSignedLength;
} KF_HEADER_EXTRA_DATA_V1;

typedef struct __tagKF_DEVICE_APP_DATA
{
    DRM_BOOL         fValid;
    DRM_DWORD        dwAppVersion;
    DRM_XB_BYTEARRAY xbbaAppId;
} KF_DEVICE_APP_DATA;

typedef struct __tagDRM_KEYFILE
{
    DRM_BOOL                    fValid;
    DRM_DWORD                   dwVersion; /* Filled during DRM_KF_Parse */

    KF_HEADER_EXTRA_DATA_V1     HeaderExtra;
    KF_PLATFORM_OBJECT          Platform;
    KF_ESCROWDATA               EscrowedData;

    KF_KEYHISTORY_V1               KeyHistoryV1;
    KF_KEYHISTORY_V2               KeyHistoryV2;


    KF_CERTIFICATE_CHAIN_OBJECT_V1 *pCertChainV1;
    KF_CERTIFICATE_CHAIN_OBJECT_V2 *pCertChainV2;

    KF_HWID_OBJECT  HWIDObject; /* Only used in NDT keyfiles */

    KF_SECURE_STORE_KEY_V1_OBJECT  SecureStoreKeyV1;
    KF_SECURE_STORE_KEY_V2_OBJECT  SecureStoreKeyV2;
    KF_SECURE_STORE_KEY_V3_OBJECT  SecureStoreKeyV3;

    KF_CERT_PARSER_CACHE_OBJECT CertParserCache;
    KF_DEVICE_APP_DATA          DeviceAppData;

    KF_SIGNATURE_OBJECT         Signature;
} DRM_KEYFILE;

/* Escrowed group private key and certificate chain
   that is required for activation.  This data is stored 
   in robust form in static lib and is sent up as part
   of the activation request.  The Activation service uses
   these creds to issue a client cert/priv key that is
   returned as part of the keyfile back to the device.
*/

typedef struct __tagDRM_ACTIVATION_DATA
{
    DRM_BOOL                    fValid;
    DRM_DWORD                   dwVersion; /* Filled during DRM_AD_Parse */

    KF_SYMMETRICKEY_OBJECT          SymmetricKey;
    KF_ASYMMETRICKEY_OBJECT         ASymmetricKey;
    KF_CERTIFICATE_CHAIN_OBJECT_V2 *pCertChainV2;

} DRM_ACTIVATION_DATA;

#define AD1_ENTRY_TYPE_COUNT 4
#define ACTIVATION_DATA_VERSION_1                                 (0x00000001)
#define CURRENT_ACTIVATION_DATA_VERSION                           ACTIVATION_DATA_VERSION_1

/* Use DRM_KF_Start to begin the building of a keyfile.
** Add object to the move page using
**   DRM_XB_AddObject
*/
DRM_API DRM_RESULT DRM_CALL DRM_KF_Start(
     __inout_bcount( f_cbStack ) DRM_BYTE                *f_pbStack,
    __in                        DRM_DWORD                f_cbStack,
    __in                        DRM_DWORD                f_dwVersion,
    __in_opt                    KF_SIGNATURE_OBJECT     *f_psignatureData,
    __inout_ecount( 1 )         DRM_XB_BUILDER_CONTEXT  *f_pcontextBuilder  );

typedef DRM_RESULT ( DRM_CALL *DRM_KF_CB_SIGNATURE )(
    __in_ecount( 1 )        const                            DRM_VOID  *f_pvSignatureContext,
    __in_bcount( f_cbData )                                  DRM_BYTE  *f_pbData,
    __in                                                     DRM_DWORD  f_cbData,
    __out_bcount( f_cbSignatureBuffer )                      DRM_BYTE  *f_pbSignatureBuffer,
    __in                                                     DRM_DWORD  f_cbSignatureBuffer,
    __in                                                     DRM_DWORD  f_iSignatureBuffer );

DRM_API DRM_RESULT DRM_CALL DRM_KF_Finish(
    __inout_ecount( 1 )             DRM_XB_BUILDER_CONTEXT  *f_pcontextBuilder,
    __inout_bcount(*f_pcbKeyFile)   DRM_BYTE                *f_pbKeyFile,
    __inout                         DRM_DWORD               *f_pcbKeyFile,
    __in                            DRM_KF_CB_SIGNATURE      f_pfnSignatureCallback,
    __in                            DRM_VOID                *f_pvSignatureContext);

DRM_API DRM_RESULT DRM_CALL DRM_KF_Parse(
    __in                              DRM_VOID                *f_pOEMContext,
    __inout_bcount( f_cbStack )       DRM_BYTE                *f_pbStack,
    __in                              DRM_DWORD                f_cbStack,
    __in_bcount( f_cbStack )    const DRM_BYTE                *f_pbKeyFile,
    __in                              DRM_DWORD                f_cbKeyFile,
    __in                              DRM_BOOL                 f_fValidateSignature,
    __out                             DRM_KEYFILE             *f_pKeyFile );

DRM_API DRM_RESULT DRM_CALL DRM_AD_Start(
    __inout_bcount( f_cbStack ) DRM_BYTE                *f_pbStack,
    __in                        DRM_DWORD                f_cbStack,
    __inout_ecount( 1 )         DRM_XB_BUILDER_CONTEXT  *f_pcontextBuilder);

DRM_API DRM_RESULT DRM_CALL DRM_AD_Parse(
    __inout_bcount( f_cbStack )       DRM_BYTE                *f_pbStack,
    __in                              DRM_DWORD                f_cbStack,
    __in_bcount( f_cbActivationData ) const DRM_BYTE          *f_pbActivationData,
    __in                              DRM_DWORD                f_cbActivationData,
    __out                             DRM_ACTIVATION_DATA     *f_pActivationData );

DRM_DWORD _VersionToFormatDescriptionTranslation( DRM_DWORD f_dwVersion );

#define ADJUST_VERSION_TO_INDEX( x ) _VersionToFormatDescriptionTranslation(x)
extern const DRM_XB_FORMAT_DESCRIPTION g_kfFormatDescription[];

EXIT_PK_NAMESPACE;

#endif /* _DRMKEYFILEFORMAT_H_ */

/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _DRMKEYFILE_H_
#define _DRMKEYFILE_H_ 1

#include <drmkeyfileformat.h>
#include <drmstkalloc.h>
#include <drmdevcertcache.h>

ENTER_PK_NAMESPACE;

/* TASK: Set reasonable numbers here for the buffer/stack sizes */
#define DRM_KEYFILE_MAX_SIZE 16384

typedef struct {
    DRM_WORD       wKeypairDecryptionMethod;
    DRM_WORD       wKeySize;
    DRM_BYTE       rgbInitializationVector[64];
DRM_OBFUS_FILL_BYTES(4)
DRM_OBFUS_PTR_TOP
    DRM_BYTE      *pbEncryptedKey;
DRM_OBFUS_PTR_BTM
    DRM_DWORD      cbEncryptedKey;
    DRM_GUID       guidKeypairDecryptionKeyID;
    DRM_WORD       wReserved;
} DRM_ENCRYPTED_KEY;

typedef struct
{
    DRM_BOOL fInited;
    DRM_BOOL fLoaded;
    DRM_VOID *pOEMContext;
    OEM_FILEHDL hKeyfile;
    DRM_KEYFILE keyfile;
    DRM_XB_BUILDER_CONTEXT builderContext;
    DRM_BYTE rgbParserStack[DRM_KEYFILE_PARSER_STACK_SIZE];
    DRM_BYTE rgbBuilderStack[DRM_KEYFILE_BUILDER_STACK_SIZE];
    DRM_DWORD cbKeyfileBuffer;
    DRM_BYTE *pbKeyfileBuffer;
    DRM_BYTE rgbKeyfileBuffer[DRM_KEYFILE_MAX_SIZE];
} DRM_KEYFILE_CONTEXT;


DRM_API DRM_RESULT DRM_CALL DRM_KF_Initialize (
    IN       DRM_VOID                   *f_pOEMContext,
       OUT   DRM_KEYFILE_CONTEXT        *f_pKeyFile);

DRM_API DRM_RESULT DRM_CALL DRM_KF_Uninitialize (
       IN   DRM_KEYFILE_CONTEXT         *f_pKeyFile);

DRM_API DRM_RESULT DRM_CALL DRM_KF_Open (
    __inout  DRM_KEYFILE_CONTEXT     *f_pKeyFile,
    __in     DRM_DWORD                f_fCreateIfNotExists,
    __in     DRM_DWORD                f_dwAccessMode );

DRM_API DRM_RESULT DRM_CALL DRM_KF_LoadFromMemory (
    __in_ecount(f_cbKeyFile)   const DRM_BYTE            *f_pbKeyFile,
    __in                             DRM_DWORD            f_cbKeyFile,  
    __inout                          DRM_KEYFILE_CONTEXT *f_pKeyFile );

DRM_API DRM_RESULT DRM_CALL DRM_KF_LoadFromFile (
    IN OUT   DRM_KEYFILE_CONTEXT        *f_pKeyFile,
    IN       DRM_BOOL                    f_fValidateSignature);

DRM_API DRM_RESULT DRM_CALL DRM_KF_StoreToFile (
    IN OUT   DRM_KEYFILE_CONTEXT         *f_pKeyFile);

DRM_API DRM_RESULT DRM_CALL DRM_KF_Close(
    IN   DRM_KEYFILE_CONTEXT   *f_pKeyFile);

typedef enum DRM_KF_CERT_TYPE
{
    eKF_CERT_TYPE_INVALID,
    eKF_CERT_TYPE_NDT,
    eKF_CERT_TYPE_WMDRM,
    eKF_CERT_TYPE_PLAYREADY,
} DRM_KF_CERT_TYPE;

DRM_API DRM_KEYFILE_CERT_TYPE DRM_CALL MapApiCertTypeToFormatCertType( DRM_KF_CERT_TYPE f_eCertType );

DRM_API DRM_RESULT DRM_CALL DRM_KF_GetCertificate (
    IN     DRM_KEYFILE_CONTEXT   *f_pKeyFile,
    IN     DRM_KF_CERT_TYPE       f_eCertType,
    OUT    DRM_BYTE             **f_ppbCertData,
    IN OUT DRM_DWORD             *f_pcbCertData);

DRM_API DRM_RESULT DRM_CALL DRM_KF_SetCertificate (
    IN                              DRM_KEYFILE_CONTEXT   *f_pKeyFile,
    IN                              DRM_KF_CERT_TYPE       f_eCertType,
    __in_bcount(f_cbCertData)       const DRM_BYTE        *f_pbCertData,
    IN                              DRM_DWORD              f_cbCertData);

typedef enum DRM_KF_KEY_TYPE
{
    eKF_KEY_TYPE_INVALID,
    eKF_KEY_TYPE_RSA,
    eKF_KEY_TYPE_ECC_160,
    eKF_KEY_TYPE_ECC_256,
    eKF_KEY_SECURE_STORE
} DRM_KF_KEY_TYPE;

DRM_API DRM_KEYFILE_ASYMMETRIC_KEY_TYPE DRM_CALL MapApiKeyTypeToFormatKeyType( DRM_KF_KEY_TYPE f_eKeyType );

DRM_API DRM_RESULT DRM_CALL DRM_KF_GetPrivateKey (
    IN                              DRM_KEYFILE_CONTEXT   *f_pKeyFile,
    IN                              DRM_KF_KEY_TYPE        f_eKeyType,
    __in_bcount_opt(f_cbPubKey)     const DRM_BYTE        *f_pbPubKey,
    IN                              DRM_DWORD              f_cbPubKey,
    OUT                             DRM_ENCRYPTED_KEY     *f_pKey);

DRM_API DRM_RESULT DRM_CALL DRM_KF_SetPrivateKey (
    IN                              DRM_KEYFILE_CONTEXT   *f_pKeyFile,
    IN                              DRM_KF_KEY_TYPE        f_eKeyType,
    __in_bcount_opt(f_cbPubKey)     const DRM_BYTE        *f_pbPubKey,
    IN                              DRM_DWORD              f_cbPubKey,
    IN                              DRM_ENCRYPTED_KEY     *f_pKey);

DRM_API DRM_RESULT DRM_CALL DRM_KF_SetCertParserCache (
    IN  DRM_KEYFILE_CONTEXT          *f_pKeyFile,
    IN  DRM_DEVICE_CERT_PARSER_CACHE *f_pCertParserCache);

EXIT_PK_NAMESPACE;

#endif /* _DRMKEYFILE_H_ */


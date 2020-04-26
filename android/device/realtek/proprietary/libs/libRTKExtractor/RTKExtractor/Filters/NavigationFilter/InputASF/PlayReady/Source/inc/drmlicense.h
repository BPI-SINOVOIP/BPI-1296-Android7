/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMLICENSE_H__
#define __DRMLICENSE_H__

#include "drmpkcrypto.h"

ENTER_PK_NAMESPACE;

#define ISSUER_LEN           4
#define SUBJ_LEN             4
#define DATE_LEN             4
#define SER_NUM_LEN          4
#define INT_LEN              4

#define VER_STRING_MAX_LEN 20

/* Constant indicating the number of times SHA has to be used to get the hash of key. */
/* This hash is used to check whether the key is good or bad. */
/* CHECKSUM_LENGTH indicates the number of bytes in final hash value to use as CHECKSUM. */
#define SHA_ITERATIONS      5
#define CHECKSUM_LENGTH     7
#define DRM_AES_CK_CHECKSUM_LENGTH 8
#define APPSEC_LEN   4
#define RIGHTS_LEN   4

/* V1 rights bits */
extern const DRM_BYTE RIGHT_PLAY_ON_PC            [__CB_DECL(RIGHTS_LEN)];
extern const DRM_BYTE RIGHT_COPY_TO_NONSDMI_DEVICE[__CB_DECL(RIGHTS_LEN)];
extern const DRM_BYTE RIGHT_NO_RESTORE            [__CB_DECL(RIGHTS_LEN)];
extern const DRM_BYTE RIGHT_BURN_TO_CD            [__CB_DECL(RIGHTS_LEN)];
extern const DRM_BYTE RIGHT_COPY_TO_SDMI_DEVICE   [__CB_DECL(RIGHTS_LEN)];
extern const DRM_BYTE RIGHT_ONE_TIME              [__CB_DECL(RIGHTS_LEN)];
extern const DRM_BYTE RIGHT_COPY                  [__CB_DECL(RIGHTS_LEN)];
extern const DRM_BYTE RIGHT_COLLABORATIVE_PLAY    [__CB_DECL(RIGHTS_LEN)];


extern const DRM_LID g_rgbSecStoreGlobalName;


/* Version Format: a.b.c.d */
/*            a = not used. */
/*            b = major version */
/*            c = minor version */
/*            d = revision version */

extern const DRM_CONST_STRING g_dstrDRM_VERSION_STRING;
extern const DRM_BYTE DRM_VERSION    [__CB_DECL(VERSION_LEN)];
extern const DRM_BYTE PK_VER         [__CB_DECL(VERSION_LEN)]; /* Indicates the version of the pubkey to be used for verification of PK CERT. */
extern const DRM_BYTE KEYFILE_VER    [__CB_DECL(VERSION_LEN)]; /* Version of the key file. */
extern const DRM_BYTE LICREQUEST_VER [__CB_DECL(VERSION_LEN)];
extern const DRM_BYTE CERT_VER       [__CB_DECL(VERSION_LEN)]; /* Indicates the public root key needed to verify the license server certificates. */
extern const DRM_BYTE LICENSE_VER    [__CB_DECL(VERSION_LEN)]; /* Indicates the license version delivered. */
extern const DRM_BYTE CLIENT_ID_VER  [__CB_DECL(VERSION_LEN)]; /* The version for client id. */
extern const DRM_BYTE CONTENT_VERSION[__CB_DECL(VERSION_LEN)]; /* Content Version. Keep this in sync with CONTENT_VERSION_STRING above. */
extern const DRM_BYTE PM_LICENSE_VER [__CB_DECL(VERSION_LEN)]; /* Indicates the license version delivered to PMs */


extern const PUBKEY g_pubkeyLicenseServer;

#if DRM_NO_FORCE_ALIGN
#if defined (_MSC_VER)
#pragma pack(push)
#pragma pack(1)
#endif /* #if defined (_MSC_VER) */
#endif /* #if DRM_NO_FORCE_ALIGN */

typedef struct CERTDATAtag{
    PUBKEY pk;
    DRM_BYTE expiryDate  [__CB_DECL(DATE_LEN)];
    DRM_BYTE serialNumber[__CB_DECL(SER_NUM_LEN)];
    DRM_BYTE issuer      [__CB_DECL(ISSUER_LEN)];
    DRM_BYTE subject     [__CB_DECL(SUBJ_LEN)];
} CERTDATA, *PCERTDATA;


typedef struct CERTtag{
    DRM_BYTE certVersion[__CB_DECL(VERSION_LEN)];
    DRM_BYTE datalen    [__CB_DECL(INT_LEN)];
    DRM_BYTE sign       [__CB_DECL(DRM_ECC160_SIGNATURE_LEN)];
    CERTDATA cd;
} CERT, *PCERT;

#if DRM_NO_FORCE_ALIGN
#if defined (_MSC_VER)
#pragma pack(pop)
#endif /* #if defined (_MSC_VER) */
#endif /* #if DRM_NO_FORCE_ALIGN */

typedef struct {
    PKV pk;
    DRM_BYTE appSec [__CB_DECL(APPSEC_LEN)];
    DRM_BYTE subject[__CB_DECL(SUBJ_LEN)];
} APPCERTDATA;

typedef struct {
    DRM_BYTE appcertVersion[__CB_DECL(VERSION_LEN)];
    DRM_BYTE datalen       [__CB_DECL(INT_LEN)];
    DRM_BYTE sign          [__CB_DECL(DRM_ECC160_SIGNATURE_LEN)];
    APPCERTDATA appcd;
} APPCERT;

typedef struct __tagDRM_CRYPTO_CONTEXT
{
    DRM_BYTE rgbCryptoContext[__CB_DECL(DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE)];   /* size of the buffer pkcrypto code needs. Note this may have alignment problems. */
    PUBKEY   pubKey;  /* 40 BYTES */
    PUBKEY   pubKey2; /* 40 BYTES */
    union  _tagcert
    {
        CERT     cert;    /* 108 bytes */
        PKCERT   pkcert;  /* 84 bytes  */
    } union_cert;
    DRM_BYTE signature[__CB_DECL(DRM_ECC160_SIGNATURE_LEN)];   /* 40 BYTES     */
} DRM_CRYPTO_CONTEXT;

typedef struct __tagCLIENTID
{
    DRM_BYTE version[__CB_DECL(VERSION_LEN)];
    DRM_BYTE randNum[__CB_DECL(DRM_ECC160_CIPHERTEXT_LEN)];
    PKCERT pk;
} CLIENTID;


typedef struct __tagDRM_V1Header 
{
    DRM_BYTE  *pbKeyID;
    DRM_DWORD  cbKeyID;
    DRM_BYTE  *pbSecretData;
    DRM_DWORD  cbSecretData;
    DRM_BYTE  *pbURL;
    DRM_DWORD  cbURL;
    DRM_BYTE   bBuffer[1];
} DRM_V1Header;

EXIT_PK_NAMESPACE;

#endif /* __DRMLICENSE_H__ */

/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMREVOCATIONTYPES_H__
#define __DRMREVOCATIONTYPES_H__


#ifdef __cplusplus
extern "C" {
#endif

typedef enum _DRM_REVOCATION_TYPE_ENUM
{
    DRM_REVOCATION_TYPE_WMDRM_APP = 0,
    DRM_REVOCATION_TYPE_WMDRM_DEVICE,
    DRM_REVOCATION_TYPE_WMDRM_ND,
    DRM_REVOCATION_TYPE_WMDRM_REVINFO,
    DRM_REVOCATION_TYPE_PLAYREADY_APP,
    DRM_REVOCATION_TYPE_PLAYREADY_RUNTIME,
    DRM_REVOCATION_TYPE_PLAYREADY_REVINFO2
} DRM_REVOCATION_TYPE_ENUM;

typedef struct __tagDRM_RLVI_RECORD
{
    DRM_GUID   listID;
    DRM_UINT64 qwVersion;
} DRM_RLVI_RECORD;

typedef struct __tagDRM_RLVI_HEAD
{
    DRM_DWORD   dwID;
    DRM_DWORD   cbSignedBytes;
    DRM_BYTE    bFormatVersion;
    DRM_BYTE    bReserved[3];
    DRM_DWORD   dwRIV;
    DRMFILETIME ftIssuedTime;
    DRM_DWORD   dwRecordCount;
} DRM_RLVI_HEAD;

typedef struct __tagDRM_RLVI_SIGNATURE
{
    DRM_BYTE  bSignatureType;
    DRM_DWORD ibSignature;
} DRM_RLVI_SIGNATURE;

typedef struct __tagDRM_RLVI_CERTCHAIN
{
    DRM_DWORD  cbCertChain;
    DRM_DWORD  ibCertChain;
} DRM_RLVI_CERTCHAIN;

typedef struct __tagDRM_RLVI
{
    DRM_RLVI_HEAD       head;
    DRM_DWORD           ibEntries;    /* Byte offset to the entries in the revocation info */
    DRM_RLVI_SIGNATURE  signature;
    DRM_RLVI_CERTCHAIN  certchain; 
} DRM_RLVI;

typedef struct
{    DRM_GUID guidRevocationList;
    DRM_BYTE *pbRevocationList;
    DRM_DWORD cbRevocationList;
} DRM_RVK_LIST;

#define RLVI_MAGIC_NUM_V1   ((DRM_DWORD) 0x524C5649) /* 'RLVI' */
#define RLVI_MAGIC_NUM_V2   ((DRM_DWORD) 0x524C5632) /* 'RLV2' */

#define RLVI_FORMAT_VERSION_V1 ((DRM_BYTE) 1)
#define RLVI_FORMAT_VERSION_V2 ((DRM_BYTE) 2)
#define RLVI_SIGNATURE_TYPE_1 ((DRM_BYTE) 1)
#define RLVI_SIGNATURE_SIZE_1 ((DRM_DWORD) 128)
#define RLVI_SIGNATURE_TYPE_2 ((DRM_BYTE) 2)
#define RLVI_SIGNATURE_SIZE_2 ((DRM_DWORD) 256)

#define DRM_NO_PREVIOUS_CRL 0xFFFFFFFF


#if DRM_SUPPORT_APP_REVOCATION

typedef DRM_DWORD DRM_CERT_TYPE;
#define CERTTYPE_V1APP 0

typedef struct __tagDRM_REVOCATION_LIST_ELEMENT
{
    DRM_CERT_TYPE certificatetype;
    DRM_DWORD     cbCert;
    DRM_BYTE      rgbCert [1];
}DRM_REVOCATION_LIST_ELEMENT;

typedef struct __tagDRM_REVOCATION_LIST
{
    DRM_BYTE  rgbSignature [__CB_DECL( DRM_ECC160_SIGNATURE_LEN) ];
    DRM_DWORD dwVersion;
    DRM_DWORD cb;
    DRM_DWORD idSerial;
    DRM_BYTE  rgbData [1]; /* An array of DRM_REVOCATION_LIST_ELEMENT structs which are variable length */
} DRM_REVOCATION_LIST;

#endif /*DRM_SUPPORT_APP_REVOCATION*/

/*
** CRL defines
*/
#define WMDRMNET_CRL_ENTRY_SIZE                     DRM_SHA1_DIGEST_LEN
#define WMDRMNET_CRL_SIGNATURE_TYPE_RSA_SHA1        2
#define WMDRMNET_CRL_SIGNATURE_LEN_RSA_SHA1         128
    
typedef struct
{
    DRM_BYTE val[ __CB_DECL( WMDRMNET_CRL_ENTRY_SIZE ) ];
    
} WMDRMNET_CRL_ENTRY;


#ifdef __cplusplus
}
#endif

#endif /* __DRMREVOCATIONTYPES_H__ */

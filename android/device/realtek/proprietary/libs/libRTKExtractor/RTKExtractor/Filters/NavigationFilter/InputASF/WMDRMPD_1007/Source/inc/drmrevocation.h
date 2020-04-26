/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMREVOCATION_H__
#define __DRMREVOCATION_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _DRM_REVOCATION_TYPE_ENUM
{
    WM_DRM_REVOCATION_TYPE_APP = 0,
    WM_DRM_REVOCATION_TYPE_WMDRMPD,
    WM_DRM_REVOCATION_TYPE_WMDRMND,
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
    DRM_DWORD           ibEntries;    
    DRM_RLVI_SIGNATURE  signature;
    DRM_RLVI_CERTCHAIN  certchain; 
} DRM_RLVI;

typedef struct
{	DRM_GUID guidRevocationList;
	DRM_BYTE *pbRevocationList;
	DRM_DWORD cbRevocationList;
} DRM_RVK_LIST;

#define RLVI_MAGIC_NUM      ((DRM_DWORD) 0x524C5649) /* 'RLVI' */
#define RLVI_FORMAT_VERSION ((DRM_BYTE) 1)
#define RLVI_SIGNATURE_TYPE ((DRM_BYTE) 1)
#define RLVI_SIGNATURE_SIZE ((DRM_DWORD) 128)

#define DRM_NO_PREVIOUS_CRL 0xFFFFFFFF

#if DRM_SUPPORT_REVOCATION

#define DRM_ALL_REVOCATIONS NULL
DRM_RESULT DRM_API DRM_RVK_GetCurrentRevocationInfo(
    IN     DRM_SECSTORE_CONTEXT *f_pContextSST,
    IN     DRM_BB_CONTEXT       *f_pContextBB,
    IN     DRM_HDS_CONTEXT      *f_pContextHDS,
       OUT DRM_BYTE             *f_pbBuffer,
    IN OUT DRM_DWORD            *f_pcbBuffer,
       OUT DRM_RLVI             *f_pRLVI);

DRM_RESULT DRM_API DRM_RVK_ProcessRevocationInfo (
    IN DRM_LICEVAL_CONTEXT   *f_pContextLEVL,
    IN DRM_HDS_CONTEXT       *f_pContextHDS,
    IN DRM_SECSTORE_CONTEXT  *f_pContextSST,
    IN DRM_BB_CONTEXT        *f_pContextBB,
    IN DRM_ANSI_CONST_STRING *f_pdastrLicenseXML);


DRM_RESULT DRM_API DRM_RVK_UpdateRevocationVersionsCache( 
 IN OUT DRM_MANAGER_CONTEXT_INTERNAL  *f_pContext,
    OUT DRM_BOOL                     *f_pfUpdated);

DRM_RESULT DRM_API DRM_RVK_StoreRevInfo (
    IN DRM_LICEVAL_CONTEXT   *f_pContextLEVL,
    IN DRM_HDS_CONTEXT       *f_pContextHDS,
    IN DRM_SECSTORE_CONTEXT  *f_pContextSST,
    IN DRM_BB_CONTEXT        *f_pContextBB,
    IN DRM_BYTE              *f_pbPassword,
    IN DRM_BYTE              *f_pbRevocationInfo,
    IN DRM_DWORD              f_cbRevocationInfo,
    IN DRM_BOOL              *f_fWasStored );

DRM_RESULT DRM_RVK_StoreRevocationLists(
    IN DRM_LICEVAL_CONTEXT   *f_pContextLEVL,
    IN DRM_HDS_CONTEXT       *f_pContextHDS,
    IN DRM_SECSTORE_CONTEXT  *f_pContextSST,
    IN DRM_BB_CONTEXT        *f_pContextBB,
    IN DRM_DWORD              f_cRevocationLists,
    IN DRM_RVK_LIST          *f_pRevocationLists );

#if DRM_SUPPORT_APP_REVOCATION

#define DRM_APP_REVOCATION_VERSION_NONE    0xFFFFFFFF

DRM_RESULT DRM_API DRM_RVK_GetCurrentAppRevocationList(
    IN      DRM_SECSTORE_CONTEXT *f_pcontextSST,
    IN      DRM_BYTE              f_rgbPasswordSST [__CB_DECL(SHA_DIGEST_LEN)],
    IN      DRM_BYTE             *f_pbBuffer,
    IN  OUT DRM_DWORD            *f_pcbBuffer,
    IN      DRM_HDS_CONTEXT      *f_pcontextHDS,
    OUT     DRM_DWORD            *f_pidCRLApp);

DRM_RESULT DRM_API DRM_RVK_PerformAppRevocationCheck(
    IN  DRM_LICEVAL_CONTEXT  *f_pcontextLEVL,
    IN  DRM_HDS_CONTEXT      *f_pcontextHDS);

DRM_RESULT DRM_API DRM_RVK_CheckAppCertForRevocation(
    IN  DRM_BYTE             *f_pbBuffer,
    IN  DRM_DWORD             f_cbBuffer,
    IN  APPCERT              *f_pappcert);

DRM_RESULT DRM_API DRM_RVK_UpdateAppRevocationListA(
    IN  DRM_CRYPTO_CONTEXT   *f_pcontextCRY,
    IN  DRM_SECSTORE_CONTEXT *f_pcontextSST,
    IN  DRM_BYTE              f_rgbPassword [__CB_DECL(SHA_DIGEST_LEN)],
    IN  DRM_BYTE             *f_pbBuffer,
    IN  DRM_DWORD             f_cbBuffer,
    IN  DRM_CHAR             *f_rgchBase,
    IN  const DRM_SUBSTRING  *f_pdasstrRevocationList,
    IN  DRM_HDS_CONTEXT      *f_pcontextHDS,
    IN  APPCERT              *f_pappcert,
    IN  DRM_DWORD             f_idCRLAppCurrent,
    OUT DRM_BOOL             *f_pfUpdatedAppRevocationList);

DRM_RESULT DRM_API DRM_RVK_UpdateAppRevocationListW(
    DRM_CRYPTO_CONTEXT   *f_pcontextCRY,
    DRM_SECSTORE_CONTEXT *f_pcontextSST,
    DRM_BYTE              f_rgbPassword [__CB_DECL(SHA_DIGEST_LEN)],
    DRM_BYTE             *f_pbBuffer,
    DRM_DWORD             f_cbBuffer,
    DRM_CONST_STRING     *f_pdstrRevocationList,
    DRM_HDS_CONTEXT      *f_pcontextHDS,
    APPCERT              *f_pappcert,
    DRM_DWORD             f_idCRLAppCurrent,
    DRM_BOOL             *f_pfUpdatedAppRevocationList);

#endif

#if DRM_SUPPORT_DEVICE_REVOCATION

DRM_RESULT DRM_RVK_UpdateRevocationList(
    IN          DRM_CRYPTO_CONTEXT   *f_pcontextCrypto,
    IN          DRM_SECSTORE_CONTEXT *f_pcontextSST,
    IN          DRM_BYTE              f_rgbPassword[__CB_DECL(SHA_DIGEST_LEN)],
    IN          DRM_CHAR             *f_rgchBase,
    IN  const   DRM_SUBSTRING        *f_pdasstrRevocationList,
    IN          DRM_BYTE             *f_pbRevocationBuffer,
    IN          DRM_DWORD             f_cbRevocationBuffer,
    IN  const   DRM_ID               *f_pID,
    IN  const   PUBKEY               *f_ppubkey,
    IN          DRM_HDS_CONTEXT      *f_pcontextHDS );

DRM_RESULT DRM_API DRM_RVK_GetDeviceRevocationList(
    IN     DRM_CRYPTO_CONTEXT   *f_pcontextCRYP, 
    IN     DRM_SECSTORE_CONTEXT *f_pcontextSST,
    IN     DRM_HDS_CONTEXT      *f_pcontextHDS,
    IN     DRM_BYTE              f_rgbPasswordSST [__CB_DECL(SHA_DIGEST_LEN)],
    OUT    DRM_BYTE             *f_pbBuffer,
    IN OUT DRM_DWORD            *f_pcbBuffer,
    OUT    DRM_DWORD            *f_pidSerial);

DRM_RESULT DRM_API DRM_RVK_CheckDeviceRevocation(
    IN     DRM_CONST_STRING *f_pdstrdevcert,
    IN     DRM_CONST_STRING *f_pdstrXMLExclusion, 
       OUT DRM_BOOL         *f_pfExcluded);

#endif /* DRM_SUPPORT_DEVICE_REVOCATION */

/*
** CRL defines
*/
#define WMDRMNET_CRL_ENTRY_SIZE                     SHA_DIGEST_LEN
#define WMDRMNET_CRL_SIGNATURE_TYPE_RSA_SHA1        2
#define WMDRMNET_CRL_SIGNATURE_LEN_RSA_SHA1         128
#define WMDRMNET_CRL_SIGNATURE_LEN_RSA_SHA1         128
    
typedef struct
{
    DRM_BYTE val[ __CB_DECL( WMDRMNET_CRL_ENTRY_SIZE ) ];
    
} WMDRMNET_CRL_ENTRY;

#if DRM_SUPPORT_WMDRMNET
DRM_RESULT DRM_RVK_UpdateWMDRMNETRevocationList(
    IN          DRM_CRYPTO_CONTEXT   *f_pcontextCrypto,
    IN          DRM_SECSTORE_CONTEXT *f_pcontextSST,
    IN          DRM_BYTE              f_rgbPassword[__CB_DECL(SHA_DIGEST_LEN)],
    IN          DRM_CHAR             *f_rgchBase,
    IN  const   DRM_SUBSTRING        *f_pdasstrRevocationList,
    IN          DRM_BYTE             *f_pbRevocationBuffer,
    IN          DRM_DWORD             f_cbRevocationBuffer,
    IN  const   DRM_ID               *f_pID,
    IN  const   PUBKEY               *f_ppubkey,
    IN          DRM_HDS_CONTEXT      *f_pcontextHDS );

DRM_RESULT DRM_API DRM_RVK_GetWMDRMNETList(
    IN     DRM_CRYPTO_CONTEXT   *f_pcontextCRYP, 
    IN     DRM_SECSTORE_CONTEXT *f_pcontextSST,
    IN     DRM_HDS_CONTEXT      *f_pcontextHDS,
    IN     DRM_BYTE              f_rgbPasswordSST [__CB_DECL(SHA_DIGEST_LEN)],
    OUT    DRM_BYTE             *f_pbBuffer,
    IN OUT DRM_DWORD            *f_pcbBuffer,
    OUT    DRM_DWORD            *f_pidSerial);

#endif /* DRM_SUPPORT_WMDRMNET */


DRM_RESULT DRM_API _CreateRevocationStorePassword(
    IN  DRM_BB_CONTEXT *f_pcontextBBX,
    OUT DRM_BYTE        f_rgbPassword [__CB_DECL(SHA_DIGEST_LEN)]);


#endif /* DRM_SUPPORT_REVOCATION */

#ifdef __cplusplus
}
#endif

#endif /* __DRMREVOCATION_H__ */

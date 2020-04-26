/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMREVOCATION_H__
#define __DRMREVOCATION_H__

#include <drmcontextsizes.h>
#include <drmliceval.h>
#include <drmrevocationstore.h>

ENTER_PK_NAMESPACE;
#include <drmrevocationtypes.h>

#if DRM_SUPPORT_REVOCATION

#define DRM_ALL_REVOCATIONS NULL
DRM_API DRM_RESULT DRM_CALL DRM_RVK_GetCurrentRevocationInfo(
    IN     DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN     const DRM_GUID              *f_pguidRevocationType,
       OUT DRM_BYTE                    *f_pbBuffer,
    IN OUT DRM_DWORD                   *f_pcbBuffer,
       OUT DRM_RLVI                    *f_pRLVI,
    IN     DRM_CRYPTO_CONTEXT          *f_pCryptoCtx );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_ProcessRevocationInfo (
    IN     DRM_CRYPTO_CONTEXT          *f_pContextCrypto,
    IN     DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN     DRM_ANSI_CONST_STRING       *f_pdastrLicenseResponse,
    IN     DRM_BYTE                    *f_pbRevocationBuffer,
    IN     DRM_DWORD                    f_cbRevocationBuffer,
    IN     APPCERT                     *f_pAppcert,
    IN OUT DRM_REVLIST_ID_VALUES       *f_pRevList );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_UpdateRevocationVersionsCache(
 IN OUT DRM_REVOCATIONSTORE_CONTEXT  *f_pContextRev,
 IN     DRM_CRYPTO_CONTEXT           *f_pContextCrypto,
 IN OUT DRM_REVLIST_ID_VALUES        *f_pRevListIDs,
 IN     DRM_BYTE                     *f_pbRevocationBuffer,
 IN     DRM_DWORD                     f_cbRevocationBuffer,
    OUT DRM_BOOL                     *f_pfUpdated);

DRM_API DRM_RESULT DRM_CALL DRM_RVK_StoreRevInfo (
    IN     DRM_CRYPTO_CONTEXT          *f_pContextCrypto,
    IN     DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN     const DRM_GUID              *f_pguidRevocationType,
    IN     DRM_BYTE                    *f_pbRevocationInfo,
    IN     DRM_DWORD                    f_cbRevocationInfo,
    IN     DRM_BYTE                    *f_pbRevocationBuffer,
    IN     DRM_DWORD                    f_cbRevocationBuffer,
    IN OUT DRM_REVLIST_ID_VALUES       *f_pRevList,
       OUT DRM_BOOL                    *f_fWasStored );

DRM_RESULT DRM_RVK_StoreRevocationLists(
    IN     DRM_CRYPTO_CONTEXT    *f_pContextCrypto,
    IN     DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN     DRM_DWORD              f_cRevocationLists,
    IN     DRM_RVK_LIST          *f_pRevocationLists,
    IN     APPCERT               *f_pAppcert,
    IN     DRM_BYTE              *f_pbRevocationBuffer,
    IN     DRM_DWORD              f_cbRevocationBuffer,
    IN OUT DRM_REVLIST_ID_VALUES *f_pRevList );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_FindEntryInRevInfo(
    __in                        DRM_RLVI    *f_prlvi,
    __in_bcount(f_cbRevInfo)    DRM_BYTE    *f_pbRevInfo,
    __in                        DRM_DWORD    f_cbRevInfo,
    __in                  const DRM_GUID    *f_pguidEntry,
    __out                       DRM_DWORD   *f_pdwVersion );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_VerifyRevocationInfo(
    __in_bcount( f_cbRevInfo ) DRM_BYTE           *f_pbRevInfo,
    __in                       DRM_DWORD           f_cbRevInfo,
    __out                      DRM_RLVI           *f_pRLVI,
    __in                       DRM_CRYPTO_CONTEXT *f_pCryptoCtx,
    __in_opt                   DRM_BB_CONTEXT     *f_pcontextBBX,
    __in_opt                   DRM_SECSTORE_CONTEXT *f_pcontextSST,
    __in_opt                   DRM_DST            *f_pDatastore );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_VerifyRevocationList(
    IN          DRM_CRYPTO_CONTEXT *f_pcontextCRYP,
    IN          DRM_CONST_STRING   *f_pdstrList,
    IN  const   PUBKEY             *f_ppubkey,
        OUT     DRM_DWORD          *f_pidSerial);

DRM_API DRM_RESULT DRM_CALL DRM_RVK_ProcessRevInfo(
    IN          DRM_CHAR                    *f_pszResponse,
    IN          DRM_SUBSTRING               *f_pdasstrRevType,
    IN          DRM_SUBSTRING               *f_pdasstrRevData,
    IN          DRM_CRYPTO_CONTEXT          *f_pcontextCRYP,
    IN          DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN          DRM_BYTE                    *f_pbRevocationBuffer,
    IN          DRM_DWORD                    f_cbRevocationBuffer,
    IN          APPCERT                     *f_pAppCert,
    IN          DRM_DWORD                    f_idCRLAppCurrent,
       OUT      DRM_BOOL                    *f_pfUpdatedAppRevocationList );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_VerifyBinaryWMDRMNETSignature(
    IN     DRM_BYTE  *f_pbData,
    IN     DRM_DWORD  f_cbData,
    IN OUT DRM_DWORD *f_pidSerial);

DRM_API DRM_RESULT DRM_CALL DRM_RVK_StoreMsdbCRLPackage
(
    __inout                             DRM_REVOCATIONSTORE_CONTEXT *f_poRevContext,
    __inout_opt                         DRM_VOID                    *f_pOEMContext,
    __in_bcount(f_cbCRLData)      const DRM_BYTE                    *f_pbCRLData,
    __in                          const DRM_DWORD                    f_cbCRLData,
    __in_bcount( f_cbRevocationBuffer ) DRM_BYTE                    *f_pbRevocationBuffer,
    __in                          const DRM_DWORD                    f_cbRevocationBuffer
);


#define DRM_APP_REVOCATION_VERSION_NONE    0xFFFFFFFF

#if DRM_SUPPORT_APP_REVOCATION


DRM_API DRM_RESULT DRM_CALL DRM_RVK_GetCurrentAppRevocationList(
    IN      DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_bcount( *f_pcbBuffer ) DRM_BYTE *f_pbBuffer,
    IN  OUT DRM_DWORD            *f_pcbBuffer,
    OUT     DRM_DWORD            *f_pidCRLApp);

DRM_API DRM_RESULT DRM_CALL DRM_RVK_PerformAppRevocationCheck(
    IN     DRM_CRYPTO_CONTEXT          *f_pContextCrypto,
    IN     DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN     DRM_CONST_STRING            *f_pdstrContentLicense,
    IN     APPCERT                     *f_pAppCert,
    __in_bcount( f_cbRevocationBuffer ) DRM_BYTE *f_pbRevocationBuffer,
    IN     DRM_DWORD                    f_cbRevocationBuffer,
    IN OUT DRM_REVLIST_ID_VALUES       *f_pRevListIDs );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_CheckAppCertForRevocation(
    __in_bcount( f_cbBuffer ) DRM_BYTE *f_pbBuffer,
    IN  DRM_DWORD             f_cbBuffer,
    IN  APPCERT              *f_pappcert);

DRM_API DRM_RESULT DRM_CALL DRM_RVK_UpdateAppRevocationListA(
        DRM_CRYPTO_CONTEXT   *f_pcontextCRY,
        DRM_REVOCATIONSTORE_CONTEXT * f_pContextRev,
        __in_bcount( f_cbBuffer ) DRM_BYTE *f_pbBuffer,
        DRM_DWORD             f_cbBuffer,
        __in_ecount( f_pdasstrRevocationList->m_ich + f_pdasstrRevocationList->m_cch ) DRM_CHAR *f_rgchBase,
  const DRM_SUBSTRING        *f_pdasstrRevocationList,
        APPCERT              *f_pappcert,
        DRM_DWORD             f_idCRLAppCurrent,
        DRM_BOOL             *f_pfUpdatedAppRevocationList);

DRM_API DRM_RESULT DRM_CALL DRM_RVK_UpdateAppRevocationListW(
    DRM_CRYPTO_CONTEXT   *f_pcontextCRY,
    DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_bcount( f_cbBuffer ) DRM_BYTE *f_pbBuffer,
    DRM_DWORD             f_cbBuffer,
    DRM_CONST_STRING     *f_pdstrRevocationList,
    APPCERT              *f_pappcert,
    DRM_DWORD             f_idCRLAppCurrent,
    DRM_BOOL             *f_pfUpdatedAppRevocationList,
    OUT DRM_DWORD        *f_pdwVersion);

DRM_RESULT DRM_CALL DRM_RVK_UpdateAppRevocationListDecoded(
    DRM_CRYPTO_CONTEXT   *f_pcontextCRY,
    DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_bcount( f_cbRevocationList ) DRM_BYTE *f_pbRevocationList,
    DRM_DWORD             f_cbRevocationList,
    APPCERT              *f_pappcert,
    DRM_DWORD             f_idCRLAppCurrent,
    DRM_BOOL             *f_pfUpdatedAppRevocationList,
    OUT DRM_DWORD        *f_pdwVersion);

#endif

#if DRM_SUPPORT_DEVICE_REVOCATION

DRM_RESULT DRM_RVK_UpdateRevocationList(
    IN          DRM_CRYPTO_CONTEXT   *f_pcontextCrypto,
    IN          DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_ecount( f_pdasstrRevocationList->m_ich + f_pdasstrRevocationList->m_cch ) DRM_CHAR *f_rgchBase,
    IN  const   DRM_SUBSTRING        *f_pdasstrRevocationList,
    __in_bcount( f_cbRevocationBuffer ) DRM_BYTE *f_pbRevocationBuffer,
    IN          DRM_DWORD             f_cbRevocationBuffer,
    IN  const   DRM_GUID             *f_pGUID,
    IN  const   PUBKEY               *f_ppubkey );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_GetDeviceRevocationList(
    IN     DRM_CRYPTO_CONTEXT   *f_pcontextCRYP,
    IN     DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __out_bcount_opt( *f_pcbRevocationData ) DRM_BYTE *f_pbRevocationData,
    IN OUT DRM_DWORD            *f_pcbRevocationData,
    OUT    DRM_DWORD            *f_pidSerial);

DRM_API DRM_RESULT DRM_CALL DRM_RVK_CheckDeviceRevocation(
    __in  const DRM_CONST_STRING *f_pdstrDevCert,
    __in  const DRM_CONST_STRING *f_pdstrXMLExclusion,
    __out       DRM_BOOL         *f_pfExcluded );

DRM_RESULT DRM_CALL DRM_RVK_GetCRL(
    IN     DRM_CRYPTO_CONTEXT           *f_pContextCrypto,
    IN     DRM_REVOCATIONSTORE_CONTEXT  *f_pContextRev,
    IN     const DRM_GUID               *f_pguidRevocationType,
    __out_bcount_opt( *f_pcbCRL ) DRM_BYTE *f_pbCRL,
    IN OUT DRM_DWORD                    *f_pcbCRL,
       OUT DRM_DWORD                    *f_pdwCRLVersion);

#endif /* DRM_SUPPORT_DEVICE_REVOCATION */

DRM_API DRM_RESULT DRM_CALL DRM_RVK_SetCRL(
    IN     DRM_CRYPTO_CONTEXT           *f_pContextCrypto,
    IN     DRM_REVOCATIONSTORE_CONTEXT  *f_pContextRev,
    IN     const DRM_GUID               *f_pguidRevocationType,
    __in_bcount( f_cbCRL ) DRM_BYTE  *f_pbCRL,
    IN     DRM_DWORD                     f_cbCRL,
    __in_bcount( f_cbRevocationBuffer ) DRM_BYTE *f_pbRevocationBuffer,
    IN     DRM_DWORD                     f_cbRevocationBuffer,
    IN OUT DRM_BOOL                     *f_pfUpdated);

#if DRM_SUPPORT_WMDRMNET

DRM_RESULT DRM_CALL DRM_RVK_GetWMDRMNETRevocationEntries (
    __in_bcount( f_cbRevocationData ) DRM_BYTE *f_pbRevocationData,
    IN     DRM_DWORD            f_cbRevocationData,
       OUT DRM_DWORD           *f_pcEntries,
    __deref_out_ecount( *f_pcEntries ) WMDRMNET_CRL_ENTRY **f_ppEntries);

DRM_RESULT DRM_RVK_UpdateWMDRMNETRevocationList(
    IN          DRM_CRYPTO_CONTEXT   *f_pcontextCrypto,
    IN          DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_ecount( f_pdasstrRevocationList->m_ich + f_pdasstrRevocationList->m_cch ) DRM_CHAR *f_rgchBase,
    IN  const   DRM_SUBSTRING        *f_pdasstrRevocationList,
    __in_bcount( f_cbRevocationBuffer ) DRM_BYTE *f_pbRevocationBuffer,
    IN          DRM_DWORD             f_cbRevocationBuffer,
    IN  const   PUBKEY               *f_ppubkey );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_GetWMDRMNETList(
    IN      DRM_CRYPTO_CONTEXT   *f_pcontextCRYP,
    IN      DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __out_bcount_opt( *f_pcbRevocationData ) DRM_BYTE *f_pbRevocationData,
    IN  OUT DRM_DWORD            *f_pcbRevocationData,
        OUT DRM_DWORD            *f_pidSerial);

#endif /* DRM_SUPPORT_WMDRMNET */

#endif /* DRM_SUPPORT_REVOCATION */

EXIT_PK_NAMESPACE;

#endif /* __DRMREVOCATION_H__ */

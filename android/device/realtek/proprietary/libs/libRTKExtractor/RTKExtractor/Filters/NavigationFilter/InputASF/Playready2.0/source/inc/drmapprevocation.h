/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMAPPREVOCATION_H__
#define __DRMAPPREVOCATION_H__

#include <drmcommon.h>
#include <drmliceval.h>
#include <drmrevocationtypes.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_APPREVOCATION_IsAppRevocationSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_APPREVOCATION_IsAppRevocationUnsupported(DRM_VOID);

DRM_API DRM_RESULT DRM_CALL DRM_RVK_GetCurrentAppRevocationList(
    IN      DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_bcount( *f_pcbBuffer ) DRM_BYTE *f_pbBuffer,
    IN  OUT DRM_DWORD            *f_pcbBuffer,
    OUT     DRM_DWORD            *f_pidCRLApp) DRM_NO_INLINE_ATTRIBUTE;

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
    OUT DRM_DWORD        *f_pdwVersion) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_NO_INLINE DRM_CALL DRM_RVK_UpdateAppRevocationListDecoded(
    DRM_CRYPTO_CONTEXT   *f_pcontextCRY,
    DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_bcount( f_cbRevocationList ) DRM_BYTE *f_pbRevocationList,
    DRM_DWORD             f_cbRevocationList,
    APPCERT              *f_pappcert,
    DRM_DWORD             f_idCRLAppCurrent,
    DRM_BOOL             *f_pfUpdatedAppRevocationList,
    OUT DRM_DWORD        *f_pdwVersion) DRM_NO_INLINE_ATTRIBUTE;

EXIT_PK_NAMESPACE;

#endif /*__DRMAPPREVOCATION_H__ */


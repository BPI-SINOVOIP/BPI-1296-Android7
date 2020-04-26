/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMDEVICEREVOCATION_H__
#define __DRMDEVICEREVOCATION_H__

#include <drmcommon.h>
#include <oemcryptoctx.h>
#include <drmrevocationstore.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_DEVICEREVOCATION_IsDeviceRevocationSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_DEVICEREVOCATION_IsDeviceRevocationUnsupported(DRM_VOID);

/******************************************************************************
**
** Function :   DRM_RVK_UpdateRevocationListDecoded
**
** Synopsis :   updates the revocation list in the secure store
**
** Arguments :
** [f_pcontextCrypto]      --
** [f_pcontextRev]         -- revocation store context
** [f_pbRevocationList]    -- input buffer for exclusion list
** [f_cbRevocationList]    -- buffer size
** [f_pdstrRevocationList] -- the new DCRL
** [f_pGUID]               -- DRM_GUID used to store this list
** [f_pfUpdated]           -- Optional flag indicates whether the CRL was
**                            updated in the HDS or not.
** Returns :
**
** Notes :
**
******************************************************************************/
DRM_NO_INLINE DRM_RESULT DRM_RVK_UpdateRevocationListDecoded(
    IN          DRM_CRYPTO_CONTEXT   *f_pcontextCrypto,
    IN          DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN          DRM_BYTE             *f_pbRevocationList,
    IN          DRM_DWORD             f_cbRevocationList,
    IN  const   DRM_BYTE             *f_pbRevocationBuffer,
    IN          DRM_DWORD             f_cbRevocationBuffer,
    IN  const   DRM_GUID             *f_pGUID,
    IN  const   PUBKEY               *f_ppubkey,
    OUT         DRM_BOOL             *f_pfUpdated) DRM_NO_INLINE_ATTRIBUTE;

DRM_RESULT DRM_RVK_UpdateRevocationList(
    IN                                                                                   DRM_CRYPTO_CONTEXT          *f_pcontextCrypto,
    IN                                                                                   DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __in_ecount( f_pdasstrRevocationList->m_ich + f_pdasstrRevocationList->m_cch )       DRM_CHAR                    *f_rgchBase,
    IN                                                                             const DRM_SUBSTRING               *f_pdasstrRevocationList,
    __in_bcount( f_cbRevocationBuffer )                                            const DRM_BYTE                    *f_pbRevocationBuffer,
    IN                                                                                   DRM_DWORD                    f_cbRevocationBuffer,
    IN                                                                             const DRM_GUID                    *f_pGUID,
    IN                                                                             const PUBKEY                      *f_ppubkey );

DRM_API DRM_RESULT DRM_CALL DRM_RVK_GetDeviceRevocationList(
    IN                                             DRM_CRYPTO_CONTEXT          *f_pcontextCRYP,
    IN                                             DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    __out_bcount_opt( *f_pcbRevocationData ) const DRM_BYTE                    *f_pbRevocationData,
    IN OUT                                         DRM_DWORD                   *f_pcbRevocationData,
    OUT                                            DRM_DWORD                   *f_pidSerial ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL DRM_RVK_CheckDeviceRevocation(
    __in  const DRM_CONST_STRING *f_pdstrDevCert,
    __in  const DRM_CONST_STRING *f_pdstrXMLExclusion,
    __out       DRM_BOOL         *f_pfExcluded );

DRM_API DRM_RESULT DRM_CALL DRM_DEVICEREVOCATION_IsDeviceRevoked(
    __in                      const DRM_CONST_STRING *f_pdstrDeviceCert,
    __in_bcount(f_ccbCRLSize) const DRM_BYTE         *f_pbCRLData,
    __in                      const DRM_DWORD         f_ccbCRLSize );

EXIT_PK_NAMESPACE;

#endif /*__DRMDEVICEREVOCATION_H__ */


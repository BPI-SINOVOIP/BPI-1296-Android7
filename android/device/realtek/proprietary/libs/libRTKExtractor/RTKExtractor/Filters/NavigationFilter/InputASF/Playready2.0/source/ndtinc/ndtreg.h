/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTREG_H__
#define __NDTREG_H__

#include <drmndtmanager.h>

ENTER_PK_NAMESPACE;

DRM_RESULT NDT_ValidateCertificate(
    __in        NDTMGR_CONTEXT_INTERNAL                        *f_pNdtMgrContext,
    __in_ecount(f_cbDeviceCertificate) const DRM_CHAR   * const f_rgchDeviceCertificate,
    __in const  DRM_DWORD                                       f_cbDeviceCertificate,
    __inout     DRM_SUBSTRING                                  *f_pdasstrWMDRMNDCertificate,
    __out       NDT_CERTIFICATE_INFO                    * const f_pCertificateInfo,
    __deref_opt_out_bcount(*f_pcbCRL) DRM_BYTE                **f_ppbCRL,
    __out_opt   DRM_DWORD                               * const f_pcbCRL,
    __out       DRM_DWORD                               * const f_pdwCRLVersion
);

DRM_RESULT NDT_EncryptSeed( 
    __in                        const DRM_RSA_PUBLIC_KEY  * const f_pDevicePublicKey,
    __in                        const DRM_ID              * const f_pSeed,
    __in                        const DRM_DWORD                   f_wBufferSize,
    __out_bcount(f_wBufferSize) const DRM_BYTE            * const f_pbEncryptedSeed,
    __out                             DRM_DWORD           * const f_pdwBytesWritten );

DRM_RESULT NDT_SignRegistrationMessage(
    __in_bcount(f_cbMessage) const  DRM_BYTE    * const f_pbMessage,
    __in const                      DRM_DWORD           f_cbMessage,
    __in const                      DRM_ID      * const f_pSeed,
    __in const                      DRM_DWORD           f_cbSignature,
    __inout_bcount(f_cbSignature)   DRM_BYTE    * const f_pbSignature,
    __out                           DRM_DWORD   * const f_pdwBytesWritten
);

DRM_API DRM_AES_KEY * DRM_CALL WmdrmNetGetRegistrationKey(
    __in const DRM_ID    * const f_pSeed,
    __in const DRM_BOOL          f_bComputeContentIntegrityKey
);

EXIT_PK_NAMESPACE;

#endif /* __NDTREG_H__ */


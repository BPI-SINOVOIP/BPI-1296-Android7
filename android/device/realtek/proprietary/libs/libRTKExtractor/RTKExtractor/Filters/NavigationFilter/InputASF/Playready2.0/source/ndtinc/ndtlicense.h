/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTLICENSE_H__
#define __NDTLICENSE_H__

ENTER_PK_NAMESPACE;


#define DRM_XMR_CHAINED_CHECKSUM_LENGTH                 16
#define DRM_XMR_CHAINED_CHECKSUM_SHA_ITERATIONS         5


/* DRM status */
#define NDT_DRM_STATUS_UNPROTECTED 0
#define NDT_DRM_STATUS_PROTECTED   1

/* License status */
#define NDT_DRM_LICENSE_NOT_AVAILABLE   0
#define NDT_DRM_LICENSE_ALLOWS_PLAYBACK 1


DRM_VOID DRM_CALL DRM_NDT_License_FreePolicy(
    __out WMDRMNET_POLICY *f_pPolicy
);

DRM_RESULT DRM_CALL DRM_NDT_License_CreateDefaultPolicy(
    __in  NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT  * const f_pContSessContext, 
    __deref_out_bcount(*f_pcbPolicy)  WMDRMNET_POLICY   **f_ppPolicy, 
    __out DRM_DWORD                               * const f_pcbPolicy
);

DRM_RESULT DRM_CALL DRM_NDT_License_SetRootPolicy( 
    __in  const NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT  * const pContentSession,
    __out       WMDRMNET_POLICY                         * const pPolicy
);

DRM_RESULT DRM_CALL DRM_NDT_License_GenerateKeys(
    __in_opt  DRM_VOID                               *f_pOEMContext,
    __in      NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT *f_pNdtContentSessionContext,
    __inout   NDT_LICENSE_CONTEXT                    *f_pLicContext
);

DRM_RESULT DRM_CALL NDT_RetrieveLicenseDataV1(
    __in NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT        * const f_pNdtContentSessionContext,
    __in_bcount(f_cbKeyID)      const DRM_BYTE         * const f_pbKeyID,
    __in                        const DRM_DWORD                f_cbKeyID,
    __in_bcount(f_cbSecretData) const DRM_BYTE         * const f_pbSecretData,
    __in                        const DRM_DWORD                f_cbSecretData,
    __in_bcount(f_cbURL)        const DRM_BYTE         * const f_pbURL,
    __in                        const DRM_DWORD                f_cbURL,
    __in                        const DRM_CONST_STRING * const f_pdstrAction
);

DRM_API DRM_RESULT DRM_CALL NDT_RetrieveLicenseDataV2(
    __in       NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT        * const f_pNdtContentSessionContext,
    __in_bcount(f_cbContentEncryptionObject) const DRM_BYTE  * const f_pbContentEncryptionObject,
    __in const DRM_DWORD                                             f_cbContentEncryptionObject,
    __in const DRM_CONST_STRING                              * const f_pdstrAction
);

DRM_RESULT DRM_CALL NDT_RetrieveLicenseDataPR(
    __in       NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT * const f_pNdtContentSessionContext,
    __in_bcount(f_cbPlayReadyObject) const DRM_BYTE   * const f_pbPlayReadyObject,
    __in const DRM_DWORD                                      f_cbPlayReadyObject,
    __in const DRM_CONST_STRING                       * const f_pdstrAction
);

DRM_RESULT DRM_CALL NDT_License_BuildRootXMR(
    __in const  NDTMGR_CONTEXT                          * const f_pNdtMgrContext,
    __in const  NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT  * const f_pNdtContentSessionContext,
    __in const  WMDRMNET_POLICY                         * const f_pPolicy,
    __in const  DRM_ID                                          f_idSerial,
    __in const  NDT_CERTIFICATE_INFO                    * const f_pCertificateInfo,
    __deref_out_bcount(*f_pcbXMR) DRM_BYTE                    **f_ppbXMR,
    __out       DRM_DWORD                                      *f_pcbXMR 
);

DRM_RESULT DRM_CALL NDT_License_BuildSimpleXMR(
    __in const  NDTMGR_CONTEXT                          * const f_pNdtMgrContext,
    __in const  NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT  * const f_pNdtContentSessionContext,
    __in const  WMDRMNET_POLICY                         * const f_pPolicy,
    __in const  DRM_ID                                          f_idSerial,
    __in const  NDT_CERTIFICATE_INFO                    * const f_pCertificateInfo,
    __deref_out_bcount(*f_pcbXMR) DRM_BYTE                    **f_ppbXMR,
    __out       DRM_DWORD                                      *f_pcbXMR 
);

DRM_RESULT DRM_CALL NDT_License_BuildLeafXMR(
    __in const  NDTMGR_CONTEXT                          * const f_pNdtMgrContext,
    __in const  NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT  * const f_pNdtContentSessionContext,
    __in const  WMDRMNET_POLICY                         * const f_pPolicy,
    __in const  DRM_ID                                          f_idSerial,
    __deref_out_bcount(*f_pcbXMR) DRM_BYTE                    **f_ppbXMR,
    __out       DRM_DWORD                                      *f_pcbXMR 
);

EXIT_PK_NAMESPACE;

#endif /* __NDTLICENSE_H__ */

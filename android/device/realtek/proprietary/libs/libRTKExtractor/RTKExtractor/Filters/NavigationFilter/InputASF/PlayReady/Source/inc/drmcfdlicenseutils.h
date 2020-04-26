/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#include <drmnamespace.h>   /* defining proper namespace (if used) */

ENTER_PK_NAMESPACE;

/*
**  File drmcfdlicenseutils.h - helper functions to build license response and
**  CRL update license response.
**  Functions are developed and used for MSDB project
*/

DRM_API DRM_RESULT DRM_CALL DRM_LBD_AddXMLNodeFromDWORD
(
    __inout       _XMBContext      *f_pXMLContext,
    __in    const DRM_CONST_STRING *f_dstrName,
    __in    const DRM_DWORD         f_dwVal
);

DRM_API DRM_RESULT DRM_CALL DRM_LBD_AddXMLEncodeDataBase64W
(
    __inout                       _XMBContext      *f_pXMLContext,
    __in                    const DRM_CONST_STRING *f_pdstrName,
    __in_opt                const DRM_CONST_STRING *f_pdstrType,
    __in_bcount(f_cbBufLen) const DRM_BYTE         *f_pbBufValue,
    __in                    const DRM_DWORD         f_cbBufLen
);


DRM_API DRM_RESULT DRM_CALL DRM_LBD_AppendSemiColonIfNotPresent(
    __inout       DRM_STRING *f_pdstrExpression,
    __in    const DRM_DWORD   f_cchMaxStringLen
);

DRM_API DRM_RESULT DRM_CALL DRM_LBD_CalculateCheckSum
(
    __in_ecount(f_cbData)         const DRM_BYTE *f_pbData,
    __in                          const DRM_DWORD f_cbData,
    __out_ecount(CHECKSUM_LENGTH)       DRM_BYTE  f_pbCheckSum[CHECKSUM_LENGTH]
);

DRM_API DRM_RESULT DRM_CALL DRM_LBD_GenerateDateTime
(
    __in  const DRMSYSTEMTIME *f_pDateTime,
    __out       DRM_STRING    *f_pdstrDateTime,
    __in  const DRM_DWORD      f_cchBuffer
);

DRM_DWORD DRM_API DRM_LBD_GetKeySize
(
    __in const DRM_ENCRYPT_KEY_TYPE f_eKeyType
);

DRM_API DRM_RESULT DRM_LBD_GenerateXMRLicenseResponse
(
    __in                             const DRM_ENCRYPT_KEY_CONTEXT   *f_pKeyContext,
    __in_opt                         const DRM_ENCRYPT_KEY_CONTEXT   *f_pBindKeyContext,
    __in_opt                         const DRM_DEVICE_CERT_CONTEXT   *f_pCertContext,
    __in                             const DRM_LICENSE_POLICY        *f_pLicensePolicy,
    __out_ecount(*f_pcchLicenseResp)       DRM_CHAR                  *f_pszLicenseResp,
    __inout                                DRM_DWORD                 *f_pcchLicenseResp
);

DRM_API DRM_RESULT DRM_CALL DRM_LBD_InitBinaryDeviceCertificateContext
(
    __inout                               DRM_APP_CONTEXT         *f_poAppContext,
    __in_bcount(f_cbDeviceCert)     const DRM_BYTE                *f_pbDeviceCert,
    __in                            const DRM_DWORD                f_cbDeviceCert,
    __in_bcount_opt(f_cbDomainCert) const DRM_BYTE                *f_pbDomainCert,
    __in                            const DRM_DWORD                f_cbDomainCert,
    __out                                 DRM_DEVICE_CERT_CONTEXT *f_pCertContext,
    __out                                 DRM_DWORD               *f_pdwFlags
);

/* Maximum length of printed dword */
#define DRM_MAX_DWORD_PRINTED_LEN 12

/* Maximum length of the string with date */
#define MAX_DATE_BUFFER_LEN   32

EXIT_PK_NAMESPACE;

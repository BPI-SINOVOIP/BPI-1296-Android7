/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_CONTENTREVOCATION__
#define __DRM_CONTENTREVOCATION__

#include <drmcommon.h>
#include <oemcryptoctx.h>
#include <drmliceval.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_NO_INLINE DRM_BOOL DRM_CALL DRM_CONTENTREVOCATION_IsContentRevocationSupported(DRM_VOID) DRM_NO_INLINE_ATTRIBUTE;
DRM_API DRM_BOOL DRM_CALL DRM_CONTENTREVOCATION_IsContentRevocationUnsupported(DRM_VOID);

DRM_API DRM_RESULT DRM_CALL DRM_CONTENTREVOCATION_LIC_VerifyContentRevocation(
    IN const DRM_CONST_STRING   *pdstrContentRevocation, 
    OUT      DRM_CONST_STRING   *pdstrLSPubKey,
    OUT      DRM_DWORD          *pdwSequenceNumber,
    OUT      DRM_CONST_STRING   *pdstrContentOwnerPubKey,
    OUT      DRM_CONST_STRING   *pdstrCondition,
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto);

DRM_API DRM_RESULT DRM_CALL DRM_CONTENTREVOCATION_LIC_GetContentRevocation(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_DWORD           dwSequenceNumber,
    OUT      DRM_DWORD          *pdwSequenceNumber,
    OUT      DRM_CONST_STRING   *pdstrContentRevocation,
    OUT      DRM_CONST_STRING   *pdstrContentOwnerPubKey,
    OUT      DRM_CONST_STRING   *pdstrCondition,
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto);

DRM_API DRM_RESULT DRM_CALL DRM_CONTENTREVOCATION_LEVL_SetOrUpdateContentRevocation(
          DRM_LICEVAL_CONTEXT *f_pContextLicEval,
    const DRM_DST             *f_pDatastore );

DRM_API DRM_BOOL DRM_CALL DRM_CONTENTREVOCATION_LEVL_IsLicenseRevoked(
    IN const DRM_LICEVAL_CONTEXT *f_pContextLicEval,
    IN const DRM_DST             *f_pDatastore );

DRM_API DRM_RESULT DRM_CALL DRM_CONTENTREVOCATION_LEVL_VerifyHeaderSignature(
    __inout   DRM_LICEVAL_CONTEXT          *f_pContextLicEval,
    __in      eDRM_LICEVAL_OPERATION_STATE  f_eOperationState,
    __in      DRM_CONST_STRING             *f_pdstrKID,
    __in      DRM_CONST_STRING             *f_pdstrLIData );

EXIT_PK_NAMESPACE;

#endif /* __DRM_CONTENTREVOCATION__ */


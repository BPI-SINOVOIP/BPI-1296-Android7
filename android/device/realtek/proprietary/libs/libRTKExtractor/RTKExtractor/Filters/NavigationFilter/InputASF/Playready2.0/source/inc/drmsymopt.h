/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_SYMOPT__
#define __DRM_SYMOPT__

#include <drmcommon.h>
#include <drmlicacqv3.h>

ENTER_PK_NAMESPACE;

typedef enum
{
    DRM_SYMMOPT_STATUS_NONE          = 0,
    DRM_SYMMOPT_STATUS_PARTIAL_NOTAG = 1,
    DRM_SYMMOPT_STATUS_PARTIAL       = 2,
    DRM_SYMMOPT_STATUS_COMPLETE      = 3
} DRM_SYMMOPT_STATUS;

DRM_API DRM_BOOL DRM_CALL DRM_SYMOPT_IsSymOptSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_SYMOPT_IsSymOptUnsupported(DRM_VOID);

DRM_API DRM_RESULT DRM_CALL DRM_SYMOPT_LIC_CheckSymmOptStatus(
    __in      const  DRM_CONST_STRING   *f_pdstrLicense,
    __out            DRM_SYMMOPT_STATUS *f_eStatus,
    __out_opt        DRM_CONST_STRING   *f_pdstrSymmSigNode,
    __out_opt        DRM_CONST_STRING   *f_pdstrSymmSigNodeData,
    __out_opt        DRM_GUID           *f_pCkVerifiedGUID );

DRM_API DRM_RESULT DRM_CALL DRM_SYMOPT_LIC_UpdateLicenseWithSymmetricData(
    __inout        DRM_STRING          *f_pdstrLicense,
    __in           DRM_DWORD            f_cchLicenseMax,
    __in           DRM_LICEVAL_CONTEXT *f_pcontextLEVL,
    __in           DRM_BOOL             f_fVerifiedCK,
    __in_opt const DRM_SLK             *f_pslk );

DRM_API DRM_RESULT DRM_CALL DRM_SYMOPT_LIC_VerifySymmetricSignatureIfPresent(
    IN const DRM_CONST_STRING *f_pdstrLicense,
    IN       DRM_BB_CONTEXT   *f_pcontextBBX,
    IN       DRM_CONST_STRING *f_pdstrLIData,
    IN       DRM_CONST_STRING *f_pdstrSignature,
       OUT   DRM_LONG         *f_plResult );

DRM_API DRM_BOOL DRM_CALL DRM_SYMOPT_LA_PrepareSLKFromLicense(
    IN           DRM_DST                              *f_pDatastore,
    IN           DRM_LICEVAL_CONTEXT                  *f_pcontextLEVL,
    IN     const DRM_CHAR                             *f_pbResponse,
    IN           DRM_SUBSTRING                        *f_pdasstrLicenseList,
    OUT          DRM_SLKDATA                          *f_pslkdata );

DRM_API DRM_RESULT DRM_CALL DRM_SYMOPT_LIC_UpdateLicenseWithSymmetricDataDuringLacq(
    __in_bcount( DRM_MAX_LICENSESIZE )  DRM_BYTE             f_rgbLicenseBuffer[__CB_DECL(DRM_MAX_LICENSESIZE)],
    __inout                             DRM_CONST_STRING    *f_pdstrKID,
    __inout                             DRM_DWORD           *f_pcbVersionLicense,
    __inout                             DRM_BOOL            *f_pfSLKValid,
    __inout                             DRM_BOOL            *f_pfTriedLoadingSLK,
    __in                                DRM_DST             *f_pDatastore,
    __in                                DRM_LICEVAL_CONTEXT *f_pcontextLEVL,
    __in_bcount( f_cbResponse )         DRM_BYTE            *f_pbResponse,
    __in                                DRM_DWORD            f_cbResponse,
    __inout                             DRM_SUBSTRING       *f_pdasstrLicenseList,
    __inout                             DRM_SLKDATA         *f_pslkdata );

DRM_API DRM_RESULT DRM_CALL DRM_SYMOPT_LA_UpdateSLK(
    IN                                 DRM_ID               *f_pSLKID,
    __in_bcount( f_cbKeyLength ) const DRM_BYTE             *f_pbSLKKey,
    IN                                 DRM_DWORD             f_cbKeyLength,
    IN                                 DRM_DST              *f_pDatastore,
    IN                                 DRM_LICEVAL_CONTEXT  *f_pcontextLEVL,
    OUT                                DRM_SLKDATA          *f_pSLK );

DRM_API DRM_RESULT DRM_CALL DRM_SYMOPT_LA_GetSLK(
    OUT     DRM_SLKDATA           *f_pSLK,
    IN      DRM_DST               *f_pDatastore,
    IN      DRM_LICEVAL_CONTEXT   *f_pcontextLEVL );

DRM_API DRM_RESULT DRM_CALL DRM_SYMOPT_LA_ExtractSLKDataFromLicenseResponse(
    __in  DRM_LICRESP_CONTEXT_V3 *f_poLicRespContext,
    __in  DRM_ANSI_CONST_STRING  *f_pdastrResponse,
    __in  DRM_SUBSTRING          *f_pdasstrResponse,
    __out DRM_SLKDATA            *f_pSLKData,
    __out DRM_BOOL               *f_pfValidSLK );

DRM_API DRM_RESULT DRM_CALL DRM_SYMOPT_DDC_VerifySymmetricSignature(
    IN DRM_VOID         *f_pOEMContext,
    IN DRM_CONST_STRING *f_pdstrCert );

EXIT_PK_NAMESPACE;

#endif /* __DRM_SYMOPT__ */


/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMWMDRM_H__
#define __DRMWMDRM_H__

#include <drmwmdrmtypes.h>
#include <drmcommon.h>
#include <drmlicevaltypes.h>
#include <drmviewrights.h>
#include <drmsymopt.h>

ENTER_PK_NAMESPACE;
DRM_API DRM_BOOL DRM_CALL DRM_WMDRM_IsWMDRMSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_WMDRM_IsWMDRMUnsupported(DRM_VOID);

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_WMDRM_LEVL_PerformOperationsXML(
    IN OUT   DRM_LICEVAL_CONTEXT          *f_pContextLicEval,
    IN       eDRM_LICEVAL_OPERATIONS       f_eOperation,
    IN       eDRM_LICEVAL_OPERATION_STATE  f_eOperationState,
    IN const DRM_CONST_STRING             *f_pdstrAction,
    OUT      DRM_BOOL                     *f_pfPerform,
    OUT      DRM_BOOL                     *f_pfActionExisted,
    IN const DRM_DST                      *f_pDatastore ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_WMDRM_LEVL_GetLicenseReasonForUnusable(
    IN  const DRM_LICEVAL_CONTEXT           *f_pcontextLicEval,
    OUT       DRM_LONG                      *f_plReasonForUnusable ) DRM_NO_INLINE_ATTRIBUTE;

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LEVL_ParseV2License(
    __in      const DRM_CONST_STRING *f_pdstrLicense, 
    __in      const DRM_CONST_STRING *f_pdstrAttrName,
    __out_opt       DRM_DWORD        *f_pdwValue, 
    __out_opt       DRMFILETIME      *f_pfiletime );

DRM_API DRM_BOOL DRM_CALL DRM_WMDRM_LEVL_HasSAP(
    __in      const DRM_CONST_STRING *f_pdstrLicense );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_CheckSymmOptStatus(
    __in      const  DRM_CONST_STRING   *f_pdstrLicense,
    __out            DRM_SYMMOPT_STATUS *f_eStatus,
    __out_opt        DRM_CONST_STRING   *f_pdstrSymmSigNode,
    __out_opt        DRM_CONST_STRING   *f_pdstrSymmSigNodeData,
    __out_opt        DRM_GUID           *f_pCkVerifiedGUID );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_UpdateLicenseWithSymmetricData(
    __inout        DRM_STRING          *f_pdstrLicense,
    __in           DRM_DWORD            f_cchLicenseMax,
    __in           DRM_LICEVAL_CONTEXT *f_pcontextLEVL,
    __in           DRM_BOOL             f_fVerifiedCK,
    __in_opt const DRM_SLK             *f_pslk );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_VerifySymmetricSignatureIfPresent(
    IN const DRM_CONST_STRING *f_pdstrLicense,
    IN       DRM_BB_CONTEXT   *f_pcontextBBX,
    IN       DRM_CONST_STRING *f_pdstrLIData,
    IN       DRM_CONST_STRING *f_pdstrSignature,
       OUT   DRM_LONG         *f_plResult );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_PerformAppRevocationCheck(
    IN     DRM_CRYPTO_CONTEXT          *f_pContextCrypto,
    IN     DRM_REVOCATIONSTORE_CONTEXT *f_pContextRev,
    IN     DRM_CONST_STRING            *f_pdstrContentLicense,
    IN     APPCERT                     *f_pAppCert,
    __in_bcount( f_cbRevocationBuffer ) DRM_BYTE *f_pbRevocationBuffer,
    IN     DRM_DWORD                    f_cbRevocationBuffer,
    IN OUT DRM_REVLIST_ID_VALUES       *f_pRevListIDs );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_GetCopyOPLRestriction(
    __inout     DRM_VIEW_RIGHTS_CONTEXT *f_pcontextLQ,
    __in  const DRM_CONST_STRING        *f_pdstrAction,
    __in  const DRM_CONST_STRING        *f_pdstrRightUsed,
    __out       DRM_BOOL                *f_pfHasOPLs,
    __out       DRM_DWORD               *f_pdwResult );

DRM_API DRM_BOOL DRM_CALL DRM_WMDRM_LIC_HasOPLRestriction(
    __in const DRM_CONST_STRING *f_pdstrLicense,
    __in const DRM_CONST_STRING *f_pdstrRightUsed );

DRM_API DRM_BOOL DRM_CALL DRM_WMDRM_LIC_ShouldCheckClockRollback(
    __in const DRM_CONST_STRING *f_pdstrLicense );

typedef enum
{
    DRM_LICENSE_ATTRIB_VERSION        =  1,
    DRM_LICENSE_ATTRIB_KID            =  2,
    DRM_LICENSE_ATTRIB_LID            =  3,
    DRM_LICENSE_ATTRIB_REVOCATION     =  4,
    DRM_LICENSE_ATTRIB_ISSUEDATE      =  5,
    DRM_LICENSE_ATTRIB_CONTENTPUBKEY  =  6,
    DRM_LICENSE_ATTRIB_META_ATTRIBUTE =  7,
    DRM_LICENSE_ATTRIB_OTHER          =  8,
    DRM_LICENSE_ATTRIB_PRIORITY       =  9,
    DRM_LICENSE_ATTRIB_METERING_ID    = 10,
    DRM_LICENSE_ATTRIB_CHAINEDKID     = 11,
    DRM_LICENSE_ATTRIB_REVINFO        = 12,
    DRM_LICENSE_ATTRIB_SOURCEID       = 13
} eDRM_LICENSE_ATTRIBUTES;

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_GetAttribute(
    IN const DRM_CONST_STRING        *pdstrLicense, 
    IN const DRM_CONST_STRING        *pdstrAttrName,
    IN       eDRM_LICENSE_ATTRIBUTES  eAttribute,
    IN  OUT  DRM_CONST_STRING        *pdstrLIData,
        OUT  DRM_CONST_STRING        *pdstrValue,
    IN       DRM_WCHAR                chXPathSeparator );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_GetEvent(
    IN const DRM_CONST_STRING *pdstrLicense, 
    IN const DRM_CONST_STRING *pdstrEvent, 
    IN const DRM_CONST_STRING *pdstrTypeValue, 
    IN  OUT  DRM_CONST_STRING *pdstrLIData,
    OUT      DRM_CONST_STRING *pdstrCondition,     
    OUT      DRM_CONST_STRING *pdstrAction,
    OUT      DRM_CONST_STRING *pdstrRestriction ); /* OPTIONAL */

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_LoadLicenseAttributes(
    __in  const DRM_CONST_STRING    *f_rgpdstrEvents[],
    __in        DRM_DWORD            f_cRights,
    __inout     DRM_LICEVAL_CONTEXT *f_pLicEval );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_VerifyCertChain(
    IN const DRM_CONST_STRING    *pdstrLicense, 
    IN       DRM_BOOL             fCheckExpiry,
    IN       DRM_LICEVAL_CONTEXT *pcontextLEVL,
    OUT      DRM_LONG            *plResult );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_WMDRM_LIC_GetLSPubKey(
    const DRM_CONST_STRING   *pdstrLicense, 
          PUBKEY             *pbPubKey, 
          DRM_CRYPTO_CONTEXT *pcontextCrypto );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_GetEnablingBits(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_DWORD           dwIndex,
    OUT      DRM_DWORD          *pdwAlgorithm,
    OUT      PUBKEY             *pPubKey, 
    __out_bcount_opt( *pdValueLen ) DRM_BYTE *pbValue, 
    OUT      DRM_DWORD          *pdValueLen, 
    OUT      PUBKEY             *pVerPubKey, 
    __out_bcount( DRM_ECC160_SIGNATURE_LEN ) DRM_BYTE *pbSignature,
    __out_bcount_opt( CHECKSUM_LENGTH ) DRM_BYTE rgbChainedChecksum[__CB_DECL(CHECKSUM_LENGTH)],
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_VerifySignature(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_BB_CONTEXT     *pcontextBBX,
    OUT      DRM_LONG           *plResult );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_PerformActionsOPLCallbacks(
    __inout       DRM_FFLICENSE               *f_pFFLicense,
    __inout       DRM_STACK_ALLOCATOR_CONTEXT *f_pstack,
    __in    const DRM_CONST_STRING            *f_rgpdstrRights[],
    __in          DRM_DWORD                    f_iRight,
    __in          DRMPFNPOLICYCALLBACK         f_pfnPolicyCallback );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_ExtractRevocationVersions(
    __inout DRM_FFLICENSE    *f_pFFLicense,
    __out   DRM_DWORD        *f_pidCRLWMDRMAppRequired,
    __out   DRM_DWORD        *f_pidCRLDeviceRequired,
    __out   DRM_DWORD        *f_pidCRLWMDRMNETRequired,
    __out   DRM_DWORD        *f_pdwRevInfo );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_GetLicenseInfoAndSetup(
    __inout       DRM_FFLICENSE    *f_pFFLicense,
    __in    const DRM_CONST_STRING *f_rgpdstrRights[],
    __in          DRM_DWORD         f_cRights );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_GetSourceIDFromLicense(
    __inout DRM_FFLICENSE *f_pFFLicense );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LIC_GetInclusionList(
    __in const DRM_CONST_STRING  *f_pdstrLicense,
    __inout    DRM_GUID           f_rgInclusionList[DRM_MAX_INCLUSION_GUIDS],
    __inout    DRM_BOOL           f_rgfGuidValid[DRM_MAX_INCLUSION_GUIDS][DRM_MAX_LICENSE_CHAIN_DEPTH],
    __in       DRM_DWORD          f_dwChainDepth );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_EXPRFUNC_FunctionGetValue(
    IN  TOKEN                       *pOperator,
    IN  DRM_LONG                     cArgs,
    IN  TOKEN                        rgArgs[],
    OUT TOKEN                       *pOperandResult,
    IN  DRM_WMDRM_EXPR_EVAL_CONTEXT *pEEContext );

DRM_API DRM_LONG DRM_CALL DRM_WMDRM_EXPRFUNC_Arity(DRM_EXPR_FUNCTION_TYPE fnType);

DRM_API DRM_BOOL DRM_CALL DRM_WMDRM_EXPRFUNC_IsValidFunction(
    IN const DRM_CONST_STRING       *pdstrToken,
    IN       DRM_DWORD              *pfnType);

/* In the following Extract*Token functions if cchExpression == 0 it is assumed that the string is NULL termintated. */
/* Note: These functions modify the pointer they are given. */
DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_WMDRM_EXPR_ExtractLongToken  ( DRM_CONST_STRING* pdstrExpression, TOKEN* pToken );
DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_WMDRM_EXPR_ExtractDateToken  ( DRM_CONST_STRING* pdstrExpression, TOKEN *pToken );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_EXPR_LEVL_EvaluateExpression(
    IN OUT    DRM_LICEVAL_CONTEXT            *f_pContextLicEval,
    IN  const DRM_CONST_STRING               *f_pdstrExpression,
    OUT       DRM_BOOL                       *f_pfValue );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_WMDRM_EXPR_EvaluateExpression(
    IN const DRM_CONST_STRING            *pdstrExpression, 
    IN OUT   DRM_WMDRM_EXPR_EVAL_CONTEXT *pEEContext, 
       OUT   TOKEN                       *ptResult );

/*
** Secure store variable accessor/mutator functions. 
** These will be given to the Expression evaluator via it's context structure.  
** The implementation lives in exprvariable.c 
*/
DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_WMDRM_EXPR_GlobalSetVariable(
    const DRM_CONST_STRING *pStringToken, 
    const TOKEN            *pNewValue, 
          TOKEN            *pResult, 
          DRM_VOID         *pvOpaqueData );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_WMDRM_EXPR_GlobalGetVariable(
    const DRM_CONST_STRING *pStringToken,
          TOKEN            *pResult,
          DRM_VOID         *pvOpaqueData );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LA_ProcessResponse(
    __in_bcount( f_cbResponse ) DRM_BYTE *f_pbResponse,
    IN     DRM_DWORD                             f_cbResponse,
    IN     DRM_LICEVAL_CONTEXT                  *f_pcontextLEVL,
    IN     DRM_LICSTORE_CONTEXT                 *f_pcontextLSTXML,
    IN     DRM_LICSTORE_CONTEXT                 *f_pcontextLSTXMR,
    __in   DRM_LICSTOREENUM_CONTEXT             *f_pLicStoreEnumContext,
    IN     DRM_INCLUSION_LIST_CALLBACK_STRUCT   *f_pInclusionListCallback,
    IN     pfnLicenseResponseCallback            f_pfnCallback,
    IN     DRM_VOID                             *f_pvCallbackContext,
    IN     DRM_DST                              *f_pDatastore,
    IN     DRM_VIEW_RIGHTS_CONTEXT              *f_pcontextASD,
    __in_bcount( DRM_MAX_LICENSESIZE ) DRM_BYTE f_rgbLicenseBuffer[__CB_DECL(DRM_MAX_LICENSESIZE)],
    IN     DRM_SYNC_CONTEXT                     *f_pcontextSync,
       OUT DRM_LONG                             *f_plReason);

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_HDR_VerifyLegacySignature(
    __in const DRM_CONST_STRING   *f_pdstrContentHeader,
    __in const DRM_CONST_STRING   *f_pdstrPubKey,
    __in       DRM_CONST_STRING   *f_pdstrData,
    __in       DRM_CRYPTO_CONTEXT *f_pContext );

DRM_API DRM_WORD DRM_CALL DRM_WMDRM_LICOPL_GetOPLLevel( 
    IN const DRM_CONST_STRING *f_pdstrRestrictions,
    IN const DRM_CONST_STRING *f_pdstrCategory );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LICOPL_ProcessExclusions( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_OPL_OUTPUT_IDS           *f_pOPLIds,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LICOPL_ProcessInclusions( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_OPL_OUTPUT_IDS           *f_pOPLIds,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LICOPL_ProcessPlayOutputLevelData( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_PLAY_OPL_EX2             *f_pPlayOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LICCOPYOPL_ASD_ExamineOutputLevels(
    IN     DRM_VIEW_RIGHTS_CONTEXT *f_pcontextLQ,
    IN     DRM_COPY_OPL            *f_pOutputLevels,
       OUT DRM_DWORD               *f_pdwResult );

DRM_API DRM_RESULT DRM_CALL DRM_WMDRM_LICCOPYOPL_ProcessCopyOutputLevelData( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_COPY_OPL                 *f_pCopyOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

EXIT_PK_NAMESPACE;

#endif /* __DRMWMDRM_H__ */


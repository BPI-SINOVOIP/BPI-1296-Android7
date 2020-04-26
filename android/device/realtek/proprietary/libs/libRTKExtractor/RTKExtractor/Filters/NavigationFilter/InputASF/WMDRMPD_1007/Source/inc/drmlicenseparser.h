/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMLICENSEPARSER_H__
#define __DRMLICENSEPARSER_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Possible drmresults. */
#define LIC_BASECODE                         0x1200
#define LIC_NOERROR                          hrOK
#define LIC_INIT_FAILURE                     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+1)
#define LIC_LICENSE_NOTSET                   MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+2)
#define LIC_PARAM_NOT_OPTIONAL               MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+3)
#define LIC_MEMORY_ALLOCATION_ERROR          MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+4)
#define LIC_INVALID_LICENSE                  MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+5)
#define LIC_FIELD_MISSING                    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+6)
#define LIC_UNSUPPORTED_VALUE                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+7)
#define LIC_UNKNOWN_ERROR                    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+8)
#define LIC_INVALID_REVLIST                  MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+9)
#define LIC_EXPIRED_CERT                     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LIC_BASECODE+10)

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

DRM_RESULT DRM_API DRM_LIC_GetAttribute(
    IN const DRM_CONST_STRING        *pdstrLicense, 
    IN const DRM_CONST_STRING        *pdstrAttrName,
    IN       eDRM_LICENSE_ATTRIBUTES  eAttribute,
    IN  OUT  DRM_CONST_STRING        *pdstrLIData,
        OUT  DRM_CONST_STRING        *pdstrValue,
    IN       DRM_WCHAR                chXPathSeparator);

DRM_RESULT DRM_API DRM_LIC_GetEvent(
    IN const DRM_CONST_STRING *pdstrLicense, 
    IN const DRM_CONST_STRING *pdstrEvent, 
    IN const DRM_CONST_STRING *pdstrTypeValue, 
    IN  OUT  DRM_CONST_STRING *pdstrLIData,
    OUT      DRM_CONST_STRING *pdstrCondition,     
    OUT      DRM_CONST_STRING *pdstrAction,
    OUT      DRM_CONST_STRING *pdstrRestriction); /* OPTIONAL */


/*--------------------------------------------------------------------
** 
** Function :   DRM_LIC_GetNextActionEvent
** 
** Synopsis :   Gets the next ONACTION event from the license buffer
** 
** Arguments :  
** 
** Returns :    
** 
** Notes :      
** 
--------------------------------------------------------------------*/
DRM_RESULT DRM_API DRM_LIC_GetNextActionEvent(
    IN OUT      DRM_CONST_STRING *pdstrLicBuffer, 
    OUT         DRM_CONST_STRING *pdstrCondition );

DRM_RESULT DRM_API DRM_LIC_GetEnablingBits(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_DWORD           dwIndex,
    OUT      DRM_DWORD          *pdwAlgorithm,
    OUT      PUBKEY             *pPubKey, 
    OUT      DRM_BYTE           *pbValue, 
    OUT      DRM_DWORD          *pdValueLen, 
    OUT      PUBKEY             *pVerPubKey, 
    OUT      DRM_BYTE           *pbSignature,
    OUT      DRM_BYTE            rgbChainedChecksum[__CB_DECL(CHECKSUM_LENGTH)],
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto);

DRM_RESULT DRM_API DRM_LIC_VerifySignature(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_BB_CONTEXT     *pcontextBBX,
    OUT      DRM_LONG           *plResult);

DRM_RESULT DRM_API DRM_LIC_VerifyCertChain(
    IN const DRM_CONST_STRING    *pdstrLicense, 
    IN       DRM_BOOL             fCheckExpiry,
    IN       DRM_LICEVAL_CONTEXT *pcontextLEVL,
    OUT      DRM_LONG            *plResult);

DRM_RESULT DRM_API DRM_LIC_GetInclusionList(
    IN const DRM_CONST_STRING  *pdstrContentRevocation, 
    IN OUT   DRM_GUID           rgInclusionList[DRM_MAX_INCLUSION_GUIDS],
    IN OUT   DRM_BOOL           rgfGuidValid[DRM_MAX_INCLUSION_GUIDS][DRM_MAX_LICENSE_CHAIN_DEPTH],
    IN       DRM_DWORD          dwChainDepth );

#if DRM_SUPPORT_CONTENT_REVOCATION

DRM_RESULT DRM_API DRM_LIC_VerifyContentRevocation(
    IN const DRM_CONST_STRING   *pdstrContentRevocation, 
    OUT      DRM_CONST_STRING   *pdstrLSPubKey,
    OUT      DRM_DWORD          *pdwSequenceNumber,
    OUT      DRM_CONST_STRING   *pdstrContentOwnerPubKey,
    OUT      DRM_CONST_STRING   *pdstrCondition,
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto);

DRM_RESULT DRM_API DRM_LIC_GetContentRevocation(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_DWORD           dwSequenceNumber,
    OUT      DRM_DWORD          *pdwSequenceNumber,
    OUT      DRM_CONST_STRING   *pdstrContentRevocation,
    OUT      DRM_CONST_STRING   *pdstrContentOwnerPubKey,
    OUT      DRM_CONST_STRING   *pdstrCondition,
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __DRMLICENSEPARSER_H__ */

/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMLICENSEPARSER_H__
#define __DRMLICENSEPARSER_H__

ENTER_PK_NAMESPACE;

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

DRM_API DRM_RESULT DRM_CALL DRM_LIC_GetAttribute(
    IN const DRM_CONST_STRING        *pdstrLicense, 
    IN const DRM_CONST_STRING        *pdstrAttrName,
    IN       eDRM_LICENSE_ATTRIBUTES  eAttribute,
    IN  OUT  DRM_CONST_STRING        *pdstrLIData,
        OUT  DRM_CONST_STRING        *pdstrValue,
    IN       DRM_WCHAR                chXPathSeparator);

DRM_API DRM_RESULT DRM_CALL DRM_LIC_GetEvent(
    IN const DRM_CONST_STRING *pdstrLicense, 
    IN const DRM_CONST_STRING *pdstrEvent, 
    IN const DRM_CONST_STRING *pdstrTypeValue, 
    IN  OUT  DRM_CONST_STRING *pdstrLIData,
    OUT      DRM_CONST_STRING *pdstrCondition,     
    OUT      DRM_CONST_STRING *pdstrAction,
    OUT      DRM_CONST_STRING *pdstrRestriction); /* OPTIONAL */

DRM_API DRM_RESULT DRM_CALL DRM_LIC_GetEnablingBits(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_DWORD           dwIndex,
    OUT      DRM_DWORD          *pdwAlgorithm,
    OUT      PUBKEY             *pPubKey, 
    __out_bcount_opt( *pdValueLen ) DRM_BYTE *pbValue, 
    OUT      DRM_DWORD          *pdValueLen, 
    OUT      PUBKEY             *pVerPubKey, 
    __out_bcount( DRM_ECC160_SIGNATURE_LEN ) DRM_BYTE *pbSignature,
    __out_bcount_opt( CHECKSUM_LENGTH ) DRM_BYTE rgbChainedChecksum[__CB_DECL(CHECKSUM_LENGTH)],
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto);

DRM_API DRM_RESULT DRM_CALL DRM_LIC_VerifySignature(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_BB_CONTEXT     *pcontextBBX,
    OUT      DRM_LONG           *plResult);

DRM_API DRM_RESULT DRM_CALL DRM_LIC_VerifyCertChain(
    IN const DRM_CONST_STRING    *pdstrLicense, 
    IN       DRM_BOOL             fCheckExpiry,
    IN       DRM_LICEVAL_CONTEXT *pcontextLEVL,
    OUT      DRM_LONG            *plResult);

DRM_API DRM_RESULT DRM_CALL DRM_LIC_GetInclusionList(
    IN const DRM_CONST_STRING  *pdstrContentRevocation, 
    IN OUT   DRM_GUID           rgInclusionList[DRM_MAX_INCLUSION_GUIDS],
    IN OUT   DRM_BOOL           rgfGuidValid[DRM_MAX_INCLUSION_GUIDS][DRM_MAX_LICENSE_CHAIN_DEPTH],
    IN       DRM_DWORD          dwChainDepth );

#if DRM_SUPPORT_CONTENT_REVOCATION

DRM_API DRM_RESULT DRM_CALL DRM_LIC_VerifyContentRevocation(
    IN const DRM_CONST_STRING   *pdstrContentRevocation, 
    OUT      DRM_CONST_STRING   *pdstrLSPubKey,
    OUT      DRM_DWORD          *pdwSequenceNumber,
    OUT      DRM_CONST_STRING   *pdstrContentOwnerPubKey,
    OUT      DRM_CONST_STRING   *pdstrCondition,
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto);

DRM_API DRM_RESULT DRM_CALL DRM_LIC_GetContentRevocation(
    IN const DRM_CONST_STRING   *pdstrLicense,
    IN       DRM_DWORD           dwSequenceNumber,
    OUT      DRM_DWORD          *pdwSequenceNumber,
    OUT      DRM_CONST_STRING   *pdstrContentRevocation,
    OUT      DRM_CONST_STRING   *pdstrContentOwnerPubKey,
    OUT      DRM_CONST_STRING   *pdstrCondition,
    IN       DRM_CRYPTO_CONTEXT *pcontextCrypto);

#endif

EXIT_PK_NAMESPACE;

#endif /* __DRMLICENSEPARSER_H__ */

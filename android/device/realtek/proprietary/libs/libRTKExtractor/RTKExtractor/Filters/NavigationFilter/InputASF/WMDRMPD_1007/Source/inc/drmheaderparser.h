/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMHEADERPARSER_H__
#define __DRMHEADERPARSER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Possible drmresults. */
#define CH_BASECODE                         (DRM_RESULT_CODE(DRM_E_CH_BASECODE))
#define CH_VERSION_MISSING                  (DRM_E_CH_VERSION_MISSING)
#define CH_KID_MISSING                      (DRM_E_CH_KID_MISSING)
#define CH_LAINFO_MISSING                   (DRM_E_CH_LAINFO_MISSING)
#define CH_CHECKSUM_MISSING                 (DRM_E_CH_CHECKSUM_MISSING)
#define CH_ATTR_MISSING                     (DRM_E_CH_ATTR_MISSING)
#define CH_INVALID_HEADER                   (DRM_E_CH_INVALID_HEADER)
#define CH_UNABLE_TO_VERIFY                 (DRM_E_CH_UNABLE_TO_VERIFY)
#define CH_UNSUPPORTED_VERSION              (DRM_E_CH_UNSUPPORTED_VERSION)
#define CH_UNSUPPORTED_HASH_ALGORITHM       (DRM_E_CH_UNSUPPORTED_HASH_ALGORITHM)
#define CH_UNSUPPORTED_SIGN_ALGORITHM       (DRM_E_CH_UNSUPPORTED_SIGN_ALGORITHM)
#define CH_BAD_KEY                          (DRM_E_CH_BAD_KEY)
#define CH_NOT_SIGNED                       (DRM_E_CH_NOT_SIGNED)
#define CH_UNKNOWN_ERROR                    (DRM_E_CH_UNKNOWN_ERROR)


typedef enum
{
    DRM_HEADER_ATTRIB_VERSION = 1,
    DRM_HEADER_ATTRIB_KID = 2,
    DRM_HEADER_ATTRIB_LAINFO = 3,
    DRM_HEADER_ATTRIB_CHECKSUM = 4,
    DRM_HEADER_ATTRIB_SECURITYVERSION = 5,
    DRM_HEADER_ATTRIB_OTHER = 6
} eDRM_HEADER_ATTRIBUTES;

DRM_RESULT DRM_API DRM_HDR_GetAttribute(
    IN const DRM_CONST_STRING       *pdstrContentHeader,
    IN const DRM_CONST_STRING       *pdstrAttrName,
    IN       eDRM_HEADER_ATTRIBUTES  eAttribute,
    OUT      DRM_CONST_STRING       *pdstrValue,
    IN       DRM_WCHAR               chXPathSeparator);

DRM_RESULT DRM_API DRM_HDR_GetUplink(
    IN const DRM_CONST_STRING *pdstrContentHeader,
    IN       DRM_DWORD         dwUplinkNumber,
    OUT      DRM_CONST_STRING *pdstrKid );

DRM_RESULT DRM_API DRM_HDR_Verify(
    IN const DRM_CONST_STRING   *pdstrContentHeader,
    IN const DRM_CONST_STRING   *pdstrPubKey,
    IN       DRM_CRYPTO_CONTEXT *pContext,
    OUT      DRM_LONG           *plResult);


#ifdef __cplusplus
}
#endif

#endif /* __DRMHEADERPARSER_H__ */

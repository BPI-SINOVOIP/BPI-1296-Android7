/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMHEADERPARSER_H__
#define __DRMHEADERPARSER_H__

ENTER_PK_NAMESPACE;

#define DRM_HEADER_MAX_SIZE_IN_BYTES 5120

typedef enum
{
    DRM_HEADER_ATTRIB_VERSION            = 1,
    DRM_HEADER_ATTRIB_KID                = 2,
    DRM_HEADER_ATTRIB_LAINFO             = 3,
    DRM_HEADER_ATTRIB_CHECKSUM           = 4,
    DRM_HEADER_ATTRIB_SECURITYVERSION    = 5,
    DRM_HEADER_ATTRIB_OTHER              = 6,
    DRM_HEADER_ATTRIB_LICENSE_ACQ_V4_URL = 7,
    DRM_HEADER_ATTRIB_LICENSE_UI_URL     = 8,
    DRM_HEADER_ATTRIB_DOMAIN_SERVICE_ID  = 9
} eDRM_HEADER_ATTRIBUTES;

typedef enum
{
    DRM_HEADER_VERSION_UNKNOWN           = 0,
    DRM_HEADER_VERSION_2                 = 1,
    DRM_HEADER_VERSION_2_4               = 3,
    DRM_HEADER_VERSION_4                 = 2,
} eDRM_HEADER_VERSION;

DRM_API DRM_RESULT DRM_CALL DRM_HDR_GetHeaderVersion( 
    IN const DRM_CONST_STRING     *f_pdstrContentHeader,
    OUT      eDRM_HEADER_VERSION  *f_peVersion );

DRM_API DRM_RESULT DRM_CALL DRM_HDR_GetAttribute(
    IN const DRM_CONST_STRING       *pdstrContentHeader,
    IN const DRM_CONST_STRING       *pdstrAttrName,
    IN       eDRM_HEADER_ATTRIBUTES  eAttribute,
    OUT      DRM_CONST_STRING       *pdstrValue,
    IN       DRM_WCHAR               chXPathSeparator);

DRM_API DRM_RESULT DRM_CALL DRM_HDR_GetUplink(
    IN const DRM_CONST_STRING *pdstrContentHeader,
    IN       DRM_DWORD         dwUplinkNumber,
    OUT      DRM_CONST_STRING *pdstrKid );

DRM_API DRM_RESULT DRM_CALL DRM_HDR_Verify(
    IN const DRM_CONST_STRING   *pdstrContentHeader,
    IN const DRM_CONST_STRING   *pdstrPubKey,
    IN       DRM_CRYPTO_CONTEXT *pContext,
    OUT      DRM_LONG           *plResult);

EXIT_PK_NAMESPACE;

#endif /* __DRMHEADERPARSER_H__ */

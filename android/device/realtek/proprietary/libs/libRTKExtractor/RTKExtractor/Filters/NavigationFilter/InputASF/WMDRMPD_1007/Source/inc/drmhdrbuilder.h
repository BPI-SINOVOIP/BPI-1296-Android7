/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMHEADERBUILDER_H__
#define __DRMHEADERBUILDER_H__


#ifdef __cplusplus
extern "C" {
#endif



/* 
** Header Builder
*/

/**********************************************************************
** Function:    DRM_HB_CreateHeader
** Synopsis:    
** Arguments:   [pbContext] -- 
** Returns:     
** Notes:
***********************************************************************/
DRM_RESULT DRM_API DRM_HB_CreateHeader(
    IN     DRM_BYTE  *pbHBContext,
    IN OUT DRM_DWORD *pcbHBContext);

DRM_RESULT DRM_API DRM_HB_SignHeader(
    IN  DRM_BYTE           *pbHBContext,
    IN  DRM_CRYPTO_CONTEXT *pCryptoContext,
    IN  DRM_CONST_STRING   *pdstrPrivKey,      /* base64 encoded */
    OUT DRM_BYTE          **ppbHeaderXML,
    OUT DRM_DWORD          *pcbHeaderXML);

DRM_RESULT DRM_API DRM_HB_ReallocHeader(
    IN DRM_BYTE *pbOldHBContext,
    IN DRM_DWORD cbNewHBContext,
    IN DRM_BYTE *pbNewHBContext);

DRM_RESULT DRM_API DRM_HB_GetContextSize(
    IN  DRM_BYTE  *pbHBContext,
    OUT DRM_DWORD *pcbContext);

DRM_RESULT DRM_API DRM_HB_SetKeyID(
    IN       DRM_BYTE         *pbHBContext,
    IN const DRM_CONST_STRING *pdstrKID);      /* base64 encoded */

DRM_RESULT DRM_API DRM_HB_SetLicAcqUrl(
    IN       DRM_BYTE         *pbHBContext,
    IN const DRM_CONST_STRING *pdstrURL);

/*
** Perform checksum algo to the given content key and set the check to header
*/
DRM_RESULT DRM_API DRM_HB_SetChecksum(
    IN       DRM_BYTE         *pbHBContext,
    IN const DRM_CONST_STRING *pdstrContentKey);     /* base64 encoded */

DRM_RESULT DRM_API DRM_HB_SetAttribute(
    IN       DRM_BYTE         *pbHBContext,
    IN const DRM_CONST_STRING *pdstrAttrName,
    IN const DRM_CONST_STRING *pdstrAttrValue);


#ifdef __cplusplus
}
#endif

#endif /* __DRMHEADERBUILDER_H__ */



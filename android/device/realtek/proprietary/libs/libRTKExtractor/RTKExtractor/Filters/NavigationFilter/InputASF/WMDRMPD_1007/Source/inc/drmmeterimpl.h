/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMMETERIMPL_H__
#define __DRMMETERIMPL_H__

#ifdef __cplusplus
extern "C" {
#endif

DRM_RESULT DRM_API DRM_MTR_GetAllData (
    IN     DRM_METERING_CONTEXT *pcontextMetering,
       OUT DRM_BYTE             *pbData,
    IN OUT DRM_DWORD            *pcbData);

DRM_RESULT DRM_API DRM_MTR_InitEnum (
    IN const DRM_MID                   *pmid,
       OUT   DRM_METERINGENUM_CONTEXT  *pcontextMeteringEnum,
    IN       DRM_HDS_CONTEXT           *pcontextHDS);

DRM_RESULT DRM_API DRM_MTR_EnumNext ( 
    IN     DRM_METERINGENUM_CONTEXT *pcontextMeteringEnum,
       OUT DRM_KID                  *pkid,
       OUT DRM_DWORD                *pcbData);

#ifdef __cplusplus
}
#endif

#endif /* __DRMMETERIMPL_H__ */


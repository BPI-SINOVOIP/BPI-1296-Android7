/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMMETERING_H__
#define __DRMMETERING_H__

#include <drmxmlbuilder.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DRM_METERING_CONTEXT        DRM_SECSTORE_CONTEXT
#define DRM_METERINGENUM_CONTEXT    DRM_SECSTOREENUM_CONTEXT

#define DRM_METER_RESPONSE_PARTIAL 0x00000001


DRM_RESULT DRM_API DRM_MTR_GenerateMeterChallenge(
    IN       DRM_LICEVAL_CONTEXT *f_pcontextLEVL,
    IN       DRM_BYTE            *f_pbStack,
    IN       DRM_DWORD            f_cbStack,
    IN const DRM_CONST_STRING    *f_pdstrMeterCert,
       OUT   _XMBContext         *f_pbChallenge,
    IN OUT   DRM_DWORD           *f_pcbChallenge,
       OUT   DRM_WCHAR           *f_pwszURL,
    IN OUT   DRM_DWORD           *f_pcchURL);

DRM_RESULT DRM_API DRM_MTR_ProcessMeterResponse(
    IN     DRM_LICEVAL_CONTEXT *f_pcontextLEVL,
    IN     DRM_BYTE            *f_pbStack,
    IN     DRM_DWORD            f_cbStack,
    IN     DRM_BYTE            *f_pbData,
    IN     DRM_DWORD            f_cbData,
       OUT DRM_DWORD           *f_pfFlags);

DRM_RESULT DRM_API DRM_MTR_CloseContext( 
    IN DRM_METERING_CONTEXT *pcontextMetering,
    IN DRM_HDS_CONTEXT      *pcontextHDS);

DRM_RESULT DRM_API DRM_MTR_OpenId(
    IN       DRM_METERING_CONTEXT *pcontextMetering,
    IN const DRM_MID              *pmid,
    IN const DRM_KID              *pkid,
    IN const DRM_BYTE              rgbPassword [__CB_DECL(SHA_DIGEST_LEN)],
    IN       DRM_HDS_CONTEXT      *pcontextHDS );

DRM_RESULT DRM_API DRM_MTR_IncrementCount( 
    IN       DRM_METERING_CONTEXT *pcontextMetering, 
    IN const DRM_CONST_STRING     *pdstrAction );

DRM_RESULT DRM_API DRM_MTR_CreateMeterStorePassword(
    IN     DRM_CONST_STRING *f_pdstrMID,
       OUT DRM_BYTE          f_rgbPasswordSST [__CB_DECL(SHA_DIGEST_LEN)],
    IN     DRM_BYTE         *f_pbcontextBBX);

DRM_RESULT DRM_API DRM_MTR_UpdateData(
    DRM_MID                 *f_pmid,
    DRM_LID                 *f_plid,
    DRM_KID                 *f_pkid,
    const DRM_CONST_STRING **f_ppdstrActions,
    DRM_DWORD                f_cActions,
    DRM_METERING_CONTEXT    *f_pcontextMTR,
    DRM_HDS_CONTEXT         *f_pcontextHDS,
    IN     DRM_BYTE         *f_pbcontextBBX);

DRM_RESULT DRM_API DRM_MTR_EnumLoadCurrent ( 
    IN       DRM_METERINGENUM_CONTEXT *f_pcontextMTREnum,
    IN       DRM_METERING_CONTEXT     *f_pcontextMTR,
    IN const DRM_BYTE                  f_rgbPasswordMTR[__CB_DECL(SHA_DIGEST_LEN)],
       OUT   DRM_KID                  *f_pkid,
       OUT   DRM_DWORD                *f_pcbData);

DRM_RESULT DRM_API DRM_MTR_EnumDeleteCurrent( 
    IN       DRM_METERINGENUM_CONTEXT *f_pcontextMTREnum,
    IN       DRM_METERING_CONTEXT     *f_pcontextMTR);
#ifdef __cplusplus
}
#endif

#endif /* __DRMMETERING_H__ */

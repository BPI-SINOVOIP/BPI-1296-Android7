/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMMETERIMPL_H__
#define __DRMMETERIMPL_H__

#include <drmmetering.h>
#include <drmxmlbuilder.h>
#include <drmutilities.h>

ENTER_PK_NAMESPACE;

#define DRM_METERING_CONTEXT        DRM_SECSTORE_CONTEXT

typedef struct __tagDRM_METERINGENUM_CONTEXT
{
    DRM_MID                     oMID;
    DRM_SECSTOREENUM_CONTEXT    oSecStoreEnum;
} DRM_METERINGENUM_CONTEXT;

#define DRM_METER_RESPONSE_PARTIAL 0x00000001

extern DRM_EXPORT_VAR const DRM_KID g_kidTID;

extern DRM_EXPORT_VAR const DRM_KID g_kidTIDPrev;

DRM_RESULT DRM_MTR_GetKIDData (IN  DRM_METERING_CONTEXT        *f_pmeterstore,
                               IN  DRM_STACK_ALLOCATOR_CONTEXT *f_pstack,
                               OUT DRM_BYTE                   **f_ppbDataOut,
                               OUT DRM_DWORD                   *f_pcbDataOut);

DRM_RESULT DRM_MTR_CreateNewTID (DRM_VOID *f_pOEMContext, DRM_TID *f_ptid);

DRM_RESULT DRM_NO_INLINE DRM_MTR_ReadTIDFromMeteringStore (
          DRM_METERING_CONTEXT   *f_pmeterstore,
    const DRM_BYTE                f_rgbPasswordSST [__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    const DRM_MID                *f_pmid,
          DRM_TID                *f_ptid,
          DRM_CONST_STRING       *f_pdstrTidBase64,
          IN DRM_DST             *f_pDatastore );

DRM_RESULT DRM_NO_INLINE DRM_MTR_WriteTIDToMeteringStore (
    IN        DRM_METERING_CONTEXT *f_pmeterstore,
    IN  const DRM_BYTE              f_rgbPasswordSST [__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN  const DRM_MID              *f_pmid,
    IN        DRM_TID              *f_ptid,
    IN        DRM_DST             *f_pDatastore );

DRM_API DRM_RESULT DRM_CALL DRM_MTR_GetAllData (
    IN     DRM_METERING_CONTEXT *f_pcontextMTR,
    __out_bcount_opt( *f_pcbData ) DRM_BYTE *f_pbData,
    IN OUT DRM_DWORD            *f_pcbData);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_InitEnum (
    IN const DRM_MID                   *pmid,
       OUT   DRM_METERINGENUM_CONTEXT  *pcontextMeteringEnum,
    IN       DRM_DST                   *pDataStore);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_EnumNext (
    IN     DRM_METERINGENUM_CONTEXT *pcontextMeteringEnum,
       OUT DRM_KID                  *pkid,
       OUT DRM_DWORD                *pcbData);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_GenerateMeterChallenge(
    IN                           DRM_LICEVAL_CONTEXT *f_pcontextLEVL,
    __in_bcount( f_cbStack )     DRM_BYTE            *f_pbStack,
    IN                           DRM_DWORD            f_cbStack,
    IN const                     DRM_CONST_STRING    *f_pdstrMeterCert,
       OUT                       _XMBContext         *f_pChallenge,
    IN OUT                       DRM_DWORD           *f_pcbChallenge,
     __out_ecount_opt(*f_pcchURL)DRM_CHAR            *f_pszURL,
    IN OUT                       DRM_DWORD           *f_pcchURL);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_ProcessMeterResponse(
    IN     DRM_LICEVAL_CONTEXT *f_pcontextLEVL,
    __in_bcount( f_cbStack ) DRM_BYTE *f_pbStack,
    IN     DRM_DWORD            f_cbStack,
    __in_bcount( f_cbData ) DRM_BYTE *f_pbData,
    IN     DRM_DWORD            f_cbData,
       OUT DRM_DWORD           *f_pfFlags);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_CloseContext(
    IN DRM_METERING_CONTEXT *pcontextMetering,
    IN DRM_DST              *pDataStore);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_OpenId(
    IN       DRM_METERING_CONTEXT *pcontextMetering,
    IN const DRM_MID              *pmid,
    IN const DRM_KID              *pkid,
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE rgbPassword [ __CB_DECL( DRM_SHA1_DIGEST_LEN ) ],
    IN DRM_DST                    *pDataStore );

DRM_API DRM_RESULT DRM_CALL DRM_MTR_IncrementCount(
    IN       DRM_METERING_CONTEXT *pcontextMetering,
    IN const DRM_CONST_STRING     *pdstrAction );

DRM_API DRM_RESULT DRM_CALL DRM_MTR_CreateMeterStorePassword(
    IN     DRM_CONST_STRING *f_pdstrMID,
    __out_bcount( DRM_SHA1_DIGEST_LEN ) DRM_BYTE f_rgbPasswordSST[ __CB_DECL( DRM_SHA1_DIGEST_LEN ) ],
    IN     DRM_BB_CONTEXT   *f_pbcontextBBX);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_CreateMeterStorePasswordFromBinaryMID(
    IN const DRM_MID           *f_pmid,
    __out_bcount( DRM_SHA1_DIGEST_LEN ) DRM_BYTE f_rgbPasswordSST[ __CB_DECL( DRM_SHA1_DIGEST_LEN ) ],
    IN       DRM_BB_CONTEXT    *f_pbcontextBBX);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_UpdateData(
    DRM_MID                 *f_pmid,
    DRM_LID                 *f_plid,
    DRM_KID                 *f_pkid,
    const DRM_CONST_STRING **f_ppdstrActions,
    DRM_DWORD                f_cActions,
    DRM_METERING_CONTEXT    *f_pcontextMTR,
    DRM_DST                 *pDataStore,
    __in  DRM_BB_CONTEXT    *f_pbcontextBBX);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_EnumLoadCurrent (
    IN       DRM_METERINGENUM_CONTEXT *f_pcontextMTREnum,
    IN       DRM_METERING_CONTEXT     *f_pcontextMTR,
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE f_rgbPasswordMTR[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
       OUT   DRM_KID                  *f_pkid,
       OUT   DRM_DWORD                *f_pcbData);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_EnumDeleteCurrent(
    IN       DRM_METERINGENUM_CONTEXT *f_pcontextMTREnum,
    IN       DRM_METERING_CONTEXT     *f_pcontextMTR);

DRM_API DRM_RESULT DRM_CALL DRM_MTR_ParseMeterCert(
    IN const DRM_CONST_STRING    *f_pdstrMeterCert,
    IN       DRM_LICEVAL_CONTEXT *f_pcontextLEVL,
       OUT   PUBKEY              *f_ppubkeyMAS,
       OUT   DRM_CONST_STRING    *f_pdstrDataMID,
       OUT   DRM_MID             *f_pmid,
       OUT   DRM_CONST_STRING    *f_pdstrDataURL);

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_MTR_ParseBinaryMeterCert(
    __inout                            DRM_BB_CONTEXT               *f_poBBXContext,
    __inout                            DRM_DST                      *f_poDSTContext,
    __inout                            DRM_SECSTORE_CONTEXT         *f_poSSTContext,
    __inout_opt                        DRM_REVOCATIONSTORE_CONTEXT  *f_poRevStore,
    __in_bcount_opt(f_cbRevocationBuffer)
                                       DRM_BYTE                     *f_pbRevocationBuffer,
    __in                               DRM_DWORD                     f_cbRevocationBuffer,
    __in_bcount(f_cbMeterCert)   const DRM_BYTE                     *f_pbMeterCert,
    __in                         const DRM_DWORD                     f_cbMeterCert,
    __out                              DRM_MID                      *f_poMID,
    __out_ecount_opt(*f_pcchURL)       DRM_CHAR                     *f_pchURL,
    __inout_opt                        DRM_DWORD                    *f_pcchURL,
    __out                              PUBKEY_P256                  *f_poServerPubKey );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_MTR_GenerateMeterDataChallenge(
    __inout                                DRM_BB_CONTEXT *f_poBBXContext,
    __inout                                DRM_DST        *f_poDSTContext,
    __inout                                DRM_SECSTORE_CONTEXT *f_poSSTContext,
    __inout_opt                            DRM_REVOCATIONSTORE_CONTEXT *f_poRevStore,
    __in_bcount_opt(f_cbRevocationBuffer)  DRM_BYTE       *f_pbRevocationBuffer,
    __in                                   DRM_DWORD       f_cbRevocationBuffer,
    __in_bcount(f_cbStack)                 DRM_BYTE       *f_pbStack,
    __in                                   DRM_DWORD       f_cbStack,
    __in_bcount(f_cbMeterCert)       const DRM_BYTE       *f_pbMeterCert,
    __in                                   DRM_DWORD       f_cbMeterCert,
    __in_bcount(f_cbDeviceCert)      const DRM_BYTE       *f_pbDeviceCert,
    __in                                   DRM_DWORD       f_cbDeviceCert,
    __in_ecount_opt(f_cchCustomData) const DRM_CHAR       *f_pchCustomData,
    __in                                   DRM_DWORD       f_cchCustomData,
    __out_ecount_opt(*f_pcchURL)           DRM_CHAR       *f_pchURL,
    __inout                                DRM_DWORD      *f_pcchURL,
    __out_bcount_opt( *f_pcbChallenge )    DRM_BYTE       *f_pbChallenge,
    __inout                                DRM_DWORD      *f_pcbChallenge );

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_MTR_ProcessMeterDataResponse(
    __in                                    DRM_BB_CONTEXT              *f_poBBXContext,
    __in                                    DRM_DST                     *f_poDSTContext,
    __inout                                 DRM_SECSTORE_CONTEXT        *f_poSSTContext,
    __inout_opt                             DRM_REVOCATIONSTORE_CONTEXT *f_poRevStore,
    __in_bcount_opt(f_cbRevocationBuffer)   DRM_BYTE                    *f_pbRevocationBuffer,
    __in                                    DRM_DWORD                    f_cbRevocationBuffer,
    __inout_bcount(*f_pcbScratch)           DRM_BYTE                    *f_pbScratch,
    __inout                                 DRM_DWORD                   *f_pcbScratch,
    __inout_bcount(f_cbResponse)            DRM_BYTE                    *f_pbResponse,
    __in                                    DRM_DWORD                    f_cbResponse,
    __inout                                 DRM_BOOL                    *f_pfResponseSignatureChecked,
    __inout                                 DRM_DWORD                   *f_pdwProtocol,
    __out_opt                               DRM_RESULT                  *f_pResult );

DRM_API DRM_RESULT DRM_CALL DRM_MTR_GenerateMeterCertChallenge(
    __in                                    DRM_BB_CONTEXT  *f_poBBXContext,
    __in                                    DRM_MID         *f_poMID,
    __in_ecount_opt(f_cchCustomData)  const DRM_CHAR        *f_pchCustomData,
    __in                                    DRM_DWORD        f_cchCustomData,
    __out_bcount_opt(*f_pcbChallenge)       DRM_BYTE        *f_pbChallenge,
    __inout                                 DRM_DWORD       *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL DRM_MTR_ProcessMeterCertResponse(
    __in                       DRM_LICEVAL_CONTEXT   *f_poLicEvalContext,
    __in                       DRM_METERCERT_CONTEXT *f_poMeterCertStoreContext,
    __in_bcount(*f_pcbScratch) DRM_BYTE              *f_pbScratch,
    __inout                    DRM_DWORD             *f_pcbScratch,
    __in_bcount(f_cbResponse)  DRM_BYTE              *f_pbResponse,
    __in                       DRM_DWORD              f_cbResponse,
    __out_opt                  DRM_RESULT            *f_pResult );

EXIT_PK_NAMESPACE;

#endif /* __DRMMETERIMPL_H__ */


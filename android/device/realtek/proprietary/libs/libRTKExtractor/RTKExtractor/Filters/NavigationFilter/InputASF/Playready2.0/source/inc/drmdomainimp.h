/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_DOMAIN_IMP_H
#define __DRM_DOMAIN_IMP_H

#include <drmlicevaltypes.h>
#include <drmxmlbuildera.h>
#include <drmdomainstoretypes.h>
#include <drmdomainkeyxmrparser.h>
#include <drmblackbox.h>

ENTER_PK_NAMESPACE;

/* 
** The following constants define the types of custom data that is sent to
** both the domain join and domain leave challenge.
** Any custom data should belong to one and only one of the following types.
**
** NULL custom data.
*/
#define DRM_REGISTER_NULL_DATA            0

/*
** XML segment that contains only the data.
*/
#define DRM_REGISTER_CUSTOM_DATA          1

/*
** XML segment that contains both the DID and the data.
*/
#define DRM_REGISTER_INDIRECT_QUERY_DATA  2

typedef DRM_DOMAINSTORE_ENUM_CONTEXT DRM_DOMAIN_CERT_ENUM_CONTEXT;

/*
** List of license response processing flags
*/
typedef enum _DRM_PROCESS_DJ_RESPONSE_FLAG
{
    DRM_PROCESS_DJ_RESPONSE_NO_FLAGS                  = 0x00000000L,      /* Opt-in signature verification (default). */
    DRM_PROCESS_DJ_RESPONSE_SIGNATURE_NOT_REQUIRED    = 0x00000001L,      /* Signature is not required, i.e. Opt-out signature verification. */
} DRM_PROCESS_DJ_RESPONSE_FLAG;

/*
** ---------------------------------------
** Domain implementation API functions
** ---------------------------------------
*/
DRM_API DRM_RESULT DRM_CALL DRM_DOM_GenerateJoinChallenge(
    IN DRM_VOID *f_poOEMContext,
    IN DRM_BB_CONTEXT *f_poBBXContext,
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_DWORD f_dwFlags,
    IN DRM_DOMAIN_ID *f_poDomainID,
    __in_ecount_opt( f_cchFriendlyName ) const DRM_CHAR *f_pchFriendlyName,
    IN DRM_DWORD f_cchFriendlyName,
    __in_bcount_opt( f_cbDeviceCert ) const DRM_BYTE *f_pbDeviceCert,
    IN DRM_DWORD f_cbDeviceCert,
    __in_ecount_opt( f_cchData) const DRM_CHAR *f_pchData,
    IN DRM_DWORD f_cchData,
    __out_bcount_opt( *f_pcbChallenge ) DRM_BYTE *f_pbChallenge,
    IN OUT DRM_DWORD *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL DRM_DOM_ProcessJoinResponse(
    IN DRM_BB_CONTEXT *f_poBBXContext,
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    __inout_bcount( f_cbResponse ) DRM_BYTE *f_pbResponse,
    IN DRM_DWORD f_cbResponse,
    OUT DRM_RESULT *f_pResult,
    OUT DRM_DOMAIN_ID *f_poDomainID );

DRM_API DRM_RESULT DRM_CALL DRM_DOM_GenerateLeaveChallenge(
    IN DRM_VOID *f_poOEMContext,
    IN DRM_BB_CONTEXT *f_poBBXContext,
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_DWORD f_dwFlags,
    IN DRM_DOMAIN_ID *f_poDomainID,
    __in_bcount_opt( f_cbDeviceCert ) const DRM_BYTE *f_pbDeviceCert,
    IN DRM_DWORD f_cbDeviceCert,
    __in_ecount_opt( f_cchData) const DRM_CHAR *f_pchData,
    IN DRM_DWORD f_cchData,
    __out_bcount_opt( *f_pcbChallenge ) DRM_BYTE *f_pbChallenge,
    IN OUT DRM_DWORD *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL DRM_DOM_ProcessLeaveResponse(
    __in_bcount( f_cbResponse ) DRM_BYTE *f_pbResponse,
    IN DRM_DWORD f_cbResponse,
    OUT DRM_RESULT *f_pResult,
    IN DRM_BB_CONTEXT *f_pBBXCtx );

DRM_API DRM_RESULT DRM_CALL DRM_DOM_FindCert(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_DOMAIN_ID *f_poDomainID,
    __out_bcount_opt( *f_pcbDomainCert ) DRM_BYTE *f_pbDomainCert,
    IN OUT DRM_DWORD *f_pcbDomainCert );

DRM_API DRM_RESULT DRM_CALL DRM_DOM_InitCertEnum(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    OUT DRM_DOMAIN_CERT_ENUM_CONTEXT *f_poDomainCertEnumContext );

DRM_API DRM_RESULT DRM_CALL DRM_DOM_EnumNextCert(
    IN DRM_DOMAIN_CERT_ENUM_CONTEXT *f_poDomainCertEnumContext,
    OUT DRM_DWORD *f_pcchDomainCert,
    OUT DRM_DOMAINCERT_INFO *f_poDomainCertInfo );

DRM_NO_INLINE DRM_API DRM_RESULT DRM_CALL DRM_DOM_IsLicenseValidForELS(
    __in    DRM_DOMAINSTORE_CONTEXT     *f_pDomainStore,
    __inout DRM_XMR_LICENSE             *f_pXMRLicense,
    __inout DRM_BOOL                    *f_pfValidForELS ) DRM_NO_INLINE_ATTRIBUTE;

DRM_NO_INLINE DRM_API DRM_RESULT DRM_CALL DRM_DOM_LA_CalcDomainCertsCharCount(
    __in  DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    __in  DRM_GUID                *f_poServiceID,
    __out DRM_DWORD               *f_pcchDomainCerts ) DRM_NO_INLINE_ATTRIBUTE;

DRM_NO_INLINE DRM_API DRM_RESULT DRM_CALL DRM_DOM_LA_BuildDomainCertsXML(
    __in    DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    __in    DRM_GUID                *f_poServiceID,
    __inout _XMBContextA            *f_poXMLContext ) DRM_NO_INLINE_ATTRIBUTE;

DRM_NO_INLINE DRM_API DRM_RESULT DRM_CALL DRM_DOM_ARCLK_ClockRollbackDetected(
    IN  DRM_BB_CONTEXT          *f_poBBXContext,
    IN  DRM_UINT64               ui64GlobalSavedTime ) DRM_NO_INLINE_ATTRIBUTE;

DRM_NO_INLINE DRM_API DRM_RESULT DRM_CALL DRM_DOM_ARCLK_LEVL_DisableOnRollback(
    __inout DRM_LICEVAL_CONTEXT *f_pContextLicEval,
    __in    DRM_XMR_LICENSE     *f_pXMR,
    __in    DRM_UINT64           f_u64MachineDateTime,
    __out   DRM_BOOL            *f_pfDisabled ) DRM_NO_INLINE_ATTRIBUTE;

DRM_NO_INLINE DRM_API DRM_RESULT DRM_CALL DRM_DOM_ARCLK_CreateDomainStorePassword(
    __in                          const DRM_GUID       *f_poAccountID,
    __in                                DRM_BB_CONTEXT *f_poBBXContext,
    __out_bcount( DRM_SHA1_DIGEST_LEN ) DRM_BYTE        f_rgbPasswordSST[__CB_DECL(DRM_SHA1_DIGEST_LEN)] ) DRM_NO_INLINE_ATTRIBUTE;

DRM_NO_INLINE DRM_API DRM_RESULT DRM_CALL DRM_DOM_ARCLK_GetDomainSavedDateTime(
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE                 f_rgbPasswordSST[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    __inout_opt                              DRM_VOID                *f_pOEMContext,
    __in                                     DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    __in                               const DRM_GUID                *f_poAccountID,
    __out                                    DRM_UINT64              *f_puiDomainSavedDateTime ) DRM_NO_INLINE_ATTRIBUTE;

EXIT_PK_NAMESPACE;

#endif /* __DRM_DOMAIN_IMP_H */


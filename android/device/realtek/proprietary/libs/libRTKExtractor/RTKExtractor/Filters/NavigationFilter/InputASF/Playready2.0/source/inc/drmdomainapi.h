/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_DOMAIN_API_H
#define __DRM_DOMAIN_API_H

#include <drmdomainimp.h>
#include <drmmanagertypes.h>

ENTER_PK_NAMESPACE;

DRM_NO_INLINE DRM_API DRM_BOOL DRM_CALL DRM_DOMAIN_IsDomainSupported(DRM_VOID) DRM_NO_INLINE_ATTRIBUTE;
DRM_API DRM_BOOL DRM_CALL DRM_DOMAIN_IsDomainUnsupported(DRM_VOID);

/*
** ---------------------------------------
** Domain API functions
** ---------------------------------------
*/
DRM_API DRM_RESULT DRM_CALL Drm_JoinDomain_GenerateChallenge(
    IN                                         DRM_APP_CONTEXT *f_poAppContext,
    IN                                         DRM_DWORD        f_dwFlags,
    IN                                         DRM_DOMAIN_ID   *f_poDomainID,
    __in_ecount_opt( f_cchFriendlyName ) const DRM_CHAR        *f_pchFriendlyName,
    IN                                         DRM_DWORD        f_cchFriendlyName,
    __in_ecount_opt( f_cchData )         const DRM_CHAR        *f_pchData,
    IN                                         DRM_DWORD        f_cchData,
    __out_bcount_opt( *f_pcbChallenge )        DRM_BYTE        *f_pbChallenge,
    IN OUT                                     DRM_DWORD       *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL Drm_JoinDomain_ProcessResponse(
    IN DRM_APP_CONTEXT *f_poAppContext,
    IN DRM_PROCESS_DJ_RESPONSE_FLAG f_dwFlags,
    __in_bcount( f_cbResponse ) DRM_BYTE *f_pbResponse,
    IN DRM_DWORD f_cbResponse,
    OUT DRM_RESULT *f_pResult,
    OUT DRM_DOMAIN_ID *f_poDomainID );

DRM_API DRM_RESULT DRM_CALL Drm_LeaveDomain_GenerateChallenge(
    IN                                        DRM_APP_CONTEXT *f_poAppContext,
    IN                                        DRM_DWORD        f_dwFlags,
    IN                                        DRM_DOMAIN_ID   *f_poDomainID,
    __in_ecount_opt( f_cchData )        const DRM_CHAR        *f_pchData,
    IN                                        DRM_DWORD        f_cchData,
    __out_bcount_opt( *f_pcbChallenge )       DRM_BYTE        *f_pbChallenge,
    IN OUT                                    DRM_DWORD       *f_pcbChallenge );

DRM_API DRM_RESULT DRM_CALL Drm_LeaveDomain_ProcessResponse(
    IN DRM_APP_CONTEXT *f_poAppContext,
    __in_bcount( f_cbResponse ) DRM_BYTE *f_pbResponse,
    IN DRM_DWORD f_cbResponse,
    OUT DRM_RESULT *f_pResult );

DRM_API DRM_RESULT DRM_CALL Drm_DomainCert_Find(
    IN DRM_APP_CONTEXT *f_poAppContext,
    IN DRM_DOMAIN_ID *f_poDomainID,
    __out_bcount_opt( *f_pcbDomainCert ) DRM_BYTE *f_pbDomainCert,
    IN OUT DRM_DWORD *f_pcbDomainCert );

DRM_API DRM_RESULT DRM_CALL Drm_DomainCert_InitEnum(
    IN DRM_APP_CONTEXT *f_poAppContext,
    OUT DRM_DOMAIN_CERT_ENUM_CONTEXT *f_poDomainCertEnumContext );

DRM_API DRM_RESULT DRM_CALL Drm_DomainCert_EnumNext(
    IN DRM_DOMAIN_CERT_ENUM_CONTEXT *f_poDomainCertEnumContext,
    OUT DRM_DWORD *f_pcchDomainCert,
    OUT DRM_DOMAINCERT_INFO *f_poDomainCertInfo );

EXIT_PK_NAMESPACE;

#endif /* __DRM_DOMAIN_API_H */


/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_DOMAIN_IMP_H
#define __DRM_DOMAIN_IMP_H

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

/*
** Structure that encapsulates the information returned by
** domain cert enumeration APIs.
*/
typedef struct __tagDRM_DOMAINCERT_INFO
{
    /* Domain ID. */
    DRM_DOMAIN_ID    m_oDomainID;

    /* Point to a URL in the domain cert. (not used at the moment) */
    DRM_ANSI_STRING  m_dastrURL;

} DRM_DOMAINCERT_INFO;

typedef DRM_DOMAINSTORE_ENUM_CONTEXT DRM_DOMAIN_CERT_ENUM_CONTEXT;

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


EXIT_PK_NAMESPACE;

#endif /* __DRM_DOMAIN_IMP_H */

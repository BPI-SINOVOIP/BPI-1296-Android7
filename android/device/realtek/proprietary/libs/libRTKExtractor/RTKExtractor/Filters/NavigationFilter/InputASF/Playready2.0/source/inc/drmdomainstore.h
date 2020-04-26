/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_DOMAIN_STORE_H
#define __DRM_DOMAIN_STORE_H

#include <drmdomainstoretypes.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL DRM_DOMST_OpenStore(
    IN  DRM_DST                 *f_pDatastore,
    OUT DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_CloseStore(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_AddCert(
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE                 f_rgbPasswordSST[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    __inout_opt                              DRM_VOID                *f_pOEMContext,
    IN                                       DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN                                       DRM_GUID                *f_poAccountID,
    IN                                       DRM_GUID                *f_poServiceID,
    __in_bcount( f_cbCert )            const DRM_BYTE                *f_pbCert,
    IN                                       DRM_DWORD                f_cbCert );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_GetCert(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_GUID *f_poAccountID,
    OUT DRM_GUID *f_poServiceID,
    __out_bcount_opt( *f_pcbCert ) DRM_BYTE *f_pbCert,
    IN OUT DRM_DWORD *f_pcbCert );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_DeleteCert(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_GUID *f_poAccountID );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_InitEnumCert(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    OUT DRM_DOMAINSTORE_ENUM_CONTEXT *f_poDomainStoreEnumContext );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_EnumNextCert(
    IN DRM_DOMAINSTORE_ENUM_CONTEXT *f_poDomainStoreEnumContext,
    OUT DRM_GUID *f_poAccountID,
    OUT DRM_GUID *f_poServiceID,
    OUT DRM_DST_SLOT_HINT *f_poSlotHint,
    OUT DRM_DWORD *f_pcbData );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_AddKey(
    IN                           DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN                           DRM_GUID                *f_poAccountID,
    IN                           DRM_DWORD                f_dwRevision,
    __in_bcount( f_cbKey ) const DRM_BYTE                *f_pbKey,
    IN                           DRM_DWORD                f_cbKey );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_GetKey(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_GUID *f_poAccountID,
    IN DRM_DWORD f_dwRevision,
    __out_bcount_opt( *f_pcbKey ) DRM_BYTE *f_pbKey,
    IN OUT DRM_DWORD *f_pcbKey );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_DeleteKey(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_GUID *f_poAccountID,
    IN DRM_DWORD f_dwRevision );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_DeleteKeys(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_GUID *f_poAccountID );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_InitEnumKey(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    OUT DRM_DOMAINSTORE_ENUM_CONTEXT *f_poDomainStoreEnumContext );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_EnumNextKey(
    IN DRM_DOMAINSTORE_ENUM_CONTEXT *f_poDomainStoreEnumContext,
    OUT DRM_GUID *f_poAccountID,
    OUT DRM_DWORD *f_pdwRevision,
    OUT DRM_DST_SLOT_HINT *f_poSlotHint,
    OUT DRM_DWORD *f_pcbData );

DRM_API DRM_RESULT DRM_CALL DRM_DOMST_CreateDomainStorePassword(
    IN  const DRM_GUID          *f_poAccountID,
    IN  DRM_BB_CONTEXT          *f_poBBXContext,
    __out_bcount( DRM_SHA1_DIGEST_LEN ) DRM_BYTE f_rgbPasswordSST[__CB_DECL(DRM_SHA1_DIGEST_LEN)] );

EXIT_PK_NAMESPACE;

#endif /* __DRM_DOMAIN_STORE_H */

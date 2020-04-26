/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_DOMAIN_STORE_H
#define __DRM_DOMAIN_STORE_H

#include <drmdatastore.h>

ENTER_PK_NAMESPACE;

/* The context of the domain store. */
typedef struct _tagDomainStoreContextPrivate
{
    /* Pointer to the underlying DST. */
    DRM_DST *m_poDST;

    /* The underlying DST namespace context. */
    DRM_DST_NAMESPACE_CONTEXT m_oDSTNameSpaceContext;

    /* The underlying DST slot context. */
    DRM_DST_SLOT_CONTEXT m_oDSTSlotContext;

    /* Flag indicating whether the context has been initialized. */
    DRM_BOOL m_fInited;
} DRM_DOMAINSTORE_CONTEXT_PRIVATE;

/* The context of the domain store in the form of a BLOB. */
typedef struct __tagDRM_DOMAINSTORE_CONTEXT
{
    DRM_BYTE rgbOpaqueData [ __CB_DECL( SIZEOF( DRM_DOMAINSTORE_CONTEXT_PRIVATE ) ) ];
} DRM_DOMAINSTORE_CONTEXT;

/* The enumeration context of the domain store. */
typedef struct __tagDRM_DOMAINSTORE_ENUM_CONTEXT
{
    /* The underlying domain store. */
    DRM_DST *m_poDomainStore;

    /* The underlying DST enumeration context. */
    DRM_DST_ENUM_CONTEXT m_oDSTEnumContext;

    /* The underlying DST slot context. */
    DRM_DST_SLOT_CONTEXT m_oDSTSlotContext;

    /*
    ** Flag indicating whether domain certificates
    ** or domain private keys to be enumerated.
    */
    DRM_BOOL m_fCert;

    /* Flag indicating whether the context has been initialized. */
    DRM_BOOL m_fInited;
} DRM_DOMAINSTORE_ENUM_CONTEXT;

DRM_API DRM_RESULT DRM_CALL DRM_DOMST_OpenStore(
    IN  DRM_DST                 *f_pDatastore,
    OUT DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_CloseStore(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext );


DRM_API DRM_RESULT DRM_CALL DRM_DOMST_AddCert(
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_GUID *f_poAccountID,
    IN DRM_GUID *f_poServiceID,
    __in_bcount( f_cbCert ) DRM_BYTE *f_pbCert,
    IN DRM_DWORD f_cbCert );


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
    IN DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    IN DRM_GUID *f_poAccountID,
    IN DRM_DWORD f_dwRevision,
    __in_bcount( f_cbKey ) DRM_BYTE *f_pbKey,
    IN DRM_DWORD f_cbKey );


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


EXIT_PK_NAMESPACE;

#endif /* __DRM_DOMAIN_STORE_H */

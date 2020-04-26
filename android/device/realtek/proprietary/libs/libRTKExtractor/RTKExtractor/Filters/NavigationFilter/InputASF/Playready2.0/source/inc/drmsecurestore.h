/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMSECURESTORE_H__
#define __DRMSECURESTORE_H__

#include <drmsecurestoretypes.h>
#include <drmblackbox.h>
#include <drmcallbacks.h>

ENTER_PK_NAMESPACE;

typedef enum _tag_EnumModeSST
{
    eSSTEnumNone=0,       /* no secure store entries found */    
    eSSTEnumFiltered,     /* enum for secure store entries matching given ID */
    eSSTEnumNatural       /* enum all secure store entries */
} _EnumModeSST;

typedef struct _tag_DRM_SECSTOREENUM_CONTEXT
{
    DRM_DST_ENUM_CONTEXT      oDstEnumContext;
    DRM_DST_NAMESPACE_CONTEXT oNsContext;
    DRM_DST_SLOT_CONTEXT      oSlotContext;
    DRM_BOOL                  fInited;
    _EnumModeSST              eMode;
    eDRM_SECURE_STORE_TYPE    eType;
    DRM_BOOL                  fCurrIsValid;
    DRM_KID                   oCurrKID;
    DRM_LID                   oCurrLID;
    DRM_DST                  *pDatastore;
} DRM_SECSTOREENUM_CONTEXT;


/* Flags for DRM_SST_OpenKeyTokens */
#define DRM_SECURE_STORE_CREATE_NEW             0x1
#define DRM_SECURE_STORE_OPEN_ALWAYS            0x2
#define DRM_SECURE_STORE_OPEN_EXISTING          0x4

#define DRM_SECURE_STORE_VALID_FLAGS    (DRM_SECURE_STORE_CREATE_NEW | DRM_SECURE_STORE_OPEN_ALWAYS | DRM_SECURE_STORE_OPEN_EXISTING)

DRM_RESULT DRM_SST_LoadAttributesIntoCache(
    IN const DRM_BYTE        *pbSlotData,
    IN       DRM_DWORD        cbSlotData,
       OUT   CachedAttribute  rgAttributes[DRM_MAX_ATTRIBUTES_PER_SST_KEY],
       OUT   DRM_WORD        *pwNumAttributes);

DRM_API DRM_RESULT DRM_CALL DRM_SST_GetTokenValueFromCache(
    IN const DRM_CONST_STRING     *pdstrAttribute,
    IN  CachedAttribute            rgAttributes[DRM_MAX_ATTRIBUTES_PER_SST_KEY], 
    IN  DRM_WORD                   wNumAttributes,
       OUT   TOKEN                *pToken );

DRM_API DRM_RESULT DRM_CALL DRM_SST_LoadKeyTokens(
    IN OUT   DRM_SECSTORE_CONTEXT   *pcontextSST,
    IN       DRM_DWORD               dwFlags,
    IN       DRM_DST                *pDatastore );

DRM_API DRM_RESULT DRM_CALL DRM_SST_OpenKeyTokens(
    IN OUT   DRM_SECSTORE_CONTEXT   *pbSecureStoreContext,
    IN const DRM_ID                 *pKey1,
    IN const DRM_ID                 *pKey2,
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE rgbPassword[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN       DRM_DWORD               dwFlags,
    IN       eDRM_SECURE_STORE_TYPE  eType,
    IN       DRM_DST                *pDatastore);

DRM_API DRM_RESULT DRM_CALL DRM_SST_GetTokenValue(
    IN       DRM_SECSTORE_CONTEXT *pbSecureStoreContext,
    IN const DRM_CONST_STRING     *pdstrAttribute,
       OUT   TOKEN                *pToken );

DRM_API DRM_RESULT DRM_CALL DRM_SST_SetTokenValue(
    IN       DRM_SECSTORE_CONTEXT *pbSecureStoreContext,
    IN const DRM_CONST_STRING     *pdstrAttribute,
    IN const TOKEN                *pToken );

DRM_API DRM_RESULT DRM_CALL DRM_SST_SetExplicitResolutionTokenValue(
    IN       DRM_SECSTORE_CONTEXT *pcontextSST,
    IN const DRM_CONST_STRING     *pdstrAttribute,
    IN const TOKEN                *pToken,
    IN       DRM_DWORD             dwFlags );

DRM_API DRM_RESULT DRM_CALL DRM_SST_CloseKey( 
    IN       DRM_SECSTORE_CONTEXT *pcontextSST, 
    IN       DRM_DST              *pDatastore,
    IN       DRMPFNPOLICYCALLBACK  pfnMergeCallback,
    IN       const DRM_VOID       *pvCallbackData );

DRM_API DRM_RESULT DRM_CALL DRM_SST_OpenAndLockSlot(
    IN          DRM_DST                *f_pDatastore,
    IN          eDRM_SECURE_STORE_TYPE  f_eType,
    IN  const   DRM_ID                 *f_pKey1,
    IN  const   DRM_ID                 *f_pKey2,
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE f_rgbPassword[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN          DRM_DWORD               f_dwFlags,
        OUT     DRM_SECSTORE_CONTEXT   *f_pcontextSST,
        OUT     DRM_DWORD              *f_pcbData );

DRM_API DRM_RESULT DRM_CALL DRM_SST_GetLockedData(
    IN       DRM_SECSTORE_CONTEXT   *f_pcontextSST,
    __out_bcount_opt( *f_pcbData ) DRM_BYTE *f_pbData,
       OUT   DRM_DWORD              *f_pcbData );

DRM_API DRM_RESULT DRM_CALL DRM_SST_SetLockedData(
    IN          DRM_SECSTORE_CONTEXT   *f_pcontextSST,
    IN          DRM_DWORD               f_cbData,
    __in_bcount( f_cbData ) const DRM_BYTE *f_pbData );

DRM_API DRM_RESULT DRM_CALL DRM_SST_CloseLockedSlot( IN  DRM_SECSTORE_CONTEXT *f_pcontextSST );

DRM_API DRM_RESULT DRM_CALL DRM_SST_GetData(
    IN       DRM_SECSTORE_CONTEXT   *f_pcontextSST,
    IN const DRM_ID                 *f_pKey1,
    IN const DRM_ID                 *f_pKey2,
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE f_rgbPassword[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN       eDRM_SECURE_STORE_TYPE  f_eType,
    IN       DRM_DST                *f_pDatastore,
    __out_bcount_opt( *f_pcbData ) DRM_BYTE *f_pbData,
       OUT   DRM_DWORD              *f_pcbData );

DRM_API DRM_RESULT DRM_CALL DRM_SST_SetData(
    IN       DRM_SECSTORE_CONTEXT   *f_pcontextSST,
    IN const DRM_ID                 *f_pKey1,
    IN const DRM_ID                 *f_pKey2,
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE f_rgbPassword[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN       eDRM_SECURE_STORE_TYPE  f_eType,
    IN       DRM_DST                *f_pDatastore,
    __in_bcount( f_cbData ) const DRM_BYTE *f_pbData,
    IN       DRM_DWORD               f_cbData );


/* Delete an entry from the secure store.  Be careful when this happens -- it could open up the system for 
   replay attacks */
DRM_API DRM_RESULT DRM_CALL DRM_SST_DeleteKey(
    IN       DRM_SECSTORE_CONTEXT   *pbSecureStoreContext,
    IN       eDRM_SECURE_STORE_TYPE  eType,
    IN const DRM_ID                 *pid1,
    IN const DRM_ID                 *pid2,
    IN       DRM_DST                *pDatastore);


DRM_API DRM_RESULT DRM_CALL DRM_SST_GetAllData( 
    IN     DRM_SECSTORE_CONTEXT *pbSecureStoreContext,
    __out_bcount_opt( *pcbData ) DRM_BYTE *pbData,
    IN OUT DRM_DWORD            *pcbData );

/* The enumeration APIs work on 2 keys or IDs.  When opening an enumerator the caller should pass in key1 (the first key used 
    calls to DRM_SST_SetData and DRM_SST_OpenKeyTokens.  All entries with this value as the first key will be listed in the
    enumeration.  On calls to DRM_SST_EnumNext the value of key2 will be returned so that the caller can use the combination of
    key1 and key2 to open a unique SST entry. */

DRM_API DRM_RESULT DRM_CALL DRM_SST_OpenEnumerator(
    IN        eDRM_SECURE_STORE_TYPE    eType,
    IN  const DRM_ID                   *pKey1,
    OUT       DRM_SECSTOREENUM_CONTEXT *pSecEnumContext,
    IN        DRM_DST                  *pDatastore,
    IN        DRM_BOOL                  fExclusiveLock);

DRM_API DRM_RESULT DRM_CALL DRM_SST_EnumNext( 
    IN  DRM_SECSTOREENUM_CONTEXT *pSecEnumContext,
    OUT DRM_ID                   *pKey1,
    OUT DRM_ID                   *pKey2,
    OUT DRM_DWORD                *pcbData );

DRM_API DRM_RESULT DRM_CALL DRM_SST_EnumLoadCurrent( 
    IN       DRM_SECSTOREENUM_CONTEXT *pcontextSSTEnum,
    IN OUT   DRM_SECSTORE_CONTEXT     *pcontextSST,
    __in_bcount( DRM_SHA1_DIGEST_LEN ) const DRM_BYTE rgbPassword[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
       OUT   DRM_ID                   *pKey1,
       OUT   DRM_ID                   *pKey2,
    IN OUT DRM_DWORD                  *pcbData);

DRM_API DRM_RESULT DRM_CALL DRM_SST_EnumDeleteCurrent( 
    IN       DRM_SECSTOREENUM_CONTEXT *pcontextSSTEnum,
    IN OUT   DRM_SECSTORE_CONTEXT     *pcontextSST);

DRM_API DRM_RESULT DRM_CALL DRM_SST_CreateGlobalStorePassword(
    __out_bcount( DRM_SHA1_DIGEST_LEN ) DRM_BYTE f_rgbPasswordSST[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN  DRM_BYTE       *f_pbContextBBX);

DRM_API DRM_RESULT DRM_CALL DRM_SST_CreateLicenseStatePassword(
    IN            const DRM_LID  *f_plid,
    __out_bcount( DRM_SHA1_DIGEST_LEN ) DRM_BYTE  f_rgbPasswordLST[__CB_DECL(DRM_SHA1_DIGEST_LEN)],
    IN            DRM_BB_CONTEXT *f_pContextBBX);

EXIT_PK_NAMESPACE;

#endif /* __DRMSECURESTORE_H__ */

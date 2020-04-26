/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMSECURESTORE_H__
#define __DRMSECURESTORE_H__

#include <drmtoken.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DRM_SEC_STORE_MAX_SLOT_SIZE     1024 + 2*SIZEOF(DRM_DWORD) + SHA_DIGEST_LEN
#define DRM_MAX_ATTRIBUTES_PER_SST_KEY    25
#define DRM_MAX_ATTRIBUTE_STRING_LENGTH   25

/* 
    Secure store API users.  Please note that there are 3 modes these APIs open the secure store in.
    The standard DRM_SST_OpenKeyTokens API will open and cache a secure store entry.  This data will
    be flushed to disk on a call to DRM_SST_CloseKey.  No file I/O is done in the calls to 
    DRM_SST_GetTokenValue and DRM_SST_SetTokenValue in between.

    Using the enumerator APIs requries that the HDS context the enum is given be kept open the whole 
    time.

    And finally the DRM_SST_GetData & DRM_SST_SetData are direct read/write tunnels to the secure data in
    the secure store.  The secure store opens and closes the HDS during this call so no caching in on.
    If the Token APIs are too restrictive going directly to these raw read/write may be the best option; but these
    should be used with caution.
*/


typedef enum
{
    SECURE_STORE_LICENSE_DATA            = 0x1,
    SECURE_STORE_GLOBAL_DATA             = 0x2,
    SECURE_STORE_REVOCATION_DATA         = 0x3,
    SECURE_STORE_METERING_DATA           = 0x4,
    SECURE_STORE_PLAYLIST_BURNING_DATA   = 0x5,
    SECURE_STORE_DEVICE_REGISTRATION_DATA= 0x6,
    SECURE_STORE_CACHED_CERTIFICATE_DATA = 0x7
} eDRM_SECURE_STORE_TYPE;

#define DRM_SECURE_STORE_NUM_CHILD_NODES 0x10

typedef struct __tagCachedAttribute
{
    TOKEN             TokenDelta;
    DRM_CONST_STRING  dstrAttribute;
    DRM_DWORD         dwFlags;    
    TOKEN            *pTokenValue;  /* Never assume this is aligned before accessing it!!*/
}CachedAttribute;

#define DRM_SST_SLOT_V0_HASH_OFFSET  SIZEOF(DRM_DWORD)
#define DRM_SST_SLOT_V0_DATA_OFFSET (DRM_SST_SLOT_V0_HASH_OFFSET + SHA_DIGEST_LEN)
#define DRM_SST_SLOT_V0_HEADER_SIZE  DRM_SST_SLOT_V0_DATA_OFFSET

#define DRM_SST_SLOT_METADATA_SIZE     SIZEOF(DRM_DWORD)
#define DRM_SST_SLOT_SIZEDATA_SIZE     SIZEOF(DRM_DWORD)
#define DRM_SST_SLOT_HASH_OFFSET  DRM_SST_SLOT_METADATA_SIZE + DRM_SST_SLOT_SIZEDATA_SIZE
#define DRM_SST_SLOT_DATA_OFFSET (DRM_SST_SLOT_HASH_OFFSET + SHA_DIGEST_LEN)
#define DRM_SST_SLOT_HEADER_SIZE  DRM_SST_SLOT_DATA_OFFSET

typedef struct __tagSEC_STORE_CONTEXT
{    
    DRM_HDS_NAMESPACE_CONTEXT oNsContext;
    DRM_HDS_SLOT_CONTEXT      oSlotContext;

    CachedAttribute rgAttributes [DRM_MAX_ATTRIBUTES_PER_SST_KEY];
    DRM_BYTE        rgbSlotData  [__CB_DECL(DRM_SEC_STORE_MAX_SLOT_SIZE)];
    DRM_BYTE        rgbPassword  [__CB_DECL(SHA_DIGEST_LEN)];
        
    DRM_HDS_HASHKEY   rgbKey1;
    DRM_HDS_UNIQUEKEY rgbKey2;
    
    const DRM_HDS_NAMESPACE *pNamespaceId;
    eDRM_SECURE_STORE_TYPE   eType;

    DRM_WORD wNumAttributes;
    DRM_WORD wNumOriginalAttributes;
    DRM_DWORD cbSlotData;
    DRM_DWORD cbSlot;
    DRM_BOOL fInited;
    DRM_BOOL fLocked;
    DRM_DWORD dwSlotVersion;
} DRM_SECSTORE_CONTEXT;


typedef enum _tag_EnumModeSST
{
    eSSTEnumNone=0,       /* no secure store entries found */    
    eSSTEnumFiltered,     /* enum for secure store entries matching given ID */
    eSSTEnumNatural       /* enum all secure store entries */
} _EnumModeSST;

typedef struct _tag_DRM_SECSTOREENUM_CONTEXT
{
    DRM_HDS_ENUM_CONTEXT      oHdsEnumContext;
    DRM_HDS_NAMESPACE_CONTEXT oNsContext;
    DRM_HDS_SLOT_CONTEXT      oSlotContext;
    DRM_BOOL                  fInited;
    _EnumModeSST              eMode;
    eDRM_SECURE_STORE_TYPE    eType;
    DRM_BOOL                  fCurrIsValid;
    DRM_KID                   oCurrKID;
    DRM_LID                   oCurrLID;
} DRM_SECSTOREENUM_CONTEXT;


/* Flags for DRM_SST_OpenKeyTokens */
#define DRM_SECURE_STORE_CREATE_IF_NOT_EXISTS   0x1
#define DRM_SECURE_STORE_OPEN_ALWAYS            0x2
#define DRM_SECURE_STORE_TRUNCATE_SLOT          0x4

#define DRM_SECURE_STORE_VALID_FLAGS    (DRM_SECURE_STORE_CREATE_IF_NOT_EXISTS | DRM_SECURE_STORE_OPEN_ALWAYS)


DRM_RESULT DRM_API DRM_SST_OpenKeyTokens(
    IN OUT   DRM_SECSTORE_CONTEXT   *pbSecureStoreContext,
    IN const DRM_ID                 *pKey1,
    IN const DRM_ID                 *pKey2,
    IN const DRM_BYTE                rgbPassword[__CB_DECL(SHA_DIGEST_LEN)],
    IN       DRM_DWORD               dwFlags,
    IN       eDRM_SECURE_STORE_TYPE  eType,
    IN       DRM_HDS_CONTEXT        *pcontextHDS);

DRM_RESULT DRM_API DRM_SST_GetTokenValue(
    IN       DRM_SECSTORE_CONTEXT *pbSecureStoreContext,
    IN const DRM_CONST_STRING     *pdstrAttribute,
       OUT   TOKEN                *pToken );

DRM_RESULT DRM_API DRM_SST_SetTokenValue(
    IN       DRM_SECSTORE_CONTEXT *pbSecureStoreContext,
    IN const DRM_CONST_STRING     *pdstrAttribute,
    IN const TOKEN                *pToken );


DRM_RESULT DRM_API DRM_SST_CloseKey( 
    IN       DRM_SECSTORE_CONTEXT *pbSecureStoreContext, 
    IN       DRM_HDS_CONTEXT      *pcontextHDS );

DRM_RESULT DRM_API DRM_SST_OpenAndLockSlot(
    IN          DRM_HDS_CONTEXT        *f_pcontextHDS,
    IN          eDRM_SECURE_STORE_TYPE  f_eType,
    IN  const   DRM_ID                 *f_pKey1,
    IN  const   DRM_ID                 *f_pKey2,
    IN  const   DRM_BYTE                f_rgbPassword[__CB_DECL(SHA_DIGEST_LEN)],
    IN          DRM_DWORD               f_dwFlags,
        OUT     DRM_SECSTORE_CONTEXT   *f_pcontextSST,
        OUT     DRM_DWORD              *f_pcbData );

DRM_RESULT DRM_API DRM_SST_GetLockedData(
    IN       DRM_SECSTORE_CONTEXT   *f_pcontextSST,
       OUT   DRM_BYTE               *f_pbData,
       OUT   DRM_DWORD              *f_pcbData );

DRM_RESULT DRM_API DRM_SST_SetLockedData(
    IN          DRM_SECSTORE_CONTEXT   *f_pcontextSST,
    IN          DRM_DWORD               f_cbData,
        OUT     DRM_BYTE               *f_pbData );

DRM_RESULT DRM_API DRM_SST_CloseLockedSlot( IN  DRM_SECSTORE_CONTEXT *f_pcontextSST );

DRM_RESULT DRM_API DRM_SST_GetData(
    IN       DRM_SECSTORE_CONTEXT   *f_pcontextSST,
    IN const DRM_ID                 *f_pKey1,
    IN const DRM_ID                 *f_pKey2,
    IN const DRM_BYTE                f_rgbPassword[__CB_DECL(SHA_DIGEST_LEN)],
    IN       eDRM_SECURE_STORE_TYPE  f_eType,
    IN       DRM_HDS_CONTEXT        *f_pcontextHDS,
       OUT   DRM_BYTE               *f_pbData,
       OUT   DRM_DWORD              *f_pcbData );

DRM_RESULT DRM_API DRM_SST_SetData(
    IN       DRM_SECSTORE_CONTEXT   *f_pcontextSST,
    IN const DRM_ID                 *f_pKey1,
    IN const DRM_ID                 *f_pKey2,
    IN const DRM_BYTE                f_rgbPassword [__CB_DECL(SHA_DIGEST_LEN)],
    IN       eDRM_SECURE_STORE_TYPE  f_eType,
    IN       DRM_HDS_CONTEXT        *f_pcontextHDS,
       OUT   DRM_BYTE               *f_pbData,
    IN       DRM_DWORD               f_cbData );


/* Delete an entry from the secure store.  Be careful when this happens -- it could open up the system for 
   replay attacks */
DRM_RESULT DRM_API DRM_SST_DeleteKey(
    IN       DRM_SECSTORE_CONTEXT   *pbSecureStoreContext,
    IN       eDRM_SECURE_STORE_TYPE  eType,
    IN const DRM_ID                 *pid1,
    IN const DRM_ID                 *pid2,
    IN       DRM_HDS_CONTEXT        *pcontextHDS);


DRM_RESULT DRM_API DRM_SST_GetAllData( 
    IN     DRM_SECSTORE_CONTEXT *pbSecureStoreContext,
    OUT    DRM_BYTE             *pbData,
    IN OUT DRM_DWORD            *pcbData );

/* The enumeration APIs work on 2 keys or IDs.  When opening an enumerator the caller should pass in key1 (the first key used 
    calls to DRM_SST_SetData and DRM_SST_OpenKeyTokens.  All entries with this value as the first key will be listed in the
    enumeration.  On calls to DRM_SST_EnumNext the value of key2 will be returned so that the caller can use the combination of
    key1 and key2 to open a unique SST entry. */

DRM_RESULT DRM_API DRM_SST_OpenEnumerator(
    IN        eDRM_SECURE_STORE_TYPE    eType,
    IN  const DRM_ID                   *pKey1,
    OUT       DRM_SECSTOREENUM_CONTEXT *pSecEnumContext,
    IN        DRM_HDS_CONTEXT          *pcontextHDS,
    IN        DRM_BOOL                  fExclusiveLock);

DRM_RESULT DRM_API DRM_SST_EnumNext( 
    IN  DRM_SECSTOREENUM_CONTEXT *pSecEnumContext,
    OUT DRM_ID                   *pKey2,
    OUT DRM_DWORD                *pcbData );
DRM_RESULT DRM_API DRM_SST_EnumLoadCurrent( 
    IN       DRM_SECSTOREENUM_CONTEXT *pcontextSSTEnum,
    IN OUT   DRM_SECSTORE_CONTEXT     *pcontextSST,
    IN const DRM_BYTE                  rgbPassword[__CB_DECL(SHA_DIGEST_LEN)],
       OUT   DRM_ID                   *pKey2,
    IN OUT DRM_DWORD                  *pcbData);

DRM_RESULT DRM_API DRM_SST_EnumDeleteCurrent( 
    IN       DRM_SECSTOREENUM_CONTEXT *pcontextSSTEnum,
    IN OUT   DRM_SECSTORE_CONTEXT     *pcontextSST);

DRM_RESULT DRM_API DRM_SST_CreateGlobalStorePassword(
    OUT DRM_BYTE        f_rgbPasswordSST [__CB_DECL(SHA_DIGEST_LEN)],
    IN  DRM_BYTE       *f_pbContextBBX);

DRM_RESULT DRM_API DRM_SST_CreateLicenseStatePassword(
    IN  DRM_LID  *f_plid,
    OUT DRM_BYTE  f_rgbPasswordLST [__CB_DECL(SHA_DIGEST_LEN)],
    IN  DRM_BYTE *f_pbContextBBX);

#ifdef __cplusplus
}
#endif

#endif /* __DRMSECURESTORE_H__ */

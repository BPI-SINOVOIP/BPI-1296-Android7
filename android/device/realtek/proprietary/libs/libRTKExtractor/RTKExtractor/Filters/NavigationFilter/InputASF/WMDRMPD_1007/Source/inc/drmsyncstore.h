/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_SYNC_STORE__
#define __DRM_SYNC_STORE__

#include <drmxmlbuilder.h>

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct _tagSyncContext
{    
    DRM_HDS_SLOT_CONTEXT      contextHDSSlot;
    DRM_HDS_NAMESPACE_CONTEXT contextNameSpace;
    DRM_HDS_CONTEXT          *pcontextHDSStorage;
    DRM_BOOL                  fInited;
} DRM_SYNC_CONTEXT_PRIVATE;

typedef struct _tagSyncListEntry
{
    DRM_BOOL fIsValid;
    DRM_KID  kid;
} SyncListEntry;

typedef struct _tagSyncEnumContext
{
    DRM_SYNC_CONTEXT_PRIVATE *pcontextSync;
    DRM_HDS_ENUM_CONTEXT      contextHDSEnum;
    DRM_HDS_SLOT_CONTEXT      contextHDSSlot;
    DRM_BOOL                  fInited,
                              fAny;
    SyncListEntry             itemSync;
} DRM_SYNC_ENUM_CONTEXT_PRIVATE;


/* opaque versions of contexts, corresponding to those actually declared in drmsyncstore.c */

typedef struct __tagDRM_SYNC_CONTEXT
{
    DRM_BYTE rgbOpaqueData [__CB_DECL(SIZEOF(DRM_SYNC_CONTEXT_PRIVATE))];
} DRM_SYNC_CONTEXT;

typedef struct __tagDRM_SYNC_ENUM_CONTEXT
{
    DRM_BYTE rgbOpaqueData [__CB_DECL(SIZEOF(DRM_SYNC_ENUM_CONTEXT_PRIVATE))];
} DRM_SYNC_ENUM_CONTEXT;

/* low-level access to the sync store */

/**********************************************************************
 *				     PUBLIC FUNCTION DRM_SNC_OpenStore
 **********************************************************************/

DRM_RESULT DRM_API DRM_SNC_OpenStore  (
    IN  DRM_HDS_CONTEXT  *f_pcontextHDSIn,
    OUT DRM_SYNC_CONTEXT *f_pcontextSyncOut);

/**********************************************************************
 *				     PUBLIC FUNCTION DRM_SNC_CloseStore
 **********************************************************************/

DRM_RESULT DRM_API DRM_SNC_CloseStore (
    IN  DRM_SYNC_CONTEXT *f_pcontextSyncIn);


/**********************************************************************
 *				    PUBLIC FUNCTION DRM_SNC_InitEnum
 ***********************************************************************/

DRM_RESULT DRM_API DRM_SNC_InitEnum   (
    IN  DRM_SYNC_CONTEXT       *f_pcontextSyncIn, 
    OUT DRM_SYNC_ENUM_CONTEXT  *f_pcontextSyncEnumOut);

DRM_RESULT DRM_API DRM_SNC_EnumNext   (
    IN  DRM_SYNC_ENUM_CONTEXT  *f_pcontextSYNEnum, 
    OUT DRM_KID                *f_pkid,
    OUT DRM_HDS_SLOT_HINT      *f_pslotHint);

DRM_RESULT DRM_API DRM_SNC_UpdateKID(
    IN       DRM_SYNC_CONTEXT        *f_pcontextSyncIn, 
    IN       DRM_VIEW_RIGHTS_CONTEXT *f_pcontextViewRightsIn,
    IN const DRM_KID                 *f_pkid);

#define DRM_SYNC_IGNORE_THRESHOLD_PARAMETER	0xFFFFFFFF

/* inner implementations of DRM_Manager-level Sync API */

DRM_RESULT DRM_API DRM_SNC_GenerateSyncChallenge (
    IN     DRM_VIEW_RIGHTS_CONTEXT     *f_pcontextViewRights,
    IN     DRM_HDS_CONTEXT			   *f_pcontextHDS,
    IN     DRM_STACK_ALLOCATOR_CONTEXT *f_pcontextStack,
    IN     DRM_DWORD					f_cMaxCount,
    IN     DRM_DWORD                    f_cMaxHours,
    IN     DRM_DWORD                    f_iKIDStart,
    IN     DRM_DWORD                    f_cKIDs,
       OUT DRM_DWORD                   *f_piKIDNext,
       OUT DRM_DWORD                   *f_pcKIDs,
       OUT _XMBContext                 *f_pbChallenge,
    IN OUT DRM_DWORD                   *f_pcbChallenge);


/*********************************************************************
** Function: DRM_SNC_DeleteKID
**
** Synopsis: delete the indicated KID from the sync store; its absense
**           is not an error
**
** Parameters:
**
** [f_pcontextSYN]          -- initialized SYNC context
** [f_pkid]                 -- pointer to KID to be added/updated
**********************************************************************/

DRM_RESULT DRM_API DRM_SNC_DeleteKID(
    IN       DRM_SYNC_CONTEXT *f_pcontextSYN, 
    IN const DRM_KID          *f_pkid);


#ifdef __cplusplus
}
#endif

#endif /* __DRM_SYNC_STORE__ */

/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_SYNC_STORE__
#define __DRM_SYNC_STORE__

#include <drmsyncstorecontext.h>
#include <drmxmlbuilder.h>
#include <drmviewrights.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_SNC_IsLicenseSyncSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_SNC_IsLicenseSyncUnsupported(DRM_VOID);

/* Sync store helper functions */
DRM_RESULT DRM_SNC_GetStoreDataForKID(
    __in                                      DRM_SYNC_CONTEXT          *f_pcontextSYN,
    __in const                                DRM_KID                   *f_pkid,
    __in_opt                                  DRM_DST_SLOT_HINT         *f_pslotHint,
    __out_opt                                 DRM_LICENSE_STATE_DATA    *f_plsd,
    __out_ecount_opt(*f_pcchContentHeader)    DRM_WCHAR                 *f_pwszContentHeader,
    __inout_opt                               DRM_DWORD                 *f_pcchContentHeader);

DRM_RESULT DRM_SNC_GetLicenseDataForKID (
    IN  DRM_DST                    *f_pDatastore,
    IN  DRM_VIEW_RIGHTS_CONTEXT    *f_pcontextViewRightsIn,
    OUT DRM_LICENSE_STATE_DATA     *f_plsd);

DRM_RESULT DRM_SNC_SetStoreDataForKID(
    IN       DRM_SYNC_CONTEXT          *f_pcontextSYN,
    IN const DRM_KID                   *f_pkid,
    IN       DRM_DST_SLOT_HINT         *f_pslotHint,
    IN const DRM_LICENSE_STATE_DATA    *f_plsd,
    IN const DRM_STRING                *f_pdstrContentHeader,
    IN       DRM_BOOL                   f_fCreateIfNotExisting );

DRM_BOOL DRM_SNC_IsSyncRequired(
    DRM_VOID                  *pOEMContext,
    DRM_DWORD                  cMaxCountIn,
    DRM_DWORD                  cMaxHoursIn,
    DRM_LICENSE_STATE_DATA    *plsd,
    DRM_UINT64                *pui64ThreshholdTime);

DRM_BOOL DRM_SNC_IsStoreRefreshRequired (
    IN DRM_VOID               *f_pOEMContext,
    IN DRM_LICENSE_STATE_DATA *f_plsd);

/* low-level access to the sync store */

/**********************************************************************
 *                     PUBLIC FUNCTION DRM_SNC_OpenStore
 **********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_SNC_OpenStore(
    IN  DRM_DST          *f_pDatastore,
    OUT DRM_SYNC_CONTEXT *f_pcontextSyncOut);

/**********************************************************************
 *                     PUBLIC FUNCTION DRM_SNC_CloseStore
 **********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_SNC_CloseStore(
    IN  DRM_SYNC_CONTEXT *f_pcontextSyncIn);


/**********************************************************************
 *                    PUBLIC FUNCTION DRM_SNC_InitEnum
 ***********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_SNC_InitEnum(
    IN  DRM_SYNC_CONTEXT       *f_pcontextSyncIn, 
    OUT DRM_SYNC_ENUM_CONTEXT  *f_pcontextSyncEnumOut);

DRM_API DRM_RESULT DRM_CALL DRM_SNC_EnumNext(
    IN  DRM_SYNC_ENUM_CONTEXT  *f_pcontextSYNEnum, 
    OUT DRM_KID                *f_pkid,
    OUT DRM_DST_SLOT_HINT      *f_pslotHint);

DRM_API DRM_RESULT DRM_CALL DRM_SNC_GetSyncStoreEntry(
    IN                                       DRM_SYNC_CONTEXT        *f_pcontextSYN, 
    IN const                                 DRM_KID                 *f_pkid,
       OUT                                   DRM_LICENSE_STATE_DATA  *f_plsd,
       __out_ecount_opt(*f_pcchContentHeader)DRM_WCHAR               *f_pwszContentHeader,
    IN OUT                                   DRM_DWORD               *f_pcchContentHeader);

typedef enum _DRM_SYNC_UPDATE_CATEGORY
{
    DRM_SYNC_UPDATE_AGGREGATE,
    DRM_SYNC_UPDATE_FORCE_SYNC, 
    DRM_SYNC_UPDATE_NO_LICENSE,
} DRM_SYNC_UPDATE_CATEGORY;


DRM_API DRM_RESULT DRM_CALL DRM_SNC_UpdateKID(
    IN       DRM_SYNC_CONTEXT          *f_pcontextSYN, 
    IN       DRM_VIEW_RIGHTS_CONTEXT   *f_pcontextViewRightsIn,
    IN const DRM_KID                   *f_pkid,
    IN const DRM_CONST_STRING          *f_pdstrContentHeader,
    IN       DRM_SYNC_UPDATE_CATEGORY   f_eSyncCategory );

#define DRM_SYNC_IGNORE_THRESHOLD_PARAMETER    0xFFFFFFFF

/* inner implementations of DRM_Manager-level Sync API */

DRM_API DRM_RESULT DRM_CALL DRM_SNC_DidLicenseExpire(
    IN     DRM_SYNC_CONTEXT          *f_pcontextSync,
    IN     DRM_VIEW_RIGHTS_CONTEXT   *f_pcontextViewRights,
    IN     DRM_DST                   *f_pDatastore,
    IN      DRM_KID                  *f_pKID,
       OUT  DRM_BOOL                 *f_pfExpired );

DRM_API DRM_RESULT DRM_CALL DRM_SNC_GenerateSyncChallenge (
    IN     DRM_SYNC_CONTEXT            *f_pcontextSync,
    IN     DRM_VIEW_RIGHTS_CONTEXT     *f_pcontextViewRights,
    IN     DRM_DST                     *f_pDatastore,
    IN     DRM_STACK_ALLOCATOR_CONTEXT *f_pcontextStack,
    IN     DRM_DWORD                    f_cMaxCount,
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

DRM_API DRM_RESULT DRM_CALL DRM_SNC_DeleteKID(
    IN       DRM_SYNC_CONTEXT *f_pcontextSYN, 
    IN const DRM_KID          *f_pkid);



extern const DRM_CONST_STRING* const g_apdstrActions [1];


EXIT_PK_NAMESPACE;

#endif /* __DRM_SYNC_STORE__ */

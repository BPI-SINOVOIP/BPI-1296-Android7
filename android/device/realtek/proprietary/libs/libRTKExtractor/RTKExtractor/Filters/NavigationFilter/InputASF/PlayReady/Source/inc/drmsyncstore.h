/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_SYNC_STORE__
#define __DRM_SYNC_STORE__

#include <drmsyncstorecontext.h>

ENTER_PK_NAMESPACE;

/* Sync store helper functions */
DRM_RESULT _GetKIDStoreData(
    IN                                        DRM_SYNC_CONTEXT          *f_pcontextSYN,
    IN const                                  DRM_KID                   *f_pkid,
    IN                                        DRM_DST_SLOT_HINT         *f_pslotHint,
       OUT                                    DRM_LICENSE_STATE_DATA    *f_plsd,
       __out_ecount_opt(*f_pcchContentHeader) DRM_WCHAR                 *f_pwszContentHeader,
    IN OUT                                    DRM_DWORD                 *f_pcchContentHeader);

DRM_RESULT _GetKIDLicenseData (IN  DRM_DST                    *f_pDatastore,
                               IN  DRM_VIEW_RIGHTS_CONTEXT    *f_pcontextViewRightsIn,
                               OUT DRM_LICENSE_STATE_DATA     *f_plsd);

DRM_RESULT _SetKIDStoreData(
    IN       DRM_SYNC_CONTEXT          *f_pcontextSYN,
    IN const DRM_KID                   *f_pkid,
    IN       DRM_DST_SLOT_HINT         *f_pslotHint,
    IN const DRM_LICENSE_STATE_DATA    *f_plsd,
    IN const DRM_STRING                *f_pdstrContentHeader,
    IN       DRM_BOOL                   f_fCreateIfNotExisting );

DRM_BOOL _SyncRequired(DRM_VOID                  *pOEMContext,
                       DRM_DWORD                  cMaxCountIn,
                       DRM_DWORD                  cMaxHoursIn,
                       DRM_LICENSE_STATE_DATA    *plsd,
                       DRM_UINT64                *pui64ThreshholdTime);

DRM_BOOL _StoreRefreshRequired (
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

DRM_API DRM_RESULT DRM_CALL DRM_SNC_UpdateKID(
    IN       DRM_SYNC_CONTEXT        *f_pcontextSYN, 
    IN       DRM_VIEW_RIGHTS_CONTEXT *f_pcontextViewRightsIn,
    IN const DRM_KID                 *f_pkid,
    IN const DRM_CONST_STRING        *f_pdstrContentHeader,
    IN       DRM_BOOL                 f_fForceSync );

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

/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_DELETEDSTORE__
#define __DRM_DELETEDSTORE__

#include <drmcommon.h>
#include <drmdatastore.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_DELETEDSTORE_IsDeletedStoreSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_DELETEDSTORE_IsDeletedStoreUnsupported(DRM_VOID);

DRM_API DRM_RESULT DRM_CALL DRM_DELETEDSTORE_UpdateRecentExpired(
    IN       DRM_DST            *pDatastore,
    IN       DRM_KID            *pKID );

DRM_API DRM_RESULT DRM_CALL DRM_DELETEDSTORE_CheckRecentExpired(
             DRM_VOID                *f_pcontextOEM,
    IN       DRM_DST                 *f_pDatastore,
    IN       DRM_KID                 *f_pKID,               /* kid to query */
       OUT   DRM_BOOL                *f_pfRecentExpired );  /* query result */

EXIT_PK_NAMESPACE;

#endif /* __DRM_DELETEDSTORE__ */


/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMMANAGERIMPL_H__
#define __DRMMANAGERIMPL_H__

#include <drmmanagertypes.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_VOID DRM_CALL _FreeDrmAppInternalContexts(
    IN DRM_APP_CONTEXT_INTERNAL *f_poAppContext );

DRM_API DRM_RESULT DRM_CALL _SetupLicEvalObjectToShare(
    IN DRM_APP_CONTEXT_INTERNAL *f_poAppContextInternal );

EXIT_PK_NAMESPACE;

#endif /* __DRMMANAGERIMPL_H__ */


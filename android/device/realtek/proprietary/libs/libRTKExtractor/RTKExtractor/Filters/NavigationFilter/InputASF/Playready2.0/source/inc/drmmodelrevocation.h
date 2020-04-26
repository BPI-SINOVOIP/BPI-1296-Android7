/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMMODELREVOCATION_H__
#define __DRMMODELREVOCATION_H__

#include <drmtypes.h>
#include <drmbcrl.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_MODELREVOCATION_IsModelRevocationSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_MODELREVOCATION_IsModelRevocationUnsupported(DRM_VOID);

DRM_API DRM_RESULT DRM_CALL DRM_MODELREVOCATION_CheckIfPlayReadyModelRevoked( DRM_VOID *f_pOEMContext,
                                                                              DRM_BCRL *f_pPRMainCRL );

EXIT_PK_NAMESPACE;

#endif /*__DRMMODELREVOCATION_H__ */


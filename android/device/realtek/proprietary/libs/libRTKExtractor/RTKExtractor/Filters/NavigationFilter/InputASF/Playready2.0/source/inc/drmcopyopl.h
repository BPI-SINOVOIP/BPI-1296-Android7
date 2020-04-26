/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_COPYOPL__
#define __DRM_COPYOPL__

#include <drmcommon.h>
#include <drmviewrights.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_COPYOPL_IsCopyOplSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_COPYOPL_IsCopyOplUnsupported(DRM_VOID);

DRM_API DRM_RESULT DRM_CALL DRM_COPYOPL_ASD_ExamineOutputLevels(
    IN     DRM_VIEW_RIGHTS_CONTEXT *f_pcontextLQ,
    IN     DRM_COPY_OPL            *f_pOutputLevels,
       OUT DRM_DWORD               *f_pdwResult );

DRM_API DRM_RESULT DRM_CALL DRM_COPYOPL_ASD_ExamineOutputLevelsXMR(
    IN     DRM_VIEW_RIGHTS_CONTEXT *f_pcontextLQ,
       OUT DRM_DWORD               *f_pdwResult );

DRM_API DRM_RESULT DRM_CALL DRM_COPYOPL_ProcessCopyOutputLevelData( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_COPY_OPL                 *f_pCopyOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

DRM_API DRM_RESULT DRM_CALL DRM_COPYOPL_ProcessCopyOutputLevelDataXMR( 
    IN const DRM_XMR_COPY_RIGHTS          *f_pdstrRestrictions,
    IN OUT   DRM_COPY_OPL                 *f_pCopyOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

EXIT_PK_NAMESPACE;

#endif /* __DRM_COPYOPL__ */


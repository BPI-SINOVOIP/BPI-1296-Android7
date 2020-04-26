/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_OUTPUTLEVELS_H__
#define __DRM_OUTPUTLEVELS_H__

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL DRM_OPL_ProcessPlayOutputLevelData( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_PLAY_OPL_EX2             *f_pPlayOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

DRM_API DRM_RESULT DRM_CALL DRM_OPL_ProcessCopyOutputLevelData( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_COPY_OPL                 *f_pCopyOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );


DRM_API DRM_RESULT DRM_CALL DRM_OPL_ProcessPlayOutputLevelDataXMR( 
    IN const DRM_XMR_PLAYBACK_RIGHTS      *f_pdstrRestrictions,
    IN OUT   DRM_PLAY_OPL_EX2             *f_pPlayOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

DRM_API DRM_RESULT DRM_CALL DRM_OPL_ProcessCopyOutputLevelDataXMR( 
    IN const DRM_XMR_COPY_RIGHTS          *f_pdstrRestrictions,
    IN OUT   DRM_COPY_OPL                 *f_pCopyOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

EXIT_PK_NAMESPACE;

#endif /* __DRM_OUTPUTLEVELS_H__ */

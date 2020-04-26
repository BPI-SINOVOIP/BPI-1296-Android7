/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_OUTPUTLEVELS_H__
#define __DRM_OUTPUTLEVELS_H__

#ifdef __cplusplus
extern "C" {
#endif

DRM_RESULT DRM_API DRM_OPL_ProcessPlayOutputLevelData( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_PLAY_OPL                 *f_pPlayOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

DRM_RESULT DRM_API DRM_OPL_ProcessCopyOutputLevelData( 
    IN const DRM_CONST_STRING             *f_pdstrRestrictions,
    IN OUT   DRM_COPY_OPL                 *f_pCopyOPL,
    IN       DRM_STACK_ALLOCATOR_CONTEXT  *f_pstack );

#ifdef __cplusplus
}
#endif


#endif /* __DRM_OUTPUTLEVELS_H__ */

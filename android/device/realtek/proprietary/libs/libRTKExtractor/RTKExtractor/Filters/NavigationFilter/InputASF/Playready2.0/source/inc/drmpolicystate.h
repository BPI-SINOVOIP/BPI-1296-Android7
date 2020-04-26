/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMPOLICYSTATE_H__
#define __DRMPOLICYSTATE_H__

#include <drmmanagertypes.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL Drm_Policy_GetStateData(
    __in    DRM_APP_CONTEXT         *f_poAppContext,
    __in    const DRM_CONST_STRING  *f_pdstrKID,
    __in    const DRM_CONST_STRING  *f_pdstrStateName,
    __out_bcount_opt( *f_pcbStateData ) 
            DRM_BYTE                *f_pbStateData,
    __inout DRM_DWORD               *f_pcbStateData );

DRM_API DRM_RESULT DRM_CALL Drm_Policy_SetStateData(
    __in    DRM_APP_CONTEXT         *f_poAppContext,
    __in    const DRM_CONST_STRING  *f_pdstrKID,
    __in    const DRM_CONST_STRING  *f_pdstrStateName,
    __in_bcount( f_cbStateData )
            DRM_BYTE                *f_pbStateData,
    __inout DRM_DWORD                f_cbStateData );

EXIT_PK_NAMESPACE;

#endif /* __DRMPOLICYSTATE_H__ */

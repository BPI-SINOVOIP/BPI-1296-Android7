/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __NDTASFDRM_H__
#define __NDTASFDRM_H__

ENTER_PK_NAMESPACE;

DRM_RESULT NDTMGR_ASF_Internal_CheckFileDRM( 
    __inout     NDTMGR_INTERNAL_CONTENTSESSION_CONTEXT  * const f_pContSessContext,
    __in const  DRM_BOOL                                        f_fEncryptClear
);

EXIT_PK_NAMESPACE;

#endif /* __NDTASFDRM_H__ */

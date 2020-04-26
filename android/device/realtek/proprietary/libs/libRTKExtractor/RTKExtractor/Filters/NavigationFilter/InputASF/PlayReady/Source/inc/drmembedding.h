/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMEMBEDDING_H__
#define __DRMEMBEDDING_H__

#include <drmcommon.h>
#include <drmlicstore.h>
#include <drmliceval.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL DRM_EMB_UpdateEmbeddedStore(
    __in                                                       DRM_LICSTORE_CONTEXT    *f_pEmbeddedStore,
    __in                                                       DRM_LICSTORE_CONTEXT    *f_pLicStore,
    __in                                                       DRM_DOMAINSTORE_CONTEXT *f_poDomainStoreContext,
    __in const                                                 DRM_LICEVAL_CONTEXT     *f_pLicEvalBackup,    
    __in_bcount( __CB_DECL( SIZEOF( DRM_LICEVAL_CONTEXT ) ) )  DRM_BYTE                 f_rgbLicEvalBuffer[ ],
    __in_bcount( *f_pcbScratchBuffer )                         DRM_BYTE                *f_pbScratchBuffer,
    __inout                                                    DRM_DWORD               *f_pcbScratchBuffer,
    __in                                                       DRM_KID                 *f_poKID,
    __out                                                      DRM_BOOL                *f_pfModifiedEST
);


EXIT_PK_NAMESPACE;

#endif /* __DRMEMBEDDING_H__ */
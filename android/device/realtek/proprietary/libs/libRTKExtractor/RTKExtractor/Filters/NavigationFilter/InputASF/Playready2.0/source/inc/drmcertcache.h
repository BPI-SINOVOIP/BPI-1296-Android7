/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_CERT_CACHE__
#define __DRM_CERT_CACHE__

#include <drmcommon.h>
#include <drmblackbox.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL DRM_CERTCACHE_IsCertCacheSupported(DRM_VOID);
DRM_API DRM_BOOL DRM_CALL DRM_CERTCACHE_IsCertCacheUnsupported(DRM_VOID);

DRM_API DRM_NO_INLINE DRM_RESULT DRM_CALL DRM_CERTCACHE_AddCachedCertificate (
    IN       DRM_BB_CONTEXT       *f_pcontextBBX,
    IN       DRM_SECSTORE_CONTEXT *f_pcontextSSTRevocation,
    IN       DRM_DST              *f_pDatastore,
    IN const DRM_BYTE             *f_pbData,
    IN       DRM_DWORD             f_cbData );

DRM_API DRM_NO_INLINE DRM_BOOL DRM_CALL DRM_CERTCACHE_VerifyCachedCertificate (
    IN       DRM_BB_CONTEXT       *f_pcontextBBX,
    IN       DRM_SECSTORE_CONTEXT *f_pcontextSST,
    IN       DRM_DST              *f_pDatastore,
    IN const DRM_BYTE             *f_pbData,
    IN       DRM_DWORD             f_cbData );

EXIT_PK_NAMESPACE;

#endif /* __DRM_CERT_CACHE__ */


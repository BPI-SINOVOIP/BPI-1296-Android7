/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMMD5_H__
#define __OEMMD5_H__ 1

#include <drmcommon.h>
#include <oemmd5types.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_VOID DRM_CALL DRM_MD5_Init( IN OUT DRM_MD5_CTX *f_contextMD5);

DRM_API DRM_VOID DRM_CALL DRM_MD5_Update(
    IN OUT   DRM_MD5_CTX *f_contextMD5,
    IN const DRM_VOID    *f_pbData,
    IN       DRM_DWORD    f_cbData);

DRM_API DRM_VOID DRM_CALL DRM_MD5_Final( IN OUT DRM_MD5_CTX *f_contextMD5);

EXIT_PK_NAMESPACE;

#endif /* __OEMMD5_H__ */


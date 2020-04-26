/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __NDTENCRYPT_H__
#define __NDTENCRYPT_H__

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL NDTMGR_Encrypt(
    __in        NDTMGR_CONTEXT      *f_pNdtMgrContext,
    __in const  DRM_ID               f_idSession,
    __in const  NDT_ENCRYPTION_MODE  f_EncryptionMode,
    __inout     DRM_VOID            *f_CryptoData
);

EXIT_PK_NAMESPACE;

#endif  /* __NDTENCRYPT_H__ */

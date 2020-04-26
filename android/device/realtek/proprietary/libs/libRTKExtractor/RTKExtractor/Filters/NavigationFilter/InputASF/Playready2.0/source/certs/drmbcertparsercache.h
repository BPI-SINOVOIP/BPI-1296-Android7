/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBCERTPARSERCACHE_H__
#define __DRMBCERTPARSERCACHE_H__

#include <drmtypes.h>
#include <drmbcert.h>

ENTER_PK_NAMESPACE;

typedef enum
{
    DRM_BINARY_DEVICE_CERT_KEY_SIGN = 0,
    DRM_BINARY_DEVICE_CERT_KEY_ENCRYPT = 1
} eDRM_BINARY_DEVICE_CERT_KEYUSAGE;

typedef struct
{
    PUBKEY_P256                 PublicKey;
    DRM_BYTE                    rgbPrivateKey[ __CB_DECL( SIZEOF( PRIVKEY_P256 ) ) ];
} DRM_BINARY_DEVICE_CERT_KEYPAIR;

typedef struct
{
    DRM_BCERT_CLIENT_ID     DeviceSerialNumber;
    DRM_DWORD               dwGroupSecurityLevel;
    DRM_DWORD               dwFeatureFlags;
    DRM_BINARY_DEVICE_CERT_KEYPAIR DeviceKeySign;
    DRM_BINARY_DEVICE_CERT_KEYPAIR DeviceKeyEncrypt;
} DRM_BINARY_DEVICE_CERT_CACHED_VALUES;

DRM_API DRM_RESULT DRM_CALL DRM_BCert_LoadPropertiesCache(
   __in_bcount(f_cbCertData) const DRM_BYTE     *f_pbCertData,
   __in                      const DRM_DWORD     f_cbCertData,
   __inout DRM_BINARY_DEVICE_CERT_CACHED_VALUES *f_pCachedValues,
   __in_opt DRM_VOID                            *f_pOEMContext );

EXIT_PK_NAMESPACE;

#endif /* __DRMBCERTPARSERCACHE_H__ */


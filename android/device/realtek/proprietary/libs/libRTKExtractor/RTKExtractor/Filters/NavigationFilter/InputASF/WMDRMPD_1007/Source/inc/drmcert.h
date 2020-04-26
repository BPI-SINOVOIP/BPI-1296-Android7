/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _DRM_CERT_H_
#define _DRM_CERT_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#define DRM_CERTIFICATE_COLLECTION_MIN 1
#define DRM_CERTIFICATE_COLLECTION_MAX 6
#define DRM_RSA_EXPONENT_VER_2_6_0_0   0x00010001
#define DRM_RSA_EXPONENT_VER_3_0_0_0   0x00010001
#define DRM_RSA_MODULUS_BITCOUNT 1024
#define DRM_CB_RSA_PUBLIC_MOD_1024       (DRM_RSA_MODULUS_BITCOUNT/8)
#define DRM_CB_RSA_PUBLIC_KEY_1024  156
#define DRM_CB_RSA_PRIVATE_KEY_1024 700
#define DRM_CB_RSA_PRIVATE_MOD_1024 (DRM_CB_RSA_PRIVATE_KEY_1024-DRM_CB_RSA_PUBLIC_KEY_1024) /* 'undocumented; secret key at the tail of a DRM_RSA_PRIVATE_KEY */

#define DRM_DSIG_TYPE_STANDARD FALSE
#define DRM_DSIG_TYPE_WMDRMNET TRUE

typedef enum 
{
    DRM_WMDRM_CERT_ATTR_SECURITY_LEVEL,
    DRM_WMDRM_CERT_ATTR_KEY_USAGE,
    DRM_WMDRM_CERT_ATTR_FEATURES,
    DRM_WMDRM_CERT_ATTR_DIGEST_VALUE,

    /* parser only */

    DRM_WMDRM_CERT_ATTR_SECURITY_VERSION,
    DRM_WMDRM_CERT_ATTR_PUBLIC_KEY_SELF_RSA,
    DRM_WMDRM_CERT_ATTR_PUBLIC_KEY_SIGNER_RSA
} DRM_WMDRM_CERT_ATTRIBUTE;

typedef enum
{
    DRM_WMDRM_CERT_KEYUSAGE_SIGNCRL     = 0x01,
    DRM_WMDRM_CERT_KEYUSAGE_SIGNCERT    = 0x02,
    DRM_WMDRM_CERT_KEYUSAGE_ENCRYPTKEY  = 0x04
} DRM_WMDRM_CERT_KEYUSAGE;

typedef enum
{
    DRM_WMDRM_CERT_FEATURES_TRUSTEDTRANSMITTER     = 0x01,
    DRM_WMDRM_CERT_FEATURES_TRUSTEDRECEIVER        = 0x02
} DRM_WMDRM_CERT_FEATURES;

#ifdef  __cplusplus
}
#endif

#endif /* _DRM_CERT_H_ */

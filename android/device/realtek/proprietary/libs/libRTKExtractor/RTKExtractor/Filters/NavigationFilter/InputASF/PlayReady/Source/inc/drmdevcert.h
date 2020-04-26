/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMDEVCERTPARSER_H__
#define __DRMDEVCERTPARSER_H__

#include <drmbcert.h>
#include <drmeccp256.h>

ENTER_PK_NAMESPACE;

typedef enum
{
    DRM_DEVCERT_SERIALNUMBER = 1,       /* CERTIFICATE type="DEVICE"/DATA/UNIQUEID */
    DRM_DEVCERT_DEVICEPUBKEY,           /* CERTIFICATE type="DEVICE"/DATA/PUBLICKEY */
    DRM_DEVCERT_DEVICEPRIVKEY,          /* CERTIFICATE type="DEVICE"/DATA/PRIVATEKEY */
    DRM_DEVCERT_DEVICEDATANODE,         /* CERTIFICATE type="DEVICE"/DATA */ 
    DRM_DEVCERT_DEVICESIGNATURE,        /* CERTIFICATE type="DEVICE"/MSDRM_SIGNATURE_VALUE */
    DRM_DEVCERT_COMPATIBILITYVERSION,   /* FALLBACK/SECURITYVERSION */
    DRM_DEVCERT_COMPATIBILITYCERT,      /* FALLBACK/CERTIFICATE */
    DRM_DEVCERT_SECURECLOCK,            /* CERTIFICATE type="GROUP"/DATA/FEATURE/CLOCK */
    DRM_DEVCERT_SECURECLOCKURL,         /* CERTIFICATE type="GROUP"/DATA/FEATURE/SECURECLOCK/URL */
    DRM_DEVCERT_SECURECLOCKPUBKEY,      /* CERTIFICATE type="GROUP"/DATA/FEATURE/SECURECLOCK/PUBLICKEY */
    DRM_DEVCERT_METERING,               /* CERTIFICATE type="GROUP"/DATA/FEATURE/METERING */
    DRM_DEVCERT_LICENSEACQUISITIONMODE, /* CERTIFICATE type="GROUP"/DATA/FEATURE/LICENSE_ACQ */
    DRM_DEVCERT_LICENSESYNCMODE,        /* CERTIFICATE type="GROUP"/DATA/FEATURE/LICENSE_SYNC */
    DRM_DEVCERT_SYMMETRICOPTIMIZATIONS, /* CERTIFICATE type="GROUP"/DATA/FEATURE/SYMMETRIC_OPT */
    DRM_DEVCERT_ENCRYPTION,             /* CERTIFICATE type="GROUP"/DATA/FEATURE/ENCRYPTION */
    DRM_DEVCERT_MAXCHAINDEPTH,          /* CERTIFICATE type="GROUP"/DATA/LIMITS/MAXCHAINDEPTH */
    DRM_DEVCERT_MAXLICENSESIZE,         /* CERTIFICATE type="GROUP"/DATA/LIMITS/MAXLICENSESIZE */
    DRM_DEVCERT_MAXHEADERSIZE,          /* CERTIFICATE type="GROUP"/DATA/LIMITS/MAXHEADERSIZE */
    DRM_DEVCERT_GROUPSECURITYLEVEL,     /* CERTIFICATE type="GROUP"/DATA/SECURITYLEVEL */
    DRM_DEVCERT_GROUPCERTPUBKEY,        /* CERTIFICATE type="GROUP"/DATA/PUBLICKEY */
    DRM_DEVCERT_GROUPCERTDATANODE,      /* CERTIFICATE type="GROUP"/DATA */ 
    DRM_DEVCERT_GROUPCERTSIGNATURE,     /* CERTIFICATE type="GROUP"/MSDRM_SIGNATURE_VALUE */
    DRM_DEVCERT_DACSECURITYLEVEL,       /* CERTIFICATE type="AUTHORIZATION"/DATA/SECURITYLEVEL */
    DRM_DEVCERT_SUBJECTID,              /* CERTIFICATE type="AUTHORIZATION"/DATA/AUTH_ID */
    DRM_DEVCERT_DACPUBKEY,              /* CERTIFICATE type="AUTHORIZATION"/DATA/PUBLICKEY */
    DRM_DEVCERT_DACDATANODE,            /* CERTIFICATE type="AUTHORIZATION"/DATA */
    DRM_DEVCERT_DACSIGNATURE,           /* CERTIFICATE type="AUTHORIZATION"/MSDRM_SIGNATURE_VALUE */
    DRM_DEVCERT_DACROOTPUBKEY,          /* CERTIFICATE type="AUTHORIZATION_ROOT"/DATA/PUBLICKEY */
    DRM_DEVCERT_AUTHROOTDATANODE,       /* CERTIFICATE type="AUTHORIZATION_ROOT"/DATA */
    DRM_DEVCERT_AUTHROOTSIGNATURE,      /* CERTIFICATE type="AUTHORIZATION_ROOT"/MSDRM_SIGNATURE_VALUE */
    DRM_DEVCERT_SUPPORT_REVOCATION,     /* CERTIFICATE type="GROUP"/DATA/FEATURE/SUPPORT_REVOCATION */
    DRM_DEVCERT_ATTRIB_OTHER,
    DRM_DEVCERT_SUPPORTCRL              /* CERTIFICATE type="GROUP"/DATA/FEATURE/SUPPORT_REVOCATION */
} eDRM_DEVCERT_ATTRIBUTES;


#define DRM_FEATURE_METERING                0x1
#define DRM_FEATURE_ANTI_ROLLBACK_CLOCK     0x2
#define DRM_FEATURE_SECURE_CLOCK            0x4

#define DRM_CLOCK_NONE          0
#define DRM_CLOCK_ANTIROLLBACK  1
#define DRM_CLOCK_SECURE        2

typedef struct
{
    DRM_DWORD appSec;
    DRM_DWORD appcd_subject;
    DRM_DWORD dwFeatureFlags;
    PUBKEY    pubkeyCurrent;
    PUBKEY    pubkeySecureClockServer;
    PKCERT    m_BBCompatibilityCert;
    DRM_BYTE  m_blobDevicePrivkey[__CB_DECL( SIZEOF(PRIVKEY) )];
    DRM_WCHAR wszLegacyVersion[VER_STRING_MAX_LEN];    /* If this device supports a legacy blackbox number and key.  This will be filled in from the devcert */
} DRM_DEVICE_CERTIFICATE_CACHED_VALUES;

#define DRM_BINARY_DEVICE_CERT_MAX_KEYUSAGES    2

typedef enum
{
    DRM_BINARY_DEVICE_CERT_KEY_SIGN = 1,
    DRM_BINARY_DEVICE_CERT_KEY_ENCRYPT
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



/* Get attribute from given devcert */
DRM_API DRM_RESULT DRM_CALL DRM_DCP_GetAttribute(
    IN const DRM_CONST_STRING        *f_pdstrDevCert, /* devcert */
    IN       eDRM_DEVCERT_ATTRIBUTES  f_eAttribute,   /* devcert attr to get */
       OUT   DRM_CONST_STRING        *f_pdstrNode,    /* extracted attr node with <node> tag */
       OUT   DRM_CONST_STRING        *f_pdstrValue);

#define DRM_DCP_VERIFY_DEVICE_CERT  1
#define DRM_DCP_VERIFY_GROUP_CERT   2
#define DRM_DCP_VERIFY_DAC          4

#define DRM_DCP_VERIFY_ENTIRE_DEVCERT (DRM_DCP_VERIFY_DEVICE_CERT \
                                     | DRM_DCP_VERIFY_GROUP_CERT  \
                                     | DRM_DCP_VERIFY_DAC)

DRM_API DRM_RESULT DRM_CALL DRM_DCP_VerifyDeviceCert(
    IN const DRM_CONST_STRING   *f_pdstrDevCert,
    IN       DRM_DWORD           f_dwVerifyFlags,
    IN       DRM_CRYPTO_CONTEXT *f_pcontextCRYP );

DRM_API DRM_RESULT DRM_CALL DRM_DCP_LoadPropertiesCache(
    IN const DRM_CONST_STRING                      *f_pdstrDevCert,
       OUT   DRM_DEVICE_CERTIFICATE_CACHED_VALUES  *f_pcacheDevCert,
    IN       DRM_CRYPTO_CONTEXT                    *f_pcontextCRYP );

#define DRM_DCP_CREATE_DEVCERT_IF_NOT_EXISTING  1
#define DRM_DCP_VERIFY_DEVCERT                  2


DRM_API DRM_RESULT DRM_CALL DRM_DDC_GetDeviceCertificate(
    __in_opt      DRM_VOID           *f_pOEMContext,
    __inout       DRM_STRING         *f_pdstrDevCert,
    __in    const DRM_DWORD           f_dwFlags,
    __inout       DRM_CRYPTO_CONTEXT *f_pcontextCRYP );


DRM_API DRM_RESULT DRM_CALL DRM_DDC_GetBinaryDeviceCertificate(
    __in_opt                                DRM_VOID          *f_pOEMContext,
    __in    const                           DRM_DWORD          f_dwFlags,
    __out_bcount(*f_pcbBinaryDevCertChain)  DRM_BYTE          *f_pBinaryDevCertChain,
    __inout                                 DRM_DWORD         *f_pcbBinaryDevCertChain,
    __inout                                DRM_CRYPTO_CONTEXT *f_pCryptoCtx  );


DRM_API DRM_RESULT DRM_CALL DRM_BCert_LoadPropertiesCache( 
   __in_bcount(f_cbCertData) const DRM_BYTE     *f_pbCertData,
   __in                      const DRM_DWORD    f_cbCertData,
   __inout DRM_BINARY_DEVICE_CERT_CACHED_VALUES *f_pCachedValues,
   __in_opt DRM_VOID                            *f_pOEMContext );


EXIT_PK_NAMESPACE;

#endif /* __DRMDEVCERTPARSER_H__ */

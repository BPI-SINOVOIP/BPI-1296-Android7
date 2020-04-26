/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*  File drm_msdb_pk_manager.h - Public interface for Copy From Device PK
**  The purpose of this PK is local authoring of content with WM DRM protection:
**  1. Processing of CRL package, validation and saving of it.
**  2. Local encryption of content into WM DRM and creation of DRM header.
**  3. Issue of license for playback ( decryption ) of content
*/

#ifndef __DRM_MSDB_PK_MANAGER_H__
#define __DRM_MSDB_PK_MANAGER_H__

/* Includes required headers for drm_msdbpk_manager.h */
#include <drmcommon.h>
#include <drmcontextsizes.h>
#include <drmmanager.h>
#include <drmoutputleveltypes.h>
#include <drmcfdalloc.h>
#include <drmcfdtypes.h>
#include <drmkeygenerator.h>


ENTER_PK_NAMESPACE;

#define DRM_POLICY_VERSION1     0x00000001

/*
** Maximum character count for the custom data in generation of Encryption header
*/
#define DRM_MGR_MAX_CUSTOM_DATA_LENGTH 1024

/*
** Maximum number of CRL types supported per device certificate
**  - Device Revocation
**  - WMDRMNET Revocation
**  - Application Revocation
**  - <future placeholder>
*/
#define DRM_MGR_MAX_CRL_TYPE             4


/*
** Device Certificate Clock Support Values:
*/
#define DRM_MGR_DEVCERT_NO_CLOCK            0
#define DRM_MGR_DEVCERT_ANTIROLLBACK_CLOCK  1
#define DRM_MGR_DEVCERT_SECURE_CLOCK        2


typedef enum __tag_DRM_ENCRYPT_KEY_TYPE
{
    DRM_ENCRYPT_KEY_RC4 = 0,  /* type of key used by XML simple of leaf licenses */

    DRM_ENCRYPT_KEY_DES,      /* type of key used by XML root licenses */

    DRM_ENCRYPT_KEY_AES_128   /* type of key used by all XMR licenses */

} DRM_ENCRYPT_KEY_TYPE;


/**********************************************************************
** Structure for encryption keys context.
** There are 3 values for encryption keys context:
**  1. Key ID
**  2. RC4 Content key or DES root license key or AES key for PlayReady content.  Initialized as SHA( Seed, KID )
**  3. Enumeration value that tells type of the key in rgbKey. ( Content key or DES root key or AES key. )
***********************************************************************/

typedef struct __tagDRM_ENCRYPT_KEY_CONTEXT
{

    /* Key ID */
    DRM_KID                 keyID;

    /* Buffer for an encryption key storage long enough to fit all supported key types */
    DRM_BYTE                rgbKey[__CB_DECL(DRM_AES_KEYSIZE_128)];

    /* Type of rgbKey data */
    DRM_ENCRYPT_KEY_TYPE    eKeyType;
} DRM_ENCRYPT_KEY_CONTEXT;


/**********************************************************************
** Structure that keeps context for device certificate.
** There are 9 values for device certificate context:
**  1. Type of the public key.
       It could be ECC160 pbulic key ( DRM_ECC160_PUBLIC_KEY_LEN = 40 )
**     Or could be ECC256 pbulic key ( ECC_P256_POINT_SIZE_IN_BYTES = 64 )
**  2. Public key of device.
**  3. GUID(s) of revocation CRL(s) supported by device.
**  4. Count of the CRL(s) supported by device.
**  5. Boolean flag that tells if Symmetric Optimizations are supported.
**  6. The Symmetric Optimization data for this "session".
**  7. Group security level of the remote device.
**  8. Boolean flag that tells if Metering is supported.
**  9. Boolean flag that tells if License Sync is supported.
**  10.Indicates the type of clock supported on the device.
***********************************************************************/
typedef enum __tag_DRM_PUBLIC_KEY_TYPE
{
    DRM_PUBLIC_KEY_NONE = 0,
    DRM_PUBLIC_KEY_ECC160,
    DRM_PUBLIC_DEVICE_KEY_ECC256,
    DRM_PUBLIC_DOMAIN_KEY_ECC256,
    DRM_PUBLIC_PC_KEY_ECC256,
    DRM_PUBLIC_SILVERLIGHT_KEY_ECC256
} DRM_PUBLIC_KEY_TYPE;

typedef struct __tagDRM_DEVICE_CERT_CONTEXT
{
    DRM_PUBLIC_KEY_TYPE eDevicePubKeyType;
    PUBKEY_P256         devicePubKey;
    DRM_GUID            domainAccountID;
    DRM_DWORD           dwDomainRevision;
    DRM_GUID            drmCRLGUID[DRM_MGR_MAX_CRL_TYPE];
    DRM_DWORD           dwNumCRLTypesSupported;
    DRM_BOOL            fSymmetricOptimizationsSupported;
    DRM_SYMDATA         symmetricData;
    DRM_DWORD           dwSecurityLevel;
    DRM_BOOL            fMeteringSupported;
    DRM_BOOL            fLicenseSyncSupported;
    DRM_DWORD           dwClockTypeSupported;
} DRM_DEVICE_CERT_CONTEXT;


/*********************************************************************
** Structure for the license policy.
** The policy structure supports following functionality:
**
**   dwPriority     - Priority for the license.
**
**   Expiration of the content. Two fields are used:
**   startExprDate - start date when user may play the content.
**   endExprDate   - end date. After this date user cannot play the content.
**
**   Output level protection.
**   opl - structure keeps values of output protection level.
**
**   dwSourceID - Source ID of the content.
**
**   ExtendedAnalogVideoProtection_type - GUID with type of video output protection.
**   dwExtendedAnalogVideoProtection_config - configuration valud for video output protection.
**
**   dwInclusionGUIDCount - count of used ( filled ) GUIDs in inclusion list
**   InclusionList        - Array of GUID's for inclusion list. Maximum size of array is 20.
**                          Inclusion list is list of GUID's included into license under node <INCLUSIONLIST>
**                          Each GUID has specific ( some may be added in future ) meaning and provides additional rights for license policy
**
**   dwSecurityLevel - Minimum security level for the content.
**   Device with security level below the one specified in license should not be able to play the content.
**
**   rgwchMeteringID - metering ID for the license. Metering ID is 24 charaters UNICODE string.
**                     It is not NULL terminated string, 24 characters are always used for this string.
**
**   dwExpireOnStoreHours - number of hours to expire license after it was stored.
**   dwPlayCount          - maximum play  count for this file.
**   clockRollBackOption  - option to enable clock rollback policies in license
**
**   dwGracePeriod - Duration of the grace period in hours. Grace period is a time when content use allowed after resetting of device clock.
**
**   dwUsedFields - this flag provides options for using of fields in DRM_LICENSE_POLICY structure
**
**   DRM_LIC_POLICY_USE_EXPIRATIONDATE        - endExprDate will be used and added to the license
**   DRM_LIC_POLICY_USE_BEGINDATE             - startExprDate will be used and added to the license
**   DRM_LIC_POLICY_USEOPL                    - OPL will be used and output protection levels added to the license
**   DRM_LIC_POLICY_USEEXTVIDEOPROT           - Extended video protection type and config will be added to the license
**   DRM_LIC_POLICY_USESOURCEID               - dwSourceID will be used and added to the license
**   DRM_LIC_POLICY_USEMETERINGID             - rgwchMeteringID will be used and metering ID tag added to the license
**   DRM_LIC_POLICY_USE_PLAYCOUNT             - dwPlayCount will be used and play count support added to the license
**   DRM_LIC_POLICY_EXPIRE_ON_STORE           - dwExpireOnStoreHours will be used and expiration of license after string it will be enabled.
**   DRM_LIC_POLICY_GRACE_PERIOD              - dwGracePeriod will be used and grace period will be enabled in the license.
**   DRM_LIC_POLICY_DISABLE_ON_CLOCK_ROLLBACK - <ONCLOCKROLLBACK> tag will be added to the license with a statement that disables the license if the clock is rolled-back
**   DRM_LIC_POLICY_EXPIRE_AFTER_FIRST_USE    - license will expire after immediately after the first time it is used
**   DRM_LIC_POLICY_USE_WMDRMNET_REV_INDEX    - Insert the supplied WMDRMNET CRL version requirement into the license
**   DRM_LIC_POLICY_USE_DEVICE_REV_INDEX      - Insert the supplied Device CRL version requirement into the license
**
***********************************************************************/

#define DRM_LIC_POLICY_USE_EXPIRATIONDATE         0x00000001
#define DRM_LIC_POLICY_USE_BEGINDATE              0x00000002
#define DRM_LIC_POLICY_USEOPL                     0x00000004
#define DRM_LIC_POLICY_USEEXTVIDEOPROT            0x00000008
#define DRM_LIC_POLICY_USESOURCEID                0x00000010
#define DRM_LIC_POLICY_USEMETERINGID              0x00000020
#define DRM_LIC_POLICY_USE_PLAYCOUNT              0x00000040
#define DRM_LIC_POLICY_EXPIRE_ON_STORE            0x00000080
#define DRM_LIC_POLICY_GRACE_PERIOD               0x00000100
#define DRM_LIC_POLICY_DISABLE_ON_CLOCK_ROLLBACK  0x00000200
#define DRM_LIC_POLICY_EXPIRE_AFTER_FIRST_USE     0x00000400
#define DRM_LIC_POLICY_USE_WMDRMNET_REV_INDEX     0x00000800
#define DRM_LIC_POLICY_USE_DEVICE_REV_INDEX       0x00001000
#define DRM_LIC_POLICY_USE_REVINFO_INDEX          0x00002000

typedef struct __tagDRM_LICENSE_POLICY

{   DRM_DWORD                               dwLicensePolicyVersion;
    DRM_DWORD                               dwPriority;
    DRMSYSTEMTIME                           startExprDate;
    DRMSYSTEMTIME                           endExprDate;
    DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS    opl;
    DRM_GUID                                ExtendedAnalogVideoProtection_type;
    DRM_DWORD                               dwExtendedAnalogVideoProtection_config;
    DRM_DWORD                               dwInclusionGUIDCount;
    DRM_GUID                                InclusionList[DRM_MAX_INCLUSION_GUIDS];
    DRM_DWORD                               dwSourceID;
    DRM_DWORD                               dwSecurityLevel;
    DRM_WCHAR                               rgwchMeteringID[__CB_DECL(CCH_BASE64_EQUIV(DRM_ID_SIZE))];
    DRM_DWORD                               dwExpirationHours;
    DRM_DWORD                               dwPlayCount;
    DRM_DWORD                               dwGracePeriod;
    DRM_DWORD                               dwWMDRMNETRevIndex;
    DRM_DWORD                               dwDEVICERevIndex;
    DRM_DWORD                               dwRevInfoIndex;
    DRM_DWORD                               dwUsedFields;
} DRM_LICENSE_POLICY;

DRM_API DRM_RESULT DRM_CALL Drm_Msdb_InitDeviceCertificateContext
(
    __inout                               DRM_APP_CONTEXT         *f_poAppContext,
    __inout_opt                           DRM_VOID                *f_pOEMContext,
    __in_bcount(f_cbDevCertData)    const DRM_BYTE                *f_pbDevCertData,
    __in                            const DRM_DWORD                f_cbDevCertData,
    __in_bcount_opt(f_cbDomainCert) const DRM_BYTE                *f_pbDomainCert,
    __in                            const DRM_DWORD                f_cbDomainCert,
    __out                                 DRM_DEVICE_CERT_CONTEXT *f_pCertContext,
    __out                                 DRM_DWORD               *f_pdwFlags
);

DRM_API DRM_RESULT DRM_CALL Drm_Msdb_InitEncryptionKeys
(
    __in_opt                         const DRM_KID                 *f_pKeyID,
    __in_bcount_opt(f_cbKey)         const DRM_BYTE                *f_pbKey,
    __in                             const DRM_DWORD                f_cbKey,
    __in_ecount_nz_opt(f_cchKeySeed) const DRM_WCHAR               *f_pwchKeySeed,
    __in                             const DRM_DWORD                f_cchKeySeed,
    __in                             const DRM_ENCRYPT_KEY_TYPE     f_eKeyType,
    __out                                  DRM_ENCRYPT_KEY_CONTEXT *f_pKeysContext
);

DRM_API DRM_RESULT DRM_CALL Drm_Msdb_CreateEncryptionHeader
(
    __in                              const DRM_CONTENT_SET_PROPERTY  f_eHeaderType,
    __in                              const DRM_ENCRYPT_KEY_CONTEXT  *f_pContentKeyCtx,
    __in                              const DRM_CONST_STRING          f_dstrDomainID,
    __in                              const DRM_CONST_STRING          f_dstrAcquisitionURL,
    __in                              const DRM_CONST_STRING          f_dstrUI_URL,
    __in                              const DRM_CONST_STRING          f_dstrCustomData,
    __in                              const DRM_DWORD                 f_dwContentID,
    __in_opt                          const DRM_PRIVATE_KEY          *f_pHDRSignKey,
    __out_ecount_z_opt(*f_pcchHeader)       DRM_WCHAR                *f_pwzHeader,
    __inout                                 DRM_DWORD                *f_pcchHeader
);

DRM_API DRM_RESULT DRM_CALL Drm_Msdb_InitEncrypt
(
    __in  const DRM_ENCRYPT_KEY_CONTEXT *f_pKeysContext,
    __out       DRM_CIPHER_CONTEXT      *f_pEncryptContext
);

DRM_API DRM_RESULT DRM_CALL Drm_Msdb_GenerateCRLUpdateResponse
(
    __inout                                  DRM_APP_CONTEXT            *f_poAppContext,
    __inout_opt                              DRM_VOID                   *f_pOEMContext,
    __in_ecount_nz_opt(f_cchDevInfo)   const DRM_WCHAR                  *f_pchDevInfo,
    __in                               const DRM_DWORD                   f_cchDevInfo,
    __in                               const DRM_DEVICE_CERT_CONTEXT    *f_pCertContext,
    __out_ecount_z_opt(*f_pcchCRLResp)       DRM_CHAR                   *f_pszCRLResp,
    __inout                                  DRM_DWORD                  *f_pcchCRLResp,
    __out                                    DRM_BOOL                   *f_pfIsDeviceUpdateRequired
);

DRM_API DRM_RESULT DRM_CALL Drm_Msdb_GenerateSimpleLicenseResponse
(
    __inout                                      DRM_APP_CONTEXT         *f_poAppContext,
    __inout_opt                                  DRM_VOID                *f_pOEMContext,
    __in                                   const DRM_ENCRYPT_KEY_CONTEXT *f_pContKeyContext,
    __in                                   const DRM_DEVICE_CERT_CONTEXT *f_pCertContext,
    __in                                   const DRM_LICENSE_POLICY      *f_pLicensePolicy,
    __out_ecount_z_opt(*f_pcchLicenseResp)       DRM_CHAR                *f_pwszLicenseResp,
    __inout                                      DRM_DWORD               *f_pcchLicenseResp
);

DRM_API DRM_RESULT DRM_CALL Drm_Msdb_GenerateRootLicenseResponse
(
    __inout                                      DRM_APP_CONTEXT         *f_poAppContext,
    __inout_opt                                  DRM_VOID                *f_pOEMContext,
    __in                                   const DRM_ENCRYPT_KEY_CONTEXT *f_pRootKeyContext,
    __in                                   const DRM_DEVICE_CERT_CONTEXT *f_pCertContext,
    __in                                   const DRM_LICENSE_POLICY      *f_pLicensePolicy,
    __out_ecount_z_opt(*f_pcchLicenseResp)       DRM_CHAR                *f_pwszLicenseResp,
    __inout                                      DRM_DWORD               *f_pcchLicenseResp
);

DRM_API DRM_RESULT DRM_CALL Drm_Msdb_GenerateLeafLicenseResponse
(
    __inout                                      DRM_APP_CONTEXT         *f_poAppContext,
    __inout_opt                                  DRM_VOID                *f_pOEMContext,
    __in                                   const DRM_ENCRYPT_KEY_CONTEXT *f_pContKeyContext,
    __in                                   const DRM_ENCRYPT_KEY_CONTEXT *f_pRootKeyContext,
    __in                                   const DRM_DEVICE_CERT_CONTEXT *f_pCertContext,
    __in                                   const DRM_LICENSE_POLICY      *f_pLicensePolicy,
    __out_ecount_z_opt(*f_pcchLicenseResp)       DRM_CHAR                *f_pwszLicenseResp,
    __inout                                      DRM_DWORD               *f_pcchLicenseResp
);

#if DRM_SUPPORT_REVOCATION

DRM_API DRM_RESULT DRM_CALL Drm_Revocation_StorePackage(
    __inout                        DRM_APP_CONTEXT *f_poAppContext,
    __in_bcount(f_cbPackage)       DRM_BYTE        *f_pbPackage,
    __in                     const DRM_DWORD        f_cbPackage );

#endif

EXIT_PK_NAMESPACE;

#endif /* __DRM_MSDB_PK_MANAGER_H__ */

/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMBCERTBUILDER_H__
#define __DRMBCERTBUILDER_H__

#include <drmbcert.h>

ENTER_PK_NAMESPACE;

typedef struct _tagDrmBCertDomainData
{
    DRM_BCERT_SERVICE_ID    *pServiceID;
    DRM_BCERT_ACCOUNT_ID    *pAccountID;
    DRM_DWORD                dwRevision;   /* a timestamp */
    DRM_BCERT_DOMAIN_URL    *pDomainURL;
} DRM_BCERT_DOMAIN_DATA;

typedef struct _tagDrmBCertPCData
{
    DRM_DWORD                dwSecurityVersion;  /* A.B.C.D */
} DRM_BCERT_PC_DATA;

typedef struct
{
    DRM_DWORD                dwSecurityVersion;  /* A.B.C.D */
    DRM_DWORD                dwPlatformIdentifier;
} DRM_BCERT_SILVERLIGHT_DATA;

typedef struct _tagDrmBCertDeviceData
{
    DRM_DWORD                cbMaxLicense;
    DRM_DWORD                cbMaxHeader;
    DRM_DWORD                dwMaxChainDepth;
} DRM_BCERT_DEVICE_DATA;

typedef struct
{
    DRM_BCERT_METERING_ID     *pMeteringID;
    DRM_BCERT_METERING_URL    *pMeteringURL;
} DRM_BCERT_METERING_DATA;

typedef struct _tagDrmBCertServerData
{
    DRM_DWORD                dwWarningDays;
} DRM_BCERT_SERVER_DATA;

typedef struct
{
    DRM_BOOL                 fValid;
    DRM_DWORD                dwSecurityVersion;  /* A.B.C.D */
    DRM_DWORD                dwPlatformIdentifier;
} DRM_BCERT_SECURITY_VERSION_DATA;

/*
** The following byte value should be equal to the largest private key length supported
*/
#define DRM_BCERT_MAX_PRIVKEY_VALUE_LENGTH     SIZEOF( PRIVKEY_P256 )


typedef struct _tagDrmBCertPrivKey
{
    DRM_BYTE    rgb[ __CB_DECL( DRM_BCERT_MAX_PRIVKEY_VALUE_LENGTH ) ];
} DRM_BCERT_PRIVKEY;


typedef struct _tagDrmBCertExtBuilderData
{
    DRM_BOOL                          fValid;                  /* if TRUE then add data in this structure to a new cert, otherwise ignore */
    DRM_WORD                          wExtDataKeyType;
    DRM_WORD                          wExtDataKeyLength;       /* in bits */
    DRM_DWORD                         dwExtDataKeyFlags;       /* bit field; optional/ignored */
    DRM_BCERT_PUBKEY                  oPubKey;                 /* optional. */
    DRM_BCERT_PRIVKEY                 oPrivKey;                /* optional. */
    DRM_DWORD                         dwNumExtDataRecords;     /* 0 or 1 */
    DRM_BCERT_EXTDATA_RECORD         *pExtDataRecords;         /* NULL or present. */
    DRM_WORD                          wExtDataSignatureType;
} DRM_BCERT_EXT_BUILDER_DATA;


typedef struct _tagDrmBCertBuilderCertKey
{
    DRM_WORD                          wKeyType;
    DRM_WORD                          wKeyLength;             /* in bits */
    DRM_DWORD                         dwKeyFlags;             /* bit field; should be ignored */
    DRM_BCERT_PUBKEY                  KeyValue;
    DRM_DWORD                         dwNumKeyUsages;         /* number of key usages */
    DRM_DWORD                         *pKeyUsageSet;          /* ptr to array of key usages, cannot be NULL */

} DRM_BCERT_BUILDER_CERTKEY;


typedef struct _tagDrmBCertBuilderData
{
    DRM_DWORD                         dwChainHeaderFlags;     /* bit field; should be ignored */

    DRM_BCERT_CERT_ID                *pCertificateID;
    DRM_DWORD                         dwSecurityLevel;
    DRM_DWORD                         dwBasicFlags;           /* bit field; can be 0x0 or 0x1 */
    DRM_DWORD                         dwType;
    DRM_DWORD                         dwExpirationDate;       /* optional */
    DRM_BCERT_CLIENT_ID               ClientID;

    DRM_DWORD                         dwNumFeatureEntries;    /* may be 0 */
    DRM_DWORD                        *prgdwFeatureSet;        /* ptr to an array; may be NULL */

    DRM_DWORD                         dwNumCertificateKeys;   /* number of elements in array, must be >= 1 */
    DRM_BCERT_BUILDER_CERTKEY        *pCertificateKeys;       /* ptr to array of cert key structures, cannot be NULL */

    DRM_DWORD                         dwManufacturerFlags;    /* bit field; optional/ignored */
    DRM_BCERT_MANUFACTURER_STRINGS   *pManufacturerStrings;   /* optional */

    PRIVKEY_P256                     *pPrivateKey;
    DRM_DWORD                         dwIssuerKeyLength;      /* in bits */
    DRM_BCERT_PUBKEY                  IssuerKey;
    DRM_WORD                          wSignatureType;

    DRM_BCERT_EXT_BUILDER_DATA        ExtendedData;

    union _tagBuilderType    /* optional */
    {
        DRM_BCERT_DOMAIN_DATA       DomainData;
        DRM_BCERT_PC_DATA           PCData;
        DRM_BCERT_DEVICE_DATA       DeviceData;
        DRM_BCERT_SILVERLIGHT_DATA  SilverLightData;
        DRM_BCERT_METERING_DATA     MeteringData;
        DRM_BCERT_SERVER_DATA       ServerData;
    } Type;

    DRM_BCERT_SECURITY_VERSION_DATA SecurityVersion;
} DRM_BCERT_BUILDER_DATA;


DRM_API DRM_VOID DRM_CALL BCert_SetDefaultBuilderData(
   __in         DRM_BCERT_CERT_ID         *f_pCertificateID,
   __in  const  DRM_DWORD                 f_dwType,
   __in         PRIVKEY_P256              *f_pPrivateKey,
   __in         PUBKEY_P256               *f_pIssuerKey,
   __out        DRM_BCERT_BUILDER_DATA    *f_pData
);


DRM_API DRM_VOID DRM_CALL BCert_SetKeyInfoBuilderData(
    __in_ecount(f_dwNumKeys) DRM_BCERT_BUILDER_CERTKEY  *f_pKeys,
    __in                     const DRM_DWORD             f_dwNumKeys,
    __out                    DRM_BCERT_BUILDER_DATA     *f_pData
);

DRM_API DRM_VOID DRM_CALL BCert_SetDomainBuilderData(
   __in         DRM_BCERT_SERVICE_ID    *f_pServiceID,
   __in         DRM_BCERT_ACCOUNT_ID    *f_pAccountID,
   __in  const  DRM_DWORD                f_dwRevision,
   __in         DRM_BCERT_CLIENT_ID     *f_pClientID,
   __in         DRM_BCERT_DOMAIN_URL    *f_pDomainURL,
   __out        DRM_BCERT_BUILDER_DATA  *f_pData
);


DRM_API DRM_VOID DRM_CALL BCert_SetPCBuilderData(
   __in           DRM_BCERT_CLIENT_ID      *f_pHardwareID,
   __in    const  DRM_DWORD                 f_dwSecurityVersion,  /* A.B.C.D */
   __in           PRIVKEY_P256             *f_pPrivateKey,
   __in           PUBKEY_P256              *f_pPublicKey,
   __in           DRM_BCERT_EXTDATA_RECORD *f_oRecord,
   __out          DRM_BCERT_BUILDER_DATA   *f_pData
);

DRM_API DRM_VOID DRM_CALL BCert_SetSilverLightBuilderData(
   __in           DRM_BCERT_CLIENT_ID     *f_pHardwareID,
   __in    const  DRM_DWORD                f_dwSecurityVersion,  /* A.B.C.D */
   __in    const  DRM_DWORD                f_dwPlatformIdentifier,
   __out          DRM_BCERT_BUILDER_DATA  *f_pData
);

DRM_API DRM_VOID DRM_CALL BCert_SetDeviceBuilderData(
   __in           DRM_BCERT_CLIENT_ID     *f_pSerialNum,
   __out          DRM_BCERT_BUILDER_DATA  *f_pData
);

DRM_API DRM_VOID DRM_CALL BCert_SetMeteringBuilderData(
   __in         DRM_BCERT_METERING_ID    *f_pMeteringID,
   __in         DRM_BCERT_METERING_URL   *f_pMeteringURL,
   __out        DRM_BCERT_BUILDER_DATA   *f_pData
);

DRM_API DRM_VOID DRM_CALL BCert_SetServerBuilderData(
   __in    const  DRM_DWORD                 f_dwWarningDays,
   __out          DRM_BCERT_BUILDER_DATA   *f_pData
);

DRM_API DRM_RESULT DRM_CALL BCert_AddCert(
            const DRM_BYTE                *f_pbParentChain,
                  DRM_BYTE                *f_pbNewChain,
   __inout        DRM_DWORD               *f_pcbNewChain,
   __in           DRM_BCERT_BUILDER_DATA  *f_pData,
   __inout        DRM_CRYPTO_CONTEXT      *f_pCryptoCtx,
   __in_opt       DRM_VOID                *f_pOEMContext );

DRM_API DRM_RESULT DRM_CALL BCert_AddExtendedDataToCert(
   __inout_bcount(f_cbCertData) DRM_BYTE *f_pbCertChain,
   __in     const DRM_DWORD              f_cbCertData,
   __in     DRM_BCERT_EXT_BUILDER_DATA   *f_pExtData,
   __inout  DRM_CRYPTO_CONTEXT           *f_pCryptoCtx
);

EXIT_PK_NAMESPACE

#endif /* __DRMBCERTBUILDER_H__ */


/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCERTPARSER_H__
#define __DRMCERTPARSER_H__


#include "drmbase64.h"
#include "drmrevocation.h"
#include "drmcert.h"

ENTER_PK_NAMESPACE;

#if DRM_SUPPORT_REVOCATION

enum DRM_CERTIFICATE_TYPE
{
    certtypeUnknown = -1,
    certtypeRoot    =  0,
    certtypeCertAuthority,
    certtypeBlackBox,
    certtypeBlackBoxVersion,
    certtypeMachine,
    certtypeIndivBox,
    certtypeExport,
    certtypeExportIncl,
    certtypeDevice,
    certtypeSampleProtection
};

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetCertificateType(
  IN  const DRM_CHAR             *f_pszBase,
  IN  const DRM_SUBSTRING        *f_pdasstrCert,
  OUT enum DRM_CERTIFICATE_TYPE *f_pcerttype);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetAttribute(
  IN const DRM_CHAR                *f_pszBase,
  IN const DRM_SUBSTRING           *f_pdasstrCert,
  IN       DRM_WMDRM_CERT_ATTRIBUTE f_attribute,
     OUT   DRM_SUBSTRING            f_rgdasstrValue [],
  IN OUT   DRM_DWORD               *f_pcEntries);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetKeyUsage(
  IN const DRM_CHAR                *f_pszBase,
  IN const DRM_SUBSTRING           *f_pdasstrCert,
     OUT DRM_DWORD                 *f_dwKeyUsage);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetFeatures(
  IN const DRM_CHAR                *f_pszBase,
  IN const DRM_SUBSTRING           *f_pdasstrCert,
     OUT DRM_DWORD                 *f_dwFeatures);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetSecurityLevel(
  IN const DRM_CHAR                *f_pszBase,
  IN const DRM_SUBSTRING           *f_pdasstrCert,
     OUT DRM_DWORD                 *f_dwSecurityLevel);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetPublicKey(
  IN const DRM_CHAR                *f_pszBase,
  IN const DRM_SUBSTRING           *f_pdasstrCert,
     OUT DRM_BYTE                  *f_pbModulus,
     OUT DRM_DWORD                 *f_pcbModulus );

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetHardwareID(
  IN const DRM_CHAR                *f_pszBase,
  IN const DRM_SUBSTRING           *f_pdasstrCert,
     OUT   DRM_BYTE                 f_rgbHardwareID [DRM_SHA1_DIGEST_LEN]);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetSignatureDigest(
  IN const DRM_CHAR                *f_pszBase,
  IN const DRM_SUBSTRING           *f_pdasstrCert,
     OUT   DRM_BYTE                 f_rgbDigest [DRM_SHA1_DIGEST_LEN]);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetPKCert(
  IN const DRM_CHAR      *f_pszBase,
  IN const DRM_SUBSTRING *f_pdasstrCert,
     OUT   PKCERT        *f_ppkcert);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_GetManufacturerNodes(
  IN const DRM_CHAR      *f_pszBase,
  IN const DRM_SUBSTRING *f_pdasstrCert,
     OUT   DRM_SUBSTRING *f_pdastrNamespace,
     OUT   DRM_SUBSTRING  f_rgdastrTag       [],
     OUT   DRM_SUBSTRING  f_rgdastrAttrLabel [],
     OUT   DRM_SUBSTRING  f_rgdastrAttrValue [],
     OUT   DRM_SUBSTRING  f_rgdastrValue     [],
  IN OUT   DRM_DWORD     *f_pcEntries);

#define DRM_WCP_VERIFY_CERT_SIGNATURE     0x00000001
#define DRM_WCP_VERIFY_CERT_REQUIRED_TAGS 0x00000002
#define DRM_WCP_VERIFY_CERT_METHODS       0x00000004
#define DRM_WCP_VERIFY_CERT_ALL      (DRM_WCP_VERIFY_CERT_SIGNATURE|DRM_WCP_VERIFY_CERT_REQUIRED_TAGS|DRM_WCP_VERIFY_CERT_METHODS)

DRM_API DRM_RESULT DRM_CALL DRM_WCP_VerifyCertificate(
    IN const DRM_CHAR            *f_pszBase,
    IN const DRM_SUBSTRING       *f_pdasstrCert,
    IN       DRM_DWORD            f_fVerify,
    IN const WMDRMNET_CRL_ENTRY  *f_pRevocationEntries,
    IN       DRM_DWORD            f_cRevocationEntries,
       OUT   DRM_SUBSTRING       *f_pdasstrPublicRSA,
    IN       DRM_CRYPTO_CONTEXT   *f_pCryptoCtx);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_VerifyCertificateCollection(
    IN const DRM_CHAR              *f_pszBase,
    IN const DRM_SUBSTRING         *f_pdasstrCert,
    IN const DRM_ANSI_CONST_STRING *f_pdastrRootPubKey,
    IN const WMDRMNET_CRL_ENTRY    *f_pRevocationEntries,
    IN       DRM_DWORD              f_cRevocationEntries,
       OUT   DRM_SUBSTRING         *f_pdasstrLeafCertificate,
       OUT   DRM_SUBSTRING         *f_pdasstrPubkeyLeafmost,
    IN       DRM_CRYPTO_CONTEXT   *f_pCryptoCtx);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_ReadPubKeyModulus(
    IN                        DRM_CHAR      *f_rgchB64,
    IN                        DRM_SUBSTRING *f_pdasstrB64,
    OUT                       DRM_BYTE      *f_pbMod,
    OUT                       DRM_DWORD     *f_pcbMod);

DRM_API DRM_RESULT DRM_CALL DRM_WCP_VerifyDigitalSignature(
    IN   const DRM_CHAR             *f_pszBase,
    IN   const DRM_SUBSTRING        *f_pdasstrDataInclusive,
    IN   const DRM_SUBSTRING        *f_pdasstrSignatureInclusive,
    IN   const DRM_BOOL              f_fWMDRMNET,
    IN   const WMDRMNET_CRL_ENTRY   *f_pRevocationEntries,
    IN         DRM_DWORD             f_cRevocationEntries,
    IN         DRM_CRYPTO_CONTEXT   *f_pCryptoCtx);
#endif

EXIT_PK_NAMESPACE;

#endif /* __DRMCERTPARSER_H__ */

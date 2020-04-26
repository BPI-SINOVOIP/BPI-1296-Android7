/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMDEVCERTPARSER_H__
#define __DRMDEVCERTPARSER_H__

#include <drmbcert.h>
#include <oemeccp256.h>
#include <drmkeyfile.h>

#include <drmdevcertcache.h>

ENTER_PK_NAMESPACE;

#define DRM_FEATURE_METERING                0x1
#define DRM_FEATURE_ANTI_ROLLBACK_CLOCK     0x2
#define DRM_FEATURE_SECURE_CLOCK            0x4

#define DRM_CLOCK_NONE          0
#define DRM_CLOCK_ANTIROLLBACK  1
#define DRM_CLOCK_SECURE        2

#define DRM_DCP_CREATE_DEVCERT_IF_NOT_EXISTING  1
#define DRM_DCP_VERIFY_DEVCERT                  2

typedef struct
{
    DRM_DWORD appSec;
    DRM_DWORD appcd_subject;
    DRM_DWORD dwFeatureFlags;
    PUBKEY    pubkeyCurrent;
    PUBKEY    pubkeySecureClockServer;
    PKCERT    m_BBCompatibilityCert;
#if DRM_SUPPORT_DEVICE_SIGNING_KEY
    PUBKEY    pubkeySigning;
    PKCERT    m_pubKeySigningCert;
#endif
    DRM_BYTE  m_blobDevicePrivkey[__CB_DECL( SIZEOF(PRIVKEY) )];
    DRM_WCHAR wszLegacyVersion[VER_STRING_MAX_LEN];    /* If this device supports a legacy blackbox number and key.  This will be filled in from the devcert */
} DRM_DEVICE_CERTIFICATE_CACHED_VALUES;

#define DRM_BINARY_DEVICE_CERT_MAX_KEYUSAGES    2

DRM_API DRM_RESULT DRM_CALL DRM_DCP_GetAttribute(
    __in_opt  DRM_DEVICE_CERT_PARSER_CACHE *f_poParserCache,
    __in      const DRM_CONST_STRING       *f_pdstrDevCert,
    __in      eDRM_DEVCERT_ATTRIBUTES       f_eAttribute,
    __out_opt DRM_CONST_STRING             *f_pdstrNode,
    __out_opt DRM_CONST_STRING             *f_pdstrValue );

DRM_API DRM_RESULT DRM_CALL DRM_DCP_StartCertParserCache(
    __in     const DRM_CONST_STRING       *f_pdstrDevCert,
    __in_opt DRM_KEYFILE_CONTEXT          *f_poKeyFileContext,
    __out    DRM_DEVICE_CERT_PARSER_CACHE *f_poParserCache );

DRM_API DRM_RESULT DRM_CALL DRM_DCP_StopCertParserCache(
    __in_opt DRM_KEYFILE_CONTEXT          *f_poKeyFileContext,
    __in     DRM_DEVICE_CERT_PARSER_CACHE *f_poParserCache );

#define DRM_DCP_VERIFY_DEVICE_CERT  1
#define DRM_DCP_VERIFY_GROUP_CERT   2
#define DRM_DCP_VERIFY_DAC          4

#define DRM_DCP_VERIFY_ENTIRE_DEVCERT (DRM_DCP_VERIFY_DEVICE_CERT \
                                     | DRM_DCP_VERIFY_GROUP_CERT  \
                                     | DRM_DCP_VERIFY_DAC)

DRM_API DRM_RESULT DRM_CALL DRM_DCP_VerifyDeviceCert(
    __in_opt   DRM_DEVICE_CERT_PARSER_CACHE *f_poParserCache,
    __in const DRM_CONST_STRING             *f_pdstrDevCert,
    __in       DRM_DWORD                     f_dwVerifyFlags,
    __in_opt   DRM_KEYFILE_CONTEXT          *f_poKeyFileContext,
    __in       DRM_CRYPTO_CONTEXT           *f_pcontextCRYP );

DRM_API DRM_RESULT DRM_CALL DRM_DCP_LoadPropertiesCache(
    __in_opt DRM_DEVICE_CERT_PARSER_CACHE          *f_poParserCache,
    __in     const DRM_CONST_STRING                *f_pdstrDevCert,
    __in_opt DRM_KEYFILE_CONTEXT                   *f_poKeyFileContext,
    __in     DRM_CRYPTO_CONTEXT                    *f_poCryptoContext,
    __out    DRM_DEVICE_CERTIFICATE_CACHED_VALUES  *f_pcacheDevCert );

EXIT_PK_NAMESPACE;

#endif /* __DRMDEVCERTPARSER_H__ */

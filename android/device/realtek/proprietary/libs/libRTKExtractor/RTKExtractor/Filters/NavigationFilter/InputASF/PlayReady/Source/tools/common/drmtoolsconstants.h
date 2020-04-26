/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

extern const DRM_CONST_STRING  g_dstrTagRequestDAC;
extern const DRM_CONST_STRING  g_dstrTagDAC;
extern const DRM_CONST_STRING  g_dstrTagDAC_WMDRM_MD;
extern const DRM_CONST_STRING  g_dstrTagDAC_PlayReadyND;
extern const DRM_CONST_STRING  g_dstrTagDACResponse;
extern const DRM_CONST_STRING  g_dstrTagManufName;

extern const DRM_BYTE          g_rgbFFFE[__CB_DECL(2)];

/*
**  Xpaths
*/
extern const DRM_CONST_STRING  g_dstrXPathDACPubKey;
extern const DRM_CONST_STRING  g_dstrXPathFallbackPubKey;
extern const DRM_CONST_STRING  g_dstrXPathWMDRM_MDPubKey;
extern const DRM_CONST_STRING  g_dstrXPathPlayReadyNDPubKeyModulus;
extern const DRM_CONST_STRING  g_dstrXPathPlayReadyNDPubKeyExponent;
extern const DRM_CONST_STRING  g_dstrXPathDACPrivKey;
extern const DRM_CONST_STRING  g_dstrXPathFallbackPrivKey;
extern const DRM_CONST_STRING  g_dstrXPathWMDRMPrivKey;
extern const DRM_CONST_STRING  g_dstrXPathWMDRMPubKey;

extern const DRM_CONST_STRING  g_dstrXPathFallbackSecVer;

extern const DRM_CONST_STRING  g_dstrTagDataSecurityVersionValue;
extern const DRM_CONST_STRING  g_dstrAuthorizationIDDAC;
extern const DRM_CONST_STRING  g_dstrAuthIDDACRoot;
extern const DRM_CONST_STRING  g_dstrTagSecurityLevelValue;

extern const DRM_CONST_STRING  g_dstrCertMetercert;
extern const DRM_CONST_STRING  g_dstrCertSecureClock;

extern const DRM_CONST_STRING  g_dstrTestTemplate;
extern const DRM_CONST_STRING  g_dstrXMLTree1;
extern const DRM_CONST_STRING  g_dstrXMLTree2;
extern const DRM_CONST_STRING  g_dstrXMLTree3;
extern const DRM_CONST_STRING  g_dstrXMLTree4;

extern const DRM_CONST_STRING  g_dstrSigningKey;
extern const DRM_CONST_STRING  g_dstrTagKeySeed;
extern const DRM_CONST_STRING  g_dstrTagRoot;
extern const DRM_CONST_STRING  g_dstrTagLicAcqInfo;
extern const DRM_CONST_STRING  g_dstrTagCustomData;
extern const DRM_CONST_STRING  g_dstrEncBlkSize;
extern const DRM_CONST_STRING  g_dstrCID;
extern const DRM_CONST_STRING  g_dstrVerifyKey;
extern const DRM_CONST_STRING  g_dstrDefaultURL;
extern const DRM_CONST_STRING  g_dstrDefaultBlksize;
extern const DRM_CONST_STRING  g_dstrDefaultFilename;
extern const DRM_CONST_STRING  g_dstrContents;
extern const DRM_CONST_STRING  g_dstrSerial;

extern const DRM_CONST_STRING  g_dstrTagPrivkey;
extern const DRM_CONST_STRING  g_dstrTagPUBKEY;
extern const DRM_CONST_STRING  g_dstrTagKeypair;
extern const DRM_CONST_STRING  g_dstrTagExpression;

extern const DRM_CONST_STRING  g_dstrVersion;
extern const DRM_CONST_STRING  g_dstrType;
extern const DRM_CONST_STRING  g_dstrDevice;
extern const DRM_CONST_STRING  g_dstrPC;
extern const DRM_CONST_STRING  g_dstrExtendedData;
extern const DRM_CONST_STRING  g_dstrExtendedDataSigningKey;
extern const DRM_CONST_STRING  g_dstrExtendedDataSigningKeyPublicKey;
extern const DRM_CONST_STRING  g_dstrExtendedDataSigningKeyPrivateKey;
extern const DRM_CONST_STRING  g_dstrExtendedDataRecord;
extern const DRM_CONST_STRING  g_dstrDomain;
extern const DRM_CONST_STRING  g_dstrMeteringCertType;
extern const DRM_CONST_STRING  g_dstrServerCertType;
extern const DRM_CONST_STRING  g_dstrSilverLight;
extern const DRM_CONST_STRING  g_dstrApplication;
extern const DRM_CONST_STRING  g_dstrIssuer;
extern const DRM_CONST_STRING  g_dstrService;
extern const DRM_CONST_STRING  g_dstrCRLSigner;
extern const DRM_CONST_STRING  g_dstrKeyFileSigner;
extern const DRM_CONST_STRING  g_dstrBasicInfo;
extern const DRM_CONST_STRING  g_dstrSecurityLevel;
extern const DRM_CONST_STRING  g_dstrClientID;
extern const DRM_CONST_STRING  g_dstrExpiration;
extern const DRM_CONST_STRING  g_dstrWarningDays;
extern const DRM_CONST_STRING  g_dstrServiceID;
extern const DRM_CONST_STRING  g_dstrAccountID;
extern const DRM_CONST_STRING  g_dstrRevision;
extern const DRM_CONST_STRING  g_dstrDomainUrl;
extern const DRM_CONST_STRING  g_dstrSecurityVersion;
extern const DRM_CONST_STRING  g_dstrSecurityVersionSeperator;
extern const DRM_CONST_STRING  g_dstrExtendedDataSigningKeyType;
extern const DRM_CONST_STRING  g_dstrExtendedDataSigningKeyPublicKeyValue;
extern const DRM_CONST_STRING  g_dstrExtendedDataSigningKeyPrivateKeyValue;
extern const DRM_CONST_STRING  g_dstrExtendedDataRecordType;
extern const DRM_CONST_STRING  g_dstrExtendedDataRecordFlags;
extern const DRM_CONST_STRING  g_dstrExtendedDataRecordMaxLength;
extern const DRM_CONST_STRING  g_dstrExtendedDataRecordValue;
extern const DRM_CONST_STRING  g_dstrTransmitter;
extern const DRM_CONST_STRING  g_dstrReceiver;
extern const DRM_CONST_STRING  g_dstrSharedCertificate;
extern const DRM_CONST_STRING  g_dstrSecureClock;
extern const DRM_CONST_STRING  g_dstrRollbackClock;
extern const DRM_CONST_STRING  g_dstrMetering;
extern const DRM_CONST_STRING  g_dstrLicenseSync;
extern const DRM_CONST_STRING  g_dstrSymmOpt;
extern const DRM_CONST_STRING  g_dstrSupportCRL;
extern const DRM_CONST_STRING  g_dstrCertFeatures;
extern const DRM_CONST_STRING  g_dstrCertManufacturer;
extern const DRM_CONST_STRING  g_dstrManufacturerName;
extern const DRM_CONST_STRING  g_dstrModelName;
extern const DRM_CONST_STRING  g_dstrModelNumber;
extern const DRM_CONST_STRING  g_dstrCertKey;
extern const DRM_CONST_STRING  g_dstrCertKeyType;
extern const DRM_CONST_STRING  g_dstrCertKeys;
extern const DRM_CONST_STRING  g_dstrCertECC256;
extern const DRM_CONST_STRING  g_dstrCertKeyUsages;
extern const DRM_CONST_STRING  g_dstrKeyUsageSign;
extern const DRM_CONST_STRING  g_dstrKeyUsageEncryptKey;
extern const DRM_CONST_STRING  g_dstrKeyUsageSignCRL;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerAll;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerIndiv;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerDevice;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerLink;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerDomain;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerSilverLight;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerApplication;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerCRLSigner;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerMetering;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerKeyFileSigner;
extern const DRM_CONST_STRING  g_dstrKeyUsageIssuerServer;
extern const DRM_CONST_STRING  g_dstrKeyUsageSignKeyFile;
extern const DRM_CONST_STRING  g_dstrCertSigningKey;
extern const DRM_CONST_STRING  g_dstrCertSigningKeyValue;
extern const DRM_CONST_STRING  g_dstrPlatformIdentifier;
extern const DRM_CONST_STRING  g_dstrMeteringID;
extern const DRM_CONST_STRING  g_dstrMeteringUrl;
extern const DRM_CONST_STRING  g_dstrOne;
extern const DRM_CONST_STRING  g_dstrSigningKeyPrivKeyXPath;
extern const DRM_CONST_STRING  g_dstrSigningKeyPubKeyXPath;

/*
** ASF Parsing Constants
*/

/*
** ASF Header Object Minimum Sizes 
*/

/*
**   SIZEOF( DRM_GUID )         ObjectID
** + SIZEOF( DRM_QWORD )        Object Size
*/
#define ASF_OBJ_MIN_SIZE        DRM_UI64LITERAL( 0, 24 )

/*
**   SIZEOF( DRM_GUID )         ObjectID
** + SIZEOF( DRM_QWORD )        Object Size
** + SIZEOF( DRM_DWORD )        Number of Sub Objects
** + SIZEOF( DRM_BYTE )         Alignment
** + SIZEOF( DRM_BYTE )         Architecture
*/
#define ASF_HDR_MIN_SIZE        DRM_UI64LITERAL( 0, 30 )

/*
**   SIZEOF( DRM_GUID )         ObjectID
** + SIZEOF( DRM_QWORD )        Object Size
** + SIZEOF( DRM_GUID )         id of header extension (clock) object
** + SIZEOF( DRM_WORD )         clock size/type
** + SIZEOF( DRM_DWORD )        clock data size
*/
#define ASF_HDREXT_MIN_SIZE     DRM_UI64LITERAL( 0, 46 )
#define ASF_V1_MIN_SIZE         DRM_UI64LITERAL( 0, 40 )
#define ASF_FILEPROP_MIN_SIZE   DRM_UI64LITERAL( 0, 104 )

/*
**   SIZEOF( DRM_GUID )         error correction type
** + SIZEOF( DRM_QWORD )        time offset
*/
#define ASF_STREAM_PROPERTIES_SKIP_SIZE  DRM_UI64LITERAL( 0, 24 )

/*
**   SIZEOF( DRM_GUID )         ObjectID
** + SIZEOF( DRM_QWORD )        Object Size
** + SIZEOF( DRM_GUID )         Stream Type
** + ASF_STREAM_PROPERTIES_SKIP_SIZE
** + SIZEOF( DRM_DWORD )        type-specific data length
** + SIZEOF( DRM_DWORD )        error correction data length  
** + SIZEOF( DRM_WORD )         Flags
** + SIZEOF( DRM_DWORD )        Reserved
*/
#define ASF_STREAM_PROPERTIES_MIN_SIZE  DRM_UI64LITERAL( 0, 86 )

/*********************************
** ASF Audio Format Tags
*********************************/
#define DRMTOOLS_ASF_AUDIO_TYPE_WMA           0x0161 /* Windows Media Audio */
#define DRMTOOLS_ASF_AUDIO_TYPE_GSM_AMR_FIXED 0x7A21 /* GSM-AMR (fixed bitrate, no SID) */
#define DRMTOOLS_ASF_AUDIO_TYPE_GSM_AMR_VBR   0x7A22 /* GSM-AMR (variable bitrate, including SID) */

/*********************************
** DRM-related ASF Header GUIDs
*********************************/
extern const DRM_GUID g_guidASFHeaderObject;
extern const DRM_GUID g_guidASFDataObject;
extern const DRM_GUID g_guidASFContentEncryptionObject;
extern const DRM_GUID g_guidASFExtendedContentEncryptionObject;
extern const DRM_GUID g_guidASFHeaderExtensionObject;
extern const DRM_GUID g_guidASFAdvancedContentEncryptionObject;
extern const DRM_GUID g_guidASFV4ContentEncryptionObject;
extern const DRM_GUID g_guidASFFilePropertiesObject;
extern const DRM_GUID g_guidASFDigitalSignatureObject;
extern const DRM_GUID g_guidSystemIDPlayReady;
extern const DRM_GUID g_guidASFReserved1;
extern const DRM_GUID g_guidASFStreamPropertiesObject;
extern const DRM_GUID g_guidASFStreamPropertiesObjectEx;
extern const DRM_GUID g_guidASFXStreamTypeAcmAudio;
extern const DRM_GUID g_guidASFXStreamTypeIcmVideo;

/********************************
** V4 and V4-in-V2 Header Tags **
********************************/
extern const DRM_CONST_STRING      g_dstrTagV4Data;
extern const DRM_CONST_STRING      g_dstrTagProtectInfo;
extern const DRM_CONST_STRING      g_dstrTagKeyLen;
extern const DRM_CONST_STRING      g_dstrTagAlgID;
extern const DRM_CONST_STRING      g_dstrTagV24LAURL;
extern const DRM_CONST_STRING      g_dstrTagV24LUIURL;
extern const DRM_CONST_STRING      g_dstrTagDS_ID;

typedef struct
{
    DRM_CHAR * m_pszScopeName;
    DRM_DWORD m_dwScopeID;


    /* The following are constants used only by the
    ** autobench tool for automated perf testing
    */
    DRM_DWORD m_dwABRPMCounter;
    DRM_DWORD m_dwABCPMCounter;
} DRM_SCOPE;

extern DRM_SCOPE g_Scopes[];

#ifndef PERF_SCOPE_ID
#define PERF_SCOPE_ID(ModuleID, FuncID, BlockID)    ((((ModuleID) & 0x0fff) << 19) + (((FuncID) & 0x0fff) << 7) + ((BlockID) & 0x007f))                               
#endif

#ifdef __cplusplus
}
#endif


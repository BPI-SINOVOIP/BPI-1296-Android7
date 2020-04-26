/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMSTRINGS_H__
#define __DRMSTRINGS_H__

#include <drmtypes.h>

ENTER_PK_NAMESPACE;

    /* Declare the characters first */
    extern const DRM_WCHAR              g_wchPathSeperator;
    extern const DRM_WCHAR              g_wchNull;
    extern const DRM_WCHAR              g_wchMinus;
    extern const DRM_WCHAR              g_wchPlus;
    extern const DRM_WCHAR              g_wchForwardSlash;
    extern const DRM_WCHAR              g_wchColon;
    extern const DRM_WCHAR              g_wchComma;
    extern const DRM_WCHAR              g_wchQuote;
    extern const DRM_WCHAR              g_wchSingleQuote;

    extern const DRM_WCHAR              g_wchNewLine;
    extern const DRM_WCHAR              g_wchBackSlash;
    extern const DRM_WCHAR              g_wch0;
    extern const DRM_WCHAR              g_wch1;
    extern const DRM_WCHAR              g_wch2;
    extern const DRM_WCHAR              g_wch9;
    extern const DRM_WCHAR              g_wcha;
    extern const DRM_WCHAR              g_wchd;
    extern const DRM_WCHAR              g_wchf;
    extern const DRM_WCHAR              g_wchh;
    extern const DRM_WCHAR              g_wchm;
    extern const DRM_WCHAR              g_wchn;
    extern const DRM_WCHAR              g_wchs;
    extern const DRM_WCHAR              g_wchy;
    extern const DRM_WCHAR              g_wchA;
    extern const DRM_WCHAR              g_wchF;
    extern const DRM_WCHAR              g_wchx;
    extern const DRM_WCHAR              g_wchX;
    extern const DRM_WCHAR              g_wchUnderscore;
    extern const DRM_WCHAR              g_wchz;
    extern const DRM_WCHAR              g_wchZ;
    extern const DRM_WCHAR              g_wchPeriod;
    extern const DRM_WCHAR              g_wchQuestionMark;
    extern const DRM_WCHAR              g_wchExclamationMark;
    extern const DRM_WCHAR              g_wchOpenParen;
    extern const DRM_WCHAR              g_wchCloseParen;
    extern const DRM_WCHAR              g_wchPound;
    extern const DRM_WCHAR              g_wchSpace;
    extern const DRM_WCHAR              g_wchTab;
    extern const DRM_WCHAR              g_wchLineFeed;
    extern const DRM_WCHAR              g_wchVerticalTab;
    extern const DRM_WCHAR              g_wchFormFeed;
    extern const DRM_WCHAR              g_wchCarriageReturn;
    extern const DRM_WCHAR              g_wchEqual;
    extern const DRM_WCHAR              g_wchOpenCurly;
    extern const DRM_WCHAR              g_wchCloseCurly;
    extern const DRM_WCHAR              g_wchLessThan;
    extern const DRM_WCHAR              g_wchGreaterThan;
    extern const DRM_WCHAR              g_wchLeftBracket;
    extern const DRM_WCHAR              g_wchRightBracket;
    extern const DRM_WCHAR              g_wchAsterisk;
    extern const DRM_WCHAR              g_wchPercent;
    extern const DRM_WCHAR              g_wchSemiColon;
    extern const DRM_WCHAR              g_wchAmpersand;
    extern const DRM_WCHAR              g_wchPipe;
    extern const DRM_WCHAR              g_rgwchWMDRM_RIGHT_ALLOW_COPY[];
    extern const DRM_WCHAR              g_rgwchCreatePMLicense[];

    /* ANSI */
    extern const DRM_CHAR               g_chForwardSlash;

    extern const DRM_ANSI_CONST_STRING  g_dastrRootPubkey;

#if DRM_SUPPORT_REVOCATION
    extern const DRM_LID                g_lidPCRevInfoRevocation;
    extern const DRM_LID                g_lidRevocationInfo;
    extern const DRM_GUID               g_guidRevocationTypeRevInfo;
    extern const DRM_GUID               g_guidRevocationTypeRevInfo2;
#if DRM_SUPPORT_DEVICE_REVOCATION
    extern const DRM_LID                g_lidDeviceRevocation;
    extern const DRM_CONST_STRING       g_dstrRevocationGuidDevice;
    extern const DRM_ANSI_CONST_STRING  g_dastrRevocationGuidDevice;
    extern const DRM_GUID               g_guidRevocationTypeDevice;
#endif               

#if DRM_SUPPORT_WMDRMNET
    extern const DRM_LID                g_lidWMDRMNET_Revocation;
    extern const DRM_CONST_STRING       g_dstrRevocationGuidWMDRMNET;
    extern const DRM_ANSI_CONST_STRING  g_dastrRevocationGuidWMDRMNET;
    extern const DRM_GUID               g_guidRevocationTypeWMDRMNET;
#endif

#if DRM_SUPPORT_APP_REVOCATION
    extern const DRM_LID                g_lidAppRevocation;
    extern const DRM_CONST_STRING       g_dstrRevocationGuidApp;
    extern const DRM_ANSI_CONST_STRING  g_dastrRevocationGuidApp;
    extern const DRM_GUID               g_guidRevocationTypeApp;
#endif /* DRM_SUPPORT_APP_REVOCATION */

    /* The following GUIDs are implicitly known CRL types that we should be able to extract from RevInfo blob */
    extern const DRM_GUID   g_guidRevocationTypeHDCP;
    extern const DRM_GUID   g_guidRevocationTypeGRL;
    extern const DRM_LID    g_lidGRLRevocation;

    /* PlayReady revocation GUIDs */
    extern const DRM_GUID   g_guidRevocationTypeRevInfo2;
    extern const DRM_LID    g_lidRevocationInfo2;
    extern const DRM_GUID   g_guidRevocationTypePlayReadySilverLightRuntime;
    extern const DRM_GUID   g_guidRevocationTypePlayReadySilverLightApplication;

    /* LIDs for PlayReady CRLs */
    extern const DRM_LID g_lidPRRTRevocation;
    extern const DRM_LID g_lidPRAppRevocation;

#endif /* DRM_SUPPORT_REVOCATION */
    extern const DRM_ID                 g_idSLKSST;

    /* Now declare the strings */
    
    extern const DRM_ANSI_CONST_STRING  g_adstrAttributeVersion;
    extern const DRM_ANSI_CONST_STRING  g_dastrAttributeType;
    extern const DRM_ANSI_CONST_STRING  g_adstrLicenseRespTag;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagLicense;
    extern const DRM_ANSI_CONST_STRING  g_dastrAttributeFALSE;
    extern const DRM_ANSI_CONST_STRING  g_adstrAttributeEncrypted;


    extern const DRM_ANSI_CONST_STRING  g_dastrFeaturesWMDRMTransmitter;
    extern const DRM_ANSI_CONST_STRING  g_dastrFeaturesWMDRMReceiver;


    extern const DRM_ANSI_CONST_STRING  g_dastrTagSLK;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagSLKID;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagSLKDATA;


    extern const DRM_CONST_STRING       g_dstrTagSLK;
    extern const DRM_CONST_STRING       g_dstrTagSLKID;
    extern const DRM_CONST_STRING       g_dstrTagSLKDATA;

    extern const DRM_CONST_STRING       g_dstrAttributeVersion;
    extern const DRM_CONST_STRING       g_dstrAttributeEncrypted;
    extern const DRM_CONST_STRING       g_dstrAttributeFALSE;
    extern const DRM_CONST_STRING       g_dstrLicVerAttrValue;
    extern const DRM_CONST_STRING       g_dstrLicenseRespTag;
    extern const DRM_CONST_STRING       g_dstrTagLicense;

    /* Script varibles used for license properties. */
    extern const DRM_CONST_STRING       g_dstrDRM_LS_BEGIN_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_COUNT_ATTR;

    extern const DRM_CONST_STRING       g_dstrDRM_LS_BEGDATE_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_ENDDATE_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_FIRSTUSE_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_FIRSTSTORE_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_EXPIRATION_AFTER_FIRSTUSE_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_MINAPPSECLEVEL_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_APPSEC_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_SAP_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_BURNTOCDCOUNT_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_TRANSFERCOUNT_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_PLAYCOUNT_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_COPYCOUNT_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_BEGINBURNTOCDCOUNT_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_BEGINTRANSFERCOUNT_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_BEGINPLAYCOUNT_ATTR;

    extern const DRM_CONST_STRING       g_dstrDRM_LS_DELETED_ATTR;

#if DRM_SUPPORT_ANTIROLLBACK_CLOCK
    extern const DRM_CONST_STRING       g_dstrDRM_LS_OLDSAVETIME_ATTR;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_MACHINE_DATETIME;
#endif

    extern const DRM_CONST_STRING       g_dstrDRM_LS_MACHINE_DATETIME_GE;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_MACHINE_DATETIME_LE;
    extern const DRM_CONST_STRING       g_dstrDRM_LS_APP_MINSECLEVEL_GE;
    extern const DRM_CONST_STRING       g_dstrEqual;

    extern const DRM_CONST_STRING       g_dstrDrmRestoreInfo;
    extern const DRM_CONST_STRING       g_dstrTagData;
    extern const DRM_CONST_STRING       g_dstrTagDataInclusionList;
    extern const DRM_CONST_STRING       g_dstrTagLID;

    extern const DRM_CONST_STRING       g_dstrTagIndex;
    extern const DRM_CONST_STRING       g_dstrTagEnablingbits;
    extern const DRM_CONST_STRING       g_dstrTagHashAlgorithm;
    extern const DRM_CONST_STRING       g_dstrTagSymValue;
    extern const DRM_CONST_STRING       g_dstrTagSymSig;

    extern const DRM_CONST_STRING       g_dstrTagPubkey;

    extern const DRM_CONST_STRING       g_dstrTagValue;
    extern const DRM_CONST_STRING       g_dstrTagSignature;
    extern const DRM_CONST_STRING       g_dstrTagHashAlg;
    extern const DRM_CONST_STRING       g_dstrTagSignAlg;
    extern const DRM_CONST_STRING       g_dstrSHA;
    extern const DRM_CONST_STRING       g_dstrMSDRM;
    extern const DRM_CONST_STRING       g_dstrMSDRM_CK;
    extern const DRM_CONST_STRING       g_dstrAttributeType;
    extern const DRM_CONST_STRING       g_dstrSavedDateTime;

    extern const DRM_CONST_STRING       g_dstrTagOEMPrivateKeys;
    extern const DRM_CONST_STRING       g_dstrTagKeyValue;
    extern const DRM_CONST_STRING       g_dstrTagRSAKeyValue;
    extern const DRM_CONST_STRING       g_dstrTagModulus;
    extern const DRM_CONST_STRING       g_dstrTagExponent;
    extern const DRM_CONST_STRING       g_dstrTagPrime0;
    extern const DRM_CONST_STRING       g_dstrTagPrime1;
    extern const DRM_CONST_STRING       g_dstrTagCRTExponent0;
    extern const DRM_CONST_STRING       g_dstrTagCRTExponent1;
    extern const DRM_CONST_STRING       g_dstrTagIQMP;
    extern const DRM_CONST_STRING       g_dstrTagPRIVKEY;
    extern const DRM_CONST_STRING       g_dstrXPathPrime0;
    extern const DRM_CONST_STRING       g_dstrXPathPrime1;
    extern const DRM_CONST_STRING       g_dstrXPathCRTExponent0;
    extern const DRM_CONST_STRING       g_dstrXPathCRTExponent1;
    extern const DRM_CONST_STRING       g_dstrXPathIQMP;
    extern const DRM_CONST_STRING       g_dstrWMDRMCertExponent;

#if DRM_SUPPORT_ANTIROLLBACK_CLOCK    
    extern const DRM_CONST_STRING       g_dstrCheckClockRollback;
    extern const DRM_CONST_STRING       g_dstrResetRollbackedClock;
#endif
    
    extern const DRM_CONST_STRING       g_dstrSetSavedDateTime;
    extern const DRM_CONST_STRING       g_dstrCreatePMLicense;
    extern const DRM_CONST_STRING       g_dstrAttributeVersion;
    extern const DRM_CONST_STRING       g_dstrVersion_1_0;
    extern const DRM_CONST_STRING       g_dstrTagIndivPrivkey;
    extern const DRM_CONST_STRING       g_dstrTagDevinfo;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagRevocation;
    extern const DRM_CONST_STRING       g_dstrTagError;
    extern const DRM_CONST_STRING       g_dstrXPathSigValue;
    extern const DRM_CONST_STRING       g_dstrFeatureIndivURL;
    extern const DRM_CONST_STRING       g_dstrCompatCert;
    extern const DRM_CONST_STRING       g_dstrCompatSecVersion;
    extern const DRM_CONST_STRING       g_dstrSubjectId;
    extern const DRM_CONST_STRING       g_dstrMaxSecLevel;
    extern const DRM_CONST_STRING       g_dstrAuthSecVer;
    extern const DRM_CONST_STRING       g_dstrAuthPubKey;
    extern const DRM_CONST_STRING       g_dstrXPathTagSignature;
    extern const DRM_CONST_STRING       g_dstrTagSignatureVal;
    extern const DRM_CONST_STRING       g_dstrXPathTagCertificateChain;
    extern const DRM_CONST_STRING       g_dstrTagSecurityLevel;
    extern const DRM_CONST_STRING       g_dstrAuthData;
    extern const DRM_CONST_STRING       g_dstrAuthHashAlg;
    extern const DRM_CONST_STRING       g_dstrAuthSignAlg;
    extern const DRM_CONST_STRING       g_dstrTagFallback;

    extern const DRM_CONST_STRING       g_dstrTagCertificate;
    extern const DRM_CONST_STRING       g_dstrXPathDataUniqueID;

    extern const DRM_CONST_STRING       g_dstrXPathManufacturer;
    extern const DRM_CONST_STRING       g_dstrXPathMake;
    extern const DRM_CONST_STRING       g_dstrXPathModel;
    extern const DRM_CONST_STRING       g_dstrXPathDistributor;
    extern const DRM_CONST_STRING       g_dstrXPathHardwareMajor;
    extern const DRM_CONST_STRING       g_dstrXPathHardwareMinor;
    extern const DRM_CONST_STRING       g_dstrXPathFirmwareMajor;
    extern const DRM_CONST_STRING       g_dstrXPathFirmwareMinor;
    extern const DRM_CONST_STRING       g_dstrXPathFeatureTemplate;
    extern const DRM_CONST_STRING       g_dstrXPathHeaderUplinks;
    extern const DRM_CONST_STRING       g_dstrXPathClientOldKeys;
    extern const DRM_CONST_STRING       g_dstrXPathClientEscrowKeys;
    extern const DRM_CONST_STRING       g_dstrXPathClientEscrowKey;
    extern const DRM_CONST_STRING       g_dstrXPathIndivServerEscrowKey;
    extern const DRM_CONST_STRING       g_dstrXPathDevicePublicKey;
    extern const DRM_CONST_STRING       g_dstrXPathOldKeysData;
    extern const DRM_CONST_STRING       g_dstrXPathIndivSvrPubKey;
    extern const DRM_CONST_STRING       g_dstrXPathSecureClock;
    extern const DRM_CONST_STRING       g_dstrXPathIndivType;
    extern const DRM_CONST_STRING       g_dstrXPathSecureStore;
    extern const DRM_CONST_STRING       g_dstrXPathMetering;
    extern const DRM_CONST_STRING       g_dstrXPathLicenseChain;
    extern const DRM_CONST_STRING       g_dstrFallback;

    extern const DRM_CONST_STRING       g_dstrTagDevCert;
    extern const DRM_CONST_STRING       g_dstrTagWrmHeader;
    extern const DRM_CONST_STRING       g_dstrAttributeHeaderXmlnsName;
    extern const DRM_CONST_STRING       g_dstrAttributeHeaderXmlnsValue;
    extern const DRM_CONST_STRING       g_dstrAttributeVersion2Value;
    extern const DRM_CONST_STRING       g_dstrAttributeVersion4Value;
    extern const DRM_CONST_STRING       g_dstrTagLAINFO;
    extern const DRM_CONST_STRING       g_dstrTagKID;
    extern const DRM_CONST_STRING       g_dstrTagV4DATA;
    extern const DRM_CONST_STRING       g_dstrTagLAURL;
    extern const DRM_CONST_STRING       g_dstrTagLUIURL;
    extern const DRM_CONST_STRING       g_dstrTagDSID;
    extern const DRM_CONST_STRING       g_dstrTagOEMData;
    extern const DRM_CONST_STRING       g_dstrTagContentID;
    extern const DRM_CONST_STRING       g_dstrTagChecksum;
    extern const DRM_CONST_STRING       g_dstrTagUplink;
    extern const DRM_CONST_STRING       g_dstrTagUplinks;

    extern const DRM_CONST_STRING       g_dstrTagLicenseRequest;
    extern const DRM_CONST_STRING       g_dstrTagAction;

    extern const DRM_CONST_STRING       g_dstrTagV1Challenge;
    extern const DRM_CONST_STRING       g_dstrDefaultV1Data;
    extern const DRM_CONST_STRING       g_dstrTagActionList;
    extern const DRM_CONST_STRING       g_dstrTagSecurityVersion;
    extern const DRM_CONST_STRING       g_dstrTagDevice;
    extern const DRM_CONST_STRING       g_dstrTagPrivateKey;
    extern const DRM_CONST_STRING       g_dstrKeyDataTag;
    extern const DRM_CONST_STRING       g_dstrGroupTag;
    extern const DRM_CONST_STRING       g_dstrTagMSDRMSignature;

    /*
    **  Devcert Non-indiv Tags
    */
    extern const DRM_CONST_STRING       g_dstrTagDNIUniqueID;
    extern const DRM_CONST_STRING       g_dstrTagMSDRMSignature;
    extern const DRM_CONST_STRING       g_dstrPrivateKeys;        
    extern const DRM_CONST_STRING       g_dstrCertTypeDevice;
    extern const DRM_CONST_STRING       g_dstrCerttypeGroup;
    extern const DRM_CONST_STRING       g_dstrCerttypeAuth;
    extern const DRM_CONST_STRING       g_dstrCerttypeAuthRoot;
    
    /*
    **  Devcert Non-indiv XPaths
    */
    extern const DRM_CONST_STRING       g_dstrXPathPubkeyData;
    extern const DRM_CONST_STRING       g_dstrXPathGCSecureClock;
    extern const DRM_CONST_STRING       g_dstrXPathGCSecureClockURL;
    extern const DRM_CONST_STRING       g_dstrXPathGCSecureClockPubKey;
    extern const DRM_CONST_STRING       g_dstrXPathGCMetering;
    extern const DRM_CONST_STRING       g_dstrXPathGCLicenseAcquisitionMode;
    extern const DRM_CONST_STRING       g_dstrXPathGCLicenseSyncMode;
    extern const DRM_CONST_STRING       g_dstrXPathGCSymmOptsMode;
    extern const DRM_CONST_STRING       g_dstrXPathGCEncryption;
    extern const DRM_CONST_STRING       g_dstrXPathGCSupportRevocation;
    extern const DRM_CONST_STRING       g_dstrXPathGCMaxChainDepth;
    extern const DRM_CONST_STRING       g_dstrXPathGCMaxLicenseSize;
    extern const DRM_CONST_STRING       g_dstrXPathGCMaxHeaderSize;
    extern const DRM_CONST_STRING       g_dstrXPathDeviceKeydata;
    extern const DRM_CONST_STRING       g_dstrXPathDACAuthID;

    extern const DRM_CONST_STRING       g_dstrReqTagClientInfo;
    extern const DRM_CONST_STRING       g_dstrReqTagClientID;
    extern const DRM_CONST_STRING       g_dstrReqTagClientVersion;
    extern const DRM_CONST_STRING       g_dstrReqTagAppSecurity;
    extern const DRM_CONST_STRING       g_dstrReqTagSubjectID1;
    extern const DRM_CONST_STRING       g_dstrReqTagSubjectID2;
    extern const DRM_CONST_STRING       g_dstrReqTagDrmkVersion;
    extern const DRM_CONST_STRING       g_dstrReqTagPlayReadyClientVersionData;
    extern const DRM_CONST_STRING       g_dstrReqTagWMDRMPDClientVersionData;
    extern const DRM_CONST_STRING       g_dstrReqTagSubjectID2Data;
    extern const DRM_CONST_STRING       g_dstrReqTagDrmkData;
    extern const DRM_CONST_STRING       g_dstrReqTagRevInfo;
	
    /*
    **  Devcert Support CRL  Tags
    */
    extern const DRM_CONST_STRING       g_dstrXPathGCSupportCRL;

    /* XML tags used in parsing licenses */
    extern const DRM_CONST_STRING       g_dstrLicense;
    extern const DRM_CONST_STRING       g_dstrGUID;
    extern const DRM_CONST_STRING       g_dstrLicensorInfoNode;
    extern const DRM_CONST_STRING       g_dstrLIData;
    extern const DRM_CONST_STRING       g_dstrMeta;

    /* Strings for various revocation queries */
    extern const DRM_CONST_STRING       g_dstrTagRevocation;
    extern const DRM_CONST_STRING       g_dstrTagRevocationInfo;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagRevocationInfo;
    extern const DRM_CONST_STRING       g_dstrAppRevocation;

    extern const DRM_CONST_STRING       g_dstrDeviceRevocationLicenseTag;
    extern const DRM_CONST_STRING       g_dstrWMDRMNETRevocationLicenseTag;

    extern const DRM_ANSI_CONST_STRING  g_dastrAppRevocation;
    extern const DRM_CONST_STRING       g_dstrDeviceRevocationList;
    extern const DRM_CONST_STRING       g_dstrDeviceRevocation;
    extern const DRM_ANSI_CONST_STRING  g_dastrDeviceRevocation;
    extern const DRM_CONST_STRING       g_dstrWMDRMNET_Revocation;
    extern const DRM_ANSI_CONST_STRING  g_dastrWMDRMNET_Revocation;

    extern const DRM_CONST_STRING       g_dstrTagRevInfoVersion;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagRevInfoVersion;
    extern const DRM_CONST_STRING       g_dstrTagSourceID;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagSourceID;


    extern const DRM_CONST_STRING       g_dstrMeteringId;
    
    
    extern const DRM_CONST_STRING       g_dstrPriority;
    extern const DRM_CONST_STRING       g_dstrIssueDate;
    extern const DRM_CONST_STRING       g_dstrContentPubKey;
    extern const DRM_CONST_STRING       g_dstrContentRevocation;
    extern const DRM_CONST_STRING       g_dstrCondition;
    extern const DRM_CONST_STRING       g_dstrAction;
    extern const DRM_CONST_STRING       g_dstrRestrictions;
    extern const DRM_CONST_STRING       g_dstrChainedEnablingBits;
    extern const DRM_CONST_STRING       g_dstrDrmRestoreInfoEnablingBits;
    extern const DRM_CONST_STRING       g_dstrCertChain;
    extern const DRM_CONST_STRING       g_dstrXPathTagCertificate;
    extern const DRM_CONST_STRING       g_dstrLISigHashAlgo;
    extern const DRM_CONST_STRING       g_dstrLISigAlgo;
    extern const DRM_CONST_STRING       g_dstrLISigValue;
    extern const DRM_CONST_STRING       g_dstrOnActionPath;
    extern const DRM_CONST_STRING       g_dstrXPathSymSig;
    extern const DRM_CONST_STRING       g_dstrXPathSymValue;


    extern const DRM_CONST_STRING       g_dstrSequenceNumber;
    extern const DRM_CONST_STRING       g_dstrLSPubKey;
    extern const DRM_CONST_STRING       g_dstrUplinkKid;
    extern const DRM_CONST_STRING       g_dstrChainedCheckSum;
    extern const DRM_CONST_STRING       g_dstrTempMeterId;
    extern const DRM_CONST_STRING       g_dstrPMLicenseVersionString;
    extern const DRM_ANSI_CONST_STRING  g_dastrPMLicenseVersionString;
    extern const DRM_ANSI_CONST_STRING  g_dastrXMRLicenseVersionString;
    extern const DRM_CONST_STRING       g_dstrRevAttribute;
    extern const DRM_CONST_STRING       g_dstrDerivedLicTemplate;
    extern const DRM_CONST_STRING       g_dstrLicDataDerivedTmpl;
    extern const DRM_CONST_STRING       g_dstrSourceID;
    extern const DRM_CONST_STRING       g_dstrInclusionList;

    /* XML tags used in writing Challenge and parsing Response for metering */
    extern const DRM_CONST_STRING       g_dstrTagMetering;
    extern const DRM_CONST_STRING       g_dstrTagHash;
    extern const DRM_CONST_STRING       g_dstrXPathMID;
    extern const DRM_CONST_STRING       g_dstrTagMID;
    extern const DRM_CONST_STRING       g_dstrTagTID;
    extern const DRM_CONST_STRING       g_dstrTagCmd;
    extern const DRM_CONST_STRING       g_dstrTagPassword;
    extern const DRM_CONST_STRING       g_dstrTagRecords;
    extern const DRM_CONST_STRING       g_dstrTagPartialData;
    extern const DRM_CONST_STRING       g_dstrTagURL;
    extern const DRM_CONST_STRING       g_dstrLabelValue;
    extern const DRM_CONST_STRING       g_dstrHMAC;
    extern const DRM_CONST_STRING       g_dstrChallenge;
    extern const DRM_CONST_STRING       g_dstrResponse;
    extern const DRM_CONST_STRING       g_dstr1;
    extern const DRM_CONST_STRING       g_dstrReset;

    extern const DRM_ANSI_CONST_STRING  g_dastrTagMeterCert;
    extern const DRM_CONST_STRING       g_dstrTagMeterCert;

    extern const DRM_CONST_STRING       g_dstrTagCertificateChain;



    extern const DRM_ANSI_CONST_STRING  g_dastrAttributeAlgorithm;
    extern const DRM_ANSI_CONST_STRING  g_dastrAttributeVersionWMDRM;      
    extern const DRM_ANSI_CONST_STRING  g_dastrTagEscrowedKeys;            
    extern const DRM_ANSI_CONST_STRING  g_dastrKeyUsageSignCert;           
    extern const DRM_ANSI_CONST_STRING  g_dastrKeyUsageSignCRL;            
    extern const DRM_ANSI_CONST_STRING  g_dastrKeyUsageEncryptKey;         
    extern const DRM_ANSI_CONST_STRING  g_dastrOne;                        
    extern const DRM_ANSI_CONST_STRING  g_dastrPrefixManufacturer;         
    extern const DRM_ANSI_CONST_STRING  g_dastrTagCanonicalization;        
    extern const DRM_ANSI_CONST_STRING  g_dastrTagCertificateCollection;   
    extern const DRM_ANSI_CONST_STRING  g_dastrTagDigestMethod;            
    extern const DRM_ANSI_CONST_STRING  g_dastrURIDSigSHA1;                
    extern const DRM_ANSI_CONST_STRING  g_dastrTagDigestValue;             
    extern const DRM_ANSI_CONST_STRING  g_dastrTagSignatureValue;          
    extern const DRM_ANSI_CONST_STRING  g_dastrTagKeyInfo;                 
    extern const DRM_ANSI_CONST_STRING  g_dastrTagSymmetricKey;            
    extern const DRM_ANSI_CONST_STRING  g_dastrTagPublicKey;               
    extern const DRM_ANSI_CONST_STRING  g_dastrTagPrivateKey;              
    extern const DRM_ANSI_CONST_STRING  g_dastrTagKeyValue;               
    extern const DRM_ANSI_CONST_STRING  g_dastrTagRSAKeyValue;             
    extern const DRM_ANSI_CONST_STRING  g_dastrTagModulus;                 
    extern const DRM_ANSI_CONST_STRING  g_dastrTagExponent;
    extern const DRM_ANSI_CONST_STRING  g_dastrOpenEndTag;
    extern const DRM_ANSI_CONST_STRING  g_dastrOpenCDATATag;
    extern const DRM_ANSI_CONST_STRING  g_dastrCloseCDATATag;  

    extern const DRM_ANSI_CONST_STRING  g_dastrTagManufacturerName;           
    extern const DRM_ANSI_CONST_STRING  g_dastrTagManufacturerData;           
    extern const DRM_ANSI_CONST_STRING  g_dastrTagModelName;                  
    extern const DRM_ANSI_CONST_STRING  g_dastrTagModelNumber;    
    extern const DRM_ANSI_CONST_STRING  g_dastrTagHardwareID;
    extern const DRM_ANSI_CONST_STRING  g_dastrURIRSASHA1;                    
    extern const DRM_ANSI_CONST_STRING  g_dastrURIRSASHA1_Old;                
    extern const DRM_ANSI_CONST_STRING  g_dastrTagReference;                  
    extern const DRM_ANSI_CONST_STRING  g_dastrTagTransforms;                 
    extern const DRM_ANSI_CONST_STRING  g_dastrTagTransform;                  
    extern const DRM_ANSI_CONST_STRING  g_dastrURITransformMSCert;            
    extern const DRM_ANSI_CONST_STRING  g_dastrURITransformMSCertColl;        
    extern const DRM_ANSI_CONST_STRING  g_dastrURITransformMSCert_Old;        
    extern const DRM_ANSI_CONST_STRING  g_dastrURITransformC14N;              
    extern const DRM_ANSI_CONST_STRING  g_dastrTagWMDRMCertificate;           
    extern const DRM_ANSI_CONST_STRING  g_dastrTagWMDRMData;                  
    extern const DRM_ANSI_CONST_STRING  g_dastrTagWMDRMCertPublicKey;         
    extern const DRM_ANSI_CONST_STRING  g_dastrTagWMDRMCertSecurityVersion;   
    extern const DRM_ANSI_CONST_STRING  g_dastrSecurityVersion;   
    extern const DRM_ANSI_CONST_STRING  g_dastrTagWMDRMCertSecurityLevel;     
    extern const DRM_ANSI_CONST_STRING  g_dastrTagWMDRMCertSerialNumber;      
    extern const DRM_ANSI_CONST_STRING  g_dastrTagWMDRMCertFeatures;          
    extern const DRM_ANSI_CONST_STRING  g_dastrTagWMDRMCertKeyUsage;          

    extern const DRM_ANSI_CONST_STRING  g_dastrTagSignature;       
    extern const DRM_ANSI_CONST_STRING  g_dastrTagSignatureMethod; 
    extern const DRM_ANSI_CONST_STRING  g_dastrTagSignedInfo;      
    extern const DRM_ANSI_CONST_STRING  g_dastrURIC14N;            
    extern const DRM_ANSI_CONST_STRING  g_dastrVersionWMDRM;       
    extern const DRM_ANSI_CONST_STRING  g_dastrWMDRMCertExponent;  


    extern const DRM_ANSI_CONST_STRING  g_dastrFeatureSampleProtection;
    extern const DRM_ANSI_CONST_STRING  g_dastrFeatureExport;
    extern const DRM_ANSI_CONST_STRING  g_dastrFeatureExportIncl;

    extern const DRM_ANSI_CONST_STRING  g_dastrPrefixMicrosoftCert;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagUnsignedTemplate;
    extern const DRM_ANSI_CONST_STRING  g_dastrMSNDRootPubKeyB64;

    /* tags used for sync lists */

    extern const DRM_CONST_STRING       g_dstrTagSyncList;

    extern const DRM_CONST_STRING       g_dstrBackup;
    extern const DRM_CONST_STRING       g_dstrRestore;
    extern const DRM_CONST_STRING       g_dstrRootSigValueVersionTag;
    extern const DRM_CONST_STRING       g_dstrRootSigValueVersionVal;
    extern const DRM_CONST_STRING       g_dstrPFRootTag;
    extern const DRM_CONST_STRING       g_dstrDACPubKeyTag;
    extern const DRM_CONST_STRING       g_dstrAuthRoot;
    extern const DRM_CONST_STRING       g_dstrTagNameValue;
    extern const DRM_CONST_STRING       g_dstrXPathTagSecurityLevel;
    extern const DRM_CONST_STRING       g_dstrTagDataId;
    extern const DRM_CONST_STRING       g_dstrTagDataPubKey;
    extern const DRM_CONST_STRING       g_dstrFallBack;
    extern const DRM_CONST_STRING       g_dstrTagName;
    extern const DRM_CONST_STRING       g_dstrManufacturer;
    extern const DRM_CONST_STRING       g_dstrModel;
    extern const DRM_CONST_STRING       g_dstrHwMajor;
    extern const DRM_CONST_STRING       g_dstrHwMinor;
    extern const DRM_CONST_STRING       g_dstrFwMajor;
    extern const DRM_CONST_STRING       g_dstrFwMinor;
    extern const DRM_CONST_STRING       g_dstrFeatures;
    extern const DRM_CONST_STRING       g_dstrSecureClockNotSet;
    extern const DRM_CONST_STRING       g_dstrSecureClockSet;
    extern const DRM_CONST_STRING       g_dstrSecureClockNeedsRefresh;
    extern const DRM_CONST_STRING       g_dstrTemplate;
    extern const DRM_CONST_STRING       g_dstrIndivPubKey;
    extern const DRM_CONST_STRING       g_dstrUniqueId;
    extern const DRM_CONST_STRING       g_dstrUnsignedtemplate;
    extern const DRM_CONST_STRING       g_dstrTagDataSecVer;
    extern const DRM_CONST_STRING       g_dstrExprFuncDateAdd;
    extern const DRM_CONST_STRING       g_dstrExprFuncDateDiff;
    extern const DRM_CONST_STRING       g_dstrExprFuncMin;
    extern const DRM_CONST_STRING       g_dstrExprFuncMax;
    extern const DRM_CONST_STRING       g_dstrExprFuncDatePart;
    extern const DRM_CONST_STRING       g_dstrExprFuncVersionCompare;
    extern const DRM_CONST_STRING       g_dstrExprFuncDeleteLicense;
    extern const DRM_CONST_STRING       g_dstrExprFuncExists;    

    /* Strings for license evaluator well known actions */
    extern const DRM_CONST_STRING       g_dstrLicEvalOnSelect;
    extern const DRM_CONST_STRING       g_dstrLicEvalOnDelete;
    extern const DRM_CONST_STRING       g_dstrLicEvalOnStore;
    extern const DRM_CONST_STRING       g_dstrLicEvalOnAction;
    extern const DRM_CONST_STRING       g_dstrLicEvalOnClockRollback;

    /* Strings for expression evaluator variable evaluation */
    extern const DRM_CONST_STRING       g_dstrDRM_PREFIX;
    extern const DRM_CONST_STRING       g_dstrDRMK_PREFIX;
    extern const DRM_CONST_STRING       g_dstrMACHINE_PREFIX;
    extern const DRM_CONST_STRING       g_dstrAPP_PREFIX;
    extern const DRM_CONST_STRING       g_dstrSECSTATE_PREFIX;
    extern const DRM_CONST_STRING       g_dstrLICENSE_PREFIX;
    extern const DRM_CONST_STRING       g_dstrCONTENT_PREFIX;
    extern const DRM_CONST_STRING       g_dstrDEVICE_PREFIX;
    extern const DRM_CONST_STRING       g_dstrPMLICENSE_PREFIX;
    extern const DRM_CONST_STRING       g_dstrSECURETIME_PREFIX;
    extern const DRM_CONST_STRING       g_dstrPLAYLISTBURN_PREFIX;

#if DRM_SUPPORT_SST_REDUNANCY
    extern const DRM_CONST_STRING       g_dstrSSTREDUNDANCY_PREFIX;
#endif

    extern const DRM_CONST_STRING       g_dstrExprVarVersion;
    extern const DRM_CONST_STRING       g_dstrExprVarParameter;
    extern const DRM_CONST_STRING       g_dstrExprVarReason;
    extern const DRM_CONST_STRING       g_dstrExprVarBBMSDRMVersion;
    extern const DRM_CONST_STRING       g_dstrExprVarDateTime;
    extern const DRM_CONST_STRING       g_dstrExprVarCount;
    extern const DRM_CONST_STRING       g_dstrExprVarMinSecLevel;
    extern const DRM_CONST_STRING       g_dstrExprVarSecLevel;
    extern const DRM_CONST_STRING       g_dstrExprVarAppSecLevel;
    extern const DRM_CONST_STRING       g_dstrExprVarSubjID;
    extern const DRM_CONST_STRING       g_dstrExprVarAppSubjID;
    extern const DRM_CONST_STRING       g_dstrExprVarGlobal;
    extern const DRM_CONST_STRING       g_dstrExprVarSaveDateTime;
    extern const DRM_CONST_STRING       g_dstrExprVarSavedDateTime;
    extern const DRM_CONST_STRING       g_dstrExprVarDRMReason;
    extern const DRM_CONST_STRING       g_dstrExprVarRights;
    extern const DRM_CONST_STRING       g_dstrExprVarExpiryDate;
    extern const DRM_CONST_STRING       g_dstrExprVarInGracePeriod;
    extern const DRM_CONST_STRING       g_dstrExprVarGpStartTime;
    extern const DRM_CONST_STRING       g_dstrExprVarIncrementCount;    
    extern const DRM_CONST_STRING       g_dstrExprVarDevReg;    
    extern const DRM_ANSI_CONST_STRING  g_dastrTagCDATA;
    extern const DRM_CONST_STRING       g_dstrTagCDATA;
    extern const DRM_ANSI_CONST_STRING  g_dastrTagCDATAOpenBracket;
    extern const DRM_CONST_STRING       g_dstrTagCDATAOpenBracket;
    extern const DRM_CONST_STRING       g_dstrOpenTag;
    extern const DRM_CONST_STRING       g_dstrSpace;
    extern const DRM_CONST_STRING       g_dstrOpenEndTag;
    extern const DRM_CONST_STRING       g_dstrCloseTag;
    extern const DRM_CONST_STRING       g_dstrEqualQuote;
    extern const DRM_CONST_STRING       g_dstrQuote;
    extern const DRM_CONST_STRING       g_dstrOpenCDATATag;
    extern const DRM_CONST_STRING       g_dstrCloseCDATATag;

    extern const DRM_CONST_STRING       g_dstrTagDRMReqRoot;
    extern const DRM_CONST_STRING       g_dstrResponse;
    extern const DRM_CONST_STRING       g_dstrStatus;
    extern const DRM_CONST_STRING       g_dstrFlag;
    extern const DRM_CONST_STRING       g_dstrTagGmtTime;
    extern const DRM_CONST_STRING       g_dstrTagRefreshDate;

    /* Output restriction strings */
    extern const DRM_CONST_STRING       g_dstrLabelGuid;
    extern const DRM_CONST_STRING       g_dstrLabelLevel;
    extern const DRM_CONST_STRING       g_dstrTagUncompressedDigitalVideo;
    extern const DRM_CONST_STRING       g_dstrTagCompressedDigitalVideo;
    extern const DRM_CONST_STRING       g_dstrTagUncompressedDigitalAudio;
    extern const DRM_CONST_STRING       g_dstrTagCompressedDigitalAudio;
    extern const DRM_CONST_STRING       g_dstrTagAnalogVideo;
    extern const DRM_CONST_STRING       g_dstrTagCopy;
    extern const DRM_CONST_STRING       g_dstrTagExtensionList;
    extern const DRM_CONST_STRING       g_dstrTagExtension;
    extern const DRM_CONST_STRING       g_dstrTagIncludeList;
    extern const DRM_CONST_STRING       g_dstrTagInclude;
    extern const DRM_CONST_STRING       g_dstrTagExcludeList;
    extern const DRM_CONST_STRING       g_dstrTagExclude;
    
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_NONE;
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_PLAYBACK;
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_COLLABORATIVE_PLAY;
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_COPY;
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_COPY_TO_SDMI_DEVICE;
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_COPY_TO_NON_SDMI_DEVICE;
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_CREATE_THUMBNAIL_IMAGE;

#if DRM_SUPPORT_PLAYLIST_BURN
    extern const DRM_CONST_STRING      g_dstrPlaylistBurnCount;
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_PLAYLIST_BURN;
#endif

#if DRM_SUPPORT_PMLICENSE
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_COPY_TO_SDMI_DEVICE;
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_COPY_TO_NON_SDMI_DEVICE;
#endif

#if DRM_BACKUP_RESTORE_SUPPORT
    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_BACKUP;
#endif

    extern const DRM_CONST_STRING      g_dstrWMDRM_RIGHT_COPY_TO_CD;    

    extern const DRM_CONST_STRING      g_dstrMSRootPubKey;

    extern const DRM_BYTE              g_rgbMSPlayReadyRootIssuerPubKey[];

/* License State strings */
    extern const DRM_CONST_STRING      g_dstrTagLicenseStateRequestRoot;
    extern const DRM_CONST_STRING      g_dstrTagCanBind;

/*Device License Revocation Blob Strings*/

    extern const DRM_CONST_STRING      g_dstrTagDeviceLicenseRevocationRoot;
    extern const DRM_CONST_STRING      g_dstrTagRevocationPubKey;

    extern const DRM_CONST_STRING      g_dstrInitialExtensibleStateAttributeName;

    extern const DRM_DWORD             g_dwWMDRMPDVersionMajor;
    extern const DRM_DWORD             g_dwWMDRMPDVersionMinor;
    extern const DRM_DWORD             g_dwWMDRMPDVersionBuild;
    extern const DRM_DWORD             g_dwWMDRMPDVersionRevision;
    

/*
** Actions GUIDs. Used with DRM_LEVL_PerformOperationsXMR() API
**
*/
    DRM_DECLARE_GUID( DRM_ACTION_PLAY );
    DRM_DECLARE_GUID( DRM_ACTION_COPY );
    DRM_DECLARE_GUID( DRM_ACTION_CREATE_THUMBNAIL );
    DRM_DECLARE_GUID( DRM_ACTION_COLLABORATIVE_PLAY );
    DRM_DECLARE_GUID( DRM_ACTION_PLAYLIST_BURN );
    DRM_DECLARE_GUID( DRM_ACTION_COPY_TO_CD );
    DRM_DECLARE_GUID( DRM_ACTION_CREATE_PM_LICENSE );
    DRM_DECLARE_GUID( DRM_ACTION_BACKUP_RESTORE );
    DRM_DECLARE_GUID( DRM_ACTION_COPY_TO_PC );
    DRM_DECLARE_GUID( DRM_ACTION_COPY_TO_DEVICE );

EXIT_PK_NAMESPACE;

#endif /* __DRMSTRINGS_H__ */

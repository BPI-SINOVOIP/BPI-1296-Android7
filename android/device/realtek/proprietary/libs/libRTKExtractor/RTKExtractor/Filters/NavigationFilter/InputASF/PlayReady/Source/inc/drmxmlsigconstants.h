/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_XML_SIG_CONSTANTS_H
#define __DRM_XML_SIG_CONSTANTS_H

#include <drmeccp256.h>

ENTER_PK_NAMESPACE;

/* Well known WMRM ECC-256 public key */
extern const PUBKEY_P256 g_oWMRMECC256PubKey;

/*
** -------------------------------------------------------------
** XML strings used in the construction of signature XML segment
** -------------------------------------------------------------
*/
extern const DRM_ANSI_CONST_STRING g_dastrSigRootTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigSignedInfoTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigCanonicalizationMethodTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigSignatureMethodTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigReferenceTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigDigestMethodTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigDigestValueTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigSignatureValueTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigKeyValueTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigECCKeyValueTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigPublicKeyTag;


extern const DRM_ANSI_CONST_STRING g_dastrSigRootAttribName;
extern const DRM_ANSI_CONST_STRING g_dastrSigRootAttribValue;

extern const DRM_ANSI_CONST_STRING g_dastrSigSignedInfoAttribName;
extern const DRM_ANSI_CONST_STRING g_dastrSigSignedInfoAttribValue;

extern const DRM_ANSI_CONST_STRING g_dastrSigCanonicalizationMethodAttribName;
extern const DRM_ANSI_CONST_STRING g_dastrSigCanonicalizationMethodAttribValue;

extern const DRM_ANSI_CONST_STRING g_dastrSigSignatureMethodAttribName;
extern const DRM_ANSI_CONST_STRING g_dastrSigSignatureMethodAttribValue;

extern const DRM_ANSI_CONST_STRING g_dastrSigDigestMethodAttribName;
extern const DRM_ANSI_CONST_STRING g_dastrSigDigestMethodAttribValue;

extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoAttribName;
extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoAttribValue;



/*
** -----------------------------------------------------------------------
** XML strings used in the construction of XML encrypted data segment
** -----------------------------------------------------------------------
*/
extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedDataTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptionMethodTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedKeyTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigCipherDataTag;

extern const DRM_ANSI_CONST_STRING g_dastrSigCipherValueTag;


extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedDataAttrib1Name;
extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedDataAttrib1Value;

extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedDataAttrib2Name;
extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedDataAttrib2Value;

extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedDataAttrib3Name;
extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedDataAttrib3Value;

extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptionMethodAttrib1Name;
extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptionMethodAttrib1Value;

extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptionMethodAttrib2Name;
extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptionMethodAttrib2Value;

extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoAttrib1Name;
extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoAttrib1Value;

extern const DRM_ANSI_CONST_STRING g_dastrSigKeyNameTag;
extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoKeyName1;
extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoKeyName2;
extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoKeyName3;

extern const DRM_ANSI_CONST_STRING g_dastrKeyInfoKeyNamePath;

extern const DRM_ANSI_CONST_STRING g_dastrSigReferenceAttribName;
extern const DRM_ANSI_CONST_STRING g_dastrSigReferenceAttribValue;

extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedKeyAttribName;
extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedKeyAttribValue;

extern const DRM_ANSI_CONST_STRING g_dastrRSAPrefixMSLP;
extern const DRM_ANSI_CONST_STRING g_dastrRSAURIMSLP;
extern const DRM_ANSI_CONST_STRING g_dastrTagRSAKeyPair;

/*
** --------------------------------------------------------
** XML strings used in the parsing of signature XML segment
** --------------------------------------------------------
*/
extern const DRM_ANSI_CONST_STRING g_dastrSigSignedInfoPath;

extern const DRM_ANSI_CONST_STRING g_dastrSigSignatureValuePath;

extern const DRM_ANSI_CONST_STRING g_dastrSigHashPath;

extern const DRM_ANSI_CONST_STRING g_dastrSigPublicKeyPath;



/*
** -------------------------------------------------------------
** XML strings used in the parsing of XML encrypted data segment
** -------------------------------------------------------------
*/
extern const DRM_ANSI_CONST_STRING g_dastrSigKeyInfoPath;

extern const DRM_ANSI_CONST_STRING g_dastrSigEncryptedKeyPath;

extern const DRM_ANSI_CONST_STRING g_dastrSigCipherValuePath;

EXIT_PK_NAMESPACE;

#endif /* __DRM_XML_SIG_CONSTANTS_H */


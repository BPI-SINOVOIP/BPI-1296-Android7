/**@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


DRM_RESULT DRMTOOLS_CreateKeyPair( 
    IN      DRM_CRYPTO_CONTEXT  *f_pcontextCrypto,
    IN  OUT PUBKEY              *f_ppubkey,
    IN  OUT PRIVKEY             *f_pprivkey,
        OUT DRM_STRING          *f_pdstrPubKey,
        OUT DRM_STRING          *f_pdstrPrivKey);

DRM_RESULT  DRMTOOLS_ReallocXmlContext(
    IN      _XMBContext        **f_ppbXmlContext);

DRM_RESULT DRMTOOLS_ReadFileData(
    IN  const   DRM_CONST_STRING    *f_pdstrFileName,
        OUT     DRM_STRING          *f_pdstrFileData);

DRM_RESULT DRMTOOLS_TestKeyPair( 
    IN  PUBKEY *f_ppubkey, 
    IN  PRIVKEY *f_pprivkey, 
    IN  DRM_CRYPTO_CONTEXT  *f_contextCrypto );

#ifdef __cplusplus
        }
#endif



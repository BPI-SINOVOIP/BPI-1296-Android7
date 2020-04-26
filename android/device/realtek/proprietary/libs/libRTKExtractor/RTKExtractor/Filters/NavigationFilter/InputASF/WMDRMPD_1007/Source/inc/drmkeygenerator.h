/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMKEYGEN_H__
#define __DRMKEYGEN_H__


#ifdef __cplusplus
extern "C" {
#endif


#define DRM_CONTENT_KEY_LENGTH  7
#define DRM_KEYSEED_LEN        40

/**************
  NOTE:  All Keygen functions include a NULL termination character
  in the character count and the string on function return
**************/

/**********************************************************************
** Function:    DRM_KG_GenerateKeyseed
** Synopsis:    Generate pseudo random keyseed
** Arguments:   [pwszKeyseed] -- Pointer of Unicode string to receive the keyseed
**              [pchKeyseed] -- size in chars of pwszKeyseed
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************
*/
DRM_RESULT DRM_API DRM_KG_GenerateKeyseed(
       OUT DRM_WCHAR *pwszKeyseed,
    IN OUT DRM_DWORD *pcchKeyseed);

/**********************************************************************
** Function:    DRM_KG_GenerateKeyID
** Synopsis:    Generate pseudo random Key ID 
** Arguments:   [pwszKeyseed] -- Pointer of Unicode string to receive the keyseed
**              [pchKeyseed] -- size in chars of pwszKeyseed
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************
*/
DRM_RESULT DRM_API DRM_KG_GenerateKeyID(
       OUT DRM_WCHAR *pwszKeyID,           /* base64 encoded string */
    IN OUT DRM_DWORD *pcchKeyID);


/*******************************************************************************
** Function:   DRMGenerateKey
** Parameters: 
**      [in] pbKeySeed:    A byte array representing seed.
**      [in] cbKeySeed:    The length of the pbKeySeed field
**      [in] pbKId:        The key id (KID). Normally, this is base-64 encoded guid.
**      [in] cbKId:        The length of the pbKId field.
**      [out] pwszContentKey: base64  
**      [in] pcchContentKey: 
** Purpose:
**      To generate the key for a given kid based on a license key seed of length up to 20 bytes.
********************************************************************************/
DRM_RESULT DRM_API DRM_KG_GenerateContentKey(
    IN const   DRM_WCHAR *pwszKeySeed,          /* base64 encoded string */
    IN         DRM_DWORD  cchKeySeed, 
    IN const   DRM_WCHAR *pwszKeyID,            /* base64 encoded string */
    IN         DRM_DWORD  cchKeyID, 
       OUT     DRM_WCHAR *pwszContentKey,       /* base64 encoded string */
    IN OUT     DRM_DWORD *pcchContentKey);


/*******************************************************************************
** Function:    DRM_KG_GenerateSigningKeys
** Synopsis:    Generate signing key pair. Generated key pair will be base64 encoded
** Arguments:   [pCrypto] -- pointer to PKCrypto context
**              [pwszPrivKey] -- pointer to unicode string to receive the generated priv key
**              [pcchPrivKey] -- length in char of pwszPrivKey
**              [pwszPubKey] -- pointer to unicode string to receive the generated pub key
**              [pcchPubKey] -- length in char of pwszPubkey
** Returns:     DRM_SUCCESS on success
** Notes:       
********************************************************************************/
DRM_RESULT DRM_API DRM_KG_GenerateSigningKeys(
    IN     DRM_CRYPTO_CONTEXT *pCrypto,
       OUT DRM_WCHAR          *pwszPrivKey,         /* base64 encoded string */
    IN OUT DRM_DWORD          *pcchPrivKey,
       OUT DRM_WCHAR          *pwszPubKey,          /* base64 encoded string */
    IN OUT DRM_DWORD          *pcchPubKey);


#ifdef __cplusplus
}
#endif

#endif /*__DRMKEYGEN_H__ */


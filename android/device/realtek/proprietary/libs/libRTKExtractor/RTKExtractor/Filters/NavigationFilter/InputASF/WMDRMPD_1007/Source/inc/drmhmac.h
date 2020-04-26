/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMHMAC_H__
#define __DRMHMAC_H__

#include <drmsha1.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct __tagHMAC_CONTEXT
{
	SHA_CONTEXT	shaContext;
	DRM_BYTE    shaDigest[__CB_DECL(SHA_DIGEST_LEN)];
	DRM_BYTE    rgbBuffer[__CB_DECL(SHA_BLOCK_SIZE)]; /* This is 64 bytes.  */
} HMAC_CONTEXT;


/*********************************************************************
**
**  Function:  HMACInit
**
**  Synopsis:  HMAC is a keyed-hash message authentication code.  It creates a
**             unique hash of data based on the data and an arbitrary key.
**
**  Arguments:  
**     [pcontextHMAC] -- Context structure to contain a running HMAC calculation.
**     [pbKey] -- Pointer a array of bytes to as the HMAC key.
**     [cbKey] -- Length of pbKey in bytes.
**
**  Notes:  This HMAC code uses SHA-1 as the Secure Hash
**
*********************************************************************/
DRM_RESULT DRM_API DRM_HMAC_Init(
    IN OUT   HMAC_CONTEXT *pcontextHMAC,
    IN const DRM_BYTE     *pbKey,
    IN       DRM_UINT      cbKey);


/*********************************************************************
**
**  Function:  DRM_HMAC_Update
**
**  Synopsis:  
**
**  Arguments:  
**     [pcontextHMAC] -- Valid HMAC_CONTEXT create with DRM_HMAC_Init
**     [pbData] -- Data to add the the secure hash
**     [cbData] -- Length of pbData in bytes
**
*********************************************************************/
DRM_RESULT DRM_API DRM_HMAC_Update(
    IN OUT   HMAC_CONTEXT *pcontextHMAC,
    IN const DRM_BYTE     *pbData,
    IN       DRM_UINT      cbData);

/*********************************************************************
**
**  Function:  DRM_HMAC_Finalize
**
**  Synopsis:  Closes the MHAC context and returns the computed secure hash.
**
**  Arguments:  
**     [pcontextHMAC] -- Valid HMAC_CONTEXT created with DRM_HMAC_Init
**     [pbKeyedHash] -- Pointer to byte array that will recieve the secure hash
**     [cbKeyedHash] -- Length of pbKeyedHash in bytes
**
**  Notes:  cbKeyedHash must be less than or equal to SHA_DIGEST_LEN and greater than 0.
*********************************************************************/
DRM_RESULT DRM_API DRM_HMAC_Finalize(
    IN OUT HMAC_CONTEXT *pcontextHMAC,
       OUT DRM_BYTE     *pbKeyedHash,
    IN     DRM_UINT      cbKeyedHash );


#ifdef __cplusplus
}
#endif

#endif /* __DRMHMAC_H__ */

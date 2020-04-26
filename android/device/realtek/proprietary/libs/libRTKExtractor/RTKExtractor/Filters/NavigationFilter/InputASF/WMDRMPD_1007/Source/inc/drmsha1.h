/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMSHA_1_H__
#define __DRMSHA_1_H__

#ifdef __cplusplus
extern "C" {
#endif


#define SHA_DIGEST_LEN 20	/*5 32-bit numbers which is 20 BYTES */
#define SHA_BLOCK_SIZE 64	/*Sha operates on 64 BYTE blocks */

#define SHA_B64ENC_DIGEST_LEN CCH_BASE64_EQUIV( SHA_DIGEST_LEN )


/**********************************************************************
** Struct:      SHA_CONTEXT
**
** Synopsis:    Maintains the running state of a sha operations
**
** Members:		[dwHighByteCount] -- High order of the byte count
**				[dwLowByteCount] -- Low order of the byte count
**				[bTempBuffer] -- Maximum temp buffer we may need 
**				[ABCDE] -- current SHA state
**
***********************************************************************/
typedef struct __tagSHA_CONTEXT
{
	DRM_DWORD dwHighByteCount;
	DRM_DWORD dwLowByteCount;	
	DRM_BYTE  bTempBuffer[__CB_DECL(SHA_BLOCK_SIZE)];
	DRM_DWORD ABCDE[5];
} SHA_CONTEXT;


/*********************************************************************
**
**  Function:  DRM_SHA_Init
**
**  Synopsis:  Start a clean new instance of Sha
**
**  Arguments:  
**     [pShaContext] -- Pointer to context to hold current state of sha
**
**  Returns:  None
**
*********************************************************************/

DRM_VOID DRM_API DRM_SHA_Init(
    IN OUT SHA_CONTEXT* pShaContext );

/*********************************************************************
**
**  Function:  DRM_SHA_Update
**
**  Synopsis:  
**
**  Arguments:  
**     [pbData] -- array of bytes to add to the sha computation
**     [cbData] -- length of pbData in bytes
**     [pShaContext] -- running sha context
**
**  Returns:  
**
*********************************************************************/

DRM_VOID DRM_API DRM_SHA_Update(
    IN const DRM_BYTE    *pbData,
    IN       DRM_DWORD    cbData,
    IN OUT   SHA_CONTEXT *pShaContext );


DRM_VOID DRM_API DRM_SHA_UpdateOffset(
    IN const DRM_BYTE    *pbData,
    IN       DRM_DWORD    ibData,
    IN       DRM_DWORD    cbData,
    IN OUT   SHA_CONTEXT *pShaContext );

/*********************************************************************
**
**  Function:  DRM_SHA_Finalize
**
**  Synopsis:  
**
**  Arguments:  
**     [pShaContext] -- current running sha context
**     [ShaDigest] -- resultant sha digest from sha operation
**
**  Returns:  None
**
*********************************************************************/

DRM_VOID DRM_API DRM_SHA_Finalize(
    IN     SHA_CONTEXT *pShaContext,
       OUT DRM_BYTE     rgbDigest[__CB_DECL(SHA_DIGEST_LEN)] );

#ifdef __cplusplus
}
#endif


#endif /* __DRMSHA_1_H__ */

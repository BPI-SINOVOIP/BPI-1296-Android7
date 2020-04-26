/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCBCMAC_H__
#define __DRMCBCMAC_H__

ENTER_PK_NAMESPACE;

typedef struct _CBCKey 
{
    DRM_UINT a1, b1, c1, d1, e1, f1, a2, b2, c2, d2, e2, f2;
} DRM_CBCKey;

/*********************************************************************
**
**  Function:  DRM_CBC_Mac
**
**  Synopsis:  Create a 64-bit MAC
**
**  Arguments:  
**     [pbData] -- Byte pointer to DWORD blocks that are to be MAC'd
**     [cBlocks] -- Length of pbData in DWORD's
**     [rgdwKey2] -- 2 DWORD array to hold the 64-bit result
**     [pCBCkey] -- Key structure filled by the caller.
**
**  Returns:  None
** Notes: dwNumBlocks must be in DWORDS and it should be multiple of 
**        DWORD. Suppose if length is 8 bytes, dwNumBlocks should be 2
*********************************************************************/
DRM_API DRM_VOID DRM_CALL DRM_CBC_Mac(
    IN        DRM_BYTE  *pbData,
    IN        DRM_DWORD  cBlocks,
    OUT       DRM_DWORD  rgdwKeys[2],
    IN  const DRM_CBCKey *pCBCkey );

/*********************************************************************
**
**  Function:  DRM_CBC_InverseMac
**
**  Synopsis:  Inverse MAC function.  It decrypts the last two bloacks of pdwData
**              ( replaces 64-bit ciphertext pdwData[dwNumBlocks-1] and pdwData[dwNumBlocks-2]
**              with plaintext ).
**
**  Arguments:  
**     [pbData] -- Byte pointer to DWORD blocks that are to be MAC'd( inverse MAC )
**     [cBlocks] -- Length of pbData in DWORD's
**     [key] -- Key structure filled by caller
**     [ikey] -- Inverse key structure filled by caller.
**
**  Returns:  
**
** Notes: dwNumBlocks must be in DWORDS and it should be multiple of 
**        DWORD. Suppose if length is 8 bytes, dwNumBlocks should be 2
**
*********************************************************************/
DRM_API DRM_VOID DRM_CALL DRM_CBC_InverseMac(
    IN OUT   DRM_BYTE  *pbData,
    IN       DRM_DWORD  cBlocks,
    IN const DRM_CBCKey *key,
    IN const DRM_CBCKey *ikey );

/*********************************************************************
**
**  Function:  DRM_MAC_inv32
**
**  Synopsis:  Returns the inverse of n ( inverse in terms of what the CBC Mac inverse code wants ).
**
**  Arguments:  
**     [n] -- Value of n to compute inverse of.
**
**  Returns:  Inverse of n
**
*********************************************************************/
DRM_API DRM_UINT DRM_CALL DRM_MAC_inv32( DRM_UINT n );

typedef struct __tagCBCState
{
    DRM_UINT sum,t;
    DRM_BYTE buf[__CB_DECL(8)];
    DRM_DWORD dwBufLen;
} DRM_CBCState;

DRM_API DRM_VOID DRM_CALL DRM_CBC64Init(
    DRM_CBCKey *cbckey,
    DRM_CBCState *cbcstate,
    __in_bcount( SIZEOF( DRM_UINT ) * 12 ) DRM_BYTE *pKey );

#define DRMV2_MAC_LENGTH 8

DRM_API DRM_VOID DRM_CALL DRM_CBC64Update(
    DRM_CBCKey *key,
    DRM_CBCState *cbcstate,
    DRM_DWORD dwLen,
    __in_bcount( dwLen ) DRM_BYTE *pData );
    
DRM_API DRM_UINT DRM_CALL DRM_CBC64Finalize( DRM_CBCKey *key, DRM_CBCState *cbcstate, DRM_UINT *pKey2 );
DRM_API DRM_UINT DRM_CALL DRM_CBC64Invert( 
    DRM_CBCKey  *key, 
    DRM_CBCKey  *ikey, 
    DRM_UINT     MacA1, 
    DRM_UINT     MacA2,
    DRM_UINT     MacB1, 
    DRM_UINT     MacB2, 
    DRM_UINT    *pInvKey2 );

DRM_API DRM_VOID DRM_CALL DRM_CBC64InitState( DRM_CBCState *cbcstate );

EXIT_PK_NAMESPACE;

#endif /* __DRMCBCKEY_H__ */

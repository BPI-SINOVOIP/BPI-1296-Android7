/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMCIPHER_H__
#define __DRMCIPHER_H__


#include <drmsha1.h>
#include <drmdes.h>
#include <drmrc4.h>
#include <drmcbc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __tagDRM_CIPHER_CONTEXT
{
    DRM_BYTE      m_shaOut [__CB_DECL(SHA_DIGEST_LEN)];
    DRM_UINT      m_desS1  [2],
                  m_desS2  [2];
    DESTable      m_destable;
    DRM_CBCKey    m_mackey,
                  m_invmackey;    
    RC4_KEYSTRUCT m_rc4ks;
    DRM_BOOL      m_fInited;
    DRM_BOOL      m_fDecryptInited;
    DRM_CBCState  m_cbcstate;                   
    DRM_DWORD     m_rc4key[2]; 
    DRM_UINT      m_rguiLast8[2];
    DRM_DWORD     m_cbPacket;            
    DRM_DWORD     m_iPacket;                                  
} DRM_CIPHER_CONTEXT;

#define DRM_CPHR_CB_FOR_INITDECRYPT     15

/*********************************************************************
**
**  Function:  DRM_CPHR_Init
**
**  Synopsis:  Initializes a content cipher context.  This must be done prior to using DRM_CPHR_Encrypt or
**             DRM_CHPR_Decrypt.
**
**  Arguments:  
**     [pContext] -- The cipher content to be initialized
**     [cbContKey] -- The length of pbContKey in bytes
**     [pbContKey] -- The content key
**
**  Notes:  For most DRM content cbContKey should be DRMCIPHERKEYLEN
**
*********************************************************************/
DRM_RESULT DRM_API DRM_CPHR_Init(
    OUT       DRM_CIPHER_CONTEXT *pContext,
    IN        DRM_DWORD           cbContKey,
    IN  const DRM_BYTE           *pbContKey );

/*********************************************************************
**
**  Function:  DRM_CPHR_Encrypt
**
**  Synopsis:  Encrypts an array of bytes using the DRM content cipher algorithm
**
**  Arguments:  
**     [pContext] -- Cipher context initialized with DRM_CPHR_Init
**     [cbData] -- Length of pbData in bytes
**     [pbData] -- Array of bytes to encrypt
**
**  Notes:  Encryption is done in place.
**
*********************************************************************/
DRM_RESULT DRM_API DRM_CPHR_Encrypt(
    IN     DRM_CIPHER_CONTEXT *pContext,
    IN     DRM_DWORD           cbData,
    IN OUT DRM_BYTE           *pbData );

/******************************************************************************
** 
** Function :   DRM_CPHR_InitDecrypt
** 
** Synopsis :   Initializes state for phased decryption
** 
** Arguments :  f_pcontextCipher - Cipher context initialized with DRM_CPHR_Init,
**                                 followed by DRM_CPHR_InitDecrypt
**              f_pbLast15    - Last 15 bytes of ciphertext
**              f_cbData      - # of bytes in ENTIRE ciphertext              
** 
** Returns :    
** 
** Notes :      
** 
******************************************************************************/
DRM_RESULT DRM_API DRM_CPHR_InitDecrypt(
    IN  DRM_CIPHER_CONTEXT  *pcontextCipher,
    IN  DRM_BYTE            *pbLast15,
    IN  DRM_DWORD            cbData );

/******************************************************************************
** 
** Function :   DRM_CPHR_Decrypt
** 
** Synopsis :   Decrypts part of the ciphertext. This function must be called 
**              after DRM_CPHR_InitDecrypt has been successful
** 
** Arguments :  f_pcontextCipher - Cipher context initialized with DRM_CPHR_Init,
**                                 followed by DRM_CPHR_InitDecrypt
**              f_cbData  - # of bytes of data to be decrypted
**              f_pbData  - Pointer to encrypted buffer
** 
** Returns :    
** 
** Notes :      IMPORTANT: Phased decrypt should be called on segments of 
**              encrypted data sequentially, i.e. if encrypted data is divided 
**              four segments, the order of decryption should be segment 1,
**              followed by 2, 3, 4.
** 
******************************************************************************/
DRM_RESULT DRM_API DRM_CPHR_Decrypt(
    IN     DRM_CIPHER_CONTEXT *pContext,
    IN     DRM_DWORD           cbData, 
    IN OUT DRM_BYTE           *pbData );

#ifndef DRMCIPHERKEYLEN
#define DRMCIPHERKEYLEN   7
#endif


#ifdef __cplusplus
}
#endif

#endif /* __DRMCIPHER_H__ */

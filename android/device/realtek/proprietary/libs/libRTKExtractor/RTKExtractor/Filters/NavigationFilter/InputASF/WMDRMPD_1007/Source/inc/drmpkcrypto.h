/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMPKCRYPTO_H__
#define __DRMPKCRYPTO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "drmstkalloc.h"

/* The following sizes are in BYTEs */
#define PK_ENC_PUBLIC_KEY_LEN	40
#define PK_ENC_PRIVATE_KEY_LEN	20
#define PK_ENC_PLAINTEXT_LEN	16
#define PK_ENC_CIPHERTEXT_LEN	80
#define PK_ENC_SIGNATURE_LEN	40
#define PK_ENC_SIGNATURE_B64LEN	56


#define PKSYMM_KEY_LEN_DRMV2	7
#define PKSYMM_ALG_TYPE_RC4		1

#ifdef DRM_SUPPORT_OPT_CRYPTO
#define DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE 5500 
#define DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE  (DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE + SIZEOF( DRM_STACK_ALLOCATOR_CONTEXT )) 
#else 
#define DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE 19600 
#define DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE  (DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE + SIZEOF(DRM_STACK_ALLOCATOR_CONTEXT))
#endif

typedef struct __tagPUBKEY
{
    DRM_BYTE y[ __CB_DECL(PK_ENC_PUBLIC_KEY_LEN) ];
} PUBKEY;

typedef struct __tagPRIVKEY
{
    DRM_BYTE x[ __CB_DECL(PK_ENC_PRIVATE_KEY_LEN) ];
} PRIVKEY;

typedef struct {
	PUBKEY pk;
	DRM_BYTE version[__CB_DECL(VERSION_LEN)];  
} PK;

typedef struct {
	PK pk;  /* pk.version indicates the pubkey needed to verify. */
	DRM_BYTE sign[__CB_DECL(PK_ENC_SIGNATURE_LEN)];
} PKCERT;


/* */
/*  A set of old keys. */
/* */
typedef struct __tagDRMKEYPAIR
{
    PRIVKEY PrivKey;     /*  an old private key */
    PUBKEY  PubKey;      /*  an old public key */
} DRMKEYPAIR;


/*********************************************************************
**
**  Function:  DRM_PK_GenKeyPair
**
**  Synopsis:  Generate a matched public and private key pair
**
**  Arguments:  
**     [f_pContext] -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_ppubkey]  -- New public key
**     [f_pprivkey] -- New private key
**
*********************************************************************/
DRM_RESULT DRM_API DRM_PK_GenKeyPair( 
    IN     DRM_VOID *f_pContext,
       OUT PUBKEY   *f_ppubkey,
       OUT PRIVKEY  *f_pprivkey );

/*********************************************************************
**
**  Function:  DRM_PK_Encrypt
**
**  Synopsis:  Encrypt a block of data with a given public key.
**
**  Arguments:  
**     [f_pContext] -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_ppubkey]  -- Public key to encrypt with
**     [f_rgbIn]    -- Data to be encrypted
**     [f_rgbOut]   -- Buffer to hold the encrypted data value.
**
**  Notes:  Data is not encrpyted in place.  It is put in the rgbOut buffer.
*********************************************************************/
DRM_RESULT DRM_API DRM_PK_Encrypt( 
    IN        DRM_VOID *f_pContext, 
    IN  const PUBKEY   *f_ppubkey,
    IN  const DRM_BYTE  f_rgbIn [__CB_DECL(PK_ENC_PLAINTEXT_LEN) ],
        OUT   DRM_BYTE  f_rgbOut[__CB_DECL(PK_ENC_CIPHERTEXT_LEN)] );


/*********************************************************************
**
**  Function:  DRM_PK_Decrypt
**
**  Synopsis:  
**
**  Arguments:  
**     [f_pContext] -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_pprivkey] -- Private key to decrypt with
**     [f_rgbIn]    -- Encrypted bytes that are to be decrypted
**     [f_rgbOut]   -- Clear text result
**
**  Notes:  Data is not decrpyted in place.  It is put in the rgbOut buffer.
**
*********************************************************************/
DRM_RESULT DRM_API DRM_PK_Decrypt(
    IN        DRM_VOID *f_pContext,
    IN  const PRIVKEY  *f_pprivkey,
    IN  const DRM_BYTE  f_rgbIn [__CB_DECL(PK_ENC_CIPHERTEXT_LEN)],
        OUT   DRM_BYTE  f_rgbOut[__CB_DECL(PK_ENC_PLAINTEXT_LEN) ] );


/*********************************************************************
**
**  Function:  DRM_PK_Sign
**
**  Synopsis:  Generate a digital signature with a private key
**
**  Arguments:  
**     [f_pContext]     -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_privkey]      -- Private key to create a signature with
**     [f_pbData]       -- Array of bytes to create a signature over
**     [f_cbData]       -- Length of pbBuffer in bytes
**     [f_rgbSignature] -- Buffer to hold result signature
*********************************************************************/
DRM_RESULT DRM_API DRM_PK_Sign(
    IN       DRM_VOID  *f_pContext,
    IN const PRIVKEY   *f_privkey,
    IN const DRM_BYTE  *f_pbData,
    IN       DRM_DWORD  f_cbData,
       OUT   DRM_BYTE   f_rgbSignature[__CB_DECL(PK_ENC_SIGNATURE_LEN)] );

/*********************************************************************
**
**  Function:  DRM_PK_Verify
**
**  Synopsis:  Verify a digital signature created by DRM_PK_Sign.
**
**  Arguments:  
**     [f_pContext]     -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_ppubkey]      -- Pubkey to check the signature with
**     [f_pbData]       -- Data buffer that the signature was created over
**     [f_cbData]       -- Length of pbBuffer in bytes
**     [f_rgbSignature] -- The signature to verify
**
**  Returns:  TRUE if the signature verified correctly.  FALSE is it didn't
**
*********************************************************************/
DRM_BOOL DRM_API DRM_PK_Verify( 
    IN       DRM_VOID  *f_pContext,
    IN const PUBKEY    *f_ppubkey, 
    IN const DRM_BYTE  *f_pbData, 
    IN       DRM_DWORD  f_cbData, 
    IN const DRM_BYTE   f_rgbSignature[__CB_DECL(PK_ENC_SIGNATURE_LEN)] );

#define DRM_PK_MAX_SECRET_ARRAYS 5
#define DRM_PK_SECRET_FUNC_COUNT 7

typedef DRM_RESULT (*pfnSetSecretFunction)();
typedef pfnSetSecretFunction pfnSetSecretArray[DRM_PK_SECRET_FUNC_COUNT];
typedef pfnSetSecretArray   *ppfnSetSecretArray;

typedef struct __PK_SET_SECRET_FUNCS
{
    ppfnSetSecretArray secretArray[DRM_PK_MAX_SECRET_ARRAYS];
} PK_SET_SECRET_FUNCS;

extern PK_SET_SECRET_FUNCS g_SetSecretFuncs;


#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* __DRMPKCRYPTO_H__ */

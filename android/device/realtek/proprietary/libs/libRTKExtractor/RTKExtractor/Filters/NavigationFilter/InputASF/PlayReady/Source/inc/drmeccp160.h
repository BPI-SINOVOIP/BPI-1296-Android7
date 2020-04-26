/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMECCP160_H__
#define __DRMECCP160_H__

#include <drmstkalloc.h>

ENTER_PK_NAMESPACE;

/* The following sizes are in BYTEs */
#define DRM_ECC160_PUBLIC_KEY_LEN   40
#define DRM_ECC160_PRIVATE_KEY_LEN  20
#define DRM_ECC160_PLAINTEXT_LEN    16
#define DRM_ECC160_CIPHERTEXT_LEN   80
#define DRM_ECC160_SIGNATURE_LEN    40
#define DRM_ECC160_SIGNATURE_B64LEN CCH_BASE64_EQUIV_SAL(DRM_ECC160_SIGNATURE_LEN)


#define PKSYMM_KEY_LEN_DRMV2    7
#define PKSYMM_ALG_TYPE_RC4        1

#if ( DRM_SUPPORT_WMDRMNET )
#define DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE ( 5500 + 4100 )
#else
#define DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE ( 5500 )
#endif

/* This is size for PK_DATA structure defined in pkcrypto file.
   This size of structure on x86 is 3036.
   I choose 3100 to allow larger size for different architecture
*/
#define DRM_PK_DATA_SIZE                          3200

#define LNGQDW 5
#define LNGQ DWORDS_TO_DIGITS(LNGQDW)
#define QBITS (LNGQDW * DWORD_BITS)

/*
** Length of ECC integer types in 32 bit values.
** For ECC 160 - lenght is 5 32 bit dwords.
** For ECC 256 - lenght is 8 32 bit dwords.
*/
#define LNGQ_MODULO_5  5
#define LNGQ_MODULO_8  8

#define DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE  ( DRM_PKCRYPTO_CONTEXT_BUFFER_INTERNAL_SIZE + SIZEOF( DRM_STACK_ALLOCATOR_CONTEXT ) + DRM_PK_DATA_SIZE + SIZEOF( DRM_DWORD ) + SIZEOF( DRM_VOID* ) ) 

typedef DRM_DWORD_ALIGN struct __tagPUBKEY
{
    DRM_BYTE y[ __CB_DECL(DRM_ECC160_PUBLIC_KEY_LEN) ];
} PUBKEY;

typedef DRM_DWORD_ALIGN struct __tagPRIVKEY
{
    DRM_BYTE x[ __CB_DECL(DRM_ECC160_PRIVATE_KEY_LEN) ];
} PRIVKEY;

typedef struct {
    PUBKEY pk;
    DRM_BYTE version[__CB_DECL(VERSION_LEN)];  
} PKV;

/********************************************************************* 
** Structure for keeping of private key for signing of header. 
** The structure DRM_PRIVATE_KEY keeps ECC private key and
** enumeration value that corresponds to type of the key.
**   
**   privKey  -  The private key for signing of the header:
**   
**   keyType  - Type of the key
**   
**   Currently we have only one type of the key - ECC key.
**   In future new types will be added and new data for private keys
**********************************************************************/
typedef enum { DRM_PRIVATE_KEY_ECC } DRM_PRIVATE_KEY_TYPE;

typedef struct __tagDRM_PRIVATE_KEY

{ 
    DRM_PRIVATE_KEY_TYPE keyType;
    PRIVKEY              privKey;
} DRM_PRIVATE_KEY;


typedef struct {
    PKV pk;  /* pk.version indicates the pubkey needed to verify. */
    DRM_BYTE sign[__CB_DECL(DRM_ECC160_SIGNATURE_LEN)];
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
**  Function:  DRM_ECC_GenKeyPair_P160
**
**  Synopsis:  Generate a matched public and private key pair
**
**  Arguments:  
**     [f_pContext] -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE
**     [f_ppubkey]  -- New public key
**     [f_pprivkey] -- New private key
**
*********************************************************************/
DRM_API DRM_RESULT DRM_CALL DRM_ECC_GenKeyPair_P160( 
    IN     DRM_VOID *f_pContext,
       OUT PUBKEY   *f_ppubkey,
       OUT PRIVKEY  *f_pprivkey );

/*********************************************************************
**
**  Function:  DRM_ECC_Encrypt_P160
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
DRM_API DRM_RESULT DRM_CALL DRM_ECC_Encrypt_P160( 
    IN        DRM_VOID *f_pContext, 
    IN  const PUBKEY   *f_ppubkey,
    __in_bcount( DRM_ECC160_PLAINTEXT_LEN ) const DRM_BYTE  f_rgbIn [__CB_DECL(DRM_ECC160_PLAINTEXT_LEN) ],
    __out_bcount( DRM_ECC160_CIPHERTEXT_LEN ) DRM_BYTE  f_rgbOut[__CB_DECL(DRM_ECC160_CIPHERTEXT_LEN)] );


/*********************************************************************
**
**  Function:  DRM_ECC_Decrypt_P160
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
DRM_API DRM_RESULT DRM_CALL DRM_ECC_Decrypt_P160(
    IN        DRM_VOID *f_pContext,
    IN  const PRIVKEY  *f_pprivkey,
    __in_bcount( DRM_ECC160_CIPHERTEXT_LEN ) const DRM_BYTE  f_rgbIn [__CB_DECL(DRM_ECC160_CIPHERTEXT_LEN)],
    __out_bcount( DRM_ECC160_PLAINTEXT_LEN ) DRM_BYTE  f_rgbOut[__CB_DECL(DRM_ECC160_PLAINTEXT_LEN) ] );


/*********************************************************************
**
**  Function:  DRM_ECC_Sign_P160
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
DRM_API DRM_RESULT DRM_CALL DRM_ECC_Sign_P160(
    IN       DRM_VOID  *f_pContext,
    IN const PRIVKEY   *f_privkey,
    __out_bcount( f_cbData ) const DRM_BYTE  *f_pbData,
    IN       DRM_DWORD  f_cbData,
    __out_bcount( DRM_ECC160_SIGNATURE_LEN ) DRM_BYTE   f_rgbSignature[__CB_DECL(DRM_ECC160_SIGNATURE_LEN)] );

/*********************************************************************
**
**  Function:  DRM_ECC_Verify_P160
**
**  Synopsis:  Verify a digital signature created by DRM_ECC_Sign_P160.
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
DRM_API DRM_BOOL DRM_CALL DRM_ECC_Verify_P160( 
    IN       DRM_VOID  *f_pContext,
    IN const PUBKEY    *f_ppubkey, 
    __in_bcount( f_cbData ) const DRM_BYTE  *f_pbData, 
    IN       DRM_DWORD  f_cbData, 
    __in_bcount( DRM_ECC160_SIGNATURE_LEN ) const DRM_BYTE   f_rgbSignature[__CB_DECL(DRM_ECC160_SIGNATURE_LEN)] );

/*********************************************************************
**
**  Function:  DRM_ECC_ThreadUnSafePKInit, DRM_ECC_ThreadSafeInit
**
**  Synopsis:  Initializes DRMBIGNUM_CONTEXT_STRUCT
**
**  Arguments:  
**     [f_pContext]     -- Pointer to context the size of DRM_PKCRYPTO_CONTEXT_BUFFER_SIZE

**  Returns:  DRM_SUCCESS if context is initialized.
**
*********************************************************************/

DRM_API DRM_RESULT DRM_CALL DRM_ECC_ThreadUnSafePKInit( 
    IN OUT  DRM_VOID* f_pContext,
    IN      DRM_VOID* f_pOEMContext );

DRM_VOID DRM_CALL DRM_RSA_PKInit(
    IN OUT DRM_VOID *f_pContext );

DRM_VOID DRM_CALL DRM_RSA_PKInitWithSize(
    IN OUT DRM_VOID *f_pContext,
    IN     DRM_DWORD f_dwSize );

DRM_API DRM_RESULT DRM_CALL DRM_ECC_ThreadSafeInit( 
    IN OUT  DRM_VOID* f_pContext,
    IN      DRM_VOID* f_pOEMContext );



#if DRM_THREAD_SAFE
#define DRM_ECC_PKInit( pContext, pOEMContext  ) DRM_ECC_ThreadSafeInit( pContext, pOEMContext )
#else
#define DRM_ECC_PKInit( pContext, pOEMContext  ) DRM_ECC_ThreadUnSafePKInit( pContext, pOEMContext  )
#endif

EXIT_PK_NAMESPACE;

#endif /* __DRMECCP160_H__ */

/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef _DRMRSA_H_
#define _DRMRSA_H_

#include <oemrsa.h>
#include <drmrsacommon.h>
#include <drmtypes.h>

ENTER_PK_NAMESPACE;

/*
** Definitions for OAEP padding scheme (see PKCS #1 v2.1)
*/ 
#define OAEP_IB_EM_START                    0
#define OAEP_CB_EM_START                    1

#define OAEP_IB_SEED                        ( OAEP_IB_EM_START + OAEP_CB_EM_START )
#define OAEP_CB_SEED                        DRM_SHA1_DIGEST_LEN

#define OAEP_IB_DATABLOCK                   ( OAEP_IB_SEED + OAEP_CB_SEED )
#define OAEP_CB_DATABLOCK( keyLength ) \
    ( OAEP_CB_ENCRYPTED_MSG( keyLength ) - OAEP_CB_EM_START - OAEP_CB_SEED )
#define OAEP_CB_DATABLOCK_MAX               OAEP_CB_DATABLOCK( eDRM_RSA_MAXIMUM_KEY_LENGTH )

#define OAEP_IB_DATABLOCK_HASH              OAEP_IB_DATABLOCK
#define OAEP_CB_DATABLOCK_HASH              DRM_SHA1_DIGEST_LEN

#define OAEP_IB_DATABLOCK_PS                ( OAEP_IB_DATABLOCK_HASH + OAEP_CB_DATABLOCK_HASH )
#define OAEP_CB_DATABLOCK_PS( keyLength, msgLength ) \
    ( OAEP_CB_DATABLOCK_MSG_MAX( keyLength ) - msgLength )  

#define OAEP_IB_DATABLOCK_MSG_START( keyLength, msgLength ) \
    ( OAEP_IB_DATABLOCK_PS + OAEP_CB_DATABLOCK_PS( keyLength, msgLength ) )                   
#define OAEP_CB_DATABLOCK_MSG_START         1

#define OAEP_IB_DATABLOCK_MSG( keyLength, msgLength ) \
    ( OAEP_IB_DATABLOCK_MSG_START( keyLength, msgLength ) + OAEP_CB_DATABLOCK_MSG_START )
#define OAEP_CB_DATABLOCK_MSG( msgLength )  ( msgLength )
#define OAEP_CB_DATABLOCK_MSG_MAX( keyLength ) \
    ( OAEP_CB_ENCRYPTED_MSG( keyLength ) - OAEP_CB_EM_START - OAEP_CB_SEED - OAEP_CB_DATABLOCK_HASH - OAEP_CB_DATABLOCK_MSG_START )

#define OAEP_CB_ENCRYPTED_MSG( keyLength )  DRM_RSA_CB_CIPHERTEXT( keyLength )
#define OAEP_CB_ENCRYPTED_MSG_MAX           DRM_RSA_CB_CIPHERTEXT( eDRM_RSA_MAXIMUM_KEY_LENGTH )
    
#define OAEP_B_EM_START                     0x00
#define OAEP_B_MSG_START                    0x01

/*
** Definitions for PSS signature scheme (see PKCS #1 v2.1)
*/
#define PSS_B_END_MSG      0xBC
#define PSS_CB_ZERO_PAD    8
#define PSS_CB_SALT        0
#define PSS_CB_MPRIME      ( BITS_PER_STANDARD_BYTE + DRM_SHA1_DIGEST_LEN + PSS_CB_SALT )
#define PSS_IB_SALT        ( PSS_CB_ZERO_PAD + DRM_SHA1_DIGEST_LEN )
 
/*
** Definitions for mask generation (MGF1)
*/ 
#define MGF1_CB_MAX_SEED( keyLength )   OAEP_CB_DATABLOCK( keyLength )
#define MGF1_CB_MAX_SEED_MAX            MGF1_CB_MAX_SEED( eDRM_RSA_MAXIMUM_KEY_LENGTH )
#define MGF1_CB_MAX_MASK( keyLength )   ( ( ( OAEP_CB_DATABLOCK( keyLength ) / DRM_SHA1_DIGEST_LEN ) + 1 ) * DRM_SHA1_DIGEST_LEN )
#define MGF1_CB_MAX_MASK_MAX            MGF1_CB_MAX_MASK( eDRM_RSA_MAXIMUM_KEY_LENGTH )


DRM_API DRM_RESULT DRM_CALL DRM_RSA_OaepDecrypt(
    __in const                          DRM_RSA_PRIVATE_KEY *f_pPrivateKey,
    __in const                          DRM_DWORD            f_cbCiphertext,
    __in_ecount( f_cbCiphertext ) const DRM_BYTE            *f_pbCiphertext,
    __inout                             DRM_DWORD           *f_pcbPlaintext,
    __out_ecount( *f_pcbPlaintext )     DRM_BYTE            *f_pbPlaintext,
    __in                                DRM_CRYPTO_CONTEXT  *f_pCryptoCtx 
    ); 

DRM_API DRM_RESULT DRM_CALL DRM_RSA_OaepEncrypt(
    __in_opt                           DRM_VOID           *f_pOEMContext,
    __in const                         DRM_RSA_PUBLIC_KEY *f_pPublicKey,
    __in const                         DRM_DWORD           f_cbPlaintext,
    __in_ecount( f_cbPlaintext ) const DRM_BYTE           *f_pbPlaintext,
    __inout                            DRM_DWORD          *f_pcbCiphertext,
    __out_ecount( *f_pcbCiphertext )   DRM_BYTE           *f_pbCiphertext,
    __in                               DRM_CRYPTO_CONTEXT *f_pCryptoCtx 
    );

DRM_API DRM_RESULT DRM_CALL DRM_RSA_PssSign(
    __in const                      DRM_RSA_PRIVATE_KEY *f_pPrivateKey,
    __in const                      DRM_DWORD            f_cbMsg,
    __in_ecount( f_cbMsg ) const    DRM_BYTE            *f_pbMsg,
    __in const                      DRM_DWORD            f_ibMsg,    
    __inout                         DRM_DWORD           *f_pcbSignature,
    __out_ecount( *f_pcbSignature ) DRM_BYTE            *f_pbSignature,
    __in                            DRM_CRYPTO_CONTEXT  *f_pCryptoCtx 
    );

DRM_API DRM_RESULT DRM_CALL DRM_RSA_PssVerify(
    __in const                         DRM_RSA_PUBLIC_KEY  *f_pPublicKey,
    __in                               DRM_DWORD            f_cbMsg,
    __in_ecount( f_cbMsg ) const       DRM_BYTE            *f_pbMsg,
    __in const                         DRM_DWORD            f_ibMsg,
    __in const                         DRM_DWORD            f_cbSignature,
    __in_ecount( f_cbSignature ) const DRM_BYTE            *f_pbSignature,
    __in                                DRM_CRYPTO_CONTEXT *f_pCryptoCtx 
    );
    
EXIT_PK_NAMESPACE;

#endif /* _DRMRSA_H_ */

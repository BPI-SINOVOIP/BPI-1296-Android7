/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMRSACOMMON_H__
#define __OEMRSACOMMON_H__

#include <byteorder.h>

ENTER_PK_NAMESPACE;

/*
** Supported key lengths
*/
typedef enum __tageDRM_RSA_SUPPORTED_KEY_LENGTHS
{
    eDRM_RSA_UNKNOWN = 0,
    eDRM_RSA_1024 = 1024,
    eDRM_RSA_2048 = 2048,
} DRM_RSA_SUPPORTED_KEY_LENGTHS;

/* Value cannot from enum because it used to define DRM_RSA_CB_PRIME_MAX that used for SAL annotations */
#define eDRM_RSA_MAXIMUM_KEY_LENGTH               2048

#define IS_VALID_DRM_RSA_KEY_LENGTH( keyLength )  ( ( eDRM_RSA_1024 == ( keyLength ) )   \
                                                 || ( eDRM_RSA_2048 == ( keyLength ) ) )

/*
** Primes are either 1024 or 2048 bits (128 or 256 bytes) each
*/
#define DRM_RSA_CB_PRIME( keyLength )           ( ( keyLength ) / BITS_PER_STANDARD_BYTE )
#define DRM_RSA_CB_PRIME_MAX                    DRM_RSA_CB_PRIME( eDRM_RSA_MAXIMUM_KEY_LENGTH )

/*
** Modulus is 128/256 bytes or 1024/2048 bits
*/
#define DRM_RSA_CB_MODULUS( keyLength )         DRM_RSA_CB_PRIME( keyLength )
#define DRM_RSA_CB_MODULUS_MAX                  DRM_RSA_CB_MODULUS( eDRM_RSA_MAXIMUM_KEY_LENGTH )

#define DRM_RSA_CBITS_MODULUS( keyLength )    ( DRM_RSA_CB_MODULUS( keyLength ) * BITS_PER_STANDARD_BYTE )
#define DRM_RSA_CBITS_MODULUS_MAX               DRM_RSA_CBITS_MODULUS( eDRM_RSA_MAXIMUM_KEY_LENGTH )

/*
** RSA ciphertext length is the same as the prime length
*/
#define DRM_RSA_CB_CIPHERTEXT( keyLength )      DRM_RSA_CB_MODULUS( keyLength )
#define DRM_RSA_CB_CIPHERTEXT_MAX               DRM_RSA_CB_CIPHERTEXT( eDRM_RSA_MAXIMUM_KEY_LENGTH )

/*
** Public and private exponent sizes
*/
#define DRM_RSA_CB_PUBEXP                       4
#define DRM_RSA_CB_PRIVEXP( keyLength )         DRM_RSA_CB_MODULUS( keyLength )
#define DRM_RSA_CB_PRIVEXP_MAX                  DRM_RSA_CB_PRIVEXP( eDRM_RSA_MAXIMUM_KEY_LENGTH )

/*
** For regular encryption (not large encryption) the plaintext must be smaller than the modulus
*/
#define DRM_RSA_CB_MAX_PLAINTEXT( keyLength ) ( DRM_RSA_CB_MODULUS( keyLength ) - ( 2 * DRM_SHA1_DIGEST_LEN + 2 ) )
#define DRM_RSA_CB_MAX_PLAINTEXT_MAX            DRM_RSA_CB_MAX_PLAINTEXT( eDRM_RSA_MAXIMUM_KEY_LENGTH )

EXIT_PK_NAMESPACE;

#endif /* __OEMRSACOMMON_H__ */


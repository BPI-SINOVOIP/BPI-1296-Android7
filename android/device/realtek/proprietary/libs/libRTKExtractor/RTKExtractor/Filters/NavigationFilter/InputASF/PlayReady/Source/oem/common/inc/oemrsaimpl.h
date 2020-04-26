/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __OEMRSAIMPL_H__
#define __OEMRSAIMPL_H__

#include <drmtypes.h>
#include <drmrsacommon.h>
#include <bigdefs.h>

ENTER_PK_NAMESPACE;

#define CBITS_PUBEXP                  ( DRM_RSA_CB_PUBEXP * BITS_PER_STANDARD_BYTE )

#define CBITS_PRIVEXP( keyLength )    ( DRM_RSA_CB_PRIVEXP( keyLength ) * BITS_PER_STANDARD_BYTE )
#define CBITS_PRIVEXP_MAX               CBITS_PRIVEXP( eDRM_RSA_MAXIMUM_KEY_LENGTH )

#define CDIGITS_MODULUS( keyLength )    BITS_TO_DIGITS( DRM_RSA_CBITS_MODULUS( keyLength ) )
#define CDIGITS_MODULUS_MAX             CDIGITS_MODULUS( eDRM_RSA_MAXIMUM_KEY_LENGTH )

#define CDIGITS_PUBEXP                  BITS_TO_DIGITS( CBITS_PUBEXP )

#define CDIGITS_PRIVEXP( keyLength )    BITS_TO_DIGITS( CBITS_PRIVEXP( keyLength ) )
#define CDIGITS_PRIVEXP_MAX             CDIGITS_PRIVEXP( eDRM_RSA_MAXIMUM_KEY_LENGTH )

/*
** Internal representation of public and private key structures
*/

/*
** In the following structures, a digit_t array represents a single big number,
** stored in big-endian format.  In other words, the most significant digit is 
** lowest in contiguous memory.  Since the digit_t itself is a multi-octet value, 
** please note that it is also stored in big-endian form; the most significant
** octet of a given digit_t instance is lowest in contiguous memory.
**
** Thus, a "big-endian" digit array is a sequence of digit_t's stored in 
** big-endian order, with the individual octets of each digit_t also stored
** in big-endian order.
*/

/*
** Public key: Contains only the modulus and exponent
*/
typedef struct __tagINTERNAL_DRM_RSA_PUBLIC_KEY
{       
    DRM_RSA_SUPPORTED_KEY_LENGTHS eKeyLength;                               /* key length in bits. */
    DRM_DWORD                     cdModulus;                                /* count of digits in the public modulus. */
    digit_t                       rgdPubExp[CDIGITS_PUBEXP];                /* public exponent; stored as big-endian digits. */
    digit_t                       rgdModulus[CDIGITS_MODULUS_MAX];          /* public modulus; stored as big-endian digits. */
} INTERNAL_DRM_RSA_PUBLIC_KEY;

/*
** Private key: Contains the primes, exponent, and chinese remainder theorem residuals
*/
typedef struct __tagINTERNAL_DRM_RSA_PRIVATE_KEY
{   
    DRM_RSA_SUPPORTED_KEY_LENGTHS eKeyLength;                               /* key length in bits. */
    DRM_DWORD                     cdPrimes[2];                              /* count of digits in each prime. */
    digit_t                       rgdPrimes[2][CDIGITS_MODULUS_MAX];        /* larger prime (p) stored first; stored as big-endian digits. */
    digit_t                       rgdCRTExponents[2][CDIGITS_MODULUS_MAX];  /* chinese remainder theorem exponents; stored as big-endian digits. */     
    digit_t                       rgdIQMP[CDIGITS_MODULUS_MAX];             /* stored as big-endian digits. */
} INTERNAL_DRM_RSA_PRIVATE_KEY;

EXIT_PK_NAMESPACE;

#endif /* __OEMRSAIMPL_H__ */

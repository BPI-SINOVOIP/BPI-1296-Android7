/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __rsaimpl_h
#define __rsaimpl_h 1

/*
       File bigimpl.h.   Version 19 November 2002
*/

/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

#include <bigpriv.h>
#include <bignum.h>
#include <mprand.h>
#ifndef assert
#include <assert.h>
#endif

static DWORDREGC NUM_RABIN_TEST = 50;
static DWORDREGC MAX_PRIME_CANDIDATES = 5000;

/******************************************************************************/
extern DRM_BOOL big_endian_bytes_to_digits
        (const DRM_BYTE  *barray,   /* IN */
         DWORDREGC    bitlen,   /* IN */
         digit_t     *darray);  /* OUT */

extern DRM_BOOL little_endian_bytes_to_digits
        (const DRM_BYTE  *barray,   /* IN */
         DWORDREGC    bitlen,   /* IN */
         digit_t     *darray);  /* OUT */

extern DRM_BOOL digits_to_big_endian_bytes
        (digit_tc    *darray,   /* IN */
         DWORDREGC    bitlen,   /* IN */
         DRM_BYTE        *barray);  /* OUT */

extern DRM_BOOL digits_to_little_endian_bytes
        (digit_tc    *darray,   /* IN */
         DWORDREGC    bitlen,   /* IN */
         DRM_BYTE        *barray);  /* OUT */


/*
 external_rsakey_t is used in a broad context,
 including network transfers.
 The declaration should likely be elsewhere.

 TBD -- Is there a big/little endian problem
        reading the DWORD lengths at the start of the struct?

 Do we want only one pointer within the struct, with other addresses
 computed from this pointer (i.e, as offsets from free_me)?
*/

typedef struct {
      DRM_DWORD   bitlen_modulus;   /* Length of modulus in bits */
      DRM_DWORD   bitlen_pubexp;    /* Length of public exponent in bits */
      DRM_DWORD   bitlen_primes[2]; /* Lengths of primes in bits */
                                /* (zero in public key) */
      DRM_BYTE  *free_me;           /* Address to be passed to free() */
                                /* when done with struct */

/* Next fields are in big-endian order (most significant byte first). */
/* Their lengths are (bitlen_modulus + 7)/8 to (bitlen_primes[1] + 7)/8. */

      DRM_BYTE  *modulus;           /* Public key modulus p1*p2 */
      DRM_BYTE  *pubexp;            /* Public key exponent */
      DRM_BYTE  *primes[2];         /* Primes p1 and p2 (private key only) */
      DRM_BYTE  *privexps[2];       /* Private exponents (private key only */
                                /*                  -- same lengths as primes) */
/* privexps is easily computed from the primes -- */
/* we can suppress these from the external key */
} external_rsa_key_t;

typedef struct {
      DRM_DWORD     bitlen_modulus; /* Lengths of messages to encrypt/decrypt (bits) */
      DRM_DWORD     diglen_pubexp;
      digit_t  *free_me;
      digit_t  *pubexp;         /* Public exponent.  Length diglen_pubexp */
      digit_t  *modulus;        /* p1*p2   Length */
                                /*     BITS_TO_DIGITS(bitlen_modulus), */
 /* Private key data */
      mp_modulus_t moduli[2];   /* Moduli, including primes and their lengths */
      digit_t  *privexps[2];    /* Private exponents.  Same lengths as primes */
      digit_t  *chineses[2];    /* Chinese Remainder Theorem constants: */
                                /* (1/p2) mod p1  and  (1/p1) mod p2 */
                                /*                (private key only) */
                                /* Same lengths as the primes. */
} internal_rsa_key_t;

extern DRM_BOOL rsa_encryption
        (const external_rsa_key_t  *prsaext,   /* IN */
         const DRM_BYTE      *msgin,     /* IN, length prsaext->bitlen_modulus bits */
         DRM_BYTE            *msgout);   /* OUT */

extern DRM_BOOL rsa_key_internalize
        (const external_rsa_key_t  *prsaext,   /* IN */
         internal_rsa_key_t        *prsaint);  /* OUT */

extern DRM_BOOL rsa_decryption
        (const internal_rsa_key_t  *prsaint,   /* IN */
         const DRM_BYTE      *msgin,     /* IN, length prsaint->bitlen_modulus bits */
         DRM_BYTE            *msgout);   /* OUT */

#ifdef __cplusplus
}
#endif
#endif /* __rsaimpl_h */

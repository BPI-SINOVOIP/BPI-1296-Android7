/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __CRYPTDSA_H
#define __CRYPTDSA_H

//  September, 2002 -- petmon.  This and cryptdsa.c have not been altered
//       to use DWORDREG since I don't know the implications
//       on the rest of CAPI.   TBD
//
//       P.S.  DWORD arrays in dsa_dwkey_t should perhaps be converted to BYTE.

#ifdef __cplusplus
extern "C" {
#endif

#include "ecapi.h"          // SHA
#include "mprand.h"         // RNGINFO

#define SHA_BITS      (8*A_SHA_DIGEST_LEN)
                            // Number of bits output by SHA
#define SHA_DWORDS    (A_SHA_DIGEST_LEN / 4)
                            // Number of DWORDS output by SHA
#if SHA_BITS != DWORD_BITS*SHA_DWORDS
   #error -- "A_SHA_DIGEST_LEN not multiple of 4 bytes"
#endif


#define DSA_Q_MINDWORDS 5
                            // Minimum number of DWORDS in q
#define DSA_Q_MAXDWORDS 128
                            // Maximum number of DWORDS in q
#define DSA_P_MINDWORDS 16
                            // Minimum number of DWORDS in p
#define DSA_P_MAXDWORDS 128
                            // Maximum number of DWORDS in p

#define DSA_Q_MAXDIGITS DWORDS_TO_DIGITS(DSA_Q_MAXDWORDS)
#define DSA_P_MAXDIGITS DWORDS_TO_DIGITS(DSA_P_MAXDWORDS)

//   The DSA algorithms require modular exponentiation.
//   The DSA codes will normally invoke
//   DSA_exponentiator_default to do this exponentiation,
//   but the application may substitute a specialized one of its own.
//   The dsa_exponentiator_t type declares the interface
//   to that routine.

//   A specialized exponentiation routine might
//   observe that, when signing, the base never varies.
//   When verifying, there is one base from the generator
//   and one base per public key.  The exponentiator might
//   take advantage of the fixed bases.

//   The precise argument list to an exponentiator
//   may change in later versions of this library.

//   If one is using a specialized hardware coprocessor, then
//   the exponentiation interface should copy the
//   digit_t arguments to BYTE arrays or whatever
//   the hardware expects.
//   You may need to convert between FROM_LEFT and FROM_RIGHT
//   formats.


typedef BOOL DRM_API dsa_exponentiator_t
        (MP_INPUT       base,       // In
         MP_INPUT       exponent,   // In
         DWORDREGC      nbitq,      // In (number of bits in exponent)
                                    // Unused bits in top word are zero-filled
         MP_OUTPUT      result,     // Out
         mp_modulus_tc *modulo,     // In   Modulus
         PBIGCTX_ARG);

//    dsa_dwkey_t holds a public or private key, in external (DWORD) format

typedef struct {
        DWORD    nbitp;             // Number of significant bits in p.
                                    // (Multiple of 64,   512 <= nbitp <= 1024)
        DWORD    nbitq;             // Number of significant bits in q.
                                    // Must be exactly 160.
        DWORD    nbitx;             // Number of bits in x.
                                    // Will equal nbitq for new keys
                                    //      (Longhorn and later).
                                    // May equal nbitp for older keys.
        DWORD    p[DSA_P_MAXDWORDS];// Public prime p, 512-1024 bits
        DWORD    q[DSA_Q_MAXDWORDS];// Public prime q (160 bits, divides p-1)
        DWORD    g[DSA_P_MAXDWORDS];// Public generator g of order q (mod p)
 //       DWORD    j[DSA_P_MAXDWORDS];// j = (p - 1) / q
        DWORD    x[DSA_P_MAXDWORDS];// Private exponent (zero in putlic key)
                                             //    x (mod q or mod p)
        DWORD    y[DSA_P_MAXDWORDS];// Public value g^x (mod p),
                                    //     where x is private
        DWORD    S[SHA_DWORDS];     // 160-bit pattern used to construct q
        DWORD    C;                 // 12-bit value of C used to construct p
               } dsa_dwkey_t;
typedef const dsa_dwkey_t  dsa_dwkey_tc;

typedef struct {
 //       VOID *pOffload;           // pointer to exponentiation offload info
// TBD above not used
        dsa_exponentiator_t *pExponentiator;
        RNGINFO *pRNGInfo;        // pointer to Random Number Generator info
        } dsa_other_info_t;

typedef const dsa_other_info_t  dsa_other_info_tc;

typedef struct {
                dsa_dwkey_tc  *pdwkey;                // Pointer to public or
                                                      // private key in
                                                      // external format
                dsa_other_info_t other;               // Exponentiator
                                                      //   RNG addresses
                DWORDREG       lngq_digits;           // Length of q in digits
                reciprocal_1_t qrecip;                // Information about 1/q
                digit_t        gmodular[DSA_P_MAXDIGITS];
                                                      // g as residue mod p
                digit_t        qdigit[DSA_Q_MAXDIGITS];
                digit_t        xdigit[DSA_P_MAXDIGITS];
                                                // Private exponent x (or zero)
                digit_t        ymodular[DSA_P_MAXDIGITS];
                                                         // y as residue mod p
                mp_modulus_t   pmodulus;                 // Constants mod p
               } dsa_fullkey_t;

typedef const dsa_fullkey_t     dsa_fullkey_tc;

typedef struct {
        DWORD r[SHA_DWORDS];            // (g^k mod p)       mod q
        DWORD s[SHA_DWORDS];            // (SHA(m) + x*r)/k  mod q
               } dsa_signature_t;

typedef const dsa_signature_t   dsa_signature_tc;

// DSA_check_g switched to static November, 2002   petmon

//BOOL DRM_API DSA_check_g
//        (DWORDREGC       lngp_digits,                   // In
//         digit_tc       *pGModular,                    // In
//         mp_modulus_t   *pPModulo,                     // In
//         DWORDREGC      lngq_digits,                   // In
//         digit_tc       *pQDigit);                     // In

extern BOOL DRM_API DSA_build_fullkey
        (dsa_dwkey_tc      *pdwkey,          // In
         dsa_other_info_tc *pother,          // In
         dsa_fullkey_t     *pfullkey,        // In
         const BOOL         checkSC,         // In
         PBIGCTX_ARG);

extern dsa_exponentiator_t DSA_exponentiator_default;


extern BOOL DRM_API DSA_key_generation
        (DWORDC             nbitp,           // In
         DWORDC             nbitq,           // In
         dsa_other_info_tc *pother,          // In
         dsa_fullkey_t     *pfullkey,        // Out
         dsa_dwkey_t       *dwkey,           // Out
         PBIGCTX_ARG);

extern BOOL DRM_API DSA_parameter_verification(
                                dsa_fullkey_tc  *fullkey,  // In
                                PBIGCTX_ARG
                                );
#if 0
extern BOOL DRM_API DSA_convert_pgy
        (dsa_dwkey_tc      *dwkey,            // In
         dsa_fullkey_t     *pfullkey);        // Out
#endif

extern BOOL DRM_API DSA_sign
        (DWORDC             message_hash[SHA_DWORDS],   // In
         dsa_fullkey_tc    *pfullkey,                   // In
         dsa_signature_t   *signature,                  // Out
         PBIGCTX_ARG);

extern BOOL DRM_API DSA_signature_verification
        (DWORDC              message_hash[SHA_DWORDS],  // In
         dsa_fullkey_tc      *pfullkey,                 // In
         dsa_signature_tc    *signature,                // In
         BOOL                *pverified,                // Out
         PBIGCTX_ARG);
       // Users of
       //            DSA_build_fullkey,
       //            DSA_key_generation

       //       are responsible for subsequent call to DSA_unbuild_fullkey

extern void DRM_API DSA_unbuild_fullkey
        (dsa_fullkey_t   *pfullkey,                     // In/Out
         PBIGCTX_ARG);


#if 0
extern BOOL DRM_API DSA_verify_j
        (dsa_fullkey_tc  *fullkey);                     // In
// TBD -- Who uses verify_j?  It needs to be put in the test suite.
#endif

#ifdef __cplusplus
}
#endif

#endif __CRYPTDSA_H

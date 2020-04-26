/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
/*
     File ecpriv.h .

     This file has elliptic curve declarations which
     are visible to the bignum library but not to
     application programs.  Data types and constants
     herein can be altered with (theoretically)
     no impact on applications.

*/
#ifndef ECPRIV_H
#define ECPRIV_H 1

/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

#include "fieldpriv.h"
#include "ecurve.h"


#define EC_EXPENSIVE_DEBUGGING 0
          // 1  while testing code, 0 in production

#define MAX_ECTEMPS 5
          // Maximum number of field temporaries needed
          // by any EC addition/subtraction/doubling routine,
          // including

          //     ecaffine_addition,
          //     ecaffine_addition_subtraction
          //     ecaffine_line_through
          //     ecaffine_on_curve
          //     ecaffine_PPQ
          //     ecaffine_random

#define ecNIST_GFp       1
                           // Curves y^2 = x^3 + ax + b mod p,
                           // where p is an odd prime.
                           // Group order is a prime Gr.
                           // a = p - 3 for NIST curves.
#define ecNIST_GF2m      2
                           // y^2 + xy = x^3 + ax^2 + b over GF(2^m)
                           // Group order is 2*Gr where Gr is prime.
                           // a = 1 for NIST curves.
#define ecNIST_Koblitz2  3
                           // y^2 + x*y = x^3 + ax^2 + 1
                           // where a = 0 or 1.
                           // Group order is 2*Gr if a = 1
                           // and 4*Gr if a = 0.
/*
     ecNISTtype -- One of ecNIST_GFp, ecNIST_GF2n, ecNIST_Koblitz2.

     field_identification -- Pointer to gf2_predefined_tc
                 structure (for GF(2^m) polynomial basis)
                 or to TBD (for GF(p) fields).
     a        -- Pointer to a.
                 If this is NULL, assume a = 1 (ecNIST_GF2n curves)
                 or a = p - 3 (ecNIST_GFp curves).
                 For ecNIST_Koblitz2 curves, this is not used
     bKoblitz -- Pointer to b, or to Koblitz2_tc struct.
     (Gx, Gy) -- pointers to x and y coordinates of generator,
     Gr       -- pointer to order of generator (prime)

*/


typedef struct ec_NIST_t {
     BYTE         ecNISTtype;
     BYTE         dwords;
     const void  *field_identification;
     ec_exponentiation_t *fexpon;  // Special exponentiation routine
     DWORDC      *a;
     const void  *bKoblitz;
     DWORDC      *Gx;
     DWORDC      *Gy;
     DWORDC      *Gr;
} ec_NIST_t;

typedef const ec_NIST_t ec_NIST_tc;

typedef struct {
    BYTE         a;         // 0 or 1
} Koblitz2_t;

#define NIST_a_default 0
                          // Null pointer
typedef const Koblitz2_t Koblitz2_tc;

extern ec_exponentiation_t ec_Koblitz2_exponentiation;

// ecaffine_attributes2 inspects two points
// being added/subtracted.
// It sets the flags in the struct.

typedef struct {
    BOOL inf1;           // Set if first point infinite
    BOOL inf2;           // Set if second point infinite
    BOOL equal;          // Set if points are equal
    BOOL negatives;      // Set if points are negatives
} ecaffine_attributes2_t;


extern BOOL DRM_API ecaffine_attributes2(
            AFFINE_INPUT, AFFINE_INPUT,
            ecaffine_attributes2_t*,
            CURVE_INPUT, PBIGCTX_ARG);

#ifdef __cplusplus
}
#endif

#endif // ECPRIV_H

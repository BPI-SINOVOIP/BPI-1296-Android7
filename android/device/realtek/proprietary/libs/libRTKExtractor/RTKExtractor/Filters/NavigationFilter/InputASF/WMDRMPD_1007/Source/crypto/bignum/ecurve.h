/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef ECURVE_H
#define ECURVE_H 1
#include "bignum.h"
#include "field.h"

/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

//
//           This file (ecurve.h) has definitions for
//       elliptic curve operations.


//           Over a field GF(q), where q > 3 is prime,
//       an elliptic curve is all points (x, y) in GF(q) x GF(q)
//       satisfying the Weierstrass equation
//
//                y^2 = x^3 + ax + b,          (4a^3 + 27 b^2 != 0)
//
//       plus a point at infinity.  The points form an abelian group
//       under a well-known addition rule.
//
//           Over GF(2^m), the curve is instead
//
//                 y^2 + xy = x^3 + ax^2 + b   (b != 0)

//       Fields a and b of the struct should have length
//       elng = fdesc->elng.
//       generator should have length 2*elng or be digit_NULL.
//       gorder, the order of generator, should be have length
//       elng+1 or be digit_NULL.

//       Field deallocate is the address to pass to free
//       (unless it is digit_NULL) when the curve becomes inactive.

// Points are usually in affine form, with only x and y.
// When b <> 0,  the point at infinity has x = y = 0.
// When b = 0, the point at infinity has x = 0 and y = 1.
// fdesc.elng digit_t words are used for each of x and y,
// with no gap between them.

typedef digit_t ecaffine_t;
typedef const ecaffine_t ecaffine_tc;

#define AFFINE_INPUT ecaffine_tc*
#define AFFINE_OUTPUT ecaffine_t*

Future_Struct(ecurve_t);
#define CURVE_INPUT const struct ecurve_t*
#define CURVE_OUTPUT struct ecurve_t*

typedef BOOL DRM_API ec_exponentiation_t(AFFINE_INPUT, MP_INPUT, DWORDREGC,
                                        AFFINE_OUTPUT, CURVE_INPUT,
                                        PBIGCTX_ARG);

#define ec_exponentiation_NULL ((ec_exponentiation_t*)0)


typedef struct ecurve_t {
             field_desc_tc     *fdesc;
             digit_t           *a;
             digit_t           *b;
             ecaffine_t        *generator;  // Generator of cyclic group
                                            // (affine form, x and y)
             digit_t           *gorder;     // Order of cyclic group
             digit_t           *deallocate;
             ec_exponentiation_t *fexpon;   // Special exponentiation code
                                            // Used for Koblitz curves
             BOOL               free_field; // Flag telling ec_free to call
                                            // Kfree.  For system use only.
             BOOL               biszero;    // Is b == 0?
             DWORDREG           ndigtemps;  // Number of digit_t temporaries
                                            // needed by EC
                                            // addition/subtraction routines:
                                            //
                                            //     ecaffine_addition
                                            //     ecaffine_addition_subtraction
                                            //     ecaffine_line_through
                                            //     ecaffine_on_curve
                                            //     ecaffine_PPQ
                                            //     ecaffine_random
                                            //
                                            // Includes enough for a
                                            // multiplication or inversion
                                            // (or Kmuladd, etc.)
                                            // in the base field.
             DWORDREG           lnggorder;  // Number of significant words
                                            // in *gorder, if not digit_NULL.
             reciprocal_1_t     grecip;     // Reciprocal for gorder
               } ecurve_t;

typedef const ecurve_t ecurve_tc;

/*
     Some elliptic curves are predefined, typically because
     they appear in a standards document.
     Use ec_useNIST to access these.
*/

exportable_var const struct ec_NIST_t ec_NIST_B163;
exportable_var const struct ec_NIST_t ec_SECT163R1;
exportable_var const struct ec_NIST_t ec_NIST_B233;
exportable_var const struct ec_NIST_t ec_NIST_B283;
exportable_var const struct ec_NIST_t ec_NIST_B409;
exportable_var const struct ec_NIST_t ec_NIST_B571;

exportable_var const struct ec_NIST_t ec_NIST_K163;
exportable_var const struct ec_NIST_t ec_NIST_K233;
exportable_var const struct ec_NIST_t ec_NIST_K283;
exportable_var const struct ec_NIST_t ec_NIST_K409;
exportable_var const struct ec_NIST_t ec_NIST_K571;

exportable_var const struct ec_NIST_t ec_NIST_P192;
exportable_var const struct ec_NIST_t ec_NIST_P224;
exportable_var const struct ec_NIST_t ec_NIST_P256;
exportable_var const struct ec_NIST_t ec_NIST_P384;
exportable_var const struct ec_NIST_t ec_NIST_P521;

exportable_var const struct ec_NIST_t ec_WTLS_1;
          /* As of June, 2000, there is no ec_WTLS_2 */
#define ec_WTLS_3 ec_NIST_K163
exportable_var const struct ec_NIST_t ec_WTLS_4;
exportable_var const struct ec_NIST_t ec_WTLS_5;
exportable_var const struct ec_NIST_t ec_WTLS_6;
exportable_var const struct ec_NIST_t ec_WTLS_7;
exportable_var const struct ec_NIST_t ec_WTLS_8;
exportable_var const struct ec_NIST_t ec_WTLS_9;


extern BOOL DRM_API ecaffine_addition
      (AFFINE_INPUT, AFFINE_INPUT, AFFINE_OUTPUT,
       const int, CURVE_INPUT, MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_addition_subtraction
      (AFFINE_INPUT, AFFINE_INPUT, AFFINE_OUTPUT,
       AFFINE_OUTPUT, CURVE_INPUT, MP_MODIFIED, PBIGCTX_ARG);

#define ecaffine_copy(p1, p2, E, ctx) \
            (Kcopy_many((p1), (p2), 2, (E)->fdesc, ctx))

#define ecaffine_equal(p1, p2, E, ctx) \
            (Kequal_many((p1), (p2), 2, (E)->fdesc, ctx))

extern BOOL DRM_API ecaffine_exponentiation1998
          (AFFINE_INPUT, MP_INPUT, DWORDREGC, AFFINE_OUTPUT,
           CURVE_INPUT, PBIGCTX_ARG);

#define ecaffine_exponentiation2001 ecaffine_exponentiation
extern BOOL DRM_API ecaffine_exponentiation
          (AFFINE_INPUT, MP_INPUT, DWORDREGC, AFFINE_OUTPUT,
           CURVE_INPUT, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_is_infinite(AFFINE_INPUT, CURVE_INPUT, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_line_through(AFFINE_INPUT, AFFINE_INPUT,
               AFFINE_OUTPUT, FIELD_OUTPUT, CURVE_INPUT,
               MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_multiply_pm1
          (AFFINE_INPUT, AFFINE_OUTPUT, const int, CURVE_INPUT, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_negate(AFFINE_INPUT, AFFINE_OUTPUT,
                                   CURVE_INPUT, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_on_curve(AFFINE_INPUT, CURVE_INPUT,
                                     const char*, MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_PPQ(AFFINE_INPUT, AFFINE_INPUT,
                                AFFINE_OUTPUT, const int,
                                CURVE_INPUT, MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_random(AFFINE_OUTPUT, CURVE_INPUT,
                                   MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_set_infinite(AFFINE_OUTPUT,
                                         CURVE_INPUT, PBIGCTX_ARG);

extern BOOL DRM_API ec_initialize(FIELD_INPUT, FIELD_INPUT,
                                field_desc_tc*, CURVE_OUTPUT, PBIGCTX_ARG, struct bigctx_t*);

extern BOOL DRM_API ec_free(CURVE_OUTPUT, PBIGCTX_ARG);     // IN/OUT

extern BOOL DRM_API ec_useNIST(const struct ec_NIST_t*,
                              CURVE_OUTPUT, PBIGCTX_ARG);


// The next two are not supported as of April, 2002


EXTERN_INLINE BOOL DRM_API ecaffine_exponentiation_tabular
          (AFFINE_INPUT, DWORDREGC, DWORDREGC, MP_INPUT,
           DWORDREGC, AFFINE_OUTPUT, CURVE_INPUT, PBIGCTX_ARG);

extern BOOL DRM_API ecaffine_table_construction(AFFINE_INPUT, DWORDREGC,
                                               DWORDREGC, AFFINE_OUTPUT,
                                               CURVE_INPUT, PBIGCTX_ARG);

#ifdef __cplusplus
}
#endif

#endif // ECURVE_H

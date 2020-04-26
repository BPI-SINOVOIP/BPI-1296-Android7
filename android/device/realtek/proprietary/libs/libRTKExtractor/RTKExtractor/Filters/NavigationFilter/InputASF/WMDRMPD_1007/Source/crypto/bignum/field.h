/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __FIELD_H
#define __FIELD_H 1
#include "bignum.h"


/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

/*
        This file defines field_desc_t, a struct representing a field.
        The data structures allow
        GF(2^m) or GF(q) (with multiple precision q).
        finite-degree extensions.

        The letter `K' is often used in mathematics
        to denote a field.  We use names like Kadd
        for field addition, since the name `Fadd'
        suggests a floating point addition routine.

        A field element is an array of type digit_t.
        The elng element of the field_sesc_t struct tells its length

    Arithmetic routines:

        Kadd(f1, f2, f3, &fdesc) -- f3 = f1 + f2
        Kdiv(f1, f2, f3, &fdesc, ftemps) -- f3 = f1 / f2
                            Array of fdesc->ndigtemps_arith temps supplied
        Kdot(ops. nop, result, &fdesc, &ftemps) --
                 Evaluate a sum of terms of form x_i or x_i*x_j,
                 where some operands may be negated.
        Kequal (f1, f2,  &fdesc) -- Is f1 == f2?
        Kexpon (f1, exponent, lng_expon, f2, &fdesc) -- f2 = f1^exponent .
        Kimmediate(scalar, f1, &fdesc) -- f1 = scalar (a signed digit_t)
        Kinvert(f1, f2,  &fdesc, ftemps) -- f2 = 1/f1
                            Array of fdesc-->ndigtemps_invert1 supplied
        Kiszero(f1,      &fdesc) -- Is f1 == 0?
        Kmulpower2(f1, ishift, f3, fdesc) -- f3 = f1 * 2^ishift
        Kmul(f1, f2, f3, &fdesc) -- f3 = f1 * f2
        Kmul1998(f1, f2, f3, &fdesc) -- f3 = f1*f2,
        Kmul2000(f1, f2, f3, &fdesc, ftemps) -- f3 = f1*f2,
                            Array of fdesc->ndigtemps_mul temps supplied
        Kmul -- defaults to Kmul2000
        Kmuladd(f1, f2, f3, f4, &fdesc, ftemps) -- f4 = f1 * f2 + f3
                            Array of fdesc->ndigtemps_arith temps supplied
        Kmulsub(f1, f2, f3, f4, &fdesc, ftemps) -- f4 = f1 * f2 - f3
                            Array of fdesc->ndigtemps_arith temps supplied
        Kmulsubfrom(f1, f2, f3, f4, &fdesc, ftemps) -- f4 = f3 - f1 * f2
                            Array of fdesc->ndigtemps_arith temps supplied
        Knegate(f1, f2,  &fdesc) -- f2 = -f1
        Ksqrt(f1, f2,    &fdesc) -- f2 = sqrt(f1) (either root)
        Ksub(f1, f2, f3, &fdesc) -- f3 = f1 - f2

     Mixed base-extension field arithemtic

        Kbase_to_ext(f1, fdesc1, f3, fdesc3)   f3 = f1, converting from
                                               fdesc1 to fdesc3.
                                               fdesc3 is an extension of fdesc1
        Kext_to_base(f1, fdesc1, f3, fdesc3)   f3 = f1, converting from
                                               fdesc1 to fdesc3
                                               fdesc3 is a subfield of fdesc1.
        Kin_base_field(f1, fdesc1, fdesc3)     Test whether f1 (in fdesc1) is an
                                               element of fdesc3.  fdesc3 must
                                               be a subfield of fdesc1.

        Kbase_mul_ext(f1, fdesc1, f2, f3, fdesc3, &temps)
                                               f3 = f1 * f2, where
                                               f1 is in fdesc1 but f2 and f3
                                               are in fdesc3.  fdesc3 must
                                               be an extension of fdesc1.

    Miscellaneous routines:

        Kclear(f1, &fdesc)              -- Set one field element to zero
        Kclear_many(f1, nelmt,    &fdesc) -- Set nelmt elements to zero.
        Kcopy(f1, f2, &fdesc)             -- Copy a field element from f1 to f2.
        Kcopy_many(f1, f2, nelmt, &fdesc) -- Copy nelmt elements from f1 to f2
        Kfree  (&fdesc)                -- Free any memory malloc-ed
                                          when field was initialized.
        Kget_field_size(mpnum, &fdesc) -- Get (finite) field size, as a
                                          normal integer (length elng + 1)
        Kinitialize_extension(&bfdesc, minpoly, degree, &fdesc) --
                                       -- Initialize extension field fdesc
                                          minpoly is monic minimal polynomial,
                                          coefficients in subfield bfdesc.
        Kinitalize_normal2(m, T, &fdesc) -- Initialize for normal basis.
        Kinitialize_polynomial2(minpoly, degree, reduction_code, &fdesc)
                                       -- Initialize GF(2^m) field with
                                             polynomial basis
        Kinitialize_prime(&modulus, &fdesc)
                                       -- Initialize field with prime modulus.
        Krandom     (f1, &fdesc)       -- Set f1 to random value
        Krandom_many(f1, nelmt,     &fdesc) -- Set f1 to random values
                                               (nelmt values)
        Krandom_nonzero(f1,    &fdesc) -- Set f1 to random nonzero element
        Krandom_nonzero_many(f1, nelmt, &fdesc) -- Set f1 to random nonzero
                                                   values (nelmt values)

        K2n_normal_basis_exists(m, T) -- Test for existence of normal basis.


    Polynomials over GF(2), packed coefficients,
                           least significant bit for X^0,
                           most significant word zero filled.
                           Some algorithms have not been optimized.

        mbit3 = GF2_add_diff(p1, nbit1, p2, nbit2, p3)
                         -- Add two polynomials of possibly different length,
                            p3 = p1 + p2.

        GF2_add_same(p1, p2, p3, nbit) -- Add two polynomials of same length,
                            p3 = p1 + p2.

        GF2_extended_GCD(p1, nbit1, p2, nbit2, p1invmodp2, gcd)
                         -- Find gcd = GCD(p1, p2)
                            and pinvmodp2 such that
                            p1*pinvmodp2 == gcd (mod p2).
                            Either output can be digit_NULL.

        GF2_irreducible(p, deg) -- Test whether p is irreducible of degree deg.

        GF2_mod_exp(base, expon, lng_expon, result, modulus, nbit) --
                            Compute result = base^expon (mod modulus).
                            base and result have nbit bits.
                            modulus has degree nbit (i.e., nbit+1 nbits).

        GF2_mod_mul(p1, p2, p3, modulus, pinv, nbit, freduc, temps) --
                            Compute p3 == p1*p2 (mod modulus).
                            p1, p2, p3, pinv have nbit bits.
                            modulus has degree nbit (nbit+1 bits).
                            pinv must be the inverse of modulus,
                            from GF2_poly_inverse.
                            freduc is optionally a special
                            polynomial reduction code.
                            temps may be digit_NULL or a digit_t
                            array of length GF2_mod_mul_temps(nbit)

        GF2_mod_mul_temps(nbit) -- Number of digit_t temporaries
                            needed by GF2_mod_mul.

        DWORDREG GF2_mulk_multiplications(elng) --
                            Return the number of single-precision
                            (RADIX_BITS x RADIX_BITS) multiplications
                            needed to multiply two GF(2) polynomials
                            of length elng.  This is used to compute
                            the space for GF2_multiply_same.

        GF2_multiply_same(a, b, elng, prod, temps. ltemps) --
                            Multiply prod = a * b over GF(2)
                            where a, b have length elng and prod has
                            length 2*elng.
                            temps should be digit_NULL or have ltemps
                            entries of type digit_t, where
                            ltemps > 2*GF2_mulk_multiplications(elng).

        GF2_poly_inverse(poly, deg, pinv) --
                            Compute pinv = FLOOR(X^(2*deg-1) / poly).
                            poly should be monic with degree deg.

        GF2_random(p, nbit) -- Generate random polynomial with nbit bits
                               (degree <= nbit-1).

        GF2_random_monic(p, deg) -- Generate random monic polynomial
                                    of degree deg (deg+1 bits).

        GF2_random_nonzero(p, nbit) -- Generate random nonzero polynomial
                                       with nbit bits (nbit > 0).
*/
#include "bignum.h"

typedef enum {FIELD_TYPE_INVALID = 0,
              FIELD_Q_MP,          // Field mod multiple-precision prime q
              FIELD_2_NORMAL,      // Field GF(2^m) with normal basis over GF(2)
              FIELD_2_POLYNOMIAL}  // Field GF(2^m) with polynomial basis
                                   //   over GF(2)
           field_type_t;

typedef const field_type_t field_type_tc;

#define CHARACTERISTIC_2(fdesc) ((fdesc)->ftype >= FIELD_2_NORMAL)
                         // Test for characteristic 2 field.

#if TARGET_OS == OS_ATTILA
#define FIELD_LONGEST (1 + 255/RADIX_BITS)
#else
#define FIELD_LONGEST MP_LONGEST
#endif
typedef digit_t flongest_t[FIELD_LONGEST];
           // Avoid using FIELD_LONGEST in new code

#define k2nmulshifts_NULL ((struct k2nmulshifts_t*)0)
                    // See field2n.c for struct definition.

   //   Special GF(2^m) fields defined by sparse polynomials.

typedef BOOL DRM_API gf2_reduction_t(MP_MODIFIED, PBIGCTX_ARG);
#define gf2_reduction_NULL ((gf2_reduction_t*)0)
                                      // See gf2NIST.c

typedef struct {
    DWORD weight;             // 3 for trinomial, 5 for pentanomial
    DWORD exponents[5];       // Nonzero coefficients. exponents[0] is degree.
    gf2_reduction_t *freduc;  // Function to reduce double-length polynomial
                              //   modulo field-defining polynomial
} gf2_predefined_t;

typedef const gf2_predefined_t gf2_predefined_tc;

exportable_var gf2_predefined_tc gf2NIST163;
exportable_var gf2_predefined_tc gf2NIST233;
exportable_var gf2_predefined_tc gf2NIST283;
exportable_var gf2_predefined_tc gf2NIST409;
exportable_var gf2_predefined_tc gf2NIST571;

exportable_var gf2_predefined_tc gf2WTLS113;
exportable_var gf2_predefined_tc gf2WTLS163;

#define FIELD_INPUT  digit_tc*
#define FIELD_OUTPUT digit_t*

Future_Struct(field_desc_t);

typedef BOOL DRM_API field_0op_notemps_t (FIELD_OUTPUT, DWORDREGC,
                                      const struct field_desc_t*, PBIGCTX_ARG);
typedef BOOL DRM_API field_1op_notemps_t (FIELD_INPUT, FIELD_OUTPUT,
                                      DWORDREGC, const struct field_desc_t*,
                                      PBIGCTX_ARG);
typedef BOOL DRM_API field_1op_notemps_err_t (FIELD_INPUT, FIELD_OUTPUT,
                                      DWORDREGC, const struct field_desc_t*,
                                      BOOL*, PBIGCTX_ARG);
typedef BOOL DRM_API field_2op_notemps_t (FIELD_INPUT, FIELD_INPUT, FIELD_OUTPUT,
                                      DWORDREGC, const struct field_desc_t*,
                                      PBIGCTX_ARG);

typedef struct {
          field_2op_notemps_t *adder;       // x + y
          BOOL        (DRM_API *equaler)     (FIELD_INPUT, FIELD_INPUT,
                                             DWORDREGC,
                                             const struct field_desc_t*,
                                             PBIGCTX_ARG);
                                            // Does x == y?


          //
          //  BEVANS -- Removed exponentiator1 -- it's currently unreferenced.
          //

//          BOOL        (DRM_API *exponentiator1) (FIELD_INPUT, MP_INPUT,
//                                                DWORDREGC, FIELD_OUTPUT,
//                                                const struct field_desc_t*,
//                                                PBIGCTX_ARG);



                                            // Compute base^exponent
                                            // (exponent an integer)
          BOOL        (DRM_API *freer)       (struct field_desc_t*, PBIGCTX_ARG);
                                             // Free ftype-dependent data when
                                             // Kfree called.
          BOOL        (DRM_API *immediater)  (sdigit_tc*, FIELD_OUTPUT,
                                             DWORDREGC,
                                             const struct field_desc_t*,
                                             PBIGCTX_ARG);

          BOOL        (DRM_API *inverter1)   (FIELD_INPUT, FIELD_OUTPUT,
                                             const struct field_desc_t*,
                                             const struct digit_tempinfo_t*,
                                             PBIGCTX_ARG);
          BOOL        (DRM_API *iszeroer)    (FIELD_INPUT, DWORDREGC,
                                             const struct field_desc_t*,
                                             PBIGCTX_ARG);
                                            // Does x == 0?
          BOOL        (DRM_API *mulpower2er) (FIELD_INPUT, const int,
                                             FIELD_OUTPUT, DWORDREGC,
                                             const struct field_desc_t*,
                                             PBIGCTX_ARG);
                                            // Multiply by 2^(second argument)
          BOOL        (DRM_API *multiplier1) (FIELD_INPUT, FIELD_INPUT,
                                             FIELD_OUTPUT,
                                             const struct field_desc_t*,
                                             MP_MODIFIED, PBIGCTX_ARG);
                                            // x * y

          field_1op_notemps_t *negater;     // -x
          field_1op_notemps_err_t *quad1_solver; // Find x given x^2 + x
                                             // (characteristic 2 only)
          
          //
          //  BEVANS:  Removed the following pointer because it is never referenced (randomizer).
          //
          
          // field_0op_notemps_t *randomizer;  // random
          BOOL        (DRM_API *sizer) (MP_OUTPUT,  const struct field_desc_t*,
                                       PBIGCTX_ARG);
                                // Return size of field (array length elng+1)
          //
          //  BEVANS:  Removed the following pointer because it is never referenced (sqrter).
          //
          //field_1op_notemps_err_t *sqrter;   // sqrt(x)


          field_2op_notemps_t *subtracter;   // x - y
          field_0op_notemps_t *zeroizer;     // 0
} field_arithmetic_t;

#define field_arithmetic_NULL ((field_arithmetic_t*)0)

#define Kadd_many(f1, f2, f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->adder) ((f1), (f2), (f3), (nelmt), (fdesc), ctx))
#define Kclear_many(      f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->zeroizer)  ((f3), (nelmt), (fdesc), ctx))
#define Kequal_many(f1, f2,   nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->equaler)   ((f1), (f2), (nelmt), (fdesc), ctx))
#define Kget_field_size(mp, fdesc, ctx) \
        (((fdesc)->arithmetic->sizer) ((mp), (fdesc), ctx))

#define Kimmediate_many(sarray, f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->immediater) \
                ((sarray), (f3), (nelmt), (fdesc), ctx))
#define Kiszero_many(f1,      nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->iszeroer)  ((f1), (nelmt), (fdesc), ctx))
#define Kmulpower2_many(f1, ishift, f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->mulpower2er) \
         ( (f1), (ishift), (f3), (nelmt), (fdesc), ctx))
#define Knegate_many(f1,  f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->negater)   ((f1), (f3), (nelmt), (fdesc), ctx))
#define Krandom_many(     f3, nelmt, fdesc, ctx) \
        (((fdesc)->arithmetic->randomizer)((f3), (nelmt), (fdesc), ctx))
#define Ksqrt_many(f1,    f3, nelmt, fdesc, are_squares, ctx) \
        (((fdesc)->arithmetic->sqrter) \
         ((f1), (f3), (nelmt), (fdesc), (are_squares), ctx))
#define Ksub_many(f1, f2, f3, nelmt, fdesc, ctx) \
      (((fdesc)->arithmetic->subtracter) \
             ((f1), (f2), (f3), (nelmt), (fdesc), ctx))

// N.B.  Prior to March, 2002, Kcopy, Kclear, Krandom had an extra argument.
//       Those older versions are now named
//       Kcopy_many, Kclear_many, Krandom_many.
//       are_squares argument added to Ksqrt_many in November, 2002


#define Kadd(f1, f2, f3, fdesc, ctx)   \
        Kadd_many(   (f1), (f2), (f3), 1, (fdesc), ctx)
#define Kbase_to_ext(f1, fdesc1, f3, fdesc3, ctx) \
        Kbase_to_ext_many((f1), (fdesc1), (f3), 1, (fdesc3), ctx)
#define Kclear(      f3, fdesc, ctx)   \
        Kclear_many(             (f3), 1, (fdesc), ctx)
#define Kcopy(f1,    f3, fdesc, ctx)   \
        Kcopy_many(  (f1),       (f3), 1, (fdesc), ctx)
#define Kequal(f1, f2,   fdesc, ctx)   \
        Kequal_many( (f1), (f2),       1, (fdesc), ctx)
#define Kexpon(f1, exponent, lng_expon, f3, fdesc, ctx) \
                     (((fdesc)->arithmetic->exponentiator1) \
                       ( (f1), (exponent), (lng_expon), (f3), (fdesc), ctx ) )

#define Kext_to_base(f1, fdesc1, f3, fdesc3, ctx) \
        Kext_to_base_many((f1), (fdesc1), (f3), 1, (fdesc3), ctx)

#define Kin_base_field(f1, fdesc1, fdesc3, ctx) \
        Kin_base_field_many((f1), (fdesc1), 1, (fdesc3), ctx)
#define Kin_base_field_many(f1, fdesc1, nelmt, fdesc3, ctx) \
        Kext_to_base_many((f1), (fdesc1), digit_NULL, (nelmt), (fdesc3), ctx)

#define Kiszero(f1,      fdesc, ctx)   \
        Kiszero_many((f1),             1, (fdesc), ctx)
#define Kmulpower2(f1, ishift, f3, fdesc, ctx)\
        Kmulpower2_many((f1), (ishift), (f3), 1, (fdesc), ctx)
#define Knegate(f1,  f3, fdesc, ctx)   \
        Knegate_many((f1),       (f3), 1, (fdesc), ctx)
#define Krandom(     f3, fdesc, ctx)   \
        Krandom_many(            (f3), 1, (fdesc), ctx)
#define Ksqrt(f1,    f3, fdesc, is_square, ctx)  \
        Ksqrt_many(  (f1),       (f3), 1, (fdesc), (is_square), ctx)
#define Ksub(f1, f2, f3, fdesc, ctx)   \
        Ksub_many(   (f1), (f2), (f3), 1, (fdesc), ctx)


typedef struct field_desc_t {
          DWORDREG     elng;   // Length of each field element, in digit_t's
          DWORDREG     degree; // Extension degree m if GF(2^m)
                               // Also used for extension fields
          DWORDREG     ndigtemps_arith;  // Number of digit_t temporaries
                                         // adequate for any of the following:

                                         // Kbase_mul_ext
                                         //   (use subfield->ndigtemps_arith)
                                         // Kdiv
                                         // Kinvert
                                         // Kinvert_many
                                         // Kmul2000
                                         // Kmuladd
                                         // Kmulsubfrom
          DWORDREG     ndigtemps_mul;
                               // Number of digit_t temporaries
                               // needed for a multiplication.
          DWORDREG     ndigtemps_invert1;
                               // Number of digit_t temporaries
                               // needed for an inversion.
          field_type_t ftype;  // Type of base field
          BOOL         free_modulus;   // If TRUE, Kfree frees
                                       // the modulo field.
                                       // Can be set by application.
          digit_t      *one;   // Constant 1
          digit_t      *deallocate;

          const field_arithmetic_t *arithmetic;
                                     // Pointers to arithmetic routines

// Next items apply only for extension fields
          const struct field_desc_t *base_field;  // Recursive data type
                                 // field_desc_NULL if not an extension field.
// Next item applies for extension fields and FIELD_2_POLYNOMIAL
          digit_t      *minimal_polynomial;

// Next items apply only if ftype = FIELD_Q_MP
          mp_modulus_tc *modulo;      // Information about q
          digit_t *inverse_adjustment;
                                      // Multiplier to adjust reciprocal
                                      // for FROM_RIGHT arithmetic
// Next items apply only if ftype = FIELD_2_NORMAL
          DWORDREG     T;      // See field2n.c
          DWORDREG     lng_mulshifts;
          struct k2nmulshifts_t *mulshifts;
                               // Pairs of shifts.  See field2n.c
// Next items apply only if ftype = FIELD_2_POLYNOMIAL or extension field.
//
          DWORDREG     nonzero_trace_power;
                               // Chosen so trace(alpha^nonzero_trace_power) = 1
          digit_t      *leading_inverse;
                     // FLOOR(X^(2*degree-1) / minimal_polynomial)
                     // N.B. Either leading_inverse = NULL or freduc = NULL.
// For extension fields, nonzero_trace_power is chosen so
//     minimal_polynomial = X^degree
//        + (nonzero const)*X^(nonzero_trace_power - 1) + ...
//     That is, it counts 1 + degree(minimal polynomial, minus leading term).

//     leading_inverse has signed bytes converting these low coefficients
//     into signed integers in [-16, 16], if possible.

          gf2_reduction_t  *freduc;

// Following apply to FIELD_2_NORMAL and FIELD_2_POLYNOMIAL.

          DWORDREG     unused_bits_top;  // elng*RADIX_BITS - degree
          digit_t      mask_used_bits;
                                  // Mask to avoid extraneous bits in
                                  // most significant word of a field element
} field_desc_t;


typedef const field_desc_t field_desc_tc;

#define field_desc_NULL ((field_desc_t*)0)


//     The following typedefs are for functions
//     indexed indirectly via pointers.
//     A call to GF2_get_funcs (after mp_initialization)
//     selects the best version of each routine
//     (depending possibly upon the client's hardware,
//     such as availability of MMX) and initializes the pointers.


typedef BOOL Stdcall86 gf2mul_t(MP_MODIFIED, DWORDREGC, PBIGCTX_ARG);

typedef BOOL Stdcall86 gf2mat22_t(digit_tc, digit_tc,
                                  digit_tc, digit_tc,
                                  digit_t[4], PBIGCTX_ARG);

typedef BOOL Stdcall86 gf2mul22_t(digit_tc[4],
                              MP_MODIFIED, MP_MODIFIED, DWORDREGC, MP_OUTPUT,
                              MP_MODIFIED, MP_MODIFIED, DWORDREGC, MP_OUTPUT,
                              PBIGCTX_ARG);

#define   gf2mul_NULL ((gf2mul_t*)0)
#define gf2mat22_NULL ((gf2mat22_t*)0)
#define gf2mul22_NULL ((gf2mul22_t*)0)

typedef struct {
    gf2mul_t   *pgf2mul;
    gf2mat22_t *pgf2mat22;
    gf2mul22_t *pgf2mul22;
} GF2_funcs_t;


extern BOOL DRM_API GF2_get_funcs(GF2_funcs_t*, PBIGCTX_ARG);

extern DWORDREG DRM_API GF2_add_diff(FIELD_INPUT, DWORDREGC,
                                    FIELD_INPUT, DWORDREGC,
                                    FIELD_OUTPUT, PBIGCTX_ARG);

extern BOOL DRM_API GF2_add_same(FIELD_INPUT, FIELD_INPUT,
                                FIELD_OUTPUT, DWORDREGC, PBIGCTX_ARG);

#define GF2_copy(p1, p2, nbit, ctx) \
        (mp_copy(p1, p2, BITS_TO_DIGITS(nbit), ctx), TRUE)
#define GF2_equal(p1, p2, nbit, ctx) \
        (compare_same(p1, p2, BITS_TO_DIGITS(nbit))==0)
#define GF2_iszero(p1, nbit, ctx) (all_zero(p1, BITS_TO_DIGITS(nbit)))

extern BOOL DRM_API GF2_double_reduce_general(FIELD_INPUT, FIELD_INPUT,
                            FIELD_INPUT, DWORDREGC, FIELD_OUTPUT, PBIGCTX_ARG);

extern BOOL DRM_API GF2_extended_GCD(FIELD_INPUT, DWORDREGC, FIELD_INPUT,
                            DWORDREGC, FIELD_OUTPUT, FIELD_OUTPUT, PBIGCTX_ARG);

extern BOOL DRM_API GF2_irreducible(FIELD_INPUT, DWORDREGC, PBIGCTX_ARG);

#define GF2_monic(f, deg, ctx) \
               ((f)[(deg)/RADIX_BITS] >> ((deg) % RADIX_BITS) == 1)
                      // Is f monic of degree deg?

extern BOOL DRM_API GF2_mod_exp(FIELD_INPUT, MP_INPUT, DWORDREGC,
                        FIELD_OUTPUT, FIELD_INPUT, DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API GF2_mod_mul(FIELD_INPUT, FIELD_INPUT, FIELD_OUTPUT,
                        FIELD_INPUT, FIELD_INPUT, DWORDREGC,
                        gf2_reduction_t*, MP_MODIFIED, PBIGCTX_ARG);

extern DWORDREG DRM_API GF2_mulk_multiplications(DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API GF2_multiply_same(FIELD_INPUT, FIELD_INPUT, DWORDREGC,
                                     FIELD_OUTPUT, MP_MODIFIED,
                                     DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API GF2_poly_inverse(FIELD_INPUT, DWORDREGC,
                                    FIELD_OUTPUT, PBIGCTX_ARG);

extern BOOL DRM_API GF2_random        (FIELD_OUTPUT, DWORDREGC, PBIGCTX_ARG);
extern BOOL DRM_API GF2_random_monic  (FIELD_OUTPUT, DWORDREGC, PBIGCTX_ARG);
extern BOOL DRM_API GF2_random_nonzero(FIELD_OUTPUT, DWORDREGC, PBIGCTX_ARG);

extern BOOL Stdcall86 GF2_square(FIELD_INPUT, DWORDREGC,
                                      FIELD_OUTPUT, PBIGCTX_ARG);

extern BOOL DRM_API Kbase_mul_ext(FIELD_INPUT, field_desc_tc*,
                                 FIELD_INPUT, FIELD_OUTPUT, field_desc_tc*,
                                 MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API Kbase_to_ext_many(FIELD_INPUT, field_desc_tc*,
                                     FIELD_OUTPUT, DWORDREGC,
                                     field_desc_tc*, PBIGCTX_ARG);
extern BOOL DRM_API Kext_to_base_many(FIELD_INPUT, field_desc_tc*,
                                     FIELD_OUTPUT, DWORDREGC,
                                     field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Kcopy_many(FIELD_INPUT, FIELD_OUTPUT,
                              DWORDREGC, field_desc_tc*, PBIGCTX_ARG);
extern BOOL DRM_API Kdiv(FIELD_INPUT, FIELD_INPUT, FIELD_OUTPUT,
                        field_desc_tc*, MP_MODIFIED, PBIGCTX_ARG);
extern BOOL DRM_API Kfree   (field_desc_t*, PBIGCTX_ARG);
extern BOOL DRM_API Kimmediate(sdigit_tc, FIELD_OUTPUT,
                              field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API Kinitialize_extension(field_desc_tc*, FIELD_INPUT,
                                         DWORDREGC, field_desc_t*, PBIGCTX_ARG);
extern BOOL DRM_API Kinitialize_normal2(DWORDREGC, DWORDREGC,
                                       field_desc_t*, PBIGCTX_ARG);
extern BOOL DRM_API Kinitialize_polynomial2(FIELD_INPUT, DWORDREGC,
                                   gf2_reduction_t*, field_desc_t*,
                                   PBIGCTX_ARG);
extern BOOL DRM_API Kinitialize_prime(mp_modulus_tc*, field_desc_t*,
                                     struct bigctx_t *,
                                     PBIGCTX_ARG);
extern BOOL DRM_API Kinvert(FIELD_INPUT, FIELD_OUTPUT,
                           field_desc_tc*, MP_MODIFIED, PBIGCTX_ARG);
extern BOOL DRM_API Kinvert_many(FIELD_INPUT, FIELD_OUTPUT,
                                DWORDREGC, field_desc_tc*,
                                MP_MODIFIED, PBIGCTX_ARG);

#define Kmul1998(f1, f2, f3, fdesc, ctx) \
        Kmul2000(f1, f2, f3, fdesc, digit_NULL, ctx)
#define Kmul2000(f1, f2, f3, fdesc, temps, ctx) Kmul_many((f1), (f2),\
                                           (f3), 1, (fdesc), (temps), ctx)
#define Kmul Kmul2000
// Old code may use Kmul with only four arguments (no temps).
// Those applications should   #undef Kmul  and   #define Kmul Kmul1998
// until the source code is converted.



extern BOOL DRM_API Kmul_many(FIELD_INPUT, FIELD_INPUT, FIELD_OUTPUT,
                             DWORDREGC, field_desc_tc*,
                             MP_MODIFIED, PBIGCTX_ARG);
extern BOOL DRM_API Kmuladd (FIELD_INPUT, FIELD_INPUT,
                          FIELD_INPUT, FIELD_OUTPUT,
                          field_desc_tc*, MP_MODIFIED, PBIGCTX_ARG);
extern BOOL DRM_API Kmulsub (FIELD_INPUT, FIELD_INPUT,
                          FIELD_INPUT, FIELD_OUTPUT,
                          field_desc_tc*, MP_MODIFIED, PBIGCTX_ARG);
extern BOOL DRM_API Kmulsubfrom(FIELD_INPUT, FIELD_INPUT, FIELD_INPUT,
                               FIELD_OUTPUT, field_desc_tc*,
                               MP_MODIFIED, PBIGCTX_ARG);
#define K2_quad1(f1, f3, fdesc, psuccess, ctx) \
                        ((fdesc)->arithmetic->quad1_solver) \
                         ((f1), (f3), 1, (fdesc), (psuccess), ctx)

#define Krandom_nonzero(f3, fdesc, ctx) \
            Krandom_nonzero_many((f3), 1, (fdesc), ctx)
extern BOOL DRM_API Krandom_nonzero_many(FIELD_OUTPUT, DWORDREGC,
                                        field_desc_tc*, PBIGCTX_ARG);

extern BOOL DRM_API K2n_normal_basis_exists(DWORDREGC,
                                           DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API Kprime_sqrter
        (digit_tc      *f1,        // IN
         digit_t       *f3,        // OUT
         DWORDREGC      nelmt,     // IN
         field_desc_tc *fdesc,     // IN
         BOOL          *psquares,  // OUT
         PBIGCTX_ARG);

#ifdef __cplusplus
}
#endif

#endif /* __FIELD_H */

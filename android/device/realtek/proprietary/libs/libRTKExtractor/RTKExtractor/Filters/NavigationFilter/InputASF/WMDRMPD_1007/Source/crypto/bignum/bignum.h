/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef BIGNUM_H              /* If not previously #included */
#define BIGNUM_H 1

/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

#include "bigdefs.h"


#if TARGET_OS == OS_ATTILA
#define MP_LONGEST_BITS 1024
#else
#define MP_LONGEST_BITS 2048
#endif
                        /*
                           Multiple precision moduli can have up to
                           MP_LONGEST_BITS bits, which is
                           MP_LONGEST words.  Some routines allow
                           longer operands.

                           Older codes have used this (and MP_LONGEST, below)
                           to dimension arrays.  New code should avoid
                           referencing MP_LONGEST and MP_LONGEST_BITS.

                        */


                        /*
                           Error messages are not printed in the
                           production version of the code.
                           In the test version, compiled
                           with PRINT_ERROR_MESSAGES=1,
                           they are printed,
                        */



/*
      _bignum_SOURCE_INFO() returns a string with file name
       and line number information
      (identifying where macro is invoked, not where it is declared).
*/

#define _bignum_SOURCE_INFO() __FILE__ " line " _bignum_STRINGIFY(__LINE__)
#define _bignum_STRINGIFY(arg) _bignum_STRINGIFY1(arg)
#define _bignum_STRINGIFY1(arg) #arg


/*
    _bignum_DEBUG_ARGUMENT is used to pass an argument only during debugging.
    In release mode a zero is passed.
    It could be used to pass a string identifying the caller, for example.
*/


#if PRINT_ERROR_MESSAGES
    #define _bignum_DEBUG_ARGUMENT(arg) (arg)
#else     
    #define _bignum_DEBUG_ARGUMENT(arg) 0
#endif

#define MP_LONGEST (MP_LONGEST_BITS/RADIX_BITS)


/*
        USEASM_ALPHA, ... specify whether to use assembly language,
        if it has been written for a platform.
*/
// #define USEASM 0

#ifndef USEASM
    #if TARGET == TARGET_ALPHA
        #define USEASM 1
    #elif TARGET == TARGET_AMD64
        #define USEASM 1
    #elif TARGET == TARGET_ARM && TARGET_OS == OS_ATTILA
        #define USEASM 1
    #elif TARGET == TARGET_IA64
        #define USEASM 1
    #elif TARGET == TARGET_IX86
        #define USEASM 1
    #elif TARGET == TARGET_MIPS
        #define USEASM 1
    #elif TARGET == TARGET_PPC
        #define USEASM 1
    #elif TARGET == TARGET_SHX
        #define USEASM 1
    #elif TARGET == TARGET_SPARC
        #define USEASM 1
    #else
        #define USEASM 0
    #endif
#endif

#if !defined(USEASM) || (USEASM != 0 && USEASM != 1)
    #error "USEASM not defined"
#endif

#define USEASM_ALPHA    (USEASM && TARGET == TARGET_ALPHA)
#define USEASM_AMD64    (USEASM && TARGET == TARGET_AMD64)
#define USEASM_ARM      (USEASM && TARGET == TARGET_ARM)
#define USEASM_IA64     (USEASM && TARGET == TARGET_IA64)
#define USEASM_IX86     (USEASM && TARGET == TARGET_IX86)
#define USEASM_MIPS     (USEASM && TARGET == TARGET_MIPS)
#define USEASM_PPC      (USEASM && TARGET == TARGET_PPC)
#define USEASM_SHX      (USEASM && TARGET == TARGET_SHX)
#define USEASM_SPARC    (USEASM && TARGET == TARGET_SPARC)


#if USEASM_SHX
void __asm(const char*, ...);  // this declaration needed to allow inline of asm
#endif

/*
        Some non-inlined x86 assembly routines are declared naked,
        so they do their own stack management and register saving.

        When using a DLL on Intel platforms, all functions use
        the __stdcall convention, so the assembly routines use it too.
        To ensure they are called with the __stdcall
        conventions always (i.e., even when compiled under Microsoft
        Developer Studio), we put __stdcall explicitly in the prototypes.
*/

#if USEASM_IX86    // TBD -- eliminate this
    #define Naked86 __declspec(naked)
    #define Stdcall86 __stdcall
#else
    #define Naked86
    #define Stdcall86
#endif


/*
        DOUBLE_SHIFT_LEFT(n1, n0, amt) returns
        n1 shifted left by amt bits,
        with new bits coming in from the top of n0.

        DOUBLE_SHIFT_RIGHT(n1, n0, amt) returns n0 shifted right
        by amt bits, with new bits coming from the bottom of n1.

        The shift counts must satisfy 0 <= amt <= RADIX_BITS - 1.
        The shift by    RADIX_BITS - amt   is done in two stages
        (first by 1, then by RADIX_BITS - 1 - amt),
        to avoid an illegal shift count of RADIX_BITS when amt = 0.

        DOUBLE_SHIFT_LEFT_NONZERO and DOUBLE_SHIFT_RIGHT_NONZERO
        are similar, but disallow a zero shift count, allowing the
        RADIX_BITS - amt shift to be done in one stage,
        DOUBLE_SHIFT_LEFT_NONZERO(n1, n0, amt) is the same as
        DOUBLE_SHIFT_RIGHT_NONZERO(n1, n0, RADIX_BITS - amt).

        TBD -- If the x86 VC compiler optimizes __int64 shifts,
        (6.0 SP3 does not), try to rewrite these definitions to generate
        SHLD and SHRD instructions..
*/

#define DOUBLE_SHIFT_LEFT(n1, n0, amt)  \
        (((n1) << (amt)) | (((n0) >> 1) >> (RADIX_BITS - 1 - (amt))))

#define DOUBLE_SHIFT_LEFT_NONZERO(n1, n0, amt)  \
        (((n1) << (amt)) | ((n0) >> (RADIX_BITS - (amt))))

#define DOUBLE_SHIFT_RIGHT(n1, n0, amt)  \
        (((n0) >> (amt)) | (((n1) << 1) << (RADIX_BITS - 1 - (amt))))

#define DOUBLE_SHIFT_RIGHT_NONZERO(n1, n0, amt)  \
        (((n0) >> (amt)) | ((n1) << (RADIX_BITS - (amt))))

#include "dblint.h"

#define exportable_var extern
#define exportable_var_declaration

/*
      Some struct names are referenced in #include files before they are
      defined.  For example, there might be two struct definitions each
      containing a pointer to the other struct type.
      We list some struct names in advance here, to avoid warnings.
*/
Future_Struct(mp_modulus_t);      // See this file
Future_Struct(digit_tempinfo_t);  // See bigpriv.h

#ifndef __cplusplus
// typedef struct ec_NIST_t;          TBD   // See ecpriv.h
// typedef struct k2nmulshifts_t;        // See field2n.c
#endif

/*
        The reciprocal_1_t type is used when div21
        or divide or divide_immediate would otherwise
        divide by the same number repeatedly.  See file divide.c.
*/

typedef struct {
                digit_t    multiplier;
                DWORDREG   shiftamt;
               } reciprocal_1_t;

typedef const reciprocal_1_t reciprocal_1_tc;

/*
        mp_modulus_t struct has modulus-dependent constants
        used for fast reduction (typically for a fixed modulus,
        which will be used several times, as in modular exponentiation).
        These constants are initialized by function create_modulus:

        modulus -- Modulus used for computations.  Must be nonzero.

        length  -- Length (>= 1) of the modulus, without leading zeros.
                   Operands to mod_add, mod_mul, mod_sub, ...
                   are assumed to have this length.

        reddir  -- Equal to FROM_LEFT if reductions of
                   products are done from the left (traditional
                   division), and to FROM_RIGHT if reductions of
                   products are done from the right (Montgomery reduction).

                   When using FROM_RIGHT, the modulus must be odd.
                   Arguments to mod_mul should be pre-scaled by
                   2^scaling_power2 (mod modulus).
                   The product will be similarly scaled.

        scaling_power2 --  Equal to length*RADIX_BITS when reddir = FROM_RIGHT.
                   Zero if reddir = FROM_LEFT.

        one --     Constant 1 (muldiplicative identity), length length.
                   Nmerical value is 2^scaling_power2 (mod modulus).
    Denote

              length = pmodulo->length
              modulus = pmodulo->modulus
              shiftamt = pmodulo->left_reciprocal_1.shiftamt.

          Then

              0 <= shiftamt < RADIX_BITS
              RADIX^length/2 <= modulus * 2^shiftamt < RADIX^length
              modulus < RADIX^length / 2^shiftamt <= 2*modulus

          Some variants of modmul_algorithm use additional constants
          lngred2, multiplier_first, multiplier_second.
          FROM_LEFT arithmetic, these constants satisfy

              modulus * (multiplier_second + RADIX^lngred2)
            = RADIX^(length + lngred2) / 2^shiftamt + multiplier_first

              0 <= multiplier_first < modulus
              0 <= multiplier_second < RADIX^lngred2
              lngred2 = CEIL(length/2)

         For FROM_RIGHT arithmetic, these constants satisfy

              multiplier_second * modulus
            = 1 + multiplier_first * RADIX^lngred2

              0 <= multipler_first < modulus
              0 <= multiplier_second < RADIX^lngred2
              lngred2 = CEIL(length/2)

        one --     Constant 1 (multiplicative identity).
                   For FROM_LEFT arithmetic, pmodulo->one = 1.
                   For FROM_RIGHT arithmetic,
                       pmodulo->one = (RADIX^length) % pmodulus;

        left_reciprocal_1 -- Reciprocal of the divisor starting at the
                   leftmost digit (i.e., modulus[length-1]);

                   See file divide.c for an explanation
                   about how this constant is used to get accurate
                   quotients when dividing from the left.

        right_reciprocal_1 -- If modulus is odd, this holds
                   1/modulus (mod RADIX), for use in mod_shift.
                   Otherwise the field is zero.

          Denote

              length = pmodulo->length
              modulus = pmodulo->modulus
              shiftamt = pmodulo->left_reciprocal_1.shiftamt.

          Then

              0 <= shiftamt < RADIX_BITS
              RADIX^length/2 <= modulus * 2^shiftamt < RADIX^length
              modulus < RADIX^length / 2^shiftamt <= 2*modulus

        modmul_algorithm --
              This library has a variety of codes for modular multiplication.
              The mp_modulus_t struct has a pointer to the precise code
              being used for a particular number and architecture.  A call

                    (*modmul_algorithm)(a, b, c, pmodulo, temps)

              is supposed to set

                     c = a*b/3^(pmodulo->scaling_power2)  (mod pmodulo->modulus)

              where 0 <= a, b, < pmodulo->modulus.
              The output c may overlap a or b.

              The temps array will have at least pmodulo->modmul_algorithm_temps
              elements of type digit_t, aligned on a digit_t boundary.

              The simplest modmul_algoriuthm procedures,
              modmul_from_left_default and modmul_from_right_default,
              work on all architectures.

              In some implementations of this library, create_modulus may
              examine the precise modulus and the precise hardware
              we are running on, substituting another algorithm
              or an assembly language code.

              Some variants of modmul_algorithm use additional constants
              lngred2, multiplier_first, multiplier_second.
              In FROM_LEFT arithmetic, these constants satisfy

                  modulus * (multiplier_second + RADIX^lngred2 + 1))
                = RADIX^(length + lngred2) / 2^shiftamt + multiplier_first

                  0 <= multiplier_first < modulus
                  0 <= multiplier_second < RADIX^lngred2
                  lngred2 = CEIL(length/2)

             For FROM_RIGHT arithmetic, these constants satisfy

                  multiplier_second * modulus
                = 1 + multiplier_first * RADIX^lngred2

                  0 <= multipler_first < modulus
                  0 <= multiplier_second < RADIX^lngred2
                  lngred2 = CEIL(length/2)
*/

Future_Struct(mp_modulus_t);

typedef BOOL DRM_API modmul_algorithm_t (MP_INPUT, MP_INPUT, MP_OUTPUT,
                                   const struct mp_modulus_t*,
                                   MP_MODIFIED, PBIGCTX_ARG);

typedef enum {FROM_LEFT, FROM_RIGHT} reddir_t;
typedef const reddir_t reddir_tc;

typedef struct mp_modulus_t {
                  DWORDREG  length;         // Length passed to create_modulus
                  DWORDREG  lngred2;        // CEIL(length/2)
                  DWORDREG  modmul_algorithm_temps; // Number of digit_t temps
                                                    // used by modmul_algorithm
                  int       scaling_power2; // RADIX_BITS*length for FROM_RIGHT,
                                            // 0 for FROM_LEFT
                  reddir_t  reddir;         // FROM_LEFT or FROM_RIGHT */
                  reciprocal_1_t  left_reciprocal_1;
                  digit_t   right_reciprocal_1;  // 1/modulus[0] mod RADIX,
                                                 // if modulus is odd
                  digit_t   *modulus;
                  digit_t   *multiplier_first;  // See text
                  digit_t   *multiplier_second; // See text
                  digit_t   *one;               // Multiplicative constant 1
                  digit_t   *cofactor;
                             /*
                                In factorization codes, this holds
                                the cofactor (length length) after
                                dividing modulus by any factors found.
                                Used by gcdex_jacobi.
                            */
                  modmul_algorithm_t *modmul_algorithm;
                                  // Function pointer for multiplication
                  const char *modmul_algorithm_name;
                                  // Name of modmul_algorithm procedure
                                  // while debugging or testing, else zero.
                } mp_modulus_t;

typedef const mp_modulus_t mp_modulus_tc;
#define mp_modulus_NULL ((mp_modulus_t*)0)

/*
        mod_exp2000 returns statistics on what happened during an
        exponentiation.
*/

typedef struct {              // Statistics from mod_exp2000
                              // This struct may change in future versions.
        DWORDREG cnt_mod_add;           // Calls to mod_add
        DWORDREG cnt_mod_mul;           // Calls to mod_mul
        DWORDREG cnt_mod_shift;         // Calls to mod_shift
        DWORDREG cnt_mp_copy;           // Calls to mp_copy
    } mod_exp_stats_t;

/*
        When an error is detected, the SetMpErrno_clue macro gets
        an error code (below) and an English-language string
        with more information.
        This macro will normally call an application-supplied routine.
        The application routine might print a message or set a global variable.

        The library routine detecting the error will exit with return value
        FALSE, notifying its caller that something abnormal occurred.

        Except for MP_ERRNO_NO_ERROR, the error codes are
        in alphabetical order.
*/

typedef enum {
        MP_ERRNO_NO_ERROR = 0,     // Success
               // Broader codes, introduced September, 2002.
        MP_ERRNO_DEGREE_OVERFLOW,  // Polynomial degree too high
                                   // for allocated memory
        MP_ERRNO_DIVIDE_ZERO, // Divide by zero (or by number with leading zero)
        MP_ERRNO_ELSEWHERE,        // Error indicator returned by some routine
                                   // which may not have called SetMpErrno_clue
                                   // (e.g., CRYPTAPI library, assembly codes)
        MP_ERRNO_INTERNAL_ERROR,   // Bug found -- please report
        MP_ERRNO_INVALID_DATA,     // Invalid arguments
        MP_ERRNO_MODULAR_TOO_BIG,  // Modular operand >= modulus
        MP_ERRNO_NO_MEMORY,        // From mp_alloc_temp - malloc failure
        MP_ERRNO_NOT_IMPLEMENTED,  // Case not implemented
        MP_ERRNO_NOT_INVERTIBLE,   // Perhaps trying to invert modulo non-prime
        MP_ERRNO_NOT_ON_CURVE,     // Point is not on elliptic curve
        MP_ERRNO_NULL_POINTER,   // NULL argument where valid argument expected
        MP_ERRNO_OVERFLOW,       // Integer overflow (or unexpectedly negative)
        MP_ERRNO_OVERLAPPING_ARGS, // Overlapping (i.e., duplicate) arguments
                                   // where they are disallowed
        MP_ERRNO_TOO_MANY_ITERATIONS,  // e.g., unable to find large prime
        MP_ERRNO_ZERO_OPERAND,     // Zero operand(s) where nonzero expected

        MP_ERRNO_COUNT             // Number of entries above
    } mp_errno_t;

typedef const mp_errno_t mp_errno_tc;
extern const char* mp_errno_name(mp_errno_tc);  // Table of strings

//   User-supplied error routines.
//   In debug mode,the SetMpErrno_clue macro passes
//   the string directly to the SetMpErrno_clue1 function.
//   In release mode, this string is replaced by a zero.

extern mp_errno_t GetMpErrno(PBIGCTX_ARG);
extern void SetMpErrno(mp_errno_tc, PBIGCTX_ARG);
extern void SetMpErrno_clue1(mp_errno_tc, const char*, PBIGCTX_ARG);
#define SetMpErrno_clue(errcode, debug_info, ctx) \
        SetMpErrno_clue1(errcode, "", ctx)

/*
        Some routine allow an argument of digit_NULL or
        reciprocal_1_NULL when the corresponding argument
        is not otherwise used.  For example, the division
        routine allows but does not require a
        reciprocal structure as argument,
        and allows the quotient to be suppressed.
*/

#define digit_NULL ((digit_t*)0)
#define reciprocal_1_NULL ((reciprocal_1_t*)0)

/*
        The next several #defines are used in function prototypes.
*/

extern digit_t DRM_API accumulate(MP_INPUT, digit_tc, MP_MODIFIED,
                                 DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API add_diff(MP_INPUT, DWORDREGC, MP_INPUT, DWORDREGC,
                            MP_OUTPUT, digit_t*, PBIGCTX_ARG);

extern BOOL DRM_API add_full(MP_INPUT, DWORDREGC, MP_INPUT,
                            DWORDREGC, MP_OUTPUT, DWORDREG*, PBIGCTX_ARG);

extern digit_t DRM_API add_immediate(MP_INPUT, digit_tc, MP_OUTPUT, DWORDREGC);

extern BOOL DRM_API add_mod(MP_INPUT, MP_INPUT, MP_OUTPUT, MP_INPUT,
                           DWORDREGC, PBIGCTX_ARG);

extern digit_t DRM_API add_same(MP_INPUT, MP_INPUT, MP_OUTPUT, DWORDREGC);

extern BOOL DRM_API add_signed(MP_INPUT, DWORDC, MP_INPUT, DWORDC,
                              MP_OUTPUT, DWORD*, PBIGCTX_ARG);

extern BOOL DRM_API add_signed_immediate(MP_INPUT, DWORDC, sdigit_tc,
                                        MP_OUTPUT, DWORD*, PBIGCTX_ARG);

extern sdigit_t DRM_API add_sub_same(MP_INPUT, MP_INPUT, MP_INPUT,
                                    MP_OUTPUT, DWORDREGC);

extern int DRM_API compare_diff(MP_INPUT, DWORDREGC, MP_INPUT, DWORDREGC);

extern int DRM_API compare_same(MP_INPUT, MP_INPUT, DWORDREGC);

extern int DRM_API compare_sum_diff(MP_INPUT, DWORDREGC,
                                   MP_INPUT, DWORDREGC, MP_INPUT, DWORDREGC);

extern int DRM_API compare_sum_same(MP_INPUT, MP_INPUT, MP_INPUT, DWORDREGC);

extern BOOL DRM_API create_modulus(MP_INPUT, DWORDREGC, reddir_tc,
                                  mp_modulus_t*, PBIGCTX_ARG, 
                                  struct bigctx_t *);

extern BOOL DRM_API dblint_gcd(dblint_tc, dblint_tc, dblint_t*, PBIGCTX_ARG);

extern BOOL DRM_API dblint_ogcd(dblint_tc, dblint_tc, dblint_t*, PBIGCTX_ARG);

extern digit_t DRM_API dblint_sqrt(dblint_tc, PBIGCTX_ARG);

extern digit_t DRM_API decumulate(MP_INPUT, digit_tc, MP_MODIFIED,
                                 DWORDREGC, PBIGCTX_ARG);

extern digit_t* DRM_API digit_allocate_named(DWORDREGC,
                                            const char*, PBIGCTX_ARG);
#define digit_allocate(nelmt, name, ctx) digit_allocate_named(nelmt, \
                                              _bignum_DEBUG_ARGUMENT(name), ctx)

extern DWORDREG DRM_API digit_factor(digit_tc, digit_t[],
                                    DWORDREG[], PBIGCTX_ARG);

extern BOOL DRM_API digit_gcd(digit_tc, digit_tc, digit_t*, PBIGCTX_ARG);

extern BOOL DRM_API digit_jacobi(digit_tc, digit_tc, int*, PBIGCTX_ARG);

extern digit_t DRM_API digit_least_prime_divisor(digit_tc, PBIGCTX_ARG);

extern BOOL DRM_API digit_mod_divide_odd(digit_tc, digit_tc, digit_tc,
                                        digit_t*, PBIGCTX_ARG);

extern BOOL DRM_API digit_ogcd(digit_tc, digit_tc, digit_t*, PBIGCTX_ARG);

#define MP_DECIMAL_BUFFER(lng) (3 + 28*RADIX_BITS*(lng)/93)
       /* Upper bound on length of ASCII decimal output buffer for number
          with lng digit_t's.  Includes sign and trailing zero.
          28/93 = 0.301075 ... is upper bound for log10(2) = 0.301030 ... */

typedef char digit_out_buf_t[MP_DECIMAL_BUFFER(1)];
extern char* DRM_API digit_out  ( digit_tc, digit_out_buf_t,
                                PBIGCTX_ARG); // Decimal unsigned
extern char* DRM_API sdigit_out (sdigit_tc, digit_out_buf_t,
                                PBIGCTX_ARG); // Decimal   signed
extern char* DRM_API digit_out_hex( digit_tc, digit_out_buf_t,
                                  PBIGCTX_ARG);  // Hex unsigned

extern digit_t DRM_API digit_sqrt(digit_tc, PBIGCTX_ARG);

extern BOOL DRM_API div21(dblint_tc, digit_tc, digit_t*, digit_t*, PBIGCTX_ARG);

EXTERN_INLINE BOOL DRM_API div21_fast(dblint_tc, digit_tc,
                       reciprocal_1_tc*, digit_t*, digit_t*, PBIGCTX_ARG);

extern BOOL DRM_API divide(MP_INPUT, DWORDREGC, MP_INPUT, DWORDREGC,
                       reciprocal_1_tc*, MP_OUTPUT, MP_OUTPUT, PBIGCTX_ARG);

extern BOOL DRM_API divide_rounded(MP_INPUT, DWORDREGC, MP_INPUT, DWORDREGC,
                       reciprocal_1_tc*, MP_OUTPUT, MP_OUTPUT,
                       DWORD*, PBIGCTX_ARG);

extern BOOL DRM_API divide_precondition_1(MP_INPUT, DWORDREGC,
                                         reciprocal_1_t*, PBIGCTX_ARG);

extern BOOL DRM_API divide_immediate(MP_INPUT, digit_tc,
                         reciprocal_1_tc*, MP_OUTPUT, DWORDREGC,
                         digit_t*, PBIGCTX_ARG);

EXTERN_INLINE digit_t DRM_API estimated_quotient_1(digit_tc, digit_tc,
                                    digit_tc, reciprocal_1_tc*, PBIGCTX_ARG);

extern BOOL DRM_API find_big_prime(DWORDREGC, MP_INPUT, DWORDREGC,
                           MP_INPUT, DWORDREGC, MP_OUTPUT, PBIGCTX_ARG);

extern BOOL DRM_API from_modular(MP_INPUT, MP_OUTPUT,
                                mp_modulus_tc*, PBIGCTX_ARG);

extern BOOL DRM_API gcdex_jacobi(MP_INPUT, mp_modulus_tc*, MP_OUTPUT,
                                MP_OUTPUT, int*, PBIGCTX_ARG);

// extern BOOL DRM_API get_generator(DWORD*, DWORD*, DWORDC);

// extern BOOL DRM_API get_prime(MP_OUTPUT, DWORDREGC);

extern digit_t* DRM_API low_prime_prod_construction(PBIGCTX_ARG);
extern void DRM_API low_prime_prod_destruction(digit_t*, PBIGCTX_ARG);
extern BOOL DRM_API low_prime_divisibility(MP_INPUT, DWORDREGC,
                                          digit_tc*, digit_t*, PBIGCTX_ARG);

#define mod_add(a, b, c, pmodulo, ctx) add_mod(a, b, c, (pmodulo)->modulus,\
                                                   (pmodulo)->length, ctx)

extern BOOL DRM_API mod_exp(MP_INPUT, MP_INPUT, DWORDREGC, MP_OUTPUT,
                        mp_modulus_tc*, PBIGCTX_ARG);

extern BOOL DRM_API mod_exp2000(MP_INPUT, MP_INPUT, DWORDREGC, MP_OUTPUT,
                        mp_modulus_tc*, mod_exp_stats_t*, PBIGCTX_ARG);

extern DWORDREG DRM_API mod_exp_immediate(MP_INPUT, digit_tc, MP_OUTPUT,
                                  mp_modulus_tc*, PBIGCTX_ARG);

#if 0
#define mod_jacobi_immediate(r, modulo, pjcbi, ctx) \
      mp_jacobi_immediate(r, (modulo)->modulus, (modulo)->length, pjcbi, ctx)

extern BOOL DRM_API mod_jacobi_immediate(sdigit_tc, mp_modulus_tc*,
                                        int*, PBIGCTX_ARG);
#endif

extern BOOL DRM_API mod_Lucas(MP_INPUT, MP_INPUT, DWORDREGC, MP_OUTPUT,
                      mp_modulus_tc*, PBIGCTX_ARG);

extern BOOL DRM_API mod_LucasUV(MP_INPUT, MP_INPUT, MP_INPUT, DWORDREGC,
                        MP_OUTPUT, MP_OUTPUT, mp_modulus_tc*, PBIGCTX_ARG);

extern BOOL DRM_API mod_mul(MP_INPUT, MP_INPUT, MP_OUTPUT,
                    mp_modulus_tc*, MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API mod_mul_immediate(MP_INPUT, digit_tc,
                              MP_OUTPUT, mp_modulus_tc*, PBIGCTX_ARG);

#define mod_negate(a, b, pmodulo, ctx) neg_mod(a, b, (pmodulo)->modulus,\
                                             (pmodulo)->length, ctx)

extern BOOL DRM_API mod_shift(MP_INPUT, intc, MP_OUTPUT,
                             mp_modulus_tc*, PBIGCTX_ARG);

extern BOOL DRM_API mod_sqrt(MP_INPUT, MP_OUTPUT, mp_modulus_tc*,
                            BOOL*, PBIGCTX_ARG);

#define mod_sub(a, b, c, pmodulo, ctx) sub_mod(a, b, c, (pmodulo)->modulus, \
                                                        (pmodulo)->length, ctx)

extern void* DRM_API mp_alloc_temp(DWORDREGC, charc*, PBIGCTX_ARG);
#define Allocate_Temporaries(typename, ptr, ctx) \
        Allocate_Temporaries_Multiple(1, typename, ptr, ctx)

#define Allocate_Temporaries_Multiple(nelmt, typename, ptr, ctx) \
               ptr = (typename*)mp_alloc_temp((nelmt)*SIZEOF(typename), \
                          _bignum_DEBUG_ARGUMENT(_bignum_SOURCE_INFO()), ctx )

extern char* DRM_API mp_decimal(MP_INPUT, DWORDREGC, char*, PBIGCTX_ARG);

extern long DRM_API mp_decimal_parse(charc*, MP_OUTPUT, DWORDREGC,
                                    charc**, PBIGCTX_ARG);

extern char* DRM_API mp_dword_decimal(DWORDC*, DWORDREGC,
                                     char *buf, PBIGCTX_ARG);

extern int DRM_API mp_format(MP_INPUT, DWORDREGC,
                     digit_tc, charc*, char*, DWORDREGC, PBIGCTX_ARG);

extern void DRM_API mp_free_temp(void*, charc*, PBIGCTX_ARG);
#define Free_Temporaries(ptr, ctx)    mp_free_temp(/* (void*) */ (ptr), \
                  _bignum_DEBUG_ARGUMENT(_bignum_SOURCE_INFO()), ctx )


extern BOOL DRM_API mp_gcd(MP_INPUT, DWORDREGC, MP_INPUT, DWORDREGC,
                          MP_OUTPUT, DWORDREG*, PBIGCTX_ARG);

extern BOOL DRM_API mp_gcdex
       (MP_INPUT, DWORDREGC,
        MP_INPUT, DWORDREGC,
        MP_OUTPUT, MP_OUTPUT,
        MP_OUTPUT, MP_OUTPUT,
        DWORDREG*, MP_MODIFIED, PBIGCTX_ARG);

EXTERN_INLINE DWORDREG DRM_API mp_gcdex_ntemps
        (DWORDREGC, DWORDREGC, PBIGCTX_ARG);
           // Temporary count required by last argument to mp_gcdex

extern BOOL DRM_API mp_initialization(PBIGCTX_ARG);

EXTERN_INLINE DWORDREG DRM_API mp_invert_ntemps(DWORDREGC, PBIGCTX_ARG);
           // Temporary count required by last argument to mp_invert

extern BOOL DRM_API mp_invert(MP_INPUT, MP_INPUT, DWORDREGC,
                             MP_OUTPUT, const char*,
                             MP_MODIFIED, PBIGCTX_ARG);

extern BOOL DRM_API mp_jacobi_immediate   (sdigit_tc, MP_INPUT,
                                          DWORDREGC, int*, PBIGCTX_ARG);

extern BOOL DRM_API mp_jacobi_wrt_immediate(MP_INPUT, DWORDREGC,
                                           digit_tc, int*, PBIGCTX_ARG);

/* The assembly versions of these calls were hardcoded to use stdcall calling
   convention. For the C implementation we will use DRM_API as it is standard
   throughout the porting kits. */
#if USEASM_IX86
extern BOOL Stdcall86 mp_mul22s(digit_tc[4], MP_MODIFIED, MP_MODIFIED,
                             DWORDREGC, sdigit_t[2], PBIGCTX_ARG);

extern BOOL Stdcall86 mp_mul22u(digit_tc[4], MP_MODIFIED, MP_MODIFIED,
                             DWORDREGC,  digit_t[2], PBIGCTX_ARG);
#else
extern BOOL DRM_API mp_mul22s(digit_tc[4], MP_MODIFIED, MP_MODIFIED,
                             DWORDREGC, sdigit_t[2], PBIGCTX_ARG);

extern BOOL DRM_API mp_mul22u(digit_tc[4], MP_MODIFIED, MP_MODIFIED,
                             DWORDREGC,  digit_t[2], PBIGCTX_ARG);
#endif

extern DWORDREG mp_remove2(MP_MODIFIED, DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API mp_shift(MP_INPUT, intc, MP_OUTPUT,
                            DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API mp_shift_lost(MP_INPUT, intc, MP_OUTPUT,
                                 DWORDREGC, digit_t*, PBIGCTX_ARG);

extern BOOL DRM_API mp_sqrt(MP_INPUT, MP_OUTPUT, DWORDREGC,
                           BOOL*, PBIGCTX_ARG);

extern digit_t DRM_API multiply_immediate(MP_INPUT, digit_tc,
                                         MP_OUTPUT, DWORDREGC, PBIGCTX_ARG);


/* The assembly versions of these calls were hardcoded to use stdcall calling
   convention. For the C implementation we will use DRM_API as it is standard
   throughout the porting kits. */
#if USEASM_IX86
extern void Stdcall86 multiply_low(MP_INPUT, MP_INPUT, MP_OUTPUT,
                                DWORDREGC, PBIGCTX_ARG);
#else
extern void DRM_API multiply_low(MP_INPUT, MP_INPUT, MP_OUTPUT,
                                DWORDREGC, PBIGCTX_ARG);
#endif

// TBD -- should interface change?

extern BOOL DRM_API multiply_signed(MP_INPUT, DWORDC, MP_INPUT, DWORDC,
                                   MP_OUTPUT, DWORD*, PBIGCTX_ARG);

extern BOOL DRM_API multiply_signed_immediate(MP_INPUT, DWORDC,
                                       sdigit_tc, MP_OUTPUT,
                                       DWORD*, PBIGCTX_ARG);

extern BOOL DRM_API neg_mod(MP_INPUT, MP_OUTPUT, MP_INPUT,
                           DWORDREGC, PBIGCTX_ARG);

#if TARGET_OS == OS_WCE || TARGET_OS == OS_ATTILA
    #define PRIME_SIEVE_LENGTH 300
#else
    #define PRIME_SIEVE_LENGTH 3000
#endif
          // Must be multiple of 3
#if PRIME_SIEVE_LENGTH % 3 != 0
    #error "PRIME_SIEVE_LENGTH must be a multiple of 3"
#endif

typedef struct {
    digit_t sievbeg;
    digit_t max_sieved_squared;
    digit_t sieve[PRIME_SIEVE_LENGTH];
} next_prime_info_t;

#define next_prime_info_NULL ((next_prime_info_t*)0)

extern BOOL DRM_API next_prime(digit_tc, next_prime_info_t*,
                              digit_t*, PBIGCTX_ARG);
extern BOOL DRM_API next_prime_initialization(next_prime_info_t*, PBIGCTX_ARG);

extern BOOL DRM_API probable_prime(MP_INPUT, DWORDREGC, MP_INPUT,
                                  DWORDREGC, DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API remove_small_primes(MP_INPUT, DWORDREGC, digit_tc,
                                digit_t[], DWORDREG[], DWORDREG*,
                                MP_OUTPUT, DWORDREG*, PBIGCTX_ARG);

extern BOOL DRM_API set_immediate(MP_OUTPUT, digit_tc, DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API set_immediate_signed(MP_OUTPUT, sdigit_tc,
                                        DWORD*, PBIGCTX_ARG);

extern BOOL DRM_API signed_ashift(MP_INPUT, intc, MP_OUTPUT,
                                 DWORDC, DWORD*, PBIGCTX_ARG);

extern digit_t DRM_API signed_low(MP_INPUT, DWORDC);

extern DWORDREG DRM_API signed_mp_significant_bit_count(MP_INPUT, DWORDC,
                                                       PBIGCTX_ARG);

#define validate_modular_data(a, mod, lng, name, ctx) validate_modular_data1 \
                  ((a), (mod), (lng), _bignum_DEBUG_ARGUMENT((name)), ctx )
extern BOOL DRM_API validate_modular_data1(MP_INPUT, MP_INPUT,
                                         DWORDREGC, const char*, PBIGCTX_ARG);
#if !INLINING_SUPPORTED

extern BOOL DRM_API multiply(digit_tc  a[],DWORDREGC lnga,digit_tc  b[],DWORDREGC lngb,digit_t   c[],PBIGCTX_ARG);

#else

/****************************************************************************/
FORCE_INLINE BOOL DRM_API multiply
        (digit_tc  a[],
         DWORDREGC lnga,
         digit_tc  b[],
         DWORDREGC lngb,
         digit_t   c[],
         PBIGCTX_ARG)
/*
//        Multiply a (length lnga) times b (length lngb),
//        getting a product c (length lnga + lngb).

//        The output should not overlap the inputs.
*/
{
    BOOL OK = TRUE;
    digit_tc *p1, *p2;
    DWORDREG i, lng1, lng2;

    if (lnga > lngb) {           /* Put longer operand in p1 */
        p1 = a; p2 = b; lng1 = lnga; lng2 = lngb;
    } else {
        p2 = a; p1 = b; lng2 = lnga; lng1 = lngb;
    }

    if (!OK) {
    } else if (a == c || b == c) {
        OK = FALSE;        
    } else if (lng2 == 0) {       /* If an operand has length zero */
        mp_clear(c, lng1, PBIGCTX_PASS);
    } else {
        c[lng1] = multiply_immediate(p1, p2[0], c, lng1, PBIGCTX_PASS);
        for (i = 1; i != lng2; i++) {
            c[i + lng1] = accumulate(p1, p2[i], &c[i], lng1, PBIGCTX_PASS);
        }
    }
    return OK;
} /* multiply */

#endif

#if INLINING_SUPPORTED && USEASM_IX86
    #define significant_bit_count significant_bit_count_ix86
    #define UNIFORM_SIGNIFICANT_BIT_COUNT 1
    #pragma warning(disable : 4035)      /* No return value */
    static inline DWORDREG significant_bit_count(digit_tc pattern)
    {
    _asm {
            mov  eax,pattern        ; Nonzero pattern
            bsr  eax,eax            ; eax = index of leftmost nonzero bit
            inc  eax                ; Add one to get significant bit count
         }
    }
    #pragma warning(default : 4035)
#elif INLINING_SUPPORTED && USEASM_ALPHA
    #define significant_bit_count significant_bit_count_alpha
    #define UNIFORM_SIGNIFICANT_BIT_COUNT 1
    extern const BYTE half_byte_significant_bit_count[128];  /* See bignum.c */
            /*
                The Alpha code uses the CMPBGE instruction to
                identify which bytes are nonzero.  The most significant
                bit must occur within the leftmost nonzero byte.
                We use the CMPBGE output to identify which byte that is.
                After we extract that byte,
                we identify its most significant bit.

                (Starting with EV6 (21264), one can use leading zero count.)
            */
    static inline DWORDREG significant_bit_count(digit_tc pattern)
    {
        DWORDREGC zero_byte_pattern = __asm("cmpbge  zero, %0, v0", pattern);

        DWORDREGC byte_offset_plus_1
                = 8*half_byte_significant_bit_count[127
                                      - (zero_byte_pattern >> 1)] + 1;

        return byte_offset_plus_1
               + half_byte_significant_bit_count[pattern >> byte_offset_plus_1];
    }
#else
    #define UNIFORM_SIGNIFICANT_BIT_COUNT 0
           /* Algorithm faster for larger inputs.  See mpmisc.c */
#endif

extern BOOL DRM_API sub_diff(MP_INPUT, DWORDREGC, MP_INPUT, DWORDREGC,
                            MP_OUTPUT, digit_t*, PBIGCTX_ARG);

extern digit_t DRM_API sub_immediate(MP_INPUT, digit_tc, MP_OUTPUT, DWORDREGC);

extern BOOL DRM_API sub_mod(MP_INPUT, MP_INPUT, MP_OUTPUT, MP_INPUT,
                           DWORDREGC, PBIGCTX_ARG);

extern digit_t DRM_API sub_same(MP_INPUT, MP_INPUT, MP_OUTPUT, DWORDREGC);

#define sub_signed(a, lnga, b, lngb, c, plngc, ctx) \
        add_signed(a, lnga, b, 0-(lngb), c, plngc, ctx)

extern BOOL DRM_API test_primality(MP_INPUT, DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API test_primality_check_low(MP_INPUT, DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API to_modular(MP_INPUT, DWORDREGC, MP_OUTPUT,
                              mp_modulus_tc*, PBIGCTX_ARG);

extern BOOL DRM_API two_adic_inverse(digit_tc, digit_t*, PBIGCTX_ARG);

extern void DRM_API uncreate_modulus(mp_modulus_t*, PBIGCTX_ARG);

#if PRINT_ERROR_MESSAGES
    extern BOOL DRM_API mp_decimal_output(FILE*, MP_INPUT, DWORDREGC,
                                         charc*, PBIGCTX_ARG);

    extern void DRM_API mp_display(FILE*, charc*, MP_INPUT,
                                  DWORDREGC, PBIGCTX_ARG);
#endif /* PRINT_ERROR_MESSAGES */


#if !INLINING_SUPPORTED
extern int DRM_API compare_immediate(MP_INPUT, digit_tc, DWORDREGC);
extern digit_t DRM_API sub_immediate(MP_INPUT, digit_tc, MP_OUTPUT, DWORDREGC);
#endif


#if INLINING_SUPPORTED
/***************************************************************************/
INLINE int DRM_API compare_immediate
        (digit_tc  a[],
         digit_tc  ivalue,
         DWORDREGC    lng)
/*
        Compare a multiple-precision number to a scalar.
*/
{
    return compare_diff(a, lng, &ivalue, 1);
}
/****************************************************************************/
#if USEASM_MIPS
   /* See mips.s */
#else
#define significant_digit_count significant_digit_count_inlined
INLINE DWORDREG DRM_API significant_digit_count
        (digit_tc  a[],
         DWORDREGC lng)
/*
        Return the number of significant digits in a.
        Function value is zero precisely when a == 0.
*/
#if USEASM_IX86
    #pragma warning(disable : 4035)      /* No return value */
{
                /*
                   We could use REPE SCASD,
                   but the REPE overhead is
                   four cycles/compare on early Pentiums.
                   We would also need sld and cld.
                   It is shorter to use RISC instructions.
                   We anticipate that the leading term a[lng-1]
                   will usually be nonzero.
                */

    _asm {
        mov  eax,lng
        mov  edx,a
     label1:
        test eax,eax
        jz   label2             ; If nothing left in number, return 0

        mov  ecx,[edx+4*eax-4]
        dec  eax

        test ecx,ecx            ; Test leading digit
        jz   label1

        inc  eax                ; Nonzero element found; return old eax
     label2:
    }
}
    #pragma warning(default : 4035)
#else
{
    DWORDREG i = lng;

    while (i != 0 && a[i-1] == 0) i--;
    return i;
}  /* significant_digit_count */
#endif
#endif
/****************************************************************************/
#if USEASM_IX86
#define trailing_zero_count trailing_zero_count_ix86
INLINE DWORDREG trailing_zero_count(digit_tc d)
    #pragma warning(disable : 4035)      /* No return value */
{
    _asm {
            mov  eax,d
            bsf  eax,eax            ; eax = index of rightmost nonzero bit
         }
}
    #pragma warning(default : 4035)
#elif UNIFORM_SIGNIFICANT_BIT_COUNT
#define trailing_zero_count trailing_zero_count_inlined
static inline DWORDREG DRM_API trailing_zero_count(digit_tc d)
/*
        Given a nonzero integer d, this routine computes
        the largest integer n such that 2^n divides d.

        If d = 2^n * (2k + 1), then

                        d =     k *2^(n+1) + 2^n
                       -d = (-1-k)*2^(n+1) + 2^n

        The integers k and -1 - k are one's complements of
        each other, so d & (-d) = 2^n.  Once we determine
        2^n from d, we can get n via significant_bit_count.

        If architecture has population count, one can use
        POPULATION((d-1) & ~d)).
*/
{
    return significant_bit_count(d & (-d)) - 1;
}  /* trailing_zero_count */
#endif

#endif  // INLINING_SUPPORTED


#ifdef __cplusplus
}
#endif

#endif // BIGNUM_H

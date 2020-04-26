/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef DBLINT_H
#define DBLINT_H 1


/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

/*
        File: dblint.h.  Supplement to bignum.h.  Version 10 December, 2002.

        This file has declarations related to double-precision integers,
        such as typedefs, constants, and primitive operations.

        Before #including this, one should #define

                digit_t -- typedef for unsigned single-precision integers.
                sdigit_t -- typedef for signed single-precision integers
                RADIX_BITS -- Number of bits per digit_t.

        and identify which compiler one is using.

        Constants defined herein include

                DBLINT_BUILTIN -- 1 if compiler directly
                                  supports double integers, 0 if not.

                DBLINT_HIGH_INDEX (optional)  -- When DBLINT_BUILTIN == 1,
                                  this is 0 if compiler stores
                                  the most significant half of a
                                  dblint_t datum first, and 1
                                  if compiler stores the least
                                  significant half first.  See
                                  HIGH_DIGIT and MAKE_DBLINT below.

                                  If this is not defined, then HIGH_DIGIT
                                  and MAKE_DBLINT are defined using
                                  shifts by RADIX_BITS.  If the compiler
                                  optimizes such shifts, then
                                  leave DBLINT_HIGH_INDEX undefined.


        The dblint_t type is unsigned and holds twice as many bits
        as a digit_t datum.  If (DBLINT_BUILTIN = 1), then use the type
        already in the language.  Otherwise (DBLINT_BUILTIN = 0)
        construct one of our own, using a struct with two digit_t fields.

        Let u, u1, u2 have type digit_t and
        d, d1, d2 have type dblint_t.
        The following primitives are defined,
        whether we use the built-in type or our own type:

                DBLINT(u) -- Convert u from type digit_t to type dblint_t.
                SDBLINT(s) -- Convert signed value s from sdigit_t to dblint_t.
                DBLINT_ADD(d1, d2) -- Sum d1 + d2.
                DBLINT_EQ(d1, d2)  -- Test whether d1 == d2.
                DBLINT_GE(d1, d2)  -- Test whether d1 >= d2.
                DBLINT_GT(d1, d2)  -- Test whether d1 > d2.
                DBLINT_LE(d1, d2)  -- Test whether d1 <= d2.
                DBLINT_LT(d1, d2)  -- Test whether d1 < d2.
                DBLINT_NE(d1, d2)  -- Test whether d1 != d2.
                DBLINT_SHIFT_LEFT(d, sh) -- Shift d left by sh bits,
                                       where 0 <= sh <= 2*RADIX_BITS - 1
                DBLINT_SUB(d1, d2) -- Difference d1 - d2
                DBLINT_XOR(d1, d2) -- Exclusive OR d1 ^ d2.
                DBLINT_SUM(u1, u2) -- Sum u1 + u2.
                DPRODUU(u1, u2) -- Product of u1 and u2, as a dblint_t.
                HPRODUU(u1, u2) -- Most significant half of product
                                   of u1 and u2, as a digit_t.
                HIGH_DIGIT(d) -- Most significant half of d.
                SHIGH_DIGIT(d) -- Most significant half of d, as signed value.
                LOW_DIGIT(d) -- Least significant half of d.
                MAKE_DBLINT(u1, u2) -- Construct a dblint_t
                        whose most significant half is u1 and
                        whose least significant half is u2.
*/

#if COMPILER == COMPILER_ARM_ATTILA
     /*
      __value_in_regs tells the ARM cc compiler to pass
      small structs (up to four words) in registers, for function returns.
     */

    #define DBLINT_FUNCTION __value_in_regs dblint_t
#else
    #define DBLINT_FUNCTION dblint_t
#endif

#if COMPILER == COMPILER_GCC

    #define DBLINT_BUILTIN 1
    typedef unsigned long long dblint_t;
    #define DBLINT_HIGH_INDEX 1
                /* FIX ME!!!!!!!!!!! GCC on MIPS little endian CPU stores high half of dblint_t second */
#endif

#if COMPILER == COMPILER_VC && RADIX_BITS == 32
    #define DBLINT_BUILTIN 1
    typedef unsigned __int64 dblint_t;
#if TARGET == TARGET_ALPHA
/* If the Alpha is using RADIX_BITS == 32,
   then use the shift instruction
   for HIGH_DIGIT and MAKE_DBLINT */
#else
    #define DBLINT_HIGH_INDEX 1
                /* Visual C++ on ix86 stores low half of dblint_t first */
// Next line inserted March 2000 -- hope the compilers inline shifts.
// Quickly removed for SHx.
//    #undef DBLINT_HIGH_INDEX
#endif
#endif


#ifndef DBLINT_BUILTIN
                        /* No language support -- simulate using structs */
    #define DBLINT_BUILTIN 0
    #include <drmint64.h>

    typedef DRM_UINT64 dblint_t;

    #define HIGH_DIGIT(d) DRM_UI64High32((d))
    #define LOW_DIGIT(d)  DRM_UI64Low32((d))
#endif

typedef const dblint_t dblint_tc;


#if DBLINT_BUILTIN
/*
        If language has support for double-length integers, use it.
        Good compilers will inline these simple operations.
*/

#define DBLINT(u) ((dblint_t)(u))
#define SDBLINT(s) ((dblint_t)(sdigit_t)(s))

#define DBLINT_ADD(d1, d2) ((d1) + (d2))
#define DBLINT_EQ( d1, d2) ((d1) == (d2))
#define DBLINT_GE( d1, d2) ((d1) >= (d2))
#define DBLINT_GT( d1, d2) ((d1) > (d2))
#define DBLINT_LE( d1, d2) ((d1) <= (d2))
#define DBLINT_LT( d1, d2) ((d1) < (d2))
#define DBLINT_NE( d1, d2) ((d1) != (d2))
#define DBLINT_SHIFT_LEFT(d, sh) ((d) << (sh))
#define DBLINT_SUB(d1, d2) ((d1) - (d2))
#define DBLINT_XOR(d1, d2) ((d1) ^ (d2))


#if COMPILER == COMPILER_GCC
#define DPRODUU(u1, u2) (DBLINT(u1) * DBLINT(u2))
#endif

#if COMPILER == COMPILER_VC
/*
        A problem in Visual C/C++ 4.0 (x86 version, 1995)
        prevents proper inlining of the DPRODUU function
        if we code it in a straightforward way.  Specifically,
        if we have two nearby references DPRODUU(x, y)
        and DPRODUU(x, z), where one argument (here x) is
        repeated, then the compiler calls library function
        __allmul rather than emit a MUL instruction.

        x86 version of VC 4.1 adds an __emulu function.
        VC 7.0 (2002) does not recognize it in C+ programs.
*/
#if INLINING_SUPPORTED
static __forceinline dblint_t DRM_API DPRODUU(digit_tc u1, digit_tc u2)
{
#if TARGET == TARGET_IX86 && !defined(__cplusplus)
//        #pragma intrinsic(__emulu)
        return __emulu(u1, u2);
#elif TARGET == TARGET_MIPS && defined(NDEBUG)    // MIPS Release
       // #pragma intrinsic(__emulu)
        return __emulu(u1, u2);
#elif TARGET == TARGET_SHX && USEASM_SHX
        dblint_t result;
    #if 0
           // Following fails sporadically (March, 2000, Jornada SJ3)
        __asm(  "    DMULU.L R4, R5      ; 32*32 => MACH MACL\n"
                "    STS.L   MACH,@-R6   ; Store high product\n"
                "    STS.L   MACL,@-R6   ; Store low product\n"
              , u1, u2, &result + 1);    // R4==u1, R5==u2, R6==&result + 1
    #else
           // Following seems OK
        __asm(  "    DMULU.L R4, R5      ; 32*32 => MACH MACL\n"
                "    STS     MACL,R4     ;\n"
                "    STS     MACH,R5     ;\n"
                "    MOV.L   R4,@R6      ; Store low product\n"
                "    MOV.L   R5,@(4,R6)  ; Store high product\n"
              , u1, u2, &result);        // R4==u1, R5==u2, R6==&result
    #endif

        return result;
#else
        return DBLINT(u1) * DBLINT(u2);
#endif
}
#else // INLINING
#define DPRODUU(u1, u2) (DBLINT(u1) * DBLINT(u2))
#endif
#endif // COMPILER_VC

#define LOW_DIGIT(d)   ((digit_t)((d) & RADIXM1))

#ifdef DBLINT_HIGH_INDEX
    #if DBLINT_HIGH_INDEX < 0 || DBLINT_HIGH_INDEX > 1
        #error "Illegal value of DBLINT_HIGH_INDEX"
    #endif

    #if !INLINING_SUPPORTED
        extern digit_t DRM_API HIGH_DIGIT(dblint_tc);
        extern dblint_t DRM_API MAKE_DBLINT(digit_tc, digit_tc);
    #endif

    #if INLINING_SUPPORTED
        INLINE digit_t DRM_API HIGH_DIGIT(dblint_tc d)
        {
            dblint_tc dcopy = d;
            return ((digit_tc*)&dcopy)[DBLINT_HIGH_INDEX];
        }

        INLINE dblint_t DRM_API MAKE_DBLINT(digit_tc high, digit_tc low)
        {
            dblint_t build = low;
            ((digit_t*)&build)[DBLINT_HIGH_INDEX] = high;
            return build;
        }
    #endif
#else /* DBLINT_HIGH_INDEX */
    #define HIGH_DIGIT(d)  ((digit_t)((d) >> RADIX_BITS))

    #define MAKE_DBLINT(high, low) \
       ( (DBLINT(high) << RADIX_BITS) | DBLINT(low) )

#endif /* DBLINT_HIGH_INDEX */

#define DBLINT_SUM(d1, d2) DBLINT_ADD(DBLINT(d1), DBLINT(d2))
            /* d1 + d2 */

#define MULTIPLY_ADD1(d1, d2, d3) \
        DBLINT_ADD(DPRODUU(d1, d2), DBLINT(d3))
           /* d1*d2 + d3 */

#define MULTIPLY_ADD2(d1, d2, d3, d4) \
        DBLINT_ADD(DBLINT_ADD(DPRODUU(d1, d2), DBLINT(d3)), \
                   DBLINT(d4))
          /* d1*d2 + d3 + d4 */

#else  /* DBLINT_BUILTIN */

#define DBLINT_LE(d1, d2) DBLINT_GE(d2, d1)
#define DBLINT_LT(d1, d2) DBLINT_GT(d2, d1)

#if TARGET == TARGET_ALPHA
    #pragma intrinsic(__UMULH)
    #define HPRODUU(u1, u2) __UMULH(u1, u2)
#endif

#if TARGET == TARGET_AMD64
    #pragma intrinsic(__umulh)
    #define HPRODUU(u1, u2) __umulh(u1, u2)
#endif

// HPRODUU not presently available directly on other platforms

#if !INLINING_SUPPORTED
extern DBLINT_FUNCTION DRM_API MAKE_DBLINT(digit_tc, digit_tc);

extern DBLINT_FUNCTION DRM_API DBLINT(digit_tc);
extern DBLINT_FUNCTION DRM_API SDBLINT(sdigit_tc);
extern DBLINT_FUNCTION DRM_API DBLINT_ADD(dblint_tc, dblint_tc);
extern DBLINT_FUNCTION DRM_API DBLINT_SHIFT_LEFT(dblint_tc, DWORDREGC);
extern DBLINT_FUNCTION DRM_API DBLINT_XOR(dblint_tc, dblint_tc);
extern DBLINT_FUNCTION DRM_API DBLINT_SUB(dblint_tc, dblint_tc);
extern DBLINT_FUNCTION DRM_API DBLINT_SUM(digit_tc, digit_tc);
extern DBLINT_FUNCTION DRM_API DPRODUU(digit_tc, digit_tc);
extern DBLINT_FUNCTION DRM_API MULTIPLY_ADD1(digit_tc, digit_tc, digit_tc);
extern DBLINT_FUNCTION DRM_API MULTIPLY_ADD2(digit_tc, digit_tc,
                                            digit_tc, digit_tc);

extern BOOL DRM_API DBLINT_EQ(dblint_tc, dblint_tc);
extern BOOL DRM_API DBLINT_GE(dblint_tc, dblint_tc);
extern BOOL DRM_API DBLINT_GT(dblint_tc, dblint_tc);
extern BOOL DRM_API DBLINT_NE(dblint_tc, dblint_tc);

#endif // INLINING_SUPPORTED

#if INLINING_SUPPORTED

INLINE DBLINT_FUNCTION DRM_API DPRODUU(digit_tc u1, digit_tc u2)
/*
        Multiply two single-precision operands,
        return double precision product.
        This will normally be replaced by an assembly language routine.
        unless the top half of the product (HPRODUU) is available in C.
*/
{
    return DRM_UI64Mul( DRM_UI64( u1 ), DRM_UI64( u2 ) );
}   // end DPRODUU
 




INLINE DBLINT_FUNCTION DRM_API DBLINT(digit_tc d)
{
    return DRM_UI64( d );    
}

INLINE DBLINT_FUNCTION DRM_API SDBLINT(sdigit_tc s)
                                // Convert signed value to DBLINT
{
    DRM_INT64 i64 = DRM_I64( s );
    DRM_UINT64 ui64;

    ui64 = *(DRM_UINT64*)&i64;
    return ui64;
}  // SDBLINT


INLINE DBLINT_FUNCTION DRM_API DBLINT_ADD(dblint_tc d1, dblint_tc d2)
{
    return DRM_UI64Add( d1, d2 );
}

INLINE DBLINT_FUNCTION DRM_API DBLINT_SHIFT_LEFT(dblint_tc d, DWORDREGC sh)
{
//  Left shift.  An excellent compiler will sometimes infer
//  whether sh >= RADIX_BITS from other code near the invocation.
    return DRM_UI64ShL( d, sh );
} // DBLINT_SHIFT_LEFT

#if 0
INLINE DBLINT_FUNCTION DRM_API DBLINT_XOR(dblint_tc d1, dblint_tc d2)
{
    dblint_t answer;
    answer.low = d1.low ^ d2.low;
    answer.high = d1.high ^ d2.high;
    return answer;
}  // DBLINT_XOR
#endif


INLINE BOOL DRM_API DBLINT_EQ(dblint_tc d1, dblint_tc d2)
{
   return DRM_UI64Eql(d1, d2);
}  // DBLINT_RQ

INLINE BOOL DRM_API DBLINT_GE(dblint_tc d1, dblint_tc d2)
{
   return !DRM_UI64Les(d1, d2) || DRM_UI64Eql(d1, d2);
}  // DBLINT_GE

INLINE BOOL DRM_API DBLINT_GT(dblint_tc d1, dblint_tc d2)
{
   return !DRM_UI64Les(d1, d2);
}  // DBLINT_GT

INLINE BOOL DRM_API DBLINT_NE(dblint_tc d1, dblint_tc d2)
{
   return !DRM_UI64Eql(d1, d2);
}  // DVLINT_NE

INLINE DBLINT_FUNCTION DRM_API DBLINT_SUB(dblint_tc d1, dblint_tc d2)
{
    return DRM_UI64Sub(d1, d2);
}  // DBLINT_SUB


INLINE DBLINT_FUNCTION DRM_API MAKE_DBLINT(digit_tc high, digit_tc low)
{
     return DRM_UI64HL( high, low );
}  // MAKE_DBLINT

/*
    The DBLINT_SUM, MULTIPLY_ADD1. MULTIPLY_ADD2
    functions take single-length (digit_t) operands and
    return double-length (dblint_t) results.
    Overflow is impossible.
*/

INLINE DBLINT_FUNCTION DRM_API DBLINT_SUM(digit_tc d1, digit_tc d2)
{
    return DRM_UI64Add( DRM_UI64(d1), DRM_UI64(d2) );
}  // DBLINT_SUM

INLINE DBLINT_FUNCTION DRM_API MULTIPLY_ADD1(digit_tc d1, digit_tc d2,
                                                 digit_tc d3)
{
    return DRM_UI64Add( DRM_UI64Mul( DRM_UI64(d1), DRM_UI64(d2) ), DRM_UI64(d3) );
} // MULTIPLY_ADD1


INLINE DBLINT_FUNCTION DRM_API MULTIPLY_ADD2(digit_tc d1, digit_tc d2,
                                                 digit_tc d3, digit_tc d4)
{
    return DRM_UI64Add( DRM_UI64Add( DRM_UI64Mul( DRM_UI64(d1), DRM_UI64(d2) ), DRM_UI64(d3) ), DRM_UI64(d4) );
} // MULTIPLY_ADD2

#endif // INLINING_SUPPORTED

#endif // DBLINT_BUILTIN

#undef DBLINT_FUNCTION

#define SHIGH_DIGIT(d) ((sdigit_t)HIGH_DIGIT((d)))

#ifndef HPRODUU
    #define HPRODUU(u1, u2) HIGH_DIGIT(DPRODUU((u1), (u2)))
#endif

#ifdef __cplusplus
}
#endif

#endif // DBLINT_H

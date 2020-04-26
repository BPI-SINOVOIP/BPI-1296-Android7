/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef BIGDEFS_H              /* If not previously #included */
#define BIGDEFS_H 1

//       File bigdefs.h     Version 10 February 2004

//       This file declared the fundamental types and constants used
//       within the bignum code.

//           BOOL
//           BYTE
//           digit_t and digit_tc
//           DWORD and DWORDC
//           DWORDREG and DWORDREGC
//           sdigit_t and sdigit_tc
//
//        used within the bignum code and headers.
//        BOOL, BYTE, DWORD, FALSE, TRUE are defined in windef.h
//        but repeated here.
//        We also declare the most fundamental routines for operating on these.
//
//        If you need to perform arithmetic, not just copy data, see bignum.h .


#include "bigenv.h"    // Identify compiler, operating system, target architecture 
#include <drmcommon.h>
#include <drmtypes.h>
#include <string.h>    // memcpy, memset procedures
#include <stdlib.h>    // 

/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

/*
        The letter 'c' following a type name identifies
        a const entity of that type.
*/
typedef const DRM_CHAR charc;
typedef const DRM_INT intc;

#if TARGET_OS == OS_WCE || TARGET_OS == OS_WNT
// #include <stdarg.h>
#include <windows.h>
#include <windef.h>
#include <winbase.h>     /* SYSTEM_INFO struct */
#else
typedef DRM_BOOL BOOL;       /* Same as windef.h */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef DRM_BYTE BYTE;
typedef DRM_VOID VOID;
typedef DRM_DWORD DWORD;
typedef DRM_INT INT;
typedef DRM_RESULT HRESULT;
#endif

typedef const DWORD DWORDC;


#define DWORD_BITS 32
#define DWORD_LEFT_BIT 0x80000000UL

/*
     Pre-2002 bignum interfaces declared most lengths as DWORD,
     which is 32 bits on all platforms.  DWORD arithmetic operates
     modulo 2^32, making it hard for compilers to optimize

              DWORD i, j;   // 32-bits, unsigned
              for (i = 0; i < 100; i++) array[i+j] = 0;

     on 64-bit platforms, since array[i+j] and array[i+j+1] won't be adjacent
     when i+j = 0xffffffff.  DWORDREG has the same size as a machine register
     but is allowed only to have values through 2^32 - 1.
     It (rather than DWORD) should be used for subscripts and loop indices.
*/
#if (COMPILER == COMPILER_TMS320C55X)
/* TI Compiler uses size_t */
typedef DWORD DWORDREG;
#else
typedef size_t DWORDREG;
#endif

typedef const DWORDREG DWORDREGC;


//          Multiple-precision data is normally represented
//      in radix 2^RADIX_BITS, with RADIX_BITS bits per word.
//      Here ``word'' means type digit_t.  RADIX_BITS
//      is 32 on some architectures (Intel x86, MIPS, PowerPC)
//      and 64 bits on other architectures (Alpha, IA-64).

//          Within Windows NT, the data type DWORD predominates.
//      DWORD is a 32-bit unsigned data type on all platforms
//      (Intel x86, Alpha, IA-64, MIPS, PowerPC, Windows CE).
//      DWORD data can safely be written to disk on one architecture
//      and read back on another, unlike digit_t.

//      [CAUTION -- Even DWORD is not safe when sending data to
//      big-endian architectures, such as Office products for the Macintosh.]

#if (COMPILER == COMPILER_ARM_ATTILA ) || (COMPILER == COMPILER_GCC) || (COMPILER == COMPILER_TMS320C55X)
    #define RADIX_BITS 32
    #define RADIX_BYTES 4
    typedef   signed long sdigit_t;
    typedef unsigned long digit_t;
#elif TARGET == TARGET_ALPHA || TARGET == TARGET_IA64 /*|| TARGET == TARGET_AMD64*/
    #define RADIX_BITS 64
    #define RADIX_BYTES 8
    typedef   signed __int64 sdigit_t;
    typedef unsigned __int64  digit_t;
#else 
    // Use 32-bit path for x64 too, since the 64-bit path gives incorrect results (on x64 at least).
    // We may want to eventually go back to the 64-bit path for speed once it is fixed. -thjones 11/9/04
    #define RADIX_BITS 32
    #define RADIX_BYTES 4
    typedef   signed __int32 sdigit_t;
    typedef unsigned __int32 digit_t;
#endif

typedef const  digit_t  digit_tc;
typedef const sdigit_t sdigit_tc;


#define DIGIT_ZERO ((digit_t)0)
#define DIGIT_ONE  ((digit_t)1)

#define RADIX_HALF (DIGIT_ONE << (RADIX_BITS - 1))
#define RADIXM1 (DIGIT_ZERO - DIGIT_ONE)
#define F_RADIX ((double)RADIXM1 + 1.0)

#define HALF_RADIX_BITS (RADIX_BITS/2)
#if (RADIX_BITS != 2*HALF_RADIX_BITS)
    #error -- "RADIX_BITS must be even"
#endif
#define RADIX_HALFMASK_BOTTOM (RADIXM1 >> HALF_RADIX_BITS)




//    DWORD is 32 bits on all platforms.
//    Many codes simply require at least 32 bits,
//    but a few assume it is exactly 32 bits,
//    with integer overflow ignored.  The SHAVE32 macro
//    is intended to identify where only 32 bits are to be used.
//    For example, it should be used before a right shift
//    if earlier calculations may have overflowed.

#if DWORD_BITS == 32
#define SHAVE32(x) ((DWORD)(x))
#else
#define SHAVE32(x) ((DWORD)((x) & 0xffffffffU))
#endif


//  Rotate a 32-bit DWORD value left circularly by amt bits,
//  where 0 <= amt < 32.


#if TARGET == TARGET_IX86
#pragma intrinsic(_rotl)
#define ROTATE32L(x, amt) ((DWORD)_rotl((x), (amt)))
#else
#define ROTATE32L(x, amt) (((x) << (amt)) \
                  | (SHAVE32(x) >> (31 & (0 - (DWORD)(amt)))))
#endif



#if RADIX_BITS % DWORD_BITS != 0
    #error "RADIX_BITS not a multiple of 32"
#endif


#define DWORDS_PER_DIGIT (RADIX_BITS/DWORD_BITS)

//      DWORDS_TO_DIGITS(lng_dwords) computes the number of digit_t
//      elements required to store an array with -lng_dwords- DWORDs.
//      DIGITS_TO_DWORDS converts in the opposite direction.


#define DWORDS_TO_DIGITS(lng_dwords) \
                ( ((lng_dwords) + DWORDS_PER_DIGIT - 1)/DWORDS_PER_DIGIT)

#define DIGITS_TO_DWORDS(lng_digits) ((lng_digits) * DWORDS_PER_DIGIT)

#define BITS_TO_DIGITS(nb) (((nb) + RADIX_BITS - 1)/RADIX_BITS)


#define digit_getbit(iword, ibit) (((iword) >> (ibit)) & 1)
// TBD #define dword_getbit(iword, ibit) digit_getbit(iword, ibit)
                        /* Extract bit from a word.
                        // 0 <= ibit <= RADIX_BITS - 1.
                        // Rightmost (i.e., least significant) bit is bit 0.
                        */

/*
        Test whether a (possibly negative) number is odd or even.
*/
#define IS_EVEN(n) (~(unsigned int)(n) & 1)
#define IS_ODD(n) ((unsigned int)(n) & 1)

/*
        Maximum and minimum of two arguments
        (no side effects in arguments)
*/

#if 0
    #define MAX _max
    #define MIN _min
#else
    #define MAX(x, y) ((x) > (y) ? (x) : (y))
    #define MIN(x, y) ((x) > (y) ? (y) : (x))
#endif



/*
      The library can be compiled in to print messages (in English)
      when certain errors are detected.  That flag is PRINT_ERROR_MESSAGES.
      Messages go to file mp_errfil unless mp_errfil == NULL.
      The application program can change the file.
*/

#ifndef PRINT_ERROR_MESSAGES
    #ifdef ENABLE_ERROR_MESSAGES
        #define PRINT_ERROR_MESSAGES 1
    #else
        #define PRINT_ERROR_MESSAGES 0
    #endif
#endif

#if PRINT_ERROR_MESSAGES
    #include <stdio.h>
    extern FILE* mp_errfil;   // Declared in bigerrno.h or its substitute.
                              // May be a NULL pointer.
                              // Application may initialize
                              //     mp_errfil = stdout or stderr.
#endif /* PRINT_ERROR_MESSAGES */



//  The following are used in function prototypes

#define MP_INPUT      digit_tc*
#define MP_OUTPUT     digit_t*
#define MP_MODIFIED   digit_t*

/*
      Occasionally a struct name is used before the struct itself is
      declared.  The Future_Struct macro avoids a warning message
      with Vicusl C .NET (2002).
*/
#define Future_Struct(strname) struct strname

Future_Struct(bigctx_t);

/*
        Many procedures have a final argument identifying the
        caller (or caller's caller, etc.).  The application
        can use this for thread-specific data, such as
        having a separate random number generator seed
        for each thread or forwarding error information
        to the caller.

        The last formal argument to a procedure
        should be PBIGCTX_ARG, which will declare a
        pointer pbigctx to a type not decided as
        of February, 2004.  Use PBIGCTX_PASS
        to pass pbigctx to a lower procedure.
*/

#define PBIGCTX_PASS  pbigctx
#define PBIGCTX_ARG   struct bigctx_t *pbigctx
#define PBIGCTX_UNUSED (void)pbigctx
#define PBIGCTX_NULL  0



//  The following functions may be referenced without calling
//  mp_initialization.

//  Some of these have assembly language or machine-specific
//  versions in bignum.h or bignum.c.

#if 0
extern BOOL DRM_API big_endian_bytes_to_digits
        (const BYTE  *barray,   // IN
         digit_t     *darray,   // OUT
         DWORDREGC    bitlen,   // IN
         PBIGCTX_ARG);

extern BOOL DRM_API little_endian_bytes_to_digits
        (const BYTE  *barray,   // IN
         digit_t     *darray,   // OUT
         DWORDREGC    bitlen,   // IN
         PBIGCTX_ARG);

extern BOOL DRM_API digits_to_big_endian_bytes
        (digit_tc    *darray,   // IN
         BYTE        *barray,   // OUT
         DWORDREGC    bitlen,   // IN
         PBIGCTX_ARG);

extern BOOL DRM_API digits_to_little_endian_bytes
        (digit_tc    *darray,   // IN
         BYTE        *barray,   // OUT
         DWORDREGC    bitlen,   // IN
         PBIGCTX_ARG);

extern BOOL DRM_API little_endian_bytes_to_digits
        (const BYTE  *barray,   // IN
         digit_t     *darray,   // OUT
         DWORDREGC    bitlen,   // IN
         PBIGCTX_ARG);

#endif
extern DWORDREG DRM_API mp_trailing_zero_count(MP_INPUT, DWORDREGC, PBIGCTX_ARG);

#define mp_clear(dest, lng, ctx) (BOOL)(ctx, memset((void *)(dest), 0,\
                                                (size_t)(lng)*sizeof(digit_t)), TRUE)
#define mp_copy(src, dest, lng, ctx) \
            (BOOL)(ctx, memcpy((void *)(dest), (const void *)(src), \
                               (size_t)(lng)*sizeof(digit_t)), TRUE)

#define all_zero(a, lng) (significant_digit_count(a, lng) == 0)

extern DWORDREG DRM_API trailing_zero_count(digit_tc);
                 /* Has special code on some platforms */

#if !INLINING_SUPPORTED

extern DWORDREG DRM_API significant_bit_count(digit_tc);
                 /* Has special code on some platforms */

extern DWORDREG DRM_API mp_significant_bit_count
                            (MP_INPUT, DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API digits_to_dwords(MP_INPUT, DWORD*,
                                    DWORDREGC, PBIGCTX_ARG);

extern BOOL DRM_API dwords_to_digits(DWORDC*, MP_OUTPUT,
                                    DWORDREGC, PBIGCTX_ARG);

extern void DRM_API mp_extend(MP_INPUT, DWORDREGC, MP_OUTPUT,
                             DWORDREGC, PBIGCTX_ARG);

extern digit_t DRM_API mp_getbit(MP_INPUT, DWORDREGC, PBIGCTX_ARG);

extern void DRM_API mp_setbit(MP_MODIFIED, DWORDREGC, digit_tc, PBIGCTX_ARG);

extern DWORDREG DRM_API significant_digit_count(MP_INPUT, DWORDREGC);
#endif

extern void * BignumSecureZeroMemory(
    IN void* ptr,
    IN DWORD cnt
    );

#if INLINING_SUPPORTED

INLINE void DRM_API mp_extend
        (digit_tc  a[],
         DWORDREGC lnga,
         digit_t   b[],
         DWORDREGC lngb,
         PBIGCTX_ARG)
/*
        Copy a to b, while changing its length from
        lnga to lngb (zero fill).  Require lngb >= lnga.
*/
{
    PBIGCTX_UNUSED;
    mp_copy(a, b, lnga, PBIGCTX_PASS);
    mp_clear(b + lnga, lngb - lnga, PBIGCTX_PASS);
}  // mp_extend
/****************************************************************************/
INLINE digit_t DRM_API mp_getbit
        (digit_tc a[],
         DWORDREGC ibit,
         PBIGCTX_ARG)
                /* Extract bit of multiple precision number */
{
    PBIGCTX_UNUSED;
    return digit_getbit(a[ibit/RADIX_BITS],  ibit % RADIX_BITS);
}
/****************************************************************************/
INLINE void DRM_API mp_setbit
        (digit_t   a[],
         DWORDREGC ibit,
         digit_tc  new_value,
         PBIGCTX_ARG)
/*
        Set a bit to 0 or 1,
        when the number is viewed as a bit array.
*/

{
    DWORDREGC j       = ibit / RADIX_BITS;
    DWORDREGC ishift  = ibit % RADIX_BITS;

    digit_tc mask1 = (DIGIT_ONE &  new_value) << ishift;
    digit_tc mask2 = (DIGIT_ONE & ~new_value) << ishift;

    PBIGCTX_UNUSED;
    a[j] = (a[j] & ~mask2) | mask1;
} // end mp_setbit
/****************************************************************************/
INLINE DWORDREG DRM_API significant_digit_count
        (digit_tc  a[],
         DWORDREGC    lng)
/*
        Return the number of significant digits in a.
        Function value is zero precisely when a == 0.
*/
{
    DWORDREG i = lng;

    while (i != 0 && a[i-1] == 0) i--;
    return i;
}  /* significant_digit_count */
/****************************************************************************/
INLINE BOOL DRM_API digits_to_dwords
        (digit_tc  pdigit[],
         DWORD     pdword[],
         DWORDREGC lng_dwords,
         PBIGCTX_ARG)
{
    BOOL OK = TRUE;

#if DWORDS_PER_DIGIT == 1
    PBIGCTX_UNUSED;
    mp_copy(pdigit, (digit_t*)pdword, lng_dwords, PBIGCTX_PASS);
#elif DWORDS_PER_DIGIT == 2
    DWORDREGC lng_half = lng_dwords >> 1;
    DWORD i;
    PBIGCTX_UNUSED;


    if (IS_ODD(lng_dwords)) {
        pdword[lng_dwords-1] = (DWORD)pdigit[lng_half];
    }
    for (i = 0; i != lng_half; i++) {
        digit_tc dig = pdigit[i];
        pdword[2*i    ] = (DWORD)dig;
        pdword[2*i + 1] = (DWORD)(dig >> DWORD_BITS);
    }
#else
    #error "Unexpected DWORDS_PER_DIGIT"
#endif
    return OK;
}  /* digits_to_dwords */
/****************************************************************************/
INLINE BOOL DRM_API dwords_to_digits
        (DWORDC    pdword[],
         digit_t   pdigit[],
         DWORDREGC lng_dwords,
         PBIGCTX_ARG)
{
    BOOL OK = TRUE;

#if DWORDS_PER_DIGIT == 1
    PBIGCTX_UNUSED;
    mp_copy((digit_t*)pdword, pdigit, lng_dwords, PBIGCTX_PASS);
#elif DWORDS_PER_DIGIT == 2
    DWORDREGC lng_half = lng_dwords >> 1;
    DWORDREG i;
    PBIGCTX_UNUSED;

    if (IS_ODD(lng_dwords)) {
        pdigit[lng_half] = (digit_t)pdword[lng_dwords - 1];  // Zero fill
    }
    for (i = 0; i != lng_half; i++) {
        pdigit[i] =    ((digit_t)pdword[2*i+1] << DWORD_BITS)
                     |  (digit_t)pdword[2*i];
    }
#else
    #error "Unexpected DWORDS_PER_DIGIT"
#endif
    return OK;
}  /* dwords_to_digits */

#undef significant_bit_count    // In case a macro version existed
FORCE_INLINE DWORDREG DRM_API significant_bit_count(digit_tc d)
/*
        Compute the number of significant bits in d.
        This is one more than the truncated base 2 logarithm of d.
        significant_bit_count(0)  is undefined.

        For example, significant_bit_count(d) = 8 for 128 <= d <= 255.

        On platforms which have a Count Leading Zeroes or
        similar instruction, UNIFORM_SIGNIFICANT_BIT_COUNT
        should be 1, with special code used.
        For other platforms, use this algorithm.
        The algorithm performs best when the argument is large,
        a feature used by the GCD routines.
*/
{
    digit_t dadj = d | 1;
    DWORDREG width = RADIX_BITS;
    DWORDREG width_adj;

    while (dadj < (DIGIT_ONE << (RADIX_BITS - 5))) {
        width -= 5;
        dadj <<= 5;
    }
    dadj >>= (RADIX_BITS - 4);     /* From 0 to 15 */
    width_adj = ((DWORDREG)0000000011112234 >> dadj) >> (2*dadj);
                     // Shift by 3*dadj, to index into array of octal digits
    return width - (width_adj & 7);
                    // Subtract 5 - significant_bit_count(2*dadj + 1)
} /* significant_bit_count */

FORCE_INLINE DWORDREG DRM_API mp_significant_bit_count
        (digit_tc  a[],
        DWORDREGC  lnga,
        PBIGCTX_ARG)
/*
        Return the number of significant bits in a, which
        is one more than the truncated base 2 logarithm of a.
        Return 0 if a == 0.
*/
{
    DWORDREGC lng_sig = significant_digit_count(a, lnga);

    PBIGCTX_UNUSED;
    if (lng_sig == 0) {
        return 0;
    } else {
       return    (lng_sig - 1)*RADIX_BITS
               + significant_bit_count(a[lng_sig-1]);
    }
}


/****************************************************************************/
#endif // INLINING_SUPPORTED

/*
        The application should define the
        following three malloc-like functions.
        Sample definitions appear in bigalloc.h.
*/

extern void* DRM_API bignum_alloc(const size_t, PBIGCTX_ARG);
extern void* DRM_API bignum_realloc(void*, const size_t, PBIGCTX_ARG);
extern void  DRM_API bignum_free(void*, PBIGCTX_ARG);

/*
        The test version of the library includes
        mp_print_allocation_statistics to help identify
        memory leaks and other errors.
*/
#if PRINT_ERROR_MESSAGES
    extern void DRM_API mp_print_allocation_statistics(FILE*, PBIGCTX_ARG);
#else
    extern void DRM_API mp_print_allocation_statistics(void*, PBIGCTX_ARG);
          // Declare it, but library routine does nothing.
#endif /* PRINT_ERROR_MESSAGES */

#ifdef __cplusplus
}
#endif

#endif // BIGDEFS_H

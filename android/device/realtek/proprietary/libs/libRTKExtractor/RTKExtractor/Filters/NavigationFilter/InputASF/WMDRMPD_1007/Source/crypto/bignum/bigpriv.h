/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#ifndef __BIGPRIV_H
#define __BIGPRIV_H 1
/*
      File bigpriv.h.   Version 20 September, 2002

      Declarations accessible to bignum library but invisible to application.
      Also see ecpriv.h and fieldpriv.h
*/
/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif

#include "bignum.h"

#if COMPILER == COMPILER_VC

    #pragma intrinsic(abs, labs, memcpy)

    #if TARGET != TARGET_SHX
    #pragma intrinsic(memset)
    #endif

    #pragma warning(disable: 4146 4514)  // TBD
         /* 4146 -- unary minus operator applied
            to unsigned type, result still unsigned.
            4514 -- unreferenced inline function
          */
#endif

// The next symbols relate to existence of certain hardware features.

#if TARGET == TARGET_IX86
exportable_var BOOL   MMX_available;   /* See mpglobal.c */
exportable_var BOOL  SSE2_available;
#endif

/*
     Convert between void* pointers and size_t
     (same binary representation, different types).
*/
#define vptr_to_size_t(vp) ((size_t)(vp))
#define size_t_to_vptr(vp) ((void*)((vp) + (char*)0))
/*
        vptr_roundup_alignnment(p, al) gets a pointer vp
        and rounds it up to a multiple of al, returning a void*.
        al must be a power of 2.

        If one wants to malloc a struct with 32-byte aligned items,
        then one should first malloc SIZEOF(struct) + 31 bytes.
        Use this macro to adjust the pointer received, but
        retain the original pointer for a free.
*/
#define vptr_roundup_alignment(vp, al) \
        size_t_to_vptr((vptr_to_size_t(vp) + (al) - 1) & ~(al - 1))



/*
      On IX86, Pentium 4 and higher have have an SSE2
      (Streaming SIMD Extensions 2) instruction set,
      which allows operations such as two independent 64-bit integer adds.

      All AMD64 hosts have SSE2.
      However, since digit_t has 32 bits on IX86 and 64 bits on AMD64,
      many SSE2 algorithms are different on the two architectures.
*/
#if defined(USESSE2)
        // Retain existing value
#elif TARGET == TARGET_IX86 || TARGET == TARGET_AMD64
    #define USESSE2 (TARGET_OS == OS_WNT) // Avoid under WinCE
#else
    #define USESSE2 0
#endif

#if USESSE2
#include <emmintrin.h>    // SSE2 intrinsics
/*
      SSE2_digit0 through SSE2_digit3 extract
      32-bit pieces of a 128-bit integer.
      digit_0 is the rightmost (least significant) 32 bits.

      N.B.  The shift count to _mm_srli_si128 is bytes, not bits.
*/

#define SSE2_digit0(m128i) ((digit_t)_mm_cvtsi128_si32(m128i))
#define SSE2_digit1(m128i) SSE2_digit0(_mm_srli_si128(m128i, 4))
#define SSE2_digit2(m128i) SSE2_digit0(_mm_srli_si128(m128i, 8))
#define SSE2_digit3(m128i) SSE2_digit0(_mm_srli_si128(m128i, 12))

/*
     ptr_roundup_m128i rounds a pointer up to a multiple of 16 bytes,
     casting the output as __m128i .
*/
#define ptr_roundup_m128i(p) \
        ((__m128i*)vptr_roundup_alignment((void*)(p), __alignof(__m128i)))

#endif // USESSE2

/*
     Some routines allow the caller to supply temps, but
     accept a digit_NULL argument to say "Allocate them yourself!".
     possible_digit_allocate assists in doing the allocate
     if the caller passed digit_NULL.
*/

typedef struct digit_tempinfo_t {
    digit_t *address;       // Address supplied by user.
                            // Updated to specify address
                            // to use for temporaries.
    DWORDREG nelmt;         // Number of digit_t elements needed
    BOOL     need_to_free;  // Should be set FALSE by application.
                            // Changed to TRUE if a free is required.
} digit_tempinfo_t;

typedef const digit_tempinfo_t digit_tempinfo_tc;

extern BOOL DRM_API possible_digit_allocate_named(digit_tempinfo_t*,
                                                 charc*, PBIGCTX_ARG);

#define possible_digit_allocate(tempinfo, name, ctx) \
         possible_digit_allocate_named((tempinfo), \
                                       _bignum_DEBUG_ARGUMENT(name), ctx)

/*
       digit2_aligned(array) checks that _array_ is
       aligned on a 2*SIZEOF(digit_t) boundary.

       Assembly code versions of the software sometimes load
       or store two digit_t values with one instruction.
       Specifically, MMX code on X86 can load or store two 32-bit
       digit_t values with one 64-bit MOVQ instruction.
       IA-64 code can load two 64-bit values to the floating
       point registers with a load pair instruction.

       The digit2_aligned macro checks whether its operand is
       appropriately aligned.  The required alignment is never
       worse than that returned by mp_alloc_temp.
*/

#if TARGET == TARGET_IX86 || TARGET == TARGET_IA64
   // TBD -- revise this
#define digit2_aligned(array) (((size_t)(array) & (2*SIZEOF(digit_t) - 1)) == 0)
#else
#define digit2_aligned(array) (TRUE)
#endif

/*
     The following are used while manipulating signed operands.
     Only the bottom 32 bits of the signed length are used.
*/

#define sign_of(lng) ((DWORD)(lng) & DWORD_LEFT_BIT)
#define abs_of(lng)  (SHAVE32(sign_of(lng) ? 0-(lng) : (lng)) )

extern BOOL DRM_API modmul_choices1(mp_modulus_t*, int*, PBIGCTX_ARG);

extern BOOL DRM_API modmul_choicesSSE2(mp_modulus_t*, int*, PBIGCTX_ARG);

#ifdef __cplusplus
}
#endif

#endif  //  __BIGPRIV_H

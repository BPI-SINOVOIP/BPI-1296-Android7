/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*      File mprand.h */

#ifndef _MPRAND_H
#define _MPRAND_H

#include "bignum.h"

#if (TARGET_OS != OS_ATTILA) && (TARGET_OS != OS_ANSI)
    #include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


extern BOOL DRM_API random_bytes(BYTE*, const size_t, PBIGCTX_ARG);
         // Procedure to be supplied by application.
         // bigrand0.h and bigrand1.h have possible implementations.


//   The DSA routines let the user have multiple random number generators,
//   rather than be restricted to random_bytes().
//   The RNGINFO struct identifies this alternate generator,
//   which is passed to new_ random number routines.


typedef struct {
        void   *pInfo;      // Data for user function
#if 0
        void   *pFuncRNG;   // Typically a function pointer
#else
        void   (*pFuncRNG)(BYTE*, size_t, void*, PBIGCTX_ARG);
                            // Called with byte arrsy.
                            // array length, and
                            // pointer to pInfo herein

                            // Changed by petmon April, 2002
                            // to eliminate warnings
#endif

} RNGINFO;
#define RNGINFO_NULL ((RNGINFO*)0)
typedef const RNGINFO RNGINFOC;


#if !INLINING_SUPPORTED
// extern BOOL DRM_API new_random_bytes(BYTE*, const size_t, RNGINFO*);

extern BOOL DRM_API     random_digits(digit_t*, DWORDREGC, PBIGCTX_ARG);
extern BOOL DRM_API     random_dwords(DWORD*,   DWORDREGC, PBIGCTX_ARG);
extern BOOL DRM_API new_random_digits(digit_t*, DWORDREGC,
                                     RNGINFOC*, PBIGCTX_ARG);
extern BOOL DRM_API new_random_dwords(DWORD*,   DWORDREGC,
                                     RNGINFOC*, PBIGCTX_ARG);


#endif // INLINING_SUPPORTED

// Macros


#define random_digit_interval(d1, d2, dout, ctx) \
    new_random_digit_interval(d1, d2, dout, RNGINFO_NULL, ctx)
#define random_dword_interval(d1, d2, dout, ctx) \
    new_random_dword_interval(d1, d2, dout, RNGINFO_NULL, ctx)


#define random_mod(n, arr, lng, ctx) \
    new_random_mod(n, arr, lng, RNGINFO_NULL, ctx)

#define random_mod_nonzero(n, arr, lng, ctx) \
    new_random_mod_nonzero(n, arr, lng, RNGINFO_NULL, ctx)
                        /* Generate random value modulo another value */
                        /* new_random_mod_nonzero generates a nonzero value */

#define random_mod_inverse(n, arr, arrinv, ninverse, nrecip, lng, ctx) \
    new_random_mod_inverse(n, arr, arrinv, ninverse, \
                           nrecip, lng, RNGINFO_NULL, ctx)
                  // Generate random values and their multiplicative inverses



// Following are extern on all platforms

extern BOOL DRM_API new_random_bytes(BYTE*, const size_t,
                                    RNGINFOC*, PBIGCTX_ARG);

extern BOOL DRM_API new_random_digit_interval(digit_tc, digit_tc,
                                             digit_t*, RNGINFOC*, PBIGCTX_ARG);
extern BOOL DRM_API new_random_dword_interval(DWORDC, DWORDC, DWORD*,
                                             RNGINFOC*, PBIGCTX_ARG);
          /* Generate random digit_t or DWORD in specified closed interval. */

extern BOOL DRM_API new_random_mod        (MP_INPUT, MP_OUTPUT,
                                          DWORDREGC, RNGINFOC*, PBIGCTX_ARG);
extern BOOL DRM_API new_random_mod_nonzero(MP_INPUT, MP_OUTPUT,
                                          DWORDREGC, RNGINFOC*, PBIGCTX_ARG);
                        // Generate random value modulo another value.
                        // new_random_mod_nonzero generates a nonzero value.

extern BOOL DRM_API new_random_mod_inverse(MP_INPUT, MP_OUTPUT, MP_OUTPUT,
                        DWORDREGC, reciprocal_1_tc*, DWORDREGC,
                        RNGINFOC*, PBIGCTX_ARG);
                        /* Generate random values and
                           their multiplicative inverses. */

#if INLINING_SUPPORTED

// Next four are inline, rather than macros, so compiler can issue
// error when array argument has wrong pointer type.

INLINE BOOL DRM_API random_digits(digit_t array[],
                                      DWORDREGC lng,
                                      PBIGCTX_ARG)
{    /* Generate random multiple-precision number */
     /* It may have a leading zero. */
    return random_bytes((BYTE*)array,
                        (size_t)lng*SIZEOF(digit_t),
                        PBIGCTX_PASS);
} /* random digits */


INLINE BOOL DRM_API new_random_digits(digit_t array[],
                                     DWORDREGC lng,
                                     RNGINFOC* pRNGInfo,
                                     PBIGCTX_ARG)
{    /* Generate random multiple-precision number */
     /* It may have a leading zero. */
    return new_random_bytes((BYTE*)array,
                             (size_t)lng*SIZEOF(digit_t),
                             pRNGInfo, PBIGCTX_PASS);
} /* random digits */


INLINE BOOL DRM_API random_dwords(DWORD array[], DWORDREGC lng, PBIGCTX_ARG)
{    /* Generate random DWORDs */

    return random_bytes((BYTE*)array, (size_t)lng*SIZEOF(DWORD), PBIGCTX_PASS);
} /* random dwords */


INLINE BOOL DRM_API new_random_dwords(DWORD array[], DWORDREGC lng,
                                          RNGINFOC *pRNGInfo, PBIGCTX_ARG)
{    /* Generate random DWORDs */

    return new_random_bytes((BYTE*)array, (size_t)lng*SIZEOF(DWORD),
                            pRNGInfo, PBIGCTX_PASS);
} /* random dwords */


#endif  // INLINING_SUPPORTED

#ifdef __cplusplus
}
#endif

#endif   // _MPRAND_H

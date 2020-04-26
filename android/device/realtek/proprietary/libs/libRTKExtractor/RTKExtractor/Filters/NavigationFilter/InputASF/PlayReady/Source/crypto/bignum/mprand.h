/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/*      File mprand.h */

#ifndef _MPRAND_H
#define _MPRAND_H

#include "bignum.h"

ENTER_PK_NAMESPACE;

DRM_API DRM_BOOL DRM_CALL random_bytes(DRM_BYTE*, const DRM_DWORD, struct bigctx_t *f_pBigCtx);
         // Procedure to be supplied by application.



#if !DRM_INLINING_SUPPORTED

DRM_API DRM_BOOL DRM_CALL     random_digits(digit_t*, const DRM_DWORD, struct bigctx_t *f_pBigCtx);

#endif // DRM_INLINING_SUPPORTED


// Following are extern on all platforms

DRM_API DRM_BOOL DRM_CALL random_digit_interval(const digit_t, const digit_t,
                                             digit_t*, struct bigctx_t *f_pBigCtx);
          /* Generate random digit_t or DRM_DWORD in specified closed interval. */

DRM_API DRM_BOOL DRM_CALL random_mod        (const digit_t *, digit_t *,
                                          const DRM_DWORD, struct bigctx_t *f_pBigCtx);

/*
** Checked stublib expects the function name to be new_random_mod_nonzero
** Temporary fix.
*/
#define random_mod_nonzero new_random_mod_nonzero
DRM_API DRM_BOOL DRM_CALL random_mod_nonzero(const digit_t *, digit_t *,
                                          const DRM_DWORD, struct bigctx_t *f_pBigCtx);
                        // Generate random value modulo another value.
                        // random_mod_nonzero generates a nonzero value.


#if DRM_INLINING_SUPPORTED

// Next four are inline, rather than macros, so compiler can issue
// error when array argument has wrong pointer type.

DRMINLINE DRM_BOOL DRM_CALL random_digits(digit_t dtArray[],
                                      const DRM_DWORD lng,
                                      struct bigctx_t *f_pBigCtx)
{    /* Generate random multiple-precision number */
     /* It may have a leading zero. */
    return random_bytes((DRM_BYTE*)dtArray,
                        (DRM_DWORD)lng*SIZEOF(digit_t),
                        f_pBigCtx);
} /* random digits */



#endif  // DRM_INLINING_SUPPORTED

EXIT_PK_NAMESPACE;

#endif   // _MPRAND_H

/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef BIGDEFS_H              /* If not previously #included */
#define BIGDEFS_H 1

//       File bigdefs.h     Version 10 February 2004

//       This file declared the fundamental types and constants used
//       within the bignum code.

//           digit_t
//           sdigit_t
//
//        used within the bignum code and headers.
//        We also declare the most fundamental routines for operating on these.
//
//        If you need to perform arithmetic, not just copy data, see bignum.h .

#include <drmcommon.h>

ENTER_PK_NAMESPACE;

/*
      Occasionally a struct name is used before the struct itself is
      declared.  The Future_Struct macro avoids a warning message
      with Visual C .NET (2002).
*/
#define Future_Struct(strname) struct strname


#if !DRM_INLINING_SUPPORTED

DRM_API DRM_DWORD DRM_CALL significant_bit_count(const digit_t);
                 /* Has special code on some platforms */

DRM_API DRM_DWORD DRM_CALL mp_significant_bit_count
                            (const digit_t *, const DRM_DWORD);

DRM_API DRM_BOOL DRM_CALL digits_to_dwords(const digit_t *, DRM_DWORD*, const DRM_DWORD );

DRM_API DRM_BOOL DRM_CALL dwords_to_digits(const DRM_DWORD*, digit_t *, const DRM_DWORD );

DRM_API DRM_VOID DRM_CALL mp_extend(const digit_t *, const DRM_DWORD, digit_t *, const DRM_DWORD );

DRM_API digit_t DRM_CALL mp_getbit(const digit_t *, const DRM_DWORD);

DRM_API DRM_VOID DRM_CALL mp_setbit(digit_t *, const DRM_DWORD, const digit_t );

DRM_API DRM_DWORD DRM_CALL significant_digit_count(const digit_t *, const DRM_DWORD);
#endif

#if DRM_INLINING_SUPPORTED

DRMINLINE DRM_VOID DRM_CALL mp_extend
        (const digit_t  a[],
         const DRM_DWORD lnga,
         digit_t   b[],
         const DRM_DWORD lngb )
/*
        Copy a to b, while changing its length from
        lnga to lngb (zero fill).  Require lngb >= lnga.
*/
{    
    MEMCPY( b,a,( lnga)*SIZEOF( digit_t ));
    ZEROMEM(b + lnga,( lngb - lnga)*SIZEOF( digit_t ));
}  // mp_extend
/****************************************************************************/
DRMINLINE digit_t DRM_CALL mp_getbit
        (const digit_t a[],
         const DRM_DWORD ibit)
                /* Extract bit of multiple precision number */
{
    return digit_getbit(a[ibit/RADIX_BITS],  ibit % RADIX_BITS);
}
/****************************************************************************/
DRMINLINE DRM_VOID DRM_CALL mp_setbit
        (digit_t   a[],
         const DRM_DWORD ibit,
         const digit_t  new_value)
/*
        Set a bit to 0 or 1,
        when the number is viewed as a bit array.
*/

{
    const DRM_DWORD j       = ibit / RADIX_BITS;
    const DRM_DWORD ishift  = ibit % RADIX_BITS;

    const digit_t mask1 = (DIGIT_ONE &  new_value) << ishift;
    const digit_t mask2 = (DIGIT_ONE & ~new_value) << ishift;
    
    a[j] = (a[j] & ~mask2) | mask1;
} // end mp_setbit
/****************************************************************************/
DRMINLINE DRM_DWORD DRM_CALL significant_digit_count
        (const digit_t  a[],
         const DRM_DWORD    lng)
/*
        Return the number of significant digits in a.
        Function value is zero precisely when a == 0.
*/
{
    DRM_DWORD i = lng;

    while (i != 0 && a[i-1] == 0) i--;
    return i;
}  /* significant_digit_count */
/****************************************************************************/
DRMINLINE DRM_BOOL DRM_CALL digits_to_dwords
        (const digit_t  pdigit[],
         DRM_DWORD     pdword[],
         const DRM_DWORD lng_dwords)
{
    DRM_BOOL OK = TRUE;

#if DWORDS_PER_DIGIT == 1    
    MEMCPY( (digit_t*)pdword,pdigit,( lng_dwords)*SIZEOF( digit_t ));
#elif DWORDS_PER_DIGIT == 2
    const DRM_DWORD lng_half = lng_dwords >> 1;
    DRM_DWORD i;
    
    if (IS_ODD(lng_dwords)) {
        pdword[lng_dwords-1] = (DRM_DWORD)pdigit[lng_half];
    }
    for (i = 0; i != lng_half; i++) {
        const digit_t dig = pdigit[i];
        pdword[2*i    ] = (DRM_DWORD)dig;
        pdword[2*i + 1] = (DRM_DWORD)(dig >> DWORD_BITS);
    }
#else
    #error "Unexpected DWORDS_PER_DIGIT"
#endif
    return OK;
}  /* digits_to_dwords */
/****************************************************************************/
DRMINLINE DRM_BOOL DRM_CALL dwords_to_digits
        (const DRM_DWORD    pdword[],
         digit_t   pdigit[],
         const DRM_DWORD lng_dwords)
{
    DRM_BOOL OK = TRUE;

#if DWORDS_PER_DIGIT == 1    
    MEMCPY( pdigit,(digit_t*)pdword,( lng_dwords)*SIZEOF( digit_t ));
#elif DWORDS_PER_DIGIT == 2
    const DRM_DWORD lng_half = lng_dwords >> 1;
    DRM_DWORD i;    

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
DRMFORCEINLINE DRM_DWORD DRM_CALL significant_bit_count(const digit_t d)
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
    DRM_DWORD width = RADIX_BITS;
    DRM_DWORD width_adj;

    while (dadj < (DIGIT_ONE << (RADIX_BITS - 5))) {
        width -= 5;
        dadj <<= 5;
    }
    dadj >>= (RADIX_BITS - 4);     /* From 0 to 15 */
    width_adj = ((DRM_DWORD)0000000011112234 >> dadj) >> (2*dadj);
                     // Shift by 3*dadj, to index into array of octal digits
    return width - (width_adj & 7);
                    // Subtract 5 - significant_bit_count(2*dadj + 1)
} /* significant_bit_count */

DRMFORCEINLINE DRM_DWORD DRM_CALL mp_significant_bit_count
        (const digit_t  a[],
        const DRM_DWORD  lnga)
/*
        Return the number of significant bits in a, which
        is one more than the truncated base 2 logarithm of a.
        Return 0 if a == 0.
*/
{
    const DRM_DWORD lng_sig = significant_digit_count(a, lnga);
    
    if (lng_sig == 0) {
        return 0;
    } else {
       return    (lng_sig - 1)*RADIX_BITS
               + significant_bit_count(a[lng_sig-1]);
    }
}


/****************************************************************************/
#endif // DRM_INLINING_SUPPORTED

/*
        The application should define the
        following three malloc-like functions.
        Sample definitions appear in bigalloc.h.
*/

DRM_API DRM_VOID* DRM_CALL bignum_alloc(const DRM_DWORD, struct bigctx_t *f_pBigCtx);
DRM_API DRM_VOID  DRM_CALL bignum_free(DRM_VOID*, struct bigctx_t *f_pBigCtx);

EXIT_PK_NAMESPACE;

#endif // BIGDEFS_H

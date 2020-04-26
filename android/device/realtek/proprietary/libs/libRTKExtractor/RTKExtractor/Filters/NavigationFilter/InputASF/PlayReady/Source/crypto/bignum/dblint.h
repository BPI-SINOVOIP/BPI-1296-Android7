/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef DBLINT_H
#define DBLINT_H 1

#include <drmint64.h>

ENTER_PK_NAMESPACE;

/*
        File: dblint.h.  Supplement to bignum.h.  Version 10 December, 2002.

        This file has declarations related to double-precision integers,
        such as typedefs, constants, and primitive operations.

        The DRM_UINT64 type is unsigned and holds twice as many bits
        as a digit_t datum.  If (DRM_NO_NATIVE_64_TYPES = 0), then use the type
        already in the language.  Otherwise (DRM_NO_NATIVE_64_TYPES = 1)
        construct one of our own, using a struct with two digit_t fields.

        Let u, u1, u2 have type digit_t and
        d, d1, d2 have type DRM_UINT64.
        The following primitives are defined,
        whether we use the built-in type or our own type:
                                        
                DPRODUU(u1, u2) -- Product of u1 and u2, as a DRM_UINT64.
                HPRODUU(u1, u2) -- Most significant half of product
                                   of u1 and u2, as a digit_t.                
*/

#if !(DRM_NO_NATIVE_64_TYPES)

#if defined (_MSC_VER)
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
#if DRM_INLINING_SUPPORTED
static __forceinline DRM_UINT64 DRM_CALL DPRODUU(const digit_t u1, const digit_t u2)
{
#if defined(_M_IX86) && !defined(__cplusplus)
        return __emulu(u1, u2);
#else
        return DRM_UI64(u1) * DRM_UI64(u2);
#endif
}
#else // INLINING
#define DPRODUU(u1, u2) (DRM_UI64(u1) * DRM_UI64(u2))
#endif

#else // _MSC_VER

#define DPRODUU(u1, u2) (DRM_UI64(u1) * DRM_UI64(u2))

#endif // _MSC_VER

#define MULTIPLY_ADD1(d1, d2, d3) \
        DRM_UI64Add(DPRODUU(d1, d2), DRM_UI64(d3))
           /* d1*d2 + d3 */

#define MULTIPLY_ADD2(d1, d2, d3, d4) \
        DRM_UI64Add(DRM_UI64Add(DPRODUU(d1, d2), DRM_UI64(d3)), \
                   DRM_UI64(d4))
          /* d1*d2 + d3 + d4 */

#define HPRODUU(u1, u2) DRM_UI64High32(DRM_UI64Mul(DRM_UI64((u1)), DRM_UI64((u2))))

#else  /* DRM_NO_NATIVE_64_TYPES */

/* No native support for 64-bit types */

#if defined( _M_AMD64_ )
    #pragma intrinsic(__umulh)
    #define HPRODUU(u1, u2) __umulh(u1, u2)
#else
    #define HPRODUU(u1, u2) DRM_UI64High32(DRM_UI64Mul(DRM_UI64((u1)), DRM_UI64((u2))))
#endif

#if !DRM_INLINING_SUPPORTED

DRM_API DRM_UINT64 DRM_CALL DPRODUU(const digit_t, const digit_t);
DRM_API DRM_UINT64 DRM_CALL MULTIPLY_ADD1(const digit_t, const digit_t, const digit_t);
DRM_API DRM_UINT64 DRM_CALL MULTIPLY_ADD2(const digit_t, const digit_t,
                                            const digit_t, const digit_t);


#endif // !DRM_INLINING_SUPPORTED

#if DRM_INLINING_SUPPORTED

DRMINLINE DRM_UINT64 DRM_CALL DPRODUU(const digit_t u1, const digit_t u2)
/*
        Multiply two single-precision operands,
        return double precision product.
        This will normally be replaced by an assembly language routine.
        unless the top half of the product (HPRODUU) is available in C.
*/
{
    return DRM_UI64Mul( DRM_UI64( u1 ), DRM_UI64( u2 ) );
}   // end DPRODUU
 


/*
    The MULTIPLY_ADD1. MULTIPLY_ADD2
    functions take single-length (digit_t) operands and
    return double-length (DRM_UINT64) results.
    Overflow is impossible.
*/


DRMINLINE DRM_UINT64 DRM_CALL MULTIPLY_ADD1(const digit_t d1, const digit_t d2,
                                                 const digit_t d3)
{
    return DRM_UI64Add( DRM_UI64Mul( DRM_UI64(d1), DRM_UI64(d2) ), DRM_UI64(d3) );
} // MULTIPLY_ADD1


DRMINLINE DRM_UINT64 DRM_CALL MULTIPLY_ADD2(const digit_t d1, const digit_t d2,
                                                 const digit_t d3, const digit_t d4)
{
    return DRM_UI64Add( DRM_UI64Add( DRM_UI64Mul( DRM_UI64(d1), DRM_UI64(d2) ), DRM_UI64(d3) ), DRM_UI64(d4) );
} // MULTIPLY_ADD2

#endif // DRM_INLINING_SUPPORTED

#endif // DRM_NO_NATIVE_64_TYPES

EXIT_PK_NAMESPACE;

#endif // DBLINT_H

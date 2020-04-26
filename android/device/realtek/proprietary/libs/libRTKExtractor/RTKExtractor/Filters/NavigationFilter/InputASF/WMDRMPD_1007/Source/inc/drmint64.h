/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMINT64_H__
#define __DRMINT64_H__


#ifdef __cplusplus 
extern "C"
{
#endif

#if DRM_NO_NATIVE_64_TYPES

#ifdef __TMS320C55X__

typedef struct _DRM_INT64 { 
    unsigned long val[2];
} DRM_INT64;

typedef struct _DRM_UINT64 {   
    unsigned long val[2];
} DRM_UINT64;

#else /* __TMS320C55X__ */

typedef struct _DRM_INT64 { 
    unsigned char val[8];
} DRM_INT64;

typedef struct _DRM_UINT64 {   
    unsigned char val[8];
} DRM_UINT64;

#endif /* ! __TMS320C55X__ */

extern DRM_INT64 DRM_I64Add(const DRM_INT64 a, const DRM_INT64 b);
extern DRM_INT64 DRM_I64Sub(const DRM_INT64 a, const DRM_INT64 b);
extern DRM_INT64 DRM_I64Mul(const DRM_INT64 a, const DRM_INT64 b);
extern DRM_INT64 DRM_I64Div(const DRM_INT64 a, const DRM_INT64 b);
extern DRM_INT64 DRM_I64Mod(const DRM_INT64 a, const DRM_INT64 b);
extern DRM_INT64 DRM_I64And(const DRM_INT64 a, const DRM_INT64 b);
extern DRM_INT64 DRM_I64ShR(const DRM_INT64 a, const int unsigned b);
extern DRM_INT64 DRM_I64ShL(const DRM_INT64 a, const int unsigned b);
extern int       DRM_I64Eql(const DRM_INT64 a, const DRM_INT64 b);
extern int       DRM_I64Les(const DRM_INT64 a, const DRM_INT64 b);
extern DRM_INT64 DRM_I64(const long int b);
extern DRM_INT64 DRM_I64Asgn(const long int a, const long int b);
extern DRM_INT64 DRM_UI2I64(const DRM_UINT64 b);
extern unsigned long int DRM_I64ToUI32(const DRM_INT64 b);

extern DRM_UINT64 DRM_UI64Add(const DRM_UINT64 a, const DRM_UINT64 b);
extern DRM_UINT64 DRM_UI64Sub(const DRM_UINT64 a, const DRM_UINT64 b);
extern DRM_UINT64 DRM_UI64Mul(const DRM_UINT64 a, const DRM_UINT64 b);
extern DRM_UINT64 DRM_UI64Div(const DRM_UINT64 a, const DRM_UINT64 b);
extern DRM_UINT64 DRM_UI64Mod(const DRM_UINT64 a, const DRM_UINT64 b);
extern DRM_UINT64 DRM_UI64And(const DRM_UINT64 a, const DRM_UINT64 b);
extern DRM_UINT64 DRM_UI64ShR(const DRM_UINT64 a, const unsigned int b);
extern DRM_UINT64 DRM_UI64ShL(const DRM_UINT64 a, const unsigned int b);
extern int        DRM_UI64Eql(const DRM_UINT64 a, const DRM_UINT64 b);
extern int        DRM_UI64Les(const DRM_UINT64 a, const DRM_UINT64 b);
extern DRM_UINT64 DRM_UI64(const unsigned long int b);

extern DRM_UINT64 DRM_UI64HL(const unsigned long int a, const unsigned long int b);
extern unsigned long int DRM_UI64High32( DRM_UINT64 a );
extern unsigned long int DRM_UI64Low32( DRM_UINT64 a );

#define DRM_UI64LITERAL(a,b) { a, b }

#define FILETIME_TO_UI64( ft, ui64 ) {ui64=DRM_UI64HL((ft).dwHighDateTime,(ft).dwLowDateTime);}
#define UI64_TO_FILETIME( ui64, ft ) {(ft).dwLowDateTime=DRM_UI64Low32(ui64);(ft).dwHighDateTime=DRM_UI64High32((ui64));}
#define NATIVE64_TO_NONNATIVE64( ui64 ) {ui64=DRM_UI64HL(DRM_UI64Low32(ui64),DRM_UI64High32(ui64));} /* Reverse the high and low */

#else

#if defined (_MSC_VER)

typedef          __int64 DRM_INT64;
typedef unsigned __int64 DRM_UINT64;

#elif defined (__GNUC__)

typedef          long long int DRM_INT64;
typedef unsigned long long int DRM_UINT64;

#endif

#define DRM_I64LITERAL(a,b) (DRM_INT64)(((DRM_INT64)a)<<32|b)
#define DRM_UI64LITERAL(a,b)(DRM_UINT64)(((DRM_UINT64)a)<<32|b)

#define FILETIME_TO_UI64( ft, ui64 ) { (ui64) = *((DRM_UINT64*)&(ft)); } 
#define UI64_TO_FILETIME( ui64, ft ) { (ft)   = *((DRMFILETIME*)&(ui64)); }
#define NATIVE64_TO_NONNATIVE64( ui64 )  

#define DRM_I64Add(a, b) ( (a) + (b) )
#define DRM_I64Sub(a, b) ( (a) - (b) )
#define DRM_I64Mul(a, b) ( (a) * (b) )
#define DRM_I64Div(a, b) ( (a) / (b) )
#define DRM_I64Mod(a, b) ( (a) % (b) )
#define DRM_I64And(a, b) ( (a) & (b) )
#define DRM_I64ShR(a, b) ( (a) >> (b) )
#define DRM_I64ShL(a, b) ( (a) << (b) )
#define DRM_I64Eql(a, b) ( (a) == (b) )
#define DRM_I64Les(a, b) ( (a) < (b) )
#define DRM_I64(b) ( (DRM_INT64) (b) )
#define DRM_I64Asgn(a, b) (((DRM_INT64)(a)<<32) | (b))
#define DRM_UI2I64(b) ((DRM_INT64)(b))

#define DRM_I64ToUI32(b) ((DRM_ULONG)(b))


#define DRM_UI64Add(a, b) ( (a) + (b) )
#define DRM_UI64Sub(a, b) ( (a) - (b) )
#define DRM_UI64Mul(a, b) ( (a) * (b) )
#define DRM_UI64Div(a, b) ( (a) / (b) )
#define DRM_UI64Mod(a, b) ( (a) % (b) )
#define DRM_UI64And(a, b) ( (a) & (b) )
#define DRM_UI64ShR(a, b) ( (a) >> (b) )
#define DRM_UI64ShL(a, b) ( (a) << (b) )
#define DRM_UI64Eql(a, b) ( (a) == (b) )
#define DRM_UI64Les(a, b) ( (a) < (b) )
#define DRM_UI64(b) ( (DRM_UINT64) (b) )

#define DRM_UI64HL(a,b) (((DRM_UINT64)(a)<<32) | (b))
#define DRM_UI64High32(a) ((unsigned long int)(a>>32))
#define DRM_UI64Low32(a) ((unsigned long int)(a&DRM_UI64LITERAL(0,0xFFFFFFFF)))

#endif

#ifdef __cplusplus 
}
#endif

#endif /* __DRMINT64_H__ */

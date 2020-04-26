/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMTYPES_H__
#define __DRMTYPES_H__

#include <drmnamespace.h>   /* defining proper namespace (if used) */
#include <drmcompiler.h>
#include <drmint64.h>

ENTER_PK_NAMESPACE;

//typedef void      DRM_VOID;
#define  DRM_VOID   void

#ifndef DRM_RESULT_DEFINED
#define DRM_RESULT_DEFINED
typedef DRM_LONG  DRM_RESULT;
#endif /*DRM_RESULT_DEFINED*/

typedef DRM_WORD  DRM_LANGID;


#define CHAR_BIT      8                /* number of bits in a char    */
#define SCHAR_MIN   (-128)             /* minimum signed char value   */
#define SCHAR_MAX     127              /* maximum signed char value   */
#define UCHAR_MAX     0xff             /* maximum unsigned char value */
#define LONG_MIN    (-2147483647L - 1) /* minimum (signed) long value */
#define LONG_MAX      2147483647L      /* maximum (signed) long value */
#define ULONG_MAX     0xffffffffUL     /* maximum unsigned long value */

#if SIXTEEN_BIT_ADDRESSING
#define CB_NATIVE_BYTE 2
#define __CB_DECL(x) ((x)/CB_NATIVE_BYTE+(((x)%CB_NATIVE_BYTE)?1:0))
#else
#define CB_NATIVE_BYTE 1
#define __CB_DECL(x) (x)
#endif

#ifndef SIZEOF
#define SIZEOF(x) (sizeof(x)*CB_NATIVE_BYTE)
#endif

#ifndef BITS_PER_BYTE
#define BITS_PER_BYTE (CB_NATIVE_BYTE*8)
#endif

#if !defined( _W64 )
#if  ( defined(_X86_) || defined(_M_IX86) ) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif


#if defined( _WIN64)
typedef DRM_INT64 DRM_INT_PTR;
typedef DRM_UINT64 DRM_UINT_PTR;
typedef DRM_UINT64 DRM_DWORD_PTR;
typedef DRM_UINT64 DRM_SIZE_T;
#else
typedef _W64 DRM_INT DRM_INT_PTR;
typedef _W64 DRM_UINT DRM_UINT_PTR;
typedef _W64 DRM_DWORD DRM_DWORD_PTR;
typedef _W64 DRM_DWORD DRM_SIZE_T;
#endif

#define IS_DWORD_ALIGNED( x ) ( ( DRM_DWORD_PTR )( x ) % SIZEOF( DRM_DWORD_PTR ) == 0 )
#define DWORD_ALIGNED_SIZE( x ) ( ( (x) % SIZEOF( DRM_DWORD_PTR ) == 0 ) ? (x) : (x) + ( SIZEOF( DRM_DWORD_PTR ) - ( (x) % SIZEOF( DRM_DWORD_PTR ) ) ) )

typedef struct __tagDRM_GUID {
    DRM_DWORD Data1;
    DRM_WORD  Data2;
    DRM_WORD  Data3;
    DRM_BYTE  Data4 [__CB_DECL(8)];
} DRM_GUID;

#define EMPTY_DRM_GUID  { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } }

#define IDENTICAL_GUIDS(p0,p1) \
    (MEMCMP((p0), (p1), SIZEOF(DRM_GUID)) == 0)

/*
** The following macro should be used to define GUIDs that are local to a single code file.
*/
#define DRM_DEFINE_LOCAL_GUID( name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8 )  \
        static const DRM_GUID name                                                 \
                = { l, w1, w2, { TWO_BYTES( b1, b2 ), TWO_BYTES( b3, b4 ),  \
                                 TWO_BYTES( b5, b6 ), TWO_BYTES( b7, b8 ) } }

/*
** The following two macros should be used to declare and define GUIDs that are
** shared among multiple code files.
*/
#define DRM_DEFINE_GUID( name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8 )  \
        const DRM_EXPORT_VAR DRM_GUID name                                  \
                = { l, w1, w2, { TWO_BYTES( b1, b2 ), TWO_BYTES( b3, b4 ),  \
                                 TWO_BYTES( b5, b6 ), TWO_BYTES( b7, b8 ) } }

#define DRM_DECLARE_GUID( name )    \
        extern const DRM_EXPORT_VAR DRM_GUID name

/*
** This macro will calculate the maximum value of an unsigned data type 
** where the type is passed in as x.
** For example MAX_UNSIGNED_TYPE( DRM_BYTE  ) will evaluate to 0xFF
**             MAX_UNSIGNED_TYPE( DRM_DWORD ) will evaluate to 0xFFFFFFFF
*/
#define MAX_UNSIGNED_TYPE(x) ( (x)~((x)0) )

/*
**
*/
typedef struct tagDRM_CONST_STRING
{
    const DRM_WCHAR *pwszString;
    DRM_DWORD        cchString;
} DRM_CONST_STRING;

/*
**
*/
typedef struct tagDRM_STRING
{
    DRM_WCHAR *pwszString;
    DRM_DWORD  cchString;
} DRM_STRING;

typedef struct tagDRM_ANSI_CONST_STRING
{
    const DRM_CHAR *pszString;
    DRM_DWORD       cchString;
} DRM_ANSI_CONST_STRING;

typedef struct tagDRM_ANSI_STRING
{
    DRM_CHAR  *pszString;
    DRM_DWORD  cchString;
} DRM_ANSI_STRING;

typedef struct __tagSubString
{
    DRM_DWORD m_ich;
    DRM_DWORD m_cch;
} DRM_SUBSTRING;

typedef struct tagDRM_STRING_WINDOW
{
    DRM_DWORD m_ichMin; /* inclusive */
    DRM_DWORD m_ichMaxExclusive; /* exclusive */
} DRM_STRING_WINDOW;

#define ASSIGN_DRM_ANSI_STRING(x,y) \
{ \
    (x).pszString = (y); \
    (x).cchString = NO_OF(y)-1; \
}

#define ASSIGN_DRM_STRING(x,y) \
{ \
    (x).pwszString = (y).pwszString; \
    (x).cchString  = (y).cchString; \
}

#define INIT_DRM_STRING(x) \
{ \
    (x).pwszString = NULL; \
    (x).cchString  = 0; \
}

#define INIT_DRM_ANSI_STRING(x) \
{ \
    (x).pszString = NULL; \
    (x).cchString = 0; \
}

/* This macro can only be used at the declaration of DRM_STRING 
** It initailizes DRM  string from DRM_WCHAR array.
*/

#if TARGET_LITTLE_ENDIAN || SIXTEEN_BIT_ADDRESSING
/* Little endian machines will use L"".  Packed strings will use TWO_BYTES( ) which will be a short and the right size */
#define CREATE_DRM_STRING(x) { ((const DRM_WCHAR *) (x)), (NO_OF(x)) - 1 }
#else
/* Big Endian processors that are 8-bit referencable will have strings like "S\0t\0r\0i\0n\0g\0" */
#define CREATE_DRM_STRING(x) { (DRM_WCHAR*) x, (NO_OF(x)-1)/SIZEOF(DRM_WCHAR)}
#endif

#if TARGET_LITTLE_ENDIAN || SIXTEEN_BIT_ADDRESSING
#define CREATE_ANSI_DRM_STRING(x) { ((const DRM_CHAR *) (x)), (NO_OF(x)) - 1 }
#else
#define CREATE_ANSI_DRM_STRING(x) { (DRM_CHAR*) x, SIZEOF(x) - 1}
#endif

/* This macro can be used to initialize DRM_STRING from DRM_WCHAR 
** in the middle of the function code
*/

#if TARGET_LITTLE_ENDIAN || SIXTEEN_BIT_ADDRESSING
/* 
** Little endian machines will use L"". Packed strings will use TWO_BYTES( ) 
** which will be a short and the right size 
**
** Assert if the pointer is not DRM_WCHAR aligned
*/
#define MAKE_DRM_STRING(x, y) \
    x.pwszString = y; \
    x.cchString = (NO_OF(y)) - 1; \
    DRMASSERT( (DRM_DWORD_PTR)(y) % sizeof( DRM_WCHAR ) == 0 );
#else
/* 
** Big Endian processors that are 8-bit referencable will have strings like "S\0t\0r\0i\0n\0g\0" 
*/
#define MAKE_DRM_STRING(x, y) \
    x.pwszString = (DRM_WCHAR*)y; \
    x.cchString = ((NO_OF(y)-1)/SIZEOF(DRM_WCHAR); \
    DRMASSERT( (DRM_DWORD_PTR)(y) % sizeof( DRM_WCHAR ) == 0 );
#endif 


#if TARGET_LITTLE_ENDIAN
#define WCHAR_CAST(x)   ((DRM_WCHAR)(x))
#define NATIVE_WCHAR(x) (x)
#else
#define WCHAR_CAST(x)   ((DRM_WCHAR)(x)<<8)
#define NATIVE_WCHAR(x) ((x)>>8)
#endif

/* 
** Note: in drmconstants.c CREATE_DRM_ANSI_STRING should be used
** with caution. When we create a const ansi string from const char[] arrays
** an array may have one or two trailing zero chars for alignment reasons,
** but this macro just always sets ansi string's char size into a size of array - 1.
*/
#define CREATE_DRM_ANSI_STRING(x) {(x),((NO_OF(x))-1)*CB_NATIVE_BYTE}

#define EMPTY_DRM_STRING        { NULL, 0 }
#define EMPTY_DRM_SUBSTRING     { 0, 0 }
#define EMPTY_DRM_STRING_WINDOW { 0, 0 }

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(p)   (p)
#endif

/*
**  Assert if the pointer is not DRM_WCHAR aligned
 */

#define DSTR_FROM_PB(pdstr,pb,cb) \
    (pdstr)->cchString=(cb)/SIZEOF(DRM_WCHAR); \
    (pdstr)->pwszString=(DRM_WCHAR*)(pb); \
    DRMASSERT( (DRM_DWORD_PTR)(pb) % sizeof( DRM_WCHAR ) == 0 );

#define DASTR_FROM_PB(pdastr,pb,cb) \
    (pdastr)->cchString=(cb)/( SIZEOF( DRM_CHAR ) / CB_NATIVE_BYTE ); \
    (pdastr)->pszString=(DRM_CHAR*)(pb); 

/* for frequent uses of DRM_CONST_STRINGs' char counts and
  wide-char buffers as byte counts and byte buffers */

#define PB_DSTR(pdstr) (DRM_BYTE*)((pdstr)->pwszString)
#define CB_DSTR(pdstr) ((pdstr)->cchString*SIZEOF(DRM_WCHAR))

#define PB_DASTR(pdstr) (DRM_BYTE*)((pdstr)->pszString)
#define CB_DASTR(pdstr) ((pdstr)->cchString)

#define C_TICS_PER_SECOND 10000000
#define C_SECONDS_FROM_1601_TO_1970  DRM_UI64Mul(DRM_UI64(1164447360), DRM_UI64(10))
    
#define CREATE_FILE_TIME(dw, ft) {  \
            DRM_UINT64 ui64Tics = DRM_UI64Add( DRM_UI64( dw ), C_SECONDS_FROM_1601_TO_1970 ); \
            ui64Tics = DRM_UI64Mul( ui64Tics, DRM_UI64( C_TICS_PER_SECOND ) );                \
            UI64_TO_FILETIME( ui64Tics, ft );                                                 \
        }

#define FILE_TIME_TO_DWORD(ft, dw) {  \
            DRM_UINT64 ui64Tics; \
            FILETIME_TO_UI64( ft, ui64Tics ); \
            ui64Tics = DRM_UI64Div( ui64Tics, DRM_UI64( C_TICS_PER_SECOND ) );                \
            dw = DRM_UI64Low32( DRM_UI64Sub( ui64Tics, C_SECONDS_FROM_1601_TO_1970 ) ); \
        }

typedef struct _tagDRMSYSTEMTIME
{
    DRM_WORD wYear;
    DRM_WORD wMonth;
    DRM_WORD wDayOfWeek;
    DRM_WORD wDay;
    DRM_WORD wHour;
    DRM_WORD wMinute;
    DRM_WORD wSecond;
    DRM_WORD wMilliseconds;
}     DRMSYSTEMTIME;

#if TARGET_LITTLE_ENDIAN
typedef struct _tagDRMFILETIME
{
    DRM_DWORD dwLowDateTime;
    DRM_DWORD dwHighDateTime;
} DRMFILETIME;
#else
typedef struct _tagDRMFILETIME
{
    DRM_DWORD dwHighDateTime;
    DRM_DWORD dwLowDateTime;
} DRMFILETIME;
#endif
/* generic ID type, currently all the same size */

#define DRM_ID_SIZE  16

typedef struct _tagDRM_ID 
{
    DRM_BYTE rgb [__CB_DECL(DRM_ID_SIZE)];
} DRM_ID;

typedef DRM_ID DRM_MID;
typedef DRM_ID DRM_KID;
typedef DRM_ID DRM_LID;
typedef DRM_ID DRM_TID;

/* Structure that contains the definition of a domain ID. */
typedef struct
{
    /* Service ID. */
    DRM_GUID    m_oServiceID;

    /* Account ID. */
    DRM_GUID    m_oAccountID;

    /* Revision. */
    DRM_DWORD   m_dwRevision;
} DRM_DOMAIN_ID;

#define EMPTY_DRM_DOMAIN_ID { EMPTY_DRM_GUID, EMPTY_DRM_GUID, 0 }

/* SLK Size: SLK is either DES (8 byte) or AES (16 byte) */
#define DRM_SLK_SIZE 16

typedef struct __tagDRM_SLK
{
    DRM_BYTE  rgbKey[__CB_DECL( DRM_SLK_SIZE )];
    DRM_DWORD cbKey;
} DRM_SLK;

typedef struct _tagDRM_SLKDATA
{
    DRM_SLK slk;
    DRM_ID  idSLK;    
} DRM_SLKDATA;

typedef struct _tagDRM_BYTEBLOB
{
    DRM_BYTE *pbBlob;
    DRM_DWORD cbBlob;
} DRM_BYTEBLOB;

EXIT_PK_NAMESPACE;

/******* The following definitions are pulled from bigdefs.h and are for use by bignum *******/
#define DWORD_BITS 32
#define DWORD_LEFT_BIT 0x80000000UL

/*      Multiple-precision data is normally represented
**      in radix 2^RADIX_BITS, with RADIX_BITS bits per word.
**      Here ``word'' means type digit_t.  RADIX_BITS
**      should be 32 for all supported architectures */
#define RADIX_BITS      32
#define RADIX_BYTES     4

/* Datatypes used by bignum */
typedef DRM_DWORD       digit_t;
typedef DRM_LONG        sdigit_t;
struct bigctx_t;

#define DIGIT_ZERO ((digit_t)0)
#define DIGIT_ONE  ((digit_t)1)

#define RADIX_HALF (DIGIT_ONE << (RADIX_BITS - 1))
#define RADIXM1 (DIGIT_ZERO - DIGIT_ONE)

#define DWORDS_PER_DIGIT (RADIX_BITS/DWORD_BITS)

/* DWORDS_TO_DIGITS(lng_dwords) computes the number of digit_t
** elements required to store an array with -lng_dwords- DWORDs.
** DIGITS_TO_DWORDS converts in the opposite direction. */
#define DWORDS_TO_DIGITS(lng_dwords) \
                ( ((lng_dwords) + DWORDS_PER_DIGIT - 1)/DWORDS_PER_DIGIT)
#define DIGITS_TO_DWORDS(lng_digits) ((lng_digits) * DWORDS_PER_DIGIT)
#define BITS_TO_DIGITS(nb) (((nb) + RADIX_BITS - 1)/RADIX_BITS)
#define digit_getbit(iword, ibit) (((iword) >> (ibit)) & 1)

/*
**      Test whether a (possibly negative) number is odd or even.
*/
#define IS_EVEN(n) (~(DRM_UINT)(n) & 1)
#define IS_ODD(n) ((DRM_UINT)(n) & 1)

/*
        Maximum and minimum of two arguments
        (no side effects in arguments)
*/
#ifndef MAX
    #define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif
#ifndef MIN
    #define MIN(x, y) ((x) > (y) ? (y) : (x))
#endif

#endif  /* __DRMTYPES_H__ */

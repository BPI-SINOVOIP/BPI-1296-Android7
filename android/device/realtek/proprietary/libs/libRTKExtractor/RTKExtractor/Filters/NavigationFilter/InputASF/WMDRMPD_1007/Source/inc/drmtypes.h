/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMTYPES_H__
#define __DRMTYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Would be good to have a test for platform.  If the platform isn't Windows
	we should define __cdecl to be nothing.  We can't just use _MSC_VER though
	as there are other compilers for the Windows platforms. */
#if defined (_MSC_VER)
#ifdef __STDC__ /* when compiling as ANSI, parameter-passing specifications aren't allowed */
#define DRM_API 
#define DRMINLINE
#define DRMFORCEINLINE
#else
#define DRM_API __cdecl   
#define DRMINLINE _inline
#define DRMFORCEINLINE  __forceinline
#define DRM_INLINING_SUPPORTED 1
#endif
#elif defined (__GNUC__)
#define DRM_API 
#define DRMINLINE inline
#define DRMFORCEINLINE  inline
#define DRM_INLINING_SUPPORTED 1
#elif defined (__TMS320C55X__)
#define SIXTEEN_BIT_ADDRESSING 1
#define DRM_NO_NATIVE_64_TYPES 1
#define DRM_API 
#define DRMINLINE inline
#define DRMFORCEINLINE  inline
#define DRM_INLINING_SUPPORTED 1
#elif defined (__arm)
#define DRM_API    
#define DRMINLINE inline
#define DRMFORCEINLINE  inline
#define DRM_INLINING_SUPPORTED 1
#else
#error Unknown compiler -- fix up drmcommon.h to understand this compiler.
#define DRM_API    
#endif  


/* 
* In General, 
* DRM_CHAR* is used for null terminated ANSI strings
* DRM_BYTE* is used for just plain byte buffer, w/o any termination
*/

#if defined(_M_IX86)
#define TARGET_LITTLE_ENDIAN 1
#elif defined( _M_AMD64 )
#define TARGET_LITTLE_ENDIAN 1
#elif defined( _M_IA64 )
#define TARGET_LITTLE_ENDIAN 1
#elif defined( ARM )
#define TARGET_LITTLE_ENDIAN 1
#elif defined(__TMS320C55X__)
#define TARGET_LITTLE_ENDIAN 0
#define SIXTEEN_BIT_ADDRESSING 1
#else
#define TARGET_LITTLE_ENDIAN 1
#endif
/* _M_IA64 _M_AMD64 */

                                                                        /* 8-bit addressing    16 bit addressing*/
typedef void                    DRM_VOID;
typedef unsigned char           DRM_BYTE;                               /* 1 byte              1 byte */
typedef char                    DRM_CHAR;                               /* 1 byte              1 byte */
typedef unsigned short          DRM_WORD, DRM_WCHAR;                    /* 2 bytes             1 byte */
typedef short                   DRM_SHORT;                              /* 2 bytes             1 byte */
#if SIXTEEN_BIT_ADDRESSING
typedef unsigned int            DRM_BOOL;                               /* 1 bytes             1 bytes */
typedef unsigned long           DRM_UINT;                               /* 4 bytes             2 bytes */
typedef long                    DRM_INT;                                /* 4 bytes             2 bytes */
typedef long                    DRM_LONG;                               /* 4 bytes             2 bytes */
typedef long                    DRM_RESULT;                             /* 4 bytes             2 bytes */
#else
typedef unsigned int            DRM_UINT,DRM_BOOL;                      /* 4 bytes             2 bytes */
typedef int                     DRM_INT;                                /* 4 bytes             2 bytes */
typedef long                    DRM_LONG, DRM_RESULT;                   /* 4 bytes             2 bytes */
#endif
typedef unsigned long           DRM_DWORD, DRM_ULONG;                   /* 4 bytes             2 bytes */
typedef unsigned short          DRM_LANGID;

#include <drmint64.h>

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

#include <drmbytemanip.h>

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
#else
typedef _W64 DRM_INT DRM_INT_PTR;
typedef _W64 DRM_UINT DRM_UINT_PTR;
typedef _W64 DRM_DWORD DRM_DWORD_PTR;
#endif

#define BYTE2LITTLEENDIAN(dword, byte)     DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),0,SIZEOF(DRM_DWORD));
#define DWORD2LITTLEENDIAN( byte, dword )  DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(dword),0,SIZEOF(DRM_DWORD));

#define BYTE2BIGENDIAN(dword, byte)        {DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),0,SIZEOF(DRM_DWORD));FIX_ENDIAN_DWORD((dword));}
#define DWORD2BIGENDIAN(byte, dword)       {FIX_ENDIAN_DWORD((dword));DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(dword),0,SIZEOF(DRM_DWORD));FIX_ENDIAN_DWORD((dword));}


#if TARGET_LITTLE_ENDIAN
#define BYTES_TO_DWORD BYTE2LITTLEENDIAN
#define DWORD_TO_BYTES DWORD2LITTLEENDIAN
#define FIX_ENDIAN_WORD(w)
#define FIX_ENDIAN_DWORD(dw)
#define FIX_ENDIAN_QWORD(qw)
#else
#define BYTES_TO_DWORD BYTE2BIGENDIAN
#define DWORD_TO_BYTES DWORD2BIGENDIAN

#if __TMS320C55X__
/* Work around a bug in the TI compiler */
#define FIX_ENDIAN_WORD(w)   {DRM_DWORD __dw = SIZEOF(DRM_WORD);DRM_BYT_ReverseBytes((DRM_BYTE*)&(w), __dw);}
#define FIX_ENDIAN_DWORD(dw) {DRM_DWORD __dw = SIZEOF(DRM_DWORD);DRM_BYT_ReverseBytes((DRM_BYTE*)&(dw),__dw);}
#define FIX_ENDIAN_QWORD(qw) {DRM_DWORD __dw = SIZEOF(DRM_UINT64);DRM_BYT_ReverseBytes((DRM_BYTE*)&(qw),__dw);NATIVE64_TO_NONNATIVE64((qw));}
#else
#define FIX_ENDIAN_WORD(w)   DRM_BYT_ReverseBytes((DRM_BYTE*)&(w), SIZEOF(DRM_WORD))
#define FIX_ENDIAN_DWORD(dw) DRM_BYT_ReverseBytes((DRM_BYTE*)&(dw),SIZEOF(DRM_DWORD))
#define FIX_ENDIAN_QWORD(qw) {DRM_BYT_ReverseBytes((DRM_BYTE*)&(qw),SIZEOF(DRM_UINT64));NATIVE64_TO_NONNATIVE64((qw));}
#endif

#endif

typedef struct __tagDRM_GUID {
    DRM_DWORD Data1;
    DRM_WORD  Data2;
    DRM_WORD  Data3;
    DRM_BYTE  Data4 [__CB_DECL(8)];
} DRM_GUID;

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

#if TARGET_LITTLE_ENDIAN || SIXTEEN_BIT_ADDRESSING
/* Little endian machines will use L"".  Packed strings will use TWO_BYTES( ) which will be a short and the right size */
#define CREATE_DRM_STRING(x) { (x), (NO_OF(x)) - 1 }
#else
/* Big Endian processors that are 8-bit referencable will have strings like "S\0t\0r\0i\0n\0g\0" */
#define CREATE_DRM_STRING(x) { (DRM_WCHAR*) x, (NO_OF(x)-1)/SIZEOF(DRM_WCHAR)}
#endif

#if TARGET_LITTLE_ENDIAN
#define WCHAR_CAST(x)   ((DRM_WCHAR)(x))
#define NATIVE_WCHAR(x) (x)
#else
#define WCHAR_CAST(x)   ((DRM_WCHAR)(x)<<8)
#define NATIVE_WCHAR(x) ((x)>>8)
#endif

#define CREATE_DRM_ANSI_STRING(x) {(x),((NO_OF(x))-1)*CB_NATIVE_BYTE}

#define EMPTY_DRM_STRING     { NULL, 0 }
#define EMPTY_DRM_SUBSTRING  { 0, 0 }

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(p)   (p)
#endif

#define DSTR_FROM_PB(pdstr,pb,cb) \
    (pdstr)->cchString=(cb)/SIZEOF(DRM_WCHAR); \
    (pdstr)->pwszString=(DRM_WCHAR*)(pb); 

#define DASTR_FROM_PB(pdastr,pb,cb) \
    (pdastr)->cchString=(cb)/( SIZEOF( DRM_CHAR ) / CB_NATIVE_BYTE ); \
    (pdastr)->pszString=(DRM_CHAR*)(pb); 

/* for frequent uses of DRM_CONST_STRINGs' char counts and
  wide-char buffers as byte counts and byte buffers */

#define PB_DSTR(pdstr) (DRM_BYTE*)((pdstr)->pwszString)
#define CB_DSTR(pdstr) ((pdstr)->cchString*SIZEOF(DRM_WCHAR))

#define PB_DASTR(pdstr) (DRM_BYTE*)((pdstr)->pszString)
#define CB_DASTR(pdstr) ((pdstr)->cchString)

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
} 	DRMSYSTEMTIME;

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

#define DRM_SLK_SIZE 8

typedef struct __tagDRM_SLK
{
    DRM_BYTE rgb[__CB_DECL(DRM_SLK_SIZE)];
} DRM_SLK;

typedef struct _tagDRM_BYTEBLOB
{
    DRM_BYTE *pbBlob;
    DRM_DWORD cbBlob;
} DRM_BYTEBLOB;


#ifdef __cplusplus
}
#endif

#endif  /* __DRMTYPES_H__ */

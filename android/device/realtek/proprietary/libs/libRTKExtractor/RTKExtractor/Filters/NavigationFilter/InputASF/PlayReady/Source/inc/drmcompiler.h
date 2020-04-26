/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMCOMPILER_H__
#define __DRMCOMPILER_H__

/*
** +-----------------------------------+
** | COMPILATION QUALIFIER DEFINITIONS |
** +-----------------------------------+
*/

/* ====================== Microsoft specific qualifiers ======================= */
/* ============================================================================ */
#if defined (_MSC_VER)

#define DRM_CCALL      __cdecl
#define DRM_STDCALL    __stdcall 
#define DRM_FASTCALL   __fastcall

#define DRM_DLLEXPORT  __declspec(dllexport)
#define DRM_DLLIMPORT  __declspec(dllimport)
#define DRM_ALIGN_4    __declspec(align(4))


/* ========================= GNU specific qualifiers ========================== */
/* ============================================================================ */
#elif defined (__GNUC__)

#include <stdint.h>

#define DRM_CCALL      __attribute__((cdecl))
#define DRM_STDCALL    __attribute__((stdcall))
#define DRM_FASTCALL   __attribute__((fastcall))

#define DRM_DLLEXPORT  __attribute__((dllexport))
#define DRM_DLLIMPORT  __attribute__((dllimport))
#define DRM_ALIGN_4    __attribute__((aligned(4)))


/* ======================== Default (empty) qualifiers ======================== */
/* ============================================================================ */
#else

#define DRM_CCALL
#define DRM_STDCALL
#define DRM_FASTCALL

#define DRM_DLLEXPORT
#define DRM_DLLIMPORT
#define DRM_ALIGN_4

#endif


#if DRM_NO_FORCE_ALIGN
#undef DRM_ALIGN_4
#define DRM_ALIGN_4
#endif

/*
** This option specifies that assembly routines will be used, if they exist for the given target
** The RMSDK disables this option for use in managed C++ components
*/
#ifndef DRM_USE_ASSEMBLY
#define DRM_USE_ASSEMBLY 1
#endif

    
/*
** +-----------------------------------+
** |   COMPILATION BEHAVIOR SETTINGS   |
** +-----------------------------------+
*/

/* ============================ Microsoft Compiler ============================ */
/* ============================================================================ */
#if defined (_MSC_VER)

/* ------------------------------ MS ANSI build ------------------------------- */
/* ---------------------------------------------------------------------------- */
/* when compiling as ANSI, parameter-passing specifications aren't allowed      */
#if defined (__STDC__) 
#define SIXTEEN_BIT_ADDRESSING 0
#define DRM_CALL
#define DRMINLINE
#define DRMFORCEINLINE
#define DRM_EXTERN_INLINE
#define DRM_EXPORTED_INLINE
#define DRM_INLINING_SUPPORTED 0
#define DRM_DWORD_ALIGN
#define DRM_NO_INLINE

/* ---------------------------- MS non-ANSI build ----------------------------- */
/* ---------------------------------------------------------------------------- */
#else

/* ----------------- Building the PK DLL ----------------- */
/* Set the macros to export the APIs and global variables  */
#if DRM_EXPORT_APIS_TO_DLL
#define DRM_API         DRM_DLLEXPORT
#define DRM_EXPORT_VAR  DRM_DLLEXPORT
#endif

/* ------------ Building the Test Executables ------------ */
/* Set the macro to import the global variables            */
#if DRM_TEST_LINK_TO_DRMAPI_DLL
#define DRM_EXPORT_VAR  DRM_DLLIMPORT
#endif

#define SIXTEEN_BIT_ADDRESSING  0
#define DRM_CALL                DRM_CCALL
#define DRMINLINE               _inline
#define DRMFORCEINLINE          __forceinline
#define DRM_EXTERN_INLINE       extern DRMINLINE
#define DRM_EXPORTED_INLINE     DRMINLINE
#define DRM_INLINING_SUPPORTED  1
#define DRM_DWORD_ALIGN         DRM_ALIGN_4
#define DRM_NO_INLINE           __declspec(noinline)
#endif

/* =============================== GNU Compiler =============================== */
/* ============================================================================ */
#elif defined (__GNUC__)
#define SIXTEEN_BIT_ADDRESSING  0
#define DRM_CALL
#define DRMINLINE               static inline
#if defined (_NATIVE_COMPILE)
#define DRMFORCEINLINE          inline
#else
#define DRMFORCEINLINE          static inline
#endif
#define DRM_EXTERN_INLINE       extern inline
#define DRM_EXPORTED_INLINE     inline
#define DRM_INLINING_SUPPORTED  1
#define DRM_DWORD_ALIGN         DRM_ALIGN_4
#define DRM_NO_INLINE

/* =============================== TI Compiler ================================ */
/* ============================================================================ */
#elif defined (__TMS320C55X__)
#define SIXTEEN_BIT_ADDRESSING  1
#define DRM_CALL
#define DRMINLINE               inline
#define DRMFORCEINLINE          inline
#define DRM_EXTERN_INLINE       extern 
#define DRM_EXPORTED_INLINE
#define DRM_INLINING_SUPPORTED  1
#define DRM_DWORD_ALIGN         DRM_ALIGN_4
#define DRM_NO_INLINE

/* ============================= Unknown Compiler ============================= */
/* ============================================================================ */
#else
#error Unknown compiler - please supply appropriate definitions for the above
#endif  


/*
** Currently, creating a DLL is only supported on Microsoft compiler non-ANSI builds,
** so ensure that the following DLL specific macros are empty if not set above.
*/
#ifndef DRM_API
#define DRM_API
#endif

#ifndef DRM_EXPORT_VAR
#define DRM_EXPORT_VAR
#endif


/*
** +-----------------------------------+
** |     LITTLE ENDIAN / BIG ENDIAN    |
** +-----------------------------------+
*/
#ifndef TARGET_LITTLE_ENDIAN
#if defined(_M_IX86)       /* Microsoft X86 compiler detected   */
#define TARGET_LITTLE_ENDIAN 1
#elif defined( _M_AMD64 )  /* Microsoft AMD64 compiler detected */
#define TARGET_LITTLE_ENDIAN 1
#elif defined( _M_IA64 )   /* Microsoft IA64 compiler detected  */
#define TARGET_LITTLE_ENDIAN 1
#elif defined( ARM )
#define TARGET_LITTLE_ENDIAN 1
#elif defined(__TMS320C55X__)
#define TARGET_LITTLE_ENDIAN 0
#else
#define TARGET_LITTLE_ENDIAN 1
//#error Unknown target - you will need to define TARGET_LITTLE_ENDIAN to 0 or 1.
#endif
#endif



/*
** +-----------------------------------+
** |    BASIC TYPE/SIZE DEFINITIONS    |
** +-----------------------------------+
*/

/* 
** In General, 
** DRM_CHAR* is used for null terminated ANSI strings
** DRM_BYTE* is used for just plain byte buffer, w/o any termination
*/

/*
** Wide character is special type in C++ - wchar_t, 
** while for "C" compilation it is just unsigned short.
** So 2 different types for C and C++.
*/

/* ======================== Microsoft compiler sizes ========================== */
/* ============================================================================ */
#if defined (_MSC_VER)
                                                   /* 8-bit addressing    16 bit addressing*/
typedef unsigned __int8         DRM_BYTE;               /* 1 byte              1 byte */
typedef __int8                  DRM_CHAR;               /* 1 byte              1 byte */
typedef unsigned __int16        DRM_WORD;               /* 2 bytes             1 byte */
typedef __int16                 DRM_SHORT;              /* 2 bytes             1 byte */
typedef unsigned __int16        DRM_WCHAR;              /* 2 bytes             1 byte */
typedef __int32                 DRM_BOOL;               /* 4 bytes             2 bytes */
typedef unsigned long           DRM_DWORD;              /* 4 bytes             2 bytes */
typedef unsigned __int32        DRM_WCHAR32;            /* 4 bytes             2 bytes */
typedef long                    DRM_LONG;               /* 4 bytes             2 bytes */
typedef unsigned __int32        DRM_UINT;               /* 4 bytes             2 bytes */
typedef __int32                 DRM_INT;                /* 4 bytes             2 bytes */

#ifdef __cplusplus
typedef wchar_t                 DRM_WCHAR_NATIVE;      /* 2 bytes             1 byte */
#else
typedef unsigned __int16        DRM_WCHAR_NATIVE;      /* 2 bytes             1 byte */
#endif


/* =========================== GNU compiler sizes ============================= */
/* ============================================================================ */
#elif defined (__GNUC__)
                                                   /* 8-bit addressing    16 bit addressing*/
typedef uint8_t                 DRM_BYTE;               /* 1 byte              1 byte */
typedef int8_t                  DRM_CHAR;               /* 1 byte              1 byte */
typedef uint16_t                DRM_WORD;               /* 2 bytes             1 byte */
typedef int16_t                 DRM_SHORT;              /* 2 bytes             1 byte */
typedef uint16_t                DRM_WCHAR;              /* 2 bytes             1 byte */
typedef int32_t                 DRM_BOOL;               /* 4 bytes             2 bytes */
typedef uint32_t                DRM_DWORD;              /* 4 bytes             2 bytes */
typedef uint32_t                DRM_WCHAR32;            /* 4 bytes             2 bytes */
typedef int32_t                 DRM_LONG;               /* 4 bytes             2 bytes */
typedef uint32_t                DRM_UINT;               /* 4 bytes             2 bytes */
typedef int32_t                 DRM_INT;                /* 4 bytes             2 bytes */

#ifdef __cplusplus
typedef wchar_t                 DRM_WCHAR_NATIVE;      /* 2 bytes             1 byte */
#else
typedef uint16_t                DRM_WCHAR_NATIVE;      /* 2 bytes             1 byte */
#endif


/* ============================== Default sizes =============================== */
/* ============================================================================ */
#else
                                                   /* 8-bit addressing    16 bit addressing*/
typedef unsigned char           DRM_BYTE;               /* 1 byte              1 byte */
typedef char                    DRM_CHAR;               /* 1 byte              1 byte */
typedef unsigned short          DRM_WORD;               /* 2 bytes             1 byte */
typedef short                   DRM_SHORT;              /* 2 bytes             1 byte */
typedef unsigned short          DRM_WCHAR;              /* 2 bytes             1 byte */
typedef int                     DRM_BOOL;               /* 1 bytes             1 bytes */
typedef unsigned long           DRM_DWORD;              /* 4 bytes             2 bytes */
typedef unsigned int            DRM_WCHAR32;            /* 4 bytes             2 bytes */
typedef long                    DRM_LONG;               /* 4 bytes             2 bytes */

#ifdef __cplusplus
typedef wchar_t                 DRM_WCHAR_NATIVE;      /* 2 bytes             1 byte */
#else
typedef unsigned short          DRM_WCHAR_NATIVE;      /* 2 bytes             1 byte */
#endif

#if SIXTEEN_BIT_ADDRESSING
typedef unsigned long           DRM_UINT;               /*   n/a               2 bytes */
typedef long                    DRM_INT;                /*   n/a               2 bytes */
#else
typedef unsigned int            DRM_UINT;               /* 4 bytes               n/a   */
typedef int                     DRM_INT;                /* 4 bytes               n/a   */
#endif

#endif


/*
** +-----------------------------------+
** | MACRO DEFINITIONS                 |
** +-----------------------------------+
*/

/* ====================== Microsoft specific qualifiers ======================= */
/* ============================================================================ */
#if defined (_MSC_VER)

/* 
** Compile-time asserts cause PREfast warnings regarding the comparison of two constants.
** So, enable this macro only when the PREfast tool is not analyzing the code.
*/
#ifndef _PREFAST_ 

    /* Definition of the compile time assert. */
    #define DRMCASSERT( x ) switch(0){case 0:case (x):;}
     
    /* Assertion of the alignment of a member field within a structure. */
    #define DRMALIGNASSERT( struc, member ) DRMCASSERT( ( DRM_OFFSET_OF( struc, member ) ) % SIZEOF( DRM_WCHAR ) == 0 )

    /* Assertion of adjacency of two member fields within the same structure. */
    #define DRMADJASSERT( struc, member1, member2 ) DRMCASSERT( ENSURE_MEMBERS_ADJACENT( struc, member1, member2 ) )
    
#else
    /* Definition of the compile time assert. */
    #define DRMCASSERT( x )
    
    /* Assertion of the alignment of a member field within a structure. */
    #define DRMALIGNASSERT( struc, member ) DRMASSERT( ( DRM_OFFSET_OF( struc, member ) ) % SIZEOF( DRM_WCHAR ) == 0 )
    
    /* Assertion of adjacency of two member fields within the same structure. */
    #define DRMADJASSERT( struc, member1, member2 ) DRMASSERT( ENSURE_MEMBERS_ADJACENT( struc, member1, member2 ) )
    
#endif

#else

/* Definition of the compile time assert. */
#define DRMCASSERT( x ) switch(0){case 0:case (x):;}

/* Assertion of the alignment of a member field within a structure. */
#define DRMALIGNASSERT( struc, member ) DRMASSERT( ( DRM_OFFSET_OF( struc, member ) ) % SIZEOF( DRM_WCHAR ) == 0 )

/* Assertion of adjacency of two member fields within the same structure. */
#define DRMADJASSERT( struc, member1, member2 ) DRMASSERT( ENSURE_MEMBERS_ADJACENT( struc, member1, member2 ) )

#endif

#endif   /* __DRMCOMPILER_H__ */

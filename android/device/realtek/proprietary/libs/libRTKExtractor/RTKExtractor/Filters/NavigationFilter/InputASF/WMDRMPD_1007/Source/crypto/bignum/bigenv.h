/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef BIGENV_H              /* If not previously #included */
#define BIGENV_H 1

/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif


//     File bigenv.h   Version 16 November, 2000

//     This file looks at predefined macros.
//     It sets the flags
//
//          COMPILER        -- Identification of compiler or family of compilers
//          COMPILER_NAME   -- String with description of compiler.
//          EXPORTED_INLINE --.Used for local defintions of functions which are
//                            inlined on that file but available elsewhere.
//          EXTERN_INLINE   -- Used for extern declarations of functions which
//                             are inline on their definition file.
//          INLINING_SUPPORTED -- Does compiler support the _inline_ keyword?
//          TARGET          -- Identification of target architecture.
//          TARGET_NAME     -- String with description of target architecture
//          TARGET_OS       -- Identification of target operating system.

//     The COMPILER_NAME and TARGET_NAME strings should be used
//     only in printf-like contexts.
//     These are English-language descriptions even
//     in international versions of the software.


#define COMPILER_GCC 1
#define COMPILER_VC  2
#define COMPILER_ARM_ATTILA 3
#define COMPILER_TMS320C55X 4

#ifndef COMPILER
    #ifdef __GNUC__
        #define COMPILER COMPILER_GCC
                        /* GNU compiler */
    #endif
    #ifdef _MSC_VER
        #define COMPILER COMPILER_VC
    #endif              /* Microsoft Visual C compiler */

    #if defined(__arm) && !defined(COMPILER)
        #define COMPILER COMPILER_ARM_ATTILA
                        /* Native ARM compiler */
        #define TARGET TARGET_ARM
    #endif
    #ifdef __TMS320C55X__
        #define COMPILER COMPILER_TMS320C55X
        #define _ANSI 1
    #endif
#endif

/*
        Define a string with the compiler name,

        INLINING_SUPPORTED should be 1 if the compiler
        inlines procedures declared with the -inline-
        keyword (while deleting unused static procedures).
        If the compiler lacks these features,
        INLINING_SUPPORTED should be 0.
        These flags may vary with the version of the compiler.
*/


#if COMPILER == COMPILER_GCC
    #define COMPILER_NAME "GCC compiler"
    #define INLINING_SUPPORTED 1
    #define EXTERN_INLINE extern inline
    #define EXPORTED_INLINE inline
    #define FORCE_INLINE static inline
    #define INLINE static inline
#elif COMPILER == COMPILER_TMS320C55X
    #define COMPILER_NAME "Texas Instruments TMS320C55X compiler"
    #define INLINING_SUPPORTED 1
    #define EXTERN_INLINE extern
    #define EXPORTED_INLINE
    #define FORCE_INLINE inline
    #define INLINE inline
#elif COMPILER == COMPILER_VC
    #define COMPILER_NAME "Microsoft Visual C++ compiler"
    #if 1 /*defined(NDEBUG)*/    // Release mode
        #define INLINING_SUPPORTED 1
        #define inline __inline
    #else                  // Debug mode
        #define INLINING_SUPPORTED 0
        #define inline __inline
    #endif
    #define EXTERN_INLINE extern __inline
    #define EXPORTED_INLINE __inline
    #define FORCE_INLINE __forceinline
    #define INLINE __inline
#elif defined(_ANSI)
    #define COMPILER_NAME "ANSI C"
    #define EXTERN_INLINE extern
    #define EXPORTED_INLINE 
    #define INLINING_SUPPORTED 0
    #define INLINE
    #define FORCE_INLINE 
#endif

#if COMPILER == COMPILER_ARM_ATTILA
    #define COMPILER_NAME "ARM compiler for Attila"
#if 0
    // As of November, 2000, inlining increases code size 10%.
    // An elliptic curve test found less than 1% performance difference.

    // The compiler compiled vmul66 and vmul88 incorrectly with inlining.

    #define inline __inline
    #define INLINING_SUPPORTED 1
#else
    #define inline
    #define INLINING_SUPPORTED 0
#endif
    #define EXTERN_INLINE extern
    #define EXPORTED_INLINE  /* nothing */
    #define FORCE_INLINE 
#endif

#if    !defined(COMPILER) || COMPILER <= 0 \
    || !defined(COMPILER_NAME) || !defined(INLINING_SUPPORTED)
    #error -- "Unknown COMPILER"
#endif

/*
        Major Windows operating systems.
        Windows 95/98/Millennium are not supported.
*/

#define OS_WCE 1
           // Windows CE
#define OS_WNT 2
           // Windows NT or 2000 or XP or Palladium or Longhorn or ...
#define OS_ATTILA 3
           // Attila wireless phone -- no operating system (2000)
#define OS_ANSI 4
           // ANSI C


#if defined(_WIN32_WCE)
    #define TARGET_OS OS_WCE
#elif defined(WIN32)  // Includes 64-bit IA-64 and AMD-64
    #define TARGET_OS OS_WNT
#elif defined(__arm)
    #define TARGET_OS OS_ATTILA
#elif defined(_ANSI)
    #define TARGET_OS OS_ANSI
#else
    #error "Unknown OS target"
#endif
/*
        List of architectures on which code has been run.
        The inclusion of an architecture in this list does
        not necessarily imply that the present code
        will run on that architecture.
*/

#define TARGET_ALPHA 1
#define TARGET_AMD64 2
#define TARGET_ARM 3
#define TARGET_IA64 4
#define TARGET_IX86 5
#define TARGET_MIPS 6
#define TARGET_PPC 7
#define TARGET_SHX 8
#define TARGET_SPARC 9
#define TARGET_ANSI 10

#ifndef TARGET
    #ifdef _M_ALPHA     // DEC (Compaq) Alpha
        #define TARGET TARGET_ALPHA
    #endif
    #if defined(_ARM_)
        #define TARGET TARGET_ARM
    #endif
    #if defined(_M_IA64)      // Intel IA-64 (e.g., Merced, McKinley)
        #define TARGET TARGET_IA64
    #endif
    #if defined(_M_IX86) || defined(_x86)  // Intel X86 (e.g., 486, Pentium)
        #define TARGET TARGET_IX86
    #endif
    #if defined(_M_MRX000) || defined(_MIPS_) // MIPS 32-bit systems
        #define TARGET TARGET_MIPS
    #endif
    #if defined(_M_PPC)       // Motorola/Macintosh Power PC
        #define TARGET TARGET_PPC
    #endif
    #if defined(_SHX_)   // Hitachi SH-3 or SH-4
        #define TARGET TARGET_SHX
    #endif
    #if defined(__sparc__)    // Sun SPARC
        #define TARGET TARGET_SPARC
    #endif
    #if defined(_SH3_) || defined(_SH4_)   // Hitachi SH-3 or SH-4
        #define TARGET TARGET_SHX
    #endif
    #if defined(_AMD64_)
        #define TARGET TARGET_AMD64
    #endif
    #if defined(__TMS320C55X__)
        #define TARGET TARGET_ANSI
    #endif
#endif

#if TARGET == TARGET_ALPHA
#define TARGET_NAME "Compaq Alpha"
#endif

#if TARGET == TARGET_AMD64
#define TARGET_NAME "AMD x86-64"
#endif

#if TARGET == TARGET_ARM
#define TARGET_NAME "ARM"
#endif

#if TARGET == TARGET_IA64
#define TARGET_NAME "Intel IA-64"
#endif

#if TARGET == TARGET_IX86
#define TARGET_NAME "Intel Pentium"
#endif

#if TARGET == TARGET_MIPS || TARGET == TARGET_PPC
#define TARGET_NAME "Macintosh/Motorola PowerPC"
#endif

#if TARGET == TARGET_SHX
#define TARGET_NAME "Hitachi SHx"
#endif

#if TARGET == TARGET_SPARC
#define TARGET_NAME "Sun SPARC"
#endif

#if TARGET == TARGET_ANSI
#define TARGET_NAME "ANSI C"
#endif

#if !defined(TARGET) || TARGET <= 0 || !defined(TARGET_NAME)
    #error -- "Unknown TARGET"
#endif

#ifdef __cplusplus
}
#endif

#endif // BIGENV_H

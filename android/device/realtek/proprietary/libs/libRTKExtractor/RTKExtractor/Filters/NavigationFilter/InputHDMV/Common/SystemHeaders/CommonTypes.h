//-----------------------------------------------------------------------------
// CommonTypes.h
// Copyright (c) 1997 - 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef	__CommonTypes_h__
#define	__CommonTypes_h__

#if defined(_MSC_VER)

// sized types:
typedef signed __int8		SInt8;
typedef unsigned __int8		UInt8;
typedef signed __int16		SInt16;
typedef unsigned __int16	UInt16;
typedef signed __int32		SInt32;
typedef unsigned __int32	UInt32;
typedef signed __int64		SInt64;
typedef unsigned __int64	UInt64;

// function attributes:
#define __deprecated		__declspec(deprecated)

// variable attributes:
#define __threadlocal		__declspec(thread)

#elif defined(__GNUC__)

// sized types:
typedef signed char			SInt8;
typedef unsigned char		UInt8;
typedef signed short		SInt16;
typedef unsigned short		UInt16;
typedef signed long			SInt32;
typedef unsigned long		UInt32;
typedef signed long long	SInt64;
typedef unsigned long long	UInt64;

// function attributes:
#define __deprecated		__attribute__((deprecated))

// variable attributes:
#define __threadlocal		__thread

#else

#error Unknown compiler!!!

#endif

typedef SInt32 SonicError;

// common macros:

//#define __QUOTE(symbol) #symbol
//#define __TOKEN(symbol) __QUOTE(symbol)
//#define __WARNING(string) message(__FILE__ "(" __TOKEN(__LINE__) ") : WARNING : " string)

// DEBUG macros:
/*
#if defined(_WIN32) && defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUG) && defined(NDEBUG)
#error "DEBUG and NDEBUG are defined (mutually exclusive)"
#elif !defined(DEBUG) && !defined(NDEBUG)
#define NDEBUG
//#pragma __WARNING("DEBUG and NDEBUG are undefined - defining NDEBUG")
#endif

#ifdef DEBUG

#undef	DEBUG
#define DEBUG
#define	DEBUG_EMIT(a) a
#define NDEBUG_EMIT(a)

#else

#undef  NDEBUG
#define NDEBUG
#define DEBUG_EMIT(a)
#define NDEBUG_EMIT(a) a

#endif*/

#endif // __CommonTypes_h__

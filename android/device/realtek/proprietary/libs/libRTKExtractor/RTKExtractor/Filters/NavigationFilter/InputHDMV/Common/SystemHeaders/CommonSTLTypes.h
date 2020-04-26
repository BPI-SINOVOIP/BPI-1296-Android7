//-----------------------------------------------------------------------------
// CommonSTLTypes.h
// Copyright (c) 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef	__CommonSTLTypes_h__
#define	__CommonSTLTypes_h__

#ifdef __cplusplus

// unicode types

#include <string>

#if defined DONT_USE_BASIC_STRING
#define _XSTRING_
#include "String/SString.h"
#include "String/SString8.h"
#include "String/SString16.h"
#include "String/SString32.h"
#include "String/SStringW.h"
#endif

#if defined(_MSC_VER)
#if defined(_UNICODE) || defined(UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#endif

namespace unicode
{

#if defined(_MSC_VER)

	typedef UInt8 char8;
	typedef wchar_t char16;
	typedef UInt32 char32;

#elif defined(__GNUC__)

	typedef UInt8 char8;
	typedef UInt16 char16;
	typedef wchar_t char32;

#else

#error Unknown compiler!!!

#endif

#ifdef DONT_USE_BASIC_STRING

	typedef String8 string8;
	typedef String16 string16;
	typedef String32 string32;

#else

	typedef std::basic_string<char8, std::char_traits<char8>, std::allocator<char8> > string8;
	typedef std::basic_string<char16, std::char_traits<char16>, std::allocator<char16> > string16;
	typedef std::basic_string<char32, std::char_traits<char32>, std::allocator<char32> > string32;

#endif

}

#ifdef __GNUC__

namespace std
{

#if defined DONT_USE_BASIC_STRING

	typedef StringW wstring;

#else

	typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstring;

#endif

}

#endif

#endif // __cplusplus

#endif // __CommonSTLTypes_h__

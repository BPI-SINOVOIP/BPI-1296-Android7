///////////////////////////////////////////////////////////////////////////////
// UnicodeUtilities.h
// Copyright (c) 1999 - 2004, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

//SR FS: Reviewed [JAW 20040911]
//SR FS: Reviewed [JHL 20040924]

#ifndef __UnicodeUtilities_h__
#define __UnicodeUtilities_h__

#include "CommonTypes.h"
#include "CommonSTLTypes.h"

namespace unicode
{
	std::string to_string(const std::wstring & string);
	std::string to_string(const string8 & string);
	std::string to_string(const string16 & string);
	std::string to_string(const string32 & string);
	std::wstring to_wstring(const std::string & string);
	std::wstring to_wstring(const string8 & string);
	std::wstring to_wstring(const string16 & string);
	std::wstring to_wstring(const string32 & string);
	string8 to_string8(const std::string & string);
	string8 to_string8(const std::wstring & string);
	string8 to_string8(const string16 & string);
	string8 to_string8(const string32 & string);
	string16 to_string16(const std::string & string);
	string16 to_string16(const std::wstring & string);
	string16 to_string16(const string8 & string);
	string16 to_string16(const string32 & string);
	string32 to_string32(const std::string & string);
	string32 to_string32(const std::wstring & string);
	string32 to_string32(const string8 & string);
	string32 to_string32(const string16 & string);
	std::string to_string(const std::string& string);

#if defined(_MSC_VER)
	template<typename StringTypeT>
		tstring to_tstring(StringTypeT string)
	{
#if defined(_UNICODE) || defined(UNICODE)
		return to_wstring(string);
#else
		return to_string(string);
#endif
	}
#endif

	bool is_ascii(const std::string & string);
	string16 swap(const string16 & string);
	string32 swap(const string32 & string);
	string16 to_BE(const string16 & string);
	string32 to_BE(const string32 & string);
	string16 to_LE(const string16 & string);
	string32 to_LE(const string32 & string);

	std::string to_lower(const std::string & string);
	std::wstring to_lower(const std::wstring & string);
	string8 to_lower(const string8 & string);
	string16 to_lower(const string16 & string);
	string32 to_lower(const string32 & string);
	std::string to_upper(const std::string & string);
	std::wstring to_upper(const std::wstring & string);
	string8 to_upper(const string8 & string);
	string16 to_upper(const string16 & string);
	string32 to_upper(const string32 & string);
}

std::ostream& operator<<(std::ostream& ioStream, const unicode::string8& inString8);
std::ostream& operator<<(std::ostream& ioStream, const unicode::string16& inString16);
std::ostream& operator<<(std::ostream& ioStream, const unicode::string32& inString32);

#endif // __UnicodeUtilities_h__

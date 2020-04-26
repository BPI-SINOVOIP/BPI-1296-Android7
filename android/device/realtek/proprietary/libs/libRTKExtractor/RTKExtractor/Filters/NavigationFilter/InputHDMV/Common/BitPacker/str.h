//-----------------------------------------------------------
// str.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------

#ifndef _str_h_
#define _str_h_

#include <string>
#include "BitFieldArray.h"

namespace bitpacker {

class str : 
	public BitFieldArray
{
   typedef std::string string;
   typedef string::size_type size_type;

public:
	str();
	str(const str& a);
	str(const string& a);
	virtual ~str();

	str& operator=(const str& a);

	//	ctors from string
	str& operator=(const string &a);
	str& operator=(const char* a);
	str& operator=(char a);

	virtual void 
	addOffset(size_t& s) const;

	virtual void 
	bind(ConstBitPtr& p);

	virtual void 
	layoutArray(size_t base, size_t max, bool isFixed);

	virtual void
	layout(size_t)	//	for nonarray str objects
		{ layoutArray(0, 1, true); }

	virtual void 
	pack(BitPtr& p) const;

	virtual void
	read(rt_stream& is, size_t& offset);

	//	string method overrides
   void resize(size_type size);
   void resize(size_type size, const char value);
	const char& operator[](size_type index) const
		{ return mString[(mapIndex(index))]; }
	char&       operator[](size_type index)
		{ return mString[(mapIndex(index))]; }
	const char& at(size_type index) const
		{ return mString.at(mapIndex(index)); }
	char&       at(size_type index)
		{ return mString.at(mapIndex(index)); }

   const char *data() const {return mString.data();}
   size_type length() const{return mString.length();}
   size_type size() const{return mString.size();}
   const char *c_str() const {return mString.c_str();}
   str substr(size_type off = 0, size_type count = std::string::npos) const
      {return mString.substr(off, count);}
   str& replace(size_type pos, size_type num, size_type count, char ch) 
      {mString.replace(pos, num, count, ch); return *this;}
   size_type find_last_not_of(char ch, size_type off = std::string::npos) const
      {return mString.find_last_not_of(ch, off);}

   // converstion
   operator string() const {return mString;}

   // comparison to str
   bool operator==(const str& rhs) const {return rhs.mString == mString;}
   bool operator!=(const str& rhs) const {return mString != rhs.mString;}
   bool operator<(const str& rhs) const {return mString < rhs.mString;}
   bool operator<=(const str& rhs) const {return mString <= rhs.mString;}
   bool operator>(const str& rhs) const {return mString > rhs.mString;}
   bool operator>=(const str& rhs) const {return mString >= rhs.mString;}

   // comparison to string
   bool operator==(const string& rhs) const {return mString == rhs;}
   bool operator!=(const string& rhs) const {return mString != rhs;}
   bool operator<(const string& rhs) const {return mString < rhs;}
   bool operator<=(const string& rhs) const {return mString <= rhs;}
   bool operator>(const string& rhs) const {return mString > rhs;}
   bool operator>=(const string& rhs) const {return mString >= rhs;}

private:
   string mString;
};

rt_stream& operator << (rt_stream& os, const bitpacker::str& s);

// allow rhs comparison to std::string
bool operator==(const std::string& lhs, const str& rhs);
bool operator!=(const std::string& lhs, const str& rhs);
bool operator<(const std::string& lhs, const str& rhs);
bool operator<=(const std::string& lhs, const str& rhs);
bool operator>(const std::string& lhs, const str& rhs);
bool operator>=(const std::string& lhs, const str& rhs);

}

#endif // _str_h_



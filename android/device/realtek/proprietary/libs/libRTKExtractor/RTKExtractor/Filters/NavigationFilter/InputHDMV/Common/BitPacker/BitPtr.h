//-----------------------------------------------------------------------------
// BitPtr.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _BitPtr_h_
#define _BitPtr_h_

#include "CommonTypes.h"
#include "SystemHeaders_sal.h"

#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4267)
#endif

namespace bitpacker {

class BitPtr;

class ConstBitPtr {
public:
	inline static void byteAlign(size_t& bits)
	{ bits = (bits+7)&(~7); }

	inline static size_t toBytes(size_t bits)
	{ byteAlign(bits); return bits>>3; }

	inline static size_t remainder(size_t bits)
	{ return (8 - bits%8)%8; }

	ConstBitPtr();
	ConstBitPtr(const ConstBitPtr &rhs);
	ConstBitPtr(const BitPtr& rhs);
	ConstBitPtr(const char* start, int offset = 0);
	ConstBitPtr(const std::string& a, int offset = 0)
		: mStart(const_cast<char*const>(a.data())), mOffset(offset) {}
	virtual ~ConstBitPtr();

	ConstBitPtr& operator=(const ConstBitPtr &rhs);
	ConstBitPtr& operator=(const BitPtr &rhs);
	ConstBitPtr& operator=(const char* rhs);
	ConstBitPtr& operator=(const std::string& rhs);

// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
	ConstBitPtr operator+ (int offset) const;
	ConstBitPtr& operator+=(int offset);

// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
	int operator-(const ConstBitPtr &rhs);

	void round();

	operator const char*() const
	{ return mStart; }
	const char* start() const
	{ return mStart; }
	size_t offset() const
	{ return mOffset; }
	size_t remainder() const
	{ return remainder(mOffset); }

	operator bool() const
	{ return mStart != 0 && mOffset != 0; }

	void get(UInt32& value, size_t size, bool isSigned, bool isBigend) const;
	void get(SInt32& value, size_t size, bool isSigned, bool isBigend) const;
	void get(UInt64& value, size_t size, bool isSigned, bool isBigend) const;
	void get(SInt64& value, size_t size, bool isSigned, bool isBigend) const;

protected:
	char * const		mStart;
	const int			mOffset;
};

class BitPtr : public ConstBitPtr {
public:
	BitPtr();
	BitPtr(const BitPtr &rhs);
	BitPtr( /*__in*/ char* start, int offset = 0);
	BitPtr(std::string& str, int offset = 0)
		: ConstBitPtr(static_cast<const char*>(str.data()), offset) {}

	virtual ~BitPtr();

	BitPtr& operator=(const BitPtr &rhs);
	BitPtr& operator=( __in char* rhs);
	BitPtr& operator=(const ConstBitPtr &rhs);
	BitPtr& operator=(std::string &rhs);

// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
	BitPtr operator+ (int offset) const;
	BitPtr& operator+=(int offset);

// **CodeWizzard** - Violation:  Effective C++ item 19  - Differentiate among member functions, global functions and friend function
	int operator-(const BitPtr &rhs);

// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
	operator char*() const
// **CodeWizzard** - Severe Violation:  Effective C++ item 29  - Avoid returning "handles" to internal data from const member function
	{ return const_cast<char*>(mStart); }
// **CodeWizzard** - Violation:  Effective C++ item 37  - Never redefine an inherited nonvirtual functio
	char* start() const
// **CodeWizzard** - Severe Violation:  Effective C++ item 29  - Avoid returning "handles" to internal data from const member function
	{ return const_cast<char*>(mStart); }

	void fill();

	void set(UInt32 value, size_t size, bool isSigned, bool isBigend) const;
	void set(SInt32 value, size_t size, bool isSigned, bool isBigend) const;
	void set(UInt64 value, size_t size, bool isSigned, bool isBigend) const;
	void set(SInt64 value, size_t size, bool isSigned, bool isBigend) const;

private:
	explicit BitPtr(const ConstBitPtr& rhs);
};

}	//	namespace

#if defined(_WIN32)
#pragma warning(pop)
#endif

#endif // _BitPtr_h_



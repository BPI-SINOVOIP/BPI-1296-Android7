//-----------------------------------------------------------------------------
// BitField.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _BitField_h_
#define _BitField_h_

#include <string>
#include "rt_stream.h"
#include "BitPtr.h"

#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4267)
#endif

namespace bitpacker {

class BitField {
public:
	BitField() : mSize(0) {}
	BitField(const BitField& rhs) : mSize(rhs.mSize) {}

	virtual ~BitField() {}

	BitField& operator=(const BitField &rhs)
	{ if (this != &rhs) mSize = rhs.mSize; return *this; }

	//	---------------	Following functions implemented in subclasses
	//	bind: Make the BitField get/set its data at a particular
	//	memory location.
	virtual void 
	bind(ConstBitPtr& ptr)
	{ ptr += mSize; }

	//	size: Return the size in bits.
	virtual size_t
	bitSize() const	//	in bits
	{ return mSize; }

	//	addOffset: Accumulate the number of bits occupied by *this,
	//	including padding if any.
	virtual void 
	addOffset(size_t& s) const
	{ s += mSize; }

	//	bitOffset: Like addOffset, but quits if the given field 
	//	was found.  Used to accumulate the offset to a given field 
	//	within a larger structure.
	virtual bool
	bitOffset(size_t& offset, const BitField* field) const
	{
		if (this == field) return true;
		else { addOffset(offset); return false; }
	}

	//	print: Write the data to a stream in pretty format.
	virtual void 
	print(rt_stream& /*s*/, int /*indent*/) const
	{}

	//	pack: copy the data to the given memory location.
	virtual void 
	pack(BitPtr& ptr) const
	{ ptr += mSize; }

	//	layout: set the size of the BitField.  The size parameter
	//	is used only for scalar bit fields.
	//	It's required to be here so the templatized layout call in 
	//	bitpacker::allocator (in array.h) will work for both scalar
	//	and nonscalar bit fields.
	virtual void
	layout(size_t size = 0)
	{ mSize = size; }

	//	read: Set the data from the given rt_stream.  
	virtual void
	read(rt_stream& s, size_t& offset);

	//	---------------	Following functions not overridden
	static void Check(rt_stream& os, bool, const std::string&);

	//	write: Write the data to an rt_stream.  The data written is 
	//	implicitly extended to a byte boundary if necessary.
	void
	write(rt_stream& s) const;

	//	byteSize: Return the size of the data rounded to the next byte.
	//	A convenience function.
	size_t 
	byteSize() const
	{ return (bitSize()+7)/8; }

	rt_stream&
	leader(
		rt_stream& s, 
		int indent, 
		const std::string& label="") const;

	rt_stream&
	leader(
		rt_stream& s, 
		int indent, 
		const BitField& field,
		const std::string& label="") const;

	size_t
	byteOffset(const BitField* target) const
	{ size_t size = 0; bitOffset(size, target); return (size+7)/8; }

	size_t
	sectorOffset(const BitField* target) const
	{ size_t size = byteOffset(target); return (size+2047)/2048; }

	//	parameterless print for debugging
	void Print() const;

private:
	//static unsigned			sLabelWidth;
	//static std::string 		sTabString;

	size_t					mSize;	//	size in bits
};

rt_stream&
operator<<(rt_stream& s, const BitField& a);

// rt_stream support for fundamental types
const std::string newline = "\n";

rt_stream&
operator<<(rt_stream& s, const char* a);

rt_stream&
operator<<(rt_stream& s, const std::string& a);

rt_stream&
operator<<(rt_stream& s, const UInt32& a);

rt_stream&
operator<<(rt_stream& s, const SInt32& a);

rt_stream&
operator<<(rt_stream& s, const UInt64& a);

rt_stream&
operator<<(rt_stream& s, const SInt64& a);

rt_stream&
operator<<(rt_stream& s, const bool& a);
#if !defined (_WIN32)
rt_stream&
operator<<(rt_stream& s, const size_t& a);

#endif

}	//	namespace

#if defined(_WIN32)
#pragma warning(pop)
#endif

#endif


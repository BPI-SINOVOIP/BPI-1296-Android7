//-----------------------------------------------------------------------------
// BitPackerTypes.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _BitPackerTypes_h_
#define _BitPackerTypes_h_

#include "BitPacker.h"

namespace bitpacker {

typedef IntField<UInt32, false, true>	uint;
typedef IntField<SInt32, true, true>	sint;
typedef IntField<UInt32, false, false>	uint_dcba;
typedef IntField<SInt32, true, false>	sint_dcba;
typedef IntField<UInt64, false, true>	uint64;
typedef IntField<SInt64, true, true>	sint64;
typedef IntField<UInt64, false, false>	uint64_dcba;
typedef IntField<SInt64, true, false>	sint64_dcba;
typedef IntField<bool, false, true>		b00l;

typedef uint							bits;
typedef uint64							bits64;

//	----------------------------------- b00l

template <>
inline
void IntField<bool, false, true>::Set(bool value){
	mPtr.set(UInt32(value), bitSize(), false, true);
}

template <>
inline
void 
IntField<bool, false, true>::Get(bool& value) const{
	UInt32 intVal = 0;
	mPtr.get(intVal, bitSize(), false, true);
	value = intVal != 0;
}

template <>
inline IntField<bool, false, true>::IntField() {
	layout(1);
}


//	----------------------------------- BCDIntField

template <class IntType>
class BCDIntField : public IntField<IntType, false, true> {	//	always big endian, never signed
public:
	virtual void Set(IntType value)
	{
		IntType bcd = 0;
		if (value == 0xFF || value == 0x3F) // when value = 0xFF, 0x3F, not a valid bcd
		{
			bcd = value;
		}
		else
		{
			for (int i = 0; i < sizeof(IntType)*2; ++i){
				bcd = (bcd>>4) + ((value%10) << (sizeof(IntType)*8-4));
				value /= 10;
			}
		}
		IntField<IntType, false, true>::Set(bcd);
	}

	virtual void Get(IntType& value) const
	{
		IntType bcd = 0;
		IntField<IntType, false, true>::Get(bcd);
		if (bcd == 0xFF || bcd == 0x3F) // when value = 0xFF, 0x3F, not a valid bcd
		{
			value = bcd;   
		}
		else
		{
			value = 0;
			for (int i = 0; i < sizeof(IntType)*2; ++i){
				value = value*10 + (bcd >> (sizeof(IntType)*8-4));
				bcd <<= 4;
			}
		}
	}

	virtual void ConvertFromBCD(IntType& value)
	{
		if (value == 0xFF || value == 0x3F)   // when value = 0xFF, 0x3F, not a valid bcd
			return; 
		IntType bcd = value;
		value = 0;
		for (int i = 0; i < sizeof(IntType)*2; ++i){
			value = value*10 + (bcd >> (sizeof(IntType)*8-4));
			bcd <<= 4;
		}
	}

	virtual void ConvertToBCD(IntType& value) const
	{
		if (value == 0xFF || value == 0x3F)   // when value = 0xFF, 0x3F, not a valid bcd
			return; 

		IntType bcd = 0;
		for (int i = 0; i < sizeof(IntType)*2; ++i){
			bcd = (bcd>>4) + ((value%10) << (sizeof(IntType)*8-4));
			value /= 10;
		}
		value = bcd;
	}


	operator IntType() const { IntType value = 0; Get(value); return value; }

	BCDIntField& operator =(IntType value) { Set(value); return *this; }
};

typedef BCDIntField<UInt32>			bcd;
typedef BCDIntField<UInt64>			bcd64;

//	--------------- reserved
class reserved : public BitField {
public:
	virtual void
	pack(BitPtr& ptr) const;
};

//	--------------- max_size
//	XXX this guy needs some work....
class max_size : public BitField {
public:
	max_size() : mMax(0) {}
	~max_size() {}
	max_size(const max_size& a) : BitField(a), mMax(a.mMax) {}

	virtual void
	layout(size_t size)
	{ mMax = size; BitField::layout(0); }

	virtual void 
	addOffset(size_t& s) const
	{
		if (s > mMax)
			assert(0);
			//throw std::range_error("");
	}

private:
	size_t mMax;
};


//	--------------- align
class align : public reserved {
public:
	align() : mAlignment(0) {}
	align(const align& a) : reserved(), mAlignment(a.mAlignment) {}
	virtual ~align() {}

	align& operator=(const align& a) { 
		if (this != &a){
			reserved::operator=(a);
			mAlignment = a.mAlignment;
		}
		return *this; 
	}

	size_t
	alignment() const
	{ return mAlignment; }

	virtual void
	layout(size_t alignment)
	{ mAlignment = alignment; }

	virtual void 
	setSize(size_t offset)
	{ BitField::layout((mAlignment - offset%mAlignment)%mAlignment); }

private:
	size_t mAlignment;
};

//	--------------- pad_to
class pad_to : public align {
public:
	size_t
	max_size() const
	{ return alignment(); }

	virtual void 
	setSize(size_t offset)
	{ BitField::layout(offset < max_size() ? max_size()-offset : 0); }
};

}

rt_stream&
operator << (rt_stream& s, const bitpacker::reserved& a);

rt_stream&
operator << (rt_stream& s, const bitpacker::max_size& a);

rt_stream&
operator << (rt_stream& s, const bitpacker::align& a);

rt_stream&
operator << (rt_stream& s, const bitpacker::pad_to& a);

#endif

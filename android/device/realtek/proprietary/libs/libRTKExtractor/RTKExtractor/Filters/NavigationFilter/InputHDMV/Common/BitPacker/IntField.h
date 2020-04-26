//-----------------------------------------------------------------------------
// IntField.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// IntField.h
// Copyright (c) 2003, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _IntField_h_
#define _IntField_h_

#include "ScalarBitField.h"

#if defined(_WIN32)
#pragma warning(disable:4100) // unreferenced formal parameter
#endif

namespace bitpacker {

template <class Target, bool IsSigned, bool IsBigend>
class IntField : public ScalarBitField {
public:
	IntField() { setValue(0); }
	IntField(const IntField& a) : ScalarBitField(a) { 
		Target t; a.Get(t); Set(t); 
	}
	IntField& operator=(const IntField& a) {
		if (&a != this){
			ScalarBitField::operator=(a);
			Target t; a.Get(t); Set(t);
		}
		return *this;
	}

	virtual void 
	print(rt_stream& s, int indent) const { 
		leader(s, indent);
		Target value = 0;
		Get(value);
		s << value;
	}

	virtual void 
	pack(bitpacker::BitPtr& p) const { 
		IntField<Target, IsSigned, IsBigend> tmp;
		tmp.layout(bitSize());
		tmp.bind(p);	//	increments p
		Target value = 0;
		Get(value);
		ConvertToBCD(value);
		tmp.Set(value);
	}

	virtual void 
	unpack(ConstBitPtr& p) { 
		IntField<Target, IsSigned, IsBigend> tmp;
		tmp.layout(bitSize());
		tmp.bind(p);	//	increments p
		Target value = 0;
		tmp.Get(value);
		ConvertFromBCD(value);
		Set(value);
	}

	operator Target() const { Target value; Get(value); return value; }
	IntField& operator =(Target value) { Set(value); return *this; }

protected:
	virtual void Set(Target value) { 
		setValue(value);
		mPtr.set(value, bitSize(), IsSigned, IsBigend); 
	}
	virtual void Get(Target& value) const { 
		mPtr.get(value, bitSize(), IsSigned, IsBigend); 
		setValue(value);
	}
	virtual void ConvertFromBCD(Target& value)
	{
	}

	virtual void ConvertToBCD (Target& value) const
	{
	}

private:
#if defined(DEBUG) || defined(_DEBUG)
	mutable Target mValue;
#endif
	inline void setValue(Target value) const {
#if defined(DEBUG) || defined(_DEBUG)
		mValue = value;
#endif
	}
};

template <class Target, bool IsSigned, bool IsBigend>
rt_stream&
operator<<(rt_stream& s, const IntField<Target, IsSigned, IsBigend>& a) { 
	a.print(s, 0);
	return s;
}

}	//	namespace

//#include "IntField.cpp"

#endif


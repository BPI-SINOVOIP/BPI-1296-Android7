//-----------------------------------------------------------------------------
// ScalarBitField.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _ScalarBitField_h_
#define _ScalarBitField_h_

#include "BitField.h"
//#include "CommonTypes.h"	//	needed by safe_ptr.h
//#include "safe_ptr.h"

#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4267)	//	loss of data converting size_t to int
#endif

namespace bitpacker {

class ScalarBitField : public BitField {
public:
	ScalarBitField();
	ScalarBitField(const ScalarBitField& a);
	virtual ~ScalarBitField();

	ScalarBitField& 
	operator=(const ScalarBitField &rhs);

	virtual void 
	bind(ConstBitPtr& ptr)
	{ mPtr = ptr; ptr += bitSize(); }

	//	unpack: copy the data from the given memory location.
	virtual	void 
	unpack(ConstBitPtr& ptr)
	{ ptr += bitSize(); }

	virtual void
	layout(size_t = 0);

	virtual void
	read(rt_stream& is, size_t& offset);

protected:
	enum { kStorageSize = 8 };

	void copyData(const ScalarBitField& a);

	BitPtr					mPtr;
	char					mStorage[kStorageSize];
};

}	//	namespace

#if defined(_WIN32)
#pragma warning(pop)
#endif

#endif


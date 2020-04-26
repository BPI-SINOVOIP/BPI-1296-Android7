//-----------------------------------------------------------------------------
// AggregateBitField.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _AggregateBitField_h_
#define _AggregateBitField_h_

#include <iosfwd>
#include "BitField.h"

namespace bitpacker {

class AggregateBitField : public BitField {
public:
   static void Check(rt_stream& os, bool, const std::string&);

	AggregateBitField() : mParent(0) {}

	//	Get/set the parent
	virtual AggregateBitField* parent() const { return mParent; }

	virtual void parent(const AggregateBitField* dad) 
		{ mParent = const_cast<AggregateBitField*>(dad); }

	//	size of the field in bits.
	virtual size_t
	bitSize() const;

private:
	mutable AggregateBitField*	mParent;
};

}	//	namespace

#endif


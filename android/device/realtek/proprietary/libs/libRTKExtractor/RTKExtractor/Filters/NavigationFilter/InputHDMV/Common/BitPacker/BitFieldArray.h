//-----------------------------------------------------------------------------
// BitFieldArray.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _BitFieldArray_h_
#define _BitFieldArray_h_

#include <new>
#include "AggregateBitField.h"

namespace bitpacker {

class BitPacker;

class BitFieldArray : public AggregateBitField {
public:
	BitFieldArray();
	BitFieldArray(const BitFieldArray& a);

	BitFieldArray&
	operator = (const BitFieldArray& a);

	void
	hookup(BitPacker* parent, size_t fieldID);

	void 
	layoutArray(size_t base, size_t max, bool isFixed);

	size_t mapIndex(size_t index) const
	{ return index-mBase; }

	void checkResize(size_t oldSize, size_t newSize) const;

	void resizeEvent(size_t newSize) const;

protected:
	size_t			mBase;
	size_t			mMax;
	bool			mIsFixed;
   BitPacker*	mParent;
	size_t			mFieldID;

	void dieRange() const;
	void dieFixed() const;
};

}

#endif // _BitFieldArray_h_


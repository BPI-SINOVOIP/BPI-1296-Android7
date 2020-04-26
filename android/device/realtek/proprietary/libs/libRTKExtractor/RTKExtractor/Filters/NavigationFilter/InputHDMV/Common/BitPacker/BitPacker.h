//------------------------------------------------------------
// BitPacker.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//------------------------------------------------------------

#ifndef _BitPacker_h_
#define _BitPacker_h_

#include "BitField.h"
#include "ScalarBitField.h"
#include "AggregateBitField.h"
#include "IntField.h"
#include "BitPackerTypes.h"
#include "array.h"
#include "str.h"

#ifndef BITPACKER_ENABLE_PRINT
#define BITPACKER_ENABLE_PRINT
#endif

namespace bitpacker {

class BitPacker : 
	public AggregateBitField
{
public:
	virtual void init() = 0;
	virtual void validate(rt_stream&) const = 0;
   virtual void resizeEvent(size_t selector, size_t newSize) = 0;
};

} 	//	namespace

#endif


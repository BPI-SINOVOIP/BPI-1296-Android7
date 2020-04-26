//-----------------------------------------------------------------------------
// BitPackingEngine.h
// Copyright (c) 1997 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
#ifndef __BitPackingEngine_h__
#define __BitPackingEngine_h__

#include <cstddef>

namespace bitpacker
{

#include "CommonTypes.h"
#include "SystemHeaders_sal.h"

UInt32	BitEngineGet32(const char* buf, size_t offset, size_t len, 
		bool isSigned = false, bool Big_Endian = true);
void	BitEngineSet32(__out_bcount_nz(4) char* buf, size_t offset, size_t len, UInt32 value,
		bool isSigned = false, bool Big_Endian = true);
UInt64	BitEngineGet64(const char* buf, size_t offset, size_t len, 
		bool isSigned = false, bool Big_Endian = true);
void	BitEngineSet64(__out_bcount_nz(8) char* buf, size_t offset, size_t len, UInt64 value, 
		bool isSigned = false, bool Big_Endian = true);

}
#endif


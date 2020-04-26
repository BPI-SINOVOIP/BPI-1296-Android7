//-----------------------------------------------------------------------------
// SytemHeaders_sal.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------


#ifndef __SytemHeaders_sal__
#define __SytemHeaders_sal__

#ifdef USE_SAL

#include <sal.h>

#elif defined(_MSC_VER) && _MSC_VER >= 1400

#include <sal.h>

#else

// Taken from specstrings.h (VCNet 2003 SP2)


//#define __null	//REALTEK_MODIFY
#define __notnull
#define __maybenull
#define __reserved
#define __readonly
#define __notreadonly
#define __maybereadonly
#define __valid
#define __novalid
#define __maybevalid
#define __checkReturn
#define __readableTo(size)
#define __writableTo(size)
#define __typefix(ctype)        
#define __deref
#define __pre
#define __post
#define __exceptthat
#define __override
#define __fallthrough
#define __callback
#define __in
#define __in_opt
#define __out
#define __out_opt
#define __inout
#define __inout_opt
#define __opt
#define __out_ecount(size)
#define __out_ecount_opt(size)
#define __out_ecount_nz(size)
#define __out_ecount_nz_opt(size)
#define __out_ecount_z(size)
#define __out_ecount_z_opt(size)
#define __in_ecount(size)
#define __in_ecount_opt(size)
#define __in_ecount_nz(size)
#define __in_ecount_nz_opt(size)
#define __in_ecount_z(size)
#define __in_ecount_z_opt(size)
#define __inout_ecount(size)
#define __inout_ecount_opt(size)
#define __inout_ecount_nz(size)
#define __inout_ecount_nz_opt(size)
#define __inout_ecount_z(size)
#define __inout_ecount_z_opt(size)
#define __out_bcount(size)
#define __out_bcount_opt(size)
#define __out_bcount_nz(size)
#define __out_bcount_z(size)
#define __in_bcount(size)
#define __in_bcount_opt(size)
#define __in_bcount_nz(size)
#define __in_bcount_nz_opt(size)
#define __in_bcount_z(size)
#define __in_bcount_z_opt(size)
#define __inout_bcount(size)
#define __inout_bcount_opt(size)
#define __inout_bcount_nz(size)
#define __inout_bcount_nz_opt(size)
#define __inout_bcount_z(size)
#define __inout_bcount_z_opt(size)
#define __format_string

#endif // USE_SAL

#endif //__SytemHeaders_sal__

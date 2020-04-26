//-----------------------------------------------------------------------------
// md_stream.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

//SR FS: Reviewed [DDT 20040916] 
//SR FS: Reviewed [wwt 20040929] - second pass

#ifndef __md_stream_h__
#define __md_stream_h__

#include <queue>
#include "ref_ptr.h"
#include "safe_ptr.h"
#include "rt_stream.h"

typedef SInt32 SonicError;

//
// Base class for metadata objects to be posted to an rt_stream
//
class rt_stream_metadata {
public:
	explicit rt_stream_metadata(UInt64 pos = 0) : mPos(pos) {}
	virtual ~rt_stream_metadata();

	virtual UInt64 get_pos() const;

	virtual rt_stream_metadata* copy() const = 0;

private:
	UInt64	mPos;
};

//
// Standard metadata types
//

//
// Timestamp
//
class rt_stream_timestamp : public rt_stream_metadata {
	typedef rt_stream_metadata INHERITED;
public:
	explicit rt_stream_timestamp(UInt64 pos, double time) :	INHERITED(pos), mTime(time) {}
// **CodeWizzard** - Violation:  More Effective C++ item 24  - Understand the costs of virtual functions ...
	virtual ~rt_stream_timestamp() {}

	double get_time() const { return mTime; }

// **CodeWizzard** - Violation:  More Effective C++ item 24  - Understand the costs of virtual functions ...
	virtual rt_stream_metadata* copy() const { return new rt_stream_timestamp(*this); }

private:
	double	mTime;
};

//
// End Of Stream
//
class rt_stream_end_of_stream : public rt_stream_metadata {
	typedef rt_stream_metadata INHERITED;
public:
	rt_stream_end_of_stream() {}
	virtual ~rt_stream_end_of_stream();

	virtual rt_stream_metadata* copy() const;

private:
};

class md_stream {
public:
	explicit md_stream(rt_stream_metadata_state);
	virtual ~md_stream();

	virtual UInt64		next_pos() const;

	virtual void		push(const rt_stream_metadata&);
	virtual safe_ptr<rt_stream_metadata> pop();
	virtual std::size_t		size() const;

private:
	md_stream(const md_stream&);	// No copying
	md_stream& operator=(const md_stream&);	// No assignment

	// metadata posted on an rt_stream is always held with a safe_ptr to avoid
	//	memory leaks and unnecessary copying as it passes through the stream
	typedef safe_ptr<rt_stream_metadata> rt_stream_metadata_ptr;

	typedef std::queue<rt_stream_metadata_ptr>	md_queue;

	rt_stream_metadata_state					md_metadata_state;
	md_queue									md_metadata_queue;
};

typedef ref_ptr<md_stream> md_stream_ref;

#endif

//-----------------------------------------------------------------------------
// rt_stream.h
// Copyright (c) 1997 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

//SR FS: Reviewed [DDT 20040916] 
//SR FS: Reviewed [wwt 20040929] - second pass

#ifndef __rt_stream_h__
#define __rt_stream_h__

#include "ref_ptr.h"
#include <vector>

class md_stream;
class SonicException;

enum rt_stream_metadata_state
{
	no_metadata				= 0,
	accepts_metadata		= 1,
	provides_metadata		= 2,
	passes_metadata			= 3
};


enum rt_preferred_xfr
{
	RT_PREFERS_STREAM		= 0,
	RT_PREFERS_POINTER		= 1,
	RT_INDIFFERENT			= 2
};

//SR SC: D [JSB 20040929] header_sizes, block_sizes, and trailer_sizes should be unsigned integers
//SR DI: DP4 [JSB 20040929] {dread_score: 4} 
//SR MS: D [ZLF 20041015];
class rt_stream
{

public:
	rt_stream();
	rt_stream(const rt_stream&);
	virtual ~rt_stream();
	const rt_stream& operator=(const rt_stream&);

	SonicError get_err();

	virtual SonicError set_read_pos(UInt64 pos);
	virtual void get_read_pos(UInt64 *pos) const;
	virtual SonicError read_from_pos(UInt64 pos, __out_bcount_nz(nbytes) char *data, size_t nbytes, size_t *bytes_read);
	virtual SonicError advance_read_pos(SInt64 nbytes);
	virtual SonicError read( __out_bcount_nz(nbytes) char *data, size_t nbytes, size_t *bytes_read);
	virtual SonicError read_to_stream(rt_stream *st, size_t nbytes, size_t *bytes_read);

	virtual SonicError set_write_pos(UInt64 pos);
	virtual void get_write_pos(UInt64 *pos) const;

	virtual SonicError write_to_pos(UInt64 pos, const char *data, size_t nbytes, size_t *bytes_written);
	virtual SonicError advance_write_pos(SInt64 nbytes);
	virtual SonicError write(const char *data, size_t nbytes, size_t *bytes_written);
	virtual SonicError write_from_stream(rt_stream *st, size_t nbytes, size_t *bytes_written);
	virtual SonicError write_zeroes(size_t nbytes, size_t *bytes_written);

	virtual SonicError compare_to_pos(UInt64 pos, const char *data, size_t nbytes, size_t *bytes_compared);

	virtual void reset();
	virtual UInt64 free_bytes() const;
	virtual UInt64 content_bytes() const;
	virtual rt_preferred_xfr preferred_xfr() const;

	virtual SonicError byte_pos_to_write_addrs(UInt64 pos, size_t *nbytes, __out char **xfr1_adr, size_t *xfr1_bytes, __out char **xfr2_adr, size_t *xfr2_bytes);
	virtual SonicError get_write_addrs(size_t *nbytes, __out char **xfr_adr);
	virtual SonicError get_write_addrs(size_t *nbytes, __out char **xfr1_adr, size_t *xfr1_bytes, __out char **xfr2_adr, size_t *xfr2_bytes);
	virtual SonicError byte_pos_to_read_addrs(UInt64 pos, size_t *nbytes, __out char ** xfr1_adr, size_t *xfr1_bytes, __out char ** xfr2_adr, size_t *xfr2_bytes) const;
	virtual SonicError get_read_addrs(size_t *nbytes, __out char ** xfr_adr) const;
	virtual SonicError get_read_addrs(size_t *nbytes, __out char ** xfr1_adr, size_t *xfr1_bytes, __out char ** xfr2_adr, size_t *xfr2_bytes) const;

	virtual SonicError flush_read_buffer();
	virtual SonicError flush_write_buffer();

	// convenience methods
	inline UInt64 get_read_pos() const { UInt64 read_pos; get_read_pos(&read_pos); return read_pos; }
	inline UInt64 get_write_pos() const { UInt64 read_pos; get_write_pos(&read_pos); return read_pos; }
	inline SonicError read_from_pos(UInt64 pos, __out_bcount_nz(nbytes) char *data, size_t nbytes)
		{ size_t actual_bytes; return read_from_pos(pos, data, nbytes, &actual_bytes); }
	inline SonicError read(__out_bcount_nz(nbytes) char *data, size_t nbytes)
		{ size_t actual_bytes; return read(data, nbytes, &actual_bytes); }
	inline SonicError read_to_stream(rt_stream *st, size_t nbytes)
		{ size_t actual_bytes; return read_to_stream(st, nbytes, &actual_bytes); }
	inline SonicError write_to_pos(UInt64 pos, const char *data, size_t nbytes)
		{ size_t actual_bytes; return write_to_pos(pos, data, nbytes, &actual_bytes); }
	inline SonicError write(const char *data, size_t nbytes)
		{ size_t actual_bytes; return write(data, nbytes, &actual_bytes); }
	inline SonicError write_from_stream(rt_stream *st, size_t nbytes)
		{ size_t actual_bytes; return write_from_stream(st, nbytes, &actual_bytes); }
	inline SonicError write_zeroes(size_t nbytes)
		{ size_t actual_bytes; return write_zeroes(nbytes, &actual_bytes); }

	virtual	UInt64	get_min_read_amount();
	virtual	UInt64	get_min_write_amount();

	// metadata support
	virtual SonicError enable_metadata(rt_stream_metadata_state);
	virtual bool has_metadata() const;
	virtual ref_ptr<md_stream> get_metadata_stream();

	// exception support
	SonicException get_exception();
	static SonicException get_exception(SonicError error);

	static SonicError eof_err;
	static SonicError not_supported;

protected:

	mutable SonicError rt_err; // all error returns are stored here

private:

	ref_ptr<md_stream> metadata_stream;
};

typedef ref_ptr<rt_stream> rt_stream_ref;
typedef std::vector<rt_stream_ref> rt_stream_vector;

#endif /* __rt_stream_h__ */


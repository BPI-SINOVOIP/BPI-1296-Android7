//-----------------------------------------------------------------------------
// ring_rt_stream.h
// Copyright (c) 1997 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

//SR FS: Reviewed [DDT 20040916] 
//SR FS: Reviewed [wwt 20040929] - second pass

#ifndef ring_rt_streamring_rt_streamring_rt_stream
#define ring_rt_streamring_rt_streamring_rt_stream

#include "rt_stream.h"

class ring_rt_stream : public rt_stream {

	public:
// **CodeWizzard** - Informational:  More Effective C++ item 5  - Be wary of user-defined conversion function
		ring_rt_stream(size_t nbytes);
		ring_rt_stream( __out_bcount_nz(nbytes) char *buffer, size_t nbytes);
		virtual ~ring_rt_stream();

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

		virtual void reset();
		virtual UInt64 free_bytes() const;
		virtual UInt64 content_bytes() const;
		virtual rt_preferred_xfr preferred_xfr() const;

		size_t	buffer_size() const;

		virtual SonicError byte_pos_to_write_addrs(
						UInt64 pos,
						size_t *nbytes,
						__out char **xfr1_adr,
						size_t *xfr1_bytes,
						__out char **xfr2_adr,
						size_t *xfr2_bytes);

		virtual SonicError get_write_addrs(
						size_t *nbytes,
						__out char **xfr_adr);

		virtual SonicError get_write_addrs(
						size_t *nbytes,
						__out char **xfr1_adr,
						size_t *xfr1_bytes,
						__out char **xfr2_adr,
						size_t *xfr2_bytes);

		virtual SonicError byte_pos_to_read_addrs(
						UInt64 pos,
						size_t *nbytes,
						__out char ** xfr1_adr,
						size_t *xfr1_bytes,
						__out char ** xfr2_adr,
						size_t *xfr2_bytes) const;

		virtual SonicError get_read_addrs(size_t *nbytes,
						__out char ** xfr_adr) const;

		virtual SonicError get_read_addrs(size_t *nbytes,
						__out char ** xfr1_adr,
						size_t *xfr1_bytes,
						__out char ** xfr2_adr,
						size_t *xfr2_bytes) const;

		ptrdiff_t	get_pos_offset() const;
		ptrdiff_t	set_pos_offset(ptrdiff_t pos_offset);
		ptrdiff_t	set_buf_len_mod(size_t modulo_size);

		enum rt_io_flavor{
			rt_io_is_output 		= 1,
			rt_io_is_input			= 2,
			rt_io_is_input_output 	= 3
		};
private:
		ring_rt_stream(const ring_rt_stream&);
		ring_rt_stream& operator=(const ring_rt_stream&);
		char *rt_buf_ptr;
  		size_t rt_buf_len;
 		size_t rt_allocated_buf_len;
 		size_t rt_buf_w_index;
  		size_t rt_buf_r_index;
		UInt64 rt_write_pos;
 		UInt64 rt_read_pos;
  		ptrdiff_t rt_pos_offset;
  		bool rt_self_allocated;
};


#endif

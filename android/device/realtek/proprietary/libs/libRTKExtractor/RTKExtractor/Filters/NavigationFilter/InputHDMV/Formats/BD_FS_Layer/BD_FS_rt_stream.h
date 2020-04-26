//-----------------------------------------------------------------------------
// BD_FS_rt_stream.h
// Copyright (c) 2005-2007, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BD_FS_RT_STREAM_H__
#define __BD_FS_RT_STREAM_H__

//#include <iostream>
#include <string>

#include "rt_stream.h"
#include "FilePath.h"

struct BD_FS_DateTime
{
  UInt16      year;               // Year as an integer
  UInt8       month;              // Month from 1(Jan) to 12(Dec)
  UInt8       day;                // Day of month from 1 to 31

  UInt8       hour;               // Hour from 0 to 23
  UInt8       minute;             // Minute from 0 to 59
  UInt8       second;             // Second from 0 to 59
  SInt8       zone;               // Time offset from GMT in 30 minute increments
};

class BD_FS_rt_stream : public rt_stream
{
public : 
	BD_FS_rt_stream() {}
	virtual ~BD_FS_rt_stream() {}

	virtual bool IsValid(void) = 0;

	// File Properties
	//
	virtual SonicError get_file_size(UInt64 *fileSize) = 0;								// file size
	virtual SonicError get_file_timestamp_created(BD_FS_DateTime & timestamp) = 0;		// file creation date and time
	virtual SonicError get_file_timestamp_modified(BD_FS_DateTime & timestamp) = 0;		// file modification data and time

	// Read
	//
	virtual SonicError read(__out_bcount_nz(nbytes) char *data, size_t nbytes, size_t *bytes_read) = 0;
	virtual SonicError read_from_pos(UInt64 pos, __out_bcount_nz(nbytes) char *data, size_t nbytes, size_t *bytes_read) = 0;

	// Write
	//
	virtual SonicError write(__in_bcount_nz(nbytes) const char *data, size_t nbytes, size_t *bytes_written) = 0;
	virtual SonicError write_to_pos(UInt64 pos, __in_bcount_nz(nbytes) const char *data, size_t nbytes, size_t *bytes_written) = 0;

	// Current Position control
	//
	virtual SonicError get_current_pos(UInt64 &pos) = 0;		// gets current read/write position
	virtual SonicError advance_read_pos(SInt64 pos) = 0;		// advances current position by 'pos'
	virtual SonicError set_read_pos(UInt64 pos) = 0;			// set current read location to 'pos'
	virtual SonicError seek_pos(UInt64 pos) = 0;				// seeks to location 'pos'.
};

#endif //__BD_FS_RT_STREAM_H__

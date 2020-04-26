//-----------------------------------------------------------------------------
// BD_FS_rt_stream_Linux.h
// Copyright (c) 2005-2007, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef BD_FS_RT_STREAM_LINUX_H
#define BD_FS_RT_STREAM_LINUX_H

#include "BD_FS_rt_stream.h"



class BD_Linux_rt_stream : public BD_FS_rt_stream
{
public :
	//BD_Linux_rt_stream();
	BD_Linux_rt_stream(char* filePath, bool bTryDirectIOFirst = false);
	~BD_Linux_rt_stream();

	virtual bool IsValid(void);

	// File Properties
	//
	virtual SonicError get_file_size(UInt64 *fileSize);								// file size
	virtual SonicError get_file_timestamp_created(BD_FS_DateTime & timestamp);		// file creation date and time
	virtual SonicError get_file_timestamp_modified(BD_FS_DateTime & timestamp);		// file modification data and time

	// Read
	//
	virtual SonicError read(__out_bcount_nz(nbytes) char *data, size_t nbytes, size_t *bytes_read);
	virtual SonicError read_from_pos(UInt64 pos, __out_bcount_nz(nbytes) char *data, size_t nbytes, size_t *bytes_read);

	// Write
	//
	virtual SonicError write(__in_bcount_nz(nbytes) const char *data, size_t nbytes, size_t *bytes_written);
	virtual SonicError write_to_pos(UInt64 pos, __in_bcount_nz(nbytes) const char *data, size_t nbytes, size_t *bytes_written);

	// Current Position control
	//
	virtual SonicError get_current_pos(UInt64 &pos);		// gets current read/write position
	virtual SonicError advance_read_pos(SInt64 pos);		// advances current position by 'pos'
	virtual SonicError set_read_pos(UInt64 pos);			// set current read location to 'pos'
	virtual SonicError seek_pos(UInt64 pos);				// seeks to location 'pos'.
	
	void set_buffer(unsigned char* pBuffer, int bufSize);
		
protected:
	int					m_Stream;
	UInt64				m_CurrentPos;
	int					m_phyPosOffset;
	unsigned char* 		m_pBuffer;
	int 				m_bufSize;
	int					m_residueBytes;
	char* 				m_pResidueByteAddr;
	bool 				m_bUseDirectIO;
};

#endif //BD_FS_RT_STREAM_LINUX_H

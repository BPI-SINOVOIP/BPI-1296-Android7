//-----------------------------------------------------------------------------
// BDROM_PlayList_PipMetaData.h
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _BDROM_PlayList_PipMetaData_H
#define _BDROM_PlayList_PipMetaData_H

#include "SystemHeaders.h"

namespace BDROM
{

struct metadata_block_header
{
	UInt16		mref_to_PlayItem_id;
	UInt8		mref_to_secondary_video_stream_id;
	UInt8		mpip_timeline_type;
	bool		mis_luma_key;
	bool		mtrick_playing_flag;
	UInt8		mupper_limit_luma_key;				// valid only if is_luma_key is 1
	UInt32		mmetadata_block_data_start_address;
};

struct pip_composition_metadata
{
	UInt16		mpip_horizontal_position;
	UInt16		mpip_vertical_position;
	UInt8		mpip_scale;
};

struct pip_metadata_entry
{
	UInt32						mpip_metadata_time_stamp;
	pip_composition_metadata	mpip_composition_metadata;
};

struct metadata_block_data
{
	metadata_block_data();
	~metadata_block_data();

	bool Init(UInt16 num_entries);

	UInt16						mnumber_of_pip_metadata_entries;
	pip_metadata_entry	*		mpip_metadata_entries;
};

class pip_metadata
{
public:
	UInt32		mlength;
	UInt16		mnumber_of_metadata_block_entries;
	
	metadata_block_header	*	mmetadata_block_headers;
	metadata_block_data		*	mmetadata_block_datas;

	pip_metadata(void);

	void Init(UInt8 * data_block);		// pass 0 to clear/deinit

protected:
	void Deinit(void);
	void FreeMetadataMemory(void);
	bool AllocMetadataMemory(void);
};

}	// namespace

#endif //_BDROM_PlayList_PipMetaData_H

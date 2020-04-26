//---------------------------------------------------------------------------
// EncryptedMetaData_rt_stream.h
//
// Copyright (c) 2008, Sonic Solutions.  All rights reserved.
//
// rt_stream metadata stream wrapper for EncryptedMetaData class.
//---------------------------------------------------------------------------

#ifndef __ENCRYPTEDMETADATA_RT_STREAM_H__
#define __ENCRYPTEDMETADATA_RT_STREAM_H__

#include "rt_stream.h"
#include "IEncryptedStreamControlTypes.h"

// The NavMetaData classes and structs are going to be phased out.
#include "NavMetadata.h"

class EncryptedMetaData_rt_stream : public rt_stream_metadata
{
	typedef rt_stream_metadata INHERITED;

public:
	EncryptedMetaData_rt_stream(const EncryptedMetaData& data) : mData(data) {}

	EncryptedMetaData_rt_stream(const NavMetaData::MetaData& data)
	{
		ConvertMetaData(data, &mData);
	}

	EncryptedMetaData_rt_stream(const NavMetaData::HDNav_rt_stream_metadata& data)
	{
		ConvertHDNav_rt_stream_metadata(data, &mData); 
	}

	virtual ~EncryptedMetaData_rt_stream() { }

	const EncryptedMetaData& GetData() const { return mData; }

	virtual rt_stream_metadata* copy() const
	{
		return new EncryptedMetaData_rt_stream(*this);
	}

	bool ConvertHDNav_rt_stream_metadata(const NavMetaData::HDNav_rt_stream_metadata& from, EncryptedMetaData* to)
	{
		assert(to);

		NavMetaData::MetaData md = from.GetData();

		return ConvertMetaData(md, to);
	}

	bool ConvertMetaData(const NavMetaData::MetaData& md, EncryptedMetaData* to)
	{
		assert(to);
		bool bSuccess = true;

		to->streamID = md.streamID;
		to->subStreamID = md.subStreamID;

		switch (md.format)
		{
		case NavMetaData::DATA_DISCONTINUITY:
			to->format = EMD_DATA_DISCONTINUITY;
			break;
		case NavMetaData::TIME_OFFSET:
			to->format = EMD_TIME_DISCONTINUITY;
			to->value  = md.nInt64;
			break;
		case NavMetaData::RATE_CHANGE:
			to->format = EMD_RATE_CHANGE;
			to->value  = (UInt64)(md.dDouble * 1000);
			break;
		case NavMetaData::PAUSE_DELTA:
			to->format = EMD_TIME_DELTA;
			to->value  = md.nInt64;
			break;
		case NavMetaData::STILL_FRAME:
			to->format = EMD_END_OF_MENU;
			break;
		case NavMetaData::END_OF_FILE:
			to->format = EMD_END_OF_FILE;
			break;
		case NavMetaData::SKIP_HEAD_BYTES:
			to->format = EMD_SKIP_HEAD_BYTES;
			to->value  = md.nInt64;
			break;
		case NavMetaData::SKIP_TAIL_BYTES:
			to->format = EMD_SKIP_TAIL_BYTES;
			to->value  = md.nInt64;
			break;
		default:
			bSuccess = false;
			break;
		}

		return bSuccess;
	}


private:
	EncryptedMetaData mData;
};

#endif  //__ENCRYPTEDMETADATA_RT_STREAM_H__

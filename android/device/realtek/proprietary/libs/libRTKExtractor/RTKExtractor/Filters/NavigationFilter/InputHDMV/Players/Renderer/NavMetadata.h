//-----------------------------------------------------------------------------
// NavMetaData.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _NAVMETADATA_H_
#define _NAVMETADATA_H_

#include "CommonTypes.h"
#include "md_stream.h"

namespace NavMetaData
{

// Corresponds to format entry of MetaData.
// Note, time units are in units of 100 nano-seconds (milliseconds * 10,000)
enum
{
    DATA_DISCONTINUITY, // union = 0
    
    TIME_OFFSET,        // union = SInt64 with the offset in 100 nano-secs units.
                        // This is added to the native pts's in the stream.  All
                        // streams are internally forced to send 0 as its first 
                        // time stamp, so use this to modify the starting pts
                        // of a stream.
                        
    RATE_CHANGE,        // union = double of the new rate (neg means reverse play)
    
    PAUSE_DELTA,        // union = SInt64 with the paused duration in 100 nano-secs units.
                        // Use this to sync a paused stream (one not receiving data) to
                        // the clock time...this is a cummulative value. 
                        // (this gets added to any existing TIME_OFFSET  values
                        //  so call this after calling TIME_OFFSET).

    STILL_FRAME,        // union (not used)
                        // Use this on the first sample of an I frame 
                        // that has no other associated frames (like a menu,
                        // slide show, or I frame only playback modes).
                        // Decoders use this flag to decode and immediately
                        // display the I frame.

    END_OF_FILE,		// union = 0

	SKIP_HEAD_BYTES,    // union = SInt64 indicating number of bytes
                        //
                        // Skip specified bytes at the beginning of the 
                        // buffer after decryption and before processing.

	SKIP_TAIL_BYTES,    // union = SInt64 indicating number of bytes
                        //
                        // Skip specified bytes at the end of the 
                        // buffer after decryption and before processing.
};

typedef struct _tagMetaData
{
    union
    {
    double  dDouble;
    SInt64  nInt64;
    SInt32  nInt32;
    };                  // Various meta data value types
    
    UInt32  format;     // Describes the type of meta data
    UInt16  streamID;   // Identify a specific stream in the input muxed stream.
    UInt16  subStreamID;// Sub stream ID associated the stream ID.

    _tagMetaData(){
        nInt64 = format = 0; streamID = subStreamID = 0xFFFF;
    }
} MetaData;

#define NAV_METADATA_STREAM_ALL         0xFFFF
#define NAV_METADATA_SUB_STREAM_ALL     0xFFFF

class HDNav_rt_stream_metadata : public rt_stream_metadata
{
	typedef rt_stream_metadata INHERITED;
public:
	HDNav_rt_stream_metadata(const MetaData& data) : mData(data) {}
	virtual ~HDNav_rt_stream_metadata() { }

	const MetaData& GetData() const { return mData; }

	virtual rt_stream_metadata* copy() const
	{
		return new HDNav_rt_stream_metadata(*this);
	}

private:
	MetaData mData;
};

};

#endif //_NAVMETADATA_H_

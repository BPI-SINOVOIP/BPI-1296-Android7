// ***************************************************************
//  CEStreamDescriptor   version:  1.0   ¡¤  date: 06/14/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - Sonic Solutions. All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CEStreamDescriptor_H
#define CEStreamDescriptor_H

#include "ref_ptr.h"
#include <vector>
#include "typedesc.h"
#include "BD_FS_rt_stream.h"



class StreamDescriptor  // Different from STREAM_DESC, this is the composite(muxed) stream desctirptor.
{
public:
	enum
	{
		//Note : don't change enum order, because StreamControl add stream in this order.
		OUTPUT_INDEX_VIDEO = 0,
		OUTPUT_INDEX_AUDIO,
		OUTPUT_INDEX_SUB_VIDEO,
		OUTPUT_INDEX_SUB_AUDIO,
		//OUTPUT_INDEX_SPU = 4,

		OUTPUT_INDEX_MAX
	};

	StreamDescriptor(UInt32 index, UInt32 allocBufSize,UInt32 allocBufCount, ref_ptr<rt_stream>& pSrcStream);

	virtual ~StreamDescriptor();

	UInt32 GetIndex() {return mIndex;}
	void ResetStreamDesc();
	void SetOutputID(int index, int streamID, int subStreamID);
	void SetOutputDesc(int index, STREAM_DESC* pStreamDesc);
	STREAM_DESC* GetOutputDesc(int index)
	{
		assert(index < OUTPUT_INDEX_MAX && index >= OUTPUT_INDEX_VIDEO);
		return &(mOutput[index].streamDesc);
	}
	bool IsOutputVideo(int index);
	//bool IsOutputSpu(int index);
	bool IsOutputAudio(int index);
	int GetOutputID(int index)
	{
		assert(index < OUTPUT_INDEX_MAX && index >= OUTPUT_INDEX_VIDEO);
		return mOutput[index].outputID;
	}
	//void SetOutputPID(int index, int streamID, int subStreamID);
	//void ClearAllOutputID();
	//void printOutputID();
	//void ClearOutputID(UInt32 id);
	//void ClearPathOutputID(bool main);
	void GetStreamDescritption(UInt32 &allocBufSize, UInt32 &allocBufCount, ref_ptr<rt_stream> &pSrcStream);
	
	rt_stream* GetSrcStream() { return mpSrcStream.get();}
	//static const UInt32 kOutputNums = 4; 	 
private:
	UInt32 mIndex;
	//UInt32 mOutputNum;	// max 4 output (video , audio ,sub video and sub audio), if no output attached, num is 0.
	//UInt32 mOutputID[kOutputNums];	//  0---video, 1---audio ,2 --sub video,, 3---sub audio
	//PARSER_TYPE mParserType;
	UInt32 mAllocBufSize; 
	UInt32 mAllocBufCount;
    ref_ptr<rt_stream> mpSrcStream;
    
	struct {
		int			outputID;	// -1 means invalid entry
		STREAM_DESC streamDesc;
	} mOutput[(int)OUTPUT_INDEX_MAX];
};

#endif


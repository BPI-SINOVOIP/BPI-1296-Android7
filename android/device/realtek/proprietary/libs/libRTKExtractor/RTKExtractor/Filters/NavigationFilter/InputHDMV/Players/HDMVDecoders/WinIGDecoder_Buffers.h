//-----------------------------------------------------------------------------
// WinIGDecoder_Buffers.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINIGDECODER_BUFFERS_H__
#define __WINIGDECODER_BUFFERS_H__

#include "IGDecoder_Buffers.h"
#include "WinHDMVSysObject.h"


//
// WinIGDecoder_CodedDataBuffer
// 
class WinIGDecoder_CodedDataBuffer : public IGDecoder_CodedDataBuffer
{
public:
	WinIGDecoder_CodedDataBuffer();
	~WinIGDecoder_CodedDataBuffer();

protected:
	SonicError BufferPacket(ref_ptr<PACKET_DATA> packet);
	SonicError PushBufferedPacket(void);
};

//__________________________________________________________________________________________________________________________________________________
// WinIGDecoder_DecodedDataBuffer
// Represents the DB in an IG Decoder.
//
class WinIGDecoder_DecodedDataBuffer : public IGDecoder_DecodedDataBuffer
{
public:
	WinIGDecoder_DecodedDataBuffer();
	~WinIGDecoder_DecodedDataBuffer();

	void Init(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas);

protected:
	virtual HDMVSysObject * CreateHDMVSysObject(void)		{ WinHDMVSysObject * pObj = new WinHDMVSysObject(); return pObj; }
	virtual void RendererPrepare()							{ }
	virtual void RendererCommit()							{ }

private:
	WinIGDecoder_DecodedDataBuffer(const WinIGDecoder_DecodedDataBuffer&); // no copying
	const WinIGDecoder_DecodedDataBuffer& operator=(const WinIGDecoder_DecodedDataBuffer&); // no copying
};

#endif // __WINIGDECODER_BUFFERS_H__

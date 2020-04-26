//-----------------------------------------------------------------------------
// WinTextSTDecoder_Buffers.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINTEXTSTDECODER_BUFFERS_H__
#define __WINTEXTSTDECODER_BUFFERS_H__

#include "TextSTDecoder_Buffers.h"
#include "WinHDMVSysObject.h"


class WinTextSTDecoder_CodedDataBuffer : public TextSTDecoder_CodedDataBuffer
{
public:
	WinTextSTDecoder_CodedDataBuffer();
	~WinTextSTDecoder_CodedDataBuffer();

protected:
	SonicError BufferPacket(ref_ptr<PACKET_DATA> packet);
	SonicError PushBufferedPacket(void);
};

//__________________________________________________________________________________________________________________________________________________
// WinTextSTDecoder_DecodedDataBuffer
// Represents the DB in a Windows TextST Decoder.
//
class WinTextSTDecoder_DecodedDataBuffer : public TextSTDecoder_DecodedDataBuffer
{
public:
	WinTextSTDecoder_DecodedDataBuffer();
	~WinTextSTDecoder_DecodedDataBuffer();

	void Init(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas, UInt32 maxTextSTObjects);

protected:
	virtual HDMVSysObject * CreateHDMVSysObject(void)		{ WinHDMVSysObject * pObj = new WinHDMVSysObject(); return pObj; }
	virtual void RendererPrepare()							{ }
	virtual void RendererCommit()							{ }

private:
	WinTextSTDecoder_DecodedDataBuffer(const WinTextSTDecoder_DecodedDataBuffer&); // no copying
	const WinTextSTDecoder_DecodedDataBuffer& operator=(const WinTextSTDecoder_DecodedDataBuffer&); // no copying
};

#endif // __WINTEXTSTDECODER_BUFFERS_H__

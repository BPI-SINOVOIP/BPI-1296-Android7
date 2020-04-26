//-----------------------------------------------------------------------------
// TextSTDecoder_Buffers.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __TEXTSTDECODER_BUFFERS_H__
#define __TEXTSTDECODER_BUFFERS_H__

#include "ref_ptr.h"
//#include "ring_rt_stream.h"
//#include "SonicBinarySemaphore.h"
//#include "IPacketSinkCallback.h"
#include "BaseRenderer.h"
#include "BDROM_NavigatorTypes.h"
#include "HDMVDecoder_EB.h"
#include "HDMVDecoder_Types_Internal.h"
#include "HDMVDecoder_CodedDataBuffer.h"
#include "HDMVDecoder_CompositionBuffer.h"
#include "HDMVDecoder_DecodedDataBuffer.h"

using namespace BDROM_HDMVDecoder;

class TextSTDecoder_StreamGraphicsProcessor;


//__________________________________________________________________________________________________________________________________________________
// TextSTDecoder_CodedDataBuffer
// Represents the EB in an IG Decoder
//
class TextSTDecoder_CodedDataBuffer : public HDMVDecoder_CodedDataBuffer
{
public:
	TextSTDecoder_CodedDataBuffer();
	virtual ~TextSTDecoder_CodedDataBuffer();

	void Init(TextSTDecoder_StreamGraphicsProcessor * pStreamGraphicsProcessor, ref_ptr<BaseRenderer> pRenderer);

protected:
	virtual SonicError ReceivePacket(ref_ptr<PACKET_DATA> packet, bool bHavePlatformDTSPTS = false, SInt64 platformDTS = 0, SInt64 platformPTS = 0);

	bool	m_bDisplaySetStarted;

private:
	TextSTDecoder_CodedDataBuffer(const TextSTDecoder_CodedDataBuffer &);
	TextSTDecoder_CodedDataBuffer& operator=(const TextSTDecoder_CodedDataBuffer &);
};



//__________________________________________________________________________________________________________________________________________________
// TextSTDecoder_CompositionBuffer
// Represents the CB in an IG Decoder.
//
class TextSTDecoder_CompositionBuffer : public HDMVDecoder_CompositionBuffer
{
public:
	TextSTDecoder_CompositionBuffer();
	~TextSTDecoder_CompositionBuffer();

	void Init(ref_ptr<BaseRenderer> pRenderer);
	virtual void Reset(void);

	txt_dialog_style_segment * CreateDialogStyleSegment(void);
	txt_dialog_presentation_segment * CreateDialogPresentationSegment(UInt32 dpsNum);
	void CreateRegionStyles(txt_dialog_style_segment * pDPS);
	void CreateUserControlStyles(txt_dialog_style_segment * pDPS);

	txt_dialog_style_segment *        GetDialogStyleSegment(void)						{ return m_pDSS; }
	txt_dialog_presentation_segment * GetDialogPresentationSegment(UInt32 dpsNum);

	void DestroyDialogPresentationSegment(txt_dialog_presentation_segment * pDPS);
	void DestroyAllDialogPresentationSegments(void);

protected:
	txt_dialog_style_segment *						m_pDSS;
	std::vector<txt_dialog_presentation_segment*>	m_DPSArray;	// array to store all allocated DPSs

private:
	TextSTDecoder_CompositionBuffer(const TextSTDecoder_CompositionBuffer&); // no copying
	const TextSTDecoder_CompositionBuffer& operator=(const TextSTDecoder_CompositionBuffer&); // no copying
};




//__________________________________________________________________________________________________________________________________________________
// TextSTDecoder_DecodedDataBuffer
// Represents the DB in an IG Decoder.
//
class TextSTDecoder_DecodedDataBuffer : public HDMVDecoder_DecodedDataBuffer
{
public:
	TextSTDecoder_DecodedDataBuffer();
	~TextSTDecoder_DecodedDataBuffer();

	void Init(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas, UInt32 maxTextSTObjects);
	virtual void Reset(void);

private:
	TextSTDecoder_DecodedDataBuffer(const TextSTDecoder_DecodedDataBuffer&); // no copying
	const TextSTDecoder_DecodedDataBuffer& operator=(const TextSTDecoder_DecodedDataBuffer&); // no copying
};

#endif // __TEXTSTDECODER_BUFFERS_H__

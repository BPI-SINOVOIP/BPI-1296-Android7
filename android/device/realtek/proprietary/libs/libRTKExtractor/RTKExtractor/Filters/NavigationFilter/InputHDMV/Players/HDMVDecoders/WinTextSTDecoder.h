//-----------------------------------------------------------------------------
// WinTextSTDecoder.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINTEXTSTDECODER_H__
#define __WINTEXTSTDECODER_H__

#include "TextSTDecoder.h"
#include "HDMVDecoders_DeviceLost.h"
#include "BDROM_PTSAccess.h"

#include "DisplayLayerClientImpl.h"


//__________________________________________________________________________________________________________________________________________________
// WinTextSTDecoder
// Windows implementation of a BDROM TextST Decoder
//

class WinTextSTDecoder : public TextSTDecoder, public HDMVDecoders_DeviceLost, protected DisplayLayerClientImpl
{
public:
	WinTextSTDecoder();
	virtual ~WinTextSTDecoder();

	void Init(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas, DisplayLayer * pDisplayLayer, UInt32 nFramesCount, ref_ptr<BDROM_PTSAccess> pPtsAccess);

	virtual void Play(void);
	virtual void Stop(void);
	virtual void StartOfStream(void);

protected:
	// creation and initialization of TextST Decoder components.
	virtual void CreateCodedDataBuffer(void);
	virtual void CreateDecodedDataBuffer(void);
	virtual void CreateGraphicsController(void);
	virtual void InitDecodedDataBuffer(void);
	virtual void InitGraphicsController(void);

	// device lost handling
	virtual void DeviceLost(void);
	virtual void DeviceNew(void);
	
private:
	WinTextSTDecoder(const WinTextSTDecoder&); // no copying
	const WinTextSTDecoder& operator=(const WinTextSTDecoder&); // no copying
};

#endif // __WINTEXTSTDECODER_H__

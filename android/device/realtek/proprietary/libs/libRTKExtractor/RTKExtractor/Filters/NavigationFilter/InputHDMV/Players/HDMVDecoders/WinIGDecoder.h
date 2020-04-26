//-----------------------------------------------------------------------------
// WinIGDecoder.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINIGDECODER_H__
#define __WINIGDECODER_H__

#include "HDMVDecoders_DeviceLost.h"
#include "IGDecoder.h"
#include "BDROM_PTSAccess.h"

#include "DisplayLayerClientImpl.h"


//__________________________________________________________________________________________________________________________________________________
// WinIGDecoder
// Windows implementation of a BDROM IG Decoder
//

class WinIGDecoder : public IGDecoder, public HDMVDecoders_DeviceLost, protected DisplayLayerClientImpl
{
public:
	WinIGDecoder();
	virtual ~WinIGDecoder();

	void Init(ref_ptr<BaseRenderer> pRenderer,
				UInt32 rendererCanvas,
				ref_ptr<BDROM_SoundPlayer> pSoundPlayer,
				DisplayLayer * pDisplayLayer,
				UInt32 nFramesCount,
				BDROM_DiscType discType,
				ref_ptr<BDROM_PTSAccess> pPtsAccess);

	virtual void Play(void);
	virtual void Stop(void);
	virtual void StartOfStream(void);

protected:
	virtual void CreateCodedDataBuffer(void);
	virtual void CreateDecodedDataBuffer(void);
	virtual void CreateGraphicsController(void);
	virtual void InitDecodedDataBuffer(void);
	virtual void InitGraphicsController(void);

	// device lost handling
	virtual void DeviceLost(void);
	virtual void DeviceNew(void);

private:
	WinIGDecoder(const WinIGDecoder&); // no copying
	const WinIGDecoder& operator=(const WinIGDecoder&); // no copying
};

#endif // __WINIGDECODER_H__

#ifndef __SILK_DECODER_H__
#define __SILK_DECODER_H__

#include <OSAL.h>
#include <CStream.h>
#include <Platform_Lib/SILK_SDK/interface/SKP_Silk_SDK_API.h>
#include "SilkDecInputPin.h"
#include "SilkDecOutputPin.h"
#include <ae/ddk/fl31nativesoundoutput/SoundOutput.h>

#define MAX_BYTES_PER_FRAME     1024
#define MAX_INPUT_FRAMES        5
#define MAX_FRAME_LENGTH        480
#define FRAME_LENGTH_MS         20
#define MAX_API_FS_KHZ          48
#define MAX_LBRR_DELAY          2

class SilkDecoder: public CBaseFilter
{
	friend class SilkDecInputPin;
	friend class SilkDecOutputPin;
	
public:
			
	SilkDecoder();		
	~SilkDecoder();
	
    HRESULT		Run(void);												
	HRESULT		Stop(void);												
	HRESULT		Pause(FILTER_STATE State);	

	HRESULT		Decode(BYTE *pBuf, long bufSize);
	
protected:
	SilkDecInputPin* 	m_pInputPin;	
	SilkDecOutputPin*	m_pOutputPin;
	CMemListAllocator* 	m_pOutputAllocator;
	CMediaSample*		m_pMediaSample[1];

	SoundOutputRTK*		m_pSoundOutput;

	void*				m_psDec;
	SKP_SILK_SDK_DecControlStruct m_DecControl;

	SKP_int32 totPackets, frames;
	SKP_int16 nBytes;
    SKP_uint8 payload[    MAX_BYTES_PER_FRAME * MAX_INPUT_FRAMES * ( MAX_LBRR_DELAY + 1 ) ];
    SKP_uint8 *payloadEnd, *payloadToDec;
    SKP_uint8 FECpayload[ MAX_BYTES_PER_FRAME * MAX_INPUT_FRAMES ], *payloadPtr;
    SKP_int16 nBytesFEC;
    SKP_int16 nBytesPerPacket[ MAX_LBRR_DELAY + 1 ], totBytes;
    SKP_int16 out[ ( ( FRAME_LENGTH_MS * MAX_API_FS_KHZ ) << 1 ) * MAX_INPUT_FRAMES ], *outPtr;	

#ifdef DUMP_OUT_FILE
	FILE* m_speechOutFile;
#endif
};

#endif	// __SILK_DECODER_H__

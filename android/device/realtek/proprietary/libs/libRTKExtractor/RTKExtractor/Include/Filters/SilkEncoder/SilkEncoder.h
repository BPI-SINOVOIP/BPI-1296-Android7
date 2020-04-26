#ifndef __SILK_ENCODER_H__
#define __SILK_ENCODER_H__

#include <OSAL.h>
#include <CStream.h>
#include <Platform_Lib/SILK_SDK/interface/SKP_Silk_SDK_API.h>
#include "SilkEncInputPin.h"
#include "SilkEncOutputPin.h"

#define MAX_BYTES_PER_FRAME     250 // Equals peak bitrate of 100 kbps 
#define MAX_INPUT_FRAMES        5
#define MAX_LBRR_DELAY          2
#define MAX_FRAME_LENGTH        480
#define FRAME_LENGTH_MS         20
#define MAX_API_FS_KHZ          48

class SilkEncoder: public CBaseFilter
{
	friend class SilkEncInputPin;
	friend class SilkEncOutputPin;
	
public:
			
	SilkEncoder(int freqHz);		
	~SilkEncoder();
	
    HRESULT		Run(void);												
	HRESULT		Stop(void);												
	HRESULT		Pause(FILTER_STATE State);	

	HRESULT		Encode(BYTE *pBuf, long bufSize);
	int			GetPacketSize() { return m_encControl.packetSize * sizeof(SKP_int16); }

protected:
	SilkEncInputPin* 	m_pInputPin;	
	SilkEncOutputPin*	m_pOutputPin;
	CMemListAllocator* 	m_pOutputAllocator;	

	void*      			m_psEnc;
	SKP_SILK_SDK_EncControlStruct m_encControl; // Struct for input to encoder
	int					m_sampleFreq;			// sample frequency of input pcm data 
												// (supporting values: 8000,

#ifdef DUMP_OUT_FILE
	FILE* 		m_pOutFile;
#endif	
};

#endif	// __SILK_ENCODER_H__


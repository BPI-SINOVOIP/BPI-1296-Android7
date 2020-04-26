/**
// ///////////////////////////////////////////////////////////////////////////
//	@file CMPPAOut.h
//
//	@author Neco     20100421
//
//	copyright (c)RealTek.   All rights reserved.
// ///////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_M_PP_AOUT_FILTER_H__
#define __C_M_PP_AOUT_FILTER_H__

#include "Filters/AudioFilter_c/CPPAOut.h"
#include "Filters/AudioFilter_c/CAudioOut.h"
#include "Filters/AudioFilter_c/CAudioPin.h"
#include "Filters/AudioFilter_c/CMultiPin.h"
#include "Filters/AudioFilter_c/CMultiAllocator.h"

#define	MAX_MIC_INPUT_CH_NUM	AUDIO_IN_OUTPIN
#define	MAX_MIDI_INPUT_CH_NUM	AUDIO_IN_OUTPIN

typedef enum{
	MIX_FOCUS_MIC = 0,
	MIX_FOCUS_MP3,
	MIX_FOCUS_MIDI,
	MIX_FOCUS_ALL,
} MPP_FOCUS_PIN;


class CAudioMultiInputPin;
class CAudioMPPAOutInputPin;
class CAudioMPPAOutOutputPin;
//class CMPPRingAllocator;

// Midi & karaoke and pp complex filter	
class CAudioMPPAOutFilter : public CAudioPPAOutFilter
{
friend class CAudioMPPAOutInputPin;

public:

	CAudioMPPAOutFilter(AUDIO_MODULE_TYPE PPtype = AUDIO_KARAOKE_MIXER, MPP_FOCUS_PIN FocusPin = MIX_FOCUS_ALL);
	virtual ~CAudioMPPAOutFilter();

	HRESULT				Pause(FILTER_STATE State);
	HRESULT				Run(void);
	HRESULT				Stop(void);

	int					GetAOUTInstanceID() { return m_aoutInstanceID; };
	HRESULT				SetFocus();
//	HRESULT				InitInPinRingData(); 

	HRESULT				RequestMidiRWriteSpace(BYTE **pWritePtr, long *pnFreeSpace, long nRequestSize);
	HRESULT				RequestMidiLWriteSpace(BYTE **pWritePtr, long *pnFreeSpace, long nRequestSize);
	HRESULT				PutMidiRData(BYTE *pData, long Size);
	HRESULT				PutMidiLData(BYTE *pData, long Size);
	long				GetADPCMData(BYTE *pBuffer, long nReadBytes);
	
private:
	void				xGetMidiBuffer(void);
	void				MyMemcpy(unsigned char* pSrc, unsigned char* pDes, long size);
	
protected:

	// Mic IN Pin & Allocator array
	CAudioMPPAOutInputPin*			m_pMicInPin;
	CBaseInputPin*					m_pCHMicInPinArr[MAX_MIC_INPUT_CH_NUM];
	CMemRingAllocator*				m_pMicInputAllocator;
	CMemRingAllocator*				m_pMicInAllocatorArr[MAX_MIC_INPUT_CH_NUM];
	
	// Midi IN Pin & Allocator array
	CAudioMPPAOutInputPin*			m_pMidiInPin;
	CBaseInputPin*					m_pCHMidiInPinArr[MAX_MIDI_INPUT_CH_NUM];
	CMemRingAllocator*				m_pMidiInputAllocator;
	CMemRingAllocator*				m_pMidiInAllocatorArr[MAX_MIDI_INPUT_CH_NUM];

	// Midi IN Right channel ring buffer info
	IMemAllocator*					m_pMidiRAllocator;
    BYTE*							m_pMidiRBufferLower;
    BYTE*							m_pMidiRBufferUpper;
    long							m_nMidiRBufferSize;

	// Midi IN Left channel ring buffer info
	IMemAllocator*					m_pMidiLAllocator;
    BYTE*							m_pMidiLBufferLower;
    BYTE*							m_pMidiLBufferUpper;
    long							m_nMidiLBufferSize;

	// Audio Out IN PinID
	AUDIO_IO_PIN					m_AoutInPinID;
	
	static long						m_MICInstanceNum;
	
	// Ring buffer for handling data to do ADPCM (used by customer)
    CAudioMPPAOutOutputPin*			m_pInnerOutPin; // @FIXME
    CMemRingAllocator*				m_pADPCMAllocator;
    BYTE*							m_pADPCMBufferLower;
    BYTE*							m_pADPCMBufferUpper;
    long							m_nADPCMBufferSize;
    IReadPointerHandle*				m_pRPHandle;

};                  

class CAudioMPPAOutInputPin: public CAudioMultiInputPin
{
public:
    CAudioMPPAOutInputPin(long pinID)
                : CAudioMultiInputPin() { m_pinID = pinID; };
};

// This is a Pin class for Filter Output Pin, but using the CBaseInputPin attribute
class CAudioMPPAOutOutputPin : public CBaseInputPin
{
public:
    CAudioMPPAOutOutputPin() {};
};


#endif 

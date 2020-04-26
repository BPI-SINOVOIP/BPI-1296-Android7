
/**
/////////////////////////////////////////////////////////////////////////////
//	@file CAudioMPEGDecAgent.h
//
//  @brief Header file for DVD-VENUS Audio MPEG Decoder Agent Class interface define
//
//	@author Taro     09/21/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_MPEG_DECODER_AGENT_H__
#define __C_AUDIO_MPEG_DECODER_AGENT_H__


#include "CAudioDecAgent.h"
#include "MPEGDecType.h"
#include "CAudioQueue.h"

class   CAudioMPEGDecAgentInPin;
class   CAudioMPEGDecAgentOutPin;

/** 

  class CAudioMPEGDecAgent : public CAudioDecAgent

*/

class CAudioMPEGDecAgent : public CAudioDecAgent 
{

friend class   CAudioMPEGDecAgentInPin;
friend class   CAudioMPEGDecAgentOutPin;

public:

	CAudioMPEGDecAgent(long* ID);
	virtual ~CAudioMPEGDecAgent();

	HRESULT							ProcessFilterPrivateInfo(BYTE* pData, long length);

protected:
	bool							MainProcess(void);
	int64_t							CalculatePTS(BYTE* pRingLower, BYTE* pRingUpper, BYTE* pReadPointer, int framePTS);

protected:
	CAudioAgentBaseOutputPin*		m_pCHPinArr[AUDIO_DEC_OUTPIN];
	CMemRingAllocator*				m_pAllocatorArr[AUDIO_DEC_OUTPIN];

	AUDIO_DEC_MPEG_CFG				m_Config;

	CAudioPackQueue					m_AudioPackQueue;
	int								m_ValidReadDataLen;
	AUDIO_PCM_FORMAT				m_pcmFormat;


};


/** 
**********************************************************************************

class CAudioDecInputPin

**********************************************************************************
*/

class CAudioMPEGDecAgentInPin : public CAudioDecAgentInPin
{
	friend class CAudioMPEGDecAgent;
public:

    CAudioMPEGDecAgentInPin() {};
    virtual ~CAudioMPEGDecAgentInPin() {};

	HRESULT							Receive(BYTE* pData, long length);

private:

};


class CAudioMPEGDecAgentExtInPin : public CAudioDecAgentInPin
{
	friend class CAudioMPEGDecAgent;
public:

    CAudioMPEGDecAgentExtInPin() {};
    virtual ~CAudioMPEGDecAgentExtInPin() {};

private:

};


/** 
**********************************************************************************

class CAudioDecOutputPin

**********************************************************************************
*/
// several output pin is wrapped in the class

class CAudioMPEGDecAgentOutPin : public CAudioDecAgentOutPin
{    

	friend class CAudioMPEGDecAgent;
public:

    CAudioMPEGDecAgentOutPin() 
		: CAudioDecAgentOutPin() {};
	virtual ~CAudioMPEGDecAgentOutPin(void) {};	

//	HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

private:

};	


/** 
**********************************************************************************

**********************************************************************************
*/



#endif



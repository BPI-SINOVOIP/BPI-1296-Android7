   
/**
/////////////////////////////////////////////////////////////////////////////
//  @file CAudioMPEGDecAgent.h
//
//  @brief Header file for DVD-VENUS Audio MPEG Decoder Agent Class interface define
//
//  @author Taro     09/21/2004
//
//  copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_AC3_DECODER_AGENT_H__
#define __C_AUDIO_AC3_DECODER_AGENT_H__


#include "CAudioDecAgent.h"
#include "AC3DecType.h"
#include "CAudioAgentPin.h"
#include "CAudioQueue.h"

class   CAudioAC3DecAgentInPin;
class   CAudioAC3DecAgentOutPin;

/** 

  class CAudioAC3DecAgent : public CAudioDecAgent

*/
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////

class CAudioAC3DecAgent : public CAudioDecAgent
{

friend class   CAudioAC3DecAgentInPin;
friend class   CAudioAC3DecAgentOutPin;

public:

    CAudioAC3DecAgent(long* ID);
    virtual ~CAudioAC3DecAgent();

    HRESULT                         ProcessFilterPrivateInfo(BYTE* pData, long length);

protected:
    bool                            MainProcess(void);
    int64_t                         CalculatePTS(BYTE* pRingLower, BYTE* pRingUpper, BYTE* pReadPointer, int framePTS);

protected:
    bool                            Flush(void);
    
    CAudioAgentBaseOutputPin*       m_pCHPinArr[AUDIO_DEC_OUTPIN];
    CMemRingAllocator*              m_pAllocatorArr[AUDIO_DEC_OUTPIN];

    AUDIO_DEC_AC3_CFG               m_Config;

    CAudioPackSemQueue              m_AudioPackQueue;
    AUDIO_PCM_FORMAT                m_pcmFormat;

    CAudioRingReadSizeControl       m_RingSizeControl;
    
};


/** 
**********************************************************************************

class CAudioAC3DecAgentInPin

**********************************************************************************
*/

class CAudioAC3DecAgentInPin : public CAudioDecAgentInPin
{
    friend class CAudioAC3DecAgent;
public:

    CAudioAC3DecAgentInPin() {};
    virtual ~CAudioAC3DecAgentInPin() {};

    HRESULT                         Receive(BYTE* pData, long length);
    // HRESULT Flush(void);

protected:

private:

};


/** 
**********************************************************************************

class CAudioDecOutputPin

**********************************************************************************
*/
// several output pin is wrapped in the class

class CAudioAC3DecAgentOutPin : public CAudioDecAgentOutPin
{    

    friend class CAudioAC3DecAgent;
public:

    CAudioAC3DecAgentOutPin() 
        : CAudioDecAgentOutPin() {};
    virtual ~CAudioAC3DecAgentOutPin(void) {};  

//  HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

private:

};  


/** 
**********************************************************************************

**********************************************************************************
*/

#endif

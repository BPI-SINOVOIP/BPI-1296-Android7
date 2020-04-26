
/**
/////////////////////////////////////////////////////////////////////////////////
//  @file CAudioDecAgent.h
//
//  @brief Header file for DVD-VENUS Audio Decoder Agent Class interface define
//
//  @author Taro     08/20/2004
//
//  copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_DECODER_AGENT_H__
#define __C_AUDIO_DECODER_AGENT_H__

#include <CStream.h>
#include "CAudioAgent.h"
#include "CAudioAgentPin.h"

class   CAudioDecAgentInPin;
class   CAudioDecAgentOutPin;

/** 

  class CAudioDecAgent : public CAudioAgent

*/

class CAudioDecAgent : public CAudioTransAgent,
                       public IFilterSeeking
{
public:
    CAudioDecAgent(long* ID);
    virtual ~CAudioDecAgent();

    HRESULT             CheckCapabilities(
                            FILTER_SEEKING_CAP*     pSeekingCap
                        );
    HRESULT             SetSeekingInfo(
                            long                    Speed,
                            long                    Skip
                        );
    HRESULT             GetSeekingInfo(
                            long*                   pSpeed,
                            long*                   pSkip
                        );
protected:

    bool                            MainProcess(void);

protected:
    // CCritSec                    m_InputThreadLock;
    long                        m_Speed;
    long                        m_Skip;
};


/** 
**********************************************************************************

class CAudioDecInputPin

**********************************************************************************
*/

class CAudioDecAgentInPin : public CAudioAgentInputPin
{
    friend class CAudioDecAgent;
public:

    CAudioDecAgentInPin() {};
    virtual ~CAudioDecAgentInPin() {};

private:

};


/** 
**********************************************************************************

class CAudioDecOutputPin

**********************************************************************************
*/
// several output pin is wrapped in the class

class CAudioDecAgentOutPin : public CMultiAudioAgentOutPin
{    

    friend class CAudioDecAgent;
public:

    CAudioDecAgentOutPin() 
        : CMultiAudioAgentOutPin() {};
    virtual ~CAudioDecAgentOutPin(void) {}; 

//  HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

private:

};  



#endif



/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CAudioEncoder.h
//
//  @brief Header file for DVD-VENUS Audio Encoder Agent Class interface define
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_ENCODER_AGENT_H__
#define __C_AUDIO_ENCODER_AGENT_H__

#include "CAudioAgent.h"
#include "CAudioAgentPin.h"


class CAudioEncAgentInputPin;
class CAudioEncAgentOutputPin;

/**
class CAudioEncAgent : public CAudioAgent
*/

class CAudioEncAgent : public CAudioTransAgent 
{
public:
	CAudioEncAgent(long* ID);
	virtual ~CAudioEncAgent();

protected:
	
	bool							MainProcess(void);

};


/** 
**********************************************************************************

class CAudioDecInputPin

**********************************************************************************
*/

class CAudioEncAgentInPin : public CMultiAudioAgentInPin
{
	friend class CAudioEncAgent;
public:

	CAudioEncAgentInPin()
		: CMultiAudioAgentInPin() {};
	virtual ~CAudioEncAgentInPin(void) {};	
//    CAudioDecInputPin();
//    ~CAudioDecInputPin();

private:

};


/** 
**********************************************************************************

class CAudioDecOutputPin

**********************************************************************************
*/
// several output pin is wrapped in the class

class CAudioEncAgentOutPin : public CAudioAgentOutputPin
{    

	friend class CAudioEncAgent;
public:



//	HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

private:

};	


/** 
**********************************************************************************

**********************************************************************************
*/


#endif



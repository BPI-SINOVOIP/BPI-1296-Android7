 
/**
/////////////////////////////////////////////////////////////////////////////
//	@file CAudioDecPin.h
//
//	@brief Implementation for DVD-VENUS Audio Decoder Pin Class
//
//	@author Taro     02/16/2005
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////
*/


#ifndef __C_AUDIO_DEC_PIN_H__
#define __C_AUDIO_DEC_PIN_H__


#include "CAudioPin.h"
#include "Filters/AudioFilter_c/CAudioDecoder.h"

/** 
**********************************************************************************

class CAudioDecInputPin

**********************************************************************************
*/

class CAudioDecInputPin : public CAudioInputPin
{
	friend class CAudioDecFilter;
public:

    CAudioDecInputPin(long pinID) { m_pinID = pinID; };
    virtual ~CAudioDecInputPin() {};

	// call RPC endofstream to peer agent
	HRESULT						NewSegment(void);
	HRESULT						EndOfStream(unsigned int eventID);
    HRESULT                     Flush(void);
	HRESULT						PrivateInfo(int infoId, BYTE* pInfo, int length);

protected:

	HRESULT						MyMemcpy(BYTE* pCurr, BYTE* Data, long Size);
	HRESULT						DeliverInBandCommand(BYTE* Data, long Size);
	
};


/** 
**********************************************************************************

class CAudioDecOutputPin

**********************************************************************************
*/
// several output pin is wrapped in the class

class CAudioDecOutputPin : public CAudioMultiOutputPin
{    

	friend class CAudioDecFilter;
public:

    CAudioDecOutputPin(long pinID)
		: CAudioMultiOutputPin() { m_pinID = pinID; };
	virtual ~CAudioDecOutputPin(void) {};	

	HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

protected:
	HRESULT						GetConnectedInputPin(int n, IMemInputPin** ppPin);								
};	


#endif

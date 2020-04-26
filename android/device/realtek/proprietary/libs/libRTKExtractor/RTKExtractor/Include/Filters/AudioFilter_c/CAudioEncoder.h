/**
/////////////////////////////////////////////////////////////////////////////////
//
//  @brief Header file for DVD-VENUS Audio Decoder Filter Class interface define
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_ENCODER_FILTER_H__
#define __C_AUDIO_ENCODER_FILTER_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include <CStream.h>
#include "StreamClass/SType.h"
#include "StreamClass/EType.h"

#include "CAudioPin.h"
#include "CAudioFilter.h"

typedef ulDArray<CMediaType*>            MediaTypeArr;


/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////

class   CAudioEncInputPin;
class   CAudioEncOutputPin;

/** 

  class CAudioEncFilter : public CAudioFilter

*/

class CAudioEncFilter : public CAudioFilter
{
    friend class CAudioEncInputPin;
    friend class CAudioEncOutputPin;
	
public:

	CAudioEncFilter(void);
	HRESULT					UpdateReadPtr(AUDIO_RPC_READPACK* pReadPack);
	HRESULT					UpdateWritePtr(AUDIO_RPC_WRITEPACK* pWritePack);
	// handle RPC call of end of stream
	HRESULT					EndOfStream(long pinID, long eventID);
	HRESULT					DeliverFlush(long pinID);
	HRESULT					InitAgentRingBuffer();

protected:
			
    CAudioInputPin*					m_pInPin;
    CAudioOutputPin*				m_pOutPin;
    CAudioOutputPin*				m_pExtOutPin;	// for extension bistream

	CMemRingAllocator*				m_pInputAllocator;
	CMemRingAllocator*				m_pOutputAllocator;
	CMemRingAllocator*				m_pExtOutputAllocator;

//	IReadPointerHandle			*m_pReadPointerHandle;
//	CCritSec					SemInput;

};



/** 
**********************************************************************************

class CAudioDecInputPin

**********************************************************************************
*/

class CAudioEncInputPin : public CAudioMultiInputPin
{
	friend class CAudioEncFilter;
public:

    CAudioEncInputPin(long pinID)
		: CAudioMultiInputPin() { m_pinID = pinID; };

	virtual ~CAudioEncInputPin(void) {};	
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

class CAudioEncOutputPin : public CAudioOutputPin
{    

	friend class CAudioEncFilter;
public:

    CAudioEncOutputPin(long pinID) { m_pinID = pinID; };

	HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

private:

};	


/** 
**********************************************************************************

**********************************************************************************
*/


#endif

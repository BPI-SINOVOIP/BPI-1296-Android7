/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CAudioDecoder.h
//
//  @brief Header file for DVD-VENUS Audio Decoder Filter Class interface define
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_DECODER_FILTER_H__
#define __C_AUDIO_DECODER_FILTER_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include "StreamClass/SType.h"
#include "StreamClass/EType.h"

#include "CAudioPin.h"
#include "CAudioFilter.h"

typedef ulDArray<CMediaType*>            MediaTypeArr;


/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////


class   CAudioDecInputPin;
class   CAudioDecOutputPin;

/** 

  class CAudioDecFilter : public CAudioFilter

*/

class CAudioDecFilter : public CAudioTransFilter,
                        public IFilterSeeking
{
    friend class CAudioDecInputPin;
    friend class CAudioDecOutputPin;
	
public:

	CAudioDecFilter(void);
	virtual ~CAudioDecFilter() {};
	virtual void SetConfigure(char* pCFG, int length) PURE;

    HRESULT                     FilterType(
                                    FILTER_TYPE*            pFilterType
                                );

    HRESULT                     CheckCapabilities(
                                    FILTER_SEEKING_CAP*     pSeekingCap
                                );
    HRESULT                     SetSeekingInfo(
                                    long                    Speed,
                                    long                    Skip
                                );
    HRESULT                     GetSeekingInfo(
                                    long*                   pSpeed,
                                    long*                   pSkip
                                );
protected:
													
//	IReadPointerHandle			*m_pReadPointerHandle;
//	CCritSec					SemInput;

};



/** 
**********************************************************************************

class CAudioDecInputPin

**********************************************************************************
*/

class CAudioDecInputPin : public CAudioInputPin
{
	friend class CAudioDecFilter;
public:

    CAudioDecInputPin() {};
    virtual ~CAudioDecInputPin() {};

private:

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

    CAudioDecOutputPin()
		: CAudioMultiOutputPin() {};
	virtual ~CAudioDecOutputPin(void) {};	

	HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

private:

};	


/** 
**********************************************************************************

**********************************************************************************
*/


#endif

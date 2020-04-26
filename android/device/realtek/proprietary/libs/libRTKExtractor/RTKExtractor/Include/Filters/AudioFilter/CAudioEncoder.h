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

class CAudioEncFilter : public CAudioTransFilter
{
    friend class CAudioEncInputPin;
    friend class CAudioEncOutputPin;
	
public:

	CAudioEncFilter(void);

protected:
													
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

	CAudioEncInputPin()
		: CAudioMultiInputPin() {};
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



	HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

private:

};	


/** 
**********************************************************************************

**********************************************************************************
*/


#endif

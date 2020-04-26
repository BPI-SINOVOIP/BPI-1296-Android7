  

/**
// ///////////////////////////////////////////////////////////////////////////
//	@file CAudioOut.h
//
//  @brief Header file for DVD-VENUS Audio Out Filter Class interface define
//
//	@author Taro     20041115
//
//	copyright (c)RealTek.   All rights reserved.
// ///////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_OUT_FILTER_H__
#define __C_AUDIO_OUT_FILTER_H__

#include <CStream.h>
#include <Util.h>

#include "Filters/AudioFilter/AudioType.h"
#include "Filters/AudioFilter/CMultiPin.h"
///////////////////////////////////////////////////////////////////


class IAudioOutControl
{
public:
	// RPC call
	virtual	HRESULT					OpenDevice()			PURE;
	virtual	HRESULT					SetOutputCH()			PURE;
	virtual	HRESULT					SetSamplingRate()		PURE;
	virtual HRESULT					SetSamplingBit()		PURE;
	
};


////////////////////////////////////////////////////////////////////
class CAudioOutInputPin;

class CAudioOutFilter : public CBaseFilter
						//,public IAudioOutControl
{

public:

	CAudioOutFilter();
	virtual ~CAudioOutFilter();

    HRESULT                     FilterType(
                                    FILTER_TYPE*            pFilterType
                                );

	HRESULT							Run(void);
	HRESULT							Pause(FILTER_STATE State);
	HRESULT							Stop(void);

protected:

	static void*					MainProcessThread(void* pData);
    virtual bool					MainProcess(void) PURE;

protected:

	CThread*						m_pThread;

	CAudioOutInputPin*				m_pInPin;
	CMemRingAllocator*				m_pInputAllocator;

private:

};

///////////////////////////////////////////////////////////////

class CAudioOutInputPin : public  CBaseInputPin
{
public:

	CAudioOutInputPin();
	HRESULT							EndOfStream(unsigned int eventID);
	virtual bool					IsEndOfStream();
	virtual void					ResetEndOfStream();
	
protected:
	long							m_Flag;

};


class CAudioOutMultiInPin: public CMultiInputPin<CAudioOutInputPin>
{
public:

};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


#endif 



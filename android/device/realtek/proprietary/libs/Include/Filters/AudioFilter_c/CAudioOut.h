   

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

#include "CAudioFilter.h"
#include "AudioType.h"
#include "CMultiPin.h"

////////////////////////////////////////////////////////////////////
class CAudioOutInputPin;
class CAudioOutOutputPin;

class CAudioOutFilter : public CAudioFilter
						//,public IAudioOutControl
{

    friend class CAudioOutInputPin;
    friend class CAudioOutOutputPin;


public:

	CAudioOutFilter();
	virtual ~CAudioOutFilter();

    HRESULT							FilterType(
										FILTER_TYPE*            pFilterType
									);

	HRESULT							Run(void);
	HRESULT							Pause(FILTER_STATE State);
	HRESULT							Stop(void);

	virtual HRESULT					SetFocus();
	virtual HRESULT					SetUnFocus();

	HRESULT							ProcessPrivateInfo(int infoId, BYTE* pData, long length);

	virtual HRESULT					SetBilingual(int lingaul);
	virtual HRESULT					SetVolume(int volume);
	virtual HRESULT					SetSPDIFSource(int L, int R);
	virtual HRESULT					DropSample(long PTS);

protected:

	HRESULT							ProcessAgentPrivateInfo(BYTE* pData, long length);
	// handle RPC call of end of stream
	HRESULT							EndOfStream(long pinID, long eventID);
	HRESULT							DeliverFlush(long pinID);
	HRESULT							InitAgentRingBuffer();
	virtual HRESULT					SetupAgentConnection(IPin* pConnectedPin);
	HRESULT							SetOutConnetedPin(IMemInputPin* pin) { m_pOutConnectedPin = pin; return S_OK;}


	CAudioOutInputPin*				m_pInPin;

	// connect to encoder
	CAudioOutOutputPin*				m_pOutPin;

	CMemRingAllocator*				m_pInputAllocator;
	CMemRingAllocator*				m_pOutputAllocator;
	
	CBaseInputPin*					m_pCHInPinArr[AUDIO_DEC_OUTPIN];
	CBaseOutputPin*					m_pCHOutPinArr[AUDIO_DEC_OUTPIN];

	CMemRingAllocator*				m_pInAllocatorArr[AUDIO_DEC_OUTPIN];
	CMemRingAllocator*				m_pOutAllocatorArr[AUDIO_DEC_OUTPIN];


	// really used allocators
	CMemRingAllocator*				m_pUsedInputAllocator;
	CMemRingAllocator*				m_pUsedOutputAllocator;

	IReadPointerHandle*				m_pInputReadHdl;
	
	static long						m_aoutInstanceID;
	static long						m_aoutInstanceNum;

	bool                            m_Init;

	IMemInputPin*					m_pOutConnectedPin;

};

/** 
**********************************************************************************

class CAudioOutInputPin

**********************************************************************************
*/

class CAudioOutInputPin : public  CAudioMultiInputPin
{

	friend class CAudioOutFilter;

public:

    CAudioOutInputPin(long pinID) { m_pinID = pinID; };
    virtual ~CAudioOutInputPin() {};

	// call RPC endofstream to peer agent
	HRESULT						EndOfStream(unsigned int eventID);
    HRESULT                     Flush(void);
	HRESULT						Receive(BYTE* pWritePointer, long WriteBufferSize);
	//HRESULT						PrivateInfo(int infoId, BYTE* pInfo, int length);
protected:

};

/** 
**********************************************************************************

class CAudiooutOutputPin

**********************************************************************************
*/
// several output pin is wrapped in the class

class CAudioOutOutputPin : public CAudioMultiOutputPin
{    

	friend class CAudioOutFilter;
public:

    CAudioOutOutputPin(long pinID)
		: CAudioMultiOutputPin() { m_pinID = pinID; };
	virtual ~CAudioOutOutputPin(void) {};	

	HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

protected:
	HRESULT						GetConnectedInputPin(int n, IMemInputPin** ppPin);								

};	

#endif 



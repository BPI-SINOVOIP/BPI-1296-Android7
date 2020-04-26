 

/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CAudioFilter.h
//
//  @brief Header file for DVD-VENUS Audio Filter Class interface define
//
//	@author Taro     08/31/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_FILTER_H__
#define __C_AUDIO_FILTER_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include <CStream.h>
#include "AudioType.h"
#include "CAudioPin.h"
#include "RPC.h"
#include "CMultiPin.h"
/** 
**********************************************************************************

	CAudioFilter class
**********************************************************************************
*/


class CSystemRPCCall
{
public:
	// RPC call
	virtual	HRESULT					UpdateReadPtr(RPC_SENDPIN_LONG* pReadPack)			PURE;
	virtual	HRESULT					UpdateWritePtr(RPC_SENDPIN_LONG* pWritePack)		PURE;
	virtual HRESULT					ProcessAgentPrivateInfo(BYTE* pData, long length)	PURE;

};

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class CAudioFilter : public CBaseFilter,
					 public CSystemRPCCall
{
	friend class CAudioInputPin;
	friend class CAudioOutputPin;

public:	

	// Override AddPin for output pin

	CAudioFilter();
	virtual ~CAudioFilter();

		// RPC call
	HRESULT							UpdateReadPtr(RPC_SENDPIN_LONG* pReadPack);
	HRESULT							UpdateWritePtr(RPC_SENDPIN_LONG* pWritePack);
	HRESULT							ProcessAgentPrivateInfo(BYTE* pData, long length);

protected:

	virtual	long					GetAgentInstanceID()
										{ return m_agentInstanceID; };

	virtual HRESULT					InitAgentRingBuffer(CBasePin* pPin);
	virtual HRESULT					CreateAgent(AUDIO_FILTER_TYPE type, long instanceID = AUDIO_UNDEFINED_AGENTID);


	// handle RPC call of end of stream
	virtual void					EndOfStream();

protected:

	long							m_agentInstanceID;


};


/////////////////////////////////////////////////////////////

/** 
**********************************************************************************

	CAudioTransFilter class
**********************************************************************************
*/

class CAudioTransFilter : public CAudioFilter
{
	friend class CAudioInputPin;
	friend class CAudioOutputPin;

public:	

	// Override AddPin for output pin

	CAudioTransFilter();
	virtual ~CAudioTransFilter();
	
    // Override Stop(). 
    HRESULT							Stop(void);

    //Override Pause().
    HRESULT							Pause(FILTER_STATE State);

    //Override Run. 
    HRESULT							Run(void);

protected:
	
protected:

    CAudioInputPin*					m_pInPin;
    CAudioOutputPin*				m_pOutPin;
    CAudioInputPin*					m_pExtInPin;	// for extension bistream

	CMemRingAllocator*				m_pInputAllocator;
	CMemRingAllocator*				m_pOutputAllocator;
	CMemRingAllocator*				m_pExtInputAllocator;

};

#endif

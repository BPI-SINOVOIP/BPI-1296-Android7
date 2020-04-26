 
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

#ifndef __C_AUDIO_AGENT_H__
#define __C_AUDIO_AGENT_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include "CStream.h"
#include "AudioType.h"
#include "CAudioPin.h"
#include "CAudioFilter.h"

/** 
class CRPCCall
*/

class CRPCCall
{
public:	

	virtual HRESULT					InitRingBuffer(RPC_RINGBUFFER* pRingBuffer)		PURE;
	virtual HRESULT					UpdateWritePtr(RPC_SENDPIN_LONG* pWritePack)	PURE;
	virtual	HRESULT					UpdateReadPtr(RPC_SENDPIN_LONG* pReadPack)		PURE;
	virtual	HRESULT					ProcessFilterPrivateInfo(BYTE* pData, long length) PURE;


};

/** 
class CAudioAgent
*/
class CAudioAgent : public CBaseFilter,
					public CRPCCall
{

	friend class CAudioAgentInputPin;
	friend class CAudioAgentOutputPin;

public:
	CAudioAgent(long* ID);
	virtual ~CAudioAgent();
	
	HRESULT							InitRingBuffer(RPC_RINGBUFFER* pRingBuffer);
	HRESULT							UpdateWritePtr(RPC_SENDPIN_LONG* pWritePack);
	HRESULT							UpdateReadPtr(RPC_SENDPIN_LONG* pReadPack);
	HRESULT							ProcessFilterPrivateInfo(BYTE* pData, long length);

protected:

	static void*					MainProcessThread(void* pData);
    virtual bool					MainProcess(void) PURE;

protected:

	long							m_instanceID;
	CThread*						m_pThread;
private:

	static long						GetUniqueID();
	static long						m_uniqueID;
};




class CAudioTransAgent : public CAudioAgent
{
	friend class CAudioAgentInputPin;
	friend class CAudioAgentOutputPin;

public:

	CAudioTransAgent(long* ID);
	virtual ~CAudioTransAgent();
	
	HRESULT							Run(void);
    HRESULT							Pause(FILTER_STATE State);
    HRESULT							Stop(void);


protected:

protected:

    CAudioAgentInputPin*			m_pInPin;
    CAudioAgentOutputPin*			m_pOutPin;
    CAudioAgentInputPin*			m_pExtInPin;


	CMemRingAllocator*				m_pInputAllocator;
	CMemRingAllocator*				m_pOutputAllocator;
	CMemRingAllocator*				m_pExtInputAllocator;

};



#endif


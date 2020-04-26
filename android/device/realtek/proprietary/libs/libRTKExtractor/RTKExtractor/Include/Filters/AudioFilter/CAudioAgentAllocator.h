

/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CAudioAgentAllocator.h
//
//	@brief Header file for DVD-VENUS Audio Agent Ring Allocator Class
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/


#ifndef __C_AUDIO_AGENT_ALLOCATOR_H__
#define __C_AUDIO_AGENT_ALLOCATOR_H__


///////////////////////////////////////////////////////////

/**

	class CAudioAgentRingAllocator : CMemRingAllocator
 
*/

#include "CAudioAgentPin.h"
#include "CMultiAllocator.h"


class CAudioAgentReadPointerHandle : public CReadPointerHandle
{
	friend	class CAudioAgentBaseRingAllocator;
public:
	CAudioAgentReadPointerHandle(CMemRingAllocator* pRingAllocator)
		: CReadPointerHandle(pRingAllocator) {};

protected:

};

// remove some assert if number of readptr is 0;
class CAudioAgentBaseRingAllocator : public CMemRingAllocator
{
public:

	CAudioAgentBaseRingAllocator() {};
	virtual ~CAudioAgentBaseRingAllocator() {};
	
	HRESULT                     GetWritePointer(BYTE** ppWritePointer, long* pWriteBufferSize);
    HRESULT                     SetWritePointer(BYTE* pWritePointer);
	HRESULT						GetReadPointerHandle(IReadPointerHandle**	ppReadPointer,
													IMemInputPin*			pInputPin);

protected:
    long                        GetWriteSize(void);

};


//////////////////////////////////////////////////////////////

class CAudioAgentRingAllocator;
typedef CMultiRingAllocator<CAudioAgentRingAllocator,CAudioAgentReadPointerHandle>	CAudioAgentMultiAllocator;


class CAudioAgentRingAllocator : public CAudioAgentBaseRingAllocator
{
	friend class CAudioAgentInputPin;
	friend class CAudioAgentOutputPin;

public:
	CAudioAgentRingAllocator();
	virtual ~CAudioAgentRingAllocator() {};
    HRESULT                     SetBuffer(BYTE* pBuffer, long Size);

protected:
	bool						IsRingBufferCreated();

private:
	long						m_Flag;

};


#endif

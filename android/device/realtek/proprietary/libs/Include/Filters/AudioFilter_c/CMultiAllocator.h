          
/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CMultiAllocator.h
//
//	@brief Header file for DVD-VENUS Multiple Ring Allocator Class
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

/*********** HISTORY BUG LIST ************************************************
   
	- 20041026: change some function of CMemRingAllocator::function to T::function,
				or cause implicit bug..

*******************************************************************************
*/

#ifndef __C_MULTI_ALLOCATOR_H__
#define __C_MULTI_ALLOCATOR_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include <CStream.h>
#include "StreamClass/SType.h"
#include "StreamClass/EType.h"

typedef ulDArray<IMemRingAllocator*>	IRingAllocatorArr;

//////////////////////////////////////////////////////////////////////////////////////////

// only used for allocate buffer, not set r/w pointer, for updating r/w pointer by shared memory
class CMultiAllocator
{
public:

	CMultiAllocator();
	virtual ~CMultiAllocator();
	

	// not check if write to unread area	
	//HRESULT						WriteInnerData(unsigned int idx, BYTE* pDesPtr, BYTE* pSrcPtr, int size);
	HRESULT                     GetInnerAllocator(unsigned int idx, IMemAllocator** ppAllocator);
	unsigned int		        GetAllocatorCount() 
										{ return m_AllocatorArray.Size(); };	
	
	virtual HRESULT				AppendAllocator(IMemRingAllocator* pAllocator) PURE;
	virtual HRESULT				RemoveAllocator(IMemRingAllocator* pAllocator) PURE;

protected:

	IRingAllocatorArr			m_AllocatorArray;	
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

class CMultiRingAllocator :	public CMemRingAllocator, 
							public CMultiAllocator	 
{
public:

	CMultiRingAllocator(RING_BUFFER_STYLE style)
		: CMemRingAllocator(style),
		  CMultiAllocator() {};
	
//////////////////////////////////////////////////////////////////////////////////////////
	    // IMemAllocator interface
	HRESULT                     Commit(void);
    HRESULT                     Decommit(bool bFreeBuffer= false);

    HRESULT                     SetBuffer(
                                    BYTE*                   pBuffer,
                                    long                    Size
                                );

	// should not call GetReadPointerHandle, SetWritePointer, SetReadPointer ..

	HRESULT						AppendAllocator(IMemRingAllocator* pAllocator);
	HRESULT						RemoveAllocator(IMemRingAllocator* pAllocator);

protected:
	
};

#endif

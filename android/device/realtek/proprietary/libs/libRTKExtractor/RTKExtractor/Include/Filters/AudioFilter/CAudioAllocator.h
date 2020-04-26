        
/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CAudioAllocator.h
//
//	@brief Header file for DVD-VENUS Audio Ring Allocator Class
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_ALLOCATOR_H__
#define __C_AUDIO_ALLOCATOR_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include <CStream.h>
#include "StreamClass/SType.h"
#include "StreamClass/EType.h"

#include "CMultiAllocator.h"


class CAudioRingAllocator;

typedef CMultiRingAllocator<CAudioRingAllocator,CAudioReadPointerHandle>	CAudioMultiRingAllocator;

/** 

  typedef HRESULT (*RdPtrChangedCallBack)(BYTE* pReadPointer, void* pData);

*/

typedef HRESULT (*PtrChangedCallBack)(BYTE* pPointer, void* pData);


/** 

  RCallBackPin_T

*/

typedef struct
{
    PtrChangedCallBack				pCallBackFunc;
    IPin*							pPin;
} CallBackPin_T;

typedef ulDArray<CallBackPin_T*>		CallbackPinArr;

class CAudioReadPointerHandle;

/** 

  class CAudioRingAllocator

*/

class CAudioRingAllocator : public CMemRingAllocator
{
	friend class CAudioReadPointerHandle;
public:

	CAudioRingAllocator()
		: m_wtCallbackPinArray(2),
		m_rdCallbackPinArray(2), 		  
		  m_LastReadPointer(NULL) {};

	virtual ~CAudioRingAllocator();

	virtual HRESULT				RegRdPtrChangedCallback(IPin* pPin, PtrChangedCallBack rdPtrCallback);
	virtual HRESULT				RegWtPtrChangedCallback(IPin* pPin, PtrChangedCallBack wtPtrCallback);

	
	HRESULT                     GetReadPointerHandle(IReadPointerHandle** ppReadPointer, IMemInputPin* pInputPin);
	HRESULT						SetWritePointer(BYTE* pWritePointer);


protected:

	virtual HRESULT				ReceiveRdPtrChanged();
	virtual HRESULT				ReceiveWtPtrChanged();

	CallbackPinArr				m_wtCallbackPinArray;
	CallbackPinArr				m_rdCallbackPinArray;
	BYTE*						m_LastReadPointer;
};

//////////////////////////////////////////////////////////////////////////////////

/** 

  class CAudioReadPointerHandle 

 */

class CAudioReadPointerHandle : public CReadPointerHandle
{
	friend	class CAudioRingAllocator;
public:
	CAudioReadPointerHandle(CMemRingAllocator* pRingAllocator)
		: CReadPointerHandle(pRingAllocator) {};

	virtual ~CAudioReadPointerHandle() {};
	
    HRESULT						SetReadPointer(BYTE* pReadPointer);

protected:

};


#endif

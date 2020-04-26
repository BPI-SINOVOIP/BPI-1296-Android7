          
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

class CAudioReadPointerHandle;

//////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

template<class T>
class CMultiReadPointerHandle : public T
{

public:
	CMultiReadPointerHandle(CMemRingAllocator* pRingAllocator, IMemInputPin* pInPin)
		: T(pRingAllocator),
		  m_pInputPin(pInPin){};
	

    HRESULT						SetReadPointer(BYTE* pReadPointer);
	void                        Active(void);
	bool                        IsActive(void);

protected:
	IMemInputPin*				m_pInputPin;
};

template<class T>
HRESULT CMultiReadPointerHandle<T>::SetReadPointer(BYTE* pReadPointer)
{

	T::SetReadPointer(pReadPointer);

	BYTE*	pBuffer;
	long	size;
	long	offset;
	m_pAllocator->GetBuffer(&pBuffer,&size);
	offset = pReadPointer - pBuffer;

	CMultiAllocator* pCplxAllocator = dynamic_cast<CMultiAllocator*>(m_pAllocator);
	if (pCplxAllocator != NULL)
	{

		IMemAllocator* pInnerAllocator;
		IReadPointerHandle*	pInnerRdPtrHandle;
		for (unsigned int i = 0; i < pCplxAllocator->GetAllocatorCount(); i++)
		{
			pCplxAllocator->GetInnerAllocator(i,&pInnerAllocator);
			dynamic_cast<IMemRingAllocator*>(pInnerAllocator)->
				GetReadPointerHandle(&pInnerRdPtrHandle,m_pInputPin);
			dynamic_cast<IMemRingAllocator*>(pInnerAllocator)->GetBuffer(&pBuffer,&size);
			pInnerRdPtrHandle->SetReadPointer(pBuffer+offset);
		}		

	}

	return S_OK;

}


template<class T>
void CMultiReadPointerHandle<T>::Active(void)
{
	T::Active();
}

template<class T>
bool CMultiReadPointerHandle<T>::IsActive(void)
{
	return T::IsActive();
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

template<class T, class T1>
class CMultiRingAllocator :	public T, 
								public CMultiAllocator	 
{
public:

	CMultiRingAllocator()
		: CMultiAllocator() {};
	
//////////////////////////////////////////////////////////////////////////////////////////
	    // IMemAllocator interface
	HRESULT                     Commit(void);
    HRESULT                     Decommit(bool bFreeBuffer= false);

	// set write pointer of inner allocator to relative address
    HRESULT                     SetWritePointer(BYTE* pWritePointer);

    HRESULT                     GetReadPointerHandle(IReadPointerHandle** ppReadPointer, IMemInputPin* pInputPin);
/*
    HRESULT                     ReleaseReadPointerHandle(
                                    IReadPointerHandle*     pReadPointer
                                );
*/

	HRESULT						AppendAllocator(IMemRingAllocator* pAllocator);
	HRESULT						RemoveAllocator(IMemRingAllocator* pAllocator);


protected:

	
};


template<class T, class T1>
HRESULT CMultiRingAllocator<T,T1>::SetWritePointer(BYTE* pWritePointer)
{	
	HRESULT hr = T::SetWritePointer(pWritePointer);
	BYTE*	pLower;
	long	size;
	long	offset;


	T::GetBuffer(&pLower,&size);
	offset = pWritePointer-pLower;

	for(unsigned int i = 0; i < (unsigned int)m_AllocatorArray.Size(); i++)
	{
		m_AllocatorArray[i]->GetBuffer(&pLower,&size);
		m_AllocatorArray[i]->SetWritePointer(pLower+offset);
	}

	return hr;
}


template<class T, class T1>
HRESULT CMultiRingAllocator<T,T1>::Commit()
{	
	HRESULT hr = T::Commit();

	for(unsigned int i = 0; i < (unsigned int)m_AllocatorArray.Size(); i++)
	{
		m_AllocatorArray[i]->Commit();
	}
	return hr;
}

template<class T, class T1>
HRESULT CMultiRingAllocator<T,T1>::Decommit(bool bFreeBuffer)
{			
	HRESULT hr = T::Decommit(bFreeBuffer);
	for(unsigned int i = 0; i < (unsigned int)m_AllocatorArray.Size(); i++)
	{
		m_AllocatorArray[i]->Decommit(bFreeBuffer);
	}
	return hr;
}

template<class T, class T1>
HRESULT CMultiRingAllocator<T,T1>::GetReadPointerHandle(IReadPointerHandle** ppReadPointer, IMemInputPin* pInputPin)
{
	if (IsCommit() != S_OK)
	{
        DASSERT(0);
        return E_FAIL;
    }
    // CAutoLock           AutoLock(&m_Lock);
    m_Lock.Lock();
    *ppReadPointer=     NULL;

    CHECKPOINTER(ppReadPointer, E_GEN_POINTER_NULL);
    CHECKPOINTER(pInputPin, E_GEN_POINTER_NULL);

    int         nReadPointer=   m_RPointerPinArr.Size();
    int         i;
    RPointerPin_T*              pRPointerPin;

    for (i= 0; i< nReadPointer; i++){
        pRPointerPin=   (RPointerPin_T*)m_RPointerPinArr[i];
        if (pRPointerPin== NULL)
            continue;
        if (pRPointerPin->pInputPin== pInputPin){
            // pRPointerPin->pReadPointer->SetReadPointer(m_pWritePointer);
            // pRPointerPin->pReadPointer->Active();
            *ppReadPointer= pRPointerPin->pReadPointer;
            // return S_OK;
        }
    }

    if (NULL== *ppReadPointer){
        pRPointerPin= new RPointerPin_T;
		*ppReadPointer = pRPointerPin->pReadPointer = new CMultiReadPointerHandle<T1>(this,pInputPin);
        pRPointerPin->pInputPin=    pInputPin;
        m_RPointerPinArr.Append(pRPointerPin);
        m_Lock.Unlock();
    }
    else{
        m_Lock.Unlock();
        if (false== dynamic_cast<CMultiReadPointerHandle<T1>* >(pRPointerPin->pReadPointer)->IsActive()){
            pRPointerPin->pReadPointer->SetReadPointer(m_pWritePointer);
            dynamic_cast<CMultiReadPointerHandle<T1>* >(pRPointerPin->pReadPointer)->Active();
        }
    }
    return S_OK;

	
/*	if (IsCommit() != S_OK)
	{
        DASSERT(0);
        return E_FAIL;
    }
    CAutoLock           AutoLock(&m_Lock);

    CHECKPOINTER(ppReadPointer, E_GEN_POINTER_NULL);
    CHECKPOINTER(pInputPin, E_GEN_POINTER_NULL);

    int         nReadPointer=   m_RPointerPinArr.Size();
    int         i;
    RPointerPin_T*              pRPointerPin;

    for (i= 0; i< nReadPointer; i++)
	{
        pRPointerPin=   (RPointerPin_T*)m_RPointerPinArr[i];
        if (pRPointerPin== NULL)
            continue;
        if (pRPointerPin->pInputPin== pInputPin)
		{
            dynamic_cast<CMultiReadPointerHandle<T1>* >(pRPointerPin->pReadPointer)->Active();
            *ppReadPointer= pRPointerPin->pReadPointer;
            // *ppReadPointer->Active();
            return S_OK;
        }
    }

    pRPointerPin = new RPointerPin_T;
    *ppReadPointer = pRPointerPin->pReadPointer = new CMultiReadPointerHandle<T1>(this,pInputPin);
    pRPointerPin->pInputPin = pInputPin;
    m_RPointerPinArr.Append(pRPointerPin);
    return S_OK;
*/
}



template<class T, class T1>
HRESULT CMultiRingAllocator<T,T1>::AppendAllocator(IMemRingAllocator* pAllocator)
{
	m_AllocatorArray.Append(pAllocator);

	return S_OK;
}

template<class T, class T1>
HRESULT CMultiRingAllocator<T,T1>::RemoveAllocator(IMemRingAllocator* pAllocator)
{
	m_AllocatorArray.Remove(pAllocator);

	return S_OK;
}


typedef CMultiRingAllocator<CMemRingAllocator,CReadPointerHandle>	CBaseMultiRingAllocator;

#endif

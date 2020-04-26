 /**
/////////////////////////////////////////////////////////////////////////////////
//	@file CMultiPin.h
//
//  @brief Header file for DVD-VENUS Multi Pin Class interface define
//
//	Functions of this header file includes
//
//	1. A pin wrapper which controls multiple pin
//
//	2. Determine to connect to downstream filter or not by downstream filter type, if it is
//		audio filter too, call RPC to ghost filter
//
//	@author Taro     08/23/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_MULTI_PIN_H__
#define __C_MULTI_PIN_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include <CStream.h>
#include "StreamClass/SType.h"
#include "StreamClass/EType.h"

#include "AudioType.h"
#include "CMultiAllocator.h"

typedef ulDArray<IPin*>					IPinArr;

////////////////////////////////////////////////////////////////////////
/**

  Multi Pin Class
  control multiple pins inside
*/
////////////////////////////////////////////////////////////////////////

class CMultiPin
{
public:

	CMultiPin() :
		m_PinArray(2) {};
	virtual ~CMultiPin();

	HRESULT                     GetInnerPin(unsigned int idx, IPin** ppPin);
	unsigned int		        GetPinCount()
										{ return m_PinArray.Size(); };

	virtual HRESULT				AppendPin(IPin* pPin) PURE;
	virtual HRESULT				RemovePin(IPin* pPin) PURE;

protected:

	virtual HRESULT             ConnectMainPin(IPin *pReceivePin,const MEDIA_TYPE *pmt) PURE;
    virtual HRESULT             DisconnectMainPin() PURE;

	IPinArr						m_PinArray;
	int							m_ActivePinCount;
};


////////////////////////////////////////////////////////////////////////
/**

  Multi Input Pin Class

  this class has multiple instance of CBaseInputPin, and itself is
  also some derive class of CBaseInputPin

*/
////////////////////////////////////////////////////////////////////////

template <class T>
class CMultiInputPin :  public CMultiPin,
						  public T

{
public:

	CMultiInputPin();
	virtual ~CMultiInputPin() {};

	// set allocator to inner pin
    HRESULT                     SetAllocator(IMemAllocator*  pAllocator);

	// disconnect all inner pin
	HRESULT                     Disconnect(void);


	HRESULT						AppendPin(IPin* pPin);
	HRESULT						RemovePin(IPin* pPin);

protected:

	// return connected pin count
	unsigned int				GetConnectedPinCount();

    HRESULT                     ConnectMainPin(IPin *pReceivePin,const MEDIA_TYPE *pmt)
								{	return W_FUNC_MIS_USED; };

    HRESULT                     DisconnectMainPin()
								{	return T::Disconnect(); };



};

/**

template <class T>
CMultiInputPin<T>::CMultiInputPin(unsigned int PinNum)

*/
template <class T>
CMultiInputPin<T>::CMultiInputPin()
{
}

template <class T>
HRESULT CMultiInputPin<T>::AppendPin(IPin* pPin)
{

	if (IS_PIN_CONNECTED(this))
		return E_FAIL;

	m_PinArray.Append(pPin);

	return S_OK;
}

template <class T>
HRESULT CMultiInputPin<T>::RemovePin(IPin* pPin)
{

	if (IS_PIN_CONNECTED(this))
		return E_FAIL;

	m_PinArray.Remove(pPin);

	return S_OK;
}

/**
	set alloctor to child input pins
*/

template <class T>
HRESULT CMultiInputPin<T>::SetAllocator(IMemAllocator*  pAllocator)
{
	unsigned int nSize = (unsigned int)m_PinArray.Size();

	T::SetAllocator(pAllocator);
	CMultiAllocator *pMultiAllocator = (dynamic_cast<CMultiAllocator*>(pAllocator));

	DASSERT(pMultiAllocator != NULL);

	if (pMultiAllocator->GetAllocatorCount() < nSize) {
	            nSize = ((pMultiAllocator->GetAllocatorCount() < nSize) ? pMultiAllocator->GetAllocatorCount() : nSize);
		//return E_FAIL;
        }

	for(unsigned int i = 0; i < nSize; i++)
	{
		IMemAllocator* pTmpAllocator;
		pMultiAllocator->GetInnerAllocator(i,&pTmpAllocator);
		m_PinArray[i]->SetAllocator(pTmpAllocator);
	}

	return S_OK;
}

template <class T>
unsigned int CMultiInputPin<T>::GetConnectedPinCount()
{
	unsigned int i;
	unsigned int nSize = (unsigned int)m_PinArray.Size();
	for(i = 0; i < nSize; i++)
	{
		if (!IS_PIN_CONNECTED(m_PinArray[i]))
			break;
	}
	return i;
}

// override disconnect
template <class T>
HRESULT CMultiInputPin<T>::Disconnect()
{
	HRESULT hr;

	if ((hr = DisconnectMainPin()) != S_OK)
		return hr;

	unsigned int pinCount = GetConnectedPinCount();
	for(unsigned int i = 0; i < pinCount; i++)
	{
		m_PinArray[i]->Disconnect();
	}

	return S_OK;
}

////////////////////////////////////////////////////////////////////////
/**

  Multi Output Pin Class

  this class has multiple instance of CBaseInputPin, and itself is also
  derive class of CBaseOutputPin

*/
////////////////////////////////////////////////////////////////////////
// several output pin is wrapped in the class

template <class T>
class CMultiOutputPin :	public CMultiPin,
							public T
{

public:

    CMultiOutputPin();
	virtual ~CMultiOutputPin() {};

    // Connect all inner pin
    HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

//	bool						test(CBaseFilter* pFilter, char* Id);

	//////////////////////////////////////////////////////////////////////////////
	HRESULT						AppendPin(IPin* pPin);
	HRESULT						RemovePin(IPin* pPin);

protected:

    HRESULT                     ConnectMainPin(IPin *pReceivePin,const MEDIA_TYPE *pmt)
								{
									if (dynamic_cast<CMultiPin*>(pReceivePin) == NULL)
										return E_GEN_UNEXPECTED;
									return T::Connect(pReceivePin,pmt);
								};
    HRESULT                     DisconnectMainPin()
								{	return T::Disconnect(); };

};


template <class T>
CMultiOutputPin<T>::CMultiOutputPin()
{
}


template <class T>
HRESULT CMultiOutputPin<T>::AppendPin(IPin* pPin)
{

	if (IS_PIN_CONNECTED(this))
		return E_FAIL;

	m_PinArray.Append(pPin);


	return S_OK;
}

template <class T>
HRESULT CMultiOutputPin<T>::RemovePin(IPin* pPin)
{

	if (IS_PIN_CONNECTED(this))
		return E_FAIL;

	m_PinArray.Remove(pPin);

	return S_OK;
}


template <class T>
HRESULT CMultiOutputPin<T>::Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt)
{

	HRESULT			hr;
	IPin*			pReceiveInnerPin;

	if ((hr = ConnectMainPin(pReceivePin,pmt)) != S_OK)
		return hr;

	CMultiPin*	pReceivePCMPin = dynamic_cast<CMultiPin*>(pReceivePin);

//	CMultiPin* pReceivePCMPin;
	DASSERT(pReceivePCMPin != NULL);

	DASSERT (pReceivePCMPin->GetPinCount() == (unsigned int)m_PinArray.Size());
	//unsigned int pinCount = min(pReceivePCMPin->GetPinCount(),(unsigned int)m_PinArray.Size());
	// connect inner pins
	unsigned pinCount = (unsigned int)m_PinArray.Size();
	for(unsigned int i = 0; i < pinCount; i++)
	{
		pReceivePCMPin->GetInnerPin(i,&pReceiveInnerPin);
		m_PinArray[i]->Connect(pReceiveInnerPin,pmt);
	}

	return S_OK;
}

#endif // __C_MULTI_PIN_H__

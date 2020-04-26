/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CAudioPin.h
//
//  @brief Header file for DVD-VENUS Audio Pin Class interface define
//
//	@author Taro     08/31/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/


#ifndef __C_AUDIO_PIN_H__
#define __C_AUDIO_PIN_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include <CStream.h>

#include "CMultiPin.h"
#include "AudioType.h"
#include "CAudioFilter.h"
#include "RPC.h"
#include "StreamClass/SType.h"


/** 
// T must be derived from 
// CBaseInputPin or CBaseOutputPin or CMultiInputPin or CMultiOutputPin
*/


class IAudioOutputPin;
class IAudioInputPin;
class CAudioInputPin;
class CAudioOutputPin;
class CAudioMultiOutputPin;

typedef CMultiInputPin<CBaseInputPin>			CMultiBaseInputPin;
typedef CMultiOutputPin<CBaseOutputPin>		CMultiBaseOutputPin;

/**
class IAudioPin
*/

class IAudioPin
{
public:
	virtual bool				IsAgentConnected()	PURE;

};

/**
class IAudioInputPin : public IAudioPin
*/

class IAudioInputPin : public IAudioPin
{
public:

protected:	
	virtual HRESULT				UpdateAgentWritePtr(BYTE* pWritePointer) PURE;

};

/**
class IAudioOutputPin : public IAudioPin
*/


class IAudioOutputPin : public IAudioPin
{
public:

protected:		
	virtual HRESULT				UpdateAgentReadPtr(BYTE* pReadPointer) PURE;

};

/**

class CAudioInputPin : public CBaseInputPin,
					   public IAudioInputPin

*/

class CAudioInputPin : public CBaseInputPin,
					   public IAudioInputPin
{
	friend class CAudioFilter;
	friend class CAudioTransFilter;

public:
	CAudioInputPin() {};

/*	// not override functions
	
    HRESULT                     ReceiveCanBlock(void);
    HRESULT                     GetAllocator(
                                    IMemAllocator**         ppAllocator
                                );

    HRESULT                     ConnectedTo(
                                    IPin***                 ppPinArr,
                                    int*                    pPinArrSize
                                );

*/
	// call RPC disconnect
	HRESULT                     Disconnect(void); 

	// register input pin to audio allocator, receive write pointer change event
	HRESULT                     SetAllocator(IMemAllocator*  pAllocator);

	// call RPC endofstream to peer agent
	HRESULT						EndOfStream(unsigned int eventID);


    HRESULT                     Receive(BYTE* pWritePointer, long WriteBufferSize);
    HRESULT                     Flush(void);

    HRESULT                     PrivateInfo(int infoId, BYTE* pInfo, int length);
    HRESULT                     PrivateInfo(BYTE* pInfo, int length);

	// agent connected check
	bool						IsAgentConnected();	
protected:		

        int64_t m_pts;

	// receive write point change event
	static HRESULT				WtPtrChangedHandler(BYTE* pReadPointer, void* pData);			

	// RPC call
	HRESULT						UpdateAgentWritePtr(BYTE* pWritePointer);

};

/**

class CAudioOutputPin : public CBaseOutputPin,
						public IAudioOutputPin

*/

class CAudioOutputPin : public CBaseOutputPin,
						public IAudioOutputPin
{

	friend class CAudioFilter;
	friend class CAudioTransFilter;

public:
	CAudioOutputPin() {};	

	// if two audio filters connects, send RPC call
	HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

	// disconnect
	HRESULT                     Disconnect(void);

	// register output pin to audio allocator, receive read pointer change event
	HRESULT                     SetAllocator(IMemAllocator*  pAllocator);

	// handle RPC enddof stream call
	HRESULT						EndOfStream(unsigned int eventID);

	// agent connected check
	bool						IsAgentConnected();

protected:
	static HRESULT				RdPtrChangedHandler(BYTE* pReadPointer, void* pData);			
	HRESULT						UpdateAgentReadPtr(BYTE* pReadPointer);


};

/**
	if connects both audio filter
		only Main Pin connected and call RPC call
	else
		acts like Multi ouput pin
*/

//CBaseOutputPin
class CAudioMultiOutputPin : public CMultiOutputPin<CAudioOutputPin>
{

public:
	
    CAudioMultiOutputPin() 
		: CMultiOutputPin<CAudioOutputPin>() {};
	virtual ~CAudioMultiOutputPin() {};

    //Override Connect. If agent connect, only connect the main pin
    HRESULT                     Connect(IPin *pReceivePin,const MEDIA_TYPE *pmt);

protected:
	//bool                JoinFilter(CBaseFilter* pFilter, char* Id);

};

//CBaseInputPin
class CAudioMultiInputPin : public CMultiInputPin<CAudioInputPin>
{

public:
	
    CAudioMultiInputPin() 
		: CMultiInputPin<CAudioInputPin>() {};
	virtual ~CAudioMultiInputPin() {};

protected:
	//bool				JoinFilter(CBaseFilter* pFilter, char* Id);

};

#endif  //__C_AUDIO_PIN_H__


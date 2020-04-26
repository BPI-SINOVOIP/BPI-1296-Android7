

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
#include "CMultiPin.h"

//#include "middleware/amw_rpc.h"

/** 
**********************************************************************************

	CAudioFilter class
**********************************************************************************
*/


// RPC call by agent

class CSystemRPCCall
{
public:
	// RPC call
	virtual HRESULT					ProcessAgentPrivateInfo(BYTE* pData, long length)	PURE;
	// handle RPC call of end of stream
	virtual HRESULT					EndOfStream(long pinID, long eventID)				PURE;
	virtual HRESULT					DeliverFlush(long pinID)			PURE;


protected:

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class CAudioFilter : public CBaseFilter,
					 public CSystemRPCCall,
					 public IFilterSeeking
{
	friend class CAudioInputPin;
	friend class CAudioOutputPin;

public:	

	// Override AddPin for output pin

	CAudioFilter();
	virtual ~CAudioFilter();

	// agent sent private info
	virtual HRESULT					ProcessAgentPrivateInfo(BYTE* pData, long length);
	// peer filter sent private info
	virtual HRESULT					ProcessPrivateInfo(BYTE* pData, long length);
	virtual HRESULT					ProcessPrivateInfo(int infoId, BYTE* pData, long length);

    HRESULT							CheckCapabilities(
										FILTER_SEEKING_CAP*     pSeekingCap
									);
	HRESULT		                     SetSeekingInfo(
						                long                    Speed,
							            long                    Skip
					                );
    HRESULT						    GetSeekingInfo(
									    long*                   pSpeed,
										long*                   pSkip
									);
	virtual	long					GetAgentInstanceID()
										{ return m_agentInstanceID; };

protected:

	virtual	void					SetAgentInstanceID(long instanceID)
										{ m_agentInstanceID = instanceID; };

	virtual HRESULT					CreateAgent(AUDIO_MODULE_TYPE type, long instanceID = AUDIO_UNDEFINED_AGENTID);
	virtual HRESULT					GetPinRingBufferHeaderList(CBasePin* pPin, long (*RBufHeaderList)[8], long& listSize);

	virtual HRESULT					InitAgentRingBuffer() PURE;


protected:

	long							m_agentInstanceID;

	// seek info
	long							m_speed;
    long							m_skip;

};

#endif

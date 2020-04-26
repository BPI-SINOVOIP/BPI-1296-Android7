
/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CVideoAgent.h
//
//  @brief Header file for 
//
//	@author Stream     12/28/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_VIDEO_FILTER_H__
#define __C_VIDEO_FILTER_H__

#include "CStream.h"
//#include <Filters/VideoDecoder/VideoType.h>
#include <Filters/VideoDecoder/VP_RPC.h>
#include <Filters/CFilterInstanceMap.h>


class CVideoFilter : public CBaseFilter
{


public:
	CVideoFilter() {};
	virtual ~CVideoFilter(){};
	
			  
			virtual HRESULT PostEOSEvent();
	virtual HRESULT	CreateAgent(VIDEO_VF_TYPE type, long instanceID);
			HRESULT GetAgentInstanceID(long* ID);
			HRESULT RemoveAgent();
	static CFilterInstanceMap m_instanceMap;

protected:
			long m_agentInstanceID;


};


#endif


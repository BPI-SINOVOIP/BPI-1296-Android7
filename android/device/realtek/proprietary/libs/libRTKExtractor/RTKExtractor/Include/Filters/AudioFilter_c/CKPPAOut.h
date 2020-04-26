/**
// ///////////////////////////////////////////////////////////////////////////
//	@file CPPAOut.h
//
//  @brief Header file for DVD-VENUS Audio Out ALSA Filter Class interface define
//
//	@author Taro     20041115
//
//	copyright (c)RealTek.   All rights reserved.
// ///////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_K_PP_AOUT_FILTER_H__
#define __C_K_PP_AOUT_FILTER_H__

#include "CPPAOut.h"
#include "CAudioOut.h"

			
// karaoke and pp complex filter	
class CAudioKPPAOutFilter : public CAudioPPAOutFilter
{
public:

	CAudioKPPAOutFilter();
	virtual ~CAudioKPPAOutFilter();

	HRESULT						Pause(FILTER_STATE State);
	int							GetAOUTInstanceID() { return m_aoutInstanceID; };
protected:

	CAudioOutInputPin*				m_pMicInPin;
	CBaseInputPin*					m_pCHMicInPinArr[AUDIO_DEC_OUTPIN];
	static long						m_MICInstanceNum;
};                  

#endif 

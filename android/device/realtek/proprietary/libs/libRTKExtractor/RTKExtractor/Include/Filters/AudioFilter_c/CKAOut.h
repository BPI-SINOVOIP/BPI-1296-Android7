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

#ifndef __C_K_AOUT_FILTER_H__
#define __C_K_AOUT_FILTER_H__

#include "CAudioOut.h"
				
// Agent AI-AO connect directly, but not yet ready
class CAudioKAOutFilter : public CAudioOutFilter
{
public:

	CAudioKAOutFilter();
	virtual ~CAudioKAOutFilter();

protected:
	static long						m_MICInstanceNum;

};                  

#endif 

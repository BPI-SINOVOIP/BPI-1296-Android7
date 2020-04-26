/**
// ///////////////////////////////////////////////////////////////////////////
//	@file CALSAOut.h
//
//  @brief Header file for DVD-VENUS Audio Out ALSA Filter Class interface define
//
//	@author Taro     20041115
//
//	copyright (c)RealTek.   All rights reserved.
// ///////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_ALSA_OUT_FILTER_H__
#define __C_ALSA_OUT_FILTER_H__


#include "CAudioOut.h"
				
class CAudioALSASimpleOutFilter : public CAudioOutFilter
{
public:

	CAudioALSASimpleOutFilter();
	virtual ~CAudioALSASimpleOutFilter();

protected:

protected:
};                  

#endif 

/**
// ///////////////////////////////////////////////////////////////////////////
//	@file CAudioDecoder.h
//
//  @brief Header file for DVD-VENUS Audio Decoder Filter Class interface define
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
// ///////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_MPEG_DECODER_FILTER_H__
#define __C_AUDIO_MPEG_DECODER_FILTER_H__


#include "CAudioDecoder.h"
#include "MPEGDecType.h"

/** 

class CAudioMPEGDecFilter : public CAudioDecFilter

*/

class CAudioMPEGDecFilter : public CAudioDecFilter
{
	
public:

	CAudioMPEGDecFilter(void);
	virtual ~CAudioMPEGDecFilter();

	void SetConfigure(char* pCFG, int length);
					
protected:
													
	CBaseOutputPin*				m_pCHPinArr[AUDIO_DEC_OUTPIN];
	CMemRingAllocator*			m_pAllocatorArr[AUDIO_DEC_OUTPIN];

	AUDIO_DEC_MPEG_CFG			m_Config;


};


#endif


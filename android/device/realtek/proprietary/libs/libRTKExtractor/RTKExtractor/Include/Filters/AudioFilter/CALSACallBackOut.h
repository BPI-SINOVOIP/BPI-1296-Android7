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

#ifndef __C_ALSA_CALLBACK_OUT_FILTER_H__
#define __C_ALSA_CALLBACK_OUT_FILTER_H__


#include <alsa/asoundlib.h>
#include <poll.h>

#include "Filters/AudioFilter/CALSAOut.h"
#include "Filters/AudioFilter/AudioType.h"
#include "Filters/AudioFilter/CAudioQueue.h"


class  CAudioALSACallBackOutInPin : public  CAudioALSAOutInPin
{
public:
		CAudioALSACallBackOutInPin() {};
		virtual ~CAudioALSACallBackOutInPin() {};

		HRESULT					Receive(BYTE* pData, long length);
};


class CAudioALSACallBackOutFilter : public CAudioALSAOutFilter
{

	friend class CAudioALSACallBackOutInPin;

public:

	CAudioALSACallBackOutFilter();
	virtual ~CAudioALSACallBackOutFilter();

protected:
    // re-write this function for user's purpose
    bool                        MainProcess(void);

	HRESULT						InitParam();
	HRESULT						FreeResource();
	bool						ProcessPrivateInfo(BYTE* pInfo, int length);
	HRESULT						PlayOutCallBack(BYTE* buf, int size);
	HRESULT						SetCallBackParam(int frameSampleNum);
	HRESULT						SetOutBufferSize(int frameSampleNum, int ALSAFrameSize);
	void						AVSync(void);

protected:

	snd_pcm_sw_params_t*		m_sw_params;
	int							m_frameSampleNum;

	CAudioPackQueue				m_AudioPackQueue;
	int							m_ValidReadDataLen;
	int64_t						m_PTS;

	IReferenceClock*			m_pReferClock;

private:


};


#endif 

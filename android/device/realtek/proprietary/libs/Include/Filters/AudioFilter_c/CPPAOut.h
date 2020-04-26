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

#ifndef __C_PP_AOUT_FILTER_H__
#define __C_PP_AOUT_FILTER_H__

#define APP_MAX_INPIN_NUM 4
#define APP_OUTPIN_ID APP_MAX_INPIN_NUM

#include "CAudioOut.h"

#ifdef ENABLE_MIDI_MIXER				
#ifdef RPC_MECHANISM
#include "AudioRPC_System.h"
#endif
#endif
				
typedef enum
{
    OUT_PIN_FOR_AOUT,
    OUT_PIN_FOR_APP,
} OUT_PIN_TYPE;

class CAudioPPAOutFilter : public CAudioOutFilter
{
public:
#ifdef ENABLE_MIDI_MIXER
    CAudioPPAOutFilter(OUT_PIN_TYPE type = OUT_PIN_FOR_APP, AUDIO_MODULE_TYPE PPtype = AUDIO_PP_OUT);
#else
    CAudioPPAOutFilter(OUT_PIN_TYPE type = OUT_PIN_FOR_APP);
#endif
	virtual ~CAudioPPAOutFilter();

	HRESULT						InitAgentInnerRingBuffer();
#ifdef FILEOUT
	HRESULT						OpenFile(char*	pFileName);
	HRESULT						CloseFile();
#endif
	HRESULT						Run();
	HRESULT						Stop();
	HRESULT						Pause(FILTER_STATE State);
	HRESULT						SetFocus();
	HRESULT						SetUnFocus();

	HRESULT						SetSeekingInfo(long skip,long speed);

	HRESULT						InitAgentRingBuffer();
	HRESULT						SetupAgentConnection(IPin* pConnectedPin);

	HRESULT						SetPPReverb(u_char ena, int reverb_mode);
	HRESULT						SetPPKeyShift(u_char ena, int semitone);
	HRESULT						SetPPVocalRemover(u_char ena, int mode);

    

protected:
    OUT_PIN_TYPE                    m_outPinType;
	static CMemRingAllocator*		m_pPPAoutAllocator;
	static CMemRingAllocator*		m_pPPAoutAllocatorArr[AUDIO_PP_OUTPIN];
	static long						m_appInstanceID;
	static long						m_pinMap[APP_MAX_INPIN_NUM];
	static long						m_focusPinID;
};                  

#endif 

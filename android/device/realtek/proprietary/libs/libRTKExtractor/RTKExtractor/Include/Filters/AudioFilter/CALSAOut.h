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


#include <alsa/asoundlib.h>

#include "Filters/AudioFilter/CAudioOut.h"
#include "Filters/AudioFilter/AudioType.h"
#include <Filters/AudioFilter/CAudioQueue.h>

class  CAudioALSAOutInPin : public  CAudioOutMultiInPin
{
public:
		HRESULT					PrivateInfo(BYTE* pInfo, int length);
		
    HRESULT     Receive(BYTE* pWritePointer, long WriteBufferSize);
    // HRESULT     Flush(void);
};


class CAudioALSAOutFilter : public CAudioOutFilter
{

	friend class CAudioALSAOutInPin;

public:

	CAudioALSAOutFilter();
	virtual ~CAudioALSAOutFilter();

	HRESULT						Pause(FILTER_STATE State);
	HRESULT						Stop();

protected:
    // re-write this function for user's purpose
    bool                        MainProcess(void);

	HRESULT						OpenDevice(char* dev = "plughw:0,0");
	HRESULT						InitParam();
	HRESULT						SetOutFormat(AUDIO_PCM_FORMAT* pcmFormat);
	HRESULT						CommitOutFormat();
	HRESULT						WriteData(BYTE* buf, int size);
	HRESULT						FreeResource();
	virtual bool				ProcessPrivateInfo(BYTE* pInfo, int length);
	

protected:

	CBaseInputPin*				m_pCHPinArr[AUDIO_DEC_OUTPIN];
	CMemRingAllocator*			m_pAllocatorArr[AUDIO_DEC_OUTPIN];

	snd_pcm_t*					m_playback_handle;
	snd_pcm_hw_params_t*		m_hw_params;

	int							m_ALSAFrameSize;
	AUDIO_PCM_FORMAT			m_pcmFormat;

};

				
/*
typedef struct{
    BYTE*               pAudioFrame;
    long                length;
} AUDIO_FRAME;

typedef ulSemCirQueue<AUDIO_FRAME>              AUDIO_FRAME_QUEUE;
typedef ulSemCirQueue<int64_t>                  AUDIO_PTS_QUEUE;
*/

typedef ulSemCirQueue<EStream_T>                AUDIO_FRAME_QUEUE;

class CAudioALSASimpleOutFilter : public CAudioALSAOutFilter
{

	friend class CAudioALSAOutInPin;

public:

	CAudioALSASimpleOutFilter();
	virtual ~CAudioALSASimpleOutFilter();

protected:
    bool    ThreadInit(
                IReadPointerHandle*             pChannelReadPtrHandle[AUDIO_DEC_OUTPIN],
                BYTE*                           pChannelPtrLower[AUDIO_DEC_OUTPIN],
                BYTE*                           pChannelPtrUpper[AUDIO_DEC_OUTPIN]);
                
    bool    CollectFrameFromChannel(
                bool&                       bEOF,
                BYTE*                       pChannelFrame[AUDIO_DEC_OUTPIN],
                IReadPointerHandle*         pChannelReadPtrHandle[AUDIO_DEC_OUTPIN]);

    bool    WeaveFrameChannel(
                BYTE*               pOutFrame,
                BYTE*               pChannelFrameInOut[AUDIO_DEC_OUTPIN],
                int                 SizePerFrame,
                BYTE*               pRingLower[AUDIO_DEC_OUTPIN],
                BYTE*               pRingUpper[AUDIO_DEC_OUTPIN]);
    // re-write this function for user's purpose
    bool                        MainProcess(void);
    bool                        Flush();
protected:
    AUDIO_FRAME_QUEUE           m_AudioFrameQueue;
    // CCritSec                    m_LockFlush;
    // AUDIO_PTS_QUEUE             m_AudioPTSQueue;
};                  
                    
                    

#endif 

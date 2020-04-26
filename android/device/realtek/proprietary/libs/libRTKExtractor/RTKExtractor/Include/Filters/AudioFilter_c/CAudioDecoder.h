  /**
/////////////////////////////////////////////////////////////////////////////////
//	@file CAudioDecoder.h
//
//  @brief Header file for DVD-VENUS Audio Decoder Filter Class interface define
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_DECODER_FILTER_H__
#define __C_AUDIO_DECODER_FILTER_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"

#include "StreamClass/SType.h"
#include "StreamClass/EType.h"

#include "CAudioQueue.h"

#include "CAudioDecPin.h"
#include "CAudioFilter.h"

/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////

class   CAudioDecInputPin;
class   CAudioDecOutputPin;

/** 

  class CAudioDecFilter : public CAudioFilter

*/
class CAudioDecFilter : public CAudioFilter
						
{
    friend class CAudioDecInputPin;
    friend class CAudioDecOutputPin;

public:	

	// Override AddPin for output pin

	CAudioDecFilter();
	virtual ~CAudioDecFilter();
	
    // Override Stop(). 
    HRESULT						Stop(void);

    //Override Pause().
    HRESULT						Pause(FILTER_STATE State);

    //Override Run. 
    HRESULT						Run(void);


	virtual void				SetConfigure(BYTE* pCFG, int length);


    HRESULT                     FilterType(
                                    FILTER_TYPE*            pFilterType
                                );

	HRESULT						EndOfStream(long pinID, long eventID);
	HRESULT						DeliverFlush(long pinID);

	HRESULT						ProcessAgentPrivateInfo(BYTE* pData, long length);
	HRESULT						ProcessPrivateInfo(BYTE* pData, long length);
	HRESULT						ProcessPrivateInfo(int infoId, BYTE* pData, long length);

	// for ac3 config
	HRESULT						SetDownmixMode(int mode);
	HRESULT						SetLFEMode(int mode);
	HRESULT						SetCompressionHILO(int scalehi, int scalelo);
	HRESULT						SetCompressionMode(int mode);
	HRESULT						SetDualMonoMode(int mode);
	HRESULT						SetStereoMode(int mode);
	HRESULT						SetNightMode(int mode);
	HRESULT						CheckCapabilities(FILTER_SEEKING_CAP*     pSeekingCap);

	// format info
	HRESULT						GetAudioFormatInfo(AUDIO_RPC_DEC_FORMAT_INFO *formatInfo);

    static  long                MediaTypeToDecoderType(ENUM_MEDIA_TYPE type);
protected:

	HRESULT						InitAgentRingBuffer();

	HRESULT						GetICQRingBufferHeader(AUDIO_RPC_RINGBUFFER_HEADER* header);

	HRESULT						SwitchDecoder(AUDIOFORMAT* a_format);
	HRESULT						QueryCreatedDecoder(ENUM_MEDIA_TYPE am_type, long& index);
	HRESULT						DeliverLPCMPrivateInfo(AUDIOFORMAT* a_format);
	HRESULT						SetupAgentConnection(IPin* pConnectedPin);
	HRESULT						GetAgentInfo(int pinID,int type, long* info);


protected:

	// self create pin
    CAudioInputPin*				m_pInPin;
    CAudioOutputPin*			m_pOutPin;
    CAudioInputPin*				m_pExtInPin;	// for extension bistream

	CBaseOutputPin*				m_pCHPinArr[AUDIO_DEC_OUTPIN];

	// self create allocators
	CMemRingAllocator*			m_pInputAllocator;
	CMemRingAllocator*			m_pOutputAllocator;
	CMemRingAllocator*			m_pExtInputAllocator;

	CMemRingAllocator*			m_pAllocatorArr[AUDIO_DEC_OUTPIN];

	// really used allocators
	CMemRingAllocator*			m_pUsedInputAllocator;
	CMemRingAllocator*			m_pUsedOutputAllocator;
	CMemRingAllocator*			m_pUsedExtInputAllocator;

	CMemRingAllocator*			m_pICQAllocator;

	IReadPointerHandle*			m_pInputReadHdl;
	IReadPointerHandle*			m_pExtInputReadHdl;
	IReadPointerHandle*			m_pICQReadHdl;

	// output configuration
	BYTE*						m_config;
	bool                        m_Init;
	long						m_decodeType;
	AUDIO_DEC_CFG				m_cfg;
};

/** 
**********************************************************************************

**********************************************************************************
*/

// compression scale factors (examples)
#define COMP_SCALE_FULL ((long)0x7fffffff)
#define COMP_SCALE_HALF ((long)0x40000000)
#define COMP_SCALE_NONE ((long)0x00000000)

// dual mono downmix mode
enum
{
	DUAL_MONO_MIX,
	DUAL_MONO_L,
	DUAL_MONO_R,
	DUAL_MONO_STEREO
};


// compression mode
enum
{
	COMP_CUSTOM_A,
	COMP_CUSTOM_D,
	COMP_LINEOUT,
	COMP_RF
};


// audio coding mode
enum 
{
    MODE_11,
    MODE_10,
    MODE_20,
    MODE_30,
    MODE_21,
    MODE_31,
    MODE_22,
    MODE_32 
};


enum
{
	LFE_OFF,
	LFE_ON
};

enum 
{
	MODE_AUTO_DETECT,
	MODE_DOLBY_SURROUND,	//	Lt/Rt
	MODE_STEREO,			//	Lo/Ro
};

#endif

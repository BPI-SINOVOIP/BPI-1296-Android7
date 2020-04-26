#ifndef C_Dummy_DECODER_H
#define C_Dummy_DECODER_H

#include <stdlib.h>
#include <stdio.h>
#include <Filters/DummyDecoder/CDummyInputPin.h>
#include <Filters/VideoDecoder/CVideoFilter.h>
#include <Filters/VideoDecoder/VP_RPC.h>
//#include <Filters/VideoDecoder/VideoType.h>
#include <AudioRPCBaseDS_data.h>
#include <OSAL/SmartClass.h>


#ifdef USE_VIDEO_DECODER_SIMULATOR
	#define BIT_STREAM_BUFFER_SIZE 		2048*1024
	#define INBAND_COMMAND_BUFFER_SIZE 	2048*1024
#else
	#define BIT_STREAM_BUFFER_SIZE 		512*1024
	#define INBAND_COMMAND_BUFFER_SIZE 	256*1024 /* for bug 11324 */
#endif

//#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
#if 1
#ifdef ENABLE_SMART_DISPLAY_STAND_ALONE
	#define MIN_BIT_STREAM_BUFFER_SIZE_FOR_SMOOTH_REVERSE   4096*1024
#else
	#define MIN_BIT_STREAM_BUFFER_SIZE_FOR_SMOOTH_REVERSE   4096*1024*2
#endif
#else
#define MIN_BIT_STREAM_BUFFER_SIZE_FOR_SMOOTH_REVERSE 2048*1024
#endif

class CDummyDecoder: public IFilterSeeking, public CVideoFilter
{

friend class    CDummyInputPin;

public:  
  
    CDummyDecoder(ENUM_MEDIA_TYPE type = MEDIASUBTYPE_Dummy_VIDEO,
                  long streamBufSize = BIT_STREAM_BUFFER_SIZE,
                  long cmdBufSize = INBAND_COMMAND_BUFFER_SIZE);

    virtual ~CDummyDecoder(void) ;

    HRESULT FilterType(FILTER_TYPE* pFilterType){
      *pFilterType = FILTER_DECODE_VIDEO;
      return S_OK;
    }
    
    ENUM_MEDIA_TYPE GetDecoderType()
    {
    	return m_pDummyInputPin->GetVideoMajorType();
    }
    
    //derived from CBaseFilter
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);
	
    //derived from IFilterSeeking
    HRESULT    SetSeekingInfo(long Speed, long Skip);
    HRESULT    GetSeekingInfo(long* pSpeed, long* pSkip);
    HRESULT    CheckCapabilities(FILTER_SEEKING_CAP* pSeekingCap);
	
    HRESULT    SetDeblock(unsigned char OnOff);
    HRESULT    SetDNR(VIDEO_DNR_MODE mode);
    HRESULT    InitPTSRingBuffer(CMemRingAllocator *pPTSRingAllocator);
    
    HRESULT    GetVideoSequenceInfo(VIDEO_RPC_DEC_SEQ_INFO* pSeqInfo);
    
	
protected:


    CDummyInputPin*     m_pDummyInputPin;	//the entry that video decoder receives data from demux
  	CBaseOutputPin*	  	m_pDummyOutputPin;
  	CBaseOutputPin*	  	m_pDummyOutputPin2;
    long 				m_Speed;
    long 				m_Skip;
    long                m_streamBufSize;

} ;



#endif


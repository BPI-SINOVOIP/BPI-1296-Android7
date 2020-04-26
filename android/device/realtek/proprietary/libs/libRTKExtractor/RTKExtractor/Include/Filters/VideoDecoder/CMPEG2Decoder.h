#ifndef C_MPEG2_DECODER_H
#define C_MPEG2_DECODER_H

#include <stdlib.h>
#include <stdio.h>
#include <Filters/VideoDecoder/CMPEG2InputPin.h>
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

#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
#ifdef ENABLE_SMART_DISPLAY_STAND_ALONE
	#define MIN_BIT_STREAM_BUFFER_SIZE_FOR_SMOOTH_REVERSE   4096*1024
#else
	#define MIN_BIT_STREAM_BUFFER_SIZE_FOR_SMOOTH_REVERSE   4096*1024*2
#endif
#else
#define MIN_BIT_STREAM_BUFFER_SIZE_FOR_SMOOTH_REVERSE 2048*1024
#endif

class CMPEG2Decoder: public IFilterSeeking, public CVideoFilter
{

friend class    CMPEG2InputPin;

public:  
  
    CMPEG2Decoder(ENUM_MEDIA_TYPE type = MEDIASUBTYPE_MPEG2_VIDEO,
                  long streamBufSize = BIT_STREAM_BUFFER_SIZE,
                  long cmdBufSize = INBAND_COMMAND_BUFFER_SIZE);

    virtual ~CMPEG2Decoder(void) ;

    HRESULT FilterType(FILTER_TYPE* pFilterType){
      *pFilterType = FILTER_DECODE_VIDEO;
      return S_OK;
    }
    
    ENUM_MEDIA_TYPE GetDecoderType()
    {
    	return m_pMPEG2InputPin->GetVideoMajorType();
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
#if IS_CHIP(SATURN)
	HRESULT	   GetSEI3DInfo(ENUM_SEI_3D_TYPE* type);
#endif
    
#if IS_CHIP(VENUS) || IS_CHIP(NEPTUNE)
    /* the following functions are only for RM */
	
	/**
	* \fn  HRESULT InitRMAssistant()
	*
	* \brief Init a RM assistant to decode VLD which running on audio CPU.
	*
	* \return HRESULT:
	*    S_OK if it is successfully to create a audio agent and allocate 3 ring buffer
	*    shared with audio.
	*
	* \param  none
 	*/
	HRESULT InitRMAssistant();
private:	
	
	HRESULT InitRMAssistantRingBuf();
	HRESULT InitAudioRingBuffer(RINGBUFFER_HEADER*  pRingBufferHeader, long instanceID, AUDIO_IO_PIN pinID, long bufferHeaderPhyAddr);
	HRESULT InitVideoRingBuffer(RINGBUFFER_HEADER* pRingBufferHeader, RINGBUFFER_TYPE type, long bufferHeaderPhyAddr);
	HRESULT RMAssistantRequestCoeffBuf();
	HRESULT RMAssistantReleaseCoeffBuf();
		
	/**
	* \fn  HRESULT CreateRMAssistant(long *id, AUDIO_MODULE_TYPE type)
	*
	* \brief send audio RPC to create a RM assistant
	*
	* \return HRESULT:
	*    S_OK if it is successfully to create a audio agent.
 	*
	* \param  id:
	*    the assistant id.
	*
	* \param  type:
	*    the assistant type.
	*/
	HRESULT CreateRMAssistant(long *id, AUDIO_MODULE_TYPE type);
	
	/**
	* \fn  CMemRingAllocator* CreateRMRingBuffer(int size)
	*
	* \brief create ring buffer which will shared with audio
	*
	* \return CMemRingAllocator*:
	*    a pointer with type CMemRingAllocator which defined in StreamClass.
 	*
	* \param  size:
	*    the ring buffer size.
	*/
	CMemRingAllocator* CreateRMRingBuffer(int size);
	
	/**
	* \fn  HRESULT InitVideoRingBuffer(CMemRingAllocator* pAllocator, RINGBUFFER_TYPE type)
	*
	* \brief send video RPC to init ring buffer header.
	*
	* \return HRESULT:
	*    the result returned from video.
 	*
	* \param  pAllocator:
	*    the ring buffer which will shared with audio.
	*
	* \param  type
	*	 the ring buffer type
	*/
	HRESULT InitVideoRingBuffer(CMemRingAllocator* pAllocator, RINGBUFFER_TYPE type);
	
	/**
	* \fn  HRESULT InitAudioRingBuffer(CMemRingAllocator* pAllocator, long instanceID, AUDIO_IO_PIN pinID)
	*
	* \brief send audio RPC to RM assistant to init ring buffer header.
	*
	* \return HRESULT:
	*    the result returned from audio.
 	*
	* \param  pAllocator:
	*    the ring buffer which will shared with video.
	*
	* \param  instanceID
	*	 the RM assistant ID
	*
	* \param  pinID
	*	 a ID to indicate which pin to carry this ring buffer
	*/
	HRESULT InitAudioRingBuffer(CMemRingAllocator* pAllocator, long instanceID, AUDIO_IO_PIN pinID);

	/**
	* \fn  HRESULT RMAssistantRun()
	*
	* \brief send audio RPC to change state to running state.
	*
	* \return HRESULT:
	*    the result returned from audio.
 	*
	* \param  none
	*/
	HRESULT RMAssistantRun();
	
	/**
	* \fn  HRESULT RMAssistantPause()
	*
	* \brief send audio RPC to change state to pause state.
	*
	* \return HRESULT:
	*    the result returned from audio.
 	*
	* \param  none
	*/
	HRESULT RMAssistantPause();
	
	/**
	* \fn  HRESULT RMAssistantStop()
	*
	* \brief send audio RPC to change state to stop state.
	*
	* \return HRESULT:
	*    the result returned from audio.
 	*
	* \param  none
	*/
	HRESULT RMAssistantStop();
	
	/**
	* \fn  HRESULT RMAssistantDestroy()
	*
	* \brief send audio RPC to destroy RM assistant.
	*
	* \return HRESULT:
	*    the result returned from audio.
 	*
	* \param  none
	*/
	HRESULT RMAssistantDestroy();
	
	HRESULT RMAssistantFlush();
#endif	    
	
///////////////////////////////////// for VP6 //////////////////////////////
	HRESULT InitVP6AudioAgent();
	HRESULT CreateVP6AudioAgent();
	HRESULT DestroyVP6AudioAgent();
	
protected:


    CMPEG2InputPin*     m_pMPEG2InputPin;	//the entry that video decoder receives data from demux
  	CBaseOutputPin*	  	m_pMPEG2OutputPin;
  	CBaseOutputPin*	  	m_pMPEG2OutputPin2;
    long 				m_Speed;
    long 				m_Skip;
    long                m_streamBufSize;
    
#if IS_CHIP(VENUS) || IS_CHIP(NEPTUNE)
	long				m_RMAssistantID;	/* the RM audio agent ID which is responsible for decoding VLD.
										 	 * this value is legal only when source's media type is RM.
										 	 */

	RINGBUFFER_HEADER* m_pRMCoeffBufferHeader;
	unsigned char*     m_pRMCoeffBufferHeaderAddr;
    CMemRingAllocator*	m_pRMICQAllocator;	/* This ring buffer is shared between RM video and audio.
    										 * It is a input command buffer for RM video to send command to audio.
    										 */
#endif    


	long 				m_VP6AudioAgent;
	
#if IS_CHIP(JUPITER)|| IS_CHIP(SATURN)
	CThread*			m_pVP8Thread;
	long				m_VP8AudioAgent;
	void*				m_optr;
	
	void 	DestroyVP8Decoder();
	void 	InitVP8();
	HRESULT CreateVP8AudioAgent();
	HRESULT DestroyVP8AudioAgent();
	HRESULT CreateVP8Decoder();
	static void VP8_DecodeFrameThread( void *dec);
#endif

#if defined(ENABLE_BLURAY_3D) 
	CMPEG2InputPin*		m_pAuxInPin;
#endif

} ;



#endif


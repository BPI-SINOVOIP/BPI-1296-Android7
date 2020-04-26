#ifndef __C_PSEUDO_AUDIO_OUT_FILTER_H__
#define __C_PSEUDO_AUDIO_OUT_FILTER_H__

#include <CStream.h>
#include <Filters/AudioFilter_c/CAudioFilter.h>
#include <Filters/AudioFilter_c/CMultiAllocator.h>
#include <Filters/AudioFilter_c/CMultiPin.h>
#include <Filters/AudioFilter_c/CAudioPin.h>
#include <AudioRPC_System.h>


class CPseudoAudioOutOutputPin;
class CPseudoAudioOutInputPin;

class CPseudoAudioOutFilter: public CAudioFilter{
friend class CPseudoAudioOutOutputPin;
friend class CPseudoAudioOutInputPin;

public:
    CPseudoAudioOutFilter();
    virtual ~CPseudoAudioOutFilter(void);

    HRESULT    CreateAudioAgent(AUDIO_MODULE_TYPE type);
    HRESULT    ConnectToAEAgent();
    HRESULT    InitPTSRingBuffer(CMemRingAllocator *pPTSRingAllocator);
    HRESULT    InitOutPinRingData();
    HRESULT    InitInPinRingData();

    //derived from CBaseFilter
    HRESULT    Pause(FILTER_STATE State);
    HRESULT    Run();
    HRESULT    Stop();

    //derived from CAudioFilter
    HRESULT    UpdateReadPtr(AUDIO_RPC_READPACK* pReadPack);
    HRESULT    UpdateWritePtr(AUDIO_RPC_WRITEPACK* pWritePack);
    HRESULT    EndOfStream(long pinID, long eventID);
    HRESULT    ProcessAgentPrivateInfo(BYTE* pData, long length);
    HRESULT    DeliverFlush(long pinID);
    HRESULT    InitAgentRingBuffer();



private:

    CPseudoAudioOutOutputPin*     m_pPAOOutputPin;
    CPseudoAudioOutInputPin*      m_pPAOInputPin;

    CLNT_STRUCT                 m_rpcclnt;

    BYTE*                       m_pRingUpper;
    BYTE*                       m_pRingLower;
    BYTE*                       m_RingBufferWrPtr;
    long                        m_BufferSize;    
    RINGBUFFER_HEADER*          m_pRingBufferHeader;
    CMemRingAllocator*          m_pInAllocatorArr[AUDIO_PSEUDO_OUT_PIN];
    CMemRingAllocator*          m_pOutAllocatorArr[AUDIO_PSEUDO_OUT_PIN];
    CBaseInputPin*              m_pCHInPinArr[AUDIO_PSEUDO_OUT_PIN];
    CBaseOutputPin*             m_pCHOutPinArr[AUDIO_PSEUDO_OUT_PIN];
    CMemRingAllocator*          m_pInputAllocator;
    CMemRingAllocator*          m_pOutputAllocator;

    AUDIO_MODULE_TYPE           m_EncoderType;
    bool                        m_Init;

};


class CPseudoAudioOutOutputPin: public CAudioMultiOutputPin
{
public:
    CPseudoAudioOutOutputPin(long pinID)
		: CAudioMultiOutputPin() { m_pinID = pinID; };
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);
    CBaseInputPin* GetInputPin();
};

class CPseudoAudioOutInputPin: public CAudioMultiInputPin
{
public:
    CPseudoAudioOutInputPin(long pinID)
                : CAudioMultiInputPin() { m_pinID = pinID; };
};


#endif // __C_PSEUDO_AUDIO_OUT_FILTER_H__


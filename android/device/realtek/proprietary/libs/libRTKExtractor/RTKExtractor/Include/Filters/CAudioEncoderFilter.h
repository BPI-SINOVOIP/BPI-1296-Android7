#ifndef __C_AUDIO_ENCODER_FILTER_H__
#define __C_AUDIO_ENCODER_FILTER_H__

#include <CStream.h>
#include <Filters/AudioFilter_c/CAudioFilter.h>
#include <Filters/AudioFilter_c/CMultiAllocator.h>
#include <Filters/AudioFilter_c/CMultiPin.h>
#include <Filters/AudioFilter_c/CAudioPin.h>
#include <Application/AppClass/setupdef.h>

#include <AudioRPC_System.h>

typedef struct _ae_quality_setting_t {
    long datarate;
    long samprate;
} AE_QUALITY_SETTING_T;

class CAudioEncoderOutputPin;
class CAudioEncoderInputPin;

class CAudioEncoderFilter: public CAudioFilter{
friend class CAudioEncoderOutputPin;
friend class CAudioEncoderInputPin;

public:
    CAudioEncoderFilter(AUDIO_MODULE_TYPE type=AUDIO_MPEG_ENCODER);
    virtual ~CAudioEncoderFilter(void);

    HRESULT    CreateAudioAgent(AUDIO_MODULE_TYPE type);
    HRESULT    InitOutPinRingData();
    HRESULT    InitInPinRingData();

    //derived from CBaseFilter
    HRESULT    Pause(FILTER_STATE State);
    HRESULT    Run();
    HRESULT    Stop();

    //derived from CAudioFilter
    HRESULT    UpdateReadPtr(AUDIO_RPC_READPACK* pReadPack);
    HRESULT    UpdateWritePtr(AUDIO_RPC_WRITEPACK* pWritePack);
    HRESULT    ProcessAgentPrivateInfo(BYTE* pData, long length);
    HRESULT    DeliverFlush(long pinID);
    HRESULT    InitAgentRingBuffer();

    HRESULT    PauseEncode();
    HRESULT    ResumeEncode();
    HRESULT    StartEncode();
    HRESULT    StopEncode();
    HRESULT    AutoPauseEncode();

    HRESULT    SetNonRealTime(bool isTranscode);
    HRESULT    SetSourceFile(char *fileName);

    int64_t    DeliverAudioFrame();
    HRESULT    DeliverPrivateInfo(ENUM_DVD_AUDIO_ENCODER_OUTPUT_INFO_TYPE infoType);
    HRESULT    EndOfStream(long pinID, long eventID);

    HRESULT    SetAllEncoderQuality(ENUM_REC_QUALITY res, bool isKaraoke);
    HRESULT    EncoderConfig(AUDIO_ENC_CFG ecfg);
    HRESULT    GetBitrate(long *datarate, long *samplerate);

    HRESULT    SetInputRefClock(IReferenceClock* pRefClock);
    int        InputDataEmpty();
private:
    static void deliverData_loop(void* Data);
    CAudioEncoderOutputPin*     m_pAEOutputPin;
    CAudioEncoderInputPin*      m_pAEInputPin;
    FILTER_STATE                m_EncodeState;    
    unsigned int                m_startPTS;

    CLNT_STRUCT                 m_rpcclnt;
    // int                      m_rpcclnt;  #ifndef TARGET_BOARD
    BYTE*                       m_pRingUpper;
    BYTE*                       m_pRingLower;
    BYTE*                       m_RingBufferWrPtr;
    long                        m_BufferSize;    
    RINGBUFFER_HEADER*          m_pRingBufferHeader;
    CMemRingAllocator*          m_pInAllocatorArr[AUDIO_IN_OUTPIN];
    CBaseInputPin*              m_pCHInPinArr[AUDIO_IN_OUTPIN];
    CMemRingAllocator*          m_pInputAllocator;
    CMemRingAllocator*          m_pStreamAllocator;
    bool                        m_Init;
    AUDIO_MODULE_TYPE           m_EncoderType;
    bool                        m_bIsConnectToAIN;
    AE_QUALITY_SETTING_T        m_qualitySetting;

    //FILE*                     m_myfd;    #ifdef AUDIO_ELEMENTARY_ONLY
    unsigned int                m_frameNumber;
    bool                        m_startEncodeFlag;  // prevent receive frameinfo after EOS!
    bool                        m_sendGenInfoFlag;  // decide transfer GenInfo to muxer or not
                                                    // (StartEncode: Yes, ResumeEncode: No)
                                                        
    //Msg Ring Buffer Related
    CMemRingAllocator*          m_pMsgAllocator;
    BYTE*                       m_pMsgRingUpper;
    BYTE*                       m_pMsgRingLower;
    BYTE*                       m_MsgRingBufferRdPtr;
    long                        m_MsgBufferSize;
    RINGBUFFER_HEADER*          m_pMsgRingBufferHeader;   
    int64_t                     m_audioPTS;
    //This ref closk is designed for A/V input, which is used to deal with Auto Pause
    IReferenceClock*            m_pInputRefClock;
};


class CAudioEncoderOutputPin: public CAudioOutputPin
{
public:
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);
    HRESULT QueryAccept(const MEDIA_TYPE *pmt);
    HRESULT EndOfStream(unsigned int eventID);
    CBaseInputPin * GetInputPin();
};

class CAudioEncoderInputPin: public CAudioMultiInputPin
{
public:
    CAudioEncoderInputPin(long pinID)
                : CAudioMultiInputPin() { m_pinID = pinID; };
    HRESULT SetAllocator(IMemAllocator* pAllocator);
};


#endif // __C_AUDIO_ENCODER_FILTER_H__


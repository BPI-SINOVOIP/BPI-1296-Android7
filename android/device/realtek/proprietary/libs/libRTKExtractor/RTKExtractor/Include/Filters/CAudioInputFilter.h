#ifndef __CAUDIO_INPUT_FILTER_H__
#define __CAUDIO_INPUT_FILTER_H__

#include <CStream.h>
#include <Filters/inst_map.h>
#include <Filters/AudioFilter_c/CMultiAllocator.h>
#include <Filters/AudioFilter_c/CMultiPin.h>
#include <Filters/AudioFilter_c/CAudioPin.h>
#include <Platform_Lib/Board/board.h> // for enum AUDIO_INPUT_ENUM

#ifdef TARGET_BOARD
  #include <VideoRPC_System.h>
#endif

typedef struct {
    int count;
    bool adc0_config;
    bool adc1_config; 
    bool spdif_config;
} ai_config_t;

class CAudioInOutputAOUTPin;
class CAudioInOutputAEPin;

class CAudioInputFilter: public CAudioFilter
{
friend class CAudioInOutputAOUTPin;
friend class CAudioInOutputAEPin;

public:
    CAudioInputFilter();
    virtual ~CAudioInputFilter();

    HRESULT    CreateAudioAgent(AUDIO_MODULE_TYPE type);
    //derived from CBaseFilter
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);
    
    // derived from class CSystemRPCCall
    HRESULT    UpdateReadPtr(AUDIO_RPC_READPACK* pReadPack);
    HRESULT    UpdateWritePtr(AUDIO_RPC_WRITEPACK* pWritePack);
    HRESULT    ProcessAgentPrivateInfo(BYTE* pData, long length);
    HRESULT    EndOfStream(long pinID, long eventID);
    HRESULT    DeliverFlush(long pinID);
    HRESULT    InitAgentRingBuffer();  

    HRESULT    InitializeInputStatus(bool adc0Exist, bool adc1Exist, bool spdifExist);
    HRESULT    ADC0Config(AUDIO_GENERAL_CONFIG audioGeneralConfig,
                               AUDIO_SAMPLE_INFO sampleInfo);
    HRESULT    ADC1Config(AUDIO_GENERAL_CONFIG audioGeneralConfig,
                               AUDIO_SAMPLE_INFO sampleInfo);
    HRESULT    SPDIFConfig(AUDIO_GENERAL_CONFIG audioGeneralConfig,
                               AUDIO_SPDIF_ERROR audioSPDIFError);

    HRESULT    InitRingData(CAudioOutputPin* pOutputPin);
    HRESULT    ConnectToAOUTAgent();
    HRESULT    ConnectToAEAgent();
    HRESULT    SetMasterShip();
    HRESULT    ConfigFirmware();
    HRESULT    SetInputRefClock(IReferenceClock* pRefClock);
    HRESULT    SwitchFocus(AUDIO_INPUT_ENUM focus);
    HRESULT    Mute(char mute_en);
    HRESULT    SetAudioMicEqualizer(u_char mic_sel, ENUM_EQUALIZER_ENA mode, AUDIO_RPC_EQUALIZER_MODE info);
    HRESULT    TSDConfig(char tsd_en);

private:
    CAudioOutputPin*            m_pOutputAOUTPin;
    CAudioOutputPin*            m_pOutputAEPin;
    CBaseOutputPin*		m_pCHOutPinArr1[6];
    CBaseOutputPin*		m_pCHOutPinArr2[6];
#ifdef TARGET_BOARD
    CLNT_STRUCT                 m_rpcclnt;
#else
    int                         m_rpcclnt;
#endif
    FILTER_STATE                m_EncodeState;
    FILTER_STATE                m_EncodeStateNext;
    CMemRingAllocator*		m_pAinAllocator;
    CMemRingAllocator*		m_pAinAllocatorArr[6];
    int                         m_pAinAllocNum;
    // This ref clock is designed for A/V Output
    IReferenceClock*            m_pRefClock;
    //This ref closk is designed for A/V input, which is used to deal with the different
    //Clock between system(video) and audio
    IReferenceClock*            m_pInputRefClock;
    ai_config_t                 m_configType;
    AUDIO_ADC_CONFIG            m_pAdc0Config;
    AUDIO_ADC_CONFIG            m_pAdc1Config;
    AUDIO_SPDIF_CONFIG          m_pSpdifConfig;
};


class CAudioInOutputAOUTPin: public CAudioMultiOutputPin
{
public:
    CAudioInOutputAOUTPin(long pinID)
		: CAudioMultiOutputPin() { m_pinID = pinID; };
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);	
};

class CAudioInOutputAEPin: public CAudioMultiOutputPin
{
public:
    CAudioInOutputAEPin(long pinID)
		: CAudioMultiOutputPin() { m_pinID = pinID; };
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);	
};

#endif // __CAUDIO_INPUT_FILTER_H__


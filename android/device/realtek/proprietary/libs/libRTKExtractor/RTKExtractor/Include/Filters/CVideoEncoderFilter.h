#ifndef __CVIDEO_ENCODER_FILTER_H__
#define __CVIDEO_ENCODER_FILTER_H__

#include <CStream.h>
#include <Filters/VideoDecoder/CVideoFilter.h>
#include <video_filter_pin_def.h>
#include <Application/AppClass/setupdef.h>

#include <VideoRPC_System.h>

#include <Platform_Lib/ExternalTVD/ExternalTVD.h>


class CVideoEncoderOutputMuxerDataPin;
class CVideoEncoderOutputVoutPin;
class CVideoEncoderInputPin;



typedef struct _ve_quality_setting_t {
    ENUM_VIDEO_HORIZONTAL_RESOLUTION resolution;
    VIDEO_RATE_CONTROL_MODE rMode;
    long peakBitRate;
    long aveBitRate;
    long bitBufferSize;
    long initBufferFullness;
    long configVobu;
    long Gop_M;
    long Gop_N;
} VE_QUALITY_SETTING_T;

class CVideoEncoderFilter : public CVideoFilter
{
friend class CVideoEncoderOutputMuxerDataPin;
friend class CVideoEncoderOutputVoutPin;
friend class CVideoEncoderInputPin;

public:
    CVideoEncoderFilter(int editOnly = 0, VIDEO_FORMAT videoFormat=VIDEO_FORMAT_NTSC, VIDEO_SOURCE videoSource=VIDEO_SOURCE_VIN);
    virtual ~CVideoEncoderFilter();

    //derived from CBaseFilter
    
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);
    
    HRESULT    IsRunnable(bool *isRunnable);
    HRESULT    SetNonRealTime(bool isTranscode);
    HRESULT    CreateVideoAgent(VIDEO_VF_TYPE type);
    HRESULT    InitRingData();
    HRESULT    InitializeVideoAgent(VIDEO_FORMAT videoFormat, VIDEO_SOURCE videoSource);
    HRESULT    ConnectToVdecAgent(long srcAgentInstanceID);
    HRESULT    SetInputRefClock(IReferenceClock* pRefClock);
    
    HRESULT    PauseEncode(int pauseAtVOBU);
    HRESULT    ResumeEncode();
    HRESULT    StartEncode();
    HRESULT    StopEncode();
    HRESULT    EndOfStream();
    HRESULT    IssueAutoPauseEncode(u_char isFinish);

    int64_t    DeliverVideoFrame(int64_t audioPTS);
    HRESULT    DeliverPrivateInfo(enum ENUM_DVD_VIDEO_ENCODER_OUTPUT_INFO_TYPE infoType);
    HRESULT    DeliverFlushToVout();
    
    HRESULT    GetInputSignalStatus(bool *isCopyFree, bool *noSignal, 
                                    ENUM_TVD_VIDEO_FORMAT *formatType, 
                                    VO_VBI_ASPECT_RATIO *ar=NULL);
    HRESULT    SetCGMSSetting(bool case0_cpfree, bool case1_cponce, 
                              bool case2_cpunknown, bool case3_cpnever);                                    

    HRESULT    SetAllEncoderQuality(ENUM_REC_QUALITY res, VIDEO_FORMAT format, bool isLivePause);
    HRESULT    SetResolution(ENUM_VIDEO_HORIZONTAL_RESOLUTION resolution);
    HRESULT    SetSourceFile(char *fileName);
    HRESULT    SetBitRate(VIDEO_RATE_CONTROL_MODE rMode, long peakBitRate, long aveBitRate, 
                          long bitBufferSize, long initBufferFullness);
    HRESULT    SetGOPStructure(long M, long N);
    HRESULT    SetNewGOPOnSceneChange(long newGOPOnSceneChange);
    HRESULT    SetInverseTelecineDetection(long inverseTelecineDetection);
    HRESULT    SetTF(long mode, long strength);
    HRESULT    SetNR(long Hstrength, long Vstrength);
    HRESULT    SetEncodeFormat(VIDEO_STREAM_TYPE pSType);
    HRESULT    ConfigureCCData(long option, long CCDataSize, u_char *CCDAta);
    HRESULT    SetClosedGOPFrequency(long closedGOPFreq);
    HRESULT    ConfigVOBU(long minGOPs);
    HRESULT    SetVBVBuffer(long vbv_buf_size, long vbv_buf_fullness);
    
    HRESULT    GetResolution(ENUM_VIDEO_HORIZONTAL_RESOLUTION *resolution);
    HRESULT    GetBitRatae(VIDEO_RATE_CONTROL_MODE *rMode, long *peakBitRate, long *aveBitRate, 
                          long *bitBufferSize, long *initBufferFullness);
    HRESULT    GetGOPStructure(long *M, long *N);   
    HRESULT    GetConfigVOBU(long *minGOPs);                       
    
    CMemRingAllocator* InitVBIRingbuffer(int size);


    bool prepareMenuData(BYTE **pStart, unsigned char *pStream, unsigned int length);
    int  InputDataEmpty();

private:
    static void query_status_thread(void* Data);
    CVideoEncoderOutputMuxerDataPin*    m_pVEOutputMuxerPin;
    CVideoEncoderOutputVoutPin*         m_pVEOutputVoutPin;
    CVideoEncoderInputPin*              m_pInputPin;
    int                         m_frameSize;
    CMemRingAllocator*          m_pStreamAllocator;
    BYTE*                       m_pRingUpper;
    BYTE*                       m_pRingLower;
    BYTE*                       m_RingBufferWrPtr;
    long                        m_BufferSize;    
    CLNT_STRUCT                 m_rpcclnt;
    // int                      m_rpcclnt;  #ifndef TARGET_BOARD

    FILTER_STATE                m_EncodeState;    
    RINGBUFFER_HEADER*          m_pRingBufferHeader;    
    ENUM_VIDEO_HORIZONTAL_RESOLUTION m_resolution;
    VIDEO_STREAM_TYPE           m_streamType;
    VE_QUALITY_SETTING_T        m_qualitySetting;
    int                         m_EditOnly;
    //int                       m_fd;    #ifdef VIDEO_ELEMENTARY_ONLY
    unsigned int                m_frameNumber;

    // WSS_Info related
    bool                        m_vinNoSignal;
    ENUM_TVD_VIDEO_FORMAT       m_videoFormatType;
    bool                        m_isCopyFree;    
    unsigned int                m_wssSetting;
    bool                        m_wssInfoActivate;
    VO_VBI_ASPECT_RATIO         m_vinAspectRatio;
    CMemRingAllocator*          m_pVbiAllocator;

    //Msg Ring Buffer Related
    CMemRingAllocator*          m_pMsgAllocator;
    BYTE*                       m_pMsgRingUpper;
    BYTE*                       m_pMsgRingLower;
    BYTE*                       m_MsgRingBufferRdPtr;
    long                        m_MsgBufferSize;
    RINGBUFFER_HEADER*          m_pMsgRingBufferHeader; 
    int64_t                     m_videoPTS;
    MEDIA_TYPE                  m_mediaType;
    //This ref closk is designed for A/V input, which is used to deal with the different
    //Clock between system(video) and audio
    IReferenceClock*            m_pInputRefClock;
    unsigned int                m_PrintInfo;
    
    // for MARS ExternalTVD related
    CThread*                    m_pStatusThread;
#if AP_CONFIG_WITH(APCFG_HW_ANALOG_INPUT) && IS_CHIP(MARS)
    ExternalTVD*                m_ExternalTVD;
#endif
    unsigned char               m_NoSignalFilter;      
    unsigned char               m_CopyFreeFilter;
    unsigned char               m_VideoFormatFilter;    
    unsigned char               m_isCopyFreeMacroV;
    
};


class CVideoEncoderInputPin: public CBaseInputPin
{
public:
    HRESULT ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
    HRESULT Flush();
};

class CVideoEncoderOutputMuxerDataPin: public CBaseOutputPin
{
public:
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);	
    HRESULT QueryAccept(const MEDIA_TYPE *pmt);
    HRESULT EndOfStream(unsigned int eventID);
    CBaseInputPin * GetInputPin();
};

class CVideoEncoderOutputVoutPin: public CBaseOutputPin
{
public:
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);
};

#endif // __CVIDEO_ENCODER_FILTER_H__

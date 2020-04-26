#ifndef __CPSEUDO_VIDEO_ENCODER_FILTER_H__
#define __CPSEUDO_VIDEO_ENCODER_FILTER_H__

#include <CStream.h>
#include <Filters/VideoDecoder/CVideoFilter.h>
#include <video_filter_pin_def.h>
#include <VideoRPC_System.h>
#include <InbandAPI.h>


#define  PVE_STREAM_BUF_SIZE  (2048*1024)
#define  PVE_MSG_BUF_SIZE   (2048*1024)

class CPVEOutputMuxPin;
class CPVEInputVDPin;

class CPseudoVideoEncoderFilter : public CVideoFilter
{
friend class CPVEOutputMuxPin;
friend class CPVEInputVDPin;

public:
    CPseudoVideoEncoderFilter();
    virtual ~CPseudoVideoEncoderFilter();

    //derived from CBaseFilter
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);
        
    HRESULT    InitMsgRingBuffer(); 
    HRESULT    InitStreamRingBuffer();
    void       SetVideoDecoderAgentID(long vdAgentID);
    
    HRESULT    PauseEncode(int pauseAtVOBU);
    HRESULT    ResumeEncode();
    HRESULT    StartEncode();
    HRESULT    StopEncode();
    HRESULT    EndOfStream();

    void       DeliverVideoFrame();
    HRESULT    DeliverPrivateInfo(enum ENUM_DVD_VIDEO_ENCODER_OUTPUT_INFO_TYPE infoType);
    
private:
    static void deliverData_loop(void* Data);
    CPVEOutputMuxPin*           m_pOutputMuxPin;    
    CPVEInputVDPin*             m_pInputVDPin;
    int                         m_frameSize;
    long                        m_VDAgentID;
    
    CLNT_STRUCT                 m_rpcclnt;      
    u_char                      m_TVSystemFormat;       
    int                         m_frameNumber;

    // stream buffer related
    BYTE*                       m_pStreamRingUpper;
    BYTE*                       m_pStreamRingLower;
    BYTE*                       m_pStreamRingWrPtr;
    long                        m_StreamBufferSize;    

    // msg buffer related
    CMemRingAllocator*          m_pMsgAllocator;
    BYTE*                       m_pMsgRingUpper;
    BYTE*                       m_pMsgRingLower;
    long                        m_MsgBufferSize;
    BYTE*                       m_MsgRingBufferRdPtr;
    
};


class CPVEInputVDPin: public CBaseInputPin
{
public:
    HRESULT ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);    
};

class CPVEOutputMuxPin: public CBaseOutputPin
{
public:
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);	
    HRESULT QueryAccept(const MEDIA_TYPE *pmt);
    HRESULT EndOfStream(unsigned int eventID);
    CBaseInputPin * GetInputPin();
};


#endif // __CPSEUDO_VIDEO_ENCODER_FILTER_H__

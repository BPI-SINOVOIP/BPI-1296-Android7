#ifndef __C_VIDEO_ENCODER_AGENT_H__
#define __C_VIDEO_ENCODER_AGENT_H__


#include <CStream.h>
#include "../../Filters/VideoEncoder/rpc_func_map.h"



class CVideoEncoderAgent : public CBaseFilter {

public:
    CVideoEncoderAgent();
    ~CVideoEncoderAgent();

    HRESULT     SetInstanceID(long instanceID);
    HRESULT     Run(void);
    HRESULT     Stop(void);
    HRESULT     Pause(FILTER_STATE State);
    HRESULT     PauseEncode();
    HRESULT     StartEncode();
    HRESULT     StopEncode();
    HRESULT     InitRingBuffer(RPC_RINGBUFFER* pRingBuffer);
    HRESULT     SetResolution(int hres);
    HRESULT     SetSourceFile(char *);
    HRESULT     myRequestWriteSpace(BYTE **pRingWrPtr, BYTE **pRingRdPtr, int frameSize);
/*
	HRESULT 	EndOfStream(void);
	
	HRESULT		SetEndOfStream();
    void		ResetEndOfStream(void);
    bool		IsEndOfStream();
*/        
    CThread*    m_pMPEG2Thread;    //the thread for MPEG2 decoder
    
protected:
    long        m_instanceID;
    FILE       *m_StreamFileHandle;
    char        *m_StreamFileName;

    ENUM_VIDEO_HORIZONTAL_RESOLUTION	m_hres;    
    CLNT_STRUCT                        m_rpcclnt;
    
    	
private:
	//BYTE 	m_Flag;
	/* static function only can access static member datas
    *  thus we must pass in the instance   */
    static void video_encoder_loop(void* Data);
    

    int                m_totalFrame;
    int               *m_pVideoFrameSize;
    bool               m_firstTime;
    unsigned int       m_startDTS;
    BYTE              *m_pRingWrPtr;
    BYTE              *m_pRingRdPtr;
    BYTE              *m_pRingUpper;
    BYTE              *m_pRingLower;
    long               m_pRingBuffSize;
    FILTER_STATE       m_EncodeState;
    FILTER_STATE       m_EncodeStateNext;
    
    // for msg ring buffer
    BYTE              **m_pMsgRingWrPtr;
    BYTE              *m_pMsgRingRdPtr;
    BYTE              *m_pMsgRingUpper;
    BYTE              *m_pMsgRingLower;
    long               m_pMsgRingBuffSize;
  
  
};

#endif


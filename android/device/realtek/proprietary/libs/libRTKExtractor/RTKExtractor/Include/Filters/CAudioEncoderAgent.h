#ifndef __C_VIDEO_ENCODER_AGENT_H__
#define __C_VIDEO_ENCODER_AGENT_H__


#include <CStream.h>
#include "../../Filters/AudioEncoder/rpc_func_map.h"



class CAudioEncoderAgent : public CBaseFilter {

public:
    CAudioEncoderAgent();
    ~CAudioEncoderAgent();

    HRESULT     SetInstanceID(long instanceID);
    HRESULT     Run(void);
    HRESULT     Stop(void);
    HRESULT     Pause(FILTER_STATE State);
    HRESULT     PauseEncode();
    HRESULT     StartEncode();
    HRESULT     StopEncode();
    HRESULT     InitRingBuffer(AUDIO_RPC_RINGBUFFER_HEADER* pRingBuffer);
    HRESULT     SetSourceFile(char *);
    HRESULT     myRequestWriteSpace(BYTE **pRingWrPtr, BYTE **pRingRdPtr, int frameSize);

    CThread*    m_pMPEG2Thread;    //the thread for MPEG2 decoder
    
protected:
    long             m_instanceID;
    FILE            *m_StreamFileHandle;
    char            *m_StreamFileName;
    CLNT_STRUCT     m_rpcclnt;
    

private:
    //BYTE 	m_Flag;
    /* static function only can access static member datas
    *  thus we must pass in the instance   */
    static void audio_encoder_loop(void* Data);
    

    bool                    m_firstTime;
    unsigned int           m_startPTS;
    BYTE                   *m_pRingWrPtr;
    BYTE                   *m_pRingRdPtr;
    BYTE                   *m_pRingUpper;
    BYTE                   *m_pRingLower;
    long                    m_pRingBuffSize;
    FILTER_STATE            m_EncodeState;
    FILTER_STATE            m_EncodeStateNext;
    // for msg ring buffer
    BYTE              **m_pMsgRingWrPtr;
    BYTE              *m_pMsgRingRdPtr;
    BYTE              *m_pMsgRingUpper;
    BYTE              *m_pMsgRingLower;
    long               m_pMsgRingBuffSize;


    
};

#endif

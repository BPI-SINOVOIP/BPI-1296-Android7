/********************************************************************************/
/*    File: CAVIMuxFilter.h                                                     */
/*    Desc: Header file for AVI Mux Filter Class interface define               */
/*    Author: Yuantao Wu     04/25/2007                                         */
/*            Yi-Chian Yang  04/25/2007                                         */
/*    Copyright (c)RealTek.   All rights reserved.                              */
/********************************************************************************/

#ifndef __CAVI_MUXER_FILTER_H__
#define __CAVI_MUXER_FILTER_H__

#include <OSAL.h>
#include <Util.h>
#include <CStream.h>
#include <Filters/CBaseMuxFilter.h>
#include <VideoRPCBaseDS_data.h>
#include <VideoRPC_Agent_data.h>
#include <AudioRPCBaseDS_data.h>
#include <AudioRPC_Agent_data.h>

//define input elementary type
typedef enum {
	AVI_MUX_AUDIO=0, 	
	AVI_MUX_VIDEO, 
	NUMBER_OF_AVIMUXER_INPUT
} AVI_MUX_INPUTSTREAMTYPE;


// output info type
typedef enum _ENUM_AVI_MUX_OUTPUT_INFO_TYPE {
    AVIMUX_OUTPUT_RECORD_GEN,
    AVIMUX_OUTPUT_AVCHUNKS,
    AVIMUX_OUTPUT_EOS,
    AVIMUX_OUTPUT_TOTAL
} ENUM_AVI_MUX_OUTPUT_INFO_TYPE;

// Mux output info format -- GenInfo
typedef struct _tag_avi_stream_gen_info {
    ENUM_AVI_MUX_OUTPUT_INFO_TYPE infoType;
    ENUM_MEDIA_TYPE TVSYStemFormat;
    unsigned int videoBitRate;
    unsigned int audioRate;
} AVI_STREAM_GEN_INFO;

// Mux output info format -- AVChunkInfo
typedef struct _tag_avi_stream_avchunks_info {
    ENUM_AVI_MUX_OUTPUT_INFO_TYPE infoType;
    int chunkNumber;
    int chunkinfo[127]; //bit 31: audio(0) or video(1); bit 30: key frame; bit 29: vobuStart
} AVI_STREAM_AVCHUNKS_INFO;

struct _AVI_MUX_DATA;

class CAVIMuxInputPin;
class CAVIMuxInfoOutputPin;
class CAVIMuxDataOutputPin;


class CAVIMuxFilter : public CBaseMuxFilter
{
friend class CAVIMuxInputPin;
friend class CAVIMuxInfoOutputPin;
friend class CAVIMuxDataOutputPin;

public:
    CAVIMuxFilter();
    ~CAVIMuxFilter();

    //derived from CBaseFilter    
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);

    void       ProcessData();
    int        InputDataEmpty();
    HRESULT    CheckOutputAvailable(int bufferNum);
    int        FindFirstAudio();
    AVI_MUX_INPUTSTREAMTYPE DecideNextChunkType(unsigned int *size);
    unsigned int CheckOutputSpace();
    HRESULT    ProcessPrivateInfo(BYTE *pInfo, int length, AVI_MUX_INPUTSTREAMTYPE input);
    HRESULT    ProcessDataInfo(BYTE *pWritePointer, long WriteBufferSize, AVI_MUX_INPUTSTREAMTYPE input);
    void       RequestOutputSpace();
    int        WriteAVIheader();
    unsigned int GenerateVideoChunk();
    unsigned int GenerateAudioChunk();
    void       CopyData(AVI_MUX_INPUTSTREAMTYPE input, unsigned int dest, unsigned int src, unsigned int size);
    void       updateOutput();
    void       CheckCondition();
    HRESULT    ProcessEndOfStream(AVI_MUX_INPUTSTREAMTYPE input);
    void       SendOutData();
    void       NormalInit();
    void       SendOutMessage(ENUM_AVI_MUX_OUTPUT_INFO_TYPE type);
private:
    // pin and allocator related
    CAVIMuxInputPin*              m_pVideoInputPin;
    CAVIMuxInputPin*              m_pAudioInputPin;
    CAVIMuxInfoOutputPin*         m_pInfoOutputPin;
    CAVIMuxDataOutputPin*         m_pDataOutputPin;  
    CMemRingAllocator*            m_pVideoInputAllocator;
    CMemRingAllocator*            m_pAudioInputAllocator;
    IReadPointerHandle*           m_pReadPointerHandle[NUMBER_OF_AVIMUXER_INPUT];    

    struct _AVI_MUX_DATA*         m_pMuxData;
};


class CAVIMuxInputPin: public CBaseInputPin
{
public:
    CAVIMuxInputPin(const char *ID);
    ~CAVIMuxInputPin();
    HRESULT ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
    HRESULT Receive(BYTE *pWritePointer, long WriteBufferSize);
    HRESULT PrivateInfo(BYTE *pInfo, int length);    
    HRESULT EndOfStream(unsigned int eventID);    
private:
	AVI_MUX_INPUTSTREAMTYPE input;
    osal_mutex_t m_pInputMutex;
};

class CAVIMuxInfoOutputPin: public CBaseOutputPin
{
public:
    HRESULT DeliverPrivateInfo(BYTE *pInfo, int length);
};

class CAVIMuxDataOutputPin: public CBaseOutputPin
{
public:
    CAVIMuxDataOutputPin();
    ~CAVIMuxDataOutputPin();
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);
    HRESULT EndOfStream(unsigned int eventID);
};

#endif // __CAVI_MUXER_FILTER_H__

                    

/********************************************************************************/
/*    File: CGeneralMuxFilter.h                                                     */
/*    Desc: Header file for ElememtaryStream Mux Filter Class interface define               */
/*    Author: Yuantao Wu     05/23/2007                                         */
/*            Yi-Chian Yang  05/23/2007                                         */
/*    Copyright (c)RealTek.   All rights reserved.                              */
/********************************************************************************/

#ifndef __CES_MUXER_FILTER_H__
#define __CES_MUXER_FILTER_H__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <OSAL.h>
#include <Util.h>
#include <CStream.h>
#include <Filters/CBaseMuxFilter.h>
#include <Filters/CDataOutput.h>
#include <io/GeneralFileAccess/file_access.h>
#include <AudioRPCBaseDS_data.h>
#include <AudioRPC_Agent_data.h>



#define ESMUX_FIRMWARE_INPUT_BUF_SIZE  (1024 * 512)
#define ESMUX_NUMBER_OF_FRAME_PER_CHUNK 5
#define ESMUX_MIN_DATA_OUTPUT 10

#define ESMUXING_IDLE   0
#define ESMUXING_RECORD 1
#define ESMUXING_STOPPING 2

#define ESMUXING_DIRECTIO_ALGNSIZE 2048

typedef enum _tag_esmux_outtype {
    ESMUX_OUTPUT_WAV = 0,
    ESMUX_OUTOUT_MP3,
    ESMUX_OUTOUT_AUDIO_MPEG,
    ESMUX_OUTPUT_UNKNOWN
} ENUM_ESMUX_OUTPUT_TYPE;

struct _ES_MUX_DATA;


class CESMuxInputPin;
class CESMuxInfoOutputPin;
class CESMuxDataOutputPin;


class CESMuxFilter : public CBaseMuxFilter
{
friend class CESMuxInputPin;
friend class CESMuxInfoOutputPin;
friend class CESMuxDataOutputPin;

public:
    CESMuxFilter();
    ~CESMuxFilter();

    //derived from CBaseFilter    
    HRESULT    Run(void);
    HRESULT    Stop(void);
    HRESULT    Pause(FILTER_STATE State);

    void       ProcessData();
    int        InputDataEmpty();
    HRESULT    ProcessDataInfo(BYTE *pWritePointer, long WriteBufferSize);
    HRESULT    ProcessPrivateInfo(BYTE *pInfo, int length);    
    unsigned int CheckOutputSpace();
    void         RequestOutputSpace();
    unsigned int CheckCondition();
    void         SendOutData();
    void         WriteFileheader();
    HRESULT      ProcessEndOfStream();
    unsigned int GenerateChunk();
    void         updateOutput();
    void         CopyData(unsigned int dest, unsigned int src, unsigned int size, bool needChangeEndian);
    HRESULT      CheckOutputAvailable(int bufferNum);
    void         NormalInit();   
    
    int createWAVHeader(void *ptr);
    int createMP3ID3v1Tag();
    
private:
    ENUM_ESMUX_OUTPUT_TYPE          m_outputType;
    // pin and allocator related
    CESMuxInputPin*                 m_pFirmwareInputPin;  
    CMemRingAllocator*              m_pFirmwareInputAllocator;
    CESMuxInfoOutputPin*            m_pInfoOutputPin;
    CESMuxDataOutputPin*            m_pDataOutputPin;   
    CBaseInputPin*                  m_pNoUseInputPin;
    CMemRingAllocator*              m_pNoUseInputAllocator;
    int                             m_fileHandle;
    char                            m_filePath[1024];
    off64_t                         m_fileOffset;
    IReadPointerHandle*             m_pReadPointerHandle;
    
    struct _ES_MUX_DATA*            m_pMuxData;
};


class CESMuxInputPin: public CBaseInputPin
{
public:
    CESMuxInputPin(const char *ID);
    ~CESMuxInputPin();
    HRESULT ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
    HRESULT Receive(BYTE *pWritePointer, long WriteBufferSize);
    HRESULT PrivateInfo(BYTE *pInfo, int length);    
    HRESULT EndOfStream(unsigned int eventID);    
private:
    osal_mutex_t m_pInputMutex;
};


class CESMuxInfoOutputPin: public CBaseOutputPin
{
public:
    HRESULT DeliverPrivateInfo(BYTE *pInfo, int length);
};

class CESMuxDataOutputPin: public CBaseOutputPin
{
public:
    CESMuxDataOutputPin();
    ~CESMuxDataOutputPin();
    HRESULT Connect(IPin *pReceivePin, const MEDIA_TYPE *pmt);
    HRESULT EndOfStream(unsigned int eventID);
};
    
#endif //__CES_MUXER_FILTER_H__


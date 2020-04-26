#ifndef __C_PIN_H__
#define __C_PIN_H__

#include <OSAL.h>
#include <IStream.h>
#include <Util.h>
#include "CObject.h"
#include "CMediaType.h"

class   CBaseFilter;
class   CBasePin;
class   CBaseInputPin;
class   CBaseOutputPin;

#define IS_PIN_CONNECTED(pPin)   (pPin->ConnectedTo(NULL, NULL)== S_OK)

class CBasePin: public IPin, public CObject{
friend class                    CBaseFilter;
public:
// CPin method
                                CBasePin(
				    // const char*             pPinName,
				    // CBaseFilter*            pBaseFilter,
				    // CCritSec*               pLock,
				    const PIN_DIRECTION     dir
				);

    virtual                     ~CBasePin(void);

// IPin method
    HRESULT                     Connect(
		                    IPin*                   pReceivePin,
		                    const MEDIA_TYPE*       pmt
				);
    HRESULT                     ConnectionMediaType(
		                    MEDIA_TYPE*             pmt
				);
    HRESULT                     UpdateConnectionMediaType(
                                    MEDIA_TYPE*            pmt
                                );
    HRESULT                     EndOfStream(unsigned int eventID);
    HRESULT                     QueryPinInfo(
		                    PIN_INFO*               pInfo
				);
    HRESULT                     QueryAccept(
		                    const MEDIA_TYPE*       pmt
				);
    HRESULT                     NewSegment(void);

    HRESULT                     ReceiveConnection(
		                    IPin*                   pConnector,
                                    const MEDIA_TYPE*       pmt
				);
    HRESULT                     GetStatus(PIN_STATUS        *pStatus) { *pStatus = m_status; return S_OK;}
    HRESULT                     SetStatus(PIN_STATUS        status)   { m_status = status;  return S_OK; }


protected:
    bool                        IsStopped(void) const;

    bool                        GetState(
                                    FILTER_STATE*           pState
				) const;

    bool                        JoinFilter(
                                    CBaseFilter*            pFilter,
                                    char*                   Id
				);
    virtual bool                IsConnected(void) {return true;};

protected:
    CBaseFilter*                m_pFilter;
    PIN_DIRECTION               m_dir;
    char*                       m_pName;
    CMediaType                  m_mt;
    PIN_STATUS                  m_status;
};

class CBaseInputPin: public CBasePin, public IMemInputPin{
friend class                    CBasePin;
friend class                    CBaseOutputPin;
public:
	                        CBaseInputPin(void);
    virtual                     ~CBaseInputPin(void);

    HRESULT                     Flush(void);
    HRESULT                     Receive(
                                    IMediaSample*           pSample
                                );
    HRESULT                     Receive(
                                    BYTE*                   pWritePointer,
                                    long                    WriteBufferSize
                                );
    HRESULT                     ReceiveCanBlock(void);
    HRESULT                     GetAllocator(
                                    IMemAllocator**         ppAllocator
                                );
    HRESULT                     SetAllocator(
                                    IMemAllocator*          pAllocator
                                );
    HRESULT                     PrivateInfo(
                                    int                     infoId,
                                    BYTE*                   pInfo,
                                    int                     length
                                );
    HRESULT                     PrivateInfo(
                                    BYTE*                   pInfo,
                                    int                     length
                                );
    HRESULT                     Disconnect(void);
    HRESULT                     ConnectedTo(
                                    IPin***                 ppPinArr,
                                    int*                    pPinArrSize
                                );
    HRESULT                     Block();    // function will block data receive, privateInfo ..
    HRESULT                     Unblock();

    osal_mutex_t                m_flowMutex; // mutex to prevent pin thread colide with flow thread
protected:
    virtual HRESULT             CheckStreaming(void);

    bool                        IsConnected(void);

protected:
    unsigned int                m_flag;
    IMemAllocator*              m_pAllocator;
    CBaseOutputPin*             m_pOutputPin;
};


typedef ulDArray<CBaseInputPin*>        CInputPinArr;
class CBaseOutputPin: public CBasePin, public IMemOutputPin{
friend class                    CBasePin;
friend class                    CBaseInputPin;
public:
	                        CBaseOutputPin(void);
    virtual                     ~CBaseOutputPin(void);
    HRESULT                     Deliver(
                                    IMediaSample*           pSample
				);
    HRESULT                     Deliver(
                                    BYTE*                   ppWritePointer,
                                    long                    WriteBufferSize
                                );

    HRESULT                     SetWritePointer(BYTE* wrPtr);
    HRESULT                     DeliverFlush(void);
    HRESULT                     GetAllocator(
                                    IMemAllocator**         ppAllocator
                                );
    HRESULT                     SetAllocator(
                                    IMemAllocator*          pAllocator
                                );
    HRESULT                     DeliverPrivateInfo(
                                    int                     infoId,
                                    BYTE*                   pInfo,
                                    int                     length
                                );
    HRESULT                     DeliverPrivateInfo(
                                    BYTE*                   pInfo,
                                    int                     length
                                );
    HRESULT                     Disconnect(void);
    HRESULT                     ConnectedTo(
                                    IPin***                 ppPinArr,
                                    int*                    pPinArrSize
                                );
    HRESULT                     UpdateConnectionMediaType(
                                    MEDIA_TYPE*            pmt
                                );
    HRESULT                     EndOfStream(unsigned int eventID);
    HRESULT                     NewSegment(void);
protected:
    bool                        Stop(void);
    bool                        Pause(void);
    bool                        Run(void);

    bool                        IsConnected(void);
protected:
    CInputPinArr                m_InputPinArr;

};
#endif

#ifndef __I_STREAM_H__
#define __I_STREAM_H__

#include <OSAL.h>
#include <hresult.h>
#include <StreamClass/EType.h>
#include <StreamClass/SType.h>
#include <FlowEvent.h>
//#include <IPC/rpc_common.h>
#include <rpc_common.h>

#define PURE =0

class IPin;
class IMemInputPin;
class IEnumPins;
class IMediaSample;
class IMemAllocator;
class IReferenceClock;
class IEnumFilters;
class IReadPointerHandle;

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
class IPin{
public:
    virtual                     ~IPin(void) {};
    virtual HRESULT             Connect(
		                    IPin*                   pReceivePin,
		                    const MEDIA_TYPE*      pmt
				) PURE;
    virtual HRESULT             ConnectedTo(
                                    IPin***                 ppPinArr,
                                    int*                    pPinArrSize
				) PURE;
    virtual HRESULT             ReceiveConnection(
		                    IPin*                   pConnector,
                                    const MEDIA_TYPE*      pmt
				) PURE;
    virtual HRESULT             ConnectionMediaType(
		                    MEDIA_TYPE*            pmt
				) PURE;
    virtual HRESULT             UpdateConnectionMediaType(
		                    MEDIA_TYPE*            pmt
				) PURE;
#if 0
    virtual HRESULT             Disconnect(
                                    const bool             bRecursion= true
                                ) PURE; 
#else
    virtual HRESULT             Disconnect(void) PURE; 
#endif
    virtual HRESULT             EndOfStream(unsigned int eventID) PURE;
    virtual HRESULT             QueryPinInfo(
		                    PIN_INFO*               pInfo
				) PURE;
    virtual HRESULT             QueryAccept(
		                    const MEDIA_TYPE*      pmt
				) PURE;
    virtual HRESULT             NewSegment(void) PURE;
    virtual HRESULT             GetAllocator(
                                    IMemAllocator**         ppAllocator
                                ) PURE;
    virtual HRESULT             SetAllocator(
                                    IMemAllocator*          pAllocator
                                ) PURE;
    virtual HRESULT             GetStatus(PIN_STATUS        *pStatus) PURE;
    virtual HRESULT             SetStatus(PIN_STATUS        status) PURE;
};

class IMemInputPin{
public:
    virtual                     ~IMemInputPin(void) {};
    virtual HRESULT             Flush(void) PURE;
    virtual HRESULT             Receive(
                                    IMediaSample*     pSample
				) PURE;
    virtual HRESULT             Receive(
                                    BYTE*                   pWritePointer,
                                    long                    WriteBufferSize
                                ) PURE;

    // virtual HRESULT             ReceiveMultiple() PURE;
    virtual HRESULT             ReceiveCanBlock(void) PURE;

    virtual HRESULT             PrivateInfo(
                                    int                     infoId,
                                    BYTE*                   pInfo,
                                    int                     length
                                ) PURE;

    // the function below is becoming obselete, will be removed later
    virtual HRESULT             PrivateInfo(
                                    BYTE*                   pInfo,
                                    int                     length
                                ) PURE;
};

class IMemOutputPin{
public:
    virtual                     ~IMemOutputPin(void) {};
    virtual HRESULT             Deliver(
                                    IMediaSample*           pSample
                                ) PURE;

    virtual HRESULT             Deliver(
                                    BYTE*                   ppWritePointer,
                                    long                    WriteBufferSize
                                ) PURE;

    virtual HRESULT             DeliverFlush(void) PURE;
    virtual HRESULT             DeliverPrivateInfo(
                                    int                     infoId,
                                    BYTE*                   pInfo,
                                    int                     length
                                ) PURE;

    // the function below is becoming obselete, will be removed later
    virtual HRESULT             DeliverPrivateInfo(
                                    BYTE*                   pInfo,
                                    int                     length
                                ) PURE;
};

class IBaseFilter{
public:
    virtual                     ~IBaseFilter(void) {};
    virtual HRESULT             EnumPins(
		                    IEnumPins**             ppEnum
				) PURE;
    virtual HRESULT             FindPin(
                                    IPin**                  ppPin,
                                    char*                   Id
                                ) PURE;
    virtual HRESULT             JoinFlow(
                                    IFlowManager*           pFlow,
                                    char*                   pName
                                ) PURE;
    virtual HRESULT             QueryFilterInfo(
                                    FILTER_INFO*            pInfo
                                ) PURE;
    virtual HRESULT             FilterType(
                                    FILTER_TYPE*            pFilterType
                                ) PURE;
    virtual HRESULT             SetFilterProperty(
                                    FILTER_PROPERTY         propId,
                                    void*                   propData,
                                    unsigned int            propSize
                                ) PURE;
    
    virtual HRESULT             GetAgentInstanceID(
    								long* 					ID
    							) PURE;
                                
// protected:
    // virtual HRESULT             AddPin(IPin* ppPin);
};

class IEnumPins{
public:
    virtual                     ~IEnumPins(void) {};
    virtual HRESULT             Next(
                                    IPin**                  ppIPin
                                ) PURE;
    virtual HRESULT             Reset(void) PURE;
};

class IEnumFilters{
public:
    virtual                     ~IEnumFilters(void) {};
    virtual HRESULT             Next(
                                    IBaseFilter**           ppIFilter
                                ) PURE;
    virtual HRESULT             Reset(void) PURE;
};

class IFlowManager{
public:
    virtual                     ~IFlowManager(void) {};
    virtual HRESULT             AddFilter(
                                    IBaseFilter*            pFilter,
                                    char*                   Id
                                ) PURE;
    virtual HRESULT             ConnectDirect(
                                    IPin*                   pPinOut,
                                    IPin*                   pPinIn,
                                    const MEDIA_TYPE*       pmt
                                ) PURE;
    virtual HRESULT             Disconnect(
                                    IPin*                   pPin
                                ) PURE;
    virtual HRESULT             EnumFilters(
                                    IEnumFilters**          ppEnumFilters
                                ) PURE;
    virtual HRESULT             FindFilterByName(
                                    IBaseFilter**           ppFilter,
                                    char*                   Id
                                ) PURE;
    virtual HRESULT             RemoveFilter(
                                    IBaseFilter*            pFilter
                                ) PURE;
    virtual HRESULT             SetRefClock(
                                    IReferenceClock*        pClock
                                ) PURE;
    virtual HRESULT             Notify(
                                    EVCode                  EventCode,
                                    long                    ParamSize,
                                    long*                   pEventParams
                                ) PURE;
};

class IFilterControl{
public:
    virtual                     ~IFilterControl(void) {};
    virtual HRESULT             Stop(void) PURE;
    virtual HRESULT             Pause(
                                    FILTER_STATE            State
                                ) PURE;
    virtual HRESULT             Run(void) PURE;
    virtual HRESULT             Step(void) PURE;
    virtual HRESULT             GetState(
                                    FILTER_STATE*           pState
                                ) PURE;
    virtual HRESULT             GetRefClock(
                                    IReferenceClock**       ppClock
                                ) PURE;
    virtual HRESULT             SetRefClock(
                                    IReferenceClock*        pClock
                                ) PURE;
    virtual HRESULT             GetMediaInfo(
                                    MEDIA_INFO*             pInfo
                                ) PURE;
//#ifndef NDEBUG
    virtual HRESULT             ExecuteCommandLine(
                                    char*                   command
                                ) PURE;
//#endif
};

class IFlowContorl{
public:
    virtual                     ~IFlowContorl(void) {};
    virtual HRESULT             Run(void) PURE;
    virtual HRESULT             Pause(void) PURE;
    virtual HRESULT             Stop(void) PURE;
    virtual HRESULT             Step(void) PURE;
    virtual HRESULT             GetState(
                                    long                    msTimeOut,
                                    FILTER_STATE*           pState
                                ) PURE;
};


// just for compilation; remove it afterward

class IFlowEvent{
public:
    virtual                     ~IFlowEvent(void) {};
    virtual HRESULT             CancelDefaultHandling(
                                    EVCode                  EventCode
                                ) PURE;
    virtual HRESULT             FreeEventParams(
                                    EVId                    EventId
                                ) PURE;
    virtual HRESULT             GetEvent(
                                    EVId*                   pEventId,
                                    EVCode*                 pEventCode,
                                    long*                   pParamSize,
                                    long**                  ppParams,
                                    long                    msTimeout
                                ) PURE;
    virtual HRESULT             FlushUserEvents(void) PURE;
    // virtual HRESULT             RestoreDefaultHandling(
    //                                 long                    EventCode
    //                             ) PURE;
};


class IFilterSeeking{
public:
    virtual                     ~IFilterSeeking(void) {};
    virtual HRESULT             CheckCapabilities(
                                    FILTER_SEEKING_CAP*     pSeekingCap
                                ) PURE;
    virtual HRESULT             SetSeekingInfo(
                                    long                    Speed,
                                    long                    Skip
                                ) PURE;
    virtual HRESULT             GetSeekingInfo(
                                    long*                   pSpeed,
                                    long*                   pSkip
                                ) PURE;
};

class IFlowSeeking{
public:
    virtual                     ~IFlowSeeking(void) {};
    virtual HRESULT             CheckCapabilities(
                                    FILTER_SEEKING_CAP*     pSeekingCap
                                ) PURE;
    virtual HRESULT             IsFormatSupported(
                                    TIME_FORMAT             format
                                ) PURE;
    virtual HRESULT             GetTimeFormat(
                                    TIME_FORMAT*            pFormat
                                ) PURE;
    virtual HRESULT             SetTimeFormat(
                                    TIME_FORMAT             format
                                ) PURE;
    virtual HRESULT             SetRate(
                                    long                    rate
                                ) PURE;
    virtual HRESULT             GetRate(
                                    long*                   pRate
                                ) PURE;
    virtual HRESULT             SetPositions(
                                    long*                   pCurrent,
                                    long                    dwCurrentFlags,
                                    long*                   pStop,
                                    long                    dwStopFlags
                                ) PURE;
    virtual HRESULT             GetPositions(
                                    long*                   pCurrent,
                                    long*                   pStop
                                ) PURE;
    virtual HRESULT             ConverTimeFormat(
                                    long*                   pTarget,
                                    TIME_FORMAT             format,
                                    long                    Source,
                                    TIME_FORMAT             SourceFormat
                                ) PURE;
};

class IMemAllocator{
public:
    virtual                     ~IMemAllocator(void) {};
    virtual HRESULT             Commit(void) PURE;
    virtual HRESULT             Decommit(
                                    bool                    bFreeBuffer
                                ) PURE;
    virtual HRESULT             IsCommit(void) PURE;
    virtual HRESULT             GetStyle(
                                    ALLOCATOR_STYLE*        pAllocatorStyle
                                ) PURE;
};

class IMemListAllocator: public IMemAllocator{
public:
    virtual                     ~IMemListAllocator(void) {};
    virtual HRESULT             SetProperties(
                                    ALLOCATOR_PROPERTIES*   pRequest,
				    ALLOCATOR_PROPERTIES*   pActual
                                ) PURE;
    virtual HRESULT             GetProperties(
                                    ALLOCATOR_PROPERTIES*   pProps
                                ) PURE;
    virtual HRESULT             GetBuffer(
                                    IMediaSample**          ppBuffer,
                                    ALLOCATOR_WAIT          waitFlag
				) PURE;
    virtual HRESULT             ReleaseBuffer(
                                    IMediaSample*           pBuffer
                                ) PURE;
};

class IMemRingAllocator: public IMemAllocator{
public:
    virtual                     ~IMemRingAllocator(void) {};
    virtual HRESULT             GetBuffer(
                                    BYTE**                  ppBuffer,
                                    long*                   pSize
                                ) PURE;  
    virtual HRESULT             SetBuffer(
                                    BYTE*                   pBuffer,
                                    long                    Size
                                ) PURE;
    virtual HRESULT             GetWritePointer(
                                    BYTE**                  ppWritePointer,
                                    long*                   pWriteBufferSize
                                ) PURE;
    virtual HRESULT             SetWritePointer(
                                    BYTE*                   pWritePointer
                                ) PURE;
    virtual HRESULT             RequestWriteSpace(
                                    BYTE**                  ppWritePointer,
                                    long*                   pWriteBufferSize,
                                    long                    RequestSize,
                                    long                    MilliTimeOut
                                ) PURE;
    virtual HRESULT             GetReadPointerHandle(
                                    IReadPointerHandle**    ppReadPointer,
				    IMemInputPin*           pInputPin
                                ) PURE;
    virtual HRESULT             ReleaseReadPointerHandle(
                                    IReadPointerHandle*     pReadPointer
                                ) PURE;
};

class IMediaSample{
public:
    virtual                     ~IMediaSample(void) {};
    virtual HRESULT             GetActualSize(
                                    ULONG*                  pActualSize
                                ) PURE;
    virtual HRESULT             GetAcutaulPointer(
                                    BYTE**                  pActualPointer
                                ) PURE;
    virtual HRESULT             GetBeginSize(
                                    ULONG*                  pBeginSize
                                ) PURE;
    virtual HRESULT             GetBeginPointer(
                                    BYTE**                  pBeginPointer
                                ) PURE;
    virtual ULONG               GetPhysicalAddress(
                                    ULONG                   cachedMemAddr
                                ) PURE;
    virtual ULONG               GetNonCachedAddress(
                                    ULONG                   cachedMemAddr
				) PURE;
    virtual  HRESULT            GetNonCachedBeginPointer(    
				    BYTE**                  pBeginPointer) PURE;

    // set
    virtual HRESULT             SetActualPointer(
                                    BYTE*                   pBuffer,
				    ULONG                   size
                                ) PURE;

    virtual HRESULT             Release(void) PURE;
    virtual HRESULT             AddRef(void) PURE;
};

class IReadPointerHandle{
public:
    virtual                     ~IReadPointerHandle(void) {};
    virtual HRESULT             GetReadPointer(
                                    BYTE**                  ppReadPointer,
                                    long*                   pReadSize
                                ) PURE;
    virtual HRESULT             SetReadPointer(
                                    BYTE*                   pReadPointer
                                ) PURE;
    virtual HRESULT             GetRingBuffer(
                                    BYTE**                  ppRingBuffer,
                                    long*                   pRingBufferSize
                                ) PURE;
    virtual HRESULT             Flush(void) PURE;
    virtual HRESULT             Release(void) PURE;
};

class IReferenceClock{
public:
  virtual         ~IReferenceClock(void) {};
  virtual HRESULT  GetGlobalSCR(int64_t *pSCR)=0;  // 27MHz  
  virtual HRESULT  GetGlobalPTS(int64_t *pPTS)=0;
  virtual HRESULT  GetLocalPTS (int64_t *pPTS)=0;
  virtual HRESULT  SetLocalPTS (int64_t PTS)=0;
  virtual HRESULT  GetRCD (int64_t *pRCD)=0;
  virtual HRESULT  SetRCD (int64_t RCD)=0;
  virtual HRESULT  SetTimeout(int64_t timeout)=0;
  virtual HRESULT  SetFreeRunThreshold(unsigned long video, unsigned long audio)=0;
  virtual HRESULT  SetMastership(MASTERSHIP *mastership)=0;
  virtual HRESULT  GetMastership(MASTERSHIP *mastership)=0;
  virtual HRESULT  GetPresentationPositions(PRESENTATION_POSITIONS *pos)=0;
  virtual HRESULT  ResetPresentationPositions()=0;
  virtual HRESULT  GetUnderflowStates(long* video, long* audio, int64_t *RCD)=0;
  virtual HRESULT  ResetUnderflowStates()=0;
};


#endif

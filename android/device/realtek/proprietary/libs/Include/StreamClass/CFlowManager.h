#ifndef __C_FLOW_MANAGER_H__
#define __C_FLOW_MANAGER_H__

#include <IStream.h>
#include <Util.h>
#include <StreamClass/CObject.h>
#include <StreamClass/CReferenceClock.h>

class CBaseFilter;

// IFlowEvent definitions
#define MAX_FLOW_EVENT_ITEMS  32
#define FLOW_EVENT_QUEUE_SIZE 512 // DWORDs
// ----------------------------------------


#define __FILTER_ORDER__

typedef ulDArray<IBaseFilter*>  IFilterArr;

class CFlowManager: public      IFlowManager,
                    public      IFlowContorl,
                    public      IFlowSeeking,
                    public      IFlowEvent,
                    public      CObject{
friend class                    CEnumFilters;
friend class                    CBaseFilter;
public:
                                CFlowManager(
                                    bool                    bNeedRefClock= false
                                );
    virtual                     ~CFlowManager(void);

    // IFlowManager methods
    HRESULT                     AddFilter(
                                    IBaseFilter*            pFilter,
                                    char*                   Id
                                );
    HRESULT                     ConnectDirect(
                                    IPin*                   pPinOut,
                                    IPin*                   pPinIn,
                                    const MEDIA_TYPE*       pmt
                                );
    HRESULT                     Disconnect(
                                    IPin*                   pPin
                                );
    HRESULT                     EnumFilters(
                                    IEnumFilters**          ppEnumFilters
                                );
    HRESULT                     FindFilterByName(
                                    IBaseFilter**           ppFilter,
                                    char*                   Id
                                );
    HRESULT                     RemoveFilter(
                                    IBaseFilter*            pFilter
                                );
    HRESULT                     SetRefClock(
                                    IReferenceClock*        pClock
                                );

    HRESULT                     SendCommand(
					    char*                   command);

    // IFlowContorl methods
    HRESULT                     Run(void);
    HRESULT                     Pause(void);
    HRESULT                     Stop(void);
    HRESULT                     Step(void);
    HRESULT                     GetState(
                                    long                    msTimeOut,
                                    FILTER_STATE*           pState
                                );

    // IFlowSeeking methods
    HRESULT                     CheckCapabilities(
                                    FILTER_SEEKING_CAP*     pSeekingCap
                                );
    HRESULT                     IsFormatSupported(
                                    TIME_FORMAT             format
                                );
    HRESULT                     GetTimeFormat(
                                    TIME_FORMAT*            pFormat
                                );
    HRESULT                     SetTimeFormat(
                                    TIME_FORMAT             format
                                );
    HRESULT                     SetRate(
                                    long                    rate
                                );
	HRESULT						CanSetRate(
									long rate
								);

    HRESULT                     GetRate(
                                    long*                   pRate
                                );
    HRESULT                     SetPositions(
                                    long*                   pCurrent,
                                    long                    dwCurrentFlags,
                                    long*                   pStop,
                                    long                    dwStopFlags
                                );
    HRESULT                     GetPositions(
                                    long*                   pCurrent,
                                    long*                   pStop
                                );
    HRESULT                     ConverTimeFormat(
                                    long*                   pTarget,
                                    TIME_FORMAT             format,
                                    long                    Source,
                                    TIME_FORMAT             SourceFormat
                                );


	HRESULT						StartUpdateFlowState() 
								{ 
									if(osal_MutexTimedLock(&m_stateMutex, 500000000) != S_OK)
										return E_TIMEOUT;
									return S_OK;
								}
	void						StopUpdateFlowState()  {osal_MutexUnlock(&m_stateMutex);}

    // IFlowEvent
public:

    HRESULT GetEvent(
        EVId*   pEventId,
        EVCode* pEventCode,
        long*   pParamSize,
        long**  ppParams,
        long    msTimeout
    );

    HRESULT FlushUserEvents(void);

    HRESULT FreeEventParams(
        EVId EventId
    );

    HRESULT CancelDefaultHandling(
        EVCode EventCode
    );

    HRESULT RestoreDefaultHandling(
        EVCode EventCode
    );

    // IFlowEventSink

    HRESULT Notify(
        EVCode EventCode,
        long   ParamSize,
        long*  pEventParams
    );

protected:

    static void Thread(
        void* pInstance
    );

    HRESULT StartDefaultHandlingThread();
    HRESULT StopDefaultHandlingThread();

    virtual void HandleDefaultEvents(
        EVCode evCode,
        long*  pParams,
        long   paramSize
    );

    virtual bool HandleCriticalEvents(
        EVCode evCode,
        long*  pParams,
        long   paramSize
    );

    virtual bool HasDefaultHandling(
        EVCode evCode
    );

    osal_thread_t m_thread;
    osal_mutex_t m_setRateMutex;
    bool m_bThreadRunning;
    CVariableSizeQueue<FLOW_EVENT_QUEUE_SIZE,MAX_FLOW_EVENT_ITEMS> m_EventDefQueue, m_EventUserQueue;
    long m_lastEventID;

    // -------------------------------------------------------------------

protected:
    bool                        IsFilterSync();
    void                        SetFilterSync(void);

#ifdef __FILTER_ORDER__
    bool                        ReOrderFilter(void);
    void                        InitFilterLevel(void);
    static int                  SortFilterByLevel(
                                    const void*             p1,
                                    const void*             p2
                                );
    void                        Leveling(
                                    CBaseFilter*            pSeed,
                                    int                     level,
                                    int                     direction
                                );
#endif



protected:
    IReferenceClock*            m_pRefClock;
    IFilterArr                  m_IFilterArr;
    BYTE                        m_Flag;
    long                        m_rate;
    bool                        m_bStepping;
    bool                        m_bNeedRefClock;
    MASTERSHIP                  m_mastershipBeforeStepping; // saved mastership status before start stepping

	osal_mutex_t 				m_stateMutex;
};

#endif

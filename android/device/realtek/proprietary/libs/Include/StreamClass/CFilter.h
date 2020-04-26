#ifndef __C_FILTER_H__
#define __C_FILTER_H__

#include <OSAL.h>
#include <IStream.h>
#include <Util.h>
#include "CObject.h"

// #define PIN_ARR_INC             2

class   CBaseOutputPin;
class   CBaseInputPin;
class   IFlowManager;
class   CBasePin;
// class   CEnumPins;

// typedef DArray<CBaseOutputPin*> CPinArrOut;
// typedef DArray<CBaseInputPin*>  CPinArrIn;

typedef ulDArray<IPin*>            IPinArr;

class CBaseFilter: public IBaseFilter, public IFilterControl, public CObject{
friend class                    CBasePin;
friend class                    CBaseInputPin;
friend class                    CEnumPins;
friend class                    CFlowManager;
public:

 IFlowManager*               m_pFlowManager;

                                CBaseFilter(
			            // const char*             pName
				    // CCritSec*               pLock
                                );

				~CBaseFilter(void);

    // IBaseFilter method
    HRESULT                     EnumPins(
                                    IEnumPins**             ppEnumPins
                                );
    HRESULT                     FindPin(
                                    IPin**                  ppPin,
                                    char*                   Id
                                );
    HRESULT                     QueryFilterInfo(
		                    FILTER_INFO*            pInfo
                                );
    HRESULT                     JoinFlow(
		                    IFlowManager*           pFlow,
				    char*                   pName
                                );
    HRESULT                     FilterType(
                                    FILTER_TYPE*            pFilterType
                                );
    HRESULT                     SetFilterProperty(
                                    FILTER_PROPERTY         propId,
                                    void*                   propData,
                                    unsigned int            propSize
                                );

 	HRESULT             		GetAgentInstanceID(
 									long* 					ID
 								);
    // IFilterControl method
    HRESULT                     Stop(void);
    HRESULT                     Pause(
                                    FILTER_STATE            State
                                );
    HRESULT                     Run(void);
    HRESULT                     Step(void);
    HRESULT                     GetState(
                                    FILTER_STATE*           pState
                                );
    HRESULT                     GetRefClock(
                                    IReferenceClock**       ppClock
                                );
    HRESULT                     SetRefClock(
                                    IReferenceClock*        pClock
                                );
    HRESULT                     GetMediaInfo(
                                    MEDIA_INFO*             pInfo
                                );
//#ifndef NDEBUG
    HRESULT                     ExecuteCommandLine(
                                    char*                   command
                                );
//#endif

protected:
    bool                        AddPin(
                                    CBasePin*               pCPin,
                                    char*                   Id
                                );


    bool                        IsPinSync(void);
    void                        SetPinSync(void);
    // void                        ResetPinSync(void);
    bool                        RemoveFromFlow(void);

    bool                        IsFlush(void);
    bool                        FlushRequest(void);
    bool                        FlushWaitComplete(void);
    bool                        FlushPostComplete(void);

protected:
    FILTER_STATE                m_State;
    FILTER_STATE                m_StateNext;
    IPinArr                     m_IPinArr;
    char*                       m_pName;
    BYTE                        m_Flag;
    void*                       m_pUserData;
    
    // IReferenceClock*            m_pReferClock;
    CCritSec                    m_Lock;
    CSemaphore                  m_SemFlush;

    static const char*          m_ClassId;
};

#endif

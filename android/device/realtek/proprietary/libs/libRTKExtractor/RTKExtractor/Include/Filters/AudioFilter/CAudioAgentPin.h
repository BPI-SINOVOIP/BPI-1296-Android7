

/**
/////////////////////////////////////////////////////////////////////////////////
//  @file CAudioAgentPin.h
//
//  @brief Header file for DVD-VENUS Audio Agent Pin Class interface define
//
//  @author Taro     08/31/2004
//
//  copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_AGENT_PIN_H__
#define __C_AUDIO_AGENT_PIN_H__


#include <CStream.h>
#include "CMultiPin.h"

/** 
class CAudioAgentInputPin : public CBaseInputPin
*/

class CAudioAgentInputPin : public CBaseInputPin
{

    friend class CAudioAgent;
    friend class CAudioTransAgent;

public:
    CAudioAgentInputPin();
    virtual ~CAudioAgentInputPin() {};

    //  set read pointer to allocator and if need, send RPC call
    virtual HRESULT             SetReadPointer(BYTE* pReadPointer);

    HRESULT                     EndOfStream(unsigned int eventID);


    virtual bool                IsEndOfStream(void);
    virtual void                ResetEndOfStream(void);

protected:
     
    HRESULT                     UpdateFilterReadPtr(BYTE* pReadPointer);

protected:
    int                         m_Flag;
    
private:

    
};


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


class CAudioAgentBaseOutputPin : public CBaseOutputPin
{

    friend class CAudioAgent;
    friend class CAudioTransAgent;


public:
    CAudioAgentBaseOutputPin();
    virtual ~CAudioAgentBaseOutputPin() {};
    
    //  if not connect to any pin, return virtual allocator;
    HRESULT                     GetAllocator(IMemAllocator** ppAllocator);

    //  if not connect to any pin, set to virtual allocator;
    HRESULT                     SetAllocator(IMemAllocator*  pAllocator);

protected:

private:

    IMemAllocator*              m_pVirtualAllocator;        // simulate allocator in peer filter if not connected 
                                                            // normal input pin is recode in m_InputPinArr
};


/** 

  class CAudioAgentOutputPin : public CBaseOutputPin

*/


class CAudioAgentOutputPin : public CAudioAgentBaseOutputPin
{

    friend class CAudioAgent;
    friend class CAudioTransAgent;


public:
    CAudioAgentOutputPin();
    virtual ~CAudioAgentOutputPin();
    
    //  setwrite pointer to allocator and if need, send RPC call
    virtual HRESULT             SetWritePointer(BYTE* pReadPointer);
    HRESULT                     EndOfStream(unsigned int eventID);

    HRESULT                     Deliver(BYTE* ppWritePointer, long  WriteBufferSize);

    HRESULT                     DeliverPrivateInfo(BYTE* pInfo, int length);
    HRESULT                     DeliverFlush(void);

protected:
    HRESULT                     GetVirtualInputPin(IMemInputPin** ppInPin);

    HRESULT                     RPCDeliverPrivateInfo(BYTE* pInfo, int length);
    HRESULT                     RPCEndOfStream(void);
    HRESULT                     RPCDeliver(BYTE* ppWritePointer, long  WriteBufferSize);
    HRESULT                     RPCDeliverFlush(void);
    HRESULT                     RPCSetWritePointer(BYTE* pWritePointer);

protected:

private:

    IMemInputPin*               m_pVirtualInputPin;         // need to get readhandle of allocator
                                                            // normal input pin is recode in m_InputPinArr
};



class CAudioAgentInputPin;
class CAudioAgentOutputPin;

//CAudioAgentBaseOutputPin
class CMultiAudioAgentOutPin : public CMultiOutputPin<CAudioAgentOutputPin> 
{
public:

    CMultiAudioAgentOutPin() 
        : CMultiOutputPin<CAudioAgentOutputPin>() {};

    virtual ~CMultiAudioAgentOutPin() {};
    // set allocator to inner pin
    HRESULT                     SetAllocator(IMemAllocator*  pAllocator);

protected:
    
};

//CBaseInputPin
typedef CMultiInputPin<CAudioAgentInputPin>     CMultiAudioAgentInPin;

#endif




#ifndef __C_READ_POINTER_HANDLE_H__
#define __C_READ_POINTER_HANDLE_H__

#include "IStream.h"
#include "CObject.h"
#include <StreamClass/CMemAllocator.h>

class CReadPointerHandle: public IReadPointerHandle, public CObject{
friend class                    CMemRingAllocator;
public:
    HRESULT                     GetReadPointer(
                                    BYTE**                  ppReadPointer,
                                    long*                   pReadSize
                                );
    HRESULT                     SetReadPointer(
                                    BYTE*                   pReadPointer
                                );
    HRESULT                     GetRingBuffer(
                                    BYTE**                  ppRingBuffer,
                                    long*                   pRingBufferSize
                                );
    HRESULT                     Flush(void);
    HRESULT                     Release(void);
    unsigned long               GetReadPtrIndex() { return   m_readPtrIndex; }
protected:
                                CReadPointerHandle(
						   CMemRingAllocator*      pRingAllocator,
						   long                    readPtrIndex
                                );
                                ~CReadPointerHandle(void);
    void                        Active(void);
    void                        InActive(void);
    bool                        IsActive(void);

    void                        SetLastOPRead(void);
    void                        SetLastOPWrite(void);
/*
    bool                        GetReadWriteSize(
                                    long&                   ReadSize,
                                    long&                   WriteSize
                                );
*/

    long                        GetWriteSize(void);
    long                        GetReadSize(void);

    bool                        ValidateWritePointer(
                                    BYTE*                   pWrtiePointerNew
                                );
    bool                        ValidateReadPointer(
                                    BYTE*                   pReadPointer
                                );

#ifdef RDDEBUG
    bool                        IsLastOPRead(void);
    bool                        IsLastOPWrite(void);
#endif
protected:
    CMemRingAllocator*          m_pAllocator;
    unsigned char               m_Flag;
    long                        m_readPtrIndex; // for multiple read fan out

};

#endif

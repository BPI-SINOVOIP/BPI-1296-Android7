#ifndef __C_MEM_ALLOCATOR_H__
#define __C_MEM_ALLOCATOR_H__


#include <OSAL.h>

#include <IStream.h>
#include <Util.h>
#include "CObject.h"
//#include <IPC/rpc_common.h>
#include <rpc_common.h>

#ifdef USE_IOCACHE
#include <io/dvd_reader.h>
#include <io/IOCache_buffer.h>
#endif /* USE_IOCACHE */

#define MEM_LIST_ALLOCATOR_SIZE (32 * 1024) // 32k
#define IMEMALLOCATOR_TO_CMEMRINGALLOCATOR(pIAllocator)          \
     (dynamic_cast<CMemRingAllocator*>(pIAllocator))
#define IMEMALLOCATOR_TO_CMEMLISTALLOCATOR(pIAllocator)          \
     (dynamic_cast<CMemListAllocator*>(pIAllocator))

#define IS_ALLOCATOR_LIST(pAllocator)                           \
({                                                              \
    ALLOCATOR_STYLE             style;                          \
    (pAllocator)->GetStyle(&style);                             \
    (style== STYLE_BUFFERLIST);                                 \
})

#define IS_ALLOCATOR_RING(pAllocator)                           \
({                                                              \
    ALLOCATOR_STYLE             style;                          \
    (pAllocator)->GetStyle(&style);                             \
    (style== STYLE_RING);                                       \
})

typedef enum RING_BUFFER_STYLE {
  STYLE_IPC,
  STYLE_LOCAL
} RING_BUFFER_STYLE;

class   CMediaSample;
class   CReadPointerHandle;

typedef ulCircularQueue<IMediaSample*>          IMediaQueue;
typedef Array_T<IMediaSample*>                  IMediaArray;

class CMemListAllocator: public IMemListAllocator, public CObject{
friend class                    CMediaSample;
public:
	                        CMemListAllocator(
                                    bool                    bCritSectSample= false
                                );
                                ~CMemListAllocator(void);

    // IMemAllocator interface
    HRESULT                     Commit(void);
    HRESULT                     Decommit(
                                    bool                    bFreeBuffer
                                );
    HRESULT                     IsCommit(void);
    HRESULT                     GetStyle(
                                    ALLOCATOR_STYLE*        pAllocatorStyle
                                );

    // IMemListAllocator interface
    HRESULT                     GetProperties(
                                    ALLOCATOR_PROPERTIES*   pProps
                                );
    HRESULT                     SetProperties(
                                    ALLOCATOR_PROPERTIES*   pRequest,
				    ALLOCATOR_PROPERTIES*   pActual
                                );
    HRESULT                     GetBuffer(
                                    IMediaSample**          ppBuffer,
                                    ALLOCATOR_WAIT          waitFlag
				);
    HRESULT                     ReleaseBuffer(
                                    IMediaSample*           pBuffer
                               );

#ifdef USE_IOCACHE
	//IOCache interface
    HRESULT                     IsReader(void);
    HRESULT                     GetReader(
                                    dvd_reader_t**   pDvdReader
                                );
    HRESULT                     SetReader(
                                    dvd_reader_t*   pDvdReader
                                );
    HRESULT                     ClearReader(void);
    HRESULT			ReleaseIOCache(void);
    dvd_reader_t*			m_pDvdReader;
    IOCACHE_BUFFER_POOL * m_pFreeBufferPool;
#endif /* USE_IOCACHE */

protected:
    void                        ClearBuffer(void);
protected:
#ifdef USE_IOCACHE
    HRESULT			GetBufferFromIOCache(
					IMediaSample**               ppMSample
					);
    HRESULT			ReleaseBufferToIOCache(
					IMediaSample*               pMSample
					);
#endif /* USE_IOCACHE */
    ALLOCATOR_PROPERTIES        m_Properties;
    BYTE                        m_flag;
    IMediaQueue*                m_pCleanMediaQueue;
    // this Media Array is mainly for garbage colllection
    IMediaArray                 m_MediaArray;
    CSemaphore                  m_Semaphore;
};

// structure to store reading side information
typedef struct{
  CReadPointerHandle*         pReadPointer;
  IMemInputPin*               pInputPin;
  long                        readPtrIndex; // the index of read pointer array
} RPointerPin_T;

typedef ulDArray<RPointerPin_T*>                ReadPointerArray;

class CMemRingAllocator: public IMemRingAllocator, public CObject{
friend class                    CReadPointerHandle;
public:
                                CMemRingAllocator(RING_BUFFER_STYLE style = STYLE_LOCAL);
                                ~CMemRingAllocator(void);
    // IMemAllocator interface
    HRESULT                     Commit(void);
    HRESULT                     Decommit(
                                    bool                    bFreeBuffer= false
                                );
    HRESULT                     IsCommit(void);
    HRESULT                     GetStyle(
                                    ALLOCATOR_STYLE*        pAllocatorStyle
                                );

    // IMemRingAllocator interface
    HRESULT                     GetBuffer(
                                    BYTE**                  ppBuffer,
                                    long*                   pSize
                                );
    HRESULT                     SetBuffer(
                                    BYTE*                   pBuffer,
                                    long                    Size
                                );
    HRESULT                     GetWritePointer(
                                    BYTE**                  ppWritePointer,
                                    long*                   pWriteBufferSize
                                );
    HRESULT                     SetWritePointer(
                                    BYTE*                   pWritePointer
                                );
    HRESULT                     RequestWriteSpace(
                                    BYTE**                  ppWritePointer,
                                    long*                   pWriteBufferSize,
                                    long                    RequestSize,
                                    long                    MilliTimeOut
                                );
    HRESULT                     GetReadPointerHandle(
                                    IReadPointerHandle**    ppReadPointer,
                                    IMemInputPin* pInputPin
                                );
    HRESULT                     ReleaseReadPointerHandle(
                                    IReadPointerHandle*     pReadPointer
                                );

	HRESULT						RemoveReadPointerHandle(IMemInputPin* pInputPin);

    HRESULT                     GetBufferHeader(RINGBUFFER_HEADER **virtualAddress, unsigned long *physicalAddress);

    // get the non-cached base address of ring buffer
    HRESULT                     GetBuffer_Cached(BYTE** ppBuffer,
						    long*  pSize);

    // the following Get/Set Read/Write pointer function will access the non-cached read/write pointer location and do necessary
    // byte swaping, it also will map to virtual address to physical address when Setting, and revert the physical address back
    // to virtual address when Getting. In case of local accessing, the following function will not execute the operations mentioned
    // above.
    BYTE*                       GetReadPtr(long readPtrIndex);
    void                        SetReadPtr(long readPtrIndex, BYTE* readPtr);
    BYTE*                       GetWritePtr();
    void                        SetWritePtr(BYTE* writePtr);
    HRESULT                     GetReadPointer(long                    readPtrIndex,
					       BYTE**                  ppReadPointer,
					       long*                   pReadBufferSize);
    HRESULT                     SetReadPointer(long                    readPtrIndex,
					       BYTE*                   pReadPointer);

    // The following Get/Set function will get/set the read/write pointers by physical address. It will do necessary byte swapping 
    // for endianess. You get the physical address of the place you can write the data.
    unsigned long               GetPhysicalWriteAddress();
    void                        SetPhysicalWriteAddress(unsigned long address);
    unsigned long               GetPhysicalReadAddress(long readPtrIndex);
    void                        SetPhysicalReadAddress(long readPtrIndex, unsigned long address);
    unsigned long               GetPhysicalAddress(unsigned long nonCachedAddress);



    void                        Flush(long readPtrIndex);
protected:
    bool                        ValidateBufferRange(
                                    BYTE*                   pWritePointer
                                );
    long                        GetWriteSize(void);

#ifdef RDDEBUG
    void                        Dump(void);
#endif

protected:

    ReadPointerArray            m_RPointerPinArr;
    unsigned char               m_Flag;

    bool                        m_bNeedSizeNotifier;
    long                        m_RequestSize;
    CSemaphore                  m_SemWriteSize;

    CCritSec                    m_Lock;


    RING_BUFFER_STYLE           m_style;   // denote this is IPC ring or Local ring buffer


    // header addresses
    BYTE*                                           m_pBufferHeaderAddr;    // for free (free must use cache-able address)
    RINGBUFFER_HEADER*          m_pBufferHeader; // point to ring buffer property structure
    unsigned long               m_bufferHeaderPhyAddr;
    long                        m_headerPhyAddrDiff;  // value = PhysicalAddress - VirtualAddress

    // ring buffer addresses and size
    long                        m_bufferSize;
    BYTE*                       m_pBufferLower;
    BYTE*                       m_pBufferUpper;
    BYTE*                       m_pBufferNonCachedLower;
    unsigned long               m_bufferPhyAddr;
};
#endif

#ifndef __C_MEDIA_SAMPLE_H__
#define __C_MEDIA_SAMPLE_H__

#include <OSAL.h>
#include <IStream.h>
#include "CObject.h"
#include "CMemAllocator.h"

#ifdef USE_IOCACHE
#include <io/IOCache.h>
#include <io/IOCache_buffer.h>
#include "IOCache_MS.h"
#endif /* USE_IOCACHE */

class CMediaSample: public IMediaSample, public CObject{
// friend class                    CMemListAllocator;
// friend class                    CMemQueueAllocator;
// friend CMediaSample*            CMemListAllocator::GetCMediaSample(void);
friend void                     CMemListAllocator::ClearBuffer(void);
friend HRESULT                  CMemListAllocator::Commit(void);
friend HRESULT                  CMemListAllocator::GetBuffer(
                                    IMediaSample**              ppSample,
                                    ALLOCATOR_WAIT              waitFlag
                                );
#ifdef USE_IOCACHE
friend HRESULT              	CMemListAllocator::GetReader(
                                    dvd_reader_t**   pDvdReader
                                );
friend HRESULT			CMemListAllocator::GetBufferFromIOCache(
					IMediaSample**               ppMSample
					);

friend void			deleteMediaSample(
					void*	pMedisSample
				);
#endif /* USE_IOCACHE */

public:
    // query
    HRESULT                     GetActualSize(
                                    ULONG*                  pActualSize
                                );
    HRESULT                     GetAcutaulPointer(
                                    BYTE**                  pActualPointer
                                );
    HRESULT                     GetBeginSize(
                                    ULONG*                  pBeginSize
                                );
    HRESULT                     GetBeginPointer(
                                    BYTE**                  pBeginPointer
                                );
    HRESULT                     GetNonCachedBeginPointer(    
				    BYTE**                  pBeginPointer);
 
    ULONG                       GetPhysicalAddress(
                                    ULONG                   cachedMemAddr
                                );
    ULONG                       GetNonCachedAddress(
                                    ULONG                   cachedMemAddr
                                );

    // set
    HRESULT                     SetActualPointer(
                                    BYTE*                   pBuffer,
				    ULONG                   size
                                );

#ifdef USE_IOCACHE
    HRESULT                     SetIOCache(
                                    dvd_file_t*                   dvd_file,
				    __off64_t                   offset_2k,
					int                   block_count 
					);
    HRESULT                     GetIOCache(
                                    IOCACHE**                   pCache
                                );
#endif /* USE_IOCACHE */

    // memory allocation
    HRESULT                     Release(void);
    HRESULT                     AddRef(void);

#ifdef USE_IOCACHE
    IOCACHE_BUFFER		*m_pIOCacheBuffer;
#endif /* USE_IOCACHE */
    char                        m_Ref;

protected:
                                CMediaSample(
                                    IMemListAllocator*      pAllocator
                                );
                                ~CMediaSample();
/*
    HRESULT                     SetBeginPointer(
                                    BYTE*                   pBuffer
                                );
*/
protected:
    SAMPLE_INFO                 m_Buf;
    IMemListAllocator*          m_pAllocator;
    CCritSec                    m_Lock;
#ifdef USE_IOCACHE
    IOCACHE			m_cache;
#endif /* USE_IOCACHE */
};

#endif

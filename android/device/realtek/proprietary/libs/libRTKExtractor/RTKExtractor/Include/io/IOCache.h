#ifndef __C_IOCACHE_H__
#define __C_IOCACHE_H__

#include <io/IOCache_buffer.h>
#include <Utility/hash.h>
#include <OSAL.h>

#ifdef __cplusplus
extern "C" {
#endif

//MAX_DEVICE /dev/hd & /dev/dvd
#define MAX_IOCACHE_DEVICE 2

//MAX_HANDLE per device
#define MAX_IOCACHE_DEVICE_HANDLE 2
#define IOCACHE_RAED_HANDLE 1
#define IOCACHE_WRITE_HANDLE 0
#define MAX_IOCACHE_BUFFER_NUMBER 1024
#define MIN_WRITE_ONCE_BUFFER 4

#define IOCACHE_READPREFETCH_DEFAULT 10
#define IOCACHE_SIZE_K 64 // 32 or 64 K
#define IOCACHE_SIZE ( IOCACHE_SIZE_K * 1024)
#define IOCACHE_SHIFT_2K 11
#define IOCACHE_SIZE_2K (IOCACHE_SIZE >> IOCACHE_SHIFT_2K)

#define Cache_to_Device(x) (((IOCACHE_HANDLE*)((x)->cacheHandle))->pDevice)

typedef struct _tagIOCACHE_DEVICE{
  //Normally one device will handle two requests: one read, one write
  //There is no limitation for the maximum number of handles. But the device performance 
  //may becomes worse since more seek is required.
  struct _tagIOCACHE_HANDLE *handle[MAX_IOCACHE_DEVICE_HANDLE];

  IOCacheHash* pHashTable;

  //0: Hard disc/UDF2.0; 1: Optical/UDF1.1
  char deviceType;

  //Read/write task realted variables
  char threadRunning;

  osal_thread_t threadHandle;
  osal_sem_t taskPending;
  osal_sem_t taskDone;

  //Cache buffer pool to save the free cache buffers.
  //For the normal case, the newly free buffers join the buffer pool from the tail
  //When new cache buffers from the newly cache handle, they are put from the head
  IOCACHE_BUFFER_POOL *pFreeBufferPool;

  //For the first stage, assume only one read and only one write. 
  //Therefore, no need to use mutex protection when add/delete read/write request

  //Normal write queue. 
  int writeBuffer_wIndex;
  int writeBuffer_rIndex;
  IOCACHE_BUFFER *pWriteBuffer[MAX_IOCACHE_BUFFER_NUMBER];

  //Special write queue. For dubbing only when the buffer comes from the other device cache
  int extraFileHandle;
  int extraWriteBuffer_wIndex;
  int extraWriteBuffer_rIndex;
  char *pExtraWriteBuffer[MAX_IOCACHE_BUFFER_NUMBER];

  //Normal read queue
  int readBuffer_wIndex;
  int readBuffer_rIndex;
  IOCACHE_BUFFER *pReadBuffer[MAX_IOCACHE_BUFFER_NUMBER];

#ifdef IOCACHE_DEBUG
  //Statistic/Debug Info
  unsigned int readBuffer;
  unsigned int writeBuffer;
  __off64_t readOffset;
  __off64_t prefetchOffset;
  __off64_t writeOffset;
#endif
}IOCACHE_DEVICE;

typedef struct _tagIOCACHE_HANDLE{
  IOCACHE_DEVICE *pDevice;
  int readPrefetch;
  int prefetch_hit;
  int bufferNumber;
}IOCACHE_HANDLE;

typedef struct {
  int cacheHandle;
  int fileHandle;
  __off64_t offset_2k;
  void *dvd_input_fd;
}IOCACHE;

int IOCacheCreate(char *device, int readCache, int bufferNumber, int device_type);
int IOCacheDelete(int pCacheHandle, int readCache, int bufferNumber);
int IOCacheFlush(int pCacheHandle);
int IOCacheRead(IOCACHE *pCache, void **ppBuffer);
int IOCacheWrite(IOCACHE *pCache, void *pBuffer, int *pSeqNumber);
int IOCacheReleaseSpace(IOCACHE *pCache);
int IOCacheRequestSpace(IOCACHE *pCache, void **ppBuffer);
int IOCacheReadPrefetch(IOCACHE *pCache, int size);
int IOCacheQueryWriteComplete(int pCacheHandle, int seqNum);
void IOCacheProcessTask(void *pThis);

#ifdef __cplusplus
}
#endif

#endif

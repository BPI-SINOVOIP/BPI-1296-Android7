#ifndef __C_IOCACHE_BUFFER_H__
#define __C_IOCACHE_BUFFER_H__

#include <OSAL.h>

#ifdef __cplusplus
extern "C" {
#endif
//for IOCACHE_BUFFER flags
#define IOCACHE_BUFFER_NONE	(0)
#define IOCACHE_BUFFER_REQUEST	(1<<0)
#define IOCACHE_BUFFER_EMPTY	(1<<1)
#define IOCACHE_BUFFER_READ	(1<<2)
#define IOCACHE_BUFFER_WRITE	(1<<3)
#define IOCACHE_BUFFER_RELEASE	(1<<4)
#define CHECK_IOCACHE_BUFFER_REQUEST(x)	(((x)&IOCACHE_BUFFER_REQUEST)==IOCACHE_BUFFER_REQUEST)
#define CHECK_IOCACHE_BUFFER_EMPTY(x)		(((x)&IOCACHE_BUFFER_EMPTY)==IOCACHE_BUFFER_EMPTY)
#define CHECK_IOCACHE_BUFFER_READ(x)		(((x)&IOCACHE_BUFFER_READ)==IOCACHE_BUFFER_READ)
#define CHECK_IOCACHE_BUFFER_WRITE(x)		(((x)&IOCACHE_BUFFER_WRITE)==IOCACHE_BUFFER_WRITE)
#define CHECK_IOCACHE_BUFFER_RELEASE(x)	(((x)&IOCACHE_BUFFER_RELEASE)==IOCACHE_BUFFER_RELEASE)
	
typedef struct _tagIOCACHE_BUFFER{
  int fileHandle;
  __off64_t offset_2k;               //For optical, high 32 bit save the file start LBA and
                                  //low 32 bit save the file offset
  BYTE *pStart;
  BYTE *pReleaseAddr;
  BYTE *pNonCachedBuffer;
  unsigned long bufferPhyAddr;
  char read;
  char write;
  char refCount;
  void *dvd_input_fd;
  int return_size; // 2k
  //unsigned char flags;
  void *pInstance;	//for CIOCacheMediaSample
  struct _tagIOCACHE_BUFFER *previous;
  struct _tagIOCACHE_BUFFER *next;
}IOCACHE_BUFFER;

typedef struct {
  int bufferNumber;
  osal_mutex_t poolMutex;
  IOCACHE_BUFFER *pHead;
  IOCACHE_BUFFER *pTail;
} IOCACHE_BUFFER_POOL;

IOCACHE_BUFFER_POOL *IOCache_Buffer_Pool_Create(void);
int IOCache_Buffer_Pool_Delete(IOCACHE_BUFFER_POOL * pPool);
int IOCache_Buffer_Pool_Enqueue(IOCACHE_BUFFER_POOL * pPool, IOCACHE_BUFFER *pBuf, int tail);
int IOCache_Buffer_Pool_Delete_OneBuf(IOCACHE_BUFFER_POOL * pPool, IOCACHE_BUFFER * pBuf);
IOCACHE_BUFFER *IOCache_Buffer_Pool_Dequeue(IOCACHE_BUFFER_POOL * pPool);
int IOCache_Buffer_Add_Ref(IOCACHE_BUFFER_POOL * pPool, IOCACHE_BUFFER * pBuf);
int IOCache_Buffer_Get_Ref(IOCACHE_BUFFER_POOL * pPool, IOCACHE_BUFFER * pBuf);
IOCACHE_BUFFER *IOCache_Buffer_Create(int size);
int IOCache_Buffer_Delete(IOCACHE_BUFFER *pBuf);


#ifdef __cplusplus
};
#endif

#endif

#ifndef __DUMMY_RING_BUFFER_H__
#define __DUMMY_RING_BUFFER_H__

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DUMMY_RING_BUFFER_SIZE		(1024*1024*1)//		4*1024*1024
//#define USE_MMAP_RINGFILE 1
#define BUFFER_OFFSET 8192

// structure
typedef struct DUMMY_RINGBUFFER
{
	char 			*buffer;
	char 			*RBufPtr;
	int				ringSize;
	long long 		readptr;
	long long 		writeptr;
	int			isEOS;
	pthread_mutex_t bufferMutex;   // protect this structure
}DummyRingBuffer_t;

// functions
/* initialize a DummyRingBuffer structure */
void DummyRingBuffer_init(DummyRingBuffer_t *ring);

void DummyRingBuffer_clean(DummyRingBuffer_t *ring);

int DummyRingBuffer_read(DummyRingBuffer_t *ring, char *buf, int size);

int DummyRingBuffer_write(DummyRingBuffer_t *ring, const unsigned char *inbuf, int inbufSize);

int DummyRingBuffer_getAvail(DummyRingBuffer_t *ring);


#ifdef __cplusplus
}
#endif

#endif /*__DUMMY_RING_BUFFER_H__*/

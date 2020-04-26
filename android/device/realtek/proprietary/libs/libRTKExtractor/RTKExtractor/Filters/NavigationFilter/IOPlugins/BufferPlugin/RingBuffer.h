#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RING_BUFFER_SIZE				4*1024*1024

// structure
typedef struct RINGBUFFER
{
	char 			*buffer;
	int				ringSize;
	long long 		readptr;
	long long 		writeptr;
	pthread_mutex_t bufferMutex;   // protect this structure

}RingBuffer_t;

// functions
/* initialize a RingBuffer structure */
void RingBuffer_init(RingBuffer_t *ring);

void RingBuffer_clean(RingBuffer_t *ring);

int RingBuffer_read(RingBuffer_t *ring, char *buf, int size);

int RingBuffer_write(RingBuffer_t *ring, const unsigned char *inbuf, int inbufSize);


#ifdef __cplusplus
}
#endif

#endif /*__RING_BUFFER_H__*/

#ifndef __SERVER_THREAD_READER_H
#define __SERVER_THREAD_READER_H

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VLCUDP_DEFAULT_RINGSIZE          4*1024*1024 
// how much data should we buffer initially?
#define VLCUDP_INITIAL_BUFSIZE           0 //512*1024

typedef enum THREAD_OPERATION_ {
    RING_READ,
    RING_QUIT,
} THREAD_OPERATION;
    
struct server_ringBuffer 
{
    int netfd;
    char * ringBuffer;
    int    ringSize;
    int    started;
    int    wrapped;

    volatile char * readptr;
    volatile char * writeptr;
    volatile char * validptr;

    volatile THREAD_OPERATION  op;
    pthread_mutex_t bufferMutex;   // protect this structure
    int reader_wait;
    pthread_mutex_t pending_avail; // waiting?

    /* functions for protocol specific acess */
    int (*read_func)(struct server_ringBuffer *);
    int (*close_func)(struct server_ringBuffer*);
    void * etc;
};
    

/* read out of the ringbuf. return number of bytes read */
extern int server_ring_read(struct server_ringBuffer * ring,
                            char * buf,
                            int writebufSize);
    
/* how many bytes are available for write at writeptr */
extern int server_ring_write_avail(struct server_ringBuffer * ring,
                                   char ** writeptr);

        /* move the write ptr by specified amount */
extern int server_ring_update_write(struct server_ringBuffer * ring,
                                    int moveSize);
    
extern void * server_readthread_start(void *arg);

extern int server_ring_waitavail(struct server_ringBuffer * ring, int ms);    

extern void server_ring_reset0(struct server_ringBuffer* ring);


#ifdef __cplusplus
}
#endif

#endif

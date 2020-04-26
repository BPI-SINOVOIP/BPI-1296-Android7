#ifndef __AIO_H__
#define __AIO_H__


#include "OSAL.h"
#include "aio.h"
#include "list.h"



typedef struct 
{        
    int (*read) (void* fd, unsigned char* pData, unsigned long Len);        
}AIO_OPS;


typedef struct 
{
    unsigned char       state;
    osal_thread_t       thread_id;  
    int                 sem_id;    
    AIO_OPS             ops;    
    osal_mutex_t        mutex;
    struct list_head    work;
    struct list_head    finished_list;
}AIO_CTX;


typedef struct 
{   
    int                 sem_id;
    struct list_head    list;
    unsigned char       state;     // b0 : Complete     
#define AIO_CMD_COMPLETE        0x80        // Finished
#define AIO_CMD_PROCESSING      0x08        // Runing
#define AIO_CMD_WAIT            0x01        // waiting 
#define AIO_CMD_NONE            0x00        // null command
    void*               fd;
    unsigned char*      data;      // data to read
    unsigned long       len;       // number of bytes to read
    int                 ret;       // return value
}AIO_CMD;





#define AIO_START           0x01
#define AIO_THREAD_READY    0x02
#define AIO_THREAD_EXIT     0x80

extern int aio_setup(AIO_CTX* ctx, AIO_OPS* ops);
extern int aio_start(AIO_CTX* ctx);
extern int aio_stop(AIO_CTX* ctx);
extern int aio_read(AIO_CTX* ctx, void* fd, unsigned char* pData, unsigned long Len);
extern int aio_complete(AIO_CTX* ctx, int id);
extern void aio_wait_complete(AIO_CTX* ctx, int id);
extern int aio_return(AIO_CTX* ctx, int id);



///////////// for DEBUG //////////////
//#define AIO_THREAD_DEBUG_EN
//#define AIO_DEBUG_EN

#ifdef AIO_THREAD_DEBUG_EN
    #define aio_thread_debug(fmt, args...)     printf("[AIO] thread debug, " fmt, ## args)
#else    
    #define aio_thread_debug(fmt, args...)     
#endif

#define aio_warning(fmt, args...)       printf("[AIO] warning, " fmt, ## args)
#define aio_info(fmt, args...)          printf("[AIO] info, " fmt, ## args)
#ifdef AIO_DEBUG_EN
    #define aio_debug(fmt, args...)     printf("[AIO] debug, " fmt, ## args)
#else    
    #define aio_debug(fmt, args...)     
#endif

#endif //__AIO_H__ 

#ifndef __PPLIVE_PRIV_H__
#define __PPLIVE_PRIV_H__

#include <hresult.h>
#include <netinet/in.h>

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <FlowEvent.h>

void *PPLive_Callback(PP_int32);

#define PPLIVE_MAX_RETRY     5
#define PPLIVE_BUFFER_SIZE 256*1024
#define PPLIVE_SERVER_BUFFER_SIZE 1*1024*1024


#define pplive_info(fmt, args...)    printf("[PPLive][Info]," fmt, ## args)

enum PPLive_Engine_State
{
pplive_engine_open  = 0x00,
pplive_engine_close  = 0x01,
};

enum PPLive_Return_value
{
PPLive_Error =-1,
PPLive_Retry = 0,
PPLive_Success = 1
};
enum PPLive_Thread_State
{
READ_FROM_SERVER = 0x1,
BUFFER_FULL = 0x2,
WAIT_KILL_CMD = 0x4,
SEEK_CMD	= 0x8,
};

static int retry_Cond[1]={ppbox_would_block};

int GetPPLivePlayingStatus(int  *buffering_present);

char PPLive_buf_status[4][32] = {"OVERFLOW", "UNDERFLOW", "R_W", "W_R"};
enum PPLive_Buffer_Status
{
OVERFLOW = 0, //buffer full
UNDERFLOW = 1, //buffer empty
R_W = 2,
W_R = 3
};
int PPLive_GetErrorMsg(int Result, int *retry_Cond);

struct PPLiveBuffer
{
	volatile int ThreadState;
	volatile int PrevThreadState;
	volatile long long SeekTime;//mini second 
	pthread_mutex_t PPLiveBufferMutex;	
	pthread_t thread_id;
};

PPLiveBuffer *myStatus = NULL;

class BitsReader
{
public:
    BitsReader(
        uint8_t const * buf, 
        uint32_t size)
        : buf_(buf)
        , size_(size - 1)
        , size_this_byte_(8)
        , failed_(false)
    {
        //assert(size > 0);
    }

    uint32_t read_bits(
        uint32_t len)
    {
        if (size_this_byte_ + size_ * 8 < len || len > 32) {
            failed_ = true;
        }
        if (failed_) {
            return 0;
        }
        uint32_t v = 0;
        while (len > size_this_byte_) {
            v = (v << size_this_byte_) | (((uint32_t)(*buf_)) & ((1 << size_this_byte_) - 1));
            len -= size_this_byte_;
            ++buf_;
            --size_;
            size_this_byte_ = 8;
        }
        if (len) {
            v = (v << len) | ((((uint32_t)(*buf_)) >> (size_this_byte_ - len)) & ((1 << len) - 1));
            size_this_byte_ -= len;
        }
        return v;
    }

private:
    uint8_t const * buf_;
    uint32_t size_;
    uint32_t size_this_byte_;
    bool failed_;
};


class BitsWriter
{
public:
    BitsWriter(
        uint8_t * buf, 
      	 uint32_t size)
        : buf_(buf)
        , size_(size - 1)
        , size_this_byte_(8)
        , failed_(false)
    {
       // assert(size > 0);
        memset(buf, 0, size);
    }

    uint32_t write_bits(
    uint32_t v, 
       uint32_t len)
    {
        if (size_this_byte_ + size_ * 8 < len || len > 32) {
            failed_ = true;
        }
        if (failed_) {
            return 0;
        }
        while (len > size_this_byte_) {
            *buf_ = (uint8_t )(((uint32_t)(*buf_)) | (((v >> (len - size_this_byte_)) & ((1 << size_this_byte_) - 1))));
            len -= size_this_byte_;
            ++buf_;
            --size_;
            size_this_byte_ = 8;
        }
        if (len) {
            *buf_ = (uint8_t )(((uint32_t)(*buf_)) | ((v & ((1 << len) - 1)) << (size_this_byte_ - len)));
            size_this_byte_ -= len;
        }
        return v;
    }

private:
    uint8_t * buf_;
    uint32_t size_;
    uint32_t size_this_byte_;
    bool failed_;
};


#endif //__PPLIVE_PRIV_H__

#ifndef __NRDCOMMON__
#define __NRDCOMMON__

struct IBufferManager;
typedef struct _Buffer
{
	int bufferDataType_; 
	unsigned char* byteBuffer_;     
	uint32_t       size_;           
	void*          private_;        
} Buffer;

int Call_BufferManagerNative_fetchData(struct IBufferManager* pBufManager, void* pBuf);

typedef struct _tagIOPlugin_NRD
{
       //buffer
       uint8_t *buf;
       uint64_t bufsize;
       uint64_t bufpos;

       //buffer mmsh only
       uint8_t *s_buffer;
       uint32_t s_buffer_size;
       uint32_t s_data_len;
       uint32_t s_buffer_pos;

       uint8_t *data_pool;
       uint8_t *swap_pool;
       int64_t p_read;
       int64_t p_write;

       //url
       char *strprotocol;
       char *strhost;
       char *strport;
       char *strpath;
}IOPlugin_NRD;
#endif

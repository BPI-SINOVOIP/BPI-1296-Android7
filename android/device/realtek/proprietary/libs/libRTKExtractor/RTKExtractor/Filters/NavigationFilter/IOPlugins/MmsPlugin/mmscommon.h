#ifndef __MMSCOMMON__
#define __MMSCOMMON__

#include "Util.h"
#include "OSAL.h"
#include <pthread.h>

#define USER_STOP	41

typedef enum MMS_COMMAND_
{
	MMS_COMMAND_NONE		= 0,
	MMS_COMMAND_READ		= 1,
	MMS_COMMAND_SEEK		= 2,
	MMS_COMMAND_KILLCLEAN		= 3,
	MMS_COMMAND_KILL		= 4,
	MMS_COMMAND_STANDBY		= 5,
	MMS_COMMAND_RECONNECT		= 6,
} MMS_COMMAND;

typedef enum MMS_PEND_
{
	MMS_THREAD_NOWAIT		= 0,
	MMS_THREAD_BLOCK		= 1,
	MMS_THREAD_OVER			= 2,
} MMS_PEND;

typedef struct _tagIOPlugin_Mms
{
       int s;
       int protocol;
       int recv_timeout;
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

       uint8_t *header_buf;
       uint32_t header_buf_size;

       //url
       char *strprotocol;
       char *strhost;
       char *strport;
       char *strpath;

       //mmst only
       uint32_t openFileId;
       uint32_t fileAttributes;
       double fileDuration;
       uint32_t fileBlocks;
       uint32_t filePacketSize;
       uint64_t filePacketCount;
       uint32_t fileBitRate;
       uint32_t fileHeaderSize;

       uint32_t curPacketLocationId;
       uint64_t curOffsetOfFile;

       uint32_t lastCommand;

       //mmsh only
       int streaming_type;
       int packet_count;

       int n_a_streams;
       int n_v_streams;
       int a_stream_ids[20];
       int v_stream_ids[20];
       int audio_id;
       int video_id;

       int packet_length;
       int status;

       int isHeaderRecived;
       int isDataRecived;
       int isStreamEndRecived;
       int isDropHeader;
       int hasNewHeader;

       pthread_mutex_t main_lock;
       pthread_t thread_id;

       MMS_COMMAND operation;
       MMS_PEND pend_reason;

       struct rtk_http_header_t *p_http_header;
       int retVal;
       bool bRestart;

       unsigned char mms_client_guid[58];
}IOPlugin_mms;

enum {    
	UNKNOWN_PROTOCOL = 0,
	MMST,
	MMSH,
};

enum {
	STREAMING_HANDSHAKING,
	STREAMING_DOWNLOADING,
	STREAMING_FINISHED,
	STREAMING_REDIRECTED,
};
#endif

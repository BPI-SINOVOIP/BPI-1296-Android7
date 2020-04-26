#ifndef __HTTP_BUFFER_THREAD_H__
#define __HTTP_BUFFER_THREAD_H__

#include <ne_session.h>
#include <ne_uri.h>
#include <ne_basic.h>
#include <ne_utils.h>
#include <ne_redirect.h>
#include <ne_auth.h>
#include <pthread.h>
#include <Filters/NavigationFilter/NavPlugins.h>

#ifdef __cplusplus
extern "C" {
#endif

#define                   HTTP_FORCE_VIDEOMEM_FLAG  0x00000004
#define                   HTTP_AUTO_RECONNECT       0x00000008
#define                   HTTP_NO_STATS             0x00000010
#define                   HTTP_FORCE_RANGE          0x00000020
#define                   HTTP_LIMITED_SEEK         0x00000040 //upnp limited seek
#define                   HTTP_CLEAR_DLSTATS        0x00000080
#define                   HTTP_DTCPIP_FLAG          0x00000100

#define                   HTTP_FLAG_MASK            0x000fffff

#define HTTP_RAED_BUF_SIZE     16384

#ifdef ENABLE_DTCP_IP
#define DTCP_PCP_HEADER_SIZE   14
#define DTCP_DECRYPT_MIN_SIZE  8192
#define DTCP_STAT
//#define DTCP_DEBUG

#endif /* ENABLE_DTCP_IP */

typedef enum HTTP_BUFFER_STATE_ {
    BUFFER_STATUS_REQUEST        = 0,
    BUFFER_STATUS_READING        = 1,
    BUFFER_STATUS_EOF            = 2,
    BUFFER_STATUS_ERROR          = 3
}HTTP_BUFFER_STATE;

typedef enum HTTP_BUFFER_COMMAND_ {
    BUFFER_NONE                  = 0,
    BUFFER_READ                  = 1,
    BUFFER_SEEK                  = 2,
    BUFFER_KILLCLEAN             = 3, // kill thread, ask thread to cleanup
    BUFFER_KILL                  = 4, // kill pthread, do not cleanup
    BUFFER_STANDBY               = 5, // tell thread to halt
    BUFFER_RECONNECT             = 6, // re-attempt connection
}HTTP_BUFFER_COMMAND;

typedef enum HTTP_BUFFER_PEND_ {
    BUFFER_THREAD_NOWAIT         = 0,
    BUFFER_THREAD_FULL           = 1,
    BUFFER_THREAD_EOF            = 2,
    BUFFER_THREAD_ERR            = 3,
    BUFFER_THREAD_HALT           = 4, // halt by request
    BUFFER_THREAD_SEEK           = 5, // busy seeking
    BUFFER_THREAD_ERR_REDIR	 = 6, // redirect to different URL
}HTTP_BUFFER_PEND;

#define MAX_HTTP_MUTEX        2
#define HTTP_BUFFER_NOTIFY    0
#define HTTP_MAIN_NOTIFY      1    
    
struct httpBuffer 
{    
        // structures used+passed around by thread, these vars do
        // not require lock protection
    ne_session       *sess;
    ne_request       *req;
    ne_uri           uri;
    off_t            contentlength;
    //unsigned char             seekable;
    unsigned char             timeSeekable;
    unsigned char             posSeekable;
    int              duration;
	long long        bytesToSkip;

        // options for connection
    char * userAgent;
    char * extraHttpHeaders;
    
    pthread_mutex_t bufferMutex;

    int bufsize;
    char *buffer;
    volatile char* readptr;
    volatile char* writeptr;
    volatile char* validptr;
    int loop0;
    long long startOffset;
    long long startTime;

        // for any command directed at buffer thread
    HTTP_BUFFER_COMMAND operation;
    long long argument;

        // buffer thread may be pending event
    HTTP_BUFFER_PEND pend_reason;

        // mutex used between caller and callee, used as notification
    pthread_mutex_t main_lock;    // used to lock main thread
    pthread_mutex_t buffer_lock;  // used to lock buffer thread

	// the following are unprotected by lock
    pthread_t thread_id;
    void (*buf_free)(void *); // free function;
    void * free_arg;          // arg passed into free function.
    unsigned long flags;      // extra options

    char* pContentType; // String for content type
    unsigned int totalDownloadBytes;
    unsigned int elapsedTime;
    int priorityBeforeRead;
    int priorityWhenRead;

    fpGetAuthInfo getAuthInfo; // The callback function for getting authentication information.
    char * redirectUrl; // redirect to a different URL?

#ifdef ENABLE_DTCP_IP
	char * protocolInfo;
	void * SnkInstance;
	bool bDecryptStarted;
	bool bNeedAKE;
	unsigned int pcpSize;         // pcp packet size
	unsigned int padSize;         // pcp padding size
	unsigned int pcpDecSize;         // pcp packet decoded size
	unsigned int bytesInTmp;      // bytes in pcp packet temp, must < 16

	off_t        totalDecSize;
	off_t        dtcp_contentlength;
	
	unsigned char *pDtcpDecBuf;
	int          dtcpBufUsed;

	unsigned char *pReadBuf;
	unsigned char *pUncachedReadBuf;
	unsigned long phyAddr;

#ifdef DTCP_STAT
	int          decTime;
	int          decCount;
	int          readTime;
	int          readSize;
	int          readCount;
	int          pureDecTime;
	int          totalTime;
#endif /* DTCP_STAT */

#ifdef DTCP_DEBUG
	FILE *pf;
	FILE *pf_enc;
	off_t saveSize;
#endif /* DTCP_DEBUG */
#endif /* ENABLE_DTCP_IP */
    int readTimeout;   // read timeout for HTTP in second
	int connectTimeout; // connection timeout for HTTP in second
};

extern void* http_buffer_init (void *arg);    
    
#ifdef __cplusplus
}
#endif

#endif

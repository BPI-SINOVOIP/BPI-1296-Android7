#ifndef  _RPC_STRUCT_H_
#define  _RPC_STRUCT_H_

#include <pthread.h>
#include <rpc/types.h>
#include <rpc/xdr.h>


#ifdef __cplusplus
extern "C" {
#endif

#define RTD_1195
#define RPC_SUPPORT_MULTI_CALLER_SEND_TID_PID
#define RPC_CONTEXT_LOOKUP

#ifdef RPC_DEBUG
 #define RPCDEBUG(fmt, args...) printf(fmt, ## args)
#else
 #define RPCDEBUG(fmt, args...)  /*empty macro */
#endif

#define REPLYID 99   // for registering the Reply_Handler

#define NONBLOCK_MODE	0x00
#define BLOCK_MODE	    0x01
#define USE_POLL_BUF	0x00
#define USE_INTR_BUF	0x02
//#define SEND_VIDEO_CPU	0x00
#define SEND_VIDEO_CPU	0x00
#define SEND_AUDIO_CPU	0x04


/*
 * The data structure for user assign the sending mode
 * the sendMode defined in rpcapi.h
 */
typedef struct CLNT_STRUCT CLNT_STRUCT;

struct CLNT_STRUCT {
    uint32_t sendMode;  // (NONBLOCK_MODE, BLOCK_MODE) | (USE_POLL_BUF, USE_INTR_BUF)
    uint32_t programID;  // The program ID defined in IDL file
    uint32_t versionID;  // The version ID defined in IDL file
#ifdef RPC_SUPPORT_MULTI_CALLER_SEND_TID_PID
    uint32_t sysTID;
#endif
#ifdef RTD_1195
    uint32_t sysPID;        // the callee's task ID
#endif
};



/*
 * The packet header of RPC
 */
typedef struct RPC_STRUCT RPC_STRUCT;

struct RPC_STRUCT {
    uint32_t programID;      // program ID defined in IDL file
    uint32_t versionID;      // version ID defined in IDL file
    uint32_t procedureID;    // function ID defined in IDL file

    uint32_t taskID;         // the caller's task ID, assign 0 if NONBLOCK_MODE
#ifdef RPC_SUPPORT_MULTI_CALLER_SEND_TID_PID
    uint32_t sysTID;
#endif
#ifdef RTD_1195
    uint32_t sysPID;         // the callee's task ID
#endif
    uint32_t parameterSize;  // packet's body size
    uint32_t context;         // return address of reply value
};

/*
 * The data structure of context
 */
typedef struct CONTEXT_STRUCT {
    void            *context; // return address of reply value
    volatile int    ack;
    pthread_cond_t  cnd;
    pthread_mutex_t mtx;
#ifdef RPC_CONTEXT_LOOKUP
    uint32_t        idx;
#endif
} CONTEXT_STRUCT;

#ifdef RPC_CONTEXT_LOOKUP
#define MAX_CONTEXT_SIZE    64
CONTEXT_STRUCT *getContextByIndex(uint32_t idx);
#endif
CONTEXT_STRUCT *getContext(void);

bool_t                 xdr_RPC_STRUCT(XDR *, RPC_STRUCT *);

CLNT_STRUCT            prepareCLNT(int opt, long programID, long versionID);

#ifdef __cplusplus
}
#endif

#endif


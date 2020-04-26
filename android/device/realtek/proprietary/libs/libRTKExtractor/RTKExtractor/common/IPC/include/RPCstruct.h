#ifndef  _RPC_STRUCT_H_
#define  _RPC_STRUCT_H_


#include <xdr/types.h>
#include <xdr/xdr.h>


#ifdef __cplusplus
extern "C" {
#endif

#define RTD_1195

#ifdef RPC_DEBUG
 #define RPCDEBUG(fmt, args...) printf(fmt, ## args)
#else
 #define RPCDEBUG(fmt, args...)  /*empty macro */
#endif

#define REPLYID 99   // for registering the Reply_Handler

#define NONBLOCK_MODE	0x00
#define	BLOCK_MODE		0x01
#define USE_POLL_BUF	0x00
#define USE_INTR_BUF	0x02
#define SEND_VIDEO_CPU	0x00
#define SEND_AUDIO_CPU	0x04


/*
 * The data structure for user assign the sending mode
 * the sendMode defined in rpcapi.h
 */
typedef struct CLNT_STRUCT CLNT_STRUCT;

struct CLNT_STRUCT {
    int sendMode;  // (NONBLOCK_MODE, BLOCK_MODE) | (USE_POLL_BUF, USE_INTR_BUF)
    long programID;  // The program ID defined in IDL file
    long versionID;  // The version ID defined in IDL file
#ifdef RTD_1195
    long sysPID;        // the callee's task ID
#endif
};



/*
 * The packet header of RPC
 */
typedef struct RPC_STRUCT RPC_STRUCT;

struct RPC_STRUCT {
    u_long programID;      // program ID defined in IDL file
    u_long versionID;      // version ID defined in IDL file
    u_long procedureID;    // function ID defined in IDL file

    u_long taskID;         // the caller's task ID, assign 0 if NONBLOCK_MODE
#ifdef RTD_1195
    u_long sysPID;         // the callee's task ID
#endif
    u_long parameterSize;  // packet's body size
    u_int context;         // return address of reply value
};



bool_t                 xdr_RPC_STRUCT(XDR *, RPC_STRUCT *);

CLNT_STRUCT            prepareCLNT(int opt, long programID, long versionID);

#ifdef __cplusplus
}
#endif

#endif


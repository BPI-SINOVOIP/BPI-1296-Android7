#ifndef _H_CLIENT_STUB_H
#define _H_CLIENT_STUB_H

#include <xdr/types.h>
#include <xdr/xdr.h>
#include <RPCstruct.h>
#include <rpcstubapi.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RPC_SUCCESS 0
#define RPC_ERROR 1


int
client_call ( RPC_STRUCT *rpc, 
	      u_long procedureID,
	      int sendMode,
              xdrproc_t xdr_args, 
	      caddr_t args_ptr,
	      long args_size );

RPC_STRUCT
prepareRPCCall (CLNT_STRUCT *clnt,
		int result);

void
SendReply(
   u_long     req_taskID,
   int        req_context,
   char     * ReplyParameter,   // parameter's start address
   u_long     ParameterSize,    // parameter's size
   xdrproc_t  _xdr_result,
   int        opt);


#ifdef __cplusplus
}
#endif
#endif


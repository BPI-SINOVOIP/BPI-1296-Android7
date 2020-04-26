#ifndef _RPC_PROXY_SVC_FUNC_H_
#define _RPC_PROXY_SVC_FUNC_H_


#include <string.h>
#include <ClientStub.h> //@FIXME: ycyang
#ifdef ANDROID
#include <pthread.h>
#endif
    // can call client's function??
#ifdef __cplusplus
extern "C"{
#endif  

int
rpc_getargs(RPC_STRUCT *rpc,
            xdrproc_t  _xdr_argument,
            caddr_t    argument,
            int  opt);

void
Reply_Handler(RPC_STRUCT *rpc,
              int  opt,
              pthread_mutex_t *pMutex);

struct REG_STRUCT *
ipc_register(struct REG_STRUCT * reg,
            u_long       programID,
            u_long       versionID,
            void        (*func)(RPC_STRUCT *, int, pthread_mutex_t *));

struct REG_STRUCT * 
ReplyHandler_register( struct REG_STRUCT * reg);
int ipc_unregister(struct REG_STRUCT * reg);

#ifdef __cplusplus            
}
#endif
#endif

                        


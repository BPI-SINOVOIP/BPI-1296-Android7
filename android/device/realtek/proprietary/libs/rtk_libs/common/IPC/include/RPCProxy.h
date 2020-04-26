#ifndef _H_RPC_PROXY_H_
#define _H_RPC_PROXY_H_

#include <rpcapi.h>
#include <sys/types.h>
#include <RPCstruct.h>
#include <pthread.h>
#include <rpcstubapi.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*
 * The linking-list for register function
 */
typedef struct REG_STRUCT REG_STRUCT;

struct REG_STRUCT {
	u_long programID;                // program ID record in IDL file
	u_long versionID;                // version ID record in IDL file
	void (*func)(RPC_STRUCT *, int, pthread_mutex_t *); // dispatch function pointer
	REG_STRUCT * next;               // next node
};

typedef struct THREAD_STRUCT THREAD_STRUCT;
struct THREAD_STRUCT {
	REG_STRUCT *pReg;
	pthread_mutex_t *pMutex; 
	int flag;
};


void RPCProxy_loop(THREAD_STRUCT * pData);
#ifdef ANDROID
int initRPCProxy();
#else
int initRPCProxy(REG_STRUCT *reg);
#endif
int DeInitRPCProxy();

#ifdef __cplusplus
}
#endif
#endif


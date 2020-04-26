#ifndef __WIFI_HDCP2_H__
#define __WIFI_HDCP2_H__

#include "SocketAVData.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STATUS_OK 0
#define STATUS_ERROR_OUTGOING_MSG_TOO_LARGE -2
#define STATUS_OK_PENDING_MSG 1

typedef enum
{
   CID_INIT,    /* Initialize the xtask */
   CID_MESSAGE_SEND,    /* Pass a message from the RX to the XTASK */
   CID_MESSAGE_POLL,    /* Poll XTASK for a pending message */
   CID_STATE_GET,        /* Receive the HDCP2 state of the Xtask */
   CID_CHECK_HDCP1X,     /* poll check the status of HDCP1X */
   CID_SEND_KSVS,
   CID_STATE_HDMI1X_POLICING_DECT
} CID_t;

int cpu_spu_sendmsg(CID_t cmd, unsigned char* indata, int insize);
int cpu_spu_rcvmsg(unsigned char *outdata, int *outsize);
bool spu_nego_status(SOCKET_CW_OFFSET * aeskey, SOCKET_CW_OFFSET* aesiv) ;
void spu_set_thread_mutex(pthread_mutex_t* pMutex);

#ifdef __cplusplus
}
#endif

#endif

#ifndef _RVSD_INBANDCMD_H_
#define _RVSD_INBANDCMD_H_

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <poll.h>
#include "pthread-macro.h"
#include "rvsd_InBandAPI.h"
#include "domain_socket.h"
#include "rvsd_OSAL.h"
#include <errno.h>
//#include <AwesomePlayer.h>

//using namespace android;

//#define MAX_RVSD_VDEC_BUFFER 256

#define IPC_PACKET_INDEX 0
#define IPC_PACKET_TOTAL_SIZE 4
#define IPC_PACKET_PAYLOAD_SIZE 8
#define IPC_PACKET_PAYLOAD_START 12

#endif
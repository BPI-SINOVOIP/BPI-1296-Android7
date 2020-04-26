#ifndef _RPCAPI_H
#define _RPCAPI_H

#define NONBLOCK_MODE	0x00
#define	BLOCK_MODE		0x01
#define USE_POLL_BUF	0x00
#define USE_INTR_BUF	0x02
#define SEND_VIDEO_CPU	0x00
#define SEND_AUDIO_CPU	0x04

#ifdef __cplusplus
extern "C" {
#endif 

int			openRPC();
int			closeRPC();

void		setTimeout(int msec);

int			readRingBuf(int opt, char *buf, int size);

void		ackTask(int taskid);

#ifdef __cplusplus
}
#endif
#endif

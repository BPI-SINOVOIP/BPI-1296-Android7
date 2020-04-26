#ifndef _RPCSTUBAPI_H
#define _RPCSTUBAPI_H

#define NONBLOCK_MODE	0x00
#define	BLOCK_MODE		0x01
#define USE_POLL_BUF	0x00
#define USE_INTR_BUF	0x02
#define SEND_VIDEO_CPU	0x00
#define SEND_AUDIO_CPU	0x04

#ifdef __cplusplus
extern "C" {
#endif

int			openStubRPC();
int			closeStubRPC();

long		getTaskID();

int			writeRingBuf(int opt, char *buf, int size);

void		waitReply();

#ifdef __cplusplus
}
#endif
#endif

#ifndef __SERVER_PROTO_RTP_H
#define __SERVER_PROTO_RTP_H

#include "thread_reader.h"

extern int server_rtp_open(const char * url, struct server_ringBuffer * ring);

#endif

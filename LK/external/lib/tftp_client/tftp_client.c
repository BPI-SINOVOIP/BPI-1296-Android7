/****************************************************************//**
 *
 * @file tftp_client.c
 *
 * @author   Eric Wang <ericwang@realtek.com>
 *
 * @brief    Trivial File Transfer Protocol (RFC 1350)
 *
 * Copyright (c) Eric Wang 2016
 * All rights reserved.
 *
 ********************************************************************/

/* 
 * Redistribution and use in source and binary forms, with or without
 * modification,are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include <lwip/opt.h>
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/udp.h>
#include <lwip/debug.h>
#include <string.h>
#include <trace.h>

#define LOCAL_TRACE 0

#ifndef TFTP_DEBUG
#define TFTP_DEBUG LWIP_DBG_ON
#endif

#ifndef TFTP_PORT
#define TFTP_PORT 69
#endif

#ifndef TFTP_TIMEOUT_MSECS
#define TFTP_TIMEOUT_MSECS    10000
#endif

#ifndef TFTP_MAX_RETRIES
#define TFTP_MAX_RETRIES      5
#endif

/*
 *      TFTP operations.
 */
#define RRQ   1
#define WRQ   2
#define DATA  3
#define ACK   4
#define ERROR 5
#define OACK  6

/*
 *	TFTP mode
 */
#define TFTP_CLIENT_GET       0
#define TFTP_CLIENT_PUT       1

#define TFTP_BLOCK_SIZE     512
#define TFTP_RX_BUF_SIZE    550
#define TFTP_TX_BUF_SIZE    550
#define TFTP_ACK_BUF_SIZE     4


#define FILENAME_SIZE       128

static char remote_file[FILENAME_SIZE];
static addr_t tftp_addr;
static uint32_t tftp_len;

err_t tftp_start(uint8_t mode, addr_t buf_addr, uint32_t buf_len,
		 ip_addr_t remote_ip, char *filename)
{
	int s;
	struct sockaddr_in to;
	struct sockaddr_in from;
	u8_t request[TFTP_BLOCK_SIZE];
	u8_t rx_buf[TFTP_RX_BUF_SIZE];
	u8_t tx_buf[TFTP_TX_BUF_SIZE];
	int request_len;
	int fromlen, len;
	u8_t *ptr, *pre_ptr = NULL;
	u16_t *op, *u16_ptr, *rx_blk, block = 0, code;
	int err = ERR_OK;
	int cnt = 0, tot_len = 0;
	bool finish = false;
	char *msg;

	strcpy(remote_file, filename);
	remote_file[FILENAME_SIZE-1] = '\0';
	tftp_addr = buf_addr;
	tftp_len = buf_len;
	if (mode == TFTP_CLIENT_GET)
		printf("TFTP get file %s to addr 0x%lx from server ",
			remote_file, buf_addr);
	else
		printf("TFTP put addr 0x%lx (%d bytes) to file %s in ",
			buf_addr, buf_len, remote_file);
	ip_addr_print(&remote_ip);
	printf("\n");

	/* create UDP socket */
	if ((s = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		LTRACEF("Can't create socket\n");
		return ERR_MEM;
	}

	/* assign server addr */
	memset(&to, 0, sizeof(struct sockaddr_in));
	to.sin_len = sizeof(to);
	to.sin_family = AF_INET;
	inet_addr_from_ipaddr(&to.sin_addr, &remote_ip);
	to.sin_port = PP_HTONS(TFTP_PORT);

	/* prepare RRQ/WRQ request */
	memset(request, 0, TFTP_BLOCK_SIZE);
	ptr = request;
	op = (u16_t *) ptr;
	if (mode == TFTP_CLIENT_GET)
		*op = PP_HTONS(RRQ);
	else
		*op = PP_HTONS(WRQ);
	ptr += 2;
	ptr += sprintf((char *)ptr,"%s%c%s%c%s%c%u%c%s%c%d%c",
		remote_file, 0, "octet", 0, "timeout", 0,
		TFTP_TIMEOUT_MSECS / 1000, 0, "blksize", 0, TFTP_BLOCK_SIZE, 0);
	request_len = (uint64_t) (ptr - request);

	LTRACEF("TFTP request %s len %d\n",
		(mode == TFTP_CLIENT_GET)?"RRQ":"WRQ", request_len);
	#if LOCAL_TRACE
	hexdump8(request, (u16_t)request_len);
	#endif

	if((err = lwip_sendto(s, request, request_len, 0,
				(struct sockaddr*)&to, sizeof(to))) == -1) {
		LTRACEF("can't send TFTP %s, ret = %d\n",
			(mode == TFTP_CLIENT_GET)?"RRQ":"WRQ", err);
		goto OUT;
	}

	ptr = (u8_t *) tftp_addr;
	memset(rx_buf, 0, TFTP_RX_BUF_SIZE);
	fromlen = sizeof(from);

	do {
		len = lwip_recvfrom(s, rx_buf, sizeof(rx_buf), 0,
				(struct sockaddr*)&from, (socklen_t*)&fromlen);
		LTRACEF("recv %d bytes, from port %d\n",
			len, PP_NTOHS(from.sin_port));
		#if LOCAL_TRACE
		hexdump8(rx_buf, 4);
		#endif
		to.sin_port = from.sin_port;
		
		op = (u16_t *) rx_buf;
		memset(tx_buf, 0, TFTP_TX_BUF_SIZE);
		u16_ptr = (u16_t *) &tx_buf[0];
		switch(PP_NTOHS(*op)) {
		case DATA:
			*u16_ptr = PP_HTONS(ACK);
			if (len < (TFTP_BLOCK_SIZE + 4))
				finish = true;
			memcpy(ptr, &rx_buf[4], len - 4);
			ptr += (len - 4);
			tot_len += (len - 4);
			rx_blk = (u16_t *) &rx_buf[2];
			block = PP_NTOHS(*rx_blk);
			tx_buf[2] = rx_buf[2];
			tx_buf[3] = rx_buf[3];
			if ((block % 10) == 0) {
				printf("#");
				if ((block % 700) == 0)
					printf("\n");
			}
			cnt = 0;
			len = TFTP_ACK_BUF_SIZE;
			break;
		case ACK:
			LTRACEF("recv ACK\n");
			*u16_ptr = PP_HTONS(ACK);
			len = TFTP_ACK_BUF_SIZE;
			if (mode == TFTP_CLIENT_PUT) {
				if ( (uintptr_t) ptr >= tftp_addr + tftp_len) {
					finish = true;
					printf("\nTotal %d bytes are sent\n",
						tot_len);
					goto OUT;
				}
				rx_blk = (u16_t *) &rx_buf[2];
				if (block == PP_NTOHS(*rx_blk)) {
					if ((block % 10) == 0) {
						printf("#");
						if ((block % 700) == 0)
							printf("\n");
					}
					block++;
					cnt = 0;
				} else {
					LTRACEF("TX block %d RX block %d\n",
						block, PP_NTOHS(*rx_blk));
					ptr = pre_ptr;
					cnt++;
				}

				*u16_ptr = PP_HTONS(DATA);
				len = LWIP_MIN((tftp_addr + tftp_len) - ((uintptr_t) ptr),
					  (uintptr_t)TFTP_BLOCK_SIZE);
				if (ptr != pre_ptr)
					tot_len += len;
				memcpy(&tx_buf[4], ptr, len);
				pre_ptr = ptr;
				ptr += (uintptr_t) len;
				len += TFTP_ACK_BUF_SIZE;
			}
			*(u16_ptr + 1) = PP_HTONS(block);
			break;
		case OACK:
			LTRACEF("recv OACK\n");
			*u16_ptr = PP_HTONS(ACK);
			len = TFTP_ACK_BUF_SIZE;
			if (mode == TFTP_CLIENT_PUT) {
				*u16_ptr = PP_HTONS(DATA);
				len = LWIP_MIN((tftp_addr + tftp_len) - ((uintptr_t) ptr),
					  (uintptr_t)TFTP_BLOCK_SIZE);
				memcpy(&tx_buf[4], ptr, len);
				ptr += (uintptr_t) len;
				tot_len += len;
				block++;
				len += TFTP_ACK_BUF_SIZE;
			}
			*(u16_ptr + 1) = PP_HTONS(block);
			break;
		case ERROR:
			LTRACEF("recv ERROR\n");
			u16_ptr = (u16_t *) &rx_buf[2];
			code = PP_NTOHS(*u16_ptr);
			msg = (char *) &rx_buf[4];
			printf("TFTP ERROR: [%s] (%d)\n", msg, code);
			goto OUT;
		default:
			LTRACEF("TFTP: unsupport op code %d\n", PP_NTOHS(*op));
			*u16_ptr = PP_HTONS(ERROR);
			*(u16_ptr + 1) = PP_HTONS(4); /* Illegal TFTP operation */
			strcpy((char *) &tx_buf[4], "Illegal TFTP operation");
			len = TFTP_ACK_BUF_SIZE + 23;
			printf("T");
			cnt++;
		}
		if((err = lwip_sendto(s, tx_buf, len, 0, (struct sockaddr*)&to,
				sizeof(to))) == -1) {
			LTRACEF("can't send TFTP ACK, rx len = %d, ret = %d\n",
				tot_len, err);
			goto OUT;
		}
	} while ((!finish) && cnt <= TFTP_MAX_RETRIES);
	printf("\nReceive %s size %d bytes at addr 0x%lx\n",
		remote_file, tot_len, tftp_addr);
	arch_clean_cache_range(tftp_addr, tot_len);
OUT:
	lwip_close(s);
	return err;
}

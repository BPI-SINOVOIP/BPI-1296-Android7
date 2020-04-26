/*
 * Copyright (c) 2013 Corey Tabaka
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#if defined(WITH_LIB_CONSOLE)
#include <lib/console.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lwip/api.h>
#include <lwip/ip_addr.h>
#include <lwip/netif.h>
#include <lwip/dhcp.h>
#include <lib/sysparam.h>

#define MAX_NAME_LEN 128
extern void ping_cmd(ip_addr_t ip_addr);
extern err_t tftp_start(uint8_t mode, addr_t buf_addr, uint32_t len,
			ip_addr_t remote_ip, char *filename);
extern void r8168_set_addr(ip_addr_t ipaddr, ip_addr_t netmask, ip_addr_t gw);

static int net_cmd(int argc, const cmd_args *argv)
{
	if (argc < 2) {
		printf("%s commands:\n", argv[0].str);
usage:
		printf("%s lookup <hostname>\n", argv[0].str);
		printf("%s ping <target IP>\n", argv[0].str);
		printf("%s tftp <addr> <target IP> <filename>\n", argv[0].str);
		printf("%s tftpput <addr> <size> <target IP> <filename>\n",
			argv[0].str);
		printf("%s ifconfig <ip addr> <netmask> <gw>\n", argv[0].str);
		printf("%s dhcp_client\n", argv[0].str);
		goto out;
	}

	if (!strcmp(argv[1].str, "lookup")) {
		if (argc < 3)
			goto usage;

		ip_addr_t ip_addr;
		const char *hostname = argv[2].str;
		err_t err;

		err = netconn_gethostbyname(hostname, &ip_addr);
		if (err != ERR_OK) {
			printf("Failed to resolve host: %d\n", err);
		} else {
			printf("%s: %u.%u.%u.%u\n", hostname,
					ip4_addr1_16(&ip_addr),
					ip4_addr2_16(&ip_addr),
					ip4_addr3_16(&ip_addr),
					ip4_addr4_16(&ip_addr));
		}
	}
	if (!strcmp(argv[1].str, "ping")) {
		if (argc < 3)
			goto usage;

		ip_addr_t ip_addr;
		const char *ip_str = argv[2].str;

		ipaddr_aton(ip_str, &ip_addr);
		ping_cmd(ip_addr);
	}

	if (!strcmp(argv[1].str, "tftp")) {
		if (argc < 5)
			goto usage;

		addr_t buf_addr;
		ip_addr_t ip_addr;
		char name[MAX_NAME_LEN];
		char *endptr;
		const char *mem_addr_str = argv[2].str;
		const char *ip_str = argv[3].str;
		const char *filename = argv[4].str;

		buf_addr = (addr_t) strtoul(mem_addr_str, &endptr, 0);
		ipaddr_aton(ip_str, &ip_addr);
		strncpy(name, filename, MAX_NAME_LEN);
		name[MAX_NAME_LEN - 1] = 0;

		tftp_start(0, buf_addr, 0, ip_addr, name);
	}

	if (!strcmp(argv[1].str, "tftpput")) {
		if (argc < 6)
			goto usage;

		addr_t buf_addr;
		uint32_t len;
		ip_addr_t ip_addr;
		char name[MAX_NAME_LEN];
		char *endptr;
		const char *mem_addr_str = argv[2].str;
		const char *mem_size_str = argv[3].str;
		const char *ip_str = argv[4].str;
		const char *filename = argv[5].str;

		buf_addr = (addr_t) strtoul(mem_addr_str, &endptr, 0);
		len = strtoul(mem_size_str, &endptr, 0);
		ipaddr_aton(ip_str, &ip_addr);
		strncpy(name, filename, MAX_NAME_LEN);
		name[MAX_NAME_LEN - 1] = 0;

		tftp_start(1, buf_addr, len, ip_addr, name);
	}

	if (!strcmp(argv[1].str, "ifconfig")) {
		if (argc < 5)
			goto usage;

		ip_addr_t ip_addr, netmask, gw;
		const char *ip_str = argv[2].str;
		const char *netmask_str = argv[3].str;
		const char *gw_str = argv[4].str;

		sysparam_add("ipaddr", ip_str, strlen(ip_str));
		sysparam_add("netmask", netmask_str, strlen(netmask_str));
		sysparam_add("gatewayip", gw_str, strlen(gw_str));
		ipaddr_aton(ip_str, &ip_addr);
		ipaddr_aton(netmask_str, &netmask);
		ipaddr_aton(gw_str, &gw);
#ifndef DEVICE_NO_ETHERNET
		r8168_set_addr(ip_addr, netmask, gw);
#endif
	}

	if (!strcmp(argv[1].str, "dhcp_client")) {
		char name[MAX_NAME_LEN];
		struct netif *en0;

		sprintf(name, "%s", "en0");
		en0 = netif_find(name);
		if (en0) {
			printf("DHCP client start\n");
			netif_set_down(en0);
			if(dhcp_start(en0) != ERR_OK)
				printf("DHCP client fail\n");
		} else {
			printf("can't get network interface en0\n");
		}
	}

out:
	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("net", "net toolbox", &net_cmd)
STATIC_COMMAND_END(net);

#endif


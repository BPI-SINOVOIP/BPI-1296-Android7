
#ifndef __CAPWAP_WTPInternetCheck_HEADER__
#define __CAPWAP_WTPInternetCheck_HEADER__

#include <netinet/in.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE			8192
#define	MAX_PING_TIME	500		//in 0.1ms
#define INTERNET_CHECK_INTERVAL	5		//in seconds

char gGatewayIp[255];
char PingCmd[128];
FILE* fp_cmd;					//PingCmd
CWTimerID InternetCheckTimerID;



struct route_info {
    struct in_addr dstAddr;
    struct in_addr srcAddr;
    struct in_addr gateWay;
    char ifName[IF_NAMESIZE];
};
#endif

#ifdef NEW_CHECKINTERNET
CWBool CWStartInternetCheckTimer();
CWBool CWStopInternetCheckTimer();
#endif


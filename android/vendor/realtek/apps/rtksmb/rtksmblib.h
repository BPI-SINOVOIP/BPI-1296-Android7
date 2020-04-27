#ifndef __UTILITY_SAMBA_H__
#define __UTILITY_SAMBA_H__

extern "C" {
#include <arpa/inet.h>


    // get IP from NetBios Name
    int Samba_NetBiosName2Ip(char *, struct in_addr *);
    // Query Domain 
    int Samba_QueryDomainList(void **Handle,int *num);
    int Samba_GetDomainList(void *Handle,int num,char **Domain);
    // Query Server
    int Samba_QueryServerList(void **Handle,int *num,char *domain);
    int Samba_GetServerList(void *Handle,int num,char **Server);
    // free Domain or Server's Handle
    int Samba_freeHandle(void *Handle);
    // Query Server Share List
    int Samba_QueryServerShareList(void **Handle,int *num,char *ip,char *p_username,char *p_password,char *p_domain=0);
    int Samba_GetServerShareList(void *Handle,int num,char **Share,int *type);
    // free Server Share Handle , must input total number from Samba_QueryServerShareList
    int Samba_freeServerShareHandle(void *Handle,int Total);
}

#define MAIN "MAIN"
#define COMMAND "COMMAND"
#define NUMBER "num"
#define P1 "P1"
#define P2 "P2"
#define P3 "P3"
#define P4 "P4"
#endif

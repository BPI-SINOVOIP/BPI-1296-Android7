#ifndef _DOMAIN_SOCKET_H_
#define _DOMAIN_SOCKET_H_

typedef struct _GENERAL_PKT
{
    //header
    unsigned char 	Control;
    unsigned char 	Subtype;

    unsigned short	DataSize;
    unsigned char	Data[16];
//	unsigned char	CmdInst[1024];
} GeneralPkt, *pGeneralPkt;

#endif
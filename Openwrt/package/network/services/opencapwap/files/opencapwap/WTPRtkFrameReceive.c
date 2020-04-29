/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	       *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.                *
 *                                                                                         *
 * You should have received a copy of the GNU General Public License along with this       *
 * program; if not, write to the:                                                          *
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,                    *
 * MA  02111-1307, USA.                                                                    *
 *                                                                                         *
 * --------------------------------------------------------------------------------------- *
 * Project:  Capwap                                                                        *
 *                                                                                         *
 *******************************************************************************************/	

#ifdef RTK_SMART_ROAMING

#include "CWWTP.h"

#define NETLINK_TYPE_STA_INFO					1
#define NETLINK_TYPE_NEW_STA					2
#define NETLINK_TYPE_STA_FT_OVER_DS			3
#define NETLINK_TYPE_STA_FT_OVER_AIR		4

#define NETLINK_USER						31


CW_THREAD_RETURN_TYPE CWWTPRtkReceiveFrame(void *arg)
{
	int retval,i,offset,data_len;
	unsigned char type;

	unsigned char *payload;	
	unsigned int	ping_speed;		/* do host-to-network convert before sending */
	
	struct nlmsghdr *nlh;
	struct iovec iov;	 
	struct msghdr msg;
	gNetlinkSock = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
	
	if (gNetlinkSock < 0) { 
		CWRoamLog("Error to create socket for ioctl"); 
		return CW_FALSE;
	}

	memset(&gNetlinkSrcAddr, 0, sizeof(gNetlinkSrcAddr));
	gNetlinkSrcAddr.nl_family = AF_NETLINK;
	gNetlinkSrcAddr.nl_pid = rtk_getpid(); /* self pid */
	gNetlinkSrcAddr.nl_groups = 0; /* unicast */
	memset(&gNetlinkDestAddr, 0, sizeof(gNetlinkDestAddr));
	gNetlinkDestAddr.nl_family = AF_NETLINK;
	gNetlinkDestAddr.nl_pid = 0; /* For Linux Kernel */
	gNetlinkDestAddr.nl_groups = 0; /* unicast */
				
	if (bind(gNetlinkSock, (struct sockaddr*)&gNetlinkSrcAddr, sizeof(gNetlinkSrcAddr)) < 0) { 	 
		CWRoamLog("Error to bind socket for ioctl"); 
		close(gNetlinkSock);
		return CW_FALSE;
	}


	/* Fill the netlink message header */		 
	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(CW_MAX_STR_BUFFER_SIZE));
	memset(nlh, 0, NLMSG_SPACE(CW_MAX_STR_BUFFER_SIZE));
	nlh->nlmsg_len = NLMSG_SPACE(CW_MAX_STR_BUFFER_SIZE);
	nlh->nlmsg_pid = rtk_getpid(); //sending process ID
	nlh->nlmsg_flags = 0;

	strcpy(NLMSG_DATA(nlh), "S");

	/*iov structure */
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;

	/* msg */
	memset(&msg, 0, sizeof(msg));
	msg.msg_name = (void *)&gNetlinkDestAddr;
	msg.msg_namelen = sizeof(gNetlinkDestAddr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
		
	/*Send message to kernel for pid */	 
	if( sendmsg(gNetlinkSock, &msg, 0) <= 0) {
		CWRoamLog("user sendmsg err: errno=%d!", errno);
		return -errno;
	}else{
		CWRoamDebugLog("send driver an S to start timer");
	}

	/* Read message from kernel */
	CW_REPEAT_FOREVER 
	{
		memset(nlh, 0, NLMSG_SPACE(CW_MAX_STR_BUFFER_SIZE));
		retval = recvmsg(gNetlinkSock, &msg, 0);
	
		if( retval <= 0) {
			CWRoamLog("user recvmsg err: errno=%d!", errno);
			return -errno;
		}
	
		payload = NLMSG_DATA(nlh);
		offset = 0;

		memcpy(&data_len, payload+offset, sizeof(data_len));
		offset += sizeof(data_len);
		if(!data_len || data_len>CW_MAX_STR_BUFFER_SIZE) {
			CWRoamLog("Invalid data len!");
			break;
		}
		memcpy(&type, payload+offset, sizeof(type));
		offset += sizeof(type);
		
		CWRoamDebugLog("wlan info from driver (len=%d type=%d)",data_len,type);

		CWListElement msgElemList;
		msgElemList.next = NULL;

		CW_CREATE_OBJECT_ERR(msgElemList.data, CWMsgElemData, ;); 				

		switch(type){
			case NETLINK_TYPE_STA_INFO:
				((CWMsgElemData*)(msgElemList.data))->type = CW_MSG_ELEMENT_ADD_STATION_CW_TYPE;
				break;
			case NETLINK_TYPE_NEW_STA:
				((CWMsgElemData*)(msgElemList.data))->type = CW_MSG_ELEMENT_NEW_STATION_CW_TYPE;
				break;
			case NETLINK_TYPE_STA_FT_OVER_DS:
				((CWMsgElemData*)(msgElemList.data))->type = CW_MSG_ELEMENT_STATION_FT_OVER_DS_CW_TYPE;
				break;
			case NETLINK_TYPE_STA_FT_OVER_AIR:
				((CWMsgElemData*)(msgElemList.data))->type = CW_MSG_ELEMENT_STATION_FT_OVER_AIR_CW_TYPE;
				break;
			default:
				goto END;
		}

		//do host-to-network convert and add ping speed result
		ping_speed = htonl(gPingSpeed);
		memcpy(&(((CWMsgElemData*)(msgElemList.data))->value.str),&ping_speed,sizeof(ping_speed));
		memcpy(&(((CWMsgElemData*)(msgElemList.data))->value.str[sizeof(ping_speed)]),payload+offset,data_len);
		CWWTPCheckForWTPEventRequest(&msgElemList, data_len-sizeof(type)+sizeof(ping_speed));

END:
		CW_FREE_OBJECT(msgElemList.data);
	}
	return(NULL);
}

#endif

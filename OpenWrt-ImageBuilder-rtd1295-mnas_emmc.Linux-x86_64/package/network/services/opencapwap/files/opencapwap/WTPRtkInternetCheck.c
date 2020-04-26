
#include "CWWTP.h"

unsigned int calc_time = INTERNET_CHECK_INTERVAL;


#ifdef RTK_SMART_ROAMING
int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId)
{
    struct nlmsghdr *nlHdr;
    int readLen = 0, msgLen = 0;

 	do {
    	/* Recieve response from the kernel */
        if ((readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0) {
            CWRoamLog("SOCK READ: ");
            return -1;
        }

        nlHdr = (struct nlmsghdr *) bufPtr;

    	/* Check if the header is valid */
        if ((NLMSG_OK(nlHdr, readLen)==0) || (nlHdr->nlmsg_type==NLMSG_ERROR)) {
            CWRoamLog("Error in recieved packet");
            return -1;
        }

    	/* Check if the its the last message */
        if (nlHdr->nlmsg_type == NLMSG_DONE) {
            break;
        } else {
    	/* Else move the pointer to buffer appropriately */
            bufPtr += readLen;
            msgLen += readLen;
        }

    	/* Check if its a multi part message */
        if ((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) {
           /* return if its not */
            break;
        }
    } while ((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));

    return msgLen;
}


void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo)
{
    struct rtmsg *rtMsg;
    struct rtattr *rtAttr;
    int rtLen;

	char tmp[20];

    rtMsg = (struct rtmsg *) NLMSG_DATA(nlHdr);

	/* Check if the route is not for AF_INET or does not belong to main routing table */
    if ((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN)){
		CWRoamLog("Error Route");
        return;
    }

	/* get the rtattr field */
    rtAttr = (struct rtattr *) RTM_RTA(rtMsg);
    rtLen = RTM_PAYLOAD(nlHdr);
    for (; RTA_OK(rtAttr, rtLen); rtAttr = RTA_NEXT(rtAttr, rtLen)) {
        switch (rtAttr->rta_type) {
        case RTA_OIF:
            if_indextoname(*(int *) RTA_DATA(rtAttr), rtInfo->ifName);
			CWRoamLog("rtInfo->ifName=%s\n", rtInfo->ifName);
            break;
        case RTA_GATEWAY:
            rtInfo->gateWay.s_addr= *(u_int *) RTA_DATA(rtAttr);
			sprintf(tmp, (char *) inet_ntoa(rtInfo->gateWay));
			CWRoamLog("RTA_GATEWAY %s\n", tmp);
            break;
        case RTA_PREFSRC:
            rtInfo->srcAddr.s_addr= *(u_int *) RTA_DATA(rtAttr);
			sprintf(tmp, (char *) inet_ntoa(rtInfo->srcAddr));
			CWRoamLog("RTA_PREFSRC %s\n", tmp);
            break;
        case RTA_DST:
            rtInfo->dstAddr .s_addr= *(u_int *) RTA_DATA(rtAttr);
			sprintf(tmp, (char *) inet_ntoa(rtInfo->dstAddr));
			CWRoamLog("RTA_DST %s\n", tmp);
            break;
        }
    }

	if(gMesh_portal) {
		if(!strcmp(rtInfo->ifName, BR_NAME) ){
			sprintf(gGatewayIp, (char *) inet_ntoa(rtInfo->srcAddr));
			CWRoamLog("Set gGatewayIp=%s\n", gGatewayIp);
		}
	}
	else if (rtInfo->dstAddr.s_addr == 0) {
		sprintf(gGatewayIp, (char *) inet_ntoa(rtInfo->gateWay));
		CWRoamLog("Set gGatewayIp=%s\n", gGatewayIp);
	}

	//printf("get gateway IP: %s\n", gGatewayIp);		
	CWRoamLog("get gateway IP: %s\n", gGatewayIp);
    return;
}

void getGatewayIpAddr()
{
    struct nlmsghdr *nlMsg;
    struct rtmsg *rtMsg;
    struct route_info *rtInfo;
    char msgBuf[BUFSIZE];

    int sock, len, msgSeq = 0;

	/* Create Socket */
    if ((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)
        CWRoamLog("Socket Creation: ");

    memset(msgBuf, 0, BUFSIZE);

	/* point the header and the msg structure pointers into the buffer */
    nlMsg = (struct nlmsghdr *) msgBuf;
    rtMsg = (struct rtmsg *) NLMSG_DATA(nlMsg);

	/* Fill in the nlmsg header*/
    nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));  // Length of message.
    nlMsg->nlmsg_type = RTM_GETROUTE;   // Get the routes from kernel routing table .

    nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;    // The message is a request for dump.
    nlMsg->nlmsg_seq = msgSeq++;    // Sequence of the message packet.
    nlMsg->nlmsg_pid = rtk_getpid();    // PID of process sending the request.

	/* Send the request */
    if (send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0) {
        CWRoamLog("Error: Write To Socket Failed (%s)", __FUNCTION__);
        return;
    }

	/* Read the response */
    if ((len = readNlSock(sock, msgBuf, msgSeq, rtk_getpid())) < 0) {
        CWRoamLog("Error: Read From Socket Failed (%s)", __FUNCTION__);
    	return;
    }
	
	/* Parse and print the response */
    rtInfo = (struct route_info *) malloc(sizeof(struct route_info));
	//fprintf(stdout, "Destination\tGateway\tInterface\tSource\n");
	
    for (; NLMSG_OK(nlMsg, len); nlMsg = NLMSG_NEXT(nlMsg, len)) {
        memset(rtInfo, 0, sizeof(struct route_info));
        parseRoutes(nlMsg, rtInfo);
    }
	
    free(rtInfo);
    close(sock);

    return;
}

#ifdef NEW_CHECKINTERNET

void CWWTPInternetCheckTimerExpiredHandler(void *arg)
{
	FILE *fp_txt = NULL;
	char ch;
	int i = 0;
	unsigned int total_ping_time = 0;
	unsigned int tmp[7], ping_time;

	//get ping time
	fp_txt = fopen("/var/log/ping.txt", "r");
#ifdef RTK_129X_PLATFORM
	total_ping_time = 0;
	for (i=0;i<calc_time;i++){
		fgets(tmp, 7,fp_txt);
		if(!strcmp("0\n",tmp))
			total_ping_time += MAX_PING_TIME;
		else
			total_ping_time += atoi(tmp);
	}

	ping_time = total_ping_time/calc_time;
	CWRoamLog("avg ping time=%d", ping_time);
#else
	if(ch = getc(fp_txt) == EOF){
		gPingSpeed = 0;
		goto restart;
	}

	i = 0;

	while((ch=getc(fp_txt))!='/' && i<7){
		tmp[i++] = ch - '0';
	}
	switch(i){
		//get ping_time in 0.1ms
		case 5:
			ping_time = tmp[i-3] + tmp[0]*10;
			break;
		case 6:
			ping_time = tmp[i-3] + tmp[0]*100 + tmp[1]*10;
			break;
		case 7:
			ping_time = tmp[i-3] + tmp[0]*1000 + tmp[1]*100 + tmp[2]*10;
			break;
		default:
			ping_time = -1;
			break;
	}
#endif
	/* 1-500 in 0.1ms */
	if(ping_time <= MAX_PING_TIME)
		gPingSpeed = MAX_PING_TIME - ping_time;
	else
		gPingSpeed = 0;

restart:
	fclose(fp_txt);
	pclose(fp_cmd);

	CWStartInternetCheckTimer();
	return;
}

CWBool CWStartInternetCheckTimer()
{
	//CWRoamDebugLog("start InternetCheck timer to issue PING request (%s)", PingCmd);
	//send ping command
#ifdef RTK_129X_PLATFORM
	int i;
	char realcmd[1000]="rm -rf /var/log/ping.txt;";

	for (i=0;i<calc_time;i++)
	{
		strcat(realcmd, PingCmd);
	}

	fp_cmd = popen(realcmd,"r");
	//printf("%s testcmd=%s \n",__func__, testcmd);
#else
	fp_cmd = popen(PingCmd, "r");
#endif

	//add timer handler to timer queue
	InternetCheckTimerID = timer_add(INTERNET_CHECK_INTERVAL,
									0,
									&CWWTPInternetCheckTimerExpiredHandler,
									NULL);

	if(InternetCheckTimerID == -1)	{
		return CW_FALSE;
	}
	//CWRoamDebugLog("InternetCheck Timer Started");
	return CW_TRUE;
}

CWBool CWStopInternetCheckTimer()
{
	timer_rem(InternetCheckTimerID, NULL);

	return CW_TRUE;
}

void CWWTPRtkCheckInternet()
{
#ifndef RTK_129X_PLATFORM
	int lan_dhcp_mode;
	apmib_get(MIB_DHCP,(void *)&lan_dhcp_mode);
	if(lan_dhcp_mode == DHCP_LAN_SERVER){
#else
	if(gMesh_portal){
#endif
		CWRoamLog("DHCP LAN SERVER: exit %s", __FUNCTION__);
		gPingSpeed = MAX_PING_TIME;
		return;
	}else{
		getGatewayIpAddr();
		if(!strlen(gGatewayIp)){
			CWRoamLog("No DHCP Server: exit %s", __FUNCTION__);
			gPingSpeed = 0;
			return;
		}else{
#ifdef RTK_129X_PLATFORM
			sprintf(PingCmd, "ping -c 1 -w 1 %s | tail -n1 | awk -F \"/\" '{ print 10*$4 }' >> /var/log/ping.txt;", gGatewayIp);
#else
			sprintf(PingCmd, "ping -c 1 -w 1 %s | tail -n1 | grep -o '/[^a-z].*/' > /var/log/ping.txt", gGatewayIp);
#endif
			CWRoamDebugLog("DHCP Server: %s", gGatewayIp);
		}
	}

	CWStartInternetCheckTimer();
	return;	
}
#else
CW_THREAD_RETURN_TYPE CWWTPRtkCheckInternet(void *arg)
{	
	FILE *fp1=NULL, *fp2=NULL;
	char	ch, cmd[128];
	unsigned int	i, tmp[7], ping_time;

	unsigned int calc_time = 5;
	unsigned int total_ping_time = 0;
	getGatewayIpAddr();
	if(strlen(gGatewayIp)){
#ifdef RTK_129X_PLATFORM
		sprintf(cmd, "ping -c 1 -w 1 %s | tail -n1 | awk -F \"/\" '{ print 10*$4 }' >> /var/log/ping.txt", gGatewayIp);
#else
		sprintf(cmd, "ping -c 1 -w 1 %s | tail -n1 | grep -o '/[^a-z].*/' > /var/log/ping.txt", gGatewayIp);
#endif
		CWRoamDebugLog("%s (%s)", cmd, __FUNCTION__);
	}else{
		CWRoamLog("NO Gateway: exit %s", __FUNCTION__);
		return NULL;
	}

	CW_REPEAT_FOREVER{
#ifdef RTK_129X_PLATFORM
		total_ping_time = 0;
		system("rm /var/log/ping.txt");
		//send ping command
		for (i=0;i<calc_time;i++)
			system(cmd);
#else
		fp1 = popen(cmd, "r");
#endif
		sleep(5);
		fp2 = fopen("/var/log/ping.txt", "r");
#ifdef RTK_129X_PLATFORM
		for (i=0;i<calc_time;i++){
			fgets(tmp, 7,fp2);
			if(!strcmp("0\n",tmp))
				total_ping_time += MAX_PING_TIME;
			else
				total_ping_time += atoi(tmp);
		}
		ping_time = total_ping_time/calc_time;
#else
		//get ping time
		fp2 = fopen("/var/log/ping.txt", "r");
		ch = getc(fp2);
		if(ch == EOF){
			gPingSpeed = 0;
			fclose(fp2);
			system("killall -9 ping");
			continue;
		}
		
		i = 0;
		while((ch=getc(fp2))!='/' && i<7){
			tmp[i++] = ch - '0';
		}
		switch(i){
			//get ping_time in 0.1ms
			case 5:
				ping_time = tmp[i-3] + tmp[0]*10;
				break;
			case 6:
				ping_time = tmp[i-3] + tmp[0]*100 + tmp[1]*10;
				break;
			case 7:	
				ping_time = tmp[i-3] + tmp[0]*1000 + tmp[1]*100 + tmp[2]*10;
				break;
			default:
				ping_time = -1;
				break;
		}
		fclose(fp2);
#endif
#ifndef RTK_129X_PLATFORM
		pclose(fp1);
#endif

		if(ping_time > MAX_PING_TIME)
			gPingSpeed = 0;
		else /* 1-500 in 0.1ms */
			gPingSpeed = MAX_PING_TIME - ping_time;
	}

	return NULL;	
}
#endif

#endif

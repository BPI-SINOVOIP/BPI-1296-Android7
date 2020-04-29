/************************************************************************************************
 * Copyright (c) 2006-2009 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica	*
 *                          Universita' Campus BioMedico - Italy								*
 *																								*
 * This program is free software; you can redistribute it and/or modify it under the terms		*
 * of the GNU General Public License as published by the Free Software Foundation; either		*
 * version 2 of the License, or (at your option) any later version.								*
 *																								*
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY				*
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A				*
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.						*
 *																								*
 * You should have received a copy of the GNU General Public License along with this			*
 * program; if not, write to the:																*
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,							*
 * MA  02111-1307, USA.																			*
 *																								*
 * -------------------------------------------------------------------------------------------- *
 * Project:  Capwap																				*
 *																								*
 * Authors : Ludovico Rossi (ludo@bluepixysw.com)												*  
 *           Del Moro Andrea (andrea_delmoro@libero.it)											*
 *           Giovannini Federica (giovannini.federica@gmail.com)								*
 *           Massimo Vellucci (m.vellucci@unicampus.it)											*
 *           Mauro Bisson (mauro.bis@gmail.com)													*
 *	         Antonio Davoli (antonio.davoli@gmail.com)											*
 ************************************************************************************************/

#include "CWWTP.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
#include "WTPRtkSystemInteraction.h"

/* addresses of ACs for Discovery */
char	**gCWACAddresses;// 2013 Babylon: RTK don't use this. We use the flash mib.
int	gCWACCount = 0;

int gIPv4StatusDuplicate = 0;
int gIPv6StatusDuplicate = 0;

char *gWTPLocation = NULL;
char *gWTPName = NULL; // 2013 Babylon: RTK don't use this. We use the flash mib.
unsigned char gWTPSessionID[16];

/* if not NULL, jump Discovery and use this address for Joining */
char 		*gWTPForceACAddress = NULL;
CWAuthSecurity 	gWTPForceSecurity;

/* UDP network socket */
CWSocket 		gWTPSocket = -1;

/* DTLS session vars */
#ifndef CW_NO_DTLS
CWSecurityContext	gWTPSecurityContext = NULL;
CWSecuritySession 	gWTPSession = NULL;
#endif

CWThread gThreadReceiveFrame;


extern int dtls_thread_exit;

/* list used to pass CAPWAP packets from AC to main thread */
CWSafeList 		gPacketReceiveList;

/* used to synchronize access to the lists */
CWThreadCondition    gInterfaceWait;
CWThreadMutex 		gInterfaceMutex;

#ifdef KM_BY_AC
CWSocket 		gWTPDataSocket = -1;
/* list used to pass frames from wireless interface to main thread */
CWSafeList 		gFrameList;
CWThread gThreadReceiveDataFrame;
#endif

/* infos about the ACs to discover */
CWACDescriptor *gCWACList = NULL; // 2013 Babylon: if defined RTK_CAPWAP, we use the information in flash, otherwise, use the information in config file.
/* infos on the better AC we discovered so far */
CWACInfoValues *gACInfoPtr = NULL;
#ifdef RTK_AUTO_AC
CWACInfoValues *gminACInfoPtr = NULL;
#endif
/* WTP statistics timer */
int gWTPStatisticsTimer = CW_STATISTIC_TIMER_DEFAULT;

WTPRebootStatisticsInfo gWTPRebootStatistics;
CWWTPRadiosInfo gRadiosInfo;

/* path MTU of the current session */
int gWTPPathMTU = 0;


int gWTPRetransmissionCount;

CWThreadMutex 		gPendingRequestMutex;
CWPendingRequestMessage gPendingRequestMsgs[MAX_PENDING_REQUEST_MSGS];	
unsigned char		gRcvSeqNum[SEQUENCE_ARRAY_SIZE];

CWBool WTPExitOnUpdateCommit = CW_FALSE;
//#define CW_SINGLE_THREAD

/* 
 * Receive a message, that can be fragmented. This is useful not only for the Join State
 */
CWBool CWReceiveMessage(CWProtocolMessage *msgPtr) {
	CWList fragments = NULL;
	int readBytes;
	unsigned char buf[CW_BUFFER_SIZE];
	CWBool dataFlag = CW_FALSE;
	
	CW_REPEAT_FOREVER {
		CW_ZERO_MEMORY(buf, CW_BUFFER_SIZE);
#ifdef CW_NO_DTLS
		char *pkt_buffer = NULL;

		CWLockSafeList(gPacketReceiveList);

		while (CWGetCountElementFromSafeList(gPacketReceiveList) == 0)
			CWWaitElementFromSafeList(gPacketReceiveList);

		pkt_buffer = (char*)CWRemoveHeadElementFromSafeListwithDataFlag(gPacketReceiveList, &readBytes,&dataFlag);

		CWUnlockSafeList(gPacketReceiveList);

		CW_COPY_MEMORY(buf, pkt_buffer, readBytes);
		CW_FREE_OBJECT(pkt_buffer);
#else
		if(!CWSecurityReceive(gWTPSession, buf, CW_BUFFER_SIZE, &readBytes)) {
//printf("(%s) fail CWSecurityReceive\n", __FUNCTION__);
			return CW_FALSE;
		}
#endif

		if(!CWProtocolParseFragment(buf, readBytes, &fragments, msgPtr, &dataFlag, NULL)) {

			if(CWErrorGetLastErrorCode() == CW_ERROR_NEED_RESOURCE) { // we need at least one more fragment
				continue;
			} else { // error
				CWErrorCode error;
				error=CWErrorGetLastErrorCode();
				switch(error)
				{
					case CW_ERROR_SUCCESS: {CWDebugLog("ERROR: Success"); break;}
					case CW_ERROR_OUT_OF_MEMORY: {CWDebugLog("ERROR: Out of Memory"); break;}
					case CW_ERROR_WRONG_ARG: {CWDebugLog("ERROR: Wrong Argument"); break;}
					case CW_ERROR_INTERRUPTED: {CWDebugLog("ERROR: Interrupted"); break;}
					case CW_ERROR_NEED_RESOURCE: {CWDebugLog("ERROR: Need Resource"); break;}
					case CW_ERROR_COMUNICATING: {CWDebugLog("ERROR: Comunicating"); break;}
					case CW_ERROR_CREATING: {CWDebugLog("ERROR: Creating"); break;}
					case CW_ERROR_GENERAL: {CWDebugLog("ERROR: General"); break;}
					case CW_ERROR_OPERATION_ABORTED: {CWDebugLog("ERROR: Operation Aborted"); break;}
					case CW_ERROR_SENDING: {CWDebugLog("ERROR: Sending"); break;}
					case CW_ERROR_RECEIVING: {CWDebugLog("ERROR: Receiving"); break;}
					case CW_ERROR_INVALID_FORMAT: {CWDebugLog("ERROR: Invalid Format"); break;}
					case CW_ERROR_TIME_EXPIRED: {CWDebugLog("ERROR: Time Expired"); break;}
					case CW_ERROR_NONE: {CWDebugLog("ERROR: None"); break;}
				}
				CWDebugLog("~~~~~~");
//printf("(%s) fail CWProtocolParseFragment\n", __FUNCTION__);
				return CW_FALSE;
			}
		} else break; // the message is fully reassembled

	}

	return CW_TRUE;
}

CWBool CWWTPSendAcknowledgedPacket(int seqNum, 
				   CWList msgElemlist,
				   CWBool (assembleFunc)(CWProtocolMessage **, int *, int, int, CWList),
				   CWBool (parseFunc)(unsigned char*, int, int, void*), 
				   CWBool (saveFunc)(void*),
				   void *valuesPtr) {

	CWProtocolMessage *messages = NULL;
	CWProtocolMessage msg;
	int fragmentsNum = 0, i;

	struct timespec timewait;
	
	int gTimeToSleep = CW_RETRANSMIT_INTERVAL_SECOND;

	msg.msg = NULL;
	
	if(!(assembleFunc(&messages, 
			  &fragmentsNum, 
			  gWTPPathMTU, 
			  seqNum, 
			  msgElemlist))) {
		goto cw_failure;
	}
	
	gWTPRetransmissionCount= 0;

	while(gWTPRetransmissionCount < gCWMaxRetransmit) 
	{
		CWDebugLog("Transmission Num:%d", gWTPRetransmissionCount);
#if 0
		for(i = 0; i < fragmentsNum; i++) 
		{
#ifdef CW_NO_DTLS
			if(!CWNetworkSendUnsafeConnected(gWTPSocket, 
							 messages[i].msg,
							 messages[i].offset))
#else
			if(!CWSecuritySend(gWTPSession,
					   messages[i].msg, 
					   messages[i].offset))
#endif
			{
				CWDebugLog("Failure sending Request");

				goto cw_failure;
			}
		}

#else
		for(i = 0; i < fragmentsNum; i++) {
			int j;
			CWBool sendResult;
			for (j=0; j<5; j++) {
#ifdef CW_NO_DTLS
				sendResult = CWNetworkSendUnsafeConnected(gWTPSocket, messages[i].msg, messages[i].offset);
#else
				sendResult = CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset);
#endif
				if (sendResult) break;
				sleep(3);
				CWLog("Retry send (%s), fragment index: %d, times:%d", __FUNCTION__, i, j+1);
			}
			if (!sendResult) {
				CWDebugLog("Failure sending Request");
				goto cw_failure;
				break;
			}
		}
#endif

		
		timewait.tv_sec = time(0) + gTimeToSleep;
		timewait.tv_nsec = 0;

		CW_REPEAT_FOREVER 
		{
			CWThreadMutexLock(&gInterfaceMutex);

			if (CWGetCountElementFromSafeList(gPacketReceiveList) > 0)
				CWErrorRaise(CW_ERROR_SUCCESS, NULL);
			else {
				if (CWErr(CWWaitThreadConditionTimeout(&gInterfaceWait, &gInterfaceMutex, &timewait)))
					CWErrorRaise(CW_ERROR_SUCCESS, NULL);
			}

			CWThreadMutexUnlock(&gInterfaceMutex);

			switch(CWErrorGetLastErrorCode()) {

				msg.msg = NULL;
				case CW_ERROR_TIME_EXPIRED:
				{
					gWTPRetransmissionCount++;
					goto cw_continue_external_loop;
					break;
				}

				case CW_ERROR_SUCCESS:
				{
					/* there's something to read */
					if(!(CWReceiveMessage(&msg))) 
					{
						CWDebugLog("Failure Receiving Response");
						goto cw_failure;
					}
					
					if(!(parseFunc(msg.msg, msg.offset, seqNum, valuesPtr))) 
					{
						if(CWErrorGetLastErrorCode() != CW_ERROR_INVALID_FORMAT) {

							CWDebugLog("Failure Parsing Response");
							goto cw_failure;
						}
						else {
							CWErrorHandleLast();
                            CW_FREE_PROTOCOL_MESSAGE(msg);
							{ 
								gWTPRetransmissionCount++;
								goto cw_continue_external_loop;
							}
							break;
						}
					}
					
					if((saveFunc(valuesPtr))) {

						goto cw_success;
					} 
					else {
						if(CWErrorGetLastErrorCode() != CW_ERROR_INVALID_FORMAT) {
							CWDebugLog("Failure Saving Response");
							goto cw_failure;
						} 
					}
                    CW_FREE_PROTOCOL_MESSAGE(msg);
					break;
				}

				case CW_ERROR_INTERRUPTED: 
				{
					gWTPRetransmissionCount++;
					goto cw_continue_external_loop;
					break;
				}	
				default:
				{
					CWErrorHandleLast();
					CWDebugLog("Failure");
					goto cw_failure;
					break;
				}
			}
		}
		
cw_continue_external_loop:
		CWDebugLog("Retransmission time is over");
			
	}

	/* too many retransmissions */
	return CWErrorRaise(CW_ERROR_NEED_RESOURCE, "Peer Dead");
	
cw_success:	
	for(i = 0; i < fragmentsNum; i++) {
		CW_FREE_PROTOCOL_MESSAGE(messages[i]);
	}
	
	CW_FREE_OBJECT(messages);
	CW_FREE_PROTOCOL_MESSAGE(msg);
	
	return CW_TRUE;
	
cw_failure:
	if(messages != NULL) {
		for(i = 0; i < fragmentsNum; i++) {
			CW_FREE_PROTOCOL_MESSAGE(messages[i]);
		}
		CW_FREE_OBJECT(messages);
	}
    
    CW_FREE_PROTOCOL_MESSAGE(msg);
	CWDebugLog("Failure");
	return CW_FALSE;
}


int main (int argc, const char * argv[]) 
{
	pid_t pid;
	
	if (argc <= 1)
		printf("Usage: WTP working_path\n");

#ifdef RTK_CAPWAP
	/* Init for RTK AP */
	if (!cwRtkWtpInit()) {		
		printf("cwRtkWtpInit() error!\n");
		exit(0);
	}

	/* Check mib */
	if (!cwRtkWtpSystemEnabled()) {		
		printf("WTP is not enabled. Exit!\n");
		exit(0);
	}

#if 0
	if (cwRtkWtpGetAcIp() == 0) {
		printf("AC IP = 0.0.0.0. Exit!\n");
		exit(0);
	}
#endif

	if (cwRtkWtpGetWtpId() == 0) {
		printf("Invalid WTP ID (0). Exit\n");
		exit(0);
	}
#endif

	/* Daemon Mode */
	if ((pid = fork()) < 0)
		exit(1);
	else if (pid != 0)
		exit(0);
	else {
		setsid();
		if (chdir(argv[1]) != 0){
			printf("chdir Fail\n");
			exit(1);
		}

//		fclose(stdout);
	}	

	CWStateTransition nextState = CW_ENTER_DISCOVERY;
	if (argc>2)  CWLogInitFile(argv[2]);
	else
	{
		CWLogInitFile(WTP_LOG_FILE_NAME);
#ifdef RTK_SMART_ROAMING
		CWRoamLogInitFile(SMART_ROAMING_WTP_LOG_FILE_NAME);
#endif
	}


#ifndef CW_SINGLE_THREAD
	CWDebugLog("Use Threads");
#else
	CWDebugLog("Don't Use Threads");
#endif


	CWErrorHandlingInitLib();

	if(!CWParseSettingsFile()){
		CWLog("Can't start WTP");
		exit(1);
	}


	/* Capwap receive packets list */
	if (!CWErr(CWCreateSafeList(&gPacketReceiveList)))
	{
		CWLog("Can't start WTP");
		exit(1);
	}
	
#ifdef KM_BY_AC
	/* Capwap receive frame list */
	if (!CWErr(CWCreateSafeList(&gFrameList)))
	{
		CWLog("Can't start WTP");
		exit(1);
	}
#endif

	{
		gProgName=argv[0];
	}

	// destroy old process and create a PID file
	{
		int pid_fd;
		FILE *fp;
		char line[20], pid_file_name[100];
		pid_t pid;

		sprintf(pid_file_name,"/etc/capwap/WTP.pid");

		if ((fp = fopen(pid_file_name, "r")) != NULL) {
			fgets(line, sizeof(line), fp);
			if (sscanf(line, "%d", &pid)) {
				if (pid > 1)
					kill(pid, SIGTERM);
			}
			fclose(fp);
		}
		pid_fd = pidfile_acquire(pid_file_name);
		if (pid_fd < 0)
			return 0;

		pidfile_write_release(pid_fd);
	}

	CWCreateThreadMutex(&gInterfaceMutex);
	CWSetMutexSafeList(gPacketReceiveList, &gInterfaceMutex);
#ifdef KM_BY_AC
	CWSetMutexSafeList(gFrameList, &gInterfaceMutex);
#endif
	CWCreateThreadCondition(&gInterfaceWait);
	CWSetConditionSafeList(gPacketReceiveList, &gInterfaceWait);
#ifdef KM_BY_AC
	CWSetConditionSafeList(gFrameList, &gInterfaceWait);
#endif
	CWLog("Starting WTP...");
	
	CWRandomInitLib();

	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);

//printf("Init Timer\n");
	if (timer_init() == 0) {
//printf("timer_init failed\n");
		CWLog("Can't init timer module");
		exit(1);
	}

#ifdef CW_NO_DTLS
	if( !CWErr(CWWTPLoadConfiguration()) )
#else
	if( !CWErr(CWSecurityInitLib())	|| !CWErr(CWWTPLoadConfiguration()) )
#endif
	{
		CWLog("Can't start WTP");
		exit(1);
	}
	CWDebugLog("Init WTP Radio Info");
	if(!CWWTPInitConfiguration())
	{
		CWLog("Error Init Configuration");
		exit(1);
	}

	/* if AC address is given jump Discovery and use this address for Joining */
	if(gWTPForceACAddress != NULL)	nextState = CW_ENTER_JOIN;

	/* start CAPWAP state machine */	
	CW_REPEAT_FOREVER {

		switch(nextState) {
			case CW_ENTER_DISCOVERY:
				nextState = CWWTPEnterDiscovery();
				break;
			case CW_ENTER_SULKING:
				nextState = CWWTPEnterSulking();
				break;
			case CW_ENTER_JOIN:
				nextState = CWWTPEnterJoin();
				break;
			case CW_ENTER_CONFIGURE:
				nextState = CWWTPEnterConfigure();
				break;	
			case CW_ENTER_DATA_CHECK:
				nextState = CWWTPEnterDataCheck();
				break;	
			case CW_ENTER_RUN:
				nextState = CWWTPEnterRun();
				break;
 			case CW_ENTER_IMAGE_DATA:
				nextState = CWWTPEnterImageData(gJoinResponseVal.image_identifier.imageIdentifier);
				break;
 			case CW_ENTER_RESET:
				CWLog("\n");
				CWLog("######### WTP enters in RESET State #########");
				/*
				 * CWStopHeartbeatTimer();
				 * CWStopNeighborDeadTimer();
				 * CWNetworkCloseSocket(gWTPSocket);
				 * CWSecurityDestroySession(gWTPSession);
				 * CWSecurityDestroyContext(gWTPSecurityContext);
				 * gWTPSecurityContext = NULL;
				 * gWTPSession = NULL;
				 */
				CWLog("%s %d calling CWWTPCloseControlSession", __FUNCTION__, __LINE__);
				CWWTPCloseControlSession();
#ifdef KM_BY_AC		
				CWWTPCloseDataSession();
#endif
				if (gWillReboot) {
					CWLog("!!!!WTP RESET(Closed) !!!!"); 
					nextState = CW_QUIT;
				} else {
					CWLog("!!!!WTP RESET(Back to Discovery) !!!!"); 
					nextState = CW_ENTER_DISCOVERY;
				}
				break;
			case CW_QUIT:
				CWWTPDestroy();
				return 0;
		}
	}
}

__inline__ unsigned int CWGetSeqNum() {
	static unsigned int seqNum = 0;
	
	if (seqNum==CW_MAX_SEQ_NUM) seqNum=0;
	else seqNum++;
	return seqNum;
}

__inline__ int CWGetFragmentID() {
	static int fragID = 0;
	return fragID++;
}


/* 
 * Parses config file and inits WTP configuration.
 */
CWBool CWWTPLoadConfiguration() 
{
	unsigned int i, j;
	CWLog("WTP Loads Configuration");

	/* get saved preferences */
	if(!CWErr(CWParseConfigFile())) {
		CWLog("Can't Read Config File");
		exit(1);
	}

#ifdef RTK_CAPWAP
	for(j=0;j<10;j++)
	{
		i = cwRtkWtpGetAcIp();
		if(i!=0)
			break;
		sleep(1);
	}

	if (i!=0) 
	{
		if(gCWACCount != 0) {
			CW_FREE_OBJECTS_ARRAY(gCWACAddresses, gCWACCount);
		}
		gCWACCount = 1;	
		CW_CREATE_ARRAY_ERR(gCWACList, 
					gCWACCount,
					CWACDescriptor,
					return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

		CW_CREATE_STRING_ERR(gCWACList[0].address, 16, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		sprintf(gCWACList[0].address, "%d.%d.%d.%d", (i>>24)&0xff, (i>>16)&0xff, (i>>8)&0xff, i&0xff);
		CWLog("Got Rtk AC IP: %s", gCWACList[0].address);
	} else
#endif
	{
		if(gCWACCount == 0) 
			return CWErrorRaise(CW_ERROR_NEED_RESOURCE, "No AC Configured");
		
		CW_CREATE_ARRAY_ERR(gCWACList, 
				    gCWACCount,
				    CWACDescriptor,
				    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

		for(i = 0; i < gCWACCount; i++) {
			CWDebugLog("Init Configuration for AC at %s", gCWACAddresses[i]);
			CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[i].address, gCWACAddresses[i],
							 return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		}
		CW_FREE_OBJECTS_ARRAY(gCWACAddresses, gCWACCount);
	}

	return CW_TRUE;
}

void CWWTPDestroy() {
	int i;
	
	CWLog("Destroy WTP");
	
	for(i = 0; i < gCWACCount; i++) {
		CW_FREE_OBJECT(gCWACList[i].address);
	}
	
	timer_destroy();

	CW_FREE_OBJECT(gCWACList);
	CW_FREE_OBJECT(gRadiosInfo.radiosInfo);
}

CWBool CWWTPInitConfiguration() {
	CWDebugLog("CWWTPInitConfiguration"); 
	int i;

	//Generate 128-bit Session ID,
	initWTPSessionID(&gWTPSessionID[0]);

	CWWTPResetRebootStatistics(&gWTPRebootStatistics);

	gRadiosInfo.radioCount = cwRtkWtpGetMaxRadios();

	CW_CREATE_ARRAY_ERR(gRadiosInfo.radiosInfo, gRadiosInfo.radioCount, CWWTPRadioInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	gRadiosInfo.radiosInfo[0].radioID= 0;

	/* gRadiosInfo.radiosInfo[0].numEntries = 0; */
	gRadiosInfo.radiosInfo[0].decryptErrorMACAddressList = NULL;
	gRadiosInfo.radiosInfo[0].reportInterval= CW_REPORT_INTERVAL_DEFAULT;
	gRadiosInfo.radiosInfo[0].adminState= CW_ENABLED;
	gRadiosInfo.radiosInfo[0].adminCause= AD_NORMAL;
	gRadiosInfo.radiosInfo[0].operationalState= CW_ENABLED;
	gRadiosInfo.radiosInfo[0].operationalCause= OP_NORMAL;
	gRadiosInfo.radiosInfo[0].TxQueueLevel= 0;
	gRadiosInfo.radiosInfo[0].wirelessLinkFramesPerSec= 0;

	CWWTPResetRadioStatistics(&(gRadiosInfo.radiosInfo[0].statistics));
#ifndef RTK_CAPWAP
	if(!CWWTPInitBinding(0)) {return CW_FALSE;}
#endif
	for (i=1; i<gRadiosInfo.radioCount; i++)
	{
		gRadiosInfo.radiosInfo[i].radioID= i;
		/* gRadiosInfo.radiosInfo[i].numEntries = 0; */
		gRadiosInfo.radiosInfo[i].decryptErrorMACAddressList = NULL;
		gRadiosInfo.radiosInfo[i].reportInterval= CW_REPORT_INTERVAL_DEFAULT;
		/* Default value for CAPWA� */
		gRadiosInfo.radiosInfo[i].adminState= CW_ENABLED; 
		gRadiosInfo.radiosInfo[i].adminCause= AD_NORMAL;
		gRadiosInfo.radiosInfo[i].operationalState= CW_DISABLED;
		gRadiosInfo.radiosInfo[i].operationalCause= OP_NORMAL;
		gRadiosInfo.radiosInfo[i].TxQueueLevel= 0;
		gRadiosInfo.radiosInfo[i].wirelessLinkFramesPerSec= 0;
		CWWTPResetRadioStatistics(&(gRadiosInfo.radiosInfo[i].statistics));
#ifndef RTK_CAPWAP
		if(!CWWTPInitBinding(i)) {return CW_FALSE;}
#endif
	}

	return CW_TRUE;
}
void RTKCleanACInfo(CWACInfoValues *gACInfoPtr)
{	
	int i;
	if(gACInfoPtr!=NULL)
	{
		//CW_FREE_OBJECT(gACInfoPtr->name);
		if(gACInfoPtr->name)
		{
			//printf("%s:%d##free gACInfoPtr->name!\n",__FUNCTION__,__LINE__);
			CW_FREE_OBJECT(gACInfoPtr->name);
		}

		if(gACInfoPtr->IPv4AddressesCount>0)
		{
			//printf("%s:%d##free gACInfoPtr->IPv4Addresses!\n",__FUNCTION__,__LINE__);
			CW_FREE_OBJECT(gACInfoPtr->IPv4Addresses);
		}

		if(gACInfoPtr->IPv6AddressesCount > 0) 
		{
			//printf("%s:%d##free gACInfoPtr->IPv6Addresses!\n",__FUNCTION__,__LINE__);
			CW_FREE_OBJECT(gACInfoPtr->IPv6Addresses);	
		}
		
		//printf("vendorInfosCount=%d\n",gACInfoPtr->vendorInfos.vendorInfosCount);
		
        	for(i = 0; i < gACInfoPtr->vendorInfos.vendorInfosCount; i++) 
			CW_FREE_OBJECT(gACInfoPtr->vendorInfos.vendorInfos[i].valuePtr);
        
        	CW_FREE_OBJECT(gACInfoPtr->vendorInfos.vendorInfos);
		

		if(gACInfoPtr->ACIPv4ListInfo.ACIPv4ListCount>0)
		{
			//printf("%s:%d##free gACInfoPtr->ACIPv4ListInfo.ACIPv4List!\n",__FUNCTION__,__LINE__);
			CW_FREE_OBJECT(gACInfoPtr->ACIPv4ListInfo.ACIPv4List);
		}

		if(gACInfoPtr->ACIPv6ListInfo.ACIPv6ListCount>0)
		{
			//printf("%s:%d##free gACInfoPtr->ACIPv6ListInfo.ACIPv6List!\n",__FUNCTION__,__LINE__);
			CW_FREE_OBJECT(gACInfoPtr->ACIPv6ListInfo.ACIPv6List);
		}

		//printf("%s:%d##free gACInfoPtr!\n",__FUNCTION__,__LINE__);
		CW_FREE_OBJECT(gACInfoPtr);	
	}
}

extern int gBreakRcvLoop;

void CWWTPCloseControlSession()
{
	CWLog("%s", __FUNCTION__);
#ifndef CW_NO_DTLS
	CWThread cur_tid=gettid();
#endif

	gBreakRcvLoop = CW_TRUE;
	 while(dtls_thread_exit == 0) {
	  printf("WTP Wait thread to exit...\n"); 
	  sleep(1);
	 }

	CWNetworkCloseSocket(gWTPSocket);
	CWLockSafeList(gPacketReceiveList);
	CWCleanSafeList(gPacketReceiveList, free);
	CWUnlockSafeList(gPacketReceiveList);


#ifndef CW_NO_DTLS
	CWSecurityDestroySession(gWTPSession);
	gWTPSession = NULL;
	
	CWSecurityDestroyContext(gWTPSecurityContext);
	gWTPSecurityContext = NULL;

	//printf("cur_tid = %d\n", cur_tid);
    //ERR_remove_state(gThreadReceiveFrame);
    ERR_remove_state(cur_tid);
    ENGINE_cleanup();
	CONF_modules_unload(1);
    ERR_free_strings();
    //EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
#endif


	RTKCleanACInfo(gACInfoPtr);
}

#ifdef KM_BY_AC		
void CWWTPCloseDataSession()
{
	CWLog("%s", __FUNCTION__);
	CWNetworkCloseSocket(gWTPDataSocket);
	//CWDestroyThread(gThreadReceiveDataFrame);
}
#endif


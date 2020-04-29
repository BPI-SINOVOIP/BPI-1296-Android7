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
#include <sys/socket.h>
#include <sys/types.h>  
#include <linux/if_ether.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include "CWWTP.h"
#include "CWVendorPayloads.h"
#include "common.h"
#include "ieee802_11_defs.h"
#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

#include "WTPRtkSystemInteraction.h"

#ifdef RTK_AUTO_AC
extern CWBool breakACInfo;
#endif

static CWBool CWWTPManageGenericRunMessage(CWProtocolMessage *orgMsgPtr);

CWBool CWWTPCheckForBindingFrame();

#ifdef RTK_SMART_ROAMING
static CWBool CWParseWTPEventResponseMessage(CWControlHeaderValues *controlVal,
				      int len);

CWBool CWSaveWTPEventResponseMessage(void *WTPEventResp);
#endif

CWBool CWAssembleEchoRequest(CWProtocolMessage **messagesPtr,
			     int *fragmentsNumPtr,
			     int PMTU,
			     int seqNum,
			     CWList msgElemList);

CWBool CWParseConfigurationUpdateRequest (unsigned char *msg,
										  int len,
										  CWProtocolConfigurationUpdateRequestValues *valuesPtr, 
										  int *updateRequestType);

void CWSaveConfigurationUpdateRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr,
										CWProtocolResultCode* resultCode,
										int updateRequestType, 
										char *newImageVersion);

CWBool CWAssembleConfigurationUpdateResponse(CWProtocolMessage **messagesPtr,
					     int *fragmentsNumPtr,
					     int PMTU,
					     int seqNum,
					     CWProtocolResultCode resultCode,
						 CWProtocolConfigurationUpdateRequestValues values);

CWBool CWSaveClearConfigurationRequest(CWProtocolResultCode* resultCode);

CWBool CWAssembleClearConfigurationResponse(CWProtocolMessage **messagesPtr,
					    int *fragmentsNumPtr,
					    int PMTU,
					    int seqNum,
					    CWProtocolResultCode resultCode);

void CWConfirmRunStateToACWithEchoRequest();

CWBool CWAssembleImageDataRequestByWTP(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList cw_list);


/* Note: msg of CWParseImageDataResponseByWTP is the position of Capwap header. This is different from the other parsing functions here. */
CWBool CWParseImageDataResponseByWTP (unsigned char *msg, int len, int seqNum, void *values);
CWBool CWSaveImageDataResponseByWTP(void *values);
CWBool CWParseImageDataRequestByWTP(unsigned char *msg, int len, CWBool needSave, CWBool (*save_data)(const CWProtocolImageDataValues *, CWBool *), CWProtocolResultCode *resultCode);
CWBool CWAssembleImageDataResponseByWTP(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode);


CWBool CWParseResetRequest (unsigned char *msg, int len,
						  CWProtocolImageIdentifierValues *image_identifier, CWProtocolResultCode *resultCode);

CWBool CWAssembleResetResponse(CWProtocolMessage **messagesPtr,
					     int *fragmentsNumPtr,
					     int PMTU,
					     int seqNum,
					     CWProtocolResultCode resultCode);

#ifdef RTK_SMART_ROAMING
CWBool CWParseStationConfigurationRequest (unsigned char *msg, int len, CWProtocolResultCode *resultCode);
CWBool CWAssembleStationConfigurationResponse(CWProtocolMessage **messagesPtr,
					      int *fragmentsNumPtr,
					      int PMTU,
					      int seqNum,
					      CWProtocolResultCode resultCode);
#endif

#ifdef KM_BY_AC
void CWParseWLANConfigurationRequest (unsigned char *msg, int len, CWProtocolWlanConfigurationRequestValues *pCfgReqVal);
void CWSaveWLANConfigurationRequest (const CWProtocolWlanConfigurationRequestValues *pCfgReqVal, 
										CWProtocolWlanConfigurationResponseValues *pCfgRspVal);

CWBool CWAssembleWLANConfigurationResponse(CWProtocolMessage **messagesPtr,
					      int *fragmentsNumPtr,
					      int PMTU,
					      int seqNum, 
						const CWProtocolWlanConfigurationResponseValues *cfgRspVal);
#endif

#ifdef RTK_CAPWAP
CWBool CWParseRtkWTPStatusRequest (unsigned char *msg, int len, CWProtocolRtkWTPStatusRequestValues *pStatusReq);
CWBool CWAssembleRtkWTPStatusResponse(CWProtocolMessage **messagesPtr,
						 int *fragmentsNumPtr,
						 int PMTU,
						 int seqNum,
						 const CWProtocolRtkWTPStatusResponseValues *wtpResponse);
#endif

void CWWTPKeepAliveDataTimerExpiredHandler(void *arg);

#ifdef RTK_SMART_ROAMING
unsigned int gPingSpeed;
#endif

CWTimerID gCWHeartBeatTimerID;
#ifdef KM_BY_AC
CWTimerID gCWKeepAliveTimerID;
#endif
CWTimerID gCWNeighborDeadTimerID;
CWBool gNeighborDeadTimerSet=CW_FALSE;
	
int gEchoInterval = CW_ECHO_INTERVAL_DEFAULT;
#ifdef KM_BY_AC
int gDataChannelKeepAlive = CW_DATA_CHANNEL_KEEP_ALIVE_DEFAULT;
#endif


int dtls_thread_exit;

int wtpInRunState=0;


/*
void handle_quit(int signo)
{
    printf("handle sig %d \n", signo);
    pthread_exit(NULL);
}
*/
/* 
 * Manage DTLS packets.
 */
CW_THREAD_RETURN_TYPE CWWTPReceiveDtlsPacket(void *arg) {

	int 			readBytes;
	unsigned char	buf[CW_BUFFER_SIZE];
	CWSocket 		sockDTLS = (CWSocket)arg;
	CWNetworkLev4Address	addr;
	char* 			pData;

//	signal(SIGQUIT, handle_quit);
	dtls_thread_exit = 0;
	
	CWLog("%s started", __FUNCTION__);
//	printf("\n%s cur_tid=%d\n",__FUNCTION__, gettid()); 
	CW_REPEAT_FOREVER 
	{
		if(!CWErr(CWNetworkReceiveUnsafe(sockDTLS,
						 buf, 
						 CW_BUFFER_SIZE - 1,
						 0,
						 &addr,
						 &readBytes))) {
						 
			CWErrorCode ec = CWErrorGetLastErrorCode();
			CWLog("CWNetworkReceiveUnsafe error: %d (%s)", ec, __FUNCTION__);
			if (ec == CW_ERROR_INTERRUPTED)
				continue;

			break;
		}
//printf("(%s) WTP recv thread: got message from AC. readBytes=%d\n", __FUNCTION__, readBytes);
//int i;
//for (i=0; i<readBytes; i++) printf("%02x", buf[i]);
//printf("\n");

		CWLog("WTP recv thread: got message from AC. (Will pass to another thread by queue and notify it by signal) %s\n", __FUNCTION__);
		/* Clone data packet */
		CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWLog("Out Of Memory"); return NULL; });
		memcpy(pData, buf, readBytes);

		CWLockSafeList(gPacketReceiveList);
		CWAddElementToSafeListTailwitDataFlag(gPacketReceiveList, pData, readBytes,CW_FALSE);
		CWUnlockSafeList(gPacketReceiveList);		
	}

	/* 2014 Babylon:
	 *		since packet receving thread is dead, 
	 *		should wake up packet handling thread, and should make it go to reset state.
	 */
	CWLog("(%s) WTP Packet RX Thread Ended.", __FUNCTION__);
#ifdef RTK_AUTO_AC
	char cmd[128];
	if (breakACInfo){
		CWLog("!!!!Kill AC AACWTP in WTPRunState.c!!!!\n");
#ifdef NOT_DELETE_LOG
		sprintf(cmd, "killall -9 AC AACWTP;AC /etc/capwap;AACWTP /etc/capwap");
#else
		sprintf(cmd, "killall -9 AC AACWTP;rm /var/log/ac.log.txt;rm /var/log/smart_roaming.ac.log.txt;rm /var/log/aac.wtp.log.txt");
#endif
	    system(cmd);
		printf("!!!!Finish kill AC AACWTP in WTPRunState.c!!!!\n");
	}
	else{
		printf("!!!!Restart AC AACWTP in WTPRunState.c!!!!\n"); 
		//sprintf(cmd, "killall -9 AC AACWTP;AC /etc/capwap;AACWTP /etc/capwap");
		if(gMesh_portal) {
#ifdef NOT_DELETE_LOG
			sprintf(cmd, "killall -9 AC AACWTP;AC /etc/capwap;AACWTP /etc/capwap");
#else
        	sprintf(cmd, "killall -9 AC AACWTP;rm /var/log/ac.log.txt;rm /var/log/smart_roaming.ac.log.txt;rm /var/log/aac.wtp.log.txt;AC /etc/capwap;AACWTP /etc/capwap");
#endif
        	CWLog("killall -9 AC AACWTP;AC /etc/capwap;AACWTP /etc/capwap");
		}else {
#ifdef NOT_DELETE_LOG
			sprintf(cmd, "killall -9 AC AACWTP");
#else
        	sprintf(cmd, "killall -9 AC AACWTP;rm /var/log/ac.log.txt;rm /var/log/smart_roaming.ac.log.txt;rm /var/log/aac.wtp.log.txt");
#endif
        	CWLog("killall -9 AC AACWTP");			
		}
		system(cmd);
		printf("!!!!Finish restart AC AACWTP in WTPRunState.c!!!!\n"); 
	}
#ifdef NOT_DELETE_LOG
	CWLog("killall -9 WTP");
	sprintf(cmd, "killall -9 WTP");
#else
	CWLog("killall -9 WTP;rm /var/log/wtp.log.txt;rm /var/log/smart_roaming.wtp.log.txt;WTP /etc/capwap");
	CWLogCloseFile();
	sprintf(cmd, "killall -9 WTP;rm /var/log/wtp.log.txt;rm /var/log/smart_roaming.wtp.log.txt;WTP /etc/capwap");
#endif
	system(cmd);
#endif
	gGotoReset = CW_TRUE;
	CWLockSafeList(gPacketReceiveList);	
	CWCleanSafeList(gPacketReceiveList, free);
	if(wtpInRunState)
	CWSignalElementSafeList(gPacketReceiveList);
	CWUnlockSafeList(gPacketReceiveList);		
	//ERR_remove_state(gThreadReceiveFrame);
#ifndef CW_NO_DTLS
	ERR_remove_state(gettid());
	//ERR_remove_thread_state(&cur_tid);
	ENGINE_cleanup();
	CONF_modules_unload(1);
	ERR_free_strings();
	//EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
#endif
	dtls_thread_exit = 1;
	CWLog("(%s) WTP Packet RX Thread Ended.", __FUNCTION__);

	return NULL;
}



extern int gRawSock;
/* 
 * Manage data packets.
 */

#define HLEN_80211	24
int isEAPOL_Frame( unsigned char *buf, int len){
	unsigned char rfc1042_header[6] = { 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00 };
	int i;
	
	for(i=0; i<6; i++)if(rfc1042_header[i]!=buf[i + HLEN_80211])return 0;
	return 1;
}

#ifdef KM_BY_AC
CW_THREAD_RETURN_TYPE CWWTPReceiveDataPacket(void *arg) {

	int 			n,readBytes;
	unsigned char	buf[CW_BUFFER_SIZE];
	struct sockaddr_ll 	rawSockaddr;	
	CWSocket 		sockDTLS = (CWSocket)arg;
	CWNetworkLev4Address	addr;
	CWList 			fragments = NULL;
	CWProtocolMessage 	msgPtr;
	CWBool 			dataFlag = CW_TRUE;

    memset(&rawSockaddr, 0, sizeof(rawSockaddr));
	
	rawSockaddr.sll_family		= AF_PACKET;
	rawSockaddr.sll_protocol	= htons(ETH_P_ALL);
	rawSockaddr.sll_ifindex		= if_nametoindex(gRadioInterfaceName_0);
	rawSockaddr.sll_pkttype		= PACKET_OTHERHOST;
	rawSockaddr.sll_halen		= ETH_ALEN;	

//printf("babylon test: %s started\n", __FUNCTION__);
	CW_REPEAT_FOREVER 
	{
		if(!CWErr(CWNetworkReceiveUnsafe(gWTPDataSocket,
						buf, 
						CW_BUFFER_SIZE - 1,
						0,
						&addr,
						&readBytes))) {

			if (CWErrorGetLastErrorCode() == CW_ERROR_INTERRUPTED)
				continue;
//printf("babylon test: CWNetworkReceiveUnsafe error: %s\n", __FUNCTION__);
			break;
		}
//printf("babylon test: CWNetworkReceiveUnsafe error: %s\n", __FUNCTION__);
		msgPtr.msg = NULL;
		msgPtr.offset = 0;

		if(!CWProtocolParseFragment(buf, readBytes, &fragments, &msgPtr, &dataFlag, NULL)) {
			if(CWErrorGetLastErrorCode()){
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
			}
		}else{
			if (msgPtr.data_msgType == CW_DATA_MSG_KEEP_ALIVE_TYPE) {

				unsigned char *valPtr=NULL;
				unsigned short int elemType = 0;
				unsigned short int elemLen = 0;

				CWDebugLog("Got KeepAlive len:%d from AC",msgPtr.offset);
				msgPtr.offset=0;				
				CWParseFormatMsgElem(&msgPtr, &elemType, &elemLen);
				valPtr = CWParseMsgElemSessionID(&msgPtr, elemLen);
	

			}else if (msgPtr.data_msgType == CW_IEEE_802_3_FRAME_TYPE) {

				CWDebugLog("Got 802.3 len:%d from AC",msgPtr.offset);
				
				/*MAC - begin*/
				rawSockaddr.sll_addr[0]  = msgPtr.msg[0];		
				rawSockaddr.sll_addr[1]  = msgPtr.msg[1];		
				rawSockaddr.sll_addr[2]  = msgPtr.msg[2];
				rawSockaddr.sll_addr[3]  = msgPtr.msg[3];
				rawSockaddr.sll_addr[4]  = msgPtr.msg[4];
				rawSockaddr.sll_addr[5]  = msgPtr.msg[5];
				/*MAC - end*/
				rawSockaddr.sll_addr[6]  = 0x00;/*not used*/
				rawSockaddr.sll_addr[7]  = 0x00;/*not used*/
				
				rawSockaddr.sll_hatype   = htons(msgPtr.msg[12]<<8 | msgPtr.msg[13]);
#ifdef KM_BY_AC
				n = sendto(gRawSock,msgPtr.msg ,msgPtr.offset,0,(struct sockaddr*)&rawSockaddr, sizeof(rawSockaddr));
#endif		
			}else if(msgPtr.data_msgType == CW_IEEE_802_11_FRAME_TYPE) {
								
				struct ieee80211_hdr *hdr;
				u16 fc;
				hdr = (struct ieee80211_hdr *) msgPtr.msg;
				fc = le_to_host16(hdr->frame_control);
	
				if( WLAN_FC_GET_TYPE(fc) == WLAN_FC_TYPE_MGMT || isEAPOL_Frame(msgPtr.msg,msgPtr.offset) ){
					CWDebugLog("Got 802.11 Management Packet (stype=%d) from AC(hostapd) len:%d",WLAN_FC_GET_STYPE(fc),msgPtr.offset);
#ifdef SOFTMAC
					CWWTPsend_data_to_hostapd(msgPtr.msg, msgPtr.offset);
#else
					// TODO:
#endif
				}else if( WLAN_FC_GET_TYPE(fc) == WLAN_FC_TYPE_DATA ){
										
					if(  WLAN_FC_GET_STYPE(fc) == WLAN_FC_STYPE_NULLFUNC ){
						
						CWDebugLog("Got 802.11 Data Packet (stype=%d) from AC(hostapd) len:%d",WLAN_FC_GET_STYPE(fc),msgPtr.offset);
#ifdef SOFTMAC
						CWWTPsend_data_to_hostapd(msgPtr.msg, msgPtr.offset);
#else
						// TODO:
#endif				
					}else{
						
						CWDebugLog("Got 802.11 Data Packet (stype=%d) from AC(hostapd) len:%d",WLAN_FC_GET_STYPE(fc),msgPtr.offset);
#ifdef KM_BY_AC
						CWWTPSendFrame(msgPtr.msg, msgPtr.offset);
#endif		
					}
					
				}else{
					CWLog("Control/Unknow Type type=%d",WLAN_FC_GET_TYPE(fc));
				}
			
				
			}else{
				CWLog("Unknow data_msgType");
			}
			CW_FREE_PROTOCOL_MESSAGE(msgPtr);
		}
	}

	return NULL;
}
#endif
/* 
 * Manage Run State.
 */
CWBool gFirstRunMsg = CW_FALSE;
extern int gRawSock;


CWStateTransition CWWTPEnterRun() 
{
	int k;

printf("(%d)%s\n", rtk_getpid(), __FUNCTION__);
	CWLog("\n");
	CWLog("######### WTP enters in RUN State #########");

#ifdef KM_BY_AC	
	CWWTPKeepAliveDataTimerExpiredHandler(NULL);
#endif


    CWCreateThreadMutex(&gPendingRequestMutex);
	for (k = 0; k < MAX_PENDING_REQUEST_MSGS; k++)
		CWInitPendingMsgBox(gPendingRequestMsgs + k);
	for (k = 0; k < SEQUENCE_ARRAY_SIZE; k++)
		gRcvSeqNum[k] = k+1;

	if (!CWErr(CWStartHeartbeatTimer())) {
        CWDestroyThreadMutex(&gPendingRequestMutex);
//printf("%s %d goto CW_ENTER_RESET\n", __FUNCTION__, __LINE__);
		return CW_ENTER_RESET;
	}
	wtpInRunState=1;
	gFirstRunMsg = CW_TRUE;


#ifdef RTK_SMART_ROAMING
	/* thread to communicate with wlan driver to receive wlan info */
	CWThread thread_receiveFrame;
	if(!CWErr(CWCreateThread(&thread_receiveFrame, CWWTPRtkReceiveFrame, NULL))) {
        CWDestroyThreadMutex(&gPendingRequestMutex);
		CWRoamLog("Error starting Thread that receive frame from driver");
		exit(1);
	}
	CWRoamDebugLog("create thread_receiveFrame");

#ifdef NEW_CHECKINTERNET
	/* timer handler to collect ping reply time */
	CWRoamDebugLog("create InternetCheck handler");
	CWWTPRtkCheckInternet();
#else
	/* thread to collect ping reply time */
	CWThread thread_checkInternet;
	if(!CWErr(CWCreateThread(&thread_checkInternet, CWWTPRtkCheckInternet, NULL))) {
        CWDestroyThreadMutex(&gPendingRequestMutex);
		CWRoamLog("Error starting Thread that check ping reply time");
		exit(1);
	}
	CWRoamDebugLog("create thread_checkInternet");
#endif
#endif

	CW_REPEAT_FOREVER
	{
		struct timespec timenow;
		CWBool bReceivePacket = CW_FALSE;
#ifdef KM_BY_AC
		CWBool bReveiveBinding = CW_FALSE;
#endif

		/* Wait packet */
		timenow.tv_sec = time(0) + CW_NEIGHBORDEAD_RESTART_DISCOVERY_DELTA_DEFAULT;	 /* greater than NeighborDeadInterval */
		timenow.tv_nsec = 0;

		CWThreadMutexLock(&gInterfaceMutex);

		/*
		 * if there are no frames from stations
		 * and no packets from AC...
		 */

		if ((CWGetCountElementFromSafeList(gPacketReceiveList) == 0) 

#ifdef KM_BY_AC
					&& (CWGetCountElementFromSafeList(gFrameList) == 0)
#endif
		) {
			/*
			 * ...wait at most 4 mins for a frame or packet.
			 */

			if (!CWErr(CWWaitThreadConditionTimeout(&gInterfaceWait, &gInterfaceMutex, &timenow))) {

			
				if (CWErrorGetLastErrorCode() == CW_ERROR_TIME_EXPIRED
					&& CWGetCountElementFromSafeList(gPacketReceiveList) == 0
#ifdef KM_BY_AC
					&& CWGetCountElementFromSafeList(gFrameList) == 0
#endif
					)	{
					CWLog("No Message from AC for a long time... restart Discovery State");
					gFirstRunMsg = CW_FALSE;					
					CWThreadMutexUnlock(&gInterfaceMutex);
					break;
				}
				CWThreadMutexUnlock(&gInterfaceMutex);
				continue;
			}

		}

		/* 2014 Babylon:
		 *		since packet receving thread is dead, 
		 *		should wake up packet handling thread, and should make it go to reset state.
		 */
		if (gGotoReset) {
			CWLog("WTP Should goto RESET (and then Discovery) State");
			gFirstRunMsg = CW_FALSE;			
			CWThreadMutexUnlock(&gInterfaceMutex);
			break;
		}

		CWLog("WTP thread waked up to handle incoming message\n");
		bReceivePacket = ((CWGetCountElementFromSafeList(gPacketReceiveList) != 0) ? CW_TRUE : CW_FALSE);
#ifdef KM_BY_AC
		bReveiveBinding = ((CWGetCountElementFromSafeList(gFrameList) != 0) ? CW_TRUE : CW_FALSE);
#endif
		CWThreadMutexUnlock(&gInterfaceMutex);

		if (bReceivePacket) {

			CWProtocolMessage msg;

			msg.msg = NULL;
			msg.offset = 0;

			if (!(CWReceiveMessage(&msg))) {

				CW_FREE_PROTOCOL_MESSAGE(msg);
				CWLog("Failure Receiving Response");
                CWDestroyThreadMutex(&gPendingRequestMutex);
				wtpInRunState=0;
				gFirstRunMsg = CW_FALSE;

#if 1
				CWLog("%s %d goto CW_ENTER_RESET", __FUNCTION__, __LINE__);
				return CW_ENTER_RESET;
#else
				
				CWWTPCloseControlSession();
#ifdef KM_BY_AC		
				CWWTPCloseDataSession();
#endif
				return CW_ENTER_DISCOVERY;
#endif
			}

			if (!CWErr(CWWTPManageGenericRunMessage(&msg))) {

				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) {

					/* Log and ignore message */
					CWErrorHandleLast();
					CWLog("--> Received something different from a valid Run Message");
				} 
				else {
					CW_FREE_PROTOCOL_MESSAGE(msg);
					CWLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
                    CWDestroyThreadMutex(&gPendingRequestMutex);
					wtpInRunState=0;
					gFirstRunMsg = CW_FALSE;
					return CW_ENTER_RESET;
				}
			}			
            CW_FREE_PROTOCOL_MESSAGE(msg);
			gFirstRunMsg = CW_FALSE;
		}
#ifdef KM_BY_AC
		if (bReveiveBinding)
			CWWTPCheckForBindingFrame();
#endif
	}

#ifdef RTK_SMART_ROAMING
	struct nlmsghdr *nlh;
	struct iovec iov;	 
	struct msghdr msg;

	//Fill the netlink message header 	 
	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(CW_MAX_STR_BUFFER_SIZE));
	memset(nlh, 0, NLMSG_SPACE(CW_MAX_STR_BUFFER_SIZE));
	nlh->nlmsg_len = NLMSG_SPACE(CW_MAX_STR_BUFFER_SIZE);
	nlh->nlmsg_pid = rtk_getpid(); //sending process ID
	nlh->nlmsg_flags = 0;

	strcpy(NLMSG_DATA(nlh), "E");

	//iov structure 
	iov.iov_base=(void *)nlh;
	iov.iov_len=nlh->nlmsg_len;

	//msg 
	memset(&msg, 0, sizeof(msg));
	msg.msg_name=(void *)&gNetlinkDestAddr;
	msg.msg_namelen=sizeof(gNetlinkDestAddr);
	msg.msg_iov=&iov;
	msg.msg_iovlen=1;
	
	//Send message to kernel for pid  
	if( sendmsg(gNetlinkSock, &msg, 0) <= 0) {
        CWDestroyThreadMutex(&gPendingRequestMutex);
		CWRoamLog("user2 sendmsg err: errno=%d!\n", errno);
	} else{
		CWRoamDebugLog("send driver a E to end timer");
	}

	if(!CWErr(CWDestroyThread(thread_receiveFrame))) {
        CWDestroyThreadMutex(&gPendingRequestMutex);
		CWRoamLog("Error stopping Thread that receive frame from driver");
	}else{
		CWRoamDebugLog("destrory thread_receiveFrame");
	}

	if(!cwRtkWtpExit())		//close netlink socket
		CWRoamLog("close gNetlinkSock");
	else
		CWRoamLog("Error closing gNetlinkSock");


	/* check alive thread_checkInternet then destroy */	
	if(strlen(gGatewayIp)){
#ifndef NEW_CHECKINTERNET
		if(!CWErr(CWDestroyThread(thread_checkInternet))) {
	        CWDestroyThreadMutex(&gPendingRequestMutex);
			CWRoamLog("Error stopping Thread that check ping reply time");
			exit(1);
		}else{
			CWRoamDebugLog("destroy thread_checkInternet");
		}
#else
		CWRoamDebugLog("destroy InternetCheck handler and kill PING");
		CWStopInternetCheckTimer();
		system("killall -9 ping");
#endif //NEW_CHECKINTERNET
	}
#endif

	wtpInRunState=0;
	CWStopHeartbeatTimer();
	if(gNeighborDeadTimerSet) {
		CWStopNeighborDeadTimer();
	}

    CWDestroyThreadMutex(&gPendingRequestMutex);
    
	CWLog("%s %d goto CW_ENTER_RESET", __FUNCTION__, __LINE__);	
	return CW_ENTER_RESET;
}

static CWBool CWWTPManageGenericRunMessage(CWProtocolMessage *orgMsgPtr) 
{
	CWProtocolMessage msg;
	CWBool holdMsg = CW_FALSE;
	CWControlHeaderValues controlVal;
	
	if(orgMsgPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	msg = *orgMsgPtr;
	msg.offset = 0;
	
	/* will be handled by the caller */
	if(!(CWParseControlHeader(&msg, &controlVal))) 
		return CW_FALSE;	

	if (gFirstRunMsg) {
		gLastSeqNum = controlVal.seqNum;
		if (gLastSeqNum == 255) gLastSeqNum = -1;
	}

	int len = controlVal.msgElemsLen - CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;

	int pendingMsgIndex = 0;

    
    CWThreadMutexLock(&gPendingRequestMutex);
	pendingMsgIndex = CWFindPendingRequestMsgsBox(gPendingRequestMsgs,
						      MAX_PENDING_REQUEST_MSGS,
						      controlVal.messageTypeValue,
						      controlVal.seqNum);

    if(pendingMsgIndex >= 0) {
        CWResetPendingMsgBox(&(gPendingRequestMsgs[pendingMsgIndex]));
    }

    CWThreadMutexUnlock(&gPendingRequestMutex);

	// Babylon: when pendingMsgIndex>=0, it means we got a message response we're waiting for.

	/* we have received a new Request or an Echo Response */
	if (pendingMsgIndex < 0) {

		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		CWBool toSend=CW_FALSE;
		CWBool reset = CW_FALSE;
		char newImageVersion[CW_IMAGE_IDENTIFIER_MAX_SIZE] = "\0";
		
		CWLog("Got Message From AC:%d", controlVal.messageTypeValue);
		switch(controlVal.messageTypeValue) {
			// =================================
			//	Got REQUEST messages
			// =================================
			case CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST:
			{
				CWProtocolResultCode resultCode;				
				CWProtocolConfigurationUpdateRequestValues values;
				int updateRequestType;

				/*Update 2009:
					check to see if a time-out on session occur...
					In case it happens it should go back to CW_ENTER_RESET*/
				if (!CWResetTimers()) {
#if 0
/* 2013 Babylon:
	i have no idea about the intention here...
	since messages is NULL, access messages[x] would cause fragmentation fault.
 */

					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
#endif
					return CW_FALSE;
				}
				
				CWLog("Configuration Update Request received");
				
				/************************************************************************************************
				 * Update 2009:																					*
				 *				These two function need an additional parameter (Pointer to updateRequestType)	*
				 *				for distinguish between all types of message elements.							*
				 ************************************************************************************************/

				if(!CWParseConfigurationUpdateRequest(msg.msg + msg.offset, len, &values, &updateRequestType))
					return CW_FALSE;

				CWSaveConfigurationUpdateRequest(&values, &resultCode, updateRequestType, newImageVersion);

				/*
				if ( updateRequestType == BINDING_MSG_ELEMENT_TYPE_OFDM_CONTROL )
					break; 
				*/
				
				/*Update 2009:
				 Added values (to return stuff with a conf update response)*/
				if(!CWAssembleConfigurationUpdateResponse(&messages,
														  &fragmentsNum,
														  gWTPPathMTU,
														  controlVal.seqNum,
														  resultCode,
														  values)) {
					return CW_FALSE;
				}
				
				toSend=CW_TRUE;				

				 /*
                                 * BUG-ML01- memory leak fix
                                 *
                                 * 16/10/2009 - Donato Capitella
                                 */
				/*
                                CWProtocolVendorSpecificValues* psValues = values.protocolValues;
                                if (psValues->vendorPayloadType == CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI)
					CW_FREE_OBJECT(((CWVendorUciValues *)psValues->payload)->response);
                                CW_FREE_OBJECT(psValues->payload);
                                CW_FREE_OBJECT(values.protocolValues);
				*/
				break;
			}

			case CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_REQUEST:
			{
				CWProtocolResultCode resultCode;

				/*Update 2009:
					check to see if a time-out on session occur...
					In case it happens it should go back to CW_ENTER_RESET*/
				if (!CWResetTimers()) {
#if 0
/* 2013 Babylon:
	i have no idea about the intention here...
	since messages is NULL, access messages[x] would cause fragmentation fault.
 */

					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
#endif
					return CW_FALSE;
				}
				CWLog("Clear Configuration Request received");
				/*WTP RESET ITS CONFIGURAION TO MANUFACTURING DEFAULT}*/
				if(!CWSaveClearConfigurationRequest(&resultCode))
					return CW_FALSE;
				if(!CWAssembleClearConfigurationResponse(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, resultCode)) 
					return CW_FALSE;

				toSend=CW_TRUE;
				break;
			}

#ifdef RTK_SMART_ROAMING
			case CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST:
			{
				/* check dupliacte request message */
				int SeqIndex = controlVal.seqNum % SEQUENCE_ARRAY_SIZE;
				if(gRcvSeqNum[SeqIndex] == controlVal.seqNum){
					CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Duplicate Request Recvied");
					return CW_FALSE;
				}
				else
					gRcvSeqNum[SeqIndex] = controlVal.seqNum;
				
				CWProtocolResultCode resultCode = CW_PROTOCOL_SUCCESS;
				//CWProtocolStationConfigurationRequestValues values;  --> da implementare
				/*Update 2009:
					check to see if a time-out on session occur...
					In case it happens it should go back to CW_ENTER_RESET*/
				if (!CWResetTimers()) {
					return CW_FALSE;
				}
				CWLog("Station Configuration Request received");
				
				if(!CWParseStationConfigurationRequest(msg.msg + msg.offset, len, &resultCode)) 
					return CW_FALSE;
				if(!CWAssembleStationConfigurationResponse(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, resultCode)) 
					return CW_FALSE;

				toSend=CW_TRUE;
				break;
			}
#endif
#ifdef KM_BY_AC
			case CW_MSG_TYPE_VALUE_WLAN_CONFIGURATION_REQUEST:
			{
				CWProtocolWlanConfigurationRequestValues cfgReqVal;
				CWProtocolWlanConfigurationResponseValues cfgRspVal;
				
				if (!CWResetTimers()) {
#if 0
/* 2013 Babylon:
	i have no idea about the intention here...
	since messages is NULL, access messages[x] would cause fragmentation fault.
 */

					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
#endif
					return CW_FALSE;
				}
				CWLog("WLAN Configuration Request received");


				
				CWParseWLANConfigurationRequest (msg.msg + msg.offset, len, &cfgReqVal);
				CWSaveWLANConfigurationRequest(&cfgReqVal, &cfgRspVal);
				
				CWDebugLog("WLAN configuration request resultCode=%d", cfgRspVal.result_code);
				if(!CWAssembleWLANConfigurationResponse(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, &cfgRspVal)) 
					return CW_FALSE;

				toSend=CW_TRUE;
				break;
				
			}
#endif
			case CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST:
			{	
				CWProtocolResultCode resultCode = CW_PROTOCOL_SUCCESS;
				int thisSeqNum = controlVal.seqNum;
				CWBool needSave;
				
				if (!CWResetTimers()) {
#if 0
/* 2013 Babylon:
	i have no idea about the intention here...
	since messages is NULL, access messages[x] would cause fragmentation fault.
 */

					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
#endif
					return CW_FALSE;
				}
				if (!cwRtkWtpGetTempFwId()) {
					CWLog("Error: Got Image Data Request, but no reboot firware specified from previous Image Data Response");
					return CW_FALSE;
				}

				// handle seqNum, if not in-sequence, just ignore this message
				if (!gFirstRunMsg && thisSeqNum==gLastSeqNum) {
					//(thisSeqNum - gLastSeqNum < 1 || thisSeqNum - gLastSeqNum > 40)) {	
					CWLog("Error: Got Image Data Request seqNum=%d, gLastSeqNum=%d. So, ignore!", thisSeqNum, gLastSeqNum);
					needSave = CW_FALSE;
				} else {
					needSave = CW_TRUE;
				}

				if(!CWParseImageDataRequestByWTP(msg.msg + msg.offset, len, needSave, cwRtkWtpSaveFirmwareDataPiece, &resultCode)) {
					CWLog("Error: Image Data Parse Error");
					return CW_FALSE;
				}

				gLastSeqNum = thisSeqNum;
				if (gLastSeqNum == 255) gLastSeqNum = -1;
								// TODO: Babylon: should have a more healthy way.....


				CWLog("Image Data Request received");

				if(!CWAssembleImageDataResponseByWTP(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, resultCode)) {
					return CW_FALSE;
				}
				
				toSend=CW_TRUE;
				break;
			}
			case CW_MSG_TYPE_VALUE_RESET_REQUEST:
			{	
				CWProtocolImageIdentifierValues image_identifier;
				CWProtocolResultCode resultCode = CW_PROTOCOL_SUCCESS;
				if (!CWResetTimers()) {
#if 0
/* 2013 Babylon:
	i have no idea about the intention here...
	since messages is NULL, access messages[x] would cause fragmentation fault.
 */

					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
#endif
					return CW_FALSE;
				}

				if(!CWParseResetRequest(msg.msg + msg.offset, len, &image_identifier, &resultCode)) {
					CWLog("parse reset request error");
					return CW_FALSE;
				}
				
				CWLog("#________ Reset (Run) ________#");

				if (resultCode == CW_PROTOCOL_SUCCESS) {
					CWDebugLog("checking vendor id");
					if (image_identifier.vendor_id != 0 && cwRtkWtpGetVendorId() != image_identifier.vendor_id) {
						CWLog("WTP Can't be reset since no valid firmware is available.(Vendor ID invalid)");
						resultCode = CW_PROTOCOL_RST_FAILURE_UNABLE_TO_RESET;
					}
					CWDebugLog("checking firmware");
					if (cwRtkWtpGetTempFwId() != NULL &&
						strcmp(cwRtkWtpGetTempFwId(), image_identifier.imageIdentifier) != 0) {
						CWLog("WTP Can't be reset since no valid firmware is available.(Requested firmware does not exist.)");
						resultCode = CW_PROTOCOL_RST_FAILURE_UNABLE_TO_RESET;
					} else if (!cwRtkWtpCheckFirmware()) {
						CWLog("WTP Can't be reset since no valid firmware is available.(Requested firmware is not a valid firmware.)");
						resultCode = CW_PROTOCOL_RST_FAILURE_UNABLE_TO_RESET;
					}
				}
				CWLog("Got Reset Request from AC! WTP will reboot with %s", cwRtkWtpGetTempFwId());
				if(!CWAssembleResetResponse(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, resultCode)) {
					CWLog("Can't assemble reset responese");
					return CW_FALSE;
				}
				toSend=CW_TRUE;
				if (resultCode == CW_PROTOCOL_SUCCESS) {
					CWLog("WTP will reboot with %s!", cwRtkWtpGetTempFwId());
					reset = CW_TRUE;
				} else {
					CWLog("WTP won't reboot!");
					reset = CW_FALSE;
				}
				break;
			}
#ifdef RTK_CAPWAP
			case CW_MSG_TYPE_VALUE_RTK_WTP_STATUS_REQUEST:
			{
				CWProtocolRtkWTPStatusRequestValues wtpStatusReq;
				CWProtocolRtkWTPStatusResponseValues wtpStatusResp;

				CWLog("#________ RTK WTP Status (Run) ________#");

				if (!CWResetTimers()) {
					return CW_FALSE;
				}

				if(!CWParseRtkWTPStatusRequest(msg.msg + msg.offset, len, &wtpStatusReq)) {
					return CW_FALSE;
				}

				wtpStatusResp.resultCode = CW_PROTOCOL_SUCCESS;
				if (wtpStatusReq.siteSurveyReq) {
					int nRadios = cwRtkWtpGetMaxRadios();
					int numSiteSurvey = 0;
					int i, nBss;
					for (i=0; i<nRadios; i++) {
						if (cwRtkWtpSiteSurvey(i, CW_MAX_BSS_RTK_SITE_SURVEY, &nBss, wtpStatusResp.siteSurvey[numSiteSurvey].bssInfo)) {
							// Babylon: Note that nRadios is not assigned to numSiteSurvey, because it is possible that we can't survey sites for this radio.
							wtpStatusResp.siteSurvey[numSiteSurvey].radioID = i;
							wtpStatusResp.siteSurvey[numSiteSurvey].numBss = nBss;
							numSiteSurvey ++;
						}
					}
					wtpStatusResp.numSiteSurvey = numSiteSurvey;					
					wtpStatusResp.siteSurveyResponsed = CW_TRUE;
				} else {
					wtpStatusResp.siteSurveyResponsed = CW_FALSE;
				}

				if (wtpStatusReq.stationReq) {
					int nRadios = cwRtkWtpGetMaxRadios();
					int nWlans = cwRtkWtpGetMaxWlanPerRadio();
					int numStations = 0;
					int i, j, left = CW_MAX_STATIONS_PER_WTP, n;
					for (i=0; i<nRadios; i++) {
						for (j=0; j<nWlans; j++) {
							if (left>0) {
								if (cwRtkWtpGetWlanStations(i, j, left, &n, &wtpStatusResp.rtkStations[numStations])) {
									left -= n;
									numStations += n;	
								}
							}
						}
					}
					wtpStatusResp.numRtkStations = numStations;					
					wtpStatusResp.rtkStationsResponsed = CW_TRUE;
				} else {
					wtpStatusResp.rtkStationsResponsed = CW_FALSE;
				}

				if(!CWAssembleRtkWTPStatusResponse(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, &wtpStatusResp)) {
					return CW_FALSE;
				}
				toSend=CW_TRUE;
				break;
			}
#endif

			// =================================
			//	Got RESPONSE messages
			// =================================
			case CW_MSG_TYPE_VALUE_ECHO_RESPONSE:
			{
				/*Update 2009:
					check to see if a time-out on session occur...
					In case it happens it should go back to CW_ENTER_RESET*/
				if (!CWResetTimers()) {
#if 0
/* 2013 Babylon:
	i have no idea about the intention here...
	since messages is NULL, access messages[x] would cause fragmentation fault.
 */

					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
#endif
					return CW_FALSE;
				}
				CWLog("Echo Response received");
				break;
			}
			default:
			{
				/* 
				 * We can't recognize the received Request so
				 * we have to send a corresponding response
				 * containing a failure result code
				 */
			    if((controlVal.messageTypeValue%2) == 0) {					
					CWLog("--> Not valid Response in Run State");
					break; /*it is a response*/
			    }
				
				CWLog("--> Not valid Request in Run State... we send a failure Response");
				/*Update 2009:
					check to see if a time-out on session occur...
					In case it happens it should go back to CW_ENTER_RESET*/
				if (!CWResetTimers()) {
#if 0
/* 2013 Babylon:
	i have no idea about the intention here...
	since messages is NULL, free access messages[x] would cause fragmentation fault.
 */

					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
#endif
					return CW_FALSE;
				}
				if(!(CWAssembleUnrecognizedMessageResponse(&messages,
									   &fragmentsNum,
									   gWTPPathMTU,
									   controlVal.seqNum,
									   controlVal.messageTypeValue+1))) 
					return CW_FALSE;

				toSend = CW_TRUE;
				/* return CWErrorRaise(CW_ERROR_INVALID_FORMAT,
				 * 		       "Received Message not valid in Run State");
				 */
			}
		}
		if(toSend) {
			int i, j;			
			CWBool sendResult;
			for(i = 0; i < fragmentsNum; i++) {
				for (j=0; j<5; j++) {
#ifdef CW_NO_DTLS
					sendResult = CWNetworkSendUnsafeConnected(gWTPSocket, messages[i].msg, messages[i].offset);
#else
					sendResult = CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset);
#endif
					if (sendResult) break;
					sleep(3);
					CWLog("Retry send (to response message: %d), fragment index: %d, times:%d", controlVal.messageTypeValue, i, j+1);
				}
				if (!sendResult) {
					CWLog("Error: cannot send message");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					return CW_FALSE;
				}
			}

			//CWLog("Message Sent\n");
			CWFreeMessageFragments(messages, fragmentsNum);
			CW_FREE_OBJECT(messages);

			/*
			 * Check if we have to exit due to an update commit request.
			 */
			if (WTPExitOnUpdateCommit) {
			     exit(EXIT_SUCCESS);
			}
		}
		if (newImageVersion[0] != '\0') {
			// handle for image download
			if (!CWWTPInitiateImageDownload(newImageVersion)) {
				return CW_FALSE;
			}
		}
		if (reset) {
			CWLog("#### !!!! WTP RESET by AC !!! #####");
			//gWillReboot = CW_TRUE;
			//cwRtkWtpFirmwareUpgradeReboot();
		}

	} else {/* we have received a Response */

		/*Update 2009:
		  		check to see if a time-out on session occur...
		 		 In case it happens it should go back to CW_ENTER_RESET*/
		if (!CWResetTimers())
			return CW_FALSE;

		switch(controlVal.messageTypeValue) 
		{
			case CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE:
				CWLog("Change State Event Response received");
				break;
		
#ifdef RTK_SMART_ROAMING
			case CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE:
				CWLog("Recieve WTP Event Response pendingMsg:%d, seqNum: %d", pendingMsgIndex, controlVal.seqNum);
				if(!CWParseWTPEventResponseMessage(&controlVal, len)) 
					return CW_FALSE;
				break;
#endif
			case CW_MSG_TYPE_VALUE_DATA_TRANSFER_RESPONSE:
				CWLog("Data Transfer Response received");
				break;

			default:
				/* 
				 * We can't recognize the received Response: we
				 * ignore the message and log the event.
				 */
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT,
						    "Received Message not valid in Run State");
		}
	}
	if (!holdMsg) {
		CW_FREE_PROTOCOL_MESSAGE(*orgMsgPtr);
	}
	gLastSeqNum = controlVal.seqNum;
	if (gLastSeqNum == 255) gLastSeqNum = -1;
	return CW_TRUE;
}

/*______________________________________________________________*/
/*  *******************___TIMER HANDLERS___*******************  */
void CWWTPHeartBeatTimerExpiredHandler(void *arg) {

	CWList msgElemList = NULL;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum = 0;
	int seqNum;
	if(!gNeighborDeadTimerSet) {

		if (!CWStartNeighborDeadTimer()) {

			CWStopHeartbeatTimer();
			CWStopNeighborDeadTimer();
			return;
		}
	}
//printf("(%s)\n", __FUNCTION__);
	CWLog("WTP HeartBeat Timer Expired... we send an ECHO Request");	
	CWLog("\n");
	CWLog("#________ Echo Request Message (Run) ________#");
	
	seqNum = CWGetSeqNum();

	if(!CWAssembleEchoRequest(&messages,
				  &fragmentsNum,
				  gWTPPathMTU,
				  seqNum,
				  msgElemList)){
		int i;

		CWDebugLog("Failure Assembling Echo Request");
		if(messages)
			for(i = 0; i < fragmentsNum; i++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[i]);
			}	
		CW_FREE_OBJECT(messages);
		return;
	}
#if 1	
	int i;
	for(i = 0; i < fragmentsNum; i++) {
#ifdef CW_NO_DTLS
		CWLog("(%s) Echo Request no dtls send \n", __FUNCTION__);
		if(!CWNetworkSendUnsafeConnected(gWTPSocket, messages[i].msg, messages[i].offset)) {
#else
		CWLog("(%s) Echo Request security send \n", __FUNCTION__);
		if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset)){
#endif
			CWLog("Failure sending Request");
			int k;
			for(k = 0; k < fragmentsNum; k++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[k]);
			}	
			CW_FREE_OBJECT(messages);
			break;
		}
	}


#else
	int i, j;			
	CWBool sendResult;
	for(i = 0; i < fragmentsNum; i++) {
		for (j=0; j<5; j++) {
#ifdef CW_NO_DTLS
			sendResult = CWNetworkSendUnsafeConnected(gWTPSocket, messages[i].msg, messages[i].offset);
#else
			sendResult = CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset);
#endif
			if (sendResult) break;
			sleep(3);
			CWLog("Retry send Echo Request, fragment index: %d, times:%d", i, j+1);
		}
		if (!sendResult) {
			CWLog("Failure sending Echo Request");
			break;
		}
	}
#endif

	int k;
	for(k = 0; messages && k < fragmentsNum; k++) {
		CW_FREE_PROTOCOL_MESSAGE(messages[k]);
	}	
	CW_FREE_OBJECT(messages);

//printf("(%s) -- start timer\n", __FUNCTION__);
	if(!CWStartHeartbeatTimer()) {
		return;
	}

}

#ifdef KM_BY_AC
void CWWTPKeepAliveDataTimerExpiredHandler(void *arg) {

	CWProtocolMessage *messages = NULL;
	CWProtocolMessage sessionIDmsgElem;
	int fragmentsNum = 0;

	CWAssembleMsgElemSessionID(&sessionIDmsgElem, &gWTPSessionID[0]);
	
	/* Send WTP Event Request */
	if (!CWAssembleDataMessage(&messages, 
			    &fragmentsNum, 
			    gWTPPathMTU, 
			    &sessionIDmsgElem, 
			    NULL,
			    CW_PACKET_PLAIN,
			    1
			    ))
	{
		int i;

		CWDebugLog("Failure Assembling KeepAlive Request");
		if(messages)
			for(i = 0; i < fragmentsNum; i++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[i]);
			}	
		CW_FREE_OBJECT(messages);
		return;
	}
	
	int i;
	for(i = 0; i < fragmentsNum; i++) {

		if(!CWNetworkSendUnsafeConnected(gWTPDataSocket, messages[i].msg, messages[i].offset)) {
			CWLog("Failure sending  KeepAlive Request");
			int k;
			for(k = 0; k < fragmentsNum; k++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[k]);
			}	
			CW_FREE_OBJECT(messages);
			break;
		}
	}

	int k;
	for(k = 0; messages && k < fragmentsNum; k++) {
		CW_FREE_PROTOCOL_MESSAGE(messages[k]);
	}	
	CW_FREE_OBJECT(messages);
}
#endif

void CWWTPNeighborDeadTimerExpired(void *arg) {

	CWLog("WTP NeighborDead Timer Expired... we consider Peer Dead.");

#ifdef DMALLOC
	dmalloc_shutdown(); 
#endif

	return;
}


CWBool CWStartHeartbeatTimer() {
	CWDebugLog("(%s)\n", __FUNCTION__);
	gCWHeartBeatTimerID = timer_add(gEchoInterval,
					0,
					&CWWTPHeartBeatTimerExpiredHandler,
					NULL);
	
	if (gCWHeartBeatTimerID == -1)	return CW_FALSE;

	CWDebugLog("Echo Heartbeat Timer Started");
#ifdef KM_BY_AC
	gCWKeepAliveTimerID = timer_add(gDataChannelKeepAlive,
					0,
					&CWWTPKeepAliveDataTimerExpiredHandler,
					NULL);
	
	if (gCWKeepAliveTimerID == -1)	return CW_FALSE;

	CWDebugLog("Keepalive Heartbeat Timer Started");
#endif
	return CW_TRUE;
}


CWBool CWStopHeartbeatTimer(){
//printf("(%s)\n", __FUNCTION__);
 	timer_rem(gCWHeartBeatTimerID, NULL);
	//CWDebugLog("Echo Heartbeat Timer Stopped");
#ifdef KM_BY_AC
	timer_rem(gCWKeepAliveTimerID, NULL);
	CWDebugLog("KeepAlive Heartbeat Timer Stopped");
#endif
	return CW_TRUE;
}


CWBool CWStartNeighborDeadTimer() {

	gCWNeighborDeadTimerID = timer_add(gCWNeighborDeadInterval,
					   0,
					   &CWWTPNeighborDeadTimerExpired,
					   NULL);
	
	if (gCWNeighborDeadTimerID == -1)	return CW_FALSE;

	CWDebugLog("NeighborDead Timer Started");
	gNeighborDeadTimerSet = CW_TRUE;
	return CW_TRUE;
}


CWBool CWStopNeighborDeadTimer() {
	
	timer_rem(gCWNeighborDeadTimerID, NULL);
	CWDebugLog("NeighborDead Timer Stopped");
	gNeighborDeadTimerSet = CW_FALSE;
	return CW_TRUE;
}


CWBool CWResetTimers() {

	if(gNeighborDeadTimerSet) {
	
		if (!CWStopNeighborDeadTimer()) return CW_FALSE;
	}
	
	if(!CWStopHeartbeatTimer()) 
		return CW_FALSE;
	
	if(!CWStartHeartbeatTimer()) 
		return CW_FALSE;
	
	return CW_TRUE;
}

/*__________________________________________________________________*/
/*  *******************___ASSEMBLE FUNCTIONS___*******************  */
CWBool CWAssembleEchoRequest (CWProtocolMessage **messagesPtr,
			      int *fragmentsNumPtr,
			      int PMTU,
			      int seqNum,
			      CWList msgElemList) {
//printf("(%s)\n", __FUNCTION__);

	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 0;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Echo Request...");
		
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_ECHO_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else			       
			       CW_PACKET_CRYPT
#endif
			       ))) 
		return CW_FALSE;
	
	CWLog("Echo Request Assembled");
	
	return CW_TRUE;
}

CWBool CWAssembleWTPDataTransferRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList)
{
	CWProtocolMessage *msgElems= NULL;
	int msgElemCount = 0;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	int i;
	CWListElement *current;
	int k = -1;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL || msgElemList == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	msgElemCount = CWCountElementInList(msgElemList);

	if (msgElemCount > 0) {
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	} 
	else msgElems = NULL;
		
	CWLog("Assembling WTP Data Transfer Request...");

	current=msgElemList;
	for (i=0; i<msgElemCount; i++)
	{
		switch (((CWMsgElemData *) current->data)->type)
		{
			case CW_MSG_ELEMENT_DATA_TRANSFER_DATA_CW_TYPE:
				if (!(CWAssembleMsgElemDataTransferData(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value.int32)))
					goto cw_assemble_error;	
				break;
			/*case CW_MSG_ELEMENT_DATA_TRANSFER_MODE_CW_TYPE:
				if (!(CWAssembleMsgElemDataTansferMode(&(msgElems[++k]))))
					goto cw_assemble_error;
				break;*/
		
			default:
				goto cw_assemble_error;
				break;	
		}

		current = current->next;	
	}

	if (!(CWAssembleMessage(messagesPtr,
				fragmentsNumPtr,
				PMTU,
				seqNum,
				CW_MSG_TYPE_VALUE_DATA_TRANSFER_REQUEST,
				msgElems,
				msgElemCount,
				msgElemsBinding,
				msgElemBindingCount,
#ifdef CW_NO_DTLS
				CW_PACKET_PLAIN
#else
				CW_PACKET_CRYPT
#endif
				)))
	 	return CW_FALSE;

	CWLog("WTP Data Transfer Request Assembled");
	
	return CW_TRUE;

cw_assemble_error:
	{
		int i;
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
}

#ifdef RTK_SMART_ROAMING
CWBool CWAssembleWTPEventRequest(CWProtocolMessage **messagesPtr,
				 int *fragmentsNumPtr,
				 int PMTU,
				 int seqNum,
				 CWList msgElemList,
				 int len) 
{

	CWProtocolMessage *msgElems= NULL;
	int msgElemCount = 0;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	int i;
	CWListElement *current;
	int k = -1;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL || msgElemList == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	msgElemCount = CWCountElementInList(msgElemList);

	if (msgElemCount > 0) {
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	} 
	else 
		msgElems = NULL;
		
	//CWLog("Assembling WTP Event Request...");

	current=msgElemList;
	for (i=0; i<msgElemCount; i++) {

#if 0
		switch (((CWMsgElemData *) current->data)->type) {

			case CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE:
				if (!(CWAssembleMsgElemDecryptErrorReport(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value.int32)))
					goto cw_assemble_error;	
				break;
			case CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE:
				if (!(CWAssembleMsgElemDuplicateIPv4Address(&(msgElems[++k]))))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE:
				if (!(CWAssembleMsgElemDuplicateIPv6Address(&(msgElems[++k]))))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE:
				if (!(CWAssembleMsgElemWTPOperationalStatistics(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value.int32)))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE:
				if (!(CWAssembleMsgElemWTPRadioStatistics(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value.int32)))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE:
				if (!(CWAssembleMsgElemWTPRebootStatistics(&(msgElems[++k]))))
					goto cw_assemble_error;	
				break;
			case CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE:
			{
				// 2013 Babylon: when station deleted by WTP, notify AC.			
				if (!CWAssembleMsgElemDeleteStation(((CWMsgElemData *) current->data)->value.del_sta.radio_id, &msgElems[++k], ((CWMsgElemData *) current->data)->value.del_sta.mac_addr)) {
					goto cw_assemble_error; 
				}
				break;
			}
			default:
				goto cw_assemble_error;
				break;	
		}
#endif
		if (!CWAssembleMsgElemAddStation(&msgElems[++k], len,
				((CWMsgElemData *) current->data)->value.str,
				((CWMsgElemData *) current->data)->type))				
			goto cw_assemble_error; 
		current = current->next;	
	}

	if (!(CWAssembleMessage(messagesPtr,
				fragmentsNumPtr,
				PMTU,
				seqNum,
				CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
				msgElems,
				msgElemCount,
				msgElemsBinding,
				msgElemBindingCount,
#ifdef CW_NO_DTLS
				CW_PACKET_PLAIN
#else
				CW_PACKET_CRYPT
#endif
				)))
	 	return CW_FALSE;

	//CWLog("WTP Event Request Assembled");
	
	return CW_TRUE;

cw_assemble_error:
	{
		int i;
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
}
#endif
/*Update 2009:
	Added values to args... values is used to determine if we have some 
	payload (in this case only vendor and only UCI) to send back with the
	configuration update response*/
CWBool CWAssembleConfigurationUpdateResponse(CWProtocolMessage **messagesPtr,
					     int *fragmentsNumPtr,
					     int PMTU,
					     int seqNum,
					     CWProtocolResultCode resultCode,
						 CWProtocolConfigurationUpdateRequestValues values) {

	CWProtocolMessage *msgElems = NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	CWProtocolVendorSpecificValues *protoValues = NULL;

	/*Get protocol data if we have it*/
	if (values.protocolValues) 
		protoValues = (CWProtocolVendorSpecificValues *) values.protocolValues;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Configuration Update Response...");
	
	CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if (protoValues)  {
		switch (protoValues->vendorPayloadType) {
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI:
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_WUM:
					if (!(CWAssembleVendorMsgElemResultCodeWithPayload(msgElems,resultCode, protoValues))) {
						CW_FREE_OBJECT(msgElems);
						return CW_FALSE;
					}

			break;

			default:
				/*Result Code only*/
				if (!(CWAssembleMsgElemResultCode(msgElems,resultCode))) {
					CW_FREE_OBJECT(msgElems);
					return CW_FALSE;
				}
		}
	} else  {
		/*Result Code only*/
		if (!(CWAssembleMsgElemResultCode(msgElems,resultCode))) {
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE;
		}
	}
		
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       CW_PACKET_CRYPT
#endif
			       ))) 
		return CW_FALSE;
	
	CWLog("Configuration Update Response Assembled");
	
	return CW_TRUE;
}

CWBool CWAssembleClearConfigurationResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode) 
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Clear Configuration Response...");
	
	CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if (!(CWAssembleMsgElemResultCode(msgElems,resultCode))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}

	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_RESPONSE,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       CW_PACKET_CRYPT
#endif
			       ))) 
		return CW_FALSE;
	
	CWLog("Clear Configuration Response Assembled");
	
	return CW_TRUE;
}
#ifdef RTK_SMART_ROAMING
CWBool CWAssembleStationConfigurationResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode) 
{
	
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Sattion Configuration Response...");
	
	CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if (!(CWAssembleMsgElemResultCode(msgElems,resultCode))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}

	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       CW_PACKET_CRYPT
#endif
			       ))) 
		return CW_FALSE;
	
	CWLog("Station Configuration Response Assembled");
	
	return CW_TRUE;
}
#endif

#ifdef KM_BY_AC
CWBool CWAssembleWLANConfigurationResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, 
					const CWProtocolWlanConfigurationResponseValues *cfgRspVal)
{
	
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount;
	CWProtocolMessage *msgElemsBinding = NULL;
	const int msgElemBindingCount = 0;
	
	if (cfgRspVal->wlancfg_type == CW_WLANCFG_TYPE_ADD) {
		msgElemCount = 2;
	} else {
		msgElemCount = 1;
	}
	
	CWLog("Assembling WLAN Configuration Response...");
	
	//CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	

	if (!(CWAssembleMsgElemResultCode(&msgElems[0],cfgRspVal->result_code))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}

	if (cfgRspVal->wlancfg_type == CW_WLANCFG_TYPE_ADD) {
		if (!(CWAssembleMsgElemAssignedWTPBSSID(&msgElems[1],cfgRspVal->radio_id, cfgRspVal->wlan_id, cfgRspVal->bssid))) {
			CW_FREE_PROTOCOL_MESSAGE(msgElems[0]);		
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE;
		}
	}
	/*
	2013 Babylon: I don't know what this is for. But the parsing logic in AC part does not recognize this.
	if (!(CWAssembleMsgElemVendorSpecificPayload((&(msgElems[++k]))))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}
	*/
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WLAN_CONFIGURATION_RESPONSE,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       CW_PACKET_CRYPT
#endif
			       ))) {
		int i;
		for (i=0; i<msgElemCount; i++) CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}
	
	CWLog("WLAN Configuration Response Assembled");
	
	return CW_TRUE;
}
#endif

CWBool CWAssembleResetResponse(CWProtocolMessage **messagesPtr,
					     int *fragmentsNumPtr,
					     int PMTU,
					     int seqNum,
					     CWProtocolResultCode resultCode) 
{

		CWProtocolMessage *msgElems = NULL;
		const int msgElemCount = 1;
		CWProtocolMessage *msgElemsBinding = NULL;
		const int msgElemBindingCount = 0;
		if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
		
		CWLog("Assembling Reset Response...");
		
		//CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		
	
		if (!CWAssembleMsgElemResultCode(&msgElems[0],resultCode)) {
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE;
		}
		
		if(!CWAssembleMessage(messagesPtr,
					   fragmentsNumPtr,
					   PMTU,
					   seqNum,
					   CW_MSG_TYPE_VALUE_RESET_RESPONSE,
					   msgElems,
					   msgElemCount,
					   msgElemsBinding,
					   msgElemBindingCount,
#ifdef CW_NO_DTLS
					   CW_PACKET_PLAIN
#else
					   CW_PACKET_CRYPT
#endif
					   )) 
			return CW_FALSE;
		
		CWLog("Reset Response Assembled");
		
		return CW_TRUE;

}

#ifdef RTK_CAPWAP
CWBool CWAssembleRtkWTPStatusResponse(CWProtocolMessage **messagesPtr,
						 int *fragmentsNumPtr,
						 int PMTU,
						 int seqNum,
						 const CWProtocolRtkWTPStatusResponseValues *wtpResponse)
{

	CWProtocolMessage *msgElems = NULL;
	int msgElemCount = 1 + 	// result Code
					(wtpResponse->siteSurveyResponsed? (1+wtpResponse->numSiteSurvey) : 0) +  // for site survey
					(wtpResponse->rtkStationsResponsed? (1+wtpResponse->numRtkStations) : 0);	// for list stations
	CWProtocolMessage *msgElemsBinding = NULL;
	const int msgElemBindingCount = 0;
	int i, k=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling RTK WTP Status Response...");
	
	//CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	if (!CWAssembleMsgElemResultCode(&msgElems[k],wtpResponse->resultCode)) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}
	k++;

	if (wtpResponse->siteSurveyResponsed) {
		CWAssembleMsgElemRtkSiteSurveyRequest(&msgElems[k]);
		k++;
		for (i=0; i<wtpResponse->numSiteSurvey; i++, k++) {
			if (!CWAssembleMsgElemRtkSiteSurvey(&msgElems[k], wtpResponse->siteSurvey[i].radioID, wtpResponse->siteSurvey[i].numBss, wtpResponse->siteSurvey[i].bssInfo)) {
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
		}		
	}

	if (wtpResponse->rtkStationsResponsed) {

		CWAssembleMsgElemRtkStationRequest(&msgElems[k]);
		k++;
		for (i=0; i<wtpResponse->numRtkStations; i++, k++) {
			if (!CWAssembleMsgElemRtkStation(&msgElems[k], &wtpResponse->rtkStations[i])) {
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
		}		
	}
	if(!CWAssembleMessage(messagesPtr,
				   fragmentsNumPtr,
				   PMTU,
				   seqNum,
				   CW_MSG_TYPE_VALUE_RTK_WTP_STATUS_RESPONSE,
				   msgElems,
				   msgElemCount,
				   msgElemsBinding,
				   msgElemBindingCount,
#ifdef CW_NO_DTLS
				   CW_PACKET_PLAIN
#else
				   CW_PACKET_CRYPT
#endif
				   )) 
		return CW_FALSE;
	
	CWLog("RTK WTP Status Response Assembled");
	
	return CW_TRUE;

}

#endif

/*_______________________________________________________________*/
/*  *******************___PARSE FUNCTIONS___*******************  */
/*Update 2009:
	Function that parses vendor payload,
	filling in valuesPtr*/
CWBool CWParseVendorMessage(unsigned char *msg, int len, void **valuesPtr){
	CWProtocolMessage completeMsg;
	unsigned short int GlobalElemType=0;// = CWProtocolRetrieve32(&completeMsg);

	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Parsing Vendor Specific Message...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	CWProtocolVendorSpecificValues *vendPtr;
  

	// parse message elements
	while(completeMsg.offset < len) {
	  unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
	  unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		GlobalElemType = elemType;

		//CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
			  completeMsg.offset += elemLen;
			  break;
		default:
				if(elemType == CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE) 
				{
					CW_FREE_OBJECT(valuesPtr);
					return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
				}
				else 
				{
					completeMsg.offset += elemLen;
					break;
				}
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");


	switch(GlobalElemType) {
		case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
			CW_CREATE_OBJECT_ERR(vendPtr, CWProtocolVendorSpecificValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			/*Allocate various other vendor specific fields*/
		break;
	}

	completeMsg.offset = 0;
	while(completeMsg.offset < len) {
		unsigned short int type=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&elemLen);		

		switch(type) {
			/*Once we know it is a vendor specific payload...*/
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
				{
					if (!(CWParseVendorPayload(&completeMsg, elemLen, (CWProtocolVendorSpecificValues *) vendPtr)))
					{
						CW_FREE_OBJECT(vendPtr);
						return CW_FALSE; // will be handled by the caller
					}
				}
				break;
			default:
				completeMsg.offset += elemLen;
			break;
		}
	}
	
	*valuesPtr = (void *) vendPtr;
	CWLog("Vendor Message Parsed");
	
	return CW_TRUE;
}


CWBool CWParseConfigurationUpdateRequest (unsigned char *msg,
					  int len,
					  CWProtocolConfigurationUpdateRequestValues *valuesPtr, 
					  int *updateRequestType) {
#ifndef RTK_CAPWAP
	CWBool bindingMsgElemFound=CW_FALSE;
#endif
	CWBool vendorMsgElemFound=CW_FALSE;
	CWBool imageIdentifierFound=CW_FALSE;

	CWProtocolMessage completeMsg;
	unsigned short int GlobalElementType = 0;
	
	if(msg == NULL || valuesPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Parsing Configuration Update Request...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	valuesPtr->nWtpQos = 0;
#ifndef RTK_CAPWAP
	valuesPtr->bindingValues = NULL;
#endif
	/*Update 2009:
		added protocolValues (non-binding)*/
	valuesPtr->protocolValues = NULL;

#ifdef RTK_CAPWAP
	valuesPtr->nRtkWlanCfg = 0;
	valuesPtr->nRtkPowerScaleCfg = 0;
	valuesPtr->nRtkChannelCfg = 0;
#endif

	*updateRequestType = GlobalElementType;


	/* parse message elements */
	while(completeMsg.offset < len) {

		unsigned short int elemType=0;	/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int elemLen=0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		GlobalElementType = elemType;

		/* CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen); */
		CWLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
#ifndef RTK_CAPWAP
		if(CWBindingCheckType(elemType)) {
			bindingMsgElemFound=CW_TRUE;
		}
#endif
		switch(elemType) {
			/*Update 2009:
				Added case for vendor specific payload
				(Used mainly to parse UCI messages)...*/
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
				vendorMsgElemFound=CW_TRUE;
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:
				//printf("Got image identifier\n");
				imageIdentifierFound = CW_TRUE;
				CWParseMsgElemImageIdentifier(&completeMsg, elemLen, &valuesPtr->image_identifier);
				break;
#ifdef RTK_CAPWAP
			case CW_MSG_ELEMENT_RTK_POWER_SCALE_CW_TYPE:
				//printf("To update power scale\n");
				if (CWParseMsgElemRtkPowerScale(&completeMsg, elemLen, &valuesPtr->rtkPowerScaleCfg[valuesPtr->nRtkPowerScaleCfg]))
					valuesPtr->nRtkPowerScaleCfg++;
				else
					return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "RTK Power Scale Format Error");
				break;				

			case CW_MSG_ELEMENT_RTK_CHANNEL_CW_TYPE:
				//printf("To update channel\n");
				if (CWParseMsgElemRtkChannel(&completeMsg, elemLen, &valuesPtr->rtkChannelCfg[valuesPtr->nRtkChannelCfg]))
					valuesPtr->nRtkChannelCfg++;
				else
					return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "RTK Channel Format Error");
				break;
#endif
			case CW_MSG_ELEMENT_IEEE80211_WTP_QOS_CW_TYPE:
				if(!CWParseMsgElemWtpQos(&completeMsg, len, &valuesPtr->wtpQos[valuesPtr->nWtpQos])) {
					return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "WTP_QOS Format Error");
				}
				valuesPtr->nWtpQos++;
				break;

			default:
				CWLog("Unrecognized Message Element: %d (len=%d)", elemType, elemLen);
				completeMsg.offset += elemLen;
				break;
				/*
				printf("Unrecognized message element %d\n", elemType);
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT,
						    "Unrecognized Message Element");
				*/
		}
	}

	if (completeMsg.offset != len) {
		//printf("%s Garbage at the End of the Message\n", __FUNCTION__);
//		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}


	// 2013 Babylon: added for image identifier.
	if (imageIdentifierFound) {
		*updateRequestType = CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE;
		CWLog("Configure Update Request Parsed - IMAGE_IDENTIFIER");
		return CW_TRUE;
	}

	/* 2013 Babylon:
	  * The original logic was not correct. The original programmer must be an idiot....
	  * This kind of code is spreading everywhere in the source, such that I have no time to correct all of these...
	  * (1) GlobalElementType would update with elemType. The variable is redundent. It's NOT global at all!
	  * (2) Actually, ConfigurationUpdate Command should be capable to handle multiple types of update.
	  *       If the designe idea was handling only one type of update, it should immediately return after handling, but not continued to do the next kind.
	  */

	/*Update 2009:
		deal with vendor specific messages*/
	if (vendorMsgElemFound) {
		/* For the knownledge of SaveConfiguration */
	  	*updateRequestType = GlobalElementType;

		if (!(CWParseVendorMessage(msg, len, &(valuesPtr->protocolValues)))) {

			return CW_FALSE;
		}
	}
#ifndef RTK_CAPWAP
	if (bindingMsgElemFound) {
	  /* For the knownledge of SaveConfiguration */
	  *updateRequestType = GlobalElementType;
		if (!(CWBindingParseConfigurationUpdateRequest(msg, len, &(valuesPtr->bindingValues)))) {
			return CW_FALSE;
		}
	}
#endif
	CWLog("Configure Update Request Parsed");
	
	return CW_TRUE;
}
#ifdef KM_BY_AC
#ifdef SOFTMAC
static CWBool CWParseWLANConfigurationRequest (char *msg, int len, CWProtocolResultCode *pResultCode)
{

	CWProtocolMessage completeMsg;
	
	if(msg == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Parsing WLAN Configuration Request...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		
		 					
		switch(elemType) { 

			case CW_MSG_ELEMENT_IEEE80211_ADD_WLAN_CW_TYPE:
				
				if (!(CWParseMsgElemAddWLAN(&completeMsg,  elemLen)))
					return CW_FALSE;
				break;
				
			case CW_MSG_ELEMENT_IEEE80211_DELETE_WLAN_CW_TYPE:
				
				if (!(CWParseMsgElemDeleteWLAN(&completeMsg,  elemLen)))
					return CW_FALSE;
				break;
				
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

	CWLog("Station WLAN Request Parsed");
	
	return CW_TRUE;
}

#else
void CWParseWLANConfigurationRequest (unsigned char *msg, int len, CWProtocolWlanConfigurationRequestValues *pCfgReqVal)
{
	CWProtocolMessage completeMsg;

	// begin to parse
	completeMsg.msg = msg;
	completeMsg.offset = 0;	
	pCfgReqVal->wlancfg_type = CW_WLANCFG_TYPE_INVALID;
	
	CWLog("Parsing WLAN Configuration Request...");
	while(completeMsg.offset < len) {
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);
		CWLog("\telemType=%u", elemType);

		switch(elemType) { 
			case CW_MSG_ELEMENT_IEEE80211_ADD_WLAN_CW_TYPE:
			{
				if (pCfgReqVal->wlancfg_type != CW_WLANCFG_TYPE_INVALID) {
					pCfgReqVal->wlancfg_type = CW_WLANCFG_TYPE_INVALID;
					return;
				}

				// parse and create struct of AddWlan
				if ((pCfgReqVal->wlan_cfg.addWlanCfg = CWParseMsgElemAddWLAN(&completeMsg,  elemLen)) == NULL) {
					pCfgReqVal->wlancfg_type = CW_WLANCFG_TYPE_INVALID;
					return;
				}
				pCfgReqVal->wlancfg_type = CW_WLANCFG_TYPE_ADD;
				break;
			}
				
			case CW_MSG_ELEMENT_IEEE80211_DELETE_WLAN_CW_TYPE:		
			{
				if (pCfgReqVal->wlancfg_type != CW_WLANCFG_TYPE_INVALID) {
					pCfgReqVal->wlancfg_type = CW_WLANCFG_TYPE_INVALID;
					return;
				}

				// parse and create struct of DeleteWlan
				if ((pCfgReqVal->wlan_cfg.deleteWlanCfg = CWParseMsgElemDeleteWLAN(&completeMsg,  elemLen)) == NULL) {
					pCfgReqVal->wlancfg_type = CW_WLANCFG_TYPE_INVALID;
					return;
				}
				pCfgReqVal->wlancfg_type = CW_WLANCFG_TYPE_DEL;
				break;
			}
			
			case CW_MSG_ELEMENT_IEEE80211_UPDATE_WLAN_CW_TYPE:
				/*
				if (wlancfg_type != CW_WLANCFG_TYPE_NONE)
				{
					*pResultCode = CW_PROTOCOL_FAILURE_UNRECOGNIZED_MSG_ELEM;
					CWLog("\tParse Err: More than one WLAN configuration, prev: (%u), now: UPDATE_WLAN(0)", wlancfg_type);
					return CW_FALSE;
				}
				// TODO
				wlancfg_type = CW_WLANCFG_TYPE_UPDATE;
				break;
				*/
				// TODO: AddWLAN should have RSN IE etc... WLAN UPDATE not supported yet.

				pCfgReqVal->wlancfg_type = CW_WLANCFG_TYPE_INVALID;
				return;

			default:
				CWProtocolSkipElement(&completeMsg,  elemLen);
				break;
		}
	}

	if(completeMsg.offset != len) {
//printf("completeMsg.offset=%d, len=%d\n", completeMsg.offset, len);
//		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}
}
#endif
#endif

#ifdef RTK_SMART_ROAMING
CWBool CWParseStationConfigurationRequest (unsigned char *msg, int len, CWProtocolResultCode *resultCode) 
{
	//CWBool bindingMsgElemFound=CW_FALSE;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Parsing Station Configuration Request...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	//valuesPtr->bindingValues = NULL;

	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		//CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);

		/*if(CWBindingCheckType(elemType))
		{
			bindingMsgElemFound=CW_TRUE;
			completeMsg.offset += elemLen;
			continue;	
		}*/
									
		switch(elemType) { 
			case CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE:
			{
				CWProtocolRequestStationValues del_sta;			
				if (!CWParseMsgElemDeleteStation(&completeMsg,  elemLen, &del_sta))
					return CW_FALSE;

				*resultCode = cwRtkWtpDeleteStation(del_sta.radio_id, del_sta.mac_addr);

				break;
			}
			case CW_MSG_ELEMENT_FREE_STATION_CW_TYPE:
			{
				CWProtocolRequestStationValues free_sta;
				if (!CWParseMsgElemFreeStation(&completeMsg,  elemLen, &free_sta))
					return CW_FALSE;
				
				*resultCode = cwRtkWtpFreeStation(free_sta.radio_id, free_sta.mac_addr);
							
				break;
			}
			case CW_MSG_ELEMENT_SILENT_STATION_CW_TYPE:
			{
				CWProtocolRequestStationValues silent_sta;
				if (!CWParseMsgElemSilentStation(&completeMsg,  elemLen, &silent_sta))
					return CW_FALSE;

				*resultCode = cwRtkWtpSilentStation(silent_sta.radio_id, silent_sta.mac_addr);
							
				break;
			}			
			case CW_MSG_ELEMENT_UNSILENT_STATION_CW_TYPE:
			{
				CWProtocolRequestStationValues unsilent_sta;
				if (!CWParseMsgElemUnsilentStation(&completeMsg,  elemLen, &unsilent_sta))
					return CW_FALSE;

				*resultCode = cwRtkWtpUnsilentStation(unsilent_sta.radio_id, unsilent_sta.mac_addr);
							
				break;
			}
			case CW_MSG_ELEMENT_DOT11V_STATION_CW_TYPE:
			{
				CWProtocolRequestStationValues dot11v_sta;
				if(!CWParseMsgElemDot11vStation(&completeMsg, elemLen, &dot11v_sta))
					return CW_FALSE;

				*resultCode = cwRtkWtpDot11vStation(dot11v_sta.radio_id, dot11v_sta.mac_addr);

				break;
			}
			case CW_MSG_ELEMENT_DUAL_STATION_CW_TYPE:
			{
				CWProtocolRequestStationValues dual_sta;
				if(!CWParseMsgElemDualStation(&completeMsg, elemLen, &dual_sta))
					return CW_FALSE;
								
				*resultCode = cwRtkWtpDualStation(dual_sta.radio_id, dual_sta.mac_addr);
				
				break;
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	/*
	if(bindingMsgElemFound)
	{
		if(!(CWBindingParseConfigurationUpdateRequest (msg, len, &(valuesPtr->bindingValues))))
		{
			return CW_FALSE;
		}
	}*/

	CWLog("Station Configuration Request Parsed");
	
	return CW_TRUE;
}

static CWBool CWParseWTPEventResponseMessage (CWControlHeaderValues *controlVal, int len)
{
	
	if(controlVal == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	//CWLog("Parsing WTP Event Response...");
	
	if(len != 0 )
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "WTP Event Response must carry no message element");
	
	//CWLog("WTP Event Response Parsed...");

	return CW_TRUE;
}
#endif

CWBool CWParseResetRequest (unsigned char *msg, int len,
						  CWProtocolImageIdentifierValues *image_identifier, CWProtocolResultCode *resultCode)
{
	CWBool imageIdentifierFound=CW_FALSE;
	CWProtocolMessage completeMsg;
	
	CWLog("Parsing Reset Request...");
	*resultCode = CW_PROTOCOL_SUCCESS;

	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	/* parse message elements */
	while(completeMsg.offset < len) {

		unsigned short int elemType=0;	/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int elemLen=0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		switch(elemType) {
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:
				CWParseMsgElemImageIdentifier(&completeMsg, elemLen, image_identifier);
				CWLog("Got Reset Request - image identifier: vedor id=%d, image id=%s.", image_identifier->vendor_id, image_identifier->imageIdentifier);
				imageIdentifierFound = CW_TRUE;
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT,
							"Unrecognized Message Element");
		}
	}

	if (completeMsg.offset != len) {
		//printf("%s Garbage at the End of the Message\n", __FUNCTION__);
//		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}

	if (!imageIdentifierFound) {
		*resultCode = CW_PROTOCOL_FAILURE_MISSING_MSG_ELEM;
	}

	CWLog("Reset Request Parsed");
	
	return CW_TRUE;
}

#ifdef RTK_CAPWAP

CWBool CWParseRtkWTPStatusRequest (unsigned char *msg, int len, CWProtocolRtkWTPStatusRequestValues *pStatusReq)
{
	CWProtocolMessage completeMsg;

	// begin to parse
	completeMsg.msg = msg;
	completeMsg.offset = 0; 

	pStatusReq->siteSurveyReq = CW_FALSE;
	pStatusReq->stationReq = CW_FALSE;
	CWLog("Parsing RTK WTP Status Request...");
	while(completeMsg.offset < len) {
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);
		CWLog("\telemType=%u", elemType);

		switch(elemType) { 
			case CW_MSG_ELEMENT_RTK_SITE_SURVEY_REQUEST_CW_TYPE:
			{
				pStatusReq->siteSurveyReq = CW_TRUE;
				break;
			}
			case CW_MSG_ELEMENT_RTK_STATION_REQUEST_CW_TYPE:
			{
				pStatusReq->stationReq = CW_TRUE;
				break;
			}
			default:
				CWProtocolSkipElement(&completeMsg,  elemLen);
				break;
		}
	}
	if(completeMsg.offset != len) {
//printf("completeMsg.offset=%d, len=%d\n", completeMsg.offset, len);
//		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	}

	
	CWLog("RTK WTP Status Request Parsed");
	return CW_TRUE;
}

#endif

/*______________________________________________________________*/
/*  *******************___SAVE FUNCTIONS___*******************  */
CWBool CWSaveWTPEventResponseMessage (void *WTPEventResp) {

	CWDebugLog("Saving WTP Event Response...");
	CWDebugLog("WTP Response Saved");
	return CW_TRUE;
}

/*Update 2009:
	Save a vendor message (mainly UCI configuration messages)*/
CWBool CWSaveVendorMessage(void* protocolValuesPtr, CWProtocolResultCode* resultCode) {
	if(protocolValuesPtr==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	*resultCode = CW_PROTOCOL_SUCCESS;

	CWProtocolVendorSpecificValues* vendorPtr=(CWProtocolVendorSpecificValues *)protocolValuesPtr; 

	/*Find out which custom vendor paylod really is...*/
	switch(vendorPtr->vendorPayloadType) {
		case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI:
			if(!CWWTPSaveUCIValues((CWVendorUciValues *)(vendorPtr->payload), resultCode))
			{
				CW_FREE_OBJECT(((CWVendorUciValues *)vendorPtr->payload)->commandArgs);
				CW_FREE_OBJECT(vendorPtr->payload);
				CW_FREE_OBJECT(vendorPtr);
				return CW_FALSE;
			}
		break;

		case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_WUM:
			if(!CWWTPSaveWUMValues((CWVendorWumValues *)(vendorPtr->payload), resultCode))
			{
				CW_FREE_OBJECT(vendorPtr->payload);
				CW_FREE_OBJECT(vendorPtr);
				return CW_FALSE;
			}
		break;
	}

	return CW_TRUE;
}

void CWSaveConfigurationUpdateRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr,
										CWProtocolResultCode* resultCode,
										int updateRequestType, 
										char *newImageVersion)
{
	*resultCode = CW_PROTOCOL_SUCCESS;
	newImageVersion[0] = '\0';
	
	// 2013 Babylon: add for image identifier
	if (updateRequestType == CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE) {
CWDebugLog("vendorID = %d %d", cwRtkWtpGetVendorId(), valuesPtr->image_identifier.vendor_id);		
		if (cwRtkWtpGetVendorId() == valuesPtr->image_identifier.vendor_id) {
			if (strcmp(cwRtkWtpGetCurrentFwId(), valuesPtr->image_identifier.imageIdentifier) == 0) {

				*resultCode = CW_PROTOCOL_IMG_FAILURE_IMAGE_ALREADY_PRESENT;
			} else {
				// going to have image download.
				strcpy(newImageVersion, valuesPtr->image_identifier.imageIdentifier);
				*resultCode = CW_PROTOCOL_SUCCESS;
			}
		} else {
			// not for this vendor
			*resultCode = CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED;
		}
		return;
	}
// ------------------------------------------------------------------------------
	int i;
	CWBool all_success=CW_TRUE, need_reload=CW_FALSE;
#ifdef RTK_CAPWAP
	// RTK Wlan configuration
	if (valuesPtr->nRtkWlanCfg > 0) {		
//printf("WTP save wlans' config\n");
		CWBool driver_flash_unsync;
		if (!cwRtkWtpCfgWlans(valuesPtr->nRtkWlanCfg,valuesPtr->rtkWlanCfg, CW_FALSE, CW_FALSE, &driver_flash_unsync)) {
			all_success = CW_FALSE;
			*resultCode = CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED;
		} else {
			if (driver_flash_unsync) 
				need_reload = CW_TRUE;
		}
	}

	// Power Scale
	if (valuesPtr->nRtkPowerScaleCfg> 0) {
		CWBool driver_flash_unsync;
//printf("WTP save power scales\n");
		if (!cwRtkWtpCfgPowerScales(valuesPtr->nRtkPowerScaleCfg, valuesPtr->rtkPowerScaleCfg, CW_TRUE, &driver_flash_unsync)) {
			all_success = CW_FALSE;
			*resultCode = CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED;
		} else {
			if (driver_flash_unsync) 
				need_reload = CW_TRUE;
		}
	}

	// Channel
	if (valuesPtr->nRtkChannelCfg> 0) {
		CWBool driver_flash_unsync;
//printf("WTP save channels\n");
		if (!cwRtkWtpCfgChannels(valuesPtr->nRtkChannelCfg, valuesPtr->rtkChannelCfg, CW_TRUE, &driver_flash_unsync)) {
			all_success = CW_FALSE;
			*resultCode = CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED;
		} else {
			if (driver_flash_unsync) 
				need_reload = CW_TRUE;
		}
	}
	
	if (all_success && need_reload) {
		cwRtkWtpActivateFlashCfg();
	}

	// qos
	for (i=0; i<valuesPtr->nWtpQos; i++) {
		if(cwRtkWtpSetQos(&valuesPtr->wtpQos[i]) != CW_PROTOCOL_SUCCESS) {
			all_success = CW_FALSE;
			*resultCode = CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED;
		}
	}
#else
	if (valuesPtr->bindingValues!=NULL) {
		CWProtocolResultCode r;
		if(!CWBindingSaveConfigurationUpdateRequest(valuesPtr->bindingValues, &r, updateRequestType)) {
			*resultCode = r;	
			all_success = CW_FALSE;
		}
	}
#endif

	if (valuesPtr->protocolValues!=NULL) {
		/*Update 2009:
			We have a msg which is not a 
			binding specific message... */
			CWProtocolResultCode r;
		if(!CWSaveVendorMessage(valuesPtr->protocolValues, &r))  
			*resultCode = r;
	}	
}

CWBool CWSaveClearConfigurationRequest(CWProtocolResultCode* resultCode)
{
	*resultCode=CW_PROTOCOL_SUCCESS; 
	
	/*Back to manufacturing default configuration*/

	if ( !CWErr(CWWTPLoadConfiguration()) || !CWErr(CWWTPInitConfiguration()) ) 
	{
			CWLog("Can't restore default configuration...");
			return CW_FALSE;
	}

	return CW_TRUE;
}


// -----------------

/*
CWBool CWWTPManageACRunRequest(char *msg, int len)
{
	CWControlHeaderValues controlVal;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	switch(controlVal.messageTypeValue) {
		case CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST:
			break;
		case CW_MSG_TYPE_VALUE_ECHO_REQUEST:
			break;
		case CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_REQUEST:
			break;
		case CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST:
			break;
		default:
			return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Change State Event Response as Expected");
	}

	
	
	//if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	if(controlVal.msgElemsLen != 0 ) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Change State Event Response must carry no message elements");

	CWDebugLog("Change State Event Response Parsed");




	CWDebugLog("#########################");
	CWDebugLog("###### STO DENTRO #######");
	CWDebugLog("#########################");

	return CW_TRUE;
}
*/

#ifdef KM_BY_AC
#ifndef SOFTMAC
void CWSaveWLANConfigurationRequest (const CWProtocolWlanConfigurationRequestValues *pCfgReqVal, 
										CWProtocolWlanConfigurationResponseValues *pCfgRspVal)
{
	pCfgRspVal->wlancfg_type = pCfgReqVal->wlancfg_type;
	switch(pCfgReqVal->wlancfg_type)
	{
		case CW_WLANCFG_TYPE_ADD:
			CWLog("Adding an WLAN...");
			pCfgRspVal->result_code = cwRtkWtpAddWlan(pCfgReqVal->wlan_cfg.addWlanCfg, pCfgRspVal->bssid);
			break;

		case CW_WLANCFG_TYPE_DEL:
			//TODO
		case CW_WLANCFG_TYPE_UPDATE:
			//TODO
			
		case CW_WLANCFG_TYPE_INVALID:
		default: 
			CWLog("Adding an WLAN...");
			pCfgRspVal->result_code = CW_PROTOCOL_FAILURE_UNRECOGNIZED_MSG_ELEM;
			break;
	}
}
#endif
#endif

void CWConfirmRunStateToACWithEchoRequest() {

	CWList msgElemList = NULL;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum = 0;
	int seqNum;

	CWLog("\n");
	CWLog("#________ Echo Request Message (Confirm Run) ________#");
	
	/* Send WTP Event Request */
	seqNum = CWGetSeqNum();

	if(!CWAssembleEchoRequest(&messages,
				  &fragmentsNum,
				  gWTPPathMTU,
				  seqNum,
				  msgElemList)){
		int i;

		CWDebugLog("Failure Assembling Echo Request");
		if(messages)
			for(i = 0; i < fragmentsNum; i++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[i]);
			}	
		CW_FREE_OBJECT(messages);
		return;
	}

#if 1	
	int i;
	for(i = 0; i < fragmentsNum; i++) {
#ifdef CW_NO_DTLS
//printf("(%s) Echo Request no dtls send \n", __FUNCTION__);
		if(!CWNetworkSendUnsafeConnected(gWTPSocket, messages[i].msg, messages[i].offset))
#else
//printf("(%s) Echo Request security send \n", __FUNCTION__);
		if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
#endif
		{
			CWLog("Failure sending Request");
			int k;
			for(k = 0; k < fragmentsNum; k++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[k]);
			}	
			CW_FREE_OBJECT(messages);
			break;
		}
	}
#else
	int i, j;			
	CWBool sendResult;
	for(i = 0; i < fragmentsNum; i++) {
		for (j=0; j<5; j++) {
#ifdef CW_NO_DTLS
			sendResult = CWNetworkSendUnsafeConnected(gWTPSocket, messages[i].msg, messages[i].offset);
#else
			sendResult = CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset);
#endif
			if (sendResult) break;
			sleep(3);
			CWLog("Retry send Echo Request, fragment index: %d, times:%d", i, j+1);
		}
		if (!sendResult) {
			CWLog("Failure sending Echo Request");
			break;
		}
	}
#endif

	int k;
	for(k = 0; messages && k < fragmentsNum; k++) {
		CW_FREE_PROTOCOL_MESSAGE(messages[k]);
	}	
	CW_FREE_OBJECT(messages);

}


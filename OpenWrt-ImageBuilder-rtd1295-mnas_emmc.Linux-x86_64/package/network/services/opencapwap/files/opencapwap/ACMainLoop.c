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
 *           Daniele De Sanctis (danieledesanctis@gmail.com)									* 
 *	         Antonio Davoli (antonio.davoli@gmail.com)											*
 ************************************************************************************************/

#include "CWAC.h"
#include "ACAppsProtocol.h"
#include "ACRtkSystemInteraction.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

/* index of the current thread in the global array */
CWThreadSpecific gIndexSpecific; //per thread has one its own gIndexSpecific

int gCWWaitJoin = CW_WAIT_JOIN_DEFAULT;

CW_THREAD_RETURN_TYPE CWManageWTP(void *arg);
CW_THREAD_RETURN_TYPE CWManageTimers(void *arg);
void CWCriticalTimerExpiredHandler(int arg);
void CWSoftTimerExpiredHandler(int arg);

void CWACManageIncomingPacket(CWSocket sock,
			      unsigned char *buf,
			      int len,
			      int incomingInterfaceIndex,
			      CWNetworkLev4Address *addrPtr,
			      CWBool dataFlag );
void _CWCloseThread(int i);
void CWResetWTPProtocolManager(CWWTPProtocolManager *WTPProtocolManager);
void CWInitWTPProtocolManager(CWWTPProtocolManager *WTPProtocolManager);
__inline__ CWWTPManager *CWWTPByName(const char *addr);
__inline__ CWWTPManager *CWWTPByAddress(CWNetworkLev4Address *addressPtr,
					CWSocket sock);


CWBool ac_run = CW_TRUE;

void turnOffAC()
{
	ac_run = CW_FALSE;
}

CWBool CWIsACRun()
{
	return ac_run;
}

void CWACEnterMainLoop() {

	struct sigaction act;
	
	CWLog("AC enters in the MAIN_LOOP");
	
	/* set signals
	 * all the thread we spawn will inherit these settings
	 */
	memset(&act, 0, sizeof(act));
	act.sa_handler = turnOffAC;
	sigaction(SIGINT, &act, NULL);

	/*
	 * BUG UMR03
	 *
	 * 20/10/2009 - Donato Capitella 
	 */
	sigemptyset(&act.sa_mask);

	act.sa_flags = 0;
	/* called when a timer requested by the thread has expired */
	act.sa_handler = CWCriticalTimerExpiredHandler;
	sigaction(CW_CRITICAL_TIMER_EXPIRED_SIGNAL, &act, NULL);
	
	act.sa_flags = 0;
	/* called when a timer requested by the thread has expired */
	act.sa_handler = CWSoftTimerExpiredHandler;
	sigaction(CW_SOFT_TIMER_EXPIRED_SIGNAL, &act, NULL);
	
	/* signals will be unblocked by the threads that needs timers */
	CWThreadSetSignals(SIG_BLOCK, 2, CW_CRITICAL_TIMER_EXPIRED_SIGNAL,
 					 CW_SOFT_TIMER_EXPIRED_SIGNAL);

	if(!(CWThreadCreateSpecific(&gIndexSpecific, NULL))) {
		CWLog("Critical Error With Thread Data");
		exit(1);
	}

//disable wum thread for memory and cpu usage
#if 0
	CWThread thread_interface;
	if(!CWErr(CWCreateThread(&thread_interface, CWInterface, NULL))) {
		CWLog("Error starting Interface Thread");
		exit(1);
	}
#endif
	
	CW_REPEAT_FOREVER {
		/* CWACManageIncomingPacket will be called 
		 * when a new packet is ready to be read 
		 */
		if(!CWErr(CWNetworkUnsafeMultiHomed(&gACSocket, 
						    CWACManageIncomingPacket,
						    CW_FALSE))) {
			CWLog("Error Loop CWNetworkUnsafeMultiHomed()");
			exit(1);
		}		
		if (!CWIsACRun()) {
			CWLog("ACMainLoop exit");
			break;
		}
	}
}

/* argument passed to the thread func */
typedef struct {
	int index;
	CWSocket sock;
	int interfaceIndex;
} CWACThreadArg;

/*
 * This callback function is called when there is something to read in a 
 * CWMultiHomedSocket (see ACMultiHomed.c).
 * 
 * Params: sock,	is the socket that can receive the packet and it can be
 * 			used to reply.
 * 	   buf,		(array of len chars) contains the packet which is ready
 * 	   		on the socket's queue (obtained with MSG_PEEK).
 *	   incomingInterfaceIndex,  is the index (different from the system 
 *	   			    index, see ACMultiHomed.c) of the interface
 *	   			    the packet was sent to, in the array returned
 *	   			    by CWNetworkGetInterfaceAddresses. If the
 *	   			    packet was sent to a broadcast/multicast address,
 *	   			    incomingInterfaceIndex is -1.
 */
void CWACManageIncomingPacket(CWSocket sock,
			      unsigned char *buf,
			      int readBytes,
			      int incomingInterfaceIndex,
			      CWNetworkLev4Address *addrPtr,CWBool dataFlag) {
 
	CWWTPManager *wtpPtr = NULL;
	char* pData;
		
	/* check if sender address is known */
	wtpPtr = CWWTPByAddress(addrPtr, sock);
	if ((wtpPtr != NULL) && dataFlag && (wtpPtr->dataaddress.ss_family == AF_UNSPEC)) {
		CW_COPY_NET_ADDR_PTR(&(wtpPtr->dataaddress), addrPtr);
	}

	if(wtpPtr != NULL) {
		/* known WTP */
		/* Clone data packet */
		CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWLog("Out Of Memory"); return; });
		memcpy(pData, buf, readBytes);

		CWLockSafeList(wtpPtr->packetReceiveList);
		CWAddElementToSafeListTailwitDataFlag(wtpPtr->packetReceiveList, pData, readBytes,dataFlag);
		CWUnlockSafeList(wtpPtr->packetReceiveList);
	} else { 
		/* unknown WTP */
		int seqNum;
		CWDiscoveryRequestValues values;
		
		if(!CWErr(CWThreadMutexLock(&gActiveWTPsMutex))) 
			exit(1);
			
		CWThreadMutexUnlock(&gActiveWTPsMutex);

		if(gActiveWTPs >= gMaxWTPs) {

			CWLog("Too many WTPs, gActiveWTPs=%d, gMaxWTPs=%d", gActiveWTPs, gMaxWTPs);
			return;
		}
		CWLog("got packet from unknown WTP\n");	
		
		if(CWErr(CWParseDiscoveryRequestMessage(buf, readBytes, &seqNum, &values))) {
		
			CWProtocolMessage *msgPtr;
		
			CWLog("\n");
			CWLog("######### Discovery State #########");

			CWUseSockNtop(addrPtr, CWLog("CAPWAP Discovery Request from %s", str););
	
			/* don't add this WTP to our list to minimize DoS 
			 * attacks (will be added after join) 
			 */

			/* destroy useless values */
			CWDestroyDiscoveryRequestValues(&values);
			
			/* send response to WTP 
			 * note: we can consider reassembling only changed part
			 * AND/OR do this in a new thread.
			 */
			if(!CWErr(CWAssembleDiscoveryResponse(&msgPtr, seqNum))) {
				/* 
				 * note: maybe an out-of-memory memory error 
				 * can be resolved without exit()-ing by 
				 * killing some thread or doing other funky 
				 * things.
				 */
				CWLog("Critical Error Assembling Discovery Response");
				exit(1);
			}

			if(!CWErr(CWNetworkSendUnsafeUnconnected(sock,
								 addrPtr,
								 (*msgPtr).msg,
								 (*msgPtr).offset))) {

				CWLog("Critical Error Sending Discovery Response");
				exit(1);
			}
			
			CW_FREE_PROTOCOL_MESSAGE(*msgPtr);
			CW_FREE_OBJECT(msgPtr);
		} else { 
			CWLog("Got Non-Discovery Packet From an Unkown WTP.");
			/* this isn't a Discovery Request */
			int i;
			CWACThreadArg *argPtr;
			
			CWUseSockNtop(addrPtr, CWDebugLog("Possible Client Hello from %s", str););
			
			if(!CWErr(CWThreadMutexLock(&gWTPsMutex))) exit(1);
			/* look for the first free slot */
			for(i = 0; i < gMaxWTPs && gWTPs[i].isNotFree; i++);

			CW_COPY_NET_ADDR_PTR(&(gWTPs[i].address), addrPtr);
			gWTPs[i].dataaddress.ss_family = AF_UNSPEC;
			gWTPs[i].isNotFree = CW_TRUE;
			gWTPs[i].isRequestClose = CW_FALSE;
			CWThreadMutexUnlock(&gWTPsMutex);

			/* Capwap receive packets list */
			if (!CWErr(CWCreateSafeList(&gWTPs[i].packetReceiveList))) {

				if(!CWErr(CWThreadMutexLock(&gWTPsMutex))) 
					exit(1);
				gWTPs[i].isNotFree = CW_FALSE;
				
				CWLog("cannot create safe list idx=%d", i);
				CWThreadMutexUnlock(&gWTPsMutex);
				return;
			}
			
			CWSetMutexSafeList(gWTPs[i].packetReceiveList, 
					   &gWTPs[i].interfaceMutex);
			CWSetConditionSafeList(gWTPs[i].packetReceiveList,
					       &gWTPs[i].interfaceWait);

			/* Capwap active work list */
			if (!CWErr(CWCreateSafeList(&gWTPs[i].activeWorkList))) {
				CWDestroySafeList(gWTPs[i].packetReceiveList);
				if(!CWErr(CWThreadMutexLock(&gWTPsMutex))) 
					exit(1);
				gWTPs[i].isNotFree = CW_FALSE;
				CWThreadMutexUnlock(&gWTPsMutex);
				return;
			}
			
			CWSetMutexSafeList(gWTPs[i].activeWorkList, 
					   &gWTPs[i].interfaceMutex);
			CWSetConditionSafeList(gWTPs[i].activeWorkList,
						   &gWTPs[i].interfaceWait);

			/* thread arguments */
			CW_CREATE_OBJECT_ERR(argPtr, CWACThreadArg, { CWLog("%s: Out Of Memory"); return; });

			argPtr->index = i;
			argPtr->sock = sock;
			argPtr->interfaceIndex = incomingInterfaceIndex;
						
			/* 
			 * If the packet was addressed to a broadcast address,
			 * just choose an interface we like (note: we can consider
			 * a bit load balancing instead of hard-coding 0-indexed 
			 * interface). Btw, Join Request should not really be 
			 * accepted if addressed to a broadcast address, so we 
			 * could simply discard the packet and go on.
			 * If you leave this code, the WTP Count will increase 
			 * for the interface we hard-code here, even if it is not
			 * necessary the interface we use to send packets to that
			 * WTP. If we really want to accept Join Request from 
			 * broadcast address, we can consider asking to the kernel
			 * which interface will be used to send the packet to a 
			 * specific address (if it remains the same) and than 
			 * increment WTPCount for that interface instead of 0-indexed one.
			 */
			if (argPtr->interfaceIndex < 0) argPtr->interfaceIndex = 0; 
			
			/* create the thread that will manage this WTP */
			if(!CWErr(CWCreateThread(&(gWTPs[i].thread), CWManageWTP, argPtr))) {

				CW_FREE_OBJECT(argPtr);
				if(!CWErr(CWThreadMutexLock(&gWTPsMutex))) 
					exit(1);
				
				CWDestroySafeList(gWTPs[i].packetReceiveList);
				CWDestroySafeList(gWTPs[i].activeWorkList);
				gWTPs[i].isNotFree = CW_FALSE;
				CWThreadMutexUnlock(&gWTPsMutex);
				
				return;
			}

			/* wait CWManageWTP() to finish initialization */
			while(gWTPs[i].subState != CW_WAITING_REQUEST){
				usleep(100);
			}
			
			/* Clone data packet */
			CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWLog("Out Of Memory"); return; });
			memcpy(pData, buf, readBytes);

			CWLockSafeList(gWTPs[i].packetReceiveList);
			CWAddElementToSafeListTailwitDataFlag(gWTPs[i].packetReceiveList,
 						   pData,
						   readBytes,
						  dataFlag);
			CWUnlockSafeList(gWTPs[i].packetReceiveList);
		}
	}
}

/*
 * Simple job: see if we have a thread that is serving address *addressPtr
 */
__inline__ CWWTPManager *CWWTPByAddress(CWNetworkLev4Address *addressPtr, CWSocket sock) {

	int i;
	if(addressPtr == NULL) return NULL;
	CWThreadMutexLock(&gWTPsMutex);
	for(i = 0; i < gMaxWTPs; i++) {
		if(gWTPs[i].isNotFree && 
		   !sock_cmp_addr((struct sockaddr*)addressPtr, 
			   	  (struct sockaddr*)&(gWTPs[i].address), 
				  sizeof(CWNetworkLev4Address)) )
		{ 
		
			/* we treat a WTP that sends packet to a different 
			 * AC's interface as a new WTP
			 */
			CWThreadMutexUnlock(&gWTPsMutex);
			return &(gWTPs[i]);
		}
	}
	
	CWThreadMutexUnlock(&gWTPsMutex);
	
	return NULL;
}

#ifdef RTK_SMART_ROAMING
CWBool CWAssembleNotifyToWTP(int notify_index, int wtp_index, int pendingIndex, int seqNum) {
	switch (gWTPs[wtp_index].notifyCommand[notify_index]) {
		case FREE_NOTIFY:
		{
			CWRoamLog("Request WTP%d wlan%d free info of %02x:%02x:%02x:%02x:%02x:%02x",
					wtp_index, gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.radio_id,
					gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.mac_addr[0],
					gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.mac_addr[1],
					gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.mac_addr[2],
					gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.mac_addr[3],
					gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.mac_addr[4],
					gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.mac_addr[5]);
														
			if (!CWAssembleStationConfigurationRequest(&gWTPs[wtp_index].PendingReq[pendingIndex].msg,
						&gWTPs[wtp_index].PendingReq[pendingIndex].fragNum,
						gWTPs[wtp_index].pathMTU,
						seqNum,
						gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.mac_addr,
						gWTPs[wtp_index].RoamingRequest[notify_index].free_sta.radio_id, 
						CW_MSG_ELEMENT_FREE_STATION_CW_TYPE))
			{
				CWDebugLog("\tCWAssembleStationConfigurationRequest FAIL!");						
				return CW_FALSE;				
			}		
			break;
		}
	
		case SILENT_NOTIFY:
		{
			CWRoamLog("Request WTP%d wlan%d keep silent to %02x:%02x:%02x:%02x:%02x:%02x",
					wtp_index, gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.radio_id,
					gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.mac_addr[0],
					gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.mac_addr[1],
					gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.mac_addr[2],
					gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.mac_addr[3],
					gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.mac_addr[4],
					gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.mac_addr[5]);
			if (!CWAssembleStationConfigurationRequest(&gWTPs[wtp_index].PendingReq[pendingIndex].msg,
						&gWTPs[wtp_index].PendingReq[pendingIndex].fragNum,
						gWTPs[wtp_index].pathMTU,
						seqNum,
						gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.mac_addr,
						gWTPs[wtp_index].RoamingRequest[notify_index].silent_sta.radio_id, 
						CW_MSG_ELEMENT_SILENT_STATION_CW_TYPE))
			{
				CWDebugLog("\tCWAssembleStationConfigurationRequest FAIL!");						
				return CW_FALSE;				
			}				
			break;
			
		}
	
		case UNSILENT_NOTIFY:
		{
			CWRoamLog("Request WTP%d wlan%d un-silent to %02x:%02x:%02x:%02x:%02x:%02x",
					wtp_index, gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.radio_id,
					gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.mac_addr[0],
					gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.mac_addr[1],
					gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.mac_addr[2],
					gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.mac_addr[3],
					gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.mac_addr[4],
					gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.mac_addr[5]);
			if (!CWAssembleStationConfigurationRequest(&gWTPs[wtp_index].PendingReq[pendingIndex].msg,
						&gWTPs[wtp_index].PendingReq[pendingIndex].fragNum,
						gWTPs[wtp_index].pathMTU,
						seqNum,
						gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.mac_addr,
						gWTPs[wtp_index].RoamingRequest[notify_index].unsilent_sta.radio_id, 
						CW_MSG_ELEMENT_UNSILENT_STATION_CW_TYPE))
			{
				CWDebugLog("\tCWAssembleStationConfigurationRequest FAIL!");						
				return CW_FALSE;				
			}				
			break;
			
		}

		case DOT11V_NOTIFY:
		{				
			CWRoamLog("Request WTP%d wlan%d issue 802.11v command to %02x:%02x:%02x:%02x:%02x:%02x",
					wtp_index, gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.radio_id,
					gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.mac_addr[0],
					gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.mac_addr[1],
					gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.mac_addr[2],
					gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.mac_addr[3],
					gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.mac_addr[4],
					gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.mac_addr[5]);

			if (!CWAssembleStationConfigurationRequest(&gWTPs[wtp_index].PendingReq[pendingIndex].msg,
						&gWTPs[wtp_index].PendingReq[pendingIndex].fragNum,
						gWTPs[wtp_index].pathMTU,
						seqNum,
						gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.mac_addr,
						gWTPs[wtp_index].RoamingRequest[notify_index].dot11v_sta.radio_id, 
						CW_MSG_ELEMENT_DOT11V_STATION_CW_TYPE))
			{
				CWDebugLog("\tCWAssembleStationConfigurationRequest FAIL!");						
				return CW_FALSE;				
			}
			break;							
		}
		case DUALBAND_NOTIFY:
		{
			CWRoamLog("Notify WTP%d wlan%d of dual-band %02x:%02x:%02x:%02x:%02x:%02x",
					wtp_index, gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.radio_id,
					gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.mac_addr[0],
					gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.mac_addr[1],
					gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.mac_addr[2],
					gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.mac_addr[3],
					gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.mac_addr[4],
					gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.mac_addr[5]);
			if (!CWAssembleStationConfigurationRequest(&gWTPs[wtp_index].PendingReq[pendingIndex].msg,
					&gWTPs[wtp_index].PendingReq[pendingIndex].fragNum,
					gWTPs[wtp_index].pathMTU,
					seqNum,
					gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.mac_addr,
					gWTPs[wtp_index].RoamingRequest[notify_index].dual_sta.radio_id, 
					CW_MSG_ELEMENT_DUAL_STATION_CW_TYPE))
			{						
				
				CWDebugLog("\tCWAssembleStationConfigurationRequest FAIL!");						
				return CW_FALSE;				
			}
			break;
		}
		
	}
	return CW_TRUE;
	
}
#endif

/* 
 * Session's thread function: each thread will manage a single session 
 * with one WTP.
 */
CW_THREAD_RETURN_TYPE CWManageWTP(void *arg) {

	int 		i = ((CWACThreadArg*)arg)->index;
	CWSocket 	sock = ((CWACThreadArg*)arg)->sock;
	int 		interfaceIndex = ((CWACThreadArg*)arg)->interfaceIndex;

	CW_FREE_OBJECT(arg);
	
	if(!(CWThreadSetSpecific(&gIndexSpecific, &i))) {

		CWLog("Critical Error with Thread Data");
		_CWCloseThread(i);
	}

	if(!CWErr(CWThreadMutexLock(&gActiveWTPsMutex))) 
		exit(1);

	gActiveWTPs++;

	gInterfaces[interfaceIndex].WTPCount++;
	CWUseSockNtop(((struct sockaddr*) &(gInterfaces[interfaceIndex].addr)),
				  CWDebugLog("One more WTP on %s (%d)", str, interfaceIndex);
				  );
	
	CWThreadMutexUnlock(&gActiveWTPsMutex);

	CWACInitBinding(i);
	
	gWTPs[i].interfaceIndex = interfaceIndex;
	gWTPs[i].socket = sock;
	
	gWTPs[i].fragmentsList = NULL;
	/* we're in the join state for this session */
	gWTPs[i].currentState = CW_ENTER_JOIN;
	gWTPs[i].subState = CW_DTLS_HANDSHAKE_IN_PROGRESS;

	
	/**** ACInterface ****/
	gWTPs[i].interfaceCommandProgress = CW_FALSE;
	gWTPs[i].interfaceCommand = NO_CMD;
	gWTPs[i].applicationIndex = -1;
	CWDestroyThreadMutex(&gWTPs[i].interfaceMutex);	
	CWCreateThreadMutex(&gWTPs[i].interfaceMutex);
	CWDestroyThreadCondition(&gWTPs[i].interfaceWait);	
	CWCreateThreadCondition(&gWTPs[i].interfaceWait);
	CWDestroyThreadCondition(&gWTPs[i].interfaceComplete);	
	CWCreateThreadCondition(&gWTPs[i].interfaceComplete);
	gWTPs[i].qosValues = NULL;
	gWTPs[i].pRtkPowerScale = NULL;	
	gWTPs[i].pRtkChannel = NULL;	
	//for image update
	gWTPs[i].imageActionState = NO_ACTION;
	gWTPs[i].reqImageIdentifier[0] = '\0';
	gWTPs[i].pReqImageData = NULL;
	/**** ACInterface ****/

	gWTPs[i].echoRequestCounter = 0;
	gWTPs[i].lastEchoRequestTime = 0;
	gWTPs[i].messages = NULL;
 	gWTPs[i].messagesCount = 0;
 	gWTPs[i].isRetransmitting = CW_FALSE;
	gWTPs[i].retransmissionCount = 0;

	int k;
#ifdef RTK_SMART_ROAMING	
	for(k=0; k<AC_MAX_NOTIFY_NUM; k++)
		gWTPs[i].notifyCommand[k] = NO_NOTIFY;
	gWTPs[i].notifyCommandHead = 0;
	gWTPs[i].notifyCommandTail = 0;
	
	for(k=0 ; k<MAX_PENDING_REQUEST_MSGS; k++)
		CWACInitPendingRequest(&gWTPs[i].PendingReq[k]);
	CWCreateThreadMutex(&gWTPs[i].PendingReqLock);
#endif
	for(k=0; k<SEQUENCE_ARRAY_SIZE; k++)
		gWTPs[i].RcvSeqNum[k] = k+1;


	CWInitWTPProtocolManager(&(gWTPs[i].WTPProtocolManager));

	CWLog("New Session for index %d", i);

	/* start WaitJoin timer */
	sleep(1);  // 2014 Babylon: bug reported by Tino: to let kernel have time to obtaid the pid.
	if(!CWErr(CWTimerRequest(gCWWaitJoin,
				 0,
				 &(gWTPs[i].thread),
				 &(gWTPs[i].currentTimer),
				 CW_CRITICAL_TIMER_EXPIRED_SIGNAL))) {

		CWCloseThread();
	}

#ifndef CW_NO_DTLS
	CWDebugLog("Init DTLS Session for index %d", i);

 	if(!CWErr(CWSecurityInitSessionServer(&gWTPs[i],
					      sock,
					      gACSecurityContext,
					      &((gWTPs[i]).session),
					      &(gWTPs[i].pathMTU)))) {

		CWDebugLog("Init DTLS Session failed for index  %d", i);
		CWTimerCancel(&(gWTPs[i].currentTimer));		
		CWCloseThread();
	}	
	CWDebugLog("Init DTLS Session success for index	%d", i);
#endif
	(gWTPs[i]).subState = CW_WAITING_REQUEST;

	if(gCWForceMTU > 0) gWTPs[i].pathMTU = gCWForceMTU;

	CWDebugLog("Path MTU for this Session: %d",  gWTPs[i].pathMTU);
	
	CW_REPEAT_FOREVER {
		int readBytes;
		CWProtocolMessage msg;
		CWBool dataFlag = CW_FALSE;

		
		if (!CWIsACRun()) {
			gWTPs[i].isRequestClose = CW_TRUE;
			CWDebugLog("gWTP[%d] thread: set isRequestClose to CW_TRUE", i);
		}

		msg.msg = NULL;
		msg.offset = 0;

		/* Wait WTP action */
//printf("(%s) Waiting condition gWTPs[i].interfaceWait\n", __FUNCTION__);

		CWThreadMutexLock(&gWTPs[i].interfaceMutex);

		while (gWTPs[i].isRequestClose == CW_FALSE
				&& CWGetCountElementFromSafeList(gWTPs[i].packetReceiveList) == 0
				&& CWGetCountElementFromSafeList(gWTPs[i].activeWorkList) == 0
				&& gWTPs[i].interfaceCommand == NO_CMD
#ifdef RTK_SMART_ROAMING
				&& gWTPs[i].notifyCommandHead == gWTPs[i].notifyCommandTail
#endif
				) {

			 /*TODO: Check system */
			CWWaitThreadCondition(&gWTPs[i].interfaceWait, 
					      &gWTPs[i].interfaceMutex);

//printf("(%s) Condition gWTPs[i].interfaceWait waked up\n", __FUNCTION__);
		}

		CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
//printf("(%s) Stop waiting condition gWTPs[%d].interfaceWait,Head=%d,Tail=%d\n", __FUNCTION__,i,gWTPs[i].notifyCommandHead,gWTPs[i].notifyCommandTail);

		if (gWTPs[i].isRequestClose) {
#ifdef KM_BY_AC // key management
			int j;
			for (j=0; j<gWTPs[i].WTPProtocolManager.radiosInfo.radioCount; j++) {
				CWACclose_hostapd(i, j);
				 // Babylon: Althohgh we do multi-times, 
				 //          actually only the first time terminates the process of hostapd. 
				 //          Other times are to remove the client we memorized in ACipcHostapd.c.
				 //          The command sent to hostapd will fail, but no harm.
			}
#else
			//printf("TODO: replace CWACclose_hostapd()\n");
#endif
			CWLog("Request close thread");
			_CWCloseThread(i);
		}

		CWThreadSetSignals(SIG_BLOCK, 
				   2,
				   CW_SOFT_TIMER_EXPIRED_SIGNAL,
				   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);

		if (CWGetCountElementFromSafeList(gWTPs[i].activeWorkList) > 0 && 
			gWTPs[i].currentState == CW_ENTER_RUN && 
			gWTPs[i].responseSeqNum == 0) {

			ac_active_work_t *work;

			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
			work = CWRemoveHeadElementFromSafeList(gWTPs[i].activeWorkList, NULL);
			CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);

			switch(work->type)
			{
#ifndef KM_BY_AC // key management
			case WTP_ADD_WLAN_CMD:	
				// TODO				

#if 0
				int seqNum; 
				seqNum = CWGetSeqNum();
				if (CWAssembleWLANConfigurationRequestAddWlan(&gWTPs[i].messages, &gWTPs[i].messagesCount,
													  gWTPs[i].pathMTU, seqNum, &work->data.wtp_add_wlan)) {	
					if(!CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_WLAN_CONFIGURATION_RESPONSE, seqNum))
						CWACStopRetransmission(i);
				}
#endif
				break;


#endif
			default:
				CWLog("Got Unknown CMD in activeWorkList.");
				break;
			}
			CW_FREE_OBJECT(work);
		} else if (	gWTPs[i].currentState == CW_ENTER_RUN && 
			gWTPs[i].responseSeqNum == 0 &&
			gWTPs[i].interfaceCommand != NO_CMD) {

			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
		  
			if (gWTPs[i].interfaceCommand != NO_CMD) {
				
				CWBool bResult = CW_FALSE;
				CWLog("Got WUM Command = %d", gWTPs[i].interfaceCommand);			
				switch (gWTPs[i].interfaceCommand) {
				case QOS_CMD:
					  {
						int seqNum = CWGetSeqNum();
						
						/* CWDebugLog("~~~~~~seq num in Check: %d~~~~~~", seqNum); */
						if (CWAssembleConfigurationUpdateRequest(&(gWTPs[i].messages), 
																 &(gWTPs[i].messagesCount),
																 gWTPs[i].pathMTU,
																 seqNum, CW_MSG_ELEMENT_IEEE80211_WTP_RADIO_INFORMATION_CW_TYPE, NULL)) {
						  
						  if(CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum)) 
							bResult = CW_TRUE;
						  else
							CWACStopRetransmission(i);
						}
						break;
					  }
				case CLEAR_CONFIG_MSG_CMD:
					  {
						int seqNum = CWGetSeqNum();
						
								/* Clear Configuration Request */
						if (CWAssembleClearConfigurationRequest(&(gWTPs[i].messages),
																&(gWTPs[i].messagesCount),
																gWTPs[i].pathMTU, seqNum)) {
						  
						  if(CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_RESPONSE, seqNum)) 
										bResult = CW_TRUE;
						  else
							CWACStopRetransmission(i);
						}
						break;
					  }
				/********************************************************
				 * 2009 Update:											*
				 *				New switch case for OFDM_CONTROL_CMD	*
				 ********************************************************/
				  
				case OFDM_CONTROL_CMD: 
					  {
						int seqNum = CWGetSeqNum();
						
						  if (CWAssembleConfigurationUpdateRequest(&(gWTPs[i].messages), 
															 &(gWTPs[i].messagesCount),
															 gWTPs[i].pathMTU,
															 seqNum, BINDING_MSG_ELEMENT_TYPE_OFDM_CONTROL, NULL)) {
					  
						  if(CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum)) 
							bResult = CW_TRUE;
						  else
							CWACStopRetransmission(i);
						}
					  break;
					  }
				/*Update 2009
					Added case to manage UCI configuration command*/
				case UCI_CONTROL_CMD: 
					{
						int seqNum = CWGetSeqNum();

						  if (CWAssembleConfigurationUpdateRequest(&(gWTPs[i].messages), 
															 &(gWTPs[i].messagesCount),
															 gWTPs[i].pathMTU,
															 seqNum, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI, NULL)) {
					  
						  if(CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum)) 
							bResult = CW_TRUE;
						  else
							CWACStopRetransmission(i);
						}
						break;
					}
				case WTP_UPDATE_CMD:
					{
						int seqNum = CWGetSeqNum();
						if (CWAssembleConfigurationUpdateRequest(&(gWTPs[i].messages),
                                                             &(gWTPs[i].messagesCount),
                                                             gWTPs[i].pathMTU,
                                                             seqNum, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_WUM, NULL)) {

	                          if(CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum))
	                                bResult = CW_TRUE;
	                          else
	                                CWACStopRetransmission(i);
						}
						break;
					}
				/* 2013 Babylon: for Image Update */
				case WTP_IMAGE_CMD:
					{
						const char *fn;
						char stringToWum[128];
						int seqNum;
						int wtp_id;
						// check file 
						fn = ACGetImageFile((char *)gWTPs[i].extra_info_buffer);
						if ( !fn || access(fn, R_OK) ) {
							sprintf(stringToWum, "Error! Cannot access file %s for %s", fn, gWTPs[i].extra_info_buffer);
							//printf("babylon Test: %s\n", stringToWum);
							CWLog(stringToWum);
							CWResponseToWum(i, -1, strlen(stringToWum)+1, stringToWum);
							break;
						}

						// save fwid into flash
						if (cwRtkAcTransferWtpName2Id(gWTPs[i].WTPProtocolManager.name, &wtp_id)) {
//printf("test name=%s, wtp_id=%d\n", gWTPs[i].WTPProtocolManager.name, wtp_id);

#ifndef X86_SIMULATION
							if (!cwRtkAcSetFwId(wtp_id, (char *)gWTPs[i].extra_info_buffer)) {
								sprintf(stringToWum, "Cannot save firmware ID to flash");
								//printf("babylon Test: %s\n", stringToWum);
								CWLog(stringToWum);
								//CWResponseToWum(i, -1, strlen(stringToWum)+1, stringToWum);
								//break;
							} else {
								sprintf(stringToWum, "Saved firmware ID to flash");
								//printf("babylon Test: %s\n", stringToWum);
								CWLog(stringToWum);
							}
#elif defined _FAKE_AC_DB_
							if (!CWAcDbSetFwId(gWTPs[i].WTPProtocolManager.name, (char *)gWTPs[i].extra_info_buffer)) {
								sprintf(stringToWum, "Cannot save firmware ID to FAKE AC DB");
								//printf("babylon Test: %s\n", stringToWum);
								CWLog(stringToWum);
							//	CWResponseToWum(i, -1, strlen(stringToWum)+1, stringToWum);
							//	break;
							} else {
								sprintf(stringToWum, "Saved firmware ID to FAKE AC DB");
								//printf("babylon Test: %s\n", stringToWum);
								CWLog(stringToWum);
							}
#endif
						}

						// save fwId into gWTPs
						strncpy(gWTPs[i].wtpConfig.fwImageIdentifier, (char *)gWTPs[i].extra_info_buffer, CW_IMAGE_IDENTIFIER_MAX_SIZE);

						// check WTP ImageActionState
						if ( (gWTPs[i].imageActionState != NO_ACTION)) {
							sprintf(stringToWum, "Error! imageActionState = %d. Can't send image now.", gWTPs[i].imageActionState);
							//printf("babylon Test: %s\n", stringToWum);
							CWLog(stringToWum);
							CWResponseToWum(i, -1, strlen(stringToWum)+1, stringToWum);
							break;
						}

						// send message
						seqNum = CWGetSeqNum();							
						CWLog("# Configuration Update - Image #");
						if (!CWAssembleConfigurationUpdateRequest(&(gWTPs[i].messages), &(gWTPs[i].messagesCount),
																gWTPs[i].pathMTU, seqNum, 
																CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE,
																gWTPs[i].extra_info_buffer)) {
							sprintf(stringToWum, "Fail to assemble ConfigurationUpdateRequest");
							//printf("babylon Test: %s\n", stringToWum);
							CWLog(stringToWum);
							CWResponseToWum(i, -1, strlen(stringToWum)+1, stringToWum);
							break;
						} else {
							if(CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum)){
								gWTPs[i].imageActionState = WAITNG_FOR_CONFIGURATION_UPDATE_RESPONSE_FROM_WTP;
								strcpy(gWTPs[i].reqImageIdentifier, (char *)gWTPs[i].extra_info_buffer);
								bResult = CW_TRUE;
							} else {
								CWACStopRetransmission(i);
								sprintf(stringToWum, "Can't got wtp's response. Stopped!");
								//printf("babylon Test: %s\n", stringToWum);
								CWLog(stringToWum);
								CWResponseToWum(i, -1, strlen(stringToWum)+1, stringToWum);
								break;
							}
						}
						break;
					}
				case WTP_DEL_STATION_CMD:
#ifdef RTK_SMART_ROAMING
					{
						int seqNum = CWGetSeqNum();
						station_t *sta = (station_t *)gWTPs[i].extra_info_buffer;
						CWBool flag = CW_FALSE;

						while (!flag) { // can't send request if last is not response
							CWThreadMutexLock(&gStationReq.lock);
							if (gStationReq.seqNum == -1) {
								gStationReq.seqNum = seqNum;		
								gStationReq.sta_state = STATION_REQUESTING_DELETE;
								memcpy(gStationReq.sta_mac, sta->mac_addr, 6);
								flag = CW_TRUE;
							}
							CWThreadMutexUnlock(&gStationReq.lock);
						}
						if (CWAssembleStationConfigurationRequest(&gWTPs[i].messages, &gWTPs[i].messagesCount,
																gWTPs[i].pathMTU, seqNum, sta->mac_addr, sta->radio_id, 
																CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE)) {

							if(CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE, seqNum)){
								bResult = CW_TRUE;
							} else {
								CWACStopRetransmission(i);
							}
						} else {	
							char stringToWum[128];
							sprintf(stringToWum, "Station %02x:%02x:%02x:%02x:%02x:%02x del reqeust can't assembled",
							sta->mac_addr[0], sta->mac_addr[1], sta->mac_addr[2], sta->mac_addr[3], sta->mac_addr[4], sta->mac_addr[5]);							
							CWLog(stringToWum);
							CWResponseToWum(i, -1, strlen(stringToWum)+1, stringToWum);
						}
						break;
					}
#else
					{
						// TODO
						break;
					}
#endif
#ifdef RTK_CAPWAP
				case WTP_SITE_SURVEY_CMD:
					{
						int seqNum = CWGetSeqNum();
						CWLog("# RTK WTP Status #");
						if (CWAssembleRtkWTPStatusRequestSiteSurvey(&gWTPs[i].messages, &gWTPs[i].messagesCount,
															  gWTPs[i].pathMTU, seqNum)) {	
							if(!CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_RTK_WTP_STATUS_RESPONSE, seqNum))
								CWACStopRetransmission(i);
							else 
								bResult = CW_TRUE;
						}
						break;
					}
				case WTP_LIST_STATIONS_CMD:
					{
						int seqNum = CWGetSeqNum();
						CWLog("# List Stations #");
						if (CWAssembleRtkWTPStatusRequestStations(&gWTPs[i].messages, &gWTPs[i].messagesCount,
															  gWTPs[i].pathMTU, seqNum)) {	
							if(!CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_RTK_WTP_STATUS_RESPONSE, seqNum))
								CWACStopRetransmission(i);
							else 
								bResult = CW_TRUE;
						}
						break;
					}
					
				case WTP_MODIFY_CHANNEL_CMD:
					{
						int seqNum = CWGetSeqNum();
						CWLog("# Configuration Update - Channel #");
						if (CWAssembleConfigurationUpdateRequest(&(gWTPs[i].messages), &(gWTPs[i].messagesCount),
																gWTPs[i].pathMTU, seqNum, 
																CW_MSG_ELEMENT_RTK_CHANNEL_CW_TYPE, 
																gWTPs[i].extra_info_buffer)) {

							if(!CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum))
								CWACStopRetransmission(i);
							else {
								CW_CREATE_OBJECT_ERR(gWTPs[i].pRtkChannel, CWProtocolRtkChannelValues, {});
								memcpy(gWTPs[i].pRtkChannel, gWTPs[i].extra_info_buffer, sizeof(CWProtocolRtkChannelValues));
								bResult = CW_TRUE;
							}
						}
						break;
					}					
				case WTP_MODIFY_POWER_CMD:
					{
						int seqNum = CWGetSeqNum();
						CWLog("# Configuration Update - Power #");
						if (CWAssembleConfigurationUpdateRequest(&(gWTPs[i].messages), &(gWTPs[i].messagesCount),
																gWTPs[i].pathMTU, seqNum, 
																CW_MSG_ELEMENT_RTK_POWER_SCALE_CW_TYPE, 
																gWTPs[i].extra_info_buffer)) {
							if(!CWACSendAcknowledgedPacket(i, CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE, seqNum))
								CWACStopRetransmission(i);
							else {
								
								CW_CREATE_OBJECT_ERR(gWTPs[i].pRtkPowerScale, CWProtocolRtkPowerScaleValues, {});
								memcpy(gWTPs[i].pRtkPowerScale, gWTPs[i].extra_info_buffer, sizeof(CWProtocolRtkPowerScaleValues));
								bResult = CW_TRUE;
							}
						}
						break;
					}					
#endif

				}

				gWTPs[i].interfaceCommand = NO_CMD;

				if (bResult)
					gWTPs[i].interfaceCommandProgress = CW_TRUE;
				else {
					CWSignalThreadCondition(&gWTPs[i].interfaceComplete);
					CWDebugLog("Error sending command");
				}
			}
			CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);	
		}
#ifdef RTK_SMART_ROAMING
		else if (	gWTPs[i].currentState == CW_ENTER_RUN && 
					gWTPs[i].notifyCommandHead != gWTPs[i].notifyCommandTail)
		{					
			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
			
            while(gWTPs[i].notifyCommandHead != gWTPs[i].notifyCommandTail) {
				CWPendingTimerArg *timer_arg;
				unsigned char idx;

				int seqNum = CWGetSeqNum();
				
				CWThreadMutexLock(&gWTPs[i].PendingReqLock);
				int pendingIndex = CWACFindFreePendingIndex(i);
				if(pendingIndex < 0) {
					CWThreadMutexUnlock(&gWTPs[i].PendingReqLock);
					break;
				}
				
				CW_CREATE_OBJECT_ERR(timer_arg, CWPendingTimerArg, return CW_FALSE;);
				timer_arg->WTPIndex = i;
				timer_arg->pendingIndex = pendingIndex;


				idx = gWTPs[i].notifyCommandTail;

				CWDebugLog("AC Pending Message Sent to WTP[%d] pendingReq:%d seqNum: %d", i, pendingIndex, seqNum);
						
				if(!CWAssembleNotifyToWTP(idx, i, pendingIndex, seqNum))
				{
					goto next;
				}
				if(!CWACUpdatePendingRequest(&gWTPs[i].PendingReq[pendingIndex],
							CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE,
							seqNum,
							timer_arg,
							&CWACRetransmitTimerExpiredHandler))
				{
					goto next;
				}
				if(!CWACSendPendingRequest(i, gWTPs[i].PendingReq[pendingIndex].msg, gWTPs[i].PendingReq[pendingIndex].fragNum))
				{
					CWDebugLog("\tCWACSendPendingRequest FAIL!");													
					CWACResetPendingRequest(&gWTPs[i].PendingReq[pendingIndex]);
					goto next;
				}

next:			
				CWThreadMutexUnlock(&gWTPs[i].PendingReqLock);
			    gWTPs[i].notifyCommand[idx] = NO_NOTIFY;		
                gWTPs[i].notifyCommandTail = (gWTPs[i].notifyCommandTail+1) % AC_MAX_NOTIFY_NUM;
            }
			CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
			
		}
#endif
		else if (CWGetCountElementFromSafeList(gWTPs[i].packetReceiveList) > 0)
			{

			char	*pBuffer;

#ifndef CW_NO_DTLS
			CWBool 	bCrypt = CW_FALSE;
//printf("(%s) should dequeue packetReceiveList (count = %d)\n", __FUNCTION__, CWGetCountElementFromSafeList(gWTPs[i].packetReceiveList));

			CWThreadMutexLock(&gWTPs[i].interfaceMutex);
			pBuffer = (char *)CWGetHeadElementFromSafeList(gWTPs[i].packetReceiveList, NULL);


			if (((pBuffer[0] & 0x0f) == CW_PACKET_CRYPT) && ((gWTPs[i].buf[0] & 0x0f) == CW_PACKET_CRYPT))
			  bCrypt = CW_TRUE;	

		
			CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);

			if (bCrypt) {

//printf("(%s) calling CWSecurityReceive()\n", __FUNCTION__);

			  if(!CWErr(CWSecurityReceive(gWTPs[i].session,
										  gWTPs[i].buf,
										  CW_BUFFER_SIZE - 1,
										  &readBytes))) {
					/* error */

				CWDebugLog("Error during security receive");
				CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
//printf("(%s) Error during security receive\n", __FUNCTION__);

				continue;
			  }
			} else 
#endif

			{
//printf("(%s) dequeue packetReceiveList (count = %d)\n", __FUNCTION__, CWGetCountElementFromSafeList(gWTPs[i].packetReceiveList));
              CWLockSafeList(gWTPs[i].packetReceiveList);
			  pBuffer = (char*)CWRemoveHeadElementFromSafeListwithDataFlag(gWTPs[i].packetReceiveList, &readBytes,&dataFlag);
              CWUnlockSafeList(gWTPs[i].packetReceiveList);
			  
			  memcpy(gWTPs[i].buf, pBuffer, readBytes);
			  CW_FREE_OBJECT(pBuffer);
			}

			if(!CWProtocolParseFragment(gWTPs[i].buf,
						    readBytes,
						    &(gWTPs[i].fragmentsList),
						    &msg,
						    &dataFlag,
						    gWTPs[i].RadioMac)) {

				if(CWErrorGetLastErrorCode() == CW_ERROR_NEED_RESOURCE) {

					CWDebugLog("Need At Least One More Fragment");
				} 
				else {
					CWErrorHandleLast();
				}
				CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
				
				continue;
			}

			switch(gWTPs[i].currentState) 
			{
				case CW_ENTER_JOIN:
				{
//printf("(%s) WTP[%d] case CW_ENTER_JOIN\n", __FUNCTION__, i);
					/* we're inside the join state */
					if(!ACEnterJoin(i, &msg)) 
					{
						if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
						{
							/* Log and ignore other messages */
							CWErrorHandleLast();
							CWLog("Received something different from a Join Request");
						} 
						else 
						{
							/* critical error, close session */
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							CWCloseThread();
						}
					}
					break;
				}
#if 0
				case CW_ENTER_POST_JOIN:
				{
					if (!ACEnterJoinPost(i, &msg))
					{
						if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT && gWTPs[i].msgTypeErrCounter < 5) 
						{
							/* Log and ignore other messages */
							CWErrorHandleLast();
							CWLog("Received something different from a Configure Request or Image Data Request, msgTypeErrCounter =%d", gWTPs[i].msgTypeErrCounter);
							gWTPs[i].msgTypeErrCounter ++;
							// Babylon: work-around XXX
						} 
						else 
						{
							/* critical error, close session */
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							CWCloseThread();
						}
					}
					break;
				}
#endif
				case CW_ENTER_CONFIGURE:
				{
//printf("(%s) WTP[%d] case CW_ENTER_CONFIGURE\n", __FUNCTION__, i);
					if(!ACEnterPostJoin(i, &msg)) // may receive "config req" or "image data req"
					{
						if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT && gWTPs[i].msgTypeErrCounter < 5) 
						{
							/* Log and ignore other messages */
							CWErrorHandleLast();
							CWLog("Received something different from a Configure Request and Image Data Request, msgTypeErrCounter =%d", gWTPs[i].msgTypeErrCounter);
							gWTPs[i].msgTypeErrCounter ++;
							// Babylon: work-around XXX
						} 
						else 
						{
							/* critical error, close session */
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							CWCloseThread();
						}
					}
					break;
				}
 				case CW_ENTER_IMAGE_DATA:
				{
//printf("(%s) WTP[%d] ENTER_IMAGE_DATA\n", __FUNCTION__, i);
					if(!ACEnterImageData2ndStage(i, &msg)) 
					{
#if 0
						if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT && gWTPs[i].msgTypeErrCounter < 5) 
						{
							/* Log and ignore other messages */
							CWErrorHandleLast();
							printf("Received something different from Image Data Response, msgTypeErrCounter =%d\n", gWTPs[i].msgTypeErrCounter);
							CWLog("Received something different from Image Data Response, msgTypeErrCounter =%d", gWTPs[i].msgTypeErrCounter);
							gWTPs[i].msgTypeErrCounter ++;

							ret = ACEnterImageData(WTPIndex, &completeMsg, controlVal.msgElemsLen, controlVal.seqNum);
							// Babylon: work-around XXX
						} 
						else 
#endif
						{

						CWLog("Image Data Stage Failed. Close Thread!");
						/* critical error, close session */
						CWErrorHandleLast();
						CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
						CWCloseThread();
					        }
					}
					break;
				}
 				case CW_ENTER_DATA_CHECK:
				{
//printf("(%s) WTP[%d] ENTER_DATA_CHECK\n", __FUNCTION__, i);
					if(!ACEnterDataCheck(i, &msg)) 
					{
						if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
						{
							/* Log and ignore other messages */
							CWErrorHandleLast();
							CWLog("Received something different from a Change State Event Request");
						} 
						else 
						{
							/* critical error, close session */
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							CWCloseThread();
						}
					}
					break;
				}	
				case CW_ENTER_RUN:
				{

//printf("(%s) WTP[%d] case CW_ENTER_RUN\n", __FUNCTION__, i);
					if(!ACEnterRun(i, &msg, dataFlag)) 
					{
						if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
						{
							/* Log and ignore other messages */
							CWErrorHandleLast();
							CWLog("--> Received something different from a valid Run Message");
//printf("!!!!!!!!ACEnterRun return false, but continue\n");
						} 
						// TODO Babylon: ACEnterRun() returns CW_FALSE in many way. Need to review again. (prevent AC closing the thread in wrong ways.)
						else 
						{
							/* critical error, close session */
							CWLog("--> Critical Error... closing thread");
							CWErrorHandleLast();
							CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
							CWCloseThread();
//printf("!!!!!!!!ACEnterRun return false, and exit.\n");
						}
					}
					break;
				}
				default:
				{
					CWLog("Not Handled Packet");
					break;
				}
			}
			CW_FREE_PROTOCOL_MESSAGE(msg);
		}
		CWThreadSetSignals(SIG_UNBLOCK, 2, 
				   CW_SOFT_TIMER_EXPIRED_SIGNAL, 
				   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
	}
}

void _CWCloseThread(int i) {

 	CWThreadSetSignals(SIG_BLOCK, 2, 
			   CW_SOFT_TIMER_EXPIRED_SIGNAL, 
			   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);

	/**** ACInterface ****/
	gWTPs[i].qosValues=NULL;
	CW_FREE_OBJECT(gWTPs[i].pRtkPowerScale);
	CW_FREE_OBJECT(gWTPs[i].pRtkChannel);
	//for image update
	gWTPs[i].imageActionState = NO_ACTION;
	gWTPs[i].reqImageIdentifier[0] = '\0';
	gWTPs[i].pReqImageData = NULL;
	CWThreadMutexUnlock(&(gWTPs[i].interfaceMutex));
	/**** ACInterface ****/

	if(!CWErr(CWThreadMutexLock(&gActiveWTPsMutex))) 
		exit(1);
	
	gInterfaces[gWTPs[i].interfaceIndex].WTPCount--;
	gActiveWTPs--;
	
	CWUseSockNtop( ((struct sockaddr*)&(gInterfaces[gWTPs[i].interfaceIndex].addr)),
			CWLog("Remove WTP on Interface %s (%d)", str, gWTPs[i].interfaceIndex););

	CWThreadMutexUnlock(&gActiveWTPsMutex);
	
	CWDebugLog("Close Thread: %p", CWThreadSelf());
#ifdef RTK_SMART_ROAMING
	memset(gWTPsWlan[i].wlan0, 0, 6);
	memset(gWTPsWlan[i].wlan1, 0, 6);
	gWTPsStatus[i] = 0;
#endif

#ifndef CW_NO_DTLS	
	if(gWTPs[i].subState != CW_DTLS_HANDSHAKE_IN_PROGRESS) {
	
		CWSecurityDestroySession(gWTPs[i].session);
	}
#endif

	/* this will do nothing if the timer isn't active */
	CWTimerCancel(&(gWTPs[i].currentTimer));
	CWACStopRetransmission(i);

	if (gWTPs[i].interfaceCommandProgress == CW_TRUE) {

		CWThreadMutexLock(&gWTPs[i].interfaceMutex);
		
		gWTPs[i].interfaceCommandProgress = CW_FALSE;
		CWSignalThreadCondition(&gWTPs[i].interfaceComplete);

		CWThreadMutexUnlock(&gWTPs[i].interfaceMutex);
	}

#ifndef CW_NO_DTLS	
	gWTPs[i].session = NULL;
#endif
	gWTPs[i].subState = CW_DTLS_HANDSHAKE_IN_PROGRESS;
	CWDeleteList(&(gWTPs[i].fragmentsList), CWProtocolDestroyFragment);
	
	/* CW_FREE_OBJECT(gWTPs[i].configureReqValuesPtr); */
	
	CWCleanSafeList(gWTPs[i].packetReceiveList, free);
	CWDestroySafeList(gWTPs[i].packetReceiveList);
	
	CWCleanSafeList(gWTPs[i].activeWorkList, free);
	CWDestroySafeList(gWTPs[i].activeWorkList);

	gWTPs[i].echoRequestCounter = 0;
	gWTPs[i].lastEchoRequestTime = 0;
	CWThreadMutexLock(&gWTPsMutex);
	gWTPs[i].isNotFree = CW_FALSE;
	CWThreadMutexUnlock(&gWTPsMutex);

	//fix memory leak issue 
	int j;
	for(j= 0; j < (gWTPs[i].WTPProtocolManager.ACNameIndex).count; j++) 
	{
		if(((gWTPs[i].WTPProtocolManager.ACNameIndex).ACNameIndex)[j].ACName!=NULL)
		{
			//printf("%s:%d j=%d free ((gWTPs[i].WTPProtocolManager.ACNameIndex).ACNameIndex)[j].ACName!!!\n",__FUNCTION__,__LINE__,j);
			CW_FREE_OBJECT(((gWTPs[i].WTPProtocolManager.ACNameIndex).ACNameIndex)[j].ACName);
		}
	}
	for(j= 0; j < (gWTPs[i].WTPProtocolManager.descriptor).vendorInfos.vendorInfosCount; j++) 
	{
		if(((gWTPs[i].WTPProtocolManager.descriptor).vendorInfos.vendorInfos)[j].valuePtr!=NULL)
		{
			//printf("%s:%d j=%d free ((gWTPs[i].WTPProtocolManager.descriptor).vendorInfos.vendorInfos)[j].valuePtr\n",__FUNCTION__,__LINE__,j);
			CW_FREE_OBJECT(((gWTPs[i].WTPProtocolManager.descriptor).vendorInfos.vendorInfos)[j].valuePtr);
		}
	}
	for(j= 0; j < (gWTPs[i].WTPProtocolManager.WTPBoardData).vendorInfosCount; j++) 
	{
		if(((gWTPs[i].WTPProtocolManager.WTPBoardData).vendorInfos)[j].valuePtr!=NULL)
		{
			//printf("%s:%d j=%d free ((gWTPs[i].WTPProtocolManager.WTPBoardData).vendorInfos)[j].valuePtr\n",__FUNCTION__,__LINE__,j);
			CW_FREE_OBJECT(((gWTPs[i].WTPProtocolManager.WTPBoardData).vendorInfos)[j].valuePtr);
		}
	}
	//printf("%s calling CWResetWTPProtocolManager() to free memory!!!!\n",__FUNCTION__);
	CWResetWTPProtocolManager(&(gWTPs[i].WTPProtocolManager));
	if(gWTPs[i].WTPProtocolManager.bindingValuesPtr!=NULL)
	{
		//printf("%s:%d free WTPProtocolManager->bindingValuesPtr!!!\n",__FUNCTION__,__LINE__);
		bindingValues* aux=(bindingValues *)(gWTPs[i].WTPProtocolManager.bindingValuesPtr);
		CW_FREE_OBJECT(aux->qosValues);
		CW_FREE_OBJECT(gWTPs[i].WTPProtocolManager.bindingValuesPtr);
	}
#ifdef KM_BY_AC // key management
	//TODO: Babylon: kill hostapd
#endif

#ifndef CW_NO_DTLS
	CWThread cur_tid=gettid();
	//printf("\n%s:%d cur_tid=%d\n",__FUNCTION__,__LINE__,cur_tid);
	//ERR_remove_state(gWTPs[i].thread);
	ERR_remove_state(cur_tid);
	ENGINE_cleanup();
	CONF_modules_unload(1);
	ERR_free_strings();
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
#endif

	CWDestroyThreadMutex(&gWTPs[i].interfaceMutex);	
	CWDestroyThreadCondition(&gWTPs[i].interfaceWait);	
	CWDestroyThreadCondition(&gWTPs[i].interfaceComplete);	

	CWExitThread();
}

void CWCloseThread() {

	int *iPtr;
	
	if((iPtr = ((int*)CWThreadGetSpecific(&gIndexSpecific))) == NULL) {

		CWLog("Error Closing Thread");
		return;
	}
	
	_CWCloseThread(*iPtr);
}

void CWCriticalTimerExpiredHandler(int arg) {

	int *iPtr;

	CWThreadSetSignals(SIG_BLOCK, 2,
			   CW_SOFT_TIMER_EXPIRED_SIGNAL,
			   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);

//printf("Critical Timer Expired for Thread: %08x\n", (unsigned int)CWThreadSelf());
	CWDebugLog("Critical Timer Expired for Thread: %p", CWThreadSelf());
	CWDebugLog("Abort Session");
	/* CWCloseThread(); */

	if((iPtr = ((int*)CWThreadGetSpecific(&gIndexSpecific))) == NULL) {

		CWLog("Error Handling Critical timer");
		CWThreadSetSignals(SIG_UNBLOCK, 2, 
				   CW_SOFT_TIMER_EXPIRED_SIGNAL,
				   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
		return;
	}

	/* Request close thread */
	gWTPs[*iPtr].isRequestClose = CW_TRUE;
#ifndef CW_NO_DTLS
	//[2014.09.19 Babylon] Break SSL handshaking		
	if (gWTPs[*iPtr].session) {		
		BIO_ctrl(gWTPs[*iPtr].session->rbio, BIO_CTRL_CAPWAP_TERMINATE_RX_WAIT, 0, NULL);
	}
#endif
	CWSignalThreadCondition(&gWTPs[*iPtr].interfaceWait);
}

void CWSoftTimerExpiredHandler(int arg) {

	int *iPtr;

	CWThreadSetSignals(SIG_BLOCK, 2, 
			   CW_SOFT_TIMER_EXPIRED_SIGNAL,
			   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);

	CWDebugLog("Soft Timer Expired for Thread: %p", CWThreadSelf());
//printf("Soft Timer Expired for Thread: %08x\n", (unsigned int)CWThreadSelf());
	
	if((iPtr = ((int*)CWThreadGetSpecific(&gIndexSpecific))) == NULL) {

		CWLog("Error Handling Soft timer");
		CWThreadSetSignals(SIG_UNBLOCK, 2, 
				   CW_SOFT_TIMER_EXPIRED_SIGNAL,
				   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
		return;
	}
	
	if((!gWTPs[*iPtr].isRetransmitting) || (gWTPs[*iPtr].messages == NULL)) {

		CWDebugLog("Soft timer expired but we are not retransmitting");
		CWThreadSetSignals(SIG_UNBLOCK, 2, 
				   CW_SOFT_TIMER_EXPIRED_SIGNAL,
				   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
		return;
	}

	(gWTPs[*iPtr].retransmissionCount)++;
	
	CWDebugLog("Retransmission Count increases to %d", gWTPs[*iPtr].retransmissionCount);
	
	if(gWTPs[*iPtr].retransmissionCount >= gCWMaxRetransmit) 
	{
		CWDebugLog("Peer is Dead");
		/*
		 * Request close thread
		 */
		gWTPs[*iPtr].isRequestClose = CW_TRUE;
#ifndef CW_NO_DTLS
		//[2014.09.19 Babylon] Break SSL handshaking		
		if (gWTPs[*iPtr].session) { 	
			BIO_ctrl(gWTPs[*iPtr].session->rbio, BIO_CTRL_CAPWAP_TERMINATE_RX_WAIT, 0, NULL);
		}
#endif
		CWSignalThreadCondition(&gWTPs[*iPtr].interfaceWait);
		return;
	}

	if(!CWErr(CWACResendAcknowledgedPacket(*iPtr))) {
		/*
		 * Request close thread
		 */
		gWTPs[*iPtr].isRequestClose = CW_TRUE;
#ifndef CW_NO_DTLS
		//[2014.09.19 Babylon] Break SSL handshaking		
		if (gWTPs[*iPtr].session) { 	
			BIO_ctrl(gWTPs[*iPtr].session->rbio, BIO_CTRL_CAPWAP_TERMINATE_RX_WAIT, 0, NULL);
		}
#endif
		CWSignalThreadCondition(&gWTPs[*iPtr].interfaceWait);
	}
	
	/* CWDebugLog("~~~~~~fine ritrasmissione ~~~~~"); */
	CWThreadSetSignals(SIG_UNBLOCK, 2, 
			   CW_SOFT_TIMER_EXPIRED_SIGNAL,
			   CW_CRITICAL_TIMER_EXPIRED_SIGNAL);
}
void CWInitWTPProtocolManager(CWWTPProtocolManager *WTPProtocolManager) {
	WTPProtocolManager->locationData = NULL;
	WTPProtocolManager->name = NULL;	
	WTPProtocolManager->sessionID = NULL;

	WTPProtocolManager->descriptor.maxRadios= 0;
	WTPProtocolManager->descriptor.radiosInUse= 0;
	WTPProtocolManager->descriptor.encCapabilities= 0;
	WTPProtocolManager->descriptor.vendorInfos.vendorInfosCount= 0;
	WTPProtocolManager->descriptor.vendorInfos.vendorInfos = NULL;
	
	WTPProtocolManager->radiosInfo.radioCount= 0;
	WTPProtocolManager->radiosInfo.radiosInfo = NULL;
	WTPProtocolManager->ACName = NULL;
	WTPProtocolManager->ACNameIndex.count = 0;
	WTPProtocolManager->ACNameIndex.ACNameIndex = NULL;
	WTPProtocolManager->radioAdminInfo.radiosCount = 0;
	WTPProtocolManager->radioAdminInfo.radios = NULL;
	WTPProtocolManager->StatisticsTimer = 0;
	WTPProtocolManager->WTPBoardData.vendorInfosCount = 0;
	WTPProtocolManager->WTPBoardData.vendorInfos = NULL;
	WTPProtocolManager->WTPRebootStatistics = NULL;
}

void CWResetWTPProtocolManager(CWWTPProtocolManager *WTPProtocolManager) {
	CW_FREE_OBJECT(WTPProtocolManager->locationData);
	CW_FREE_OBJECT(WTPProtocolManager->name);
	CW_FREE_OBJECT(WTPProtocolManager->sessionID);
	
	WTPProtocolManager->descriptor.maxRadios= 0;
	WTPProtocolManager->descriptor.radiosInUse= 0;
	WTPProtocolManager->descriptor.encCapabilities= 0;
	WTPProtocolManager->descriptor.vendorInfos.vendorInfosCount= 0;
	CW_FREE_OBJECT(WTPProtocolManager->descriptor.vendorInfos.vendorInfos);
	
	WTPProtocolManager->radiosInfo.radioCount= 0;
	CW_FREE_OBJECT(WTPProtocolManager->radiosInfo.radiosInfo);
	CW_FREE_OBJECT(WTPProtocolManager->ACName);
	(WTPProtocolManager->ACNameIndex).count = 0;
	CW_FREE_OBJECT((WTPProtocolManager->ACNameIndex).ACNameIndex);
	(WTPProtocolManager->radioAdminInfo).radiosCount = 0;
	CW_FREE_OBJECT((WTPProtocolManager->radioAdminInfo).radios);
	WTPProtocolManager->StatisticsTimer = 0;
	(WTPProtocolManager->WTPBoardData).vendorInfosCount = 0;
	CW_FREE_OBJECT((WTPProtocolManager->WTPBoardData).vendorInfos);
	CW_FREE_OBJECT(WTPProtocolManager->WTPRebootStatistics);

	//CWWTPResetRebootStatistics(&(WTPProtocolManager->WTPRebootStatistics));

	/*
		**mancano questi campi:**
		CWNetworkLev4Address address;
		int pathMTU;
		struct sockaddr_in ipv4Address;
		CWProtocolConfigureRequestValues *configureReqValuesPtr;
		CWTimerID currentPacketTimer;
	*/
}



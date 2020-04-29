/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	   *
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
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *******************************************************************************************/

 
#include "CWWTP.h"
#include "WTPRtkSystemInteraction.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

extern CWBool gBreakRcvLoop;
static CWBool gSuccessfulHandshake = CW_TRUE;
int gCWWaitJoin = CW_JOIN_INTERVAL_DEFAULT;
extern int dtls_thread_exit;
CWBool gGotoReset; 
	// Babylon: When CWWTPReceiveDtlsPacket (WTP Rx Thread) ended, turn on this to ask handling thread to go to RESET state.

#ifdef RTK_AUTO_AC
extern	unsigned long minAddress;
#endif

/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */
void CWWTPWaitJoinExpired(CWTimerArg arg);
CWBool CWAssembleJoinRequest(CWProtocolMessage **messagesPtr,
			     int *fragmentsNumPtr,
			     int PMTU,
			     int seqNum,
			     CWList msgElemList);

CWBool CWParseJoinResponseMessage(unsigned char *msg,
				  int len,
				  int seqNum,
				  CWProtocolJoinResponseValues *gJoinResponseValPtr);

CWBool CWSaveJoinResponseMessage (CWProtocolJoinResponseValues *joinResponse);

/*_____________________________________________________*/
/*  *******************___FUNCTIONS___*******************  */

/*
 * Manage Join State.
 */
 
CWProtocolJoinResponseValues gJoinResponseVal;
CWStateTransition CWWTPEnterJoin() {

	CWTimerID waitJoinTimer;
	int seqNum;
	
	CWLog("\n");
	CWLog("######### Join State #########");
	
	/* reset Join state */

	/* Initialize gACInfoPtr */
	gACInfoPtr->ACIPv4ListInfo.ACIPv4ListCount=0;
	gACInfoPtr->ACIPv4ListInfo.ACIPv4List=NULL;	
	gACInfoPtr->ACIPv6ListInfo.ACIPv6ListCount=0;
	gACInfoPtr->ACIPv6ListInfo.ACIPv6List=NULL;

	gSuccessfulHandshake = CW_TRUE;

	/* Add Join Timer */
#ifndef CW_NO_DTLS
	if ((waitJoinTimer = timer_add(gCWWaitJoin, 0, CWWTPWaitJoinExpired, &gWTPSession)) == -1)
#else
	if ((waitJoinTimer = timer_add(gCWWaitJoin, 0, CWWTPWaitJoinExpired, NULL)) == -1)
#endif
	{
		return CW_ENTER_DISCOVERY;
	}

	if(gWTPForceACAddress != NULL) {
		CW_CREATE_OBJECT_ERR(gACInfoPtr, 
				     CWACInfoValues,
				     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		CWNetworkGetAddressForHost(gWTPForceACAddress, 
					   &(gACInfoPtr->preferredAddress));
		gACInfoPtr->security = gWTPForceSecurity;
	}
	
	/* Init DTLS session */
	if(!CWErr(CWNetworkInitSocketClient(&gWTPSocket,
					    &(gACInfoPtr->preferredAddress))) ) {
		
		timer_rem(waitJoinTimer, NULL);
		return CW_ENTER_DISCOVERY;
	}
#if 1
#ifndef CW_NO_DTLS
	if(gACInfoPtr->security == CW_X509_CERTIFICATE) {
		if(!CWErr(CWSecurityInitContext(&gWTPSecurityContext,
						"root.pem",
						"client.pem",
						"prova",
						CW_TRUE,
						NULL))) {
			
			timer_rem(waitJoinTimer, NULL);
			CWNetworkCloseSocket(gWTPSocket);
			gWTPSecurityContext = NULL;
			return CW_ENTER_DISCOVERY;
		}
	} else { 
		/* pre-shared keys */
		if(!CWErr(CWSecurityInitContext(&gWTPSecurityContext,
						NULL,
						NULL,
						NULL,
						CW_TRUE,
						NULL))) {
			
			timer_rem(waitJoinTimer, NULL);
			CWNetworkCloseSocket(gWTPSocket);
			gWTPSecurityContext = NULL;
			return CW_ENTER_DISCOVERY;
		}
	}
#endif
#endif	
	gGotoReset = CW_FALSE;
/*
	if(gThreadReceiveFrame>0) {
	  printf("kill thread gThreadReceiveFrame: %d\n", gThreadReceiveFrame);
	  pthread_kill(gThreadReceiveFrame, SIGQUIT);

	  sleep(1);
	  printf("dtls_thread_exit: %d\n", dtls_thread_exit);
	  gThreadReceiveFrame = 0;
	 }
 */
	if(!CWErr(CWCreateThread(&gThreadReceiveFrame, 
				 CWWTPReceiveDtlsPacket,
				 (void*)gWTPSocket))) {
		
		CWLog("Error starting Thread that receive DTLS packet");
		timer_rem(waitJoinTimer, NULL);
		CWNetworkCloseSocket(gWTPSocket);
#ifndef CW_NO_DTLS
		CWSecurityDestroyContext(gWTPSecurityContext);
		gWTPSecurityContext = NULL;
#endif
		return CW_ENTER_DISCOVERY;
	}
	
#ifndef CW_NO_DTLS
		if(!CWErr(CWSecurityInitSessionClient(gWTPSocket,
							  &(gACInfoPtr->preferredAddress),
							  gPacketReceiveList,
							  gWTPSecurityContext,
							  &gWTPSession,
							  &gWTPPathMTU))) { 
			
			/* error setting up DTLS session */
			
			timer_rem(waitJoinTimer, NULL); 				
			CWNetworkCloseSocket(gWTPSocket);
			
			gBreakRcvLoop = CW_TRUE;
			while(dtls_thread_exit == 0) {
				printf("Wait thread to exit...\n"); 
				sleep(1);
			}
			//CWDestroyThread(gThreadReceiveFrame);

			CWSecurityDestroyContext(gWTPSecurityContext);
			gWTPSecurityContext = NULL;
			gWTPSession = NULL;
			return CW_ENTER_DISCOVERY;
		}
#endif

	/* Data session */
#ifdef KM_BY_AC
	if(!CWErr(CWNetworkInitSocketClientDataChannel(&gWTPDataSocket, &(gACInfoPtr->preferredAddress))) ) {
		timer_rem(waitJoinTimer, NULL); 
		CWLog("%s %d calling CWWTPCloseControlSession", __FUNCTION__, __LINE__);
		CWWTPCloseControlSession();
		return CW_ENTER_DISCOVERY;
	}
	CWLog("Initiate Data Channel");
	CWDebugLog("gWTPSocket:%d, gWTPDataSocket:%d", gWTPSocket,gWTPDataSocket);

	if(!CWErr(CWCreateThread(&gThreadReceiveDataFrame, 
				 CWWTPReceiveDataPacket,
				 (void*)gWTPDataSocket))) {
		
		timer_rem(waitJoinTimer, NULL); 
		CWLog("%s %d calling CWWTPCloseControlSession", __FUNCTION__, __LINE__);
		CWWTPCloseControlSession();
	
		CWLog("Error starting Thread that receive data packet");
		CWNetworkCloseSocket(gWTPDataSocket);
		return CW_ENTER_DISCOVERY;
	}
#endif
		
	if(gCWForceMTU > 0) {
		gWTPPathMTU = gCWForceMTU;
	}
	
	CWDebugLog("Path MTU for this Session: %d", gWTPPathMTU);
	
	/* send Join Request */
	seqNum = CWGetSeqNum();


printf("(%d)%s\n", rtk_getpid(), __FUNCTION__);

	if(!CWErr(CWWTPSendAcknowledgedPacket(seqNum,
					      NULL,
					      CWAssembleJoinRequest,
					      (void*)CWParseJoinResponseMessage,
					      (void*)CWSaveJoinResponseMessage,
					      &gJoinResponseVal))) {
cw_join_err:
		timer_rem(waitJoinTimer, NULL);
		CWLog("%s %d calling CWWTPCloseControlSession", __FUNCTION__, __LINE__);
		CWWTPCloseControlSession();
		
#ifdef KM_BY_AC		
		CWWTPCloseDataSession();
#endif
		return CW_ENTER_DISCOVERY;
	}
	
	timer_rem(waitJoinTimer, NULL);	

	// Babylon: actually this should not happen. 
	//			if timer expired, the function would return earlier.
	// 			May remove it in the future.
	if(!gSuccessfulHandshake) { 
		/* timer expired */
		//goto cw_join_err;
		CWLog("Handshake Failed");
		#include <assert.h>
		assert(gSuccessfulHandshake);
		return CW_ENTER_DISCOVERY;
	}

	CWLog("Join Completed");

 	if (gJoinResponseVal.imageIdentifierFound && 
            gJoinResponseVal.image_identifier.imageIdentifier[0]!='\0' &&
	    strcmp(gJoinResponseVal.image_identifier.imageIdentifier, cwRtkWtpGetCurrentFwId())!=0) {
		return CW_ENTER_IMAGE_DATA;
	}

#if defined (RTK_HOME_MESH)
	return CW_ENTER_CONFIGURE;
#elif defined (RTK_SMART_ROAMING)
	return CW_ENTER_RUN;
#else
	return CW_ENTER_CONFIGURE;
#endif
}

void CWWTPWaitJoinExpired(CWTimerArg arg) 
{	
	CWLog("WTP Wait Join Expired");

	/* Babylon 2014.09.19 to stop unlimited reading the RX packing queue */	
#ifndef CW_NO_DTLS
	if (arg) {
		BIO_ctrl((*(CWSecuritySession *)arg)->rbio, BIO_CTRL_CAPWAP_TERMINATE_RX_WAIT, 0, NULL);
	}
#endif
	CWSignalElementSafeList(gPacketReceiveList);


	gSuccessfulHandshake = CW_FALSE; // Babylon: May remove this variable in the future.
	CWNetworkCloseSocket(gWTPSocket);
	//CWDestroyThread(gThreadReceiveFrame);
	
#ifdef KM_BY_AC		
	CWNetworkCloseSocket(gWTPDataSocket);
	//CWDestroyThread(gThreadReceiveDataFrame);
#endif
}

CWBool CWAssembleJoinRequest(CWProtocolMessage **messagesPtr, 
			     int *fragmentsNumPtr,
			     int PMTU,
			     int seqNum,
			     CWList msgElemList) {

	CWProtocolMessage	*msgElems= NULL;
#ifdef RTK_SMART_ROAMING
	const int		msgElemCount = 10;
#else
	const int		msgElemCount = 9;
#endif	
	CWProtocolMessage 	*msgElemsBinding= NULL;
	const int 		msgElemBindingCount=0;
	int 			k = -1;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems,
					 msgElemCount,
					 return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		
	CWLog("Sending Join Request...");
	/* Assemble Message Elements */
	if ( 
		 (!(CWAssembleMsgElemLocationData(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPBoardData(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPDescriptor(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPIPv4Address(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPName(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemSessionID(&(msgElems[++k]), &gWTPSessionID[0]))) ||
	     (!(CWAssembleMsgElemWTPFrameTunnelMode(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPMACType(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPRadioInformation(&(msgElems[++k]))))
#ifdef RTK_SMART_ROAMING
	     ||(!(CWAssembleMsgElemWTPConfiguration(&(msgElems[++k]))))
#endif
	) {
		CWLog("Cannot Assemble Join Request Elements!");
		int i;
		for(i = 0; i <= k; i++) { 
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		/* error will be handled by the caller */
		return CW_FALSE;
	}
	return CWAssembleMessage(messagesPtr,
				 fragmentsNumPtr,
				 PMTU,
				 seqNum,
				 CW_MSG_TYPE_VALUE_JOIN_REQUEST,
				 msgElems,
				 msgElemCount,
				 msgElemsBinding,
				 msgElemBindingCount,
#ifdef CW_NO_DTLS
				 CW_PACKET_PLAIN
#else
				 CW_PACKET_CRYPT
#endif
				 );
}


/* 
 * Parse Join Response and return informations in *valuesPtr.
 */
CWBool CWParseJoinResponseMessage(unsigned char *msg,
				  int len,
				  int seqNum,
				  CWProtocolJoinResponseValues *valuesPtr) {

	CWControlHeaderValues 	controlVal;
	CWProtocolMessage 	completeMsg;
	int 			offsetTillMessages;
	unsigned char tmp_ABGNTypes;
	if (msg == NULL || valuesPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing Join Response...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	/* error will be handled by the caller */
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE;

	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_JOIN_RESPONSE)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, 
				    "Message is not Join Response as Expected");
	
	if(controlVal.seqNum != seqNum) 
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT,
				    "Different Sequence Number");
	
	/* skip timestamp */
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;
	
	offsetTillMessages = completeMsg.offset;

	/* Mauro */
	valuesPtr->ACInfoPtr.IPv4AddressesCount = 0;
	valuesPtr->ACInfoPtr.IPv6AddressesCount = 0;
	valuesPtr->imageIdentifierFound = CW_FALSE;


	/* parse message elements */
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;
		unsigned short int len=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		

		CWDebugLog("Parsing Message Element: %u, len: %u", type, len);
		/*
		valuesPtr->ACInfoPtr.IPv4AddressesCount = 0;
		valuesPtr->ACInfoPtr.IPv6AddressesCount = 0;
		*/
		valuesPtr->ACIPv4ListInfo.ACIPv4ListCount=0;
		valuesPtr->ACIPv4ListInfo.ACIPv4List=NULL;
		valuesPtr->ACIPv6ListInfo.ACIPv6ListCount=0;
		valuesPtr->ACIPv6ListInfo.ACIPv6List=NULL;
	
		switch(type) {
			case CW_MSG_ELEMENT_AC_DESCRIPTOR_CW_TYPE:
				/* will be handled by the caller */
				if(!(CWParseMsgElemACDescriptor(&completeMsg, len, &(valuesPtr->ACInfoPtr)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_IEEE80211_WTP_RADIO_INFORMATION_CW_TYPE:
				/* will be handled by the caller */
				if(!CWParseMsgElemWTPRadioInformation(&completeMsg, len, &tmp_ABGNTypes)) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE:
				if(!(CWParseMsgElemACIPv4List(&completeMsg, len, &(valuesPtr->ACIPv4ListInfo)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE:
				if(!(CWParseMsgElemACIPv6List(&completeMsg, len, &(valuesPtr->ACIPv6ListInfo)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				if(!(CWParseMsgElemResultCode(&completeMsg, len, &(valuesPtr->code)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_AC_NAME_CW_TYPE:
				/* will be handled by the caller */
				if(!(CWParseMsgElemACName(&completeMsg, len, &(valuesPtr->ACInfoPtr.name)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE:
				/* 
				 * just count how many interfacess we
				 * have, so we can allocate the array 
				 */
				valuesPtr->ACInfoPtr.IPv4AddressesCount++;
				completeMsg.offset += len;
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE:
				/* 
				 * just count how many interfacess we
				 * have, so we can allocate the array 
				 */
				valuesPtr->ACInfoPtr.IPv6AddressesCount++;
				completeMsg.offset += len;
				break;
 			/*
 			case CW_MSG_ELEMENT_SESSION_ID_CW_TYPE:
 				if(!(CWParseSessionID(&completeMsg, len, valuesPtr))) return CW_FALSE;
 				break;	
 			*/
 			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:
				//printf("Got image identifier\n");
				valuesPtr->imageIdentifierFound = 
				CWParseMsgElemImageIdentifier(&completeMsg, len, &valuesPtr->image_identifier);
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}

		/* CWDebugLog("bytes: %d/%d", (completeMsg.offset-offsetTillMessages), controlVal.msgElemsLen); */
	}
	
	if(completeMsg.offset != len) 
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, 
				    "Garbage at the End of the Message");
	
	/* actually read each interface info */
	CW_CREATE_ARRAY_ERR(valuesPtr->ACInfoPtr.IPv4Addresses, 
			    valuesPtr->ACInfoPtr.IPv4AddressesCount,
			    CWProtocolIPv4NetworkInterface,
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if(valuesPtr->ACInfoPtr.IPv6AddressesCount > 0) {

		CW_CREATE_ARRAY_ERR(valuesPtr->ACInfoPtr.IPv6Addresses,
				    valuesPtr->ACInfoPtr.IPv6AddressesCount,
				    CWProtocolIPv6NetworkInterface,
				    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	}

	int i = 0;
	int j = 0;
	
	completeMsg.offset = offsetTillMessages;
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;	/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int len=0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		
		
		switch(type) {
			case CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE:
				/* will be handled by the caller */
				if(!(CWParseMsgElemCWControlIPv4Addresses(&completeMsg, 
								   len,
								   &(valuesPtr->ACInfoPtr.IPv4Addresses[i]))))
				       	return CW_FALSE;
				i++;
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE:
				/* will be handled by the caller */
				if(!(CWParseMsgElemCWControlIPv6Addresses(&completeMsg,
								   len,
								   &(valuesPtr->ACInfoPtr.IPv6Addresses[j]))))
					return CW_FALSE;
				j++;
				break;
			default:
				completeMsg.offset += len;
				break;
		}
	}

	return CW_TRUE;
}

CWBool CWSaveJoinResponseMessage(CWProtocolJoinResponseValues *joinResponse)
{

   if(joinResponse == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

   if((joinResponse->code == CW_PROTOCOL_SUCCESS) ||
      (joinResponse->code == CW_PROTOCOL_SUCCESS_NAT)) {

		if(gACInfoPtr == NULL) 
			return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL);

		gACInfoPtr->stations = (joinResponse->ACInfoPtr).stations;
		gACInfoPtr->limit = (joinResponse->ACInfoPtr).limit;
		gACInfoPtr->activeWTPs = (joinResponse->ACInfoPtr).activeWTPs;
		gACInfoPtr->maxWTPs = (joinResponse->ACInfoPtr).maxWTPs;
		gACInfoPtr->security = (joinResponse->ACInfoPtr).security;
		gACInfoPtr->RMACField = (joinResponse->ACInfoPtr).RMACField;

		/* BUG-ML07
         * Before overwriting the field vendorInfos we'd better
         * free it (it was allocated during the Discovery State by
         * the function CWParseACDescriptor()).
         *
         * 19/10/2009 - Donato Capitella
         */
         
		/*
		JOY: cannot be free here for CONFIGURE state
		CW_FREE_OBJECT(gACInfoPtr->IPv4Addresses);
		CW_FREE_OBJECT(gACInfoPtr->name);
		*/

        int i;
        for(i = 0; i < gACInfoPtr->vendorInfos.vendorInfosCount; i++) {
                CW_FREE_OBJECT(gACInfoPtr->vendorInfos.vendorInfos[i].valuePtr);
        }
        CW_FREE_OBJECT(gACInfoPtr->vendorInfos.vendorInfos);


		gACInfoPtr->vendorInfos = (joinResponse->ACInfoPtr).vendorInfos;
		
		if(joinResponse->ACIPv4ListInfo.ACIPv4ListCount >0) {

			gACInfoPtr->ACIPv4ListInfo.ACIPv4ListCount = joinResponse->ACIPv4ListInfo.ACIPv4ListCount; 
			gACInfoPtr->ACIPv4ListInfo.ACIPv4List = joinResponse->ACIPv4ListInfo.ACIPv4List; 
		}
		
		if(joinResponse->ACIPv6ListInfo.ACIPv6ListCount >0) {
			gACInfoPtr->ACIPv6ListInfo.ACIPv6ListCount = joinResponse->ACIPv6ListInfo.ACIPv6ListCount; 
			gACInfoPtr->ACIPv6ListInfo.ACIPv6List = joinResponse->ACIPv6ListInfo.ACIPv6List; 
		}
	
		/* 
         * This field name was allocated for storing the AC name; however, it
         * doesn't seem to be used and it is certainly lost when we exit
         * CWWTPEnterJoin() as joinResponse is actually a local variable of that
         * function.
         *
         * Thus, it seems good to free it now.   
         * 
         * BUG ML03  
         * 16/10/2009 - Donato Capitella
         */
        CW_FREE_OBJECT(joinResponse->ACInfoPtr.name);
        /* BUG ML08 */
        CW_FREE_OBJECT(joinResponse->ACInfoPtr.IPv4Addresses);

		CWDebugLog("Join Response Saved");	
		return CW_TRUE;
	}else {
		if (joinResponse->code == CW_PROTOCOL_JOIN_FAILURE_UNKNOWN_SRC) {
			CWLog("AC doesn't know this WTP, and has rejected join.");
		} else {
			CWLog("AC has rejected join: JoinResult=%d", joinResponse->code);	
		}
		return CWErrorRaise(CW_ERROR_NEED_RESOURCE, "AC has rejected Join!");
	}
}

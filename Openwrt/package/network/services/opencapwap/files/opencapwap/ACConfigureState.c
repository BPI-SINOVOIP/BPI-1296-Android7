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

#include "CWAC.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

int gCWChangeStatePendingTimer = CW_CHANGE_STATE_INTERVAL_DEFAULT;

CWBool CWAssembleConfigureResponse(CWProtocolMessage **messagesPtr,
				   int *fragmentsNumPtr,
				   int PMTU,
				   int seqNum,
#ifndef KM_BY_AC
	   			   const AC_DB_WTP_T *ac_db_wtp
#endif
	   			   );

CWBool CWParseConfigureRequestMessage(unsigned char *msg,
				      int msgLen,
				      //int *seqNumPtr,
				      CWProtocolConfigureRequestValues *valuesPtr, unsigned char*, unsigned char*, unsigned char*);

CWBool CWSaveConfigureRequestMessage(CWProtocolConfigureRequestValues *configureRequest,
				     CWWTPProtocolManager *WTPProtocolManager);



static CWBool ACEnterConfigure(int WTPIndex, unsigned char *msg, int pktLeftLength, int seqNum)
{
	/*** tmp Radio Info ***/
	unsigned char tmp_RadioInformationABGN;
	unsigned char tmp_SuppRates[8];
	unsigned char tmp_MultiDomCapa[6];
	int i;	

	//int seqNum;
	CWProtocolConfigureRequestValues configureRequest;
	
	CWLog("\n");
	CWLog("######### Configure State #########");	


	memset(&configureRequest, 0, sizeof(CWProtocolConfigureRequestValues));
	if(!(CWParseConfigureRequestMessage(msg, 
										pktLeftLength, 
										&configureRequest,
										&tmp_RadioInformationABGN,
										tmp_SuppRates,
										tmp_MultiDomCapa))) {
		/* note: we can kill our thread in case of out-of-memory 
		 * error to free some space.
		 * we can see this just calling CWErrorGetLastErrorCode()
		 */
		return CW_FALSE;
	}

	CWLog("Configure Request Received, WTPIndex=%d", WTPIndex);
	
	if(!(CWSaveConfigureRequestMessage(&configureRequest, &(gWTPs[WTPIndex].WTPProtocolManager)))){
		return CW_FALSE;
	}
	
	
	/* Store Radio Info in gWTPs */

	//TODO Babylon: The information is fake now. Should implement!
	for (i=0; i<gWTPs[WTPIndex].WTPProtocolManager.radiosInfo.radioCount; i++) {
		//TODO Babylon
		gWTPs[WTPIndex].WTPProtocolManager.radios[i].RadioInformationABGN = tmp_RadioInformationABGN;
		memcpy( gWTPs[WTPIndex].WTPProtocolManager.radios[i].SuppRates, tmp_SuppRates, 8 );
		memcpy( gWTPs[WTPIndex].WTPProtocolManager.radios[i].MultiDomCapa, tmp_MultiDomCapa, 6);		
	}
	
	CWLog("(WTP %d)RadioInformationABGN=%d", WTPIndex, tmp_RadioInformationABGN);
	CWLog("(WTP %d)SuppRates=%s", WTPIndex, tmp_SuppRates);
	CWLog("(WTP %d)MultiDomCapa=%s", WTPIndex, tmp_MultiDomCapa);
	
	
	if(!CWAssembleConfigureResponse(&gWTPs[WTPIndex].messages, 
					 &gWTPs[WTPIndex].messagesCount, 
					 gWTPs[WTPIndex].pathMTU, 
					 seqNum,					 
#ifndef KM_BY_AC
					 &gWTPs[WTPIndex].wtpConfig
#endif
					 ))  { 
		return CW_FALSE;
	}
	
	if(!CWACSendFragments(WTPIndex)) {
		return CW_FALSE;
	}
	
	CWLog("Configure Response Sent");
	
	/* start Change State Pending timer */
	if(!CWErr(CWTimerRequest(gCWChangeStatePendingTimer,
				 0,
				 &(gWTPs[WTPIndex].thread),
				 &(gWTPs[WTPIndex].currentTimer),
				 CW_CRITICAL_TIMER_EXPIRED_SIGNAL))) {

		CWCloseThread();
	}

#ifdef RTK_HOME_MESH
	gWTPs[WTPIndex].currentState = CW_ENTER_RUN;
#else
	gWTPs[WTPIndex].currentState = CW_ENTER_DATA_CHECK;
#endif
	return CW_TRUE;
}

CWBool ACEnterPostJoin(int WTPIndex, CWProtocolMessage *msgPtr)
{
	// parse header
	CWProtocolMessage completeMsg;	
	CWControlHeaderValues controlVal;
	CWBool ret;
	
	completeMsg.msg = msgPtr->msg;
	completeMsg.offset = 0;
		
	if(!(CWParseControlHeader(&completeMsg, &controlVal)))
		/* will be handled by the caller */
		return CW_FALSE;

	/* skip timestamp */
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;

	switch (controlVal.messageTypeValue) {
		case CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST:
			ret = ACEnterConfigure(WTPIndex, completeMsg.msg + completeMsg.offset, controlVal.msgElemsLen, controlVal.seqNum);
			break;
		case CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST:
			ret = ACEnterImageData(WTPIndex, &completeMsg, controlVal.msgElemsLen, controlVal.seqNum);
			break;
		default:
		{	
			char msg[256];
			sprintf(msg, "Message is not valid in this state. MessageTypeValue=%u, currentState=%u", controlVal.messageTypeValue, gWTPs[WTPIndex].currentState);
			ret = CWErrorRaise(CW_ERROR_INVALID_FORMAT, msg);
			break;
		}
	}

	return ret;
}



CWBool CWParseConfigureRequestMessage(unsigned char *msg,
				      int msgLen,
				      //int *seqNumPtr,
				      CWProtocolConfigureRequestValues *valuesPtr,
				      unsigned char *tmp_RadioInformationABGN,
				      unsigned char *tmp_SuppRates,
				      unsigned char *tmp_MultiDomCapa) {

	CWControlHeaderValues controlVal;
	int i,j;
	int offsetTillMessages;
	
	CWProtocolMessage completeMsg;
	
	//if(msg == NULL || seqNumPtr == NULL || valuesPtr == NULL) 
	//	return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CWDebugLog("Parsing Configure Request...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	//if(!(CWParseControlHeader(&completeMsg, &controlVal))) 
		/* will be handled by the caller */
	//	return CW_FALSE;

	/* different type */
	//if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST) {
	//	char msg[256];
	//	sprintf(msg, "Message is not Configure Request. MessageTypeValue=%u", controlVal.messageTypeValue);
	//	return CWErrorRaise(CW_ERROR_INVALID_FORMAT, msg);
	//}
	//*seqNumPtr = controlVal.seqNum;
	/* skip timestamp */
	//controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;
	
	offsetTillMessages = completeMsg.offset;
	
	/* valuesPtr->WTPRadioInfo.radiosCount=0; */
	valuesPtr->ACinWTP.count=0;
	valuesPtr->radioAdminInfoCount=0;
	
	/* parse message elements */
	while(completeMsg.offset < msgLen) {
		unsigned short int elemType = 0;/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int elemLen = 0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		/*CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);*/
									
		switch(elemType) {
			case CW_MSG_ELEMENT_AC_NAME_CW_TYPE:
				if(!(CWParseMsgElemACName(&completeMsg, elemLen, &(valuesPtr->ACName)))) 
					/* will be handled by the caller */
					return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE:
				/* just count how many radios we have,
				 * so we can allocate the array
				 */
				valuesPtr->ACinWTP.count++;
				completeMsg.offset += elemLen;
				break;			
			case CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE:
				/* just count how many radios we have,
				 * so we can allocate the array
				 */
				(valuesPtr->radioAdminInfoCount)++;
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_STATISTICS_TIMER_CW_TYPE:
				if(!(CWParseMsgElemWTPStatisticsTimer(&completeMsg, elemLen, &(valuesPtr->StatisticsTimer)))) 
					/* will be handled by the caller */
					return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->WTPRebootStatistics,
						     WTPRebootStatisticsInfo,
						     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				if(!(CWParseMsgElemWTPRebootStatistics(&completeMsg,
								elemLen,
								valuesPtr->WTPRebootStatistics))) 
					/* will be handled by the caller */
					return CW_FALSE;
				break;
			
			case CW_MSG_ELEMENT_IEEE80211_WTP_RADIO_INFORMATION_CW_TYPE:
				if(!(CWParseMsgElemWTPRadioInformation(&completeMsg, elemLen, tmp_RadioInformationABGN)))return CW_FALSE;
				break;

			case CW_MSG_ELEMENT_IEEE80211_MULTI_DOMAIN_CAPABILITY_CW_TYPE:
				if(!(CWParseMsgElemWTPMultiDomainCapability(&completeMsg, elemLen, tmp_MultiDomCapa)))return CW_FALSE;
				break;

			case CW_MSG_ELEMENT_IEEE80211_SUPPORTED_RATES_CW_TYPE:
				if(!(CWParseMsgElemWTPSupportedRates(&completeMsg, elemLen, tmp_SuppRates)))return CW_FALSE;
				break;	

			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}

	
	if(completeMsg.offset != msgLen) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	/* actually read each radio info */
	CW_CREATE_ARRAY_ERR((valuesPtr->ACinWTP).ACNameIndex, 
			    (valuesPtr->ACinWTP).count,
			    CWACNameWithIndexValues, 
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
	CW_CREATE_ARRAY_ERR(valuesPtr->radioAdminInfo,
			    valuesPtr->radioAdminInfoCount,
			    CWRadioAdminInfoValues,
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	i = 0;
	j = 0;

	completeMsg.offset = offsetTillMessages;
	while(completeMsg.offset-offsetTillMessages < msgLen) {
		
		unsigned short int type=0;
		unsigned short int len=0;
		CWDebugLog("completeMsg.offset=%d, offsetTillMessages=%d, msgLen=%d", 
			completeMsg.offset, offsetTillMessages, msgLen);
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		
		
		CWDebugLog("Parsing Message Element: %u, elemLen: %u", type, len);
		switch(type) {
			case CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE:
				if(!(CWParseMsgElemACNameWithIndex(&completeMsg, len, &(valuesPtr->ACinWTP.ACNameIndex[i])))) 
					/* will be handled by the caller */
					return CW_FALSE;
				i++;
				break;
			case CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE:
				if(!(CWParseMsgElemWTPRadioAdminState(&completeMsg, len, &(valuesPtr->radioAdminInfo[j]))))
					/* will be handled by the caller */
					return CW_FALSE;
				j++;
				break;
			default:
				completeMsg.offset += len;
				break;
		}
	}
	CWDebugLog("Configure Request Parsed");	
	return CW_TRUE;
}

CWBool CWAssembleConfigureResponse(CWProtocolMessage **messagesPtr,
				   int *fragmentsNumPtr,
				   int PMTU,
				   int seqNum,
#ifndef KM_BY_AC
				   const AC_DB_WTP_T *ac_db_wtp
#endif
				   )
{
	CWProtocolMessage *msgElems = NULL;
	int MsgElemCount=7;
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	int k = -1; // index of message elements
	int i, j;
	
#ifndef KM_BY_AC
	int wlan_count, radio_count;
#endif	

	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Assembling Configure Response...");
	
#ifndef KM_BY_AC
	// count wlans
	for (i=0, wlan_count=0; i<ac_db_wtp->nRadios; i++) {
		for (j=0; j<ac_db_wtp->nWlans; j++) {
			if(ac_db_wtp->radios[i].wlans[j].enable) {
				wlan_count++;
			}
		}
	}
	radio_count = ac_db_wtp->nRadios;
	MsgElemCount += wlan_count + radio_count*2;
#endif
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, MsgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	/* Assemble Message Elements */
	if (!CWAssembleMsgElemACIPv4List(&msgElems[++k]) ||
	    !CWAssembleMsgElemACIPv6List(&msgElems[++k]) ||
	    !CWAssembleMsgElemCWTimer(&msgElems[++k]) ||
	    !CWAssembleMsgElemRadioOperationalState(-1, &msgElems[++k]) ||
	    !CWAssembleMsgElemDecryptErrorReportPeriod(&msgElems[++k]) ||
	    !CWAssembleMsgElemIdleTimeout(&msgElems[++k]) ||
	    !CWAssembleMsgElemWTPFallback(&msgElems[++k]) ){
		int i;
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		/* error will be handled by the caller */
		return CW_FALSE;
	}

#ifndef KM_BY_AC
	// fill wlan configuration
	for (i=0; i<ac_db_wtp->nRadios; i++) {
		for (j=0; j<ac_db_wtp->nWlans; j++) {
			if(ac_db_wtp->radios[i].wlans[j].enable) {
				CWProtocolRtkWlanConfigValues wlanConfig;
				wlanConfig.radio_id = i;
				wlanConfig.wlan_id = j;
				memcpy(wlanConfig.bssid, ac_db_wtp->radios[i].wlans[j].bssid, 6);
				strcpy(wlanConfig.ssid, ac_db_wtp->radios[i].wlans[j].ssid);

				wlanConfig.key_type = ac_db_wtp->radios[i].wlans[j].keyType; 
				wlanConfig.psk_format = ac_db_wtp->radios[i].wlans[j].pskFormat;
				memcpy(wlanConfig.key, ac_db_wtp->radios[i].wlans[j].key, CW_MAX_KEY_SIZE*2+1);
				
				if (!CWAssembleMsgElemRtkWlanConfiguration(&msgElems[++k], &wlanConfig)){
					int i;
					for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
					CW_FREE_OBJECT(msgElems);
					/* error will be handled by the caller */
					return CW_FALSE;
				}
			}
		}
	}

	// fill channels & power scales
	for (i=0; i<ac_db_wtp->nRadios; i++) {
		CWProtocolRtkChannelValues channelConfig;
		CWProtocolRtkPowerScaleValues powerConfig;
		channelConfig.radio_id = i;
		channelConfig.channel = ac_db_wtp->radios[i].channel;
		powerConfig.radio_id = i;
		powerConfig.power_scale = ac_db_wtp->radios[i].powerScale;


		if (!CWAssembleMsgElemRtkChannel(&msgElems[++k], &channelConfig) ||
			!CWAssembleMsgElemRtkPowerScale(&msgElems[++k], &powerConfig)){
			int i;
			for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
			CW_FREE_OBJECT(msgElems);
			/* error will be handled by the caller */
			return CW_FALSE;
		}
	}	
#endif

	if (k!= MsgElemCount -1) {
		CWLog("Error! message element count: k=%d, MsgElemCount%d", k, MsgElemCount);
	}
	if(!CWBindingAssembleConfigureResponse(&msgElemsBinding, &msgElemBindingCount))
	{
		int i;
		for(i = 0; i < MsgElemCount; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}
	/*CWDebugLog("~~~~~ msg count: %d ", msgElemBindingCount);*/
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_CONFIGURE_RESPONSE,
			       msgElems,
			       MsgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN)))
#else
			       CW_PACKET_CRYPT)))
#endif
	{
		return CW_FALSE;
	}
	
	CWDebugLog("Configure Response Assembled");
	return CW_TRUE;
}

CWBool CWSaveConfigureRequestMessage (CWProtocolConfigureRequestValues *configureRequest,
				      CWWTPProtocolManager *WTPProtocolManager) {

	CWDebugLog("Saving Configure Request...");
	
	CW_FREE_OBJECT(WTPProtocolManager->ACName);

	if((configureRequest->ACName) != NULL)
		WTPProtocolManager->ACName = configureRequest->ACName;
	
	CW_FREE_OBJECT((WTPProtocolManager->ACNameIndex).ACNameIndex);
	WTPProtocolManager->ACNameIndex = configureRequest->ACinWTP;
	
	CW_FREE_OBJECT((WTPProtocolManager->radioAdminInfo).radios);
	(WTPProtocolManager->radioAdminInfo).radiosCount = configureRequest->radioAdminInfoCount;
	(WTPProtocolManager->radioAdminInfo).radios = configureRequest->radioAdminInfo;
		
	WTPProtocolManager->StatisticsTimer = configureRequest->StatisticsTimer;
	
	/*	
	CW_FREE_OBJECT((WTPProtocolManager->WTPRadioInfo).radios);
	WTPProtocolManager->WTPRadioInfo = configureRequest->WTPRadioInfo;	
	*/

	CW_FREE_OBJECT(WTPProtocolManager->WTPRebootStatistics);
	WTPProtocolManager->WTPRebootStatistics = configureRequest->WTPRebootStatistics;

	CWDebugLog("Configure Request Saved");
	return CW_TRUE;
}

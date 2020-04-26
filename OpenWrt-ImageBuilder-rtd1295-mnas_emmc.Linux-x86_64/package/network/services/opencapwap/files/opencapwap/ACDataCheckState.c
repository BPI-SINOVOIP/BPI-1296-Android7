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

CWBool ACEnterDataCheck(int WTPIndex, CWProtocolMessage *msgPtr) 
{
	int seqNum;	

	/*CWProtocolMessage *messages = NULL;*/
	CWProtocolChangeStateEventRequestValues *changeStateEvent;
	
	CWLog("\n");
	CWDebugLog("######### Status Event #########");	
	
	CW_CREATE_OBJECT_ERR(changeStateEvent, 
			     CWProtocolChangeStateEventRequestValues,
			     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if(!(CWParseChangeStateEventRequestMessage(msgPtr->msg, msgPtr->offset, &seqNum, changeStateEvent))) {
		/* note: we can kill our thread in case of out-of-memory 
		 * error to free some space.
		 * we can see this just calling CWErrorGetLastErrorCode()
		 */
		return CW_FALSE;
	}

	// babylon 2014/02/21: bug fix: should make sure the message is what we expected before cancel the timer!
	/* Destroy ChangeStatePending timer */
	if(!CWErr(CWTimerCancel(&(gWTPs[WTPIndex].currentTimer)))) {

		CWCloseThread();
	}
	CWLog("Change State Event Received");
	
	if(!(CWSaveChangeStateEventRequestMessage(changeStateEvent, &(gWTPs[WTPIndex].WTPProtocolManager))))
		return CW_FALSE;
	if(!(CWAssembleChangeStateEventResponse(&(gWTPs[WTPIndex].messages), 
						&(gWTPs[WTPIndex].messagesCount),
						gWTPs[WTPIndex].pathMTU,
						seqNum))) { 
		return CW_FALSE;
	}
	
	if(!CWACSendFragments(WTPIndex)) {

		return CW_FALSE;
	}
	
	/* Start NeighbourDeadInterval timer */
	if(!CWErr(CWTimerRequest(gCWNeighborDeadInterval, 
				  0,
				 &(gWTPs[WTPIndex].thread),
				 &(gWTPs[WTPIndex].currentTimer),
				 CW_CRITICAL_TIMER_EXPIRED_SIGNAL))) {

		CWCloseThread();
	}

	CWLog("Change State Event Response Sent");

	
#ifdef KM_BY_AC // key management
	const AC_DB_WTP_T *ac_db_wtp = &gWTPs[WTPIndex].WTPProtocolManager.wtpConfig;
	/* run hostapd */
	int i;		
	char cmd[256];

	sprintf(cmd, " cd "AC_HAPD_PATH "; ./" AC_HAPD_EXEC);
	for (i=0; i<ac_db_wtp->nRadios; i++) {
		// activate hostapd for this radio
		strcat(cmd, " ");
		strcat(cmd, ac_db_wtp->radios[i].hapd_cfg_file);
		CWLog("Activate Corresponded Hostapd for Radio %d..(%s)", i, ac_db_wtp->radios[i].hapd_cfg_file);
	}		
	strcat(cmd, "&");
	system(cmd);
	CWLog("cmd: %s", cmd);
#else
	// babylon: we don't user WlanConfiguration Messages any more. 
	//			We config wlans by adding information into Configuration Status Response Message.
	#if 0
	
	/* Lookup WLAN configuration. Add all works of configuration into activeWorkList */
		const AC_DB_WTP_T *ac_db_wtp = &gWTPs[WTPIndex].WTPProtocolManager.wtpConfig;
		int i, j;
		ac_active_work_t *work;
		for (i=0; i<ac_db_wtp->nRadios; i++) {
			for (j=0; j<ac_db_wtp->nWlans; j++) {
				const struct wlan_s *wlan_cfg = &ac_db_wtp->radios[i].wlans[j];
				CWProtocolAddWLANValues *pAddWlan;

				CW_CREATE_OBJECT_ERR(work, ac_active_work_t, 
						return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, "cannot allocate memory for Add Wlan Work"););
				work->type = WTP_ADD_WLAN_CMD;
				pAddWlan = &work->data.wtp_add_wlan;
				memset(pAddWlan, 0, sizeof (CWProtocolAddWLANValues));
				pAddWlan->capability.s.ess = 1;
				pAddWlan->mac_mode = CW_LOCAL_MAC;
				pAddWlan->auth_type = CW_AUTH_TYPE_PSK; // TODO
				pAddWlan->tunnel_mode = CW_TUNNEL_MODE_802_11;

				/* by AC DB configuration */
				pAddWlan->radio_id = i;
				pAddWlan->wlan_id = j;
					
				switch(wlan_cfg->key_type) {
				case WLAN_CFG_KEY_TYPE_NONE :
					pAddWlan->key_status = CW_WLAN_KEY_STATUS_SHARED_WEP;
					pAddWlan->key_len = 0;
					break;

				case WLAN_CFG_KEY_TYPE_SHARED_WEP40 :					
					pAddWlan->key_status = CW_WLAN_KEY_STATUS_SHARED_WEP;
					pAddWlan->key_len = 5;
					memcpy(pAddWlan->key, wlan_cfg->key, 5);
					break;

				case WLAN_CFG_KEY_TYPE_SHARED_WEP104 :
					pAddWlan->key_status = CW_WLAN_KEY_STATUS_SHARED_WEP;
					pAddWlan->key_len = 13;
					memcpy(pAddWlan->key, wlan_cfg->key, 13);
					break;
/*					
				case WLAN_CFG_KEY_TYPE_SHARED_TKIP :
					pAddWlan->key_status = CW_WLAN_KEY_STATUS_SHARED_TKIP;
					pAddWlan->key_len = 32;

					// TODO: gen key
					//memcpy(pAddWlan->key, wlan_cfg->key, pAddWlan->key_len);
					break;
				case WLAN_CFG_KEY_TYPE_SHARED_AES :
					pAddWlan->key_status = CW_WLAN_KEY_STATUS_SHARED_AES;
					pAddWlan->key_len = 32;

					// TODO: gen key
					//memcpy(pAddWlan->key, wlan_cfg->key, pAddWlan->key_len);
					break;
*/
				default:				
					pAddWlan->key_status = CW_WLAN_KEY_STATUS_SHARED_WEP;
					pAddWlan->key_len = 0;
					CWLog("TODO: WLAN key type currently not supported...");
					break;
				}
				strcpy(pAddWlan->ssid, wlan_cfg->ssid);
				
				// need NOT do lock again, since have locked.
				CWAddElementToSafeListTail(gWTPs[WTPIndex].activeWorkList, work, sizeof(ac_active_work_t));
			}
		}	
	#endif
#endif

	gWTPs[WTPIndex].currentState = CW_ENTER_RUN;
	gWTPs[WTPIndex].subState = CW_WAITING_REQUEST;
	return CW_TRUE;
}

CWBool CWParseChangeStateEventRequestMessage(unsigned char *msg,
					     int len,
					     int *seqNumPtr,
					     CWProtocolChangeStateEventRequestValues *valuesPtr) {

	CWProtocolMessage completeMsg;
	CWControlHeaderValues controlVal;
	int i;
	int offsetTillMessages;
		
	if(msg == NULL || seqNumPtr == NULL || valuesPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing Change State Event Request...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
		
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) 
		/* will be handled by the caller */
		return CW_FALSE;

	/* different type */
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Change State Event Request");
	
	*seqNumPtr = controlVal.seqNum;
	/* skip timestamp */
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;

	offsetTillMessages = completeMsg.offset;
	valuesPtr->radioOperationalInfo.radiosCount=0;

	/* parse message elements */
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int elemType = 0;/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int elemLen = 0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		/*CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);*/
									
		switch(elemType) {
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				/* just count how many radios we have, 
				 * so we can allocate the array 
				 */
				valuesPtr->radioOperationalInfo.radiosCount++;
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE: 
				if(!(CWParseMsgElemResultCode(&completeMsg, elemLen, &(valuesPtr->resultCode))))
					return CW_FALSE;
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}
	
	if(completeMsg.offset != len) 
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
		
	CW_CREATE_ARRAY_ERR(valuesPtr->radioOperationalInfo.radios,
			    valuesPtr->radioOperationalInfo.radiosCount,
			    CWRadioOperationalInfoValues, 
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
	completeMsg.offset = offsetTillMessages;
	i = 0;

	while(completeMsg.offset-offsetTillMessages < controlVal.msgElemsLen) {
		unsigned short int type = 0;	/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int len = 0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		

		switch(type) {
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				if(!(CWParseMsgElemWTPRadioOperationalState(&completeMsg, len, &(valuesPtr->radioOperationalInfo.radios[i])))) 
					/* will be handled by the caller */
					return CW_FALSE;
				i++;
				break;
			default:
				completeMsg.offset += len;
				break;
		}
	}
	CWDebugLog("Change State Event Request Parsed");
	
	return CW_TRUE;
}

CWBool CWAssembleChangeStateEventResponse(CWProtocolMessage **messagesPtr,
					  int *fragmentsNumPtr,
					  int PMTU,
					  int seqNum) {

	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount=0;
	CWProtocolMessage *msgElemsBinding= NULL;
	int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Assembling Change State Event Response...");
	/*CW_CREATE_ARRAY_ERR(msgElems, 
	 * 		      msgElemCount,
	 * 		      CWProtocolMessage,
	 * 		      return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	 */
	if(!(CWAssembleMessage(messagesPtr, 
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			      CW_PACKET_PLAIN))) 
#else
			      CW_PACKET_CRYPT))) 
#endif
		return CW_FALSE;
	
	CWDebugLog("Change State Event Response Assembled");	
	return CW_TRUE;
}

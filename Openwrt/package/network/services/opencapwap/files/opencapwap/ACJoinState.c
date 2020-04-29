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
#include "ACAppsProtocol.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
#ifdef RTK_CAPWAP
#include "ACRtkSystemInteraction.h"
#endif

AC_DB_WTP_T default_wtp_config =
	{
		.name = "RTKCWAP_0",   //wtp_name_prefix = "RTKCWAP_";
		.nRadios = 1,		
		.nWlans = 1,
		.radios[0] = { 
		.baseMAC = {0xAA, 0xE0, 0x4C, 0x81, 0x96, 0x00}, 
		.channel = 10,
		.powerScale = 0,
#ifndef KM_BY_AC // key management
		.wlans[0] = {
			.enable = CW_TRUE,
			.keyType = WLAN_CFG_KEY_TYPE_SHARED_WEP40,
			.key = "12345",
			.pskFormat = WLAN_CFG_PSK_FORMAT_PASSPHRASE,
			.ssid = "RTKCWAP",
			.bssid = {0xAA, 0xE0, 0x4C, 0x81, 0x96, 0x00},
		},
#else
	.hapd_cfg_file = "ap0r0.conf" ,
#endif
	},
};
#ifdef _FAKE_AC_DB_
struct {
	uint32_t nWtps;
	AC_DB_WTP_T wtps[10];
} gAcDbWtp =
{
	.nWtps = 3,
	.wtps[0] =
	{
		.name = "RTKCWAP_1",   //wtp_name_prefix = "RTKCWAP_";
		.nRadios = 2,		
		.nWlans = 3,
		.radios[0] = { 
					.baseMAC = {0x00, 0xE0, 0x4C, 0x81, 0x96, 0xC1}, 
				#ifndef KM_BY_AC // key management
					.wlans[0] = {
						.keyType = WLAN_CFG_KEY_TYPE_SHARED_WEP40,
						//.key_len = 5,
						.key = "12345",
						.ssid = "CW1_00",
					},
					.wlans[1] = {					
						.keyType = WLAN_CFG_KEY_TYPE_SHARED_WEP40,
						//.key_len = 5,
						.key = "abcde",
						.ssid = "CW1_01",
					},
				#else
					.hapd_cfg_file = "ap1r0.conf" ,
				#endif
		},
		.radios[1] = { 
					.baseMAC = {0x00, 0xE0, 0x4C, 0x81, 0x96, 0xD1}, 
				#ifndef KM_BY_AC // key management
					.wlans[0] = {					
						.keyType = WLAN_CFG_KEY_TYPE_SHARED_WEP40,
						//.key_len = 5,
						.key = "12345",
						.ssid = "CW1_10",
					},
					.wlans[1] = {					
						.keyType = WLAN_CFG_KEY_TYPE_SHARED_WEP40,
						//.key_len = 5,
						.key = "abcde",
						.ssid = "CW1_11",
					},
					.wlans[2] = {					
						.keyType = WLAN_CFG_KEY_TYPE_NONE,
						.ssid = "CW1_11",
					},
				#else
					.hapd_cfg_file = "ap1r1.conf",
				#endif
		},
		.fwImageIdentifier = "",
	},
	.wtps[1] =
	{
		.name = "RTKCWAP_2", 
		.nRadios = 1,
		.nWlans = 2,
		.radios[0] = { 
					.baseMAC = {0x00, 0xE0, 0x4C, 0x81, 0x96, 0xC1}, 
		#ifndef KM_BY_AC // key management
					.wlans[0] = {					
						.keyType = WLAN_CFG_KEY_TYPE_SHARED_WEP40,
						//.key_len = 5,
						.key = "12345",
						.ssid = "CW1_00",
					},
					.wlans[1] = {					
						.keyType = WLAN_CFG_KEY_TYPE_SHARED_WEP40,
						//.key_len = 5,
						.key = "abcde",
						.ssid = "CW1_01",
					},
		#else
					.hapd_cfg_file = "ap2r0.conf" ,
		#endif
		},
		.fwImageIdentifier = "",
	},
	.wtps[2] =
	{
		.name = "RTKCWAP_3", 
		.nRadios = 1,
		.nWlans = 1,
		.radios[0] = { 
					.baseMAC = {0x00, 0xE0, 0x4C, 0x81, 0x96, 0xE1}, 
		#ifndef KM_BY_AC // key management
					.wlans[0] = {					
						.keyType = WLAN_CFG_KEY_TYPE_SHARED_WEP40,
						//.key_len = 5,
						.key = "12345",
						.ssid = "CW1_00",
					},
		#else
					.hapd_cfg_file = "ap3r0.conf" ,
		#endif
		},
		.fwImageIdentifier = "",
	},


};
#endif

CWBool CWAcDbWtpLookup(const char *wtp_name, AC_DB_WTP_T *wtp)
{
#ifdef _FAKE_AC_DB_
	unsigned int i;
	for (i=0; i<gAcDbWtp.nWtps; i++)
	{
		if (strcmp(wtp_name, gAcDbWtp.wtps[i].name) == 0) // match
		{
			memcpy(wtp, &gAcDbWtp.wtps[i], sizeof(AC_DB_WTP_T));
			return CW_TRUE;
		}
	}
	return CW_FALSE;
#else
	return cwRtkAcGetWtpConfig(wtp_name, wtp);
#endif
}

CWBool CWAcDbSetFwId(const char *wtp_name, const char *fwId)
{
#ifdef _FAKE_AC_DB_
	unsigned int i;
	for (i=0; i<gAcDbWtp.nWtps; i++)
	{
		if (strcmp(wtp_name, gAcDbWtp.wtps[i].name) == 0) // match
		{
			strncpy(gAcDbWtp.wtps[i].fwImageIdentifier, fwId, CW_IMAGE_IDENTIFIER_MAX_SIZE);
			return CW_TRUE;
		}
	}
	return CW_FALSE;
#else
	int wtp_id;
	return (cwRtkAcTransferWtpName2Id(wtp_name, &wtp_id) &&	
			cwRtkAcSetFwId(wtp_id, fwId));
#endif
}

CWBool CWAcDbSetPowerScale(const char *wtp_name, int radio_id, rtk_power_scale_t power_scale)
{
#ifdef _FAKE_AC_DB_
	unsigned int i;
	for (i=0; i<gAcDbWtp.nWtps; i++)
	{
		if (strcmp(wtp_name, gAcDbWtp.wtps[i].name) == 0) // match
		{
			gAcDbWtp.wtps[i].radios[0].powerScale = power_scale;
			return CW_TRUE;
		}
	}
	return CW_FALSE;
#else
	int wtp_id;
	return (cwRtkAcTransferWtpName2Id(wtp_name, &wtp_id) &&	
			cwRtkAcSetPowerScale(wtp_id, radio_id, power_scale));
#endif
}

CWBool CWAcDbSetSetChannel(const char *wtp_name, int radio_id, unsigned char channel)
{
#ifdef _FAKE_AC_DB_
	unsigned int i;
	for (i=0; i<gAcDbWtp.nWtps; i++)
	{
		if (strcmp(wtp_name, gAcDbWtp.wtps[i].name) == 0) // match
		{
			gAcDbWtp.wtps[i].radios[0].channel = channel;
			return CW_TRUE;
		}
	}
	return CW_FALSE;
#else
	int wtp_id;
	return (cwRtkAcTransferWtpName2Id(wtp_name, &wtp_id) && 
			cwRtkAcSetChannel(wtp_id, radio_id, channel));
#endif
}
	

//==================================================================================

CWBool CWAssembleJoinResponse(CWProtocolMessage **messagesPtr,
			      int *fragmentsNumPtr,
			      int PMTU,
			      int seqNum,
			      CWList msgElemList);

CWBool CWParseJoinRequestMessage(unsigned char *msg,
				 int len,
				 int *seqNumPtr,
				 CWProtocolJoinRequestValues *valuesPtr);

CWBool CWSaveJoinRequestMessage(int WTP_Index, CWProtocolJoinRequestValues *joinRequest, CWWTPManager *wtpManager, CWProtocolResultCode *resultCodeValue);


CWBool ACEnterJoin(int WTPIndex, CWProtocolMessage *msgPtr)
{	
	int seqNum;
	CWProtocolJoinRequestValues joinRequest;
	CWList msgElemList = NULL;
	
	CWLog("\n");
	CWLog("######### Join State (wtp index=%d)#########", WTPIndex);	

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	memset(&joinRequest, 0, sizeof(CWProtocolJoinRequestValues));
	
	if(!(CWParseJoinRequestMessage(msgPtr->msg, msgPtr->offset, &seqNum, &joinRequest))) {
		/* note: we can kill our thread in case of out-of-memory 
		 * error to free some space.
		 * we can see this just calling CWErrorGetLastErrorCode()
		 */
		return CW_FALSE;
	}

	// cancel waitJoin timer
	if(!CWTimerCancel(&(gWTPs[WTPIndex].currentTimer)))
	{
		return CW_FALSE;
	}

	CWBool ACIpv4List = CW_FALSE;
	CWBool ACIpv6List = CW_FALSE;
	CWBool resultCode = CW_TRUE;
	CWProtocolResultCode resultCodeValue = CW_PROTOCOL_SUCCESS;
	/* CWBool sessionID = CW_FALSE; */

	CWSaveJoinRequestMessage(WTPIndex, &joinRequest, &gWTPs[WTPIndex], &resultCodeValue);
	CWDebugLog("WTP%d Join Request Saved - resultCodeValue = %d\n", WTPIndex, resultCodeValue);
	
	CWMsgElemData *auxData=NULL;
	if(ACIpv4List) {
		CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
                auxData->type = CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE;
		auxData->value.int32 = 0;
		CWAddElementToList(&msgElemList,auxData);
	}
	if(ACIpv6List){
		CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
                auxData->type = CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE;
                auxData->value.int32 = 0;
                CWAddElementToList(&msgElemList,auxData);
	}
	if(resultCode){
		CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
                auxData->type =  CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE;
                auxData->value.int32 = resultCodeValue;
                CWAddElementToList(&msgElemList,auxData);
	}
	/*
 	if(sessionID){
 		CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
                 auxData->type =  CW_MSG_ELEMENT_SESSION_ID_CW_TYPE;
                 auxData->value = CWRandomIntInRange(0, INT_MAX);
                 CWAddElementToList(&msgElemList,auxData);
 	}
 	*/
	if (strcmp(gWTPs[WTPIndex].wtpConfig.fwImageIdentifier, "") != 0) {
		const char *fn = ACGetImageFile(gWTPs[WTPIndex].wtpConfig.fwImageIdentifier);
		if (fn && access(fn, R_OK)==0) {
			CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			auxData->type =  CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE;
			strcpy(auxData->value.str, gWTPs[WTPIndex].wtpConfig.fwImageIdentifier);
			CWAddElementToList(&msgElemList,auxData);
		}
			
	} else {
		char fwImageIdentifier[CW_IMAGE_IDENTIFIER_MAX_SIZE];
		cwRtkAcGetWTPDefaultFwId(fwImageIdentifier);
		if (strcmp(fwImageIdentifier, "") != 0) {
			const char *fn = ACGetImageFile(gWTPs[WTPIndex].wtpConfig.fwImageIdentifier);
			if (fn && access(fn, R_OK)==0) {
				CW_CREATE_OBJECT_ERR(auxData, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				auxData->type =  CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE;
				strncpy(auxData->value.str, fwImageIdentifier, CW_IMAGE_IDENTIFIER_MAX_SIZE);
				CWAddElementToList(&msgElemList,auxData);
				strncpy(gWTPs[WTPIndex].wtpConfig.fwImageIdentifier, fwImageIdentifier, CW_IMAGE_IDENTIFIER_MAX_SIZE);
			}
		}
	}

	/* random session ID */
	if(!(CWAssembleJoinResponse(&(gWTPs[WTPIndex].messages),
				    &(gWTPs[WTPIndex].messagesCount),
				    gWTPs[WTPIndex].pathMTU,
				    seqNum,
				    msgElemList))){

		CWDeleteList(&msgElemList, CWProtocolDestroyMsgElemData);
		return CW_FALSE;
	}
	
	CWDeleteList(&msgElemList, CWProtocolDestroyMsgElemData);
	
	if(!CWACSendFragments(WTPIndex)) {
		return CW_FALSE;
 	}

	if (resultCodeValue != CW_PROTOCOL_SUCCESS) {
		return CW_FALSE;
	}

#if defined (RTK_HOME_MESH)
	gWTPs[WTPIndex].currentState = CW_ENTER_CONFIGURE;
#elif defined (RTK_SMART_ROAMING)
	gWTPs[WTPIndex].currentState = CW_ENTER_RUN;
#else
	gWTPs[WTPIndex].currentState = CW_ENTER_CONFIGURE;
#endif

	gWTPs[WTPIndex].msgTypeErrCounter = 0;
	return CW_TRUE;
}

/*
 * Assemble Join Response.
 */
CWBool CWAssembleJoinResponse(CWProtocolMessage **messagesPtr,
			      int *fragmentsNumPtr,
			      int PMTU,
			      int seqNum,
			      CWList msgElemList) {

	CWProtocolMessage *msgElems= NULL;
	int msgElemCount = 0;
	/* Result code is not included because it's already
	 * in msgElemList. Control IPv6 to be added.
	 */
	const int mandatoryMsgElemCount=4;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	int i;
	CWListElement *current;
	int k = -1;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL || msgElemList == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	msgElemCount = CWCountElementInList(msgElemList);
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems,
					 msgElemCount + mandatoryMsgElemCount,
					 return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	CWDebugLog("Assembling Join Response...");

	
	if(
	   (!(CWAssembleMsgElemACDescriptor(&(msgElems[++k])))) ||
	   (!(CWAssembleMsgElemACName(&(msgElems[++k])))) ||
	   (!(CWAssembleMsgElemCWControlIPv4Addresses(&(msgElems[++k])))) || 
	   (!(CWAssembleMsgElemACWTPRadioInformation(&(msgElems[++k]))))
	) {
		CWErrorHandleLast();
		int i;
		for(i = 0; i <= k; i++) {CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		/* error will be handled by the caller */
		return CW_FALSE;
	} 

	current=msgElemList;
	for (i=0; i<msgElemCount; i++) {

        switch (((CWMsgElemData *) (current->data))->type) {

			case CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE:
				if (!(CWAssembleMsgElemACIPv4List(&(msgElems[++k]))))
					goto cw_assemble_error;	
				break;			
			case CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE:
				if (!(CWAssembleMsgElemACIPv6List(&(msgElems[++k]))))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				if (!(CWAssembleMsgElemResultCode(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value.int32)))
					goto cw_assemble_error;
				break;
			/*
			case CW_MSG_ELEMENT_SESSION_ID_CW_TYPE:
				if (!(CWAssembleMsgElemSessionID(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value)))
					goto cw_assemble_error;
				break;
			*/
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:
				if (!(CWAssembleMsgElemImageIdentifier(&(msgElems[++k]), cwRtkAcGetVendorId(), ((CWMsgElemData *) current->data)->value.str))){
					goto cw_assemble_error;
				}
				break;				
	        default: {
                int j;
                for(j = 0; j <= k; j++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[j]);}
                CW_FREE_OBJECT(msgElems);
                return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element for Join Response Message");
				break;
	        }
        }
		current = current->next;
	}

	if (!(CWAssembleMessage(messagesPtr,
				fragmentsNumPtr,
				PMTU,
				seqNum,
				CW_MSG_TYPE_VALUE_JOIN_RESPONSE,
				msgElems,
				msgElemCount + mandatoryMsgElemCount,
				msgElemsBinding,
				msgElemBindingCount,
#ifdef CW_NO_DTLS
				CW_PACKET_PLAIN)))
#else
 				CW_PACKET_CRYPT)))
#endif
		return CW_FALSE;

	CWDebugLog("Join Response Assembled");
	
	return CW_TRUE;

cw_assemble_error:
	{
		int i;
		CWLog("(%s)cw_assemble_error", __FUNCTION__);
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		/* error will be handled by the caller */
		return CW_FALSE;
	}
	return CW_TRUE;
}

/* 
 * Parses Join Request.
 */
CWBool CWParseJoinRequestMessage(unsigned char *msg,
				 int len,
				 int *seqNumPtr,
				 CWProtocolJoinRequestValues *valuesPtr) {

	CWControlHeaderValues controlVal;
	int offsetTillMessages;
	CWProtocolMessage completeMsg;
	unsigned char RadioInfoABGN;
	
	if(msg == NULL || seqNumPtr == NULL || valuesPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parse Join Request");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
		
	if(!(CWParseControlHeader(&completeMsg, &controlVal)))
		/* will be handled by the caller */
		return CW_FALSE;

	/* different type */
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_JOIN_REQUEST)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Join Request as Expected");
	
	*seqNumPtr = controlVal.seqNum;
	/* skip timestamp */
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;
	offsetTillMessages = completeMsg.offset;
	
	/* parse message elements */
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {

		unsigned short int elemType = 0;/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int elemLen =0 ;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);
		
		/* CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen); */
									
		switch(elemType) {
			case CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE:
				if(!(CWParseMsgElemLocationData(&completeMsg, elemLen, &(valuesPtr->location)))) 
					/* will be handled by the caller */
					return CW_FALSE;
				break;
				
			case CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE:
				if(!(CWParseMsgElemWTPBoardData(&completeMsg, elemLen, &(valuesPtr->WTPBoardData)))) 
					/* will be handled by the caller */
					return CW_FALSE;
				break; 
				
			case CW_MSG_ELEMENT_SESSION_ID_CW_TYPE:
				valuesPtr->sessionID  = CWParseMsgElemSessionID(&completeMsg, elemLen);
				break;
				
			case CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE:
				if(!(CWParseMsgElemWTPDescriptor(&completeMsg, elemLen, &(valuesPtr->WTPDescriptor))))
					/* will be handled by the caller */
					return CW_FALSE;
				break;
				
			case CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE:
				if(!(CWParseMsgElemWTPIPv4Address(&completeMsg, elemLen, valuesPtr)))
					/* will be handled by the caller */
					return CW_FALSE;
				break;
				
			case CW_MSG_ELEMENT_WTP_NAME_CW_TYPE:
				if(!(CWParseMsgElemWTPName(&completeMsg, elemLen, &(valuesPtr->name))))
					/* will be handled by the caller */
					return CW_FALSE;
				break;
				
			case CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE:
				if(!(CWParseMsgElemWTPFrameTunnelMode(&completeMsg, elemLen, &(valuesPtr->frameTunnelMode))))
					/* will be handled by the caller */
					return CW_FALSE;
				break;
				
			case CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE:
				if(!(CWParseMsgElemWTPMACType(&completeMsg, elemLen, &(valuesPtr->MACType))))
					/* will be handled by the caller */
					return CW_FALSE;
				break;
				
			case CW_MSG_ELEMENT_IEEE80211_WTP_RADIO_INFORMATION_CW_TYPE:
				if(!(CWParseMsgElemWTPRadioInformation(&completeMsg, elemLen,&RadioInfoABGN)))	return CW_FALSE;
				break;
#ifdef RTK_SMART_ROAMING
			case CW_MSG_ELEMENT_IEEE80211_WTP_CONFIGURATION_CW_TYPE:
				if(!(CWParseMsgElemWTPConfiguration(&completeMsg, elemLen, &(valuesPtr->confs))))
					return CW_FALSE;
				break;
#endif
				
			default:
				completeMsg.offset += elemLen;
				CWLog("Unrecognized Message Element(%d) in Join Request",elemType);
				break;
		}
		/*CWDebugLog("bytes: %d/%d", (completeMsg.offset-offsetTillMessages), controlVal.msgElemsLen);*/
	}

	if (completeMsg.offset != len) 
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
		
	return CW_TRUE;
}

CWBool CWSaveJoinRequestMessage(int WTP_Index, CWProtocolJoinRequestValues *joinRequest, CWWTPManager *wtpManager, CWProtocolResultCode *resultCodeValue)
{

	CWWTPProtocolManager *WTPProtocolManager = &wtpManager->WTPProtocolManager;
	CWDebugLog("Saving Join Request...");

	// location
	if ((joinRequest->location)!= NULL) {
		CW_FREE_OBJECT(WTPProtocolManager->locationData);
		WTPProtocolManager->locationData= joinRequest->location;
	} else {
		*resultCodeValue = CW_PROTOCOL_FAILURE_MISSING_MSG_ELEM;
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}

	// board data
	if (joinRequest->WTPBoardData.vendorInfosCount >0) {		
		int i;
		CWBool our_board = CW_FALSE;
		for (i=0; i<joinRequest->WTPBoardData.vendorInfosCount; i++) {
			if (joinRequest->WTPBoardData.vendorInfos[i].type == CW_WTP_MODEL_NUMBER && 
				joinRequest->WTPBoardData.vendorInfos[i].vendorIdentifier == VENDOR_IANA_ID) {
					our_board = CW_TRUE;
					break;
				}

		}
		if (!our_board) {
			CWLog("AC rejected an WTP(%s) since VENDOR ID is wrong...", joinRequest->name);
			*resultCodeValue = CW_PROTOCOL_FAILURE_MISSING_MSG_ELEM;
			return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
		}
		
		CW_FREE_OBJECT(WTPProtocolManager->WTPBoardData.vendorInfos);
		WTPProtocolManager->WTPBoardData = joinRequest->WTPBoardData;

	} else {
		CWLog("AC rejected an WTP(%s) since no Vender Info in Join Request...", joinRequest->name);
		*resultCodeValue = CW_PROTOCOL_FAILURE_MISSING_MSG_ELEM;
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	// name
	if ((joinRequest->name)!= NULL && CWAcDbWtpLookup(joinRequest->name, &wtpManager->wtpConfig)) {
	} else {
		wtpManager->wtpConfig = default_wtp_config;
		default_wtp_config.radios[0].wlans[0].bssid[5]++;
	}
	CW_FREE_OBJECT(WTPProtocolManager->name);
	WTPProtocolManager->name= joinRequest->name;

#ifdef RTK_SMART_ROAMING
	gWTPsStatus[WTP_Index] = 1;
	memset(gWTPsWlan[WTP_Index].wlan0, 0, 6);
	memset(gWTPsWlan[WTP_Index].wlan1, 0, 6);
	
	int k;
	for(k=0; k<joinRequest->confs.wlan_num; k++)
	{			
		if(joinRequest->confs.wlan[k].ID == 0){
			memcpy(gWTPsWlan[WTP_Index].wlan0,joinRequest->confs.wlan[k].mac_addr,6);			
			CWDebugLog("Save WTP%d wlan0 mac=%02x:%02x:%02x:%02x:%02x:%02x",
				WTP_Index, gWTPsWlan[WTP_Index].wlan0[0], gWTPsWlan[WTP_Index].wlan0[1],
				gWTPsWlan[WTP_Index].wlan0[2], gWTPsWlan[WTP_Index].wlan0[3],
				gWTPsWlan[WTP_Index].wlan0[4], gWTPsWlan[WTP_Index].wlan0[5]);
		}
		else if (joinRequest->confs.wlan[k].ID == 1){
			memcpy(gWTPsWlan[WTP_Index].wlan1,joinRequest->confs.wlan[k].mac_addr,6);
			CWDebugLog("Save WTP%d wlan1 mac=%02x:%02x:%02x:%02x:%02x:%02x",
				WTP_Index, gWTPsWlan[WTP_Index].wlan1[0], gWTPsWlan[WTP_Index].wlan1[1],
				gWTPsWlan[WTP_Index].wlan1[2], gWTPsWlan[WTP_Index].wlan1[3],
				gWTPsWlan[WTP_Index].wlan1[4], gWTPsWlan[WTP_Index].wlan1[5]);
		}
	}

#endif

#ifdef KM_BY_AC // key management	
	{
		int i;		
		char cmd[256];
		for (i=0; i<wtpManager->wtpConfig.nRadios; i++) {
			char str_buff[128];

			// TODO Babylon: the check should also in Discovery state.
			// test access of config file.
			sprintf(str_buff, AC_HAPD_PATH "%s", wtpManager->wtpConfig.radios[i].hapd_cfg_file);
			if (access(str_buff, R_OK)!=0) {			
				CWLog("Hostapd Config File (%s) is not R_OK!", str_buff);
				*resultCodeValue = CW_PROTOCOL_JOIN_FAILURE_UNKNOWN_SRC;
				return CW_FALSE;
			}
			if (access(str_buff, W_OK)!=0) {			
				CWLog("Hostapd Config File (%s) is not W_OK!", str_buff);
				*resultCodeValue = CW_PROTOCOL_JOIN_FAILURE_UNKNOWN_SRC;
				return CW_FALSE;
			}			
		}
	}
#endif

	if(WTPProtocolManager->sessionID!=NULL)
	{
		//printf("%s:%d free WTPProtocolManager->sessionID!!!\n",__FUNCTION__,__LINE__);
		CW_FREE_OBJECT(WTPProtocolManager->sessionID);
	}
	WTPProtocolManager->sessionID= joinRequest->sessionID;
	WTPProtocolManager->ipv4Address= joinRequest->addr;
	
	if(WTPProtocolManager->descriptor.vendorInfos.vendorInfos!=NULL)
	{
		//printf("%s:%d free WTPProtocolManager->descriptor.vendorInfos.vendorInfos!!!\n",__FUNCTION__,__LINE__);
		CW_FREE_OBJECT(WTPProtocolManager->descriptor.vendorInfos.vendorInfos);
	}
	WTPProtocolManager->descriptor= joinRequest->WTPDescriptor;
	WTPProtocolManager->radiosInfo.radioCount = (joinRequest->WTPDescriptor).radiosInUse;
	CW_FREE_OBJECT(WTPProtocolManager->radiosInfo.radiosInfo);

	CW_CREATE_ARRAY_ERR(WTPProtocolManager->radiosInfo.radiosInfo, 
			    WTPProtocolManager->radiosInfo.radioCount, 
			    CWWTPRadioInfoValues,
			    *resultCodeValue = CW_PROTOCOL_JOIN_FAILURE_RES_DEPLETION; return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	int i;

	for(i=0; i< WTPProtocolManager->radiosInfo.radioCount; i++) {

		WTPProtocolManager->radiosInfo.radiosInfo[i].radioID = i;
        /*WTPProtocolManager->radiosInfo.radiosInfo[i].stationCount = 0;*/

		/* default value for CAPWAP */
        WTPProtocolManager->radiosInfo.radiosInfo[i].adminState = CW_ENABLED;
        WTPProtocolManager->radiosInfo.radiosInfo[i].adminCause = AD_NORMAL;
        WTPProtocolManager->radiosInfo.radiosInfo[i].operationalState = CW_DISABLED;
        WTPProtocolManager->radiosInfo.radiosInfo[i].operationalCause = OP_NORMAL;
        WTPProtocolManager->radiosInfo.radiosInfo[i].TxQueueLevel = 0;
        WTPProtocolManager->radiosInfo.radiosInfo[i].wirelessLinkFramesPerSec = 0; 
	}
	CWDebugLog("Join Request Saved (%s)", WTPProtocolManager->name);
	
	printf("AP <%s> Joined with %d radios.\n", WTPProtocolManager->name, WTPProtocolManager->radiosInfo.radioCount);
	return CW_TRUE;
}

#if 0
CWBool ACEnterJoinPost(int WTPIndex, CWProtocolMessage *msgPtr)
{
	CWProtocolMessage completeMsg;	
	CWControlHeaderValues controlVal;
	
	completeMsg.msg = msgPtr->msg;
	completeMsg.offset = 0;
		
	if(!(CWParseControlHeader(&completeMsg, &controlVal)))
		/* will be handled by the caller */
		return CW_FALSE;

	switch (controlVal.messageTypeValue)
	{
	case CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST:
		break;
	case CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST:
		break;
	default:
		break;
	}
	/* different type */
	if( controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_JOIN_REQUEST && 
		controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST &&
		)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Join Request as Expected");
	

	return CW_TRUE;
}
#endif

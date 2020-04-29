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

#include "CWAC.h"
#include "CWVendorPayloads.h"
#include "CWFreqPayloads.h"
#include "ACAppsProtocol.h"
#include "common.h"
#include "ieee802_11_defs.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include "ACRtkSystemInteraction.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

CWBool CWACParseGenericRunMessage(int WTPIndex,
				  CWProtocolMessage *msg,
				  CWControlHeaderValues* controlVal);

CWBool CWAssembleResetRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, const char *reqImage);								
CWBool CWParseResetResponseMessage(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode);

CWBool CWAssembleImageDataRequestByAC(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, 
										CWBool data_type_eof, void *data, int byteToSend);


CWBool CWParseConfigurationUpdateResponseMessage(CWProtocolMessage* msgPtr,
						 int len,
						 CWProtocolConfigurationUpdateResponseValues *pVals);

/*
CWBool CWSaveConfigurationUpdateResponseMessage(CWProtocolResultCode resultCode,
						int WTPIndex,
						CWProtocolVendorSpecificValues* protocolValues);
 */
void CWSaveConfigurationUpdateResponseMessage(int WTPIndex,
						CWProtocolConfigurationUpdateResponseValues* updateResp);

CWBool CWParseClearConfigurationResponseMessage(CWProtocolMessage* msgPtr,
						int len,
						CWProtocolResultCode* resultCode);

CWBool CWParseWLANConfigurationResponseMessage(CWProtocolMessage* msgPtr,
						int len,
						CWProtocolResultCode* resultCode);

CWBool CWParseStationConfigurationResponseMessage(CWProtocolMessage* msgPtr,
						  int len,
						  CWProtocolResultCode* resultCode);

CWBool CWParseWTPDataTransferRequestMessage(CWProtocolMessage *msgPtr,
					    int len,
					    CWProtocolWTPDataTransferRequestValues *valuesPtr);

CWBool CWAssembleWTPDataTransferResponse(CWProtocolMessage **messagesPtr,
					 int *fragmentsNumPtr,
					 int PMTU, int seqNum);
#ifdef RTK_SMART_ROAMING
CWBool CWParseMsgElemUpdateStationValues (CWProtocolMessage *msgPtr,
					int len,
					CWProtocolUpdateStationValues *valPtr);
CWBool CWSaveMsgElemUpdateStationValues (int WTP_Index,
					CWProtocolUpdateStationValues *upd_station);
CWBool CWParseMsgElemAddStationValues(CWProtocolMessage *msgPtr, 
					int len, 
					CWProtocolAddStationValues *valPtr);	
CWBool CWSaveMsgElemAddStationValues(int WTP_Index, 
					CWProtocolAddStationValues *add_station);
CWBool CWParseMsgElemUpdateStationDot11kValues(unsigned char radio_id,
					CWProtocolMessage *msgPtr,
					int len,
					Dot11kValues *valPtr);	
CWBool CWSaveMsgElemUpdateStationDot11kValues(int wlanNum,
					CWProtocolUpdateStationDot11kValues *upd_station_11k);
CWBool CWParseMsgElemNewStationValues(CWProtocolMessage *msgPtr,
					int len,
					CWProtocolAddStationValues *valPtr);
CWBool CWSaveMsgElemNewStationNotifyValues(int WTP_Index,
					unsigned char radio_id,
					unsigned char *station_mac);
CWBool CWParseWTPEventRequestMessage(int WTP_Index, 
					CWProtocolMessage *msgPtr,
				    int len,
				    CWProtocolWTPEventRequestValues *valuesPtr);
#if 0
CWBool CWSaveWTPEventRequestMessage(int WTP_Index, 
				CWProtocolWTPEventRequestValues *WTPEventRequest);
#endif
CWBool CWAssembleWTPEventResponse(int WTP_Index, 
				CWProtocolMessage **messagesPtr,
				  int *fragmentsNumPtr,
				  int seqNum);
#endif

CWBool CWParseChangeStateEventRequestMessage2(CWProtocolMessage *msgPtr,
					      int len,
					      CWProtocolChangeStateEventRequestValues **valuesPtr);

CWBool CWParseEchoRequestMessage(CWProtocolMessage *msgPtr,
				 int len);

CWBool CWAssembleEchoResponse(CWProtocolMessage **messagesPtr,
			      int *fragmentsNumPtr,
			      int PMTU,
			      int seqNum);

CWBool CWStartNeighborDeadTimer(int WTPIndex);
CWBool CWStopNeighborDeadTimer(int WTPIndex);
CWBool CWRestartNeighborDeadTimer(int WTPIndex);
/* Babylon: This function is not used.
CWBool CWRestartNeighborDeadTimerForEcho(int WTPIndex);
*/
int flush_pcap(u_char *buf,int len,char *filename){
	
	FILE *file;
	file = fopen(filename,"a+");
	u_char index=0x00;
	int cnt=0;
	int i;
	int giro=0;
	for(i=0;cnt<len ;i++){
		fprintf(file,"0%02X0   ",index);
		for(;cnt<len;){
			fprintf(file,"%02X ",buf[cnt]);
			cnt++;
			if(giro==15){
				giro=0;
				break;
			}
			giro++;
		}
		fprintf(file,"\n");
		index++;
	}

	fprintf(file,"\n");
	fclose(file); 
	return 0;
}

#define HLEN_80211 24
int isEAPOL_Frame( unsigned char *buf, int len){
	unsigned char rfc1042_header[6] = { 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00 };
	int i;
	
	for(i=0; i<6; i++)if(rfc1042_header[i]!=buf[i + HLEN_80211])return 0;
	return 1;
}

CWBool ACEnterRun(int WTPIndex, CWProtocolMessage *msgPtr, CWBool dataFlag) {
	
	CWBool toSend= CW_FALSE, timerSet = CW_TRUE;
	CWControlHeaderValues controlVal;
	CWProtocolMessage* messages =NULL;
	int messagesCount=0;

	msgPtr->offset = 0;
#ifdef KM_BY_AC
	if(dataFlag){
		/* We have received a Data Message... now just log this event and do actions by the dataType */
		
		CWDebugLog("--> Received a DATA Message");

		if(msgPtr->data_msgType == CW_DATA_MSG_FRAME_TYPE)	{
			unsigned char StationMacAddr[MAC_ADDR_LEN];
			/*Retrive mac address station from msg*/
			memset(StationMacAddr, 0, MAC_ADDR_LEN);
			memcpy(StationMacAddr, msgPtr->msg+SOURCE_ADDR_START, MAC_ADDR_LEN);
	
			int seqNum = CWGetSeqNum();

			//Send a Station Configuration Request
			if (CWAssembleStationConfigurationRequest(&(gWTPs[WTPIndex].messages),
							  &(gWTPs[WTPIndex].messagesCount),
							  gWTPs[WTPIndex].pathMTU,
							  seqNum,StationMacAddr, 0, // TODO Babylon: should lookup to find the radio index.
							  CW_MSG_ELEMENT_ADD_STATION_CW_TYPE)) {
// Just for test !!!
//printf("Babylon Test: !!!!!Unexpected!!!!\n");
return CW_FALSE;
				if(CWACSendAcknowledgedPacket(WTPIndex, 
						      CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE,
						      seqNum)) 
					return CW_TRUE;
				else
					CWACStopRetransmission(WTPIndex);
			}
			
			CWDebugLog("Send a Station Configuration Request");
		}else if(msgPtr->data_msgType == CW_DATA_MSG_KEEP_ALIVE_TYPE){
			
			unsigned char *valPtr=NULL;
			CWProtocolMessage *messages = NULL;
			CWProtocolMessage sessionIDmsgElem;
			int fragmentsNum = 0;
			int i;
			int dataSocket=0;
			unsigned short int elemType = 0;
			unsigned short int elemLen = 0;
			CWNetworkLev4Address address;

			CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
			valPtr = CWParseMsgElemSessionID(msgPtr, elemLen);
			CWAssembleMsgElemSessionID(&sessionIDmsgElem, valPtr);
			
			if (!CWAssembleDataMessage(&messages, 
				  &fragmentsNum, 
				  gWTPs[WTPIndex].pathMTU, 
				  &sessionIDmsgElem, 
				  NULL,
				  CW_PACKET_PLAIN,
				  1
				  ))
			{
				CWLog("Failure Assembling KeepAlive Request");
				if(messages)
					for(i = 0; i < fragmentsNum; i++) {
						CW_FREE_PROTOCOL_MESSAGE(messages[i]);
					}	
				CW_FREE_OBJECT(messages);
				return CW_FALSE;
			}

			for(i = 0; i < gACSocket.count; i++) {
			    if (gACSocket.interfaces[i].sock == gWTPs[WTPIndex].socket){
				dataSocket = gACSocket.interfaces[i].dataSock;
				CW_COPY_NET_ADDR_PTR(&address,&(gWTPs[WTPIndex].dataaddress));
				break;
			    }
			}

			if (dataSocket == 0){
			      CWLog("data socket of WTP %d isn't ready.");
			      return CW_FALSE;
			}

			for(i = 0; i < fragmentsNum; i++) {
				if(!CWNetworkSendUnsafeUnconnected(	dataSocket, 
									&(address), 
									messages[i].msg, 
									 messages[i].offset))
				{
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
			
		}else if(msgPtr->data_msgType == CW_IEEE_802_3_FRAME_TYPE){
			CWDebugLog("802.3 data - TODO");
/*	Babylon 2014: we have removed TAP..
			CWDebugLog("Write 802.3 data to TAP[%d], len:%d",gWTPs[WTPIndex].tap_fd,msglen);
			write(gWTPs[WTPIndex].tap_fd, msgPtr->msg, msglen);
 */		
		}else if(msgPtr->data_msgType == CW_IEEE_802_11_FRAME_TYPE)	{
			int msglen = msgPtr->offset;
			struct ieee80211_hdr *hdr;
			u16 fc;
			hdr = (struct ieee80211_hdr *) msgPtr->msg;
			fc = le_to_host16(hdr->frame_control);
//printf("ACRunState: Got frame\n");
			if( WLAN_FC_GET_TYPE(fc) == WLAN_FC_TYPE_MGMT || isEAPOL_Frame(msgPtr->msg,msglen) ){
//printf("ACRunState: Got 802.11 management frame, fc = %04x\n", fc);
//printf("hdr->addr3 = %02x %02x %02x %02x %02x %02x \n", hdr->addr3[0], hdr->addr3[1], hdr->addr3[2], hdr->addr3[3], hdr->addr3[4], hdr->addr3[5]);
				if ((fc & (WLAN_FC_TODS|WLAN_FC_TODS)) == 0) {
					int i;
					for (i=0; i<gWTPs[WTPIndex].WTPProtocolManager.radiosInfo.radioCount; i++) {
						unsigned char *mac = gWTPs[WTPIndex].wtpConfig.radios[i].baseMAC;						
//printf("radio: %d, mac = %02x %02x %02x %02x %02x %02x \n", i, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
						if (hdr->addr3[0]==mac[0] && hdr->addr3[1]==mac[1] && hdr->addr3[2]==mac[2] && 
							hdr->addr3[3]==mac[3] && hdr->addr3[4]==mac[4] && hdr->addr3[5]-mac[5]<16) {

#ifdef KM_BY_AC // key management
							CWACsend_data_to_hostapd(WTPIndex, i, msgPtr->msg, msglen); 
#else
							//printf("TODO: replace CWACsend_data_to_hostapd()\n");
#endif
							break;
						}
					}
				}
			}else if( WLAN_FC_GET_TYPE(fc) == WLAN_FC_TYPE_DATA ){
//printf("Babylon test: Got 802.11 data frame\n");				
				if( WLAN_FC_GET_STYPE(fc) == WLAN_FC_STYPE_NULLFUNC ){
					if ((fc & (WLAN_FC_TODS|WLAN_FC_TODS)) == 0) {
						int i;
						for (i=0; i<gWTPs[WTPIndex].WTPProtocolManager.radiosInfo.radioCount; i++) {
							unsigned char *mac = gWTPs[WTPIndex].wtpConfig.radios[i].baseMAC;
							if (hdr->addr3[0]==mac[0] && hdr->addr3[1]==mac[1] && hdr->addr3[2]==mac[2] && 
								hdr->addr3[3]==mac[3] && hdr->addr3[4]==mac[4] && hdr->addr3[5]-mac[5]<16) {
#ifdef KM_BY_AC // key management								
								CWACsend_data_to_hostapd(WTPIndex, i, msgPtr->msg, msglen); 	
#else
								//printf("TODO: replace CWACsend_data_to_hostapd()\n");
#endif
								break;
							}
						}
					}
				}else{

/*	Babylon 2014: we have removed TAP..
					int write_bytes = write(gWTPs[WTPIndex].tap_fd, msgPtr->msg + HLEN_80211, msglen - HLEN_80211);
			
			
					if(write_bytes != (msglen - 24)){
						CWLog("%02X %02X %02X %02X %02X %02X ",msgPtr->msg[0],
																	msgPtr->msg[1],
																	msgPtr->msg[2],
																	msgPtr->msg[3],
																	msgPtr->msg[4],
																	msgPtr->msg[5]);
						
						CWLog("Error:. RecvByte:%d, write_Byte:%d ",msglen - 24,write_bytes);
					}
*/				
				}

				
			}else{
//printf("Babylon test: Got 802.11 control frame\n");
/*	Babylon 2014: we have removed TAP..
				write(gWTPs[WTPIndex].tap_fd, msgPtr->msg + HLEN_80211, msglen - HLEN_80211);
 */
				CWDebugLog("Control Frame !!!\n");
			}
			
			//flush_pcap(msgPtr->msg, msglen, "cap_wtp_to_ac.txt");
			
		}else{
			CWDebugLog("Manage special data packets with frequency");

			/************************************************************
			 * Update 2009:												*
			 *				Manage special data packets with frequency	*
			 *				statistics informations.					*
			 ************************************************************/
			
			if( msgPtr->data_msgType == CW_DATA_MSG_FREQ_STATS_TYPE ) {
				
				int cells; /* How many cell are heard */
                int isAck;
				char * freqPayload; 
				int socketIndex, indexToSend = htonl(WTPIndex);
				
				int sizeofAckInfoUnit = CW_FREQ_ACK_SIZE;
				int sizeofFreqInfoUnit = CW_FREQ_CELL_INFO_PAYLOAD_SIZE;
				int sizeOfPayload = 0, payload_offset = 0;
				
				/*-----------------------------------------------------------------------------------------------
				 *	Payload Management ( infos for frequency application) :
				 *		Ack       Structure : |  WTPIndex  |   Ack Value  | 
				 *      Freq Info Structure : |  WTPIndex  |  Number of cells  |  Frequecies Info Payload | 
				 *-----------------------------------------------------------------------------------------------*/
				
                memcpy(&isAck, msgPtr->msg, sizeof(int));

				isAck = ntohl(isAck);
				
                if ( isAck == 0 ) { /* isnt an ack message */
					memcpy(&cells, msgPtr->msg+sizeof(int), sizeof(int));
					cells = ntohl(cells);
					sizeOfPayload = ( cells * sizeofFreqInfoUnit) + (2*sizeof(int)); 
				}
				else {
					sizeOfPayload = sizeofAckInfoUnit;
				}
				
                if ( ( freqPayload = malloc(sizeOfPayload) ) != NULL ) {
					
					memset(freqPayload, 0, sizeOfPayload);
					memcpy(freqPayload, &indexToSend, sizeof(int));
					payload_offset += sizeof(int);
					
					if ( isAck == 0 ) {
						memcpy(freqPayload+payload_offset, msgPtr->msg+sizeof(int), sizeOfPayload-payload_offset);
					}
					else {
						memcpy(freqPayload+payload_offset, msgPtr->msg+sizeof(int), sizeOfPayload-payload_offset);
					}
					
					CWThreadMutexLock(&(gWTPs[WTPIndex].interfaceMutex));
					socketIndex = gWTPs[WTPIndex].applicationIndex; 
					gWTPs[WTPIndex].applicationIndex = -1;
					CWThreadMutexUnlock(&(gWTPs[WTPIndex].interfaceMutex));
					
					/****************************************************
					 *		Forward payload to correct application 		*
					 ****************************************************/
					
					if(!CWErr(CWThreadMutexLock(&appsManager.socketMutex[socketIndex]))) {
						CWLog("[ACrunState]:: Error locking socket Application Mutex");
						free(freqPayload);
						return CW_FALSE;
					}
					
					if ( Writen(appsManager.appSocket[socketIndex], freqPayload, sizeOfPayload)  < 0 ) {
                      CWThreadMutexUnlock(&appsManager.socketMutex[socketIndex]);
                      free(freqPayload);
                      CWLog("[ACrunState]:: Error writing Message To Application");
                      return CW_FALSE;
					}
					
					CWThreadMutexUnlock(&appsManager.socketMutex[socketIndex]);
					free(freqPayload);
				}
				else
					 CWLog("[ACrunState]:: Malloc error (payload to frequency application");
			}
			

		  if(msgPtr->data_msgType == CW_DATA_MSG_STATS_TYPE)
			{
			  char socketctl_path_name[50];
			  char socketserv_path_name[50];
			  char string[10];
			  if(!UnixSocksArray[WTPIndex].data_stats_sock)
				{	//Init Socket only the first time when the function is called
				  if ((UnixSocksArray[WTPIndex].data_stats_sock = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) 
					{
					  CWDebugLog("Error creating socket for data send");
					  return CW_FALSE;
    				}
				  
				  memset(&(UnixSocksArray[WTPIndex].clntaddr),(int)NULL, sizeof(UnixSocksArray[WTPIndex].clntaddr));
				  UnixSocksArray[WTPIndex].clntaddr.sun_family = AF_UNIX;
				  
				  //make unix socket client path name by index i 
				snprintf(string,sizeof(string),"%d",WTPIndex);
				string[sizeof(string)-1]=0;
				strcpy(socketctl_path_name,SOCKET_PATH_AC);
				strcat(socketctl_path_name,string);
				strcpy(UnixSocksArray[WTPIndex].clntaddr.sun_path,socketctl_path_name);
				
				unlink(socketctl_path_name);
				
				memset(&(UnixSocksArray[WTPIndex].servaddr),(int)NULL, sizeof(UnixSocksArray[WTPIndex].servaddr));
				UnixSocksArray[WTPIndex].servaddr.sun_family = AF_UNIX;

				//make unix socket server path name by index i 
				strcpy(socketserv_path_name, SOCKET_PATH_RECV_AGENT);
				strcat(socketserv_path_name, string);
				strcpy(UnixSocksArray[WTPIndex].servaddr.sun_path, socketserv_path_name);
				//printf("\n%s\t%s",socketserv_path_name,socketctl_path_name);fflush(stdout);
				}
			  

			  int nbytes;
			  int pDataLen=656; //len of Monitoring Data
			  
			  //Send data stats from AC thread to monitor client over unix socket
			  nbytes = sendto(UnixSocksArray[WTPIndex].data_stats_sock, msgPtr->msg,pDataLen, 0,
							  (struct sockaddr *) &(UnixSocksArray[WTPIndex].servaddr),sizeof(UnixSocksArray[WTPIndex].servaddr));
			  if (nbytes < 0) 
				{
				  CWDebugLog("Error sending data over socket");
				  return CW_FALSE;
				}
			
			}
		}

		return CW_TRUE;
	}
#else
	
	if(dataFlag){	CWLog("Error: %s should not get but got data flag\n", __FUNCTION__); }
#endif
	if(!(CWACParseGenericRunMessage(WTPIndex, msgPtr, &controlVal))) {
		/* Two possible errors: WRONG_ARG and INVALID_FORMAT
		 * In the second case we have an unexpected response: ignore the
		 * message and log the event.
		 */
		CWDebugLog("Parse Error (header)\n");
		return CW_FALSE;
	}

	switch(controlVal.messageTypeValue) {
		case CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE:
		{
			CWProtocolConfigurationUpdateResponseValues updateRsp;
			
			if(!(CWParseConfigurationUpdateResponseMessage(msgPtr, controlVal.msgElemsLen, &updateRsp)))
				return CW_FALSE;
			
			CWACStopRetransmission(WTPIndex);
			
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}

			CWSaveConfigurationUpdateResponseMessage(WTPIndex, &updateRsp);

			
			if (gWTPs[WTPIndex].interfaceCommandProgress == CW_TRUE) {

				CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
				
				gWTPs[WTPIndex].interfaceCommandProgress = CW_FALSE;
				CWSignalThreadCondition(&gWTPs[WTPIndex].interfaceComplete);

				CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
			}

			break;
		}
		case CW_MSG_TYPE_VALUE_RESET_RESPONSE:
		{
			CWProtocolResultCode resultCode;
			if(!CWParseResetResponseMessage(msgPtr, controlVal.msgElemsLen, &resultCode))
				return CW_FALSE;
			
			CWACStopRetransmission(WTPIndex);

			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}

			if (resultCode != CW_PROTOCOL_SUCCESS) {
				CWLog("WTP %d Reset Failed! result code = %d", WTPIndex, resultCode);
				break;
			}
			
			gWTPs[WTPIndex].isRequestClose = CW_TRUE;
			CWSignalThreadCondition(&gWTPs[WTPIndex].interfaceWait); 
					/* TODO Babylon: 
					 *		I think this can be removed, since in the same thread, 
					 * 		signal is not necessary but no harm.
					 */
			break;			
		}
		case CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE:
		{
			CWProtocolResultCode resultCode;
			int seqNum;
			if (gWTPs[WTPIndex].imageActionState != TRANSMITING_IMAGE_DATA_TO_WTP) {
				CWLog("Error: imageActionState != WATING_FOR_IMAGE_DATA_RESPONSE_FROM_WTP, but got Image Data Response"
					"imageActionState=%d", gWTPs[WTPIndex].imageActionState);
				break;
			}
			
			if(!CWParseImageDataResponseByAC(msgPtr, controlVal.msgElemsLen, &resultCode)) {
				CWLog("CWParseImageDataResponseByAC failed. Ignored!");
				break;
					}
//printf("stop retransmition\n");
			if (resultCode != CW_PROTOCOL_SUCCESS) {
				CWLog("Error: image data response result code = %d", resultCode);				
				break; //ignore this response. (remain ReTx)
			}
			CWACStopRetransmission(WTPIndex);
			//static int r=0;
//printf("stop retrans %d\n", r++);
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}

			if (gWTPs[WTPIndex].reqImageLastMsg) {
				// All data transfer done. reset WTP!
				seqNum = CWGetSeqNum();
				if (!CWAssembleResetRequest(&(gWTPs[WTPIndex].messages),
										&(gWTPs[WTPIndex].messagesCount),
										gWTPs[WTPIndex].pathMTU,
										seqNum, gWTPs[WTPIndex].reqImageIdentifier)) {
					CWLog("Cannot assemble Reset Request");
					return CW_FALSE;				  
				}
				if(!CWACSendAcknowledgedPacket(WTPIndex, CW_MSG_TYPE_VALUE_RESET_RESPONSE, seqNum)) {
					CWLog("CWACSendAcknowledgedPacket for RESET failed!");
					CWACStopRetransmission(WTPIndex);
					return CW_FALSE;
				}	
				gWTPs[WTPIndex].imageActionState = WATING_FOR_RESET_RESPONSE_FROM_WTP;
				
			} else if (gWTPs[WTPIndex].pReqImageData) { 
				// file still open, has more data to transfer
				if(!CWSendOneImageDataRequest(WTPIndex)) {
					CWLog("CWSendOneImageDataRequest filed");
					return CW_FALSE;
				}
			} else {
				// Last message was sent, but image file was closed, strange situation!
				CWLog("Last message was sent, but image file was closed, strange situation!");
				return CW_FALSE;
			}
			break;
		}

		case CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST:
		{
			CWProtocolChangeStateEventRequestValues *valuesPtr;
		
			if(!(CWParseChangeStateEventRequestMessage2(msgPtr, controlVal.msgElemsLen, &valuesPtr)))
				return CW_FALSE;
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
			if(!(CWSaveChangeStateEventRequestMessage(valuesPtr, &(gWTPs[WTPIndex].WTPProtocolManager))))
				return CW_FALSE;
			if(!(CWAssembleChangeStateEventResponse(&messages,
								&messagesCount,
								gWTPs[WTPIndex].pathMTU,
								controlVal.seqNum)))
				return CW_FALSE;

			toSend = CW_TRUE;
			break;
		}
		case CW_MSG_TYPE_VALUE_ECHO_REQUEST:
		{
			if(!(CWParseEchoRequestMessage(msgPtr, controlVal.msgElemsLen)))
				return CW_FALSE;

			gWTPs[WTPIndex].echoRequestCounter++;
			gWTPs[WTPIndex].lastEchoRequestTime = time(NULL);
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
			
			if(!(CWAssembleEchoResponse(&messages,
						    &messagesCount,
						    gWTPs[WTPIndex].pathMTU,
						    controlVal.seqNum)))
				return CW_FALSE;

			toSend = CW_TRUE;	
			break;
		}
		case CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST:
		{
			int i;
			CWProtocolImageIdentifierValues image_identifier;
			CWBool GotInitiate;
			
			if (gWTPs[WTPIndex].imageActionState != WATING_FOR_IMAGE_DATA_REQUEST_FROM_WTP) {
				CWLog("got Image Data Request when imageActionState is not in WATING_FOR_IMAGE_DATA_REQUEST_FROM_WTP."
					"imageActionState=%d", gWTPs[WTPIndex].imageActionState);
				return CW_FALSE;
			}

			image_identifier.imageIdentifier[0] = '\0';
			if(!(CWParseImageDataRequestByAC(msgPtr, controlVal.msgElemsLen, &image_identifier, &GotInitiate)))
				return CW_FALSE;

			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}

			if (image_identifier.imageIdentifier[0] == '\0' || GotInitiate == CW_FALSE ||
				strcmp(image_identifier.imageIdentifier, gWTPs[WTPIndex].reqImageIdentifier) != 0) {
				CWLog("Got Image Data Request, but unexpected. image id=%s", image_identifier.imageIdentifier);
				if (!GotInitiate) {
					CWLog("No Message Element 'Initiated Download'");
				}
			}
			
			// Assemble Image Data Response
			if(!(CWAssembleImageDataResponseByAC(&messages,
						    &messagesCount,
						    gWTPs[WTPIndex].pathMTU,
						    controlVal.seqNum, CW_PROTOCOL_SUCCESS, 0, NULL))) {	// TODO Babylon: count hash
				gWTPs[WTPIndex].imageActionState = NO_ACTION;
				gWTPs[WTPIndex].reqImageIdentifier[0] = '\0'; 						    
				return CW_FALSE;
			}
			// send Image Data Response
			for(i = 0; i < messagesCount; i++) {
#ifdef CW_NO_DTLS
				if(!CWNetworkSendUnsafeUnconnected(gWTPs[WTPIndex].socket, 
								   &gWTPs[WTPIndex].address, 
								   messages[i].msg, 
								   messages[i].offset)	)
#else
				if(!(CWSecuritySend(gWTPs[WTPIndex].session,
							messages[i].msg,
							messages[i].offset)))
#endif
				{
					CWFreeMessageFragments(messages, messagesCount);
					CW_FREE_OBJECT(messages);
					
					gWTPs[WTPIndex].imageActionState = NO_ACTION;
					gWTPs[WTPIndex].reqImageIdentifier[0] = '\0'; 
					return CW_FALSE;
				}
			}
			CWFreeMessageFragments(messages, messagesCount);
			CW_FREE_OBJECT(messages);

			if (!CWSendOneImageDataRequest(WTPIndex)) {				
				gWTPs[WTPIndex].imageActionState = NO_ACTION;
				gWTPs[WTPIndex].reqImageIdentifier[0] = '\0'; 
				return CW_FALSE;
			}
			break;
		}
#ifdef RTK_SMART_ROAMING
		case CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE:
		{
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
			//CWSaveStationConfigurationResponseMessage(resultCode, WTPIndex);	<-- Must be Implemented ????
			break;
		}	
#endif
#ifdef RTK_CAPWAP
		case CW_MSG_TYPE_VALUE_RTK_WTP_STATUS_RESPONSE:
		{
			CWProtocolRtkWTPStatusResponseValues wtpStatus;
			
			if(!CWParseRtkWTPStatusResponse(msgPtr, controlVal.msgElemsLen, &wtpStatus)) {
				break;
			}
			CWACStopRetransmission(WTPIndex);
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
			// for site survey, response to Wum.
			if (wtpStatus.siteSurveyResponsed) {				
				char *msgToWum = NULL;
				int msgToWumLen = 0;
				
				if (wtpStatus.resultCode == CW_PROTOCOL_SUCCESS) {
					int i;
					msgToWum = malloc(sizeof(CWRtkSiteSurveyValues)*wtpStatus.numSiteSurvey+1);	
//printf("malloc(%u) -> %p\n", 1+ sizeof(CWRtkSiteSurveyValues) * wtpStatus.numSiteSurvey, msgToWum);			
					msgToWum[msgToWumLen] = wtpStatus.numSiteSurvey;
					msgToWumLen++;
					for (i=0; i<wtpStatus.numSiteSurvey; i++) {			
						int copyBytes = CW_RTK_SITE_SURVEY_VALUES_USED_BYTES(wtpStatus.siteSurvey[i]);
						memcpy(&msgToWum[msgToWumLen], &wtpStatus.siteSurvey[i], copyBytes);
						msgToWumLen += copyBytes;
//printf("copyBytes=%d, msgToWumLen=%d\n", copyBytes, msgToWumLen);
					}
				} else {
					msgToWum = malloc (64);	
					CWLog("Error!! GotRTK  WTP Status Response - result code not success = %d", wtpStatus.resultCode);
					sprintf(msgToWum, "Site Survey Failed <= responsed from wtp %s\n", gWTPs[WTPIndex].wtpConfig.name);
					msgToWumLen = strlen(msgToWum) + 1;
				}
				CWResponseToWum(WTPIndex, wtpStatus.resultCode, msgToWumLen, msgToWum);

				free (msgToWum);
				
				if (gWTPs[WTPIndex].interfaceCommandProgress == CW_TRUE) {

					CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
					
					gWTPs[WTPIndex].interfaceCommandProgress = CW_FALSE;
					CWSignalThreadCondition(&gWTPs[WTPIndex].interfaceComplete);

					CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
				}
			}

			// for station list, response to Wum.
			else if (wtpStatus.rtkStationsResponsed) {				
				char *msgToWum = NULL;
				int msgToWumLen = 0;
				
				if (wtpStatus.resultCode == CW_PROTOCOL_SUCCESS) {
					int i;
					int bytes = sizeof(CWRtkStationValues)*wtpStatus.numRtkStations+4;
					msgToWum = malloc(bytes); 
//printf("malloc(%u) -> %p\n", bytes, msgToWum); 		
					*(int *)&msgToWum[msgToWumLen] = htonl(wtpStatus.numRtkStations);
					msgToWumLen+= sizeof(int);
					for (i=0; i<wtpStatus.numRtkStations; i++) { 		
						memcpy(&msgToWum[msgToWumLen], &wtpStatus.rtkStations[i], sizeof(CWRtkStationValues));
						msgToWumLen += sizeof(CWRtkStationValues);
//printf("copyBytes=%d, msgToWumLen=%d\n", sizeof(CWRtkStationValues), msgToWumLen);
					}
				} else {
					msgToWum = malloc (64); 
					CWLog("Error!! Got RTK WTP Status Response - result code not success = %d", wtpStatus.resultCode);
					sprintf(msgToWum, "List Stations Failed <= responsed from wtp %s\n", gWTPs[WTPIndex].wtpConfig.name);
					msgToWumLen = strlen(msgToWum) + 1;
				}
				CWResponseToWum(WTPIndex, wtpStatus.resultCode, msgToWumLen, msgToWum);

				free (msgToWum);
				
				if (gWTPs[WTPIndex].interfaceCommandProgress == CW_TRUE) {

					CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
					
					gWTPs[WTPIndex].interfaceCommandProgress = CW_FALSE;
					CWSignalThreadCondition(&gWTPs[WTPIndex].interfaceComplete);

					CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
				}
			}else {
				CWLog("Protocol error\n");
			}
			break;
		}	
#endif

		case CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_RESPONSE:
		{
			CWProtocolResultCode resultCode;
			if(!(CWParseClearConfigurationResponseMessage(msgPtr, controlVal.msgElemsLen, &resultCode)))
				return CW_FALSE;
			CWACStopRetransmission(WTPIndex);
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
			
			
			if (gWTPs[WTPIndex].interfaceCommandProgress == CW_TRUE)
			{
				CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
				
				gWTPs[WTPIndex].interfaceCommandProgress = CW_FALSE;
				CWSignalThreadCondition(&gWTPs[WTPIndex].interfaceComplete);

				CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
			}

			break;
		}
#ifdef KM_BY_AC
		case CW_MSG_TYPE_VALUE_WLAN_CONFIGURATION_RESPONSE:
		{
			CWProtocolResultCode resultCode;
			if(!(CWParseWLANConfigurationResponseMessage(msgPtr, controlVal.msgElemsLen, &resultCode)))
				return CW_FALSE;
			CWACStopRetransmission(WTPIndex);
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
			
			
			if (gWTPs[WTPIndex].interfaceCommandProgress == CW_TRUE)
			{
				CWThreadMutexLock(&gWTPs[WTPIndex].interfaceMutex);
				
				gWTPs[WTPIndex].interfaceCommandProgress = CW_FALSE;
				CWSignalThreadCondition(&gWTPs[WTPIndex].interfaceComplete);

				CWThreadMutexUnlock(&gWTPs[WTPIndex].interfaceMutex);
			}

			break;
		}
#endif
		case CW_MSG_TYPE_VALUE_DATA_TRANSFER_REQUEST:
		{
			CWProtocolWTPDataTransferRequestValues valuesPtr;
			
			if(!(CWParseWTPDataTransferRequestMessage(msgPtr, controlVal.msgElemsLen, &valuesPtr)))
				return CW_FALSE;
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
			if(!(CWAssembleWTPDataTransferResponse(&messages, &messagesCount, gWTPs[WTPIndex].pathMTU, controlVal.seqNum))) 
				return CW_FALSE;
			toSend = CW_TRUE;
			break;
		}
#ifdef RTK_SMART_ROAMING
		case CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST:
		{
			CWProtocolWTPEventRequestValues valuesPtr;

			if(!(CWParseWTPEventRequestMessage(WTPIndex, 
							msgPtr, 
							controlVal.msgElemsLen, 
							&valuesPtr)))
				return CW_FALSE;
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
			
			if(!(CWAssembleWTPEventResponse(WTPIndex,
							&messages,
							&messagesCount,
							controlVal.seqNum)))
 				return CW_FALSE;

			toSend = CW_TRUE;	
			break;
		}
#endif
		default: 
			/*
			 * We have an unexpected request and we have to send
			 * a corresponding response containing a failure result code
			 */
			CWDebugLog("(%d)--> Not valid Request in Run State... ", controlVal.messageTypeValue);

			if (controlVal.messageTypeValue % 1) {
				CWLog("For invalid request, we send a failure Response");
				if(!(CWAssembleUnrecognizedMessageResponse(&messages,
								   &messagesCount,
								   gWTPs[WTPIndex].pathMTU,
								   controlVal.seqNum,
								   controlVal.messageTypeValue + 1))) 
 					return CW_FALSE;

				toSend = CW_TRUE;
			}
			/*return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message not valid in Run State");*/
	}	
	if(toSend){
		//CWLog("Sending message (wtp idx=%d)(fragment count =%d)", WTPIndex, messagesCount);
		int i;
		if(messages == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
		
		CWLog("Sending message (wtp idx=%d)(fragment count =%d)", WTPIndex, messagesCount);
		for(i = 0; i < messagesCount; i++) {
#ifdef CW_NO_DTLS
			if(!CWNetworkSendUnsafeUnconnected(gWTPs[WTPIndex].socket, 
							   &gWTPs[WTPIndex].address, 
							   messages[i].msg, 
							   messages[i].offset)	)
#else
			if(!(CWSecuritySend(gWTPs[WTPIndex].session,
					    messages[i].msg,
					    messages[i].offset)))
#endif
			{
				CWFreeMessageFragments(messages, messagesCount);
				CW_FREE_OBJECT(messages);
				return CW_FALSE;
			}
		}
		CWFreeMessageFragments(messages, messagesCount);
		CW_FREE_OBJECT(messages);
		CWLog("Message Sent");
	}
	gWTPs[WTPIndex].currentState = CW_ENTER_RUN;
	gWTPs[WTPIndex].subState = CW_WAITING_REQUEST;

	return CW_TRUE;
}

CWBool CWACParseGenericRunMessage(int WTPIndex,
				  CWProtocolMessage *msg,
				  CWControlHeaderValues* controlVal) {

	if(msg == NULL || controlVal == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(!(CWParseControlHeader(msg, controlVal)))
		/* will be handled by the caller */
		return CW_FALSE;

	/* skip timestamp */
	controlVal->msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;

	/* Check if it is a request */
	if(controlVal->messageTypeValue % 2 == 1){
		/* check duplicate seq num */
		int	SeqIndex = controlVal->seqNum % SEQUENCE_ARRAY_SIZE;
		if(gWTPs[WTPIndex].RcvSeqNum[SeqIndex] == controlVal->seqNum){
			CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Duplicate Request Recvied");
			return CW_FALSE;
		}
		else{
			gWTPs[WTPIndex].RcvSeqNum[SeqIndex] = controlVal->seqNum;
		return CW_TRUE;	
		}
	}

#ifdef RTK_SMART_ROAMING
	CWThreadMutexLock(&gWTPs[WTPIndex].PendingReqLock);
	int pendingIndex;
	pendingIndex = CWACFindPendingIndex(gWTPs[WTPIndex].PendingReq,
							controlVal->messageTypeValue,
							controlVal->seqNum);
	if(pendingIndex != -1) {
		CWACResetPendingRequest(&gWTPs[WTPIndex].PendingReq[pendingIndex]);
		CWDebugLog("Recieve WTP[%d] pendingReq: %d  seqNum: %d\n", WTPIndex, pendingIndex, controlVal->seqNum);
	}
	CWThreadMutexUnlock(&gWTPs[WTPIndex].PendingReqLock);

	return CW_TRUE;
#endif

	if((gWTPs[WTPIndex].responseSeqNum != controlVal->seqNum) ||
	   (gWTPs[WTPIndex].responseType != controlVal->messageTypeValue)) {

		CWDebugLog("gWTPs: %d\n", gWTPs[WTPIndex].responseSeqNum);
		CWDebugLog("controlVal: %d\n", controlVal->seqNum);
		CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Seq Num or Msg Type not valid!");
		return CW_FALSE;
	}

	return CW_TRUE;	
}

/*Update 2009:
	Added vendValues to include a response payload (to pass response data)*/
CWBool CWParseConfigurationUpdateResponseMessage(CWProtocolMessage* msgPtr,
						 int len,
						 CWProtocolConfigurationUpdateResponseValues *pVals) 
{

	int offsetTillMessages;
	CWBool haveResultCode = CW_FALSE;
	
	offsetTillMessages = msgPtr->offset;
	
	CWLog("Parsing Configuration Update Response...");

	/* parse message elements */
	while((msgPtr->offset - offsetTillMessages) < len) {

		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				pVals->resultCode=CWProtocolRetrieve32(msgPtr);
				haveResultCode = CW_TRUE;
				pVals->vendValues = NULL;
//printf("Parse Configuration Update Response, Got Result Code = %d\n", pVals->resultCode);
				break;	

			/*Update 2009:
				Added case to implement conf update response with payload*/
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE_WITH_PAYLOAD:
			{
				int payloadSize = 0;
				CW_CREATE_OBJECT_ERR(pVals->vendValues, CWProtocolVendorSpecificValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

				pVals->resultCode=CWProtocolRetrieve32(msgPtr);
				haveResultCode = CW_TRUE;
				if (CWProtocolRetrieve16(msgPtr) != CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE)
					/*For now, we only have UCI payloads, so we will accept only vendor payloads for protocol data*/
						return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");

				pVals->vendValues->vendorPayloadType = CWProtocolRetrieve16(msgPtr);

				switch (pVals->vendValues->vendorPayloadType) {
					case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI:
						payloadSize = CWProtocolRetrieve32(msgPtr);
						if (payloadSize != 0) {
							pVals->vendValues->payload = (void *) CWProtocolRetrieveStr(msgPtr, payloadSize);
						} else 
							pVals->vendValues->payload = NULL;
						break;
					case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_WUM:
						payloadSize = CWProtocolRetrieve32(msgPtr);
						
						if (payloadSize <= 0) {
							/* Payload can't be zero here,
							 * at least the message type must be specified */
							return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
						} 
						pVals->vendValues->payload = (void *) CWProtocolRetrieveRawBytes(msgPtr, payloadSize);
						break;
					default:
						return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
						break;	
				}
				break;
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

	CWLog("Configuration Update Response Parsed");

	return CW_TRUE;	
}

#ifdef KM_BY_AC
CWBool CWParseWLANConfigurationResponseMessage(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode)
{
	int offsetTillMessages;

	if(msgPtr == NULL || resultCode==NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	offsetTillMessages = msgPtr->offset;
	
	CWLog("Parsing WLAN Configuration Response...");

	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				*resultCode = CWProtocolRetrieve32(msgPtr);
				break;
			case CW_MSG_ELEMENT_IEEE80211_ASSINED_WTP_BSSID_CW_TYPE:
			{
				int radio_id, wlan_idx;
				unsigned char *mac_addr;
				radio_id = CWProtocolRetrieve8(msgPtr);
				wlan_idx = CWProtocolRetrieve8(msgPtr);
				mac_addr = CWProtocolPointRawBytes(msgPtr, 6);
				break;
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration WLAN Response");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

	CWLog("WLAN Configuration Response Parsed");

	return CW_TRUE;	
}
	
CWBool CWParseStationConfigurationResponseMessage(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode)
{
	int offsetTillMessages;

	if(msgPtr == NULL || resultCode==NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	offsetTillMessages = msgPtr->offset;
	
	CWLog("Parsing Station Configuration Response...");

	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				*resultCode=CWProtocolRetrieve32(msgPtr);
				break;	
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Station Configuration Response");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

	CWLog("Station Configuration Response Parsed");

	return CW_TRUE;	
}

#endif
CWBool CWParseClearConfigurationResponseMessage(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode)
{
	int offsetTillMessages;

	if(msgPtr == NULL || resultCode==NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	offsetTillMessages = msgPtr->offset;
	
	CWLog("Parsing Clear Configuration Response...");

	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				*resultCode=CWProtocolRetrieve32(msgPtr);
				break;	
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

	CWLog("Clear Configuration Response Parsed");

	return CW_TRUE;	
}	



void CWSaveConfigurationUpdateResponseMessage(int WTPIndex,	CWProtocolConfigurationUpdateResponseValues* updateResp)

{
//printf("gWTPs[WTPIndex].imageActionState = %d\n", gWTPs[WTPIndex].imageActionState);
	if (gWTPs[WTPIndex].imageActionState == WAITNG_FOR_CONFIGURATION_UPDATE_RESPONSE_FROM_WTP) {		
		//1. image update (have sent Configuration Update Request for image update.)
		char stringToWum[64];

		switch (updateResp->resultCode)
		{
			case CW_PROTOCOL_SUCCESS:
				gWTPs[WTPIndex].imageActionState = WATING_FOR_IMAGE_DATA_REQUEST_FROM_WTP;
				sprintf(stringToWum, "Successfully Transfered Configuration Update Request to the WTP. WTP will ask for new image.");
				break;
			case CW_PROTOCOL_IMG_FAILURE_IMAGE_ALREADY_PRESENT:
			{
				int seqNum = CWGetSeqNum();
				if (!CWAssembleResetRequest(&(gWTPs[WTPIndex].messages),
											&(gWTPs[WTPIndex].messagesCount),
											gWTPs[WTPIndex].pathMTU,
											seqNum, gWTPs[WTPIndex].reqImageIdentifier)) {
					goto imageActionError;
				}
				  
				if(!CWACSendAcknowledgedPacket(WTPIndex, CW_MSG_TYPE_VALUE_RESET_RESPONSE, seqNum)) {							
					CWACStopRetransmission(WTPIndex);
					goto imageActionError;
				}
				
				gWTPs[WTPIndex].imageActionState = WATING_FOR_RESET_RESPONSE_FROM_WTP;
				sprintf(stringToWum, "Successfully Transfered Configuration Update Request to the WTP. WTP already has the image. Sending Reset.");
				break;
			}
			default:
imageActionError:	
				gWTPs[WTPIndex].imageActionState = NO_ACTION; // back to NO_ACTION when getting things we don't know...
				gWTPs[WTPIndex].reqImageIdentifier[0] = '\0';
				sprintf(stringToWum, "Failed. Cancel the action of changing firmware. resultCode = %d", updateResp->resultCode);
				break;
		}
//printf("%s\n", stringToWum);
		CWLog(stringToWum);
		CWResponseToWum(WTPIndex, updateResp->resultCode, strlen(stringToWum)+1, stringToWum);
	} else if (updateResp->vendValues != NULL) {				
		// 2. non-rtk vendor values
		char *wumPayloadBytes = NULL;
		int closeWTPManager = CW_FALSE;
		char * responseBuffer; 
		int socketIndex, payloadSize, headerSize, netWTPIndex, netresultCode, netpayloadSize;


		/********************************
		 *Payload Management		*
		 ********************************/

		headerSize = 3*sizeof(int);
		
		switch (updateResp->vendValues->vendorPayloadType) {
		case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI:
			if (updateResp->vendValues->payload != NULL)
				payloadSize = strlen((char *) updateResp->vendValues->payload);
			else
				payloadSize = 0;
			break;
		case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_WUM:
			wumPayloadBytes = updateResp->vendValues->payload;
			payloadSize = 1;
			
			/*
			 * When dealing with WUM responses, the dafault size
			 * is 1 bytes, which is used for the type.
			 *
			 * The only response message with a bigger payload is the
			 * WTP_VERSION_RESPONSE (4 bytes), as it carries the WTP version
			 * together with the response type.
			 */
			if (wumPayloadBytes[0] == WTP_VERSION_RESPONSE)
				payloadSize = 4;

			/*
			 * If we received a positive WTP_COMMIT_ACK, we need to terminate
			 * the WTP Manager Thread.
			 */
			if (wumPayloadBytes[0] == WTP_COMMIT_ACK && updateResp->resultCode == CW_PROTOCOL_SUCCESS)
				closeWTPManager = CW_TRUE;
			break;
		}

		if ( ( responseBuffer = malloc( headerSize+payloadSize ) ) != NULL ) {

			netWTPIndex = htonl(WTPIndex);
			memcpy(responseBuffer, &netWTPIndex, sizeof(int));

			netresultCode = htonl(updateResp->resultCode);
			memcpy(responseBuffer+sizeof(int), &netresultCode, sizeof(int));

			netpayloadSize = htonl(payloadSize);
			memcpy(responseBuffer+(2*sizeof(int)), &netpayloadSize, sizeof(int));
			
			if (payloadSize > 0) {
				memcpy(responseBuffer+headerSize, (char *) updateResp->vendValues->payload, payloadSize);
				if (updateResp->vendValues->vendorPayloadType == CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI) 
					((char *)updateResp->vendValues->payload)[payloadSize] = '\0';
			}

			CWThreadMutexLock(&(gWTPs[WTPIndex].interfaceMutex));
			socketIndex = gWTPs[WTPIndex].applicationIndex; 
			gWTPs[WTPIndex].applicationIndex = -1;
			CWThreadMutexUnlock(&(gWTPs[WTPIndex].interfaceMutex));

			/****************************************************
		         * Forward payload to correct application  	    *
			 ****************************************************/
			if(!CWErr(CWThreadMutexLock(&appsManager.socketMutex[socketIndex]))) {
				CWLog("Error locking numSocketFree Mutex");
			} else {
				if ( Writen(appsManager.appSocket[socketIndex], responseBuffer, headerSize+payloadSize)  < 0 ) {
					CWLog("Error locking numSocketFree Mutex");
				}
				CWThreadMutexUnlock(&appsManager.socketMutex[socketIndex]);
			}
	
		}
		CW_FREE_OBJECT(responseBuffer);
		CW_FREE_OBJECT(updateResp->vendValues->payload);
		CW_FREE_OBJECT(updateResp->vendValues);
		
		/*
		 * On a positive WTP_COMMIT_ACK, we need to close the WTP Manager.
		 */
		if (closeWTPManager) {
			gWTPs[WTPIndex].isRequestClose = CW_TRUE;
			CWSignalThreadCondition(&gWTPs[WTPIndex].interfaceWait);
			
						/* TODO Babylon: 
						 *		I think this can be removed, since in the same thread, 
						 *		signal is not necessary but no harm.
			 */
		}
	} else {
		char stringToWum[128];
		stringToWum[0] = '\0';
		// 3. qos
		if (gWTPs[WTPIndex].qosValues!=NULL) {
			int i;		
			bindingValues* aux=(bindingValues*)gWTPs[WTPIndex].WTPProtocolManager.bindingValuesPtr;
			strcat(stringToWum, "[Qos]");
			if (updateResp->resultCode==CW_PROTOCOL_SUCCESS) 
			{
				for(i=0; i<NUM_QOS_PROFILES; i++) {
					aux->qosValues[i].cwMin=gWTPs[WTPIndex].qosValues[i].cwMin;
					aux->qosValues[i].cwMax=gWTPs[WTPIndex].qosValues[i].cwMax;
					aux->qosValues[i].AIFS=gWTPs[WTPIndex].qosValues[i].AIFS;
				}
				strcat(stringToWum, "updated. ");
			} else {
				strcat(stringToWum, "can't update.");
			}
			gWTPs[WTPIndex].qosValues = NULL;
		}

#ifdef RTK_CAPWAP
		if (gWTPs[WTPIndex].pRtkPowerScale != NULL) {		
			// 4. rtk power scale
			strcat(stringToWum, "[Power]");
			if (updateResp->resultCode==CW_PROTOCOL_SUCCESS && 
				CWAcDbSetPowerScale(gWTPs[WTPIndex].wtpConfig.name,gWTPs[WTPIndex].pRtkPowerScale->radio_id, 
									gWTPs[WTPIndex].pRtkPowerScale->power_scale)) {
				strcat(stringToWum, "updated. ");
			} else {			
				CWLog("!!Error! Can't save power scale!! wtp: %d, radio: %d, power_scale: %d\n", 
					WTPIndex, gWTPs[WTPIndex].pRtkPowerScale->radio_id, gWTPs[WTPIndex].pRtkPowerScale->power_scale);
				strcat(stringToWum, "can't update.");
			}
			CW_FREE_OBJECT(gWTPs[WTPIndex].pRtkPowerScale);
		}

		if (gWTPs[WTPIndex].pRtkChannel != NULL) {		
			// 5. rtk channel			
			strcat(stringToWum, "[Channel]");			
			if (updateResp->resultCode==CW_PROTOCOL_SUCCESS && 
				CWAcDbSetSetChannel(gWTPs[WTPIndex].wtpConfig.name,gWTPs[WTPIndex].pRtkChannel->radio_id, 
									gWTPs[WTPIndex].pRtkChannel->channel)) {
				strcat(stringToWum, "updated. ");
			} else {			
				CWLog("!!Error! Can't save channel!! wtp: %d, radio: %d, channel: %d", 
					WTPIndex, gWTPs[WTPIndex].pRtkChannel->radio_id, gWTPs[WTPIndex].pRtkChannel->channel);
				strcat(stringToWum, "can't update.");
			}
			CW_FREE_OBJECT(gWTPs[WTPIndex].pRtkChannel);
		}
#endif
		strcat(stringToWum, "\n");
		CWLog(stringToWum);
		CWResponseToWum(WTPIndex, updateResp->resultCode, strlen(stringToWum)+1, stringToWum);
	}
	CWDebugLog("Configuration Update Response Saved");

}

CWBool CWParseWTPDataTransferRequestMessage(CWProtocolMessage *msgPtr, int len, CWProtocolWTPDataTransferRequestValues *valuesPtr)
{
	int offsetTillMessages;
	

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	

	offsetTillMessages = msgPtr->offset;
	
	CWLog("");
	CWLog("#________ WTP Data Transfer (Run) ________#");
	CWLog("Parsing WTP Data Transfer Request...");
	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_DATA_TRANSFER_DATA_CW_TYPE:{	
				if (!(CWParseMsgElemDataTransferData(msgPtr, elemLen, valuesPtr)))
				{
					if(valuesPtr->debug_info!=NULL)
					{
						//printf("%s:%d free valuesPtr->debug_info!!!!\n",__FUNCTION__,__LINE__);
						CW_FREE_OBJECT(valuesPtr->debug_info);
					}
					
					return CW_FALSE;
				}
				CWDebugLog("----- %s --------\n",valuesPtr->debug_info);
				if(valuesPtr->debug_info!=NULL)
				{
					//printf("%s:%d free valuesPtr->debug_info!!!!\n",__FUNCTION__,__LINE__);
					CW_FREE_OBJECT(valuesPtr->debug_info);
				}
				break;	
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in WTP Data Transfer Request");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");


	return CW_TRUE;	
}
CWBool CWAssembleWTPDataTransferResponse (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum) 
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount=0;
	CWProtocolMessage *msgElemsBinding= NULL;
	int msgElemBindingCount=0;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling WTP Data Transfer Response...");
		
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			               CW_MSG_TYPE_VALUE_DATA_TRANSFER_RESPONSE,
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
	
	CWLog("WTP Data Transfer Response Assembled");
	
	return CW_TRUE;
}

#ifdef RTK_SMART_ROAMING
CWBool CWIssueSilenceNotify(int WTP_Index, uint8_t radio_id, uint8_t* mac_addr) {


    unsigned char idx;
    CWThreadMutexLock(&(gWTPs[WTP_Index].interfaceMutex));

    if(((gWTPs[WTP_Index].notifyCommandHead + 1)%AC_MAX_NOTIFY_NUM)  == gWTPs[WTP_Index].notifyCommandTail) {
        /*notifyCommand queue is full*/    

        CWRoamLog("!!!Notify WTP%d wlan%d of slilence station %02x:%02x:%02x:%02x:%02x:%02x Fail!! NotifyCommand Queue Full!! head:%d, tail:%d",
					WTP_Index, radio_id, mac_addr[0], mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5],
					gWTPs[WTP_Index].notifyCommandHead, gWTPs[WTP_Index].notifyCommandTail);
        
        CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));
        return CW_FALSE;
    }

    idx = gWTPs[WTP_Index].notifyCommandHead;

    
    gWTPs[WTP_Index].notifyCommand[idx] = SILENT_NOTIFY;
    gWTPs[WTP_Index].RoamingRequest[idx].silent_sta.radio_id = radio_id;
    memcpy(gWTPs[WTP_Index].RoamingRequest[idx].silent_sta.mac_addr, mac_addr, 6);
    gWTPs[WTP_Index].notifyCommandHead = (gWTPs[WTP_Index].notifyCommandHead+1)%AC_MAX_NOTIFY_NUM;

    CWSignalThreadCondition(&gWTPs[WTP_Index].interfaceWait);
    CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));

    return CW_TRUE;
}

CWBool CWIssueUnsilenceNotify(int WTP_Index, uint8_t radio_id, uint8_t* mac_addr) {


    unsigned char idx;
    CWThreadMutexLock(&(gWTPs[WTP_Index].interfaceMutex));

    if(((gWTPs[WTP_Index].notifyCommandHead + 1)%AC_MAX_NOTIFY_NUM)  == gWTPs[WTP_Index].notifyCommandTail) {
        /*notifyCommand queue is full*/    

        CWRoamLog("!!!Notify WTP%d wlan%d of un-slilence station %02x:%02x:%02x:%02x:%02x:%02x Fail!! NotifyCommand Queue Full!! head:%d, tail:%d",
					WTP_Index, radio_id, mac_addr[0], mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5],
					gWTPs[WTP_Index].notifyCommandHead, gWTPs[WTP_Index].notifyCommandTail);
        
        CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));
        return CW_FALSE;
    }

    idx = gWTPs[WTP_Index].notifyCommandHead;

    
    gWTPs[WTP_Index].notifyCommand[idx] = UNSILENT_NOTIFY;
    gWTPs[WTP_Index].RoamingRequest[idx].unsilent_sta.radio_id = radio_id;
    memcpy(gWTPs[WTP_Index].RoamingRequest[idx].unsilent_sta.mac_addr, mac_addr, 6);
    gWTPs[WTP_Index].notifyCommandHead = (gWTPs[WTP_Index].notifyCommandHead+1)%AC_MAX_NOTIFY_NUM;

    CWSignalThreadCondition(&gWTPs[WTP_Index].interfaceWait);
    CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));

    return CW_TRUE;
}

CWBool CWIssueDot11vNotify(int WTP_Index, uint8_t radio_id, uint8_t* mac_addr) {
    unsigned char idx;
    CWThreadMutexLock(&(gWTPs[WTP_Index].interfaceMutex));

    if(((gWTPs[WTP_Index].notifyCommandHead + 1)%AC_MAX_NOTIFY_NUM)  == gWTPs[WTP_Index].notifyCommandTail) {
        /*notifyCommand queue is full*/    

        CWRoamLog("!!!Notify WTP%d wlan%d of dot11v station %02x:%02x:%02x:%02x:%02x:%02x Fail!! NotifyCommand Queue Full!! head:%d, tail:%d",
					WTP_Index, radio_id, mac_addr[0], mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5],
					gWTPs[WTP_Index].notifyCommandHead, gWTPs[WTP_Index].notifyCommandTail);        
        CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));
        return CW_FALSE;
    }

    idx = gWTPs[WTP_Index].notifyCommandHead;

    gWTPs[WTP_Index].notifyCommand[idx] = DOT11V_NOTIFY;
    gWTPs[WTP_Index].RoamingRequest[idx].dot11v_sta.radio_id = radio_id;
    memcpy(gWTPs[WTP_Index].RoamingRequest[idx].dot11v_sta.mac_addr, mac_addr, 6);
    gWTPs[WTP_Index].notifyCommandHead = (gWTPs[WTP_Index].notifyCommandHead+1)%AC_MAX_NOTIFY_NUM;
    
    CWSignalThreadCondition(&gWTPs[WTP_Index].interfaceWait);
    CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));
    
    return CW_TRUE;
}

CWBool CWIssueFreeNotify(int WTP_Index, uint8_t radio_id, uint8_t* mac_addr) {
    unsigned char idx;
    CWThreadMutexLock(&(gWTPs[WTP_Index].interfaceMutex));

    if(((gWTPs[WTP_Index].notifyCommandHead + 1)%AC_MAX_NOTIFY_NUM)  == gWTPs[WTP_Index].notifyCommandTail) {
        /*notifyCommand queue is full*/    

        CWRoamLog("!!!Notify WTP%d wlan%d of free station %02x:%02x:%02x:%02x:%02x:%02x Fail!! NotifyCommand Queue Full!! head: %d, tail:%d",
					WTP_Index, radio_id, mac_addr[0], mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5],
					gWTPs[WTP_Index].notifyCommandHead, gWTPs[WTP_Index].notifyCommandTail);   
        
        CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));
        return CW_FALSE;
    }
    idx = gWTPs[WTP_Index].notifyCommandHead;

    
    gWTPs[WTP_Index].notifyCommand[idx] = FREE_NOTIFY;
    gWTPs[WTP_Index].RoamingRequest[idx].free_sta.radio_id = radio_id;
    memcpy(gWTPs[WTP_Index].RoamingRequest[idx].free_sta.mac_addr, mac_addr, 6);    
    gWTPs[WTP_Index].notifyCommandHead = (gWTPs[WTP_Index].notifyCommandHead+1)%AC_MAX_NOTIFY_NUM;
    
    CWSignalThreadCondition(&gWTPs[WTP_Index].interfaceWait);
    CWThreadMutexUnlock(&gWTPs[WTP_Index].interfaceMutex);
    return CW_TRUE;
}

CWBool CWIssueDualBandNotify(int WTP_Index, uint8_t* mac_addr) {
    unsigned char idx;
    CWThreadMutexLock(&(gWTPs[WTP_Index].interfaceMutex));

    if(((gWTPs[WTP_Index].notifyCommandHead + 1)%AC_MAX_NOTIFY_NUM)  == gWTPs[WTP_Index].notifyCommandTail) {
        /*notifyCommand queue is full*/    
        CWRoamLog("!!!Notify WTP%d wlan0 of dual-band station %02x:%02x:%02x:%02x:%02x:%02x Fail!! NotifyCommand Queue Full!! head:%d tail:%d",
					WTP_Index, mac_addr[0], mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5],
					gWTPs[WTP_Index].notifyCommandHead, gWTPs[WTP_Index].notifyCommandTail);        
        CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));
        return CW_FALSE;
    }

    idx = gWTPs[WTP_Index].notifyCommandHead;

    
    gWTPs[WTP_Index].notifyCommand[idx] = DUALBAND_NOTIFY;
    gWTPs[WTP_Index].RoamingRequest[idx].dual_sta.radio_id = 0;	/* wlan0 -5GHz */
    memcpy(gWTPs[WTP_Index].RoamingRequest[idx].dual_sta.mac_addr, mac_addr, 6);    
    gWTPs[WTP_Index].notifyCommandHead = (gWTPs[WTP_Index].notifyCommandHead+1)%AC_MAX_NOTIFY_NUM;
    
    CWSignalThreadCondition(&gWTPs[WTP_Index].interfaceWait);
    CWThreadMutexUnlock(&(gWTPs[WTP_Index].interfaceMutex));

    return CW_TRUE;
}

CWBool CWParseMsgElemUpdateStationValues(CWProtocolMessage *msgPtr,int len, CWProtocolUpdateStationValues *valPtr) 
{
	CWParseMessageElementStart();

	CWProtocolCopyRawBytes(valPtr->mac_addr, msgPtr, 6);
	valPtr->rssi = CWProtocolRetrieve8(msgPtr);

	CWParseMessageElementEnd();
	
	return CW_TRUE;
}

CWBool CWSaveMsgElemUpdateStationValues (int WTP_Index, CWProtocolUpdateStationValues *upd_station)
{
	if(upd_station == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	return CWStationUpdate(WTP_Index, upd_station);
}

CWBool CWParseMsgElemAddStationValues(CWProtocolMessage *msgPtr, int len, CWProtocolAddStationValues *valPtr) 
{
	CWParseMessageElementStart();
	
	CWProtocolCopyRawBytes(valPtr->mac_addr, msgPtr, 6);
	valPtr->rssi = CWProtocolRetrieve8(msgPtr);
	valPtr->data_rate = CWProtocolRetrieve8(msgPtr);	
	valPtr->link_time = CWProtocolRetrieve32(msgPtr); 

	CWParseMessageElementEnd();
		
	return CW_TRUE;
}


CWBool CWSaveMsgElemAddStationValues(int WTP_Index, CWProtocolAddStationValues *add_station)
{
	if(add_station == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	return CWStationAdd(WTP_Index, add_station);
}

CWBool CWParseMsgElemUpdateStationDot11kValues(unsigned char radio_id, CWProtocolMessage *msgPtr, int len, Dot11kValues *valPtr) 
{
	CWParseMessageElementStart();

	int i;
	unsigned char wlan_mac[6];

	CWProtocolCopyRawBytes(wlan_mac, msgPtr, 6);	
	for(i=0; i<gMaxWTPs; i++){	
		if(!radio_id && !memcmp(gWTPsWlan[i].wlan0,wlan_mac,6))
		{
			valPtr->wtp_index = i;
			break;
		}
		else if(radio_id && !memcmp(gWTPsWlan[i].wlan1,wlan_mac,6))
		{
			valPtr->wtp_index = i;
			break;
		}
		else		//cannot match and find mac_addr from db
			valPtr->wtp_index = AC_MAX_WTP_NUM;
	}

	valPtr->ch_util = CWProtocolRetrieve8(msgPtr);
	valPtr->rcpi = CWProtocolRetrieve8(msgPtr);

	CWParseMessageElementEnd();
	
	return CW_TRUE;
}

CWBool CWSaveMsgElemUpdateStationDot11kValues(int wlanNum, CWProtocolUpdateStationDot11kValues *upd_station_11k)
{
	if(upd_station_11k == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	return CWStationUpdateDot11k(wlanNum, upd_station_11k);
}

CWBool CWParseMsgElemNewStationValues(CWProtocolMessage *msgPtr, int len, CWProtocolAddStationValues *valPtr) 
{
	CWParseMessageElementStart();

	CWProtocolCopyRawBytes(valPtr->mac_addr, msgPtr, 6);
	valPtr->rssi = CWProtocolRetrieve8(msgPtr);
	valPtr->data_rate = -1;
	valPtr->link_time = 0;
	
	CWParseMessageElementEnd();

	return CW_TRUE;
}


CWBool CWSaveMsgElemNewStationNotifyValues(int WTP_Index, unsigned char radio_id, unsigned char *station_mac)
{
	if(station_mac == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	return CWStationNewNotify(WTP_Index, radio_id, station_mac);
}


CWBool CWParseWTPEventRequestMessage(int WTP_Index, CWProtocolMessage *msgPtr,
				     int len,
				     CWProtocolWTPEventRequestValues *valuesPtr) {

	int offsetTillMessages;
	int i=0, k=0, j;
	int offset;

	unsigned char radio_id, ch_num, ch_util, sta_mac[6], wlan_num;	
	unsigned int sta_num;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	/*
	CW_CREATE_OBJECT_ERR(valuesPtr, CWProtocolWTPEventRequestValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	*/
	offsetTillMessages = msgPtr->offset;
	
	CWLog("");
	CWLog("#________ WTP Event (Run) ________#");
	CWLog("Parsing WTP Event Request...");

#if 0
	valuesPtr->errorReportCount = 0;
	valuesPtr->errorReport = NULL;
	valuesPtr->duplicateIPv4 = NULL;
	valuesPtr->duplicateIPv6 = NULL;
	valuesPtr->WTPOperationalStatisticsCount = 0;
	valuesPtr->WTPOperationalStatistics = NULL;
	valuesPtr->WTPRadioStatisticsCount = 0;
	valuesPtr->WTPRadioStatistics = NULL;
	valuesPtr->WTPRebootStatistics = NULL;
	valuesPtr->WTPDeleteStationNotify = NULL;
#endif
	valuesPtr->WTPUpdateStationNotify = NULL;
	valuesPtr->WTPAddStationNotify = NULL;
	valuesPtr->WTPUpdateStationDot11kNotify = NULL;

	/* parse message elements */
	while((msgPtr->offset - offsetTillMessages) < len) {

		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);

		switch(elemType) {
#if 0
			case CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->errorReport, 
						     CWDecryptErrorReportValues,
						     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

				if (!(CWParseMsgElemDecryptErrorReport(msgPtr, elemLen, valuesPtr->errorReport)))
					return CW_FALSE;
				break;	
			case CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->duplicateIPv4,
						     WTPDuplicateIPv4,
						     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
				
				CW_CREATE_ARRAY_ERR((valuesPtr->duplicateIPv4)->MACoffendingDevice_forIpv4,
						    6,
						    unsigned char,
						    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

				if (!(CWParseMsgElemDuplicateIPv4Address(msgPtr, elemLen, valuesPtr->duplicateIPv4)))
					return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->duplicateIPv6,
						     WTPDuplicateIPv6,
						     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

				CW_CREATE_ARRAY_ERR((valuesPtr->duplicateIPv6)->MACoffendingDevice_forIpv6,
						    6,
						    unsigned char,
						    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

				if (!(CWParseMsgElemDuplicateIPv6Address(msgPtr, elemLen, valuesPtr->duplicateIPv6)))
					return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE:
				valuesPtr->WTPOperationalStatisticsCount++;
				msgPtr->offset += elemLen;
				break;
			case CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE:
				valuesPtr->WTPRadioStatisticsCount++;
				msgPtr->offset += elemLen;
				break;
			case CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->WTPRebootStatistics,
						     WTPRebootStatisticsInfo,
						     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

				if (!(CWParseMsgElemWTPRebootStatistics(msgPtr, elemLen, valuesPtr->WTPRebootStatistics)))
					return CW_FALSE;	
				break;
			case CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE:
				CW_CREATE_OBJECT_ERR(valuesPtr->WTPDeleteStationNotify,
						     CWProtocolDeleteStationValues,
						     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				if (!(CWParseMsgElemDeleteStation(msgPtr, elemLen, valuesPtr->WTPDeleteStationNotify)))
					return CW_FALSE;	

				break;
#endif
			case CW_MSG_ELEMENT_ADD_STATION_CW_TYPE:
			{	
				gWTPsSpeed[WTP_Index] = CWProtocolRetrieve32(msgPtr);
				radio_id = CWProtocolRetrieve8(msgPtr);
				ch_num = CWProtocolRetrieve8(msgPtr);
				ch_util = CWProtocolRetrieve8(msgPtr);
				
				CWRoamDebugLog("");
				if(radio_id==0)
					CWRoamDebugLog("WTP%d wlan%d(%02X:%02X) (ch_num=%d ch_util=%d speed=%d)", WTP_Index,radio_id, gWTPsWlan[WTP_Index].wlan0[4], gWTPsWlan[WTP_Index].wlan0[5], ch_num,ch_util,gWTPsSpeed[WTP_Index]);
				else if(radio_id==1)
					CWRoamDebugLog("WTP%d wlan%d(%02X:%02X) (ch_num=%d ch_util=%d speed=%d)", WTP_Index,radio_id, gWTPsWlan[WTP_Index].wlan1[4], gWTPsWlan[WTP_Index].wlan1[5], ch_num,ch_util,gWTPsSpeed[WTP_Index]);

				CW_CREATE_OBJECT_ERR(valuesPtr->WTPUpdateStationNotify,
						 CWProtocolUpdateStationValues,
						 return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				valuesPtr->WTPUpdateStationNotify->radio_id = radio_id;
				valuesPtr->WTPUpdateStationNotify->ch_util = ch_util;
				sta_num = CWProtocolRetrieve32(msgPtr);
				for(j=0; j<sta_num; j++)
				{
					if (!(CWParseMsgElemUpdateStationValues(msgPtr, UPD_STA_ELEM_LEN, valuesPtr->WTPUpdateStationNotify))){
						CW_FREE_OBJECT(valuesPtr->WTPUpdateStationNotify);
						return CW_FALSE;
					}
					if(!(CWSaveMsgElemUpdateStationValues(WTP_Index, valuesPtr->WTPUpdateStationNotify))){
						CW_FREE_OBJECT(valuesPtr->WTPUpdateStationNotify);
						return CW_FALSE;
					}
				}
				CW_FREE_OBJECT(valuesPtr->WTPUpdateStationNotify);

				CW_CREATE_OBJECT_ERR(valuesPtr->WTPAddStationNotify,
						 CWProtocolAddStationValues,
						 return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				valuesPtr->WTPAddStationNotify->radio_id = radio_id;
				valuesPtr->WTPAddStationNotify->ch_util = ch_util;
				sta_num = CWProtocolRetrieve32(msgPtr);
				for(j=0; j<sta_num; j++)
				{
					if (!(CWParseMsgElemAddStationValues(msgPtr, ADD_STA_ELEM_LEN, valuesPtr->WTPAddStationNotify))){
						CW_FREE_OBJECT(valuesPtr->WTPAddStationNotify);
						return CW_FALSE;
					}
					if(!(CWSaveMsgElemAddStationValues(WTP_Index, valuesPtr->WTPAddStationNotify))){
						CW_FREE_OBJECT(valuesPtr->WTPAddStationNotify);
						return CW_FALSE;
					}
				}
				CW_FREE_OBJECT(valuesPtr->WTPAddStationNotify);

				CW_CREATE_OBJECT_ERR(valuesPtr->WTPUpdateStationDot11kNotify,
						 CWProtocolUpdateStationDot11kValues,
						 return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				valuesPtr->WTPUpdateStationDot11kNotify->radio_id = radio_id;
				while(msgPtr->offset < elemLen)
				{
					valuesPtr->WTPUpdateStationDot11kNotify->flag = CWProtocolRetrieve8(msgPtr);
					CWProtocolCopyRawBytes(valuesPtr->WTPUpdateStationDot11kNotify->mac_addr, msgPtr, 6);
					wlan_num = CWProtocolRetrieve8(msgPtr);
					for(j=0; j<wlan_num && j<STA_MAX_11K_NUM; j++)
					{
						if (!(CWParseMsgElemUpdateStationDot11kValues(radio_id, msgPtr, UPD_STA_DOT11K_ELEM_LEN, &valuesPtr->WTPUpdateStationDot11kNotify->Dot11k[j]))){
							CW_FREE_OBJECT(valuesPtr->WTPUpdateStationDot11kNotify);
							return CW_FALSE;	
						}
					}						
					if(!(CWSaveMsgElemUpdateStationDot11kValues(wlan_num, valuesPtr->WTPUpdateStationDot11kNotify))){
						CW_FREE_OBJECT(valuesPtr->WTPUpdateStationDot11kNotify);
						return CW_FALSE;
					}
				}				
				CW_FREE_OBJECT(valuesPtr->WTPUpdateStationDot11kNotify);

				break;
			}
			case CW_MSG_ELEMENT_NEW_STATION_CW_TYPE:
			{
				msgPtr->offset += sizeof(unsigned int);		/* ping_speed */
				radio_id = CWProtocolRetrieve8(msgPtr);
				msgPtr->offset += sizeof(unsigned char);	/* channel number */
				ch_util = CWProtocolRetrieve8(msgPtr);

				CW_CREATE_OBJECT_ERR(valuesPtr->WTPAddStationNotify,
						CWProtocolAddStationValues,
						return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				valuesPtr->WTPAddStationNotify->radio_id = radio_id;
				valuesPtr->WTPAddStationNotify->ch_util = ch_util;
				if (!(CWParseMsgElemNewStationValues(msgPtr, NEW_STA_ELEM_LEN, valuesPtr->WTPAddStationNotify))){
					CW_FREE_OBJECT(valuesPtr->WTPAddStationNotify);
					return CW_FALSE;  
                }

				CWRoamDebugLog("");
				if(radio_id==0) {
					CWRoamDebugLog("WTP%d wlan%d(%02X:%02X) New Station (%02x:%02x:%02x:%02x:%02x:%02x)",
			                    WTP_Index,radio_id,
			                    gWTPsWlan[WTP_Index].wlan0[4],gWTPsWlan[WTP_Index].wlan0[5],
			                    valuesPtr->WTPAddStationNotify->mac_addr[0],
			                    valuesPtr->WTPAddStationNotify->mac_addr[1],
			                    valuesPtr->WTPAddStationNotify->mac_addr[2],
								valuesPtr->WTPAddStationNotify->mac_addr[3],
								valuesPtr->WTPAddStationNotify->mac_addr[4],
								valuesPtr->WTPAddStationNotify->mac_addr[5]);
				}else if(radio_id==1){
					CWRoamDebugLog("WTP%d wlan%d(%02X:%02X) New Station (%02x:%02x:%02x:%02x:%02x:%02x)",
			                    WTP_Index,radio_id,
								gWTPsWlan[WTP_Index].wlan1[4],gWTPsWlan[WTP_Index].wlan1[5],
			                    valuesPtr->WTPAddStationNotify->mac_addr[0],
			                    valuesPtr->WTPAddStationNotify->mac_addr[1],
			                    valuesPtr->WTPAddStationNotify->mac_addr[2],
								valuesPtr->WTPAddStationNotify->mac_addr[3],
								valuesPtr->WTPAddStationNotify->mac_addr[4],
								valuesPtr->WTPAddStationNotify->mac_addr[5]);
				}

				/* NOT change following sequence, AddStation() should be after NewStation() */
				if(!CWSaveMsgElemNewStationNotifyValues(WTP_Index, radio_id, valuesPtr->WTPAddStationNotify->mac_addr)) {
                    CW_FREE_OBJECT(valuesPtr->WTPAddStationNotify);
					return CW_FALSE;
                }
				if(!(CWSaveMsgElemAddStationValues(WTP_Index, valuesPtr->WTPAddStationNotify))) {
					CW_FREE_OBJECT(valuesPtr->WTPAddStationNotify);
                    return CW_FALSE;
				}

                CW_FREE_OBJECT(valuesPtr->WTPAddStationNotify);
				break;
			}
			case CW_MSG_ELEMENT_STATION_FT_OVER_DS_CW_TYPE:
			{
				msgPtr->offset += sizeof(unsigned int);		/* ping_speed */
				radio_id = CWProtocolRetrieve8(msgPtr);
				msgPtr->offset += sizeof(unsigned char);	/* channel number */
				msgPtr->offset += sizeof(unsigned char);	/* channel util */ 
				CWProtocolCopyRawBytes(sta_mac, msgPtr, sizeof(sta_mac));
				msgPtr->offset += sizeof(unsigned char); 	/*rssi*/

				CWRoamDebugLog("");
				if(radio_id==0)
					CWRoamDebugLog("WTP%d wlan%d(%02X:%02X) FT_Over_DS (%02x:%02x:%02x:%02x:%02x:%02x)",WTP_Index,radio_id,gWTPsWlan[WTP_Index].wlan0[4],gWTPsWlan[WTP_Index].wlan0[5],sta_mac[0],sta_mac[1],sta_mac[2],sta_mac[3],sta_mac[4],sta_mac[5]);
				else if(radio_id==1)
					CWRoamDebugLog("WTP%d wlan%d(%02X:%02X) FT_Over_DS (%02x:%02x:%02x:%02x:%02x:%02x)",WTP_Index,radio_id,gWTPsWlan[WTP_Index].wlan1[4],gWTPsWlan[WTP_Index].wlan1[5],sta_mac[0],sta_mac[1],sta_mac[2],sta_mac[3],sta_mac[4],sta_mac[5]);
				
				break;
			}
			case CW_MSG_ELEMENT_STATION_FT_OVER_AIR_CW_TYPE:
			{
				msgPtr->offset += sizeof(unsigned int);		/* ping_speed */
				radio_id = CWProtocolRetrieve8(msgPtr);
				msgPtr->offset += sizeof(unsigned char);	/* channel number */
				msgPtr->offset += sizeof(unsigned char); 	/* channel util */
				CWProtocolCopyRawBytes(sta_mac, msgPtr, sizeof(sta_mac));
				msgPtr->offset += sizeof(unsigned char); 	/*rssi*/

				CWRoamDebugLog("");
				if(radio_id==0)
					CWRoamDebugLog("WTP%d wlan%d(%02X:%02X) FT_Over_Air (%02x:%02x:%02x:%02x:%02x:%02x)",WTP_Index,radio_id,gWTPsWlan[WTP_Index].wlan0[4],gWTPsWlan[WTP_Index].wlan0[5],sta_mac[0],sta_mac[1],sta_mac[2],sta_mac[3],sta_mac[4],sta_mac[5]);
				else if(radio_id==1)
					CWRoamDebugLog("WTP%d wlan%d(%02X:%02X) FT_Over_Air (%02x:%02x:%02x:%02x:%02x:%02x)",WTP_Index,radio_id,gWTPsWlan[WTP_Index].wlan0[4],gWTPsWlan[WTP_Index].wlan0[5],sta_mac[0],sta_mac[1],sta_mac[2],sta_mac[3],sta_mac[4],sta_mac[5]);

				break;
			}

			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in WTP Event Request");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) 
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

#if 0
	CW_CREATE_ARRAY_ERR(valuesPtr->WTPOperationalStatistics,
			    valuesPtr->WTPOperationalStatisticsCount,
			    WTPOperationalStatisticsValues,
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	CW_CREATE_ARRAY_ERR(valuesPtr->WTPRadioStatistics,
			    valuesPtr->WTPRadioStatisticsCount,
			    WTPRadioStatisticsValues,
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	

	msgPtr->offset = offsetTillMessages;

	while((msgPtr->offset - offsetTillMessages) < len) {
	
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE:
				if (!(CWParseMsgElemWTPOperationalStatistics(msgPtr,
								      elemLen,
								      &(valuesPtr->WTPOperationalStatistics[k++]))))
					return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE:
				if (!(CWParseMsgElemWTPRadioStatistics(msgPtr,
								elemLen,
								&(valuesPtr->WTPRadioStatistics[i++]))))
					return CW_FALSE;
				break;
			default:
				msgPtr->offset += elemLen;
				break;
		}
	}
#endif

	//CWLog("WTP Event Request Parsed");
	return CW_TRUE;	
}

#if 0
CWBool CWSaveWTPEventRequestMessage(CWProtocolWTPEventRequestValues *WTPEventRequest,
				    CWWTPProtocolManager *WTPProtocolManager) {

	if(WTPEventRequest == NULL || WTPProtocolManager == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(WTPEventRequest->WTPDeleteStationNotify) {	
		CWLog("Delete Station: %02x:%02x:%02x:%02x:%02x:%02x", 
			WTPEventRequest->WTPDeleteStationNotify->mac_addr[0],
			 WTPEventRequest->WTPDeleteStationNotify->mac_addr[1],
			  WTPEventRequest->WTPDeleteStationNotify->mac_addr[2],
			   WTPEventRequest->WTPDeleteStationNotify->mac_addr[3],
			    WTPEventRequest->WTPDeleteStationNotify->mac_addr[4],
			     WTPEventRequest->WTPDeleteStationNotify->mac_addr[5]);
#ifdef KM_BY_AC
		CWStationDelete(WTPEventRequest->WTPDeleteStationNotify->mac_addr);
#endif
		CW_FREE_OBJECT(WTPEventRequest->WTPDeleteStationNotify);
	}

	if(WTPEventRequest->WTPRebootStatistics) {	

		CW_FREE_OBJECT(WTPProtocolManager->WTPRebootStatistics);
		WTPProtocolManager->WTPRebootStatistics = WTPEventRequest->WTPRebootStatistics;
	}

	if((WTPEventRequest->WTPOperationalStatisticsCount) > 0) {

		int i,k;
		CWBool found = CW_FALSE;

		for(i = 0; i < (WTPEventRequest->WTPOperationalStatisticsCount); i++) {
			
			found=CW_FALSE;
			for(k=0; k<(WTPProtocolManager->radiosInfo).radioCount; k++) {

				if((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == (WTPEventRequest->WTPOperationalStatistics[i]).radioID) {

					found=CW_TRUE;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].TxQueueLevel = (WTPEventRequest->WTPOperationalStatistics[i]).TxQueueLevel;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].wirelessLinkFramesPerSec = (WTPEventRequest->WTPOperationalStatistics[i]).wirelessLinkFramesPerSec;
				}
			}
			/*if(!found)
			{
				for(k=0; k<(WTPProtocolManager->radiosInfo).radioCount; k++)
				{
					if((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == UNUSED_RADIO_ID); 
					{
						(WTPProtocolManager->radiosInfo).radiosInfo[k].radioID = (WTPEventRequest->WTPOperationalStatistics[i]).radioID;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].TxQueueLevel = (WTPEventRequest->WTPOperationalStatistics[i]).TxQueueLevel;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].wirelessLinkFramesPerSec = (WTPEventRequest->WTPOperationalStatistics[i]).wirelessLinkFramesPerSec;
				}
			}
			}*/
		}
	}

	if((WTPEventRequest->WTPRadioStatisticsCount) > 0) {
		
		int i,k;
		CWBool found;

		for(i=0; i < (WTPEventRequest->WTPRadioStatisticsCount); i++) {
			found=CW_FALSE;
			for(k = 0; k < (WTPProtocolManager->radiosInfo).radioCount; k++)  {

				if((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == (WTPEventRequest->WTPOperationalStatistics[i]).radioID) {

					found=CW_TRUE;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].statistics = (WTPEventRequest->WTPRadioStatistics[i]).WTPRadioStatistics;
				}
			}
			/*if(!found)
		{
			for(k = 0; k < (WTPProtocolManager->radiosInfo).radioCount; k++)
			{
					if((WTPProtocolManager->radiosInfo).radiosInfo[k].radioID == UNUSED_RADIO_ID);
				{
						(WTPProtocolManager->radiosInfo).radiosInfo[k].radioID = (WTPEventRequest->WTPOperationalStatistics[i]).radioID;
					(WTPProtocolManager->radiosInfo).radiosInfo[k].statistics = (WTPEventRequest->WTPRadioStatistics[i]).WTPRadioStatistics;
				}
			}
			}*/
		}
	}
	/*
	CW_FREE_OBJECT((WTPEventRequest->WTPOperationalStatistics), (WTPEventRequest->WTPOperationalStatisticsCount));
	CW_FREE_OBJECTS_ARRAY((WTPEventRequest->WTPRadioStatistics), (WTPEventRequest->WTPRadioStatisticsCount));
	Da controllare!!!!!!!
	*/
	CW_FREE_OBJECT(WTPEventRequest->WTPOperationalStatistics);
	CW_FREE_OBJECT(WTPEventRequest->WTPRadioStatistics);
	/*CW_FREE_OBJECT(WTPEventRequest);*/

	return CW_TRUE;
}
#endif

CWBool CWAssembleWTPEventResponse(int WTP_Index, 
				CWProtocolMessage **messagesPtr,
				int *fragmentsNumPtr,
				int seqNum){

	CWProtocolMessage *msgElems= NULL;
	int k = -1;
	int i;
	int msgElemCount=0;
	CWProtocolMessage *msgElemsBinding= NULL;
	int msgElemBindingCount=0;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling WTP[%d] Event Response...seqNum: %d", WTP_Index, seqNum);

	if(!(CWAssembleMessage(messagesPtr, 
	                       fragmentsNumPtr, 
	                       gWTPs[WTP_Index].pathMTU,
	                       seqNum,
	                       CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE,
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
	
	//CWLog("WTP Event Response Assembled");
	
	return CW_TRUE;
}
#endif
CWBool CWParseChangeStateEventRequestMessage2(CWProtocolMessage *msgPtr,
					      int len,
					      CWProtocolChangeStateEventRequestValues **valuesPtr) {

	int offsetTillMessages;
	int i=0;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_OBJECT_ERR(*valuesPtr,
			     CWProtocolChangeStateEventRequestValues,
			     return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	offsetTillMessages = msgPtr->offset;
	
	CWLog("");
	CWLog("#________ WTP Change State Event (Run) ________#");
	
	(*valuesPtr)->radioOperationalInfo.radiosCount = 0;
	(*valuesPtr)->radioOperationalInfo.radios = NULL;
	
	/* parse message elements */
	while((msgPtr->offset-offsetTillMessages) < len) {
		unsigned short int elemType = 0;/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int elemLen = 0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(msgPtr,&elemType,&elemLen);		

		/*CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);*/

		switch(elemType) {
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				/* just count how many radios we have, so we 
				 * can allocate the array
				 */
				((*valuesPtr)->radioOperationalInfo.radiosCount)++;
				msgPtr->offset += elemLen;
				break;
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE: 
				if(!(CWParseMsgElemResultCode(msgPtr, elemLen, &((*valuesPtr)->resultCode)))) 
					return CW_FALSE;
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Change State Event Request");
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	CW_CREATE_ARRAY_ERR((*valuesPtr)->radioOperationalInfo.radios,
			    (*valuesPtr)->radioOperationalInfo.radiosCount,
			    CWRadioOperationalInfoValues,
			    return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
	msgPtr->offset = offsetTillMessages;
	
	i = 0;

	while(msgPtr->offset-offsetTillMessages < len) {
		unsigned short int type = 0;	/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int len = 0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(msgPtr,&type,&len);		

		switch(type) {
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				/* will be handled by the caller */
				if(!(CWParseMsgElemWTPRadioOperationalState(msgPtr, len, &((*valuesPtr)->radioOperationalInfo.radios[i])))) 
					return CW_FALSE;
				i++;
				break;
			default:
				msgPtr->offset += len;
				break;
		}
	}
	CWLog("Change State Event Request Parsed");
	return CW_TRUE;
}

CWBool CWSaveChangeStateEventRequestMessage(CWProtocolChangeStateEventRequestValues *valuesPtr,
					    CWWTPProtocolManager *WTPProtocolManager) {

	CWBool found;
	CWBool retValue = CW_TRUE;
	int i,k;

	if(valuesPtr == NULL || WTPProtocolManager == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	
	for(i=0; i<valuesPtr->radioOperationalInfo.radiosCount; i++) {

		found=CW_FALSE;
		for(k=0; k<WTPProtocolManager->radiosInfo.radioCount; k++) {

			if(WTPProtocolManager->radiosInfo.radiosInfo[k].radioID == valuesPtr->radioOperationalInfo.radios[i].ID) {
				found=CW_TRUE;
				WTPProtocolManager->radiosInfo.radiosInfo[k].operationalState = valuesPtr->radioOperationalInfo.radios[i].state;
				WTPProtocolManager->radiosInfo.radiosInfo[k].operationalCause = valuesPtr->radioOperationalInfo.radios[i].cause;
				break;
			}
		}
		
		if(!found) {
			retValue= CW_FALSE;
			break;
		}
	}
	
	CW_FREE_OBJECT(valuesPtr->radioOperationalInfo.radios);
	CW_FREE_OBJECT(valuesPtr);	

	return retValue;
}


CWBool CWParseEchoRequestMessage(CWProtocolMessage *msgPtr, int len) {

	int offsetTillMessages;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	offsetTillMessages = msgPtr->offset;
	
	CWLog("");
	CWLog("#________ Echo Request (Run) ________#");
	
	/* parse message elements */
	while((msgPtr->offset-offsetTillMessages) < len) {
		unsigned short int elemType = 0;/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int elemLen = 0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(msgPtr,&elemType,&elemLen);		

		/*CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);*/

		switch(elemType) {
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Echo Request must carry no message elements");
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) 
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	CWLog("Echo Request Parsed");
	
	return CW_TRUE;
}

CWBool CWAssembleEchoResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum) {

	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount=0;
	CWProtocolMessage *msgElemsBinding= NULL;
	int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Echo Response...");
		
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_ECHO_RESPONSE,
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
	
	CWLog("Echo Response Assembled, fragment number: %d", *fragmentsNumPtr);
	return CW_TRUE;
}

CWBool CWAssembleConfigurationUpdateRequest(CWProtocolMessage **messagesPtr,
					    int *fragmentsNumPtr,
					    int PMTU,
						int seqNum,
						int msgElement, const void *extraInfo) 
{
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount=0;	
	int *iPtr;

	iPtr = (int*)CWThreadGetSpecific(&gIndexSpecific);

	if (messagesPtr == NULL || fragmentsNumPtr == NULL)
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Configuration Update Request...");

	switch (msgElement) {
		// capwap
		case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:
		{
			if (extraInfo == NULL) {
				return CWErrorRaise(CW_ERROR_WRONG_ARG, "NULL Image Identifier.");
			}

			if (gWTPs[*iPtr].imageActionState != NO_ACTION) {
				CWLog("Error: %s function is called, when imageActionState is not NO_ACTION, imageActionState = %d", __FUNCTION__, gWTPs[*iPtr].imageActionState);
				return CW_FALSE;
			}

			msgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!(CWAssembleMsgElemImageIdentifier(&msgElems[0], cwRtkAcGetVendorId(), (const char *)extraInfo))) {
				CW_FREE_PROTOCOL_MESSAGE(msgElems[0]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
		}
		
		// 802.11 binding
		case CW_MSG_ELEMENT_IEEE80211_WTP_RADIO_INFORMATION_CW_TYPE:
		case BINDING_MSG_ELEMENT_TYPE_OFDM_CONTROL:
		{
			// XXX gWTPs[*iPtr].qosValues is not assigned....
			if(!CWBindingAssembleConfigurationUpdateRequest(&msgElemsBinding, &msgElemBindingCount, msgElement))
			  return CW_FALSE;
			break;
		}

		// vendor specific - rtk
		case CW_MSG_ELEMENT_RTK_POWER_SCALE_CW_TYPE:
		{
			if (extraInfo == NULL) {
				return CWErrorRaise(CW_ERROR_WRONG_ARG, "NULL Power Scale.");
			}
			msgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!CWAssembleMsgElemRtkPowerScale(&msgElems[0], (CWProtocolRtkPowerScaleValues *)extraInfo)) {
				CW_FREE_PROTOCOL_MESSAGE(msgElems[0]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
		}	
		case CW_MSG_ELEMENT_RTK_CHANNEL_CW_TYPE:
		{
			if (extraInfo == NULL) {
				return CWErrorRaise(CW_ERROR_WRONG_ARG, "NULL Channel.");
			}
			msgElemCount = 1;
			CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			if (!CWAssembleMsgElemRtkChannel(&msgElems[0], (CWProtocolRtkChannelValues *)extraInfo)) {
				CW_FREE_PROTOCOL_MESSAGE(msgElems[0]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
		}	
		
 		// vendor specific - others
		case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI:	
		case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_WUM:
		{
			CWLog("Assembling UCI Conf Update Request");
			if(!CWProtocolAssembleConfigurationUpdateRequest(&msgElems, &msgElemCount, msgElement))
			  return CW_FALSE;
			break;
		}
		
 		// not supported
		default:
		{
			CWLog("Error: Cannot Assemble the specified Conf Update Request, element type: %d.", msgElement);
			return CW_FALSE;
			break;
		}
	}	  

	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST,
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

	CWLog("Configuration Update Request Assembled");

	if (msgElement == CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE) {
		gWTPs[*iPtr].imageActionState = WAITNG_FOR_CONFIGURATION_UPDATE_RESPONSE_FROM_WTP;
	}
	
	return CW_TRUE;
}

CWBool CWAssembleClearConfigurationRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum) 
{
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Clear Configuration Request...");
	
	
	if(!(CWAssembleMessage(messagesPtr, 
						   fragmentsNumPtr, 
						   PMTU, 
						   seqNum,
						   CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_REQUEST, 
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

	CWLog("Clear Configuration Request Assembled");
	
	return CW_TRUE;
}

#ifdef KM_BY_AC	//key management
CWBool CWAssembleWLANConfigurationRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum,unsigned char* recv_packet,int Operation, int len_packet) {
	
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount=1;
	int k = -1;
	
		
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling WLAN 802.11 Configuration Request...");
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	// Assemble Message Elements
	
	if( Operation==CW_MSG_ELEMENT_IEEE80211_ADD_WLAN_CW_TYPE ){
		if (!(CWAssembleMsgElemAddWLAN(0,&(msgElems[++k]), recv_packet, len_packet)))   //radioID = 0 -valore predefinito-
		{
			
			CWErrorHandleLast();
			int i;
			for(i = 0; i <= k; i++) {CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE; // error will be handled by the caller
		}
		
	}else if( Operation==CW_MSG_ELEMENT_IEEE80211_DELETE_WLAN_CW_TYPE ){
		
		if (!(CWAssembleMsgElemDeleteWLAN(0,&(msgElems[++k]),recv_packet, len_packet)))   //radioID = 0 -valore predefinito-
		{
			CWErrorHandleLast();
			int i;
			for(i = 0; i <= k; i++) {CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE; // error will be handled by the caller
		}
	}
	

/*  to be implemented in a case of Binding with appropriate messages elements -- see draft capwap-spec && capwap-binding 
	if(!CWBindingAssembleConfigurationUpdateRequest(&msgElemsBinding, &msgElemBindingCount)){
		return CW_FALSE;
	}
*/
	if(!(CWAssembleMessage(messagesPtr, 
	                       fragmentsNumPtr, 
	                       PMTU, 
	                       seqNum,
	                       CW_MSG_TYPE_VALUE_WLAN_CONFIGURATION_REQUEST, 
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
	
	CWLog("WLAN 802.11 Configuration Request Assembled");
	
	return CW_TRUE;
}
#if 0
CWBool CWAssembleWLANConfigurationRequestAddWlan(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum,
					const CWProtocolAddWLANValues *addWlan)
{
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount=1;
	int k = -1;
	
	
	CWLog("Assembling WLAN Configuration Request (Add Wlan)...");
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	// Assemble Message Elements
	
	if (!CWAssembleMsgElemAddWLAN(&msgElems[++k], addWlan))
	{
		
		CWErrorHandleLast();
		int i;
		for(i = 0; i <= k; i++) {CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

	if(!(CWAssembleMessage(messagesPtr, 
	                       fragmentsNumPtr, 
	                       PMTU, 
	                       seqNum,
	                       CW_MSG_TYPE_VALUE_WLAN_CONFIGURATION_REQUEST, 
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
	
	CWLog("WLAN Configuration Request Assembled (Add Wlan)");
	
	return CW_TRUE;
}
#endif
#endif

#ifdef RTK_SMART_ROAMING
CWBool CWAssembleStationConfigurationRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum,unsigned char* StationMacAddr, int radio_id, int Operation) 
{
	
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount=1;
	int k = -1;
	
		
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	//CWLog("Assembling Station Configuration Request...");
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	// Assemble Message Elements
	switch(Operation){
		case CW_MSG_ELEMENT_DELETE_STATION_CW_TYPE:
			if (!(CWAssembleMsgElemDeleteStation(radio_id,&(msgElems[++k]),StationMacAddr)))
			{
				CWErrorHandleLast();
				int i;
				for(i=0; i<=k; i++) CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE; // error will be handled by the caller
			}
			break;
		case CW_MSG_ELEMENT_FREE_STATION_CW_TYPE:
			if(!(CWAssembleMsgElemFreeStation(radio_id,&(msgElems[++k]),StationMacAddr)))
			{
				CWErrorHandleLast();
				int i;
				for(i=0; i<=k; i++) CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
		case CW_MSG_ELEMENT_SILENT_STATION_CW_TYPE:
			if(!(CWAssembleMsgElemSilentStation(radio_id,&(msgElems[++k]),StationMacAddr)))
			{
				CWErrorHandleLast();
				int i;
				for(i=0; i<=k; i++) CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
		case CW_MSG_ELEMENT_UNSILENT_STATION_CW_TYPE:
			if(!(CWAssembleMsgElemUnsilentStation(radio_id,&(msgElems[++k]),StationMacAddr)))
			{
				CWErrorHandleLast();
				int i;
				for(i=0; i<=k; i++) CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
		case CW_MSG_ELEMENT_DOT11V_STATION_CW_TYPE:
			if(!(CWAssembleMsgElemDot11vStation(radio_id,&(msgElems[++k]),StationMacAddr)))
			{
				CWErrorHandleLast();
				int i;
				for(i=0; i<=k; i++) CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
		case CW_MSG_ELEMENT_DUAL_STATION_CW_TYPE:
			if(!(CWAssembleMsgElemDualStation(radio_id,&(msgElems[++k]),StationMacAddr)))
			{
				CWErrorHandleLast();
				int i;
				for(i=0; i<=k; i++) CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
				CW_FREE_OBJECT(msgElems);
				return CW_FALSE;
			}
			break;
	}	


/*  to be implemented in a case of Binding with appropriate messages elements -- see draft capwap-spec && capwap-binding 
	if(!CWBindingAssembleConfigurationUpdateRequest(&msgElemsBinding, &msgElemBindingCount)){
		return CW_FALSE;
	}
*/
	if(msgElemCount != PENDING_MESSAGE_ELEMENT_COUNT)
		CWLog("Wrong Pending Message Count");
	
	if(!(CWAssembleMessage(messagesPtr, 
	                       fragmentsNumPtr, 
	                       PMTU, 
	                       seqNum,
	                       CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST, 
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

	//CWLog("Station Configuration Request Assembled");
	
	return CW_TRUE;
}
#endif


CWBool CWAssembleResetRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, const char *reqImage)
{
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount=1;

	CWLog("Assembling Reset Request...");
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if (!CWAssembleMsgElemImageIdentifier(&msgElems[0], cwRtkAcGetVendorId(), reqImage)) {
		CWErrorHandleLast();
		CW_FREE_PROTOCOL_MESSAGE(msgElems[0]);
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
		
	if(!(CWAssembleMessage(messagesPtr, 
						   fragmentsNumPtr, 
						   PMTU, 
						   seqNum,
						   CW_MSG_TYPE_VALUE_RESET_REQUEST, 
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

	CWLog("Reset Request Assembled");

	return CW_TRUE;

}


CWBool CWParseResetResponseMessage(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode)
{
	int offsetTillMessages;

	offsetTillMessages = msgPtr->offset;
	*resultCode = CW_PROTOCOL_SUCCESS;
	CWLog("Parsing Reset Response...");

	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				*resultCode=CWProtocolRetrieve32(msgPtr);
				break;	
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Reset Response");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

	CWLog("Reset Response Parsed");

	return CW_TRUE;	
}	


CWBool CWStartNeighborDeadTimer(int WTPIndex) {

	/* start NeighborDeadInterval timer */
	if(!CWErr(CWTimerRequest(gCWNeighborDeadInterval,
			     0,
				 &(gWTPs[WTPIndex].thread),
				 &(gWTPs[WTPIndex].currentTimer),
				 CW_CRITICAL_TIMER_EXPIRED_SIGNAL))) {
		return CW_FALSE;
	}
	return CW_TRUE;
}

#if 0
Babylon: This function is not used.
CWBool CWStartNeighborDeadTimerForEcho(int WTPIndex){
	
	int echoInterval;

	/* start NeighborDeadInterval timer */
	CWACGetEchoRequestTimer(&echoInterval);
	if(!CWErr(CWTimerRequest(echoInterval,
				 &(gWTPs[WTPIndex].thread),
				 &(gWTPs[WTPIndex].currentTimer),
				 CW_CRITICAL_TIMER_EXPIRED_SIGNAL))) {
		return CW_FALSE;
	}
	return CW_TRUE;
}
#endif

CWBool CWStopNeighborDeadTimer(int WTPIndex) {

	if(!CWTimerCancel(&(gWTPs[WTPIndex].currentTimer))) {
	
		return CW_FALSE;
	}
	return CW_TRUE;
}

CWBool CWRestartNeighborDeadTimer(int WTPIndex) {
	
	CWThreadSetSignals(SIG_BLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);	
	
	if(!CWStopNeighborDeadTimer(WTPIndex)) return CW_FALSE;
	if(!CWStartNeighborDeadTimer(WTPIndex)) return CW_FALSE;
	
	CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
	
	//CWDebugLog("NeighborDeadTimer restarted");
	return CW_TRUE;
}

/* Babylon: This function is not used.
CWBool CWRestartNeighborDeadTimerForEcho(int WTPIndex) {
	
	CWThreadSetSignals(SIG_BLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);

	if(!CWStopNeighborDeadTimer(WTPIndex)) return CW_FALSE;
	if(!CWStartNeighborDeadTimerForEcho(WTPIndex)) return CW_FALSE;

	CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
	
	CWDebugLog("NeighborDeadTimer restarted for Echo interval");
	return CW_TRUE;
}
*/



#ifdef RTK_CAPWAP
CWBool CWAssembleRtkWTPStatusRequestSiteSurvey(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum)
{
		CWProtocolMessage *msgElemsBinding = NULL;
		int msgElemBindingCount=0;
		CWProtocolMessage *msgElems = NULL;
		int msgElemCount=1;
	
		CWLog("Assembling RTK WTP Status Request (Site Survey)...");
		
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
		if (!CWAssembleMsgElemRtkSiteSurveyRequest(&msgElems[0])) {
			CWErrorHandleLast();
			CW_FREE_PROTOCOL_MESSAGE(msgElems[0]);
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE; // error will be handled by the caller
		}
			
		if(!(CWAssembleMessage(messagesPtr, 
							   fragmentsNumPtr, 
							   PMTU, 
							   seqNum,
							   CW_MSG_TYPE_VALUE_RTK_WTP_STATUS_REQUEST, 
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
	
		CWLog("RTK WTP Status Request Assembled");
	
		return CW_TRUE;
	
}

CWBool CWAssembleRtkWTPStatusRequestStations(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum)
{
		CWProtocolMessage *msgElemsBinding = NULL;
		int msgElemBindingCount=0;
		CWProtocolMessage *msgElems = NULL;
		int msgElemCount=1;
	
		CWLog("Assembling RTK WTP Status Request (List Stations)...");
		
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
		if (!CWAssembleMsgElemRtkStationRequest(&msgElems[0])) {
			CWErrorHandleLast();
			CW_FREE_PROTOCOL_MESSAGE(msgElems[0]);
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE; // error will be handled by the caller
		}
			
		if(!(CWAssembleMessage(messagesPtr, 
							   fragmentsNumPtr, 
							   PMTU, 
							   seqNum,
							   CW_MSG_TYPE_VALUE_RTK_WTP_STATUS_REQUEST, 
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
	
		CWLog("RTK WTP Status Request Assembled");
	
		return CW_TRUE;
	
}


CWBool CWParseRtkWTPStatusResponse(CWProtocolMessage* msgPtr, int len, CWProtocolRtkWTPStatusResponseValues *wtpStatus)
{
	int offsetTillMessages;
	
	if(msgPtr == NULL || wtpStatus==NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if((msgPtr->msg) == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	offsetTillMessages = msgPtr->offset;

	CWBool	haveResultCode = CW_FALSE;
	wtpStatus->numSiteSurvey = 0;
	wtpStatus->siteSurveyResponsed = CW_FALSE;
	wtpStatus->numRtkStations = 0;
	wtpStatus->rtkStationsResponsed = CW_FALSE;
	
	CWLog("Parsing RTK WTP Status Response...");
	
	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				wtpStatus->resultCode = CWProtocolRetrieve32(msgPtr);
				haveResultCode = CW_TRUE;
				break;
			case CW_MSG_ELEMENT_RTK_SITE_SURVEY_REQUEST_CW_TYPE:
				wtpStatus->siteSurveyResponsed = CW_TRUE;
				break;
			case CW_MSG_ELEMENT_RTK_SITE_SURVEY_CW_TYPE:

				CWParseMsgElemRtkSiteSurvey(msgPtr, elemLen, &wtpStatus->siteSurvey[wtpStatus->numSiteSurvey]);
				wtpStatus->numSiteSurvey++;
				break;
			case CW_MSG_ELEMENT_RTK_STATION_REQUEST_CW_TYPE:				

				wtpStatus->rtkStationsResponsed = CW_TRUE;
				break;
			case CW_MSG_ELEMENT_RTK_STATION_CW_TYPE:

				CWParseMsgElemRtkStation(msgPtr, elemLen, &wtpStatus->rtkStations[wtpStatus->numRtkStations]);
				wtpStatus->numRtkStations++;
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Configuration Update Response");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	CWLog("RTK WTP Status Response Parsed (site survey number=%d, station number=%d)", wtpStatus->numSiteSurvey, wtpStatus->numRtkStations);

	if (!haveResultCode || (!wtpStatus->siteSurveyResponsed && !wtpStatus->rtkStationsResponsed)) {		
		CWLog("RTK WTP Status Response without resultCode or without type(site-survey or station-list)!! ignored the message!");
		return CW_FALSE;
	}
	return CW_TRUE;

}

#endif

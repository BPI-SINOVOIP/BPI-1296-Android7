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
 * Authors : babylon (sptyan@gmail.com)
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

CWBool CWParseImageDataRequestByAC(CWProtocolMessage* msgPtr, int len, CWProtocolImageIdentifierValues *image_identifier, CWBool *GotInitiate)
{
	int offsetTillMessages;	
	offsetTillMessages = msgPtr->offset;
	*GotInitiate = CW_FALSE;;
	CWLog("Parsing Image Data Request ...");

	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:
				CWParseMsgElemImageIdentifier(msgPtr, elemLen, image_identifier);
				break;
				
			case CW_MSG_ELEMENT_INITIATED_DOWNLOAD_CW_TYPE:
				*GotInitiate = CW_TRUE;;
				break;	

			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Image Data Request");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

	CWLog("Image Data Request Parsed");

	return CW_TRUE;	


}

CWBool CWAssembleImageDataResponseByAC(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode, uint32_t fileSize, uint8_t *hash)
{
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount;
	
	if (fileSize) {
		msgElemCount=2;
	} else {
		msgElemCount=1;
	}

	CWLog("Assembling Image Data Response...");
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if (!CWAssembleMsgElemResultCode(&msgElems[0],resultCode)) {
		int i;
		CWErrorHandleLast();
		for (i=0; i<msgElemCount; i++) {
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
		}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

	if (fileSize) {
		if (!CWAssembleMsgElemImageInformation(&msgElems[1], fileSize, hash)) {
			int i;
			CWErrorHandleLast();
			for (i=0; i<msgElemCount; i++) {
				CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
			}
			CW_FREE_OBJECT(msgElems);
			return CW_FALSE; // error will be handled by the caller
		}
	}

	
	if(!(CWAssembleMessage(messagesPtr, 
						   fragmentsNumPtr, 
						   PMTU, 
						   seqNum,
						   CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE, 
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

	CWLog("Image Data Response Assembled");

	return CW_TRUE;

}

CWBool CWAssembleImageDataRequestByAC(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWBool data_type_eof, void *data, int byteToSend)
{
	CWProtocolMessage *msgElemsBinding = NULL;
	int msgElemBindingCount=0;
	CWProtocolMessage *msgElems = NULL;
	int msgElemCount = 1;

	CWLog("Assembling Image Data Requst...");	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
/*
	{
		int j;
		printf("data:");
		for (j=0; j<byteToSend; j++) {
			printf(" %02x", *(char *)data+j);
		}
		printf("\n");
	}
	
*/
	if (!CWAssembleMsgElemImageData(&msgElems[0], data, byteToSend, data_type_eof)) {
		CWErrorHandleLast();
		CW_FREE_PROTOCOL_MESSAGE(msgElems[0]);
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	 
	if(!(CWAssembleMessage(messagesPtr, 
							fragmentsNumPtr, 
							PMTU, 
							seqNum,
							CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST, 
							msgElems, 
							msgElemCount, 
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
 
	CWLog("Image Data Requst Assembled");
	return CW_TRUE;
}

CWBool CWParseImageDataResponseByAC(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode)
{
	CWBool resultExist = CW_FALSE;
	int offsetTillMessages;
	offsetTillMessages = msgPtr->offset;
	CWLog("Parsing Image Data Response...");


	// parse message elements
	while((msgPtr->offset - offsetTillMessages) < len) {
		unsigned short int elemType=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(msgPtr, &elemType, &elemLen);
		
		switch(elemType) {
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				*resultCode=CWProtocolRetrieve32(msgPtr);
				resultExist = CW_TRUE;
				break;	
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element in Image Data Response");
				break;	
		}
	}
	
	if((msgPtr->offset - offsetTillMessages) != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	if (!resultExist) {
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "No Result Code in Image Data Response");
	}

	CWLog("Image Data Response Parsed");

	return CW_TRUE;	

}

CWBool CWSendOneImageDataRequest(int WTPIndex)
{
	struct stat statBuf;
	int byteToSend;
	int seqNum;

//static int c=0;
//printf("%d ", c++);
	// open file to send file data to WTP.
	if (gWTPs[WTPIndex].pReqImageData == NULL) {
		const char *fn = ACGetImageFile(gWTPs[WTPIndex].reqImageIdentifier);
		gWTPs[WTPIndex].reqImageFd = open(fn, O_RDONLY);
		if (gWTPs[WTPIndex].reqImageFd< 0) {
			CWLog("Error: cannot open file %s for assembling Image Data Request", fn);
			return CW_FALSE;
		}
	
		if (fstat(gWTPs[WTPIndex].reqImageFd, &statBuf) == -1) {				
			CWLog("Error: cannot read stat of file %s for assembling Image Data Request", fn);
			close(gWTPs[WTPIndex].reqImageFd);
			return CW_FALSE;
		}
	
		gWTPs[WTPIndex].reqImageSize = statBuf.st_size;		
		gWTPs[WTPIndex].reqImageDataSent = 0;
		gWTPs[WTPIndex].pReqImageData = mmap(NULL, statBuf.st_size, PROT_READ, MAP_SHARED , gWTPs[WTPIndex].reqImageFd, 0);
	}
	
	// Find the data to send			
	if (gWTPs[WTPIndex].reqImageSize - gWTPs[WTPIndex].reqImageDataSent <= CW_MAX_BYTES_PER_IMAGE_DATA) {
		gWTPs[WTPIndex].reqImageLastMsg = CW_TRUE;
		byteToSend = gWTPs[WTPIndex].reqImageSize - gWTPs[WTPIndex].reqImageDataSent;
	} else {
		gWTPs[WTPIndex].reqImageLastMsg = CW_FALSE; 	
		byteToSend = CW_MAX_BYTES_PER_IMAGE_DATA;
	}
	
	// Assemble Image Data Request
	seqNum = CWGetSeqNum();
	if (!CWAssembleImageDataRequestByAC(&gWTPs[WTPIndex].messages,
					  &gWTPs[WTPIndex].messagesCount,
					  gWTPs[WTPIndex].pathMTU, seqNum, gWTPs[WTPIndex].reqImageLastMsg, 
					  gWTPs[WTPIndex].pReqImageData + gWTPs[WTPIndex].reqImageDataSent, byteToSend)) {
		
		munmap(gWTPs[WTPIndex].pReqImageData, gWTPs[WTPIndex].reqImageSize);
		gWTPs[WTPIndex].pReqImageData = NULL;
		close(gWTPs[WTPIndex].reqImageFd);
		return CW_FALSE;
	}
	fflush(NULL);

	// Send Image Data Request
	if(!CWACSendAcknowledgedPacket(WTPIndex, 
			  CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE,
			  seqNum)) {
		CWACStopRetransmission(WTPIndex);
		
		munmap(gWTPs[WTPIndex].pReqImageData, gWTPs[WTPIndex].reqImageSize);
		gWTPs[WTPIndex].pReqImageData = NULL;
		close(gWTPs[WTPIndex].reqImageFd);					
		return CW_FALSE;
	}
	gWTPs[WTPIndex].reqImageDataSent += byteToSend;
	gWTPs[WTPIndex].imageActionState = TRANSMITING_IMAGE_DATA_TO_WTP;
	
	// close file if is last message
	if (gWTPs[WTPIndex].reqImageLastMsg) {
		//printf("closefile ");
		munmap(gWTPs[WTPIndex].pReqImageData, gWTPs[WTPIndex].reqImageSize);
		gWTPs[WTPIndex].pReqImageData = NULL;
		close(gWTPs[WTPIndex].reqImageFd);					
	}
	return CW_TRUE;

}

CWBool ACEnterImageData(int WTPIndex, CWProtocolMessage *msgPtr, int pktLeftLength, int seqNum)
{
	int i;
	CWProtocolImageIdentifierValues image_identifier;
	CWBool GotInitiate;
	CWProtocolMessage* messages =NULL;
	int messagesCount=0;
	
	if (gWTPs[WTPIndex].imageActionState != NO_ACTION) {
		CWLog("got Image Data Request in ImageDataState when imageActionState is not in NO_ACTION."
			"imageActionState=%d", gWTPs[WTPIndex].imageActionState);
		return CWErrorRaise(CW_ERROR_OPERATION_ABORTED, NULL);
	}

	image_identifier.imageIdentifier[0] = '\0';
	if(!(CWParseImageDataRequestByAC(msgPtr, pktLeftLength, &image_identifier, &GotInitiate)))
		return CW_FALSE;

/*
	if (timerSet) {
		if(!CWRestartNeighborDeadTimer(WTPIndex)) {
			CWCloseThread();
		}
	} else {
		if(!CWStartNeighborDeadTimer(WTPIndex)) {
			CWCloseThread();
		}
	}
*/
	if (image_identifier.imageIdentifier[0] == '\0' || GotInitiate == CW_FALSE ||
		strcmp(image_identifier.imageIdentifier, gWTPs[WTPIndex].wtpConfig.fwImageIdentifier) != 0) {
		CWLog("Got Image Data Request, but unexpected. version_str=%s", image_identifier.imageIdentifier);
		if (!GotInitiate) {
			CWLog("No Message Element 'Initiated Download'");
		}
		return CWErrorRaise(CW_ERROR_OPERATION_ABORTED, NULL);
	}
	
	// Assemble Image Data Response
	if(!(CWAssembleImageDataResponseByAC(&messages,
					&messagesCount,
					gWTPs[WTPIndex].pathMTU,
					seqNum, CW_PROTOCOL_SUCCESS, 0, NULL))) {	// TODO Babylon: count hash
		gWTPs[WTPIndex].imageActionState = NO_ACTION;
		gWTPs[WTPIndex].reqImageIdentifier[0] = '\0'; 							
		return CWErrorRaise(CW_ERROR_OPERATION_ABORTED, NULL);
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
			return CWErrorRaise(CW_ERROR_OPERATION_ABORTED, NULL);
		}
	}
	CWFreeMessageFragments(messages, messagesCount);
	CW_FREE_OBJECT(messages);

	
	CWLog("\n");
	CWLog("######### Image Data State #########");	

	strncpy(gWTPs[WTPIndex].reqImageIdentifier, gWTPs[WTPIndex].wtpConfig.fwImageIdentifier, CW_IMAGE_IDENTIFIER_MAX_SIZE);
	gWTPs[WTPIndex].currentState = CW_ENTER_IMAGE_DATA;

	if (!CWSendOneImageDataRequest(WTPIndex)) { 			
		gWTPs[WTPIndex].imageActionState = NO_ACTION;
		gWTPs[WTPIndex].reqImageIdentifier[0] = '\0'; 
		return CWErrorRaise(CW_ERROR_OPERATION_ABORTED, NULL);
	}

	CWLog("Image Data Request Sent");

	return CW_TRUE;
}

CWBool ACEnterImageData2ndStage(int WTPIndex, CWProtocolMessage *msgPtr)
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

	ret = CW_TRUE;
	/* skip timestamp */
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;

	switch (controlVal.messageTypeValue) {
		case CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE:
		{
			CWProtocolResultCode resultCode;
			if (gWTPs[WTPIndex].imageActionState != TRANSMITING_IMAGE_DATA_TO_WTP) {
				CWLog("Error: imageActionState != WATING_FOR_IMAGE_DATA_RESPONSE_FROM_WTP, but got Image Data Response"
					"imageActionState=%d", gWTPs[WTPIndex].imageActionState);
				ret = CWErrorRaise(CW_ERROR_OPERATION_ABORTED, NULL);
				break;
			}
			
			if(!CWParseImageDataResponseByAC(&completeMsg, controlVal.msgElemsLen, &resultCode)) {
				CWLog("CWParseImageDataResponseByAC failed. Ignored!");
				ret = CWErrorRaise(CW_ERROR_INVALID_FORMAT, NULL);
				break;
			}
			//printf("stop retransmition\n");
			if (resultCode != CW_PROTOCOL_SUCCESS) {
				CWLog("Error: image data response result code = %d", resultCode);				
				ret = CWErrorRaise(CW_ERROR_INVALID_FORMAT, NULL);
				break; //ignore this response. (remain ReTx)
			}
			CWACStopRetransmission(WTPIndex);
//static int r=0;
//printf("stop retrans %d\n", r++);
/*
			if (timerSet) {
				if(!CWRestartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			} else {
				if(!CWStartNeighborDeadTimer(WTPIndex)) {
					CWCloseThread();
				}
			}
*/
			if (gWTPs[WTPIndex].reqImageLastMsg) {
				// All data transfer done. WTP will reset by it self!				
				CWLog("Image Tx Done in Image Data State, Close Thread!");
				CWCloseThread();
/*
				seqNum = CWGetSeqNum();
				if (!CWAssembleResetRequest(&(gWTPs[WTPIndex].messages),
										&(gWTPs[WTPIndex].messagesCount),
										gWTPs[WTPIndex].pathMTU,
										seqNum, gWTPs[WTPIndex].reqImageIdentifier)) {
printf("Cannot assemble Reset Request\n");
					return CW_FALSE;				  
				}
				if(!CWACSendAcknowledgedPacket(WTPIndex, CW_MSG_TYPE_VALUE_RESET_RESPONSE, seqNum)) {
					CWLog("CWACSendAcknowledgedPacket for RESET failed!");
printf("CWACSendAcknowledgedPacket for RESET filed\n");
					CWACStopRetransmission(WTPIndex);
					return CW_FALSE;
				}	
				gWTPs[WTPIndex].imageActionState = WATING_FOR_RESET_RESPONSE_FROM_WTP;
 */			
			} else if (gWTPs[WTPIndex].pReqImageData) { 
				// file still open, has more data to transfer
				if(!CWSendOneImageDataRequest(WTPIndex)) {
					CWLog("CWSendOneImageDataRequest filed");
					ret = CWErrorRaise(CW_ERROR_OPERATION_ABORTED, NULL);
				}
			} else {
				// Last message was sent, but image file was closed, strange situation!
				CWLog("Last message was sent, but image file was closed, strange situation!");
				ret = CWErrorRaise(CW_ERROR_OPERATION_ABORTED, NULL);
			}
			break;
		}
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

#include "CWWTP.h"
#include "WTPRtkSystemInteraction.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif


CWBool gImageDataStarted = CW_FALSE;
CWBool gImageDataEof	= CW_FALSE;
int gLastSeqNum; // handle in-sequence problem
				// TODO: Babylon: Currently it is only used for image data request from AC. Should be general used in the future.
CWBool gWillReboot = CW_FALSE;

CWBool CWAssembleImageDataRequestByWTP(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList cw_list) 
{
	
	CWProtocolMessage *msgElems = NULL;
	const int msgElemCount = 2;
	CWProtocolMessage *msgElemsBinding = NULL;
	const int msgElemBindingCount = 0;
	const char *version_str = cw_list->data;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Image Data Request ... (%s)", version_str);

	//CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	

	if (!CWAssembleMsgElemImageIdentifier(&msgElems[0], cwRtkWtpGetVendorId(), version_str)) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}
	if (!CWAssembleMsgElemInitiateDownload(&msgElems[1])) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
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
			       CW_PACKET_PLAIN
#else
			       CW_PACKET_CRYPT
#endif
			       ))) 
		return CW_FALSE;
	
	CWLog("Image Data Request Assembled");
	return CW_TRUE;
}

CWBool CWAssembleImageDataResponseByWTP(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode) 
{
	
	CWProtocolMessage *msgElems = NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding = NULL;
	const int msgElemBindingCount = 0;
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWLog("Assembling Image Data Response...");
	
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
			       CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE,
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
	
	CWLog("Image Data Response Assembled");
	
	return CW_TRUE;
}

CWBool CWParseImageDataResponseByWTP (unsigned char *msg, int len, int seqNum, void *values) 
{
	CWControlHeaderValues controlVal;
	CWProtocolMessage completeMsg;
	int offsetTillMessages;
	CWProtocolImageDataResponseToWTPValues *valuesPtr = values;
	CWBool haveResultCode;

	CWLog("Parsing Image Data Response...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	/* error will be handled by the caller */
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE;
	
	/* different type */
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, 
				    "Message is not Image Data Response as Expected");
	
	if(controlVal.seqNum != seqNum) 
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, 
				    "Different Sequence Number");
	
	/* skip timestamp */
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;
	offsetTillMessages = completeMsg.offset;	
	
	/* parse message elements */
	haveResultCode = CW_FALSE;
	valuesPtr->withImageInformation = CW_FALSE;
	
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;	/* = CWProtocolRetrieve32(&completeMsg); */
		unsigned short int len=0;	/* = CWProtocolRetrieve16(&completeMsg); */
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);
		/* CWDebugLog("Parsing Message Element: %u, len: %u", type, len); */
	
		switch(type) {
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				haveResultCode = CW_TRUE;
				if(!CWParseMsgElemResultCode(&completeMsg, len, &valuesPtr->resultCode)) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_IMAGE_INFO_CW_TYPE:
				valuesPtr->withImageInformation = CW_TRUE;
				if(!CWParseMsgElemImageInformation(&completeMsg, len, &valuesPtr->imageInformation)) return CW_FALSE;
				break;
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT,
					"Unrecognized Message Element");
		}
	}
	
	if(completeMsg.offset != len) 
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT,
				    "Garbage at the End of the Message");

	CWLog("Image Data Response Parsed...");
	return haveResultCode;
}

	
CWBool CWParseImageDataRequestByWTP(unsigned char *msg, int len, CWBool needSave,
									CWBool (*save_data)(const CWProtocolImageDataValues *, CWBool *), CWProtocolResultCode *resultCode)
{
	CWProtocolMessage completeMsg;

	// defulat resultCode
	*resultCode = CW_PROTOCOL_SUCCESS;

	// prepare to parse the message
	CWLog("Parsing Image Data Request...");
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType = 0;
		unsigned short int elemLen = 0;
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		
							
		switch(elemType) { 

			case CW_MSG_ELEMENT_IMAGE_DATA_CW_TYPE:
			{
				CWProtocolImageDataValues image_data;
				if (!CWParseMsgElemImageData(&completeMsg, elemLen, &image_data)) {
					return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Image Data (Message Element) format Error");
				}
				if (needSave) {
					if (!save_data(&image_data, &gImageDataEof)) {
						CWLog("save_data() failed %s", __FUNCTION__);
						*resultCode = CW_PROTOCOL_IMG_FAILURE_OTHER_ERROR;
					}
				}
				break;
			}
			default:
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");

	CWLog("Image Data Request Parsed");
	return CW_TRUE;

}

CWBool CWSaveImageDataResponseByWTP(void *values)
{
	CWProtocolImageDataResponseToWTPValues *valuesPtr = values;
	if (valuesPtr->resultCode == CW_PROTOCOL_SUCCESS) {
		cwRtkWtpPrepareFwDownload(valuesPtr->newFirmwareVersion, &valuesPtr->imageInformation, valuesPtr->withImageInformation);
	}
	return CW_TRUE;
}

CWBool CWWTPInitiateImageDownload(const char *newImageVersion)
{
	CWListElement cw_list;
	CWProtocolImageDataResponseToWTPValues imageDataResponseValues;

	cw_list.data = (void *)newImageVersion;
	cw_list.next = NULL;
	imageDataResponseValues.newFirmwareVersion = newImageVersion;

	CWLog("Start to Handle Image Download...");
	
	if(!CWErr(CWWTPSendAcknowledgedPacket(CWGetSeqNum(),
					  &cw_list,
					  CWAssembleImageDataRequestByWTP,
					  (void*)CWParseImageDataResponseByWTP,
					  (void*)CWSaveImageDataResponseByWTP, &imageDataResponseValues))) {

		CWLog("Error when CWWTPSendAcknowledgedPacket CWAssembleImageDataRequestByWTP...");

		return CW_FALSE;					  
	}
	gImageDataStarted = CW_FALSE;
	gImageDataEof = CW_FALSE;
	return CW_TRUE;
}

CWStateTransition CWWTPEnterImageData(const char *newImageVersion)
{
printf("(%d)%s\n", rtk_getpid(), __FUNCTION__);
	CWLog("\n");
	CWLog("######### Image Data State #########");

	if (!CWWTPInitiateImageDownload(newImageVersion)) {
		return CW_ENTER_RESET;
	}

	CW_REPEAT_FOREVER {
		struct timespec timenow;
		CWBool bReceivePacket = CW_FALSE;

//printf("(%s) forever loop\n", __FUNCTION__);
		/* Wait packet */
		timenow.tv_sec = time(0) + CW_NEIGHBORDEAD_RESTART_DISCOVERY_DELTA_DEFAULT;	 /* greater than NeighborDeadInterval */
		timenow.tv_nsec = 0;

		CWThreadMutexLock(&gInterfaceMutex);

		/*
		 * if there are no packets from AC...
		 */

		if ((CWGetCountElementFromSafeList(gPacketReceiveList) == 0)) {
			/*
			 * ...wait for a packet.
			 */
			if (!CWErr(CWWaitThreadConditionTimeout(&gInterfaceWait, &gInterfaceMutex, &timenow))) {
			
				if (CWErrorGetLastErrorCode() == CW_ERROR_TIME_EXPIRED && CWGetCountElementFromSafeList(gPacketReceiveList) == 0)	{
					CWLog("No Message from AC for a long time... restart Discovery State");
					printf("No Message from AC for a long time... restart Discovery State\n");
					CWThreadMutexUnlock(&gInterfaceMutex);
					break;
				}
				CWThreadMutexUnlock(&gInterfaceMutex);
				continue;
			}

		}
//printf("(%s) waked up to handle incoming message\n", __FUNCTION__);
		CWDebugLog("(%s) waked up to handle incoming message", __FUNCTION__);
	
		bReceivePacket = ((CWGetCountElementFromSafeList(gPacketReceiveList) != 0) ? CW_TRUE : CW_FALSE);

		CWThreadMutexUnlock(&gInterfaceMutex);

		if (bReceivePacket) {
//printf("(%s) with packet\n", __FUNCTION__);

			CWProtocolMessage msg, msg_tmp;
			CWControlHeaderValues controlVal;

			msg.msg = NULL;
			msg.offset = 0;

			if (!(CWReceiveMessage(&msg))) {
				CW_FREE_PROTOCOL_MESSAGE(msg);
				CWLog("Failure Receiving Response");
				return CW_ENTER_RESET;
			}

			msg_tmp = msg;
			msg_tmp.offset = 0;
			
			if(!(CWParseControlHeader(&msg_tmp, &controlVal))) goto err_check;
	
			CWLog("Got Message From AC:%d", controlVal.messageTypeValue);
			if (controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST) {
				CWErrorRaise(CW_ERROR_INVALID_FORMAT, NULL);
				goto err_check;
			}

			
			int len = controlVal.msgElemsLen - CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;


			//.................
			CWProtocolResultCode resultCode = CW_PROTOCOL_SUCCESS;
			int thisSeqNum = controlVal.seqNum;
			CWBool needSave;
			CWProtocolMessage *messages = NULL;
			int fragmentsNum=0;
			
			if (!cwRtkWtpGetTempFwId()) {
				CWLog("Error: Got Image Data Request, but no reboot firware specified from previous Image Data Response");
				return CW_FALSE;
			}

			// handle seqNum, if not in-sequence, just ignore this message
			//int diff = thisSeqNum - gLastSeqNum;
			//if (diff < 0) diff += 256;
			if (gImageDataStarted && thisSeqNum==gLastSeqNum) {
			//if (gImageDataStarted && (diff < 1 || diff > 100)) {	
				CWLog("Error: Got Image Data Request seqNum=%d, gLastSeqNum=%d. So, ignore!", thisSeqNum, gLastSeqNum);
				needSave = CW_FALSE;
				CWErrorRaise(CW_ERROR_INVALID_FORMAT, NULL);
				goto err_check;
			} else {
				gImageDataStarted = CW_TRUE;
				needSave = CW_TRUE;
			}
			

			// Parse Image Data Request
			if(!CWParseImageDataRequestByWTP(msg_tmp.msg + msg_tmp.offset, len, needSave, cwRtkWtpSaveFirmwareDataPiece, &resultCode)) {
				CWLog("Error: Image Data Parse Error");
				goto err_check;
			}

			gLastSeqNum = thisSeqNum;
			if (gLastSeqNum == 255) gLastSeqNum = -1;
			CWLog("Image Data Request received");

			// assemble image Data Response
			if(!CWAssembleImageDataResponseByWTP(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, resultCode)) {
				goto err_check;
			}

			// send Image Data Response
			{
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
						goto err;
					}
				}
	
				CWLog("Message Sent\n");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
	
			}

			goto ok;

err_check:
			if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) {
			
				/* Log and ignore message */
				CWErrorHandleLast();
				CWLog("--> Received something different from a valid message of current state");
			} else {
err:
				CW_FREE_PROTOCOL_MESSAGE(msg);
				CWLog("--> Critical Error Managing Image Data Message... we enter RESET State");
				return CW_ENTER_RESET;
			}
ok:	
			CW_FREE_PROTOCOL_MESSAGE(msg);

//printf("(%s) before if gImageDataEof\n", __FUNCTION__);
			if (gImageDataEof) break;	// no more packet, and goto reset
//printf("(%s) after if gImageDataEof (did not break\n", __FUNCTION__);

		}

		/*  Babylon:
		 *		since packet receving thread is dead, 
		 *		should wake up packet handling thread, and should make it go to reset state.
		 */
		 
//printf("(%s) before if gotoreset\n", __FUNCTION__);
		if (gGotoReset) {			
			CWLog("WTP Should goto RESET (and then Discovery) State");
			break;
		}
	}//CW_REPEAT_FOREVER

//printf("(%s) out of forever loop\n", __FUNCTION__);

	if (!cwRtkWtpCheckFirmware()) {
		CWLog("WTP Can't be reset since no valid firmware is available.(Requested firmware is not a valid firmware.)");		
		return CW_ENTER_RESET;
	}
	cwRtkWtpFirmwareUpgradeReboot();
	gWillReboot = CW_TRUE;
	return CW_ENTER_RESET;
}


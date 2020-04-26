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

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

/*_______________________________________________________________*/
/*  *******************___CHECK FUNCTIONS___*******************  */
#ifdef KM_BY_AC
CWBool CWWTPCheckForBindingFrame()
{
	//	
	CWLockSafeList(gFrameList);
	
	while (CWGetCountElementFromSafeList(gFrameList) > 0)
	{
		CWBindingDataListElement* dataFirstElem = CWRemoveHeadElementFromSafeList(gFrameList, NULL);
		if (dataFirstElem)
		{
			int k;
			int fragmentsNum = 0;
			CWProtocolMessage *completeMsgPtr = NULL;
			
			if (!CWAssembleDataMessage(&completeMsgPtr, 
						   &fragmentsNum, 
						   gWTPPathMTU, 
						   dataFirstElem->frame, 
						   dataFirstElem->bindingValues,
 			       			   CW_PACKET_PLAIN
						   ,0))			  
			{	
				for(k = 0; k < fragmentsNum; k++)
				{
					CW_FREE_PROTOCOL_MESSAGE(completeMsgPtr[k]);
				}
				
				CW_FREE_OBJECT(completeMsgPtr);
				CW_FREE_PROTOCOL_MESSAGE(*(dataFirstElem->frame));
				CW_FREE_OBJECT(dataFirstElem->frame);
				CW_FREE_OBJECT(dataFirstElem->bindingValues);
				CW_FREE_OBJECT(dataFirstElem);
				continue;
			}
								
			for (k = 0; k < fragmentsNum; k++) 
			{
				if (!CWNetworkSendUnsafeConnected(gWTPDataSocket, completeMsgPtr[k].msg, completeMsgPtr[k].offset)) {
					CWDebugLog("Failure sending Request");
					break;
				}
				CWDebugLog("Sending binding Request to AC");
			}
			
			for (k = 0; k < fragmentsNum; k++)
			{
				CW_FREE_PROTOCOL_MESSAGE(completeMsgPtr[k]);
			}
			
			CW_FREE_OBJECT(completeMsgPtr);				
			CW_FREE_PROTOCOL_MESSAGE(*(dataFirstElem->frame));
			CW_FREE_OBJECT(dataFirstElem->frame);
			CW_FREE_OBJECT(dataFirstElem->bindingValues);
			CW_FREE_OBJECT(dataFirstElem);
		}	
	}

	CWUnlockSafeList(gFrameList);	
	
	return CW_TRUE;
}
#endif
#if 0
CWBool CWWTPCheckForWTPEventRequest(){

	CWLog("\n");
	CWLog("#________ WTP Event Request Message (Run) ________#");
	
	/* Send WTP Event Request */
	CWList msgElemList = NULL;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum = 0;
	int seqNum;
	int *pendingReqIndex;
		
	seqNum = CWGetSeqNum();

	CW_CREATE_OBJECT_ERR(pendingReqIndex, int, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CW_CREATE_OBJECT_ERR(msgElemList, CWListElement, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_OBJECT_ERR(msgElemList->data, CWMsgElemData, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	msgElemList->next= NULL;
	//Change type and value to change the msg elem to send
	((CWMsgElemData*)(msgElemList->data))->type = CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE;
	((CWMsgElemData*)(msgElemList->data))->value = 0;

	if(!CWAssembleWTPEventRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, msgElemList)){
		int i;
		if(messages)
			for(i = 0; i < fragmentsNum; i++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[i]);
			}	
		CW_FREE_OBJECT(messages);
		return CW_FALSE;
	}
	
	*pendingReqIndex = CWSendPendingRequestMessage(gPendingRequestMsgs,messages,fragmentsNum);
	if (*pendingReqIndex<0) {
		CWDebugLog("Failure sending WTP Event Request");
		int k;
		for(k = 0; k < fragmentsNum; k++) {
			CW_FREE_PROTOCOL_MESSAGE(messages[k]);
		}
		CW_FREE_OBJECT(messages);
		CWDeleteList(&msgElemList, CWProtocolDestroyMsgElemData);
		return CW_FALSE;
	}
	CWUpdatePendingMsgBox(&(gPendingRequestMsgs[*pendingReqIndex]),
			      CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE,
			      seqNum,
			      gCWRetransmitTimer,
			      pendingReqIndex,
			      CWWTPRetransmitTimerExpiredHandler,
			      0,
			      messages,
			      fragmentsNum);
	
	CWDeleteList(&msgElemList, CWProtocolDestroyMsgElemData);

	return CW_TRUE;
}
#endif

#ifdef RTK_SMART_ROAMING
CWBool CWWTPCheckForWTPEventRequest(CWList msgElemList, int len)
{

	CWLog("\n");
	CWLog("#________ WTP Event Request Message (Run) ________#");
	
	/* Send WTP Event Request */
	CWProtocolMessage *messages = NULL;
	int fragmentsNum = 0;
	int seqNum;
	int *pendingReqIndex;
		
	seqNum = CWGetSeqNum();

	if(!CWAssembleWTPEventRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, msgElemList, len)){
		int i;
		if(messages)
			for(i = 0; i < fragmentsNum; i++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[i]);
			}	
		CW_FREE_OBJECT(messages);
		return CW_FALSE;
	}
		
	CW_CREATE_OBJECT_ERR(pendingReqIndex, int, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
    CWThreadMutexLock(&gPendingRequestMutex);
    *pendingReqIndex = CWFindFreePendingMsgBox(gPendingRequestMsgs, MAX_PENDING_REQUEST_MSGS);        
	if (*pendingReqIndex < 0) {        
        CWThreadMutexUnlock(&gPendingRequestMutex);
		CWDebugLog("Failure finding free pending msg box");
		int k;
		for(k = 0; k < fragmentsNum; k++) {
			CW_FREE_PROTOCOL_MESSAGE(messages[k]);
		}
		CW_FREE_OBJECT(messages);
		CW_FREE_OBJECT(pendingReqIndex);
		return CW_FALSE;
	}

    if(!CWUpdatePendingMsgBox(&(gPendingRequestMsgs[*pendingReqIndex]),
								CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE,
								seqNum,
								gCWRetransmitTimer,
								pendingReqIndex,
								&CWWTPRetransmitTimerExpiredHandler,
								0,
								messages,
								fragmentsNum))
	{
		goto fail;
    }

	if (!CWSendPendingRequestMessage(messages,fragmentsNum))
	{
		CWDebugLog("Failure sending WTP Event Request");
        goto fail;
	}
	CWThreadMutexUnlock(&gPendingRequestMutex);
	CWDebugLog("wlan info sent to ac, add pendingReqIndex: %d, seqNum: %d", *pendingReqIndex, seqNum);
	return CW_TRUE;
fail:
	
	CWResetPendingMsgBox(&(gPendingRequestMsgs[*pendingReqIndex]));
	CW_FREE_OBJECT(pendingReqIndex);
	CWThreadMutexUnlock(&gPendingRequestMutex);
	return CW_FALSE;


}
#endif

void CWWTPRetransmitTimerExpiredHandler(CWTimerArg hdl_arg)
{
	CWThreadMutexLock(&gPendingRequestMutex);
	if(hdl_arg == NULL){
		CWDebugLog("%s: Invalid hdl_arg", __FUNCTION__);
		goto error;
	}

	int index = *((int *)hdl_arg);
	if(index > MAX_PENDING_REQUEST_MSGS){
		CWDebugLog("%s: Invalid hdl_arg (index=%d)", __FUNCTION__, index);
		goto error;
	}

	CWDebugLog("pendingRequest %d Retransmit Timer Expired", index);

	if(gPendingRequestMsgs[index].msgType != UNUSED_MSG_TYPE) {
		if(gPendingRequestMsgs[index].retransmission == gCWMaxRetransmit) {
			CWDebugLog("\tgCWMaxRetransmit Met to Stop Retransmission");
			goto reset;
	}

	if(!CWSendPendingRequestMessage(gPendingRequestMsgs[index].msgElems,gPendingRequestMsgs[index].fragmentsNum)) {
			CWDebugLog("\tFailure re-sending Request");
			goto reset;
	}	

	gPendingRequestMsgs[index].retransmission++;
	    CWDebugLog("\tRetransmission Count increases to %d", gPendingRequestMsgs[index].retransmission);

		gPendingRequestMsgs[index].timer = timer_add(0, 
							   gPendingRequestMsgs[index].timer_usec, 
						   gPendingRequestMsgs[index].timer_hdl,
						   gPendingRequestMsgs[index].timer_arg);
	}
	CWThreadMutexUnlock(&gPendingRequestMutex);
	return;

reset:
	CWResetPendingMsgBox(&(gPendingRequestMsgs[index]));
error:	
	CWThreadMutexUnlock(&gPendingRequestMutex);
	return;
}


/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	       *
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

/* 
 * CW_FREE_WTP_MSG_ARRAY - free the array of the messages
 * to be sent relative to the WTP with the specified index.
 *	
 * ref -> BUG ML12
 * 20/10/2009 - Donato Capitella
 */
static void inline CW_FREE_WTP_MSG_ARRAY(int WTPIndex) {
        int i;
        for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++) {
                CW_FREE_OBJECT(gWTPs[WTPIndex].messages[i].msg);
        }
        CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
        gWTPs[WTPIndex].messagesCount = 0;
}

CWBool CWACSendFragments(int WTPIndex) {
	int i;

	// Babylon TODO: maby we should do lock here.
	//		Since both timer thread and the wtpmanager thread call this function, 
	//		gWTPs[WTPIndex].messages could be freed.
	
	if(gWTPs[WTPIndex].messages == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++) {
#ifdef CW_NO_DTLS
		if(!CWNetworkSendUnsafeUnconnected(	gWTPs[WTPIndex].socket, 
							&gWTPs[WTPIndex].address, 
							gWTPs[WTPIndex].messages[i].msg, 
							gWTPs[WTPIndex].messages[i].offset))
#else
		if(!(CWSecuritySend(gWTPs[WTPIndex].session,
							gWTPs[WTPIndex].messages[i].msg,
							gWTPs[WTPIndex].messages[i].offset)))
#endif
		{
			return CW_FALSE;
		}
	}
	
	/*
         * BUG - ML12
         *
         * 20/10/2009 - Donato Capitella
         */
        CW_FREE_WTP_MSG_ARRAY(WTPIndex);

	CWLog("Message Sent\n");
	
	return CW_TRUE;
}

void CWACRetransmitTimerExpiredHandler(CWPendingTimerArg *hdl_arg)
{
	if(hdl_arg == NULL){
		CWDebugLog("%s: Invalid hdl_arg", __FUNCTION__);
		return;
	}
	
	int WTPIndex = hdl_arg->WTPIndex;
	int pendingIndex = hdl_arg->pendingIndex;
	if(WTPIndex>AC_MAX_WTP_NUM || pendingIndex>MAX_PENDING_REQUEST_MSGS){
		CWDebugLog("%s: Invalid hdl_arg (WTPIndex=%d pendingIndex=%d)", __FUNCTION__, WTPIndex, pendingIndex);
		return;
	}

	CWThreadMutexLock(&gWTPs[WTPIndex].PendingReqLock);
	CWDebugLog("WTP[%d] pendingIndex[%d] rspType=%d SeqNum=%d Retransmit Timer Expired", WTPIndex, pendingIndex,
				gWTPs[WTPIndex].PendingReq[pendingIndex].rspType, gWTPs[WTPIndex].PendingReq[pendingIndex].rspSeqNum);

	if(gWTPs[WTPIndex].PendingReq[pendingIndex].rspType==UNUSED_MSG_TYPE){
		CWDebugLog("This PendingReq is free already");
		goto reset;
	}
	
	if(gWTPs[WTPIndex].PendingReq[pendingIndex].reTxCount == gCWMaxRetransmit) {
		CWDebugLog("gCWMaxRetransmit Met to Stop Retransmission");
		goto reset;
	}

	if(!CWACSendPendingRequest(WTPIndex, gWTPs[WTPIndex].PendingReq[pendingIndex].msg, gWTPs[WTPIndex].PendingReq[pendingIndex].fragNum)){
		CWDebugLog("Re-send Request Error");
		goto reset;
	}
	
	gWTPs[WTPIndex].PendingReq[pendingIndex].reTxCount ++;
	CWDebugLog("Retransmission Count increases to %d", gWTPs[WTPIndex].PendingReq[pendingIndex].reTxCount);
	
	gWTPs[WTPIndex].PendingReq[pendingIndex].TimerID = timer_add(0, 
						   gCWRetransmitTimer, 
						   gWTPs[WTPIndex].PendingReq[pendingIndex].timer_hdl,
						   gWTPs[WTPIndex].PendingReq[pendingIndex].timer_arg);
	CWThreadMutexUnlock(&gWTPs[WTPIndex].PendingReqLock);
	return;						   
						   
reset:	
	CWACResetPendingRequest(&gWTPs[WTPIndex].PendingReq[pendingIndex]);	
	CWThreadMutexUnlock(&gWTPs[WTPIndex].PendingReqLock);
	return;
}

CWBool CWACInitPendingRequest (CWPendingReq *pendingReq)
{
	pendingReq->msg = NULL;
	pendingReq->reTxCount = 0;
	pendingReq->fragNum = 0;
	pendingReq->rspType = UNUSED_MSG_TYPE;
	pendingReq->rspSeqNum = 0;
	pendingReq->TimerID = 0;
	pendingReq->timer_hdl = NULL;
	pendingReq->timer_arg = NULL;

	return CW_TRUE;
}

CWBool CWACResetPendingRequest(CWPendingReq *pendingReq)
{
	if (pendingReq->TimerID > 0)
		timer_rem(pendingReq->TimerID, NULL);
	CW_FREE_OBJECT(pendingReq->timer_arg);
	pendingReq->timer_hdl = NULL;

	int i;
	for(i=0; i<pendingReq->fragNum; i++)
		CW_FREE_PROTOCOL_MESSAGE(pendingReq->msg[i]);	
	CW_FREE_OBJECT(pendingReq->msg);

	pendingReq->reTxCount = 0;
	pendingReq->fragNum = 0;
	pendingReq->rspType = UNUSED_MSG_TYPE;
	pendingReq->rspSeqNum = 0;
	pendingReq->TimerID = 0;

	//CWDebugLog("Pending Msg Box Reset\n");		
	return CW_TRUE;
}

int CWACFindFreePendingIndex(int WTPIndex)
{
	int k;
	for(k=0; k<MAX_PENDING_REQUEST_MSGS; k++)
	{
		if(gWTPs[WTPIndex].PendingReq[k].rspType == UNUSED_MSG_TYPE)
			return k;
	}
	CWDebugLog("Cannot Find Free Pending Index");
	return -1;
}
int CWACFindPendingIndex(CWPendingReq *pendingReq,int msgType,int seqNum)
{
	if(pendingReq == NULL)	return -1;
	
	int k;
	for(k=0; k<MAX_PENDING_REQUEST_MSGS; k++){
		if(pendingReq[k].rspType==msgType && pendingReq[k].rspSeqNum==seqNum)
			return k;
	}
	return -1;
}

CWBool CWACUpdatePendingRequest(CWPendingReq *pendingReq,
										int rspType,
										int rspSeqNum,
										CWPendingTimerArg *timer_arg,
										void (*timer_hdl)(CWPendingTimerArg))
{
		
	if(pendingReq==NULL || pendingReq->msg==NULL || pendingReq->msg[0].msg==NULL){
		return CW_FALSE;
	}

	pendingReq->rspType = rspType;
	pendingReq->rspSeqNum = rspSeqNum;
	pendingReq->timer_arg = timer_arg;
	pendingReq->timer_hdl = timer_hdl;
	pendingReq->TimerID = timer_add(0,gCWRetransmitTimer,pendingReq->timer_hdl,pendingReq->timer_arg);
	if(pendingReq->TimerID == -1){
		CWACResetPendingRequest(pendingReq);
		return CW_FALSE;
	}

//	CWDebugLog("Pending Request Updated");	
 	return CW_TRUE;

}
CWBool CWACSendPendingRequest(int WTPIndex, CWProtocolMessage *message, int fragNum)
{
	if(message == NULL) 	return CW_FALSE;

	int i;
	for(i=0; i<fragNum; i++){
#ifdef CW_NO_DTLS
		if(!CWNetworkSendUnsafeUnconnected(gWTPs[WTPIndex].socket, 
							&gWTPs[WTPIndex].address, 
							message[i].msg, 
							message[i].offset) )
#else
		if(!(CWSecuritySend(gWTPs[WTPIndex].session, 
							message[0].msg,
							message[0].offset)))
#endif
		return CW_FALSE;
	}
//	CWDebugLog("AC Pending Message Sent");
	
	return CW_TRUE;
}

CWBool CWACResendAcknowledgedPacket(int WTPIndex) {
	if(!CWACSendFragments(WTPIndex))
	   return CW_FALSE;
	
	CWThreadSetSignals(SIG_BLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);	
	if(!(CWTimerRequest(CW_RETRANSMIT_INTERVAL_SECOND, 0, &gWTPs[WTPIndex].thread, &(gWTPs[WTPIndex].currentPacketTimer), CW_SOFT_TIMER_EXPIRED_SIGNAL))) {
		return CW_FALSE;
	}
	CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);
	return CW_TRUE;
}


__inline__ CWBool CWACSendAcknowledgedPacket(int WTPIndex, int msgType, int seqNum) {
	gWTPs[WTPIndex].retransmissionCount = 0;
	gWTPs[WTPIndex].isRetransmitting = CW_TRUE;
	gWTPs[WTPIndex].responseType=msgType;
	gWTPs[WTPIndex].responseSeqNum=seqNum;
//	CWDebugLog("~~~~~~seq num in Send: %d~~~~~~", gWTPs[WTPIndex].responseSeqNum);
	return CWACResendAcknowledgedPacket(WTPIndex);
}


void CWACStopRetransmission(int WTPIndex) {
	if(gWTPs[WTPIndex].isRetransmitting) {
		int i;
		CWDebugLog("Stop Retransmission");
		gWTPs[WTPIndex].isRetransmitting = CW_FALSE;
		CWThreadSetSignals(SIG_BLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);		
		if(!CWTimerCancel(&(gWTPs[WTPIndex].currentPacketTimer)))
			{CWDebugLog("Error Cancelling a Timer... possible error!");}
		CWThreadSetSignals(SIG_UNBLOCK, 1, CW_SOFT_TIMER_EXPIRED_SIGNAL);	
		gWTPs[WTPIndex].responseType=UNUSED_MSG_TYPE;
		gWTPs[WTPIndex].responseSeqNum=0;
		
		for(i = 0; i < gWTPs[WTPIndex].messagesCount; i++) {
			CW_FREE_PROTOCOL_MESSAGE(gWTPs[WTPIndex].messages[i]);
		}

		CW_FREE_OBJECT(gWTPs[WTPIndex].messages);
//		CWDebugLog("~~~~~~ End of Stop Retransmission");
	}
}


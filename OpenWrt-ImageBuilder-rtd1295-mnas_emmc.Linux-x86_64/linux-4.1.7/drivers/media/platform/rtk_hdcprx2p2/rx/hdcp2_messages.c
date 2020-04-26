
/**
 * hdcp2 message creation / processing functions.
 *
 * INTEL CONFIDENTIAL
 * Copyright 2010-2011 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Intel Corporation or its
 * suppliers or licensors.  Title to the Material remains with Intel
 * Corporation or its suppliers and licensors.  The Material contains trade
 * secrets and proprietary and confidential information of Intel or its
 * suppliers and licensors.  The Material is protected by worldwide copyright
 * and trade secret laws and treaty provisions. No part of the Material may
 * be used, copied, reproduced, modified, published, uploaded, posted,
 * transmitted, distributed, or disclosed in any way without Intel's prior
 * express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or
 * delivery of the Materials,  either expressly, by implication, inducement,
 * estoppel or otherwise.  Any license under such intellectual property
 * rights must be express and approved by Intel in writing.
 */

/**
 * These functions are designed to pack / unpack data
 * into / from HDCP2 messages. The functions are used
 * to avoid any potential issues with compiler struct packing
 * that could cause inconsistent behavior.
 */

#include <hdcp2_hal.h>
#include <hdcp2_messages.h>
#include <hdcp2_session.h>


H2_RepeaterAuthSendRxIdList_PayLoad g_repeater_auth_rx_payload;
unsigned char bhdcp_repeater_get_state ;
#ifdef TEST_HDCP2_2_RX_DRIVER
/**
 * Unpack rtx into H2_AKEInitPayLoad from the AKE_INIT message.
 *
 * The first byte of the message should be msgId = AKE_INIT.
 * The rest of the message should be the RTX value, RTX_SIZE bytes ( 5 ).
 *
 * If msgId is valid, and the length passed in is correct, RTX will be
 * copied from the message to the H2_AKEInitPayLoad struct.
 *
 * @param[in] pMsg The incoming message pointer.
 * @param[in] length Size of the incoming message.
 * @param[out] payload H2_AKEInitPayload pointer to contain rTx.
 * @return 0 on success.
 * @return Negative number on error.
 */
 

void HdmiRx_save_hdcp2p2_repeater_state(struct H2_RepeaterAuthSendRxIdList_PayLoad *repeater_info)
{
	memcpy(&g_repeater_auth_rx_payload,repeater_info,sizeof(g_repeater_auth_rx_payload));
	bhdcp_repeater_get_state = 1;
	return;
}

int h2MsgAkeInitUnpack(const unsigned char *pMsg, unsigned int length, H2_AKEInitPayLoad *payload)
{
	unsigned char msgId;
	int rc = 0;
	do {
		if ((NULL == pMsg) || (NULL == payload)) {
			rc = -1;
			break;
		}
		msgId = *pMsg;

      /** Check msgId */
		if (msgId != AKE_INIT) {
			H2DBGLOG((ENABLE, "Message received is not AKE_INIT!\r\n"));
			rc = -1;
			break;
		}

      /** Check length */
		if (length < (RTX_SIZE + 1)) {
			H2DBGLOG((ENABLE, "Incoming length is invalid!\r\n"));
			rc = -1;
			break;
		}

      /** Copy data */
		memcpy(payload->rTx, pMsg + 1, RTX_SIZE);
		rc = 0;
	} while (0);
	return rc;
}

/**
 * Pack RX Public Key Certificate for the AKE_SEND_CERT message.
 *
 * This function will create an AKE_SEND_CERT message from the values
 * repeater, and pRxCert.
 *
 * If pLength is passed in, it will be validated and updated. It must be at
 * least RX_CERT_SIZE + 2 bytes.
 *
 * @param[out] pMsg The message pointer. Can be NULL.
 * @param[inout] pLength Pointer to the size of the data pointed to by pMsg.
 * @param[in] payload Data to be stored in the AKESendCert message.
 * @param[in] pRxCert Pointer to public key certificate.
 * @param[in] RxCertSize Size of data pointed to by pRxCert.
 * @return 0 on success.
 * @return 1 if *pLength is updated but no data is copied.
 * @return -1 on error.
 * @return pLength is updated if it is non-NULL.
 */
int h2MsgAkeSendCertPack(unsigned char *pMsg, unsigned int *pLength, const H2_AKESendCertPayLoad *payload)
{
	int rc = 0;

#ifdef HDCP2_2_WIFI
	do {
      /** If pMsg is NULL, return how many bytes are needed for the
       * message in *pLength
       */
		if ((NULL == pMsg) || (NULL == payload)) {
			if (NULL != pLength) {
				*pLength = CERT_RX_SIZE + 2;
				rc = 1;
				break;
			} else {
				H2DBGLOG((ENABLE, "Message is NULL and pLength is NULL!\n"));
				rc = -1;
				break;
			}
		}
		if ((pLength) && (*pLength < (CERT_RX_SIZE + 2))) {
			H2DBGLOG((ENABLE, "Error: Message is not large enough\n"));
			*pLength = CERT_RX_SIZE + 2;
			rc = -1;
			break;
		}
		pMsg[0] = AKE_SEND_CERT;
				/** Set msgId = AKE_SEND_CERT */
		pMsg[1] = payload->repeater;
					 /** Set repeater */
		memcpy(pMsg + 2, payload->Cert_rx, CERT_RX_SIZE);
							/** Copy certificate */
		if (pLength) {
			*pLength = CERT_RX_SIZE + 2;
		}
		rc = 0;
		break;
	} while (0);

#else
	do {
      /** If pMsg is NULL, return how many bytes are needed for the
       * message in *pLength
       */
		if ((NULL == pMsg) || (NULL == payload)) {
			if (NULL != pLength) {
				*pLength = CERT_RX_SIZE + RRX_SIZE + RXCAPS_SIZE + 1;
				rc = 1;
				break;
			} else {
				H2DBGLOG((ENABLE, "Message is NULL and pLength is NULL!\n"));
				rc = -1;
				break;
			}
		}
		#if 0
		if ((pLength) && (*pLength < (CERT_RX_SIZE + RRX_SIZE + RXCAPS_SIZE + 1))) {
			H2DBGLOG((ENABLE, "Error: Message is not large enough\n"));
			*pLength = CERT_RX_SIZE + 2;
			rc = -1;
			break;
		}
		#endif
		pMsg[0] = AKE_SEND_CERT;
				/** Set msgId = AKE_SEND_CERT */

		    /* pMsg[1] = payload->repeater;   */    /** Set repeater */
		memcpy(pMsg + 1, payload->Cert_rx, CERT_RX_SIZE);
							/** Copy certificate */
		memcpy(pMsg + 1 + CERT_RX_SIZE, payload->rrx, RRX_SIZE);
		memcpy(pMsg + 1 + CERT_RX_SIZE + RRX_SIZE, payload->rxcaps, RXCAPS_SIZE);
		if (pLength) {
			*pLength = CERT_RX_SIZE + RRX_SIZE + RXCAPS_SIZE + 1;
		}
		//printk("func=%s,line=%d\n",__FUNCTION__, __LINE__);
		#if ENABLE_DDC_WRITE
	 	drvif_Hdmi_HDCP2_2_Write_Data_to_TX((unsigned int*)pMsg, *pLength);
		#endif
		rc = 0;
		break;
	} while (0);

#endif
	return rc;
}

/**
 * Unpack the AKE_NO_STORED_EKM message.
 *
 * The first byte of the message should be msgId = AKE_NO_STORED_EKM.
 * The rest of the message should be the EKpubKm value, EKPUBKM_SIZE bytes.
 *
 * @param pMsg The incoming message pointer.
 * @param length Size of the incoming message.
 * @param payload Pointer to structure for the data to be copied to.
 * @return 0 on success, -1 on error.
 */
int h2MsgAkeNoStoredEkmUnpack(const unsigned char *pMsg, unsigned int length, H2_AKENoStoredEkmPayLoad *payload)
{
	unsigned char msgId;
	int rc = 0;
	do {
		if ((NULL == pMsg) || (NULL == payload)) {
			rc = -1;
			break;
		}
		msgId = *pMsg;

      /** Check msgId */
		if (msgId != AKE_NO_STORED_EKM) {
			H2DBGLOG((ENABLE, "Message received is not AKE_NO_STORED_EKM!\n"));
			rc = -1;
			break;
		}
#if 0
      /** Check length */
		if (length < (EKPUBKM_SIZE + 1)) {
			H2DBGLOG((ENABLE, "Length is invalid!\r\n"));
			rc = -1;
			break;
		}
#endif
      /** Copy data */
		memcpy(payload->EKpub_Km, pMsg + 1, EKPUBKM_SIZE);
		rc = 0;
	} while (0);
	return rc;
}

/**
 * Unpack the AKE_STORED_EKM message.
 *
 * The first byte of the message should be msgId = AKE_STORED_EKM.
 * The next EKHKM_SIZE bytes should be the EKh_Km value.
 * The rest of the message should be the master key value, M_SIZE bytes.
 *
 * @param[in] pMsg The incoming message pointer.
 * @param[in] length Size of the incoming message.
 * @param[out] payload Pointer to H2_AKEStoredEkmPayLoad struct.
 * @return 0 on success, or -1 on error.
 */
int h2MsgAkeStoredEKmUnpack(const unsigned char *pMsg, unsigned int length, H2_AKEStoredEkmPayLoad *payload)
{
	unsigned char msgId;
	int rc = 0;
	do {
      /** if pMsg or payload are NULL, return error. */
		if ((NULL == pMsg) || (NULL == payload)) {
			rc = -1;
			break;
		}
		msgId = *pMsg;
      /** Check msgId */
		if (msgId != AKE_STORED_EKM) {
			H2DBGLOG((ENABLE, "Message received is not AKE_STORED_EKM!\n"));
			rc = -1;
			break;
		}
#if 0
      /** Check length */
		if (length < (EKHKM_SIZE + M_SIZE + 1)) {
			H2DBGLOG((ENABLE, "Length is invalid!\r\n"));
			rc = -1;
			break;
		}
#endif
      /** Copy data */
		memcpy(payload->EKh_Km, pMsg + 1, EKHKM_SIZE);
		memcpy(payload->m, pMsg + 1 + EKHKM_SIZE, M_SIZE);
		rc = 0;
	} while (0);
	return rc;
}

/**
 * Pack Rrx for the AKE_Send_rrx message.
 *
 * This function will create an AKE_SEND_RRX message from the values stored
 * in the payload struct.
 *
 * If pLength is passed in, it will be validated and updated. It must be at
 * least RRX_SIZE + 1 bytes.
 *
 * @param[out] pMsg The message pointer. Can be NULL.
 * @param[inout] pLength Pointer to the size of the data pointed to by pMsg.
 * @param[in] payload Data to be stored in the AKE_Send_rrx message.
 * @return 0 on success.
 * @return 1 if size is returned in *pLength.
 * @return -1 on error.
 */
int h2MsgAkeSendRrxPack(unsigned char *pMsg, unsigned int *pLength, const H2_AKESendrrxPayLoad *payload)
{
	int rc = 0;
	do {

      /** If pMsg is NULL, return how many bytes are needed for the
       * message in *pLength
       */
		if ((NULL == pMsg) || (NULL == payload)) {
			if (NULL != pLength) {
				*pLength = RRX_SIZE + 1;
				rc = 0;
				break;
			} else {
				H2DBGLOG((ENABLE, "Message is NULL and pLength is NULL!\n"));
				rc = -1;
				break;
			}
		}
		if ((pLength) && (*pLength < (RRX_SIZE + 1))) {
			H2DBGLOG((ENABLE, "Error: Message is not large enough\n"));
			*pLength = RRX_SIZE + 1;
			rc = 1;
			break;
		}
		pMsg[0] = AKE_SEND_RRX;
		memcpy(pMsg + 1, payload->rrx, RRX_SIZE);
						/** Copy rrx */
		if (pLength) {
			*pLength = RRX_SIZE + 1;
		}
		rc = 0;
		break;
	} while (0);
	return rc;
}

/**
 * Pack Hprime for the AKE_Send_H_prime message.
 *
 * This function will create an AKE_SEND_H_PRIME message from the values stored
 * in the payload struct.
 *
 * If pLength is passed in, it will be validated and updated. It must be at
 * least H_SIZE + 1 bytes.
 *
 * @param[out] pMsg The message pointer. Can be NULL.
 * @param[inout] pLength Pointer to the size of the data pointed to by pMsg.
 * @param[in] payload Data to be stored in the AKE_Send_H_prime message.
 * @return 0 on success.
 * @return 1 if size is returned in *pLength.
 * @return -1 on error.
 */

extern unsigned char g_bmsg_id;
#define AKE_NO_Stored_Km  		4
extern void Clear_Hdmi_hdcp_2_2_Read_Status(void);
extern char Get_Hdmi_hdcp_2_2_Read_Message(void);

int h2MsgAkeSendHprimePack(unsigned char *pMsg, unsigned int *pLength, const H2_AKESendHprimePayLoad *payload)
{
	int rc = 0;
	do {

      /** If pMsg is NULL, return how many bytes are needed for the
       * message in *pLength
       */
	if ((NULL == pMsg) || (NULL == payload)) {
			if (NULL != pLength) {
				*pLength = H_SIZE + 1;
				rc = 0;
				break;
			} else {
				H2DBGLOG((ENABLE, "Message is NULL and pLength is NULL!\n"));
				rc = -1;
				break;
			}
		}
	#if 0
		if ((pLength) && (*pLength < (H_SIZE + 1))) {
			H2DBGLOG((ENABLE, "Error: Message is not large enough\n"));
			*pLength = H_SIZE + 1;
			rc = 1;
			break;
		}
	#endif
		pMsg[0] = AKE_SEND_H_PRIME;
		memcpy(pMsg + 1, payload->Hprime, H_SIZE);
						 /** Copy rrx */
		if (pLength) {
			*pLength = H_SIZE + 1;
		}
	#if ENABLE_DDC_WRITE
	#if 1
	if(g_bmsg_id ==  AKE_NO_Stored_Km)
		Clear_Hdmi_hdcp_2_2_Read_Status();
	 pr_debug("Flag =%x  \n",(Get_Hdmi_hdcp_2_2_Read_Message()));
	#endif

	 drvif_Hdmi_HDCP2_2_Write_Data_to_TX((unsigned int*)pMsg, *pLength);

	  #endif
		rc = 0;
		break;
	} while (0);
	return rc;
}

/**
 * Pack EKhKm for the AKE_Send_PairingInfo message.
 *
 * This function will create an AKE_Send_Pairing_Info message from the values
 * stored in the payload struct.
 *
 * If pLength is passed in, it will be validated and updated. It must be at
 * least EKHKM_SIZE + 1 bytes.
 *
 * @param[out] pMsg The message pointer. Can be NULL.
 * @param[inout] pLength Pointer to the size of the data pointed to by pMsg.
 * @param[in] payload Data to be stored in the AKE_Send_PairingInfo message.
 * @return 0 on success.
 * @return 1 if size is returned in *pLength.
 * @return -1 on error.
 */
int h2MsgAkeSendPairingInfoPack(unsigned char *pMsg, unsigned int *pLength, const H2_AKESendPairingInfoPayLoad *payload)
{
	int rc = 0;
	unsigned char count = 0;
	do {

      /** If pMsg is NULL, return how many bytes are needed for the
       * message in *pLength
       */
		if ((NULL == pMsg) || (NULL == payload)) {
				if (NULL != pLength) {
					*pLength = EKHKM_SIZE + 1;
					rc = 0;
					break;
				} else {
					H2DBGLOG((ENABLE, "Message is NULL and pLength is NULL!\n"));
					rc = -1;
					break;
				}
		}
	#if 0
		if ((pLength) && (*pLength < (EKHKM_SIZE + 1))) {
			H2DBGLOG((ENABLE, "Error: Message is not large enough\n"));
			*pLength = EKHKM_SIZE + 1;
			rc = 1;
			break;
		}
	#endif
		pMsg[0] = AKE_SEND_PAIRING_INFO;
		memcpy(pMsg + 1, payload->Ekh_Km, EKHKM_SIZE);
						     /** Copy rrx */
		if (pLength) {
			*pLength = EKHKM_SIZE + 1;
		}
	#if ENABLE_DDC_WRITE
#if 1
//wait h prime TX read finish
pr_debug("wait hprime start !!!!!!!!");

	while(count < 10)
	{
	    count++;
	  //printk("wait hprime  count =%x !!!!!!!!",count);
	  usleep_range(10000, 11000);//10ms
		if(Get_Hdmi_hdcp_2_2_Read_Message())
			break;

	}
pr_debug("wait hprime finish count =%x !!!!!!!!",count);
#endif
	drvif_Hdmi_HDCP2_2_Write_Data_to_TX((unsigned int*)pMsg, *pLength);
	#endif
		rc = 0;
		break;
	} while (0);
	return rc;
}

/**
 * Unpack the LC_INIT message.
 *
 * The first byte of the message should be msgId = LC_INIT.
 * The remaining RN_SIZE bytes should be the Rn value.
 *
 * @param[in] pMsg The incoming message pointer.
 * @param[in] length Size of the incoming message.
 * @param[out] payload Pointer to H2_LCInitPayLoad struct.
 * @return 0 on success, or -1 on error.
 */
int h2MsgLcInitUnpack(const unsigned char *pMsg, unsigned int length, H2_LCInitPayLoad *payload)
{
	unsigned char msgId;
	int rc = 0;

	do {

      /** if pMsg or payload are NULL, return error. */
		if ((NULL == pMsg) || (NULL == payload)) {
			rc = -1;
			break;
		}
		msgId = *pMsg;

      /** Check msgId */
		 if (msgId != LC_INIT) {
			H2DBGLOG((ENABLE, "Message received is not LC_INIT!\n"));
			rc = -1;
			break;
		}
#if 0
      /** Check length */
		if (length < (RN_SIZE + 1)) {
			H2DBGLOG((ENABLE, "Length is invalid!\r\n"));
			rc = -1;
			break;
		}
#endif
      /** Copy data */
		memcpy(payload->rn, pMsg + 1, RN_SIZE);
		rc = 0;
	} while (0);
	return rc;
}

/**
 * Pack Lprime for the SKE_SEND_Lprime message.
 *
 * This function will create an SKE_SEND_LPRIME message from the values stored
 * in the payload struct.
 *
 * If pLength is passed in, it will be validated and updated. It must be at
 * least L_SIZE + 1 bytes.
 *
 * @param[out] pMsg The message pointer. Can be NULL.
 * @param[inout] pLength Pointer to the size of the data pointed to by pMsg.
 * @param[in] payload Data to be stored in the LCSendLprime message.
 * @return 0 on success.
 * @return 1 if size is returned in *pLength.
 * @return -1 on error.
 */
int h2MsgLcSendLprimePack(unsigned char *pMsg, unsigned int *pLength, const H2_LCSendLprimePayLoad *payload)
{
	int rc = 0;
	do {

      /** If pMsg is NULL, return how many bytes are needed for the
       * message in *pLength
       */
		if ((NULL == pMsg) || (NULL == payload)) {
			if (NULL != pLength) {
				*pLength = L_SIZE + 1;
				rc = 0;
				break;
			} else {
				H2DBGLOG((ENABLE, "Message is NULL and pLength is NULL!\n"));
				rc = -1;
				break;
			}
		}
		#if 0
		if ((pLength) && (*pLength < (L_SIZE + 1))) {
			H2DBGLOG((ENABLE, "Error: Message is not large enough\n"));
			*pLength = L_SIZE + 1;
			rc = 1;
			break;
		}
		#endif
		pMsg[0] = LC_SEND_L_PRIME;
				  /** Set msgId = LC_SEND_L_PRIME */
		memcpy(pMsg + 1, payload->Lprime, L_SIZE);
						 /** Copy Lprime */
		if (pLength) {
			*pLength = L_SIZE + 1;
		}
		#if ENABLE_DDC_WRITE
	drvif_Hdmi_HDCP2_2_Write_Data_to_TX((unsigned int*)pMsg, *pLength);
	#endif
		rc = 0;
		break;
	} while (0);
	return rc;
}

/**
 * Unpack the SKE_SEND_EKS message.
 *
 * The first byte of the message should be msgId = SKE_SEND_EKS.
 * The next EDKEYKS_SIZE bytes should be Edkey_Ks.
 * The remaining RIV_SIZE bytes should be the riv value.
 *
 * @param[in] pMsg The incoming message pointer.
 * @param[in] length Size of the incoming message.
 * @param[out] payload Pointer to H2_SKESendEksPayload struct.
 * @return 0 on success, or -1 on error.
 */
int h2MsgSkeSendEksUnpack(const unsigned char *pMsg, unsigned int length, H2_SKESendEksPayLoad *payload)
{
	unsigned char msgId;
	int rc = 0;
	do {

      /** if pMsg or payload are NULL, return error. */
		if ((NULL == pMsg) || (NULL == payload)) {
			rc = -1;
			break;
		}
		msgId = *pMsg;

      /** Check msgId */
		if (msgId != SKE_SEND_EKS) {
			H2DBGLOG((ENABLE, "Message received is not SKE_SEND_EKS!\n"));
			rc = -1;
			break;
		}
#if 0
      /** Check length */
		if (length < (EDKEYKS_SIZE + RIV_SIZE + 1)) {
			H2DBGLOG((ENABLE, "Length is invalid!\r\n"));
			rc = -1;
			break;
		}
#endif
      /** Copy data */
		memcpy(payload->Edkey_Ks, pMsg + 1, EDKEYKS_SIZE);
		memcpy(payload->riv, pMsg + EDKEYKS_SIZE + 1, RIV_SIZE);
		rc = 0;
	} while (0);
	return rc;
}

int h2MsgRepeaterAuthStreamReadyPack(unsigned char *pMsg, unsigned int *pLength,
					      const H2_RepeaterAuthStreamReady_PayLoad *payload)
{
int rc = 0;
	unsigned int ulSize = 0;
	unsigned int ii;
	do {

      /** If pMsg is NULL, return how many bytes are needed for the
       * message in *pLength
       */
		if (NULL != payload) {
			#if 0
			if ((payload->maxDevsExceeded) || (payload->maxCascadeExceeded)) {
				ulSize = 3;
			} else {
				ulSize = REPEATERAUTHLIST_BASE_SIZE + payload->deviceCount * RECEIVERID_SIZE;
			}
			#else
			ulSize = Mprime_SIZE+1;
			*pLength = ulSize ;
			#endif
		}
		if (NULL == payload) {

	 /** Need payload to determine message size */
			rc = -1;
			break;
		}
		if (NULL == pMsg) {
			if (NULL != pLength) {
				*pLength = ulSize;
				rc = 0;
				break;
			} else {

	    /** Need pLength or pMsg */
				rc = -1;
				break;
			}
		}

		if ((pLength) && (*pLength < ulSize)) {
			H2DBGLOG((ENABLE, "Error: Message is not large enough\n"));
			*pLength = ulSize;
			rc = 1;
			break;
		}
		//pr_err("TTTTTTTTTTTTTT4444444444444444444\n ");
		pMsg[0] = REPEATERAUTH_STREAM_READY;

		memcpy(pMsg + 1, &(payload->MPrime[0]), Mprime_SIZE);  //?????????? need check
						 /** Copy Lprime */

		if (pLength) {
			*pLength = ulSize;
		}
		#if ENABLE_DDC_WRITE  //if data prepare ready
			drvif_Hdmi_HDCP2_2_Write_Data_to_TX(pMsg, *pLength);
		#endif
		rc = 0;
		break;
	} while (0);
	return rc;

}

extern unsigned char stream_manage_flag;
int h2MsgRepeaterAuthSendReceiverIdListPack(unsigned char *pMsg, unsigned int *pLength,
					      const H2_RepeaterAuthSendRxIdList_PayLoad *payload,
					      const H2uint8 *rcvrIds)
{
	int rc = 0;
	unsigned int ulSize = 0;
	unsigned int ii;
	do {

      /** If pMsg is NULL, return how many bytes are needed for the
       * message in *pLength
       */
		if (NULL != payload) {
			#if 0
			if ((payload->maxDevsExceeded) || (payload->maxCascadeExceeded)) {
				ulSize = 3;
			} else {
				ulSize = REPEATERAUTHLIST_BASE_SIZE + payload->deviceCount * RECEIVERID_SIZE;
			}
			#else
			ulSize = 1+2+3+16+(5*payload->deviceCount);
			*pLength = ulSize ;
			#endif
		}
		if (NULL == payload) {

	 /** Need payload to determine message size */
			rc = -1;
			break;
		}
		if (NULL == pMsg) {
			if (NULL != pLength) {
				*pLength = ulSize;
				rc = 0;
				break;
			} else {

	    /** Need pLength or pMsg */
				rc = -1;
				break;
			}
		}
		if (NULL == rcvrIds) {
			if ((0 == payload->maxDevsExceeded) && (0 == payload->maxCascadeExceeded)) {
	    /** Receiver ID list cant be null
		* if cascade or devs are not exceeded.*/
				rc = -1;
				break;
			}
		}
		if ((pLength) && (*pLength < ulSize)) {
			H2DBGLOG((ENABLE, "Error: Message is not large enough\n"));
			*pLength = ulSize;
			rc = 1;
			break;
		}
		pMsg[0] = REP_AUTH_SEND_RXID_LIST;
		#if 1
              //remap to Rxinfo 
              //big endian
		pMsg[1] =payload->Rxinfo[0];
		pMsg[2] =payload->Rxinfo[1] ; //Rxinfor[7:0]
		#else
		pMsg[1] = payload->maxDevsExceeded;
		pMsg[2] = payload->maxCascadeExceeded;
		#endif
		if ((payload->maxDevsExceeded) || (payload->maxCascadeExceeded)) {
			ulSize = 3;
			rc = 0;
			//break;
		}
		#if 1
		pMsg[3] = payload->Seq_num[2]; //
		pMsg[4] = payload->Seq_num[1];
		pMsg[5] = payload->Seq_num[0];
		#else
		pMsg[3] = payload->deviceCount;
		pMsg[4] = payload->depth;
		#endif
		memcpy(pMsg + 6, &(payload->VPrime[0]), (V_SIZE/2));  //?????????? need check
						 /** Copy Lprime */
		for (ii = 0; ii < payload->deviceCount; ii++) {
			memcpy(pMsg + 6 + (V_SIZE/2) + ii * RECEIVERID_SIZE,
				rcvrIds + ii * RECEIVERID_SIZE,
				RECEIVERID_SIZE);
		}
		if (pLength) {
			*pLength = ulSize;
		}
		#if ENABLE_DDC_WRITE  //if data prepare ready
		if (stream_manage_flag) {
			usleep_range(100000, 101000);
			stream_manage_flag = 0;
		}
		drvif_Hdmi_HDCP2_2_Write_Data_to_TX(pMsg, *pLength);
		Set_Rx_status_Ready();
		#endif
		rc = 0;
		break;
	} while (0);
	return rc;
}

int h2MsgRepeaterAuthStraeamManageUnpack(const unsigned char *pMsg, unsigned int length, H2_RepeaterAuthStreamManage_PayLoad *payload)
{
	unsigned char msgId;
	int rc = 0;
	do {

      /** if pMsg or payload are NULL, return error. */
		if ((NULL == pMsg) || (NULL == payload)) {
			rc = -1;
			break;
		}
		msgId = *pMsg;

      /** Check msgId */
		if (msgId != REPEATERAUTH_STREAM_MANAGE) {
			H2DBGLOG((ENABLE, "Message received is not SKE_SEND_EKS!\n"));
			rc = -1;
			break;
		}
#if 0
      /** Check length */
		if (length < (EDKEYKS_SIZE + RIV_SIZE + 1)) {
			H2DBGLOG((ENABLE, "Length is invalid!\r\n"));
			rc = -1;
			break;
		}
#endif
      /** Copy data */
		memcpy(payload->seq_num_M, pMsg + 1, seq_num_M_SIZE);
		memcpy(payload->Manage_k, pMsg + seq_num_M_SIZE + 1, Manage_k_SIZE);
		memcpy(payload->Stream_Type, pMsg + seq_num_M_SIZE +Manage_k_SIZE+ 1, Stream_Type_SIZE);
		rc = 0;
	} while (0);

	return rc;
}

#endif

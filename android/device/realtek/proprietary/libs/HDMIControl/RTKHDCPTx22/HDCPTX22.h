/*
 * HDCPTX22.h
 *
 *  Created on: 2014/7/31
 *      Author: bruce_huang
 *  Descriptions:
 *  	A simple class to control HDCP 2.2 TX nego protocol (over HDMI interface).
 *  	one may need to convert this class into a HAL structure for android porting.
 */

#ifndef HDCPTX22_H_
#define HDCPTX22_H_

#include <sys/time.h>

#include "types.h"
#include "HDCP_KEY_SIZE.h"
#include "HDCP_TXRX_CMD.h"
#include "HDCP_ERROR.h"

#define	MSG_BUF_SIZE	64
/** transmitter version **/
#define	HDCP_TRANSMITTER_INFO_VERSION		0x2
#define MAX_DOWNSTREAM_RECEIVERINUM			32
#define MAX_SRM_GENERATION_LEVEL			1024	// 1024 level is enough for SRM I think..

#define IS_NEED_REAUTH(x)	( x == HDCP_ERR_READ_RECEIVERID_LIST_ERROR_NEED_REAUTH ||	\
			x == HDCP_ERR_RECEIVERID_LIST_MSG_SIZE_ERROR_NEED_REAUTH ||	\
			x == HDCP_ERR_V_COMPARE_ERROR_NEED_REAUTH ||	\
			x == HDCP_ERR_SEQ_NUM_V_ROLL_OVER_ERROR_NEED_REAUTH ||	\
			x == HDCP_ERR_DOWNSTREAM_EXCEED_ERROR_NEED_REAUTH ||	\
			x == HDCP_ERR_REPEATER_SEND_RECEIVERID_LIST_TIMEOUT_NEED_REAUTH || \
			x == HDCP_ERR_INCORRECT_SEQ_NUM_V_NEED_REAUTH )

#define IS_NEED_REAUTH_WITH_MAX_RETRY_COUNT(x)  ( x == HDCP_ERR_READ_CERT_TIMEOUT_NEED_REAUTH || \
			 x == HDCP_ERR_LLC_SIGNATURE_ERROR || \
			 x == HDCP_ERR_READ_H_PRIME_TIMEOUT_NEED_REAUTH || \
			 x == HDCP_ERR_H_COMPARE_ERROR || \
			 x == HDCP_ERR_READ_PAIRING_INFO_TIMEOUT_NEED_REAUTH || \
			 x == HDCP_ERR_LC_INIT_RETRY_COUNT_OVERFLOW || \
			 x == HDCP_ERR_L_COMPARE_ERROR || \
			 x == HDCP_ERR_WRITE_FAILED )

// define what kinds of ERROR case can we reset HDMI insteadof disable HDMI display
#define DO_RESET_HDMI(x) (x == HDCP_ERR_READ_CERT_TIMEOUT_NEED_REAUTH \
    || x == HDCP_ERR_READ_H_PRIME_TIMEOUT_NEED_REAUTH \
    || x == HDCP_ERR_READ_PAIRING_INFO_TIMEOUT_NEED_REAUTH \
    || x == HDCP_ERR_IO_BROKEN \
    || x == HDCP_ERR_RESET_HDMI )

namespace hdmi_hidl {

/** HDCP TX state machine **/
typedef enum{
	HDCP_STATE_IDLE,
	HDCP_STATE_SEND_AKE_INIT,
	HDCP_STATE_SEND_NO_STORED_KM,
	HDCP_STATE_SEND_STORED_KM,
	HDCP_STATE_SEND_LC_INIT,
	HDCP_STATE_RESEND_LC_INIT,
	HDCP_STATE_SEND_SKE,
	HDCP_STATE_REPEATER_AUTH_SEND_ACK,
	HDCP_STATE_REPEATER_SEND_STREAM_MANAGE,
	HDCP_STATE_POLLING_RXSTATUS,
    HDCP_STATE_EXIT, // for hw work-around
    HDCP_STATE_POLLING_RXSTATUS_I2C_ERROR,
}HDCP_STATE;

static const char *hdcp_state_str[]={
	STR(HDCP_STATE_IDLE),
	STR(HDCP_STATE_SEND_AKE_INIT),
	STR(HDCP_STATE_SEND_NO_STORED_KM),
	STR(HDCP_STATE_SEND_STORED_KM),
	STR(HDCP_STATE_SEND_LC_INIT),
	STR(HDCP_STATE_RESEND_LC_INIT),
	STR(HDCP_STATE_SEND_SKE),
	STR(HDCP_STATE_REPEATER_AUTH_SEND_ACK),
	STR(HDCP_STATE_REPEATER_SEND_STREAM_MANAGE),
	STR(HDCP_STATE_POLLING_RXSTATUS),
    STR(HDCP_STATE_EXIT),
    STR(HDCP_STATE_POLLING_RXSTATUS_I2C_ERROR)
};

/** define IO callback used by HDCP tx module **/
typedef INT32 (*hdcp_nego_read)(INT8 *buf, INT32 size);
typedef INT32 (*hdcp_nego_write)(INT8 *buf, INT32 size);
typedef INT32 (*hdcp_nego_can_read)(INT32 size);
typedef void (*hdcp_store_rx_info)(UINT8 *rID, INT32 rIDSize, UINT8 *km, INT32 kmSize, UINT8 *ekhkm, INT32 ekhkmSize, UINT8* m, INT32 mSize);
typedef INT32 (*hdcp_find_km_ekhkm)(UINT8 *rID, INT32 rIDSize, UINT8 *km, INT32 kmSize, UINT8 *ekhkm, INT32 ekhkmSize, UINT8* m, INT32 mSize);
typedef HDCP_RST (*hdcp_read_rx_status)(INT8 *reauth, INT8 *ready, INT32 *msg_size);
// called when Send_SKE is complete, deliver riv & ks to upper call module and enable HDCP encryption.
typedef void (*hdcp_auth_complete_handler)(UINT8 *riv, UINT32 riv_size, UINT8 *ks, UINT32 ks_size);
// re-auth happened, a callback for upper module to do something, maybe disable encryption or something.
typedef void (*hdcp_reauth_handler)();
// To get file path of SRM
typedef const INT8 * (*hdcp_get_SRM_filepath)();

// notify outside to update HDCP 2.2 state
typedef void (*hdcp_update_callback)(INT32 state);

// Repeater downstream control:
typedef struct{
	UINT8 receiverId[HDCP_RECEIVER_ID_SIZE];
}RepeaterReceiverID;

/**
 * A simple structure to record "one" receiver ID in revocation list
 */
typedef struct{
	UINT8 receiverId[HDCP_RECEIVER_ID_SIZE];
}RevocatedReceiverID;

/**
 * A simple structure to record receiver IDs of "one" generation SRM
 * HDCP engine would allocate the memory to store these information.
 * and memory should be released when HDCP engine is deleted.
 */
typedef struct{
	UINT32 length;
	UINT32 numsOfDevices;
	// free in deletion
	RevocatedReceiverID *pIDs;
}SRMLevelData;

/**
 * A simple structure to record device SRM info,
 * up to 1024 level of SRM could be handled in this structure
 */
typedef struct{
	UINT16 version;
	UINT8 generationNumber;
	UINT16 level;
	SRMLevelData pLevelData[MAX_SRM_GENERATION_LEVEL];
}SRMInfo;

typedef struct{
	UINT8 hdcp1_device_downstream;
	UINT8 hdcp2_0_repeater_downstream;
	UINT8 max_cascade_exceeded;
	UINT8 max_devs_exceeded;
	UINT8 device_count;
	UINT8 depth;
	RepeaterReceiverID downStreamReceivers[MAX_DOWNSTREAM_RECEIVERINUM];
}RepeaterRxInfo;

class HDCPTx22
{
public:
	HDCPTx22();
	~HDCPTx22();

public:
	// function pointer, provided by Application
	hdcp_nego_read read_fun;
	hdcp_nego_write write_fun;
	hdcp_nego_can_read check_read_fun;
	hdcp_store_rx_info store_rx_info_fun;
	hdcp_find_km_ekhkm find_km_ekhkm_fun;
	hdcp_read_rx_status read_rx_status_fun;
	hdcp_auth_complete_handler auth_complete_fun;
	hdcp_reauth_handler reauth_handler_fun;
	hdcp_get_SRM_filepath get_srm_filepath_fun;
    hdcp_update_callback update_callback_fun;

	// public APIs of HDCPTx engine
	HDCP_ERR start_hdcp_nego();
	void stop_hdcp_polling_rx_status();

        void check_auth_status();
	void stop_hdcp_nego();

	// public API for SRM analysis
	bool analysis_SRM_raw();
	void ta_reset();
            HDCP_RST dec_hdcptx_key();

	// TODO: public API to allow upper module to deliver content stream management any time
	// refine later, for now, HDCP engine would only deliver one Content Stream Management
	// message to Rx side.
private:
	// internal state control, Tx parameter config
	HDCP_STATE mState;
	UINT8 mTxVersion;
	UINT32 mLCInitRetryCount;
    UINT32 mReauthReqCnt;           // for HDCP HW bug work-around

	// Rx parameter config
	UINT8 mRepeater;
	UINT8 mRxVersion;

	INT8 mKeepPollingRxStatus;
	INT8 mStopHDCPNego;
	INT8 mReauthRetryCount;

        INT8 mWaitAuthComplete;
        INT8 mNeedRepeaterAuthContent;
	HDCP_ERR mErrCode;

	// HDCP meta data
	UINT8 mRtx[HDCP_RTX_SIZE];
	UINT8 mRrx[HDCP_RRX_SIZE];
	UINT8 mKm[HDCP_KM_SIZE];
	UINT8 mSeed[HDCP_SEED_SIZE];
	UINT8 mDKey0[HDCP_DKEY_SIZE];
	UINT8 mDKey1[HDCP_DKEY_SIZE];
	UINT8 mDKey2[HDCP_DKEY_SIZE];
	UINT8 mKD[HDCP_KD_SIZE];
	UINT8 mH[HDCP_H_SIZE];
	UINT8 mRn[HDCP_RN_SIZE];
	UINT8 mL[HDCP_L_SIZE];
	UINT8 mKs[HDCP_KS_SIZE];
	UINT8 mRIV[HDCP_RIV_SIZE];
	UINT8 mEKS[HDCP_EKS_SIZE];
	UINT8 mEKhKm[HDCP_EKH_KM_SIZE];
	// meta data for AKE_Stored_Km
	UINT8 mStoredKm[HDCP_KM_SIZE];
	UINT8 mStoredEkhKm[HDCP_EKH_KM_SIZE];
	UINT8 mStoredm[HDCP_M_SIZE];
	// meta data for Repeater
	UINT8 mRxInfo[HDCP_RXINFO_SIZE];
	UINT8 mVPrime[HDCP_V_SIZE];
	UINT8 mV[HDCP_V_OUT_SIZE];
	UINT8 mM[HDCP_M_PRIME_SIZE];
	RepeaterRxInfo mRepeaterRxInfo;
	INT32 mSeqNumV;
	// Repeater content stream management
	UINT8 *mPStreamIDType;
	UINT16 mk;
	INT32 mSeqNumM;
	UINT8 mSeqNumMBuf[HDCP_SEQ_NUM_M_SIZE];

	// cipher related buffer
	UINT8 mKsXORLc128[HDCP_KS_SIZE];
	UINT8 mRIVCipher[HDCP_RIV_SIZE];

	// useful fields in Receiver Public Cert
	UINT8 mReceiverID[HDCP_RECEIVER_ID_SIZE];
	UINT8 mReceiverPublicKey[HDCP_RECEIVER_PUBLIC_KEY_SIZE];
	UINT8 mProtocolDescriptor;
	UINT8 mLLCSignature[HDCP_RECEIVER_LLC_SIGNATURE_SIZE];

	// SRM related control:
	SRMInfo mSRMInfo;

	// outgoing payload data structure
	AKE_Init_Msg ake_init_msg;
	AKE_No_Stored_km_Msg ake_no_stored_km_msg;
	AKE_Stored_km_Msg ake_stored_km_msg;
	Lc_Init_Msg lc_init_msg;
	SKE_Send_Eks_Msg ske_send_eks_msg;
	RepeaterAuth_Send_Ack_Msg repeaterAuth_send_ack_msg;
	RepeaterAuth_Stream_Manage_Msg_Heading repeaterAuth_stream_manage_msg_h;

	// incoming payload data structure
	AKE_Send_Cert_Msg ake_send_cert_msg;
	AKE_Send_H_Prime_Msg ake_send_h_prime_msg;
	AKE_Send_Pairing_Info_Msg ake_send_pairing_info_msg;
	Lc_Send_L_Prime_No_Precompute_Msg lc_send_l_prime_no_precompute_msg;
	RepeaterAuth_Send_ReceiverID_List_Msg_Heading repeaterAuth_send_receiverId_msg_h;
	RepeaterAuth_Send_ReceiverID_List_Msg_Payload repeaterAuth_send_receiverId_msg_p;
	RepeaterAuth_Stream_Ready_Msg repeaterAuth_stream_ready_msg;

	// reset internal control variables when a new transition is going to start.
	void init();

	// basic I/O used inside HDCP engine
	int ta_hdcp_compute_h();
	HDCP_ERR hdcp_safe_write(INT8 *buf, INT32 size, INT32 *rst);
	HDCP_ERR hdcp_safe_read(INT8 *buf, INT32 size, INT32 *rst);
	HDCP_ERR hdcp_safe_read_with_timeout(INT8 *buf, INT32 size, INT32 *rst, INT32 ms, UINT32* time_consume);

	// HDCP send message functions
	HDCP_ERR send_ake_init();
        HDCP_ERR send_ake_init_ta();
	HDCP_ERR send_ake_nostored_km();
        HDCP_ERR send_ake_nostored_km_ta();
	HDCP_ERR send_ake_stored_km();
	HDCP_ERR send_lc_init();
        HDCP_ERR send_lc_init_ta();
	HDCP_ERR send_ske();
        HDCP_ERR send_ske_ta();
	HDCP_ERR send_repeaterAuth_Ack();
	HDCP_ERR send_repeater_auth_content_stream_manage();

	// HDCP read message functions
	HDCP_ERR read_ake_send_cert(UINT32 time_out,UINT32 *time_consumed);
	HDCP_ERR read_ake_send_h_prime(UINT32 time_out, UINT32 *time_consumed);
	HDCP_ERR read_ake_send_pairing_info(UINT32 time_out, UINT32 *time_consumed);
	HDCP_ERR read_lc_send_l_prime_no_precompute(UINT32 time_out, UINT32 *time_consumed);
	// repeater commands
	HDCP_ERR read_repeater_auth_receiverId_list(UINT32 time_out, UINT32 *time_consumed);
	HDCP_ERR read_repeater_auth_content_stream_ready(UINT32 time_out, UINT32 *time_consumed);

	HDCP_ERR hdcp_send_msg();
	HDCP_ERR hdcp_read_msg();

	// HDCP helper functions
	HDCP_RST hdcp_check_stored_km();
	HDCP_RST hdcp_check_stored_km_ta();
          HDCP_RST decPrivPKeyData(unsigned char *in, unsigned char *out, unsigned long len);
          
	void hdcp_generate_random_rtx();
	void hdcp_generate_random_km();
	void hdcp_generate_random_seed();
	void hdcp_generate_random_rn();
	void hdcp_generate_random_ks();
	void hdcp_generate_random_riv();

	// HDCP basic service utility functions
	void hdcp_compute_h();
	void hdcp_compute_h_hdmi();

	void hdcp_compute_kd();
	void hdcp_compute_dkey(INT8 i);
	void hdcp_compute_l_no_precomputation();
        int hdcp_compute_l_no_precomputation_ta();
	void hdcp_save_pairing_info();
        void hdcp_save_pairing_info_ta();

	// HDCP repeater service utility functions
	HDCP_ERR hdcp_process_receiverId_list(UINT8 *buf, INT32 size);

	// timeout control related
	UINT32 hdcp_get_duration(struct timeval *start, struct timeval* end);

	// debug helper functions
	INT8 msg_buf[MSG_BUF_SIZE];
	void dump_buffer(INT8 *buf, INT32 size, INT8 *description);
	void compare_buffers(INT8 *buf1, INT8 *buf2, INT32 size, INT8 *description);

	// verification functions
	HDCP_RST check_LLC_signature();
    	// verification functions
	HDCP_RST check_LLC_signature_ta();
	// random number generator
	void generate_random_number(UINT8 *dst, INT32 size, const char *msg);
	// RxStatus monitor routine.
	void polling_rx_status();
	// check SRM signature correctness
	HDCP_RST check_SRM_signature(UINT8 *pLLCSignature, UINT8 *in, UINT32 inLen);
	bool check_is_revocation_rId(UINT8 *rId);
};

}; // namespace hdmi_hidl;

#endif /* HDCPTX22_H_ */

#ifndef HDCP2_MESSAGES_H
#define HDCP2_MESSAGES_H

// Message Ids from HDCP Interface Independent Adaptation Specification 
// Revision 2.0.

#include "../../HDCP2X_VSN.h"
int sigmahdcpver2();

typedef enum 
{
   NULL_MSG=1, 
   AKE_INIT=2, 
   AKE_SEND_CERT=3,
   AKE_NO_STORED_EKM=4, 
   AKE_STORED_EKM=5, 
   AKE_SEND_RRX=6,
   AKE_SEND_H_PRIME=7, 
   AKE_SEND_PAIRING_INFO=8, 
   LC_INIT=9, 
   LC_SEND_L_PRIME=10, 
   SKE_SEND_EKS=11, 
   REP_AUTH_SEND_RXID_LIST=12,
   RTT_READY = 13,
   RTT_CHALLENGE = 14,
   REP_AUTH_SEND_ACK = 15,
   REP_AUTH_STREAM_MANAGE = 16,
   REP_AUTH_STREAM_READY = 17,
   RECEIVER_AUTH_STATUS = 18,
   AKE_TRANSMITTER_INFO=19,
   AKE_RECEIVER_INFO = 20
} HDCPMsgId_t;

/**
 * Defines from the HDCP2 Interface Independent Adaptation Spec, Rev 2.0
 */
#define RTX_SIZE (64/8)
#define CERT_RX_SIZE (4176/8)	// 522 bytes
#define EKPUBKM_SIZE (1024/8)
#define EKHKM_SIZE (128/8)
#define M_SIZE (128/8)
#define RRX_SIZE (64/8)
#define H_SIZE (256/8)
#define RN_SIZE (64/8)
#define L_SIZE (256/8)
#define EDKEYKS_SIZE (128/8)
#define RIV_SIZE (64/8)
#define V_SIZE (256/8)
#define REP_AUTH_STREAM_MANAGE_SIZE (3+7)
#define MPRIME_SIZE (256/8)
#define MAX_DEPTH (4)
#define MAX_DEVICECOUNT (32)
#define REPEATERAUTHLIST_BASE_SIZE_H21 (26) 
#define REPEATERAUTHLIST_BASE_SIZE_H20 (37)
#define RECEIVERAUTHSTATUS_SIZE (3)
#define RECEIVERID_SIZE (40/8)
#define MAX_PACKED_MSG_SIZE (CERT_RX_SIZE+2)
#define RXID_SIZE (40/8)
#define KPUBRX_SIZE (1048/8)
#define KPUBRX_P    (512/8)
#define KPUBRX_Q    (512/8)
#define KPUBRX_dP   (512/8)
#define KPUBRX_dQ   (512/8)
#define KPUBRX_qInv (512/8)

#define CERT_RESERVED_SIZE (16/8)
#define CERT_SIGN_SIZE (3072/8)
#define LC128_SIZE (128/8)
#define KPRIVRX_SIZE (2560/8)

#define AKE_MSG_SEND_RRX_SIZE (RRX_SIZE+1)
#define AKE_MSG_SEND_HPRIME_SIZE (H_SIZE+1)
#define AKE_MSG_SEND_LPRIME_SIZE (L_SIZE+1)
#define AKE_MSG_SEND_MOST_LPRIME_SIZE ((L_SIZE/2)+1)
#define AKE_TRANSMITTER_LENGTH_SIZE (16/8)
#define AKE_TRANSMITTER_VERSION_SIZE (8/8)
#define AKE_TRANSMITTER_CAPABILITY_MASK_SIZE (16/8)
#define AKE_TRANSMITTER_INFO_PAYLOAD_SIZE AKE_TRANSMITTER_LENGTH_SIZE+AKE_TRANSMITTER_VERSION_SIZE+AKE_TRANSMITTER_CAPABILITY_MASK_SIZE
#define AKE_RECEIVER_INFO_PAYLOAD_SIZE AKE_TRANSMITTER_LENGTH_SIZE+AKE_TRANSMITTER_VERSION_SIZE+AKE_TRANSMITTER_CAPABILITY_MASK_SIZE

// Bruce try to define HDCP 2.2 related data length here
#define	AKE_COMPUTE_H_PRIME_META_BUF_SIZE	(14)	//rtx + 1 + 2 + 1 + 2 refer to HDCP 2.2 spec p.78
#define	DOUBLE_RN_BUF_SIZE					(128/8)

// Added after HDCP 2.2, a global struct to keep some information needed during HDCP nego
typedef struct
{
	unsigned char gPreComputation;			// pre-computation capability of transmitter.
	unsigned char gContentCategorySupport;	// content category support of transmitter.
	unsigned char gSelfPreComputation;		// pre-computation capability of Receiver.
	unsigned char gHDCP;
	unsigned char transmitterVersion;
	unsigned char transmitterCapMask[2];
	unsigned char receiverVersion;
	unsigned char receiverCapMask[2];
	// and more
}RTK_HDCP_STRUCT;

//for hdcp2.1 
//unsigned char gPreComputation;
//unsigned char gHDCP2_1;
#ifdef __cplusplus
extern "C"{
#endif
	void init_rtk_hdcp();
	unsigned char getHDCPCAP();
	void setHDCPCAP(unsigned char val);
	void setPreComputation(unsigned char val);
	void setContentCategorySupport(unsigned char val);
	unsigned char getPreComputation();
	unsigned char getSelfPreComputation();
	void setSelfPreComputation(unsigned char val);
	//void setTransmitterVersion(unsigned char val);
	//unsigned char getTransmitterVersion();
	// avoid to add so many APIs, I decide to expose &rtk_hdcp_str.
	RTK_HDCP_STRUCT *getRTKHDCPSTR();
#ifdef __cplusplus
}
#endif

enum
{
	CAP_HDCP2_0,
	CAP_HDCP2_1,
	CAP_HDCP2_2		// add HDCP 2.2 enum
};
//
//#define IsCapHDCP2_1()	(getHDCPCAP()==CAP_HDCP2_1)
//#define IsCapHDCP2_1WithRepeater() ((getHDCPCAP()==CAP_HDCP2_1)&&(HDCP_REPEATER==1))

#define	IsCapHDCP2Plus()			(getHDCPCAP()!=CAP_HDCP2_0)
//#define	IsCapHDCP2PlusWithRepeater() ((getHDCPCAP()!=CAP_HDCP2_0)&&(HDCP_REPEATER==1))

enum
{
	H2_LC_INIT_NO_PRECOMPUTATION = 0,
	H2_LC_INIT_PRECOMPUTATION
};

typedef struct 
{
   unsigned char rTx[RTX_SIZE]; 
} H2_AKEInitPayLoad;


typedef struct 
{
   unsigned char EKpub_Km[EKPUBKM_SIZE];  

} H2_AKENoStoredEkmPayLoad;

typedef struct
{
   unsigned char EKh_Km[EKHKM_SIZE];  
   unsigned char m[M_SIZE]; 

} H2_AKEStoredEkmPayLoad;


typedef struct 
{
   unsigned char rn[RN_SIZE]; 
} H2_LCInitPayLoad; 

typedef struct
{
   unsigned char Edkey_Ks[EDKEYKS_SIZE]; 
   unsigned char riv[RIV_SIZE];

} H2_SKESendEksPayLoad; 

typedef struct
{
   unsigned char LENGTH[2];
   unsigned char VERSION;
   unsigned char TRANSMITTER_CAPABILITY_MASK[2];
} H2_AkeTransmitterInfoPayLoad;

typedef struct
{
   unsigned char L[L_SIZE/2];
} H2_AkeRTTChallengePayLoad;


// Messages sent from Rx to Tx 

typedef struct
{
   unsigned char repeater ; 
   unsigned char Cert_rx[CERT_RX_SIZE];  

} H2_AKESendCertPayLoad;


typedef struct 
{
   unsigned char rrx[RRX_SIZE];  

} H2_AKESendrrxPayLoad;

typedef struct
{
   unsigned char Hprime[H_SIZE];  

} H2_AKESendHprimePayLoad;

typedef struct
{
   unsigned char Ekh_Km[EKHKM_SIZE];  

} H2_AKESendPairingInfoPayLoad;

typedef struct 
{
   unsigned char Lprime[L_SIZE];  

} H2_LCSendLprimePayLoad;

typedef struct 
{
   unsigned char MostLprime[L_SIZE/2];  

} H2_LCSendMostLprimePayLoad;


#define H2_MAX_DEVICECOUNT 31

typedef struct
{
   unsigned char maxDevsExceeded;	  // true if devcount > 31 
   unsigned char maxCascadeExceeded; // true if depth > 4 
   unsigned char deviceCount;
   unsigned char depth; 
   unsigned char VPrime[V_SIZE];
} H2_RepeaterAuthSendRxIdList_PayLoad;

typedef struct
{
   unsigned char V[V_SIZE/2];
} H2_RepeaterAuthSendACK_PayLoad;

typedef struct
{
   unsigned char seq_num_M[3];
   unsigned char k[2];
   unsigned char streamCtr[4];
   unsigned char ContentStreamID[2];
   unsigned char Type;
   unsigned char streamCtr1[4];
   unsigned char ContentStreamID1[2];
   unsigned char Type1;
} H2_RepeaterAuthStreamManage_PayLoad;

typedef struct
{
   unsigned char MPrime[MPRIME_SIZE];
} H2_RepeaterAuthReady_PayLoad;


typedef struct
{
   unsigned char LENGTH[2];
   unsigned char REAUTH_REQ;		//true if can't send out receiverstatus within 200MS or V != V
} H2_ReceiverAuthStatus_PayLoad;

typedef struct
{
   unsigned char LENGTH[2];
   unsigned char VERSION;
   unsigned char RECEIVER_CAPABILITY_MASK[2];
} H2_AkeReceiverInfoPayLoad;

#endif // HDCP2_MESSAGES_H


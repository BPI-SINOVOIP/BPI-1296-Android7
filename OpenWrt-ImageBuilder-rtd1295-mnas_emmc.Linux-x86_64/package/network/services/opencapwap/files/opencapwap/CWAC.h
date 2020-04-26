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
 *           Daniele De Sanctis (danieledesanctis@gmail.com)									* 
 *	         Antonio Davoli (antonio.davoli@gmail.com)											*
 ************************************************************************************************/

#ifndef __CAPWAP_CWAC_HEADER__
#define __CAPWAP_CWAC_HEADER__

/*_______________________________________________________*/
/*  *******************___INCLUDE___*******************  */
#include "CWCommon.h"
#include "ACMultiHomedSocket.h"
#include "ACProtocol.h"
#include "ACInterface.h"
#include "ACBinding.h"
#include "CWVendorPayloads.h"
#ifdef KM_BY_AC // key management
#include "ACipcHostapd.h"
#endif

#include <ctype.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>
/*______________________________________________________*/
/*  *******************___DEFINE___*******************  */
#ifdef KM_BY_AC
#define CW_MAX_WLANS_PER_AC		40
#endif
#define CW_CRITICAL_TIMER_EXPIRED_SIGNAL	SIGUSR2
#define CW_SOFT_TIMER_EXPIRED_SIGNAL		SIGUSR1
#define AC_LOG_FILE_NAME					"/var/log/ac.log.txt"

#ifdef RTK_SMART_ROAMING
#define SMART_ROAMING_AC_LOG_FILE_NAME		"/var/log/smart_roaming.ac.log.txt"
#define ROAMING_REACHTHRSH_MAXIMUM					6
#define ROAMING_LINKTIME_MINIMUM					180		//seconds
#define ROAMING_SIGNAL_MAXIMUM						100		//TODO
#define ROAMING_RATE_MINIMUM						0		//TODO MCS index
#define ROAMING_CHANNELUTIL_MINIMUM					80		//70% usage, 30%*255=76
#define ROAMING_REFERENCE_DIFFERENCE				10
#define ROAMING_BEST_DURATION						10		//seconds
#define ROAMING_EFFECTIVE_TIME						10		//seconds
#define ROAMING_KICKOFF_MAXIMUM						3
#define ROAMING_KICKOFF_BOUNDTIME					10		//mins, de-auth needs wait for 3min of linktime
#ifdef STA_CONTROL
#define	BANDSTEER_DUALBAND_NOTIFY					7
#else
#define	BANDSTEER_DUALBAND_NOTIFY					0
#endif
/* dualband notify for band steering mechanism
* &1: once dualband client assoc to 2.4G, inform 5G of this device for 1 time
* &2: once dualband client assoc to 5G, inform 5G of all existed devices for 1 time
* &4: once dualband client is detected, inform 5G of all existed deveices for 1 time
*/
#endif

#define MAC_ADDR_LEN		6
#define DEST_ADDR_START		4
#define SOURCE_ADDR_START	10

// for ACInterface
#define INTERFACE_EXTRA_INFO_SIZE	256


/*Definition of socket's path to send data stats */
#define SOCKET_PATH_AC 				"/tmp/af_unix_ac_client"
#define SOCKET_PATH_RECV_AGENT     		"/tmp/monitorclt"



/********************************************************
 * 2009 Updates:                                        *
 * For manage the applications connected to AC we use   *
 * an array of socket. Through this array we can set    *
 * easily the correct answer socket.                    * 
 * isFree and numSocketFree are used for management.    *
 * The mutex array is used for serialize the correct	*
 * write operation by the different wtp thread on the	*
 * relative application socket.							*
 ********************************************************/

#define MAX_APPS_CONNECTED_TO_AC 4

typedef struct {
	CWSocket appSocket[MAX_APPS_CONNECTED_TO_AC];
	CWBool isFree[MAX_APPS_CONNECTED_TO_AC];
	CWThreadMutex socketMutex[MAX_APPS_CONNECTED_TO_AC];
	int numSocketFree;
	CWThreadMutex numSocketFreeMutex;
} applicationsManager;

typedef struct{
	int WTPIndex;
	int pendingIndex;
}CWPendingTimerArg;

typedef struct{
	CWProtocolMessage *msg;
	int reTxCount;
	int fragNum;

	int rspType;
	int rspSeqNum;

	CWTimerID TimerID;
	void (*timer_hdl)(void *);
	CWPendingTimerArg *timer_arg;
	
}CWPendingReq;

applicationsManager appsManager;

/*_____________________________________________________*/
/*  *******************___TYPES___*******************  */

/* ==================================================================
 *	WTP configuration: is an DB: 
 *	1. from RTK flash; or
 *	2. static defined for testing; or
 *	3. specify a config file to use hostapd (key managemenet by AC)
 * ==================================================================
 */
#ifdef KM_BY_AC // key management
typedef struct {
	char name[CW_WTP_NAME_MAX_SIZE];
	uint8_t nRadios;	
	uint8_t nWlans;
	struct {
		uint8_t baseMAC[6];
		char hapd_cfg_file[CW_HAPD_CFG_FILE_NAME_LEN];
	} radios[CW_MAX_RADIOS_PER_WTP];
} AC_DB_WTP_T;
#else

typedef struct {
	char name[CW_WTP_NAME_MAX_SIZE];
	uint8_t nRadios;	
	uint8_t nWlans;
	struct {
		uint8_t baseMAC[6];	
		unsigned char channel;
		rtk_power_scale_t powerScale;
		struct {			
			CWBool enable;
			uint8_t bssid[6];	
			char ssid[CW_MAX_SSID_SIZE+1];
			rtk_wlan_key_type_t keyType;
			rtk_psk_format_t pskFormat;			
			char key[CW_MAX_KEY_SIZE*2+1];
		} wlans[CW_MAX_WLANS_PER_RADIO];
	} radios[CW_MAX_RADIOS_PER_WTP];
	char fwImageIdentifier[CW_IMAGE_IDENTIFIER_MAX_SIZE];
}  rtk_wtp_t, AC_DB_WTP_T;
#endif

/* ==================================================================
 * Struct that describes a WTP from the AC's point of view 
 * ==================================================================
 */

typedef struct {
	CWNetworkLev4Address address;
	CWNetworkLev4Address dataaddress;
	CWThread thread;
#ifndef CW_NO_DTLS
	CWSecuritySession session;
#endif
	CWBool isNotFree;
	CWBool isRequestClose;  // got request to close the thread.
	CWStateTransition currentState;
	int msgTypeErrCounter;		// Babylon: to work-around
	int interfaceIndex;
	CWSocket socket;
	unsigned char buf[CW_BUFFER_SIZE];
	enum  {
		CW_DTLS_HANDSHAKE_IN_PROGRESS,
		CW_WAITING_REQUEST,
		CW_COMPLETED,
	} subState;		
	CWSafeList packetReceiveList; // list of packets from WTP
	CWSafeList activeWorkList; // works that AC does actively
	
	/* depends on the current state: WaitJoin, NeighborDead */
	CWTimerID currentTimer;
	CWTimerID heartbeatTimer; 
	CWList fragmentsList;
	int pathMTU;
	unsigned int echoRequestCounter;
	time_t lastEchoRequestTime;	

	/**** ACInterface ****/
	CWBool interfaceCommandProgress;
	ac_active_work_type_t interfaceCommand;
	uint8_t extra_info_buffer[INTERFACE_EXTRA_INFO_SIZE];
	CWThreadMutex interfaceMutex; // Babylon: to protect packetReceiveList, interfaceCommand, applicationIndex, and so on.
	CWThreadCondition interfaceWait; // Babylon: to wake up the per-WTP-thread. (when packets or commands comming)
	CWThreadCondition interfaceComplete; // Babylon: to wake up the interface-thread (after command is processed) 
	WTPQosValues* qosValues;		
	//== for rtk cfg
	CWProtocolRtkPowerScaleValues *pRtkPowerScale;
	CWProtocolRtkChannelValues *pRtkChannel;
	//== for Update Image
	enum {
		NO_ACTION = 0,
		WAITNG_FOR_CONFIGURATION_UPDATE_RESPONSE_FROM_WTP,
		WATING_FOR_IMAGE_DATA_REQUEST_FROM_WTP,
		TRANSMITING_IMAGE_DATA_TO_WTP,
		WATING_FOR_RESET_RESPONSE_FROM_WTP
	} imageActionState;
	char reqImageIdentifier[CW_IMAGE_IDENTIFIER_MAX_SIZE]; // image version
	int reqImageSize;
	int reqImageDataSent;
	int reqImageFd;
	void *pReqImageData; // set to NULL when close file
	CWBool reqImageLastMsg;

#ifdef RTK_SMART_ROAMING	
	/***** AC Notify *****/
	enum
	{
		NO_NOTIFY = 0,
		FREE_NOTIFY,
		SILENT_NOTIFY,
		UNSILENT_NOTIFY,
		DOT11V_NOTIFY,
		DUALBAND_NOTIFY,
	} notifyCommand[AC_MAX_NOTIFY_NUM];
	union{
		CWProtocolRequestStationValues silent_sta;
		CWProtocolRequestStationValues unsilent_sta;
		CWProtocolRequestStationValues dot11v_sta;
		CWProtocolRequestStationValues free_sta;
		CWProtocolRequestStationValues dual_sta;
	}RoamingRequest[AC_MAX_NOTIFY_NUM];
    
    unsigned char notifyCommandHead;
    unsigned char notifyCommandTail;
	
	CWPendingReq		PendingReq[MAX_PENDING_REQUEST_MSGS];
	CWThreadMutex 		PendingReqLock;
	
	unsigned char		RcvSeqNum[SEQUENCE_ARRAY_SIZE];
#endif

	/********************************************************
	 * 2009 Updates:                                        *
	 * - ofdmValues is a struct for setting the values of   *
	 *   a Configuration Update Request with message        *
	 *   elemente OFDM.                                     *
	 * - applicationIndex is the index of application which *
	 *   request the command to WTP                         *
	 *   (it will used for selecting the correct socket)    *
	 ********************************************************/
	
	OFDMControlValues* ofdmValues;  
	CWProtocolVendorSpecificValues* vendorValues;
	int applicationIndex;
	
	/**** ACInterface ****/	
	
	CWWTPProtocolManager WTPProtocolManager;
	
	/* Retransmission */
	CWProtocolMessage *messages;
 	int messagesCount;
 	int retransmissionCount;
 	CWTimerID currentPacketTimer;
 	CWBool isRetransmitting;
	
	/* expected response */
	int responseType;
	int responseSeqNum;

	/* //Unrecognized message type value
	 * int unrecognizedMsgType;
	 */
/*	Babylon 2014: we have removed TAP..
	// TAP interface name
	char tap_name[IFNAMSIZ];
	int tap_fd;
 */
	
	char RadioMac[6]; 
		// Babylon: This is set but not used.  This value should be the 1st Radio's MAC of the WTP. 
		//               But May also be 0...since in CAPWAP Protocol it is optional to put MAC ADDRESS in CAPWAP HEADER.
		

		
	// WTP config: copied from Database when Join.
	AC_DB_WTP_T wtpConfig;		
} CWWTPManager;	


// ==================================================================
//				To Manage Stations
// ==================================================================
#ifdef RTK_SMART_ROAMING
#define STATION_INFO_TYPE_REFERENCE				1
#define STATION_INFO_TYPE_DOT11K_REFERENCE		2

typedef enum {
	STATION_UNKNOWN,
	STATION_REQUESTING_ADD,
	STATION_REQUESTING_DELETE,
	STATION_OK,
} station_state_t;

typedef enum{
	NO_ROAM,
	SILENT,
	SILENT_ING,		//wtp daemon miss or client ignore SILENT request
	DOT11V,
	DOT11V_ING,		//wtp daemon miss or client ignore DOT11V request
	NEVER,
} roam_status;

typedef struct{
	unsigned char	rssi[AC_MAX_WTP_NUM];
	unsigned char	ch_util[AC_MAX_WTP_NUM];
	unsigned char	dot11k_rcpi[AC_MAX_WTP_NUM];	
	unsigned char	dot11k_cu[AC_MAX_WTP_NUM];
	unsigned int	reference[AC_MAX_WTP_NUM];
}wlan_info;

typedef struct station_s{
	struct station_s *next;

	unsigned char		mac_addr[6];
	unsigned char		dual_band;
	unsigned char		dot11k_flag;	/* 10-k only; 11-k&v */
	unsigned int		wtp_index;
	unsigned char		radio_id;
	unsigned char		data_rate;
	unsigned int		link_time;
 	
	unsigned int		timestamp[AC_MAX_WTP_NUM];			//seconds
	unsigned int		dot11k_timestamp[AC_MAX_WTP_NUM];	//seconds
	wlan_info			wlan[RTK_RADIOS];
	unsigned char		rssi_thrsh_count;
	unsigned char		cu_thrsh_count;
	unsigned int		disassoc_count;
	unsigned int		disassoc_timestamp;

	unsigned int		best_wtp_index;
	unsigned int		best_timestamp;
	unsigned int		dot11k_best_wtp_index;
	unsigned int		dot11k_best_timestamp;

	roam_status			roam_status;
	unsigned int		roam_wtp_index;
	unsigned long		roam_timestamp;

	unsigned int		roam_pass;
	unsigned int		roam_fail;
	unsigned int		roam_own;

	unsigned char		failure_count;
	unsigned int		failure_timestamp;
} station_t;

#define CW_STATION_HASH_BITS 8
#define CW_STATION_HASH_MAX (1<<CW_STATION_HASH_BITS) 
#define CW_STATION_HASH_MASK(x) ((x)&(CW_STATION_HASH_MAX-1))
#define CW_STATION_HASH_FUNC(mac) CW_STATION_HASH_MASK(mac[0]^mac[1]^mac[2]^mac[3]^mac[4]^mac[5])

typedef struct {
	/* all stations are saved here */
	struct {
		CWThreadMutex lock;
		station_t *list_head;
	} hash[CW_STATION_HASH_MAX];
} CWACStations;

typedef struct {
	CWThreadMutex lock;
	int seqNum; // -1 means no request is on-going
	uint8_t sta_mac[6];
	station_state_t sta_state;
} CWACStationReq; 
#endif
/*________________________________________________________________*/
/*  *******************___EXTERN VARIABLES___*******************  */
#ifdef RTK_SMART_ROAMING
typedef struct{
	unsigned char wlan0[6];
	unsigned char wlan1[6];
} wtp_wlan_config;
extern wtp_wlan_config	gWTPsWlan[AC_MAX_WTP_NUM];
extern unsigned char	gWTPsStatus[AC_MAX_WTP_NUM];
extern unsigned int		gWTPsSpeed[AC_MAX_WTP_NUM];
extern unsigned char	gWTPsLoad[AC_MAX_WTP_NUM][RTK_RADIOS];
extern unsigned char	gStationsSignalMin[RTK_RADIOS];
extern unsigned char	gStationsLoadMin[RTK_RADIOS];
extern unsigned char	gStationsSpeedWeight[RTK_RADIOS];
extern unsigned char	gStationsSignalWeight[RTK_RADIOS];
extern unsigned char	gStationsLoadWeight[RTK_RADIOS];
extern unsigned int		gStationsLinktimeMimimum[RTK_RADIOS];
extern CWACStations		gStations;
extern CWACStationReq	gStationReq;
#endif
extern CWWTPManager *gWTPs;  // array size = gMaxWTPs
extern CWThreadMutex gWTPsMutex;
#ifndef CW_NO_DTLS
extern CWSecurityContext gACSecurityContext;
#endif
extern int gACHWVersion;
extern int gACSWVersion;
extern int gActiveStations;
extern int gActiveWTPs;
extern CWThreadMutex gActiveWTPsMutex;
extern int gMaxStations;
extern int gMaxWTPs;
extern CWAuthSecurity gACDescriptorSecurity;
extern int gRMACField;
extern int gWirelessField;
extern int gDTLSPolicy;
extern CWThreadSpecific gIndexSpecific;
extern char *gACName;
extern int gDiscoveryTimer;
extern int gEchoRequestTimer;
extern int gIdleTimeout;
extern CWProtocolNetworkInterface *gInterfaces;
extern int gInterfacesCount;
extern char **gMulticastGroups;
extern int gMulticastGroupsCount;
extern CWMultiHomedSocket gACSocket;
extern WTPQosValues* gDefaultQosValues;
extern int gHostapd_port;
extern char* gHostapd_unix_path;
extern unsigned char WTPRadioInformationType;



/*________________________________________________________________*/

/*  *******************___GLOBAL VARIABLES FOR ATH_MONITOR TEST___*******************  */
typedef struct {
		struct  sockaddr_un servaddr;/* address of Receive Agent */
		struct  sockaddr_un clntaddr;/* address on AC-side */
		int data_stats_sock;
		} UNIX_SOCKS_INFO;

extern UNIX_SOCKS_INFO *UnixSocksArray; // array size = gMaxWTPs

/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */

/* in AC.c */
void CWACInit(void);
#ifdef KM_BY_AC // key management
void CWCreateConnectionWithHostapdAC(void);
#endif
void CWACEnterMainLoop(void);
CWBool CWACSendAcknowledgedPacket(int WTPIndex, int msgType, int seqNum);
CWBool CWACResendAcknowledgedPacket(int WTPIndex);
void CWACStopRetransmission(int WTPIndex);
void CWACDestroy(void);

#ifdef RTK_SMART_ROAMING
CWBool CWStationsInit();
CWBool CWStationFind(const uint8_t *mac, station_t *station);
CWBool CWStationNewNotify(int wtp_index, unsigned char radio_id, const unsigned char *mac);
CWBool CWStationAdd(int wtp_index, CWProtocolAddStationValues *add_station);
CWBool CWStationUpdate(int wtp_index, CWProtocolUpdateStationValues *upd_station);
CWBool CWStationUpdateDot11k(int wlanNum, CWProtocolUpdateStationDot11kValues *upd_station_11k);
#endif
#ifdef KM_BY_AC
CWBool CWStationUpdateState(const uint8_t *mac, station_state_t state);
int CWGetWTPStationList(int wtp_index, station_t **list_head);
void CWFreeWTPStationList(station_t *list_head);
#endif

/* in ACTest.h */
CWBool ACQosTest(int WTPIndex);

/* in ACImageDataState.c */
 CWBool ACEnterImageData(int WTPIndex, CWProtocolMessage *msgPtr, int pktLeftLength, int seqNum);
CWBool ACEnterImageData2ndStage(int WTPIndex, CWProtocolMessage *msgPtr);
CWBool CWSendOneImageDataRequest(int WTPIndex);
CWBool CWParseImageDataRequestByAC(CWProtocolMessage* msgPtr, int len, CWProtocolImageIdentifierValues *image_identifier, CWBool *GotInitiate);
CWBool CWAssembleImageDataResponseByAC(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode, uint32_t fileSize, uint8_t *hash);
CWBool CWAssembleImageDataRequestByAC(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWBool data_type_eof, void *data, int byteToSend);
CWBool CWParseImageDataResponseByAC(CWProtocolMessage* msgPtr, int len, CWProtocolResultCode* resultCode);
 
/* in ACRunState.c */
CWBool CWSaveChangeStateEventRequestMessage(CWProtocolChangeStateEventRequestValues *valuesPtr,
					    CWWTPProtocolManager *WTPProtocolManager);

#ifdef KM_BY_AC
CW_THREAD_RETURN_TYPE CWACipc_with_ac_hostapd(void *arg);
//send_data_to_hostapd(unsigned char *, int);
/****************************************************************
 * 2009 Updates:												*
 *				msgElement is used for differentiation between	*
 *				all message elements							*
 ****************************************************************/

#endif

CWBool CWAssembleConfigurationUpdateRequest(CWProtocolMessage **messagesPtr,
					    int *fragmentsNumPtr,
					    int PMTU,
						int seqNum,
						int msgElement, const void *extraInfo);
								/* 2013 Babyon:
								  * It is legacy to bring only one kind of data in Configuration Update Request. Should modify in the future.
 								  */
#ifdef RTK_SMART_ROAMING
CWBool CWAssembleStationConfigurationRequest(CWProtocolMessage **messagesPtr,
					     int *fragmentsNumPtr,
					     int PMTU, int seqNum,
					     unsigned char* StationMacAddr, int radio_id,
					     int Operation); 
#endif
#ifdef KM_BY_AC
CWBool CWAssembleWLANConfigurationRequest(CWProtocolMessage **messagesPtr, 
						int *fragmentsNumPtr, 
						int PMTU, int seqNum,
						unsigned char* recv_packet,
						int Operation, int len_packet);
#endif
#ifdef RTK_CAPWAP
CWBool CWAssembleRtkWTPStatusRequestSiteSurvey(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum);
CWBool CWAssembleRtkWTPStatusRequestStations(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum);
CWBool CWParseRtkWTPStatusResponse(CWProtocolMessage* msgPtr, int len, CWProtocolRtkWTPStatusResponseValues *wtpStatus);
#endif

#if 0
CWBool CWAssembleWLANConfigurationRequestAddWlan(CWProtocolMessage **messagesPtr, 
					int *fragmentsNumPtr, int PMTU, int seqNum,
					const CWProtocolAddWLANValues *addWlan);
#endif
CWBool CWAssembleClearConfigurationRequest(CWProtocolMessage **messagesPtr,
					   int *fragmentsNumPtr, int PMTU,
					   int seqNum);

/* in ACDiscoveryState.c */
CWBool CWAssembleDiscoveryResponse(CWProtocolMessage **messagesPtr, int seqNum);
CWBool CWParseDiscoveryRequestMessage(unsigned char *msg,
						int len,
						int *seqNumPtr,
						CWDiscoveryRequestValues *valuesPtr);

/* in ACRetransmission.c */
CWBool CWACSendFragments(int WTPIndex);
void CWACRetransmitTimerExpiredHandler(CWPendingTimerArg *hdl_arg);
CWBool CWACInitPendingRequest(CWPendingReq *pendingReq);
CWBool CWACResetPendingRequest(CWPendingReq *pendingReq);
int CWACFindFreePendingIndex(int WTPIndex);
int CWACFindPendingIndex(CWPendingReq *pendingReq,int msgType,int seqNum);
CWBool CWACUpdatePendingRequest(CWPendingReq *pendingReq,
											int rspType,
											int rspSeqNum,
											CWPendingTimerArg *timer_arg,
											void (*timer_hdl)(CWPendingTimerArg));
CWBool CWACSendPendingRequest(int WTPIndex, CWProtocolMessage *message, int fragNum);

/* in ACRunStateCheck.c */
CWBool CWACCheckForConfigurationUpdateRequest(int WTPIndex);

/* in ACJoinState.c */
CWBool CWAcDbWtpLookup(const char *wtp_name, AC_DB_WTP_T *wtp);
CWBool CWAcDbSetPowerScale(const char *wtp_name, int radio_id, rtk_power_scale_t power_scale);
CWBool CWAcDbSetSetChannel(const char *wtp_name, int radio_id, unsigned char channel);
CWBool CWAcDbSetFwId(const char *wtp_name, const char *fwId);

/* in ACProtocol_User.c */
CWBool CWACGetVendorInfos(CWACVendorInfos *valPtr);
int CWACGetRMACField();
int CWACGetWirelessField();
int CWACGetDTLSPolicy();
void CWACDestroyVendorInfos(CWACVendorInfos *valPtr);

/* in ACMainLoop.c */
void CWCloseThread();
CWBool CWIsACRun();

/* in CWSecurity.c */
#ifndef CW_NO_DTLS
CWBool CWSecurityInitSessionServer(CWWTPManager* pWtp,
				   CWSocket sock,
				   CWSecurityContext ctx,
				   CWSecuritySession *sessionPtr,
				   int *PMTUPtr);
#endif

CWBool ACEnterJoin(int WTPIndex, CWProtocolMessage *msgPtr);
CWBool ACEnterPostJoin(int WTPIndex, CWProtocolMessage *msgPtr);
CWBool ACEnterDataCheck(int WTPIndex, CWProtocolMessage *msgPtr);
CWBool ACEnterRun(int WTPIndex, CWProtocolMessage *msgPtr, CWBool dataFlag);

/* in ACInterface.c */
CW_THREAD_RETURN_TYPE CWInterface(void* arg);
CWBool CWResponseToWum(int WTPIndex, int resultCode, int payloadSize, void *payload);
/* void CWTimerExpiredHandler(int arg); */

#endif

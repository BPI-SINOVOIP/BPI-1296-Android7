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

 
#ifndef __CAPWAP_WTPProtocol_HEADER__
#define __CAPWAP_WTPProtocol_HEADER__

/*_____________________________________________________*/
/*  *******************___TYPES___*******************  */
typedef struct {
	int ACIPv4ListCount;
	int *ACIPv4List;	
}ACIPv4ListValues;

typedef struct {
	int ACIPv6ListCount;
	struct in6_addr *ACIPv6List;	
}ACIPv6ListValues;

typedef struct {
	int stations;
	int limit;
	int activeWTPs;
	int maxWTPs;
	CWAuthSecurity security;
	int RMACField;
//	int WirelessField;
	int DTLSPolicy;
	CWACVendorInfos vendorInfos;
	char *name;
	CWProtocolIPv4NetworkInterface *IPv4Addresses; // for WTP: from Discovery response or Join response
	int IPv4AddressesCount;  // for WTP: from Discovery response or Join response
	CWProtocolIPv6NetworkInterface *IPv6Addresses;
	int IPv6AddressesCount;
	ACIPv4ListValues ACIPv4ListInfo;  // for WTP: from configuration status response
	ACIPv6ListValues ACIPv6ListInfo;
	CWNetworkLev4Address preferredAddress;
	CWNetworkLev4Address incomingAddress;
} CWACInfoValues;



typedef struct {	
	uint8_t radio_id;
	uint8_t tagging_policy;
	uint8_t queue_depth;
	WTPQosValues voice;
	WTPQosValues video;
	WTPQosValues best_effrot;
	WTPQosValues back_ground;
} CWProtocolWtpQosValues;

typedef struct {
	CWACInfoValues ACInfoPtr;
	CWProtocolResultCode code;
	ACIPv4ListValues ACIPv4ListInfo;
	ACIPv6ListValues ACIPv6ListInfo;

	CWBool imageIdentifierFound;
	CWProtocolImageIdentifierValues image_identifier;	
} CWProtocolJoinResponseValues;

typedef struct {
	ACIPv4ListValues ACIPv4ListInfo;
	ACIPv6ListValues ACIPv6ListInfo;
	int discoveryTimer;
	int echoRequestTimer;
	int radioOperationalInfoCount;
	CWRadioOperationalInfoValues *radioOperationalInfo;
	WTPDecryptErrorReport radiosDecryptErrorPeriod;
	int idleTimeout;
	int fallback;
	int nWtpQos;
	CWProtocolWtpQosValues wtpQos[CW_MAX_RADIOS_PER_WTP];
#ifndef RTK_CAPWAP
	void * bindingValues;
#endif

#ifdef RTK_CAPWAP
	int nRtkWlancfg;
	CWProtocolRtkWlanConfigValues rtkWlanCfg[CW_MAX_WLANS_PER_WTP];
	int nRtkChannelCfg;
	CWProtocolRtkChannelValues rtkChannelCfg[CW_MAX_RADIOS_PER_WTP];
	int nRtkPowerScaleCfg;	
	CWProtocolRtkPowerScaleValues rtkPowerScaleCfg[CW_MAX_RADIOS_PER_WTP];
#endif

} CWProtocolConfigureResponseValues;

typedef struct {
	void *protocolValues;	
	int nWtpQos;
	CWProtocolWtpQosValues wtpQos[CW_MAX_RADIOS_PER_WTP];

#ifndef RTK_CAPWAP
	void * bindingValues;
	/*Update 2009:
		add new non-binding specific values*/
#endif


	CWProtocolImageIdentifierValues image_identifier;	

#ifdef RTK_CAPWAP
	int nRtkWlanCfg;
	CWProtocolRtkWlanConfigValues rtkWlanCfg[CW_MAX_WLANS_PER_WTP];
	int nRtkPowerScaleCfg;
	CWProtocolRtkPowerScaleValues rtkPowerScaleCfg[CW_MAX_RADIOS_PER_WTP];
	int nRtkChannelCfg;
	CWProtocolRtkChannelValues rtkChannelCfg[CW_MAX_RADIOS_PER_WTP];
#endif
} CWProtocolConfigurationUpdateRequestValues;


typedef struct {
	const char *newFirmwareVersion;
	CWProtocolResultCode resultCode;
	CWBool withImageInformation;
	CWProtocolImageInformationValues imageInformation; 
} CWProtocolImageDataResponseToWTPValues;

typedef struct {
	CWImageDataType data_type;
	void *data;  // When parse "Image Data", this pointer will point into data of the message. No memory allocation will happen.
	uint16_t data_len;
} CWProtocolImageDataValues;

#ifdef RTK_CAPWAP
typedef struct {
	CWBool siteSurveyReq;
	CWBool stationReq;
} CWProtocolRtkWTPStatusRequestValues;

#endif

/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */
CWBool CWAssembleMsgElemACName(CWProtocolMessage *msgPtr);				// 4
CWBool CWAssembleMsgElemACNameWithIndex(CWProtocolMessage *msgPtr);			// 5
CWBool CWAssembleMsgElemDataTransferData(CWProtocolMessage *msgPtr, int data_type);	//13
CWBool CWAssembleMsgElemDiscoveryType(CWProtocolMessage *msgPtr);			//20
CWBool CWAssembleMsgElemDuplicateIPv4Address(CWProtocolMessage *msgPtr);		//21
CWBool CWAssembleMsgElemLocationData(CWProtocolMessage *msgPtr);			//27
CWBool CWAssembleMsgElemStatisticsTimer(CWProtocolMessage *msgPtr);			//33
CWBool CWAssembleMsgElemWTPBoardData(CWProtocolMessage *msgPtr);			//35
CWBool CWAssembleMsgElemWTPDescriptor(CWProtocolMessage *msgPtr);			//36
CWBool CWAssembleMsgElemWTPFrameTunnelMode(CWProtocolMessage *msgPtr);			//38
CWBool CWAssembleMsgElemWTPIPv4Address(CWProtocolMessage *msgPtr);			//39
CWBool CWAssembleMsgElemWTPMACType(CWProtocolMessage *msgPtr);				//40
CWBool CWAssembleMsgElemWTPRadioInformation(CWProtocolMessage *msgPtr);				//1048
#ifdef RTK_SMART_ROAMING
CWBool CWAssembleMsgElemWTPConfiguration(CWProtocolMessage *msgPtr);
#endif
#ifdef KM_BY_AC
CWBool CWAssembleMsgElemSupportedRates(CWProtocolMessage *msgPtr);					//1040
CWBool CWAssembleMsgElemMultiDomainCapability(CWProtocolMessage *msgPtr);					//1032
#else
//TODO
#endif
CWBool CWAssembleMsgElemAssignedWTPBSSID(CWProtocolMessage *msgPtr, int radio_id, int wlan_id, const unsigned char *bssid);// 1026
CWBool CWAssembleMsgElemWTPName(CWProtocolMessage *msgPtr);				//41
CWBool CWAssembleMsgElemWTPOperationalStatistics(CWProtocolMessage *msgPtr,int radio);	//42
CWBool CWAssembleMsgElemWTPRadioStatistics(CWProtocolMessage *msgPtr,int radio);	//43
CWBool CWAssembleMsgElemWTPRebootStatistics(CWProtocolMessage *msgPtr);			//44
//CWBool CWAssembleMsgElemWTPStaticIPInfo(CWProtocolMessage *msgPtr);			//45

//CWBool CWAssembleMsgElemWTPRadioInformation(CWProtocolMessage *msgPtr);	
// Vendor
CWBool CWAssembleMsgElemVendorSpecificPayload(CWProtocolMessage *msgPtr);

#ifdef RTK_CAPWAP
CWBool CWAssembleMsgElemRtkSiteSurvey(CWProtocolMessage *msgPtr, int radio_id, int numBss, const rtk_bss_t *bssInfo);
CWBool CWAssembleMsgElemRtkStation(CWProtocolMessage *msgPtr, const CWRtkStationValues *staInfo);
#endif

//---------------------------------------------------------/
CWBool CWParseMsgElemACDescriptor(CWProtocolMessage *msgPtr, int len, CWACInfoValues *valPtr);					// 1
CWBool CWParseMsgElemACIPv4List(CWProtocolMessage *msgPtr, int len, ACIPv4ListValues *valPtr);					// 2
CWBool CWParseMsgElemACIPv6List(CWProtocolMessage *msgPtr, int len, ACIPv6ListValues *valPtr);					// 3
CWBool CWParseMsgElemCWControlIPv4Addresses(CWProtocolMessage *msgPtr, int len, CWProtocolIPv4NetworkInterface *valPtr);	//10 
CWBool CWParseMsgElemCWControlIPv6Addresses(CWProtocolMessage *msgPtr, int len, CWProtocolIPv6NetworkInterface *valPtr);	//11 
CWBool CWParseMsgElemCWTimers (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr);			//12 
CWBool CWParseMsgElemDecryptErrorReportPeriod (CWProtocolMessage *msgPtr, int len, WTPDecryptErrorReportValues *valPtr);	//16 
CWBool CWParseMsgElemIdleTimeout (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr);		//23 
CWBool CWParseMsgElemWTPFallback (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr);		//37 
CWBool CWParseMsgElemImageData(CWProtocolMessage *msgPtr, int len, CWProtocolImageDataValues *valPtr); // 24
CWBool CWParseMsgElemImageInformation (CWProtocolMessage *msgPtr, int len, CWProtocolImageInformationValues*valPtr); // 26

#if KM_BY_AC
#ifdef SOFTMAC
	CWBool CWParseMsgElemAddWLAN(CWProtocolMessage *msgPtr, int len);	//1024
	CWBool CWParseMsgElemDeleteWLAN(CWProtocolMessage *msgPtr, int len); //1027
#else
	CWProtocolDeleteWLANValues *CWParseMsgElemDeleteWLAN(CWProtocolMessage *msgPtr, int len);

	/* Function: 		CWParseAddWLAN
	  * Description: 	Parse Capwap Message Element - AddWLAN
	  *				This function will allocate memory for the new created CWProtocolAddWLANValues.
	  * return value: 	a non-NULL pointer to a CWProtocolAddWLANValues if this element is valid.
	  * Inputs:		msgPtr, len, result_code
	  * Outputs:		result_code: written if error occurs when parsing.
	  */
	CWProtocolAddWLANValues *CWParseMsgElemAddWLAN(CWProtocolMessage *msgPtr, int len); //1024
#endif
#endif

CWBool CWParseMsgElemWtpQos(CWProtocolMessage *msgPtr, int len, CWProtocolWtpQosValues *valPtr); //1045

//---------------------------------------------------------/
void CWWTPResetRebootStatistics(WTPRebootStatisticsInfo *rebootStatistics);

int CWWTPGetDiscoveryType(void);
int CWWTPGetRadiosInUse(void);
int CWWTPGetEncCapabilities(void);
CWBool CWWTPGetBoardData(CWWTPVendorInfos *valPtr);
CWBool CWWTPGetWtpDesc(CWWTPVendorInfos *valPtr);
int CWWTPGetMACType(void);
char *CWWTPGetLocation(void);
int CWWTPGetSessionID(void);
int CWWTPGetIPv4Address(void);
int CWWTPGetIPv4StatusDuplicate(void);
int CWWTPGetIPv6StatusDuplicate(void);
int CWWTPGetACIndex();
/* 2013 Babylon: use the flash mib 
char* CWWTPGetACName(); 
*/
int CWWTPGetFrameTunnelMode();
CWBool CWGetWTPRadiosOperationalState(int radioID, CWRadiosOperationalInfo *valPtr);
CWBool CWAssembleMsgElemDecryptErrorReport(CWProtocolMessage *msgPtr, int radioID);
CWBool CWAssembleMsgElemDuplicateIPv6Address(CWProtocolMessage *msgPtr);
CWBool CWAssembleMsgElemInitiateDownload(CWProtocolMessage *msgPtr);

//---------------------------------------------------------/
void CWWTPDestroyVendorInfos(CWWTPVendorInfos *valPtr);


#endif



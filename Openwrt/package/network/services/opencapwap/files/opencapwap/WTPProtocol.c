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

 
#include "CWWTP.h"

#ifdef SOFTMAC
#include "WTPipcHostapd.h"
#else
#include "WTPRtkSystemInteraction.h"
#endif

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif


/*____________________________________________________________________________*/
/*  *****************************___ASSEMBLE___*****************************  */
CWBool CWAssembleMsgElemACName(CWProtocolMessage *msgPtr) 
{
	char *name;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	name = CWWTPGetACName();
	CWDebugLog("AC Name: %s", name);	/* JOY check gACInfoPtr */
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, strlen(name), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStoreStr(msgPtr, name);
				
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_AC_NAME_CW_TYPE);
}

CWBool CWAssembleMsgElemACNameWithIndex(CWProtocolMessage *msgPtr) 
{
	const int ac_Index_length=1;
	CWACNamesWithIndex ACsinfo;
	CWProtocolMessage *msgs;
	int len = 0;
	int i;
	int j;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(!CWWTPGetACNameWithIndex(&ACsinfo)){
		return CW_FALSE;
	}
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, ACsinfo.count, return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	// create message
	for(i = 0; i < ACsinfo.count; i++) {
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], ac_Index_length+strlen(ACsinfo.ACNameIndex[i].ACName), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CWProtocolStore8(&(msgs[i]), ACsinfo.ACNameIndex[i].index); // ID of the AC
		CWProtocolStoreStr(&(msgs[i]), ACsinfo.ACNameIndex[i].ACName); // name of the AC
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_AC_NAME_INDEX_CW_TYPE))) {
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(ACsinfo.ACNameIndex);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
//		CWDebugLog("AC Name with index: %d - %s", ACsinfo.ACNameIndex[i].index, ACsinfo.ACNameIndex[i].ACName);
		len += msgs[i].offset;
	}
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < ACsinfo.count; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);

 	/*
         * They free ACNameIndex, which is an array of CWACNameWithIndexValues,
         * but nobody cares to free the actual strings that were allocated as fields
         * of the CWACNameWithIndexValues structures in the CWWTPGetACNameWithIndex() 
         * function.. Here we take care of this.
         *
         * BUG ML06
         * 16/10/2009 - Donato Capitella 
         */
        CW_FREE_OBJECT(ACsinfo.ACNameIndex[0].ACName);
        CW_FREE_OBJECT(ACsinfo.ACNameIndex[1].ACName);


	CW_FREE_OBJECT(ACsinfo.ACNameIndex);
	
	return CW_TRUE;
}

CWBool CWAssembleMsgElemDataTransferData(CWProtocolMessage *msgPtr, int data_type) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	char* debug_data= " #### DATA DEBUG INFO #### ";   //to be changed...
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 2 + strlen(debug_data) , return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	

	CWProtocolStore8(msgPtr, data_type);
	CWProtocolStore8(msgPtr,strlen(debug_data));
	CWProtocolStoreStr(msgPtr, debug_data);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DATA_TRANSFER_DATA_CW_TYPE);
}

CWBool CWAssembleMsgElemDiscoveryType(CWProtocolMessage *msgPtr) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
//	CWDebugLog("Discovery Type: %d", CWWTPGetDiscoveryType());

	CWProtocolStore8(msgPtr, CWWTPGetDiscoveryType());

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DISCOVERY_TYPE_CW_TYPE);
}

CWBool CWAssembleMsgElemLocationData(CWProtocolMessage *msgPtr) {
	char *location;
//printf("%s\n", __FUNCTION__);	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	location = CWWTPGetLocation();
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, strlen(location), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
//	CWDebugLog("Location Data: %s", location);
	CWProtocolStoreStr(msgPtr, location);
					
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_LOCATION_DATA_CW_TYPE);	
}

CWBool CWAssembleMsgElemStatisticsTimer(CWProtocolMessage *msgPtr)
{
	const int statistics_timer_length= 2;
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, statistics_timer_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, CWWTPGetStatisticsTimer());
	
//	CWDebugLog("Statistics Timer: %d", CWWTPGetStatisticsTimer());
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_STATISTICS_TIMER_CW_TYPE);	
}

CWBool CWAssembleMsgElemWTPBoardData(CWProtocolMessage *msgPtr) 
{
	const int VENDOR_ID_LENGTH = 4; 	//Vendor Identifier is 4 bytes long
	const int TLV_HEADER_LENGTH = 4; 	//Type and Length of a TLV field is 4 byte long 
	CWWTPVendorInfos infos;
	int i, size = 0;
	
	//printf("%s\n", __FUNCTION__);	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	 // get infos
	if(!CWWTPGetBoardData(&infos)) {
		return CW_FALSE;
	}
	
	//Calculate msg elem size
	size = VENDOR_ID_LENGTH;
	for(i = 0; i < infos.vendorInfosCount; i++) 
		{size += (TLV_HEADER_LENGTH + ((infos.vendorInfos)[i]).length);}
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(msgPtr, ((infos.vendorInfos)[0].vendorIdentifier));
	for(i = 0; i < infos.vendorInfosCount; i++) 
	{
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].type));
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].length));
		
		if((infos.vendorInfos)[i].length == 4) {
			*((infos.vendorInfos)[i].valuePtr) = htonl(*((infos.vendorInfos)[i].valuePtr));
		}
	
		CWProtocolStoreRawBytes(msgPtr, (unsigned char*) ((infos.vendorInfos)[i].valuePtr), (infos.vendorInfos)[i].length);
		
//		CWDebugLog("Board Data: %d - %d - %d - %d", (infos.vendorInfos)[i].vendorIdentifier, (infos.vendorInfos)[i].type, (infos.vendorInfos)[i].length, *((infos.vendorInfos)[i].valuePtr));
	}

	CWWTPDestroyVendorInfos(&infos);

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_BOARD_DATA_CW_TYPE);
}

CWBool CWAssembleMsgElemVendorSpecificPayload(CWProtocolMessage *msgPtr) {
	const int VENDOR_ID_LENGTH = 4; 	//Vendor Identifier is 4 bytes long
	const int  ELEMENT_ID = 2; 	//Type and Length of a TLV field is 4 byte long 
	const int  DATA_LEN = 2;
	CWWTPVendorInfos infos;
	int size = 0;
	int element_id_zero = 0;
	int data_zero = 0;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	 // get infos
	if(!CWWTPGetBoardData(&infos)) {
		return CW_FALSE;
	}
	
	//Calculate msg elem size
	size = VENDOR_ID_LENGTH + ELEMENT_ID + DATA_LEN ;
	
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(msgPtr, ((infos.vendorInfos)[0].vendorIdentifier));
	CWProtocolStore16(msgPtr, element_id_zero);
	CWProtocolStore16(msgPtr, data_zero);

	CWWTPDestroyVendorInfos(&infos);

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_BW_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPDescriptor(CWProtocolMessage *msgPtr) {
	const int GENERIC_RADIO_INFO_LENGTH = 4;//First 4 bytes for Max Radios, Radios In Use and Encryption Capability 
	const int VENDOR_ID_LENGTH = 4; 	//Vendor Identifier is 4 bytes long
	const int TLV_HEADER_LENGTH = 4; 	//Type and Length of a TLV field is 4 byte long 
	CWWTPVendorInfos infos;
	int i, size = 0;
	//printf("%s\n", __FUNCTION__);	
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// get infos
	if(!CWWTPGetWtpDesc(&infos)) { 
		return CW_FALSE;
	}
	
	//Calculate msg elem size
	size = GENERIC_RADIO_INFO_LENGTH;
	for(i = 0; i < infos.vendorInfosCount; i++) 
		{size += (VENDOR_ID_LENGTH + TLV_HEADER_LENGTH + ((infos.vendorInfos)[i]).length);}
		
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, size, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, cwRtkWtpGetMaxRadios()); // number of radios supported by the WTP
	CWProtocolStore8(msgPtr, CWWTPGetRadiosInUse()); // number of radios present in the WTP

	// encryption capabilities
	CWProtocolStore8(msgPtr, 1);
	CWProtocolStore8(msgPtr, 1);	// IEEE802.11 binding
	CWProtocolStore16(msgPtr, CWWTPGetEncCapabilities());
 
	for(i = 0; i < infos.vendorInfosCount; i++) {
		CWProtocolStore32(msgPtr, ((infos.vendorInfos)[i].vendorIdentifier));
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].type));
		CWProtocolStore16(msgPtr, ((infos.vendorInfos)[i].length));
		
		if((infos.vendorInfos)[i].length == 4) {
			*((infos.vendorInfos)[i].valuePtr) = htonl(*((infos.vendorInfos)[i].valuePtr));
		}
	
		CWProtocolStoreRawBytes(msgPtr, (unsigned char*) ((infos.vendorInfos)[i].valuePtr), (infos.vendorInfos)[i].length);

//		CWDebugLog("WTP Descriptor Vendor ID: %d", (infos.vendorInfos)[i].vendorIdentifier);
//		CWDebugLog("WTP Descriptor Type: %d", (infos.vendorInfos)[i].type);
//		CWDebugLog("WTP Descriptor Length: %d", (infos.vendorInfos)[i].length);
//		CWDebugLog("WTP Descriptor Value: %d", *((infos.vendorInfos)[i].valuePtr));

		//CWDebugLog("Vendor Info \"%d\" = %d - %d - %d", i, (infos.vendorInfos)[i].vendorIdentifier, (infos.vendorInfos)[i].type, (infos.vendorInfos)[i].length);
	}
	
	CWWTPDestroyVendorInfos(&infos);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_DESCRIPTOR_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPFrameTunnelMode(CWProtocolMessage *msgPtr) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	//printf("%s\n", __FUNCTION__);	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
//	CWDebugLog("Frame Tunnel Mode: %d", CWWTPGetFrameTunnelMode());
	CWProtocolStore8(msgPtr, CWWTPGetFrameTunnelMode()); // frame encryption

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_FRAME_TUNNEL_MODE_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPIPv4Address(CWProtocolMessage *msgPtr) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	//printf("%s\n", __FUNCTION__);	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 4, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

//	CWDebugLog("WTP IPv4 Address: %d", CWWTPGetIPv4Address());
	CWProtocolStore32(msgPtr, CWWTPGetIPv4Address());

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_IPV4_ADDRESS_CW_TYPE);
}

CWBool CWAssembleMsgElemWTPMACType(CWProtocolMessage *msgPtr) {
	//printf("%s\n", __FUNCTION__);	

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 1, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
//	CWDebugLog("WTP MAC Type: %d", CWWTPGetMACType());
	CWProtocolStore8(msgPtr, CWWTPGetMACType()); // mode of operation of the WTP (local, split, ...)

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_MAC_TYPE_CW_TYPE);
}
/*
CWBool CWAssembleMsgElemWTPRadioInformation(CWProtocolMessage *msgPtr) {

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 5, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	unsigned char wtp_r_info;
	wtp_r_info = CWTP_get_WTP_Radio_Information();
	int radioID = 0;
	
	
	CWProtocolStore8(msgPtr, radioID); 
	CWProtocolStore8(msgPtr, 0); 
	CWProtocolStore8(msgPtr, 0); 
	CWProtocolStore8(msgPtr, 0); 
	CWProtocolStore8(msgPtr, wtp_r_info); 


	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IEEE80211_WTP_RADIO_INFORMATION_CW_TYPE);
}
*/
#ifdef KM_BY_AC

CWBool CWAssembleMsgElemSupportedRates(CWProtocolMessage *msgPtr) {

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 9, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	unsigned char tmp_sup_rate[8];
	CWWTPGetRates(tmp_sup_rate);
	
	int radioID = 0;
	
	CWProtocolStore8(msgPtr, radioID); 
	
	CWProtocolStore8(msgPtr, tmp_sup_rate[0]);
	CWProtocolStore8(msgPtr, tmp_sup_rate[1]); 
	CWProtocolStore8(msgPtr, tmp_sup_rate[2]); 
	CWProtocolStore8(msgPtr, tmp_sup_rate[3]); 
	CWProtocolStore8(msgPtr, tmp_sup_rate[4]); 
	CWProtocolStore8(msgPtr, tmp_sup_rate[5]); 
	CWProtocolStore8(msgPtr, tmp_sup_rate[6]); 
	CWProtocolStore8(msgPtr, tmp_sup_rate[7]); 

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IEEE80211_SUPPORTED_RATES_CW_TYPE);
}

CWBool CWAssembleMsgElemMultiDomainCapability(CWProtocolMessage *msgPtr) {

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 8, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	unsigned char tmp_mdc[6];
	CWWTPGetMDC(tmp_mdc);
	
	int radioID = 0;
	
	CWProtocolStore8(msgPtr, radioID); 
	CWProtocolStore8(msgPtr, 0); 
	
	CWProtocolStore8(msgPtr, tmp_mdc[0]);
	CWProtocolStore8(msgPtr, tmp_mdc[1]); 
	CWProtocolStore8(msgPtr, tmp_mdc[2]); 
	CWProtocolStore8(msgPtr, tmp_mdc[3]); 
	CWProtocolStore8(msgPtr, tmp_mdc[4]); 
	CWProtocolStore8(msgPtr, tmp_mdc[5]); 

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IEEE80211_MULTI_DOMAIN_CAPABILITY_CW_TYPE);
}
#endif

CWBool CWAssembleMsgElemWTPName(CWProtocolMessage *msgPtr) {
	const char *name;
	//printf("%s\n", __FUNCTION__);	
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	name = cwRtkWtpGetApName();
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, strlen(name), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
//	CWDebugLog("WTPName: %s", name);
	CWProtocolStoreStr(msgPtr, name);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_NAME_CW_TYPE);	
}

CWBool CWAssembleMsgElemWTPOperationalStatistics(CWProtocolMessage *msgPtr, int radio)
{
	const int operational_statistics_length= 4;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(radio<0 || radio>=gRadiosInfo.radioCount) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, operational_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, radio);
	CWProtocolStore8(msgPtr, gRadiosInfo.radiosInfo[radio].TxQueueLevel);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].wirelessLinkFramesPerSec);

//	CWDebugLog("");	
//	CWDebugLog("WTPOperationalStatistics of radio \"%d\": %d - %d", radio,gRadiosInfo.radiosInfo[radio].TxQueueLevel,  gRadiosInfo.radiosInfo[radio].wirelessLinkFramesPerSec);

	CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE);
	
	return CW_TRUE;	
}

CWBool CWAssembleMsgElemWTPRadioStatistics(CWProtocolMessage *msgPtr, int radio)
{
	const int radio_statistics_length= 20;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(radio<0 || radio>gRadiosInfo.radioCount) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, radio_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	CWProtocolStore8(msgPtr, radio);
	CWProtocolStore8(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.lastFailureType);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.resetCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.SWFailureCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.HWFailuireCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.otherFailureCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.unknownFailureCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.configUpdateCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.channelChangeCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.bandChangeCount);
	CWProtocolStore16(msgPtr, gRadiosInfo.radiosInfo[radio].statistics.currentNoiseFloor);

//	CWDebugLog("");
//	CWDebugLog("WTPRadioStatistics of radio: \"%d\"", radio);
//	CWDebugLog("WTPRadioStatistics(1): %d - %d - %d", gRadiosInfo.radiosInfo[radio].statistics.lastFailureType, gRadiosInfo.radiosInfo[radio].statistics.resetCount, gRadiosInfo.radiosInfo[radio].statistics.SWFailureCount);
//	CWDebugLog("WTPRadioStatistics(2): %d - %d - %d", gRadiosInfo.radiosInfo[radio].statistics.HWFailuireCount, gRadiosInfo.radiosInfo[radio].statistics.otherFailureCount, gRadiosInfo.radiosInfo[radio].statistics.unknownFailureCount);
//	CWDebugLog("WTPRadioStatistics(3): %d - %d - %d - %d", gRadiosInfo.radiosInfo[radio].statistics.configUpdateCount, gRadiosInfo.radiosInfo[radio].statistics.channelChangeCount,gRadiosInfo.radiosInfo[radio].statistics.bandChangeCount,gRadiosInfo.radiosInfo[radio].statistics.currentNoiseFloor);

	//return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE);

	CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE);
	
	return CW_TRUE;
}

CWBool CWAssembleMsgElemWTPRebootStatistics(CWProtocolMessage *msgPtr)
{
	const int reboot_statistics_length= 15;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, reboot_statistics_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.rebootCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.ACInitiatedCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.linkFailurerCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.SWFailureCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.HWFailuireCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.otherFailureCount);
	CWProtocolStore16(msgPtr, gWTPRebootStatistics.unknownFailureCount);
	CWProtocolStore8(msgPtr, gWTPRebootStatistics.lastFailureType);

//	CWDebugLog("");	
//	CWDebugLog("WTPRebootStat(1): %d - %d - %d", gWTPRebootStatistics.rebootCount, gWTPRebootStatistics.ACInitiatedCount, gWTPRebootStatistics.linkFailurerCount);
//	CWDebugLog("WTPRebootStat(2): %d - %d - %d", gWTPRebootStatistics.SWFailureCount, gWTPRebootStatistics.HWFailuireCount, gWTPRebootStatistics.otherFailureCount);
//	CWDebugLog("WTPRebootStat(3): %d - %d", gWTPRebootStatistics.unknownFailureCount, gWTPRebootStatistics.lastFailureType);

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE);
}


//test version
CWBool CWAssembleMsgElemDuplicateIPv4Address(CWProtocolMessage *msgPtr) {
	const int duplicate_ipv4_length= 11;
	unsigned char *macAddress;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, duplicate_ipv4_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

//	CWDebugLog("");	
//	CWDebugLog("Duplicate IPv4 Address: %d", CWWTPGetIPv4Address());
	
	CWProtocolStore32(msgPtr, CWWTPGetIPv4Address());

	CWProtocolStore8(msgPtr, CWWTPGetIPv4StatusDuplicate());

	CWProtocolStore8(msgPtr, 6);

	CW_CREATE_ARRAY_ERR(macAddress, 6, unsigned char, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	macAddress[0] = 103;
	macAddress[1] = 204;
	macAddress[2] = 204;
	macAddress[3] = 190;
	macAddress[4] = 180;
	macAddress[5] = 0;
	
	CWProtocolStoreRawBytes(msgPtr, macAddress, 6);
	CW_FREE_OBJECT(macAddress);

	//CWProtocolStore8(msgPtr, CWWTPGetIPv4StatusDuplicate());

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE);
}

//test version
CWBool CWAssembleMsgElemDuplicateIPv6Address(CWProtocolMessage *msgPtr) {
	const int duplicate_ipv6_length= 23;
	unsigned char *macAddress;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, duplicate_ipv6_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

//	CWDebugLog("");	
//	CWDebugLog("Duplicate IPv6 Address");
	
	struct sockaddr_in6 myAddr;
	CWWTPGetIPv6Address(&myAddr);
	CWProtocolStoreRawBytes(msgPtr, myAddr.sin6_addr.s6_addr, 16);

	CWProtocolStore8(msgPtr, CWWTPGetIPv6StatusDuplicate());
	
	CWProtocolStore8(msgPtr, 6);

	CW_CREATE_ARRAY_ERR(macAddress, 6, unsigned char, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	macAddress[0] = 103;
	macAddress[1] = 204;
	macAddress[2] = 204;
	macAddress[3] = 190;
	macAddress[4] = 180;
	macAddress[5] = 0;
	
	CWProtocolStoreRawBytes(msgPtr, macAddress, 6);
	CW_FREE_OBJECT(macAddress);

	//CWProtocolStore8(msgPtr, CWWTPGetIPv6StatusDuplicate());

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE);
}



CWBool CWAssembleMsgElemRadioAdminState(CWProtocolMessage *msgPtr) 
{
	const int radio_Admin_State_Length=2;
	CWRadiosAdminInfo infos;
	CWProtocolMessage *msgs;
	int len = 0;
	int i;
	int j;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(!CWGetWTPRadiosAdminState(&infos)) {
		return CW_FALSE;
	}
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, (infos.radiosCount), return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], radio_Admin_State_Length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].state); // state of the radio
		//CWProtocolStore8(&(msgs[i]), infos.radios[i].cause);
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE))) {
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(infos.radios);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
//		CWDebugLog("Radio Admin State: %d - %d - %d", infos.radios[i].ID, infos.radios[i].state, infos.radios[i].cause);
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);
	CW_FREE_OBJECT(infos.radios);

	return CW_TRUE;
}

//if radioID is negative return Radio Operational State for all radios
CWBool CWAssembleMsgElemRadioOperationalState(int radioID, CWProtocolMessage *msgPtr) 
{
	const int radio_Operational_State_Length=3;
	CWRadiosOperationalInfo infos;
	CWProtocolMessage *msgs;
	int len = 0;
	int i;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(!(CWGetWTPRadiosOperationalState(radioID,&infos))) {
		return CW_FALSE;
	}

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, (infos.radiosCount), return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], radio_Operational_State_Length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].state); // state of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].cause);
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE))) {
			int j;
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(infos.radios);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
//		CWDebugLog("Radio Operational State: %d - %d - %d", infos.radios[i].ID, infos.radios[i].state, infos.radios[i].cause);
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);
	CW_FREE_OBJECT(infos.radios);

	return CW_TRUE;
}

CWBool CWAssembleMsgElemDecryptErrorReport(CWProtocolMessage *msgPtr, int radioID) 
{
	int decrypy_Error_Report_Length=0;
	CWDecryptErrorReportInfo infos;
	CWProtocolMessage *msgs;
	int len = 0;
	int i;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if(!(CWGetDecryptErrorReport(radioID,&infos))) {
		return CW_FALSE;
	}

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgs, (infos.radiosCount), return  CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		decrypy_Error_Report_Length = 2 + sizeof(CWMACAddress)*(infos.radios[i].numEntries); 
		
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], decrypy_Error_Report_Length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore8(&(msgs[i]), infos.radios[i].numEntries); // state of the radio

		CWProtocolStore8(&(msgs[i]), (unsigned char)sizeof(CWMACAddress)*(infos.radios[i].numEntries));

		CWProtocolStoreRawBytes(&(msgs[i]), *(infos.radios[i].decryptErrorMACAddressList), sizeof(CWMACAddress)*(infos.radios[i].numEntries));
		
		/*
		CWDebugLog("###numEntries = %d", infos.radios[i].numEntries);
		CWDebugLog("j = %d", sizeof(CWMACAddress)*(infos.radios[i].numEntries));
		
		int j;
		for (j=(sizeof(CWMACAddress)*(infos.radios[i].numEntries)); j>0; j--)
			CWDebugLog("##(%d/6) = %d", j, msgs[i].msg[(msgs[i].offset)-j]);
		*/
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE))) {
			int j;
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			for(j=0; j<infos.radiosCount; j++) {CW_FREE_OBJECT(infos.radios[j].decryptErrorMACAddressList);}
			CW_FREE_OBJECT(infos.radios);
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
//		CWDebugLog("Radio Decrypt Error Report of radio \"%d\" = %d", infos.radios[i].ID, infos.radios[i].numEntries);
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	for(i = 0; i < infos.radiosCount; i++) {
		CW_FREE_OBJECT(infos.radios[i].decryptErrorMACAddressList);
	}
	
	CW_FREE_OBJECT(msgs);
	CW_FREE_OBJECT(infos.radios);

	return CW_TRUE;
	
}

CWBool CWAssembleMsgElemWTPRadioInformation(CWProtocolMessage *msgPtr) {
	CWProtocolMessage *msgs;
	CWRadiosInformation infos;
	
	int len = 0;
	int i;
	//printf("%s\n", __FUNCTION__);	
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Assemble WTP Radio Info");

#ifdef RTK_CAPWAP
	if(!cwRtkWtpGetRadioInfo(&infos)) {
		return CW_FALSE;
	}
#else
	if(!CWWTPGetRadiosInformation(&infos)) {
		return CW_FALSE;
	}
#endif

	// create one message element for each radio	
	
	CW_CREATE_ARRAY_ERR(msgs, (infos.radiosCount), CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		// create message
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], 5, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CWProtocolStore8(&(msgs[i]), infos.radios[i].ID); // ID of the radio
		CWProtocolStore32(&(msgs[i]), infos.radios[i].type); // type of the radio

		CWDebugLog("WTPRadioInformation: %d - %d", infos.radios[i].ID, infos.radios[i].type);
		
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_IEEE80211_WTP_RADIO_INFORMATION_CW_TYPE))) {
			int j;
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
	}
	
	// return all the messages as one big message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < infos.radiosCount; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);
	
	return CW_TRUE;
}

#ifdef RTK_SMART_ROAMING
CWBool CWAssembleMsgElemWTPConfiguration(CWProtocolMessage *msgPtr) 
{
	CWProtocolMessage *msgs;
	CWWTPWlanConfiguration conf;
	
	int len = 0;
	int i;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Assemble WTP Configuration");

	if(!cwRtkWtpGetConfiguration(&conf)) {
		return CW_FALSE;
	}

	CW_CREATE_ARRAY_ERR(msgs, (conf.wlan_num), CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < conf.wlan_num; i++) {
		CW_CREATE_PROTOCOL_MESSAGE(msgs[i], 10, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CWProtocolStore32(&(msgs[i]), conf.wlan[i].ID); 
		CWProtocolStoreRawBytes(&(msgs[i]), conf.wlan[i].mac_addr, 6);
				
		CWDebugLog("WTPWlanConfiguration: %d - %02x:%02x:%02x:%02x:%02x:%02x",
				conf.wlan[i].ID, 
				conf.wlan[i].mac_addr[0], conf.wlan[i].mac_addr[1],
				conf.wlan[i].mac_addr[2], conf.wlan[i].mac_addr[3],
				conf.wlan[i].mac_addr[4], conf.wlan[i].mac_addr[5]);
		if(!(CWAssembleMsgElem(&(msgs[i]), CW_MSG_ELEMENT_IEEE80211_WTP_CONFIGURATION_CW_TYPE))) {
			int j;
			for(j = i; j >= 0; j--) { CW_FREE_PROTOCOL_MESSAGE(msgs[j]);}
			CW_FREE_OBJECT(msgs);
			return CW_FALSE;
		}
		
		len += msgs[i].offset;
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < conf.wlan_num; i++) {
		CWProtocolStoreMessage(msgPtr, &(msgs[i]));
		CW_FREE_PROTOCOL_MESSAGE(msgs[i]);
	}
	
	CW_FREE_OBJECT(msgs);
	return CW_TRUE;
}
#endif

CWBool CWAssembleMsgElemInitiateDownload(CWProtocolMessage *msgPtr)
{
	msgPtr->offset = 0;
	msgPtr->msg = NULL;
	CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_INITIATED_DOWNLOAD_CW_TYPE);
	return CW_TRUE; 
}

CWBool CWAssembleMsgElemAssignedWTPBSSID(CWProtocolMessage *msgPtr, int radio_id, int wlan_id, const unsigned char *bssid)
{
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 8, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(msgPtr, (unsigned char)radio_id);
	CWProtocolStore8(msgPtr, (unsigned char)wlan_id);
	CWProtocolStoreRawBytes(msgPtr, bssid, 6);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_IEEE80211_ASSINED_WTP_BSSID_CW_TYPE);	
}

#ifdef RTK_CAPWAP
CWBool CWAssembleMsgElemRtkSiteSurvey(CWProtocolMessage *msgPtr, int radio_id, int numBss, const rtk_bss_t *bssInfo)
{
	int i;
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, sizeof(rtk_bss_t)*numBss+2, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(msgPtr, radio_id);
	CWProtocolStore8(msgPtr, numBss);
	for (i=0; i<numBss; i++) {
		CWProtocolStoreRawBytes(msgPtr, bssInfo[i].bssid, 6);
		CWProtocolStoreRawBytes(msgPtr, bssInfo[i].ssid, CW_MAX_SSID_SIZE);
		CWProtocolStore8(msgPtr, bssInfo[i].bss_type);
		CWProtocolStore8(msgPtr, bssInfo[i].channel);
		CWProtocolStore8(msgPtr, bssInfo[i].signal_strength);
		CWProtocolStore8(msgPtr, bssInfo[i].radio_type);
	}
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_RTK_SITE_SURVEY_CW_TYPE);	
}

CWBool CWAssembleMsgElemRtkStation(CWProtocolMessage *msgPtr, const CWRtkStationValues *staInfo)
{
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, sizeof(CWRtkStationValues), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(msgPtr, staInfo->radio_id);
	CWProtocolStore8(msgPtr, staInfo->wlan_id);
	CWProtocolStoreRawBytes(msgPtr, staInfo->mac_addr, 6);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_RTK_STATION_CW_TYPE);	
}
#endif


/*_________________________________________________________________________*/
/*  *****************************___PARSE___*****************************  */
CWBool CWParseMsgElemACDescriptor(CWProtocolMessage *msgPtr, int len, CWACInfoValues *valPtr) {
	int i=0, theOffset=0;
		
	CWParseMessageElementStart();
	
	
	valPtr->stations= CWProtocolRetrieve16(msgPtr);
//	CWDebugLog("AC Descriptor Stations: %d", valPtr->stations);
	
	valPtr->limit	= CWProtocolRetrieve16(msgPtr);
//	CWDebugLog("AC Descriptor Limit: %d", valPtr->limit);
	
	valPtr->activeWTPs= CWProtocolRetrieve16(msgPtr);
//	CWDebugLog("AC Descriptor Active WTPs: %d", valPtr->activeWTPs);
	
	valPtr->maxWTPs	= CWProtocolRetrieve16(msgPtr);
//	CWDebugLog("AC Descriptor Max WTPs: %d",	valPtr->maxWTPs);
	
	valPtr->security= CWProtocolRetrieve8(msgPtr);
//	CWDebugLog("AC Descriptor Security: %d",	valPtr->security);
	
	valPtr->RMACField= CWProtocolRetrieve8(msgPtr);
//	CWDebugLog("AC Descriptor Radio MAC Field: %d",	valPtr->security);

//	valPtr->WirelessField= CWProtocolRetrieve8(msgPtr);
//	CWDebugLog("AC Descriptor Wireless Field: %d",	valPtr->security);

	CWProtocolRetrieve8(msgPtr);			//Reserved

	valPtr->DTLSPolicy= CWProtocolRetrieve8(msgPtr); // DTLS Policy
//	CWDebugLog("DTLS Policy: %d",	valPtr->DTLSPolicy);

	valPtr->vendorInfos.vendorInfosCount = 0;
	
	theOffset = msgPtr->offset;
	
	// see how many vendor ID we have in the message
	while((msgPtr->offset-oldOffset) < len) {	// oldOffset stores msgPtr->offset's value at the beginning of this function.
							// See the definition of the CWParseMessageElementStart() macro.
		int tmp, id=0, type=0;		

		//CWDebugLog("differenza:%d, offset:%d, oldOffset:%d", (msgPtr->offset-oldOffset), (msgPtr->offset), oldOffset);

		id=CWProtocolRetrieve32(msgPtr);
//		CWDebugLog("ID: %d", id); // ID
		
		type=CWProtocolRetrieve16(msgPtr);
//		CWDebugLog("TYPE: %d",type); // type
		
		tmp = CWProtocolRetrieve16(msgPtr);
		msgPtr->offset += tmp; // len
//		CWDebugLog("offset %d", msgPtr->offset);
		valPtr->vendorInfos.vendorInfosCount++;
	}
	
	msgPtr->offset = theOffset;
	
	// actually read each vendor ID
	CW_CREATE_ARRAY_ERR(valPtr->vendorInfos.vendorInfos, valPtr->vendorInfos.vendorInfosCount, CWACVendorInfoValues,
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
//	CWDebugLog("len %d", len);
//	CWDebugLog("vendorInfosCount %d", valPtr->vendorInfos.vendorInfosCount);
	for(i = 0; i < valPtr->vendorInfos.vendorInfosCount; i++) {
//		CWDebugLog("vendorInfosCount %d vs %d", i, valPtr->vendorInfos.vendorInfosCount);
		(valPtr->vendorInfos.vendorInfos)[i].vendorIdentifier = CWProtocolRetrieve32(msgPtr);
		(valPtr->vendorInfos.vendorInfos)[i].type = CWProtocolRetrieve16(msgPtr);																
		(valPtr->vendorInfos.vendorInfos)[i].length = CWProtocolRetrieve16(msgPtr);
		(valPtr->vendorInfos.vendorInfos)[i].valuePtr = (int*) (CWProtocolRetrieveRawBytes(msgPtr, (valPtr->vendorInfos.vendorInfos)[i].length));
		
		if((valPtr->vendorInfos.vendorInfos)[i].valuePtr == NULL) return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
		
		if((valPtr->vendorInfos.vendorInfos)[i].length == 4) {
			*((valPtr->vendorInfos.vendorInfos)[i].valuePtr) = ntohl(*((valPtr->vendorInfos.vendorInfos)[i].valuePtr));
		}
		
//		CWDebugLog("AC Descriptor Vendor ID: %d", (valPtr->vendorInfos.vendorInfos)[i].vendorIdentifier);
//		CWDebugLog("AC Descriptor Type: %d", (valPtr->vendorInfos.vendorInfos)[i].type);
//		CWDebugLog("AC Descriptor Value: %d", *((valPtr->vendorInfos.vendorInfos)[i].valuePtr));
	}		
//	CWDebugLog("AC Descriptor Out");
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemACIPv4List(CWProtocolMessage *msgPtr, int len, ACIPv4ListValues *valPtr) {
	int i;
	CWParseMessageElementStart();
	
	if(len == 0 || ((len % 4) != 0)) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Malformed AC IPv4 List Messame Element");
	
	valPtr->ACIPv4ListCount = (len/4);
	
	CW_CREATE_ARRAY_ERR(valPtr->ACIPv4List, valPtr->ACIPv4ListCount, int, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < valPtr->ACIPv4ListCount; i++) {
		struct sockaddr_in addr;
		(valPtr->ACIPv4List)[i] = CWProtocolRetrieve32(msgPtr);
//		CWDebugLog("AC IPv4 List (%d): %d", i+1, (valPtr->ACIPv4List)[i]);
		addr.sin_addr.s_addr = (valPtr->ACIPv4List)[i];
		addr.sin_family = AF_INET;
		addr.sin_port = 1024;
		CWUseSockNtop(&addr, CWDebugLog(str););
	}
	
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemACIPv6List(CWProtocolMessage *msgPtr, int len, ACIPv6ListValues *valPtr) 
{
	int i;
	CWParseMessageElementStart();
	
	if(len == 0 || ((len % 16) != 0)) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Malformed AC IPv6 List Messame Element");
	
	valPtr->ACIPv6ListCount = (len/16);
	
	CW_CREATE_ARRAY_ERR(valPtr->ACIPv6List, valPtr->ACIPv6ListCount, struct in6_addr, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	for(i = 0; i < valPtr->ACIPv6ListCount; i++) {
		struct sockaddr_in6 addr;
   		
		/*
                 * BUG ML09
                 * 19/10/2009 - Donato Capitella
                 */                
		void *ptr;
                ptr =  CWProtocolRetrieveRawBytes(msgPtr, 16);
                CW_COPY_MEMORY(&((valPtr->ACIPv6List)[i]), ptr, 16);
                CW_FREE_OBJECT(ptr);
		CW_COPY_MEMORY(&(addr.sin6_addr), &((valPtr->ACIPv6List)[i]), 16);
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons(CW_CONTROL_PORT);
		
//		CWUseSockNtop(&addr, CWDebugLog("AC IPv6 List: %s",str););
	}
	
	CWParseMessageElementEnd();
}
#ifdef KM_BY_AC
#ifdef SOFTMAC
CWBool CWParseMsgElemDeleteWLAN(CWProtocolMessage *msgPtr, int len) {
	int Length=0;
	unsigned char* ssid;
	
	
	//CWParseMessageElementStart();	 sostituire al posto delle righe successive quando passerò valPtr alla funzione CWarseAddStation
	/*--------------------------------------------------------------------------------------*/
	int oldOffset;												
			if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
						oldOffset = msgPtr->offset;
	/*----------------------------------------------------------------------------------*/
	
	CWDebugLog("msg len: %u", len);
	CWDebugLog("\tmsg:");
	int i;
	for (i=0; i<len; i++)
	{
		CWDebugLog(" %02x", msgPtr->msg[i]);
	}

	int radioID = CWProtocolRetrieve8(msgPtr);
	int wlanID = CWProtocolRetrieve8(msgPtr);

	CWDebugLog("[wlancfg radio_id: %u, wlan_id %u]", radioID , wlanID );
	unsigned char tmp_ssid[3];
	

	// Babylon TODO
	CWWTPsend_command_to_hostapd_DEL_WLAN( tmp_ssid,3 );

	CWParseMessageElementEnd();  
}

CWBool CWParseMsgElemAddWLAN(CWProtocolMessage *msgPtr, int len) {

	int Length=0;
	unsigned char* ssid;
	unsigned char tmp_buf[len+1];
	
	
	//CWParseMessageElementStart();	 sostituire al posto delle righe successive quando passerò valPtr alla funzione CWarseAddStation
	/*--------------------------------------------------------------------------------------*/
	int oldOffset;												
			if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
						oldOffset = msgPtr->offset;
	/*----------------------------------------------------------------------------------*/
	
	tmp_buf[1] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[2] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[3] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[4] = CWProtocolRetrieve8(msgPtr);
	
	tmp_buf[5] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[6] = CWProtocolRetrieve8(msgPtr);
	short keyLength = CWProtocolRetrieve16(msgPtr);
	
	tmp_buf[7] = *(&keyLength + 1);
	tmp_buf[8] = *(&keyLength + 0);
	
	if(keyLength){
		unsigned char *key;
		key = (unsigned char*)CWProtocolRetrieveRawBytes(msgPtr, keyLength);
		memcpy( tmp_buf+9, key, keyLength);
	}
	
	tmp_buf[9+keyLength] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[10+keyLength] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[11+keyLength] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[12+keyLength] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[13+keyLength] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[14+keyLength] = CWProtocolRetrieve8(msgPtr);
	
	tmp_buf[15+keyLength] = CWProtocolRetrieve8(msgPtr);
	
	tmp_buf[16+keyLength] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[17+keyLength] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[18+keyLength] = CWProtocolRetrieve8(msgPtr);
	tmp_buf[19+keyLength] = CWProtocolRetrieve8(msgPtr);
	
	ssid = (unsigned char*)CWProtocolRetrieveRawBytes(msgPtr, len-(19+keyLength));
	 
	memcpy( tmp_buf+20+keyLength, ssid, len-19-keyLength);
	
	CWWTPsend_command_to_hostapd_ADD_WLAN( tmp_buf, len+1 );
	CWParseMessageElementEnd();  
	
	return CW_TRUE;	
}
#else
CWProtocolDeleteWLANValues *CWParseMsgElemDeleteWLAN(CWProtocolMessage *msgPtr, int len) 
{
	CWProtocolDeleteWLANValues *pwlancfg;
	CW_CREATE_OBJECT_ERR(pwlancfg, CWProtocolDeleteWLANValues, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, "Cannot allocate memeory for deleting an Wlan");return NULL;);
	pwlancfg->radio_id = CWProtocolRetrieve8(msgPtr);
	pwlancfg->wlan_id = CWProtocolRetrieve8(msgPtr);
	return pwlancfg;
}

CWProtocolAddWLANValues *CWParseMsgElemAddWLAN(CWProtocolMessage *msgPtr, int len)
{
	CWProtocolAddWLANValues *pwlancfg;
	CW_CREATE_OBJECT_ERR(pwlancfg, CWProtocolAddWLANValues, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, "Cannot allocate memeory for adding an Wlan");return NULL;);
	pwlancfg->radio_id = CWProtocolRetrieve8(msgPtr);
	pwlancfg->wlan_id = CWProtocolRetrieve8(msgPtr);

	/*
	if ((sizeof pwlancfg->capability) != 2) {
		printf("babylon debug: align.... (sizeof pwlancfg->capability) != 2\n");
	}
	*/

	pwlancfg->capability.u16 = CWProtocolRetrieve16(msgPtr);
	pwlancfg->key_index = CWProtocolRetrieve8(msgPtr);
	pwlancfg->key_status = CWProtocolRetrieve8(msgPtr);
	pwlancfg->key_len = CWProtocolRetrieve16(msgPtr);

	if (pwlancfg->key_len>32) {
		CWProtocolAddWLANValueFree(pwlancfg);
		CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Got Add Wlan, but invalid format");
		return NULL;
	}

	CWProtocolCopyRawBytes(pwlancfg->key, msgPtr, pwlancfg->key_len);
	CWProtocolCopyRawBytes(pwlancfg->group_TSC, msgPtr, 6);
	pwlancfg->non_WMM_sta_qos = CWProtocolRetrieve8(msgPtr);
	pwlancfg->auth_type = CWProtocolRetrieve8(msgPtr);
	pwlancfg->mac_mode = CWProtocolRetrieve8(msgPtr);
	pwlancfg->tunnel_mode = CWProtocolRetrieve8(msgPtr);
	pwlancfg->suppress_ssid = (CWProtocolRetrieve8(msgPtr)!= 0);
	CWProtocolCopyRawBytes(pwlancfg->ssid, msgPtr, 32);
	CWProtocolAddWLANVlauePrint(pwlancfg);

	CWDebugLog("msgPtr->offset=%d, len=%d", msgPtr->offset, len);

	return pwlancfg;	
}

#endif

#endif

CWBool CWParseMsgElemCWControlIPv4Addresses(CWProtocolMessage *msgPtr, int len, CWProtocolIPv4NetworkInterface *valPtr) {
	CWParseMessageElementStart();

	valPtr->addr.sin_addr.s_addr = htonl(CWProtocolRetrieve32(msgPtr));
	valPtr->addr.sin_family = AF_INET;
	valPtr->addr.sin_port = htons(CW_CONTROL_PORT);
	
	CWUseSockNtop((&(valPtr->addr)), CWDebugLog("Interface Address: %s", str););
	
	valPtr->WTPCount = CWProtocolRetrieve16(msgPtr);
//	CWDebugLog("WTP Count: %d",	valPtr->WTPCount);
	
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemCWControlIPv6Addresses(CWProtocolMessage *msgPtr, int len, CWProtocolIPv6NetworkInterface *valPtr) {
	CWParseMessageElementStart();
	
	CW_COPY_MEMORY(&(valPtr->addr.sin6_addr), CWProtocolRetrieveRawBytes(msgPtr, 16), 16);
	valPtr->addr.sin6_family = AF_INET6;
	valPtr->addr.sin6_port = htons(CW_CONTROL_PORT);
	
	CWUseSockNtop((&(valPtr->addr)), CWDebugLog("Interface Address: %s", str););
	
	valPtr->WTPCount = CWProtocolRetrieve16(msgPtr);
//	CWDebugLog("WTP Count: %d",	valPtr->WTPCount);
	
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemCWTimers (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->discoveryTimer	= CWProtocolRetrieve8(msgPtr);	
//	CWDebugLog("Discovery Timer: %d", valPtr->discoveryTimer);
	valPtr->echoRequestTimer = CWProtocolRetrieve8(msgPtr);
//	CWDebugLog("Echo Timer: %d", valPtr->echoRequestTimer);
	
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemDecryptErrorReportPeriod (CWProtocolMessage *msgPtr, int len, WTPDecryptErrorReportValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->radioID = CWProtocolRetrieve8(msgPtr);
	valPtr->reportInterval = CWProtocolRetrieve16(msgPtr);
//	CWDebugLog("Decrypt Error Report Period: %d - %d", valPtr->radioID, valPtr->reportInterval);
	
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemIdleTimeout (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->idleTimeout = CWProtocolRetrieve32(msgPtr);	
//	CWDebugLog("Idle Timeout: %d", valPtr->idleTimeout);
		
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemWTPFallback (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->fallback = CWProtocolRetrieve8(msgPtr);	
//	CWDebugLog("WTP Fallback: %d", valPtr->fallback);
		
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemImageInformation (CWProtocolMessage *msgPtr, int len, CWProtocolImageInformationValues*valPtr)
{
	CWParseMessageElementStart();
	valPtr->fileSize= CWProtocolRetrieve32(msgPtr);	
	CWProtocolCopyRawBytes(valPtr->hash, msgPtr, 16); 
	CWParseMessageElementEnd();
}

CWBool CWParseMsgElemImageData(CWProtocolMessage *msgPtr, int len, CWProtocolImageDataValues *valPtr)
{
	valPtr->data_len = len - 1;
	valPtr->data_type = CWProtocolRetrieve8(msgPtr);
	valPtr->data = CWProtocolPointRawBytes(msgPtr, len-1);

	if (valPtr->data_type != CW_IMAGE_DATA_TYPE_DATA &&
		valPtr->data_type != CW_IMAGE_DATA_TYPE_EOF && 
		valPtr->data_type != CW_IMAGE_DATA_TYPE_ERROR_OCCURRED) {
		return CW_FALSE;
	}
	if (valPtr->data_len == 0 && valPtr->data_type != 5) {
		return CW_FALSE;
	}

	return CW_TRUE;
}

CWBool CWParseMsgElemWtpQos(CWProtocolMessage *msgPtr, int len, CWProtocolWtpQosValues *valPtr)
{
	CWParseMessageElementStart();
	valPtr->radio_id = CWProtocolRetrieve8(msgPtr);
	valPtr->tagging_policy = CWProtocolRetrieve8(msgPtr);

	valPtr->voice.queueDepth = CWProtocolRetrieve8(msgPtr);
	valPtr->voice.cwMin = CWProtocolRetrieve16(msgPtr);
	valPtr->voice.cwMax = CWProtocolRetrieve16(msgPtr);
	valPtr->voice.AIFS = CWProtocolRetrieve8(msgPtr);
	valPtr->voice.dot1PTag = CWProtocolRetrieve8(msgPtr);
	valPtr->voice.DSCPTag = CWProtocolRetrieve8(msgPtr);	

	valPtr->video.queueDepth = CWProtocolRetrieve8(msgPtr);
	valPtr->video.cwMin = CWProtocolRetrieve16(msgPtr);
	valPtr->video.cwMax = CWProtocolRetrieve16(msgPtr);
	valPtr->video.AIFS = CWProtocolRetrieve8(msgPtr);
	valPtr->video.dot1PTag = CWProtocolRetrieve8(msgPtr);
	valPtr->video.DSCPTag = CWProtocolRetrieve8(msgPtr);	

	valPtr->best_effrot.queueDepth = CWProtocolRetrieve8(msgPtr);
	valPtr->best_effrot.cwMin = CWProtocolRetrieve16(msgPtr);
	valPtr->best_effrot.cwMax = CWProtocolRetrieve16(msgPtr);
	valPtr->best_effrot.AIFS = CWProtocolRetrieve8(msgPtr);
	valPtr->best_effrot.dot1PTag = CWProtocolRetrieve8(msgPtr);
	valPtr->best_effrot.DSCPTag = CWProtocolRetrieve8(msgPtr);	

	valPtr->back_ground.queueDepth = CWProtocolRetrieve8(msgPtr);
	valPtr->back_ground.cwMin = CWProtocolRetrieve16(msgPtr);
	valPtr->back_ground.cwMax = CWProtocolRetrieve16(msgPtr);
	valPtr->back_ground.AIFS = CWProtocolRetrieve8(msgPtr);
	valPtr->back_ground.dot1PTag = CWProtocolRetrieve8(msgPtr);
	valPtr->back_ground.DSCPTag = CWProtocolRetrieve8(msgPtr);	
	
	CWParseMessageElementEnd();
}


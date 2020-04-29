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
 * Project:  Capwap
 * Authors : Matteo Latini (mtylty@gmail.com)
 * 	     Donato Capitella (d.capitella@gmail.com)											*  
 ************************************************************************************************/


#ifndef __CAPWAP_VendorPayloads__
#define __CAPWAP_VendorPayloads__

#include "CWCommon.h"

/***********************************************************************
 * Vendor specific payloads types
 * *********************************************************************/
/*Update 2009:
		Message type to return a payload together with the 
		configuration update response*/
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE		65528
#define 	CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE_WITH_PAYLOAD		65529
#define 	CW_MSG_ELEMENT_MTU_DISCOVERY_PADDING_CW_TYPE		65530
	
//Vendor Specific data
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_AC_PRIORITY		65531
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_AC_PRIORITY_LEN	65532
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_AC_PRIORITY_DEFAULT	65533
	

#define		CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_UCI		65534
#define 	CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_WUM		65535

/** Babylon: this is referenced by both AC & WTP. But it is defined by some one for some specified vendor, 
			so moving it here */
#define WTP_VERSION_REQUEST     1
#define WTP_VERSION_RESPONSE    2

#define WTP_UPDATE_REQUEST      3
#define WTP_UPDATE_RESPONSE     4

#define WTP_CUP_FRAGMENT        5
#define WTP_CUP_ACK             6

#define WTP_COMMIT_UPDATE       7
#define WTP_COMMIT_ACK          8

#define WTP_CANCEL_UPDATE_REQUEST	9
#define WTP_CANCEL_UPDATE_RESPONSE	10




/***********************************************************************
 * UCI Server Port
 * *********************************************************************/
#define UCI_SERVER_PORT 31337

typedef struct {
	unsigned char command;
	char *commandArgs;
	char *response;
} CWVendorUciValues;

CWBool CWParseVendorPayload(CWProtocolMessage *msg, int len, CWProtocolVendorSpecificValues* valPtr);
CWBool CWParseUCIPayload(CWProtocolMessage *msg, CWVendorUciValues **payloadPtr);
CWBool CWWTPSaveUCIValues(CWVendorUciValues *uciPayload, CWProtocolResultCode *resultCode);
CWBool CWAssembleWTPVendorPayloadUCI(CWProtocolMessage *msgPtr);
CWBool CWAssembleVendorMsgElemResultCodeWithPayload(CWProtocolMessage *msgPtr,CWProtocolResultCode code, CWProtocolVendorSpecificValues *payload);

/*************************************************************************
 *  WTP Update Messages 
 *************************************************************************/

typedef struct { unsigned char major_v; unsigned char min_v; unsigned char revision; } mess_version_info;
typedef struct { mess_version_info v_info; unsigned int pack_size; } mess_up_req;
typedef struct { unsigned int seq_num; unsigned int size; char *buf; } mess_cup;

typedef struct {
	unsigned char type;		/* Message type */
	union {		
		mess_version_info v_resp;
		mess_up_req up_req;
		mess_cup cup;
	} args;
} CWVendorWumValues;

#define _major_v_ args.v_resp.major_v
#define _minor_v_ args.v_resp.min_v
#define _revision_v_ args.v_resp.revision
#define _pack_size_ args.up_req.pack_size
#define _seq_num_ args.cup.seq_num
#define _cup_ args.cup.buf 
#define _cup_fragment_size_ args.cup.size

#define CUP_FRAGMENT_SIZE 4000

//CWBool CWParseWUMPayload(CWProtocolMessage *msg, CWVendorUciValues **payloadPtr);
CWBool CWWTPSaveWUMValues(CWVendorWumValues *wumPayload, CWProtocolResultCode *resultCode);
CWBool CWAssembleWTPVendorPayloadWUM(CWProtocolMessage *msgPtr);


#endif

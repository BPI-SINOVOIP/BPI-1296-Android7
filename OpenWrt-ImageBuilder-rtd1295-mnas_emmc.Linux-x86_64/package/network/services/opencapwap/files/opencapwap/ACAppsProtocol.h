/*
 *  appsToAcProtocol.h
 *  
 *
 *  Created by Antonio Davoli on 03/03/09.
 *  Copyright 2009 La Sapienza. All rights reserved.
 *
 */

#include "CWCommon.h"

#pragma pack(1)
typedef struct {
	char cmd_msg;
	char msg_elem;
	int wtpId;
	char wum_type;
	int payload_len;
	char *payload;
} wum_req_t; // for CONF_UPDATE_MSG

typedef struct {
	int wtpId;
	int resultCode;
	char wum_type;		// Note that, this is not always exists in packet!
	int payload_len;
	char *payload;
	int offset;
} wum_resp_t;
#pragma pack(0)

/* AC Request Message */


/* CMD_MSG Types */
// ==========================================
//		mesage types 
// ==========================================
#define QUIT_MSG 0
/****************************************
 *	For LIST, QUIT (without argument)	*
 *										*
 *	  	0			    7 				*
 *		+-+-+-+-+-+-+-+-+				*
 *		|    Cmd_msg    |				*
 *		+-+-+-+-+-+-+-+-+				*
 *										*
 ****************************************/ 

#define LIST_WTPS_MSG 1	// list all wtps that belong to the AC.

/********************************************************************************************************************************
 * List Response Message:
 *
 *																																*
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   *
 *	|   Active#   |    WTP_ID 1   | NameLength 1  |   WTP Name 1    | ... |    WTP ID N   | NameLength N  |   WTP Name N    |	*
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+	  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+	*
 *																																*
 *	where N is equal to Active# (Number of Active WTPs)																			*
 ********************************************************************************************************************************/

#define CONF_UPDATE_MSG 2
/********************************************************************************************************
 * For CONF_UPDATE_MSG type: ==> wum_req_t																*
 *																										*
 *	  	0			   7 			   15						47									X	*
 *		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+	*
 *		|    cmd_msg    |    msg_elem   |		WTP Index		|		Message Specific Payload	|	*
 *		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+	*
 *																										*
 ********************************************************************************************************/

#define LIST_WTP_INFO_MSG 3	// list some info of a specified wtp.
/********************************************************************************************************
 * For LIST_WTP_INFO_MSG type:																			*
 *																										*
 *	  	0			   7 			   15			   23												*
 *		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+												*
 *		|    cmd_msg    |    msg_elem   |   WTP Index   |												*
 *		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+												*
 *																										*
 ********************************************************************************************************/

// ==========================================
//		mesage element types 
// ==========================================
#define MSG_ELEMENT_TYPE_IMAGE_ID		1
#define MSG_ELEMENT_TYPE_DEL_STATION	2
#define MSG_ELEMENT_TYPE_LIST_STATION	3
#define MSG_ELEMENT_TYPE_LIST_SITE_SURVEY 4
#define MSG_ELEMENT_TYPE_UPD_POWER		5
#define MSG_ELEMENT_TYPE_UPD_CHANNEL	6
#define MSG_ELEMENT_TYPE_OFDM			7
/****************************************************************************************
 * Message Specific Payload for MSG_ELEMENT_TYPE_OFDM TYPE (802.11 Binding Version)		* 
 *																						*
 *		0                   1                   2                   3					*
 *		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1					*
 *		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+				*
 *		|    Radio ID   |    Reserved   | Current Chan  |  Band Support |				*
 *		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+				*
 *		|                         TI Threshold                          |				*
 *		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+				*
 *																						*										
 ****************************************************************************************/ 

/* Radio ID is filled in the creation message funcion (inside the AC) */

#define MSG_ELEMENT_TYPE_VENDOR_UCI 254
#define MSG_ELEMENT_TYPE_VENDOR_WUM 255

// ----- return from AC? ---------
#define FULL_CLIENT_CONNECTED -1
#define CONNECTION_OK 1

// -------------------------------
#define PAYLOAD_START ((sizeof(unsigned char)*2) + sizeof(int)) 
#define ALL_ACTIVE_WTPS -1

static inline const char *ACGetImageFile(const char *imageId)
{
	/* 
	 Note!! Please define the mapping between firmware-id and the firemware file.
	*/
	// XXX
	return "/tmp/fw-h3c-new.bin";
	//return imgeId;
}


/*******************************************************************************************
 * Copyright (c) 2006-2009 Laboratorio di Sistemi di Elaborazione e Bioingegneria          *
 *                         Informatica Universita' Campus BioMedico - Italy                *
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
 * Authors : Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *	         Daniele De Sanctis (danieledesanctis@gmail.com)                           *
 *	         Antonio Davoli (antonio.davoli@gmail.com)                                 *
 *		 Donato Capitella (d.capitella@gmail.com)				   *
 *******************************************************************************************/

#ifndef __CAPWAP_ACInterface_HEADER__
#define __CAPWAP_ACInterface_HEADER__

typedef enum
{
	//No Interface Command
	NO_CMD = 0,
	//Manual setting for QoS values
	QOS_CMD = 1,
	CLEAR_CONFIG_MSG_CMD = 2,
	/* 2009 Update: Manual setting for OFDM values*/
	OFDM_CONTROL_CMD = 3,
	/*Update 2009: 	Manage UCI configuration command*/
	UCI_CONTROL_CMD = 4,
	/* Manage WTP Update Command */
	WTP_UPDATE_CMD = 5,
	// 2013 ~ 2014 Babylon Added:
	WTP_IMAGE_CMD =	6,
	WTP_DEL_STATION_CMD = 7,
	WTP_ADD_WLAN_CMD = 8,
	WTP_SITE_SURVEY_CMD = 9,
	WTP_MODIFY_CHANNEL_CMD = 10,
	WTP_MODIFY_POWER_CMD = 11,
	WTP_LIST_STATIONS_CMD = 12,
} ac_active_work_type_t;
//Babylon TODO: re-arrange with cmd_type_t

typedef struct {
	ac_active_work_type_t type;
	union {
#if 0
		// for WTP_DEL_STATION_CMD
		struct {
			int radio_id;
			unsigned char sta_mac[6];		
		} dtp_del_station;

		// TODO for WTP_IMAGE_CMD
		struct {
			char fw_path[0];
		} wtp_image;
#endif
		// for WTP_ADD_WLAN_CMD
		//CWProtocolAddWLANValues wtp_add_wlan;


	} data;
} ac_active_work_t;



#endif

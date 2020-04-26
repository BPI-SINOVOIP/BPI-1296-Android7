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
 * Author :  babylon@realtek.com
 *******************************************************************************************/
#include "ACRtkSystemInteraction.h"
#ifndef X86_SIMULATION
#ifdef CONFIG_OPENWRT_SDK
#include <apmib.h>
#include "version.c"
#else
#include "../boa/apmib/apmib.h"
#include "../boa/src/version.c"
#endif
#endif

#define RTL8192CD_IOCTL_GET_MIB 0x89f2

/**************** Public APIs: Init ****************/
const char *cwRtkACGetSwVersion()
{
#ifdef X86_SIMULATION
	static const char currentFwId[CW_IMAGE_IDENTIFIER_MAX_SIZE] = "v3.4.6.3";
	return currentFwId;
#else
	return (char *)fwVersion; // in ../boa/src/version.c
#endif
}

int cwRtkAcGetVendorId()
{
	return VENDOR_IANA_ID;
}

CWBool cwRtkAcInit()
{
#ifndef X86_SIMULATION
#ifndef RTK_129X_PLATFORM
	/**** ap mib ***/
	if (!apmib_init()) {
		return CW_FALSE;
	}
#endif //RTK_129X_PLATFORM
#endif
	return CW_TRUE;
}

void cwRtkAcExit()
{
}


CWBool cwRtkAcTransferWtpName2Id(const char *wtp_name, int *id)
{
        int idx;
        for (idx = strlen(wtp_name)-1; idx>=0; idx--) {
                if(wtp_name[idx]>'9' || wtp_name[idx]<'0') {
                        break;
                }
        }
        if (idx == strlen(wtp_name)-1) return CW_FALSE;

        *id = atoi(&wtp_name[idx+1]);
        return CW_TRUE;
}

/**************** get info from flash/driver ****************/
CWBool get_mib(const char *ifname, const char *mibname, void *result, int size)
{
	int sock;	
	struct iwreq wrq;
	unsigned char tmp[30];

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		CWLog("[%s %d] Error Creating Socket for ioctl", __FUNCTION__, __LINE__); 
		goto out;
	}
	
	memset(&wrq, 0, sizeof(wrq));
	strcpy(wrq.ifr_name, ifname);
	strcpy((char *)tmp, mibname);

	wrq.u.data.pointer = tmp;
	wrq.u.data.length = strlen((char *)tmp);

	/*** get mib ***/
	if(ioctl(sock, RTL8192CD_IOCTL_GET_MIB, &wrq) < 0) {	
		CWLog("[%s %d] Error to ioctl", __FUNCTION__, __LINE__);
		goto out;
	}
	if(size)
		memcpy(result, tmp, size);
	else
		strcpy(result, (char *)tmp);

out:	
	close(sock);
	return CW_TRUE;
}

CWBool cwRtkAcSystemEnabled()
{
#ifndef X86_SIMULATION
	int capwapMode;
	#ifdef RTK_AUTO_AC
	return CW_TRUE;
	#else
	apmib_get(MIB_CAPWAP_MODE, &capwapMode);
	return ((capwapMode & CAPWAP_AC_ENABLE)!=0);
	#endif
#else
	return CW_TRUE;
#endif
}

CWBool cwRtkAcGetWTPDefaultFwId(char *fwImageIdentifier)
{
#ifndef X86_SIMULATION
	apmib_get(MIB_CAPWAP_WTP_DEFAULT_FWID, fwImageIdentifier);
	if (strcmp("", fwImageIdentifier)==0) {
		strcpy(fwImageIdentifier, cwRtkACGetSwVersion());
		apmib_set(MIB_CAPWAP_WTP_DEFAULT_FWID, fwImageIdentifier);
		apmib_update(CURRENT_SETTING);
	}
#else
	//strcpy(fwImageIdentifier, "3.6.4");
	strcpy(fwImageIdentifier, cwRtkACGetSwVersion());
#endif
	return CW_TRUE;
}

#ifndef X86_SIMULATION

#ifndef KM_BY_AC
/* return true if AC has the configuration of this wtp */
CWBool cwRtkAcWtpExists(int wtp_id)
{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int i, wtp_count;

	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);

	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
		if (capwapWtpConfig.wtpId == wtp_id) {
			return CW_TRUE;
		}
	}
	return CW_FALSE;
}

CWBool cwRtkAcGetPowerScale(int wtp_id, int radio_id, rtk_power_scale_t *power_scale)
{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int i, wtp_count;

	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);

	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
		if (capwapWtpConfig.wtpId == wtp_id) {
			if (radio_id < capwapWtpConfig.radioNum) {
				*power_scale =  capwapWtpConfig.powerScale[radio_id];
				return CW_TRUE;
			}
		}
	}
	return CW_FALSE;
}


CWBool cwRtkAcGetChannel(int wtp_id, int radio_id, unsigned char *channel)
{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int i, wtp_count;

	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);

	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
		if (capwapWtpConfig.wtpId == wtp_id) {
			if (radio_id < capwapWtpConfig.radioNum) {
				*channel =  capwapWtpConfig.channel[radio_id];
				return CW_TRUE;
			}
		}
	}
	return CW_FALSE;
}

CWBool cwRtkAcGetWlanConfig(int wtp_id, int radio_id, int wlan_id, 
	CWProtocolRtkWlanConfigValues *rtkWlanConfig)

{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int i, wtp_count;

	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);

	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
		if (capwapWtpConfig.wtpId == wtp_id) {
			if (radio_id < capwapWtpConfig.radioNum) {
				if (wlan_id < capwapWtpConfig.wlanNum && capwapWtpConfig.wlanConfig[radio_id][wlan_id].enable) {
					rtkWlanConfig->radio_id = radio_id;
					rtkWlanConfig->wlan_id = wlan_id;
					memcpy(rtkWlanConfig->bssid, capwapWtpConfig.wlanConfig[radio_id][wlan_id].bssid, 6);
					strcpy(rtkWlanConfig->ssid, capwapWtpConfig.wlanConfig[radio_id][wlan_id].ssid);
					rtkWlanConfig->key_type = capwapWtpConfig.wlanConfig[radio_id][wlan_id].keyType; 
					rtkWlanConfig->psk_format = capwapWtpConfig.wlanConfig[radio_id][wlan_id].pskFormat;
					memcpy(rtkWlanConfig->key, capwapWtpConfig.wlanConfig[radio_id][wlan_id].key, CW_MAX_KEY_SIZE*2+1);
					return CW_TRUE;
				}
			}
		}
	}
	return CW_FALSE;
}
/*
CWBool cwRtkAcGetAllWlansConfig(int wtp_id, int *wlan_count, CWProtocolRtkWlanConfigValues **ppRtkWlanConfig)

{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int i, j, k, n, wtp_count;

	apmib_get(MIB_CAPWAP_WTP_CONFIG_NUM, &wtp_count);
	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
		if (capwapWtpConfig.wtpId == wtp_id) {
			for (j=0, (*wlan_count)=0; j<capwapWtpConfig.radioNum; j++) {
				for (k=0; k<capwapWtpConfig.wlanNum; k++) {
					if(capwapWtpConfig.wlanConfig[j][k].enable) {
						(*wlan_count)++;
					}
				}
			}
			*ppRtkWlanConfig = CW_CREATE_ARRAY_ERR(*ppRtkWlanConfig, *wlan_count, 
					CWProtocolRtkWlanConfigValues, 
					return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

			for (j=0, n=0; j<capwapWtpConfig.radioNum; j++) {
				for (k=0; k<capwapWtpConfig.wlanNum; k++) {
					if(capwapWtpConfig.wlanConfig[j][k].enable) {
						(*ppRtkWlanConfig)[n].radio_id = i;
						(*ppRtkWlanconfig)[n].rtkWlanConfig->wlan_id = j;
						memcpy((*ppRtkWlanConfig)[n].bssid, capwapWtpConfig.wlanConfig[j][k].bssid, 6);
						strcpy((*ppRtkWlanConfig)[n].ssid, capwapWtpConfig.wlanConfig[j][k].ssid);

						(*ppRtkWlanConfig)[n].key_type = capwapWtpConfig.wlanConfig[j][k].keyType; 
						(*ppRtkWlanConfig)[n].psk_format = capwapWtpConfig.wlanConfig[j][k].pskFormat;
						memcpy((*ppRtkWlanConfig)[n].key, capwapWtpConfig.wlanConfig[j][k].key, CW_MAX_KEY_SIZE*2+1);
						n++;
					}
				}
			}
			return CW_TRUE;
		}
	}
	return CW_FALSE;
}
*/

CWBool cwRtkAcGetWtpConfig(const char *wtp_name, rtk_wtp_t *wtp)
{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int wtp_id, wtp_count, i, j, k;
	if(!cwRtkAcTransferWtpName2Id(wtp_name, &wtp_id)) return CW_FALSE;
	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);
	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i+1;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
//printf("capwapWtpConfig.wtpId=%d\n", capwapWtpConfig.wtpId);

		if (capwapWtpConfig.wtpId == wtp_id) {
			
			strcpy(wtp->name, wtp_name);
			wtp->nRadios = capwapWtpConfig.radioNum;
			wtp->nWlans = capwapWtpConfig.wlanNum;

			for (j=0; j<wtp->nRadios; j++) {
				memcpy(wtp->radios[j].baseMAC, capwapWtpConfig.wlanConfig[j][0].bssid, 6);
				wtp->radios[j].channel = capwapWtpConfig.channel[j];
				wtp->radios[j].powerScale = capwapWtpConfig.powerScale[j];
				for (k=0; k<wtp->nWlans; k++) {					
					wtp->radios[j].wlans[k].enable = capwapWtpConfig.wlanConfig[j][k].enable;
					memcpy(wtp->radios[j].wlans[k].bssid, capwapWtpConfig.wlanConfig[j][k].bssid, 6);
					strcpy(wtp->radios[j].wlans[k].ssid, capwapWtpConfig.wlanConfig[j][k].ssid);
					wtp->radios[j].wlans[k].keyType = capwapWtpConfig.wlanConfig[j][k].keyType;
					wtp->radios[j].wlans[k].pskFormat = capwapWtpConfig.wlanConfig[j][k].pskFormat;
					strcpy(wtp->radios[j].wlans[k].key, capwapWtpConfig.wlanConfig[j][k].key);
				}
			}
			strncpy(wtp->fwImageIdentifier, capwapWtpConfig.fwId, MAX_CAPWAP_FWID_BUF_SIZE);
			return CW_TRUE;
		}
	}
	return CW_FALSE;
}

#if 0
//Babylon: need review  this function!!!
CWBool cwRtkAcAddWtpConfig(int wtp_id, int radio_num, int wlan_num_per_radio,
							int wlan_count, const CWProtocolRtkWlanConfigValues *rtkWlanConfig,
							const unsigned char *rtkChannelConfig, const rtk_power_scale_t *rtkPowerScaleConfig)
{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig_org;
	int i, n, wtp_count;

	if (wtp_id == 0 ||  radio_num > MAX_CAPWAP_RADIO_NUM || wlan_num_per_radio > MAX_CAPWAP_WLAN_NUM) {
		printf("error: wtp_id = %d, radio_num = %d, wlan_num_per_radio = %d\n", wtp_id, radio_num, wlan_num_per_radio);
		return CW_FALSE;
	}
	memset(&capwapWtpConfig_org, 0, sizeof(CAPWAP_WTP_CONFIG_T));

	// look for old setting and delete it.
	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);
	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig_org = (unsigned char)i+1;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig_org);
		if (capwapWtpConfig_org.wtpId == wtp_id) {
			apmib_set(MIB_CAPWAP_WTP_CONFIG_DEL, &capwapWtpConfig_org);
			wtp_count --;
		}
	}

	// add new setting
	if (wtp_count < MAX_CAPWAP_WTP_NUM) {
		CAPWAP_WTP_CONFIG_T capwapWtpConfig;
		int i, j;

		memset(&capwapWtpConfig, 0, sizeof(CAPWAP_WTP_CONFIG_T));

		capwapWtpConfig.radioNum = radio_num;
		capwapWtpConfig.wlanNum = wlan_num_per_radio;

		// channels & power scales
		for (n=0; n<radio_num; n++) {
			capwapWtpConfig.channel[n] = rtkPowerScaleConfig[n];
			capwapWtpConfig.powerScale[n] = rtkPowerScaleConfig[n];
		}
		// reset all values of other radios
		for (;n<MAX_CAPWAP_RADIO_NUM; n++) {
			capwapWtpConfig.channel[n] = 0;
			capwapWtpConfig.powerScale[n] = POWER_SCALE_100;
		}

		// reset all wlan values
		for (i=0; i<MAX_CAPWAP_RADIO_NUM; i++) {
			for (j=0; j<MAX_CAPWAP_WLAN_NUM; j++) {
				capwapWtpConfig.wlanConfig[i][j].enable = 0;
			}
		}
		// fill wlan configuration to the array
		for (n=0; n<wlan_count; n++) {
			int radio_id, wlan_id;
			radio_id = rtkWlanConfig[n].radio_id;
			wlan_id = rtkWlanConfig[n].wlan_id;
			
			if (radio_id < radio_num && wlan_id < wlan_num_per_radio) {
				capwapWtpConfig.wlanConfig[radio_id][wlan_id].enable = 1;
				memcpy(capwapWtpConfig.wlanConfig[radio_id][wlan_id].bssid,rtkWlanConfig[n].bssid, 6);
				strcpy(capwapWtpConfig.wlanConfig[radio_id][wlan_id].ssid, rtkWlanConfig[n].ssid);

				capwapWtpConfig.wlanConfig[radio_id][wlan_id].keyType   = rtkWlanConfig[n].key_type;   
				capwapWtpConfig.wlanConfig[radio_id][wlan_id].pskFormat = rtkWlanConfig[n].psk_format;
				memcpy(capwapWtpConfig.wlanConfig[radio_id][wlan_id].key, rtkWlanConfig[n].key, CW_MAX_KEY_SIZE*2+1);
				
				apmib_set(MIB_CAPWAP_WTP_CONFIG_ADD, &capwapWtpConfig);
				wtp_count ++;
				if (!apmib_update(CURRENT_SETTING)) {
					return CW_FALSE;
				}
			} else {
				// invalid config
				if (capwapWtpConfig_org.wtpId == wtp_id) {
					// set the old config back
					apmib_set(MIB_CAPWAP_WTP_CONFIG_ADD, &capwapWtpConfig_org);
					wtp_count ++;
					if (!apmib_update(CURRENT_SETTING)) {
						return CW_FALSE;
					}
				}
				printf("error: radio_num = %d, wlan_num_per_radio = %d, radio_id = %d, wlan_id = %d\n", 
							radio_num, wlan_num_per_radio, radio_id, wlan_id);
				return CW_FALSE;
			}

		}
		return CW_TRUE;
	} else {
		printf("error: wtp_count = %d\n", wtp_count);
		return CW_FALSE;
	}
}
#endif

CWBool cwRtkAcSetPowerScale(int wtp_id,int radio_id, rtk_power_scale_t power_scale)
{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int i, wtp_count;
	
	CWLog("%s started", __FUNCTION__);

	// check input value
	if (wtp_id < 0 || 
		radio_id >= MAX_CAPWAP_RADIO_NUM || radio_id < 0) 
		return CW_FALSE;

	if (power_scale > MAX_POWER_SCALE_ENUM_VALUE || power_scale < MIN_POWER_SCALE_ENUM_VALUE) {
		return CW_FALSE;
	}

	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);

	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i+1;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
		if (capwapWtpConfig.wtpId == wtp_id) {
			apmib_set(MIB_CAPWAP_WTP_CONFIG_DEL, &capwapWtpConfig);
			if (radio_id >= capwapWtpConfig.radioNum) {
				capwapWtpConfig.radioNum = radio_id+1;
			}
			capwapWtpConfig.powerScale[radio_id] = power_scale;			
			apmib_set(MIB_CAPWAP_WTP_CONFIG_ADD, &capwapWtpConfig);
		}
	}

	if (!apmib_update(CURRENT_SETTING)) {
		return CW_FALSE;
	}

	
	CWLog("%s succesfully completed", __FUNCTION__);
	return CW_TRUE;
}

CWBool cwRtkAcSetChannel(int wtp_id,int radio_id, unsigned char channel)
{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int i, wtp_count;

	CWLog("%s started", __FUNCTION__);

	// check input value
	if (wtp_id < 0 || 
		radio_id >= MAX_CAPWAP_RADIO_NUM || radio_id < 0) 
		return CW_FALSE;

	// TODO: check channel value

	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);

	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i+1;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
		if (capwapWtpConfig.wtpId == wtp_id) {
			apmib_set(MIB_CAPWAP_WTP_CONFIG_DEL, &capwapWtpConfig);
			if (radio_id >= capwapWtpConfig.radioNum) {
				capwapWtpConfig.radioNum = radio_id+1;
			}
			capwapWtpConfig.channel[radio_id] = channel;			
			apmib_set(MIB_CAPWAP_WTP_CONFIG_ADD, &capwapWtpConfig);
		}
	}

	if (!apmib_update(CURRENT_SETTING)) {
		return CW_FALSE;
	}
	
	CWLog("%s succesfully completed", __FUNCTION__);
	return CW_TRUE;
}

#endif

CWBool cwRtkAcSetFwId(int wtp_id, const char *fwId)
{
	CAPWAP_WTP_CONFIG_T capwapWtpConfig;
	int i, wtp_count;

	CWLog("%s started", __FUNCTION__);

	// check input value
	if (wtp_id < 0 || fwId == NULL)
		return CW_FALSE;

	apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL_NUM, &wtp_count);

	for (i=0; i<wtp_count; i++) {
		*(unsigned char *)&capwapWtpConfig = (unsigned char)i+1;
		apmib_get(MIB_CAPWAP_WTP_CONFIG_TBL, &capwapWtpConfig);
		if (capwapWtpConfig.wtpId == wtp_id) {
			apmib_set(MIB_CAPWAP_WTP_CONFIG_DEL, &capwapWtpConfig);
			strncpy(capwapWtpConfig.fwId, fwId, MAX_CAPWAP_FWID_BUF_SIZE);
			apmib_set(MIB_CAPWAP_WTP_CONFIG_ADD, &capwapWtpConfig);
		}
	}

	if (!apmib_update(CURRENT_SETTING)) {
		return CW_FALSE;
	}
	
	CWLog("%s succesfully completed", __FUNCTION__);
	return CW_TRUE;
}

#endif

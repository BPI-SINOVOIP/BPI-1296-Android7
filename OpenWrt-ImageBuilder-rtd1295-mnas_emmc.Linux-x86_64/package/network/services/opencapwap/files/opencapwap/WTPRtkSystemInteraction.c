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
#include "WTPRtkSystemInteraction.h"
#include "WTPRtkFrameReceive.h"
#include "WTPRtkInternetCheck.h"
#include <sys/mman.h>	
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "linux/netlink.h"
#ifndef X86_SIMULATION
#ifdef CONFIG_OPENWRT_SDK
#include <net80211/ieee80211_ioctl.h>
#include "version.c"
#include <apmib.h>
#else
#include "../inband_lib/net80211/ieee80211_ioctl.h"
#include "../boa/src/version.c"
#include "../boa/apmib/apmib.h"
#endif
#endif

/************* define / enum / typedef *********************/
#define RTL8192CD_IOCTL_SET_MIB 0x89f1
#define RTL8192CD_IOCTL_GET_MIB 0x89f2
#define RTL8192CD_IOCTL_DEL_STA	0x89f7

#ifdef RTK_SMART_ROAMING
#define RTL8192CD_IOCTL_SILENT_STA      0x8BF7
#define RTL8192CD_IOCTL_FREE_STA        0x8BF8
#define SIOC11VBSSTRANSREQ				0x8BF5
#define RTL8192CD_IOCTL_DUAL_STA		0x8BF9
#define RTL8192CD_IOCTL_UNSILENT_STA      0x8BFB
#endif

#define HAPD_IOCTL_SETCONFIG	0x8BFF	// for KM_BY_AC, we borrow from the implementation of hostapd.
#define SIOCGIWRTLSCANREQ		0x8B33	// scan request
#define SIOCGIWRTLGETBSSDB		0x8B34	// get bss data base
#define SIOCGIWRTLSTAINFO   	0x8B30

#define BIT(x)	(1 << (x))

enum NETWORK_TYPE {
	WIRELESS_11B = 1,
	WIRELESS_11G = 2,
	WIRELESS_11A = 4,
	WIRELESS_11N = 8,
	WIRELESS_11AC = 64
};

typedef enum _PHY_BAND_SELECT_ {
	PHY_BAND_2G = BIT(0),
	PHY_BAND_5G = BIT(1)
} PHY_BAND_SELECT;

// ==== site survey =========
#define	MAX_BSS_DESC	64
#define MAX_MSG_BUFFER_SIZE 256
#define SSID_LEN 32
#define MESHID_LEN 32
typedef struct _OCTET_STRING {
    unsigned char *Octet;
    unsigned short Length;
} OCTET_STRING;


typedef	struct _IbssParms {
    unsigned short	atimWin;
} IbssParms;

typedef struct _BssDscr {
    unsigned char bdBssId[6];
    unsigned char bdSsIdBuf[SSID_LEN];
    OCTET_STRING  bdSsId;

//#if defined(CONFIG_RTK_MESH) || defined(CONFIG_RTL_819X) 
	unsigned char bdMeshIdBuf[MESHID_LEN]; 
	OCTET_STRING bdMeshId; 
//#endif 
    int bdType;
    unsigned short bdBcnPer;			// beacon period in Time Units
    unsigned char bdDtimPer;			// DTIM period in beacon periods
    unsigned long bdTstamp[2];			// 8 Octets from ProbeRsp/Beacon
    IbssParms bdIbssParms;			// empty if infrastructure BSS
    unsigned short bdCap;				// capability information
    unsigned char ChannelNumber;			// channel number
    unsigned long bdBrates;
    unsigned long bdSupportRates;		
    unsigned char bdsa[6];			// SA address
    unsigned char rssi, sq;			// RSSI and signal strength
    unsigned char network;			// 1: 11B, 2: 11G, 4:11G
	// P2P_SUPPORT
	unsigned char	p2pdevname[33];		
	unsigned char	p2prole;	
	unsigned short	p2pwscconfig;		
	unsigned char	p2paddress[6];	
    //add by TED to fix length difference with driver 176 to 172 1.	JIRA ticket [ENTERAP-36]:[SPT-Tek] 
    unsigned char	stage;	
    #if defined(WIFI_WPAS) || defined(RTK_NL80211)
    #if 0
        unsigned char	    p2p_ie_len;
        unsigned char	    p2p_ie[256];    
	unsigned char	    wscie_len;
	unsigned char	    wscie[256];
    #endif
	struct ht_info_elmt ht_info;
	struct ht_cap_elmt ht_cap;
    #endif
    #if defined(WIFI_WPAS) || defined(RTK_NL80211) || defined(CONFIG_RTL_COMAPI_WLTOOLS)
        unsigned char	    wpa_ie_len;
        unsigned char	    wpa_ie[256];
        unsigned char	    rsn_ie_len;    
        unsigned char	    rsn_ie[256];
    #endif  
} BssDscr, *pBssDscr;

typedef struct _sitesurvey_status {
    unsigned char number;
    unsigned char pad[3];
    BssDscr bssdb[MAX_BSS_DESC];
} SS_STATUS_T, *SS_STATUS_Tp;

#ifndef X86_SIMULATION
typedef WLAN_STA_INFO_T wlan_stations_info_t [MAX_STA_NUM+1];

typedef struct {
	unsigned char	pwrlevelCCK_A[MAX_2G_CHANNEL_NUM_MIB];
	unsigned char	pwrlevelCCK_B[MAX_2G_CHANNEL_NUM_MIB];
	unsigned char	pwrlevelHT40_1S_A[MAX_2G_CHANNEL_NUM_MIB];
	unsigned char	pwrlevelHT40_1S_B[MAX_2G_CHANNEL_NUM_MIB];
	unsigned char	pwrlevel5GHT40_1S_A[MAX_5G_CHANNEL_NUM_MIB];
	unsigned char	pwrlevel5GHT40_1S_B[MAX_5G_CHANNEL_NUM_MIB];
} rtk_power_scaled_result_t;
#endif

/*************** static common API *********************/
// utility ----
static int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

static int __inline__ string_to_hex(const char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1]))
			return 0;

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
	}
	return 1;
}

static CWradioType transTypeRadio(enum NETWORK_TYPE from)
{
	CWradioType to = 0;
	if (from & WIRELESS_11A) to |= CW_802_DOT_11a;
	if (from & WIRELESS_11B) to |= CW_802_DOT_11b;
	if (from & WIRELESS_11G) to |= CW_802_DOT_11g;
	if (from & WIRELESS_11N) to |= CW_802_DOT_11n;
	if (from & WIRELESS_11AC) to |= CW_802_DOT_11ac;
	return to;
}

// ------------
#ifdef RTK_SMART_ROAMING
static int delete_station(const char *ifname, const unsigned char *mac)
{
	int sock;	
	struct iwreq wrq;
	int err;
	char mac_str[13];

	sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	CWRoamLog("%s delete station %s", ifname, mac_str);

	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWRoamLog("del_sta: %s(%s). Error Creating Socket for ioctl.(%d)", ifname, mac_str, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill mib string ***/
	wrq.u.data.pointer = (caddr_t)mac_str;
	wrq.u.data.length = 18;

	/*** set mib ***/
	if(ioctl(sock, RTL8192CD_IOCTL_DEL_STA, &wrq) < 0) {	
		err = errno;
		CWRoamLog("del_sta: %s(%s). Error to ioctl.(%d)", ifname, mac_str, err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}
static int free_station(const char *ifname, const unsigned char *mac)
{
	int sock;	
	struct iwreq wrq;
	int err;
	char mac_str[13];

	sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	CWRoamLog("%s free info of station %s", ifname, mac_str);
	
	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) { 
		err = errno;
		CWRoamLog("free_sta: %s(%s). Error Creating Socket for ioctl.(%d)", ifname, mac_str, err); 
		goto out;
	}
		
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill mib string ***/
	wrq.u.data.pointer = (caddr_t)mac_str;
	wrq.u.data.length = 18;

	/*** set mib ***/
	if(ioctl(sock, RTL8192CD_IOCTL_FREE_STA, &wrq) < 0) { 
		err = errno;
		CWRoamLog("free_sta: %s(%s). Error to ioctl.(%d)", ifname, mac_str, err);
		goto out;
	}
	err = 0;
	
out:	
	close(sock);
	return err;
}


static int silent_station(const char *ifname, const unsigned char *mac)
{
	int sock;	
	struct iwreq wrq;
	int err;
	char mac_str[13];

	sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	CWRoamLog("%s keep silent of station %s", ifname, mac_str);

	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWRoamLog("silent_sta: %s(%s). Error Creating Socket for ioctl.(%d)", ifname, mac_str, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill mib string ***/
	wrq.u.data.pointer = (caddr_t)mac_str;
	wrq.u.data.length = 18;

	/*** set mib ***/
	if(ioctl(sock, RTL8192CD_IOCTL_SILENT_STA, &wrq) < 0) {	
		err = errno;
		CWRoamLog("silent_sta: %s(%s). Error to ioctl.(%d)", ifname, mac_str, err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}

static int unsilent_station(const char *ifname, const unsigned char *mac)
{
	int sock;	
	struct iwreq wrq;
	int err;
	char mac_str[13];

	sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	CWRoamLog("%s un-silent of station %s", ifname, mac_str);

	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWRoamLog("unsilent_sta: %s(%s). Error Creating Socket for ioctl.(%d)", ifname, mac_str, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill mib string ***/
	wrq.u.data.pointer = (caddr_t)mac_str;
	wrq.u.data.length = 18;

	/*** set mib ***/
	if(ioctl(sock, RTL8192CD_IOCTL_UNSILENT_STA, &wrq) < 0) {	
		err = errno;
		CWRoamLog("silent_sta: %s(%s). Error to ioctl.(%d)", ifname, mac_str, err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}

static int dot11v_station(const char *ifname, const unsigned char *mac)
{
	int sock;	
	struct iwreq wrq;
	int err;

	CWRoamLog("%s issue 11v to station %02x:%02x:%02x:%02x:%02x:%02x",
			ifname, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWRoamLog("dot11v_sta: %s(%s). Error Creating Socket for ioctl.(%d)", 
				mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],ifname,err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill mib string ***/
	wrq.u.data.pointer = (caddr_t)mac;
	wrq.u.data.length = 8;
	
	/*** set mib ***/
	if(ioctl(sock, SIOC11VBSSTRANSREQ, &wrq) < 0) {	
		err = errno;
		CWRoamLog("dot11v_sta: %s(%s). Error to ioctl.(%d)", 
				mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],ifname,err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}

static int dual_station(const char *ifname, const unsigned char *mac)
{
	int sock;	
	struct iwreq wrq;
	int err;
	char mac_str[13];

	sprintf(mac_str, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	CWRoamLog("%s is notified of dual band station %s", ifname, mac_str);

	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWRoamLog("dual_sta: %s(%s). Error Creating Socket for ioctl.(%d)", ifname, mac_str, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill mib string ***/
	wrq.u.data.pointer = (caddr_t)mac_str;
	wrq.u.data.length = 18;

	/*** set mib ***/
	if(ioctl(sock, RTL8192CD_IOCTL_DUAL_STA, &wrq) < 0) {	
		err = errno;
		CWRoamLog("dual_sta: %s(%s). Error to ioctl.(%d)", ifname, mac_str, err);
		goto out;
	}
	err = 0;
out:	
	close(sock);
	return err;
}
#endif

static int set_mib(const char *ifname, const char *item_str, const char *value_str)
{

	int sock;	
	struct iwreq wrq;
	static char mib_str_buf[512];
	int err;

	sprintf(mib_str_buf, "%s=%s", item_str, value_str);
	CWDebugLog("set mib for %s: %s", ifname, mib_str_buf);

	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWLog("set_mib: %s(%s). Error Creating Socket for ioctl.(%d)", mib_str_buf, ifname, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill mib string ***/
	wrq.u.data.pointer = (caddr_t)mib_str_buf;
	wrq.u.data.length = strlen(mib_str_buf)+1;

	/*** set mib ***/
	if(ioctl(sock, RTL8192CD_IOCTL_SET_MIB, &wrq) < 0) {	
		err = errno;
		CWLog("set_mib: %s(%s). Error to ioctl.(%d)", mib_str_buf, ifname, err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}


static int ifconfig_up(const char *ifname)
{
	struct ifreq ifr;
	int sk;

	sk = socket(PF_INET, SOCK_DGRAM, 0);
	if (sk < 0) return errno;

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

	if (ioctl(sk, SIOCGIFFLAGS, &ifr) < 0) {
		close(sk);
		return errno;
	}

	if (ifr.ifr_flags & IFF_UP) {
		close(sk);
		return EALREADY;
	}

	ifr.ifr_flags |= IFF_UP;

	if (ioctl(sk, SIOCSIFFLAGS, &ifr) < 0) {
		close(sk);
		return errno;
	}

	close(sk);
	return 0;
}


static int ifconfig_down(const char *ifname)
{
	struct ifreq ifr;
	int sk;

	sk = socket(PF_INET, SOCK_DGRAM, 0);
	if (sk < 0) return errno;

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

	if (ioctl(sk, SIOCGIFFLAGS, &ifr) < 0) 
	{
		close(sk);
		return errno;
	}

	if (!(ifr.ifr_flags & IFF_UP))
	{
		return EALREADY;
	}

	ifr.ifr_flags &= ~IFF_UP;

	if (ioctl(sk, SIOCSIFFLAGS, &ifr) < 0)
	{
		close(sk);
		return errno;
	}

	close(sk);
	return 0;
}

#ifndef X86_SIMULATION
static int ieee80211_net80211_getwpaie(const char *ifname, const unsigned char *mac, unsigned char *wpaie_buf, uint32_t buf_len)
{
	int sock;	
	struct iwreq wrq;

	memcpy(wpaie_buf, mac, 6);
	CWDebugLog("(%s)get wpa ie for mac %02x:%02x:%02x:%02x:%02x:%02x", 
				ifname, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		CWLog("ieee80211_net80211_getwpaie: Error creating socket for ioctl(%02x:%02x:%02x:%02x:%02x:%02x)", 
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]); 
		return errno;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill mib string ***/
	wrq.u.data.pointer = (caddr_t)wpaie_buf;
	wrq.u.data.length = buf_len;
	
	if(ioctl(sock, IEEE80211_IOCTL_GET_APPIEBUF, &wrq) < 0) {	
		CWLog("ieee80211_net80211_getwpaie: Error when ioctl(%02x:%02x:%02x:%02x:%02x:%02x)", 
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]); 
		close(sock);
		return errno;
	}		
	
	close(sock);
	return 0;
}

static int ieee80211_set_param(const char *ifname, int mode, int value)
{
	int sock;	
	struct iwreq wrq;
		
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		CWLog("ieee80211_set_param: Error Creating Socket for ioctl"); 
		return errno;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);
	wrq.u.mode = mode;
	memcpy(wrq.u.name+sizeof(int), &value, sizeof(value));
	
	if(ioctl(sock, IEEE80211_IOCTL_SETPARAM, &wrq) < 0) {	
		perror("ieee80211_set_param: Ioctl error");
		close(sock);
		return errno;
	}		
	
	close(sock);
	return 0;
}

static int ieee80211_set_group_key(const char *ifname, uint8_t ik_type, unsigned short ik_keyix, 
										uint8_t ik_keylen, const uint8_t *ik_keydata)
{

	int sock;	
	struct iwreq wrq;
	struct ieee80211req_key wkey;
		
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		CWLog("ieee80211_set_param: Error Creating Socket for ioctl"); 
		return errno;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);
	wrq.u.data.pointer = &wkey;
	wrq.u.data.length = sizeof(struct ieee80211req_key);

	wkey.ik_type = ik_type;
	wkey.ik_keyix = ik_keyix;
	wkey.ik_keylen = ik_keylen;
	memset(wkey.ik_macaddr, 0xFF, 6); // Milticase address
	memcpy(wkey.ik_keydata, ik_keydata, ik_keylen);

	if(ioctl(sock, IEEE80211_IOCTL_SETKEY, &wrq) < 0) {	
		perror("ieee80211_set_group_key: Ioctl error");
		close(sock);
		return errno;
	}		

	close(sock);
	return 0;

}

/* set basic config for a wlan through HAPD_IOCTL_SETCONFIG */
static int set_hapd_config(const char *ifname, const struct rtk_hapd_config* pCfg)
{

	int sock;	
	struct iwreq wrq;
	int err;
	struct rtk_hapd_config cfg;

	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWLog("set_basic_config: %s. Error Creating Socket for ioctl.(%d)", ifname, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** fill cfg buffer ***/
    wrq.u.data.pointer = &cfg;
    wrq.u.data.length = sizeof(cfg);
	cfg = *pCfg;

	/*** set  ***/
	if(ioctl(sock, HAPD_IOCTL_SETCONFIG, &wrq) < 0) {	
		err = errno;
		CWLog("set_basic_config: %s. Error to ioctl.(%d)", ifname, err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}

static int get_wlan_site_survey_request(const char *ifname, char *result)
{
	int sock;	
	struct iwreq wrq;
	int err;

	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWLog("%s(%s): Can't create socket for ioctl.(%d)", __FUNCTION__, ifname, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** give parameter and buffer ***/
    wrq.u.data.pointer = (caddr_t)result;
    wrq.u.data.length = 1;

	/*** ioctl ***/
	if(ioctl(sock, SIOCGIWRTLSCANREQ, &wrq) < 0) { 
		err = errno;
		CWLog("%s(%s): ioctl Error.(%d)", __FUNCTION__, ifname, err);
		goto out;
	}
	err = 0;
	
out:	
	close(sock);
	return err;
}

static int get_wlan_site_survey_result(const char *ifname, SS_STATUS_Tp pStatus )
{

	int sock;	
	struct iwreq wrq;
	int err;
	int i;
	SS_STATUS_T res_status1;

	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWLog("%s(%s): Can't create socket for ioctl.(%d)", __FUNCTION__, ifname, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

	/*** give parameter and buffer ***/
    wrq.u.data.pointer = (caddr_t)pStatus;
    if ( pStatus->number == 0 )	// query all
    	wrq.u.data.length = sizeof(SS_STATUS_T);
    else	// only request request status
        wrq.u.data.length = sizeof(pStatus->number);

	/*** ioctl ***/
	if(ioctl(sock, SIOCGIWRTLGETBSSDB, &wrq) < 0) {	
		err = errno;
		CWLog("%s(%s): ioctl Error.(%d)", __FUNCTION__, ifname, err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}

static int get_wlan_station_info (const char *ifname, wlan_stations_info_t *pInfo)
{
	int sock;	
	struct iwreq wrq;
	int err;

	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		CWLog("%s(%s): Can't create socket for ioctl.(%d)", __FUNCTION__, ifname, err); 
		goto out;
	}
	
	/*** Inizializzazione struttura iwreq ***/
	memset(&wrq, 0, sizeof(wrq));
	strncpy(wrq.ifr_name, ifname, IFNAMSIZ);
	/*** give parameter and buffer ***/

    memset(pInfo, 0, sizeof(wlan_stations_info_t));
    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = sizeof(wlan_stations_info_t);

	/*** ioctl ***/
	if(ioctl(sock, SIOCGIWRTLSTAINFO, &wrq) < 0) { 
		err = errno;
		CWLog("%s(%s): ioctl Error.(%d)", __FUNCTION__, ifname, err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}

/**************** netlink ****************/
#if 0
void rtk_driver_interaction_snd_frame(void *buf, int len)
{
	struct sockaddr_nl dst_addr;
	struct iovec iov;	
	struct nlmsghdr *nlh;
	struct msghdr msg;
	int nl_len;

	/* destination address */
	memset(&dst_addr, 0 ,sizeof(struct sockaddr_nl));
	dst_addr.nl_family = AF_NETLINK ;
	dst_addr.nl_pid = 0; /* destined to kernel */
	dst_addr.nl_groups = 0;

	/* Fill the netlink message header */
	nl_len = NLMSG_SPACE(len);
	nlh = (struct nlmsghdr *)malloc(nl_len);
	memset(nlh , 0 , nl_len);
	memcpy(NLMSG_DATA(nlh), buf, len);
	nlh->nlmsg_len = len;
	nlh->nlmsg_pid = gNetlinkSrcAddr.nl_pid;
	nlh->nlmsg_flags = 0;
	nlh->nlmsg_type = 0;

	/*iov structure */
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;

	/* msg */
	memset(&msg,0,sizeof(msg));
	msg.msg_name = (void *) &dst_addr ;
	msg.msg_namelen=sizeof(dst_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	/* send */
	sendmsg(gNetlinkSock, &msg, 0);


}
#endif



//  interaction with flash 
static CWBool get_flash_wlan_mac_addr(const char *ifname, unsigned char *mac_address)
{
	const char *p_radio_id, *p_dash, *p_wlan_idx;
	extern int wlan_idx; // global variable in apmib to index HW MIB, the same as the radio id of capwap, not the wlan_idx of capwap
	int mib_id;

	p_radio_id = ifname + sizeof("wlan");	
	p_dash = ifname + sizeof("wlan0");	
	p_wlan_idx = ifname + sizeof("wlan0-va");


	// get radio_id
	if (strncmp(ifname, "wlan", sizeof("wlan")) != 0) return CW_FALSE;
	if ( *p_radio_id < '0' || *p_radio_id > ('0'+RTK_RADIOS-1) ) return CW_FALSE;
	wlan_idx = *p_radio_id - '0'; // radio id
	
	// get capwap wlan_idx
	if (*p_dash == '\0') { // wlan0 or wlan1
		mib_id = MIB_HW_WLAN_ADDR;
	} else if (strncmp(p_dash, "-va", sizeof("-va")) == 0) {
		if ( *p_wlan_idx < '0' || *p_wlan_idx > ('0'+RTK_VAPS-1) ) return CW_FALSE;
		mib_id = *p_wlan_idx - '0' + MIB_HW_WLAN_ADDR1;
	} else {
		return CW_FALSE;
	}

	apmib_get(mib_id, mac_address);
	return CW_TRUE;
}

static CWBool get_flash_value(const char *item, int *value)
{
	char cmd[128], buf[128];
	int n;
	FILE *fp=NULL;
	sprintf(cmd, "rm -rf /var/tmp/flashval; flash get %s > /var/tmp/flashval", item);
	if(access("/var/tmp/flashval", R_OK) == -1) {
		return CW_FALSE;
	}
	fp = fopen("/var/tmp/flashval", "r");
	if (!fp) {
		return CW_FALSE;
	}

	n = fread(buf, 1, sizeof buf, fp);
	if(strncmp(buf, item, strlen(item)) == 0 && buf[strlen(item)] == '=' && 
		buf[strlen(item)+1]>='0' && buf[strlen(item)+1]<='9') { // TODO Babylon: should not be as this....need to considere blanks.
		*value = atoi(&buf[strlen(item)+1]);
		if(fp!=NULL)
		{
			CWLog("%s:%d####fclose(fp)!!!!",__FUNCTION__,__LINE__);
			fclose(fp);
		}
		return CW_TRUE;
	}
	if(fp!=NULL)
	{
		CWLog("%s:%d####fclose(fp)!!!!",__FUNCTION__,__LINE__);
		fclose(fp);
	}
	return CW_FALSE;

}

static CWBool get_flash_default_band(int radio_id, enum NETWORK_TYPE *band)
{
	char item[128];
	sprintf(item, "DEF_WLAN%d_BAND", radio_id);
	return get_flash_value(item, (int *)band);
}

static CWBool get_flash_default_band_select(int radio_id, PHY_BAND_SELECT* band_select)
{
	char item[128];
	sprintf(item, "DEF_WLAN%d_PHY_BAND_SELECT", radio_id);
	return get_flash_value(item, (int *)band_select);
}

static int get_flash_wtp_id()
{
	int id;
	apmib_get(MIB_CAPWAP_WTP_ID, &id);
	return id;
}

static unsigned int get_flash_ac_ip()
{
	unsigned int ip;
	apmib_get(MIB_CAPWAP_AC_IP, &ip);
	return ip;
}

#ifdef RTK_AUTO_AC
static int get_auto_wtp_id()
{
	int id;
	int i;
	unsigned char ipBuffer[4]={0}, smBuffer[4]={0}, acBuffer[4]={0};
	struct in_addr	intaddr;
					
	char ip[32];
	int intAC_IP;
				
	getInAddr(BR_NAME, IP_ADDR, (void *)&intaddr);
	sprintf(ip,"%s",inet_ntoa(intaddr));
	iptoint(ipBuffer,ip);
		
	id=ipBuffer[3];
	return id;

}

static unsigned int get_auto_ac_ip()
{
	unsigned char ipBuffer[4]={0}, smBuffer[4]={0}, acBuffer[4]={0};
	char cmd[128];
	struct in_addr	intaddr;
	char ip[32];
	char sm[32];
	unsigned int intAC_IP;
	int i;

	getInAddr(BR_NAME, IP_ADDR, (void *)&intaddr);
	sprintf(ip,"%s",inet_ntoa(intaddr));
	iptoint(ipBuffer,ip);

	getInAddr(BR_NAME, SUBNET_MASK, (void *)&intaddr);
	sprintf(sm,"%s",inet_ntoa(intaddr));	
	iptoint(smBuffer,sm);

	if(!gMesh_portal) {
		getGatewayIpAddr();
		iptoint(acBuffer,gGatewayIp);
	} else {
		for (i=0;i<4;i++){
			if (smBuffer[i]<255){
				acBuffer[i]=255;
			}
			else{
				acBuffer[i]=ipBuffer[i];
			}
		}
	}
	intAC_IP=acBuffer[0]*16777216+acBuffer[1]*65536+acBuffer[2]*256+acBuffer[3];

	return intAC_IP;
}
#endif

/************************** Public APIs: Set To Driver Only *********************/
// WLAN Configuration
//TODO reinit by FLASH MIB DEFAULT
static const struct rtk_hapd_config wlan_default_setting_5G_on = 
{

	.band	= WIRELESS_11A | WIRELESS_11N | WIRELESS_11AC, 	// priv->pmib->dot11BssType.net_work_type
	.channel	= 153 ,											// priv->pmib->dot11RFEntry.dot11channel	TODO
	.bcnint	= 100,											// priv->pmib->dot11StationConfigEntry.dot11BeaconPeriod 	  
	.dtimperiod	= 1,										// priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod
	.stanum = 0,												// priv->pmib->dot11StationConfigEntry.supportedStaNum
	.rtsthres = 2347,										// priv->pmib->dot11OperationEntry.dot11RTSThreshold		       
	.fragthres = 2346,										// priv->pmib->dot11OperationEntry.dot11FragmentationThreshold  TODO
	.oprates = 0,											// priv->pmib->dot11StationConfigEntry.dot11SupportedRates   	
	.basicrates = 0,											// priv->pmib->dot11StationConfigEntry.dot11BasicRates 	  	
	.preamble = 0,											// priv->pmib->dot11RFEntry.shortpreamble	
	.aclmode = 0,											// priv->pmib->dot11StationConfigEntry.dot11AclMode	 	
	.aclnum = 0,												// priv->pmib->dot11StationConfigEntry.dot11AclNum 	  	
	//.acladdr											 	=> priv->pmib->dot11StationConfigEntry.dot11AclAddr
	.hiddenAP = 0,											// priv->pmib->dot11OperationEntry.hiddenAP
	.qos_enable = 1,											// priv->pmib->dot11QosEntry.dot11QosEnable
	.expired_time = 300,										// priv->pmib->dot11OperationEntry.expiretime / 100
	.block_relay	= 0,										// priv->pmib->dot11OperationEntry.block_relay
	.shortGI20M = 1,											// priv->pmib->dot11nConfigEntry.dot11nShortGIfor20M
	.shortGI40M = 1,											// priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M

	//Above are for Hostapd owned configurations
	//=====================================================
	//Below are for RTK private configurations
	.pwrlevelCCK_A = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrlevelCCK_A
	.pwrlevelCCK_B = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrlevelCCK_B
	.pwrlevelHT40_1S_A = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//priv->pmib->dot11RFEntry.pwrlevelHT40_1S_A
	.pwrlevelHT40_1S_B = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//priv->pmib->dot11RFEntry.pwrlevelHT40_1S_B
	.pwrdiffHT40_2S = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrdiffHT40_2S
	.pwrdiffHT20 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrdiffHT20
	.pwrdiffOFDM = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrdiffOFDM
	.ther = 35,												//priv->pmib->dot11RFEntry.ther			TODO
	.phyBandSelect = PHY_BAND_5G,							//priv->pmib->dot11RFEntry.phyBandSelect
	.swcrypto = 0,											// priv->pmib->dot11StationConfigEntry.dot11swcrypto
	.regdomain = 1,											// priv->pmib->dot11StationConfigEntry.dot11RegDomain
	.autorate = 1, 											// priv->pmib->dot11StationConfigEntry.autoRate
	.fixrate = 0,											// priv->pmib->dot11StationConfigEntry.fixedTxRate
	.disable_protection = 1,									// priv->pmib->dot11StationConfigEntry.protectionDisabled
	.disable_olbc = 0,										// priv->pmib->dot11StationConfigEntry.olbcDetectDisabled
	.deny_legacy = 0, // enum NETWORK_TYPE 					//priv->pmib->dot11StationConfigEntry.legacySTADeny
	.opmode = 0X10, //WIFI_AP_STATE							//priv->pmib->dot11OperationEntry.opmode	
	.use40M	= 0,											// priv->pmib->dot11nConfigEntry.dot11nUse40M	
	._2ndchoffset = 0, //HT_2NDCH_OFFSET_DONTCARE				// priv->pmib->dot11nConfigEntry.dot11n2ndChOffset 
	.ampdu 		= 0,										// priv->pmib->dot11nConfigEntry.dot11nAMPDU	  
	.guest_access= 0,										// priv->pmib->dot11OperationEntry.guest_access
	.macPhyMode	= 0,										// priv->pmib->dot11RFEntry.macPhyMode 			TODO
	.coexist     = 0,										//priv->pmib->dot11nConfigEntry.dot11nCoexist
	.vap_enable  = 1,// This must be enable for root interfaces!!!		//priv->pmib->miscEntry.vap_enable
	.rssi_dump   = 0,										//priv->pshare->rf_ft_var.rssi_dump
	.mp_specific = 0,										//priv->pshare->rf_ft_var.mp_specific 		TODO
	.use_ext_pa  = 0,										//priv->pshare->rf_ft_var.use_ext_pa 		TODO

};

static const struct rtk_hapd_config wlan_default_setting_5G_off = 
{
	.band	= WIRELESS_11A | WIRELESS_11N | WIRELESS_11AC, 	// priv->pmib->dot11BssType.net_work_type
	.channel	= 153 ,											// priv->pmib->dot11RFEntry.dot11channel	TODO
	.bcnint	= 100,											// priv->pmib->dot11StationConfigEntry.dot11BeaconPeriod 	  
	.dtimperiod	= 1,										// priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod
	.stanum = 0,												// priv->pmib->dot11StationConfigEntry.supportedStaNum
	.rtsthres = 2347,										// priv->pmib->dot11OperationEntry.dot11RTSThreshold		       
	.fragthres = 2347,										// priv->pmib->dot11OperationEntry.dot11FragmentationThreshold  TODO	
	.oprates = 0xfff,										// priv->pmib->dot11StationConfigEntry.dot11SupportedRates   	
	.basicrates = 0xf, 										// priv->pmib->dot11StationConfigEntry.dot11BasicRates	
	.preamble = 0,											// priv->pmib->dot11RFEntry.shortpreamble
	.aclmode = 0,											// priv->pmib->dot11StationConfigEntry.dot11AclMode 	
	.aclnum = 0, 											// priv->pmib->dot11StationConfigEntry.dot11AclNum		
	//.acladdr 												=> priv->pmib->dot11StationConfigEntry.dot11AclAddr
	.hiddenAP = 0,											// priv->pmib->dot11OperationEntry.hiddenAP
	.qos_enable = 0, 										// priv->pmib->dot11QosEntry.dot11QosEnable
	.expired_time = 300, 									// priv->pmib->dot11OperationEntry.expiretime / 100
	.block_relay = 0,										// priv->pmib->dot11OperationEntry.block_relay
	.shortGI20M = 0, 										// priv->pmib->dot11nConfigEntry.dot11nShortGIfor20M
	.shortGI40M = 0, 										// priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M
	
	//Above are for Hostapd owned configurations
	//=====================================================
	//Below are for RTK private configurations
	
	.pwrlevelCCK_A = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrlevelCCK_A
	.pwrlevelCCK_B = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrlevelCCK_B
	.pwrlevelHT40_1S_A = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//priv->pmib->dot11RFEntry.pwrlevelHT40_1S_A
	.pwrlevelHT40_1S_B = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},		//priv->pmib->dot11RFEntry.pwrlevelHT40_1S_B
	.pwrdiffHT40_2S = {0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 		//priv->pmib->dot11RFEntry.pwrdiffHT40_2S
	.pwrdiffHT20 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrdiffHT20
	.pwrdiffOFDM = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},			//priv->pmib->dot11RFEntry.pwrdiffOFDM
	.ther = 0,												//priv->pmib->dot11RFEntry.ther 		TODO
	.phyBandSelect = PHY_BAND_5G,							//priv->pmib->dot11RFEntry.phyBandSelect
	.swcrypto = 0,											// priv->pmib->dot11StationConfigEntry.dot11swcrypto
	.regdomain = 1,											// priv->pmib->dot11StationConfigEntry.dot11RegDomain
	.autorate = 1,											// priv->pmib->dot11StationConfigEntry.autoRate
	.fixrate = 0,											// priv->pmib->dot11StationConfigEntry.fixedTxRate
	.disable_protection = 0, 								// priv->pmib->dot11StationConfigEntry.protectionDisabled
	.disable_olbc = 0,										// priv->pmib->dot11StationConfigEntry.olbcDetectDisabled
	.deny_legacy = 0, // enum NETWORK_TYPE					//priv->pmib->dot11StationConfigEntry.legacySTADeny
	.opmode = 0X10, //WIFI_AP_STATE							//priv->pmib->dot11OperationEntry.opmode	
	.use40M	= 0,											// priv->pmib->dot11nConfigEntry.dot11nUse40M	
	._2ndchoffset = 0, //HT_2NDCH_OFFSET_DONTCARE 			// priv->pmib->dot11nConfigEntry.dot11n2ndChOffset 
	.ampdu		= 0,										// priv->pmib->dot11nConfigEntry.dot11nAMPDU	  
	.guest_access= 0,										// priv->pmib->dot11OperationEntry.guest_access
	.macPhyMode	= 0,										// priv->pmib->dot11RFEntry.macPhyMode			TODO
	.coexist 	= 0,										//priv->pmib->dot11nConfigEntry.dot11nCoexist
	.vap_enable	= 1,										//priv->pmib->miscEntry.vap_enable

	.rssi_dump   = 0,										//priv->pshare->rf_ft_var.rssi_dump
	.mp_specific = 0,										//priv->pshare->rf_ft_var.mp_specific 		TODO
	.use_ext_pa  = 0,										//priv->pshare->rf_ft_var.use_ext_pa 		TODO
	
};

static const struct rtk_hapd_config wlan_default_setting_2G_on = 
{
	.band	= WIRELESS_11B | WIRELESS_11G | WIRELESS_11N, 	// priv->pmib->dot11BssType.net_work_type	
	.channel	= 10,											// priv->pmib->dot11RFEntry.dot11channel	TODO	
	.bcnint	= 100,											// priv->pmib->dot11StationConfigEntry.dot11BeaconPeriod
	.dtimperiod	= 1,										// priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod	
	.stanum = 0,												// priv->pmib->dot11StationConfigEntry.supportedStaNum	
	.rtsthres = 2347,										// priv->pmib->dot11OperationEntry.dot11RTSThreshold		
	.fragthres = 2346,										// priv->pmib->dot11OperationEntry.dot11FragmentationThreshold  TODO	
	.oprates = 0,											// priv->pmib->dot11StationConfigEntry.dot11SupportedRates   
	.basicrates = 0, 										// priv->pmib->dot11StationConfigEntry.dot11BasicRates		
	.preamble = 0,											// priv->pmib->dot11RFEntry.shortpreamble	
	.aclmode = 0,											// priv->pmib->dot11StationConfigEntry.dot11AclMode 	
	.aclnum = 0, 											// priv->pmib->dot11StationConfigEntry.dot11AclNum		
	//.acladdr 												=> priv->pmib->dot11StationConfigEntry.dot11AclAddr
	.hiddenAP = 0,											// priv->pmib->dot11OperationEntry.hiddenAP
	.qos_enable = 1, 										// priv->pmib->dot11QosEntry.dot11QosEnable
	.expired_time = 300, 									// priv->pmib->dot11OperationEntry.expiretime / 100
	.block_relay = 0,										// priv->pmib->dot11OperationEntry.block_relay
	.shortGI20M = 1, 										// priv->pmib->dot11nConfigEntry.dot11nShortGIfor20M
	.shortGI40M = 1, 										// priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M
	
	//Above are for Hostapd owned configurations
	//=====================================================
	//Below are for RTK private configurations
	
	.pwrlevelCCK_A = {0x20,0x20,0x20,0x21,0x21,0x21,0x21,0x21,0x21,0x23,0x23,0x23,0x23,0x23},	//priv->pmib->dot11RFEntry.pwrlevelCCK_A
	.pwrlevelCCK_B = {0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23},	//priv->pmib->dot11RFEntry.pwrlevelCCK_B
	.pwrlevelHT40_1S_A = {0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x24,0x24,0x24,0x24,0x24},//priv->pmib->dot11RFEntry.pwrlevelHT40_1S_A
	.pwrlevelHT40_1S_B = {0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24},//priv->pmib->dot11RFEntry.pwrlevelHT40_1S_B
	.pwrdiffHT40_2S = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},												//priv->pmib->dot11RFEntry.pwrdiffHT40_2S
	.pwrdiffHT20 = {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22},		//priv->pmib->dot11RFEntry.pwrdiffHT20
	.pwrdiffOFDM = {0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44},		//priv->pmib->dot11RFEntry.pwrdiffOFDM

	.ther = 33,												//priv->pmib->dot11RFEntry.ther			TODO
	.phyBandSelect = PHY_BAND_2G,							//priv->pmib->dot11RFEntry.phyBandSelect
	.swcrypto = 0,											// priv->pmib->dot11StationConfigEntry.dot11swcrypto
	.regdomain = 1,											// priv->pmib->dot11StationConfigEntry.dot11RegDomain
	.autorate = 1,											// priv->pmib->dot11StationConfigEntry.autoRate
	.fixrate = 0,											// priv->pmib->dot11StationConfigEntry.fixedTxRate
	.disable_protection = 1, 								// priv->pmib->dot11StationConfigEntry.protectionDisabled
	.disable_olbc = 0,										// priv->pmib->dot11StationConfigEntry.olbcDetectDisabled
	.deny_legacy = 0, // enum NETWORK_TYPE					//priv->pmib->dot11StationConfigEntry.legacySTADeny
	.opmode = 0X10, //WIFI_AP_STATE							//priv->pmib->dot11OperationEntry.opmode	
	.use40M	= 0,											// priv->pmib->dot11nConfigEntry.dot11nUse40M	
	._2ndchoffset = 0, //HT_2NDCH_OFFSET_DONTCARE 			// priv->pmib->dot11nConfigEntry.dot11n2ndChOffset 
	.ampdu		= 0,										// priv->pmib->dot11nConfigEntry.dot11nAMPDU	  
	.guest_access= 0,										// priv->pmib->dot11OperationEntry.guest_access
	.macPhyMode	= 0,										// priv->pmib->dot11RFEntry.macPhyMode			TODO
	.coexist 	= 0,										//priv->pmib->dot11nConfigEntry.dot11nCoexist
	.vap_enable	= 1,										//priv->pmib->miscEntry.vap_enable
	
	.rssi_dump	= 0,										//priv->pshare->rf_ft_var.rssi_dump
	.mp_specific = 0,										//priv->pshare->rf_ft_var.mp_specific		TODO
	.use_ext_pa	= 0,										//priv->pshare->rf_ft_var.use_ext_pa		TODO

};

static const struct rtk_hapd_config wlan_default_setting_2G_off = 
{
	.band	= WIRELESS_11B | WIRELESS_11G | WIRELESS_11N, 	// priv->pmib->dot11BssType.net_work_type
	.channel	= 153 ,											// priv->pmib->dot11RFEntry.dot11channel	TODO
	.bcnint	= 100,											// priv->pmib->dot11StationConfigEntry.dot11BeaconPeriod 	  
	.dtimperiod	= 1,										// priv->pmib->dot11StationConfigEntry.dot11DTIMPeriod
	.stanum = 0,												// priv->pmib->dot11StationConfigEntry.supportedStaNum
	.rtsthres = 2347,										// priv->pmib->dot11OperationEntry.dot11RTSThreshold		       
	.fragthres = 2347,										// priv->pmib->dot11OperationEntry.dot11FragmentationThreshold  TODO
	.oprates = 0xfff,										// priv->pmib->dot11StationConfigEntry.dot11SupportedRates		
	.basicrates = 0xf,										// priv->pmib->dot11StationConfigEntry.dot11BasicRates		
	.preamble = 0,											// priv->pmib->dot11RFEntry.shortpreamble	
	.aclmode = 0,											// priv->pmib->dot11StationConfigEntry.dot11AclMode 	
	.aclnum = 0, 											// priv->pmib->dot11StationConfigEntry.dot11AclNum		
	//.acladdr 												=> priv->pmib->dot11StationConfigEntry.dot11AclAddr
	.hiddenAP = 0,											// priv->pmib->dot11OperationEntry.hiddenAP
	.qos_enable = 0, 										// priv->pmib->dot11QosEntry.dot11QosEnable
	.expired_time = 300, 									// priv->pmib->dot11OperationEntry.expiretime / 100
	.block_relay = 0,										// priv->pmib->dot11OperationEntry.block_relay
	.shortGI20M = 0, 										// priv->pmib->dot11nConfigEntry.dot11nShortGIfor20M
	.shortGI40M = 0, 										// priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M	
		
	//Above are for Hostapd owned configurations
	//=====================================================
	//Below are for RTK private configurations

	.pwrlevelCCK_A = {0x20,0x20,0x20,0x21,0x21,0x21,0x21,0x21,0x21,0x23,0x23,0x23,0x23,0x23},	//priv->pmib->dot11RFEntry.pwrlevelCCK_A
	.pwrlevelCCK_B = {0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23},	//priv->pmib->dot11RFEntry.pwrlevelCCK_B
	.pwrlevelHT40_1S_A = {0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x24,0x24,0x24,0x24,0x24},//priv->pmib->dot11RFEntry.pwrlevelHT40_1S_A
	.pwrlevelHT40_1S_B = {0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24},//priv->pmib->dot11RFEntry.pwrlevelHT40_1S_B
	.pwrdiffHT40_2S = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},												//priv->pmib->dot11RFEntry.pwrdiffHT40_2S
	.pwrdiffHT20 = {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22},		//priv->pmib->dot11RFEntry.pwrdiffHT20
	.pwrdiffOFDM = {0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44},		//priv->pmib->dot11RFEntry.pwrdiffOFDM

	.ther = 33,												//priv->pmib->dot11RFEntry.ther			TODO
	.phyBandSelect = PHY_BAND_2G,							//priv->pmib->dot11RFEntry.phyBandSelect
	.swcrypto = 0,											// priv->pmib->dot11StationConfigEntry.dot11swcrypto
	.regdomain = 1,											// priv->pmib->dot11StationConfigEntry.dot11RegDomain
	.autorate = 1,											// priv->pmib->dot11StationConfigEntry.autoRate
	.fixrate = 0,											// priv->pmib->dot11StationConfigEntry.fixedTxRate
	.disable_protection = 0, 								// priv->pmib->dot11StationConfigEntry.protectionDisabled
	.disable_olbc = 0,										// priv->pmib->dot11StationConfigEntry.olbcDetectDisabled
	.deny_legacy = 0, // enum NETWORK_TYPE						//priv->pmib->dot11StationConfigEntry.legacySTADeny
	.opmode = 0X10, //WIFI_AP_STATE							//priv->pmib->dot11OperationEntry.opmode	
	.use40M	= 0,											// priv->pmib->dot11nConfigEntry.dot11nUse40M	
	._2ndchoffset = 0, //HT_2NDCH_OFFSET_DONTCARE 				// priv->pmib->dot11nConfigEntry.dot11n2ndChOffset 
	.ampdu		= 0,										// priv->pmib->dot11nConfigEntry.dot11nAMPDU	  
	.guest_access= 0,										// priv->pmib->dot11OperationEntry.guest_access
	.macPhyMode	= 0,										// priv->pmib->dot11RFEntry.macPhyMode			TODO
	.coexist 	= 0,										//priv->pmib->dot11nConfigEntry.dot11nCoexist
	.vap_enable	= 1,										//priv->pmib->miscEntry.vap_enable

	.rssi_dump   = 0,										//priv->pshare->rf_ft_var.rssi_dump
	.mp_specific = 0,										//priv->pshare->rf_ft_var.mp_specific 		TODO
	.use_ext_pa  = 0,										//priv->pshare->rf_ft_var.use_ext_pa 		TODO

};

#ifdef KM_BY_AC
struct wlancfg{
	CWBool			is_active;	// added by AC, and in use.
	CWProtocolAddWLANValues	*cwElemAddWlan;
	struct rtk_hapd_config	*rtk_basic_config;
	unsigned char bssid[6];
	// Babylon TODO IEs...
};
// ===================================

struct wlancfg gWLANCfg[RTK_RADIOS][RTK_VAPS+1];



CWBool cwRtkWtpLookupWlanByMac(const unsigned char *mac_address, int *radio_id, int *wlan_idx)
{
	int i, j;
	struct wlancfg *pWlancfg;
	for (i=0; i<RTK_RADIOS; i++) {
		for (j=0; j<RTK_VAPS+1; j++) {
			pWlancfg = &gWLANCfg[i][j];
			if (pWlancfg->is_active && memcpy(pWlancfg->bssid, mac_address, 6) == 0) {
				*radio_id = i;
				*wlan_idx = j;
				return CW_TRUE;
			}
		}
	}
	return CW_FALSE;
}
/* Function: cwRtkDriverWtpAddWlan()
  * Description: This function saves the Configuration (Add a Wlan), and tries to activate it.
  *                   pAddWlanElem will be saved, please do NOT free this pointer if return CW_PROTOCOL_SUCCESS!
  */
CWProtocolResultCode cwRtkWtpAddWlan(CWProtocolAddWLANValues* pAddWlan, unsigned char *bssid)
{
	struct wlancfg *pWlancfg = &gWLANCfg[pAddWlan->radio_id][pAddWlan->wlan_id];
	const char *ifname = rtk_ifname[pAddWlan->radio_id][pAddWlan->wlan_id];
	int err;
	CWProtocolResultCode ret;
	char tmp_str[64];
	//unsigned char mac[6];
	PHY_BAND_SELECT band_select;
	
	if (pWlancfg->cwElemAddWlan != NULL && pWlancfg->is_active)	{
		return CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED; // service will not be provided.
	}

	if (pAddWlan->radio_id>=RTK_RADIOS || pAddWlan->wlan_id >= RTK_VAPS+1) {
		return CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED; // service will not be provided.
	}

	if (!get_flash_wlan_mac_addr(ifname, bssid)) 	{
			return CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED; // service will not be provided.
	}

	if (!get_flash_default_band_select(pAddWlan->radio_id, &band_select)) {
			return CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED; // service will not be provided.
	}
	
	//CWThreadMutexLock(&gAtomicCfgAction);	
	
		pWlancfg->is_active = CW_FALSE;
		memcpy(pWlancfg->bssid, bssid, 6);
		pWlancfg->cwElemAddWlan= pAddWlan;
		pWlancfg->rtk_basic_config = malloc(sizeof(struct rtk_hapd_config));
		if (band_select == PHY_BAND_2G) {
			*pWlancfg->rtk_basic_config = wlan_default_setting_2G_on;
		} else if (band_select == PHY_BAND_5G) {	
			*pWlancfg->rtk_basic_config = wlan_default_setting_5G_on;
		} else {		
			CWLog("radio %d: band error band=%d", band_select, pAddWlan->radio_id);
			goto err_out;
		}
		
		// TODO: include config from AC (Configuration Request)
		

		CWDebugLog("Trying to add WLAN configuration: ifname: %s", ifname);		

		/* save to driver
		  * (1) ifconfig down
		  */		  
		err = ifconfig_down(ifname);
		if (err!=0 && err!=EALREADY) {
			CWLog("ifconfig down Err: %d", err);
			goto err_out;
		}

		/* (2) set basic config: struct rtk_hapd_config
		  */
		err = set_hapd_config(ifname, pWlancfg->rtk_basic_config);
		if (err!=0) {
			CWLog("set basic config Err: %d", err);		
			goto err_out;
		}

		/* (3) SSID: 
		  *			(GET_MIB(priv))->dot11OperationEntry.hiddenAP
		  */
		err = set_mib(ifname, "ssid", pAddWlan->ssid);
		if (err!=0) {
			CWLog("set mib Err: %d", err);		
			goto err_out;
		}

		/* (3) IEEE80211_PARAM_AUTHMODE
		  *	   pmib->dot1180211AuthEntry.dot11AuthAlgrthm 
		  *	   pmib->dot1180211AuthEntry.dot11PrivacyAlgrthm
		  *	   pmib->dot1180211AuthEntry.dot11EnablePSK
		  *	   pmib->dot118021xAuthEntry.dot118021xAlgrthm
		  *
		  *     Note: WEP when interface down: will modify dot11DefaultKeysTable
		  */
		if (pAddWlan->key_len == 0){
			err = ieee80211_set_param(ifname, IEEE80211_PARAM_AUTHMODE, IEEE80211_AUTH_NONE); 
			/* Note: do NOT set  key, please! */
		} else if ( pAddWlan->key_status == CW_WLAN_KEY_STATUS_SHARED_WEP) {
			/* Note: for WEP,  dot11PrivacyAlgrthm is not set until setting group key, because of key length. */
			switch (pAddWlan->auth_type)
			{
			case CW_AUTH_TYPE_OPEN_SYSTEM:
				//printf("WEP Open %s\n", ifname);
				err = ieee80211_set_param(ifname, IEEE80211_PARAM_AUTHMODE, IEEE80211_AUTH_OPEN);
				break;;
			case CW_AUTH_TYPE_PSK:
				//printf("WEP PSK %s\n", ifname);
				err = ieee80211_set_param(ifname, IEEE80211_PARAM_AUTHMODE, IEEE80211_AUTH_SHARED);
				break;;
			default:
				//printf("Unkown auth type: %u", pAddWlan->auth_type);
				err = EPERM;
				break;
			}
		} else {
			 // TODO
			//printf("Currently only support WEP: unspported key status: %u", pAddWlan->key_status);	
			err = EPERM;
		}
		if (err!=0)	{
			//printf(" set param Err: %d", err);		
			ret = CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED;
			goto err_out;
		}

		
		/* (4) set key for WEP/WPA/WPA2
		  *	   pmib->dot11GroupKeysTable.dot11Privacy
		  *	Note: for WEP, set default key and dot11PrivacyAlgrthm.
		  */
		if (pAddWlan->key_len == 0) {
			//Note: need NOT set key.
		} else if (pAddWlan->key_status == CW_WLAN_KEY_STATUS_SHARED_WEP) {  // WEP
			err = ieee80211_set_group_key(ifname, IEEE80211_CIPHER_WEP, pAddWlan->key_index, pAddWlan->key_len, pAddWlan->key);
			if (err != 0) {
				//printf("Err: WEP set key for %s: key_index=%u, key_len=%u, key[0]=%02x\n", ifname, pAddWlan->key_index, pAddWlan->key_len, pAddWlan->key[0]);
				goto err_out;
			}
			err = ieee80211_set_param(ifname, IEEE80211_PARAM_UCASTKEYLEN, 5); 
			if (err != 0) {
				//printf("Err: WEP set key len for %s: key_index=%u, key_len=%u, key[0]=%02x\n", ifname, pAddWlan->key_index, pAddWlan->key_len, pAddWlan->key[0]);
				goto err_out;
			}
		} else { // WPA or WPA 2
			//printf("Currently only support WEP: unspported key status: %u", pAddWlan->key_status);	
			err = EPERM;			
			goto err_out;
		}

		/* (5) only when WPA/WP2, (if with WPA/RSN IE in this WLAN configuration) set RSN IE -- DOT11_EVENT_SET_RSNIE  (TODO)
		* (xxx) originally, driver generate its RSN IE by:
		* (xxx.1) IEEE80211_PARAM_UCASTCIPHERS 
		*	pmib->dot1180211AuthEntry.dot11WPACipher
		*	pmib->dot1180211AuthEntry.dot11WPA2Cipher
		* (xxx.2) rsn_init() ==> generate  pmib->dot11RsnIE
		*/

		/* (6) add the interface into bridge */
		sprintf(tmp_str, "brctl addif %s %s", BR_NAME, ifname);
		err = system(tmp_str);
		if (err!=0 && err!=EALREADY) {
			//printf("brctl Err (%s): %d\n", tmp_str, err);		
			goto err_out;
		}


		/* (7) set wtp_pid */
		sprintf(tmp_str, "%u", gNetlinkSrcAddr.nl_pid);
		err = set_mib(ifname, "wtp_pid", tmp_str);
		if (err!=0) { 
			//printf("cannot set wtp_pid, %d\n", err);
			goto err_out;
		}

		/* (8) set mac address */
		sprintf(tmp_str, "ifconfig %s hw ether %02X:%02X:%02X:%02X:%02X:%02X", 
								ifname, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]); 
		err = system(tmp_str);
		if (err!=0) { 
			//printf("cannot set wtp_pid, %d\n", err);
			goto err_out;
		}
		
		/* (9) ifconfig up */
		err = ifconfig_up(ifname);
		if (err!=0 && err!=EALREADY) {
			//printf("ifconfig up Err: %d", err);		
			goto err_out;
		}

		pWlancfg->is_active = CW_TRUE;
		ret = CW_PROTOCOL_SUCCESS;
		goto out;
	/*	
		WTPDevIoctl(RTL8192CD_IOCTL_CW_ADDWLAN, 
						rtk_ifname[pWlanCfg->cfg_in_AddWlan->radio_id][pWlanCfg->cfg_in_AddWlan->wlan_id], 
						pWlancfg, sizeof(struct wlancfg));
		if (!pWlancfg->is_active) 
		{
			return CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED;
		}
	*/
err_out:
		//printf("Did not add (or fail to add) a WLAN configuration: ifname: %s\n", ifname);
		pWlancfg->is_active = CW_FALSE;
		pWlancfg->cwElemAddWlan = NULL;
		free(pWlancfg->rtk_basic_config);
		ret = CW_PROTOCOL_CFG_FAILURE_SERVICE_NOT_PROVIDED;
out:
	//CWThreadMutexUnlock(&gAtomicCfgAction);
	return ret;
	
}
#endif


#ifdef RTK_SMART_ROAMING
CWProtocolResultCode cwRtkWtpDeleteStation(int radio_id, unsigned char *sta_mac)
{
	int wlan_idx;

	for (wlan_idx=0; wlan_idx<CW_MAX_WLANS_PER_RADIO; wlan_idx++)
		delete_station(rtk_ifname[radio_id][wlan_idx], sta_mac);

		return CW_PROTOCOL_SUCCESS;
}

CWProtocolResultCode cwRtkWtpFreeStation(int radio_id, unsigned char *sta_mac)
{
	if(!radio_id)
		free_station("wlan0", sta_mac);
	else if (radio_id == 1)
		free_station("wlan1", sta_mac);
	
	return CW_PROTOCOL_SUCCESS;
}

CWProtocolResultCode cwRtkWtpSilentStation(int radio_id, unsigned char *sta_mac)
{
#ifdef RTK_DUAL_BAND_BLOCKING
	silent_station("wlan1", sta_mac);
	silent_station("wlan0", sta_mac);	
#else
	if(!radio_id)
		silent_station("wlan0", sta_mac);
	else
		silent_station("wlan1", sta_mac);
#endif


	return CW_PROTOCOL_SUCCESS;
}

CWProtocolResultCode cwRtkWtpUnsilentStation(int radio_id, unsigned char *sta_mac)
{
	if(!radio_id)
		unsilent_station("wlan0", sta_mac);
	else if (radio_id == 1)
		unsilent_station("wlan1", sta_mac);

	return CW_PROTOCOL_SUCCESS;
}

CWProtocolResultCode cwRtkWtpDot11vStation(int radio_id, unsigned char *sta_mac)
{
	if(!radio_id)
		dot11v_station("wlan0", sta_mac);
	else if(radio_id == 1)
		dot11v_station("wlan1", sta_mac);

		return CW_PROTOCOL_SUCCESS;
}

CWProtocolResultCode cwRtkWtpDualStation(int radio_id, unsigned char *sta_mac)
{
	/* dualband notify */
	dual_station("wlan0", sta_mac);
	dual_station("wlan1", sta_mac);

	return CW_PROTOCOL_SUCCESS;
}
#endif

CWBool cwRtkWtpSetPowerScale(int radio_id, const rtk_power_scaled_result_t *power_scaled_result)
{
	extern int wlan_idx;
	extern int vwlan_idx;
	char str[512], tmp_str[4];				
	CWBool ret = CW_TRUE;
	int err, j;
	const char *ifname = rtk_ifname[radio_id][0];
	
	// ifconfig down
//printf("all ifconfig down\n");
	for (j=0; j<RTK_VAPS+1; j++) {
		int wlan_disabled;
		wlan_idx = radio_id;
		vwlan_idx = j;
		apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled);

		if (!wlan_disabled)
			ifconfig_down(rtk_ifname[radio_id][j]);
	}
//system("ifconfig");
	// set to driver 
	// pwrlevelCCK_A
	str[0] = '\0';
	for (j=0; j<MAX_2G_CHANNEL_NUM_MIB; j++) {
		sprintf(tmp_str, "%02x", power_scaled_result->pwrlevelCCK_A[j]);
		strcat(str, tmp_str); 
	}
	err = set_mib(ifname, "pwrlevelCCK_A", str);
	if (err!=0) { 
		CWLog("cannot set pwrlevelCCK_A for %s, error=%d", ifname, err);
		ret = CW_FALSE;
	}
	
	// pwrlevelCCK_B
	str[0] = '\0';
	for (j=0; j<MAX_2G_CHANNEL_NUM_MIB; j++) {
		sprintf(tmp_str, "%02x", power_scaled_result->pwrlevelCCK_B[j]);
		strcat(str, tmp_str); 
	}
	err = set_mib(ifname, "pwrlevelCCK_B", str);
	if (err!=0) { 
		CWLog("cannot set pwrlevelCCK_B for %s, error=%d", ifname, err);
		ret = CW_FALSE;
	}

	// pwrlevelHT40_1S_A
	str[0] = '\0';
	for (j=0; j<MAX_2G_CHANNEL_NUM_MIB; j++) {
		sprintf(tmp_str, "%02x", power_scaled_result->pwrlevelHT40_1S_A[j]);
		strcat(str, tmp_str); 
	}
	err = set_mib(ifname, "pwrlevelHT40_1S_A", str);
	if (err!=0) { 
		CWLog("cannot set pwrlevelHT40_1S_A for %s, error=%d", ifname, err);
		ret = CW_FALSE;
	}
	
	// pwrlevelHT40_1S_B
	str[0] = '\0';
	for (j=0; j<MAX_2G_CHANNEL_NUM_MIB; j++) {
		sprintf(tmp_str, "%02x", power_scaled_result->pwrlevelHT40_1S_B[j]);
		strcat(str, tmp_str); 
	}
	err = set_mib(ifname, "pwrlevelHT40_1S_B", str);
	if (err!=0) { 
		CWLog("cannot set pwrlevelHT40_1S_B for %s, error=%d", ifname, err);
		ret = CW_FALSE;
	}

	// pwrlevel5GHT40_1S_A
	str[0] = '\0';
	for (j=0; j<MAX_5G_CHANNEL_NUM_MIB; j++) {
		sprintf(tmp_str, "%02x", power_scaled_result->pwrlevel5GHT40_1S_A[j]);
		strcat(str, tmp_str); 
	}
	err = set_mib(ifname, "pwrlevel5GHT40_1S_A", str);
	if (err!=0) { 
		CWLog("cannot set pwrlevel5GHT40_1S_A for %s, error=%d", ifname, err);
		ret = CW_FALSE;
	}

	// pwrlevel5GHT40_1S_B
	str[0] = '\0';
	for (j=0; j<MAX_5G_CHANNEL_NUM_MIB; j++) {
		sprintf(tmp_str, "%02x", power_scaled_result->pwrlevel5GHT40_1S_B[j]);
		strcat(str, tmp_str); 
	}
	err = set_mib(ifname, "pwrlevel5GHT40_1S_B", str);
	if (err!=0) { 
		CWLog("cannot set pwrlevel5GHT40_1S_B for %s, error=%d", ifname, err);
		ret = CW_FALSE;
	}
	
	// ifconfig up
//printf("all ifconfig up\n");
	for (j=0; j<RTK_VAPS+1; j++) {
		int wlan_disabled;
		wlan_idx = radio_id;
		vwlan_idx = j;
		apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled);
	
		if (!wlan_disabled)
			ifconfig_up(rtk_ifname[radio_id][j]);
	}
	if (!ret) {
		CWLog("Error: %s: Fail to set power into driver.", ifname);
		return CW_FALSE;
	}
	return CW_TRUE;
}


CWBool cwRtkWtpSetChannel(int radio_id, unsigned char channel)
{
	extern int wlan_idx;
	extern int vwlan_idx;
	char str[512];		
	int err, j;
	const char *ifname = rtk_ifname[radio_id][0];
	CWBool ret = CW_TRUE;
	
	// ifconfig down
//printf("all ifconfig down\n");
	for (j=0; j<RTK_VAPS+1; j++) {
		int wlan_disabled;
		wlan_idx = radio_id;
		vwlan_idx = j;
		apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled);

		if (!wlan_disabled)
			ifconfig_down(rtk_ifname[radio_id][j]);
	}
//system("ifconfig");

	// set to driver 
	sprintf(str, "%d", channel);
	err = set_mib(ifname, "channel", str);
	if (err!=0) { 
		CWLog("cannot set channel for %s, error=%d", ifname, err);
		ret = CW_FALSE;
	}
	
	// ifconfig up
//printf("all ifconfig up\n");
	for (j=0; j<RTK_VAPS+1; j++) {
		int wlan_disabled;
		wlan_idx = radio_id;
		vwlan_idx = j;
		apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled);
	
		if (!wlan_disabled)
			ifconfig_up(rtk_ifname[radio_id][j]);
	}
	
	if (!ret) {
		CWLog("Error: %s: Fail to set power into driver.", ifname);
		return CW_FALSE;
	}
	return CW_TRUE;
}
#endif

CWProtocolResultCode cwRtkWtpSetQos(const CWProtocolWtpQosValues* wtp_qos)
{
	const char *ifname = rtk_ifname[wtp_qos->radio_id][0];
	char tmp_str[32];
	int err;

	sprintf(tmp_str, "%u", wtp_qos->voice.cwMin);
	err = set_mib(ifname, "ap_voq_cwmin", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_voq_cwmin for %s, %d\n", ifname, err);
	}

	sprintf(tmp_str, "%u", wtp_qos->voice.cwMax);
	err = set_mib(ifname, "ap_voq_cwmax", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_voq_cwmax for %s, %d\n", ifname, err);
	}

	sprintf(tmp_str, "%u", wtp_qos->voice.AIFS);
	err = set_mib(ifname, "ap_voq_aifsn", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_voq_aifsn for %s, %d\n", ifname, err);
	}

	sprintf(tmp_str, "%u", wtp_qos->video.cwMin);
	err = set_mib(ifname, "ap_viq_cwmin", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_viq_cwmin for %s, %d\n", ifname, err);
	}

	sprintf(tmp_str, "%u", wtp_qos->video.cwMax);
	err = set_mib(ifname, "ap_viq_cwmax", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_viq_cwmax for %s, %d\n", ifname, err);
	}

	sprintf(tmp_str, "%u", wtp_qos->video.AIFS);
	err = set_mib(ifname, "ap_viq_aifsn", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_viq_aifsn for %s, %d\n", ifname, err);
	}

	sprintf(tmp_str, "%u", wtp_qos->best_effrot.cwMin);
	err = set_mib(ifname, "ap_beq_cwmin", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_beq_cwmin for %s, %d\n", ifname, err);
	}

	sprintf(tmp_str, "%u", wtp_qos->back_ground.cwMax);
	err = set_mib(ifname, "ap_baq_cwmax", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_baq_cwmax for %s, %d\n", ifname, err);
	}

	sprintf(tmp_str, "%u", wtp_qos->back_ground.AIFS);
	err = set_mib(ifname, "ap_baq_aifsn", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_baq_aifsn for %s, %d\n", ifname, err);
	}
	sprintf(tmp_str, "%u", wtp_qos->back_ground.AIFS);
	err = set_mib(ifname, "ap_baq_aifsn", tmp_str);
	if (err!=0) { 
		//printf("cannot set ap_baq_aifsn for %s, %d\n", ifname, err);
	}

	return CW_PROTOCOL_SUCCESS;
}

CWBool cwRtkWtpSiteSurvey(int radio_id, int maxNum, int *numBss, rtk_bss_t *bssInfo)
{
#ifndef X86_SIMULATION
	const char *ifname = rtk_ifname[radio_id][0];
    int wait_time;
	char req_status;
	SS_STATUS_T res_status;
	int err, i;

	// === issue scan request ====
	if (radio_id>=RTK_RADIOS) return CW_FALSE;
	wait_time = 0;	
	while(1) {
		err = get_wlan_site_survey_request(ifname, &req_status);	
		if (err != 0) {
			//printf("%s: Site-survey request failed! (line: %d, error: %d)\n", __FUNCTION__, __LINE__, err); 
			return CW_FALSE; 
		}

		switch(req_status)
		{ 
			case -2: 
				//printf("%s: Auto scan running!!please wait...\n", __FUNCTION__); 
 				return CW_FALSE; 
				break; 
			case -1: 
				//printf("%s: Site-survey request failed! (line: %d)\n", __FUNCTION__, __LINE__); 
				return CW_FALSE; 
				break; 
			default: 
				break; 
		} 

		if (req_status == 0) break;

		if (wait_time++ > 5) {	
			// otherwise, not ready.
			//printf("%s: scan request timeout!\n", __FUNCTION__); 
			return CW_FALSE;
		}
#ifdef	CONFIG_RTK_MESH
		usleep(1000000 + (rand() % 2000000));
#else
		sleep(1);
#endif
	} 

	
	// ==== wait until scan completely ====
	wait_time = 0;
	while (1) {
		// request request status
		res_status.number = 1;  // only request status
		err = get_wlan_site_survey_result(ifname, &res_status);
		if (err !=0) {
			//printf("%s: Can't read site-survey result status!\n", __FUNCTION__);			
			return CW_FALSE;
		}

		// can get result
		if (res_status.number != 0xff) break;

		// otherwise, it is in progress		
#if (defined(CONFIG_RTL_92D_SUPPORT) && defined (CONFIG_POCKET_ROUTER_SUPPORT)) || defined(CONFIG_RTL_DFS_SUPPORT)
		if (wait_time++ > 20) /*prolong wait time due to scan both 2.4G and 5G */
#else
		if (wait_time++ > 10) 
#endif		
		{
			//printf("%s: scan result status timeout!\n", __FUNCTION__);
			return CW_FALSE;
		}
		sleep(1);
		
	}
	
	// ==== get result ====
	res_status.number = 0;	// only request request status
	err = get_wlan_site_survey_result(ifname, &res_status);
	if (err !=0) {
		//printf("%s: Can't read site-survey result!\n", __FUNCTION__);			
		return CW_FALSE;
	}

	for (i=0; i<res_status.number && i<maxNum; i++) {
		memcpy(bssInfo[i].bssid, res_status.bssdb[i].bdBssId, 6);
		strcpy(bssInfo[i].ssid, (char *)res_status.bssdb[i].bdSsIdBuf);
		bssInfo[i].bss_type = res_status.bssdb[i].bdType;
		bssInfo[i].channel = res_status.bssdb[i].ChannelNumber;
		bssInfo[i].signal_strength = res_status.bssdb[i].rssi;
		bssInfo[i].radio_type = transTypeRadio(res_status.bssdb[i].network);
	}
	*numBss = i;
#else
	*numBss = 0;
#endif
	return CW_TRUE;
}


CWBool cwRtkWtpGetWlanStations(int radio_id, int wlan_id, int maxNum, int *numStation, CWRtkStationValues *staInfo)
{
#ifndef X86_SIMULATION
	wlan_stations_info_t *buff;	
	const WLAN_STA_INFO_T *pInfo;
	int i, n;
	if (radio_id<0 || radio_id>=RTK_RADIOS || wlan_id<0 || wlan_id >= RTK_VAPS+1) return CW_FALSE;

	buff = malloc(sizeof(wlan_stations_info_t));

	if (buff == NULL) return CW_FALSE;

	if (get_wlan_station_info(rtk_ifname[radio_id][wlan_id], buff)!=0) {
		//printf("(%s)Read wlan sta info failed!\n", rtk_ifname[radio_id][wlan_id]);
		free(buff);
		return CW_FALSE;
	}

	n = 0;
	pInfo = &((*buff)[0]);
	for (i=0; i<MAX_STA_NUM; i++, pInfo++) {
		if (i==maxNum) break;
		if (pInfo->aid && (pInfo->flag & STA_INFO_FLAG_ASOC)) {
			memcpy(staInfo[n].mac_addr, pInfo->addr, 6);
			staInfo[n].radio_id = radio_id;
			staInfo[n].wlan_id = wlan_id;
			n++;
		} else {
			continue;
		}
	}
	*numStation = n;

	free(buff);
	return CW_TRUE;
#else
	*numStation = 0;
	return CW_TRUE;
#endif
}

/****************************************/
char tempFwId[CW_IMAGE_IDENTIFIER_MAX_SIZE] = "";
CWBool haveInformationTempFw = CW_FALSE;
CWProtocolImageInformationValues tempFwInformation;
enum {
	TEMP_FW_STATUS_NOT_STARTED,
	TEMP_FW_STATUS_PREPARED,
	TEMP_FW_STATUS_DOWNLOADING,
	TEMP_FW_STATUS_DOWNLOADED,
	TEMP_FW_STATUS_CHECKED_INVALID,
	TEMP_FW_STATUS_CHECKED_VALID,
} tempFwStatus;
CWBool tempFwValid = CW_FALSE;

const char *cwRtkWtpGetCurrentFwId()
{
#ifdef X86_SIMULATION
	static const char currentFwId[CW_IMAGE_IDENTIFIER_MAX_SIZE] = "v3.4.6.2";
	return currentFwId;
#else
	return (char *)fwVersion; // in ../boa/src/version.c
#endif
}

const char *cwRtkWtpGetTempFwId()
{
	if (tempFwId[0] == '\0') return NULL;
	return tempFwId;
}

/* step 1 of download fw */
void cwRtkWtpPrepareFwDownload(const char *fwId, const CWProtocolImageInformationValues* image_info, CWBool with_image_info)
{
	strcpy(tempFwId, fwId);
	memcpy(&tempFwInformation, image_info, sizeof(CWProtocolImageInformationValues));
	haveInformationTempFw = with_image_info;
	tempFwStatus = TEMP_FW_STATUS_PREPARED;
}

static CWBool get_temp_firmware_path(char *temp_firmware_path)
{
	char tmp_name[CW_IMAGE_IDENTIFIER_MAX_SIZE];
	int len, i;

	if (tempFwStatus < TEMP_FW_STATUS_PREPARED) return CW_FALSE;
	strcpy(tmp_name, tempFwId);
	len = strlen(tmp_name);
	if(len == 0) {
		// empty string
		return CW_FALSE;
	}

	for (i=len-1; i>=0; i--) {
		if (tmp_name[i] == '/')break;
	}
#ifndef X86_SIMULATION
	sprintf(temp_firmware_path, "/var/tmp/%s", &tmp_name[i+1]);
#else
	sprintf(temp_firmware_path, "WTP-v3.4.6.3", &tmp_name[i+1]);
#endif
	return CW_TRUE;
}

CWBool cwRtkWtpSaveFirmwareDataPiece(const CWProtocolImageDataValues *image_data, CWBool *last)
{
	static char filepath[CW_IMAGE_IDENTIFIER_MAX_SIZE+4] = "\0";
	static FILE *fp = NULL;
	int len = (int)image_data->data_len;
	int n;

//static int c = 0;
//printf("%d", c++);
//printf("type=%d, len=%d\n", image_data->data_type, len);
	*last = CW_FALSE;
	switch (image_data->data_type) {
		case CW_IMAGE_DATA_TYPE_DATA:
		{
			// write into file
			if (!fp) {
				if (!get_temp_firmware_path(filepath)) {
					CWLog("new firmware version not exists!");
					return CW_FALSE;
				}
				fp=fopen(filepath, "wb");
				if (!fp) {
					CWLog("Cannot open file for write: %s", filepath);
					return CW_FALSE;
				}
				CWLog("Open file to write firmware: %s", filepath);
				tempFwStatus = TEMP_FW_STATUS_DOWNLOADING;
			}	
			
			while (len>0) {
				n = fwrite(image_data->data, 1, len, fp);
				if (n<=0) {
					fclose(fp);
					CWLog("Failed to write file: %s, so closed, fp=%p", filepath, fp);					
					fp = NULL;
					filepath[0] = '\0';
					return CW_FALSE;
				}
				len -= n;
			}
			fflush(stdout);
			break;
		}
		case CW_IMAGE_DATA_TYPE_EOF:
		{	// write into file, close file
			//printf("eof");
			if (!fp) {
				if (!get_temp_firmware_path(filepath)) {
					CWLog("new firmware version not exists!");
					return CW_FALSE;
				}
				fp=fopen(filepath, "wb");
				if (!fp) {
					CWLog("Cannot open file for write: %s", filepath);
					return CW_FALSE;
				}				
				CWDebugLog("Open file for write: %s", filepath);
			}	
			
			while (len>0) {
				n = fwrite(image_data->data, 1, len, fp);
				if (n<=0) {
					fclose(fp);
					CWLog("Faile to write file: %s, so closed, fp=%p", filepath, fp);					
					fp = NULL;
					filepath[0] = '\0';
					return CW_FALSE;
				}
				len -= n;
			}
			CWLog("EOF => Close file: %s", filepath);
			fclose(fp);
			fp = NULL;
			filepath[0] = '\0';
			
			if (haveInformationTempFw) {
				// TODO check file length & hash
				
			}

			*last = CW_TRUE;
			tempFwStatus = TEMP_FW_STATUS_DOWNLOADED;
			break;
		}
		case CW_IMAGE_DATA_TYPE_ERROR_OCCURRED:
		{
//printf("err");

			// close file & delete file
			if (fp) {
				fclose(fp);
				CWLog("AC abort changing firmware file: %s, so close and remove file, fp=%p", filepath, fp);
				fp = NULL;
				remove(filepath);
				filepath[0] = '\0';
			} else {
				CWLog("AC abort changing firmware file, but fp is NULL");
				return CW_FALSE;
			}
			break;
		}
		default:
		{
			CWLog("Unkown Image Data Type: %d", image_data->data_type);
			break;
		}
	}

//printf("\n");
	return CW_TRUE;

}


CWBool cwRtkWtpCheckFirmware()
{
	static const char *CHECK_OK_STRING = "Firmware upgrade check OK!";
	static const char *tmp_file = "/var/tmp/cw_tmp_result";
	char fw_path[128];
	char cmd[128];
	char *filebuf;
	int fd;
	struct stat stat_buff;
	
//printf("getting firmware path\n");
	
	if (!get_temp_firmware_path(fw_path)) {
		CWLog("Can't get temp firmware path");
		return CW_FALSE;
	}

	if (tempFwStatus < TEMP_FW_STATUS_DOWNLOADED) return CW_FALSE;
//	if (tempFwStatus == TEMP_FW_STATUS_CHECKED_INVALID) return CW_FALSE;
//	if (tempFwStatus == TEMP_FW_STATUS_CHECKED_VALID) return CW_TRUE;
#ifndef X86_SIMULATION
	sprintf(cmd, "fwupg check file %s > %s", fw_path, tmp_file); 
//printf("%s\n", cmd);
//sleep(60);
//printf("begin...\n");

	system(cmd);

//printf("opening tmp_file %s\n", tmp_file);

	fd = open(tmp_file, O_RDONLY);
	if (fd == -1) {
		CWLog("Can't open tmp_file %s", tmp_file);
		return CW_FALSE;
	}
	if (fstat(fd, &stat_buff) == -1) {
		CWLog("Can't check file size of %s", tmp_file);
		close(fd);
		remove(tmp_file);
		return CW_FALSE;
	}
	filebuf = mmap(NULL, stat_buff.st_size, PROT_READ, MAP_SHARED , fd, 0);
	if (filebuf == MAP_FAILED || filebuf == NULL) {
		CWLog("Can't mmap file %s", tmp_file);
		close(fd);		
		remove(tmp_file);
		return CW_FALSE;
	}

	if (strstr(filebuf, CHECK_OK_STRING)== NULL) {		
		munmap(filebuf, stat_buff.st_size);
		CWLog("firmware not valid! (%s)", filebuf);		

		close(fd);		
		remove(tmp_file);
		tempFwStatus = TEMP_FW_STATUS_CHECKED_INVALID;
		return CW_FALSE;
	}
	CWLog("Firmware is valid!");
	munmap(filebuf, stat_buff.st_size);
	close(fd);	
	remove(tmp_file);
#else
	CWLog("On x86, Firmware is always valid after downloaded.");
#endif

	tempFwStatus = TEMP_FW_STATUS_CHECKED_VALID;
	return CW_TRUE;

}


CWBool cwRtkWtpFirmwareUpgradeReboot()
{
	char fw_path[128];
	char cmd[128];
	
	if (!get_temp_firmware_path(fw_path)) return CW_FALSE;
	sprintf(cmd, "fwupg reboot file %s", fw_path); 

	CWLog("##### Firmware Upgrading... #####");
#ifndef X86_SIMULATION
	system(cmd);
#else
	CWLog("!On x86, won't execute this command: %s", cmd);
#endif
	return CW_TRUE;
}



/**************** Public APIs: Init ****************/
CWBool cwRtkWtpInit()
{
	tempFwStatus = TEMP_FW_STATUS_NOT_STARTED;

#ifndef X86_SIMULATION
#ifdef KM_BY_AC
	int sock;

	/**** init mutex ****/
	/*
	if (!CWCreateThreadMutex(&gAtomicCfgAction)) {		
		CWLog("Error Creating mutex gAtomicCfgAction"); 
		return CW_FALSE;
	}
	*/

	/**** init netlink ****/
	sock = socket(PF_NETLINK, SOCK_RAW, NETLINK_RTK_CAPWAP);
	if (sock < 0) { 
		CWLog("Error to create socket for ioctl"); 
		return CW_FALSE;
	}

	memset(&gNetlinkSrcAddr, 0, sizeof(struct sockaddr_nl));
	gNetlinkSrcAddr.nl_family = AF_NETLINK;
	gNetlinkSrcAddr.nl_pid = rtk_getpid(); /* self pid */
	gNetlinkSrcAddr.nl_groups = 0; /* not in mcast groups */

	//printf("wtp_pid=%d\n", gNetlinkSrcAddr.nl_pid);
	if (bind(sock, (struct sockaddr*)&gNetlinkSrcAddr, sizeof(struct sockaddr_nl)) < 0) {		
		CWLog("Error to bind socket for ioctl"); 
		close(sock);
		return CW_FALSE;
	}

#if 0
	// 2014 Babylon: we currently not use the "send" functionality of netlink in capwap.
	char *init_str = "Mr. Kernel, Are you ready ?";
	rtk_driver_interaction_snd_frame(init_str, strlen(init_str)+1);
	printf("%s\n", init_str);
#endif

	gNetlinkSock = sock;
#endif

#ifndef RTK_129X_PLATFORM
	/**** TODO: set AP MODE & let all wlaninterface down ****/
	/**** ap mib ***/
	if (!apmib_init()) {
		CWLog("Initialize AP MIB failed!"); 
		return CW_FALSE;
	}
#endif

#endif
	return CW_TRUE;
}

int cwRtkWtpExit()
{
#ifdef RTK_SMART_ROAMING
	return close(gNetlinkSock);
#endif
}
int cwRtkWtpGetVendorId()
{
	return VENDOR_IANA_ID; // IANA ID of Realtek
}
int cwRtkWtpGetSerialNo()
{
	// TODO
	return 0;
}
int cwRtkWtpGetModelNo()
{
	// TODO
	return 0;
}
int cwRtkWtpGetHwVersion()
{
	// TODO
	return 0;
}

/*
const char *cwRtkWtpGetSwVersion()
{
#ifdef X86_SIMULATION
	#define DRV_VERSION_H 3
	#define DRV_VERSION_L 6
	#define DRV_VERSION_L_SUB "4"
#endif

        static char sw_ver[128];
        sprintf(sw_ver, "%d.%d.%s", DRV_VERSION_H, DRV_VERSION_L, DRV_VERSION_L_SUB);
        return (const char *)sw_ver;
}
*/

int cwRtkWtpGetBootVersion()
{
	// TODO
	return 0;
}


#ifdef KM_BY_AC
int cwRtkWtpRcvFrame(uint8_t *buffer, uint32_t buffer_len, uint8_t **frame_pos, uint32_t *frame_len)
{
	struct nlmsghdr *nlh;
	struct iovec iov;	
	struct msghdr msg;
	
	nlh = (struct nlmsghdr *)buffer;
	nlh->nlmsg_len = NLMSG_LENGTH(buffer_len);

	iov.iov_base=(void *)nlh;
	iov.iov_len=nlh->nlmsg_len;
 
	memset(&msg, 0, sizeof(msg));
	msg.msg_name=(void *)&gNetlinkSrcAddr;
	msg.msg_namelen=sizeof(struct sockaddr_nl);
	msg.msg_iov=&iov;
	msg.msg_iovlen=1;

	if( recvmsg(gNetlinkSock, &msg, 0) <= 0) {
		CWLog("netlink recvmsg err: errno=%d!", errno);
		return -errno;
	}

	*frame_len = nlh->nlmsg_len;
	*frame_pos = NLMSG_DATA(nlh);
	return 0;
}
#endif

/************** Public APIs: Use shared memory and flash to do interaction ***********/
CWBool cwRtkWtpSystemEnabled()
{
#ifdef RTK_AUTO_AC
	return CW_TRUE;
#else
	int capwapMode;
	apmib_get(MIB_CAPWAP_MODE, &capwapMode);
	return ((capwapMode & CAPWAP_WTP_ENABLE)!=0);
#endif
}

int cwRtkWtpGetWtpId()
{
#ifdef RTK_AUTO_AC
	return get_auto_wtp_id();
#else
	return get_flash_wtp_id();
#endif
}

const char *cwRtkWtpGetApName()
{
	const char *wtp_name_prefix = "RTKCWAP_";
	static char wtp_name[16];
	sprintf(wtp_name, "%s%d", 	wtp_name_prefix, cwRtkWtpGetWtpId());
	//printf("WTP NAME=%s\n", wtp_name);
	return wtp_name;
}

unsigned int cwRtkWtpGetAcIp()
{
#ifdef RTK_AUTO_AC
	return get_auto_ac_ip();
#else
	return get_flash_ac_ip();
#endif
}

int cwRtkWtpGetMaxRadios() 
{
	return RTK_RADIOS;
}

int cwRtkWtpGetMaxWlanPerRadio() 
{
	return RTK_VAPS+1;
}


CWBool cwRtkWtpGetRadioInfo(CWRadiosInformation *valPtr) 
{
	enum NETWORK_TYPE type;
	int i;
	if(valPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	valPtr->radiosCount= RTK_RADIOS;
	for (i=0; i<RTK_RADIOS; i++) {
		valPtr->radios[i].ID = i;
#ifndef X86_SIMULATION
		get_flash_default_band(i, &type);
		valPtr->radios[i].type = transTypeRadio(type);
#else
		valPtr->radios[i].type = CW_802_DOT_11a|CW_802_DOT_11b|CW_802_DOT_11g|CW_802_DOT_11n|CW_802_DOT_11ac;
#endif
	}
	return CW_TRUE;
}

#ifdef RTK_SMART_ROAMING
CWBool cwRtkWtpGetConfiguration(CWWTPWlanConfiguration *valPtr) 
{
	extern int wlan_idx;
	extern int vwlan_idx;
#ifdef RTK_AUTO_AC
	char wlan_str[10]={0};
	struct sockaddr wlan_hwaddr;
#endif

	int i;
	if(valPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	
	valPtr->wlan_num= RTK_RADIOS;
	
	for (i=0; i<RTK_RADIOS; i++) {
		valPtr->wlan[i].ID = i;

		wlan_idx = i;
		vwlan_idx = 0;

#ifdef RTK_AUTO_AC
		sprintf(wlan_str, "wlan%d",i);
		if(getInAddr(wlan_str, HW_ADDR, (void*)&wlan_hwaddr )) {
			memcpy(valPtr->wlan[i].mac_addr, wlan_hwaddr.sa_data, 6);
		}
		else
			CWDebugLog("Can't get %s mac addr", wlan_str);
#else
		apmib_get(MIB_HW_WLAN_ADDR, (void *)valPtr->wlan[i].mac_addr);		
#endif
		CWDebugLog("wtp check wlan%d mac=%02x:%02x:%02x:%02x:%02x:%02x", 
				i,valPtr->wlan[i].mac_addr[0], valPtr->wlan[i].mac_addr[1],
				valPtr->wlan[i].mac_addr[2], valPtr->wlan[i].mac_addr[3],
				valPtr->wlan[i].mac_addr[4], valPtr->wlan[i].mac_addr[5]);
	}

	return CW_TRUE;
}
#endif
CWBool cwRtkWtpGetChannel(int radio_id, CWProtocolRtkChannelValues *pRadioConfig)
{
	extern int wlan_idx;
	extern int vwlan_idx;
	int iVal;

	if (radio_id>=RTK_RADIOS) return CW_FALSE;

#ifndef X86_SIMULATION
	wlan_idx = radio_id;
	vwlan_idx = 0;

	pRadioConfig->radio_id = radio_id;

	// CHANNEL
	apmib_get(MIB_WLAN_CHANNEL, (void *)&iVal);
	pRadioConfig->channel = iVal;
#else
	// CHANNEL
	pRadioConfig->channel = 44;
#endif
	return CW_TRUE;
}

CWBool cwRtkWtpGetPowerScale(int radio_id, CWProtocolRtkPowerScaleValues *pRadioConfig)
{
	extern int wlan_idx;
	extern int vwlan_idx;
	int iVal;

	if (radio_id>=RTK_RADIOS) return CW_FALSE;

#ifndef X86_SIMULATION
	wlan_idx = radio_id;
	vwlan_idx = 0;

	pRadioConfig->radio_id = radio_id;

	// RFPOWER_SCALE
	apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&iVal);
	pRadioConfig->power_scale= iVal;
#else
	// RFPOWER_SCALE
	pRadioConfig->power_scale= POWER_SCALE_100;
#endif
	return CW_TRUE;
}

#if 0
CWBool cwRtkWtpGetRadios(int *nRadios, CWProtocolRtkRadioValues *pRadioConfig)
{
	extern int wlan_idx;
	extern int vwlan_idx;
	int i, iVal;

	*nRadios=0;
	for (i =0; i< RTK_RADIOS; i++) {
		wlan_idx = i;
		vwlan_idx = 0;

		pRadioConfig->radio_id = i;

		// CHANNEL
		apmib_get(MIB_WLAN_CHANNEL, (void *)&iVal);
		pRadioConfig->channel = iVal;

		// RFPOWER_SCALE
		apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&iVal);
		pRadioConfig->power_percentage = iVal;

		(*nRadios)++;
		pRadioConfig++;
	}

	return CW_TRUE;
}
#endif

#ifndef X86_SIMULATION
CWBool cwRtkWtpGetWlans(int maxNum, int *nWlans, CWProtocolRtkWlanConfigValues *pWlanConfig)
{
	extern int wlan_idx;
	extern int vwlan_idx;
	int i, j, iVal;

	*nWlans=0;
	for (i =0; i<RTK_RADIOS && *nWlans<maxNum; i++) {
		wlan_idx = i;
		
		for (j=0; j<RTK_VAPS+1 && *nWlans<maxNum; j++) {
			vwlan_idx = j;
			int encrypt;

			// ------------ check ----------- 						
			// WLAN_DISABLED = 0
			apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&iVal);
			if (iVal != 0) continue;

			// WLAN_MODE = AP_MODE
			apmib_get(MIB_WLAN_MODE, (void *)&iVal);
			if (iVal != AP_MODE) continue;

			// WLAN_ENABLE_1X = 0
			apmib_get(MIB_WLAN_ENABLE_1X, (void *)&iVal);
			if (iVal != 0) continue;

			// MAC_AUTH_ENABLED = 0
			apmib_get(MIB_WLAN_MAC_AUTH_ENABLED, (void *)&iVal);
			if (iVal != 0) continue;

			// ------------ fill ----------- 			
			pWlanConfig->radio_id = i;
			pWlanConfig->wlan_id = j;

			// MIB_WLAN_ENCRYPT
			apmib_get(MIB_WLAN_ENCRYPT, (void *)&encrypt);
			switch(encrypt)
			{
			case ENCRYPT_DISABLED:
				pWlanConfig->key_type = WLAN_CFG_KEY_TYPE_NONE;
				break;

			case ENCRYPT_WEP:
				// MIB_WLAN_WEP
				apmib_get(MIB_WLAN_WEP, (void *)&iVal);
				if (iVal == WEP64) {
					pWlanConfig->key_type = WLAN_CFG_KEY_TYPE_SHARED_WEP40;
					// MIB_WLAN_WEP64_KEYn
					apmib_get(MIB_WLAN_WEP64_KEY1, (void *)pWlanConfig->key);

					
				} else { // iVal == WEP128
					pWlanConfig->key_type = WLAN_CFG_KEY_TYPE_SHARED_WEP104;
					// MIB_WLAN_WEP128_KEYn
					apmib_get(MIB_WLAN_WEP128_KEY1, (void *)pWlanConfig->key);
				}
				break;

			case ENCRYPT_WPA:
				// MIB_WLAN_WPA_CIPHER_SUITE
				apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&iVal);
				if (iVal == WPA_CIPHER_TKIP){ // wpa tkip
					pWlanConfig->key_type = WLAN_CFG_KEY_TYPE_SHARED_WPA_TKIP;			
				} else { // wpa aes
					pWlanConfig->key_type = WLAN_CFG_KEY_TYPE_SHARED_WPA_AES;
				}

				// MIB_WLAN_PSK_FORMAT
				apmib_get(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				pWlanConfig->psk_format = iVal;

				// MIB_WLAN_WPA_PSK
				apmib_get(MIB_WLAN_WPA_PSK, (void *)pWlanConfig->key);				
				break;

			case ENCRYPT_WPA2:
				// MIB_WLAN_WPA2_CIPHER_SUITE
				apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&iVal);
				if (iVal == WPA_CIPHER_TKIP){ // wpa2 tkip
					pWlanConfig->key_type = WLAN_CFG_KEY_TYPE_SHARED_WPA2_TKIP;			
				} else { // wpa2 aes
					pWlanConfig->key_type = WLAN_CFG_KEY_TYPE_SHARED_WPA2_AES;
				}

				// MIB_WLAN_PSK_FORMAT
				apmib_get(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				pWlanConfig->psk_format = iVal;
				
				// MIB_WLAN_WPA_PSK
				apmib_get(MIB_WLAN_WPA_PSK, (void *)pWlanConfig->key);				
				break;
				
			default:
				CWDebugLog("strange encryption: %d", encrypt);
				continue;
				break;

			}
			
			
			// MIB_HW_WLAN_ADDR = BSSID
			{
				int mib_id;
				if (vwlan_idx == 0) 
					mib_id = MIB_HW_WLAN_ADDR;
				else 
					mib_id = vwlan_idx - 1 + MIB_HW_WLAN_ADDR1;
	
				apmib_get(mib_id, (void *)pWlanConfig->bssid);
			}

			// WLAN_SSID
			apmib_get(MIB_WLAN_SSID, (void *)pWlanConfig->ssid);
			
			(*nWlans)++;
			pWlanConfig++;
		}
	}
	return CW_TRUE;
}
#endif

#ifndef X86_SIMULATION
/* description: write config into flash.
 * Inputs: nWlans, pWlanConfig, 
 *		invalid_all_others - disable those not listed.
 *		reload_if_changed - set config from flash to driver
 * Outputs: needReloadLater - flash and driver are unsync; need to do reload later to sync them.
 * return: configuration has been activated.
 */
CWBool cwRtkWtpCfgWlans(int nWlans, const CWProtocolRtkWlanConfigValues *pWlanConfig, 
						CWBool invalid_all_others, CWBool reload_if_changed,
						CWBool *needReloadLater)
{
	CWBool cfgChanged;
	extern int wlan_idx;
	extern int vwlan_idx;
	int i, j, iVal;
	char strVal[65];
	enum {
		UNSET,
		CHANGED,
		UNCHANGED,
	} wlan_status [RTK_RADIOS][RTK_VAPS+1];

	CWLog("%s started", __FUNCTION__);
	// ======== Step 1: check input values =============
	for (i=0; i<nWlans; i++) {
		int len;
		const unsigned char invalid_bssid1[6] = {0, 0, 0, 0, 0, 0};		
		const unsigned char invalid_bssid2[6] = {0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd};
		const unsigned char invalid_bssid3[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
printf("radio_id=%u, wlan_id=%u\n", pWlanConfig[i].radio_id, pWlanConfig[i].wlan_id);		
		if ( pWlanConfig[i].radio_id>=RTK_RADIOS )	return CW_FALSE;
		if ( pWlanConfig[i].wlan_id>=RTK_VAPS+1 )	return CW_FALSE;

		len = strlen(pWlanConfig[i].ssid);
		if (len == 0 || len >= CW_MAX_SSID_SIZE)	return CW_FALSE;
		
		if (memcmp(invalid_bssid1, pWlanConfig[i].bssid, 6) == 0) return CW_FALSE;
		if (memcmp(invalid_bssid2, pWlanConfig[i].bssid, 6) == 0) return CW_FALSE;
		if (memcmp(invalid_bssid3, pWlanConfig[i].bssid, 6) == 0) return CW_FALSE;

		switch(pWlanConfig[i].psk_format) {
			case WLAN_CFG_PSK_FORMAT_PASSPHRASE:
			case WLAN_CFG_PSK_FORMAT_HEX:
				break;
			default:
				//printf("psk format error: psk_format=%d\n", pWlanConfig[i].psk_format);
				return CW_FALSE;
				break;
		}
		
		switch(pWlanConfig[i].key_type) {
			case WLAN_CFG_KEY_TYPE_NONE:
				break;
			case WLAN_CFG_KEY_TYPE_SHARED_WEP40:
			{
				unsigned char wep64_key[5];
				if (pWlanConfig[i].psk_format == WLAN_CFG_PSK_FORMAT_HEX) {
					if (strlen(pWlanConfig[i].key) < 10) {
						//printf("Error: WTP got WEP40 hex key length<10, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
					if (!string_to_hex(pWlanConfig[i].key, wep64_key, 10)) {
						//printf("Error: WTP got WEP40 hex key format error, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
				} else {
					if (strlen(pWlanConfig[i].key) < 5) {
						//printf("Error: WTP got WEP40 key length<5, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
				}
				break;
			}
			case WLAN_CFG_KEY_TYPE_SHARED_WEP104:
			{
				unsigned char wep128_key[13];
				if (pWlanConfig[i].psk_format == WLAN_CFG_PSK_FORMAT_HEX) {
					if (strlen(pWlanConfig[i].key) < 26) {
						//printf("Error: WTP got WEP104 hex key length<26, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
					if (!string_to_hex(pWlanConfig[i].key, wep128_key, 26)) {
						//printf("Error: WTP got WEP104 hex key format error, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
				} else {
					if (strlen(pWlanConfig[i].key) < 13) {
						//printf("Error: WTP got WEP104 key length<13, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
				}
				break;
			}
			case WLAN_CFG_KEY_TYPE_SHARED_WPA_AES:
			case WLAN_CFG_KEY_TYPE_SHARED_WPA_TKIP:
			case WLAN_CFG_KEY_TYPE_SHARED_WPA2_AES:
			case WLAN_CFG_KEY_TYPE_SHARED_WPA2_TKIP:
			{
				unsigned char wpa_key[64];
				if (pWlanConfig[i].psk_format == WLAN_CFG_PSK_FORMAT_HEX) {
					if (strlen(pWlanConfig[i].key) < 64) {
						//printf("Error: WTP got WPA/WP2 hex key length<64, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
					if (!string_to_hex(pWlanConfig[i].key, wpa_key, 64)) {
						//printf("Error: WTP got WPA/WP2 hex key format error, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
				} else {
					if (strlen(pWlanConfig[i].key) < 8) {
						//printf("Error: WTP got WPA/WP2 key length<8, key:%s (Reject this config)\n", pWlanConfig[i].key);
						return CW_FALSE;
					}
				}
				break;
			}
			default:
				//printf("Error: Encrypt type error: %d\n", pWlanConfig[i].key_type);

				return CW_FALSE;
				break;
		}
		
	}

	// ======== Step 2: set into flash ==============
	// init	
	cfgChanged = CW_FALSE;
	for (i=0; i<RTK_RADIOS; i++) {
		for (j=0; j<RTK_VAPS+1; j++) {
			wlan_status[i][j] = UNSET;
		}
	}
	
	// set into shared meomry
	for (i=0; i<nWlans; i++, pWlanConfig++) {

		wlan_idx = pWlanConfig->radio_id;

		vwlan_idx = pWlanConfig->wlan_id;

		wlan_status[wlan_idx][vwlan_idx] = UNCHANGED;

		// WLAN_DISABLED = 0
		apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&iVal);
		if (iVal != 0) {
			// currently not enabled
			wlan_status[wlan_idx][vwlan_idx] = CHANGED;
			cfgChanged = CW_TRUE;
			iVal = 0;
			apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&iVal);
		}

		// WLAN_MODE = AP_MODE
		apmib_get(MIB_WLAN_MODE, (void *)&iVal);
		if (iVal != AP_MODE){
			// currently not AP_MODE
			wlan_status[wlan_idx][vwlan_idx] = CHANGED;
			cfgChanged = CW_TRUE;
			iVal = AP_MODE;
			apmib_set(MIB_WLAN_MODE, (void *)&iVal);
		}

		// WLAN_ENABLE_1X = 0
		apmib_get(MIB_WLAN_ENABLE_1X, (void *)&iVal);
		if (iVal != 0){
			// currently 1X eanbled
			wlan_status[wlan_idx][vwlan_idx] = CHANGED;
			cfgChanged = CW_TRUE;
			iVal = 0;
			apmib_set(MIB_WLAN_ENABLE_1X, (void *)&iVal);
		}

		// MAC_AUTH_ENABLED = 0
		apmib_get(MIB_WLAN_MAC_AUTH_ENABLED, (void *)&iVal);
		if (iVal != 0){
			// currently auth enabled
			wlan_status[wlan_idx][vwlan_idx] = CHANGED;
			cfgChanged = CW_TRUE;
			iVal = 0;
			apmib_set(MIB_WLAN_MAC_AUTH_ENABLED, (void *)&iVal);
		}

		// WLAN_SSID
		apmib_get(MIB_WLAN_SSID, (void *)&strVal);
		if (strcmp(strVal, pWlanConfig->ssid) != 0){
			// different SSID
				wlan_status[wlan_idx][vwlan_idx] = CHANGED;
				cfgChanged = CW_TRUE;
			apmib_set(MIB_WLAN_SSID, (void *)pWlanConfig->ssid);
		}

		// TODO
		// MIB_WLAN_BAND, MIB_WLAN_BASIC_RATES, MIB_WLAN_SUPPORTED_RATES
		// MIB_WLAN_RATE_ADAPTIVE_ENABLED
		// MIB_WLAN_HIDDEN_SSID
		// MIB_WLAN_WMM_ENABLED
		// MIB_WLAN_ACCESS, MIB_WLAN_TX_RESTRICT, MIB_WLAN_RX_RESTRICT


		switch(pWlanConfig->key_type) {
			// TODO
			// MIB_WLAN_AUTH_TYPE	
			// MIB_WLAN_WPA_GROUP_REKEY_TIME
				
			case WLAN_CFG_KEY_TYPE_NONE:
				// MIB_WLAN_ENCRYPT
				apmib_get(MIB_WLAN_ENCRYPT, (void *)&iVal);
				if (iVal != ENCRYPT_DISABLED){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = ENCRYPT_DISABLED;
					apmib_set(MIB_WLAN_ENCRYPT, (void *)&iVal);
				}
				break;
				
			case WLAN_CFG_KEY_TYPE_SHARED_WEP40:
			{
				// MIB_WLAN_ENCRYPT
				apmib_get(MIB_WLAN_ENCRYPT, (void *)&iVal);
				if (iVal != ENCRYPT_WEP){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = ENCRYPT_WEP;
					apmib_set(MIB_WLAN_ENCRYPT, (void *)&iVal);
				}

 				// MIB_WLAN_WEP
 				apmib_get(MIB_WLAN_WEP, (void *)&iVal);
 				if (iVal != WEP64){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WEP64;
					apmib_set(MIB_WLAN_WEP, (void *)&iVal);
				}

				// MIB_WLAN_WEP_KEY_TYPE = HEX
				apmib_get(MIB_WLAN_WEP_KEY_TYPE, (void *)&iVal);
				if (iVal != 1){ // always HEX
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = 1;
					apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&iVal);
				}
					
				// MIB_WLAN_WEP64_KEYn
				unsigned char wep64_key[5];
				if (pWlanConfig->psk_format == WLAN_CFG_PSK_FORMAT_HEX) {
					if (!string_to_hex(pWlanConfig->key, wep64_key, 10)) {
						memset(wep64_key, 0, 5);
					}
				} else {
					memcpy(wep64_key, pWlanConfig->key, 5);
				}				
				apmib_get(MIB_WLAN_WEP64_KEY1, (void *)&strVal);
				if (memcmp(strVal, wep64_key, 5) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WEP64_KEY1, wep64_key);
				}
				apmib_get(MIB_WLAN_WEP64_KEY2, (void *)&strVal);
				if (memcmp(strVal, wep64_key, 5) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WEP64_KEY2, wep64_key);
				}
				apmib_get(MIB_WLAN_WEP64_KEY3, (void *)&strVal);
				if (memcmp(strVal, wep64_key, 5) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WEP64_KEY3, wep64_key);
				}
				apmib_get(MIB_WLAN_WEP64_KEY4, (void *)&strVal);
				if (memcmp(strVal, wep64_key, 5) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WEP64_KEY4, wep64_key);
				}
				break;
			}	
			case WLAN_CFG_KEY_TYPE_SHARED_WEP104:
			{
				// MIB_WLAN_ENCRYPT
				apmib_get(MIB_WLAN_ENCRYPT, (void *)&iVal);
				if (iVal != ENCRYPT_WEP){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = ENCRYPT_WEP;
					apmib_set(MIB_WLAN_ENCRYPT, (void *)&iVal);
				}

				// MIB_WLAN_WEP
				apmib_get(MIB_WLAN_WEP, (void *)&iVal);
				if (iVal != WEP128){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WEP128;
					apmib_set(MIB_WLAN_WEP, (void *)&iVal);
				}

				// MIB_WLAN_WEP_KEY_TYPE = HEX
				apmib_get(MIB_WLAN_WEP_KEY_TYPE, (void *)&iVal);
				if (iVal != 1){ // always HEX
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = 1;
					apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&iVal);
				}
					
				// MIB_WLAN_WEP128_KEYn
				unsigned char wep128_key[13];
				if (pWlanConfig->psk_format == WLAN_CFG_PSK_FORMAT_HEX) {
					if (!string_to_hex(pWlanConfig->key, wep128_key, 26)) {
						memset(wep128_key, 0, 13);
					}
				} else {
					memcpy(wep128_key, pWlanConfig->key, 13);
				}				
					
				apmib_get(MIB_WLAN_WEP128_KEY1, (void *)&strVal);
				if (memcmp(strVal, wep128_key, 13) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WEP128_KEY1, wep128_key);
				}				
				apmib_get(MIB_WLAN_WEP128_KEY2, (void *)&strVal);
				if (memcmp(strVal, wep128_key, 13) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WEP128_KEY2, wep128_key);
				}
				apmib_get(MIB_WLAN_WEP128_KEY3, (void *)&strVal);
				if (memcmp(strVal, wep128_key, 13) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WEP128_KEY3, wep128_key);
				}
				apmib_get(MIB_WLAN_WEP128_KEY4, (void *)&strVal);
				if (memcmp(strVal, wep128_key, 13) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WEP128_KEY4, wep128_key);
				}
				break;
			}
			
			case WLAN_CFG_KEY_TYPE_SHARED_WPA_TKIP:
				// MIB_WLAN_ENCRYPT
				apmib_get(MIB_WLAN_ENCRYPT, (void *)&iVal);
				if (iVal != ENCRYPT_WPA){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = ENCRYPT_WPA;
					apmib_set(MIB_WLAN_ENCRYPT, (void *)&iVal);
				}

				// MIB_WLAN_WPA_AUTH
				apmib_get(MIB_WLAN_WPA_AUTH, (void *)&iVal);
				if (iVal != WPA_AUTH_PSK){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WPA_AUTH_PSK;
					apmib_set(MIB_WLAN_WPA_AUTH, (void *)&iVal);
				}

				// MIB_WLAN_WPA_CIPHER_SUITE
				apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&iVal);
				if (iVal != WPA_CIPHER_TKIP){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WPA_CIPHER_TKIP;
					apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&iVal);
				}

				// MIB_WLAN_PSK_FORMAT
				apmib_get(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				if (iVal != pWlanConfig->psk_format){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = pWlanConfig->psk_format;
					apmib_set(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				}

				// MIB_WLAN_WPA_PSK
				apmib_get(MIB_WLAN_WPA_PSK, (void *)&strVal);
				if (strcmp(strVal, (char *)pWlanConfig->key) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WPA_PSK, (void *)pWlanConfig->key);
				}
				break;

			case WLAN_CFG_KEY_TYPE_SHARED_WPA_AES:
				// MIB_WLAN_ENCRYPT
				apmib_get(MIB_WLAN_ENCRYPT, (void *)&iVal);
				if (iVal != ENCRYPT_WPA){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = ENCRYPT_WPA;
					apmib_set(MIB_WLAN_ENCRYPT, (void *)&iVal);
				}

				// MIB_WLAN_WPA_AUTH
				apmib_get(MIB_WLAN_WPA_AUTH, (void *)&iVal);
				if (iVal != WPA_AUTH_PSK){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WPA_AUTH_PSK;
					apmib_set(MIB_WLAN_WPA_AUTH, (void *)&iVal);
				}

				// MIB_WLAN_WPA_CIPHER_SUITE
				apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&iVal);
				if (iVal != WPA_CIPHER_AES){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WPA_CIPHER_AES;
					apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&iVal);
				}

				// MIB_WLAN_PSK_FORMAT
				apmib_get(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				if (iVal != pWlanConfig->psk_format){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = pWlanConfig->psk_format;
					apmib_set(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				}

				// MIB_WLAN_WPA_PSK
				apmib_get(MIB_WLAN_WPA_PSK, (void *)&strVal);
				if (strcmp(strVal, (char *)pWlanConfig->key) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WPA_PSK, (void *)pWlanConfig->key);
				}
				break;


			case WLAN_CFG_KEY_TYPE_SHARED_WPA2_TKIP:
				// MIB_WLAN_ENCRYPT
				apmib_get(MIB_WLAN_ENCRYPT, (void *)&iVal);
				if (iVal != ENCRYPT_WPA2){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = ENCRYPT_WPA2;
					apmib_set(MIB_WLAN_ENCRYPT, (void *)&iVal);
				}
					
				// MIB_WLAN_WPA_AUTH
				apmib_get(MIB_WLAN_WPA_AUTH, (void *)&iVal);
				if (iVal != WPA_AUTH_PSK){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WPA_AUTH_PSK;
					apmib_set(MIB_WLAN_WPA_AUTH, (void *)&iVal);
				}
					
				// MIB_WLAN_WPA2_CIPHER_SUITE
				apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&iVal);
				if (iVal != WPA_CIPHER_TKIP){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WPA_CIPHER_TKIP;
					apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&iVal);
				}


				// MIB_WLAN_PSK_FORMAT
				apmib_get(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				if (iVal != pWlanConfig->psk_format){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = pWlanConfig->psk_format;
					apmib_set(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				}

				// MIB_WLAN_WPA_PSK
				apmib_get(MIB_WLAN_WPA_PSK, (void *)&strVal);
				if (strcmp(strVal, (char *)pWlanConfig->key) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WPA_PSK, (void *)pWlanConfig->key);
				}
				break;
				
			case WLAN_CFG_KEY_TYPE_SHARED_WPA2_AES:
				// MIB_WLAN_ENCRYPT
				apmib_get(MIB_WLAN_ENCRYPT, (void *)&iVal);
				if (iVal != ENCRYPT_WPA2){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = ENCRYPT_WPA2;
					apmib_set(MIB_WLAN_ENCRYPT, (void *)&iVal);
				}
					
				// MIB_WLAN_WPA_AUTH
				apmib_get(MIB_WLAN_WPA_AUTH, (void *)&iVal);
				if (iVal != WPA_AUTH_PSK){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WPA_AUTH_PSK;
					apmib_set(MIB_WLAN_WPA_AUTH, (void *)&iVal);
				}
					
				// MIB_WLAN_WPA2_CIPHER_SUITE
				apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&iVal);
				if (iVal != WPA_CIPHER_AES){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = WPA_CIPHER_AES;
					apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&iVal);
				}

				// MIB_WLAN_PSK_FORMAT
				apmib_get(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				if (iVal != pWlanConfig->psk_format){
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					iVal = pWlanConfig->psk_format;
					apmib_set(MIB_WLAN_PSK_FORMAT, (void *)&iVal);
				}

				// MIB_WLAN_WPA_PSK
				apmib_get(MIB_WLAN_WPA_PSK, (void *)&strVal);
				if (strcmp(strVal, (char *)pWlanConfig->key) != 0) {
					wlan_status[wlan_idx][vwlan_idx] = CHANGED;
					cfgChanged = CW_TRUE;
					apmib_set(MIB_WLAN_WPA_PSK, (void *)pWlanConfig->key);
				}
				break;
			}

		// MIB_HW_WLAN_ADDR = BSSID
		{
			int mib_id;
			if (vwlan_idx == 0) 
				mib_id = MIB_HW_WLAN_ADDR;
			else 
				mib_id = vwlan_idx - 1 + MIB_HW_WLAN_ADDR1;

			apmib_get(mib_id, (void *)&strVal);
			if (memcmp(strVal, pWlanConfig->bssid, 6) != 0) {
printf("BSSID: %02x%02x%02x%02x%02x%02x, %02x%02x%02x%02x%02x%02x\n", 
			strVal[0], strVal[1], strVal[2], strVal[3], strVal[4], strVal[5], 			
			pWlanConfig->bssid[0], pWlanConfig->bssid[1], pWlanConfig->bssid[2], 
			pWlanConfig->bssid[3], pWlanConfig->bssid[4], pWlanConfig->bssid[5]);
				wlan_status[wlan_idx][vwlan_idx] = CHANGED;
				cfgChanged = CW_TRUE;
				apmib_set(mib_id, (void *)pWlanConfig->bssid);
			}			
		}
	
	}
		
	// reset (down the interface)
	if (invalid_all_others) {
		for (i=0; i<RTK_RADIOS; i++) {
			for (j=0; j<RTK_VAPS+1; j++) {
				if (wlan_status[i][j] == UNSET) {
					wlan_idx = i;
					vwlan_idx = j;
					apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&iVal);
					if (iVal != 1) {
						// not disabled
						wlan_status[i][j] = CHANGED;
						cfgChanged = CW_TRUE;
						iVal = 1;
						apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&iVal);
					} else {			
						wlan_status[i][j] = UNCHANGED;
					}
				}
			}
		}
	}

	*needReloadLater = cfgChanged;
	if (!cfgChanged) {
		CWLog("%s: config not changed.", __FUNCTION__);
		return CW_TRUE;
	}

	// set into flash
	if (!apmib_update(CURRENT_SETTING)) {
		CWLog("Error: cannot update into flash");
		return CW_FALSE;
	}

	// ======== Step 3: update to driver ==============
	if (reload_if_changed) {		
		CWLog("Set to Driver for New Wlan Config");
#if 1
		int i, j;
		for (i=0; i<RTK_RADIOS; i++) {
			for (j=0; j<RTK_VAPS+1; j++) {
				if (wlan_status[i][j] == CHANGED) {
					const char *ifname = rtk_ifname[i][j];
					char cmd[128];
					CWLog("%s config changed", ifname);
					sprintf(cmd, "brctl delif %s %s; flash set_mib %s", BR_NAME, ifname, ifname);
					system(cmd);
				}
			}
		}

		for (i=0; i<RTK_RADIOS; i++) {
			for (j=0; j<RTK_VAPS+1; j++) {
				int disabled;
				wlan_idx = i;
				vwlan_idx = j;
				apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disabled);
				if (!disabled) {
					const char *ifname = rtk_ifname[i][j];
					char cmd[128];
					CWLog("bring %s up", ifname);
					sprintf(cmd, "ifconfig %s up; brctl addif %s %s", ifname, BR_NAME, ifname);
					system(cmd);
				}
			}
		}


#else
		system("sysconf init gw all");
#endif
		*needReloadLater = CW_FALSE;
	}

	
	CWLog("%s succesfully completed", __FUNCTION__);
	return CW_TRUE;
}
static CWBool __calculate_scaled_power(int radio_id, rtk_power_scaled_result_t *power_scaled_result)
{
/*
#define MAX_CCK_CHAN_NUM		14
#define MAX_OFDM_CHAN_NUM		162
#define MAX_2G_CHANNEL_NUM_MIB		14
#define MAX_5G_CHANNEL_NUM_MIB		196
*/
	int dot11RFType;

	extern int wlan_idx;
	extern int vwlan_idx;
	int intVal, i;
	unsigned char buf1[196];

	if (radio_id >= RTK_RADIOS) return CW_FALSE;
	wlan_idx = radio_id;
	vwlan_idx = 0;
	
	// set RF parameters
	apmib_get(MIB_HW_RF_TYPE, (void *)&dot11RFType);
	if (dot11RFType != 10) { // !Zebra
		CWLog("Can't change power: dot11RFType=%d", dot11RFType);
		return CW_FALSE;
	}

	memset(power_scaled_result, 0, sizeof(rtk_power_scaled_result_t));
	
#if defined(CONFIG_RTL_8196B)
	// get original value (100 %)
	apmib_get(MIB_HW_TX_POWER_CCK, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelCCK, buf1, 14);
	
	apmib_get(MIB_HW_TX_POWER_OFDM_1S, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelOFDM_1SS, buf1, 162);
	
	apmib_get(MIB_HW_TX_POWER_OFDM_2S, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelOFDM_2SS, buf1, 162);

	// scale it!
	apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&intVal);
	if(intVal == 1)
		intVal = 3;
	else if(intVal == 2)
			intVal = 6;
		else if(intVal == 3)
				intVal = 9;
			else if(intVal == 4)
					intVal = 17;
	if (intVal) {
		for (i=0; i<14; i++) {
			if(power_scaled_result->pwrlevelCCK[i] != 0){ 
				if ((power_scaled_result->pwrlevelCCK[i] - intVal) >= 1)
					power_scaled_result->pwrlevelCCK[i] -= intVal;
				else
					power_scaled_result->pwrlevelCCK[i] = 1;
			}
		}
		for (i=0; i<162; i++) {
			if (power_scaled_result->pwrlevelOFDM_1SS[i] != 0){
				if((power_scaled_result->pwrlevelOFDM_1SS[i] - intVal) >= 1)
					power_scaled_result->pwrlevelOFDM_1SS[i] -= intVal;
				else
					power_scaled_result->pwrlevelOFDM_1SS[i] = 1;
			}
			if (power_scaled_result->pwrlevelOFDM_2SS[i] != 0){
				if((power_scaled_result->pwrlevelOFDM_2SS[i] - intVal) >= 1)
					power_scaled_result->pwrlevelOFDM_2SS[i] -= intVal;
				else
					power_scaled_result->pwrlevelOFDM_2SS[i] = 1;
			}
		}		
	}
#elif defined(CONFIG_RTL_8198C)||defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198) || defined(CONFIG_RTL_819XD) || defined(CONFIG_RTL_8196E) || defined(CONFIG_RTL_8198B) || defined(CONFIG_RTL_8197F)
	// get original value (100 %)
	apmib_get(MIB_HW_TX_POWER_CCK_A, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelCCK_A, buf1, MAX_2G_CHANNEL_NUM_MIB); 
	
	apmib_get(MIB_HW_TX_POWER_CCK_B, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelCCK_B, buf1, MAX_2G_CHANNEL_NUM_MIB);
	
	apmib_get(MIB_HW_TX_POWER_HT40_1S_A, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelHT40_1S_A, buf1, MAX_2G_CHANNEL_NUM_MIB);
	
	apmib_get(MIB_HW_TX_POWER_HT40_1S_B, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelHT40_1S_B, buf1, MAX_2G_CHANNEL_NUM_MIB);
		
 #if defined(CONFIG_RTL_92D_SUPPORT)
	apmib_get(MIB_HW_TX_POWER_5G_HT40_1S_A, (void *)buf1);
	memcpy(power_scaled_result->pwrlevel5GHT40_1S_A, buf1, MAX_5G_CHANNEL_NUM_MIB);
	
	apmib_get(MIB_HW_TX_POWER_5G_HT40_1S_B, (void *)buf1);
	memcpy(power_scaled_result->pwrlevel5GHT40_1S_B, buf1, MAX_5G_CHANNEL_NUM_MIB);
 #endif

	// scale it!
	apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&intVal);
	if(intVal == 1)
		intVal = 3;
	else if(intVal == 2)
			intVal = 6;
		else if(intVal == 3)
				intVal = 9;
			else if(intVal == 4)
					intVal = 17;
	if (intVal) {
		for (i=0; i<MAX_2G_CHANNEL_NUM_MIB; i++) {
			if(power_scaled_result->pwrlevelCCK_A[i] != 0){ 
				if ((power_scaled_result->pwrlevelCCK_A[i] - intVal) >= 1)
					power_scaled_result->pwrlevelCCK_A[i] -= intVal;
				else
					power_scaled_result->pwrlevelCCK_A[i] = 1;
			}
			if(power_scaled_result->pwrlevelCCK_B[i] != 0){ 
				if ((power_scaled_result->pwrlevelCCK_B[i] - intVal) >= 1)
					power_scaled_result->pwrlevelCCK_B[i] -= intVal;
				else
					power_scaled_result->pwrlevelCCK_B[i] = 1;
			}
			if(power_scaled_result->pwrlevelHT40_1S_A[i] != 0){ 
				if ((power_scaled_result->pwrlevelHT40_1S_A[i] - intVal) >= 1)
					power_scaled_result->pwrlevelHT40_1S_A[i] -= intVal;
				else
					power_scaled_result->pwrlevelHT40_1S_A[i] = 1;
			}
			if(power_scaled_result->pwrlevelHT40_1S_B[i] != 0){ 
				if ((power_scaled_result->pwrlevelHT40_1S_B[i] - intVal) >= 1)
					power_scaled_result->pwrlevelHT40_1S_B[i] -= intVal;
				else
					power_scaled_result->pwrlevelHT40_1S_B[i] = 1;
			}
		}	
		
#if defined(CONFIG_RTL_92D_SUPPORT)			
		for (i=0; i<MAX_5G_CHANNEL_NUM_MIB; i++) {
			if(power_scaled_result->pwrlevel5GHT40_1S_A[i] != 0){ 
				if ((power_scaled_result->pwrlevel5GHT40_1S_A[i] - intVal) >= 1)
					power_scaled_result->pwrlevel5GHT40_1S_A[i] -= intVal;
				else
					power_scaled_result->pwrlevel5GHT40_1S_A[i] = 1;					
			}
			if(power_scaled_result->pwrlevel5GHT40_1S_B[i] != 0){ 
				if ((power_scaled_result->pwrlevel5GHT40_1S_B[i] - intVal) >= 1)
					power_scaled_result->pwrlevel5GHT40_1S_B[i] -= intVal;
				else
					power_scaled_result->pwrlevel5GHT40_1S_B[i] = 1;
			}
		}
#endif //#if defined(CONFIG_RTL_92D_SUPPORT)						
	}	

#else
//!CONFIG_RTL_8196B => rtl8651c+rtl8190
	// get original values (100%)
	apmib_get(MIB_HW_TX_POWER_CCK, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelCCK, buf1, 14);
	
	apmib_get(MIB_HW_TX_POWER_OFDM, (void *)buf1);
	memcpy(power_scaled_result->pwrlevelOFDM, buf1, 162);


	// set output power scale
	apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&intVal);
	if(intVal == 1)
		intVal = 3;
	else if(intVal == 2)
			intVal = 6;
		else if(intVal == 3)
				intVal = 9;
			else if(intVal == 4)
					intVal = 17;
	if (intVal) {
		for (i=0; i<14; i++) {
			if(power_scaled_result->pwrlevelCCK[i] != 0){ 
				if ((power_scaled_result->pwrlevelCCK[i] - intVal) >= 1)
					power_scaled_result->pwrlevelCCK[i] -= intVal;
				else
					power_scaled_result->pwrlevelCCK[i] = 1;
			}
		}
		for (i=0; i<162; i++) {
			if (power_scaled_result->pwrlevelOFDM[i] != 0){
				if((power_scaled_result->pwrlevelOFDM[i] - intVal) >= 1)
					power_scaled_result->pwrlevelOFDM[i] -= intVal;
				else
					power_scaled_result->pwrlevelOFDM[i] = 1;
			}
		}		
	}
#endif  //For Check CONFIG_RTL_8196B

	return CW_TRUE;
}

CWBool cwRtkWtpCfgPowerScales(int nRadios, const CWProtocolRtkPowerScaleValues *pRadioConfig, CWBool reload_if_changed, CWBool *needReloadLater)
{
	CWBool cfgChanged;
	extern int wlan_idx;
	extern int vwlan_idx;
	int i, iVal;
	enum {
		UNSET,
		CHANGED,
		UNCHANGED,
	} radio_status [RTK_RADIOS];

	CWLog("%s started", __FUNCTION__);
	// ======== Step 1: check input values =============
	for (i=0; i<nRadios; i++) {
		if (pRadioConfig[i].radio_id >= RTK_RADIOS) return CW_FALSE;
		switch(pRadioConfig[i].power_scale) {
			case POWER_SCALE_100:
			case POWER_SCALE_70:
			case POWER_SCALE_50:
			case POWER_SCALE_35:
			case POWER_SCALE_15:
				break;
			default:
				return CW_FALSE;
				break;
		}
	}

	// ======== Step 2: set into flash ==============
	// init 
	cfgChanged = CW_FALSE;
	for (i=0; i<RTK_RADIOS; i++) {
		radio_status[i] = UNSET;
	}

	// set into shared meomry
	for (i=0; i<nRadios; i++) {		
		wlan_idx = pRadioConfig[i].radio_id;
		vwlan_idx = 0;
		radio_status[wlan_idx] = UNCHANGED;

		// RFPOWER_SCALE
		apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&iVal);
		if (iVal != pRadioConfig[i].power_scale) {
			CWLog("Power of %s changing from scale %d to scale %d", rtk_ifname[wlan_idx][vwlan_idx], iVal, pRadioConfig[i].power_scale);
			// power different
			radio_status[wlan_idx] = CHANGED;
			cfgChanged = CW_TRUE;
			iVal = pRadioConfig[i].power_scale;
			apmib_set(MIB_WLAN_RFPOWER_SCALE, (void *)&iVal);
		}
	}

	*needReloadLater = cfgChanged;
	if (!cfgChanged) {
		return CW_TRUE;
	} 

	// set into flash
	if (!apmib_update(CURRENT_SETTING)) {
		return CW_FALSE;
	}

	// ======== Step 3: update to driver ==============
	if (reload_if_changed) {
		CWLog("Set to Driver for New Power Scale Config");
#if 0
		system("sysconf init gw all");
#else
		int i;
		for (i=0; i<nRadios; i++) {
			rtk_power_scaled_result_t power_scaled_result;
			if (__calculate_scaled_power(pRadioConfig[i].radio_id, &power_scaled_result)) {
				if (!cwRtkWtpSetPowerScale(pRadioConfig[i].radio_id, &power_scaled_result))
					return CW_FALSE;
			}
		}
#endif
		*needReloadLater = CW_FALSE;
	}

	CWLog("%s succesfully completed", __FUNCTION__);
	return CW_TRUE;

}
CWBool cwRtkWtpCfgChannels(int nRadios, const CWProtocolRtkChannelValues *pRadioConfig, CWBool reload_if_changed, CWBool *needReloadLater)
{
	CWBool cfgChanged;
	extern int wlan_idx;
	extern int vwlan_idx;
	int i, iVal;
	enum {
		UNSET,
		CHANGED,
		UNCHANGED,
	} radio_status [RTK_RADIOS];

	CWLog("%s started", __FUNCTION__);

	// ======== Step 1: check input values =============
	for (i=0; i<nRadios; i++) {
		if (pRadioConfig[i].radio_id >= RTK_RADIOS) return CW_FALSE;
		// TODO: check channel value........
	}

	// ======== Step 2: set into flash ==============
	// init 
	cfgChanged = CW_FALSE;
	for (i=0; i<RTK_RADIOS; i++) {
		radio_status[i] = UNSET;
	}

	// set into shared meomry
	for (i=0; i<nRadios; i++) {
		wlan_idx = pRadioConfig[i].radio_id;
		vwlan_idx = 0;
		radio_status[wlan_idx] = UNCHANGED;

		// CHANNEL
		apmib_get(MIB_WLAN_CHANNEL, (void *)&iVal);
		if (iVal != pRadioConfig[i].channel) {
			CWLog("Channel of %s changing from scale %d to scale %d", rtk_ifname[wlan_idx][vwlan_idx], iVal, pRadioConfig[i].channel);
			// channel different
			radio_status[wlan_idx] = CHANGED;
			cfgChanged = CW_TRUE;
			iVal = pRadioConfig[i].channel;
			apmib_set(MIB_WLAN_CHANNEL, (void *)&iVal);
		}

	}

	*needReloadLater = cfgChanged;
	if (!cfgChanged) {
		return CW_TRUE;
	}

	// set into flash
	if (!apmib_update(CURRENT_SETTING)) {
		return CW_FALSE;
	}

	// ======== Step 3: update to driver ==============
	if (reload_if_changed) {
		CWLog("Set to Driver for New Channel Config");
#if 0
		system("sysconf init gw all");
#else
		int i;

		for (i=0; i<nRadios; i++) {
			if (!cwRtkWtpSetChannel(pRadioConfig[i].radio_id, pRadioConfig[i].channel)) {
				return CW_FALSE;
			}
		}
#endif
		*needReloadLater = CW_FALSE;
	}
	
	CWLog("%s succesfully completed", __FUNCTION__);
	return CW_TRUE;

}

void cwRtkWtpActivateFlashCfg()
{
#if 1
	int i, j;
	CWLog("RTK reloading system... (wlan interfaces only)");
	for (i=0; i<RTK_RADIOS; i++) {
		for (j=0; j<RTK_VAPS+1; j++) {
			const char *ifname = rtk_ifname[i][j];
			char cmd[128];
			CWLog("reload %s config", ifname);
			sprintf(cmd, "flash set_mib %s", ifname);
			system(cmd);
			sprintf(cmd, "brctl delif %s %s", BR_NAME, ifname);
			system(cmd);
		}
	}

	for (i=0; i<RTK_RADIOS; i++) {
		for (j=0; j<RTK_VAPS+1; j++) {
			int disabled;
			wlan_idx = i;
			vwlan_idx = j;
			apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disabled);
			if (!disabled) {
				const char *ifname = rtk_ifname[i][j];
				char cmd[128];
				CWLog("bring %s up", ifname);
				sprintf(cmd, "ifconfig %s up", ifname);
				system(cmd);
				CWLog("bring %s up", ifname);
				sprintf(cmd, "brctl addif %s %s", BR_NAME, ifname);
				system(cmd);
			}
		}
	}
#else
	CWLog("RTK reloading system...");
	system("sysconf init gw all");
#endif

}
#else
CWBool cwRtkWtpCfgWlans(int nWlans, const CWProtocolRtkWlanConfigValues *pWlanConfig, 
						CWBool invalid_all_others, CWBool reload_if_changed,
						CWBool *needReloadLater)
{
	*needReloadLater = CW_FALSE;
	return CW_TRUE;
}
CWBool cwRtkWtpCfgPowerScales(int nRadios, const CWProtocolRtkPowerScaleValues *pRadioConfig, CWBool reload_if_changed, CWBool *needReloadLater)
{
	*needReloadLater = CW_FALSE;
	return CW_TRUE;
}
CWBool cwRtkWtpCfgChannels(int nRadios, const CWProtocolRtkChannelValues *pRadioConfig, CWBool reload_if_changed, CWBool *needReloadLater)
{
	*needReloadLater = CW_FALSE;
	return CW_TRUE;
}
void cwRtkWtpActivateFlashCfg()
{
}
#endif


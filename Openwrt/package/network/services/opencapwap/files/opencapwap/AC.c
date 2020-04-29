/*******************************************************************************************
 *  Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
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
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *******************************************************************************************/
#ifdef _MEM_LEAK_DBG_
#include <mcheck.h>
#endif

#include "CWAC.h"
#include "CWCommon.h"
/* 2014 Babylon: we have removed TAP
#include "tap.h"
 */
#include "ACRtkSystemInteraction.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

/*_________________________________________________________*/
/*  *******************___VARIABLES___*******************  */
CWThreadMutex gCreateIDMutex;

#ifdef RTK_SMART_ROAMING
wtp_wlan_config	gWTPsWlan[AC_MAX_WTP_NUM];
unsigned char	gWTPsStatus[AC_MAX_WTP_NUM];
unsigned int	gWTPsSpeed[AC_MAX_WTP_NUM];
unsigned char	gStationsSignalMin[RTK_RADIOS];
unsigned char	gStationsLoadMin[RTK_RADIOS];
unsigned char	gStationsSpeedWeight[RTK_RADIOS];
unsigned char	gStationsSignalWeight[RTK_RADIOS];
unsigned char	gStationsLoadWeight[RTK_RADIOS];
unsigned int	gStationsLinktimeMimimum[RTK_RADIOS];
#endif

/* array that stores per WTPs infos */
CWWTPManager *gWTPs; // array size = gMaxWTPs
CWThreadMutex gWTPsMutex;

/* number of active WTPs */
int gActiveWTPs = 0;
CWThreadMutex gActiveWTPsMutex;

/* max WTPs */
int gMaxWTPs;
/* The Radio MAC Field of the discovery response */
int gRMACField = 0;
/* The Wireless Field of the discovery response */
int gWirelessField = 0;
/* DTLS Policy for data channel */
int gDTLSPolicy=DTLS_ENABLED_DATA;
/* special socket to handle multiple network interfaces */
CWMultiHomedSocket gACSocket;
/* AC's network interfaces */
CWProtocolNetworkInterface *gInterfaces = NULL;
int gInterfacesCount = 0;
/* DTLS Context */
#ifndef CW_NO_DTLS
CWSecurityContext gACSecurityContext;
#endif
int gActiveStations = 0;
/* max stations */
int gMaxStations;
char **gMulticastGroups;
int gMulticastGroupsCount;
CWAuthSecurity gACDescriptorSecurity;
int gACHWVersion;
int gACSWVersion;
char *gACName = NULL;

int gDiscoveryTimer=20;
//#ifdef RTK_CAPWAP
//int gEchoRequestTimer=CW_ECHO_INTERVAL_DEFAULT*2;
//#else
int gEchoRequestTimer=CW_ECHO_INTERVAL_DEFAULT;
//#endif
/* PROVVISORIO: Il valore e' scelto a caso */
int gIdleTimeout=10; 
	//Babylon: The idiot Italian chose 10 seconds as the default idle timeout. I'll make it configuriable in the future.

#ifdef RTK_SMART_ROAMING
CWACStations gStations;
CWACStationReq gStationReq;
#endif

UNIX_SOCKS_INFO *UnixSocksArray; // array size = gMaxWTPs

/*_________________________________________________________*/
/*  *******************___FUNCTIONS___*******************  */
int main (int argc, const char * argv[])
{
	pid_t pid;
	
#ifdef _MEM_LEAK_DBG_
	mtrace();
#endif
	if (argc <= 1) {
		printf("Usage: AC working_path\n");
		exit(0);
	}
	
#ifdef RTK_CAPWAP
	/* Init for RTK AP */
	if (!cwRtkAcInit()) {		
		printf("cwRtkWtpInit() error!\n");
		exit(0);
	}
	
	/* Check mib */
	if (!cwRtkAcSystemEnabled()) {
		printf("CAPWAP AC is not enabled.\n");
		exit(0);
	}
#endif

	/* Daemon Mode */
	if ((pid = fork()) < 0)
		exit(1);
	else if (pid != 0)
		exit(0);
	else {
		setsid();
		if (chdir(argv[1]) != 0){
			printf("chdir Fail\n");
			exit(1);
		}
//		fclose(stdout);
	}

	{
		gProgName=argv[0];
	}

	// destroy old process and create a PID file
	{
		int pid_fd;
		FILE *fp;
		char line[20], pid_file_name[100];
		pid_t pid;

		sprintf(pid_file_name,"/etc/capwap/AC.pid");

		if ((fp = fopen(pid_file_name, "r")) != NULL) {
			fgets(line, sizeof(line), fp);
			if (sscanf(line, "%d", &pid)) {
				if (pid > 1)
					kill(pid, SIGTERM);
			}
			fclose(fp);
		}
		pid_fd = pidfile_acquire(pid_file_name);
		if (pid_fd < 0)
			return 0;

		pidfile_write_release(pid_fd);
	}
	
	CWACInit();
#ifdef KM_BY_AC // key management
	CWCreateConnectionWithHostapdAC();
#endif
	CWACEnterMainLoop();
	
	CWACDestroy(); 
	
#ifdef _MEM_LEAK_DBG_
	muntrace();
#endif
	return 0;
}

#ifndef CW_NO_DTLS
int CWACSemPostForOpenSSLHack(void *s) {

	CWThreadTimedSem *semPtr = (CWThreadTimedSem*) s;
	
	if(!CWThreadTimedSemIsZero(semPtr)) {
		CWLog("This Semaphore's Value should really be 0");
		/* note: we can consider setting the value to 0 and going on,
		 * that is what we do here
		 */
		if(!CWErr(CWThreadTimedSemSetValue(semPtr, 0))) return 0;
	}
	
	if(!CWErr(CWThreadTimedSemPost(semPtr))) {
		return 0;
	}
	 
	return 1;
}
#endif

void CWACInit() {
	int i;
	CWNetworkLev4Address *addresses = NULL;
	struct sockaddr_in *IPv4Addresses = NULL;	

	CWLogInitFile(AC_LOG_FILE_NAME);
#ifdef RTK_SMART_ROAMING
	CWRoamLogInitFile(SMART_ROAMING_AC_LOG_FILE_NAME);
#endif
	
	#ifndef CW_SINGLE_THREAD
		CWDebugLog("Use Threads");
	#else
		CWDebugLog("Don't Use Threads");
	#endif
	
	CWErrorHandlingInitLib();
	
	if(!CWParseSettingsFile())
	{
		CWLog("Can't start AC");
		exit(1);
	}
	CWLog("Starting AC");

	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);
	if (timer_init() == 0) {
		CWLog("Can't init timer module");
		exit(1);
	}

	if(!CWErr(CWParseConfigFile()) ||
#ifndef CW_NO_DTLS
	   !CWErr(CWSecurityInitLib()) ||
#endif
	   !CWErr(CWNetworkInitSocketServerMultiHomed(&gACSocket, CW_CONTROL_PORT, gMulticastGroups, gMulticastGroupsCount)) ||
	   !CWErr(CWNetworkGetInterfaceAddresses(&gACSocket, &addresses, &IPv4Addresses)) ||
	   !CWErr(CWCreateThreadMutex(&gWTPsMutex)) ||
	   !CWErr(CWCreateThreadMutex(&gActiveWTPsMutex))) {

		/* error starting */
		CWLog("Can't start AC");
		exit(1);
	}

#ifndef CW_NO_DTLS
	if(gACDescriptorSecurity == CW_X509_CERTIFICATE) {

		if(!CWErr(CWSecurityInitContext(&gACSecurityContext,
						"root.pem",
						"server.pem",
						"prova",
						CW_FALSE,
						CWACSemPostForOpenSSLHack))) {

			CWLog("Can't start AC");
			exit(1);
		}
	} else { /* preshared */
		if(!CWErr(CWSecurityInitContext(&gACSecurityContext,
						NULL,
						NULL,
						NULL,
						CW_FALSE,
						CWACSemPostForOpenSSLHack))) {
			CWLog("Can't start AC");
			exit(1);
		}
	}
#endif
	CW_FREE_OBJECTS_ARRAY(gMulticastGroups, gMulticastGroupsCount);
	CW_CREATE_ARRAY_ERR(gWTPs, gMaxWTPs, CWWTPManager, {CWLog("Cannot create gWTPs"); exit(1);});
	CW_CREATE_ARRAY_ERR(UnixSocksArray, gMaxWTPs, UNIX_SOCKS_INFO, {CWLog("Cannot create UnixSocksArray"); exit(1);});

	//printf("sizeof(CWWTPManager)=%u, # of gWTPs=%u\n", sizeof(CWWTPManager), gMaxWTPs);


	for(i = 0; i < gMaxWTPs; i++) {
		gWTPs[i].isNotFree = CW_FALSE;
	}
#ifdef	RTK_SMART_ROAMING
	memset(gWTPsStatus, 0, AC_MAX_WTP_NUM);
	memset(gWTPsSpeed, 0, AC_MAX_WTP_NUM);
#endif

	/* store network interface's addresses */
	gInterfacesCount = CWNetworkCountInterfaceAddresses(&gACSocket);
	CWLog("Found %d Network Interface(s)", gInterfacesCount);
	
	if (gInterfacesCount<=0){
		CWLog("Can't start AC");
		exit(1);
	}

	CW_CREATE_ARRAY_ERR(gInterfaces, 
			    gInterfacesCount,
			    CWProtocolNetworkInterface,
			    CWLog("Out of Memory"); return;);

	for(i = 0; i < gInterfacesCount; i++) {
		gInterfaces[i].WTPCount = 0;
		CW_COPY_NET_ADDR_PTR(&(gInterfaces[i].addr), ((CWNetworkLev4Address*)&((addresses)[i])) );
		if(IPv4Addresses != NULL) {
			CW_COPY_NET_ADDR_PTR(&(gInterfaces[i].addrIPv4), &((IPv4Addresses)[i]));
		}
	}
	CW_FREE_OBJECT(addresses);
	CW_FREE_OBJECT(IPv4Addresses);

	if(!CWErr(CWCreateThreadMutex(&gCreateIDMutex))) {
		exit(1);
	}

#ifdef RTK_SMART_ROAMING
	/* for stations */
	CWStationsInit();
	CWCreateThreadMutex(&gStationReq.lock);
	gStationReq.seqNum = -1;
#endif

	CWLog("AC Started");
}

#ifdef KM_BY_AC // key management
void CWCreateConnectionWithHostapdAC(){
	
	CWThread thread_ipc_with_ac_hostapd;
	if(!CWErr(CWCreateThread(&thread_ipc_with_ac_hostapd, CWACipc_with_ac_hostapd, NULL))) {
		CWLog("Error starting Thread that receive command and 802.11 frame from hostapd (WTP side)");
		exit(1);
	}
	
}
#endif

void CWACDestroy() {
	
	CWNetworkCloseMultiHomedSocket(&gACSocket);
	
	/*
	for(i = 0; i < CW_MAX_WTP; i++) {
		//CW_FREE_OBJECT(gWTPs[i].addr);
	}
	*/
#ifndef CW_NO_DTLS
	CWSslCleanUp();
#endif
	CWDestroyThreadMutex(&gWTPsMutex);
	CWDestroyThreadMutex(&gCreateIDMutex);
	CWDestroyThreadMutex(&gActiveWTPsMutex);
	
	CW_FREE_OBJECT(gACName);
	CW_FREE_OBJECT(gInterfaces);
	
	CWLog("AC Destroyed");
}


__inline__ unsigned int CWGetSeqNum() {

	static unsigned int seqNum = 0;
	unsigned int r;
	
	if(!CWThreadMutexLock(&gCreateIDMutex)) {
		
		CWDebugLog("Error Locking a mutex");
	}
	
	r = seqNum;
	
	if (seqNum==CW_MAX_SEQ_NUM) 
		seqNum=0;
	else 
		seqNum++;

	CWThreadMutexUnlock(&gCreateIDMutex);
	return r;
}


__inline__ int CWGetFragmentID() {

	static int fragID = 0;
	int r;

	if(!CWThreadMutexLock(&gCreateIDMutex)) {
		
		CWDebugLog("Error Locking a mutex");
	}
	
	r = fragID;
	
	if (fragID==CW_MAX_FRAGMENT_ID) 
		fragID=0;
	else 
		fragID++;

	CWThreadMutexUnlock(&gCreateIDMutex);
	return r;
}

#ifdef RTK_SMART_ROAMING
// ==================================================================
//				To Manage Stations
// ==================================================================
CWBool CWStationsInit()
{
	int i;

	for(i=0; i<RTK_RADIOS; i++){
		get_mib(rtk_ifname[i][0], "signal_min", &gStationsSignalMin[i], sizeof(unsigned char));
		get_mib(rtk_ifname[i][0], "load_min", &gStationsLoadMin[i], sizeof(unsigned char));
		get_mib(rtk_ifname[i][0], "speed_weight", &gStationsSpeedWeight[i], sizeof(unsigned char));
		get_mib(rtk_ifname[i][0], "signal_weight", &gStationsSignalWeight[i], sizeof(unsigned char));
		get_mib(rtk_ifname[i][0], "load_weight", &gStationsLoadWeight[i], sizeof(unsigned char));
		get_mib(rtk_ifname[i][0], "linktime_min", &gStationsLinktimeMimimum[i], sizeof(unsigned int));


		CWRoamLog("");
		CWRoamLog("%s: signal_min=%d load_min=%d speed_weight=%d signal_weight=%d load_weight=%d linktime_min=%d",
					rtk_ifname[i][0],gStationsSignalMin[i],gStationsLoadMin[i],
					gStationsSpeedWeight[i],gStationsSignalWeight[i],gStationsLoadWeight[i], gStationsLinktimeMimimum[i]);
	}

	for (i=0; i<CW_STATION_HASH_MAX; i++) { 	
		if (!CWCreateThreadMutex(&gStations.hash[i].lock)) return CW_FALSE;
		gStations.hash[i].list_head = NULL;
	}
	return CW_TRUE;
}
CWBool CWStationFind(const uint8_t *mac, station_t *station)
{
	uint32_t i;
	station_t *p;
	CWBool ret = CW_FALSE;
	i = CW_STATION_HASH_FUNC(mac);
	CWThreadMutexLock(&gStations.hash[i].lock);
	p = gStations.hash[i].list_head;
	while (p){
		if (memcmp(p->mac_addr, mac, 6)==0)	break;
		else	p = p->next;
	}
	if (p) {
		*station = *p;		ret = CW_TRUE;
	}
	
	CWThreadMutexUnlock(&gStations.hash[i].lock);
	return ret;
}
CWBool CWStationDelete(const uint8_t *mac)
{
	uint32_t i;
	station_t *p, *q;
	CWBool ret;
	i = CW_STATION_HASH_FUNC(mac);
	CWThreadMutexLock(&gStations.hash[i].lock);
	p = gStations.hash[i].list_head;
	q = NULL;
	while (p)
	{
		if (memcmp(p->mac_addr, mac, 6)==0) {
			break;
		} else {
			q = p;
			p = p->next;
			
		}
	}
	if (!p) {
		printf("%s: Fail to Delete Station:%02x:%02x:%02x:%02x:%02x:%02x (not exist)", __FUNCTION__, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		ret = CW_FALSE;
	} else {
		if (p == gStations.hash[i].list_head) {
			gStations.hash[i].list_head = p->next;
		} else {
			q->next = p->next;
		}
		free(p);
		gActiveStations--;
		ret = CW_TRUE;		
		CWRoamLog("Delete Station %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}
	CWThreadMutexUnlock(&gStations.hash[i].lock);
	return ret;
}

CWBool CWStationCheckDelete()
{
	int i;
	unsigned int timestamp = UINT_MAX;
	unsigned char *mac_addr;
	station_t *p;

	for (i=0; i<CW_STATION_HASH_MAX; i++) { 			
		CWThreadMutexLock(&gStations.hash[i].lock);
		p = gStations.hash[i].list_head;
		while(p)
		{
			if(p->timestamp[p->wtp_index] < timestamp){
				timestamp = p->timestamp[p->wtp_index];
				mac_addr = p->mac_addr;
			}
			p = p->next;
		}
		CWThreadMutexUnlock(&gStations.hash[i].lock);
	}

	return CWStationDelete(mac_addr);
}

unsigned int CWStationEvaluateWtp(int wtp_index, station_t *pstat, int info_type, unsigned int current_time)
{
	unsigned int ret = 0;

	if(!gWTPsStatus[wtp_index])
		goto end;

	unsigned char signal, ch_util;	
	switch(info_type){
		case STATION_INFO_TYPE_REFERENCE:
			if(current_time-pstat->timestamp[wtp_index] > ROAMING_EFFECTIVE_TIME)
				goto end;
			
			signal = pstat->wlan[pstat->radio_id].rssi[wtp_index];
			ch_util = pstat->wlan[pstat->radio_id].ch_util[wtp_index];

			break;

		case STATION_INFO_TYPE_DOT11K_REFERENCE:
			if(current_time-pstat->dot11k_timestamp[wtp_index] > ROAMING_EFFECTIVE_TIME)
				goto end;

			signal = pstat->wlan[pstat->radio_id].dot11k_rcpi[wtp_index];
			ch_util = pstat->wlan[pstat->radio_id].dot11k_cu[wtp_index];

			break;

		default:
			CWRoamDebugLog("Wrong info_type: %d (%s)", info_type, __FUNCTION__);
			goto end;
	}

	CWRoamDebugLog("%s wtp_index=%d signal=%d gStationsSignalMin=%d ch_util=%d gStationsLoadMin=%d", __func__, wtp_index, signal, gStationsSignalMin, ch_util, gStationsLoadMin);
	if(signal<gStationsSignalMin[pstat->radio_id] || ch_util<gStationsLoadMin[pstat->radio_id])
		goto end;
	else
		ret = gWTPsSpeed[wtp_index]*gStationsSpeedWeight[pstat->radio_id] + signal*gStationsSignalWeight[pstat->radio_id];

	CWRoamDebugLog("%s() gWTPsSpeed[%d]=%d gStationsSpeedWeight=%d signal=%d gStationsSignalWeight=%d  retuen %d",
			__func__, wtp_index, gWTPsSpeed[wtp_index], gStationsSpeedWeight[pstat->radio_id], signal, gStationsSignalWeight[pstat->radio_id], ret);

end:
	return ret;
}

int CWStationFindBestWtp(station_t *pstat, int info_type, unsigned int current_time)
{
	int k, best_wtp_index, best_wtp_signal_index;
	unsigned int reference, best_reference;
	unsigned char best_signal=0;

	best_wtp_index = pstat->wtp_index;
	best_wtp_signal_index = best_wtp_index;
	best_signal = pstat->wlan[pstat->radio_id].rssi[best_wtp_signal_index] + (gWTPsSpeed[best_wtp_index]==500?3:0);
	reference = CWStationEvaluateWtp(pstat->wtp_index,pstat,info_type,current_time);	
	best_reference = reference;
	
	CWRoamDebugLog("! best_wtp[%d]=%d",best_wtp_index, reference);
	switch(info_type){
		case STATION_INFO_TYPE_REFERENCE:
			for(k=0; k<AC_MAX_WTP_NUM; k++)
			{
				if(!gWTPsStatus[k] || k==pstat->wtp_index)
					continue;

				pstat->wlan[pstat->radio_id].reference[k] = CWStationEvaluateWtp(k,pstat,info_type,current_time);
				CWRoamDebugLog("best_wtp[%d]=%d current[%d]=%d",best_wtp_index, best_reference, k, pstat->wlan[pstat->radio_id].reference[k]);
				if(pstat->wlan[pstat->radio_id].reference[k] >= reference+ROAMING_REFERENCE_DIFFERENCE
					&& pstat->wlan[pstat->radio_id].reference[k] > best_reference)
				{
					best_wtp_index = k; 
					best_reference = pstat->wlan[pstat->radio_id].reference[k];
				}

				if( (pstat->wlan[pstat->radio_id].rssi[k] + (gWTPsSpeed[k]==500?3:0))  > (best_signal +3) ) {
					best_wtp_signal_index = k;
					best_signal = pstat->wlan[pstat->radio_id].rssi[k] + (gWTPsSpeed[k]==500?3:0);
				}
			}

			if(best_reference == 0) {
				best_wtp_index = best_wtp_signal_index;
				CWRoamDebugLog("No Good Signal, choose WTP[%d] with RSSI(%d) as best WTP", best_wtp_index, best_signal);
			}
			break;
		case STATION_INFO_TYPE_DOT11K_REFERENCE:
			for(k=0; k<AC_MAX_WTP_NUM; k++)
			{
				if(!gWTPsStatus[k] || k==pstat->wtp_index)
					continue;

				pstat->wlan[pstat->radio_id].reference[k] = CWStationEvaluateWtp(k,pstat,info_type,current_time);
				if(pstat->wlan[pstat->radio_id].reference[k] >= reference+ROAMING_REFERENCE_DIFFERENCE
					&& pstat->wlan[pstat->radio_id].reference[k] > best_reference)
				{
					best_wtp_index = k; 
					best_reference= pstat->wlan[pstat->radio_id].reference[k];
				}
			}
			break;
		default:
			CWRoamDebugLog("Wrong info_type: %d (%s)", info_type, __FUNCTION__);
	}
	
	return best_wtp_index;
}

CWBool CWStationCheckAllWtp(station_t *pstat, unsigned int current_time)
{
	int k;
	
	for(k=0; k<AC_MAX_WTP_NUM; k++)
	{
		if(gWTPsStatus[k] && current_time-pstat->timestamp[k]>ROAMING_EFFECTIVE_TIME*2)
			return CW_FALSE;
	}

	if(pstat->dot11k_flag)
	{
		for(k=0; k<AC_MAX_WTP_NUM; k++)
		{
			if(gWTPsStatus[k] && current_time-pstat->dot11k_timestamp[k]>ROAMING_EFFECTIVE_TIME*2)
				return CW_FALSE;			
		}
	}

	return CW_TRUE;

}

CWBool CWStationCheckDisassocCount(station_t *pstat, unsigned int current_time)
{
	switch(pstat->disassoc_count){
		case 0:
			return CW_TRUE;
		case 1:
			if(current_time-pstat->disassoc_timestamp >= 60)
				return CW_TRUE;
			break;
		case 2:
			if(current_time-pstat->disassoc_timestamp >= 300)
				return CW_TRUE;			
			break;
		case 3:
			if(current_time-pstat->disassoc_timestamp >= 600)
				return CW_TRUE;			
			break;
		default:	
			if(current_time-pstat->disassoc_timestamp >= 1800)
				return CW_TRUE;		
			break;
	}
	
	return CW_FALSE;
}  

CWBool CWStationUpdateThrshCount(station_t *pstat, CWProtocolAddStationValues *add_station, unsigned int current_time)
{	
	if(add_station->rssi<gStationsSignalMin[add_station->radio_id] || add_station->data_rate<ROAMING_RATE_MINIMUM)
	{
		pstat->rssi_thrsh_count += 1;

		if(pstat->roam_status==NO_ROAM && pstat->rssi_thrsh_count>=ROAMING_REACHTHRSH_MAXIMUM)
		{
			if(pstat->link_time >= gStationsLinktimeMimimum[pstat->radio_id]
					&& !CWStationCheckAllWtp(pstat, current_time)
					&& CWStationCheckDisassocCount(pstat, current_time))
			{					
				CWRoamLog("");
				CWRoamLog("Process disassoc of %02x:%02x:%02x:%02x:%02x:%02x (signal)",
						pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
						pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5]);

                CWIssueSilenceNotify(pstat->wtp_index, pstat->radio_id, pstat->mac_addr);
				pstat->disassoc_count += 1;
				pstat->disassoc_timestamp = current_time;

				return CW_TRUE;
			}
		}
	}
	else 
		pstat->rssi_thrsh_count = 0;

	if(add_station->ch_util < gStationsLoadMin[add_station->radio_id])
	{
		pstat->cu_thrsh_count += 1;
		
		if(pstat->roam_status==NO_ROAM && pstat->cu_thrsh_count>=ROAMING_REACHTHRSH_MAXIMUM)
		{
			if(pstat->link_time >= gStationsLinktimeMimimum[pstat->radio_id]
					&& !CWStationCheckAllWtp(pstat, current_time)
					&& CWStationCheckDisassocCount(pstat, current_time))
			{					
				CWRoamLog("");
				CWRoamLog("Process disassoc of %02x:%02x:%02x:%02x:%02x:%02x (loading)",
						pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
						pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5]);

                CWIssueSilenceNotify(pstat->wtp_index, pstat->radio_id, pstat->mac_addr);
				pstat->disassoc_count += 1;
				pstat->disassoc_timestamp = current_time;

				return CW_TRUE;
			}
		}
	}
	else 
		pstat->cu_thrsh_count = 0;

	return CW_TRUE;
}

//normal case: roam_status only reset in this func
CWBool CWStationNewNotify(int wtp_index, unsigned char radio_id, const unsigned char *mac)
{
	uint32_t i;
	int k=0;
	station_t *pstat;
	unsigned long roam_duration;
	
	i = CW_STATION_HASH_FUNC(mac);
	CWThreadMutexLock(&gStations.hash[i].lock);
	pstat = gStations.hash[i].list_head;	
	
	while (pstat)
	{
		if(memcmp(pstat->mac_addr,mac,6)==0){
			pstat->rssi_thrsh_count = 0;
			pstat->cu_thrsh_count = 0;
			pstat->best_wtp_index = -1;
			pstat->dot11k_best_wtp_index = -1;
			break;
		}else
			pstat = pstat->next;
	}

	if(pstat)
	{	
// do NOT change the sequence to check, status should be NO_ROAM after checking
		if(pstat->roam_status == NEVER)
			pstat->roam_status = NO_ROAM;
		if(pstat->roam_status == NO_ROAM
			&& (pstat->wtp_index!=wtp_index || pstat->radio_id!=radio_id))
		{
			pstat->roam_own++;
			pstat->failure_count = 0;
			CWRoamDebugLog("%02x:%02x:%02x:%02x:%02x:%02x roam on its own (sucess=%d failure=%d own=%d)",
							pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
							pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5],
							pstat->roam_pass,pstat->roam_fail,pstat->roam_own);
		}
		else if(pstat->roam_status==SILENT || pstat->roam_status==SILENT_ING || pstat->roam_status==DOT11V || pstat->roam_status==DOT11V_ING)
		{
			pstat->roam_status = NO_ROAM;
			roam_duration = CWGetTimeInMilliSeconds()-pstat->roam_timestamp;
			if(pstat->roam_wtp_index == wtp_index){
				pstat->roam_pass++;
				pstat->failure_count = 0;
				CWRoamLog("Station %02x:%02x:%02x:%02x:%02x:%02x roam to best AP (time=%lums sucess=%d failure=%d own=%d)",
							mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
							roam_duration,pstat->roam_pass,pstat->roam_fail,pstat->roam_own);
			}else{
				pstat->roam_fail++;
				pstat->failure_count++;
				CWRoamLog("Station %02x:%02x:%02x:%02x:%02x:%02x NOT roam to best AP (time=%lums sucess=%d failure=%d own=%d)",
							mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],
							roam_duration,pstat->roam_pass,pstat->roam_fail,pstat->roam_own);
				if(pstat->failure_count == 1)
					pstat->failure_timestamp = CWGetTimeInMins();
				else if(pstat->failure_count >= ROAMING_KICKOFF_MAXIMUM)
				{
					if(CWGetTimeInMins()-pstat->failure_timestamp <= ROAMING_KICKOFF_BOUNDTIME)
					{
						pstat->roam_status = NEVER;
						CWRoamLog("Station %02x:%02x:%02x:%02x:%02x:%02x will NEVER be processed roam",
									mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
					}else
						pstat->failure_count = 0;
				}
			}
		}

		/* wlan1 - 2.4GHz */
		if(radio_id && pstat->dual_band && BANDSTEER_DUALBAND_NOTIFY&1)
		{
			CWRoamDebugLog("(dualband_notify&1) %02x:%02x:%02x:%02x:%02x:%02x",
							pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
							pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5]);	
			CWIssueDualBandNotify(wtp_index, pstat->mac_addr);		 
		}
		/* wlan0 - 5GHz */
		else if (!radio_id && pstat->dual_band && BANDSTEER_DUALBAND_NOTIFY&2)
		{
			CWRoamDebugLog("(dualband_notify&2) %02x:%02x:%02x:%02x:%02x:%02x",
							pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
							pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5]);
			for(k=0; k<AC_MAX_WTP_NUM; k++)
			{
				if(gWTPsStatus[k])
					CWIssueDualBandNotify(k, pstat->mac_addr);		
			}		
		}
	}

	CWThreadMutexUnlock(&gStations.hash[i].lock);
	return CW_TRUE;
}
		
CWBool CWStationAdd(int wtp_index, CWProtocolAddStationValues *add_station)
{
	uint32_t i;
	station_t *pstat;
	int k,j,best_wtp_index;
	
	unsigned int current_time = CWGetTimeInSeconds();
	
	i = CW_STATION_HASH_FUNC(add_station->mac_addr);
	CWThreadMutexLock(&gStations.hash[i].lock);
	pstat = gStations.hash[i].list_head;
	while (pstat) {
		if (memcmp(pstat->mac_addr,add_station->mac_addr,6) == 0){
			pstat->timestamp[wtp_index] = current_time;
			break;
		}
		else
			pstat = pstat->next;
	}
	
	/* do malloc and init for 1st time add, NOT to check roam */
	if(!pstat)
	{
		station_t *q;
		pstat = malloc(sizeof(station_t));
		if(!pstat){
			CWThreadMutexUnlock(&gStations.hash[i].lock);
			printf("malloc fail (%s)\n", __FUNCTION__);
			return CW_FALSE;
		}
		gActiveStations++;
		q = gStations.hash[i].list_head;
		gStations.hash[i].list_head = pstat;
		pstat->next = q;
		
		memcpy(pstat->mac_addr, add_station->mac_addr,  6);
		pstat->dual_band = 0;
		pstat->dot11k_flag = 0;
		pstat->wtp_index = wtp_index;
		pstat->radio_id = add_station->radio_id;
		pstat->data_rate = add_station->data_rate;
		pstat->link_time = add_station->link_time;
		
		memset(pstat->timestamp, 0, AC_MAX_WTP_NUM);
		memset(pstat->dot11k_timestamp, 0, AC_MAX_WTP_NUM);
		pstat->timestamp[wtp_index] = current_time;	
		for(k=0; k<RTK_RADIOS; k++)
			memset(&pstat->wlan[k], 0, sizeof(wlan_info));
		pstat->wlan[pstat->radio_id].rssi[wtp_index] = add_station->rssi;				
		pstat->wlan[pstat->radio_id].ch_util[wtp_index] = add_station->ch_util;
		pstat->rssi_thrsh_count = 0;
		pstat->cu_thrsh_count = 0;
		pstat->disassoc_count = 0;

		pstat->best_wtp_index = -1;		
		pstat->dot11k_best_wtp_index = -1;
		pstat->roam_status = NO_ROAM;
		pstat->roam_pass = 0;
		pstat->roam_fail = 0;
		pstat->roam_own = 0;
		pstat->failure_count=0;

		CWRoamDebugLog("First time Add Station %02x:%02x:%02x:%02x:%02x:%02x (rssi=%d rate=MCS%d link=%d status=%d)",
						pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
						pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5],
						pstat->wlan[pstat->radio_id].rssi[pstat->wtp_index],
						pstat->data_rate,pstat->link_time,pstat->roam_status);		
		CWRoamDebugLog("\tActiveStation=%d MaxStation=%d", gActiveStations, gMaxStations);
		CWStationUpdateThrshCount(pstat, add_station, current_time);

		CWThreadMutexUnlock(&gStations.hash[i].lock);
		if(gActiveStations > gMaxStations)
			CWStationCheckDelete();	
		return CW_TRUE;
	}

	/* reset for wtp or radio changed, NOT to check for roam */
	else if((wtp_index!=pstat->wtp_index || add_station->radio_id!=pstat->radio_id)
				&& add_station->link_time<=pstat->link_time)	/* = is to fix new AP crazy free */
	{	
		/* only do reset here */
		pstat->disassoc_count = 0;		

		/* in case NewNotify is missed, do reset */
		pstat->rssi_thrsh_count = 0;
		pstat->cu_thrsh_count = 0;
		pstat->best_wtp_index = -1;		
		pstat->dot11k_best_wtp_index = -1;
		pstat->roam_status = NO_ROAM;
		pstat->failure_count = 0;

		pstat->wtp_index = wtp_index;
		pstat->radio_id = add_station->radio_id;
		pstat->data_rate = add_station->data_rate;
		pstat->link_time= add_station->link_time;
		
		pstat->wlan[pstat->radio_id].rssi[wtp_index] = add_station->rssi;	
		pstat->wlan[pstat->radio_id].ch_util[wtp_index] = add_station->ch_util;


		CWRoamDebugLog("Reset add station %02x:%02x:%02x:%02x:%02x:%02x (rssi=%d rate=MCS%d link=%d status=%d)", 
						pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
						pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5],
						pstat->wlan[pstat->radio_id].rssi[pstat->wtp_index],
						pstat->data_rate,pstat->link_time,pstat->roam_status);

		if(pstat->radio_id!=add_station->radio_id && !pstat->dual_band)
		{
			pstat->dual_band = 1;
			if(BANDSTEER_DUALBAND_NOTIFY&4)
			{
				CWRoamDebugLog("(dualband_notify&4) %02x:%02x:%02x:%02x:%02x:%02x",
								pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
								pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5]);	
				for(k=0; k<AC_MAX_WTP_NUM; k++)
				{
					if(gWTPsStatus[k])
						CWIssueDualBandNotify(k, pstat->mac_addr);		
				}
			}
		}

		CWStationUpdateThrshCount(pstat, add_station, current_time);
	}

	/* update info for same wtp and radio, DO check for roam */
	else if(wtp_index==pstat->wtp_index && add_station->radio_id==pstat->radio_id) 
	{
		/* in case SILENT/DOT11V request is ignored or missed */
		if(pstat->roam_status == SILENT){
			pstat->roam_status = SILENT_ING;
		}
		else if(pstat->roam_status == DOT11V){
			pstat->roam_status = DOT11V_ING;
		}
		else if(pstat->roam_status==SILENT_ING || pstat->roam_status==DOT11V_ING){
			pstat->roam_status = NO_ROAM;
		}
			
		pstat->data_rate = add_station->data_rate;			
		pstat->link_time= add_station->link_time;

		pstat->wlan[pstat->radio_id].rssi[wtp_index] = add_station->rssi;	
		pstat->wlan[pstat->radio_id].ch_util[wtp_index] = add_station->ch_util;			

		CWRoamDebugLog("The same add station %02x:%02x:%02x:%02x:%02x:%02x (rssi=%d rate=MCS%d link=%d status=%d best_timestamp=%d currten_time=%d)", 
						pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
						pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5],
						pstat->wlan[pstat->radio_id].rssi[wtp_index],
						pstat->data_rate,pstat->link_time, pstat->roam_status, pstat->best_timestamp, current_time);

		if(pstat->roam_status==NO_ROAM && pstat->wlan[pstat->radio_id].rssi[pstat->wtp_index]<ROAMING_SIGNAL_MAXIMUM)
		{
			best_wtp_index = CWStationFindBestWtp(pstat,STATION_INFO_TYPE_REFERENCE,current_time);
			/* current is the best */
			if(pstat->wtp_index == best_wtp_index){
				CWRoamLog("Current is the best");
				pstat->best_wtp_index = -1;
			}
			/* best ap first detected */
			else if(pstat->best_wtp_index == -1){
				CWRoamLog("best wtp first detected %d", best_wtp_index);
				pstat->best_wtp_index = best_wtp_index;
				pstat->best_timestamp = current_time;
			}
			/* best ap changed, compare */
			else if(pstat->best_wtp_index != best_wtp_index){
				CWRoamLog("best ap changed current best %d(ref:%d) != new best %d(ref:%d)", 
							pstat->best_wtp_index, 
							pstat->wlan[pstat->radio_id].reference[pstat->best_wtp_index], 
							best_wtp_index, 
							pstat->wlan[pstat->radio_id].reference[best_wtp_index]);
				if(pstat->wlan[pstat->radio_id].reference[best_wtp_index] >= pstat->wlan[pstat->radio_id].reference[pstat->best_wtp_index]+ROAMING_REFERENCE_DIFFERENCE/2)
				{
					CWRoamLog("best ap changed %d => %d", pstat->best_wtp_index, best_wtp_index);
					pstat->best_wtp_index = best_wtp_index;
					pstat->best_timestamp = current_time;
				}
			}
			/* best ap over best_duration, safe linktime to roam */
			else if(pstat->best_wtp_index == best_wtp_index
					&& pstat->link_time >= gStationsLinktimeMimimum[pstat->radio_id]
					&& current_time-pstat->best_timestamp >= ROAMING_BEST_DURATION)
			{
				CWRoamLog("!!! Process roam of %02x:%02x:%02x:%02x:%02x:%02x !!!",
							pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
							pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5]);
				CWRoamLog("WTP%d (current): speed=%d rssi=%d ch_util=%d reference=%d",
							wtp_index, gWTPsSpeed[wtp_index],
							pstat->wlan[pstat->radio_id].rssi[wtp_index],
							pstat->wlan[pstat->radio_id].ch_util[wtp_index],
							pstat->wlan[pstat->radio_id].reference[wtp_index]);
				CWRoamLog("WTP%d (best):	speed=%d rssi=%d ch_util=%d reference=%d",
							best_wtp_index, gWTPsSpeed[best_wtp_index],
							pstat->wlan[pstat->radio_id].rssi[best_wtp_index],
							pstat->wlan[pstat->radio_id].ch_util[best_wtp_index],
							pstat->wlan[pstat->radio_id].reference[best_wtp_index]);
				CWIssueUnsilenceNotify(best_wtp_index, pstat->radio_id, pstat->mac_addr);
				for(k=0; k<gMaxWTPs; k++)
				{
					if(k != best_wtp_index) {
						for(j=0; gWTPsStatus[k]&&j<RTK_RADIOS; j++)
							CWIssueSilenceNotify(k, j, pstat->mac_addr);
					}
				}
			
				pstat->best_wtp_index = -1;
				pstat->roam_status = SILENT;
				pstat->roam_wtp_index = best_wtp_index;				
				pstat->roam_timestamp = CWGetTimeInMilliSeconds();
			}
		}

		CWStationUpdateThrshCount(pstat, add_station, current_time);
	}

	/* free for shorter link_time, client left */
	else{
		CWIssueFreeNotify(wtp_index, add_station->radio_id, add_station->mac_addr);
	}
	
	CWThreadMutexUnlock(&gStations.hash[i].lock);
	return CW_TRUE;
}

CWBool CWStationUpdate(int wtp_index, CWProtocolUpdateStationValues *upd_station)
{
	uint32_t i;
	station_t *pstat;
	int k, j, best_wtp_index;
	
	unsigned int current_time = CWGetTimeInSeconds();
	
	i = CW_STATION_HASH_FUNC(upd_station->mac_addr);
	CWThreadMutexLock(&gStations.hash[i].lock);
	pstat = gStations.hash[i].list_head;
	while (pstat)
	{
		if (memcmp(pstat->mac_addr,upd_station->mac_addr,6)==0)
		{
			/* associated: skip CWStationUpdate() and refer to CWStationAdd() */
			if(wtp_index==pstat->wtp_index && pstat->radio_id==upd_station->radio_id)
			{
				CWThreadMutexUnlock(&gStations.hash[i].lock);
				return CW_TRUE;
			}
			pstat->timestamp[wtp_index] = current_time;
			break;		
		}
		else
			pstat = pstat->next;
	}

	/* do update and check for roam */
	if(pstat && pstat->radio_id==upd_station->radio_id)
	{	
		pstat->wlan[pstat->radio_id].rssi[wtp_index] = upd_station->rssi;
		pstat->wlan[pstat->radio_id].ch_util[wtp_index] = upd_station->ch_util; 
		CWRoamDebugLog("\tupdate station %02x:%02x:%02x:%02x:%02x:%02x (rssi=%d)",
						pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
						pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5],
						pstat->wlan[pstat->radio_id].rssi[wtp_index]);

		if(pstat->roam_status==NO_ROAM && pstat->wlan[pstat->radio_id].rssi[pstat->wtp_index]<ROAMING_SIGNAL_MAXIMUM)
		{
			best_wtp_index = CWStationFindBestWtp(pstat,STATION_INFO_TYPE_REFERENCE,current_time);
			/* current is the best*/
			if(best_wtp_index == pstat->wtp_index){
				pstat->best_wtp_index = -1;
			}
			/* best ap first detected */
			else if(pstat->best_wtp_index == -1){
				pstat->best_wtp_index = best_wtp_index;
				pstat->best_timestamp = current_time;
			}
			/* best ap changed, compare */
			else if(pstat->best_wtp_index != best_wtp_index){
				if(pstat->wlan[pstat->radio_id].reference[best_wtp_index] >= pstat->wlan[pstat->radio_id].reference[pstat->best_wtp_index]+ROAMING_REFERENCE_DIFFERENCE/2)
				{
					pstat->best_wtp_index = best_wtp_index;
					pstat->best_timestamp = current_time;
				}
			}
			/* best ap over best_duration, safe linktime to roam */
			else if(pstat->best_wtp_index == best_wtp_index
					&& pstat->link_time >= gStationsLinktimeMimimum[pstat->radio_id]
					&& current_time-pstat->best_timestamp >= ROAMING_BEST_DURATION)
			{
				CWRoamLog("!!! Process roam of %02x:%02x:%02x:%02x:%02x:%02x !!!",
							pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
							pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5]);
				CWRoamLog("WTP%d (current): speed=%d rssi=%d ch_util=%d reference=%d",
							pstat->wtp_index, gWTPsSpeed[pstat->wtp_index],
							pstat->wlan[pstat->radio_id].rssi[pstat->wtp_index],
							pstat->wlan[pstat->radio_id].ch_util[pstat->wtp_index],
							pstat->wlan[pstat->radio_id].reference[pstat->wtp_index]);
				CWRoamLog("WTP%d (best): speed=%d rssi=%d ch_util=%d reference=%d",
							best_wtp_index, gWTPsSpeed[best_wtp_index],
							pstat->wlan[pstat->radio_id].rssi[best_wtp_index],
							pstat->wlan[pstat->radio_id].ch_util[best_wtp_index],
							pstat->wlan[pstat->radio_id].reference[best_wtp_index]);
				CWIssueUnsilenceNotify(best_wtp_index, pstat->radio_id, pstat->mac_addr);
				for(k=0; k<gMaxWTPs; k++)
				{
					if(k != best_wtp_index) {
						for(j=0; gWTPsStatus[k]&&j<RTK_RADIOS; j++)
							CWIssueSilenceNotify(k, j, pstat->mac_addr);
					}
				}
			
				pstat->best_wtp_index = -1;
				pstat->roam_status = SILENT;
				pstat->roam_wtp_index = best_wtp_index;
				pstat->roam_timestamp = CWGetTimeInMilliSeconds();
			}		
		}
	}

	else if(pstat && !pstat->dual_band)
	{
		pstat->dual_band = 1;
		if(BANDSTEER_DUALBAND_NOTIFY&4)
		{
			CWRoamDebugLog("(dualband_notify&4) %02x:%02x:%02x:%02x:%02x:%02x",
							pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
							pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5]);	
			for(k=0; k<AC_MAX_WTP_NUM; k++)
			{
				if(gWTPsStatus[k])
					CWIssueDualBandNotify(k, pstat->mac_addr);		
			}
		}
	}		

	CWThreadMutexUnlock(&gStations.hash[i].lock);
	return CW_TRUE;
}

CWBool CWStationUpdateDot11k(int wlanNum, CWProtocolUpdateStationDot11kValues *upd_station_11k)
{
	uint32_t i;
	station_t *pstat;
	int k,j,wtp_index,best_wtp_index;

	unsigned int current_time = CWGetTimeInSeconds();
	
	i = CW_STATION_HASH_FUNC(upd_station_11k->mac_addr);
	CWThreadMutexLock(&gStations.hash[i].lock);
	pstat = gStations.hash[i].list_head;
	while (pstat)
	{
		if(memcmp(pstat->mac_addr,upd_station_11k->mac_addr,6)==0)
		{
			pstat->dot11k_flag = upd_station_11k->flag;
			break;
		}
		else	
			pstat = pstat->next;
	}

	if(pstat)
	{
		/* update wlan dot11k info */
		for(k=0; k<wlanNum; k++)
		{
			wtp_index = upd_station_11k->Dot11k[k].wtp_index;
			if(wtp_index >= AC_MAX_WTP_NUM)	continue;	/* valid wtp_index */
			pstat->dot11k_timestamp[wtp_index] = current_time;
			
			if(pstat->radio_id == upd_station_11k->radio_id){
				pstat->wlan[pstat->radio_id].dot11k_rcpi[wtp_index] = upd_station_11k->Dot11k[k].rcpi;
				pstat->wlan[pstat->radio_id].dot11k_cu[wtp_index] = upd_station_11k->Dot11k[k].ch_util;
			}
			CWRoamDebugLog("add 11k station %02x:%02x:%02x:%02x:%02x:%02x (wtp=%d rcpi=%d ch_util=%d)",
							pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
							pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5],
							wtp_index,pstat->wlan[pstat->radio_id].dot11k_rcpi[wtp_index],
							pstat->wlan[pstat->radio_id].dot11k_cu[wtp_index]);
		}
		
		/* check for roam */
		if(pstat->roam_status==NO_ROAM && pstat->wlan[pstat->radio_id].rssi[pstat->wtp_index]<ROAMING_SIGNAL_MAXIMUM)
		{
			best_wtp_index = CWStationFindBestWtp(pstat, STATION_INFO_TYPE_DOT11K_REFERENCE,current_time);
			/* current is the best */
			if(best_wtp_index == pstat->wtp_index){
				pstat->dot11k_best_wtp_index = -1;
			}
			/* best ap first detected */
			else if(pstat->dot11k_best_wtp_index == -1){
				pstat->dot11k_best_wtp_index = best_wtp_index;
				pstat->dot11k_best_timestamp = current_time;
			}
			/* best ap changed, compare */
			else if(pstat->dot11k_best_wtp_index != best_wtp_index){
				if(pstat->wlan[pstat->radio_id].reference[best_wtp_index] >= pstat->wlan[pstat->radio_id].reference[pstat->dot11k_best_wtp_index]+ROAMING_REFERENCE_DIFFERENCE/2)
				{
					pstat->dot11k_best_wtp_index = best_wtp_index;
					pstat->dot11k_best_timestamp = current_time;
				}
			}
			/* best ap over best_duration, safe linktime to roam */
			else if(pstat->dot11k_best_wtp_index == best_wtp_index
					&& (pstat->link_time>=gStationsLinktimeMimimum[pstat->radio_id] || pstat->dot11k_flag==11)
					&& current_time-pstat->dot11k_best_timestamp >= ROAMING_BEST_DURATION)
			{
				CWRoamLog("!!! Process roam of %02x:%02x:%02x:%02x:%02x:%02x(11k_flag=%d) !!!",
							pstat->mac_addr[0],pstat->mac_addr[1],pstat->mac_addr[2],
							pstat->mac_addr[3],pstat->mac_addr[4],pstat->mac_addr[5],pstat->dot11k_flag);
				CWRoamLog("WTP%d (current): speed=%d rcpi=%d ch_util=%d reference=%d",
							pstat->wtp_index,gWTPsSpeed[pstat->wtp_index],
							pstat->wlan[pstat->radio_id].dot11k_rcpi[pstat->wtp_index],
							pstat->wlan[pstat->radio_id].dot11k_cu[pstat->wtp_index],
							pstat->wlan[pstat->radio_id].reference[pstat->wtp_index]);
				CWRoamLog("WTP%d (best):	speed=%d rcpi=%d ch_util=%d reference=%d",
							best_wtp_index,gWTPsSpeed[best_wtp_index],
							pstat->wlan[pstat->radio_id].dot11k_rcpi[best_wtp_index],
							pstat->wlan[pstat->radio_id].dot11k_cu[best_wtp_index],
							pstat->wlan[pstat->radio_id].reference[best_wtp_index]);

				switch(pstat->dot11k_flag)
				{
					case 11:
						CWIssueDot11vNotify(pstat->wtp_index, pstat->radio_id, pstat->mac_addr);					
						pstat->dot11k_best_wtp_index = -1;
						pstat->roam_status = DOT11V;
						pstat->roam_wtp_index = best_wtp_index;
						pstat->roam_timestamp = CWGetTimeInMilliSeconds();
						break;
					case 10:
						CWIssueUnsilenceNotify(best_wtp_index, pstat->radio_id, pstat->mac_addr);
						for(k=0; k<gMaxWTPs; k++)
						{
							if(k!= best_wtp_index) {
								for(j=0; gWTPsStatus[k]&&j<RTK_RADIOS; j++)
									CWIssueSilenceNotify(k, j, pstat->mac_addr);
							}
						}

						pstat->dot11k_best_wtp_index = -1;
						pstat->roam_status = SILENT;
						pstat->roam_wtp_index = best_wtp_index;
						pstat->roam_timestamp = CWGetTimeInMilliSeconds();
						break;
					default:
						CWRoamDebugLog("Wrong dot11k_flag: %d (%s)", pstat->dot11k_flag, __FUNCTION__);
						break;
				}
			}			
		}
	}

	CWThreadMutexUnlock(&gStations.hash[i].lock);
	return CW_TRUE;
}
#endif

#ifdef KM_BY_AC
CWBool CWStationUpdateState(const uint8_t *mac, station_state_t state)
{
	uint32_t i;
	station_t *p;
	CWBool ret;
	i = CW_STATION_HASH_FUNC(mac);
	CWThreadMutexLock(&gStations.hash[i].lock);
	p = gStations.hash[i].list_head;
	while (p) {
		if (memcmp(p->mac_addr, mac, 6)==0) {
			break;
		} else {
			p = p->next;
		}
	}
	if (!p) {
		// Not found
		ret = CW_FALSE;		
		CWLog("Fail to Update Station:%02x:%02x:%02x:%02x:%02x:%02x (not exist)", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		} else {
		p->state = state;
		ret = CW_TRUE;		
		CWLog("Update Station:%02x:%02x:%02x:%02x:%02x:%02x (%d)", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], state);
	}
	CWThreadMutexUnlock(&gStations.hash[i].lock);
	return ret;
}
		
int CWGetWTPStationList(int wtp_index, station_t **list_head)
{
	uint32_t i, cnt=0;
	station_t *p, *q;

	*list_head = NULL;
	for (i=0; i<CW_STATION_HASH_MAX; i++)
	{
		CWThreadMutexLock(&gStations.hash[i].lock);
		p = gStations.hash[i].list_head;
		while (p) {
			if (p->wtp_index == wtp_index) {
				q = 
malloc(sizeof(station_t));
				*q = *p;
				q->next = *list_head;
				*list_head = q;
				cnt++;
			}
			p = p->next;
		}
		CWThreadMutexUnlock(&gStations.hash[i].lock);
	}

	return cnt;
}

void CWFreeWTPStationList(station_t *list_head)
{
	station_t *p = list_head, *q;
	while(p){
		q = p->next;
		free(p);
		p = q;
	}
}
#endif


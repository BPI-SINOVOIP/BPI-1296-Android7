#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "wumLib.h"

void printWTPList(struct WTPInfo *wtpList, int nWTPs);
void printVersionHeader();
void printVersionInfo(struct version_info v_info, int wtpId, struct WTPInfo *wtpList);
void printVersionFooter();
	
void do_stations_cmd(int acserver, char *wtpIds, char *wtpNames);
void do_version_cmd(int acserver, char *wtpIds, char *wtpNames);
void do_update_cmd(int acserver, char *wtpIds, char *wtpNames, char *cup_path);
void do_cancel_cmd(int acserver, char *wtpIds, char *wtpNames);
void do_change_firmware(int acserver, char *wtpIds, char *wtpNames, const char *firmware_version);
void do_kick_station(int acserver, char *wtpIds, char *wtpNames, const char *mac_str);
void do_site_survey(int acserver, char *wtpIds, char *wtpNames);
void do_update_power(int acserver, char *wtpIds, char *wtpNames, int radio_id, char *value);	
void do_update_channel(int acserver, char *wtpIds, char *wtpNames, int radio_id, char *value);

int get_cmd_id(char *cmd);
char *WTP_id2name(int id);
int WTP_name2id(char *name);
char *WTP_state2string(CWStateTransition state);
char *transferBssType2String(rtk_bss_type_t bss_type);
char *transferRadioType2String(CWradioType radio_type);

void usage(char *name);

#define ACSERVER_ADDRESS "127.0.0.1"
#define ACSERVER_PORT	1235

/* Commands */

typedef enum {
	NO_CMD, 
	WTPS_CMD, 
	STATIONS_CMD, 
	VERSION_CMD, 
	UPDATE_CUP_CMD, 
	CANCEL_CMD, 
	CHANGE_FIRMWARE_CMD, 
	KICK_STATION_CMD, 
	SITE_SURVEY_CMD,
	UPDATE_POWER_CMD,
	UPDATE_CHANNEL_CMD,

	/*----*/
	CMD_NUM
} cmd_type_t;

typedef struct { 
	cmd_type_t id; 
	const char *name; 
	const char *description;
	int show_flag;
} cmd_t;


cmd_t CMDs[] = {	
	{NO_CMD, 				""      ,	"", 					0},
	{WTPS_CMD,				"wtps" , 	"list of active wtps.", 1},
	{STATIONS_CMD, 			"stations",	"list of active stations (use -w).", 1},
	{VERSION_CMD, 			"version", 	"version of the specified list of wtps (use -w or -n).", 0},
	{UPDATE_CUP_CMD, 		"cup" , 	"sends a cup (specified with -f) to the specified list of wtps (use -w or -n).", 0},
	{CANCEL_CMD, 			"cancel", 	"cancel a pending update on the desired wtps (use -w or -n).", 0},
	{CHANGE_FIRMWARE_CMD,	"firmware", "change firmware (-v version -w wtp_id).", 1},
	{KICK_STATION_CMD, 		"kick_sta", "kick station (-w wtp_id -s mac_addr).", 1},
	{SITE_SURVEY_CMD, 		"ss", 		"site survey -w wtp_id", 1},	
	{UPDATE_POWER_CMD, 		"power", 	"update radio power (-w wtp_id -r radio_id -v power_percentage)", 1},
	{UPDATE_CHANNEL_CMD,	"channel",	"update radio channel (-w wtp_id -r radio_id -v channel)", 1},
};

/* Global WTP List */
struct WTPInfo *wtpList;
int nWTPs;

int main(int argc, char *argv[])
{
	int acserver;
	cmd_type_t cmd_id;
    char *command = NULL, *cup_path = NULL, *value = NULL, *mac_str = NULL;
    char *wtpIds = NULL, *wtpNames = NULL;
    char *acserver_address = ACSERVER_ADDRESS;
	int radio_id;
	int acserver_port = ACSERVER_PORT;;
    int c;
    
    opterr = 0;
	/* Parse options */
    while ((c = getopt (argc, argv, "ha:p:w:c:f:v:n:s:r:")) != -1) {
        switch (c)
        {
		case 'a':
			acserver_address = optarg;
			break;
		case 'p':
			acserver_port = atoi(optarg);
			break;
        case 'w':
            wtpIds = optarg;
            break;
        case 'n':
        	wtpNames = optarg;
        	break;
        case 'c':
            command = optarg;
            break;
        case 'f':
        	cup_path = optarg;
        	break;
		case 'v':
			value = optarg;
			break;
		case 's':
			mac_str = optarg;
			break;
        case 'h':
        	usage(argv[0]);
        	break;
		case 'r':
			radio_id = atoi(optarg);
			break;
        case '?':
            if (optopt == 'w' || optopt == 'c' || optopt == 'f' || optopt == 'n')
           		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
            	fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
           		fprintf (stderr,
                    "Unknown option character `\\x%x'.\n",
                    optopt);
            exit(EXIT_FAILURE);
        default:
    		usage(argv[0]);
            abort();

		}
   	}
    /* Check arguments */ 
	if (command == NULL) {
		fprintf(stderr, "No command specified!\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if ((cmd_id = get_cmd_id(command)) == NO_CMD) {
		fprintf(stderr, "Wrong command specified!\n");
	}

	/* Connect to server and get WTPs list */
	acserver = ACServerConnect(acserver_address, acserver_port);
	wtpList = ACServerWTPList(acserver, &nWTPs);
	
	/* Execute command */
	switch(cmd_id) {
		case WTPS_CMD:
			printWTPList(wtpList, nWTPs);
			break;
		case STATIONS_CMD:
			do_stations_cmd(acserver, wtpIds, wtpNames);
			break;
		case VERSION_CMD:
			do_version_cmd(acserver, wtpIds, wtpNames);
			break;
		case UPDATE_CUP_CMD:
			do_update_cmd(acserver, wtpIds, wtpNames, cup_path);
			break;
		case CANCEL_CMD:
			do_cancel_cmd(acserver, wtpIds, wtpNames);
		case CHANGE_FIRMWARE_CMD:
			do_change_firmware(acserver, wtpIds, wtpNames, value);
			break;
		case KICK_STATION_CMD:
			do_kick_station(acserver, wtpIds, wtpNames, mac_str);
			break;
		case SITE_SURVEY_CMD:
			do_site_survey(acserver, wtpIds, wtpNames);
			break;
		case UPDATE_POWER_CMD:
			do_update_power(acserver, wtpIds, wtpNames, radio_id, value);
			break;
		case UPDATE_CHANNEL_CMD:			
			do_update_channel(acserver, wtpIds, wtpNames, radio_id, value);
			break;
	}
	
	freeWTPList(wtpList, nWTPs);
	ACServerDisconnect(acserver);

	exit(EXIT_SUCCESS);
}

int sanitize_wtp_list(int *work_list, int n)
{
	int i, j, z, new_size = n;

	/* Delete unknown wtp ids */
	for(i = 0; i < new_size; i++) {
		if (WTP_id2name(work_list[i]) == NULL) {
			for (j = i; j < new_size - 1; j++) {
				work_list[j] = work_list[j+1];
			}
			i--;
			new_size--;
		}
	}
	
	/* Delete duplicates */
	for(i = 0; i < new_size; i++) {
		for (j = i + 1; j < new_size; j++) {
			if (work_list[i] == work_list[j]) {
				for (z = j; z < new_size - 1; z++) {
		        	work_list[z] = work_list[z+1];
		        }
				j--;
				new_size--;
			}
		} 
	}

	return new_size;
}

int *all_WTPs()
{
	int i, ret[AC_MAX_WTP_NUM];
	for(i=0; i<nWTPs; i++){
		ret[i] = wtpList[i].wtpId;
	}
	
	return ret;
}

static int count_tokens(char *str1, char *str2)
{
	int n = 1;
	char *ptr;

	if (str1 != NULL)
		ptr = str1;
	else if (str2 != NULL) 
		ptr = str2;
	else
		return 0;
	
	while (*ptr != '\0') {
		if (*ptr == ',' && *(ptr + 1) != ',' && *(ptr + 1) != '\0') 
			n++;
		ptr++;
	}
	return n;
}

int *get_id_list(char *wtpIds, char *wtpNames, int *n)
{
	char *token;
	int *ret = NULL;
	int i;
	
	*n = count_tokens(wtpIds, wtpNames); 
	
	if (*n <= 0) return NULL;
	
	/* allocate memory */
	ret = malloc(*n*sizeof(int));
	if (ret == NULL) {
		perror("malloc error!");
		return NULL;
	}
	
	if (wtpIds != NULL) {
		/* read ids */
		token = (char*)strtok(wtpIds, ",");
		ret[0] = atoi(token);

		if (ret[0] == -1) 
			return all_WTPs();
		
		for (i = 1; i < *n; i++)
			ret[i] = atoi( (const char*)strtok(NULL, ",") );
		
	} else {	
		/* read names and convert into ids */
		for (i = 0; i < *n; i++) {
			int id;

			if (i == 0) {
				token = (char*)strtok(wtpNames, ",");
				if (strcmp(token, "all") == 0)
					return all_WTPs();
				
			} else {
				token = (char*)strtok(NULL, ",");
			}
			
			if ((id = WTP_name2id(token)) == -1) {
				fprintf(stderr, "%s: specified WTP does not exits\n", token);
			}

			ret[i] = id;
		}
	} 
	
	/* remove duplicated and unknown WTP ids */
	*n = sanitize_wtp_list(ret, *n);

	return ret;
}

void do_stations_cmd(int acserver, char *wtpIds, char *wtpNames)
{
	int *wtps, n, i, j, nStations;
	CWRtkStationValues *stations;

	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	
	if (wtps == NULL) {
		fprintf(stderr, "Either a list of wtp ids or wtp names must be specified!\n");
		return;
	}

	for (i=0;i<n;i++){
		stations = ACServerStationsList(acserver, wtps[i], &nStations);
		printf("[WTP %d - # of stations = %d]\n", wtps[i], nStations);
		for (j=0;j<nStations; j++) {
			printf("%02x:%02x:%02x:%02x:%02x:%02x\tRadio%d,Wlan%d\n", 
				stations[j].mac_addr[0], stations[j].mac_addr[1], stations[j].mac_addr[2], 
				stations[j].mac_addr[3], stations[j].mac_addr[4], stations[j].mac_addr[5], 
				stations[j].radio_id, stations[j].wlan_id);
		}
		if (stations) {
			free(stations);
		}
	}
	free(wtps);
}


void do_version_cmd(int acserver, char *wtpIds, char *wtpNames)
{
	int *wtps, n, i;
	struct version_info v_info;

	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	
	if (wtps == NULL) {
		fprintf(stderr, "Either a list of wtp ids or wtp names must be specified!\n");
		return;
	}

	printVersionHeader();	
	for (i = 0; i < n; i++) {
		WUMGetWTPVersion(acserver, wtps[i], &v_info);
		printVersionInfo(v_info, wtps[i], wtpList);	
	}
	printVersionFooter();
}

void do_update_cmd(int acserver, char *wtpIds, char *wtpNames, char *cup_path)
{
	int *wtps, n, i, ret;
	struct version_info update_v;
	void *cup;

	if (cup_path == NULL) {
		fprintf(stderr, "In order to execute an update, an update package must be specified! (-f pathname)\n");
		return;
	}

	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	if (wtps == NULL) {
		fprintf(stderr, "Either a list of wtp ids or wtp names must be specified!\n");
		return;
	}

	int fd = open(cup_path, O_RDONLY);
	if (fd < 0) {
		perror("open error");
		return;
	}
	
	if (WUMReadCupVersion(cup_path, &update_v)) {
		return;
	}
	
	cup = mmap(NULL, update_v.size, PROT_READ, MAP_SHARED , fd, 0);
	if (cup == NULL) {
		perror("mmap error");
		return;
	}

	printf("*--------*--------------------------------*------------*\n");
	printf("| %-6s | %-30s | %-10s |\n", "WTP Id", "WTp Name", "Result");
	printf("*--------*--------------------------------*------------*\n");
	for (i = 0; i < n; i++) {
		ret = WUMUpdate(acserver, i, cup, update_v);
		printf("| %-6d | %-30s | %-10s |\n", wtpList[i].wtpId, wtpList[i].name, (ret == 0) ? "SUCCESS" : "FAILURE");
	}
	printf("*--------*--------------------------------*------------*\n");

	munmap(cup, update_v.size);
	close(fd);
}

void do_cancel_cmd(int acserver, char *wtpIds, char *wtpNames)
{
	int *wtps, n, i;

	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	
	if (wtps == NULL) {
		fprintf(stderr, "Either a list of wtp ids or wtp names must be specified!\n");
		return;
	}

	for (i = 0; i < n; i++) {
		if (WUMSendCancelRequest(acserver, wtps[i])) {
			fprintf(stderr, "Error while handling cancel request to WTP %d.\n", wtps[i]);
		} else {
			printf("Cancel request sent for WTP %d\n", wtps[i]);
		}
	}
}

static int isHaxChar(char c, int *n)
{
	if (c>='0' && c<='9') {
		*n = c-'0';
		return 1;
	}
	if (c>='a' && c<='f') {		
		*n = c-'a'+10;
		return 1;
	}
	if (c>='A' && c<='F') {
		*n = c-'A'+10;
		return 1;
	}
	return 0;
}

static int get_mac_addr(const char *mac_str, unsigned char *mac)
{
	int n, m, i, j;
	for (i=0, j=0; i<6; i++) {
		if (!isHaxChar(mac_str[j++], &n)) return 0;
		if (!isHaxChar(mac_str[j++], &m)) return 0;
		mac[i] = n*16+m;
		if (i<5 && mac_str[j++] != ':') return 0;
		if (i==5 && mac_str[j++] != '\0') return 0;
	}
//	printf("%s => %02x:%02x:%02x:%02x:%02x:%02x\n", mac_str, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return 1;

}

void do_kick_station(int acserver, char *wtpIds, char *wtpNames, const char *mac_str)
{
	int *wtps, n, i;
	unsigned char mac[6];

	/* mac */
	if (mac_str == NULL) {
		fprintf(stderr, "Need station mac address speicied! (-s xx:xx:xx:xx:xx:xx)\n");
		return;
	}

	if (!get_mac_addr(mac_str, mac)) {
		fprintf(stderr, "mac address format error! (-s xx:xx:xx:xx:xx:xx)\n");
		return;
	}

	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	if (wtps == NULL) {
		fprintf(stderr, "Either a list of wtp ids or wtp names must be specified!\n");
		return;
	}

	/* action */
	for (i = 0; i < n; i++) {
		if (WUMKickStation(acserver, wtps[i], mac)) {
			fprintf(stderr, "Error while kicking station: %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	}

	free(wtps);
}

/* wtpIds and wtpNames will be changed in the step of parsing */
void do_change_firmware(int acserver, char *wtpIds, char *wtpNames, const char *firmware_version)
{
	int *wtps, n, i;
	const char* firmware_file;

	if (firmware_version == NULL) {
		fprintf(stderr, "Need firmware version speicied! (-v version)\n");
		return;
	}

	firmware_file = ACGetImageFile(firmware_version);
	if (access(firmware_file, R_OK) != 0) {
		fprintf(stderr, "%s is not a readible file\n", firmware_file);		
		return;
	}


	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	if (wtps == NULL) {
		fprintf(stderr, "Either a list of wtp ids or wtp names must be specified!\n");
		return;
	}

	for (i = 0; i < n; i++) {
		printf("starting to upgrade firmware %s for wtp %d\n", firmware_version, wtps[i]);
		if (WUMChangeImage(acserver, wtps[i], firmware_version)) {
			fprintf(stderr, "Error while sending request to change firmware for WTP %d.\n", wtps[i]);
		} else {
			printf("Sent request to change firmware for WTP %d\n", wtps[i]);
		}
	}
	free(wtps);

/*
	printVersionHeader();	
	for (i = 0; i < n; i++) {
		WUMGetWTPVersion(acserver, wtps[i], &v_info);
		printVersionInfo(v_info, wtps[i], wtpList);	
}
	printVersionFooter();
*/

}
void do_site_survey(int acserver, char *wtpIds, char *wtpNames)
{
	int *wtps, n, i, j, k, nSite;
	CWRtkSiteSurveyValues *siteSurvey;
	char msgBuf[1024];

	
	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	if (wtps == NULL) {
		fprintf(stderr, "Either a list of wtp ids or wtp names must be specified!\n");
		return;
	}

	for (i=0;i<n;i++){
		siteSurvey = WUMSiteSurvey(acserver, wtps[i], &nSite, msgBuf);
		printf("======== WTP Index %d ========\n", wtps[i]);		
		printf("%s", msgBuf);
		for (j=0;j<nSite; j++) {			
			printf("[Radio %u (bss number = %u)]\n", siteSurvey[j].radioID, siteSurvey[j].numBss );
			for (k=0; k<siteSurvey[j].numBss; k++) {
				printf("channel: %u, strength: %u, bssid: %02x%02x%02x%02x%02x%02x, ssid:%s, %s, %s\n",
					siteSurvey[j].bssInfo[k].channel, 
					siteSurvey[j].bssInfo[k].signal_strength, 
					siteSurvey[j].bssInfo[k].bssid[0], 
					siteSurvey[j].bssInfo[k].bssid[1],
					siteSurvey[j].bssInfo[k].bssid[2],
					siteSurvey[j].bssInfo[k].bssid[3], 
					siteSurvey[j].bssInfo[k].bssid[4], 
					siteSurvey[j].bssInfo[k].bssid[5], 
					siteSurvey[j].bssInfo[k].ssid,					
					transferBssType2String(siteSurvey[j].bssInfo[k].bss_type),
					transferRadioType2String(siteSurvey[j].bssInfo[k].radio_type));
			}
		}
		if (siteSurvey) {
			free(siteSurvey);
		}
	}

	free(wtps);

}
void do_update_power(int acserver, char *wtpIds, char *wtpNames, int radio_id, char *power_percentage)	
{
	int *wtps, n;
	int power;

	if (!power_percentage) {
		fprintf(stderr, "power percentage (15, 35, 50, 70, 100) must be specified!\n");
		return;
	}
	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	if (wtps == NULL || n!=1) {
		fprintf(stderr, "Either a wtp index or a wtp name must be specified!\n");
		return;
	}

	power = atoi(power_percentage);


	if (radio_id >= 2 || radio_id < 0) {
		fprintf(stderr, "Radio ID error: 0<= radio_id <2\n");
	}

	if (power < 15 || power > 100) {
		fprintf(stderr, "Power percentage error: power percentage suggestion = 15, 35, 50, 70 or 100\n");
	}

	if (wumUpdatePower(acserver, wtps[0], radio_id, power)== 0) {
		printf("power updated\n");
	} else {
		printf("power update fail\n");
	}

	free(wtps);

}
void do_update_channel(int acserver, char *wtpIds, char *wtpNames, int radio_id, char *channel_str)	
{
	int *wtps, n;
	int channel = atoi(channel_str);
	
	/* WTP work list */
	wtps = get_id_list(wtpIds, wtpNames, &n);
	if (wtps == NULL || n!=1) {
		fprintf(stderr, "Either a wtp index or a wtp name must be specified!\n");
		return;
	}

	if (radio_id >= 2 || radio_id < 0) {
		fprintf(stderr, "Radio ID error: 0<= radio_id <2\n");
	}

	// TODO: check channel

	if (wumUpdateChannel(acserver, wtps[0], radio_id, channel)== 0) {
		printf("channel updated\n");
	} else {
		printf("channel update fail\n");
	}

	free(wtps);

}

char *transferRadioType2String(CWradioType radio_type)
{
	static char str[16];
	str[0] = '\0';
	
	if (radio_type & CW_802_DOT_11a) strcat(str, "A ");
	if (radio_type & CW_802_DOT_11b) strcat(str, "B ");
	if (radio_type & CW_802_DOT_11g) strcat(str, "G ");
	if (radio_type & CW_802_DOT_11n) strcat(str, "N ");
	if (radio_type & CW_802_DOT_11ac) strcat(str, "AC");
	return str;
}

char *transferBssType2String(rtk_bss_type_t bss_type)
{
	static char str[32];
	str[0] = '\0';
	
	if (bss_type & CW_BSS_TYPE_AP) strcat(str, "AP ");
	if (bss_type & CW_BSS_TYPE_ADHOC) strcat(str, "ADHOC ");
	return str;
}


int WTP_name2id(char *name)
{
	int i;

	for (i = 0; i < nWTPs; i++) {
		if (strcmp(name, wtpList[i].name) == 0) {
			/* found WTP! */
			return wtpList[i].wtpId;
		}
	}

	return -1;
}

char *WTP_id2name(int id)
{
	int i;

	for(i = 0; i < nWTPs; i++) {
		if (wtpList[i].wtpId == id) return wtpList[i].name;
	}

	return NULL;
}

char *WTP_state2string(CWStateTransition state)
{
	switch(state)
	{
	case CW_ENTER_SULKING:
		return "SULKING";
		break;
	case CW_ENTER_DISCOVERY:
		return "DISCOVERY";
		break;
	case CW_ENTER_JOIN:
		return "JOIN";
		break;
	case CW_ENTER_CONFIGURE:
		return "CONFIGURE";
		break;
	case CW_ENTER_DATA_CHECK:
		return "DATA_CHECK";
		break;
	case CW_ENTER_RUN:
		return "RUN";
		break;
	case CW_ENTER_RESET:
		return "RESET";
		break;
	case CW_QUIT:
		return "QUIT";
		break;
	default:
		return "UNKOWN";
		break;
	}

}

int get_cmd_id(char *cmd)
{
	int i;
	for (i = 0; i < CMD_NUM; i++) {
		if (strcmp(CMDs[i].name, cmd) == 0)
			break;
	}
	
	return CMDs[i].id;
}

void printWTPList(struct WTPInfo *wtpList, int nWTPs)
{
	int i;
	printf("*-------*--------------*--------------------------*\n");
	printf("| %5s | %-12s | %24s |\n", "Index", "WTPName", "Last Echo");
	printf("*-------*--------------*--------------------------*\n");

	if (wtpList != NULL) {
		for (i = 0; i < nWTPs; i++) {
			char tm[32] = "";
			if (wtpList[i].lastEchoRequestTime != 0) {
				sprintf(tm, "%s", ctime(&wtpList[i].lastEchoRequestTime));
				tm[strlen(tm)-1] = '\0';
			} else {
				tm[0] = '\0';
			}
			printf("| %5d | %-12s | %24s |\n", 
				wtpList[i].wtpId, wtpList[i].name, tm);
		}
	}

	printf("*-------*--------------*--------------------------*\n");
}

void printVersionHeader()
{
	printf("*-------*--------------------------------*-----------------*\n");
	printf("| %5s | %-30s | %-15s |\n", "Index", "WTPName", "Version");
	printf("*-------*--------------------------------*-----------------*\n");
}

void printVersionInfo(struct version_info v_info, int wtpId, struct WTPInfo *wtpList)
{
	char buf[15];
	snprintf(buf, 15, "%d.%d.%d", v_info.major, v_info.minor, v_info.revision);
	printf("| %5d | %-30s | %-15s |\n", wtpList[wtpId].wtpId, wtpList[wtpId].name, buf);
}

void printVersionFooter()
{
	printf("*-------*--------------------------------*-----------------*\n");
}

void usage(char *name)
{
	int i;
	cmd_t *cmd;
	
	printf("%s -c command [-w id1,...] [-n name1,...] [-f cup_file] [-a address] [-p port]\n", name);
	printf("\nAvailable commands:\n");

	for (i=0; i<CMD_NUM; i++) {
		cmd = &CMDs[i];
		if (cmd->show_flag)
			printf("%s\t: %s\n", cmd->name, cmd->description);
	}
}

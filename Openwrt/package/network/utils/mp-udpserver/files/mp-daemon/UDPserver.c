//=============================================================================
// Copyright (c) 2006 Realtek Semiconductor Corporation.	All Rights Reserved.
//
//	Title:
//		UDPserver.c
//	Desc:
//		UDP server : accepts MP commands from the client
//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/wireless.h>
#include <fcntl.h>
#include <unistd.h>
#include "rtk_arch.h"

#ifndef WIN32
#define __PACK__			__attribute__ ((packed))
#else
#define __PACK__
#endif


#define MYPORT 9034                    // the port users will be connecting to
#define BUFLEN 1024                      // length of the buffer
#define MP_TX_PACKET 0x8B71
#define MP_BRX_PACKET 0x8B73
#define MP_QUERY_STATS 	0x8B6D
#define RTL8190_IOCTL_WRITE_REG				0x89f3
#define RTL8190_IOCTL_READ_REG				0x89f4
#define MP_CONTIOUS_TX	0x8B66
#define MP_TXPWR_TRACK	0x8B6E
#define MP_QUERY_TSSI	0x8B6F
#define MP_QUERY_THER 0x8B77

#define FLASH_DEVICE_NAME		("/dev/mtd")
#define FLASH_DEVICE_NAME1		("/dev/mtdblock1")
#define HW_SETTING_HEADER_TAG		((char *)"hs")
#define HW_SETTING_OFFSET		0x6000
#define DEFAULT_SETTING_OFFSET		0x8000
#define CURRENT_SETTING_OFFSET		0xc000

#if 1
#define TAG_LEN					2
#define SIGNATURE_LEN			4
#define HW_SETTING_VER			3	// hw setting version
/* Config file header */
typedef struct param_header {
	unsigned char signature[SIGNATURE_LEN];  // Tag + version
	unsigned short len __PACK__;
} __PACK__ PARAM_HEADER_T, *PARAM_HEADER_Tp;
PARAM_HEADER_T hsHeader;
#endif
#if 0
/* Do checksum and verification for configuration data */
#ifndef WIN32
static inline unsigned char CHECKSUM(unsigned char *data, int len)
#else
__inline unsigned char CHECKSUM(unsigned char *data, int len)
#endif
{
	int i;
	unsigned char sum=0;

	for (i=0; i<len; i++)
		sum += data[i];

	sum = ~sum + 1;
	return sum;
}
#ifndef WIN32
static inline int CHECKSUM_OK(unsigned char *data, int len)
#else
__inline int CHECKSUM_OK(unsigned char *data, int len)
#endif
{
	int i;
	unsigned char sum=0;

	for (i=0; i<len; i++)
		sum += data[i];

	if (sum == 0)
		return 1;
	else
		return 0;
}
/////////////////////////////////////////////////////////////////////////////////
static int flash_read(char *buf, int offset, int len)
{
	int fh;
	int ok=1;

	fh = open(FLASH_DEVICE_NAME, O_RDWR);
	if ( fh == -1 )
		return 0;

	lseek(fh, offset, SEEK_SET);

	if ( read(fh, buf, len) != len)
		ok = 0;

	close(fh);

	return ok;
}


////////////////////////////////////////////////////////////////////////////////
static int flash_write(char *buf, int offset, int len)
{
	int fh;
	int ok=1;

	fh = open(FLASH_DEVICE_NAME, O_RDWR);

	if ( fh == -1 )
		return 0;

	lseek(fh, offset, SEEK_SET);

	if ( write(fh, buf, len) != len)
		ok = 0;

	close(fh);
	sync();

	return ok;
}

int ReadSinguture(void)
{
	int ver;
	char *buff;
	// Read hw setting
	if ( flash_read((char *)&hsHeader, HW_SETTING_OFFSET, sizeof(hsHeader))==0 ) {
		printf("Read hw setting header failed!\n");
		return NULL;
	}

	if ( sscanf(&hsHeader.signature[TAG_LEN], "%02d", &ver) != 1)
		ver = -1;

	if ( memcmp(hsHeader.signature, HW_SETTING_HEADER_TAG, TAG_LEN) || // invalid signatur
		(ver != HW_SETTING_VER)  ) { // length is less than current
		printf("Invalid hw setting signature or version number [sig=%c%c, ver=%d, len=%d]!\n", hsHeader.signature[0],
			hsHeader.signature[1], ver, hsHeader.len);
		return NULL;
	}
	//printf("hw setting signature or version number [sig=%c%c, ver=%d, len=%d]!\n", hsHeader.signature[0],	hsHeader.signature[1], ver, hsHeader.len);
	buff = calloc(1, hsHeader.len);
	if ( buff == 0 ) {
		printf("Allocate buffer failed!\n");
		return NULL;
	}
	if ( flash_read(buff, HW_SETTING_OFFSET+sizeof(hsHeader), hsHeader.len)==0 ) {
		printf("Read hw setting failed!\n");
		free(buff);
		return NULL;
	}
	if ( !CHECKSUM_OK(buff, hsHeader.len) ) {
		printf("Invalid checksum of hw setting!\n");
		free(buff);
		return NULL;
	}
	//printf("CS=%x\n",buff[hsHeader.len-1]);
}
#endif
/*
 * Wrapper to extract some Wireless Parameter out of the driver
 */
static inline int iw_get_ext(int skfd,    /* Socket to the kernel */
           			char *ifname,        	/* Device name */
           			int request,        		/* WE ID */
           			struct iwreq *pwrq)    /* Fixed part of the request */
{
  	strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);	/* Set device name */
  	return(ioctl(skfd, request, pwrq));			/* Do the request */
}

static char *get_str_token(char *data, char *token, char split)
{
        char *ptr=data;
        int len=0, idx=0;

        while (*ptr && *ptr != '\n' ) {
                if (*ptr == split) {
                        if (len <= 1)
                           return NULL;
                        memcpy(token, data, len);
                        /* delete ending space */
                        for (idx=len-1; idx>=0; idx--) {
                                if (token[idx] !=  ' ')
                                        break;
                        }
                        token[idx+1] = '\0';

                        return ptr+1;
                }
                len++;
                ptr++;
        }
        return NULL;
}

static int get_str_value(char *data, char *value)
{
        char *ptr=data;
        int len=0, idx, i;

        while (*ptr && *ptr != '\n' && *ptr != '\r') {
                len++;
                ptr++;
        }

        /* delete leading space */
        idx = 0;
        while (len-idx > 0) {
                if (data[idx] != ' ')
                        break;
                idx++;
        }
        len -= idx;

        /* delete bracing '"' */
        if (data[idx] == '"') {
                for (i=idx+len-1; i>idx; i--) {
                        if (data[i] == '"') {
                                idx++;
                                len = i - idx;
                        }
                        break;
                }
        }

        if (len > 0) {
                memcpy(value, &data[idx], len);
                value[len] = '\0';
        }
        return len;
}

char* deblank(char* input)
{
    char *output = input;
    int i, j;

    for (i=0, j=0; i < strlen(input); i++, j++)
    {
        if (input[i] != ' ')
            output[j] = input[i];
        else
            j--;
    }

    output[j]='\0';

    return output;
}

int MP_get_ext(char *ifname, char *buf, unsigned int ext_num)
{
    	int skfd;
    	struct iwreq wrq;

    	skfd = socket(AF_INET, SOCK_DGRAM, 0);
    	wrq.u.data.pointer = (caddr_t)buf;
    	wrq.u.data.length = strlen(buf);

    	if (iw_get_ext(skfd, ifname, ext_num, &wrq) < 0) {
    		printf("MP_get_ext failed\n");
		    return -1;
    	}
	
    	close(skfd);
    	return 0;
}

enum content_type {
	  EFUSE_TYPE,
	  FLASH_TYPE,
	  MIB_TYPE,
	  NONE_TYPE
}; 

typedef struct map_def_s
{
    char name[30];
    char map_name[30];
    char type;
} map_def_t;

static const map_def_t cmd_mib_defs[] =
{
      {"11N_XCAP", "xcap", MIB_TYPE},
      {"11N_THER", "ther", MIB_TYPE},
      {"TX_POWER_CCK_A", "pwrlevelCCK_A",MIB_TYPE},
      {"TX_POWER_CCK_B", "pwrlevelCCK_B", MIB_TYPE},
      {"TX_POWER_HT40_1S_A", "pwrlevelHT40_1S_A", MIB_TYPE},
      {"TX_POWER_HT40_1S_B", "pwrlevelHT40_1S_B", MIB_TYPE},
      {"TX_POWER_DIFF_HT40_2S", "pwrdiffHT40_2S", MIB_TYPE},
      {"TX_POWER_DIFF_HT20", "pwrdiffHT20", MIB_TYPE},
      {"TX_POWER_DIFF_OFDM", "pwrdiffOFDM", MIB_TYPE},
      {"WLAN_ADDR", "hwaddr", MIB_TYPE},
      {"NIC1_ADDR", "hwaddr", MIB_TYPE},
      {"11N_TRSWPAE_C9", "trswitch", MIB_TYPE},
      {"11N_TRSWPAE_CC", "trswitch", MIB_TYPE},
      {"REG_DOMAIN", "dot11RegDomain", MIB_TYPE},
};

static const map_def_t cmd_efuse_defs[] =
{
      {"11N_XCAP", "HW_11N_XCAP", EFUSE_TYPE},
      {"11N_THER", "HW_11N_THER", EFUSE_TYPE},
      {"TX_POWER_CCK_A", "HW_TX_POWER_CCK_A", EFUSE_TYPE},
      {"TX_POWER_CCK_B", "HW_TX_POWER_CCK_B", EFUSE_TYPE},
      {"TX_POWER_HT40_1S_A", "HW_TX_POWER_HT40_1S_A", EFUSE_TYPE},
      {"TX_POWER_HT40_1S_B", "HW_TX_POWER_HT40_1S_B", EFUSE_TYPE},
      {"TX_POWER_DIFF_HT40_2S", "pwrdiffHT40_2S", EFUSE_TYPE},
      {"TX_POWER_DIFF_HT20", "HW_TX_POWER_DIFF_HT20_A", EFUSE_TYPE},
      {"TX_POWER_DIFF_OFDM", "HW_TX_POWER_DIFF_HT20_OFDM_A", EFUSE_TYPE},
      {"WLAN_ADDR", "hwaddr", FLASH_TYPE},
      {"NIC1_ADDR", "hwaddr", FLASH_TYPE},
      {"11N_TRSWPAE_C9", "trswitch", FLASH_TYPE},
      {"11N_TRSWPAE_CC", "trswitch", FLASH_TYPE},
      {"REG_DOMAIN", "dot11RegDomain", FLASH_TYPE},
};

static void get_drv_mib_content(char *path, char *mib, char *mib_value)
{
        FILE *fp;
        char line[400], token[40], value[300], *ptr;

        fp = fopen(path, "r");
        if (fp == NULL) {
                printf("read config file [%s] failed!\n", path);
                return;
        }

        while ( fgets(line, 200, fp) ) {
                ptr = get_str_token(line, token, ':');
                if (ptr == NULL)
                    continue;

                if (get_str_value(ptr, value) == 0) {
                    continue;
                } else {
                    deblank(token);

                    if ( strcmp(mib, token) == 0 )
                    {
                         printf("tok:[%s] mib:[%s] val:[%s]\n", token, mib, value);
                         strcpy(mib_value, value);
                         goto out;
                    }
               }
       }
out:
        fclose(fp);
}

static int convert_str_map(char *str, char *id, map_def_t cmd_map_ptr[], int sz)
{
        int i; //, sz = sizeof(cmd_map_ptr) / sizeof(struct map_def_s);
                        
        for (i = 0; i < sz; i++) {
             if( strstr(str, cmd_map_ptr[i].name) != NULL ) {
        	        // printf("convert [%s] to [%s]\n", str, cmd_map_ptr[i].name);
                  strcpy(id, cmd_map_ptr[i].map_name);
                  return cmd_map_ptr[i].type;
             }
        }

        strcpy(id, "unsupported");
        return NONE_TYPE;
}

static void read_mib_content_from_drv(char *str, char *mib_value)
{
        char file_name[40];
        char full_path[100];
        char mib[64];

        char wlan_idx=0;

        sscanf(str, "HW_WLAN%d", &wlan_idx);

        printf("get wlan idx:%d str:%s\n", wlan_idx, str);

        sprintf(full_path, "%s%d/%s", "/proc/wlan", wlan_idx, "mib_all");

        convert_str_map( str, mib, cmd_mib_defs, sizeof(cmd_mib_defs) / sizeof(struct map_def_s));

        get_drv_mib_content(full_path, mib, mib_value);
}

static void write_mib_content_to_drv(char *str, char *run_cmd)
{
       char efuse[64];
       char token[40], value[300], *ptr;                	
       char wlan_idx=0;
       char type;

       sscanf(str, "HW_WLAN%d", &wlan_idx);

       // printf("get wlan idx:%d str:%s\n", wlan_idx, str);

       ptr = get_str_token(str, token, ' ');
       if (ptr == NULL)
            return;

       if (get_str_value(ptr, value) == 0) 
            return;
              
       type = convert_str_map( (token + strlen("HW_WLAN0_")), efuse, cmd_efuse_defs, sizeof(cmd_efuse_defs) / sizeof(struct map_def_s));

       deblank(value);
       
       // printf("get flash:[%s] val:[%s] mib:[%s]\n", token, value, efuse);

       if (FLASH_TYPE == type) {
             sprintf(run_cmd, "echo flash wlan%d write %s=%s", wlan_idx, efuse, value);
       } else if (EFUSE_TYPE == type) {
             sprintf(run_cmd, "echo iwpriv wlan%d efuse_set %s=%s", wlan_idx, efuse, value);
       }
              
       return;	   
       // get_drv_mib_content(full_path, mib, mib_value);
}

int main(int argc, char *argv[]) {
	int sockfd;                     				// socket descriptors
	struct sockaddr_in my_addr;     		// my address information
	struct sockaddr_in their_addr;  			// connector¡¦s address information
	socklen_t addr_len;
	int numbytes;
	FILE *fp;
	char buf[BUFLEN], buf_tmp[BUFLEN],*pbuf,
	pre_result[BUFLEN];				// buffer that stores message
	static char cmdWrap[500];
	static int rwHW=0;
    static int is_run=1;
  
	if( argc==2 && (!strcmp(argv[1], "version")) ) 
	{
		printf("version : %s\n",DAEMON_VERSION);
		return 0;
	}

	// create a socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;         		// host byte order
	my_addr.sin_port = htons(MYPORT);     	// short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; 	// automatically fill with my IP
	memset(&(my_addr.sin_zero), '\0', 8); 	// zero the rest of the struct

	// bind the socket with the address
	if (bind(sockfd, (struct sockaddr *)&my_addr,
		sizeof(struct sockaddr)) == -1) {
		perror("bind");
		close(sockfd);
		exit(1);
	}

	addr_len = sizeof(struct sockaddr);

	printf("MP AUTOMATION daemon (ver 1.2)\n");
	//Self reading flash!!!
	#if 	0
	if(!ReadSinguture())
	{
		printf("HW Settting Error!!\n");
	}
	#endif	
	// main loop : wait for the client
	while (1) {
		//receive the command from the client
		memset(buf, 0, BUFLEN);
		memset(cmdWrap, 0, 500);
		rwHW = 0;
		is_run = 1;
		
		if ((numbytes = recvfrom(sockfd, buf, BUFLEN, 0,
			(struct sockaddr *)&their_addr, &addr_len)) == -1) {
			fprintf(stderr,"Receive failed!!!\n");
			close(sockfd);
			exit(1);
		}
		
		printf("received command 1 (%s) from IP:%s\n", buf, inet_ntoa(their_addr.sin_addr));

		if(!memcmp(buf, "orf", 3)){
                        strcat(buf, " > " WIFI_MP_TMP_FILE);
                        system(buf);
		}
		if(!memcmp(buf, "irf", 3)){
                        strcat(buf, " > " WIFI_MP_TMP_FILE);
                        system(buf);
		}	
		if (!memcmp(buf, "ther", 4)) {
                        strcpy(buf, pre_result);
		}
		if (!memcmp(buf, "tssi", 4)) {
			strcpy(buf, pre_result);
		}
		if (!memcmp(buf, "query", 5)) {

			strcpy(buf, pre_result);
		}
		if(!memcmp(buf, "cat", 3)){
			
			if(NULL != (pbuf=strstr(buf, ".//proc//")))
			{
				pbuf[0]=' ';
			}

			strcat(buf, " > " WIFI_MP_TMP_FILE);
			system(buf);		
		}
	#if 1	//wlan0
		if (!memcmp(buf, "iwpriv wlan0 mp_tssi", 20)) {
			strcpy(buf, pre_result);
			MP_get_ext("wlan0", buf_tmp, MP_QUERY_TSSI);
			strcpy(buf, buf_tmp);
			printf("buf= %s\n",buf);
			usleep(1000);
		}
		else if (!memcmp(buf, "iwpriv wlan0 mp_ther", 20)) {
			strcpy(buf, pre_result);
			MP_get_ext("wlan0", buf_tmp, MP_QUERY_THER);
			strcpy(buf, buf_tmp);
			printf("buf= %s\n",buf);
			usleep(1000);
		}
		else if (!memcmp(buf, "iwpriv wlan0 mp_query", 21)) {
			strcpy(buf, pre_result);
			MP_get_ext("wlan0", buf_tmp, MP_QUERY_STATS);
			strcpy(buf, buf_tmp);
			usleep(1000);
			printf("w0 2b= %s\n",buf);
		}
	#endif
	#if 1	//wlan1 
		else if (!memcmp(buf, "iwpriv wlan1 mp_tssi", 20)) {
			strcpy(buf, pre_result);
			MP_get_ext("wlan1", buf_tmp, MP_QUERY_TSSI);
			strcpy(buf, buf_tmp);
			printf("buf= %s\n",buf);
			usleep(1000);
		}
		else if (!memcmp(buf, "iwpriv wlan1 mp_ther", 20)) {
			strcpy(buf, pre_result);
			MP_get_ext("wlan1", buf_tmp, MP_QUERY_THER);
			strcpy(buf, buf_tmp);
			printf("buf= %s\n",buf);
			usleep(1000);
		}
		else if (!memcmp(buf, "iwpriv wlan1 mp_query", 21)) {
			strcpy(buf, pre_result);
			MP_get_ext("wlan1", buf_tmp, MP_QUERY_STATS);
			strcpy(buf, buf_tmp);
			usleep(1000);
			printf("w1 2b= %s\n",buf);
		}
	#endif	
		else 
		{
			if ( (!memcmp(buf, "flash read", 10)) ) {
				is_run = 0;
				if ((fp = fopen(WIFI_MP_TMP_FILE, "r")) == NULL)
					fprintf(stderr, "opening " WIFI_MP_TMP_FILE " failed !\n");
	
				if (fp) {
					fgets(buf, BUFLEN, fp);
					buf[BUFLEN-1] = '\0';
					fclose(fp);
				}	
				sprintf(pre_result, "data:%s", buf);
			}
			//ack to the client
			else if (!memcmp(buf, "flash get", 9))
				sprintf(pre_result, "%s > " WIFI_MP_TMP_FILE " ok", buf);
			else {
				sprintf(pre_result, "%s ok", buf);
			}
			
			if (!memcmp(buf, "iwpriv wlan0 mp_brx stop", 24)) {
				strcpy(buf, "stop");
				MP_get_ext("wlan0", buf, MP_BRX_PACKET);
			}
			else if (!memcmp(buf, "iwpriv wlan0 mp_tx", 18) && buf[18] == ' ') {
				memcpy(buf_tmp, buf+19, strlen(buf)-19);
				MP_get_ext("wlan0", buf_tmp, MP_TX_PACKET);
				strcpy(buf, buf_tmp);
			}
			else if (!memcmp(buf, "iwpriv wlan0 mp_ctx", 19) && buf[19] == ' ') {
				memcpy(buf_tmp, buf+20, strlen(buf)-20);
				MP_get_ext("wlan0", buf_tmp, MP_CONTIOUS_TX);
				strcpy(buf, buf_tmp);;
			}
			else if(!memcmp(buf, "iwpriv wlan0 read_reg", 21)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan0 read_rf", 20)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan0 mp_psd", 19)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
				system("cat /tmp/MP.txt");
			}
			else if(!memcmp(buf, "iwpriv wlan0 efuse_get", 22)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan0 efuse_set", 22)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan0 efuse_sync", 23)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan0 get_mib use_efuse", 30)) {
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
#if 1  //wlan 1
			else if (!memcmp(buf, "iwpriv wlan1 mp_brx stop", 24)) {
				strcpy(buf, "stop");
				MP_get_ext("wlan1", buf, MP_BRX_PACKET);
			}
			else if (!memcmp(buf, "iwpriv wlan1 mp_tx", 18) && buf[18] == ' ') {
				memcpy(buf_tmp, buf+19, strlen(buf)-19);
				MP_get_ext("wlan1", buf_tmp, MP_TX_PACKET);
				strcpy(buf, buf_tmp);
			}
			else if (!memcmp(buf, "iwpriv wlan1 mp_ctx", 19) && buf[19] == ' ') {
				memcpy(buf_tmp, buf+20, strlen(buf)-20);
				MP_get_ext("wlan1", buf_tmp, MP_CONTIOUS_TX);
				strcpy(buf, buf_tmp);;
			}
			else if(!memcmp(buf, "iwpriv wlan1 read_reg", 21)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan1 read_rf", 20)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan1 mp_psd", 19)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
				system("cat /tmp/MP.txt");
			}
			else if(!memcmp(buf, "iwpriv wlan1 efuse_get", 22)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan1 efuse_set", 22)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
			else if(!memcmp(buf, "iwpriv wlan1 efuse_sync", 23)){
				strcat(buf, " > " WIFI_MP_TMP_FILE);
				system(buf);
			}
#endif            
			else if (!memcmp(buf, "probe", 5))
				strcpy(buf, "ack");
			else if (!memcmp(buf, "verify_flw", 10)) {
				if ((fp = fopen(WIFI_MP_TMP_FILE, "r")) == NULL)
					fprintf(stderr, "opening " WIFI_MP_TMP_FILE " failed !\n");
	
				if (fp) {
					fgets(buf, BUFLEN, fp);
					buf[BUFLEN-1] = '\0';
					fclose(fp);
				}
			}
			else 
			{

#if 0
				if (!memcmp(buf, "flash get", 9))
					strcat(buf, " > " WIFI_MP_TMP_FILE);
#endif
				if (!memcmp(buf, "flash get", 9))
				{
					char mib_val[64];
          
					read_mib_content_from_drv(buf+10, mib_val);
					// sprintf(cmdWrap, "flash gethw %s", buf+10);
					sprintf(cmdWrap, "echo %s=%s", buf+10, mib_val); 
					rwHW = 1;
					////strcat(buf, " > " WIFI_MP_TMP_FILE);
					strcat(cmdWrap, " > " WIFI_MP_TMP_FILE);
				}

				if (!memcmp(buf, "flash set", 9)) 
				{
					// sprintf(cmdWrap, "flash sethw %s", buf+10);
					write_mib_content_to_drv(buf+10, cmdWrap);

					// sprintf(cmdWrap, "echo %s", buf+10); 
					
					rwHW = 1;
					//printf("1 sent command (%s) to IP:%s\n", pre_result, inet_ntoa(their_addr.sin_addr));
					if ((numbytes = sendto(sockfd, pre_result, strlen(pre_result), 0,
						(struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
						fprintf(stderr, "send failed\n");
						close(sockfd);
						exit(1);
					}
					//printf("2 sent command (%s) to IP:%s\n", pre_result, inet_ntoa(their_addr.sin_addr));
				}
				
				if(rwHW == 1)
				{
					//printf("UDP Server cmdWrap=%s\n",cmdWrap);
					system(cmdWrap);
				}
				else
				{
					// printf("UDP Server buf=%s\n",buf);
					if ( is_run )
					    system(buf);
				}
				
				//delay
				//open(/tmp/abc.txt)
			}
			
			strcpy(buf_tmp, pre_result);
			strcpy(pre_result, buf);
			strcpy(buf, buf_tmp);
			
			//printf("UDP Server buf=%s pre_result=%s\n",buf,pre_result);
		}

		if (memcmp(buf, "flash set", 9) != 0) {
			//printf("1 sent command (%s) to IP:%s\n", buf, inet_ntoa(their_addr.sin_addr));
			
			//printf("UDP Server_5 buf=%s\n",buf);
			
			if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,
				(struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
				fprintf(stderr, "send failed\n");
				//printf("UDP Server_5_1\n");
				close(sockfd);
				exit(1);
			}
			//printf("UDP Server_6\n");
			//printf("2 sent command (%s) to IP:%s\n", buf, inet_ntoa(their_addr.sin_addr));
		}
		
		//printf("UDP Server_7\n");
	}

	return 0;
}

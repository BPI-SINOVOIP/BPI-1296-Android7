#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/wireless.h>

#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include <time.h>
#include <sys/time.h>


#define SIOC11VBSSTRANSREQ		0x8BF5
#define MACADDRLEN				6

#pragma pack(1)

struct bss_transition_para
{
    unsigned char addr[MACADDRLEN];
    unsigned char chan_until;
    unsigned char FomUser;	
};

#pragma pack(0)


/* Commands */

typedef enum
{
    NO_CMD = 0,
    BSS_TRANS_CMD,

    /*----*/
    CMD_NUM
} cmd_type_t;

typedef struct
{
    cmd_type_t id;
    const char *name;
    const char *description;
    int show_flag;
} cmd_t;


cmd_t CMDs[] =
{
    {NO_CMD, 				""      ,	"", 					0},
    {BSS_TRANS_CMD,	"bssTrans" , 	"BSS Transition request", 1},      
    
};

unsigned char tempbuf[16384];

static int issue_bssTransReq(char *ifname, unsigned char * macaddr, int chanUtil)
{
    struct bss_transition_para bssTrans_req;
    int sock;
    struct iwreq wrq;
    int err;
    
    /*** Inizializzazione socket ***/
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        err = errno;
        printf("%s(%s): Can't create socket for ioctl.(%d)", __FUNCTION__, ifname, err);
        goto out;
    }
    memset(&bssTrans_req, 0, sizeof(struct bss_transition_para));  	
	
    memcpy(bssTrans_req.addr, macaddr, MACADDRLEN);
    bssTrans_req.chan_until = chanUtil;
    memcpy(tempbuf, &bssTrans_req, sizeof(struct bss_transition_para));
	
    /*** Inizializzazione struttura iwreq ***/
    memset(&wrq, 0, sizeof(wrq));
    strncpy(wrq.ifr_name, ifname, IFNAMSIZ);

    /*** give parameter and buffer ***/
    wrq.u.data.pointer = (caddr_t)tempbuf;
    wrq.u.data.length = sizeof(struct bss_transition_para);

    /*** ioctl ***/
    if(ioctl(sock, SIOC11VBSSTRANSREQ, &wrq) < 0)
    {
        err = errno;
        printf("%s(%s): ioctl Error.(%d)", __FUNCTION__, ifname, err);
        goto out;
    }
    err = 0;

out:
    close(sock);
    return err;
}


static void do_bss_transition(char *ifname, char * macaddr_str, int chanUtil)
{
    int i;
    unsigned char macaddr[MACADDRLEN];
    for(i=0; i<MACADDRLEN; i++)
    {
        tempbuf[0] = macaddr_str[2*i];
        tempbuf[1] = macaddr_str[2*i+1];
        tempbuf[2] = 0;
        macaddr[i] = strtol((char *)tempbuf, NULL, 16);
    }
	
    if(issue_bssTransReq(ifname, macaddr, chanUtil))
    	  printf("Request Failed!\n");
	
}

int get_cmd_id(char *cmd)
{
    int i;
    for (i = 1; i < CMD_NUM; i++)
    {
        if (strcmp(CMDs[i].name, cmd) == 0)
            break;
    }
    if(i < CMD_NUM) {
        return CMDs[i].id;
    }
    else 
        return NO_CMD;
}

void usage(char *name)
{
    int i;
    cmd_t *cmd;

    printf("%s [-c command] [-i interface_name] [-a mac_addr]\n", name);
    printf("\nAvailable commands:\n");

    for (i=0; i<CMD_NUM; i++)
    {
        cmd = &CMDs[i];
        if (cmd->show_flag)
            printf("%s\t: %s\n", cmd->name, cmd->description);
    }
}

int main(int argc, char *argv[])
{
    cmd_type_t cmd_id;
    char *command = NULL, *mac_str = NULL;
    char * interface_name = NULL;
    int c;
    int chanUtil = 0;
	
    /* Parse options */
    while ((c = getopt (argc, argv, "hc:a:i:p:")) != -1)
    {
        switch (c)
        {
            case 'c':
                command = optarg;
                break;
            case 'i':
                interface_name = optarg;
                break;
            case 'a':
                mac_str = optarg;
                break;
	     case 'p':
                chanUtil = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
		  exit(0);
                break;
            default:
                usage(argv[0]);
                abort();

        }
    }
    /* Check arguments */
    if (command == NULL || interface_name == NULL)
    {
        usage(argv[0]);
        exit(0);
    }

    if ((cmd_id = get_cmd_id(command)) == NO_CMD)
    {
        usage(argv[0]);
        exit(0);
    }

    /* Execute command */
    switch(cmd_id)
    {
    	case BSS_TRANS_CMD:
		do_bss_transition(interface_name, mac_str, chanUtil);	
		break;
        default:
            break;
    }

    exit(0);
}

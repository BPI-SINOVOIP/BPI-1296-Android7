#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>


#define RTL8192CD_IOCTL_SET_MIB 0x89f1
static int set_mib(const char *ifname, const char *item_str, const char *value_str)
{

	int sock;	
	struct iwreq wrq;
	static char mib_str_buf[512];
	int err;

	sprintf(mib_str_buf, "%s=%s", item_str, value_str);
	printf("set mib for %s: %s\n", ifname, mib_str_buf);
	
	/*** Inizializzazione socket ***/
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {	
		err = errno;
		printf("set_mib: %s(%s). Error Creating Socket for ioctl.(%d)\n", mib_str_buf, ifname, err); 
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
		printf("set_mib: %s(%s). Error to ioctl.(%d)\n", mib_str_buf, ifname, err);
		goto out;
	}
	err = 0;

out:	
	close(sock);
	return err;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s ifname mib_item mib_value\n", argv[0]);
	}
	set_mib(argv[1], argv[2], argv[3]);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#define TCP_PORT 59956
#define RECVICEFILE "/tmp/receivefile"

typedef unsigned long ip_address;
typedef unsigned short uint16;

static const int BUFFER_SIZE = 16*1024;

static void SendFile(char *ipAddress, uint16 port, char *fileName)
{
	struct sockaddr_in saAddr;
	char buf[BUFFER_SIZE];
	ssize_t bytesRead;
   	FILE * fpIn = fopen(fileName, "r");

   	if (fpIn)
   	{
      		int s = socket(AF_INET, SOCK_STREAM, 0);
      		if (s >= 0)
      		{
         		memset(&saAddr, 0, sizeof(saAddr));
			saAddr.sin_family      = AF_INET;
         		saAddr.sin_addr.s_addr = inet_addr(ipAddress);
         		saAddr.sin_port        = htons(port);

         		if (connect(s, (struct sockaddr *) &saAddr, sizeof(saAddr)) == 0)
         		{
            			printf("Connected to remote host, sending file [%s]\n", fileName);

            			while(1)
            			{
					bytesRead = fread(buf, 1, sizeof(buf), fpIn);
					if (bytesRead <= 0) break;  // EOF

					printf("Read %i bytes from file, sending them to network...\n", (int)bytesRead);
					if (send(s, buf, bytesRead, 0) != bytesRead)
					{
						perror("send");
						break;
					}
				}
			}
			else perror("connect");

			close(s);
		}
		else perror("socket");

		fclose(fpIn);
	}
	else 
		printf("Error, couldn't open file [%s] to send!\n", fileName);
}

static void ReceiveFile(uint16 port)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	const int trueValue = 1;
	struct sockaddr_in saAddr;
	int connectSocket;
	FILE *fpIn = NULL;
	char buf[BUFFER_SIZE];
	ssize_t bytesReceived;

	if (s >= 0)
	{
		(void) setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &trueValue, sizeof(trueValue));

		memset(&saAddr, 0, sizeof(saAddr));
		saAddr.sin_family      = AF_INET;
		saAddr.sin_addr.s_addr = htonl(0);  // (IPADDR_ANY)
		saAddr.sin_port        = htons(port);

		if ((bind(s, (struct sockaddr *) &saAddr, sizeof(saAddr)) == 0)&&(listen(s, 10) == 0))
		{
			memset(&saAddr, 0, sizeof(saAddr));
			socklen_t len = sizeof(saAddr);
			printf("Waiting for incoming TCP connection on port %u, to write file [%s]\n", port, RECVICEFILE);
			connectSocket = accept(s, (struct sockaddr *) &saAddr, &len);
			if (connectSocket >= 0)
			{
				fpIn = fopen(RECVICEFILE, "wb");
				if (fpIn)
				{
					while(1)
					{
						bytesReceived = recv(connectSocket, buf, sizeof(buf), 0);
						if (bytesReceived < 0) 
							perror("recv");  // network error?

						if (bytesReceived == 0) 
							break;   // sender closed connection, must be end of file

						printf("Received %i bytes from network, writing them to file...\n", (int) bytesReceived);
						if (fwrite(buf, 1, bytesReceived, fpIn) != (size_t) bytesReceived)
						{
							perror("fwrite");
							break;
						}
					}

					fclose(fpIn);
				}
				else printf("Error, couldn't open file [%s] to receive!\n", RECVICEFILE);

				close(connectSocket);
			}
		}
		else perror("bind");

		close(s);
	}
	else 
		perror("socket");
}

int main(int argc, char ** argv)
{
	char *fileName;
	char *ip;
	uint16 port = TCP_PORT;

	if (strcmp(argv[1], "send") == 0) {
		printf("tcp client mode\n");
		fileName = argv[2];
		ip = argv[3];
		SendFile(ip, port, fileName);
	}else{
		printf("tcp server mode\n");
		ip = 0;
		while(1) {
			ReceiveFile(port);
		}
	}

	printf("Exiting, bye!\n");

	return 0;
}

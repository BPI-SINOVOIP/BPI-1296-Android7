
#ifndef __SOCKET__READER_H__
#define __SOCKET__READER_H__

#include <Platform_Lib/SocketAVData/SocketAVData.h>

typedef struct _tagIOPlugin_Socket 
{
	int					socketId;
	SOCKET_DATA_HEADER	header;
	long				remainPayloadSize;
    
} IOPlugin_Socket;

int Socket_Reader_CreateConnection(char* path);
void Socket_Reader_CloseConnection(int fd);
int Socket_Reader_Read(IOPlugin_Socket* pInstance, unsigned char* pBuffer, int size, NAVBUF* pEventReceived);


#endif //__SOCKET__READER_H__

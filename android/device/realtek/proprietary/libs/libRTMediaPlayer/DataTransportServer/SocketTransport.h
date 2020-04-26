#ifndef SOCKET_TRANSPORT_H
#define SOCKET_TRANSPORT_H

#include "IDataTransport.h"
#define SOCKET_TRANSPORT_PATH "/tmp/MediaData"
class SocketTransport : public IDataTransport
{
public:
	SocketTransport();
	virtual ~SocketTransport();
	
	virtual bool RequestFreeSpace(int requestSize);
	virtual CONNECTION_STATE DeliverData(const unsigned char* pPayload, int payloadSize, bool* pbTimeOut);
	virtual bool IsReady();
	
private:
	bool xCreateConnection(char *path)	;
		
private:
	int m_listenSocket;
	int m_socket;
};
#endif

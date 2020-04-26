#ifndef SHARED_MEMORY_TRANSPORT_H
#define SHARED_MEMORY_TRANSPORT_H

#include "IDataTransport.h"
#include <Platform_Lib/SocketAVData/SocketAVData.h>
class SharedMemTransport : public IDataTransport
{
public:	
	SharedMemTransport(SOCKET_SHARED_MEM* pMemHeader, unsigned long memHeaderPhyAddr)
	: m_pMemHeader(pMemHeader)
	, m_offsetToPhyAddr((unsigned long)pMemHeader - memHeaderPhyAddr)
	 {
	 	
	 }
	
	virtual ~SharedMemTransport(){}
	
	virtual bool RequestFreeSpace(int requestSize);
	virtual CONNECTION_STATE DeliverData(const unsigned char* pPayload, int payloadSize, bool* pbTimeOut);
	virtual bool IsReady() { return true; }

    virtual long getBufferSize();
    virtual long getBufferFullness();
private:
	SOCKET_SHARED_MEM*	m_pMemHeader;
	long 				m_offsetToPhyAddr;
};

#endif

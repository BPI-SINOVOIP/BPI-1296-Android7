#ifndef IDATA_TRANSPORT_H
#define IDATA_TRANSPORT_H

typedef enum
{
	CONNECTION_NOERROR = 1,
	CONNECTION_WRITE_FAIL = 0,
	CONNECTION_BUSY = -1,
	CONNECTION_NO_MEM = -2,
	CONNECTION_SHUTDOWN = -3,
	
} CONNECTION_STATE;

class IDataTransport
{
public:
	IDataTransport() {}
	virtual ~IDataTransport(){}
	
	virtual bool RequestFreeSpace(int requestSize) = 0;
	virtual CONNECTION_STATE DeliverData(const unsigned char* pPayload, int payloadSize, bool* pbTimeOut) = 0;
	virtual bool IsReady() = 0;

    virtual long getBufferSize(){return 0;}
    virtual long getBufferFullness(){return 0;}
};
#endif

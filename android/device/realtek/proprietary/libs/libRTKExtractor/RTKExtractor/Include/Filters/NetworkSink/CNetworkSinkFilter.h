#ifndef __C_FILTER_NW_SINK_H__
#define __C_FILTER_NW_SINK_H__


#include <CStream.h>
//#include <Filters/NetworkSink/spu.h>
#include <Filters/NetworkSink/CNetworkSinkPin.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
//#include <../../Filters/NavigationFilter/IOPlugins/TranscodePlugin/TranscodeProtocol.h>
//#include <net/if.h>
/*
#include <stdio.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#include <errno.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>	
*/
//#include <Filters/NetworkSink/SpuDecoderType.h>
//#include <Filters/NetworkSink/SPU_RPC.h>
//#include <Util.h>
//#include <Filters/PrivateInfo.h>

//typedef ulCCInPinSpuircularQueue<UOP_Data_T>             DVD_UOP_QUEUE;

#include <assert.h>

#define SINKassert(exp) 				\
	if (0)							\
		assert(exp);				\
	else							\
		if (!(exp))					\
			printf("[%s:%d]%s() failed -------------------------------------> :(\n", __FILE__, __LINE__, __func__);	\

#define IO_CMD_PORT	8080	
#define IO_DATA_PORT	8084

#define IO_CMD_SERVICE               0x00000004

#define MSG_SIZE 1024

#define SEND_TO_NETWORK

class CInPinNetworkSink;


//class CFilterNetworkSink: public CBaseFilter,
//                  public CReferenceClock{
class CFilterNetworkSink: public CBaseFilter,
                  public IFilterSeeking{

friend class                    CInPinNetworkSink;

public:
                                CFilterNetworkSink(void);
                                ~CFilterNetworkSink(void);

//    HRESULT				FilterType(FILTER_TYPE* pFilterType); // ??

    HRESULT				Stop(void);
    HRESULT				Pause(FILTER_STATE State);
    HRESULT				Run(void);
//    void				init(void);
	HRESULT				Flush(void);


    HRESULT				CheckCapabilities(FILTER_SEEKING_CAP* pSeekingCap);
	HRESULT				SetSeekingInfo(long Speed, long Skip);
	HRESULT				GetSeekingInfo(long* pSpeed, long* pSkip);
#ifdef USE_RING_BUF	
	void 				SendData(void *buffer, int len);
#else
	int	 				SendData(void *buffer, int len);
	bool				IsDataPortConnect() { return m_bIsDataPortConnect;}
	bool				IsDataThreadAlive()	{ return ((m_pDATAThread == NULL) ? false : true);}
	//bool				IsRestData() { return m_bIsRestData;}
#endif	
	int					StartIoCmdServer(); 
	
protected:
    // Ring buffer allocator parts
//    bool				InitRingData(void);
	static void         SendDataThreadEntry(void* pThis);
	
#ifdef USE_RING_BUF	
    virtual bool        SendDataThread(void);    
#else

//	void				CreateConnectDataPortThread(void);
//	void				DeleteConnectDataPortThread(void);
//	virtual bool		ConnectDataPort(void);   
	bool				ConnectDataPort(void);    
 
#endif    
    static void			GetCMDThreadEntry(void* pThis);
	virtual bool		GetCMDThread(void);
	int					SendCMD(char *Message,int Message_len,int timeout_ms);
#ifdef USE_RING_BUF
	void				Clean_ReceiveQ();
#endif	
private:	
	
	void 				xInit();

#ifdef USE_RING_BUF
public:	
	BYTE*				 m_pRBLower;
	BYTE* 				 m_pRBUpper;
	//long   				 m_pRBSize;	
    RINGBUFFER_HEADER*   m_pRBHeader;
    SINK_DATA_INFO_QUEUE m_SinkDataQueue; 
    //IMemRingAllocator*	 m_pAllocator;
#endif
protected:
    CInPinNetworkSink*	m_pInPin;
//#ifndef USE_SPU_DECODER_SIMULATOR    
//#endif    
//    IReadPointerHandle*	m_pRPHandle;
    //long				m_agentInstanceID;
//    IReferenceClock*	m_pRefClock;
	//CLNT_STRUCT			m_rpcclnt;
    long 				m_Speed;
    long 				m_Skip;	
    CThread*			m_pDATAThread;
    CThread*			m_pCMDThread;
#ifdef USE_RING_BUF    
    bool				m_IsRequestFlush;    
    CCritSec           	m_mutexFlush;
#endif    
//	CCritSec			m_mutexf;

	int					m_sServer;	//server cmd sock
	int					m_sClient;	//client cmd sock
	int					m_sDataServer;  //server data sock
	int 				m_sDataClient;  //client data sock
	uint32_t			m_state;
	bool				m_bIsDataPortConnect;
	bool				m_bIsCMDPortConnect;
#ifndef USE_RING_BUF	
	bool                m_bIsResetConnection;
#endif	
#ifndef SEND_TO_NETWORK 	
    CBaseOutputPin*    	m_pOutPin;
    int 				m_nfd_test;
	long				m_FilePos;
#endif	
};

#endif

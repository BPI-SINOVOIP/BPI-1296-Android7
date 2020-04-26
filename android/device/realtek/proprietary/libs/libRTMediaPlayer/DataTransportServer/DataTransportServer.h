#ifndef SOCKET_TRANSPORT_SERVER_H
#define SOCKET_TRANSPORT_SERVER_H
#include <unistd.h>
#include <string.h>
#include "SocketTransport.h"
#include "SharedMemTransport.h"
#include <Platform_Lib/SocketAVData/SocketAVData.h>

typedef enum
{
	DATA_TRANSPORT_BY_SOCKET,
	DATA_TRANSPORT_BY_SHARED_MEM,
	
} DATA_TRANSPORT_MODE;

class MediaAttribute
{
public:
	MediaAttribute() {InitMediaAttr();}
	~MediaAttribute() {}
	
	//set stream type
	inline void SetVideoType(SOCKET_MEDIA_TYPE type) {m_mediaAttr.videoType = type;}
	inline void SetAudioType(SOCKET_MEDIA_TYPE type) {m_mediaAttr.audioType = type;}
	inline void SetSpicType(SOCKET_MEDIA_TYPE type) {m_mediaAttr.spicType = type;}
	
	//set stream pid if it is ts container
	inline void SetVideoPid(long pid) {m_mediaAttr.videoPid = pid;}
	inline void SetAudioPid(long pid) {m_mediaAttr.audioPid = pid;}
	inline void SetSpicPid(long pid) {m_mediaAttr.spicPid = pid;}
	
	//set audio attributes
	inline void SetAudioChNum(long chNum) {m_mediaAttr.channelNum = chNum;}
	inline void SetAudioSampleRate(long rate) {m_mediaAttr.sampleRate = rate;}
	inline void SetAudioBitsPerSample(long bits) {m_mediaAttr.bitsPerSample = bits;}
	inline void SetSBRflag(long sbr)	{m_mediaAttr.sbrFlag = sbr;}
	inline void SetCodecID(long id)		{m_mediaAttr.codecID = id; }
	inline void SetBlockAlignSize(long size) { m_mediaAttr.blockAlignSize = size; }
	
	//set container attributes
	inline void SetPacketSize(long packetSize) {m_mediaAttr.packetSize = packetSize;}
	inline void SetContainerType(SOCKET_CONTAINER_TYPE containerType) {m_mediaAttr.containerType = containerType;}
	inline void SetAppType(SOCKET_APP_TYPE appType) {m_mediaAttr.appType = appType;  }
	inline void SetMonitorPAT(bool bMonitorPAT) { m_mediaAttr.bMonitorPAT = bMonitorPAT; }
	inline void SetPcrPid(long pcrPid) { m_mediaAttr.pcrPid = pcrPid; }
	
	//set video attributes
	inline void SetIsDivx4(long isDivx4)	{ m_mediaAttr.isDIVX4 = isDivx4; }
	inline void SetProfile(long profile)	{ m_mediaAttr.profile = profile; }
	inline void SetPicSize(long width, long height) { m_mediaAttr.pictureWidth = width; m_mediaAttr.pictureHeight = height; }
	inline void SetPTSIncrement(long increment)	{m_mediaAttr.ptsIncrement = increment; }
	
	//set spu attributes
	inline void SetSpicLangCode(unsigned int langCode) {m_mediaAttr.spicLangCode = langCode;}
	
	inline SOCKET_MEDIA_ATTRIBUTE* GetMediaAttr() {return &m_mediaAttr;}
	inline void InitMediaAttr()
	{
		m_mediaAttr.videoAttrSize = 28;
		m_mediaAttr.videoType = S_MEDIA_TYPE_NONE;
		m_mediaAttr.videoPid = -1;
		m_mediaAttr.isDIVX4 = -1;
		m_mediaAttr.ptsIncrement = -1;
		m_mediaAttr.profile = -1;
		m_mediaAttr.pictureWidth = -1;
		m_mediaAttr.pictureHeight = -1;
		
		
		m_mediaAttr.audioAttrSize = 32;
		m_mediaAttr.audioType = S_MEDIA_TYPE_NONE;
		m_mediaAttr.channelNum = -1;
		m_mediaAttr.sampleRate = -1;
		m_mediaAttr.bitsPerSample = -1;
		m_mediaAttr.audioPid = -1;
		m_mediaAttr.sbrFlag = -1;
		m_mediaAttr.codecID = 0x0;
		m_mediaAttr.blockAlignSize = -1;
		
	
		m_mediaAttr.spicAttrSize = 12;
		m_mediaAttr.spicType = S_MEDIA_TYPE_NONE;
		m_mediaAttr.spicPid = -1;
		m_mediaAttr.spicLangCode = -1;
		
		m_mediaAttr.contentAttrSize = 20;
		m_mediaAttr.packetSize = -1;
		m_mediaAttr.containerType = SOCKET_CONTAINER_UNCHANGED;
		m_mediaAttr.appType = SOCKET_APP_NONE;
		m_mediaAttr.bMonitorPAT = false;
		m_mediaAttr.pcrPid = 0x1FFF;
	}
	
private:
	SOCKET_MEDIA_ATTRIBUTE m_mediaAttr;
};


class DataTransportServer
{
public:
	DataTransportServer(DATA_TRANSPORT_MODE mode = DATA_TRANSPORT_BY_SOCKET, SOCKET_SHARED_MEM* pMemHeader = 0, unsigned long memHeaderPhyAddr = 0);
	~DataTransportServer();

	
	inline bool IsReady() {return (m_pTransporter) ? m_pTransporter->IsReady() : false;}

	CONNECTION_STATE SendMediaAttr(const SOCKET_MEDIA_ATTRIBUTE* pAttr, bool* pbTimeOut);
	
	inline CONNECTION_STATE SendVideoESData(const unsigned char* pPayload, int payloadSize, long long pts, bool* pbTimeOut, unsigned char* pPrefixedData = 0, int prefixedDataSize = 0, unsigned char* pPaddingData = 0, int paddingDataSize = 0, int marker_bit = 0)
	{
		return SendDataAU(SOCKET_DATA_VIDEO, pPayload, payloadSize, pts, marker_bit, pbTimeOut, pPrefixedData, prefixedDataSize, pPaddingData, paddingDataSize);
	}
	inline CONNECTION_STATE SendAudioESData(const unsigned char* pPayload, int payloadSize, long long pts, bool* pbTimeOut, unsigned char* pPrefixedData = 0, int prefixedDataSize = 0, unsigned char* pPaddingData = 0, int paddingDataSize = 0)
	{
		return SendDataAU(SOCKET_DATA_AUDIO, pPayload, payloadSize, pts, 0, pbTimeOut, pPrefixedData, prefixedDataSize, pPaddingData, paddingDataSize);
	}
	inline CONNECTION_STATE SendSpicESData(const unsigned char* pPayload, int payloadSize, long long pts, bool* pbTimeOut)
	{
		return SendDataAU(SOCKET_DATA_SPIC, pPayload, payloadSize, pts, 0, pbTimeOut);
	}
	inline CONNECTION_STATE SendUndemuxedPackets(const unsigned char* pPackets, int size, unsigned int rtp_marker_bit, bool* pbTimeOut)
	{
		return SendDataAU(SOCKET_DATA_SS_PACKET, pPackets, size, -1, rtp_marker_bit, pbTimeOut);
	}
	
	inline CONNECTION_STATE SendUndemuxedPackets(const unsigned char* pPackets, int size, long long pts, unsigned int rtp_marker_bit, bool* pbTimeOut)
	{
		return SendDataAU(SOCKET_DATA_SS_PACKET, pPackets, size, pts, rtp_marker_bit, pbTimeOut);
	}	
	
	inline CONNECTION_STATE SendFlush(long type, long stamp, long iFrameOnly, bool* pbTimeOut)
	{
		long infoData[] = {type, stamp, iFrameOnly};
		
		return SendInfoAU(SOCKET_INFO_FLUSH, (unsigned char*)infoData, sizeof(infoData), pbTimeOut);
	}
	
	inline CONNECTION_STATE SendEOS(bool* pbTimeOut)
	{
		return SendInfoAU(SOCKET_INFO_EOS, 0, 0, pbTimeOut);
	}
	
	inline CONNECTION_STATE SendIV(const unsigned char* pIV, bool* pbTimeOut)
	{
		return SendInfoAU(SOCKET_INFO_IV, pIV, 16, pbTimeOut);
	}
	inline CONNECTION_STATE SendKey(const unsigned char* pEvenKey, const unsigned char* pOddKey, bool* pbTimeOut)
	{
		unsigned char key[2][16];
		memcpy(key[0], pOddKey, 16);
		memcpy(key[1], pEvenKey, 16);
		return SendInfoAU(SOCKET_INFO_KEYS, (unsigned char*)key, 32, pbTimeOut);
	}
	
	inline CONNECTION_STATE SendIVOffset(SOCKET_CW_OFFSET* pIV, bool* pbTimeOut)
	{
		return SendInfoAU(SOCKET_INFO_IV_OFFSET, (unsigned char*)pIV, sizeof(SOCKET_CW_OFFSET), pbTimeOut);
	}
	inline CONNECTION_STATE SendKeyOffset(SOCKET_CW_OFFSET* pEvenOddKey, bool* pbTimeOut)
	{
		return SendInfoAU(SOCKET_INFO_KEYS_OFFSET, (unsigned char*)pEvenOddKey, sizeof(SOCKET_CW_OFFSET), pbTimeOut);
	}
	
	inline CONNECTION_STATE SendResolution(long width, long height, bool* pbTimeOut)
	{
		long infoData[] = {width, height};
		
		return SendInfoAU(SOCKET_INFO_CLIENT_RESOLUTION, (unsigned char*)infoData, sizeof(infoData), pbTimeOut);
	}
	
	inline CONNECTION_STATE SendClearScreen(bool* pbTimeOut)
	{
		return SendInfoAU(SOCKET_INFO_CLEAR_SCREEN, 0, 0, pbTimeOut);
	}
	
	inline CONNECTION_STATE SendPicRect(int x, int y, int width, int height, int picSize, int renderFlag, bool* pbTimeOut)
	{
		static int sn = 0;
		static long long pts = 0;
		//sn++;
		SOCKET_JPEG_PIC_INFO infoData = {sn : sn, x : x, y : y, width : width, height : height, picSize : picSize, renderFlag : renderFlag, pts : pts};
		
		return SendInfoAU(SOCKET_INFO_PIC_INFO, (unsigned char*)&infoData, sizeof(infoData), pbTimeOut);
	}
	inline CONNECTION_STATE SendDescrambleConfig(SOCKET_TS_DESC_CONFIG *pConfig, bool* pbTimeOut)
	{
		return SendInfoAU(SOCKET_INFO_DES_CONFIG, (unsigned char*)pConfig, sizeof(SOCKET_TS_DESC_CONFIG), pbTimeOut);
	}
	
	inline CONNECTION_STATE SendSpuFrameInfo(SOCKET_SPU_FRAME_INFO *pInfo, bool* pbTimeOut)
	{
		return SendInfoAU(SOCKET_INFO_SPU_FRAME, (unsigned char*)pInfo, sizeof(SOCKET_SPU_FRAME_INFO), pbTimeOut);
	}
public:
	CONNECTION_STATE SendDataAU(SOCKET_DATA_TYPE auType, const unsigned char* pPayload, int payloadSize, long long pts, unsigned int rtp_marker_bit, bool* pbTimeOut, unsigned char* pPrefixedData = 0, int prefixedDataSize = 0, unsigned char* pPaddingData = 0, int paddingDataSize = 0);
	CONNECTION_STATE SendInfoAU(SOCKET_INFO_TYPE infoType, const unsigned char* pInfoData, int infoSize, bool* pbTimeOut);
	CONNECTION_STATE SendAU(const unsigned char* pHeader, const unsigned char* pPayload, int payloadSize, bool* pbTimeOut, unsigned char* pPrefixedData = 0, int prefixedDataSize = 0, unsigned char* pPaddingData = 0, int paddingDataSize = 0);
	
	
    long getBufferSize(){return (m_pTransporter == 0)? 0 : m_pTransporter->getBufferSize();}
    long getBufferFullness(){return (m_pTransporter == 0)? 0 : m_pTransporter->getBufferFullness();}
private:
	IDataTransport* m_pTransporter;
	char m_path[512];
	
	
};

#endif

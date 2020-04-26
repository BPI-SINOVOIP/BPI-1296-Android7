//-----------------------------------------------------------------------------
// HDMVDecoder_CodedDataBuffer.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_CODEDDATABUFFER_H__
#define __HDMVDECODER_CODEDDATABUFFER_H__

#include "ref_ptr.h"
#include "SonicSemaphore.h"
//#include "IPacketSinkCallback.h"
#include "BaseRenderer.h"
#include "BDROM_NavigatorTypes.h"
#include "HDMVDecoder_EB.h"
#include "HDMVDecoder_Types_Internal.h"
#include "SonicCriticalSection.h"
#include "packetdef.h"
#include <string.h>

using namespace BDROM_HDMVDecoder;

class HDMVDecoder_StreamGraphicsProcessor;

#ifdef REALTEK_MODIFY
class BufferedPacket
{
public:
	BufferedPacket(ref_ptr<PACKET_DATA> packet)
	{
		SetPacket(packet);
	}

	BufferedPacket(ref_ptr<PACKET_DATA> packet, SInt64 platformDTS, SInt64 platformPTS)
	{
		SetPacket(packet, true, platformDTS, platformPTS);
	}

	~BufferedPacket()
	{
		m_Packet = 0;
		if (m_PacketLen)
		{
			memset(m_PacketData, 0, m_PacketLen);
			m_PacketLen = 0;
		}
	}

	BufferedPacket & operator= (BufferedPacket r)
	{
		assert(0);
		return *this;
	}

	void SetPacket(ref_ptr<PACKET_DATA> packet, bool bHavePlatformDTSPTS = false, SInt64 platformDTS = 0, SInt64 platformPTS = 0)
	{
		m_Packet = packet;
		assert(packet->nHeaderLen <= 188 && packet->nPacketLen <= 188);
		m_PacketLen = packet->nPacketLen <= 188 ? packet->nPacketLen : 188;
		memcpy(m_PacketData, packet->pData, m_PacketLen);
		m_Packet->pData = m_PacketData;
		
		m_bHavePlatformDTSPTS = bHavePlatformDTSPTS;
		m_PlatformDTS = platformDTS;
		m_PlatformPTS = platformPTS;
	}

	ref_ptr<PACKET_DATA> GetPacket(void)
	{
		return m_Packet;
	}

	bool GetPlatformDTSPTS(SInt64 & platformDTS, SInt64 & platformPTS)
	{
		if (m_bHavePlatformDTSPTS)
		{
			platformDTS = m_PlatformDTS;
			platformPTS = m_PlatformPTS;
		}
		return m_bHavePlatformDTSPTS;
	}

private:
	ref_ptr<PACKET_DATA>		m_Packet;
	UInt32						m_PacketLen;
	UInt8						m_PacketData[188];

	bool						m_bHavePlatformDTSPTS;
	SInt64						m_PlatformDTS;
	SInt64						m_PlatformPTS;
};
#else
class BufferedPacket
{
public:
	BufferedPacket()
	{
		m_Packet = 0;
		m_PacketHdrLen = 0;
		m_bHavePlatformDTSPTS = false;
		m_PlatformDTS = m_PlatformPTS = 0;
	}

	BufferedPacket(ref_ptr<PACKET_DATA> packet)
	{
		SetPacket(packet);
	}

	BufferedPacket(ref_ptr<PACKET_DATA> packet, SInt64 platformDTS, SInt64 platformPTS)
	{
		SetPacket(packet, true, platformDTS, platformPTS);
	}

	~BufferedPacket()
	{
		m_Packet = 0;
		if (m_PacketHdrLen)
		{
			memset(m_PacketHdr, 0, m_PacketHdrLen);
			m_PacketHdrLen = 0;
		}
	}

	BufferedPacket & operator= (BufferedPacket r)
	{
		m_Packet = r.m_Packet;
		m_PacketHdrLen = r.m_PacketHdrLen;
		memcpy(m_PacketHdr, r.m_PacketHdr, m_PacketHdrLen);
		m_bHavePlatformDTSPTS = r.m_bHavePlatformDTSPTS;
		m_PlatformDTS = r.m_PlatformDTS;
		m_PlatformPTS = r.m_PlatformPTS;
		return *this;
	}

	void SetPacket(ref_ptr<PACKET_DATA> packet, bool bHavePlatformDTSPTS = false, SInt64 platformDTS = 0, SInt64 platformPTS = 0)
	{
		m_Packet = packet;
		assert(packet->nHeaderLen <= 188);
		m_PacketHdrLen = packet->nHeaderLen <= 188 ? packet->nHeaderLen : 188;
		memcpy(m_PacketHdr, packet->pData, m_PacketHdrLen);
		m_bHavePlatformDTSPTS = bHavePlatformDTSPTS;
		m_PlatformDTS = platformDTS;
		m_PlatformPTS = platformPTS;
	}

	ref_ptr<PACKET_DATA> GetPacket(void)
	{
		ref_ptr<PACKET_DATA> packet = m_Packet;
		memcpy(packet->pData, m_PacketHdr, m_PacketHdrLen);
		return packet;
	}

	bool GetPlatformDTSPTS(SInt64 & platformDTS, SInt64 & platformPTS)
	{
		if (m_bHavePlatformDTSPTS)
		{
			platformDTS = m_PlatformDTS;
			platformPTS = m_PlatformPTS;
		}
		return m_bHavePlatformDTSPTS;
	}

private:
	ref_ptr<PACKET_DATA>		m_Packet;
	UInt32						m_PacketHdrLen;
	UInt8						m_PacketHdr[188];

	bool						m_bHavePlatformDTSPTS;
	SInt64						m_PlatformDTS;
	SInt64						m_PlatformPTS;
};
#endif
typedef std::vector<BufferedPacket*>			BufferedPacketList;
typedef BufferedPacketList::iterator			BufferedPacketListIterator;

//__________________________________________________________________________________________________________________________________________________
// HDMVDecoder_CodedDataBuffer
// Represents the EB in an IG/PG Decoder
//
class HDMVDecoder_CodedDataBuffer
{
public:
	HDMVDecoder_CodedDataBuffer();
	virtual ~HDMVDecoder_CodedDataBuffer();

	void Init(HDMVDecoder_StreamGraphicsProcessor * pStreamGraphicsProcessor, UInt32 uiEBSize, ref_ptr<BaseRenderer> pRenderer);
	virtual SonicError Reset(void);

	// receive data from Demux
	void ReceivePacket(UInt32 id, UInt16 streamId, UInt16 subStreamId, ref_ptr<PACKET_DATA> packet);

	// functions to allow the decoder to get segments from the EB
	SonicError GetSegment(UInt32 uiIndex, HDMVDec_EBSegment & ebSeg);
	SonicError GetSegmentReadPtr(HDMVDec_EBSegment & ebSeg, HDMVDec_EBPtr & ebPtr);
	void RemoveSegment(HDMVDec_EBSegment & ebSeg);

	// stream events

	// HDMVStreamTimes will have In_location, Out_location, In_time, and Out_time
	// This data becomes valid at In_location.
	void SetStreamExtent(HDMVStreamExtent & extent);

	virtual void StartOfStream(void);
	virtual void EndOfStream(void);
	virtual void Flush(void);

	// stream properties
	virtual void SetApplicationType(BDROM_ApplicationType appType)		{ m_ApplicationType = appType; }
	virtual void SetSyncOffsetPTS(SInt64 pts)							{ m_SyncOffsetPTS = pts; }

protected:
	// process received packet
	virtual SonicError ReceivePacket(ref_ptr<PACKET_DATA> packet, bool bHavePlatformDTSPTS = false, SInt64 platformDTS = 0, SInt64 platformPTS = 0) = 0;

	// add a packet to the overflow buffer (m_PacketBuffer).  Done as a method because the return code is used
	// to indicate to the base receivepacket implementation if the use of this buffer is supported by the 
	// decoder.  Override this method in derived class to add it and return a clean kHDMVDecErr_NoErr code if you
	// want to use it.
	virtual SonicError BufferPacket(ref_ptr<PACKET_DATA> packet) {return kHDMVDecErr_NotAvailable;}

	// Push oldest packet from overflow buffer into decoder
	virtual SonicError PushBufferedPacket(void) {return kHDMVDecErr_NotAvailable;}

private:
	HDMVDecoder_CodedDataBuffer(const HDMVDecoder_CodedDataBuffer &);
	HDMVDecoder_CodedDataBuffer& operator=(const HDMVDecoder_CodedDataBuffer &);

protected:
	typedef enum tagIGStreamParseState
	{
		kIGSPS_Idle,						// Idle state. Data received in this state will be ignored/discarded.
		kIGSPS_SequenceStart_PESPktStart,	// PES pkt start of the first (or only) fragment in a sequence.
		kIGSPS_SequenceCont_PESPktStart,	// PES pkt start of the non-first fragment in a sequence.
		kIGSPS_Sequence_PESPktCont,			// PES pkt processing (i.e., copying into EB).
	} HDMVStreamParseState;

protected:
	HDMVDecoder_StreamGraphicsProcessor *	m_pStreamGraphicsProcessor;	// SGP
	ref_ptr<BaseRenderer>					m_pRenderer;				// HDR

	HDMVDec_EB								m_EB;						// The EB (including physical buffer and array of segment info)

	HDMVStreamParseState					m_InputParseState;
	HDMVDec_EBSegment						m_CurrEBSegment;			// current segment being received (which is not added to the array yet)

	UInt32									m_uiPESPayloadSkip;			// number of bytes to be skipped at the start of a Segment/PES
	UInt32									m_uiPESPayloadSize;			// size of current PES Packet's payload
	UInt32									m_uiPESPayloadLeft;			// number of bytes of current PES Packet that are yet to arrive

	BDROM_ApplicationType					m_ApplicationType;			// application type of clip providing stream data
	SInt64									m_SyncOffsetPTS;			// offset PTS to be applied to sync this stream with main video

	SonicSemaphore					m_semEBSpaceLock;			// lock to wait on for space to be available in the EB

	BufferedPacketList						m_PacketBuffer;				// storage for packets coming in to avoid blocking the demux
	SonicCriticalSection					m_csPacketBufferLock;		// Lock for packet buffer access
};

#endif // __HDMVDECODER_CODEDDATABUFFER_H__

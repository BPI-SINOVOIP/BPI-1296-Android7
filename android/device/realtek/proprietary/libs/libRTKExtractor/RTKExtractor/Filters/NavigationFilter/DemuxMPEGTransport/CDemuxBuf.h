#ifndef __C_DEMUX_BUF_H__
#define __C_DEMUX_BUF_H__

#define CHECK_BUFFER_WRAP_AROUND(_ptr)	\
	if(_ptr >= m_pCachedEnd)			\
	{									\
		assert(_ptr == m_pCachedEnd);	\
		_ptr = m_pCachedStart;			\
	}


class BUF_INFO
{
public:
	BUF_INFO(int itemSize, int itemNum) 
		: m_pCachedStart(0)
		, m_pNonCachedStart(0)
		, m_pCachedEnd(0)
		, m_pReadPtrToUnparsed(0)
		, m_pReadPtrToDelivered(0)
		, m_pWritePtr(0)
		, m_bufSize(itemSize*itemNum)
		, m_itemSize(itemSize)
	{
		
	}
	
	virtual ~BUF_INFO() { if(m_pCachedStart){pli_freeContinuousMemory(m_pCachedStart); m_pCachedStart = 0;}}

	inline void Init()
	{
		
		unsigned long phyAddr;
		m_pCachedStart = (unsigned char*)pli_allocContinuousMemoryMesg("MPEGTRANSPORT", m_bufSize, &m_pNonCachedStart, &phyAddr); 
  		m_pCachedEnd = m_pCachedStart + m_bufSize;
  		m_pReadPtrToUnparsed = m_pReadPtrToDelivered = m_pCachedStart;
		m_pWritePtr = m_pCachedStart;
	
	}
	inline int GetBufferFreeSize() { return ((m_pReadPtrToDelivered > m_pWritePtr)  ? (m_pReadPtrToDelivered - m_pWritePtr-1) : (m_pReadPtrToDelivered - m_pWritePtr+m_bufSize-1));}
	inline int  GetBufferReadSize() { return ((m_pWritePtr >= m_pReadPtrToUnparsed) ? (m_pWritePtr-m_pReadPtrToUnparsed) : (m_pWritePtr-m_pReadPtrToUnparsed+m_bufSize)); }
	inline bool IsItemAvailable() { return (GetBufferReadSize() >= m_itemSize); }
	
	inline unsigned char* Peek() { return (IsItemAvailable() ? m_pReadPtrToUnparsed : 0); }
	inline void AdvanceOneItem() 
	{
		m_pReadPtrToUnparsed += m_itemSize;
		CHECK_BUFFER_WRAP_AROUND(m_pReadPtrToUnparsed);
		
	}

	inline bool InsertItem(unsigned char* pItem)
	{
		if(GetBufferFreeSize() < m_itemSize)
			return false;

		memcpy(m_pWritePtr, pItem, m_itemSize);
		pli_flushMemory(m_pWritePtr, m_itemSize);
		m_pWritePtr += m_itemSize;
		CHECK_BUFFER_WRAP_AROUND(m_pWritePtr);
		
		return true;
	}
	
	inline void Flush()
 	{
 		m_pReadPtrToUnparsed = m_pWritePtr;
 		SyncReadPointer() ;
 	}
 	
 	virtual void SyncReadPointer() { m_pReadPtrToDelivered = m_pReadPtrToUnparsed; }

public:
	unsigned char* m_pCachedStart;
	unsigned char* m_pNonCachedStart;
	unsigned char* m_pCachedEnd;
	
	unsigned char* m_pReadPtrToUnparsed;
	unsigned char* m_pReadPtrToDelivered;
	unsigned char* m_pWritePtr;
	int				m_bufSize;
	int				m_itemSize;

};

class BS_BUF_INFO : public BUF_INFO
{
public:
	BS_BUF_INFO() : BUF_INFO(188, 5120) {}
	virtual ~BS_BUF_INFO() {}
};

struct CMD_INFO 
{
	unsigned char*	pPtrToPacketBuf;
	unsigned int	infoId;
	int				infoSize;	//if infoSize <= 4, use infoData
								//else we should allocate extra memory and use pPtrToInfoData
	union{
		int				infoData;
		unsigned char* pPtrToInfoData;
	} data;
} ;
	
class CMD_BUF_INFO : public BUF_INFO
{

public:
	CMD_BUF_INFO() : BUF_INFO(sizeof(CMD_INFO), 32) { m_navBuf.type = NAVBUF_NONE; }
	virtual ~CMD_BUF_INFO() {}

	inline bool InsertCmd(unsigned int infoId, int infoSize, unsigned char* pInfoData, unsigned char* pPacketAddr)
	{
		CMD_INFO info;
		info.pPtrToPacketBuf = pPacketAddr;
		info.infoId = infoId;
		info.infoSize = infoSize;
		if(infoSize <= 4)	info.data.infoData = *((int*)pInfoData);
		else
		{
			info.data.pPtrToInfoData = (unsigned char*)malloc(infoSize);
			memcpy(info.data.pPtrToInfoData, pInfoData, infoSize);

		}

		assert( InsertItem((unsigned char*)&info) );
		return true;
	}

	virtual void SyncReadPointer() 
	{
		assert(m_pReadPtrToDelivered < m_pCachedEnd && m_pReadPtrToDelivered >= m_pCachedStart);
		assert(m_pReadPtrToUnparsed < m_pCachedEnd && m_pReadPtrToUnparsed >= m_pCachedStart);
		while(m_pReadPtrToDelivered != m_pReadPtrToUnparsed)
		{
			if(((CMD_INFO*)m_pReadPtrToDelivered)->infoSize > 4)
				free(((CMD_INFO*)m_pReadPtrToDelivered)->data.pPtrToInfoData);
			m_pReadPtrToDelivered += m_itemSize;
			CHECK_BUFFER_WRAP_AROUND(m_pReadPtrToDelivered);
		}
	}

	inline void Flush()
 	{
 		m_navBuf.type = NAVBUF_NONE;
 		m_pReadPtrToUnparsed = m_pWritePtr;
 		SyncReadPointer() ;
 	}
 	
public:
	NAVBUF m_navBuf; 
 	
};

#endif

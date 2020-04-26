#ifndef __NEPTUNE_TP_H__
#define __NEPTUNE_TP_H__

//#define		NEPTUNETP_DUMP_TS_AND_INFO
//#define 	NEPTUNETP_DUMP_PES
#define NeptuneTP_USE_M2M_MODE
#define		NEPTUNETP_PID_TABLE_SIZE				64
#define		NEPTUNETP_MAX_RING_BUF_NUM				NEPTUNETP_PID_TABLE_SIZE
#define		NEPTUNETP_PAT_PID						0

#define		NEPTUNETP_TS_PACKET_HEADER_LEN	 		4
#define		NEPTUNETP_INFO_PACK_LEN					8	
#define		NEPTUNETP_MAX_TS_PACKET_LEN				188
//#define		NEPTUNETP_READ_SIZE						(188*16)

#define		NEPTUNETP_MAX_PRIVATE_SECTION_LEN		4096

#define 	NEPTUNETP_MAX_READ_ITERATION			1

// =========================================================================
//				PES Stream Type
// =========================================================================
#define NEPTUNETP_ISO_11172_VIDEO             0x01            /* ISO/IEC 11172 Video */
#define NEPTUNETP_ISO_13818_VIDEO             0x02            /* ISO/IEC 13818-2 Video */
#define NEPTUNETP_ISO_11172_AUDIO             0x03            /* ISO/IEC 11172 Audio */
#define NEPTUNETP_ISO_13818_AUDIO             0x04            /* ISO/IEC 13818-3 Audio */
#define NEPTUNETP_ISO_13818_PRIVATE           0x05            /* ISO/IEC 13818-1 private sections */
#define NEPTUNETP_ISO_13818_PES_PRIVATE       0x06            /* ISO/IEC 13818-1 PES packets containing private data */
#define NEPTUNETP_ISO_13522_MHEG              0x07            /* ISO/IEC 13512 MHEG */
#define NEPTUNETP_ISO_13818_DSMCC             0x08            /* ISO/IEC 13818-1 Annex A  DSM CC */
#define NEPTUNETP_ISO_13818_TYPE_A            0x0a            /* ISO/IEC 13818-6 Multiprotocol encapsulation */
#define NEPTUNETP_ISO_13818_TYPE_B            0x0b            /* ISO/IEC 13818-6 DSM-CC U-N Messages */
#define NEPTUNETP_ISO_13818_TYPE_C            0x0c            /* ISO/IEC 13818-6 Stream Descriptors */
#define NEPTUNETP_ISO_13818_TYPE_D            0x0d            /* ISO/IEC 13818-6 Sections (any type, including private data) */
#define NEPTUNETP_ISO_13818_AUX               0x0e            /* ISO/IEC 13818-1 auxiliary */
#define NEPTUNETP_ISO_13818_PART7_AUDIO       0x0f            /* ISO/IEC 13818-7 Audio with ADTS transport sytax */
#define NEPTUNETP_ISO_14496_PART2_VIDEO       0x10            /* ISO/IEC 14496-2 Visual (MPEG-4) */
#define NEPTUNETP_ISO_14496_PART3_AUDIO       0x11            /* ISO/IEC 14496-3 Audio with LATM transport syntax */
#define NEPTUNETP_ISO_14496_PART10_VIDEO      0x1b            /* ISO/IEC 14496-10 Video (MPEG-4 part 10/AVC, aka H.264) */

// =========================================================================
//				Ring Buffer Index and Size
// =========================================================================
/* Note: we don't need to handle with wraparound issue
         if the size of INFO/TS ring buffer is a multiple of 
         INFO PACK LEN/TS PACKET LEN. 
*/
#define		NEPTUNETP_INFO_RING_BUFFER_SIZE		(NEPTUNETP_INFO_PACK_LEN*1024)
#define		NEPTUNETP_TS_RING_BUFFER_SIZE		(NEPTUNETP_MAX_TS_PACKET_LEN*2*1024)


#define		NEPTUNETP_TS_RING_BUFFER_INDEX			0
#define		NEPTUNETP_INFO_RING_BUFFER_INDEX		1


// =========================================================================
//				Section Filter Index and PID Filter Index
// =========================================================================
#define		NEPTUNETP_PAT_SECTION_FILTER_INDEX		0
#define		NEPTUNETP_PMT_SECTION_FILTER_INDEX		1
#define		NEPTUNETP_PAT_PID_FILTER_INDEX			0
#define		NEPTUNETP_PMT_PID_FILTER_INDEX			1
#define		NEPTUNETP_AUDIO_PID_FILTER_INDEX		2
#define		NEPTUNETP_VIDEO_PID_FILTER_INDEX		3
#define		NEPTUNETP_SPU_PID_FILTER_INDEX			4
#define		NEPTUNETP_PCR_PID_FILTER_INDEX			5


// =========================================================================
//				States and Commands for Input Plugin
// =========================================================================
#define		NeptuneTP_STATUS_AT_RUN_STATE			(1 << 0)
#define		NeptuneTP_STATUS_AT_PAUSE_STATE			(1 << 1)
#define		NeptuneTP_STATUS_AT_STOP_STATE			(1 << 2)
#define		NeptuneTP_STATUS_HAS_TP_ERROR			(1 << 3)
#define		NeptuneTP_STATUS_HAS_PMT_PID_FILTER		(1 << 4)
#define		NeptuneTP_STATUS_HAS_AUDIO_PID_FILTER	(1 << 5)
#define		NeptuneTP_STATUS_HAS_VIDEO_PID_FILTER	(1 << 6)
#define		NeptuneTP_STATUS_HAS_SPU_PID_FILTER		(1 << 7)
#define		NeptuneTP_STATUS_HAS_PCR_PID_FILTER		(1 << 8)
#define		NeptuneTP_STATUS_HAS_SET_FIRST_CC		(1 << 9)

class NeptuneTPRingBuf
{
public:
	NeptuneTPRingBuf(unsigned long size)
	{
		unsigned long phyAddr;
		unsigned char* pNonCachedVirAddr;
		unsigned char* pCachedVirAddr;
		
		
		pCachedVirAddr = (unsigned char *)pli_allocContinuousMemoryMesg("Ring", size, (unsigned char**)&pNonCachedVirAddr, &phyAddr); 
		m_base 				= phyAddr;
		m_size 				= size;
		m_limit 			= m_base + size;
		m_pCachedVirAddr	= pCachedVirAddr;
		
		m_addrDiff 			= phyAddr - (unsigned long)pNonCachedVirAddr;
		m_virReadPtr		= GetVirAddr(m_base);
		
	}
	
	virtual ~NeptuneTPRingBuf()	
	{ 
		
		pli_freeContinuousMemoryMesg("free", m_pCachedVirAddr); 
	}				 
	unsigned long GetVirAddr(unsigned long	phyAddr) 
	{
		return (phyAddr - m_addrDiff); 
	}
	
	unsigned long GetPhyAddr(unsigned long virAddr)
	{
		return (virAddr + m_addrDiff); 
	}
	
	unsigned long GetVirWritePtr(TP_CONTEXT* pTPInterface, char bufIndex)
	{
		unsigned long 		readPhyPtr, writePhyPtr;
		if(bufIndex == -1)
			TP_GetM2MRingBufferStatus(pTPInterface, (unsigned int*)&readPhyPtr, (unsigned int*)&writePhyPtr);
		else
			TP_GetRingBufferStatus(pTPInterface, bufIndex, (unsigned int*)&readPhyPtr, 
								  (unsigned int*)&writePhyPtr) ;
		return GetVirAddr(writePhyPtr);
    
	}
	unsigned long GetVirReadPtr()
	{
		return m_virReadPtr;
	}
	
	unsigned long GetVirBasePtr()
	{
		return GetVirAddr(m_base);
	}
	
	unsigned long GetVirLimitPtr()
	{
		return GetVirAddr(m_limit);
	}
	
	void	SetReadPtr(TP_CONTEXT* pTPInterface, char bufIndex, unsigned long virAddr)
	{
		unsigned long phyAddr = GetPhyAddr(virAddr);
		if(phyAddr == m_limit)
		{
			phyAddr = m_base;
			m_virReadPtr = GetVirBasePtr();
		}
		else
			m_virReadPtr = virAddr;
		assert(phyAddr >= m_base && phyAddr < m_limit);
		assert(m_virReadPtr >= GetVirBasePtr() && m_virReadPtr < GetVirLimitPtr());
		TP_UpdateRingBufferRP(pTPInterface, bufIndex, phyAddr);
	}
	
	unsigned long Read(TP_CONTEXT* pTPInterface, char bufIndex,
					   unsigned char** pOutData, int toReadSize)
	{
		unsigned long virBase = GetVirBasePtr();
		unsigned long virLimit = GetVirLimitPtr();
		unsigned long virWritePtr = GetVirWritePtr(pTPInterface, bufIndex);
		unsigned long newReadPtr;
		int canReadSize;
		canReadSize = (virWritePtr >= m_virReadPtr) ? ((int)(virWritePtr - m_virReadPtr))
						 					    	: ((int)(m_size + virWritePtr - m_virReadPtr));

		if(canReadSize < toReadSize)	return 0xFFFFFFFF;
		newReadPtr = m_virReadPtr + toReadSize;
		
		if(newReadPtr > virLimit)
		{
			// read wraparound 
			long	len = virLimit - m_virReadPtr ;
			
			memcpy(m_dataPiece, (unsigned char*)m_virReadPtr, len);
			memcpy(m_dataPiece+len, (unsigned char*)virBase, toReadSize-len);
			newReadPtr = virBase + toReadSize - len;
			*pOutData = m_dataPiece;
		}
		else
		{
			*pOutData = (unsigned char*)m_virReadPtr;
		}
		
		return newReadPtr;
	}
	
	// ******************************* member data *****************************
	unsigned long   m_base;				// physical address
	unsigned long	m_limit;
	unsigned long	m_size;				// if size = 0, this ring buffer entry is invalid
	unsigned long 	m_virReadPtr;		// virtual read pointer
	unsigned char*  m_pCachedVirAddr;	
	long            m_addrDiff;			// value = PhysicalAddress - VirtualAddress
	unsigned char	m_dataPiece[NEPTUNETP_MAX_TS_PACKET_LEN];	// an internal memory to handle wraparound
	
};

class NeptuneTP
{

	   
public:

	NeptuneTP(){}
	virtual ~NeptuneTP()
	{
		EnableFramer(false);
		for(int i = 0; i < NEPTUNETP_MAX_RING_BUF_NUM; i++)
		{
			if(m_pRingBufArr[i])
			{
				delete m_pRingBufArr[i];
				m_pRingBufArr[i] = NULL;
			}
		}
			
	}
		
	
	// input plugin initialization
    HRESULT Open(ENUM_MEDIA_TYPE majorType, ENUM_MEDIA_TYPE subType, osal_mutex_t* mutex, INPUTPLUGIN* pInputPlugin);

    // input plugin functions
    static HRESULT LoadMedia(void* pInstance, char* path, bool* pbContinue);
    static HRESULT UnloadMedia(void* pInstance);
    static HRESULT GetMediaInfo(void* pInstance, NAVMEDIAINFO* pMediaInfo);
    static HRESULT Read(void* pInstance, NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
    static HRESULT UpdatePlaybackPosition(void* pInstance, PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    static HRESULT PrivateDataFeedback(void* pInstance, unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
    static HRESULT ExecUserCmd(void* pInstance, NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
    static HRESULT PropertySetGet(void* pInstance, NAV_PROP_ID id, void* inData, unsigned int inDataSize,
                                  void* outData, unsigned int outDataSize, unsigned int* returnedSize);
    static HRESULT Dispose(void* pInstance);
    
    inline NeptuneTP_section_piece*  GetSecPiece(unsigned short PID);
    inline HRESULT ExtractAF(unsigned char* pTSAddr, int TSSize, unsigned char** pNewTSAddr, int* pNewTSSize);
    inline HRESULT ParsePES(bool bDiscardTS, int startIndicator, unsigned short PID, unsigned char* pData, int dataSize);
    inline HRESULT ParsePSI(bool bDiscardTS, unsigned short PID, unsigned char* pData, int dataSize);
	inline bool 	IsPES(unsigned short PID);
	inline HRESULT ReadOneTSPacket();
    inline HRESULT ParsePAT(unsigned char* pData, int dataSize);
    inline HRESULT ParsePMT(unsigned char* pData, int dataSize);
    inline HRESULT SetStreamDescriptor(unsigned char streamType);
    HRESULT ReadProcess (NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
    HRESULT EnableFramer(bool enable) 
    		{
#ifdef NeptuneTP_USE_M2M_MODE
    			TP_ConfigFramer(&m_TPInterface, enable, 	// enable framer
								0x47, 							// sync byte
								1, 								// dropno
								3, 								// lockno
								0,								// packet size (0: 188;  1: 192;  2: 204 bytes)
								0,								//data order (0: big-endian;  1: little-endian)
								0, 								// force drop all frames
								0x1f, 								// sync mode
								0, 								// 0: parallel input;  1: serial input
								0, 								// data pin (0: serial input in big-endian;  1: little-endian)
								0, 								// err_pol
								0, 								// sync_pol
								0, 								// val_pol
								0);								// clk_pol
#else
    			TP_ConfigFramer(&m_TPInterface, enable, 	// enable framer
								0x47, 							// sync byte
								1, 								// dropno
								3, 								// lockno
								0,								// packet size (0: 188;  1: 192;  2: 204 bytes)
								0,								//data order (0: big-endian;  1: little-endian)
								0, 								// force drop all frames
								0, 								// sync mode
								0, 								// 0: parallel input;  1: serial input
								0, 								// data pin (0: serial input in big-endian;  1: little-endian)
								0, 								// err_pol
								0, 								// sync_pol
								0, 								// val_pol
								0);								// clk_pol
#endif
				return S_OK;
			}
    // *************************** member data **********************************
    ENUM_MEDIA_TYPE m_majorType;          // input media type (major)
    ENUM_MEDIA_TYPE m_subType;            // input media type (sub)
    osal_mutex_t*   m_pIOMutex;           // mutex of outside filter
    
    TP_CONTEXT			m_TPInterface;		
	NeptuneTPRingBuf*	m_pM2MRingBuf;
	NeptuneTPRingBuf*   m_pRingBufArr[NEPTUNETP_MAX_RING_BUF_NUM];
	char				m_ringBufNum;		// number of ring buffer for this interface
	
	NeptuneTP_PMT_t			m_currPMT;
	NeptuneTP_PAT_t			m_currPAT;
	char 					m_totalPATSection;
	char					m_CC;
	
	NeptuneTP_PAT_entry_t*	m_pCurrProgram;	//(shared porinter) the currently playing back program
	long					m_statusFlag;
	
	pes_buf_list_t			m_PESList;
};

#endif

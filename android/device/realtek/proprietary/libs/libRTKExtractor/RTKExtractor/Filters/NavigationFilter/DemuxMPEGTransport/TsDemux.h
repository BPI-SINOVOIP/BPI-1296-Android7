#ifndef DEMUX_MPEG_TRANSPORT_H
#define DEMUX_MPEG_TRANSPORT_H

#include <hresult.h>
#include <Util.h>
#include <Filters/PrivateInfo.h>
#ifdef SUPPORT_NOT_CHECK_FEATURE
#include <Platform_Lib/TSDescrambler/TSDescrambler.h>
#endif /* end of SUPPORT_NOT_CHECK_FEATURE */
#include <Filters/NavigationFilter/NavPlugins.h>
#include "CDemuxBuf.h"

#define PES_HEADER_BUF_SIZE  14
#define NUM_OF_NAV_BUFFERS   64
#define HDMV_LPCM_AUDIO_DATA_HEADER_SIZE	4

#define DMX_LUMPSUM_DECLARE  int lumpsumLoopCount; int lumpsumErrCount; const char* lumpsumMsg[256];

class CDemuxMPEGTransport
{
	enum {
		TARGET_VIDEO,
		TARGET_AUDIO,    //active audio
		TARGET_SUBTITLE,
		TARGET_TELETEXT,
		TARGET_IG,
		TARGET_TEXTST,
		TARGET_AUDIO_INACTIVE,	//inactive audio MINIMIZE_AUDIO_MUTED_DURATION 
		TARGET_VIDEO_2ND,
		NUM_OF_TARGETS,
		
		TARGET_CHECK_SCRAMBLE = 0x3D,
		TARGET_FEEDBACK = 0x3E,
		TARGET_DROP     = 0x3F
	};
	
	enum { 
		EVENT_UPDATE_AUDIO_FORMAT 	  = 1 << 0,
		EVENT_DISCONTINUITY   	      = 1 << 1,
		EVENT_DROP_PACKET	          = 1 << 2,
		EVENT_AUDIO_CHECK_PES_PRIORITY = 1 << 3,
		EVENT_AUDIO_DATA_DISCONTINUITY = 1 << 4,      
	};
	
public:
	
	CDemuxMPEGTransport();
	
	HRESULT Open(
		ENUM_MEDIA_TYPE type,
		int             channelIndex,
		INPUTPLUGIN*    pInputPlugin,
		NAVDEMUXCONFIG* pDemuxConfig,
		DEMUXPLUGIN*    pDemuxPlugin);
		
	// demux plugin functions
	
	static HRESULT parse(
		void* pInstance,
		NAVDEMUXIN* pDemuxIn,
		NAVDEMUXOUT* pDemuxOut);
		
	static HRESULT handlePrivateInfo(
		void* pInstance,
		unsigned int infoId,
		unsigned char* infoData,
		unsigned int infoSize);
		
	static HRESULT flush(
		void* pInstance);
		
	static HRESULT propertySetGet(
		void* pInstance,
		NAV_PROP_ID id,
		void* inData,
		unsigned int inDataSize,
		void* outData,
		unsigned int outDataSize,
		unsigned int* returnedSize);
		
	static HRESULT dispose(
		void* pInstance);
		
protected:
	
	// reset state, drop any buffered stuff
	HRESULT Flush();
	
	// main parser entrance
	HRESULT Parse(NAVDEMUXIN* pDemuxIn, NAVDEMUXOUT* pDemuxOut);
	
	// parse PES header
	void ParsePESHeader(int target, unsigned char* pCurr, unsigned int len);
	
	// set demux target
	void SetDemuxTarget(
		unsigned char* infoData,
		unsigned int infoSize);
		
	// update audio format information via private info
	void UpdateAudioFormat(int codecType, int sourceType, ENUM_MEDIA_TYPE audioPreference = MEDIATYPE_None);
	
	// pack private info into NAVBUF
	void PackPrivateInfo(unsigned int infoId, void* infoData, unsigned int infoSize, NAVBUF* pBuffer);
	
	
	void CheckScrambling(unsigned char* pData);
	
	void ParseHDMVLPCMHdr(int target, unsigned int headerLen, unsigned char* pCurr);
	
	
	void DemuxOutPacket(int target, NAVDEMUXOUT* pDemuxOut);			
	
	unsigned char GetStreamIDExtension(unsigned char* pCurr, unsigned int len);
	
	void ParseGraphicStreamPacket(int target, int pid,unsigned char* pCurr);
	bool ParsePacket(/*in*/ int target, unsigned char* pPacketStart,
					/*out*/NAVDEMUXOUT* pDemuxOut);
	HRESULT ConsumeInternalAudioBuffer(NAVDEMUXOUT* pDemuxOut);
	int ResyncStream(unsigned char* pData, unsigned int bytes);
#if defined(REALTEK_VUDU) || defined(REALTEK_WFDISPLAY)	
	HRESULT SetDescrambler();
#endif

#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
	void CheckNewVideo(int target);
#endif	
#ifdef ENABLE_CHECK_CONTINUITY_COUNTER
	void CatLog(int target, unsigned char* pPacketStart);
#endif
protected:
	
	// reference to input plugin upstream
	INPUTPLUGIN* m_pInputPlugin;
	
	// transport stream packet size (default = 188)
	int m_tsPacketSize;
	
	// private info for audio format update
	AUDIOFORMAT m_audioFormat;
	
	// nav buffer for demux-out
	NAVBUF m_navBuffers[NUM_OF_NAV_BUFFERS];
	
	// demux target table (one unsigned char for each possible value of the 13 bits PID)
	unsigned char m_demuxTargetTable[1 << 13];
	
	// demux target info
	struct {
		int pinIndex;
		int events;
		int contiCounter;
		bool bKeepPES;
		bool bCheckContiCounter;
		bool bResyncStartUnit;
		bool bResyncPTS;	//for bug 13648
		int64_t pts;
	#ifndef DONT_DROP_HDMV_AUDIO_DATA_HEADER 
		unsigned char pesHeaderBuf[PES_HEADER_BUF_SIZE + HDMV_LPCM_AUDIO_DATA_HEADER_SIZE];
	#else        
		unsigned char pesHeaderBuf[PES_HEADER_BUF_SIZE];
	#endif        
		unsigned int bufferedBytes;
		unsigned int remainingHeaderBytes;
		
	} m_demuxTargetInfo[NUM_OF_TARGETS];
	
	// PID where demux can find PCR and feedback to navigator (-1 means demux is not requested to do so)
	int m_PCRPID;

#ifndef DONTFIXBUG13912    
	/*enum PES_PAYLOAD_PRIORITY {
		PES_PAYLOAD_PRIORITY_0 = 0,
		PES_PAYLOAD_PRIORITY_1 = 1,
		PES_PAYLOAD_PRIORITY_AUTO_DETECT = 3,	// choose the pes_priority of the first audio packet as our preference
		PES_PAYLOAD_PRIORITY_DONT_CARE = 4,
		
	}  m_pesPriority;*/
    
	unsigned char m_streamIDExt;
#endif

#if 1	// fixbug 18043
	int64_t	m_demuxStartPts;
	bool	m_checkStartPts;
#endif

	int		m_HDMV_LPCM_frameSize;		//DONTFIXBUG21129
	int		m_audioDataSizeBetweenHDMVLPCMHdr;		//DONTFIXBUG21129
	
	/********** ifdef MINIMIZE_AUDIO_MUTED_DURATION *************/
	int				m_channelIndex;
	int				m_videoFullCounter;
	int				m_audioFullCounter;
	long 			m_videoBufFreeSpace;
	long 			m_audioBufFreeSpace;
	BS_BUF_INFO 	m_audioPacketBuf;
	CMD_BUF_INFO	m_audioCmdBuf;
	/********** endif MINIMIZE_AUDIO_MUTED_DURATION *************/
	
	bool			  m_bValidIV;
	bool			  m_bValidKey;
#ifdef SUPPORT_NOT_CHECK_FEATURE
	DESCRAMBLE_CONFIG m_decConf;
	TS_DESCRAMBLER*   m_pDes;
#endif /* end of SUPPORT_NOT_CHECK_FEATURE */
	NAVIOBUFMAPINFO   m_inputBufInfo;
	
	bool             m_bSkipLPCMHdr;
	
	DMX_LUMPSUM_DECLARE;
};

#endif //DEMUX_MPEG_TRANSPORT_H

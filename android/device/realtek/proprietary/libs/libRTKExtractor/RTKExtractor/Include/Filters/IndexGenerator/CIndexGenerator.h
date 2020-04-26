
#ifndef __C_INDEX_GENERATOR_H__
#define __C_INDEX_GENERATOR_H__


#include <OSAL.h>
#include "CStream.h"
#include <Filters/rtr_vmgi_super.h>
#include <Filters/IndexGenerator/CWriterInputParserPin.h>


typedef ulDArray<PAYLOADINFO*>	PARSE_PAYLOAD_INFO_ARRAY;
#define _ARRAY_SIZE	1024*2

class CIndexGenerator: public CBaseFilter
{
	friend class CWriterInputDemuxPin;
	
public:
	CIndexGenerator(void);		
	
	CSUP_RTR_VMGI 				m_VMGIobj;
	CThread* 	         		m_pThread;	
    CWriterInputParserPin*      m_pInputParserPin;
    CWriterInputDemuxPin*		m_pInputDemuxPin;
    
    
    HRESULT					Run(void);												
	HRESULT					Stop(void);												
	HRESULT					Pause(FILTER_STATE State);		
	
	void 					SetFileName(const char* path);

protected:

	CCritSec					m_Mutex;
	PARSE_PAYLOAD_INFO_ARRAY 	m_PackAddrArr;
	
	
private:
	const char* m_IndexFileName;
	
	inline void DeliverAttrInfo(PARSE_STREAM_ATTR_INFO* attrInfo);
	inline void DeliverVOBUInfo(PARSE_STREAM_VOBU_INFO_PACKET* VOBUInfo, int* fieldCount);
	inline void DeliverVOBInfo(PARSE_STREAM_VOB_INFO* VOBInfo, int* fieldCount, int frameDuration);
	inline unsigned long QueryPackAddr(unsigned long key, bool WantStartAddr);
	inline void  ReadParseInfo(long reqSize, /*out*/BYTE* info);
    static void ThreadEntry(void* Data);
};

#endif


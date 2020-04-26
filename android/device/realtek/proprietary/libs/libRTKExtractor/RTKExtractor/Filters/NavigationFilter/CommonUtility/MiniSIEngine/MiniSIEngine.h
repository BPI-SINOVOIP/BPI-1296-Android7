#ifndef MINI_SI_ENGINE_H
#define MINI_SI_ENGINE_H

#include "TsPacket.h"
#include "MiniSIEngineInterface.h"

class MiniSIEngine
{
public:

	MiniSIEngine(MINI_SI_ENGINE_OUT* pSIOut, int packetSize);
	~MiniSIEngine();
	
	void UpdatePacketSize(int packetSize);
	int Parse(const unsigned char* pData, int dataLen);

	int ParseTSP(const unsigned char* pTsp, TS_PACKET_INFO* pInfo);   
	int ParseSession(const unsigned char* pSec, unsigned short len, TS_SESSION_INFO* pInfo);
	int ParsePAT(const unsigned char* pData, unsigned short len, PSI_PAT* pInfo);   
	int ParsePMT(const unsigned char* pData, unsigned short len, PSI_PMT* pInfo);            
	
	
	
protected:
	MINI_SI_ENGINE_OUT m_siOut;
	int m_packetSize;
	unsigned short m_ProgramID;         
    unsigned short m_PmtPID;
    unsigned char m_PATVersion;   
    PSI_PAT       m_PAT;
    unsigned char m_PMTVersion;
    PSI_PMT       m_PMT;    
} ;

#endif //MINI_SI_ENGINE_H

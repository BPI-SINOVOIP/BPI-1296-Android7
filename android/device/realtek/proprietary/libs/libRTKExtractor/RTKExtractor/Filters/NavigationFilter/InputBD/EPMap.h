#ifndef __C_BD_EPMAP_H__
#define __C_BD_EPMAP_H__
#include <Util.h>
#include "clpi_parse.h"

class EPMap
{
public:
	EPMap() 
	: m_pEPmapEntry(0)
	, m_epCoarseNo(0)
	, m_epFineNo(0)
	, m_epMapSPN(0)
	, m_epMapPTS(0) {}
	
	~EPMap() {}
	
	inline void SetEPMapEntry(bdt_ep_map_entry_t* pEPmapEntry ) { m_pEPmapEntry = pEPmapEntry; }
	inline void ResetEPMap() 
			{
				m_pEPmapEntry = 0;
				m_epCoarseNo = m_epFineNo = 0; 
				m_epMapSPN = 0;
				m_epMapPTS = 0;
			}
	bool SearchSPNFromPTS(int64_t targetPTS, int64_t* pPTS, int* pSPN);	
	bool ForwardSearchSPNFromPTS(int64_t targetPTS, int64_t* pPTS, int* pSPN);
	bool BackwardSearchSPNFromPTS(int64_t targetPTS, int64_t* pPTS, int* pSPN);

protected:
	bdt_ep_map_entry_t* m_pEPmapEntry;
	int 				m_epCoarseNo;
	int 				m_epFineNo;
	int					m_epMapSPN;
	int64_t				m_epMapPTS;
	
};
#endif//__C_BD_EPMAP_H__

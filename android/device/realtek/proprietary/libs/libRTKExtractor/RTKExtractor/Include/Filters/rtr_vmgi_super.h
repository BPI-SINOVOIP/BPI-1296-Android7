// RTR_VMGI.h: interface for the CRTR_VMGI class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CSUP_RTR_VMGI_H_INCLUDED
#define __CSUP_RTR_VMGI_H_INCLUDED

#include <CStream.h>
#include <io/dvr_super_ifo_types.h>
#include <io/dvr_super_ifo_access.h>
#include "NavigationFilter/NavDef.h"
#include "Filters/PrivateInfo.h"

#include "VideoDecoder/StreamParseInfo.h"

#define IN
#define OUT
#define byte unsigned char


//===============================================================
class CSUP_RTR_VMGI : public CObject
{
public:
	dvr_super_ifo_handle_t*	m_pIfoHandle;

	CSUP_RTR_VMGI();
	virtual ~CSUP_RTR_VMGI();

	int				Open(IN const char *lpctstrDVDPath, byte byExtendIFO = 0);
	int				Create(IN const char *lpctstrDVDPath); // create a new index file
	int				Write();	// write parsing info to file
	void			Close();	// close file	
//	int		Initialize(const char *szDVDPath, int nExtendIFO = 0);

//	char*			GetDVDPath();
//	dvd_reader_t*	GetDVDReader()	{ return m_pDvdReader; }
	
	int		GetTimePosition(uint64_t i64FrameNo, uint64_t *pStartByte, int *pGopNo, int *pOffsetFrameNo);
	int		Dump(); //neco+
	//dvr_super_ifo_handle_t* GetIfoHandle()  {	return m_pIfoHandle; } //neco+	
		
	int  	StartStreamInfo(PARSE_STREAM_ATTR_INFO strmAttrInfo);
	int  	AddVobuInfo(PARSE_STREAM_VOBU_INFO strmVobuInfo);
	int  	EndVobInfo(PARSE_STREAM_VOB_INFO strmVobInfo);

private:
//	dvd_reader_t*		m_pDvdReader;
	int					m_fhIdx;
//	dvr_ifo_handle_t*	m_pIfoHandle;
	byte				m_byExtIFOType;		// neco+

	int		xSuper_MakeTM_ENT();
	int		xGetTMU();
	int		xSuper_Add_OrgPGC();
	int		xAdd_Org_M_EP(int nCellNo);	
	int		xAdd_M_VOB_STI(super_m_vob_sti_t *pM_VOB_STI);

};

#endif // __CRTR_VMGI_H_INCLUDED

// Static Model

// 

#ifndef __Top_Package__CVIDEOAuthoring__
#define __Top_Package__CVIDEOAuthoring__

// Include files
#include <Filters/CDVDAuthoring.h>
#include <Filters/CVRMI.h>
#include <StreamClass/CPin.h>
#include <Filters/CDVDProgMux.h>

class CVIDEOAuthoring	: public CDVDAuthoring
{

public:
	CVIDEOAuthoring(const char *pDVDPath);
	~CVIDEOAuthoring();

	HRESULT Pause(FILTER_STATE State);

	HRESULT ProcessPrivateInfo(BYTE *pInfo);

	HRESULT getTotalTitle(OUT int *Tnum);
	HRESULT getRecordInfo(IN int Tnum,
			      OUT uint32_t * begin_lbn,
			      OUT uint32_t * end_lbn,
			      OUT bool * isProtect); 
	HRESULT getRecordStartAddr(IN int Tnum, OUT uint32_t *begin_lbn);
	HRESULT SetRecordInfo(IN OUT int *Tnum);
	HRESULT CanRecord(OUT bool *value);
	HRESULT deleteTitle(IN int title);
	HRESULT Realize();
	HRESULT Initialize(const char *lpctstrDVDPath);
	HRESULT Open(const char *lpctstrDVDPath, OUT RW_OPEN_ERROR_CODE *err=NULL);
	HRESULT close();
	HRESULT Rename(IN int title, 
		       IN char *name,  IN int size,
		       IN char *altname, IN int altsize, IN char alt_charset_code);
	HRESULT getLastRecordInfo(OUT uint32_t * begin_lbn, 
				  OUT uint32_t * end_lbn,
				  OUT bool * isProtect);

	HRESULT protectTitle(IN int title, IN bool isProtect);
	HRESULT GetFreeCellID(unsigned int* pCellID);

	HRESULT SetMANID(unsigned int size,
			 unsigned char* pMANID);
      
	HRESULT RealizeVTSI();
	HRESULT getVobFilebyAddr(IN int title, IN int32_t keyFrameAddr, OUT int *vtsn, OUT int *vtsttn, OUT uint32_t *modifyAddr);
        HRESULT DeleteAllTitles();
	
protected:
private:
	int m_newTitleVTSnum;
	int m_newTitleVTSTTnum;
	uint32_t m_startLBA;
	uint32_t m_endLBA;
	int m_firstCell;
	int m_totalTiltes;
	int m_startVRCHP;
	uint32_t m_titleSize;
	unsigned int m_RecSpace;
	unsigned int m_totalChapters;
	unsigned int m_totalCells;

	unsigned int m_VobStartLBA;
	unsigned int m_VobuStartLBA;
	unsigned int m_TitleStartPTM;
private:
	void init();
};

// END CLASS DEFINITION CRWAuthoring

#endif // __Top_Package__CVIDEOAuthoring__



// VRAuthoring.h: interface for the CVRAuthoring class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CVRAUTHORING_H_INCLUDED__
#define __CVRAUTHORING_H_INCLUDED__


//#include "system.h"
//#include "SType.h"
//#include "EType.h"
//#include "ulDArray.h"
//#include "CMediaType.h"
//#include "CMediaSample.h"
#include "OSAL.h"
//#include "CPin.h"
#include <CStream.h>
#include "IBaseAuthoring.h"
#include <Filters/rtr_vmgi.h>
#include <Filters/CDVDProgMux.h>
#include <sys/vfs.h>
#include <Utility/ResourceMgr/ioResource.h>

#define IN
#define OUT

#define	LP_AUTO_CHAPTER_VOBU_NS	4
#define	LP_TOTAL_VOB_NS			30
#define VR_WRITE_FAIL_MAX		3//100
#define VR_INFO_QUEUE_LENGTH    50
#define VR_INFO_SIZE            128

class CVRAuthoringInputPin;
class CVRAuthoringInfoPin;

class CVRAuthoring :/*public IBaseAuthoring,*/ public CBaseFilter
{
	friend class CVRAuthoringInputPin;
	friend class CVRAuthoringInfoPin;
	
protected:
	CCritSec				m_csSemInput;	
	
private:
	CVRAuthoringInputPin*	m_pInputPin;
	CVRAuthoringInfoPin*	m_pInfoPin;
	CThread*                m_pThread;
    CThread*                m_pInfoThread;
	IMemAllocator*          m_pAllocator;
	VoidArrSem              m_DataQueue;
//	CCritSec				m_csSemInput;
    osal_sem_t      m_SemInfo;

	CRTR_VMGI*		m_pRtrVmgi;
//	dvd_file_t*		m_pMovieFile;
//	dvd_file_t*		m_pStillFile;
//	dvd_file_t*		m_pAudioFile;

//	bool			m_bDoNotFreeRtrVmgi;
//	int				m_nRecTitleNo;
	int				m_nRecSTINo;
	bool			m_bIsFirstVOBU;
	int				m_nRecMVOBNo;
	unsigned int	m_nRecFlag;
//	char* 			m_pszPath;  
	unsigned int 	m_nFreeSz;  
	unsigned int	m_nMVRO_SZ;	  // = 0 when get GenInfo (start Encode), for checking VRO and IFO match or not when stop encode
	unsigned int	m_nMVRO_Real_SZ;  
	// resource manage
	RESOURCE_NAME      m_ioDevice;

    int             m_nInfoRd;
    int             m_nInfoWr;
    char            m_InfoQueue[VR_INFO_QUEUE_LENGTH][VR_INFO_SIZE];
public:
	CVRAuthoring();
	virtual ~CVRAuthoring();

	HRESULT Stop();
	HRESULT Pause(FILTER_STATE State);
	HRESULT Run();

	HRESULT Initialize(IN const char *lpctstrDVDPath, int *pnOpenError=NULL/*, bool bIsLivePause=false*/);
	HRESULT Open(IN const char *lpctstrDVDPath, int *pnOpenError=NULL/*, bool bIsLivePause = false*/);
	HRESULT Realize();
	HRESULT Close();
	HRESULT Finalize();
	void	Serialize();
	void	Deserialize();
	HRESULT GetDVDPath(char **szDVDPath);
	HRESULT SetRtrVmgi(CRTR_VMGI *pRtrVmgi);
//	HRESULT SetDVDFile(dvd_file_t *pDVDFile);
	HRESULT CanRecord(bool *bCanRec);
	
//	bool	CheckFreeSpace()	{ return xCheckFreeSpace(); }

	HRESULT StartRecording(unsigned int nFlag=0);
	HRESULT StopRecording();
	HRESULT AddStreamInfo(PROG_STREAM_GEN_INFO oStreamInfo);
	HRESULT Add_M_VOB(unsigned int nAddrOffset);
	HRESULT Update_M_VOB(unsigned int nStartPTM, unsigned int nEndPTM);
	HRESULT Add_TM_ENT(unsigned int nVOBUNs, unsigned int nTimeDiff, unsigned int nVOBUAddr, bool bIsExactVOBUNo=false); // neco*
	HRESULT Add_VOBU_ENT(unsigned int n1stRefSize, unsigned int nVOBUPBTime, unsigned int nVOBUSize);

	HRESULT GetPrimaryTextCHRS(byte *pbyCHRS);
	HRESULT SetPrimaryTextCHRS(byte byCHRS);
#ifdef _DEBUG
//	HRESULT DumpIfo();  
	HRESULT DumpIfo(char *command);  
#endif
	HRESULT GetCellType(int nPGCNo, int nCellNo, byte *pbyCellType);
	HRESULT GetTVSystem(byte *pbyTVS); 
	HRESULT ABDelVROFile(byte byType, int nStartSector, int nEndSector);
	HRESULT GetIfoHandle(dvr_ifo_handle_t** pIfoHandle);
	
    // disc info.
	HRESULT GetDiscStillTime(int *pnStillTime);
	HRESULT SetDiscStillTime(int nStillTime);
	HRESULT GetDiscRepName(char *sz1stName, int n1stMaxSize, int *pn1stActualSize,
			char *sz2ndName, int n2ndMaxSize, int *pn2ndActualSize);
	HRESULT SetDiscRepName(const char *lpct1stName, const char *lpct2ndName);
	HRESULT GetDiscRepPict(int *pnPGNo, int *pnCellNo, byte *pbyCellType, int *pnFrame, dtdf_t *pCreateTime);
	HRESULT SetDiscRepPict(int nPGNo, int nCellNo, int nFrame);
    
    // Original program info.                           
	HRESULT GetOrgPGNs(int *pnPGNs);
	HRESULT GetOrgPGCellNs(int nPGNo, int *pnCellNs);
	HRESULT GetOrgPGName(int nPGNo,
			char *sz1stName, int n1stMaxSize, int *pn1stActualSize,
			char *sz2ndName, int n2ndMaxSize, int *pn2ndActualSize);
	HRESULT SetOrgPGName(int nPGNo, const char *lpct1stName, const char *lpct2ndName);
	HRESULT GetOrgPGRepPict(int nPGNo, int *pnCellNo, byte *pbyCellType, int *pnFrame);
	HRESULT SetOrgPGRepPict(int nPGNo, int nCellNo, int nFrame);
	HRESULT GetOrgPGEPNs(int nPGNo, int *pnEPNs);
//	HRESULT GetOrgPGLength(int nPGNo, ptm_t *pLength);
	HRESULT GetOrgPGLength(int nPGNo, int *nHour, int *nMin, int *nSec, int *nFrame);
	HRESULT GetOrgPGRecDateTime(int nPGNo, dtdf_t *pRecTime);
	HRESULT GetOrgPGStartSector(int nPGNo, int *pnStartSector);
	HRESULT GetOrgPGEP(int nPGNo, int nEPNo, int *pnCellNo, byte *pbyCellType, int *pnFrame, char **ppszPrimaryText); 
	HRESULT AddOrgPGEP(int nCellNo, int nFrame); 
	HRESULT DelOrgPGEP(int nPGNo, int nEPNo); 
	HRESULT DelPG(int nPGNo, bool bIsTE);  
	HRESULT	ABDelPG(int nPGNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, int *pnErrorNo=NULL); 
	HRESULT CopyPG(int nPGNo, const char* lpctstrDstDVDPath); 
	HRESULT	CombinePG(int nStartPGNo, int nCmbPGNs); 
	HRESULT	DividePG(int nPGNo, int nDivCellNo, int nFrameNo);  
	
    // Playlist info.
	HRESULT GetPLNs(int *pnPLNs);
//	HRESULT GetPLLength(int nPLNo, ptm_t *pLength);
	HRESULT GetPLLength(int nPLNo, int *nHour, int *nMin, int *nSec, int *nFrame);
	HRESULT GetPLCreatingDateTime(int nPLNo, dtdf_t *pCreateTime);
	HRESULT GetPLName(int nPLNo,
			char *sz1stName, int n1stMaxSize, int *pn1stActualSize,
			char *sz2ndName, int n2ndMaxSize, int *pn2ndActualSize);
	HRESULT SetPLName(int nPLNo, const char *lpct1stName, const char *lpct2ndName);
	HRESULT GetPLRepPict(int nPLNo, int *pnCellNo, byte *pbyCellType, int *pnFrame);
	HRESULT SetPLRepPict(int nPLNo, int nCellNo, int nFrame);
	HRESULT GetPLEPNs(int nPLNo, int *pnEPNs);
	HRESULT GetPLEP(int nPLNo, int nEPNo, int *pnCellNo, byte *pbyCellType, int *pnFrame, char **ppszPrimaryText); 
//	HRESULT GetPLEP(int nPLNo, int nEPNo, int *pnCellNo, byte *pbyCellType, int *pnCellFrame, int *pnPLFrame); 
	HRESULT AddPLEP(int nPLNo, int nCellNo, int nFrame); 
	HRESULT DelPLEP(int nPLNo, int nEPNo); 
	HRESULT DelPL(int nPLNo); 
	HRESULT CopyPL(int nSrcPLNo, int nNewPLNo); 	
	HRESULT DelPLScene(int nPLNo, int nEPNo); 
	HRESULT ABDelPL(int nPLNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame); 
	HRESULT	MovePL(int nSrcPLNo, int nDstPLNo); 
	HRESULT	MovePLScene(int nPLNo, int nSrcEPNo, int nDstEPNo); 
//	HRESULT	AddPLScene(int nPLNo, int nSceneNs, byte *byPGLType,  int *pnPGLNo, int *pnStartCellNo, int *pnStartFrame, int *pnEndCellNo, int *pnEndFrameNo); 
	HRESULT AddPLScene(int nPLNo, int nSceneNs, byte *byPGLType,  int *pnStartPGLNo, int *pnStartCellNo, int *pnStartFrame, int *pnEndPGLNo, int *pnEndCellNo, int *pnEndFrameNo); 
//	HRESULT	CreatePLScene(int nNewPLNo, int nSceneNs, byte *byPGLType,  int *pnPGLNo, int *pnStartCellNo, int *pnStartFrame, int *pnEndCellNo, int *pnEndFrameNo); 
	HRESULT	CreatePLScene(int nNewPLNo, int nSceneNs, byte *byPGLType,  int *pnStartPGLNo, int *pnStartCellNo, int *pnStartFrame, int *pnEndPGLNo, int *pnEndCellNo, int *pnEndFrameNo); 
	HRESULT	CombinePL(int nPLNo, int nCmbPLNo); 
	HRESULT	DividePL(int nPLNo, int nDivCellNo, int nFrameNo); 


	HRESULT GetOrgPGStartCellNo(int nPGNo, int *pnCellNo)	{ *pnCellNo = m_pRtrVmgi->GetOrgPGStartCellNo(nPGNo); return S_OK; }
	HRESULT	GetPbInfoFromPGLPbTime(byte byPGLType, int nPGLNo, int nPbTimeFrameNo, byte *byCellType, int *nCellNo, int *nFrameNo); 
	HRESULT	Set_RSM_MRKI(int nPGCNo, int nPGNo, int nCellNo, int nFrameNo); 
	HRESULT	SetRecordingFlag(unsigned int nFlag, unsigned int nParameter1, unsigned int nParameter2); 
	HRESULT SetAutoChapter(/*bool bIsAutoChapter, */unsigned int nAutoMVOBUNs); 
	HRESULT	SetLivePause(bool bIsLivePause, unsigned int nTotalMVOBNs, unsigned int nAutoMVOBUNs, unsigned int nCellBlocks = 0 /*nBitrate=HQ_PEAKBITRATE*/); 
	HRESULT GetPGFrameNo(int nPGNo, int nCellNo, int nFrameNo, int *pnPGFrameNo); 

	bool	CheckFreeSpace(int *pnFreeBlks = NULL); 
	HRESULT GetOrgPGAddr(int nPGNo, uint32_t *pi32StartSector, uint32_t *pi32Length); 
	HRESULT Check_PG_AB_Point(int nPGNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame); 
	HRESULT Check_PG_Split_Point(int nPGNo, int nDivCellNo, int nFrameNo);

	HRESULT GetFPS(int *pnFPS); 
	static HRESULT CreateMmapFile(const char *lpctstrDVDPath);
	HRESULT CanPlayAtPGTime(int nPGNo, int64_t i64PbSeconds, int64_t i64PbFrameNs, VR_CAN_PB_AT_TIME_ERRNO *pErrno);
	HRESULT GetLivePauseTotalLength(int *pSec, int *pFrame);
	void DropAllData();
	uint32_t CheckVROoffsetGOPBnd(uint32_t i32Offset);

	CRTR_VMGI* Get_RtrVmgi() { return m_pRtrVmgi; }
	HRESULT GetOrgPGStartEndCellNo(int nPGNo, int *pnStartCellNo, int *pnEndCellNo);
	HRESULT GetOrgPGDiscontinuousAddr(int nPGNo, int *pnFileNs, uint32_t *pi32StartSector, uint32_t *pi32Length);
	HRESULT GetFrameFromSec(int64_t Sec, int64_t *pnFrame);
	int64_t	GetFrameInterval()	{return ((!m_pRtrVmgi) ? 0 : m_pRtrVmgi->GetFrameInterval());}
	HRESULT GetOrgPGProtectState(int nPGNo, bool *pbIsProtect);
	HRESULT SetOrgPGProtectState(int nPGNo, bool bIsProtect);
	HRESULT GetIsNewRecPG(int nPGNo, bool *pbIsNewPG); 
//	HRESULT SetIsNewPG(int nPGNo, bool bIsNewPG); 
	HRESULT GetTitleChildLock(int nPGNo, bool *pbIsChildLock);
	HRESULT SetTitleChildLock(int nPGNo, bool bIsChildLock);
	HRESULT GetTitleEditStatus(int nPGNo, bool *pbIsEditStatus);
	HRESULT SetTitleEditStatus(int nPGNo, bool bIsEditStatus);

//#ifndef NDEBUG
	virtual	HRESULT	ExecuteCommandLine(char* command); 
//#endif

protected:
	bool		ThreadWriteFile(void);
	static void 	FileWriteThread(void* pData);

	bool		ThreadProcessInfo(void);
	static void 	ProcessInfoThread(void* pData);

//	unsigned long m_nTotalDataSize;
	unsigned long m_nTotalWriteDataSize;
	unsigned long m_nTotalVobuInfoSize;	// vobu real address of VRO, for build VOB
//=== neco+ ===================================================
        unsigned int    m_nOrigAutoMVOBUNs;
	unsigned int	m_nAutoMVOBUNs;	
	bool			m_bIsAutoChapter;
	bool			m_bIsLivePause;	
	unsigned int	m_nTMU;
	unsigned int	m_nTMCnt;
	unsigned long	m_nVOBU_ADR; // for build TM_ENT, = 0 when VOB beginning
	int				m_nRecTitleNo;
	unsigned int	m_nVOBUCnt;
//	unsigned int	m_nVOBCnt
	unsigned int	m_nVOBUNs;
	unsigned int	m_nTimeDiff;
	unsigned int	m_nVOBStartPTM;
	unsigned int	m_nLastVOBEndPTM;
	unsigned int	m_nLPTotalMVOBNs;		 
	unsigned int	m_nLP_Last_MVOB_sz;		// Possibly Max size of VRO (counting by the total MVOB numbers & Max size of MVOB)
	unsigned int	m_nLP_MVRO_file_prt;	// Real VRO size (counting when write ptr for LivePause)
	unsigned int	m_nLP_MAX_VOB_SZ;		// DVR_MAX_VOBU_SZ * nAutoMVOBUNs
	bool 			m_bIsSetKeyFrame;
	int				m_nLPDelVROCount;
	int				m_nLPDelIFOCount;
	//unsigned int 	m_nMaxVOBUsz;
	int				m_nWriteFailCount;
	bool			m_bIsDropData;
//=============================================================

private:
	void	xProcessPrivateInfo(byte *pInfo, int nLength);
	void	xInit();
	HRESULT xREP_NAME_2_MultiFieldString(rep_nm_t oRepName,
			char *sz1stName, int n1stMaxSize, int *pn1stActualSize,
			char *sz2ndName, int n2ndMaxSize, int *pn2ndActualSize);
//	bool	xCheckFreeSpace();
	HRESULT xEndOfStream();
	void	xLivePause(); 
    void    xEnInfoQueue(byte *pInfo, int nLength);
    bool    xDeInfoQueue(byte **pInfo, int *nLength);
};

class CVRAuthoringInputPin : public CBaseInputPin
{
	friend class CVRAuthoring;

public:
	CVRAuthoringInputPin(const char *pId);
	~CVRAuthoringInputPin();

	HRESULT Receive(IMediaSample* pSample);
	HRESULT ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pMediaType);

	HRESULT QueryAccept(const MEDIA_TYPE *pMediaType);
	HRESULT PrivateInfo(byte *pInfo, int nLength);

	HRESULT	Disconnect(void);
};


class CVRAuthoringInfoPin : public CBaseInputPin
{
	friend class CVRAuthoring;

public:
	CVRAuthoringInfoPin(const char *pId);
	~CVRAuthoringInfoPin();

	HRESULT QueryAccept(const MEDIA_TYPE *pMediaType)	{ return S_OK; }
	HRESULT PrivateInfo(byte *pInfo, int nLength);
	HRESULT	Disconnect(void);
};

#endif // __CVRAUTHORING_H_INCLUDED__

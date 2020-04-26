// RTR_VMGI.h: interface for the CRTR_VMGI class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CRTR_VMGI_H_INCLUDED
#define __CRTR_VMGI_H_INCLUDED

#include <CStream.h>
//#include <io/dvr_ifo_types.h>
#include <io/dvr_ifo_access.h>
#include "NavigationFilter/NavDef.h"
#include <Filters/PrivateInfo.h>
#include <Filters/vr_events.h>

#define IN
#define OUT
#define byte unsigned char
/*
#ifndef PATH_MAX
#define PATH_MAX	256
#endif
*/

#define DVR_UDF_RW 		1
//#define _READ_CACHE_BLK 
#define _MULTICHANNEL	

#define DISC_KEY_FRAME_CHECK 0
#define PG_KEY_FRAME_CHECK 1
#define	AUTO_CHAPTER_VOBU_NS	DVR_MAX_MVOBU_EXT  // 14400 (dvr_ifo_types.h)

#define TIMEPOSQ_SIZE	64
#define TIMEPOSQ_SIZE_FOR_REVERSE_SMOOTH	64*4

#define BLKS_PER_READ	32
#define TIMESHIFT_SAFE_RANGE		12
#define	TIMESHIFT_SAFE_RANGE_HIGH_BR	4
#define	TIMESHIFT_SAFE_RANGE_LOW_BR		6
#define TIMESHIFT_CATCH_DELAY_TIME		2
#define	DVR_MAX_READ_TIMES	1
#define	DVR_MAX_WRITE_TIMES	3

typedef enum {
	FILTER_STATE_RUN,
	FILTER_STATE_PAUSE,
	FILTER_STATE_STOP
} VR_FILTER_STATE;

typedef enum {
	NAV_STATE_PLAY,
	NAV_STATE_PGMENU,
	NAV_STATE_PLMENU,
	
	NAV_STATE_WAITFORPLAYTOSTILLEND,
	NAV_STATE_WAITFORPLAYTOPGEND,
	NAV_STATE_WAITFORCOMPLETE,
	NAV_STATE_PLAYEDTOPGEND
} VR_NAV_STATE;

typedef enum {
	RPT_FLAG_NOREPEAT,
	RPT_FLAG_TITLE,
	RPT_FLAG_CHAP,
	RPT_FLAG_AB
} VR_REPEAT_FLAG;

typedef enum {
	CAN_NOT_PLAY,
	PAST_FRAME,
	FUTURE_FRAME
} VR_CAN_PB_AT_TIME_ERRNO;

// for loading protection error number
// neco+
#define	LP_ERROR_IFO_OPEN_FAIL		0
#define	LP_ERROR_VRO_SIZE_TOO_BIG	2
#define	LP_ERROR_CHECK_CELL_ERROR	3
#define	LP_ERROR_VRO_SIZE_TOO_SMALL	4
#define	LP_ERROR_KEY_FRAME_RESET	5

//=================================================
// ErrorNo for AB Cut
#define DVR_ABCUT_ERROR_TITLE_PROTECT	5
#define DVR_ABCUT_ERROR_AB_TOO_CLOSE	4
#define DVR_ABCUT_ERROR_CELL_FULL		3
#define DVR_ABCUT_ERROR_PL_EP_FULL		2
#define DVR_ABCUT_ERROR_FAIL			0
//#define DVR_ABCUT_SUCCESS				1

// ErrorNo for open
#define DVR_OPEN_ERROR_DVDVR_NOT_EXIST	4
#define DVR_OPEN_ERROR_IFO_OPEN_FAIL	3
#define DVR_OPEN_ERROR_VRO_NOT_EXIST	2

//=================================================
typedef struct {
	int			nPGNo;
	int			nPLNo;
	int			nPGCNo;
	int			nCellNo;
	byte		byCellType;
	int			nEPNo;
	int			nVOBNo;
	int			nVOBUNo;	
	byte		byVOBUType;		// 0:movie, 
								// 1:S_VOB_ENT type A, 2:S_VOB_ENT type B
								// 3:S_VOB_ENT type C, 4:S_VOB_ENT type D	

	int 	nVOBUFrameNs;
	int64_t		i64PGFrameNs;	//in playlist domain, PG means the playlist.
	
	int64_t 	i64VOBUStartPTS;
	int64_t 	i64VOBUEndPTS;
	
	int64_t 	i64RefPTSTick;
	int64_t 	i64StartPresPTS;	
	int64_t	i64LastVOBUPTS;
	int64_t	i64VOBUPTS;
	int64_t	i64PBPTS;
	int64_t i64STCOffset;
} TM_INFO, *PTM_INFO;
typedef ulDArray<PTM_INFO> TM_INFO_QUEUE;
#define TM_INFO_EXT_SZ	sizeof(int64_t) * 6

typedef struct {
	// for PlaySegment
	bool	bIsSegmentPlay;
	bool	bIsThumbDecode;
	int		nSPStartPGCNo;
	int		nSPStartPGNo;
	int		nSPStartCellNo;
	int		nSPStartCellVOBUNo;
	int64_t i64SPStartCellFrameNo;
	int64_t i64SPStartFrameIdx;
	int		nSPEndPGCNo;
	int		nSPEndPGNo;
	int		nSPEndCellNo;
	int		nSPEndCellVOBUNo;
	int64_t i64SPEndCellFrameNo;
	int64_t i64SPEndFrameIdx;
} SEG_INFO, *PSEG_INFO;

typedef struct {
	int		nPGNo;
	int 	nPLNo;
	int		nPGCNo;
	int		nCellNo;
	byte	byCellType;
	int		nEPNo;
	int		nVOBNo;
	int		nVOBUNo;
	byte	byVOBUType;		// 0:movie, 
							// 1:S_VOB_ENT type A, 2:S_VOB_ENT type B
							// 3:S_VOB_ENT type C, 4:S_VOB_ENT type D

	int		nVOBUFrameNs;
	int64_t i64PGFrameNs;	//in playlist domain, PG means the playlist.
	
	int64_t i64VOBUStartPTS;
	int64_t i64VOBUEndPTS;

	int		nReadVOBUBlockNo;
	int		nVOBUBlockNs;	
	int		nAAVOBUBlkNs;	// additional audio 

	int64_t i64CellFrameNs;
	
//	byte	byPGCType;
	bool	bForceJump;	
	int		nENTNo;		//for Still picture only	
} PB_INFO, *PPB_INFO;

typedef struct {
	PB_INFO	oPbInfo_Current;
	PB_INFO	oPbInfo_Next;

	int 	bIsStarted;
	int		nReadBlockNo;
	int		nRdAABlkNo;	

	// forward/backward paramters
	int		nSpeed;
	int		nSkip;
	int		nScanMode;		// 0:linear-play, 1:I-scan N:VOBU-scan(N>1, also means number of frames played per stride)
//	bool	nSearchForward;	// true if search forward for target, false if search backward
	int	 	nScanStride;    // number of frames per stride (played + skipped frames)
	long	lPlaybackRate;

	byte	byNavState;		
	byte	byFilterState;
						
	int64_t i64LastPresPTS;
	int64_t i64LastRefPTS;

	// for still picture
	int64_t i64StillTime;			// S_VOB_ENT still time
	int64_t i64ElapsedStillTime;	// elapsed still time of the current still picture

#ifdef _READ_CACHE_BLK
	/// Read-ahead cache
  	read_cache_t *pCache;
#endif

	// stc offset
	int64_t i64STCOffset;
	
	// for context
//	bool	bSendOutContext;
//	int32_t i32LastEvent;
	int 	nCTLastCellNo;
	int		nCTLastVOBUNo;
	
} VR_NAV;

typedef struct del_cell_range {
	int		nStartFrameNo;
	int		nEndFrameNo;
} del_cell_range;  // neco+

typedef struct check_VOBG_STIN {
	uint8_t	i8VOBG_STIN;
	byte	byDel;
} check_VOBG_STIN;  // neco+

typedef struct dtTimeCode_t {
	uint8_t		i8Hour;
	uint8_t		i8Minute;
	uint8_t		i8Second;
	uint8_t		i8Pictures;
} dtTimeCode_t;

//======neco+====================================================
class CRecFlag
{
public:

    unsigned int m_FlagID;
    unsigned int m_size;
    unsigned char m_buffer[10];

    inline CRecFlag(unsigned int FlagID)
    {
        m_size  = 0;
        m_FlagID = FlagID;
    }

    inline void Pack(unsigned int data)
    {
        Pack((void*)&data, sizeof(unsigned int));
    }

    inline void Pack(void* data, unsigned int size)
    {
        //assert(m_size+size <= NAV_USER_CMD_SIZE);
        memcpy(m_buffer+m_size, data, size);
        m_size += size;
    }
};

typedef ulDArray<CRecFlag *> REC_FLAG_ARRAY; 
//===============================================================

class CRTR_VMGI : public CObject
{
public:
	CRTR_VMGI();
	virtual ~CRTR_VMGI();


	int		Open(IN const char *lpctstrDVDPath, byte byExtendIFO = 0, osal_mutex_t *pMutex=NULL, byte byIsLivePause=0);
	int		Create(IN const char *lpctstrDVDPath, byte byExtendIFO = 0, byte byIsLivePause=0);
	int		Write();
	void	Close();
	int		Initialize(const char *szDVDPath, byte byExtendIFO, byte byIsLivePause);
	bool	CanRecord();

	bool	IsIncludeMovieFile();
	bool	IsIncludeStillFile();
	bool	IsIncludeAudioFile();
	char*	GetDVDPath()	{ return m_pIfoHandle ? strdup(m_pDvdReader->root_dir):NULL; }
	dvd_info_t*	GetDVDReader()	{ return m_pDvdReader; }

	bool	NewVroFile(byte byVroType);
	__off64_t	SeekVroFile(byte byVroType, __off64_t nOffset);
	int		WriteVroFile(byte byVroType, byte *pbyData, int nSize, __off64_t nOffset);
	int		TruncateVroFile(byte byVroType, uint32_t i32Newsize/* __off64_t nNewFileSize*/);

	int		GetDiscStillTime();
	int		SetDiscStillTime(int nStillTime);
	byte	GetPrimaryTextCHRS();
	int		SetPrimaryTextCHRS(byte byCHRS);
	int		GetDiscRepName(rep_nm_t &oDiscName);
	int		SetDiscRepName(rep_nm_t oDiscName);
	int		GetDiscRepPict(int &nPGNo, int &nCellNo, byte &byCellType, int &nFrame, dtdf_t &oCreateTime);
	int		SetDiscRepPict(int nPGNo, int nCellNo, int nFrame);

   	int		GetCellType(int nPGCNo, int nCellNo, byte &byCellType);
	int		GetMCellLength(int nPGCNo, int nCellNo, ptm_t &oLength);
    
	int		GetOrgPGNs();
	int		GetOrgPGTotalCellNs();
	int		GetOrgPGLength(int nPGNo, int &nHour, int &nMin, int &nSec, int &nFrame);
	int		GetOrgPGRecDateTime(int nPGNo, dtdf_t &oRecTime);
	int		GetOrgPGCellNs(int nPGNo, int &nCellNs);
	int		GetOrgPGPrimaryText(int nPGNo, rep_nm_t &oPGName);
	int		SetOrgPGPrimaryText(int nPGNo, rep_nm_t oPGName);
	int		GetOrgPGRepPict(int nPGNo, int &nCellNo, byte &byCellType, int &nFrame);
	int		SetOrgPGRepPict(int nPGNo, int nCellNo, int nFrame);
	int		GetOrgPGEPNs(int nPGNo, int &nEPNs);
//	int		GetOrgPGEP(int nPGNo, int nEPNo, int &nCellNo, byte &byCellType, void **ppEPInfo);
	int		GetOrgPGEP(int nPGNo, int nEPNo, int &nCellNo, byte &byCellType, int &nFrame, char **ppszPrimaryText); 
	int		GetOrgPGEP(int nPGNo, int nEPNo, int &nCellNo, byte &byCellType, int &nCellFrame, int &nPGFrame); 

	int		GetPLEP(int nPLNo, int nEPNo, int &nCellNo, byte &byCellType, int &nFrame, char **ppszPrimaryText); 
	int		GetPLEP(int nPLNo, int nEPNo, int *pnCellNo, byte *pbyCellType, int *pnCellFrame, int *pnPLFrame); 
	int		GetOrgPGStartSector(int nPGNo, int &nStartSector);
	int		DelPG(int nPGNo, bool bIsTE); 

	int		GetCellTVSystem(int nPGCNo, int nCellNo, byte &byTVS); 
	int		AddOrgPGEP(int nCellNo, int nFrame); 
	int		AddPLEP(int nPLNo, int nCellNo, int nFrame); 
	int		DelPLEP(int nPLNo, int nEPNo); 
	int		DelPL(int nPLNo); 
	int		CopyPL(int nSrcPLNo, int nNewPLNo);
	int		DelOrgPGEP(int nPGNo, int nEPNo);
	int		ABDelPL(int nPLNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame);
	int 	DelPLScene(int nPLNo, int nEPNo);
	int		MovePL(int nSrcPLNo, int nDstPLNo);
//	int		AddPLScene(int nPLNo, int nSceneNs, byte *byPGLType,  int *pnPGLNo, int *pnStartCellNo, int *pnStartFrame, int *pnEndCellNo, int *pnEndFrameNo);
	int		AddPLScene(int nPLNo, int nSceneNs, byte *byPGLType,  int *pnStartPGLNo, int *pnStartCellNo, int *pnStartFrame, int *pnEndPGLNo, int *pnEndCellNo, int *pnEndFrameNo);
	
	int		MovePLScene(int nPLNo, int nSrcEPNo, int nDstEPNo);
//	int		CreatePLScene(int nNewPLNo, int nSceneNs, byte *byPGLType,  int *pnPGLNo, int *pnStartCellNo, int *pnStartFrame, int *pnEndCellNo, int *pnEndFrameNo);
	int		CreatePLScene(int nNewPLNo, int nSceneNs, byte *byPGLType,  int *pnStartPGLNo, int *pnStartCellNo, int *pnStartFrame, int *pnEndPGLNo, int *pnEndCellNo, int *pnEndFrameNo);
	int		GetPLNs();
	int		GetPLLength(int nPLNo, int &nHour, int &nMin, int &nSec, int &nFrame);
	int		GetPLCreatingDateTime(int nPLNo, dtdf_t &oCreateTime);
	int		GetPLCellNs(int nPLNo, int &nCellNs);
	int		GetPLPrimaryText(int nPLNo, rep_nm_t &oPLName);
	int		SetPLPrimaryText(int nPLNo, rep_nm_t oPLName);
	int		GetPLRepPict(int nPLNo, int &nCellNo, byte &byCellType, int &nFrame);
	int		SetPLRepPict(int nPLNo, int nCellNo, int nFrame);
	int		GetPLEPNs(int nPLNo, int &nEPNs);

#ifdef USE_IOCACHE
	int		GetNextBlock(uint8_t **buf, int32_t *event, int32_t *len, uint8_t **pCmdBuf);
#else

#ifdef VOB_CACHE_DATA 
	int		GetNextBlock(uint8_t **buf, int32_t *event, int32_t *len, uint8_t **pCmdBuf);
#else
	int		GetNextBlock(uint8_t **buf, int32_t *event, int32_t *len, uint8_t **pCmdBuf);
#endif /* VOB_CACHE_DATA */

#endif

	int		Add_M_VOB_STI(m_vob_sti_t oM_VOB_STI);
	int 	Add_S_VOG_STI(s_vob_sti_t oS_VOG_STI); 
	int		Add_ORG_PG(unsigned int nRecFlag);
	void	Add_ORG_PG_M_Cell(int nPGNo, int nM_VOBI_SRPN);
	void	Update_ORG_PG_M_Cell(unsigned int &nStartPTM, unsigned int &nEndPTM);
	int		Add_M_VOB(int nSTINo, unsigned int nAddrOffset);
	void	Update_M_VOB(unsigned int &nStartPTM, unsigned int &nEndPTM);
	int		Add_TM_ENT(unsigned int nVOBUNs=0, unsigned int nTimeDiff=0, unsigned int nVOBUAddr=0, bool bIsExactVOBUNo = false); 
	int		Add_VOBU_ENT(unsigned int &n1stRefSize, unsigned int &nVOBUPBTime, unsigned int &nVOBUSize);

	void	ABDelVROFile(byte byType, int nStartSector, int nEndSector);	//for simulated UDF file system opteration
	int		PGCopyVROFile(byte byType, dvd_file_t *pSrcFile, int nStartSector, int nEndSector);

	int		PlayNextPG();
	int		PlayPrevPG();
	int		PlayNextEP();
	int		PlayPrevEP();
	int		PlayAtPG(int nPGNo);
	int		PlayAtPGEP(int nPGNo, int nEPNo);
	int		PlayAtPL(int nPLNo);
	int		PlayAtPLEP(int nPLNo, int nEPNo);
	int		PlayPGSegment(int nPGNo, unsigned int nStartSeconds, unsigned int nStartFrameIdx, unsigned int nEndSeconds, unsigned int nEndFrameIdx);
	int		PlayPLSegment(int nPLNo, unsigned int nStartSeconds, unsigned int nStartFrameIdx, unsigned int nEndSeconds, unsigned int nEndFrameIdx);
	int		PlayAtPGTime(int nPGNo, int64_t i64PbSeconds, int64_t i64PbFrameNs);
	int 	PlayAtPLTime(int nPLNo, int64_t i64PbSeconds, int64_t i64PbFrameNs);

	byte	GetPlayDomain();
	int		GetStillTime()	{ return m_oVrNav.i64StillTime; }
	void	StillSkip();

	dvd_file_t* GetMovieFile()	{ return m_pMovieFile; }
	dvd_file_t* GetStillFile()	{ return m_pStillFile; }
	dvd_file_t* GetAudioFile()	{ return m_pAudioFile; }
	dvr_ifo_handle_t* GetIfoHandle()  {	return m_pIfoHandle; }  
	int		ABDelPG(int nPGNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, byte byDelType); 
	int		GetOrgPGStartCellNo(int nPGNo)	{ return xGetOrgPGStartCellNo(nPGNo); }
	int		CopyPG(CRTR_VMGI *pSrcRtrVmgi, int nPGNo);  

	int		UpdateTimePosition(PRESENTATION_POSITIONS* pPosition);
	int		GetPlaybackStatus(NAVPLAYBACKSTATUS* status);
	int		GetPbInfoFromPGLPbTime(byte byPGLType, int nPGLNo, int nPbTimeFrameNo, byte &byCellType, int &nCellNo, int &nFrameNo);  
	int		CombinePL(int nPLNo, int nCmbPLNo);  
	int		DividePL(int nPLNo, int nDivCellNo, int nFrameNo);  
	int		Set_RSM_MRKI(int nPGCNo, int nPGNo, int nCellNo, int nFrameNo);  
	int		GetVideoStatus(NAVVIDEOSTATUS* status);  
	int		GetAudioStatus(NAVAUDIOSTATUS* status, bool bIsPreCacheTitle=false);  
	int		GetSpicStatus(NAVSPICSTATUS* status);  
	int		GetVideoContext(int* pContext);
	int		GetVideoAttr(v_atr_t* vaVideoAttr);
    void    GetVideoDCICCI(VIDEODVDDCICCI* dci_cci);

	void	SetVRState(byte byNavState);
	byte	GetVRState()				{ return m_oVrNav.byNavState; }
	byte	GetFilterState()			{ return m_oVrNav.byFilterState; }
	void	SetFilterState(byte byFilterState);
	void	Set_Channel(uint32_t i32Type, uint32_t i32Chdata);  
	void	Set_Recording_Mode(uint32_t i32RecordingMode);  
	int		Set_IT_TXT();  
	int		SetSpeed(int nSpeed, int nSkip);
	int		CombinePG(int nStartPGNo, int nCmbPGNs);  
	int		DividePG(int nPGNo, int nDivCellNo, int nFrameNo);  

	int		GetDecodeChannelIdx(bool bChgChannel=false);	
	int 	AddDBCount(osal_mutex_t *pMutex = NULL);  
	int		ReleaseDBCount();  
	int		GetDBCount();  

	int		DecodeRepPict(vr_decode_keyframe_t *pDecKeyFrame);
	int		DecodeMultipleRepPict(vr_decode_multiple_keyframe_t *pMulDecKeyFrame);
						  
	void	SetIFOType(byte byExtIFOType) { m_pIfoHandle->byExtIFOType = byExtIFOType; }

	int		SetBookmark(void* state, unsigned int size);
	int		GetBookmark(void* state, unsigned int size, unsigned int* returnedSize);

	int		SetRepeat(int nFlag);	//0:reset, 1:title, 2:chapter
	void	SetRecordTitle(int nTitleNo) { m_nRecTitleNo = nTitleNo; }  
	int		GetRecordTitle() { return m_nRecTitleNo; }  

	int64_t GetSTCOffset();

	void	SetSubpictureStream();
	int		GetTMU(uint8_t C_TY1, int nSTINo);  
	int		GetMVOBNs();  
//	int		GetCellFrameNs(int nPGCNo, int nCellNo, int nMode, int *pnFrameNs, int *pnBeginFrameNo=NULL); //nMode:0->frame mode, 1->time mode	 
//	void	Get_M_VOBI_Size(uint16_t i16M_VOBI_SRPN, uint32_t *pi32M_VOBI_SZ);  
	bool	LivePause();  
	int		LPInitVroFile(uint32_t i32MaxVOBsz);  
	void	SetLivePause(bool bIsLivePause, uint32_t nTotalMVOBNs);
    bool    IsLivePause();
	bool	IsRecord();

	int		SetAutoChapter(/*bool bIsAutoChapter, */uint32_t nAutoMVOBUNs);  
	
	int 	GetCurrentTitleNo();
	int		GetCurrentChapNo();
	
	void 	SetPlaybackRate(long lPbRate) { m_oVrNav.lPlaybackRate = lPbRate; }
	byte 	GetDiscType();
	
	int		GetPGFrameNo(int nPGNo, int nCellNo, int nFrameNo, int &nPGFrameNo); 
	ssize_t	ReadBlocks(dvd_file_t *dvd_file, int offset, size_t block_count, unsigned char *data, byte byRetry = true); 
	int		FixFailRecord(byte byErrorType, uint32_t i32MVRO_SZ, uint32_t i32MVRO_Real_SZ); 
	void	Dofsync_MMAP();  
	int		GetOrgPGAddr(int nPGNo, uint32_t &i32StartSector, uint32_t &i32Length);  
	int		GetPGCCellNs(int nPGCNo);  
	int		CellNsType();  
	int		CanSpeed(byte* byCanSpeed);  
	int		Check_PG_Split_Point(int nPGNo, int nCellNo, int nFrameNo);  
	int		Check_PG_AB_Point(int nPGNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame);  

    HRESULT UpdatePrivateData(unsigned int id, unsigned char* data, unsigned int size); //gordon+
    void    OnFlush(); //gordon+
	int		CanPlayAtPGTime(int nPGNo, int64_t i64PbSeconds, int64_t i64PbFrameNs, uint32_t i32LP_MVRO_file_prt, uint32_t i32IFODelSz, uint32_t i32VRODelSz, VR_CAN_PB_AT_TIME_ERRNO &Errno);  
	void	SetTimeShiftRange(int nTimeShiftSafeRange) { m_nTimeShiftSafeRange = nTimeShiftSafeRange; }  
	int		GetLivePauseTotalLength(int &Sec, int &Frame);
	int		CheckLPVROExist(uint32_t i32TotalVOBNs, uint32_t i32CellBlocks);
	int		OpenMVROFile();
	uint32_t CheckVROoffsetGOPBnd(uint32_t i32Offset);
	void	ResetLastReadAddr();
	void	SetLastReadBuf(byte* pbyReadBufAddr);
	void	CleanLastReadAddr();
	int		GetOrgPGStartEndCellNo(int nPGNo, int &nStartCellNo, int &nEndCellNo);
	int		GetOrgPGDiscontinuousAddr(int nPGNo, int &nFileNs, uint32_t *pi32StartSector, uint32_t *pi32Length);
//	void 	PrintCellVOBInfo();
	int64_t GetFrameFromSec(int64_t Sec);
	void	SetIsFlushing(bool bIsFlushing)  { m_bIsFlushing = bIsFlushing; }
	int		GetFrameInterval()	{return xGetFrameInterval();}
	int		GetOrgPGProtectState(int nPGNo, bool &bIsProtect);
	int		SetOrgPGProtectState(int nPGNo, bool bIsProtect);
	int		GetIsNewRecPG(int nPGNo, bool &bIsNewPG); 
	int		SetIsNewRecPG(int nPGNo, bool bIsNewPG); 
	void	SetAutoResetNewRecPG(bool bIsAutoResetNewRecPG); 
	bool	SetTitleChildLock(int nPGNo, bool bChildLock);	
	int		GetTitleChildLock(int nPGNo);
	bool	SetTitleEditStatus(int nPGNo, bool bEditStatus);	
	int		GetTitleEditStatus(int nPGNo);
	
	
#ifdef _DEBUG
	int		Dump(char *command);  
#endif

private:
	int		xConvertMCellFrameToPTM(int nPGCNo, int nCellNo, int nFrame, uint32_t &NewEP_PTM, bool bCheckFrame=false);  
	void	xConvertSCellFrameToVOBENTN(int nPGCNo, int nCellNo, int nFrame, uint8_t &nNewFrame);  
	
	int		xCmp_M_EP_Element(int nPGCNo, int nCellNo, int nFrame, uint32_t &pNewEP_PTM, int &nNewEPNo);  
	int		xCmp_S_EP_Element(int nPGCNo, int nCellNo, int nFrame, uint8_t &nNewFrame, int &nNewEPNo);  
	int		xInsert_M_EP(int nPGCNo, int nCellNo, uint32_t pNewEP_PTM, int nNewEPNo);  
	int		xInsert_S_EP(int nPGCNo, int nCellNo, int nFrame ,int nNewEPNo);  
	int		xGetPGCEPNs(int nPGCNo, int &EPNs);  
	int		xGetCellFrameNs(int nPGCNo, int nCellNo, int nMode, int *pnFrameNs, int *pnBeginFrameNo=NULL); //nMode:0->frame mode, 1->time mode	 
	int		xGetCellRemainFrameNsByPTM(int nPGCNo, int nCellNo, ptm_t oPTM, int *pnRemainFrameNs, int *pnBeginFrameNo);
	int		xDelM_EP(int nPGCNo, int nCellNo, int nDelEPNo);  
	int		xDelS_EP(int nPGCNo, int nCellNo, int nDelEPNo);  
	int		xGetAll_UDPGC_CellNs(int &nPLCellNs, int &nM_CellNs, int &nS_CellNs);  
	void	xGetPGC_CellNs(int nPGCNo, int &nM_CellNs, int &nS_CellNs);  
	int 	xCheckCellFrameNs(int nPGCNo, int nCellNo, int nFrameNo, int &nType);  
	int 	xCheckEndCellFrameNs(int nPGCNo, int nCellNo, int nFrameNo, int &nType);  
	void	xGetOrgPGCellNs(int nPGNo, int &nM_CellNs, int &nS_CellNs);  
	int		xCopy_PG_S_VOGI(dvr_ifo_handle_t *pSrcIfoHandle, int nSrcSOGI);  
	int		xCopy_PG_M_VOBI(dvr_ifo_handle_t *pSrcIfoHandle, int nSrcMOBI);  
	
//	int 	xABDelPL_NoBnd(int nPGCNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame);  
	int		xDelPGLCells(int nPGCNo, int nStartCellNo, int nEndCellNo);  
	int		xDelPGLCellEPs(int nPGCNo, int nCellNo, int nStartEPNo, int nEndEPNo);  
	int 	xABDelPGL_SetFirstCell(int nPGCNo, int nCellNo, int nFrame, int &DelStartEPNo);  
	int 	xABDelPGL_SetLastCell(int nPGCNo, int nCellNo, int nFrame, int &DelEndEPNo);  
	int 	xABDelPGL_GetEndDel_M_EP(int nPGCNo, int nCellNo, int nEndFrame, int &DelEndEPNo);  
	int 	xABDelPGL_GetEndDel_S_EP(int nPGCNo, int nCellNo, int nEndFrame, int &DelEndEPNo);  
	int 	xABDelPGL_NoBnd_OneCell(int nPGCNo, int nCellNo, int nStartFrame, int nEndFrame);  
	int 	xABDelPGL_NoBnd_Cells(int nPGCNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame);  
	int		xABDelPGL_1BndSet1stCell(int nPGCNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame);  
	int		xABDelPGL_1BndSetLastCell(int nPGCNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame);  
	int 	xABDelPGL_GetStartDel_S_EP(int nPGCNo, int nCellNo, int nStartFrame, int &DelStartEPNo);  
	int		xABDelPGL_GetStartDel_M_EP(int nPGCNo, int nCellNo, int nStartFrame, int &DelStartEPNo);  
	int		xABDelPG_CheckUDPGCDelRange(int nUDPGCNo, int nUDCellNo, int nOrgCellNo, int nStartFrame, int nEndFrame, int &nDelStartFrame, int &nDelEndFrame); 
	int		xABDelPG_SetDelCellFrame(int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, int &nAB_Type, del_cell_range* DelCellFrame); 
	int		xABDelMVOB(int nAB_Type, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, int &nS_StartSector, int &nS_EndSector);  
	int		xInsert_M_VOBI(uint16_t i16New_M_VOBI_SRPN);  
	int		xDel_M_VOBI(uint16_t i16S_M_VOBI_SRPN, uint16_t i16E_M_VOBI_SRPN);  
	void	xChangeVOBI_SRPN_Delete(uint16_t i16S_M_VOBI_SRPN, uint16_t i16E_M_VOBI_SRPN);  
	void	xChangeVOBI_SRPN_Insert(uint16_t i16S_M_VOBI_SRPN, uint16_t i16E_M_VOBI_SRPN);  
	int		xDel_M_VOB_STIN(int nSTI_Rec_Ns, check_VOBG_STIN *pSTI_Rec);  
	int		xDel_VOBU_ENT(uint16_t i16M_VOBI_SRPN, int nStartDelVOBU, int nEndDelVOBU);  
	int		xDel_TM_ENT(uint16_t i16M_VOBI_SRPN, int nStartDelTMENT, int nEndDelTMENT);  
	int		xOne_TM_ENT(uint16_t i16M_VOBI_SRPN, bool bIsNewMVOB);  
	void	xChangeVOGI_SRPN_Delete(uint16_t i16S_S_VOGI_SRPN, uint16_t i16E_S_VOGI_SRPN);  
	int		xDel_S_VOGI(uint16_t i16S_S_VOGI_SRPN, uint16_t i16E_S_VOGI_SRPN);  
	int		xDel_S_VOG_STIN(int nSTI_Rec_Ns, check_VOBG_STIN *pSTI_Rec);  
	void	xGet_S_VOGI_Size(uint16_t i16S_VOGI_SRPN, uint32_t &i32S_VOGI_SZ);  
	void	xGet_S_VOB_Size(uint16_t i16S_VOGI_SRPN, uint8_t i8S_VOB_No, uint32_t &i32S_VOB_SZ);  
	int		xDel_VOB_ENT(uint16_t i16S_VOGI_SRPN, int nStartDelVOB, int nEndDelVOB);  
	void	xChangeVOGI_SRPN_Insert(uint16_t i16S_S_VOGI_SRPN, uint16_t i16E_S_VOGI_SRPN);  
	
	void	xChange_S_VOGI_Addr(uint16_t i16S_S_VOGI_SRPN, uint16_t i16E_S_VOGI_SRPN, uint32_t i32Diff_Addr);  
	int		xInsert_S_VOGI(uint16_t i16New_S_VOGI_SRPN);  
	int		xABDelSVOG(int nAB_Type, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, int &nS_StartSector, int &nS_EndSector);  
	int		xABDelSVOG_1ndSetFirstVOGI(int nStartCellNo, int nStartFrame, uint16_t i16S_S_VOGI_SRPN, uint32_t &i32Del_Start_Addr, uint32_t &i32Diff_Addr);  
	int		xABDelSVOG_1ndSetLastVOGI(int nEndCellNo, int nEndFrame, uint16_t i16E_S_VOGI_SRPN, uint32_t &i32Diff_Addr);  
	int		xABDelSVOG_OneVOGI(uint16_t i16S_S_VOGI_SRPN, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, uint32_t &i32Del_Start_Addr, uint32_t &i32Diff_Addr);  
	
	void	xChange_M_VOBI_Addr(uint16_t i16S_M_VOBI_SRPN, uint16_t i16E_M_VOBI_SRPN, uint32_t i32Diff_Addr);  
	void	xGet_M_VOBI_Size(uint16_t i16M_VOBI_SRPN, uint32_t &i32M_VOBI_SZ);  
	int		xABDelMVOB_1ndSetFirstVOBI(int nStartCellNo, int nStartFrame, uint16_t i16S_M_VOBI_SRPN, int nStartDelVOBU, uint32_t &i32Del_Start_Addr, uint32_t &i32Diff_Addr);  
	int		xABDelMVOB_1ndSetLastVOBI(int nEndCellNo, int nEndFrame, uint16_t i16E_M_VOBI_SRPN, int nEndDelVOBU, uint32_t &i32Diff_Addr);  
	int		xABDelMVOB_OneVOBI(uint16_t i16S_M_VOBI_SRPN, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, uint32_t &i32Del_Start_Addr, uint32_t &i32Diff_Addr);  

	int		xABDelOrgPG(int nAB_Type, int nPGNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame);  
	int		xGetOrgCellPGNo(int nCellNo);  
	
	int		xInsertPGL_Cell(int nPGCNo, int nNewCellNo, byte byCellType);  
	int 	xInsertPGL_M_Cell(int nPGCNo, int nNewCellNo, uint16_t i16M_VOBI_SRPN, uint32_t i32S_PTM_base, uint32_t i32E_PTM_base);  
	int 	xInsertPGL_S_Cell(int nPGCNo, int nNewCellNo, uint16_t i16S_VOGI_SRPN, uint8_t i8S_S_VOB_ENTN, uint8_t i8E_S_VOB_ENTN);  
	int		xDelPL(int nPLNo);  
	int		xDelPG(int nPGNo);  

	int		xGetPLEPNs(int nPLNo, int &nPGCNo, int &nEPNs);  
	int		xGetOrgPGStartCellNo(int nPGNo);
	int		xGetCellEPNs(int nPGCNo, int nCellNo);
	void	xGetMVOBRecTime(int nVOBNo, dtdf_t &oRecTime);
	void	xGetSVOGFirstRecTime(int nVOGNo, dtdf_t &oRecTime);
//	int		xGetUDPGCCellNs(int nPGCNo); neco-
	int		xGetPGCCellNs(int nPGCNo); 
	int		xGetCellVOBUNs(int nPGCNo, int nCellNo);

	void	xGetMVOBAddressOffset(int nVOBNo, int &nAddrOffset);
	void	xGetCellVOBNo(int nPGCNo, int nCellType, int nCellNo, int &nVOBNo);
	int		xConvertMCellPTMToFrame(int nPGCNo, int nCellNo, ptm_t oPTM);
	void	xConvertSCellVOBENTNToFrame(int nPGCNo, int nCellNo, int nFrame, int &nRelFrame);  
	void	xConvertPGEPToCellEP(int nPGNo, int nEPNo, int &nCellNo, int &nCellEPNo , byte &byCellType);
	void	xConvertPGCEPToCellEP(int nPGCNo, int nEPNo, int &nCellNo, int &nCellEPNo, byte &byCellType);  
	int		xConvertMVOBPTMToFrame(int nPGCNo, int nCellNo, ptm_t oPTM);
	int		xConvertMVOBFrameToPTM(int nPGCNo, int nCellNo, int nFrame, uint32_t &New_PTM); 
	int		xResetPLRepPict(int nPLNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame);  
//	int		xResetPGRepPict(int nAB_Type, byte byResetRepPict, int nDelCellNs, int nPGNo); 
	
	void 	xGetTime(dtdf_t &dtTime); 
	void	xCheckTime(dtdf_t &dtTime); 
	int		xCopyPL_SRP(int nSrcPLNo, int nDstPLNo); 
	int		xCopyUD_PGC(int nSrcPGCNo, int nDstPGCNo); 
	int		xCopyUDPGC_CellEP(int nSrcPGCNo, int nSrcCellNo, int nDstPGCNo, int nDstCellNo, int nStartEPNo, int nEndEPNo); 
	int		xInsert_PL(int nNewPLNo, uint8_t i8PGCN); 
	int		xInsertIT_TXT(uint16_t	i16IT_TXT_SZ, char *pIT_TXT); 
	int		xInsert_UD_PGC(int nNewPGCNo, uint16_t i16PG_Ns, uint16_t i16CI_SRP_Ns); 
	void	xChangeIT_TXT_SRPN(uint16_t i16IT_TXT_SRPN); 
	int		xCheckIT_TXT_Used(uint16_t i16IT_TXT_SRPN , byte &byDelIT_TXT); 
	int		xDelIT_TXT(uint16_t i16IT_TXT_SRPN); 
	int		xAddOnePLScene(int nDstPGCNo, int nSrcPLNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrameNo); 
	int		xMovePLScene_SprCell(int nPGCNo, int &nSrcCellNo, int &nSrcFrame, byte bySrcCellType); 
	int		xMoveSceneCells(int nPGCNo, int nSrcCellNo, int nDstCellNo, int nSceneRange); 
	int		xSetPLType(int nPLNo); 
	int		xGetDelPGCEPNo(int nPGCNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, int &nDelEPNs); 

	int		xConvertPLToUDPGC(int nPLNo);

	int		xGetCellStartLength(int nPGCNo, int nCellNo, int &nStartSector, int &nLength);
	int		xGetMVOBUNoByVOBPTM(int nMVOBNo, ptm_t oPTM, int &nMVOBUNo);
	int		xGetMVOBUNoByVOBFrame(int nMVOBNo, int nFrameNo, int &nMVOBUNo);
	int		xGetSVOBENoByVOBENT(int nSVOBNo, int nCellEPNo, int &nSVOBENo);
	int		xGetMVOBStartLengthByFrame(int nMVOBNo, int nStartFrame, int nEndFrame, int &nStartSector, int &nLength, int &nOffsetFrame);
	int		xGetSVOGStartLengthByENT(int nSVOGNo, int nStartENTNo, int nEndENTNo, int &nStartSector, int &nLength);
	int		xGetOrgPGEPStartSector(int nPGNo, int nEPNo, int &nStartSector, int &nOffsetFrame);
	int		xGetPGCEPStartSector(int nPGCNo, int nEPNo, int &nStartSector, int &nOffsetFrame);
	//void	xPlusFrameConvertToHMSF(byte byTVS, int nFrameNs, int &nHour, int &nMin, int &nSec, int &nFrame);
	void	xPlusFrameConvertToHMSF(byte byTVS, int nFrameNs, int &nHour, int &nMin, int &nSec, int &nRestPTS);
	void	xGetSCellStartEndENTNo(int nPGCNo, int nCellNo, int &nStartENTNo, int &nEndENTNo);
	void	xGetSCellEPStartENTNo(int nPGCNo, int nCellNo, int nEPNo, int &nENTNo);
	void  	xGetSCellENTStartLength(int nPGCNo, int nCellNo, int nENTNo, int &nStartSector, int &nLength);
	uint8_t	xGetSCellENTType(int nPGCNo, int nCellNo, int nENTNo);
	yuv_t*  xGetCellSP_PLT(int nPGCNo, int nCellNo, byte byCellType);
	int		xGetCellSPSTNs(int nPGCNo, int nCellNo, byte byCellType, int nENTNo=0);
	void	xGetS_VOB_ENT_Type(uint16_t i16S_VOGI_SRPN, int  &nANs, int &nBNs, int &nCNs, int &nDNs); 
	int		xGetMVOBStartSectorByVOBUNo(int nMVOBNo, int nVOBUNo, int &nStartSector);

	int		xGetMVOBUBlockNs(int nVOBNo, int nVOBUNo, int &nBlockNs);
	int		xGetSVOBEBlockNs(int nVOBNo, int nVOBUNo, int &nBlockNs, int *pnAABlkNs=NULL);
	int		xGetMVOBU1stRefBlockNs(int nVOBNo, int nVOBUNo, int &nBlockNs);
	int		xGetMVOBUFrameNs(int nVOBNo, int nVOBUNo, int &nFrameNs);
	int		xGetSVOBEFrameNs(int nVOBNo, int nVOBUNo, int &nFrameNs);
	
	int		xLoadingCheck(uint32_t &i32Ptr, const char *lpctstrDVDPath, uint32_t &i32RealVroSize, uint16_t &i16ErrorCellNo); 
	int		xCheckCells(uint32_t &i32Ptr, uint32_t &i32RealVroSize, int *pMVOBIPGCellNo, int *pSVOGIPGCellNo, uint16_t &i16ErrorCellNo); 
	int		xRecovery(uint32_t i32Ptr, const char *lpctstrDVDPath, uint32_t i32VroRealSize); 
	int		xCheckPKTHeader(byte *pbyBuf); 
	int		xCheck_RDI_PKT(byte *pbyBuf); 
	int		xCheckPKTType(byte *pbyBuf, int &nOffset); 
	int		xCheck_Video_PKT(byte *pbyBuf, int &nOffset); 
	int		xGetVSeqInfo(byte *pbyBuf, int &nOffset, uint16_t &i16Height, uint16_t &i16Width, byte &byTVSys, byte &byMPEG); 
	int		xSkipEtn_Usr_Data(int i, byte *pbyBuf, int &nOffset, int nRestBytes); 
	int		xDiffGOPTimeCode(dtTimeCode_t dtGOPTime, dtTimeCode_t *dtLastGOPTime, int nFramesPerSec); 
	int		xDiffGOPTimeCode_GetSecond(dtTimeCode_t *dtGOPTime, int nFramesPerSec); 
	int		xDiffGOPTimeCode_GetMinute(dtTimeCode_t *dtGOPTime); 
	int		xDiffGOPTimeCode_GetHour(dtTimeCode_t *dtGOPTime); 
	int		xGetPictureHeader(int &nVOBUFrameNs, byte *pbyBuf, int &offset, int &nTemporal_ref); 
	int		xGetGOPTable(dtTimeCode_t &dtLastGOPTime, dtTimeCode_t &dtGOPTime, byte *pbyBuf, int nMVOBUNs, int nFramesPerSec, int &nGOPFrameNs); 
	int		xMakeTM_ENT(int nMVOBUNs, int &nTM_ENT_Ns, vobu_ent_t *pVOBU_ENT, tm_ent_t **pTM_ENT, byte byTVSys); 
	int		xGetV_ATR_VR(uint16_t i16Height, uint16_t i16Width); 

	int		xCheckPL(int *pMVOBIPGCellNo, int *pSVOGIPGCellNo); 
	int		xCheckPG(); 
	int		xGet_MVOB_Start_PTM(uint16_t i16M_VOBI_SRPN, int nFrameInterval, int nTemporal_ref, uint32_t i32Ptr, uint32_t &i32PTM); 
	int		xCheck_KeyFrameNo(byte byPGLType, int nPGLNo, bool bIsCellError = false); 
	int		xGetMVOBFrameNs(uint16_t i16M_VOBI_SRPN); 
	int		xCheck_MVOB_TM_ENT(uint16_t i16M_VOBI_SRPN, int nCellNo, uint32_t &i32VroSize); 
	int		xCheckStillVroSize(uint16_t i16S_VOGI_SRPN, uint32_t &i32SVroSize); 

	int		xAlloc_TM_ENT(bool bIsRealloc=false);// neco*
	int		xAlloc_VOBU_ENT(bool bIsRealloc=false);// neco*
	int		xEnQueue_TimePos();
	int		xClear_TimePos();
	int		xUpdatePbInfo(int nPGNo, int nPLNo, int nEPNo,
						  bool bIsForceJump=false,
						  bool bIsSegmentPlay=false, 
						  int nSPStartPGCNo=0, int nSPStartPGNo=0, 
						  int nSPStartCellNo=0, int nSPStartCellVOBUNo=0,
						  int64_t i64SPStartCellFrameNo=0,
						  int64_t i64SPStartFrameIdx=0,
						  int nSPEndPGCNo=0, int nSPEndPGNo=0, 
						  int nSPEndCellNo=0, int nSPEndCellVOBUNo=0,
						  int64_t i64SPEndCellFrameNo=0,
						  int64_t i64SPEndFrameIdx=0);
	int		xGetAllPLEPNs(); 
	int		xCheckPGCellNo(int nPGNo, int nCellNo); 
	int		xResetOtherPGRepPict(int nDelCellNs, int nPGNo); 
	int		xCehckPLCell(uint8_t i8PGCN, int nCellNo, int *pMVOBIPGCellNo, int *pSVOGIPGCellNo); 
	int		xSetAudioStrmAtrCh(uint8_t i8AudioChannelNs, int *numChannels); 
	int		xSetAudioStrmAtrBR(uint8_t i8Bitrate, int *bitsPerSecond); 
	int		xCheck_S_VOB_STI_Spu_Exist(yuv_t *SP_PLT); 
	int		xSetAudioStrmAtrACM(uint8_t i8ACM, ENUM_MEDIA_TYPE *type); 

	void 	xUpdateScanMode();
	inline bool	xIsScanning() 		{ return (m_oVrNav.nScanMode != 0); }
	inline bool	xIsScanningIOnly()	{ return (m_oVrNav.nScanMode == 1); }
	inline bool xIsSpeedForward()	{ return (m_oVrNav.nSpeed > 0);	 } 			// play forward
//	inline bool xIsSpeedNormal()	{ return (m_oVrNav.nSpeed == 256);	 }
	inline bool xIsSpeedNormal()	{ return (m_oVrNav.lPlaybackRate == 256);	 }
	inline bool xIsFastForward()	{ return (m_oVrNav.lPlaybackRate > 256); }	// fast forward

	inline ci_t* xGetPGCCellPtr(int nPGCNo, int nCellNo) {
		return (nPGCNo == 0 ? &m_pIfoHandle->pORG_PGCI->pCI[nCellNo-1]:
				&m_pIfoHandle->pUD_PGCIT->pUD_PGCI[nPGCNo-1].pCI[nCellNo-1]); }
				
	inline pgci_t* xGetPGCPtr(int nPGCNo) {
		return (nPGCNo == 0 ? m_pIfoHandle->pORG_PGCI:
				&m_pIfoHandle->pUD_PGCIT->pUD_PGCI[nPGCNo - 1]); }

	inline m_vobi_t* xGetMCellVOBPtr(int nPGCNo, int nCellNo){
		return (&m_pIfoHandle->pM_AVFIT->M_AVFI.pM_VOBI[(nPGCNo == 0 ? ((m_ci_t*)m_pIfoHandle->pORG_PGCI->pCI[nCellNo-1].pCI)->M_C_GI.i16M_VOBI_SRPN : ((m_ci_t*)m_pIfoHandle->pUD_PGCIT->pUD_PGCI[nPGCNo-1].pCI[nCellNo-1].pCI)->M_C_GI.i16M_VOBI_SRPN)-1]);
	}
	
	int		xGetCellEndVOBUNo(int nPGCNo, int nCellNo);
	int		xCheck_MVOBU_1STREF_SZ(uint16_t i16M_VOBI_SRPN); 
//	int		xSetVideoInitCmd(int32_t event, uint8_t *buf, int32_t *len);
	int		xCheckDiscKeyFrame(); 
	int		xConvertMVOBFrameToMVOBUFrameNo(int nMVOBNo, int nFrameNo, int &nMVOBUNo, int &nMVOBUFrameNo); 
	int		xGetPGEPNoByCellPTM(int nPGNo, int nCellNo, ptm_t oPTM, int &nEPNo);
	int		xGetPLEPNoByCellPTM(int nPLNo, int nCellNo, ptm_t oPTM, int &nEPNo);
	int		xCheckKeyFrame(bool IsDiskKeyFrame, int nPGNo, int nDivCellNo, int nDivFrameNo, int &nRep_PICTPG, int &nRep_PICTCell, int &nRep_PICTFrame); 
	int		xDividePG(int nPGNo, int nDivCellNo, int nFrameNo); 
	int		xDividePG_DivideMVOB(int nDivCellNo, uint16_t i16VOBI_SRPN, int nDivideMVOBUNo, int nVOBFrameNo); 
	int		xDividePG_DividePL(int nCellNo, byte byCellType, uint16_t i16VOBI_SRPN, int nVOBFrameNo); 
	int		xDividePLCell(int nPLNo, int nDivCellNo, int nFrameNo); 
	int		xCheck_DividePL(int nCellNo, byte byCellType, uint16_t i16VOBI_SRPN, int nVOBFrameNo); 
	int		xCheckPGBnd(int nPGNo, int nDivCellNo, int nFrameNo); 

	void	xSetTmInfoToPbInfo(PTM_INFO pTimePos, bool bIsDeliverCmd, bool bIsClearTimePosQueue);
	int		xGetMCellStartEndPTS(int nPGCNo, int nCellNo, uint32_t &i32StartPTS, uint32_t &i32EndPTS);

	int		xFirstRunWhenStartup();
	void	xInit();
	int 	xGetAudioStatus(int nPGCNo, int nCellNo, NAVAUDIOSTATUS* status);
	int		xCheck_Disc_PG_RepPict(int nCheckType, int nAB_Type, int nPGNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame, int *pnDiscRepPictType, int *pnDelPGNs); 
	int		xCheckDiscRepPictlocate(int nCheckType, int nAB_Type, int nPGNo, int nStartCellNo, int nStartFrame, int nEndCellNo, int nEndFrame); 
	int 	xReset_Disc_PG_KFrame(int nResetType, int nRepPictType, int nKFramePGNo, int nKFrameCellNo, int nKFrameNo, int nDelPGNs, int nDelCellNs, int nEndFrame); 
	
	int		xDoDeliverCmd(uint8_t **buf, int32_t *event, int32_t *len);
	int		xGetMCellEPPTS(int nPGCNo, int nCellNo, int nCellEPNo, uint32_t &i32EPPTS);
	int		xGetSCellEPENTNo(int nPGCNo, int nCellNo, int nCellEPNo, int &nEPENTNo);
	int		xGetPGMCellEPStartEndPTS(int nPGNo, int nCellNo, int nEPNo,
								 	 uint32_t &i32CellEPStartPTS, uint32_t &i32CellEPEndPTS);
	int		xGetCellStartVOBUNo(int nPGCNo, int nCellNo);
	int 	xIsLivepauseWaitForReplaceCell();
	
	int		xGetSVOGStartSectorByVOBUNo(int nSVOBNo, int nVOBUNo, int &nStartSector, int &nAAStartSector);
	int 	xGetSVOGENTType(int nSVOBNo, int nVOBUNo, byte &byENTType);
	int		xGetSVOBE_AA_SRPN_ENTN(int nVOBNo, int nVOBUNo, int &nS_AAGI_SRPN, int &nAA_ENTN);
	int		xCalcCellBeginFrameGOPDuration(byte byCellType, int nPGCNo, int nCellNo, int nVOBNo, int nVOBUNo,
										   int &nBeginGOP, int &nBeginFrame, int &nDuration,
										   ptm_t *pPTM=NULL);
	int		xGetCellEPNoByFrame(int nPGCNo, int nPGNo, byte byCellType, int nCellNo, int nCellFrameNo, int &nEPNo);										   
	void	xResetPbParam();
	void	xSetDecodeBlendCmd(ptm_t *poPTM=NULL);
	int		xCellErrorRecovery(uint16_t i16ErrorCellNo, byte byRecoveryType = 0); 
	int		xCheck_MCell_EP(int nCellNo); 
	int		xGetSecFromFrame(int64_t i64TotalFrame, int &nSec, int &nFrame);
	int		xGetMCellVOBNo(int nPGCNo, int nCellNo);
	int		xGetMVOBUFrameOffsetByVOBFrame(int nMVOBNo, int nFrameNo, int &nFrameOffset);

	
typedef enum {
	RDEND_FAILED				=	0x00,
	RDEND_NOTYET				=	0x01,
	RDEND_PLAYSTILL				=	0x02,
	RDEND_VOBUEND				=	0x0100,
	RDEND_CELLEND				=	0x0200,
	RDEND_EPEND					= 	0x0400,
	RDEND_PGEND					= 	0x0800,
	RDEND_PGSETEND				=	0x1000,
	RDEND_PLEND					=	0x2000,
	RDEND_SEGEND				=	0x4000,
	RDEND_NOP					= 	0x80000000
} RDEND_TAG;

	inline bool xIsRdPGDomain()		{ return (m_oVrNav.oPbInfo_Next.nPGNo > 0); }
	inline bool xIsRdMovieCell()	{ return (m_oVrNav.oPbInfo_Next.byCellType == DVR_M_CELL); }
	inline int	xGetFrameInterval() { 
		byte byTVS;
		GetCellTVSystem(0, 1, byTVS);
		return (byTVS == DVR_TVS_525_60 ? DVR_NTSC_FRAME_INTERVAL:DVR_PAL_FRAME_INTERVAL); }
	inline int	xGetFPS()			{
		byte byTVS = DVR_TVS_525_60;
		GetCellTVSystem(0, 1, byTVS);
		return (byTVS == DVR_TVS_525_60 ? DVR_TVS_FPS_NTSC : DVR_TVS_FPS_PAL); }
	inline bool xIsRepeat()			{ return m_byRepeat; }
	inline bool xIsRepeatTitle()	{ return (m_byRepeat == RPT_FLAG_TITLE); }
	inline bool xIsRepeatChap()		{ return (m_byRepeat == RPT_FLAG_CHAP); }
	inline bool xIsSegPlay()		{ return m_oSegInfo.bIsSegmentPlay; }
	inline bool xIsEmptyDisc()		{ return (GetOrgPGNs() == 0); }
	inline bool xIsForceJump()		{ return m_oVrNav.oPbInfo_Next.bForceJump; }
	inline bool xIsTimeshift()		{ return (m_oVrNav.oPbInfo_Next.nPGNo == m_nRecTitleNo && m_nRecTitleNo != 0); }
	inline bool xIsLivepause()		{ return m_bIsLivePause; }
	inline bool xIsFilterRunning()	{ return (m_oVrNav.byFilterState == FILTER_STATE_RUN); }
	inline void xRemoveFlag(int32_t &nEvt, int nFlag) { nEvt = nFlag; m_nDeliverCmd &= ~nFlag; }
	inline bool xHasFlag(int nFlg) { return m_nDeliverCmd & nFlg; }
		
	int		xGetVOBUBlockNs(int nVOBNo, int nVOBUNo, bool bIs1stRef=false, int *pnAAVOBUBlkNs=NULL);
	int		xGetVOBUFrameNs(int nVOBNo, int nVOBUNo);
	int 	xUpdateDecodeCmd();
	void	xxRdNextVOBU(bool bIsNewCell=false);
	int		xIsExistTmInfoByContext(int nCellNo, int nVOBUNo);
	int		xIsExistTmInfoByPresPTS(int64_t i64ReferencePTS, int64_t i64PresentedPTS);
	PTM_INFO xPopupTmInfo(int nIdx);
	void	xxCalcuTmInfo_VOBUPTS(PTM_INFO pTimePos);
	void	xInitVrNav();
	int64_t xxCalcuNextSTCOffset(int nPGCNo, int nOldCellNo, int nNewCellNo);
	bool	xIsTE_MVOB(int nVOBNo);
	bool	xIsTE_SVOBENT(int nVOBNo, int nENTNo);

	int		xIsRdToEnd();	
	int		xIsRdToVOBUEnd();
	int		xIsRdToCellEnd();
	int		xIsRdToEPEnd();
	int		xIsRdToPLEnd();
	int		xIsRdToPGEnd();
	int		xIsRdToPGSETEnd();
	int		xIsRdToSegEnd();
	int		xIsPbToEPEnd(int64_t i64PresentedPTS, PTM_INFO pTimePos);
	int 	xIsPbToNextVOBU(int64_t i64ReferencePTS, int64_t i64PresentedPTS, PTM_INFO pTimeInfo);
#ifdef USE_IOCACHE
	int		xRdNextBlks(uint8_t **buf, int32_t *len);
#else

#ifdef VOB_CACHE_DATA 
	int		xRdNextBlks(uint8_t **ppbuf, int32_t *len);
#else
	int		xRdNextBlks(uint8_t **buf, int32_t *len);
#endif /* VOB_CACHE_DATA */

#endif
	int		xRdNextVOBU();
	int		xRdNextCell();
	int		xRdNextEP();
	uint8_t	xConvertUDPGCToPL(int nPGCNo);
	void	xSetIFOTableMax(bool bIsLivePause);
	int		xGetEPNoFromVOBUNo(int nPGCNo, int nPGNo, int nCellNo, int64_t i64VOBUEndPTS);


#ifdef USE_IOCACHE
public:
	dvd_reader_t*		m_pDvdReader;
	dvd_file_t*		m_pCacheFile;
	__off64_t            m_CacheOffset;
	int            m_CacheBufferSize;
	unsigned char* m_pCacheBuf;
private:
#else
private:
	dvd_info_t*		m_pDvdReader;
#endif
//	dvr_ifo_handle_t*	m_pIfoHandle;
	unsigned int		m_nRecFlag;
//	dvd_file_t*			m_pMovieFile;
	dvd_file_t*			m_pStillFile;
	dvd_file_t*			m_pAudioFile;

	VR_NAV				m_oVrNav;
	byte				m_byPlayDomain;
	TM_INFO_QUEUE		m_tiTimePosQueue;
	REC_FLAG_ARRAY		m_RecFlag_Array;	 
	int					m_nAlloc_TM_ENT_Size;
	int					m_nAlloc_VOBU_ENT_Size;
	unsigned int		m_nDeliverCmd;
//	byte				m_byExtIFOType;		// neco+ 0:nurmal 1:Extend IFO
	int 				m_nDBCount;			 
    int                 m_currAPS;
    int                 m_currCGMS;
    int                 m_currAspectRatio;

    NAVPLAYBACKSTATUS 	m_stPbStatus;

	int64_t				m_i64VideoMixerRPTS;
	VIDEODECODECOMMAND	m_oVideoDecodeCmd;
	VIDEODECODETOBUFFER m_oVideoDecodeToBuffer;
	VIDEOMIXERBLEND	 	m_oVideoMixerBlend;
	VIDEODVDVOBUINFO	m_oVideoVobuInfo;
	byte				m_byClrPalette[16*4];
	AUDIODECODECOMMAND	m_oAudioDecodeCmd;

	int					m_nRecTitleNo;		 	
	
//	bool 				m_bIsAutoChapter;
	uint32_t 			m_nAutoMVOBUNs;
	
	osal_mutex_t*		m_pNavMutex;
	
	// decoding key frame
	int		m_nKeyFrameNum;
	vr_decode_keyframe_t *m_pDecKeyFrame;
	int		m_nDecKeyFrameIdx;
	
	// for repeat
	byte	m_byRepeat;		//0:no repeat, 1:repeat title 2:repeat chapter
							//3:repeat segment
	int		m_nRptPLNo;
	int		m_nRptPGNo;
	int		m_nRptEPNo;
	
	// for segment play
	SEG_INFO m_oSegInfo;
		
	int		m_nDecodeChannelIdx;
	int64_t	m_i64PendingRefPTSTick;

	// for live pause
	bool	m_bIsLivePause;
	int		m_nLPChapNs;
	bool	m_bIsAutoResetNewRecPG;
	bool	m_bPostRecCatchPbEvent;
	byte	m_byIsShortWait;
	//int 	m_nCatchDelayTime;  // VOBU Number
	
//=== for WriteVroFile====================
#if DVR_UDF_RW 
	unsigned char *m_tmp_data;
	unsigned char m_ptr1[2048 * (16 + 1)] ;
#endif	
//========================================

	CCritSec			m_csLock;
	byte				m_byIsNeedRealize;
	
public:
	//osal_mutex_t		m_omMutex;
	dvr_ifo_handle_t*	m_pIfoHandle;
	dvd_file_t*			m_pMovieFile;
	//static osal_mutex_t	m_omOpen_Mutex;
	CCritSec			m_csAutolock;
	//CCritSec			m_csRWLock;
	//osal_sem_t                 m_sRWSem;
	osal_mutex_t		m_omRWMutex;
	bool				m_bIsFlushing;
	bool    			m_bIsIdle_TimeShift;
	
 private:
	unsigned long           m_LivePauseFrameNsOffset;
	int 				m_nTimeShiftSafeRange;
	uint32_t			m_i32LastReadAddr;
	uint32_t			m_i32LastReadSize;
	dvd_file_t*			m_fLastReadFile;
	byte*				m_pbyReadBuf;
	int					m_nNeedFixEPNoCount;
	//bool				m_bIsPrintErrorMsg;
	
public:
	
	static uint32_t		dvrMAX_VOB_STI;
	static uint32_t		dvrMAX_C_EPI;
	static uint32_t		dvrMAX_C;
	static uint32_t		dvrMAX_M_C;
	static uint32_t		dvrMAX_S_C;
	static uint32_t		dvrMAX_M_VOB;
	static uint32_t		dvrMAX_S_VOG;
	static uint32_t		dvrMAX_ORG_PG;
	static uint32_t		dvrMAX_UD_PGC;
	static uint32_t		dvrMAX_PL_SRP;
	static uint32_t		dvrMAX_VOBU;	
	static uint32_t		dvrMAX_TM;
	static uint32_t		dvrMAX_SVOBU;
	static uint32_t		dvrMAX_C_EP_NS;
	
	static uint32_t		dvrUnit_MVOBU_sz;
	static uint32_t		dvrUnit_TM_sz;
	static uint32_t		dvrUnit_S_VOBU_sz;
	static uint32_t		dvrUnit_S_VOBU_TY_sz;
	static uint32_t		dvrUnit_MS_EPI_sz;
	
};


#endif // __CRTR_VMGI_H_INCLUDED


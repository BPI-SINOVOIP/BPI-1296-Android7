#ifndef _SI_FILTER_MGR_
#define _SI_FILTER_MGR_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif


//*************************************************************
// DEFINE
//*************************************************************
#if defined(WIN32)
	#define PAT_TERRA_TIMEOUT_TICK		1500
	#define	PMT_TERRA_TIMEOUT_TICK		500
	#define	VCT_TERRA_TIMEOUT_TICK		300

	#define PAT_CABLE_TIMEOUT_TICK		500
	#define	PMT_CABLE_TIMEOUT_TICK		200
	#define	VCT_CABLE_TIMEOUT_TICK		200
#elif defined(DVR)
	#define PAT_TERRA_TIMEOUT_TICK		200
	#define	PMT_TERRA_TIMEOUT_TICK		100
	#define	VCT_TERRA_TIMEOUT_TICK		50

	#define PAT_CABLE_TIMEOUT_TICK		200
	#define	PMT_CABLE_TIMEOUT_TICK		50
	#define	VCT_CABLE_TIMEOUT_TICK		50
#else
	#define PAT_TERRA_TIMEOUT_TICK		500
	#define	PMT_TERRA_TIMEOUT_TICK		300
	#define	VCT_TERRA_TIMEOUT_TICK		300

	#define PAT_CABLE_TIMEOUT_TICK		200
	#define	PMT_CABLE_TIMEOUT_TICK		150
	#define	VCT_CABLE_TIMEOUT_TICK		150
#endif

#define	EIT_TIMEOUT_TICK		400
#define	ETT_TIMEOUT_TICK		400
#define	RRT_TIMEOUT_TICK		1
#define	CAT_TIMEOUT_TICK		1

#define MAX_ACTIVE_PMT			12
#define MAX_ACTIVE_EVENT		4

//*************************************************************
// ENUM
//*************************************************************
typedef enum _SI_FILTER_STATUS {
	SI_FILTER_OFF		= 0,
	SI_FILTER_ON		= 1,
	SI_FILTER_TIMEOUT	= 2,
	SI_FILTER_RECEIVED	= 3,
} SI_FILTER_STATUS;


//*************************************************************
// STRUCT
//*************************************************************
typedef struct _SI_FILTER_STATE {
	UINT16				pid:13;
	UINT16				status:3;	// SI_FILTER_STATUS
	UINT16				progNum;
} SI_FILTER_STATE;

typedef struct _SI_ACTIVE_FILTER {
	UINT16				pid:13;
	UINT16				reserved:3;
	UINT16				tickCount;
	UINT8				sectionIndex;

#ifdef DVR
	UINT16				sectionRemainLen;
	UINT16				lastActive;			// Used to identify different tableId with same PID.
	int					tidExt;				// Used to identify different EIT/ETT under same PID/TID.
#endif
} SI_ACTIVE_FILTER;

typedef struct _SI_ACTIVE_TABLE {
	SI_ACTIVE_FILTER	pat;
	SI_ACTIVE_FILTER	pmt[MAX_ACTIVE_PMT];
	SI_ACTIVE_FILTER	cat;
	SI_ACTIVE_FILTER	tvct;
	SI_ACTIVE_FILTER	cvct;
	SI_ACTIVE_FILTER	rrt;
	SI_ACTIVE_FILTER	stt;
	SI_ACTIVE_FILTER	mgt;
	SI_ACTIVE_FILTER	eit[MAX_ACTIVE_EVENT];
	SI_ACTIVE_FILTER	ett[MAX_ACTIVE_EVENT];	
	SI_ACTIVE_FILTER	chEtt;

#ifdef ENABLE_EAM
	SI_ACTIVE_FILTER	eam;
#endif

#ifdef ENABLE_SDDS
	SI_ACTIVE_FILTER	sdds;
#endif

	UINT8				activePmtCount;
	UINT8				activeEitCount;
	UINT8				activeEttCount;
} SI_ACTIVE_TABLE;


typedef struct _SI_MASTER_TABLE {
	SI_FILTER_STATE		pat;
	SI_FILTER_STATE		pmt[MAX_PMT_COUNT];
	SI_FILTER_STATE		cat;
	SI_FILTER_STATE		tvct;
	SI_FILTER_STATE		cvct;
	SI_FILTER_STATE		rrt;
	SI_FILTER_STATE		stt;
	SI_FILTER_STATE		mgt;
	SI_FILTER_STATE		eit[MAX_EIT_COUNT];
	SI_FILTER_STATE		ett[MAX_ETT_COUNT];	
	SI_FILTER_STATE		chEtt;

#ifdef ENABLE_EAM
	SI_FILTER_STATE		eam;
#endif

#ifdef ENABLE_SDDS
	SI_FILTER_STATE		sdds;
#endif

	UINT8				maxPmtCount;// records total number of PMT known from PAT.
	UINT8				maxEitCount;// records total number of EIT known from MGT.
	UINT8				maxEttCount;// records total number of ETT known from MGT.
	UINT8				pmtEnabledCount; // counts the number of pmt enabled so far.
} SI_MASTER_TABLE;


typedef struct _SI_STATE_MANAGER {
	SI_ACTIVE_TABLE		activeTable;
	SI_MASTER_TABLE		masterTable;
} SI_STATE_MANAGER;




//*************************************************************
// PUBLIC FUNCTIONS
//*************************************************************
void SiFilterMgr_DumpStatus(TPMOD tpmod);

BOOL SiFilterMgr_Init(TPMOD tpmod);
BOOL SiFilterMgr_DeInit(TPMOD tpmod);
BOOL SiFilterMgr_DisableAllFilters(TPMOD tpmod);
BOOL SiFilterMgr_TickPlus(TPMOD tpmod);
BOOL SiFilterMgr_FilterTimeout(TPMOD tpmod);
BOOL SiFilterMgr_LoadPat(TPMOD tpmod);
BOOL SiFilterMgr_LoadMgt(TPMOD tpmod);


BOOL SiFilterMgr_EnablePat(TPMOD tpmod);
BOOL SiFilterMgr_DisablePat(TPMOD tpmod, SI_FILTER_STATUS status);

BOOL SiFilterMgr_EnablePmt(TPMOD tpmod, UINT16 pmtPid, UINT16 progNum);
BOOL SiFilterMgr_DisablePmt(TPMOD tpmod, UINT16 pmtPid, SI_FILTER_STATUS status);
BOOL SiFilterMgr_DisablePmtbyProgNum(TPMOD tpmod, UINT16 pmtPid, int progNum, SI_FILTER_STATUS status);
BOOL SiFilterMgr_EnablePmtWaiting(TPMOD tpmod);

BOOL SiFilterMgr_EnableVct(TPMOD tpmod);
BOOL SiFilterMgr_DisableTvct(TPMOD tpmod, SI_FILTER_STATUS status);
BOOL SiFilterMgr_DisableCvct(TPMOD tpmod, SI_FILTER_STATUS status);
BOOL SiFilterMgr_DisableVct(TPMOD tpmod, SI_FILTER_STATUS status);

BOOL SiFilterMgr_EnableMgt(TPMOD tpmod);
BOOL SiFilterMgr_DisableMgt(TPMOD tpmod, SI_FILTER_STATUS status);

BOOL SiFilterMgr_EnableEit(TPMOD tpmod, UINT16 eitPid);
BOOL SiFilterMgr_DisableEit(TPMOD tpmod, UINT16 eitPid, SI_FILTER_STATUS status);
BOOL SiFilterMgr_EnableEitWaiting(TPMOD tpmod);

BOOL SiFilterMgr_EnableEtt(TPMOD tpmod, UINT16 ettPid);
BOOL SiFilterMgr_DisableEtt(TPMOD tpmod, UINT16 ettPid, SI_FILTER_STATUS status);
BOOL SiFilterMgr_EnableEttWaiting(TPMOD tpmod);

BOOL SiFilterMgr_EnableRrt(TPMOD tpmod);
BOOL SiFilterMgr_DisableRrt(TPMOD tpmod, SI_FILTER_STATUS status);

BOOL SiFilterMgr_EnableCat(TPMOD tpmod);
BOOL SiFilterMgr_DisableCat(TPMOD tpmod, SI_FILTER_STATUS status);

BOOL SiFilterMgr_EnableStt(TPMOD tpmod);
BOOL SiFilterMgr_DisableStt(TPMOD tpmod, SI_FILTER_STATUS status);

#ifdef ENABLE_EAM
BOOL SiFilterMgr_EnableEam(TPMOD tpmod);
BOOL SiFilterMgr_DisableEam(TPMOD tpmod, SI_FILTER_STATUS status);
#endif

#ifdef ENABLE_SDDS
BOOL SiFilterMgr_EnableSdds(TPMOD tpmod);
BOOL SiFilterMgr_DisableSdds(TPMOD tpmod, SI_FILTER_STATUS status);
#endif

BOOL SiFilterMgr_IsFilterActive(TPMOD tpmod, UINT32 pid, UINT32 tid);

#ifdef DVR
BOOL SiFilterMgr_SetRemainSectionLen(TPMOD tpmod, UINT32 pid, UINT32 tid, int tidExt, UINT32 remainLen);
int SiFilterMgr_GetRemainSectionLen(TPMOD tpmod, UINT32 pid, int *tid, int *tidExt);
#endif //DVR

#endif


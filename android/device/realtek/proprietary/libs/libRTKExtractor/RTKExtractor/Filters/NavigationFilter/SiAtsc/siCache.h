#ifndef _SI_CACHE_H_
#define _SI_CACHE_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif


//*************************************************************
// STRUCT
//*************************************************************
typedef struct _SECTION_BUF {
	int				secNum;
	int 			version;
	int				packetId;
	int				tableId;
	int				tableIdExt;			// This is actually sectionNum for TVCT/CVCT
	INT8			lastActive;			// Used by Eit/Ett only (last used section_buf in multi-section buf).
	INT8			multiSectionCount;	// Used by Eit/Ett only (number of section_buf in multi-section buf).
	INT16			len;
	INT16			byteCount;
	UINT8*			data;
} SECTION_BUF;

typedef struct _MULTI_SECTION_BUF {
	INT16			packetId;
	INT16			tableId;
	INT32			count;
	SECTION_BUF*	list;	
} MULTI_SECTION_BUF;

typedef struct _SECTION_BUF_LIST {
	UINT16				majorChNum;	// used to identify the frequency channel.
	UINT16				phyChIndex;
	UINT32				byteUsed;

	SECTION_BUF			Pat;
	SECTION_BUF			Pmt[MAX_PMT_COUNT];
	SECTION_BUF			Mgt[2];		// for 2 sections.
	SECTION_BUF			Cat;
	SECTION_BUF			Stt;
	SECTION_BUF			Rrt;
	SECTION_BUF			ChEtt;		// channel ETT.

#ifdef ENABLE_EAM
	SECTION_BUF			Eam;
#endif

	MULTI_SECTION_BUF	Tvct;
	MULTI_SECTION_BUF	Cvct;
	MULTI_SECTION_BUF 	Eit[MAX_EIT_COUNT];	
	MULTI_SECTION_BUF	Ett[MAX_EIT_COUNT];
	
	UINT16				pmtCount;
	UINT8				eitCount;
	UINT8				ettCount;
	UINT8				tvctMaxSecCount;
	UINT8				cvctMaxSecCount;
} SECTION_BUF_LIST;


// This table records many SECTION_BUF_LIST. We basically use it to cache more EIT/ETT sections.
typedef struct _SECTION_BUF_TABLE {
	UINT32				count;
	int					activeIndex;		// records the list that is currently active.
	SECTION_BUF_LIST*	sectionBufList;
} SECTION_BUF_TABLE;

void SiCache_DumpSection(TPMOD tpmod, SECTION_BUF *buf);
void SiCache_DumpSectionList(TPMOD tpmod);
BOOL SiCache_Init(TPMOD tpmod, SI_CACHE_LEVEL cacheLevel);
void SiCache_DeInit(TPMOD tpmod);
void SiCache_SetActiveChannel(TPMOD tpmod, UINT16 majorNum, UINT16 phyChIndex);
void SiCache_ResetActive(TPMOD tpmod);
void SiCache_ResetAll(TPMOD tpmod);

void SiCache_ResetPat(TPMOD tpmod);
void SiCache_ResetEam(TPMOD tpmod);
void SiCache_ResetPmt(TPMOD tpmod);
void SiCache_ResetCat(TPMOD tpmod);
void SiCache_ResetRrt(TPMOD tpmod);
void SiCache_ResetMgt(TPMOD tpmod);
void SiCache_ResetVct(TPMOD tpmod);
void SiCache_ResetEit0(TPMOD tpmod);
void SiCache_ResetEit(TPMOD tpmod);
void SiCache_ResetEtt(TPMOD tpmod);
void SiCache_CleanupMultiSection(TPMOD tpmod, int packetId, int tableId);
void SiCache_FreeSection(TPMOD tpmod, SECTION_BUF **buf);
void SiCache_FreeIfIncomplete(TPMOD tpmod, SECTION_BUF **buf);
SECTION_BUF *SiCache_GetSection(TPMOD tpmod, UINT32 packetId, UINT32 tableId, int tableIdExt, int secNum);
SECTION_BUF *SiCache_GetSectionEx(TPMOD tpmod, UINT32 packetId, UINT32 tableId, int tableIdExt, int secNum, BOOL getExistingOnly);

BOOL SiCache_CvctExists(TPMOD tpmod);
int SiCache_GetTvctCount(TPMOD tpmod);
int SiCache_GetCvctCount(TPMOD tpmod);
SECTION_BUF *SiCache_GetTvctTable(TPMOD tpmod, int cachePosition);
SECTION_BUF *SiCache_GetCvctTable(TPMOD tpmod, int cachePosition);
BOOL SiCache_SetMaxSecNum(TPMOD tpmod, int tableId, UINT8 maxSecNum);
BOOL SiCache_GetSecNumStatus(TPMOD tpmod, int tableId, UINT8 *secNum, UINT8 *maxSecNum);

BOOL SiCache_AddActiveCacheSize(TPMOD tpmod, UINT32 size);
BOOL SiCache_MinusActiveCacheSize(TPMOD tpmod, UINT32 size);
BOOL SiCache_GetActiveCacheSize(TPMOD tpmod, UINT32 *size);
BOOL SiCache_IsCacheSizeOk(TPMOD tpmod);
BOOL SiCache_TrimCacheSize(TPMOD tpmod);


// This are for EPG query only. No one else should be using these.
BOOL SiCache_GetEitMultiSection(MULTI_SECTION_BUF *multiSection, TPMOD tpmod, UINT16 tableType, UINT16 phyChIndex, UINT16 sourceId);
BOOL SiCache_GetEttMultiSection(MULTI_SECTION_BUF *multiSection, TPMOD tpmod, UINT16 tableType, UINT16 phyChIndex, UINT16 sourceId);
BOOL SiCache_FreeEitSection(SECTION_BUF *section);
BOOL SiCache_FreeMultiSection(MULTI_SECTION_BUF *multiSection);


#ifdef TEST_MODE
BOOL SiCacheTest_RipActiveList(TPMOD tpmod);
#endif



#endif

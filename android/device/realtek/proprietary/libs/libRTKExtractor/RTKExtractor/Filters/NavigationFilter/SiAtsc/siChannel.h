#ifndef _SI_CHANNEL_H_
#define _SI_CHANNEL_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif


//*************************************************************
// STRUCT
//*************************************************************
typedef struct _CH_INDEX {
	UINT32			frequency;
	UINT16			majorNum;
	UINT16			minorNum;
	int				phyChIndex;
	int				virChIndex;
	int				selfIndex;		// Used to record this structure's position in chIndexList
} CH_INDEX;

typedef struct _SI_VIRTUAL_CHANNEL {

	UINT16			majorNum;
	UINT16			minorNum;

	UINT8			shortName[CH_SHORTNAME_LEN+2];
	UINT8			charSetId;
	UINT8			serviceType:4;
	UINT8			isAc3Audio:2;
	UINT8			isUserAdded:1;
	UINT8			isCvct:1;
	UINT8			audioCount;		// records the number of audio channels in audioList.

	UINT8			hasAtscCC:1;
	UINT8			hasNtscCC:1;
	UINT8			isScrambled:1;
	UINT8			isHidden:1;
	UINT8			hideGuide:1;
	UINT8			redistributionControl:1;
	UINT8			isFavorite:1;
	UINT8			gotVctUpdate:1;

	// These 8 bits are for user configuration.
	UINT8			isUserDeleted:1;
	UINT8			isUserLocked:1;
	UINT8			isUserIgnored:1;
	UINT8			modulation:5;

	UINT8			hasUserAudioIndex:1;// This records if the user has selected own audio index.
	UINT8			userAudioIndex:7;	// This records the audio index that user selects.
	UINT16			progNum;
	int				pmtVersion;

	UINT8			videoStreamType;
	UINT8			vc1Profile;
	UINT16			chTsId;
	UINT8			isDifferentTsId:1;

#ifdef DVR
	UINT8			hasUserSubtitleIndex:1; // This records if the user has selected own subtitle index.
	UINT8			userSubtitleIndex:7;	// This records the subtitle index that user selects.
	UINT8			hasUserTeletextIndex:1; // This records if the user has selected own teletext index.
	UINT8			userTeletextIndex:7;	// This records the teletext index that user selects.

	UINT8			subtitleCount;
	UINT8			teletextCount;
#endif
	
	UINT16			pmtPid;
	UINT16			sourceId;
	UINT16			pcrPid;
	UINT16			videoPid;

	UINT16			timeshiftBaseMajor;
	UINT16			timeshiftBaseMinor;
	UINT16			minuteShifted;		// from timeshifted descriptor.

	SI_SDDS_LIST	sddsList;
	SI_AUDIO		*audioList;

#ifdef DVR
	SI_SUBTITLE*	subtitleList;
	SI_TELETEXT*	teletextList;
#endif

} SI_VIRTUAL_CHANNEL;


typedef struct _SI_PHYSICAL_CHANNEL {

	UINT32			frequency;
	UINT32			bandwidth;
	UINT32			phyChNum;
	int				tsId;
	int				patVersion;
	int				vctVersion;
	int				virChCount;

	SI_VIRTUAL_CHANNEL*	virChList;

} SI_PHYSICAL_CHANNEL;


typedef struct _SI_CHANNEL_MANAGER {

	UINT16				activePhyChIndex;
	UINT8				rebuildChIndex;
	UINT8				saveToFile;
	UINT16				phyChCount;
	UINT16				virChCount;

	SI_PHYSICAL_CHANNEL*phyChList;	// This is the physical number list.
	CH_INDEX*			chIndexList; // This is the sort chNum list

} SI_CHANNEL_MANAGER;


//*************************************************************
// EXTERN
//*************************************************************
extern SI_CHANNEL_MANAGER		siChManager[TP_MOD_COUNT];
extern SI_PHYSICAL_CHANNEL		streamChangeBuf[TP_MOD_COUNT];

//*************************************************************
// PUBLIC FUNCTIONS
//*************************************************************
BOOL SiChannel_Init(TPMOD tpmod);
BOOL SiChannel_DeInit(TPMOD tpmod);
BOOL SiChannel_ResetContent(TPMOD tpmod);
BOOL SiChannel_ResetChMgr(TPMOD tpmod);
BOOL SiChannel_ResetStreamChangeBuf(TPMOD tpmod);
BOOL SiChannel_ResetPhyCh(TPMOD tpmod, UINT16 phyChIndex);
BOOL SiChannel_BuildChIndex(TPMOD tpmod);

void SiChannel_DumpChList(TPMOD tpmod);
BOOL SiChannel_UpdatePhyCh(TPMOD tpmod);
BOOL SiChannel_ApplyStreamChangeResult(TPMOD tpmod);
BOOL SiChannel_SaveToFile(TPMOD tpmod);

int SiChannel_GetActiveFreqCount(TPMOD tpmod);
int SiChannel_GetTotalVirChCount(TPMOD tpmod);
int SiChannel_GetVirChCount(TPMOD tpmod, UINT32 phyChIndex);
int SiChannel_GetTotalVirChCountEx(TPMOD tpmod, SI_CHANNEL_FILTER filter);
int SiChannel_GetPhyChNum(TPMOD tpmod, UINT32 phyChIndex);
int SiChannel_GetPhyChIndex(TPMOD tpmod, UINT32 frequency);
BOOL SiChannel_IsReady(TPMOD tpmod);
int SiChannel_GetTsId(TPMOD tpmod, UINT16 phyChIndex);
BOOL SiChannel_CheckForCvct(TPMOD tpmod, UINT32 phyChIndex);

int SiChannel_GetPatVersion(TPMOD tpmod, UINT32 phyChIndex);
int SiChannel_GetPmtVersion(TPMOD tpmod, UINT32 phyChIndex, UINT16 pmtPid, UINT16 progNum);

BOOL SiChannel_InsertCustomAtvList(TPMOD tpmod, SI_ATV *atvList, UINT32 count);
BOOL SiChannel_RemoveCustomAtvList(TPMOD tpmod);

#endif

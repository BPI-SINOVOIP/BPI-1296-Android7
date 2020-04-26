#ifndef _SI_TABLE_H_
#define _SI_TABLE_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif




void SiTable_FreePat(PAT_CONTENT *patContent);
void SiTable_FreePmt(PMT_CONTENT *pmtContent);
void SiTable_FreeCat(CAT_CONTENT *catContent);
void SiTable_FreeVct(VCT_CONTENT *vctContent);
void SiTable_FreeMgt(MGT_CONTENT *mgtContent);
void SiTable_FreeStt(STT_CONTENT *sttContent);
void SiTable_FreeAtscEit(ATSC_EIT_CONTENT *eitContent);
void SiTable_FreeEtt(ETT_CONTENT *ettContent);
void SiTable_FreeRrt(RRT_CONTENT *rrtContent);

#ifdef ENABLE_EAM
void SiTable_FreeEam(EAM_CONTENT *eamContent);
EAM_CONTENT* EAMparser (const UINT16 tidExt, const UINT8* buf, UINT16 len);
#endif

PAT_CONTENT* PATparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
CAT_CONTENT* CATparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
PMT_CONTENT* PMTparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
VCT_CONTENT *TVCTparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
VCT_CONTENT *CVCTparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
MGT_CONTENT *MGTparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
STT_CONTENT *STTparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
ATSC_EIT_CONTENT *AtscEITparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
ETT_CONTENT *ETTparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);
RRT_CONTENT *RRTparser(const UINT16 tidExt, const UINT8* buf, UINT16 len);

MGT_CONTENT *combineMGT(MGT_CONTENT *mgtContent0, MGT_CONTENT *mgtContent1);
#endif

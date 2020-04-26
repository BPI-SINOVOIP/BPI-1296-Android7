#ifndef _SI_TP_H_
#define _SI_TP_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif

#ifdef DVR
SI_ERRCODE SiTp_ClearTpBuffer(TPMOD tpmod);
BOOL SiTp_SetAudioPcr(TPMOD tpmod);
#else
TP_Handle* SiTp_GetHandle(TPMOD tpmod);
SI_ERRCODE SiTp_SetSource(TPMOD tpmod, TS_SOURCE tssrc);
SI_ERRCODE SiTp_RemoveSource(TPMOD tpmod);
SI_ERRCODE SiTp_ClearTpBuffer(TPMOD tpmod);
SI_ERRCODE SiTp_Reset(TPMOD tpmod);
SI_ERRCODE SiTp_ResetTP0();
SI_ERRCODE SiTp_ResetTP1();

BOOL SiTp_SetAudioPcr(TPMOD tpmod);
#endif

#endif

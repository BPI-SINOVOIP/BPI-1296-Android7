#ifndef _SI_DVR_TS_API_H_
#define _SI_DVR_TS_API_H_

#if defined(WIN32) | defined(DVR)
	#include "rtd_types.h"
	#include "SiEngine.h"
#else
	#include <SiEngine.h>
#endif

#include "siApi.h"

BOOL SiDvrTsApi_Init(TPMOD tpmod, UINT32 tsHandle, SI_ENGINE_OUT *tsFuncPtrs);
BOOL SiDvrTsApi_DeInit(TPMOD tpmod);
BOOL SiDvrTsApi_IsInit(TPMOD tpmod);

int SiDvrTsApi_InsertFilter(TPMOD			tpmod, 
							UINT32			pid, 
							BOOL			active, 
							SI_FILTER_TYPE	type, 
							SI_CODEC_TYPE	codecType,
							void *			privateData,
							int				privateDataSize);
int SiDvrTsApi_InsertFilterList(TPMOD tpmod, UINT32 count, SI_FILTER_DESC *filterList);
int SiDvrTsApi_RemoveFilter(TPMOD tpmod, UINT32 pid);  
int SiDvrTsApi_RemoveFilterList(TPMOD tpmod, UINT32 count, UINT32 *pidList);
int SiDvrTsApi_RemoveAllFilters(TPMOD tpmod);
int SiDvrTsApi_ResetPosition(TPMOD tpmod);
int SiDvrTsApi_SetActiveComponent(TPMOD tpmod, SI_FILTER_TYPE type, UINT16 pid);    
//int SiDvrTsApi_PostEvent(TPMOD tpmod, UINT32 eventId, UINT8 *eventData, UINT32 eventSize);     
int SiDvrTsApi_SetSiState(TPMOD tpmod);
SI_SCRAMBLE_CHECK_RESULT SiDvrTsApi_CheckScrambled(unsigned int tpmod, int pid, int *pIsScramble);

// This function is basically for TS engine to call only.
void SiDvrTsApi_Flush(unsigned int tpmod);
int SiDvrTsApi_StoreSiState(unsigned int tpmod, unsigned char *pStateData, unsigned int bytes);
int SiDvrTsApi_StorePackets(unsigned int tpmod, unsigned char *buf, unsigned int bufLen);
int SiDvrTsApi_GetPackets(TPMOD tpmod, UINT8 **buf, UINT32 *bufLen);


int SiDvrTsApi_ResetChannel(unsigned int tpmod);
#endif

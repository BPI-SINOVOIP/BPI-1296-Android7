#ifndef _SI_EVENT_H_
#define _SI_EVENT_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif



//*************************************************************
// STRUCT 
//*************************************************************




//*************************************************************
// PUBLIC FUNCTIONS 
//*************************************************************
BOOL SiEvent_GetCurProgram(SI_EVENT *event, TPMOD tpmod);
BOOL SiEvent_GetNextProgram(SI_EVENT *event, TPMOD tpmod);
BOOL SiEvent_GetEpg(SI_MULTI_EVENT *multiEvent, TPMOD tpmod, SI_EVENT_PARAM param);

BOOL SiEvent_FreeMultiEventList(SI_MULTI_EVENT *multiEvent);
BOOL SiEvent_DupeMultiEventList(SI_MULTI_EVENT *dest, SI_MULTI_EVENT *src);
BOOL SiEvent_FreeEvent(SI_EVENT *event);
BOOL SiEvent_DupeEvent(SI_EVENT *dest, SI_EVENT *src);

//BOOL SiEvent_ConvertIntToDateStr(UINT8* dateStr, int intTime);
//BOOL SiEvent_ConvertIntToDate(SI_EVENT_DATETIME* date, int intTime);

void SiEvent_DumpMultiEventList(SI_MULTI_EVENT *multiEvent);


BOOL SiEvent_ReservationInit(TPMOD tpmod);
BOOL SiEvent_ReservationLoad(TPMOD tpmod);
BOOL SiEvent_ReservationSave(TPMOD tpmod);

BOOL SiEvent_SyncReservation(TPMOD tpmod);
BOOL SiEvent_IsReserved(TPMOD tpmod, SI_EVENT *event);
SI_ERRMSG SiEvent_AddReservation(TPMOD tpmod, SI_EVENT *event);
BOOL SiEvent_RemoveReservation(TPMOD tpmod, SI_EVENT *event);
BOOL SiEvent_RemoveAllReservation(TPMOD tpmod);
BOOL SiEvent_GetReservationList(TPMOD tpmod, SI_MULTI_EVENT *multiEvent);

BOOL SiEvent_SetExpiredEvent(TPMOD tpmod, UINT32 expireTime);
BOOL SiEvent_GetExpiredEvent(TPMOD tpmod, SI_EVENT *event);


#endif

#ifndef _SI_MESSAGE_H_
#define _SI_MESSAGE_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif


//*************************************************************
// DEFINE
//*************************************************************


//*************************************************************
// PUBLIC FUNCTIONS
//*************************************************************
BOOL SiMessage_Init(TPMOD tpmod);
BOOL SiMessage_Reset(TPMOD tpmod);
BOOL SiMessage_DeInit(TPMOD tpmod);
BOOL SiMessage_Store(TPMOD tpmod, SI_MESSAGE message, SI_UINT32 data);
BOOL SiMessage_Get(TPMOD tpmod, SI_MESSAGE *pMessage, SI_UINT32 *pData);
BOOL SiMessage_PeekNewest(TPMOD tpmod, SI_MESSAGE *pMessage, SI_UINT32 *pData);
BOOL SiMessage_Remove(TPMOD tpmod, SI_MESSAGE message);

#endif //_SI_MESSAGE_H_

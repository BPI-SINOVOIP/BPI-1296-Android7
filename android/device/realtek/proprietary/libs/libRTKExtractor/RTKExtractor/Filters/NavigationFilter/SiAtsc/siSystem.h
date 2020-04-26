#ifndef _SI_SYSTEM_H_
#define _SI_SYSTEM_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif


//*************************************************************
// DEFINE
//*************************************************************
typedef struct _SITEST_MESSAGE_BUF {
	UINT8	gotEit0;
	UINT8	streamChangeBegin;
	UINT8	streamChangeEnd;
	UINT8	resetChannel;
	UINT8	channelReady;	
} SITEST_MESSAGE_BUF;


#ifdef TEST_MODE
extern SITEST_MESSAGE_BUF siMessageBuf;
#endif

//*************************************************************
// PUBLIC FUNCTIONS
//*************************************************************
SI_ERRCODE SiSystem_Init(TPMOD tpmod);
SI_ERRCODE SiSystem_DeInit(TPMOD tpmod);
SI_ERRCODE SiSystem_FreeGlobalData(TPMOD tpmod);
SI_ERRCODE SiSystem_StopActivity(TPMOD tpmod, BOOL freeGlobalData);
SI_ERRCODE SiSystem_SendMsg(TPMOD tpmod, SI_MESSAGE message);

#ifdef ENABLE_SDDS
SI_ERRCODE SiSystem_SendSddsData(const UINT8 *buf, UINT32 len);
#endif

void SiSystem_LockPoller();
void SiSystem_UnLockPoller();
void SiSystem_Lock();
void SiSystem_UnLock();


#endif

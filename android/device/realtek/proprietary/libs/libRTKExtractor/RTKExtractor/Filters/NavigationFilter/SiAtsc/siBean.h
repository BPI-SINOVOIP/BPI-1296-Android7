#ifndef _SI_BEAN_H_
#define _SI_BEAN_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif

SI_ERRCODE SiBean_Init(TPMOD tpmod);
SI_ERRCODE SiBean_DeInit(TPMOD tpmod);
int SiBean_PsiQueuePoller(TPMOD tpmod, UINT8 qidx);
void SiBean_DumpStatistics(TPMOD tpmod);

#endif

#ifndef _SI_BEAN_GEN_H_
#define _SI_BEAN_GEN_H_

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
BOOL SiBeanGen_Convert(TPMOD tpmod, UINT8 **dstBuf, UINT32 *dstLen, UINT8 **srcbuf, UINT32 *srcLen);


#endif

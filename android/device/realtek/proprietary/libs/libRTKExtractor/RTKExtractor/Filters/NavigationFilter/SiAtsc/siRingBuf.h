#ifndef _SI_RING_BUF_H_
#define _SI_RING_BUF_H_


#include "siApi.h"


//*************************************************************
// STRUCT
//*************************************************************
typedef struct _SI_RING_BUF {
	UINT32			len;
	UINT8*			startPtr;
	UINT8*			endPtr;
	UINT8*			readPtr;
	UINT8*			writePtr;
	MUTEX			mutex;
	UINT32			init;
} SI_RING_BUF;


//*************************************************************
// PUBLIC FUNCTIONS
//*************************************************************
BOOL SiRingBuf_Init(SI_RING_BUF *ringBuf, UINT32 len);
BOOL SiRingBuf_DeInit(SI_RING_BUF *ringBuf);
BOOL SiRingBuf_Reset(SI_RING_BUF *ringBuf);

BOOL SiRingBuf_Write(SI_RING_BUF *ringBuf, UINT8 *buf, UINT32 buflen);
BOOL SiRingBuf_Read(SI_RING_BUF *ringBuf, UINT8 **buf, UINT32 *buflen);

#endif

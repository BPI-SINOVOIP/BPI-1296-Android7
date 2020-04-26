#ifndef _SI_RING_BUF_H_
#define _SI_RING_BUF_H_


#include <ss_typedef4integration.h>


//*************************************************************
// STRUCT
//*************************************************************

typedef struct SI_RING_BUF				SI_RING_BUF;
struct SI_RING_BUF {
	UINT32			len;
	UINT8*			startPtr;
	UINT8*			endPtr;
	UINT8*			readPtr;
	UINT8*			writePtr;
	UINT32			init;
	SI_MUTEX		mutex;
};


//*************************************************************
// PUBLIC FUNCTIONS
//*************************************************************
ErrCode SI_RingBuf_Init(SI_RING_BUF *ringBuf, UINT32 len);
ErrCode SI_RingBuf_DeInit(SI_RING_BUF *ringBuf);
ErrCode SI_RingBuf_Reset(SI_RING_BUF *ringBuf);
ErrCode SI_RingBuf_Write(SI_RING_BUF *ringBuf, UINT8 *buf, UINT32 buflen);
ErrCode SI_RingBuf_Read(SI_RING_BUF *ringBuf, UINT8 **buf, UINT32 *buflen);
ErrCode SI_RingBuf_ReSize(SI_RING_BUF *ringBuf, int new_len);

#endif

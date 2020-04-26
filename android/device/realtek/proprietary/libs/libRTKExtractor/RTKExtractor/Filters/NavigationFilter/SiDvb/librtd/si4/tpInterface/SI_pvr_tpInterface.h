
#ifndef _SI_PVR_TP_INTERFACE_H
#define _SI_PVR_TP_INTERFACE_H

#include <system/si4/tpInterface/TPInterface.h>

//------------------------------------------------------------------------------
//
//	Include Files
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//
//	Definitions
//
//------------------------------------------------------------------------------
#define SI_TP_MAKE_ERRCODE(code) (0x1e0000 | (code&0x00ff))

typedef enum
{
	SI_TP_ERR_OK = 0,

	SI_TP_ERR_BADPARAMETER	= SI_TP_MAKE_ERRCODE(0x01),
	SI_TP_ERR_INVALID_PID		= SI_TP_MAKE_ERRCODE(0x02),
	SI_TP_ERR_BEAN_PID_FULL	= SI_TP_MAKE_ERRCODE(0x03),
	//SI_TP_ERR_BEAN_RE_ENABLE_PID	= SI_TP_MAKE_ERRCODE(0x04),

	SI_TP_ERR_UNDEFINED		= SI_TP_MAKE_ERRCODE(0xff),

} SI_TP_ERRORCODE;

//------------------------------------------------------------------------------
//
//	Functions
//
//------------------------------------------------------------------------------

ErrCode SI_TPInterface_New
(
	void								**ppTP,
	unsigned int						privateHandle,
	void								*funcs
);

ErrCode SI_TPInterface_Destroy
(
	void								**ppTP
);

ErrCode SI_TPInterface_Reset
(
	void								*tp,
	unsigned int						index
);

ErrCode SI_TPInterface_CleanBuffer
(
	void								*tp,
	unsigned int						index
);

ErrCode SI_TPInterface_Lock
(
	void								*tp
);

ErrCode SI_TPInterface_Unlock
(
	void								*tp
);

ErrCode SI_TPInterface_SetFilter
(
	void								*tp,
	unsigned int						pid,
	SI_FILTER_TYPE						type,
	SI_CODEC_TYPE						codecType
);

ErrCode SI_TPInterface_SetFilterList
(
	void								*tp,
	unsigned int  						count,
	SI_FILTER_DESC* 					filterList
);

ErrCode SI_TPInterface_SetActivateComponent
(
	void								*tp,
	SI_FILTER_TYPE 						type,
	int									pid
);

ErrCode SI_TPInterface_RemoveFilter
(
	void								*tp,
	unsigned int						pid,
	SI_FILTER_TYPE						type
);

ErrCode SI_TPInterface_Remove_NonSIFilterType
(
	void								*tp,
	SI_FILTER_TYPE						type
);

ErrCode SI_TPInterface_RemoveNonSIFilterList
(
	void						*tp,
	unsigned int  				count,
	unsigned int* 				pidArray
);


ErrCode SI_TPInterface_RemoveAllFilter
(
	void								*tp
);

ErrCode SI_TPInterface_GetBeanQueue
(
	void								*tp,
	unsigned int						index,
	unsigned int						*pQueueLen,
	unsigned char						**ppQueue
);

ErrCode SI_TPInterface_DropBeanQueue
(
	void								*tp,
	unsigned char						*queue
);

ErrCode SI_TPInterface_ResetFilePosition
(
	void								*tp
);

ErrCode SI_TPInterface_ReSizeRingBuf
(
	void								*tp,
	int 							new_len
);

//------------------------------------------------------------------------------
//
//	Functions for PVR
//
//------------------------------------------------------------------------------
ErrCode SI_TPInterface_SetSIState( void *tp, unsigned char* pStateData, unsigned int bytes);
ErrCode SI_TPInterface_PostEvent(void *tp, SI_MESSAGE msg);

void SI_PVRTP_Flush(unsigned int tp);
int SI_PVRTP_StorePackets(unsigned int tp, unsigned char *buf, unsigned int bufLen);

int SI_TPInterface_FeedbackPrivateInfo(unsigned int tp, SI_PRIVATE_INFO_ID id, long infoLen, unsigned char* pInfo);	// bug 28532


#endif//_SI_PVR_TP_INTERFACE_H


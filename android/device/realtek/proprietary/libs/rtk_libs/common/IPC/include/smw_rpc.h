 
#ifndef __C_RPC_H__
#define __C_RPC_H__

#include "amw_rpc.h"

#ifndef RPC_MECHANISM
 
/////////////////////////////////////////////////////
#ifdef  __cplusplus
extern "C" {
#endif
	
/////////////////////////////////////////////////////
#define RPC_TOSYSTEM_ENDOFSTREAM(x,y,z)		RPC_SystemEndOfStream(x,y,z)
#define RPC_TOSYSTEM_DELIVERFLUSH(x,y,z)	RPC_SystemDeliverFlush(x,y,z)
/////////////////////////////////////////////////////


#define RPC_TOSYSTEM_ENDOFSTREAM_SVC		RPC_SystemEndOfStream
#define RPC_TOSYSTEM_DELIVERFLUSH_SVC		RPC_SystemDeliverFlush

/////////////////////////////////////////////////////
// ny add
#define		RPC_TOSYSTEM_ENC_GENERALINFO(x,y,z)	RPC_ENC_GeneralInfo(x,y,z)	
#define		RPC_TOSYSTEM_ENC_FRAMEINFO(x,y,z)	RPC_ENC_FrameInfo(x,y,z)
#define		RPC_TOSYSTEM_ENC_ENDOFSTREAM(x,y,z)	RPC_SystemEndOfStream(x,y,z)
#define 	RPC_TOSYSTEM_DAC_HARDWAREMUTE(x,y,z) RPC_DAC_HardwareMute_0(x,y,z)
//HRESULT*	RPC_ENC_GeneralInfo(AUDIO_RPC_ENC_GEN_INFO* geninfo, CLNT_STRUCT* clnt,HRESULT *retval);
//HRESULT*	RPC_ENC_FrameInfo(AUDIO_RPC_ENC_FRAME_INFO* frameinfo, CLNT_STRUCT* clnt,HRESULT *retval);
//HRESULT*	RPC_SystemENCEndOfStream(AUDIO_RPC_SENDPIN_LONG* pSendIO, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*	RPC_ENC_GeneralInfo(long iid, CLNT_STRUCT* clnt,HRESULT *retval);
HRESULT*	RPC_ENC_FrameInfo(long iid, CLNT_STRUCT* clnt,HRESULT *retval);
HRESULT*	RPC_SystemENCEndOfStream(long iid, RPC_STRUCT* clnt, HRESULT *retval);

///////////////////////////////////////////////////////////////////////////////////

HRESULT*	RPC_SystemEndOfStream(AUDIO_RPC_SENDPIN_LONG* pSendIO, RPC_STRUCT* clnt, HRESULT *retval);
HRESULT*	RPC_SystemDeliverFlush(AUDIO_RPC_SENDIO* pSendIO, RPC_STRUCT* clnt, HRESULT *retval);

/////////////////////////////////////////////////////////////////////////////////
HRESULT*	AUDIO_RPC_ToSystem_EndOfStream_0();
HRESULT*	AUDIO_RPC_ToSystem_HDMI_Setting_0();
HRESULT*	AUDIO_RPC_ToSystem_ENC_EndOfStream_0();
HRESULT*	AUDIO_RPC_ToSystem_DAC_HardwareMute_0();
HRESULT*	AUDIO_RPC_ToSystem_ENC_GeneralInfo_0();
HRESULT*	AUDIO_RPC_ToSystem_ENC_FrameInfo_0();
HRESULT*	AUDIO_RPC_ToSystem_ErrorStatus_0();
HRESULT*	AUDIO_RPC_ToSystem_AudioHaltDone_0();

#ifdef  __cplusplus
}
#endif

#else

#define RPC_TOSYSTEM_ENC_ENDOFSTREAM(x,y,z)	AUDIO_RPC_ToSystem_ENC_EndOfStream_0(x,y)
#define RPC_TOSYSTEM_ENDOFSTREAM(x,y,z)		AUDIO_RPC_ToSystem_EndOfStream_0(x,y)
#define RPC_TOSYSTEM_DELIVERFLUSH(x,y,z)	AUDIO_RPC_ToSystem_DeliverFlush_0(x,y)

#define RPC_TOSYSTEM_ENDOFSTREAM_SVC		AUDIO_RPC_ToSystem_EndOfStream_0_svc
#define RPC_TOSYSTEM_ENC_ENDOFSTREAM_SVC	AUDIO_RPC_ToSystem_ENC_EndOfStream_0_svc
#define RPC_TOSYSTEM_DELIVERFLUSH_SVC		AUDIO_RPC_ToSystem_DeliverFlush_0_svc

//ny add
#define RPC_TOSYSTEM_ENC_GENERALINFO(x,y,z)	AUDIO_RPC_ToSystem_ENC_GeneralInfo_0(x,y)
#define RPC_TOSYSTEM_ENC_FRAMEINFO(x,y,z)	AUDIO_RPC_ToSystem_ENC_FrameInfo_0(x,y)
#define RPC_TOSYSTEM_DAC_HARDWAREMUTE(x,y,z) AUDIO_RPC_ToSystem_DAC_HardwareMute_0(x,y)
											
#endif
#endif


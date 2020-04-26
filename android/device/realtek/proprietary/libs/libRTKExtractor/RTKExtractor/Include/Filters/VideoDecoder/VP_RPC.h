 
#ifndef __VP_RPC_H__
#define __VP_RPC_H__

#include <rpc_common.h>
#include <RPCBaseDS_data.h>
#include <VideoRPCBaseDS.h>
#include <VideoRPC_System.h>
#include <VideoRPC_Agent.h>
#include <InbandAPI.h>

struct VP_RPC_INSTANCE {
	long	type;          			//The module type, such as VIDEO_ENCODER, VIDEO_DECODER, 
			       					//SPU_DECODER, VIDEO_IN, VIDEO_OUT
	long 	system_filter_instanceID ; /*The corresponding filter on system side. */

};

inline HRESULT RETURN_RPC_RESULT(HRESULT* res)
{
    DASSERT(res);
    if (*res != S_OK)
	{			
		free(res);			
		return E_FAIL;
	}
	free(res);
	return S_OK;
}


#ifdef USE_VIDEO_DECODER_SIMULATOR

inline CLNT_STRUCT RETURN_EFAIL(int A, int B, int C)	
{
		CLNT_STRUCT res;
		return res;
}

inline HRESULT* RETURN_RESULT()
{
	HRESULT* res = (HRESULT*) malloc(sizeof(HRESULT));
	*res = S_OK;
	return res;
}


#define VP_RPC_PREPARE_CLNT(A, B, C)						RETURN_EFAIL(A, B, C)
#define VP_RPC_TOAGENT_CREATE(instance, clnt)				VP_RPC_Create(instance, clnt)
#define VP_RPC_TOAGENT_DESTROY(instanceID, clent)			VP_RPC_Destroy(instanceID, clent)
#define VP_RPC_TOAGENT_INITRINGBUFFER(ringBuffer, clent)	VP_RPC_InitRingBuffer(ringBuffer, clent)
#define VP_RPC_TOAGENT_RUN(instanceID, clent)				VP_RPC_Run(instanceID, clent)
#define VP_RPC_TOAGENT_PAUSE(instanceID, clent)				VP_RPC_Pause(instanceID, clent)
#define VP_RPC_TOAGENT_STOP(instanceID, clent)				VP_RPC_Stop(instanceID, clent)
#define VP_RPC_TOAGENT_SETREFCLOCK(pRefClock, clent)    	VP_RPC_SetRefClock(pRefClock, clent)
#define VP_RPC_TOAGENT_CONNECT(connection, clnt)			VP_RPC_Connect(connection, clnt)
#define VP_RPC_TOAGENT_FLUSH(instanceID, clnt)				VP_RPC_Flush(instanceID, clnt)
#define	VP_RPC_TOAGENT_SET_SEEKING(info, clnt)				VP_RPC_SetSeeking(info, clnt)	

#define VP_RPC_VOUT_TOAGENT_SET_VOLAYER(info, clnt)			VP_RPC_Vout_SetVoutLayer(info, clnt)
#define VP_RPC_VOUT_TOAGENT_SET_SEEKING(info, clnt)			VP_RPC_Vout_SetSeeking(info, clnt)
#define VP_RPC_VOUT_TOAGENT_SET_STEP(instanceID, clent)		VP_RPC_Vout_Step(instanceID, clent)

#define VP_RPC_VOUT_TOAGENT_SET_PRESENTATION_MODE(info, clnt)	RETURN_RESULT()
//#define VP_RPC_VOUT_TOAGENT_DISPLAY_BORDER(instanceID, clent)	RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_SHOW_STILL_PIC(info, clnt)			RETURN_RESULT()
#define VP_RPC_TOAGENT_SET_DEBLOCK(info, clnt)					RETURN_RESULT()
#define VP_RPC_TOAGENT_SET_DNR(info, clnt)					RETURN_RESULT()
#define VP_RPC_TOAGENT_SET_ERROR_LEVEL(info, clnt) 				RETURN_RESULT()
#define VP_RPC_TOAGENT_GET_VIDEO_SEQUENCE_INFO(instanceID, clnt)		RETURN_RESULT()
#else

#if 0
inline CLNT_STRUCT RETURN_EFAIL(int A, int B, int C)	
{
		CLNT_STRUCT res;
		return res;
}

inline HRESULT* RETURN_RESULT()
{
	HRESULT* res = (HRESULT*) malloc(sizeof(HRESULT));
	*res = S_OK;
	return res;
}
inline VIDEO_RPC_DEC_SEQ_INFO* RETURN_SEQUENCE_INFO()
{
	VIDEO_RPC_DEC_SEQ_INFO* res = (VIDEO_RPC_DEC_SEQ_INFO*) malloc(sizeof(VIDEO_RPC_DEC_SEQ_INFO));
	
	return res;
}

inline RPCRES_LONG* RETURN_RPCRES_LONG()
{
	RPCRES_LONG* res = (RPCRES_LONG*) malloc(sizeof(RPCRES_LONG));
	res->result = S_OK;
	return res;
}

#define VP_RPC_PREPARE_CLNT(A, B, C)						RETURN_EFAIL(A, B, C)
#define VP_RPC_TOAGENT_CREATE(instance, clnt)				RETURN_RPCRES_LONG()
#define VP_RPC_TOAGENT_DESTROY(instanceID, clent)			RETURN_RESULT()
#define VP_RPC_TOAGENT_INITRINGBUFFER(ringBuffer, clent)	RETURN_RESULT()
#define VP_RPC_TOAGENT_RUN(instanceID, clent)				RETURN_RESULT()
#define VP_RPC_TOAGENT_PAUSE(instanceID, clent)				RETURN_RESULT()
#define VP_RPC_TOAGENT_STOP(instanceID, clent)				RETURN_RESULT()
#define VP_RPC_TOAGENT_SETREFCLOCK(pRefClock, clent)    	RETURN_RESULT()
#define VP_RPC_TOAGENT_CONNECT(connection, clnt)			RETURN_RESULT()
#define VP_RPC_TOAGENT_FLUSH(instanceID, clnt)				RETURN_RESULT()
#define	VP_RPC_TOAGENT_SET_SEEKING(info, clnt)				RETURN_RESULT()

#define VP_RPC_VOUT_TOAGENT_SET_VOLAYER(info, clnt)			RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_SET_SEEKING(info, clnt)			RETURN_RESULT()
#define	VP_RPC_TOAGENT_UPDATE_MEDIA_TYPE(info, clnt)		RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_SET_STEP(instanceID, clent)		RETURN_RESULT()

#define VP_RPC_VOUT_TOAGENT_SET_PRESENTATION_MODE(info, clnt)	RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_SHOW_STILL_PIC(info, clnt)			RETURN_RESULT()
#define VP_RPC_TOAGENT_SET_DEBLOCK(info, clnt)					RETURN_RESULT()
#define VP_RPC_TOAGENT_SET_DNR(info, clnt)					RETURN_RESULT()
#define VP_RPC_TOAGENT_SET_ERROR_LEVEL(info, clnt) 				RETURN_RESULT()
#define VP_RPC_TOAGENT_GET_VIDEO_SEQUENCE_INFO(instanceID, clnt)		RETURN_SEQUENCE_INFO()

#define VP_RPC_VOUT_TOAGENT_SET_VOLAYER(info, clnt)			RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_SET_SEEKING(info, clnt)			RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_SET_STEP(instanceID, clent)		RETURN_RESULT()

#define VP_RPC_VOUT_TOAGENT_SET_PRESENTATION_MODE(info, clnt) RETURN_RESULT()

#define VP_RPC_VOUT_TOAGENT_SHOW_STILL_PIC(info, clnt)		RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_FILL_BORDER(info, clnt)			RETURN_RESULT()

#define VP_RPC_VOUT_TOAGENT_SET_OUTPUT_FORMAT(info, clnt)	RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_SET_THUMBNAIL_THRESHOLD(info, clnt)		RETURN_RESULT()
#define VP_RPC_VOUT_TOAGENT_SET_THUMBNAIL_STARTPIC(info, clnt) 		RETURN_RESULT()

#else

#define VP_RPC_PREPARE_CLNT(A, B, C)						prepareCLNT(A, B, C)
#define VP_RPC_TOAGENT_CREATE(instance, clnt)				VIDEO_RPC_ToAgent_Create_0(instance, clnt)		
#define VP_RPC_TOAGENT_DESTROY(instanceID, clent)			VIDEO_RPC_ToAgent_Destroy_0(instanceID, clent)
#define VP_RPC_TOAGENT_INITRINGBUFFER(ringBuffer, clent)	VIDEO_RPC_ToAgent_InitRingBuffer_0(ringBuffer, clent)
#define VP_RPC_TOAGENT_RUN(instanceID, clent)				VIDEO_RPC_ToAgent_Run_0(instanceID, clent)
#define VP_RPC_TOAGENT_PAUSE(instanceID, clent)				VIDEO_RPC_ToAgent_Pause_0(instanceID, clent)
#define VP_RPC_TOAGENT_STOP(instanceID, clent)				VIDEO_RPC_ToAgent_Stop_0(instanceID, clent)
#define VP_RPC_TOAGENT_SETREFCLOCK(pRefClock, clent)    	VIDEO_RPC_ToAgent_SetRefClock_0(pRefClock, clent) 
#define VP_RPC_TOAGENT_CONNECT(connection, clnt)			VIDEO_RPC_ToAgent_Connect_0(connection, clnt)
#define VP_RPC_TOAGENT_FLUSH(instanceID, clnt)				VIDEO_RPC_ToAgent_Flush_0(instanceID, clnt)
#define	VP_RPC_TOAGENT_SET_SEEKING(info, clnt)				VIDEO_RPC_DEC_ToAgent_SetSpeed_0(info, clnt)	
#define	VP_RPC_TOAGENT_UPDATE_MEDIA_TYPE(info, clnt)		VIDEO_RPC_DEC_ToAgent_Init_0(info, clnt)
#define VP_RPC_TOAGENT_SET_DEBLOCK(info, clnt)				VIDEO_RPC_DEC_ToAgent_SetDeblock_0(info, clnt)
#define VP_RPC_TOAGENT_SET_DNR(info, clnt)					VIDEO_RPC_DEC_ToAgent_SetDNR_0(info, clnt)
#define VP_RPC_TOAGENT_SET_ERROR_LEVEL(info, clnt) 			VIDEO_RPC_DEC_ToAgent_SetErrorConcealmentLevel_0(info, clnt)
#define VP_RPC_TOAGENT_GET_VIDEO_SEQUENCE_INFO(instanceID, clnt)	VIDEO_RPC_DEC_ToAgent_GetVideoSequenceInfo_0(instanceID, clnt)
#define VP_RPC_TOAGENT_GET_PRIVATEINFO(param, clnt)		    VIDEO_RPC_DEC_ToAgent_PrivateInfo_0(param, clnt);

#define VP_RPC_VOUT_TOAGENT_SET_VOLAYER(info, clnt)			VIDEO_RPC_VO_FILTER_ToAgent_Display_0(info, clnt)
#define VP_RPC_VOUT_TOAGENT_SET_SEEKING(info, clnt)			VIDEO_RPC_VO_FILTER_ToAgent_SetSpeed_0(info, clnt)
#define VP_RPC_VOUT_TOAGENT_SET_STEP(instanceID, clent)		VIDEO_RPC_VO_FILTER_ToAgent_Step_0(instanceID, clent)
//#define VP_RPC_VOUT_TOAGENT_DISPLAY_BORDER(instanceID, clent) VIDEO_RPC_VO_FILTER_ToAgent_DisplayBorderColor_0(instanceID, clent)
#define VP_RPC_VOUT_TOAGENT_SET_PRESENTATION_MODE(info, clnt) VIDEO_RPC_VOUT_ToAgent_SetRescaleMode_0(info, clnt)

#define VP_RPC_VOUT_TOAGENT_SHOW_STILL_PIC(info, clnt)		VIDEO_RPC_VO_FILTER_ToAgent_ShowStillPicture_0(info, clnt)
#define VP_RPC_VOUT_TOAGENT_FILL_BORDER(info, clnt)		    VIDEO_RPC_VO_FILTER_ToAgent_FillVideoBorder_0(info, clnt)
#define VP_RPC_VOUT_TOAGENT_SET_FAST_DISPLAY(info, clnt)	VIDEO_RPC_VO_FILTER_ToAgent_SetFastDisplay_0(info, clnt)

#define VP_RPC_VOUT_TOAGENT_SET_OUTPUT_FORMAT(info, clnt)	VIDEO_RPC_THUMBNAIL_ToAgent_SetVscalerOutputFormat_0(info, clnt)
#define VP_RPC_VOUT_TOAGENT_SET_THUMBNAIL_THRESHOLD(info, clnt)	VIDEO_RPC_THUMBNAIL_ToAgent_SetThreshold_0(info, clnt)
#define VP_RPC_VOUT_TOAGENT_SET_THUMBNAIL_STARTPIC(info, clnt) 	VIDEO_RPC_THUMBNAIL_ToAgent_SetStartPictureNumber_0(info, clnt)
#endif	//if 0

#endif //#ifdef USE_VIDEO_DECODER_SIMULATOR

HRESULT* VIDEO_RPC_ToSystem_EndOfStream_Simulator(VIDEO_RPC_ENDOFSTREAM* event, RPC_STRUCT *clnt);
#define VP_RPC_DEC_TOSYSTEM_ENDOFSTREAM(event, clnt)		VIDEO_RPC_ToSystem_EndOfStream_Simulator(event, clnt)
#define VP_RPC_VOUT_TOSYSTEM_ENDOFSTREAM(event, clnt)		VIDEO_RPC_ToSystem_EndOfStream_Simulator(event, clnt)
//#define VP_RPC_VOUT_TOSYSTEM_FEEDBACKCONTEXT(pInfo, rpc)	VIDEO_RPC_VOUT_ToSystem_FeedbackContext_0_svc(pInfo, rpc)


RPCRES_LONG* 		VP_RPC_Create(VP_RPC_INSTANCE* Instance, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_Destroy(long* instanceID, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_InitRingBuffer(RPC_RINGBUFFER* ringBuffer, CLNT_STRUCT* clent);
HRESULT* 			VP_RPC_Run(long* instanceID, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_Stop(long* instanceID, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_Pause(long* instanceID, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_SetRefClock(VIDEO_RPC_SET_REFCLOCK *pRefClock, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_Connect(RPC_CONNECTION* connection, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_Flush(long* instanceID, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_SetSeeking(VIDEO_RPC_DEC_SET_SPEED* info, CLNT_STRUCT* clnt);

HRESULT* 			VP_RPC_Vout_SetVoutLayer(VIDEO_RPC_VO_FILTER_DISPLAY* info, CLNT_STRUCT* clnt);
HRESULT* 			VP_RPC_Vout_SetSeeking(VIDEO_RPC_VO_FILTER_SET_SPEED* info, CLNT_STRUCT* clnt);	
HRESULT* 			VP_RPC_Vout_Step(long* instanceID, CLNT_STRUCT* clent);
#endif

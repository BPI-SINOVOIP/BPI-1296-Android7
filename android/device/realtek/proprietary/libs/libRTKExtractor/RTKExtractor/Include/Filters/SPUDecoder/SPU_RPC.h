#ifndef __SPU_RPC_H__
#define __SPU_RPC_H__

//#include "../../../../../common/IPC/include/rpc_common.h"
#include <CStream.h>
/*
#include <include/rpc_common.h>
#include <generate/include/system/RPCBaseDS_data.h>
#include <generate/include/system/VideoRPCBaseDS_data.h>
#include <generate/include/system/VideoRPC_System.h>
*/
#include <rpc_common.h>
#include <RPCBaseDS_data.h>
#include <VideoRPCBaseDS_data.h>
#include <VideoRPC_System.h>

#include <Filters/PrivateInfo.h>
#include <Filters/SPUDecoder/xSPUUtil.h>

#ifdef USE_SPU_DECODER_SIMULATOR

typedef int SPU_CLIENT;
typedef struct
{
	long			instanceID;
	char*			pinID;
    BYTE*           pData;
	long			length;
} SPU_RPC_RECEIVEINFO;

struct RPC_VP_DEC_SPU_PTS_INFO {
	long		instanceID;
	int64_t		pts;
//	long    pWritePointer;
};

struct RPC_VP_DEC_SPU_DVDV_PALETTE {
	long			 instanceID;
	SPUDVDVPALETTE*   color;
};
/*
struct RPC_VP_DEC_SPU_DVDV_BUTTON {
	long	instanceID;
	SPUDVDVBUTTON*  btn;
};
*/
struct RPC_VP_DEC_SPU_ON_OFF {
	long	instanceID;
	bool	spu_on;
};

struct SPU_RPC_INSTANCE {
	long	type;          			//The module type, such as VIDEO_ENCODER, VIDEO_DECODER, 
			       					//SPU_DECODER, VIDEO_IN, VIDEO_OUT
	long 	system_filter_instanceID ; /*The corresponding filter on system side. */
};

struct SPU_RPC_RING_DATA{
	BYTE*	pRingLower;
	long	RingSize;
};
/*
struct SPU_RPC_SENDPIN_LONG
{
	long	instanceID;
	char*	pinID;
	long	data;
};
*/
struct VP_SPU_RPCRES_LONG {
	HRESULT	result;
	long	data;
};

struct SPU_RPC_GET_PTS_LONG
{
	long	instanceID;
	long	PTS;
};

struct SPU_RECTANGLE {
	u_short x;
	u_short y;
	u_short width;
	u_short height;
};
typedef struct SPU_RECTANGLE SPU_RECTANGLE;

struct SUBPIC_COLORMAP {
	u_char alpha3;
	u_char alpha2;
	u_char alpha1;
	u_char alpha0;
	u_char color3;
	u_char color2;
	u_char color1;
	u_char color0;
};
typedef struct SUBPIC_COLORMAP SUBPIC_COLORMAP;

struct SPU_HILIGHT {
	long instanceID;
  	long START_PTSH ; // neco*
  	long START_PTSL ; // neco*
	struct SPU_RECTANGLE hlWindow;
	struct SUBPIC_COLORMAP colorMap;
	u_char hlEnable;
};
typedef struct SPU_HILIGHT SPU_HILIGHT;
#endif

#ifdef USE_SPU_DECODER_SIMULATOR

/*
CLNT_STRUCT RETURN_EFAIL(int A, int B, int C)	
{
		CLNT_STRUCT res;
		return res;
}
*/
#define SPU_RPC_PREPARE_CLNT			SPU_RETURN_EFAIL

#define SPU_RPC_TOAGENT_CREATE			SPU_RPC_ToAgent_Create
#define SPU_RPC_TOAGENT_DESTROY			SPU_RPC_ToAgent_Destroy
#define SPU_RPC_TOAGENT_STOP			SPU_RPC_ToAgent_Stop
#define SPU_RPC_TOAGENT_PAUSE			SPU_RPC_ToAgent_Pause
#define SPU_RPC_TOAGENT_RUN				SPU_RPC_ToAgent_Run
#define SPU_RPC_TOAGENT_INITRINGBUFFER	SPU_RPC_ToAgent_InitRingBuffer
#define SPU_RPC_TOAGENT_FLUSH			SPU_RPC_ToAgent_Flush

//???????????????????
//#define SPU_RPC_TOAGENT_CMODEL_CMD		SPU_RPC_ToAgent_CModel_Cmd
//#define SPU_RPC_TOAGENT_CONNECT			SPU_RPC_ToAgent_Connect
//???????????????????

#define SPU_RPC_TOAGENT_SETREFCLOCK    	SPU_RPC_ToAgent_SetRefClock
#define SPU_RPC_TOAGENT_SET_BUTTON		SPU_RPC_ToAgent_SetButton
#define SPU_RPC_TOAGENT_ENABLE			SPU_RPC_ToAgent_Enable
#define SPU_RPC_TOAGENT_SHOW			SPU_RPC_ToAgent_Show
#define SPU_RPC_TOAGENT_HIDE			SPU_RPC_ToAgent_Hide
#define SPU_RPC_TOAGENT_SET_SCR_SIZE    SPU_RPC_ToAgent_SetScreenSize

#define SPU_RPC_TOAGENT_RECEIVE			SPU_RPC_ToAgent_Receive
#define SPU_RPC_TOAGENT_UPDATE_PTS		SPU_RPC_ToAgent_UpdatePTS
#define SPU_RPC_TOAGENT_SET_PALETTE		SPU_RPC_ToAgent_SetPalette

#define SPU_RPC_TOAGENT_SET_SEEKING		SPU_RPC_ToAgent_SetSeeking

//======================================================================
//#define SPU_RPC_TOAGENT_GET_SEEKING			SPU_RPC_ToAgent_GetSeeking

//#define SPU_RPC_TOAGENT_SPU_SET_RING_SZ		SPU_RPC_ToAgent_SPU_StRingSize
#ifndef SPU_FLUSH_2 
#define	SPU_RPC_TOAGENT_SPU_FLUSH_REQ		SPU_RPC_ToAgent_SPU_FlushRequest
#define	SPU_RPC_TOAGENT_SPU_FLUSH_WAIT_COM	SPU_RPC_ToAgent_SPU_FlushWaitComplete
#define	SPU_RPC_TOAGENT_SPU_FLUSH_CLEAN		SPU_RPC_ToAgent_SPU_FlushClean
#endif

#else

//#define SPU_RPC_TOAGENT_RECEIVE		SPU_RPC_ToAgent_Receive_0
//#define SPU_RPC_TOAGENT_SPU_RECEIVE			E_FAIL
//#define SPU_RPC_TOAGENT_SPU_UPDATE_PTS		E_FAIL
//#define SPU_RPC_TOAGENT_SPU_SET_PALETTE		E_FAIL
#define SPU_RPC_PREPARE_CLNT(A, B, C)	prepareCLNT(A, B, C)

#define SPU_RPC_TOAGENT_CREATE			VIDEO_RPC_ToAgent_Create_0
#define SPU_RPC_TOAGENT_DESTROY			VIDEO_RPC_ToAgent_Destroy_0
#define SPU_RPC_TOAGENT_INITRINGBUFFER	VIDEO_RPC_ToAgent_InitRingBuffer_0
#define SPU_RPC_TOAGENT_STOP				VIDEO_RPC_ToAgent_Stop_0
#define SPU_RPC_TOAGENT_PAUSE			VIDEO_RPC_ToAgent_Pause_0
#define SPU_RPC_TOAGENT_RUN				VIDEO_RPC_ToAgent_Run_0
#define SPU_RPC_TOAGENT_FLUSH			VIDEO_RPC_ToAgent_Flush_0

//???????????????????
//#define SPU_RPC_TOAGENT_CMODEL_CMD		VIDEO_RPC_ToAgent_CModel_Cmd_0
//#define SPU_RPC_TOAGENT_CONNECT			VIDEO_RPC_ToAgent_Connect_0
//???????????????????
//#define SPU_RPC_TOAGENT_SET_BUTTON		VIDEO_RPC_SUBPIC_DEC_ToAgent_ConfigureHighLight_0
//#define SPU_RPC_TOAGENT_SETREFCLOCK    	VIDEO_RPC_ToAgent_SetRefClock_0
#define SPU_RPC_TOAGENT_ENABLE			VIDEO_RPC_SUBPIC_DEC_ToAgent_EnableSubPicture_0
#define SPU_RPC_TOAGENT_SHOW			VIDEO_RPC_SUBPIC_DEC_ToAgent_ShowSubPicture_0
#define SPU_RPC_TOAGENT_HIDE			VIDEO_RPC_SUBPIC_DEC_ToAgent_HideSubPicture_0
#define SPU_RPC_TOAGENT_SET_SCR_SIZE    VIDEO_RPC_SUBPIC_DEC_ToAgent_Configure_0

//#define SPU_RPC_TOAGENT_SET_SEEKING		VIDEO_RPC_DEC_ToAgent_SetSpeed_0

//======================================================================

//#define SPU_RPC_TOAGENT_GET_SEEKING			E_FAIL
//#define SPU_RPC_TOAGENT_SPU_SET_RING_SZ		E_FAIL

#endif

/////////////////////////////////////////////////////////////////////////
CLNT_STRUCT 	SPU_RETURN_EFAIL(int A, int B, int C);

//???????????????????
//RPCRES_LONG*	SPU_RPC_ToAgent_CModel_Cmd(VIDEO_RPC_CMODEL_CMD *info, CLNT_STRUCT *clnt);
//HRESULT*		SPU_RPC_ToAgent_Connect(RPC_CONNECTION *info, CLNT_STRUCT *clnt);
//???????????????????
#ifdef USE_SPU_DECODER_SIMULATOR
RPCRES_LONG* 	SPU_RPC_ToAgent_Create(SPU_RPC_INSTANCE *pInstance, CLNT_STRUCT *rpc);
HRESULT* 		SPU_RPC_ToAgent_Destroy(long *instanceID, CLNT_STRUCT *clnt);
HRESULT* 		SPU_RPC_ToAgent_Stop(long *instanceID, CLNT_STRUCT *clnt);
HRESULT* 		SPU_RPC_ToAgent_Pause(long *instanceID, CLNT_STRUCT *clnt/*, FILTER_STATE State*/);
HRESULT* 		SPU_RPC_ToAgent_Run(long *instanceID, CLNT_STRUCT *clnt);

HRESULT* 		SPU_RPC_ToAgent_InitRingBuffer(RPC_RINGBUFFER* RingBuffer, CLNT_STRUCT* clnt);
HRESULT*		SPU_RPC_ToAgent_Flush(long *instanceID, CLNT_STRUCT *clnt);

HRESULT* 		SPU_RPC_ToAgent_Enable(VIDEO_RPC_SUBPIC_DEC_ENABLE_SUBPIC *info, CLNT_STRUCT *clnt);
HRESULT* 		SPU_RPC_ToAgent_Show(long *instanceID, CLNT_STRUCT *clnt);
HRESULT* 		SPU_RPC_ToAgent_Hide(long *instanceID, CLNT_STRUCT *clnt);
HRESULT*        SPU_RPC_ToAgent_SetScreenSize(VIDEO_RPC_SUBPIC_DEC_CONFIGURE *info, CLNT_STRUCT *clnt);


HRESULT 		SPU_RPC_ToAgent_Receive(SPU_RPC_RECEIVEINFO* pReceiveInfo, SPU_CLIENT* clnt);
HRESULT 		SPU_RPC_ToAgent_UpdatePTS(RPC_VP_DEC_SPU_PTS_INFO info);
HRESULT 		SPU_RPC_ToAgent_SetPalette(RPC_VP_DEC_SPU_DVDV_PALETTE info);
HRESULT* 		SPU_RPC_ToAgent_SetButton(/*VIDEO_RPC_VO_FILTER_CONFIG_HILIGHT*/SPU_HILIGHT* info, CLNT_STRUCT *clnt);
HRESULT*		SPU_RPC_ToAgent_SetRefClock(VIDEO_RPC_SET_REFCLOCK *info, CLNT_STRUCT *clnt);

#ifndef SPU_FLUSH_2 
HRESULT		SPU_RPC_ToAgent_SPU_FlushRequest(long instanceID);
HRESULT		SPU_RPC_ToAgent_SPU_FlushWaitComplete(long instanceID);
HRESULT		SPU_RPC_ToAgent_SPU_FlushClean(long instanceID);
#endif
HRESULT* 	SPU_RPC_ToAgent_SetSeeking(VIDEO_RPC_DEC_SET_SPEED *info, CLNT_STRUCT *clnt);

#endif


//=============================================================================
//HRESULT 	SPU_RPC_ToAgent_GetSeeking(long instanceID, long* pSpeed, long* pSkip);
//HRESULT 	SPU_RPC_ToAgent_SPU_StRingSize(long instanceID, SPU_RPC_RING_DATA RingDate);


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

//#ifdef USE_SPU_DECODER_SIMULATOR

//#define SPU_RPC_TOSYSTEM_SETREADPTR			SPU_RPC_ToSystem_SetReadPointer
//#define SPU_RPC_TOSYSTEM_GET_REF_CLOCK		SPU_RPC_ToSystem_GetRefClock
//#define	SPU_RPC_TOSYSTEM_GET_LOCAL_PTS		SPU_RPC_ToSystem_GetLocalPTS
//#define	SPU_RPC_TOSYSTEM_FLUSH_POST			SPU_RPC_ToSystem_FlushPostComplete
//#define	SPU_RPC_TOSYSTEM_GET_RING_SZ		SPU_RPC_ToSystem_GetRingSZ
/*
#else

#define SPU_RPC_TOSYSTEM_SETREADPTR			E_FAIL
#define SPU_RPC_TOSYSTEM_GET_REF_CLOCK		E_FAIL
#define	SPU_RPC_TOSYSTEM_GET_LOCAL_PTS		E_FAIL
#define	SPU_RPC_TOSYSTEM_FLUSH_POST			E_FAIL
//#define	SPU_RPC_TOSYSTEM_GET_RING_SZ		E_FAIL
*/
//#endif

///////////////////////////////////////////////////////////////////
/*
#ifdef USE_SPU_DECODER_SIMULATOR

//HRESULT 	SPU_RPC_ToSystem_SetReadPointer(SPU_RPC_SENDPIN_LONG* pReadPack, SPU_CLIENT* pClnt);
//HRESULT		SPU_RPC_ToSystem_GetRefClock(long instanceID, SPU_CLIENT* pClnt);
//HRESULT		SPU_RPC_ToSystem_GetLocalPTS(SPU_RPC_GET_PTS_LONG* info, SPU_CLIENT* pClnt);
HRESULT		SPU_RPC_ToSystem_FlushPostComplete(long instanceID, SPU_CLIENT* pClnt);
//HRESULT		SPU_RPC_ToSystem_GetRingSZ(long instanceID, SPU_CLIENT* pClnt);
#endif
*/
///////////////////////////////////////////////////////////////////

#endif

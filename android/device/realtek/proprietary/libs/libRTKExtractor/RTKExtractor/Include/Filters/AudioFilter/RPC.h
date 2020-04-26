 
#ifndef __C_RPC_H__
#define __C_RPC_H__

#include "string.h"
#include <Util.h>
#include "OSAL.h"
#include "StreamClass/SType.h"

#include "AudioType.h"
/////////////////////////////////////////////////////


#define RPC_TOAGENT_CREATE(instance,clnt)			RPC_Create(instance,clnt)
#define RPC_TOAGENT_CONNECT(connection,clnt)		RPC_Connect(connection,clnt)
#define RPC_TOAGENT_DISCONNECT
#define RPC_TOAGENT_DESTROY(instanceID,clnt)		RPC_Destroy(instanceID,clnt)
#define RPC_TOAGENT_RUN(instanceID,clnt)			RPC_Run(instanceID,clnt)
#define RPC_TOAGENT_PAUSE(instanceID,clnt)			RPC_Pause(instanceID,clnt)
#define RPC_TOAGENT_STOP(instanceID,clnt)			RPC_Stop(instanceID,clnt)
#define RPC_TOAGENT_GETSTATUS(instanceID,clnt)		RPC_GetStatus(instanceID,clnt)
#define RPC_TOAGENT_SEEK(seek,clnt)					RPC_Seek(seek,clnt)
#define RPC_TOAGENT_INITRINGBUFFER(ringBuffer,clnt)	RPC_InitRingBuffer(ringBuffer,clnt)
#define RPC_TOAGENT_UPDATEWRITEPTR(writePack,clnt)	RPC_UpdateWritePtr(writePack,clnt)
#define RPC_TOAGENT_UPDATEREADPTR(readPack,clnt)	RPC_UpdateReadPtr(readPack,clnt)
#define RPC_TOAGENT_PRIVATEINFO(privateInfo,clnt)	RPC_PrivateInfo(privateInfo, clnt)

///////////////////////////////////////////////////////////////////////////////
#define RPC_TOAGENT_ENDOFSTREAM(sendPin,clnt)		RPC_EndOfStream(sendPin, clnt)
#define RPC_TOAGENT_RECEIVE(pReceiveInfo,clnt)		RPC_Receive(pReceiveInfo, clnt)

#define RPC_TOAGENT_FLUSH(sendPin, clnt)                RPC_Flush(sendPin, clnt)
#define RPC_TOAGENT_SET_SEEKING(instanceID, speed, skip)        RPC_SetSeeking(instanceID, speed, skip)

///////////////////////////////////////////////////////////////////////////////////

#define ADDTO_FILTERMAP(filter,instanceID)			AddToFilterMap(filter,instanceID)
#define RPC_TOSYSTEM_UPDATEWRITEPTR(writePack,clnt)	RPC_SystemUpdateWritePtr(writePack,clnt)
#define RPC_TOSYSTEM_UPDATEREADPTR(readPack,clnt)	RPC_SystemUpdateReadPtr(readPack,clnt)
#define RPC_TOSYSTEM_ENDOFSTREAM(sendPin,clnt)		RPC_SystemEndOfStream(sendPin, clnt)
#define RPC_TOSYSTEM_PRIVATEINFO(privateInfo,clnt)	RPC_SystemPrivateInfo(privateInfo, clnt)
#define RPC_TOSYSTEM_DELIVERFLUSH(sendPin,clnt)                RPC_SystemDeliverFlush(sendPin, clnt)
#define RPC_TOSYSTEM_DELIVER(pDeliverInfo,clnt)		RPC_SystemDeliver(pDeliverInfo, clnt)


/////////////////////////////////////////////////////

/////////////////////////////////////////////////////

typedef struct tagRPCRESLONG
{
	HRESULT	result;
	long	data;

} RPCRES_LONG;

/*
typedef struct tagRPCSENDLONG 
{
	long	instanceID;
	long	data;

} RPC_SEND_LONG;
*/

typedef struct tagRPCSENDPINLONG 
{
	long	instanceID;
	char*	pinID;
	long	data;

} RPC_SENDPIN_LONG;

typedef struct tagRPCINSTNACE
{
	long	instanceID;
	long	type;

} RPC_INSTANCE;


typedef struct tagRPCCONNECTION
{
	long				srcInstanceID;
	char*				srcPinID;
	long				desInstanceID;
	char*				desPinID;
	const MEDIA_TYPE*	mediaType;

} RPC_CONNECTION;

typedef struct tagRPCRINGBUFFER
{
	long			instanceID;
	char*			pinID;
	unsigned long	beginPhysicalAddress;
	long			bufferSize;

} RPC_RINGBUFFER;


typedef struct tagRPCSEEK
{
	long			instanceID;
	long			speed;
	long			skip;

} RPC_SEEK;


typedef struct tagRPCPRIVATEINFO
{
	long			instanceID;
	char*			pinID;
	BYTE*			pData;
	long			length;

} RPC_PRIVATEINFO;



typedef struct tagRPCSENDPINVOID
{
	long	instanceID;
	char*	pinID;

} RPC_SENDPIN_VOID;


typedef struct tagRPCRECEIVEINFO
{
	long			instanceID;
	char*			pinID;
    BYTE*           pData;
	long			length;
} RPC_RECEIVEINFO;


typedef RPC_RECEIVEINFO RPC_DELIVERINFO;

typedef int CLIENT;


//////////////////////////////////////////////////////////////



RPCRES_LONG		RPC_Create(RPC_INSTANCE *instance, CLIENT* clnt);
HRESULT			RPC_Connect(RPC_CONNECTION* connection, CLIENT* cInt);
HRESULT			RPC_InitRingBuffer(RPC_RINGBUFFER* ringBUffer, CLIENT* clnt);
HRESULT			RPC_UpdateWritePtr(RPC_SENDPIN_LONG* pWritePack, CLIENT* pClnt);
HRESULT			RPC_UpdateReadPtr(RPC_SENDPIN_LONG* pReadPack, CLIENT* pClnt);
HRESULT			RPC_Run(long instanceID, CLIENT* pClnt);
HRESULT			RPC_Pause(long instanceID, CLIENT* pClnt);
HRESULT			RPC_Stop(long instanceID, CLIENT* clnt);
HRESULT			RPC_PrivateInfo(RPC_PRIVATEINFO* pPrivateInfo, CLIENT* clnt);
HRESULT			RPC_Destroy(long instanceID, CLIENT* pClnt);

////////////////////////////////////////////////////////////////////////////////

HRESULT			RPC_EndOfStream(RPC_SENDPIN_VOID* pSendPin, CLIENT* clnt);
HRESULT			RPC_Receive(RPC_RECEIVEINFO* pReceiveInfo, CLIENT* clnt);
HRESULT                 RPC_Flush(RPC_SENDPIN_VOID* pSendPin, CLIENT* clnt);
HRESULT                 RPC_SetSeeking(long instanceID, long Speed, long Skip);

////////////////////////////////////////////////////////////////////

class CAudioFilter;

void			AddToFilterMap(CAudioFilter* pFilter, long instanceID);
HRESULT			RPC_SystemUpdateReadPtr(RPC_SENDPIN_LONG* pReadPack, CLIENT* pClnt);
HRESULT			RPC_SystemUpdateWritePtr(RPC_SENDPIN_LONG* pWritePack, CLIENT* pClnt);
HRESULT			RPC_SystemEndOfStream(RPC_SENDPIN_VOID* pSendPin, CLIENT* clnt);
HRESULT                 RPC_SystemDeliverFlush(RPC_SENDPIN_VOID* pSendPin, CLIENT* clnt);
HRESULT			RPC_SystemPrivateInfo(RPC_PRIVATEINFO* pPrivateInfo, CLIENT* clnt);
HRESULT			RPC_SystemDeliver(RPC_DELIVERINFO* pDeliverInfo, CLIENT* clnt);

#endif

#ifndef _RVSD_SOCKET_CLIENT_H_
#define _RVSD_SOCKET_CLIENT_H_

// DvdPlayer handle socket in CPlaybackFlowManager::HandleSocketRequest().
// req is 'eos ' or 'err ' or 'dmsg'.
// type:
//		if req is 'eos ', type is 'vide' or 'audi' or 'indG'.
//		if req is 'err ', type is enum of FATAL_ERROR_CODE (rpc_common.h)
//		if req is 'dmsg', type is enum of DECODER_MESSAGE_CODE (rpc_common.h)
typedef struct
{
    unsigned int req;
    unsigned int type;
} SOCKETQ;

int notifyDvdPlayerListener(SOCKETQ *req, int rvsdHandle=-1);
void SendDvdPlayerFatalErrNotification(int errCode, int rvsdHandle=-1);
void SendDvdPlayerDecoderMessage(int messageCode, int rvsdHandle=-1);

#endif //  _RVSD_SOCKET_CLIENT_H_

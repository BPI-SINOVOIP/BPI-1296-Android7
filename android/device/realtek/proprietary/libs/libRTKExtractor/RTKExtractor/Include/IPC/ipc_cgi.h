#ifndef IPC_CGI_H
#define IPC_CGI_H

#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <Platform_Lib/Graphics/user_cmd.h>

#define RET_CGI_OK   0
#define RET_CGI_FAIL -1

#define CGI_MSG_KEY     ((key_t) 0xf117)
#define CGI_MSG_PERM	0644

static int cgi_msg_id;

//CGI commands:
typedef enum
{
	CGI_GET_SYSTEM_STATUS,
	CGI_SEND_SYSTEM_STATUS,

	CGI_GET_HDD_STATUS,
	CGI_SEND_HDD_STATUS,

	CGI_CMD_OTR,

	CGI_GET_SOURCE,
	CGI_SEND_SOURCE,
}CGI_CMD_ENUM;

/* message structure:
 *------------------
 *| mtype | buffer |
 *------------------
 *       /          \
 *-------            ----------
 *| id | command | sub buffer |
 *| int|  int    |   char     |
 *-----------------------------
 * mtype     : receiver id
 * id        : sender id
 * command   : command id described by CGI_CMD_ENUM
 * sub buffer: user defined buffer
 *
 * server's id is 1, client's id is its pid
 */
typedef struct
{
	long mtype;
	char buffer[512];
}cgimsg;

void CGI_MsgSetID(cgimsg *msg, int id);
int  CGI_MsgGetID(cgimsg *msg);

inline int CGI_MsgInit(void) 
{
	if((cgi_msg_id = msgget(CGI_MSG_KEY, CGI_MSG_PERM|IPC_CREAT|IPC_EXCL)) == -1)
		cgi_msg_id = msgget(CGI_MSG_KEY, CGI_MSG_PERM);

	if(cgi_msg_id == -1)
		return RET_CGI_FAIL;
	else
		return RET_CGI_OK;
}

inline void CGI_MsgUninit(void* arg) 
{
	msgctl(cgi_msg_id, IPC_RMID, 0);
}

/* Send a message to message queue
 * msg  [in]: message to send
 * from [in]: sender id
 * to   [in]: receiver id
 */
inline int CGI_MsgSnd(cgimsg *msg, int from, int to) 
{
	msg->mtype = to;

	CGI_MsgSetID(msg, from);
	if(msgsnd(cgi_msg_id, msg, sizeof(cgimsg), IPC_NOWAIT) == -1)
		return RET_CGI_FAIL;
	else
		return RET_CGI_OK;
}

/* Get a message from message queue
 * msg    [out]: message returned
 * type   [in] : message id that equals to receiver id
 * srcid  [out]: sender id
 */
inline int CGI_MsgRcv(cgimsg *msg, int type, int *srcid)
{
	if (msgrcv(cgi_msg_id, msg, sizeof(cgimsg), type, 0) < 0)
	{
		printf("Failed to receive message\n");
	
		return RET_CGI_FAIL;
	}
	*srcid = CGI_MsgGetID(msg);
	return RET_CGI_OK;
}

inline int CGI_MsgGetID(cgimsg *msg)
{
	return *(int*)msg->buffer;
}

inline void CGI_MsgSetID(cgimsg *msg, int id)
{
	*(int *)msg->buffer = id;	
}

inline int CGI_MsgGetCmd(cgimsg *msg)
{
	return *((int*)msg->buffer + 1);
}

inline char *CGI_MsgGetBuf(cgimsg *msg)
{
	return &msg->buffer[sizeof(int) * 2];
}

inline void CGI_MsgSetCmd(cgimsg *msg, int cmd)
{
	*((int *)msg->buffer + 1) = cmd;
}

inline void CGI_MsgSetBuf(cgimsg *msg, char *buffer)
{
/*
	int len;

	len = strlen(buffer);
	if (len + 1 > 512)
		len = 511;

	if (len)
		strncpy(&msg->buffer[sizeof(int) * 2], buffer, len + 1);
*/
	memcpy(&msg->buffer[sizeof(int) * 2], buffer, 512 - 2 * sizeof(int));	
}

#endif

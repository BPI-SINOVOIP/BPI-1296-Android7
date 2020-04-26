#ifndef _SI_MESSAGE_H_
#define _SI_MESSAGE_H_

//*************************************************************
// DEFINE
//*************************************************************
#define SI_MSGQUEUE_SIZE		64
//#define SI_MSGQUEUE_SIZE		3


#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef BOOL
#define BOOL int
#endif



typedef enum {
	SI_MESSAGE_NOMESSAGE,
	SI_MESSAGE_STREAM_CHANGE_BEGIN,
	SI_MESSAGE_STREAM_CHANGE_END,
	SI_MESSAGE_FOUND_A_NEW_CH,
	SI_MESSAGE_CH_INFO_READY,
	SI_MESSAGE_CH_SCAN_FAIL,	//scan fail
	SI_MESSAGE_RESET_CHANNEL,
	SI_MESSAGE_NEW_PMT,
	SI_MESSAGE_NEW_PRESENT_EIT,
	SI_MESSAGE_NEW_FOLLOWING_EIT,
	SI_MESSAGE_NEW_SCHEDULE_EIT,
	SI_MESSAGE_EPG_RESERVATION,
	SI_MESSAGE_ALARM_TIMEOUT,
	SI_MESSAGE_RESET_TIME,
	SI_MESSAGE_LIVE_CHANGECHANNEL_DONE,	//post this msg when channel changed in playback si when timeShifting
	SI_MESSAGE_CUR_CH_DISAPPEAR,		//current channel is disappeard
	SI_MESSAGE_LAST_CH_APPEAR,		//after SI_MESSAGE_CUR_CH_DISAPPEAR occur, and same ch recover again!
	SI_MESSAGE_CHANNEL_UPDATE,			//some info in channel is updated(ex: lcn,...)
	SI_MESSAGE_CHANNEL_ADD_DELETE	// bug 26588, Evora iSuite v2.3, GBR/394
} SI_MESSAGE;


//*************************************************************
// STRUCT
//*************************************************************
typedef struct _SI_MSG {

	SI_MESSAGE			message;
	unsigned int		data;

} SI_MSG;

typedef struct _SI_MSG_QUEUE {
	SI_MSG				queue[SI_MSGQUEUE_SIZE];
	int					read;
	int					write;
} SI_MSG_QUEUE;

#ifdef FIX_DTV_LABS_HK_ISSUE_48
#define SI_MSG_MAX_DUPLCNLIST	32
typedef struct _SI_MSG_DUPLCNLIST {
	int					count;
	unsigned short		lcnList[SI_MSG_MAX_DUPLCNLIST];
} SI_MSG_DUPLCNLIST;
#endif

//*************************************************************
// PUBLIC FUNCTIONS
//*************************************************************
BOOL SiMessage_Init(SI_MSG_QUEUE* mq);
BOOL SiMessage_Reset(SI_MSG_QUEUE* mq);
BOOL SiMessage_DeInit(SI_MSG_QUEUE* mq);
BOOL SiMessage_Store(SI_MSG_QUEUE* mq, SI_MESSAGE message, unsigned int data);
BOOL SiMessage_Get(SI_MSG_QUEUE* mq, SI_MESSAGE *pMessage, unsigned int *pData);
BOOL SiMessage_PeekNewest(SI_MSG_QUEUE* mq, SI_MESSAGE *pMessage, unsigned int *pData);

#endif //_SI_MESSAGE_H_

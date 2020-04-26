
#include <system/si4/SITypes.h>
#include "agent/SI_Agent.h"


#ifndef _SI_TYPES_LOCAL_H
#define _SI_TYPES_LOCAL_H

//#define SI_RING_BUF_LENGTH_SSU				(1100*188)
#define SI_RING_BUF_LENGTH_SSU				(1400*188)
#define SI_RING_BUF_LENGTH_NORMAL 		(200*188)

//lcn 16 bits(14 bit in spec)
#define SI_CHANNEL_LCN_INVALID (0xffff)
#define SI_CHANNEL_LCN_ZERO (0)



typedef int INT32;
typedef unsigned int UINT32;
typedef unsigned char UINT8;
typedef unsigned short UINT16;


// collector
typedef struct SI_COLLECTOR_SECTION_BUFFER	SI_COLLECTOR_SECTION_BUFFER;
struct SI_COLLECTOR_SECTION_BUFFER
{
	unsigned int						pid;
	unsigned int						expectedSectionLength;
	unsigned int						collectedSectionLength;
	unsigned char						*sectionBuf;
};

typedef struct SI_COLLECTOR_BEAN_CTRL_INFO	SI_COLLECTOR_BEAN_CTRL_INFO;
struct SI_COLLECTOR_BEAN_CTRL_INFO
{
	unsigned char						*allocatedAddr;
	unsigned char						*surplusAddr;
	unsigned int						surplusLength;
};


struct SI_COLLECTOR
{
	unsigned int						pidFilterNum;
	unsigned int						beanQueueNum;
	SI_COLLECTOR_SECTION_BUFFER			sectionBuf[SI_COLLECTOR_MAX_PID_FILTER_NUM];
	SI_COLLECTOR_BEAN_CTRL_INFO			beanCtrlInfo[SI_COLLECTOR_MAX_TP_QUEUE_NUM];
	void								*tp;
	SI_Collector_SectionCallback		sectionCB[SI_SECTIONCOLLECTOR_CBF_TYPE_NUM__];
};

struct SI_AGENT
{
	SI_AGENT_STATUS						status;
	BOOL								waitingForReset;

	void								*agentObjHandle;

	SI_DATABASE							*db;
	SI_COLLECTOR						*collector;

	void								*tp;

	//to prevent others insert task when si_api 'stop' agent
	//(make sure agent->status have no race condition)
	pthread_mutex_t stopTaskMutex;

	// mode
    /* Move Global variable from SiDvb/librtd/si4/agent/SI_Agent.c to here */
    int                                 mAgentNum;
    SI_AGENT_LIST_NODE                  *mAgentList;
    pthread_mutex_t                     mAgentListMutex;
    pthread_t                           mAgentThread;
    BOOL                                mStopAgentThread;
};




/*SI_CHANNEL_EXT
ver 1:org
ver 2:add  tsID field   (sizeof: 508)
ver 3:add  offset field (sizeof: 512)	//a week later, add type in struct SI_CHANNEL_AUDIO_INFO
ver 4: add type in struct SI_CHANNEL_AUDIO_INFO, uncomment two fields in SI_CHANNEL_VIDEO_INFO
ver 5: add profileLevel in SI_CHANNEL_VIDEO_INFO (SI_CHANNEL_VIDEO_INFO_V5)
ver 6: add unsigned short userChNum in SI_CHANNEL(2009/3/18)
	(create  _SI_CHANNEL_V5 struct)
ver 7: extend SI_CH_CHANNELNAME_MAXLEN from 32 to 64 to fix DTV Lab bug 25717
ver 8: extend langCode of SI_CHANNEL_AUDIO_INFO to fix DTV Lab bug 26640
*/
//prefix 0xff is for guard	
//#define SI_CH_CHLIST_FILE_VER			0xff000002
#define SI_CHANNEL_STRUCT_VER			0xff000008

typedef enum _SI_CHANNEL_STRUCT_VER
{
	SI_CHANNEL_STRUCT_VER_2 = 0xff000002,
	SI_CHANNEL_STRUCT_VER_4 = 0xff000004,	
	SI_CHANNEL_STRUCT_VER_5 = 0xff000005,
	SI_CHANNEL_STRUCT_VER_CUR = SI_CHANNEL_STRUCT_VER,
	
} SI_CHANNEL_VERSION;


//v1~v5 is same
typedef struct _SI_CHANNEL_V5
{
	SI_LOGICAL_CH_NUM				logicalChNum;	//change to short to distinguish lcn =0 and no lcn
	unsigned int						channelNameLen;
	unsigned char						channelName[SI_CH_CHANNELNAME_MAXLEN];
} SI_CHANNEL_V5;


//v2~v4 is same
typedef struct _SI_CHANNEL_VIDEO_INFO_V4
{
	unsigned short						videoPid;
	unsigned char						streamType;
	unsigned char						videoType;
	SI_LANG								langCode;
} SI_CHANNEL_VIDEO_INFO_V4;

typedef struct _SI_CHANNEL_VIDEO_INFO_V5
{
	unsigned short						videoPid;
	unsigned char						streamType;
	unsigned char						videoType;
	SI_LANG								langCode;
	int 								profileLevel;	//new field
} SI_CHANNEL_VIDEO_INFO_V5;


//ref SI_CHANNEL_STRUCT_VER
//sizeof(_SI_CHANNEL_EXT_V2): 508
typedef struct _SI_CHANNEL_EXT_V2
{
	SI_CHANNEL						super;

	SI_CHANNEL_EXT					*next;
	SI_CHANNEL_EXT					*previous;

	unsigned int						frequency;
	unsigned int						bandwidth;
	unsigned short					onid;
	unsigned short					tsID;
	unsigned short					programNumber;

	unsigned int						channelOrder;

	SI_SERVICE_TYPE						serviceType;
	SI_CHANNEL_FLAG						channelFlags;

	unsigned int						oriChannelNameLen;
	unsigned char						oriChannelName[SI_CH_CHANNELNAME_MAXLEN];

	unsigned short						pcrPid;

	unsigned char						videoNum;

	unsigned char						currentVideo;
	unsigned char						currentAudio;

	SI_CHANNEL_VIDEO_INFO				videos[SI_CH_VIDEO_MAXNUM];
	SI_CHANNEL_AUDIO_LIST				audioList;

	SI_CHANNEL_SUBTITLE_LIST			subtitleList;
	SI_CHANNEL_TT_LIST					teletextList;
	SI_CHANNEL_TT_LIST					ttxSubtitleList;

	// only for pvr live pause
	unsigned char						currentProgramRating;

} SI_CHANNEL_EXT_V2;

typedef struct _SI_CHANNEL_EXT_V4
{
	SI_CHANNEL						super;

	SI_CHANNEL_EXT					*next;
	SI_CHANNEL_EXT					*previous;

	unsigned int						frequency;
	int								offset;		//frequency offset(add at v3)
	unsigned int						bandwidth;
	unsigned short					onid;
	unsigned short					tsID;
	unsigned short					programNumber;

	unsigned int						channelOrder;

	SI_SERVICE_TYPE						serviceType;
	SI_CHANNEL_FLAG						channelFlags;

	unsigned int						oriChannelNameLen;
	unsigned char						oriChannelName[SI_CH_CHANNELNAME_MAXLEN];

	unsigned short						pcrPid;

	unsigned char						videoNum;	//# of videos.

	unsigned char						currentVideo;
	unsigned char						currentAudio;

	SI_CHANNEL_VIDEO_INFO_V4			videos[SI_CH_VIDEO_MAXNUM];
	SI_CHANNEL_AUDIO_LIST				audioList;

	SI_CHANNEL_SUBTITLE_LIST			subtitleList;
	SI_CHANNEL_TT_LIST					teletextList;
	SI_CHANNEL_TT_LIST					ttxSubtitleList;

	// only for pvr live pause
	unsigned char						currentProgramRating;

}SI_CHANNEL_EXT_V4;

typedef struct _SI_CHANNEL_EXT_V5
{
	SI_CHANNEL_V5					super;

	SI_CHANNEL_EXT					*next;
	SI_CHANNEL_EXT					*previous;

	unsigned int						frequency;
	int								offset;
	unsigned int						bandwidth;
	unsigned short					onid;
	unsigned short					tsID;
	unsigned short					programNumber;

	unsigned int						channelOrder;

	SI_SERVICE_TYPE						serviceType;
	SI_CHANNEL_FLAG						channelFlags;

	unsigned int						oriChannelNameLen;
	unsigned char						oriChannelName[SI_CH_CHANNELNAME_MAXLEN];

	unsigned short						pcrPid;

	unsigned char						videoNum;	//# of videos.

	unsigned char						currentVideo;
	unsigned char						currentAudio;

	SI_CHANNEL_VIDEO_INFO_V4			videos[SI_CH_VIDEO_MAXNUM];
	SI_CHANNEL_AUDIO_LIST				audioList;

	SI_CHANNEL_SUBTITLE_LIST			subtitleList;
	SI_CHANNEL_TT_LIST					teletextList;
	SI_CHANNEL_TT_LIST					ttxSubtitleList;

	// only for pvr live pause
	unsigned char						currentProgramRating;

}SI_CHANNEL_EXT_V5;


typedef struct  _SI_LPCM_INFO
{
	//unsigned  int streamCodecType;
	//unsigned int audioPrenentationType;
	int samplingRate;		//sampling freq
	int bitsPerSample;
	int numberOfChannels;
	char bHDMV;
} SI_LPCM_INFO;


typedef struct  _SI_LPCM_LIST
{
	unsigned char					count;		//not used
	SI_LPCM_INFO				lpcmList[SI_CH_AUDIO_MAXNUM];
} SI_LPCM_LIST;



//org from ssuFunctionWrapper.h
typedef struct SI_IF_ES_INFO_				SI_IF_ES_INFO_T, *SI_IF_ES_INFO_P;
typedef struct SI_IF_ES_S_INFO_				SI_IF_ES_S_INFO_T, *SI_IF_ES_S_INFO_P;

struct SI_IF_ES_INFO_
{
	UINT8								streamType;
	UINT16								esPid;
};

struct SI_IF_ES_S_INFO_
{
	INT32								infoNum;
	SI_IF_ES_INFO_T						info[0];
};

#define PMT_ES_INFO_T        SI_IF_ES_S_INFO_T
#define PMT_ES_INFO_P        SI_IF_ES_S_INFO_P
//org from ssuFunctionWrapper.h

typedef enum {
  SSU_PROPRIETARY_PROFILE      = 0x0000,
  SSU_SIMPLE_PROFILE           = 0x0001,
  SSU_ENHANCED_PROFILE         = 0x0002,
  SSU_ENHANCED_PROFILE_WITH_RC = 0x0003,
  SSU_UNKNOWN_PROFILE          = 0x0004
} SSU_PROFILE ;

struct SSU_ES_RECORD_ {
 SSU_PROFILE profile ;
 UINT32 oui ;
 UINT16 pid ;
};
typedef struct SSU_ES_RECORD_       SSU_ES_RECORD_T, *SSU_ES_RECORD_P ;

typedef struct SSU_ES_INFO_ {
 INT32 ssuEsNumber ;
 SSU_ES_RECORD_T es[1];
} SSU_ES_INFO_T, *SSU_ES_INFO_P ;
//typedef struct SSU_ES_INFO_       SSU_ES_INFO_T, *SSU_ES_INFO_P ;





#endif//_SI_TYPES_LOCAL_H

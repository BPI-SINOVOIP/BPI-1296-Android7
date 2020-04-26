#ifndef __DTV_SERVICE_DEFS_H__
#define __DTV_SERVICE_DEFS_H__

typedef long HRESULT;

#define CH_CHANNELNAME_MAXLEN	64

typedef enum
{
	NO_APP		=	-1,
	APP_DTV		=	0x0001,
	APP_TOGO	=	0x0010
} DTV_APP_TYPE;

typedef enum
{
	DTV_TUNER_STATE_INVALID,
	DTV_TUNER_STATE_IDLE,
	DTV_TUNER_STATE_PREVIEW,
	DTV_TUNER_STATE_RECORD,
	DTV_TUNER_STATE_SCHEDULE_RECORD,
	DTV_TUNER_STATE_SCHEDULE_RECORD_STANDBY,
	DTV_TUNER_STATE_TIMESHIFT
} DTV_TUNER_STATE;


typedef enum {
	SCAN_INFO_STATE,
	SCAN_INFO_PROGRESS,
	SCAN_INFO_CHANNEL_COUNT,
	SCAN_INFO_CHANNEL_STRENGTH,
	SCAN_INFO_CHANNEL_QUALITY,
	SCAN_INFO_CHANNEL_FREQUENCY,
	SCAN_INFO_CHANNEL_BANDWIDTH,
	SCAN_INFO_CURRENT_CHANNEL_NUMBER,
} TV_SCAN_INFO;

typedef enum
{
	CHANNEL_FLAGS_IGNORE 		= 0x00000000,
	CHANNEL_FLAGS_IS_DELETED 	= 0x00000001,
	CHANNEL_FLAGS_IS_LOCKED 	= 0x00000002,
	CHANNEL_FLAGS_IS_FAVORITE 	= 0x00000004,
	CHANNEL_FLAGS_IS_HIDDEN 	= 0x00000008,
	CHANNEL_FLAGS_DISAPPEARED 	= 0x00000200,
	CHANNEL_FLAG_IS_DTV			= 0x00100000,
	CHANNEL_FLAG_IS_RADIO		= 0x00200000,
} CHANNEL_FLAGS;	// It must sync with SI_CHANNEL_FLAG

typedef enum
{
	SERVICE_TYPE_UNDEFINED				= 0x00,	// reserved for future use
	SERVICE_TYPE_DTV					= 0x01,	// digital television service (MPEG-2 SD)
	SERVICE_TYPE_RADIO					= 0x02,	// digital radio sound service (MPEG-1 Layer 2 audio)
	SERVICE_TYPE_TELETEXT				= 0x03,	// Teletext service
	SERVICE_TYPE_NVOD_REFERENCE			= 0x04,	// NVOD reference service
	SERVICE_TYPE_NVOD_TIMESHIFT			= 0x05,	// NVOD time-shifted service
	SERVICE_TYPE_MOSAIC					= 0x06,	// mosaic service
	SERVICE_TYPE_FMRADIO				= 0x07,	// FM radio service
	SERVICE_TYPE_DVB_SRM				= 0x08,	// DVB SRM service
	SERVICE_TYPE_RESERVED				= 0x09,	// reserved for future use
	SERVICE_TYPE_ADV_RADIO				= 0x0A,	// advanced codec digital radio sound service
	SERVICE_TYPE_ADV_MOSAIC				= 0x0B,	// advanced codec mosaic service
	SERVICE_TYPE_DATA					= 0x0C,	// data broadcast service
	SERVICE_TYPE_CIU					= 0x0D,	// reserved for Common Interface Usage
	SERVICE_TYPE_RCS_MAP				= 0x0E,	// RCS Map
	SERVICE_TYPE_RCS_FLS				= 0x0F,	// RCS FLS
	SERVICE_TYPE_MHP					= 0x10,	// DVB MHP service
	SERVICE_TYPE_VIDEO_HD				= 0x11,	// MPEG-2 HD digital television service
	SERVICE_TYPE_ADVANCED_CODEC_SD_TV	= 0x16,	// advanced codec SD digital television service
	SERVICE_TYPE_ADVANCED_CODEC_HD_TV	= 0x19,	// advanced codec HD digital television service
} SERVICE_TYPE;	// It must sync with SI_SERVICE_TYPE

typedef enum {
	CODEC_UNKNOWN = 0,
	CODEC_VIDEO_11172,
	CODEC_VIDEO_13818,
	CODEC_VIDEO_14496,
	CODEC_VIDEO_H264,
	CODEC_VIDEO_VC1,	// privateData (int) : VC1 profile ( privatDataSize = sizeof(int))
       
	CODEC_AUDIO_11172,
	CODEC_AUDIO_13818,
	CODEC_AUDIO_AC3,
	CODEC_AUDIO_E_AC3,
	CODEC_AUDIO_DTS,
	CODEC_AUDIO_AAC,
	CODEC_AUDIO_LATM_AAC,
	CODEC_AUDIO_LPCM,	// privateData (LPCM_SETTING)
	CODEC_AUDIO_HDMV_MLP,	// trueHD
	CODEC_AUDIO_HDMV_AC3,
	CODEC_AUDIO_HDMV_DDP,
	CODEC_AUDIO_HDMV_DTS,
	CODEC_AUDIO_HDMV_DTS_HD,
		
#ifdef ADD_BLURAY_SP
	CODEC_SP_HDMV,
#else
	//used to keep each value behind this consistent if ADD_BLURAY_SP undefined.
	CODEC_SP_HDMV_UNUSED,
#endif		

	//For bug 21306. For backward compatible, it is STRONGLY recommended
	//to add a new codec type in the end.
	//#ifdef SUPPORT_AVS_CODEC
	CODEC_VIDEO_AVS,
	//#endif
	CODEC_IG_HDMV,
		
	CODEC_VIDEO_MVC_DEPENDENT,
		
	CODEC_AUDIO_WFD_LPCM,
	CODEC_DEFAULT,
	CODEC_OTHERS
} CODEC_TYPE;	// it must sync with SI_CODEC_TYPE in SiEngine.h

typedef struct
{
	bool			isValid;
	unsigned short	logicalChNum;
	unsigned short	userChNum;
	unsigned int	frequency;
	unsigned int	bandwidth;
	SERVICE_TYPE	type;
	CHANNEL_FLAGS	flags;
	unsigned int	channelNameLen;
	unsigned char	channelName[CH_CHANNELNAME_MAXLEN];
} CHANNEL_INFO;

#define	SHORT_LANG_STR_LEN		4
#define	LANG_STR_LEN			30
#define	SUBTITLE_MAXNUM			32
#define	TTX_SUBTITLE_MAXNUM		6
#define	TELETEXT_MAXNUM			6
#define AUDIO_MAXNUM			8
#define AUDIO_LANG_MAXNUM		2

typedef struct
{
	unsigned short	pid;
	unsigned int	langCode;	// refer to SI_LANG in SITypes.h
	unsigned char	shortLangStr[SHORT_LANG_STR_LEN];
	unsigned char	isHardOfHearing;
	//unsigned char	langStr[LANG_STR_LEN];
} SUBTITLE_INFO;

typedef struct
{
	unsigned char	count;
	SUBTITLE_INFO	list[SUBTITLE_MAXNUM];
} SUBTITLE_LIST;

typedef struct
{
	unsigned short	pid;
	unsigned int	langCode;	// refer to SI_LANG in SITypes.h
	unsigned char	shortLangStr[SHORT_LANG_STR_LEN];
	//unsigned char	langStr[LANG_STR_LEN];
	unsigned char	magazineNum;
	unsigned char	pageNum;
} TTX_SUBTITLE_INFO;

typedef struct
{
	unsigned char		count;
	TTX_SUBTITLE_INFO	list[TTX_SUBTITLE_MAXNUM];
} TTX_SUBTITLE_LIST;

typedef struct
{
	unsigned short	pid;
	unsigned int	langCode;	// refer to SI_LANG in SITypes.h
	unsigned char	shortLangStr[SHORT_LANG_STR_LEN];
	//unsigned char	langStr[LANG_STR_LEN];
	unsigned char	magazineNum;
	unsigned char	pageNum;
} TELETEXT_INFO;

typedef struct
{
	unsigned char		count;
	TTX_SUBTITLE_INFO	list[TELETEXT_MAXNUM];
} TELETEXT_LIST;


typedef struct
{
	unsigned short		pid;
	unsigned char		streamType;
	CODEC_TYPE			type;
	unsigned char		langCount;
	unsigned int		langCode[AUDIO_LANG_MAXNUM];
	unsigned char		shortLangStr[AUDIO_LANG_MAXNUM][SHORT_LANG_STR_LEN];
} AUDIO_INFO;

typedef struct
{
	unsigned char	count;
	AUDIO_INFO		list[AUDIO_MAXNUM];
} AUDIO_LIST;

typedef enum
{	
	TT_USER_ACTION_ENTER = 0,
	TT_USER_ACTION_NEXT_PAGE,
	TT_USER_ACTION_PREVIOUS_PAGE,
	TT_USER_ACTION_NEXT_SUBPAGE,
	TT_USER_ACTION_PREVIOUS_SUBPAGE,
	TT_USER_ACTION_LINK1,
	TT_USER_ACTION_LINK2,
	TT_USER_ACTION_LINK3,
	TT_USER_ACTION_LINK4
} TELETEXT_USER_ACTION;

typedef enum
{
	SIGNAL_STATUS_NO_SIGNAL,
	SIGNAL_STATUS_ONLY_RF,
	SIGNAL_STATUS_HAS_SIGNAL_NORMAL,
	SIGNAL_STATUS_HAS_SIGNAL_SCRAMBLED
} SIGNAL_STATUS;

#define CH_PROGRAMNAME_MAXLEN	64	//	it should sync with SI_CH_PROGRAMNAME_MAXLEN in SITypes.h
#define	CH_PROGRAMDESC_MAXLEN	1024	// it should sync with SI_CH_PROGRAMDESC_MAXLEN in SITypes.h

typedef struct 
{
	bool			isValid;
	unsigned short	eventID;
	unsigned int	beginDateTime;
	unsigned int	endDateTime;
	unsigned char	iso639LanguageCode[3];
	unsigned int	titleLen;
	unsigned char	title[CH_PROGRAMNAME_MAXLEN];
	unsigned int	detailDescrLen;
	unsigned char	detailDescr[CH_PROGRAMDESC_MAXLEN];
	unsigned char	rating;
	unsigned char	audioNum;
	unsigned char	hasSubtitle:1;
	unsigned char	hasTeletext:1;
	unsigned char	hasTTXSubtitle:1;
	unsigned char	isScrambled:1;
} EPG_PROGRAM;

typedef enum _tagRecordErrorCode {
	SUCCESS,
	FAIL_NO_SIGNAL,	
	FAIL_COPY_PROTECTED,
	FAIL_VIDEO_FORMAT_NOT_MATCH,
	FAIL_TITLE_LIMIT_EXCEED,
	FAIL_NO_SPACE,
	FAIL_NO_DISC,
	FAIL_POWEROFF,
	FAIL_BAD_DISC,
	FAIL_UNWRITABLE_PARTITION,
	FAIL_RECORD_FILES_OVER_LIMIT,
	FAIL_UNKNOWN,
} RECORD_ERR_CODE;

typedef enum _tv_region {
    TV_REGION_NONE,
    TV_REGION_ARGENTINA,
    TV_REGION_AUSTRALIA,
    TV_REGION_AUSTRIA,
    TV_REGION_BELGIUM,
    TV_REGION_BRAZIL,
    TV_REGION_CHINA,
    TV_REGION_CZECH,
    TV_REGION_DENMARK,
    TV_REGION_FINLAND,
    TV_REGION_FRANCE,
    TV_REGION_GERMANY,
    TV_REGION_GREECE,
    TV_REGION_HUNGARY,
    TV_REGION_IRELAND,
    TV_REGION_ITALY,
    TV_REGION_JAPAN,
    TV_REGION_KOREA,
    TV_REGION_LUXEMBOURG,
    TV_REGION_NETHERLANDS,
    TV_REGION_NORWAY,
    TV_REGION_POLAND,
    TV_REGION_PORTUGAL,
    TV_REGION_RUSSIA,
    TV_REGION_SLOVAKIA,
    TV_REGION_SLOVENIA,
    TV_REGION_SPAIN_PENINSULA_Y_BALEARES,	// bug 25817, Evora iSuite v2.3, ESP/159
    TV_REGION_SPAIN_CANARIAS,				// bug 25817, Evora iSuite v2.3, ESP/159
    TV_REGION_SWEDEN,
    TV_REGION_SWITZERLAND,
    TV_REGION_TAIWAN,
    TV_REGION_TURKEY,
    TV_REGION_UNITED_KINGDOM,
    TV_REGION_USA,
    TV_REGION_NUMBER
} ENUM_TV_REGION;	// it should sync with ENUM_TV_REGION in channeldef.h

#endif


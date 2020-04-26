#ifndef _SI_API_H_
#define _SI_API_H_
/*
#if defined(WIN32) | defined(DVR)
	#include "rtd_types.h"
#else
	#include <rtd_types.h>
#endif
*/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_USER_AUDIO_LIST				8
#define MAX_USER_TELETEXT_LIST			8
#define MAX_USER_SUBTITLE_LIST			8
#define MAX_USER_COUNT					3

#define	MAX_SDDS_PID_COUNT				4

#define CH_SHORTNAME_LEN				14


#define SI_UINT32	unsigned int
#define SI_UINT16	unsigned short
#define SI_UINT8	unsigned char
#define SI_INT32	int
#define SI_INT16	short
#define SI_INT8		char


typedef enum _SI_BOOL {
	SI_FALSE = 0,
	SI_TRUE = 1
} SI_BOOL;

//*************************************************************
// ENUM
//*************************************************************
#ifdef DVR
typedef enum _TPMOD {
	TP_MAIN = 0,
	TP_SUB,
	TP_THIRD,
	TP_MAX
} TPMOD;
#else
typedef enum _TPMOD {
	TP_MAIN = 0,
	TP_SUB,
	TP_MAX
} TPMOD;
#endif

typedef enum _SI_MODE {
	SI_MODE_INACTIVE = 0,
	SI_MODE_MONITOR_CH,
	SI_MODE_SCAN_CH,
	SI_MODE_WATCH_CH
} SI_MODE;

typedef enum _SI_TV_MODE {
	SI_TV_TERRESTRIAL = 0,
	SI_TV_OPEN_CABLE,
	SI_TV_OPEN_FILE,
	SI_TV_MODE_MAX
} SI_TV_MODE;

typedef enum _SI_MESSAGE {
	SI_MESSAGE_NULL = 0,
	SI_STREAM_CHANGE_BEGIN,
	SI_STREAM_CHANGE_END,
	SI_FOUND_A_NEW_CH,
	SI_CH_INFO_READY,
	SI_RESET_CHANNEL,
	SI_NEW_PMT,
	SI_GOT_EIT0,
	SI_GOT_EIT,
	SI_EPG_RESERVATION,
	SI_ALARM_TIMEOUT,
	SI_RESET_TIME,
	SI_GOT_DRRT,
	SI_GOT_EAM,
	SI_MESSAGE_MAX
} SI_MESSAGE;

typedef enum _SI_ACTION_TYPE {
	SI_ACTION_NORMAL_PLAY = 0,
	SI_ACTION_NORMAL_RECORD,
	SI_ACTION_TIMESHIFT_PLAY,
	SI_ACTION_TIMESHIFT_RECORD,
	SI_ACTION_LIVEPAUSE_PLAY,
	SI_ACTION_LIVEPAUSE_RECORD,
	SI_ACTION_TYPE_MAX
} SI_ACTION_TYPE;

typedef enum _SI_CACHE_LEVEL {
	SI_CACHE_LEVEL1,		// 1 channel + 256Kb cache size
	SI_CACHE_LEVEL2,		// 3 channels + 512Kb cache size
	SI_CACHE_LEVEL3,		// 5 channels + 1024kb cache size
	SI_CACHE_LEVEL4,		// 7 channels + 1024kb cache size
	SI_CACHE_LEVEL5			// 9 channels + 1536kb cache size
} SI_CACHE_LEVEL;

typedef enum _SI_ENCODING {
	SI_ENCODING_ASCII,
	SI_ENCODING_UTF8,
	SI_ENCODING_UTF16
} SI_ENCODING;

typedef enum _SI_AUDIO_CODEC {
	SI_AUDIO_CODEC_UNKNOWN = 0,
	SI_AUDIO_CODEC_MPEG1,
	SI_AUDIO_CODEC_MPEG2,
	SI_AUDIO_CODEC_AAC,
	SI_AUDIO_CODEC_AC3,
	SI_AUDIO_CODEC_E_AC3,
	SI_AUDIO_CODEC_DTS,
	SI_AUDIO_CODEC_HDMV_LPCM,
	SI_AUDIO_CODEC_HDMV_DTS,
	SI_AUDIO_CODEC_HDMV_DOLBY_LOSELESS,
	SI_AUDIO_CODEC_HDMV_DOLBY_DIGITAL_PLUS,
	SI_AUDIO_CODEC_HDMV_DTS_HD_NO_XLL,
	SI_AUDIO_CODEC_HDMV_DTS_HD_XLL,
	SI_AUDIO_CODEC_HDMV_DOLBY_DIGITAL_PLUS_SEC,
	SI_AUDIO_CODEC_HDMV_DTS_HD_LBR,
	SI_AUDIO_CODEC_HDMV_AC3,
	SI_AUDIO_CODEC_LATM_AAC,
} SI_AUDIO_CODEC;

typedef enum _SI_MODULATION {
	SI_MOD_UNKNOWN = 0,
	SI_MOD_ATV,
	SI_MOD_OQPSK,
	SI_MOD_QAM64,
	SI_MOD_QAM256,
	SI_MOD_VSB8,
	SI_MOD_VSB16,
	SI_MOD_QAM16,
	SI_MOD_QAM32,
	SI_MOD_QAM80,
	SI_MOD_QAM96,
	SI_MOD_QAM112,
	SI_MOD_QAM128,
	SI_MOD_QAM160,
	SI_MOD_QAM192,
	SI_MOD_QAM224,
	SI_MOD_QAM320,
	SI_MOD_QAM384,
	SI_MOD_QAM448,
	SI_MOD_QAM512,
	SI_MOD_QAM640,
	SI_MOD_QAM768,
	SI_MOD_QAM896,
	SI_MOD_QAM1024,
	SI_MOD_QPSK,
	SI_MOD_BPSK,
	SI_MOD_RESERVED
} SI_MODULATION;

typedef enum _SI_TIMEZONE {
	SI_TIMEZONE_HAWAII = -10,
	SI_TIMEZONE_ALASKA,
	SI_TIMEZONE_PACIFIC,
	SI_TIMEZONE_MOUNTAIN,
	SI_TIMEZONE_CENTRAL,
	SI_TIMEZONE_EASTERN,
} SI_TIMEZONE;

typedef enum _SI_LANG {
	SI_LANG_UNKNOWN = 0,
	SI_LANG_ENG,			// English
	SI_LANG_FRE,			// French
	SI_LANG_GER,			// German
	SI_LANG_CHI,			// Chinese
	SI_LANG_ZHO,			// Chinese
	SI_LANG_JPN,			// Japanese
	SI_LANG_SPA,			// Spanish
	SI_LANG_FIN,			// Finnish
	SI_LANG_ITA,			// Italian
	SI_LANG_DAN,			// Danish
	SI_LANG_TUR,			// Turkish
	SI_LANG_KOR,			// Korean
	SI_LANG_POR,			// Portuguese
	SI_LANG_POL,			// Polish
	SI_LANG_RUS,			// Russian
	SI_LANG_DUT,			// Dutch
	SI_LANG_NLA,			// Dutch
	SI_LANG_SWE,			// Swedish
	SI_LANG_CZE,			// Czech
	SI_LANG_TON,			// Tonga

	// fix ATSC compile error for bug 25704, Evora iSuite v2.2, ESP/120
	SI_LANG_AUS,			// Australian
	SI_LANG_GLA,			// Gaelic
	SI_LANG_NLD,			// Dutch
	SI_LANG_FRA,			// French
	SI_LANG_DEU,			// German
	SI_LANG_CES,			// Czech
	SI_LANG_UND,			// Not defined
	SI_LANG_GLG,			// Galician
	SI_LANG_BAQ,			// Basque
	SI_LANG_EUS,			// Basque
	SI_LANG_CAT,			// Catalan
	SI_LANG_GRE,			// Greek
	SI_LANG_ELL,			// Greek
	SI_LANG_GLE,			// Irish
	SI_LANG_LTZ,			// Luxembourgish
	SI_LANG_NOR,			// Norwegian
	SI_LANG_WEL,			// Welsh
	SI_LANG_CYM,			// Welsh
	SI_LANG_BUL,			// Bulgarian
	SI_LANG_ICE,			// Icelandic
	SI_LANG_ISL,			// Icelandic
	SI_LANG_LAV,			// Latvian
	SI_LANG_SLO,			// Slovak
	SI_LANG_SLK,			// Slovak
	SI_LANG_EST,			// Estonian
	SI_LANG_HUN,			// Hungarian
	SI_LANG_SLV,			// Slovenian
	SI_LANG_HRV,			// Croatian
	SI_LANG_LIT,			// Lithuanian
	SI_LANG_UKR,			// Ukrainian
	SI_LANG_BEL,			// Belarusian
	// bug 26663, Evora iSuite v2.3, SWE/489
	SI_LANG_SMI,			// Sami
	SI_LANG_HEB,			// Hebrew, bug 28817
	
	SI_LANG_MAX
} SI_LANG;

typedef enum {
	SI_CATTYPE_NOTAVAILABLE = 0x00,
	SI_CATTYPE_HOBBY = 0x4F,
	SI_CATTYPE_ART = 0x80,
	SI_CATTYPE_HOCKEY = 0x50,
	SI_CATTYPE_AUTORACING = 0x81,
	SI_CATTYPE_EDUCATION = 0x20,
	SI_CATTYPE_HOME = 0x51,
	SI_CATTYPE_AVIATION = 0x82,
	SI_CATTYPE_ENTERTAINMENT = 0x21,
	SI_CATTYPE_HORROR = 0x52,
	SI_CATTYPE_BIOGRAPHY = 0x83,
	SI_CATTYPE_MOVIE = 0x22,
	SI_CATTYPE_INFORMATION = 0x53,
	SI_CATTYPE_BOATING = 0x84,
	SI_CATTYPE_NEWS = 0x23,
	SI_CATTYPE_INSTRUCTION = 0x54,
	SI_CATTYPE_BOWLING = 0x85,
	SI_CATTYPE_RELIGIOUS = 0x24,
	SI_CATTYPE_INTERNATIONAL = 0x55,
	SI_CATTYPE_BOXING = 0x86,
	SI_CATTYPE_SPORTS = 0x25,
	SI_CATTYPE_INTERVIEW = 0x56,
	SI_CATTYPE_CARTOON = 0x87,
	SI_CATTYPE_OTHER = 0x26,
	SI_CATTYPE_LANGUAGE = 0x57,
	SI_CATTYPE_CHILDREN = 0x88,
	SI_CATTYPE_ACTION = 0x27,
	SI_CATTYPE_LEGAL = 0x58,
	SI_CATTYPE_CLASSIC_FILM = 0x89,
	SI_CATTYPE_ADVERTISEMENT = 0x28,
	SI_CATTYPE_LIVE = 0x59,
	SI_CATTYPE_COMMUNITY = 0x8A,
	SI_CATTYPE_ANIMATED = 0x29,
	SI_CATTYPE_LOCAL = 0x5A,
	SI_CATTYPE_COMPUTERS = 0x8B,
	SI_CATTYPE_ANTHOLOGY = 0x2A,
	SI_CATTYPE_MATH = 0x5B,
	SI_CATTYPE_COUNTRY_MUSIC = 0x8C,
	SI_CATTYPE_AUTOMOBILE = 0x2B,
	SI_CATTYPE_MEDICAL = 0x5C,
	SI_CATTYPE_COURT = 0x8D,
	SI_CATTYPE_AWARDS = 0x2C,
	SI_CATTYPE_MEETING = 0x5D,
	SI_CATTYPE_EXTREME_SPORTS = 0x8E,
	SI_CATTYPE_BASEBALL = 0x2D,
	SI_CATTYPE_MILITARY = 0x5E,
	SI_CATTYPE_FAMILY = 0x8F,
	SI_CATTYPE_BASKETBALL = 0x2E,
	SI_CATTYPE_MINISERIES = 0x5F,
	SI_CATTYPE_FINANCIAL = 0x90,
	SI_CATTYPE_BULLETIN = 0x2F,
	SI_CATTYPE_MUSIC = 0x60,
	SI_CATTYPE_GYMNASTICS = 0x91,
	SI_CATTYPE_BUSINESS = 0x30,
	SI_CATTYPE_MYSTERY = 0x61,
	SI_CATTYPE_HEADLINES = 0x92,
	SI_CATTYPE_CLASSICAL = 0x31,
	SI_CATTYPE_NATIONAL = 0x62,
	SI_CATTYPE_HORSE_RACING = 0x93,
	SI_CATTYPE_COLLEGE = 0x32,
	SI_CATTYPE_NATURE = 0x63,
	SI_CATTYPE_HUNTING_FISHING_OUTDOORS = 0x94,
	SI_CATTYPE_COMBAT = 0x33,
	SI_CATTYPE_POLICE = 0x64,
	SI_CATTYPE_INDEPENDENT = 0x95,
	SI_CATTYPE_COMEDY = 0x34,
	SI_CATTYPE_POLITICS = 0x65,
	SI_CATTYPE_JAZZ = 0x96,
	SI_CATTYPE_COMMENTARY = 0x35,
	SI_CATTYPE_PREMIER = 0x66,
	SI_CATTYPE_MAGAZINE = 0x97,
	SI_CATTYPE_CONCERT = 0x36,
	SI_CATTYPE_PRERECORDED = 0x67,
	SI_CATTYPE_MOTORCYCLE_RACING = 0x98,
	SI_CATTYPE_CONSUMER = 0x37,
	SI_CATTYPE_PRODUCT = 0x68,
	SI_CATTYPE_MUSIC_FILM_BOOKS = 0x99,
	SI_CATTYPE_CONTEMPORARY = 0x38,
	SI_CATTYPE_PROFESSIONAL = 0x69,
	SI_CATTYPE_NEWSINTERNATIONAL = 0x9A,
	SI_CATTYPE_CRIME = 0x39,
	SI_CATTYPE_PUBLIC = 0x6A,
	SI_CATTYPE_NEWSLOCAL = 0x9B,
	SI_CATTYPE_DANCE = 0x3A,
	SI_CATTYPE_RACING = 0x6B,
	SI_CATTYPE_NEWSNATIONAL = 0x9C,
	SI_CATTYPE_DOCUMENTARY = 0x3B,
	SI_CATTYPE_READING = 0x6C,
	SI_CATTYPE_NEWSREGIONAL = 0x9D,
	SI_CATTYPE_DRAMA = 0x3C,
	SI_CATTYPE_REPAIR = 0x6D,
	SI_CATTYPE_OLYMPICS = 0x9E,
	SI_CATTYPE_ELEMENTARY = 0x3D,
	SI_CATTYPE_REPEAT = 0x6E,
	SI_CATTYPE_ORIGINAL = 0x9F,
	SI_CATTYPE_EROTICA = 0x3E,
	SI_CATTYPE_REVIEW = 0x6F,
	SI_CATTYPE_PERFORMING_ARTS = 0xA0,
	SI_CATTYPE_EXERCISE = 0x3F,
	SI_CATTYPE_ROMANCE = 0x70,
	SI_CATTYPE_PETS_ANIMALS = 0xA1,
	SI_CATTYPE_FANTASY = 0x40,
	SI_CATTYPE_SCIENCE = 0x71,
	SI_CATTYPE_POP = 0xA2,
	SI_CATTYPE_FARM = 0x41,
	SI_CATTYPE_SERIES = 0x72,
	SI_CATTYPE_ROCK = 0xA3,
	SI_CATTYPE_FASHION = 0x42,
	SI_CATTYPE_SERVICE = 0x73,
	SI_CATTYPE_SCIFI = 0xA4,
	SI_CATTYPE_FICTION = 0x43,
	SI_CATTYPE_SHOPPING = 0x74,
	SI_CATTYPE_SELFIMPROVEMENT = 0xA5,
	SI_CATTYPE_FOOD = 0x44,
	SI_CATTYPE_SOAPOPERA = 0x75,
	SI_CATTYPE_SITCOM = 0xA6,
	SI_CATTYPE_FOOTBALL = 0x45,
	SI_CATTYPE_SPECIAL = 0x76,
	SI_CATTYPE_SKATING = 0xA7,
	SI_CATTYPE_FOREIGN = 0x46,
	SI_CATTYPE_SUSPENSE = 0x77,
	SI_CATTYPE_SKIING = 0xA8,
	SI_CATTYPE_FUNDRAISER = 0x47,
	SI_CATTYPE_TALK = 0x78,
	SI_CATTYPE_SOCCER = 0xA9,
	SI_CATTYPE_GAME_QUIZ = 0x48,
	SI_CATTYPE_TECHNICAL = 0x79,
	SI_CATTYPE_TRACK_FIELD = 0xAA,
	SI_CATTYPE_GARDEN = 0x49,
	SI_CATTYPE_TENNIS = 0x7A,
	SI_CATTYPE_TRUE = 0xAB,
	SI_CATTYPE_GOLF = 0x4A,
	SI_CATTYPE_TRAVEL = 0x7B,
	SI_CATTYPE_VOLLEYBALL = 0xAC,
	SI_CATTYPE_GOVERNMENT = 0x4B,
	SI_CATTYPE_VARIETY = 0x7C,
	SI_CATTYPE_WRESTLING = 0xAD,
	SI_CATTYPE_HEALTH = 0x4C,
	SI_CATTYPE_VIDEO = 0x7D,
	SI_CATTYPE_HIGHSCHOOL = 0x4D,
	SI_CATTYPE_WEATHER = 0x7E,
	SI_CATTYPE_NULL = 0xFF,
	SI_CATTYPE_HISTORY = 0x4E,
	SI_CATTYPE_WESTERN = 0x7F,
	SI_CATTYPE_BASICRESERVED = 0x01,
	SI_CATTYPE_DETAILRESERVED = 0xAE
} SI_CATTYPE_CODE;

typedef enum _SI_RATING {	
	SI_RATING_UNKNOWN,	//get rating message,but can't recognize
	SI_RATING_TV_NONE,
	SI_RATING_TV_G,
	SI_RATING_TV_PG,
	SI_RATING_TV_14,
	SI_RATING_TV_MA,
	SI_RATING_DIALOG,
	SI_RATING_LANGUAGE,
	SI_RATING_SEX,
	SI_RATING_VIOLENCE,
	SI_RATING_TV_Y,
	SI_RATING_TV_Y7,
	SI_RATING_TV_FANTASY,
	SI_RATING_MPAA_NA,
	SI_RATING_MPAA_G,
	SI_RATING_MPAA_PG,
	SI_RATING_MPAA_PG13,
	SI_RATING_MPAA_R,
	SI_RATING_MPAA_NC17,
	SI_RATING_MPAA_X,
	SI_RATING_MPAA_NORATED,
	SI_RATING_EN_E,
	SI_RATING_EN_C,
	SI_RATING_EN_C8PLUS,
	SI_RATING_EN_G,
	SI_RATING_EN_PG,
	SI_RATING_EN_14PLUS,
	SI_RATING_EN_18PLUS,
	SI_RATING_FR_E,
	SI_RATING_FR_G,
	SI_RATING_FR_8PLUS,
	SI_RATING_FR_13PLUS,
	SI_RATING_FR_16PLUS,
	SI_RATING_FR_18PLUS,
} SI_RATING;

typedef enum _SI_TELETEXT_TYPE {
	SI_TT_TYPE_RESERVED = 0,
	SI_TT_TYPE_INITIAL_PAGE,
	SI_TT_TYPE_AIP,					// Additional information page.
	SI_TT_TYPE_PSP,					// Program schedule page.
	SI_TT_TYPE_HEARING_IMPAIRED		// For hearing impaired.
} SI_TELETEXT_TYPE;

typedef enum _SI_POSITION {
	SI_TOP = 0,
	SI_BOTTOM,
	SI_CENTER
} SI_POSITION;

typedef enum _SI_SERVICE {
	SI_SERVICE_ATV = 0x01,	// analog_television
	SI_SERVICE_DTV = 0x02,	// ATSC_digital_television
	SI_SERVICE_AUDIO = 0x03,// ATSC_audio
	SI_SERVICE_DATA = 0x04	// ATSC_data_only_service.(comforming to ATSC standards,					but no video of stream_type 0x02 or audio of stream_type0x81.)
} SI_SERVICE;

typedef enum _SI_ERRMSG {
	SI_ERRMSG_OK = 0,
	SI_ERRMSG_OOM,				// Out of Memory.
	SI_ERRMSG_INVALID_INPUT,
	SI_ERRMSG_RESERVATION_FULL,
	SI_ERRMSG_RESERVATION_PAST,
	SI_ERRMSG_RESERVATION_CONFLICT,
} SI_ERRMSG;


//------- for development usage only----------//
typedef enum _SI_PID_FILTER_TYPE {

	SI_PID_FILTER_UNKNOWN = 0,
    SI_PID_FILTER_SI,
    SI_PID_FILTER_SUBTITLE,
    SI_PID_FILTER_TELETEXT

} SI_PID_FILTER_TYPE;
//------- for development usage only----------//


//*************************************************************
// STRUCT
//*************************************************************
#define	DTV_CHNUM_BUF_SIZE	16
typedef struct _PHY_INFO {
	SI_UINT32		chNum;
	SI_UINT32		frequency;
	SI_UINT32		bandwidth;
	char			chNum_c[DTV_CHNUM_BUF_SIZE];
} PHY_INFO;


//------------- Channel Related -----------------------
typedef struct _SI_TUNER_PARAM {

	SI_UINT32			frequency;
	SI_UINT32			bandwidth;
	SI_UINT32			physicalChNum;
	SI_MODULATION		modulation;

} SI_TUNER_PARAM;

typedef struct _SI_SET_CHANNEL_PARAM {

	SI_UINT32			frequency;
	SI_UINT32			bandwidth;
	SI_INT16			physicalChNum;
	SI_MODULATION		modulation;
	SI_UINT16			majorNum;
	SI_UINT16			minorNum;
	SI_UINT16			videoPid;
	SI_UINT16			audioPid;

} SI_SET_CHANNEL_PARAM;


typedef struct _SI_LPCM_SETTING
{
	int bitsPerSample;
	int numberOfChannels;
	int samplingRate;
	char bHDMV;
	
} SI_LPCM_SETTING;


typedef struct _SI_AUDIO {

	SI_UINT16			audioPid;
	SI_UINT8			audioType;
	SI_UINT8			langId;
	SI_AUDIO_CODEC		codecType;
	SI_LPCM_SETTING		lpcm;

} SI_AUDIO;

typedef struct _SI_AUDIO_LIST {

	SI_UINT8			count;		// total number of audio in this struct.
	SI_UINT8			defaultIndex;	// this is the default language index.
	SI_AUDIO			list[MAX_USER_AUDIO_LIST];

} SI_AUDIO_LIST;

typedef struct _SI_SDDS_LIST {

	SI_UINT32			count;
	SI_UINT16			pidList[MAX_SDDS_PID_COUNT];

} SI_SDDS_LIST;

typedef struct _SI_SUBTITLE {

	SI_UINT16			pid;
	SI_UINT8			reserved;	// not used.
	SI_UINT8			type;
	SI_UINT16			compositionId;
	SI_UINT16			ancillaryId;

	SI_LANG				langId;

} SI_SUBTITLE;

typedef struct _SI_SUBTITLE_LIST {

	SI_UINT8			count;		// total number of audio in this struct.
	SI_UINT8			defaultIndex;	// this is the default language index.
	SI_SUBTITLE			list[MAX_USER_SUBTITLE_LIST];

} SI_SUBTITLE_LIST;

typedef struct _SI_TELETEXT {

	SI_UINT16			pid;
	SI_UINT8			magazine;
	SI_UINT8			page;
	SI_LANG				langId;
	SI_TELETEXT_TYPE	type;	

} SI_TELETEXT;

typedef struct _SI_TELETEXT_LIST {

	SI_UINT8			count;		// total number of audio in this struct.
	SI_UINT8			defaultIndex;	// this is the default language index.
	SI_TELETEXT			list[MAX_USER_TELETEXT_LIST];

} SI_TELETEXT_LIST;

typedef struct _SI_CHANNEL {

	SI_UINT32			frequency;
	SI_UINT32			bandwidth;
	SI_INT16			physicalChNum;
	SI_UINT16			chIndex;
	SI_INT16			majorNum;
	SI_INT16			minorNum;
	SI_MODULATION		modulation;

	SI_UINT8			serviceType:3;
	SI_UINT8			redistributionControl:1;
	SI_UINT8			hasAtscCC:1;
	SI_UINT8			hasNtscCC:1;
	SI_UINT8			hideGuide:1;
	SI_UINT8			isFavorite:1;

	SI_UINT8			isLocked:1;
	SI_UINT8			isDeleted:1;
	SI_UINT8			isIgnored:1;
	SI_UINT8			isScrambled:1;
	SI_UINT8			isHidden:1;
	SI_UINT8			reserved1:3;

	SI_UINT8			videoStreamType;
	SI_UINT8			vc1Profile;
	
	SI_UINT16			timeshiftBaseMajor;
	SI_UINT16			timeshiftBaseMinor;
	SI_UINT16			minuteShifted;

	SI_UINT16			progNum;
	SI_UINT16			sourceId;
	SI_UINT16			pcrPid;
	SI_UINT16			pmtPid;
	
	SI_UINT16			videoPid;
	SI_AUDIO_LIST		audioList;
	SI_SUBTITLE_LIST	subtitleList;
	SI_TELETEXT_LIST	teletextList;

#ifdef ENABLE_SDDS
	SI_SDDS_LIST	sddsList;
#endif

	SI_UINT8			shortName[CH_SHORTNAME_LEN*2+2];

} SI_CHANNEL;


typedef struct _SI_SHORT_CHANNEL {

	SI_UINT32			frequency;
	SI_INT16			physicalChNum;
	SI_UINT16			chIndex;
	SI_INT16			majorNum;
	SI_INT16			minorNum;
	SI_MODULATION		modulation;
	SI_UINT8			shortName[CH_SHORTNAME_LEN*2+2];

	SI_UINT8			isFavorite:1;
	SI_UINT8			isLocked:1;
	SI_UINT8			isDeleted:1;
	SI_UINT8			isIgnored:1;
	SI_UINT8			isScrambled:1;
	SI_UINT8			isHidden:1;

} SI_SHORT_CHANNEL;

typedef struct _SI_CHANNEL_INDEX_LIST {

	SI_UINT32*			list;
	SI_UINT32			count;

} SI_CHANNEL_INDEX_LIST;

typedef struct _SI_CHANNEL_FILTER {

	SI_UINT8			scrambled:1;
	SI_UINT8			locked:1;
	SI_UINT8			deleted:1;
	SI_UINT8			ignored:1;
	SI_UINT8			hidden:1;
	SI_UINT8			hiddenGuide:1;
	SI_UINT8			nonFavorite:1;

	SI_UINT8			nonTv:1;		// this filters out any channel without audio/video.
	SI_UINT8			atv:1;			// this filters out analog TV
	SI_UINT8			dtv:1;			// this filters out dtv
	SI_UINT8			avDtv:1;		// this filters out dtv with both video + audio.
	SI_UINT8			audioDtv:1;		// this filters out dtv with only audio.
	SI_UINT8			noPmt:1;		// this filters out dtv with no pmt.

} SI_CHANNEL_FILTER;

typedef struct _SI_CHANNEL_LIST {

	SI_UINT16			chCount;
	SI_SHORT_CHANNEL	*list;

} SI_CHANNEL_LIST;

typedef struct _CHLIST_PARAM {

	SI_UINT16			refMajorNum;	// Reference channel number,
	SI_UINT16			refMinorNum;
	SI_UINT32			refFrequency;
	SI_UINT8			closestRefChMatch;	// This will automatically find the current/previous ref channel.
	SI_UINT16			maxCount;		// Number of channels to get.
	SI_POSITION			refPosition;	// location to place reference channel in output.

} CHLIST_PARAM;

typedef struct _SPECIAL_CH_PARAM {

	SI_UINT16			startMajorNum;	// Reference channel number,
	SI_UINT16			startMinorNum;
	SI_UINT32			startFrequency;
	SI_UINT16			getChBefore;	// Gets channels before start channel.
	SI_UINT16			maxCount;		// Number of channels to get.
	SI_SERVICE			serviceType;

} SPECIAL_CH_PARAM;


typedef struct _FAV_LIST_PARAM {

	SI_UINT16			startMajorNum;	// Reference channel number,
	SI_UINT16			startMinorNum;
	SI_UINT32			frequency;
	SI_UINT16			favListId;
	SI_UINT16			maxCount;		// Number of channels to get.

} FAV_LIST_PARAM;


//------------- Rating Related -----------------------
typedef struct _SI_RATING_TEXT {

	SI_RATING			ratingId;
	SI_UINT16			regionId;
	SI_UINT8			dimesionId;
	SI_UINT8			valueId;
	SI_UINT16			textEncoding;
	SI_UINT16			regionNameLen;
	SI_UINT16			abbrevRatingLen;
	SI_UINT16			fullRatingLen;
	SI_UINT8*			regionName;
	SI_UINT8*			abbrevRating;
	SI_UINT8*			fullRating;

} SI_RATING_TEXT;


typedef struct _SI_MULTI_RATING {

	SI_UINT32			count;
	SI_RATING_TEXT*		list;

} SI_MULTI_RATING;


//------------- Rating Table Related -----------------------
typedef struct _SI_DRRT_VALUE {
	SI_UINT16				abbrevRatingLen;
	SI_UINT16				fullRatingLen;
	SI_UINT8*				abbrevRating;
	SI_UINT8*				fullRating;
} SI_DRRT_VALUE;


typedef struct _SI_DRRT_MULTI_VALUE {

	SI_UINT32				count;
	SI_DRRT_VALUE*			list;

} SI_DRRT_MULTI_VALUE;

typedef struct _SI_DRRT_DIMENSION {

	SI_UINT8				graduatedScale;		// graduated_scale
	SI_UINT16				nameLen;			// dimension name length.
	SI_UINT8*				name;				// demension name.
	SI_DRRT_MULTI_VALUE		value;				// rating value text 

} SI_DRRT_DIMENSION;

typedef struct _SI_DRRT_MULTI_DIMENSION {

	SI_UINT32				count;
	SI_DRRT_DIMENSION*		list;

} SI_DRRT_MULTI_DIMENSION;

typedef struct _SI_DRRT_CONTENT {

	SI_UINT8				regionId;
	SI_UINT16				regionNameLen;
	SI_UINT8*				regionName;
	SI_DRRT_MULTI_DIMENSION	dimension;			// rating dimensions.

} SI_DRRT_CONTENT;


//------------- EPG Related -----------------------
typedef struct _SI_CATEGORICAL_CODE {

	SI_UINT8			catCount;
	SI_CATTYPE_CODE*	catList;

} SI_CATEGORICAL_CODE;

typedef struct _SI_EVENT_DATETIME {    
	
	// date
	SI_INT32			year;		// the Christian era: like 2005 
	SI_INT8				month;		// 1~12
	SI_INT8				day;		// 1~31 

	// time
	SI_INT8				hour;		// 00~23
	SI_INT8				minute;		// 0~59
	SI_INT8				second;		// 0~59

	SI_INT8				weekday;	// 0~6 (sunday to saturday)

} SI_EVENT_DATETIME;

typedef struct _SI_EVENT {

	SI_UINT16			majorChNum;
	SI_UINT16			minorChNum;
	SI_UINT32			frequency;
	
	SI_UINT8			chShortName[CH_SHORTNAME_LEN*2+2];

	SI_INT16			eventId;
	SI_UINT16			textEncoding;
	SI_UINT8*			title;
    SI_UINT32			titleLen;
    
	SI_UINT32			beginDateTime;
    SI_UINT32			endDateTime;

    SI_UINT8*			detailDescr;
	SI_UINT32			detailDescrLen;
    	            
	SI_CATEGORICAL_CODE	categoryCode;

	SI_INT8				audioNum;

	SI_UINT8			hasAtscCC;	// TRUE/FALSE
	SI_UINT8			hasNtscCC;	// TRUE/FALSE
	SI_UINT8			userReserved:1;
	SI_UINT8			isScrambled:1;
	SI_UINT8			redistributionControl:6;

	SI_MULTI_RATING		multiRating;

} SI_EVENT;

typedef struct _SI_MULIT_EVENT {
	int					count;

	SI_UINT8*			chLongName;
	SI_UINT32			chLongNameLen;

	SI_EVENT*			list;
} SI_MULTI_EVENT;

typedef struct _SI_EVENT_PARAM {

	SI_UINT16			majorNum;
	SI_UINT16			minorNum;
	SI_UINT32			frequency;

	SI_UINT32			startTime;
	SI_UINT32			endTime;
	SI_UINT16			maxCount;
	SI_UINT16			sortAsc;	// Sort by BeginDateTime ascending.

} SI_EVENT_PARAM;


typedef struct _SI_ATV {

	SI_UINT32			frequency;
	SI_UINT32			chNum;	

} SI_ATV;



#ifdef ENABLE_EAM
typedef struct _SI_EMERGENCY_INFO {

	SI_UINT32		frequency;
	SI_UINT16		majorChNum;
	SI_UINT16		minorChNum;
	SI_UINT8		originatorCode[3];
	SI_UINT8		alertPriority;
	SI_UINT32		startTime;
	SI_UINT16		duration;
	SI_UINT8		alertTimeRemaining;
	SI_UINT8		natureOfActivationTxtLen;
	SI_UINT8*		natureOfActivationTxt;
	SI_UINT8		eventCodeLen;
	SI_UINT8*		eventCode;
	SI_UINT16		alertTextLen;
	SI_UINT8*		alertText;

} SI_EMERGENCY_INFO;
#endif


//*************************************************************
// SI library functions.
//*************************************************************
#ifdef DEBUG_SI
	void SiApi_CheckMemoryLeak(TPMOD tpmod);
#endif

SI_BOOL SiApi_Init(TPMOD tpmod, SI_CACHE_LEVEL cacheLevel);
SI_BOOL SiApi_DeInit(TPMOD tpmod);
SI_BOOL SiApi_UpdateRating(TPMOD tpmod);
SI_BOOL SiApi_IsInit(TPMOD tpmod);
SI_BOOL SiApi_StopActivity(TPMOD tpmod);
SI_BOOL SiApi_ResetTp(TPMOD tpmod);
SI_BOOL SiApi_RemoveAllChannels(TPMOD tpmod);

SI_MODE SiApi_GetMode(TPMOD tpmod);
SI_BOOL SiApi_SetMode(TPMOD tpmod, SI_MODE mode);

SI_BOOL SiApi_UseUtf8Encoding(TPMOD tpmod, SI_BOOL enable);
SI_BOOL SiApi_SetTimeZone(TPMOD tpmod, SI_TIMEZONE timezone);
SI_BOOL SiApi_SetLanguage(TPMOD tpmod, SI_LANG langId);
SI_BOOL SiApi_SetUserId(TPMOD tpmod, SI_UINT8 userId);
SI_BOOL SiApi_GetUserId(TPMOD tpmod, SI_UINT8 *userId);

#ifdef DVR
SI_BOOL SiApi_UseFileIfo(TPMOD tpmod, SI_BOOL enable);

SI_BOOL SiApi_ValidateSiState(SI_UINT8* pBuf, SI_UINT32 bufLen);
#endif

//-------------------
// Sets SI to Terrestrial or Open Cable mode.
//-------------------
SI_TV_MODE SiApi_GetTvMode(TPMOD tpmod);
SI_ACTION_TYPE SiApi_GetActionType(TPMOD tpmod);
SI_BOOL SiApi_SetTvMode(TPMOD tpmod, SI_TV_MODE tvMode);
SI_BOOL SiApi_SetTvModeEx(TPMOD tpmod, SI_TV_MODE tvMode, SI_ACTION_TYPE actionType, SI_BOOL resetChannel);

SI_BOOL SiApi_DumpChList(TPMOD tpmod);
//-------------------
// Channel Scan Functions.
//-------------------
SI_BOOL SiApi_IsChTableReady(TPMOD tpmod);
SI_BOOL SiApi_AutoScanChInit(TPMOD tpmod);
SI_BOOL SiApi_ManualScanChInit(TPMOD tpmod, SI_UINT16 phyChNum);
SI_BOOL SiApi_ScanFile(TPMOD tpmod);
SI_BOOL SiApi_SetTunerInfo(TPMOD tpmod, SI_TUNER_PARAM param);
SI_BOOL SiApi_SetFreqTablePath(TPMOD tpmod, SI_UINT8 *path);
SI_BOOL SiApi_GetDtvFrequencyTable(TPMOD tpmod, PHY_INFO **freqTable, int *tableSize);
SI_BOOL SiApi_FreeFrequencyTable(TPMOD tpmod, PHY_INFO *freqTable);
SI_BOOL SiApi_IsPatAvailable(TPMOD tpmod);

//-------------------
// SI Message Functions.
//-------------------
#ifdef DVR
SI_BOOL SiApi_GetMessage(TPMOD tpmod, SI_MESSAGE *pMessage, SI_UINT32 *pData);
#endif

//-------------------
// Special Analog Channel Insertion.
//-------------------
SI_BOOL SiApi_InsertCustomAtvList(TPMOD tpmod, SI_ATV *atvList, SI_UINT32 count);
SI_BOOL SiApi_RemoveCustomAtvList(TPMOD tpmod);

//-------------------
// Channel Info Functions.
//-------------------
SI_BOOL SiApi_SetChannel(TPMOD tpmod, SI_SET_CHANNEL_PARAM param);
SI_BOOL SiApi_SetChannelByIndex(TPMOD tpmod, SI_UINT16 chIndex);
SI_BOOL SiApi_SetChannelDeleted(TPMOD tpmod, SI_UINT32 frequency, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_BOOL status);
SI_BOOL SiApi_SetChannelLocked(TPMOD tpmod, SI_UINT32 frequency, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_BOOL status);
SI_BOOL SiApi_SetAudio(TPMOD tpmod, SI_UINT32 frequency, SI_INT16 physicalChNum, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT16 pid);
SI_BOOL SiApi_SetSubtitle(TPMOD tpmod, SI_UINT32 frequency, SI_INT16 physicalChNum, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT16 pid);
SI_BOOL SiApi_RemoveSubtitle(TPMOD tpmod, SI_UINT16 pid);
SI_BOOL SiApi_SetTeletext(TPMOD tpmod, SI_UINT32 frequency, SI_INT16 physicalChNum, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT16 pid);
SI_BOOL SiApi_RemoveTeletext(TPMOD tpmod, SI_UINT16 pid);

SI_BOOL SiApi_SaveUserChList(TPMOD tpmod);	// use this when delete/locked status changes.

SI_BOOL SiApi_SetChannelModulation(TPMOD tpmod, SI_UINT32 frequency, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_MODULATION modulation);
SI_BOOL SiApi_GetChannelModulation(TPMOD tpmod, SI_UINT32 frequency, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_MODULATION *modulation);

SI_BOOL SiApi_GetFirstChannel(TPMOD tpmod, SI_CHANNEL *channel);
SI_BOOL SiApi_GetFirstChannelEx(TPMOD tpmod, SI_CHANNEL *channel, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_GetLastChannel(TPMOD tpmod, SI_CHANNEL *channel);
SI_BOOL SiApi_GetLastChannelEx(TPMOD tpmod, SI_CHANNEL *channel, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_GetFirstChannelInFreq(TPMOD tpmod, SI_CHANNEL *channel, SI_UINT32 freq);
SI_BOOL SiApi_GetFirstChannelInFreqEx(TPMOD tpmod, SI_CHANNEL *channel, SI_UINT32 freq, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_GetFirstChannelInCurFreq(TPMOD tpmod, SI_CHANNEL *channel);
SI_BOOL SiApi_GetCurChannel(TPMOD tpmod, SI_CHANNEL *channel);
SI_BOOL SiApi_GetChannel(TPMOD tpmod, SI_CHANNEL *channel, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT32 frequency);
SI_BOOL SiApi_IsChannelFiltered(TPMOD tpmod, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT32 frequency, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_GetChannelByIndex(TPMOD tpmod, SI_CHANNEL *channel, SI_UINT16 chIndex);
SI_BOOL SiApi_GetNextChannel(TPMOD tpmod, SI_CHANNEL *channel, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT32 frequency, SI_BOOL autoWrap);
SI_BOOL SiApi_GetNextChannelEx(TPMOD tpmod, SI_CHANNEL *channel, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT32 frequency, SI_CHANNEL_FILTER filter, SI_BOOL autoWrap);
SI_BOOL SiApi_GetPrevChannel(TPMOD tpmod, SI_CHANNEL *channel, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT32 frequency, SI_BOOL autoWrap);
SI_BOOL SiApi_GetPrevChannelEx(TPMOD tpmod, SI_CHANNEL *channel, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT32 frequency, SI_CHANNEL_FILTER filter, SI_BOOL autoWrap);
SI_BOOL SiApi_GetChannelCount(TPMOD tpmod, int *count);
SI_BOOL SiApi_GetChannelCountEx(TPMOD tpmod, int *count, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_GetActiveFreqCount(TPMOD tpmod, int *count);

SI_BOOL SiApi_GetChIndexList(TPMOD tpmod, SI_CHANNEL_INDEX_LIST *chIndexList, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_FreeChIndexList(TPMOD tpmod, SI_CHANNEL_INDEX_LIST *chIndexList);

SI_BOOL SiApi_InitChannelList(TPMOD tpmod, SI_CHANNEL_LIST *channelList);
SI_BOOL SiApi_FreeChannelList(TPMOD tpmod, SI_CHANNEL_LIST *channelList);
SI_BOOL SiApi_GetChannelList(TPMOD tpmod, SI_CHANNEL_LIST *channelList);
SI_BOOL SiApi_GetChannelListEx(TPMOD tpmod, SI_CHANNEL_LIST *channelList, CHLIST_PARAM param, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_GetSpecialList(TPMOD tpmod, SI_CHANNEL_LIST *channelList, SPECIAL_CH_PARAM param);
SI_BOOL SiApi_GetSpecialListEx(TPMOD tpmod, SI_CHANNEL_LIST *channelList, SPECIAL_CH_PARAM param, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_GetSpecialCount(TPMOD tpmod, int *count, SI_SERVICE serviceType);
SI_BOOL SiApi_GetSpecialCountEx(TPMOD tpmod, int *count, SI_SERVICE serviceType, SI_CHANNEL_FILTER filter);
SI_BOOL SiApi_GetListInFreq(TPMOD tpmod, SI_CHANNEL_LIST *channelList, int phyChNum, int maxCount);
SI_BOOL SiApi_GetListInFreqEx(TPMOD tpmod, SI_CHANNEL_LIST *channelList, int phyChNum, int maxCount, SI_CHANNEL_FILTER filter);

SI_BOOL SiApi_GetChInputListEx(TPMOD tpmod, SI_CHANNEL_LIST *channelList, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_CHANNEL_FILTER filter);

SI_BOOL SiApi_GetPhyChNum(TPMOD tpmod, int *phyChNum, SI_UINT32 frequency);

//-------------------
// Special Channel Save/Load Functions. (used by PVR).
//-------------------
SI_BOOL SiApi_SavePhyChToFile(TPMOD tpmod, SI_UINT8 *fileName, SI_UINT32 frequency);
SI_BOOL SiApi_SavePhyChToFileEx(TPMOD tpmod, SI_UINT8 *fileName, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT32 frequency);
SI_BOOL SiApi_LoadPhyChFromFile(TPMOD tpmod, SI_UINT8 *fileName);
SI_BOOL SiApi_SavePhyChToMemory(TPMOD tpmod, SI_UINT8 **buf, SI_UINT32 *bufLen, SI_UINT16 majorNum, SI_UINT16 minorNum, SI_UINT32 frequency);
SI_BOOL SiApi_LoadPhyChFromMemory(TPMOD tpmod, SI_UINT8 *buf, SI_UINT32 bufLen);


//-------------------
// CC Functions.
//-------------------
SI_BOOL SiApi_CurChHasAtscCC(TPMOD tpmod);
SI_BOOL SiApi_CurChHasNtscCC(TPMOD tpmod);


//-------------------
// Redistribution Control.
//-------------------
SI_BOOL SiApi_CurChHasRcFlag(TPMOD tpmod);


//-------------------
// Rating Functions.
//-------------------
SI_BOOL SiApi_GetCurChRating(TPMOD tpmod, SI_MULTI_RATING *multiRating);
SI_BOOL SiApi_FreeMultiRating(TPMOD tpmod, SI_MULTI_RATING *multiRating);

SI_BOOL SiApi_RatingTableReset(TPMOD tpmod);

// regionId:
// 1 = U.S. Rating Region Table
// 2 = Canadian Rating Region Table
// 3 = Taiwan Rating Region Table
// 4 = South Korean Rating Region Table
// 5 = U.S. Rating Region Table
SI_BOOL SiApi_GetDRRT(TPMOD tpmod, SI_DRRT_CONTENT *pDrrtContent, SI_UINT32 regionId);
SI_BOOL SiApi_FreeDRRT(SI_DRRT_CONTENT *pDrrtContent);
SI_BOOL SiApi_CheckPossibleDRRT(TPMOD tpmod);

//-------------------
// Time Functions.
//-------------------
SI_BOOL SiApi_GetDayLightSaving(TPMOD tpmod, SI_BOOL *daylightSaving);
SI_BOOL SiApi_GetDayLightSavingTime(TPMOD tpmod, SI_UINT32 *dsTime);
SI_BOOL SiApi_SetAlarm(TPMOD tpmod, SI_UINT32 alarmTime);
SI_BOOL SiApi_ResetAlarm(TPMOD tpmod);
SI_BOOL SiApi_GetSourceTime(TPMOD tpmod, SI_UINT32 *srcTime);
SI_BOOL SiApi_GetTime(TPMOD tpmod, SI_UINT32 *currentTime);


//-------------------
// Epg Functions.
//-------------------
SI_BOOL SiApi_GetCurProgram(SI_EVENT *event, TPMOD tpmod);
SI_BOOL SiApi_GetNextProgram(SI_EVENT *event, TPMOD tpmod);
SI_BOOL SiApi_GetEpg(SI_MULTI_EVENT *multiEvent, TPMOD tpmod, SI_EVENT_PARAM param);

SI_BOOL SiApi_FreeMultiEventList(TPMOD tpmod, SI_MULTI_EVENT *multiEvent);
SI_BOOL SiApi_DupeMultiEventList(TPMOD tpmod, SI_MULTI_EVENT *dest, SI_MULTI_EVENT *src);
SI_BOOL SiApi_FreeEvent(TPMOD tpmod, SI_EVENT *event);
SI_BOOL SiApi_DupeEvent(TPMOD tpmod, SI_EVENT *dest, SI_EVENT *src);

SI_BOOL SiApi_ConvertIntToDateStr(SI_UINT8* dateStr, int intTime);
SI_BOOL SiApi_ConvertIntToDate(SI_EVENT_DATETIME* date, int intTime);


//-------------------
// Epg Reservation Functions.
//-------------------
SI_BOOL SiApi_ReservationSave(TPMOD tpmod);
SI_ERRMSG SiApi_AddReservation(TPMOD tpmod, SI_EVENT *event);
SI_BOOL SiApi_RemoveReservation(TPMOD tpmod, SI_EVENT *event);
SI_BOOL SiApi_RemoveAllReservation(TPMOD tpmod);
SI_BOOL SiApi_GetReservationList(TPMOD tpmod, SI_MULTI_EVENT *multiEvent);

SI_BOOL SiApi_GetExpiredEvent(TPMOD tpmod, SI_EVENT *event);


//-------------------
// Favorite Channel Functions.
//-------------------
SI_BOOL SiApi_ResetFavChList(TPMOD tpmod, SI_UINT16 favListId);
SI_BOOL SiApi_ResetAllFavChList(TPMOD tpmod);
SI_BOOL SiApi_SetFavChListName(TPMOD tpmod, SI_UINT16 favListId, SI_UINT8 *listName);
SI_BOOL SiApi_GetFavChListName(TPMOD tpmod, SI_UINT16 favListId, SI_UINT8 *listName);
SI_BOOL SiApi_AddFavCh(TPMOD tpmod, SI_UINT16 favListId, SI_SHORT_CHANNEL *channel);
SI_BOOL SiApi_RemoveFavCh(TPMOD tpmod, SI_UINT16 favListId, SI_SHORT_CHANNEL *channel);
SI_BOOL SiApi_GetFavChList(TPMOD tpmod, SI_CHANNEL_LIST *channelList, FAV_LIST_PARAM param);
SI_BOOL SiApi_GetFavChCount(TPMOD tpmod, int *count, int favListId);
SI_BOOL SiApi_SaveFavChList(TPMOD tpmod);


//-------------------
// Channel History Functions.
//-------------------
#ifdef SI_ENABLE_CH_HISTORY
SI_BOOL SiApi_GetChHistory(TPMOD tpmod, SI_CHANNEL_LIST *channelList);
SI_BOOL SiApi_ResetChHistory(TPMOD tpmod);
#endif


//-------------------
// EAM Functions.
//-------------------
#ifdef ENABLE_EAM
SI_BOOL SiApi_InitEamInfo(SI_EMERGENCY_INFO *eamInfo);
SI_BOOL SiApi_FreeEamInfo(SI_EMERGENCY_INFO *eamInfo);
SI_BOOL SiApi_GetEamInfo(TPMOD tpmod, SI_EMERGENCY_INFO *eamInfo);
#endif

//------------------
// Development functions
//------------------
SI_BOOL SiApi_SetPidfilter(TPMOD tpmod, SI_UINT16 pid, SI_PID_FILTER_TYPE type);


#ifdef __cplusplus
}
#endif

#endif

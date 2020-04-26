/*=======================================================================
 * Copyright c				Realtek Semiconductor Corporation, 2007     *
 * All rights reserved.                                                 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *
 * Module		: SI4
 *
 * Author		: ss@realtek.com.tw
 * Date			: 2007/04/23
 *
 *=======================================================================*/

#ifndef _SI_TYPES_H_
#define _SI_TYPES_H_

#include <pthread.h>
//#include <OSAL/PLI.h>

#include <SiEngine.h>

//------------------------------------------------------------------------------
//
//	Include files
//
//------------------------------------------------------------------------------

// [ss] [20070828] : for pvr..
#include <system/si4/util/siMessage.h>

#if AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)
	#include <system/si4/util/idx_table.h>
#endif//AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)


//------------------------------------------------------------------------------
//
//	Definitions
//
//------------------------------------------------------------------------------

#define RESOURCE_DIR					"./Resource/dtv_table/"

		//SI_SPEC related definition
#define SI_ENUM_STANDARD_SHIFT			24
#define SI_ENUM_COUNTRY_SHIFT			12
#define SI_ENUM_CITY_SHIFT				6

#define SI_ENUM_STANDARD_MASK			0xff000000
#define SI_ENUM_COUNTRY_MASK			0x00fff000
#define SI_ENUM_REGIONID_MASK			0x0000003f

//***kjw note
//if modify this definition, please update SI_SPEC_GET_COUNTRY, SI_SPEC_GET_REGION_ID,.. in SIMacro.h
#define SI_MAKE_SI_SPEC(std, country)	((((std)<<SI_ENUM_STANDARD_SHIFT) | ((country)<<SI_ENUM_COUNTRY_SHIFT)))
//#define SI_MAKE_SI_SPEC(std, country)	(( ((std)<<SI_ENUM_STANDARD_SHIFT) | ((country)<<SI_ENUM_COUNTRY_SHIFT) )  & (SI_ENUM_STANDARD_MASK|SI_ENUM_COUNTRY_MASK)   )
#define SI_MAKE_SI_CITY(city) 			(((city)<<SI_ENUM_CITY_SHIFT))



#define SI_API_SCANFILE_FREQUENCY			0xfffffff0
#define SI_API_SCANFILE_BANDWIDTH			0

#define SI_DB_SCHEDULEEIT_NUM			16
#define SI_DB_SECTIONHEADER_LEN			8
#define SI_DB_SECTIONCRC_LEN			4

//***don't change the following length! for si_channel_ext flash store issue	***/
#define SI_CH_CHANNELNAME_MAXLEN		64	// bug 25717, Evora iSuite v2.2, ESP/883
#define SI_CH_CHANNELDESC_MAXLEN		0xff
#define SI_CH_PROGRAMNAME_MAXLEN		64
#define SI_CH_NETWORK_NAME_MAXLEN		64
#ifdef ENABLE_PARSE_EXTENDED_EVENT_DESCRIPTOR
#define SI_CH_PROGRAMDESC_MAXLEN		1024
#else
#define SI_CH_PROGRAMDESC_MAXLEN		257	//300	// FIXBUG 20712
#endif
#define SI_CH_SINGLE_TEXT_MAXLEN		257
#define SI_CH_MAX_QUERYEVENT_NUM		64	// enlarge number to 64 for bug 17606
#define SI_CH_SUBTITLE_MAXNUM			32	// for miniBD requirement
#define SI_CH_TT_MAXNUM					6
#define SI_CH_AUDIO_MAXNUM				8
#define SI_CH_AUDIO_LANG_MAXNUM			2
#define SI_CH_VIDEO_MAXNUM				8
#define SI_CH_CA_MAXNUM					4

#define SI_COLLECTOR_MAX_PID_FILTER_NUM		64
#define SI_COLLECTOR_MAX_TP_QUEUE_NUM		4
#define SI_COLLECTOR_BEAN_HEADER_LEN		4
#define SI_COLLECTOR_NOT_A_PID_VALUE		0xffffffff

#define SI_CH_CHLIST_FILE				"/usr/local/etc/dvdplayer/dtv_channel.txt"




#define SI_MAKE_LANGCODE(a, b, c)		((a)<<16) | ((b)<<8) | (c)

//please update SI_SPEC_GET_COUNTRY, SI_SPEC_GET_REGION_ID,.. in SIMacro.h


//------------------------------------------------------------------------------
//
//	Types
//
//------------------------------------------------------------------------------
typedef unsigned int					ErrCode;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef BOOL
#define BOOL int
#endif

/*
typedef enum
{
	FALSE,
	TRUE
} BOOL ;
*/

//#define BOOL BOOL
//#define TRUE TRUE
//#define FALSE FALSE

typedef enum
{
	SI_TID_PAT							= 0x00,
	SI_TID_CAT							= 0x01,
	SI_TID_PMT							= 0x02,
	SI_TID_TSDT							= 0x03,

	SI_TID_DXI 							= 0x3B, //dsi, dii
	SI_TID_DDB 							= 0x3C,

	SI_TID_NIT_AN						= 0x40,
	SI_TID_NIT_ON						= 0x41,

	SI_TID_SDT_ATS						= 0x42,
	SI_TID_SDT_OTS						= 0x46,

	SI_TID_BAT							= 0x4A,

	SI_TID_EIT_ATS_PF					= 0x4E,
	SI_TID_EIT_OTS_PF					= 0x4F,

	SI_TID_EIT_ATS_S0					= 0x50,
	SI_TID_EIT_ATS_S1					= 0x51,
	SI_TID_EIT_ATS_S2					= 0x52,
	SI_TID_EIT_ATS_S3					= 0x53,
	SI_TID_EIT_ATS_S4					= 0x54,
	SI_TID_EIT_ATS_S5					= 0x55,
	SI_TID_EIT_ATS_S6					= 0x56,
	SI_TID_EIT_ATS_S7					= 0x57,
	SI_TID_EIT_ATS_S8					= 0x58,
	SI_TID_EIT_ATS_S9					= 0x59,
	SI_TID_EIT_ATS_SA					= 0x5A,
	SI_TID_EIT_ATS_SB					= 0x5B,
	SI_TID_EIT_ATS_SC					= 0x5C,
	SI_TID_EIT_ATS_SD					= 0x5D,
	SI_TID_EIT_ATS_SE					= 0x5E,
	SI_TID_EIT_ATS_SF					= 0x5F,

	SI_TID_EIT_OTS_S0					= 0x60,
	SI_TID_EIT_OTS_S1					= 0x61,
	SI_TID_EIT_OTS_S2					= 0x62,
	SI_TID_EIT_OTS_S3					= 0x63,
	SI_TID_EIT_OTS_S4					= 0x64,
	SI_TID_EIT_OTS_S5					= 0x65,
	SI_TID_EIT_OTS_S6					= 0x66,
	SI_TID_EIT_OTS_S7					= 0x67,
	SI_TID_EIT_OTS_S8					= 0x68,
	SI_TID_EIT_OTS_S9					= 0x69,
	SI_TID_EIT_OTS_SA					= 0x6A,
	SI_TID_EIT_OTS_SB					= 0x6B,
	SI_TID_EIT_OTS_SC					= 0x6C,
	SI_TID_EIT_OTS_SD					= 0x6D,
	SI_TID_EIT_OTS_SE					= 0x6E,
	SI_TID_EIT_OTS_SF					= 0x6F,

	SI_TID_TDT							= 0x70,
	SI_TID_RST							= 0x71,
	SI_TID_ST							= 0x72,
	SI_TID_TOT							= 0x73,
	SI_TID_DIT							= 0x7E,
	SI_TID_SIT							= 0x7F,

	SI_TID_STREAMCHANGE					= 0xFF,
} SI_TID;

typedef enum
{
	SI_DESCRIPTOR_TAG_DONT_CARE						= 0x00,

	SI_DESCRIPTOR_TAG_VIDEO_STREAM					= 0x02,
	SI_DESCRIPTOR_TAG_REGISTRATION					= 0x05,
	SI_DESCRIPTOR_TAG_DATA_STREAM_ALIGNMENT			= 0x06,
	SI_DESCRIPTOR_TAG_TARGET_BACKGROUND_GRID		= 0x07,
	SI_DESCRIPTOR_TAG_CONDITIONAL_ACCESS			= 0x09,
	SI_DESCRIPTOR_TAG_ISO_639_LANGUAGE				= 0x0A,
	SI_DESCRIPTOR_TAG_SYSTEM_CLOCK					= 0x0B,
	SI_DESCRIPTOR_TAG_SMOOTHING_BUFFER				= 0x10,
	SI_DESCRIPTOR_TAG_STD							= 0x11,

#if 1// DVB_SYSTEM

	SI_DESCRIPTOR_TAG_NETWORK_NAME					= 0x40,
	SI_DESCRIPTOR_TAG_SERVICE_LIST					= 0x41,
	SI_DESCRIPTOR_TAG_SERVICE						= 0x48,
	SI_DESCRIPTOR_TAG_LINKAGE						= 0x4A,	// bug 26488, Evora iSuite v2.3, SWE/1883
	SI_DESCRIPTOR_TAG_SHORT_EVENT					= 0x4D,
	SI_DESCRIPTOR_TAG_EXTENDED_EVENT				= 0x4E,
	SI_DESCRIPTOR_TAG_COUNTRY_AVAILABILITY			= 0x49,
	SI_DESCRIPTOR_TAG_COMPONENT						= 0x50,
	SI_DESCRIPTOR_TAG_STREAM_IDENTIFIER				= 0x52,
	SI_DESCRIPTOR_TAG_CA_IDENTIFIER					= 0x53,
	SI_DESCRIPTOR_TAG_CONTENT						= 0x54,
	SI_DESCRIPTOR_TAG_PARENTAL_RATING				= 0x55,
	SI_DESCRIPTOR_TAG_TELETEXT						= 0x56,
	SI_DESCRIPTOR_TAG_LOCAL_TIME_OFFSET				= 0x58,
	SI_DESCRIPTOR_TAG_SUBTITLING					= 0x59,
	SI_DESCRIPTOR_TAG_TERRESTRIAL_DELIVERY_SYSTEM	= 0x5A,
	SI_DESCRIPTOR_TAG_MULTILINGUAL_NETWORK_NAME		= 0x5B,
	SI_DESCRIPTOR_TAG_MULTILINGUAL_BOUQUET_NAME		= 0x5C,
	SI_DESCRIPTOR_TAG_MULTILINGUAL_SERVICE_NAME		= 0x5D,
	SI_DESCRIPTOR_TAG_MULTILINGUAL_COMPONET			= 0x5E,
	SI_DESCRIPTOR_TAG_PRIVATE_DATA_SPECIFIER		= 0x5F,
	SI_DESCRIPTOR_TAG_FREQUENCY_LIST				= 0x62,
	SI_DESCRIPTOR_TAG_DATA_BROADCAST_ID				= 0x66,
	SI_DESCRIPTOR_TAG_AC3							= 0x6A,

	SI_DESCRIPTOR_TAG_AC3_AUDIO					=0x81,	//AC3 audio descriptor
	
	SI_DESCRIPTOR_TAG_E_AC3							= 0x7A,
	SI_DESCRIPTOR_TAG_DTS							= 0x7B,
	SI_DESCRIPTOR_TAG_AAC							= 0x7C,

	SI_DESCRIPTOR_TAG_EXTENSION						= 0x7F,

	// user defined: 0x80 ~ 0xFE
	SI_DESCRIPTOR_TAG_LOGICAL_CHANNEL				= 0x83,
	SI_DESCRIPTOR_TAG_SERVICE_ATTRIBUTE				= 0x86, // D-Book
	SI_DESCRIPTOR_TAG_LCN2							= 0X87,//logical channel descriptor (version 2)
	SI_DESCRIPTOR_TAG_HD_SIMULCAST_LCD				= 0x88,	// HD Simulcast Logical Channel Descriptor

#elif ATSC_SYSTEM

	SI_DESCRIPTOR_TAG_AC3_AUDIO						= 0x81,
	SI_DESCRIPTOR_TAG_CAPTION_SERVICE				= 0x86,
	SI_DESCRIPTOR_TAG_CONTENT_ADVISORY				= 0x87,
	SI_DESCRIPTOR_TAG_EXTENDED_CHANNEL_NAME			= 0xA0,
	SI_DESCRIPTOR_TAG_SERVICE_LOCATION				= 0xA1,
	SI_DESCRIPTOR_TAG_TIME_SHIFTED_SERVICE			= 0xA2,
	SI_DESCRIPTOR_TAG_COMPONENT_NAME				= 0xA3,
	SI_DESCRIPTOR_TAG_DCC_DEPARTING_REQUEST			= 0xA8,
	SI_DESCRIPTOR_TAG_DCC_ARRIVING_REQUEST			= 0xA9,
	SI_DESCRIPTOR_TAG_REDISTRIBUTION_CONTROL		= 0xAA,
	SI_DESCRIPTOR_TAG_GENRE							= 0xAB

#endif

}SI_DESCRIPTOR_TAG;


typedef enum
{
	//user configurable flag
	SI_CHANNEL_FLAG_IGNORE			= 0x00000000,
	SI_CHANNEL_FLAG_IS_DELETED		= 0x00000001,
	SI_CHANNEL_FLAG_IS_LOCKED		= 0x00000002,
	SI_CHANNEL_FLAG_IS_FAVORITE		= 0x00000004,
	SI_CHANNEL_FLAG_IS_HIDDEN		= 0x00000008, //not used currently

	//same group
	SI_CHANNEL_FLAG_NEW				= 0x00000100,
	SI_CHANNEL_FLAG_DISAPPEARED 	= 0x00000200,
	SI_CHANNEL_FLAG_IS_OLD_KEEP		= 0x00000400,   //disappeard, but user want to keep
	SI_CHANNEL_FLAG_IS_NEW_NOT_KEEP	= 0x00000800,  //new, but user don't want to add this new service, add it for
	SI_CHANNEL_FLAG_PAT_NEW			= 0x00001000, //only new in pat's vc

	//for one project only. these are exclusive fields
	SI_CHANNEL_FLAG_IS_DTV			= 0x00100000,
	SI_CHANNEL_FLAG_IS_RADIO		= 0x00200000,
	//SI_CHANNEL_FLAG_IS_TELETEXT	= 0x00400000,

	// LCN special used
	SI_CHANNEL_FLAG_LCN_V2			= 0x01000000, // Requirement : 1285 ¡X shall give priority to LCD version 2 when both LCD version 1 and LCD version 2 are present in the DTT environment 
	SI_CHANNEL_FLAG_LCN_LOWPRIORITY	= 0x02000000, // Fix bug 25646, SWE/451
	SI_CHANNEL_FLAG_LCN_PREF_ZONE	= 0x04000000, // Requirement 107 (ITA) ¡X assigns service numbers to losers among LCN-competing services according to the DGTVi rules Applicability
	
	//system channel status
	SI_CHANNEL_FLAG_IS_SCRAMBLED	= 0x10000000,


	SI_CHANNEL_FLAG_MASK				= 0xffffffff,

} SI_CHANNEL_FLAG;

typedef enum
{
	// fix bug 25491, Evora iSuite v2.2, ESP/2
	// The value and comments below are according to ESTI EN 300 468 V1.11.1
	SI_SERVICE_TYPE_UNDEFINED				= 0x00,	// reserved for future use
	SI_SERVICE_TYPE_DTV						= 0x01,	// digital television service (MPEG-2 SD)
	SI_SERVICE_TYPE_RADIO					= 0x02,	// digital radio sound service (MPEG-1 Layer 2 audio)
	SI_SERVICE_TYPE_TELETEXT				= 0x03,	// Teletext service
	SI_SERVICE_TYPE_NVOD_REFERENCE			= 0x04,	// NVOD reference service
	SI_SERVICE_TYPE_NVOD_TIMESHIFT			= 0x05,	// NVOD time-shifted service
	SI_SERVICE_TYPE_MOSAIC					= 0x06,	// mosaic service
	SI_SERVICE_TYPE_FMRADIO					= 0x07,	// FM radio service
	SI_SERVICE_TYPE_DVB_SRM					= 0x08,	// DVB SRM service
	SI_SERVICE_TYPE_RESERVED				= 0x09,	// reserved for future use
	SI_SERVICE_TYPE_ADV_RADIO				= 0x0A,	// advanced codec digital radio sound service
	SI_SERVICE_TYPE_ADV_MOSAIC				= 0x0B,	// advanced codec mosaic service
	SI_SERVICE_TYPE_DATA				= 0x0C,	// data broadcast service
	SI_SERVICE_TYPE_CIU						= 0x0D,	// reserved for Common Interface Usage
	SI_SERVICE_TYPE_RCS_MAP					= 0x0E,	// RCS Map
	SI_SERVICE_TYPE_RCS_FLS					= 0x0F,	// RCS FLS
	SI_SERVICE_TYPE_MHP				= 0x10,	// DVB MHP service
	SI_SERVICE_TYPE_VIDEO_HD			= 0x11,	// MPEG-2 HD digital television service
	SI_SERVICE_TYPE_ADVANCED_CODEC_SD_TV	= 0x16,	// advanced codec SD digital television service
	SI_SERVICE_TYPE_ADVANCED_CODEC_HD_TV	= 0x19,	// advanced codec HD digital television service
} SI_SERVICE_TYPE;

typedef enum
{
	SI_STREAM_TYPE_UNDEFINED			= 0x00,
		//video
	SI_STREAM_TYPE_11172_VIDEO			= 0x01,
	SI_STREAM_TYPE_13818_VIDEO			= 0x02,
	SI_STREAM_TYPE_MPEG4_VIDEO			= 0x10, //ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved
	SI_STREAM_TYPE_H264_VIDEO			= 0x1b,
	SI_STREAM_TYPE_MVC_VIDEO			= 0x20,
    SI_STREAM_TYPE_HEVC_VIDEO           = 0x24, //H265
	SI_STREAM_TYPE_VC1_VIDEO				= 0xea,	//VC1, WM9?
	SI_STREAM_TYPE_ATSC_DCII_VIDEO		= 0x80,
	//audio
	SI_STREAM_TYPE_11172_AUDIO			= 0x03,
	SI_STREAM_TYPE_13818_AUDIO			= 0x04,
	SI_STREAM_TYPE_13818_PRIVATE_DATA	= 0x06, //may also be ac3 or DTS audio
	SI_STREAM_TYPE_13818_AAC				= 0x0f,	//audio type ??
	SI_STREAM_TYPE_MPEG4_AUDIO			= 0x11,
	SI_STREAM_TYPE_AC3_AUDIO				= 0x81,		//audio ac3
		//the following are defined in blu-ray spec
	SI_STREAM_TYPE_LPCM_AUDIO			=  0x80,	//part of registration descriptor....
	SI_STREAM_TYPE_DTS_AUDIO				= 0x82, //blu-ray spec  (0x06)
	SI_STREAM_TYPE_DOLBY_LOSSLESS_AUDIO = 0x83,
	SI_STREAM_TYPE_DOLBY_PLUS_AUDIO 	= 0x84,
	SI_STREAM_TYPE_DTSHD_NO_XLL_AUDIO	= 0x85,  //DTS-HD audio except XLL
	SI_STREAM_TYPE_DTSHD_XLL				= 0x86,
	SI_STREAM_TYPE_DOLBY_PLUS_SECOND_AUDIO = 0xa1,	//second audio no support
	SI_STREAM_TYPE_DTSHD_LBR_SECOND_AUDIO = 0xa2, //second audio no support
		//end of blu-ray spec

	//misc
	SI_STREAM_TYPE_13818_PRIVATE_SEC		= 0x05,
	SI_STREAM_TYPE_13522_MHEG			= 0x07,
	SI_STREAM_TYPE_13818_DSMCC			= 0x08,
	
#ifdef SUPPORT_AVS_CODEC
	SI_STREAM_TYPE_AVS_VIDEO			= 0x42,	//GB/T XXXXX.1
#endif	
	
} SI_STREAM_TYPE;

typedef enum
{
	SI_CH_QUERY_STRATEGY_LINEAR			= 0x00000000,
	SI_CH_QUERY_STRATEGY_LOOP			= 0x00000001
} SI_CH_QUERY_STRATEGY;

typedef enum
{
	SI_CH_SORTING_STRATEGY_NOSORT		= 0x00000000,
	SI_CH_SORTING_STRATEGY_FREQUENCY	= 0x00000001,
	SI_CH_SORTING_STRATEGY_LCN			= 0x00000002,
	SI_CH_SORTING_STRATEGY_CHNAME		= 0x00000003,
	SI_CH_SORTING_STRATEGY_VERONA_EURO	= 0x00000004,
	SI_CH_SORTING_STRATEGY_VERONA_RANKING = 0x00000005,		//sort by (sid, ranking list)
	SI_CH_SORTING_STRATEGY_SERVICE_ID	= 0x00000006,
	SI_CH_SORTING_STRATEGY_ONID			= 0x00000007,

	SI_CH_SORTING_STRATEGY_DEFAULT	= 0x000000ff
} SI_CH_SORTING_STRATEGY;

/* ISO 639-2 use lowercase character to represent lang code
*
*	For each lang ocde, the first one is 639-2/T , the second one is 639-2/B
*/
typedef enum
{
	SI_LANG_UNKNOWN = 0,

	SI_LANG_ENG = SI_MAKE_LANGCODE('e', 'n', 'g'),	// English
	SI_LANG_AUS = SI_MAKE_LANGCODE('a', 'u', 's'),	// Australian
	SI_LANG_GLA = SI_MAKE_LANGCODE('g', 'l', 'a'),	// Gaelic
	SI_LANG_ITA = SI_MAKE_LANGCODE('i', 't', 'a'),	// Italian
	SI_LANG_SPA = SI_MAKE_LANGCODE('s', 'p', 'a'),	// Spanish
	SI_LANG_JPN = SI_MAKE_LANGCODE('j', 'p', 'n'),	// Japanese
	SI_LANG_FIN = SI_MAKE_LANGCODE('f', 'i', 'n'),	// Finnish
	SI_LANG_DAN = SI_MAKE_LANGCODE('d', 'a', 'n'),	// Danish
	SI_LANG_TUR = SI_MAKE_LANGCODE('t', 'u', 'r'),	// Turkish
	SI_LANG_KOR = SI_MAKE_LANGCODE('k', 'o', 'r'),	// Korean
	SI_LANG_POR = SI_MAKE_LANGCODE('p', 'o', 'r'),	// Portuguese
	SI_LANG_POL = SI_MAKE_LANGCODE('p', 'o', 'l'),	// Polish
	SI_LANG_RUS = SI_MAKE_LANGCODE('r', 'u', 's'),	// Russian
	SI_LANG_SWE = SI_MAKE_LANGCODE('s', 'w', 'e'),	// Swedish
	SI_LANG_TON = SI_MAKE_LANGCODE('t', 'o', 'n'),	// Tonga

	//above landCode only have one form

	SI_LANG_ZHO = SI_MAKE_LANGCODE('z', 'h', 'o'),	// Chinese
	SI_LANG_CHI = SI_MAKE_LANGCODE('c', 'h', 'i'),	// Chinese

	SI_LANG_NLD = SI_MAKE_LANGCODE('n', 'l', 'd'),	// Dutch //some doc use nld
	SI_LANG_NLA = SI_MAKE_LANGCODE('n', 'l', 'a'),	// Dutch //some doc use nla
	SI_LANG_DUT = SI_MAKE_LANGCODE('d', 'u', 't'),	// Dutch

	SI_LANG_FRA = SI_MAKE_LANGCODE('f', 'r', 'a'),	// French
	SI_LANG_FRE = SI_MAKE_LANGCODE('f', 'r', 'e'),	// French

	SI_LANG_DEU = SI_MAKE_LANGCODE('d', 'e', 'u'),	// German
	SI_LANG_GER = SI_MAKE_LANGCODE('g', 'e', 'r'),	// German

	SI_LANG_CES = SI_MAKE_LANGCODE('c', 'e', 's'),	// Czech
	SI_LANG_CZE = SI_MAKE_LANGCODE('c', 'z', 'e'),	// Czech

	//DVB spec iso 639 language descriptor for "original audio" Soundtrack
	SI_LANG_ORIGINAL_AUDIO = SI_MAKE_LANGCODE('q', 'a', 'a'),
	SI_LANG_QAD = SI_MAKE_LANGCODE('q', 'a', 'd'),	// bug 26880, Evora iSuite v2.3, FRA/1968

	// bug 25704, Evora iSuite v2.2, ESP/120
	SI_LANG_UND	= SI_MAKE_LANGCODE('u', 'n', 'd'),	// Not defined
	SI_LANG_GLG	= SI_MAKE_LANGCODE('g', 'l', 'g'),	// Galician
	SI_LANG_BAQ	= SI_MAKE_LANGCODE('b', 'a', 'q'),	// Basque
	SI_LANG_EUS	= SI_MAKE_LANGCODE('e', 'u', 's'),	// Basque
	SI_LANG_CAT	= SI_MAKE_LANGCODE('c', 'a', 't'),	// Catalan
	SI_LANG_GRE	= SI_MAKE_LANGCODE('g', 'r', 'e'),	// Greek
	SI_LANG_ELL	= SI_MAKE_LANGCODE('e', 'l', 'l'),	// Greek
	SI_LANG_GLE	= SI_MAKE_LANGCODE('g', 'l', 'e'),	// Irish
	SI_LANG_LTZ	= SI_MAKE_LANGCODE('l', 't', 'z'),	// Luxembourgish
	SI_LANG_NOR	= SI_MAKE_LANGCODE('n', 'o', 'r'),	// Norwegian
	SI_LANG_WEL	= SI_MAKE_LANGCODE('w', 'e', 'l'),	// Welsh
	SI_LANG_CYM	= SI_MAKE_LANGCODE('c', 'y', 'm'),	// Welsh
	SI_LANG_BUL	= SI_MAKE_LANGCODE('b', 'u', 'l'),	// Bulgarian
	SI_LANG_ICE	= SI_MAKE_LANGCODE('i', 'c', 'e'),	// Icelandic
	SI_LANG_ISL	= SI_MAKE_LANGCODE('i', 's', 'l'),	// Icelandic
	SI_LANG_LAV	= SI_MAKE_LANGCODE('l', 'a', 'v'),	// Latvian
	SI_LANG_SLO	= SI_MAKE_LANGCODE('s', 'l', 'o'),	// Slovak
	SI_LANG_SLK	= SI_MAKE_LANGCODE('s', 'l', 'k'),	// Slovak
	SI_LANG_EST	= SI_MAKE_LANGCODE('e', 's', 't'),	// Estonian
	SI_LANG_HUN	= SI_MAKE_LANGCODE('h', 'u', 'n'),	// Hungarian
	SI_LANG_SLV	= SI_MAKE_LANGCODE('s', 'l', 'v'),	// Slovenian
	SI_LANG_HRV	= SI_MAKE_LANGCODE('h', 'r', 'v'),	// Croatian
	SI_LANG_LIT	= SI_MAKE_LANGCODE('l', 'i', 't'),	// Lithuanian
	SI_LANG_UKR	= SI_MAKE_LANGCODE('u', 'k', 'r'),	// Ukrainian
	SI_LANG_BEL	= SI_MAKE_LANGCODE('b', 'e', 'l'),	// Belarusian
	// bug 26663, Evora iSuite v2.3, SWE/489
	SI_LANG_SMI	= SI_MAKE_LANGCODE('s', 'm', 'i'),	// Sami
	SI_LANG_HEB	= SI_MAKE_LANGCODE('h', 'e', 'b'),	// Hebrew, bug 28817
} SI_LANG;

typedef enum
{
	SI_AGENT_STATUS_STOPPED,
	SI_AGENT_STATUS_TO_STOP,
	SI_AGENT_STATUS_RUNNING,
} SI_AGENT_STATUS;

typedef enum
{
	SI_MODE_PLAY__				= 0x01,
	SI_MODE_RECORD__			= 0x02,
	SI_MODE_READ_FILE__			= 0x04,

	SI_MODE_PLAY				= 0x10 | SI_MODE_PLAY__,
	SI_MODE_FILE_RECORD			= 0x20 | SI_MODE_PLAY__ | SI_MODE_RECORD__,
	SI_MODE_FILE_PLAYBACK		= 0x20 | SI_MODE_PLAY__ | SI_MODE_READ_FILE__,
	SI_MODE_LIVEPAUSE_RECORD	= 0x40 | SI_MODE_RECORD__,
	SI_MODE_LIVEPAUSE_PLAYBACK	= 0x40 | SI_MODE_PLAY__ | SI_MODE_READ_FILE__,
	SI_MODE_TIMESHIFT_RECORD	= 0x80 | SI_MODE_RECORD__,
	SI_MODE_TIMESHIFT_PLAYBACK	= 0x80 | SI_MODE_PLAY__ | SI_MODE_READ_FILE__,
} SI_MODE;


//kjw note: change these enum's layout also need to change SI_SPEC_GET_COUNTRY,.. macro in SIMacros.h
//bits 31~24
typedef enum
{
	SI_STANDARD_DBOOK			= 0x01,
	SI_STANDARD_EBOOK			= 0x02,
	SI_STANDARD_NORDIG			= 0x03,
	SI_STANDARD_DMB				= 0x04,
	SI_STANDARD_DGTVI			= 0x05,
	SI_STANDARD_OTHERS			= 0x0f,
} SI_STANDARD;

//**note, if add country here, please update xCheckCountry() in si_Database_Query
//***to keep the correct enum value of theses countries, don't change value of old countries
//bits  23~12(12 bits) : 2^12: 4096 countries
typedef enum
{
    SI_COUNTRY_NONE           = 0x00,
    SI_COUNTRY_AUSTRIA        = 0x01,
    SI_COUNTRY_AUSTRALIA      = 0x02,
    SI_COUNTRY_BELGIUM        = 0x03,
    SI_COUNTRY_CHINA          = 0x04,
    
    SI_COUNTRY_CZECH_REPUBLIC = 0x05,
    SI_COUNTRY_DENMARK        = 0x06,
    
    SI_COUNTRY_FINLAND        = 0x07,
    SI_COUNTRY_FRANCE         = 0x08,
    SI_COUNTRY_GERMANY        = 0x09,
    SI_COUNTRY_GREECE         = 0x0a,
    SI_COUNTRY_HUNGARY        = 0x0b,
    SI_COUNTRY_ITALY          = 0x0c,
    SI_COUNTRY_IRELAND        = 0x0d,
    SI_COUNTRY_LUXEMBOURG     = 0x0e,
    SI_COUNTRY_NETHERLANDS    = 0x0f,
    SI_COUNTRY_NEW_ZEALAND    = 0x10,
    SI_COUNTRY_NORWAY         = 0x11,
    SI_COUNTRY_POLAND         = 0x12,
    SI_COUNTRY_PORTUGAL       = 0x13,
    SI_COUNTRY_RUSSIA         = 0x14,
    SI_COUNTRY_SLOVAKIA       = 0x15,
    SI_COUNTRY_SLOVENIA       = 0x16,
    SI_COUNTRY_SWEDEN         = 0x17,
    SI_COUNTRY_SWITZERLAND    = 0x18,
    SI_COUNTRY_SPAIN          = 0x19,
    SI_COUNTRY_TAIWAN         = 0x1a,
    SI_COUNTRY_TURKEY         = 0x1b,
    SI_COUNTRY_UK             = 0x1c,

    //note**, to keep the correct enum value of above countries, add new countries below
    //**please make enum value as small as possible
    SI_COUNTRY_OTHER=0x0fff,
} SI_COUNTRY;


//6 bits only (max 2^6 = 32 cities if each city has a unique enum value~)
typedef enum
{
	SI_CITY_NONE						= 0,
	SI_CITY_1							= 0x001,
	SI_CITY_2							= 0x002,
	SI_CITY_UK_LONDON					= 0x003,
	SI_CITY_UK_BRISTOL					= 0x004

} SI_COUNTRY_CITY;

//6 bits(to fit DVB spec)
typedef enum
{
	SI_REGION_ID_NONE					= 0,
	SI_REGION_ID_1						= 0x01,
	SI_REGION_ID_2						= 0x02,
	SI_REGION_ID_3						= 0x03,
} SI_REGION_ID;


//***kjw note
//if modify bitwidth, please update SI_SPEC_GET_COUNTRY, SI_SPEC_GET_REGION_ID,.. in SIMacro.h

//bits  31~24: standard type
//bits  23~12(12 bits): country
//bits  11~6(6 bits): city
//bits  5~0(6 bits): dvb region id

typedef enum
{
    SI_SPEC_UNDEFINED               = 0,
    
	SI_SPEC_AUSTRIA                 = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_AUSTRIA),
	SI_SPEC_AUSTRALIA               = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_AUSTRALIA),
	SI_SPEC_BELGIUM                 = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_BELGIUM),
	SI_SPEC_CHINA                   = SI_MAKE_SI_SPEC(SI_STANDARD_DMB,    SI_COUNTRY_CHINA),
	SI_SPEC_CZECH_REPUBLIC          = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_CZECH_REPUBLIC),
	SI_SPEC_DENMARK                 = SI_MAKE_SI_SPEC(SI_STANDARD_NORDIG, SI_COUNTRY_DENMARK),
	SI_SPEC_FINLAND                 = SI_MAKE_SI_SPEC(SI_STANDARD_NORDIG, SI_COUNTRY_FINLAND),
	SI_SPEC_FRANCE                  = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_FRANCE),
	SI_SPEC_GERMANY                 = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_GERMANY),
	SI_SPEC_GREECE                  = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_GREECE),
	SI_SPEC_HUNGARY                 = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_HUNGARY),
	SI_SPEC_ITALY                   = SI_MAKE_SI_SPEC(SI_STANDARD_DGTVI,  SI_COUNTRY_ITALY),
	SI_SPEC_IRELAND                 = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_IRELAND),
	SI_SPEC_LUXEMBOURG              = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_LUXEMBOURG),
	SI_SPEC_NETHERLANDS             = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_NETHERLANDS),
	SI_SPEC_NEW_ZEALAND             = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_NEW_ZEALAND),
	SI_SPEC_NORWAY                  = SI_MAKE_SI_SPEC(SI_STANDARD_NORDIG, SI_COUNTRY_NORWAY),
	SI_SPEC_POLAND                  = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_POLAND),
	SI_SPEC_PORTUGAL                = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_PORTUGAL),
	SI_SPEC_RUSSIA                  = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_RUSSIA),
	SI_SPEC_SLOVAKIA                = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_SLOVAKIA),
	SI_SPEC_SLOVENIA                = SI_MAKE_SI_SPEC(SI_STANDARD_OTHERS, SI_COUNTRY_SLOVENIA),
	SI_SPEC_SPAIN                   = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_SPAIN),
	SI_SPEC_SWEDEN                  = SI_MAKE_SI_SPEC(SI_STANDARD_NORDIG, SI_COUNTRY_SWEDEN),
	SI_SPEC_SWITZERLAND             = SI_MAKE_SI_SPEC(SI_STANDARD_EBOOK,  SI_COUNTRY_SWITZERLAND),
	SI_SPEC_TAIWAN                  = SI_MAKE_SI_SPEC(SI_STANDARD_DBOOK,  SI_COUNTRY_TAIWAN),
	SI_SPEC_TURKEY                  = SI_MAKE_SI_SPEC(SI_STANDARD_DBOOK,  SI_COUNTRY_TURKEY),
	SI_SPEC_UK                      = SI_MAKE_SI_SPEC(SI_STANDARD_DBOOK,  SI_COUNTRY_UK),

    SI_SPEC_AUSTRALIA_SYDNEY			= SI_SPEC_AUSTRALIA		| SI_MAKE_SI_CITY(0x01),
    SI_SPEC_CHINA_BEIJING				= SI_SPEC_CHINA			| SI_MAKE_SI_CITY(SI_CITY_1),
    SI_SPEC_SPAIN_PENINSULA_Y_BALEARES 	= SI_SPEC_SPAIN			| SI_REGION_ID_1,	// bug 25817, Evora iSuite v2.3, ESP/159
    SI_SPEC_SPAIN_CANARIAS		 		= SI_SPEC_SPAIN			| SI_REGION_ID_2,	// bug 25817, Evora iSuite v2.3, ESP/159
    SI_SPEC_TAIWAN_TAIPEI				= SI_SPEC_TAIWAN		| SI_MAKE_SI_CITY(0x01),
    SI_SPEC_UK_BRISTOL					= SI_SPEC_UK			| SI_MAKE_SI_CITY(SI_CITY_2),
    SI_SPEC_UK_LONDON					= SI_SPEC_UK			| SI_MAKE_SI_CITY(SI_CITY_1),
    
    SI_SPEC_OTHER,

} SI_SPEC;
//si_channel_arrange_channel_list(); SiApi_GetDtvFrequencyTable(), SI_SetRegion();  SiApi_DealString()

typedef struct {
	SI_COUNTRY	country;
	char 		*threeCharCountry;
} ISO_3166_1_COUNTRY_MAP;	// bug 25742/25739, Evora iSuite v2.2, SWE/674, ESP/183

//---------------------------------------------
// Objects
//---------------------------------------------
// general
typedef struct SI_POINT_COLLECTION		SI_POINT_COLLECTION;
struct SI_POINT_COLLECTION
{
	unsigned int						num;
	void*								pointers[0];
};

// Database
typedef struct SI_DATABASE_PROGRAM_INFO	SI_DATABASE_PROGRAM_INFO;
struct SI_DATABASE_PROGRAM_INFO
{
	unsigned short						programNumber;
	unsigned short    					pid;
};

typedef struct SI_DATABASE_PROGRAM_INFO_COLLECTION	SI_DATABASE_PROGRAM_INFO_COLLECTION;
struct SI_DATABASE_PROGRAM_INFO_COLLECTION
{
	unsigned int						programInfoNum;
	SI_DATABASE_PROGRAM_INFO			programInfos[0];
};


typedef struct SI_DATABASE_SECTION		SI_DATABASE_SECTION;
struct SI_DATABASE_SECTION
{
	unsigned int						sectionLength;
	unsigned char						sectionRawBuf[0];
};

typedef struct SI_DATABASE_SCHEDULE_EIT_S	SI_DATABASE_SCHEDULE_EIT_S;
struct SI_DATABASE_SCHEDULE_EIT_S
{
	unsigned int						lastSectionNumber;
	SI_DATABASE_SECTION					*eit[0];
};

typedef struct SI_DATABASE_VC_NODE		SI_DATABASE_VC_NODE;
typedef struct SI_DATABASE_TS_NODE		SI_DATABASE_TS_NODE;


#if AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)
//store <tuner freq,tsNode> index
typedef struct _LOGICAL_FREQ_INDEX
{
	UINDEX_TABLE*  idx2TsNode;

} LOGICAL_FREQ_INDEX;
#endif//#if AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)


struct SI;//forward declaration
typedef struct SI_DATABASE				SI_DATABASE;

//#ifdef FIX_DTV_LABS_HK_ISSUE_46
typedef struct _CONTENTDESC {
	unsigned char						contentL1;	// ref. 300 468 table 27
	unsigned char						contentL2;
} CONTENTDESC;
//#endif

// Program
typedef struct SI_PROGRAM				SI_PROGRAM;
struct SI_PROGRAM
{
//	unsigned int						frequency;
//	unsigned short						programNumber;

//	unsigned short						tsID;
//	unsigned short						onID;

	unsigned short						eventID;

//	unsigned short						logicalChNum;

	time_t								beginDateTime;
	time_t								endDateTime;

	unsigned char						iso639LanguageCode[3];

	unsigned int						titleLen;
	unsigned char						title[SI_CH_PROGRAMNAME_MAXLEN];

	unsigned int						detailDescrLen;
	unsigned char						detailDescr[SI_CH_PROGRAMDESC_MAXLEN];

	unsigned char						rating;

	unsigned char						audioNum;
//#ifdef FIX_DTV_LABS_HK_ISSUE_46
	CONTENTDESC							contentDesc;
//#endif
	unsigned char						hasSubtitle:1;
	unsigned char						hasTeletext:1;
	unsigned char						hasTTXSubtitle:1;

	unsigned char						isScrambled:1;
};

typedef struct SI_EPG					SI_EPG;
struct SI_EPG
{
	unsigned int						eventNum;
	SI_PROGRAM							events[0];
};

// Channel
typedef struct SI_CHANNEL_SUBTITLE_INFO	SI_CHANNEL_SUBTITLE_INFO;
struct SI_CHANNEL_SUBTITLE_INFO
{
	unsigned short						pid;
	unsigned short						type;
	unsigned short						compositionPageId;
	unsigned short						ancillaryPageId;
	SI_LANG								langCode;
};

typedef struct SI_CHANNEL_SUBTITLE_LIST	SI_CHANNEL_SUBTITLE_LIST;
struct SI_CHANNEL_SUBTITLE_LIST
{
	unsigned char						count;
	unsigned char						defaultIndex;
	unsigned char						lastSlotIndex;
	unsigned short						lastPid;	//the last pid that system set on(for update check)
	SI_LANG								lastLangCode;
	SI_CHANNEL_SUBTITLE_INFO			list[SI_CH_SUBTITLE_MAXNUM];
};



typedef struct SI_CHANNEL_TT_INFO		SI_CHANNEL_TT_INFO;
struct SI_CHANNEL_TT_INFO
{
	unsigned short						pid;
	unsigned char						type;
	unsigned char						magazineNum;
	unsigned char						pageNum;
	SI_LANG								langCode;
};

typedef struct SI_CHANNEL_TT_LIST		SI_CHANNEL_TT_LIST;
struct SI_CHANNEL_TT_LIST
{
	unsigned char						count;
	unsigned char						defaultIndex;
	unsigned char						lastSlotIndex;
	unsigned short						lastPid;	//the last pid that system set on(for update check)
	SI_LANG								lastLangCode;
	SI_CHANNEL_TT_INFO					list[SI_CH_TT_MAXNUM];
};


typedef struct SI_CHANNEL_AUDIO_INFO	SI_CHANNEL_AUDIO_INFO;
struct SI_CHANNEL_AUDIO_INFO
{
	unsigned short						pid;
	unsigned char						streamType;
	unsigned char							type;	//codec type
	unsigned char						langCount;
	SI_LANG								langCode[SI_CH_AUDIO_LANG_MAXNUM];
};


typedef struct SI_CHANNEL_AUDIO_LIST	SI_CHANNEL_AUDIO_LIST;
struct SI_CHANNEL_AUDIO_LIST
{
	unsigned char						count;
	unsigned char						defaultIndex;
	unsigned char						lastSlotIndex;
	unsigned short						lastPid;	//the last pid that system set on(for update check)
	SI_LANG								lastLangCode;
	SI_CHANNEL_AUDIO_INFO				list[SI_CH_AUDIO_MAXNUM];
};


typedef struct SI_CHANNEL_VIDEO_INFO		SI_CHANNEL_VIDEO_INFO;
struct SI_CHANNEL_VIDEO_INFO
{
	unsigned short						videoPid;
	unsigned char						streamType;
	unsigned char						videoType;
	SI_LANG								langCode;
	int 								profileLevel;
};


typedef struct _SI_CA_INFO
{
	unsigned short		 			systemID;
	unsigned short 					caPid;

} SI_CA_INFO;


typedef struct _SI_CA_INFO_LIST
{
	unsigned char		 				count;
	SI_CA_INFO						list[SI_CH_CA_MAXNUM];
}	SI_CA_INFO_LIST;


typedef unsigned short SI_LOGICAL_CH_NUM;

typedef struct SI_CHANNEL				SI_CHANNEL;
struct SI_CHANNEL
{
	SI_LOGICAL_CH_NUM				logicalChNum;	//change to short to distinguish lcn =0 and no lcn
	SI_LOGICAL_CH_NUM				userChNum;
	unsigned int						channelNameLen;
	unsigned char						channelName[SI_CH_CHANNELNAME_MAXLEN];
};


//*****(note) modify SI_CHANNEL_EXT will cause channel restore fail when restart & ifo file playback problem
//User need to do auto scan again
//if add(remove) field in this struct, please increase version number in SI_CHANNEL_STRUCT_VER
//& SI_PVR_IFO_VER & related codes!!(SI_Channel_Convert())
typedef struct SI_CHANNEL_EXT			SI_CHANNEL_EXT;
struct SI_CHANNEL_EXT
{
	SI_CHANNEL						super;

	SI_CHANNEL_EXT					*next;
	SI_CHANNEL_EXT					*previous;

	unsigned int						frequency;
	int								offset;				//frequency offset(add at v3)
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

	SI_CHANNEL_VIDEO_INFO				videos[SI_CH_VIDEO_MAXNUM];
	SI_CHANNEL_AUDIO_LIST				audioList;

	SI_CHANNEL_SUBTITLE_LIST			subtitleList;
	SI_CHANNEL_TT_LIST					teletextList;
	SI_CHANNEL_TT_LIST					ttxSubtitleList;

	// only for pvr live pause
	unsigned char						currentProgramRating;

};

typedef struct SI_CHANNEL_LIST			SI_CHANNEL_LIST;
struct SI_CHANNEL_LIST
{
	unsigned int						channelNum;
	unsigned int						datumChannelOffset;
	SI_CHANNEL							channels[0];
};

#if AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)
typedef struct _SI_CHANNEL_LIST_EXT
{
	unsigned int							channelNum;
	SI_CHANNEL_EXT*				channels[0];
} SI_CHANNEL_LIST_EXT;
#endif//#if AP_CONFIG_WITH(APCFG_GS_GUIDEPLUS)

typedef struct _SI_CHANNEL_LIST_EX
{
	unsigned int					channelNum;
	SI_CHANNEL_EXT				channels[1];
} SI_CHANNEL_LIST_EX;

//not used currently
typedef struct _SI_CH_LIST_PARAM
{
	int refFrequency;

} SI_CH_LIST_PARAM;


typedef struct SI_CHANNEL_MGR			SI_CHANNEL_MGR;


typedef struct SI_COLLECTOR				SI_COLLECTOR;
typedef ErrCode (*SI_Collector_SectionCallback)(SI_DATABASE *pDatabase, unsigned int sectionLength,unsigned char** ppSectionRawBuffer, unsigned int pid);


typedef enum
{
	SI_SECTIONCOLLECTOR_CBF_TYPE_SSU_SECTION,
	SI_SECTIONCOLLECTOR_CBF_TYPE_CUSTOMIZED_SECTION,

	SI_SECTIONCOLLECTOR_CBF_TYPE_NUM__

} SI_SECTIONCOLLECTOR_CBF_TYPE;


// agent
typedef struct SI_AGENT					SI_AGENT;

typedef enum 
{
	CB_PMT_TYPE_SINGLE_PMT,
	CB_PMT_TYPE_ALL_PMTS
} CB_PMT_TYPE;	// bug 26759

typedef struct
{
	CB_PMT_TYPE type;
	unsigned short programNumber;
} CB_PMT_PAYLOAD;	// bug 26759


typedef enum _SI_STATE {
	SI_STATE_AUTOSCAN					=  0x00000001,
	SI_STATE_FILE_PLAYBACK_IFO 		=  0x00000002,

} SI_STATE;

typedef enum _SI_FLAG {
	//following are exclusive
	SI_FLAG_CH_NO_UPDATE			= 0x00000001,
	SI_FLAG_CH_AUTO_UPDATE		= 0x00000002,
	SI_FLAG_CH_UPDATE_CONFIRM		= 0x00000004,  // 1: update ch need confirm from user

	SI_FLAG_DISABLE_LCN_AUTO_ORDER	= 0x00000010, // Evora iSuite v2.3, ITA/1287: Test that the receiver correctly orders services when LCN detection is disabled 
} SI_FLAG;

#define MAX_APPEAR_DISAPPEAR_CH_NUM	64					// bug 26588, Evora iSuite v2.3, GBR/394
typedef struct SI_PAT_CHANGE_INFO	SI_PAT_CHANGE_INFO;	// bug 26588, Evora iSuite v2.3, GBR/394
struct SI_PAT_CHANGE_INFO
{
	SI_DATABASE_TS_NODE	*pTs;
	unsigned int		appearVcNum;
	unsigned int		disappearChNum;
	SI_DATABASE_VC_NODE	*appearVc[MAX_APPEAR_DISAPPEAR_CH_NUM];
	SI_CHANNEL_EXT		*disappearCh[MAX_APPEAR_DISAPPEAR_CH_NUM];
	unsigned char		bUserConfirm;
};

// si api
typedef struct SI						SI;
struct SI
{
	void								*tp;

	SI_COLLECTOR						*collector;
	SI_AGENT							*agent;
	SI_DATABASE							*db;
	SI_CHANNEL_MGR						*chMgr;		//major ch mgr

	SI_PAT_CHANGE_INFO					*pPatChangeInfo;	// bug 26588, Evora iSuite v2.3, GBR/394

	SI_SPEC								spec;
	unsigned short 						userONID;//onid from user(system) setting
	SI_STATE							state;	//si state,
	SI_FLAG 								flag; //config flag
	unsigned int 							lastLCN; //b'cos it's global, so it is not put in chMgr

	// [ss] [20070828] : for pvr si porting
	SI_MODE								mode;	//for pve related status
	SI_MSG_QUEUE						mq;

#ifdef FEATURE_010
	int									timeOffset;
	unsigned char						isManualTimeOffset;
#else	
	//time zone offset( uint  is second)
	unsigned int						timeOffset;
#endif

	SI_LANG								epgLanguagePreference;
};


typedef enum
{
	CHQuery_DVB_SERVICE,	//(onid, tsid, serviceID)
	CHQuery_USER_CH_NUM, //fill userChNum and serviceType(currently)
	CHQuery_USER_CH_NUM_WITH_CH_FLAGS	// fix bug 25491, Evora iSuite v2.2, ESP/2. Fill usrChNum and channelFlags
	//CHQuery_FREQ_SERVICE,
	//CHQuery_UNIQUE_LCN,  //fill lcn field
} ChannelQueryType;

typedef struct _ChannelQueryItem
{
      ChannelQueryType queryType;
    unsigned short tsID; //ch don't have tsID
    unsigned short onID;
    unsigned short serviceID;
    SI_LOGICAL_CH_NUM userChNum;
    SI_SERVICE_TYPE serviceType;
    SI_CHANNEL_FLAG channelFlags;	// fix bug 25491, Evora iSuite v2.2, ESP/2
    //SI_LOGICAL_CH_NUM logicalChNum;
} ChannelQueryItem;


typedef struct _SI_SERVICE_RANKING {
	//unsigned short onID;
	unsigned short serviceID;
	unsigned int ranking;
	//char* name;  //channel name, not used currently

} SI_SERVICE_RANKING;


typedef struct _SI_SERVICE_RANKING_TABLE {
	int tableSize;

	SI_SERVICE_RANKING rankMap[1];
} SI_SERVICE_RANKING_TABLE;

//for software section filter public interface
typedef unsigned int  SF_MODULE_HANDLE;
typedef unsigned int  SF_HANDLE;


	//for cobat mheg5
		typedef unsigned char * (*Cabot_RequestBufferCallback)(void * user_data,
		                                                      int section_length);
		typedef void (*Cabot_SectionReadyCallback)(int service_index, void* user_data, int pid, unsigned char * data);
		typedef void (*Cabot_ReleaseBufferCallback)(void * user_data, unsigned char * data);

		typedef struct _CabotFilter {
			unsigned short pid;

			unsigned char tid;
			unsigned char tidMask;//0XFF: must match, 0: don't care (this definition is different than Cabot's one)

			unsigned char tidExt;
			unsigned char tidExtMask;

			unsigned char ver;
			unsigned char verNegativeMask;	//negative mask.  0XFF: must discard, 0:don't care

			void* userData;
			Cabot_RequestBufferCallback getBuf;
			Cabot_SectionReadyCallback sendSection;
			Cabot_ReleaseBufferCallback releaseBuf;

		} CabotFilter;

typedef struct
{
	unsigned int						nameLen;
	unsigned char						name[SI_CH_NETWORK_NAME_MAXLEN];
} SI_NETWORK;

typedef struct
{
	unsigned short	reserved:1;
	unsigned short	retension_move_mode:1;
	unsigned short	retension_state:3;
	unsigned short	epn:1;
	unsigned short	dtcp_cci:2;
	unsigned short	reserved4:4;
	unsigned short	ast:1;
	unsigned short	image_constraint_token:1;
	unsigned short	aps:2;
} SI_DTCP_PRIV_DATA;	// bug 28532

typedef struct
{
	unsigned short		CA_System_ID;
	SI_DTCP_PRIV_DATA	priv_data;
} SI_DTCP;	// bug 28532

#endif//_SI_TYPES_H_



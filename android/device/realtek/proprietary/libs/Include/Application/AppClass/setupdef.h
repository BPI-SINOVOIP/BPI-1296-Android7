#ifndef __SETUP_DEF_
#define __SETUP_DEF_

#include <OSAL.h>
//#include <Filters/vbidef.h> // marked by hamer @ 20130723
#if AP_CONFIG_WITH(APCFG_HW_ATV) || AP_CONFIG_WITH(APCFG_HW_DTV)
#include <Application/AppClass/channeldef.h>
#endif
#include <Application/AppClass/NetworkSet.h>
#include <AudioRPCBaseDS_data.h>
// **************************************************************************
// SETUP Menu : Common section
// **************************************************************************
typedef enum on_off
{
    SET_OFF,
    SET_ON,
} ENUM_ON_OFF;

// **************************************************************************
// SETUP Menu : Language section
// **************************************************************************
//languages
typedef enum osd_language
{
    OSD_LANG_INVALID  = -1,
    OSD_LANG_ENGLISH  = 0,
    OSD_LANG_CHINESE  = 1,
    OSD_LANG_JAPANESE = 2,
    OSD_LANG_SPANISH  = 3,
    OSD_LANG_FRENCH   = 4,
    OSD_LANG_GERMAN   = 5,
    OSD_LANG_ITALIAN  = 6,
    OSD_LANG_KOREAN   = 7,
    OSD_LANG_DUTCH    = 8,
    OSD_LANG_RUSSIAN  = 9,
    OSD_LANG_SCHINESE = 10,
    OSD_LANG_THAI     = 11,
    OSD_LANG_HEBREW   = 12,
    OSD_LANG_ARABIC   = 13,
    OSD_LANG_PORTUGUESE = 14,
    OSD_LANG_SUDANESE = 15,
    OSD_LANG_SWEDISH = 16,
    OSD_LANG_FINNISH = 17,
    OSD_LANG_POLISH = 18,
    OSD_LANG_TURKMEN= 19,
    OSD_LANG_DANISH = 20,
    OSD_LANG_GRECCE=21,
    OSD_LANG_CZECH=22,
    OSD_LANG_SLOVAK=23,
    OSD_LANG_HUNGARIAN=24,
    OSD_LANG_ROMANIA=25,
    OSD_LANG_CANADIAN_FRENCH=25,
    OSD_LANG_MEX_SPANISH=26,
    OSD_LANG_UKRAINE=27,
    OSD_LANG_BRA_POR=28,
    OSD_LANG_BEHARASAMELAYU=29,
    OSD_LANG_NORWEGIAN=30,
    OSD_LANG_HINDI=31,
    OSD_LANG_REALTEK=32,
    OSD_LANG_MAX_NUM  = 33
} ENUM_OSD_LANGUAGE;

typedef enum minikb_language
{
    MINIKD_LANG_INVALID = -1,
    MINIKD_LANG_ENGLISHLOWER = 0,
#if AP_CONFIG_WITH(APCFG_LAG_CHINESE)
    MINIKD_LANG_TZUIN,
    MINIKD_LANG_TCHINESE,
#endif
#if AP_CONFIG_WITH(APCFG_LAG_SCHINESE)
    MINIKD_LANG_SCHINESE,
#endif
    MINIKD_LANG_MAX_NUM,
} ENUM_MINIKBD_LANGUAGE;

typedef enum text_encoding {
    TEXT_ENCODING_UTF8     = 0,
    TEXT_ENCODING_GBK      = 1,
    TEXT_ENCODING_BIG5     = 2,
    TEXT_ENCODING_WESTERN  = 3,
    TEXT_ENCODING_TURKISH  = 4,
    TEXT_ENCODING_CENTRAL_EUROPEAN = 5,
    TEXT_ENCODING_GREEK    = 6,
    TEXT_ENCODING_CYRILLIC = 7,
    TEXT_ENCODING_HEBREW   = 8,
    TEXT_ENCODING_SOUTH_EASTERN_EUROPEAN = 9,
    TEXT_ENCODING_JAPANESE = 10,
    TEXT_ENCODING_KOREAN 	= 11,
    TEXT_ENCODING_THAI	 	= 12,
    TEXT_ENCODING_ARABIC	= 13,
    TEXT_ENCODING_MAX_NUM 	= 14,
} ENUM_TEXT_ENCODING;

typedef enum subtitle_text_color {
    SUBTITLE_TEXT_WHITE = 0,
    SUBTITLE_TEXT_BLACK = 1,
    SUBTITLE_TEXT_RED = 2,
    SUBTITLE_TEXT_GREEN = 3,
    SUBTITLE_TEXT_BLUE = 4,
    SUBTITLE_TEXT_YELLOW = 5,
    SUBTITLE_TEXT_MAX_NUM = 6,
} ENUM_SUBTITLE_TEXT_COLOR;

//ISO-639
typedef enum audio_language {
    AUDIO_LANG_ENGLISH  = 0,
    AUDIO_LANG_CHINESE  = 1,
    AUDIO_LANG_JAPANESE = 2,
    AUDIO_LANG_SPANISH  = 3,
    AUDIO_LANG_FRENCH   = 4,
    AUDIO_LANG_GERMAN   = 5,
    AUDIO_LANG_ITALIAN  = 6,
    AUDIO_LANG_KOREAN   = 7,
    AUDIO_LANG_DUTCH    = 8,
    AUDIO_LANG_RUSSIAN  = 9,
    AUDIO_LANG_SCHINESE = 10,
    AUDIO_LANG_HUNGARIAN = 11,
    AUDIO_LANG_MAX_NUM  = 12
} ENUM_AUDIO_LANGUAGE;

typedef enum subtitle_language {
    SUBTITLE_LANG_OFF      = 0,
    SUBTITLE_LANG_ENGLISH  = 1,
    SUBTITLE_LANG_CHINESE  = 2,
    SUBTITLE_LANG_JAPANESE = 3,
    SUBTITLE_LANG_SPANISH  = 4,
    SUBTITLE_LANG_FRENCH   = 5,
    SUBTITLE_LANG_GERMAN   = 6,
    SUBTITLE_LANG_ITALIAN  = 7,
    SUBTITLE_LANG_KOREAN   = 8,
    SUBTITLE_LANG_DUTCH    = 9,
    SUBTITLE_LANG_RUSSIAN  = 10,
    SUBTITLE_LANG_SCHINESE = 11,
    SUBTITLE_LANG_HUNGARIAN = 12,
    SUBTITLE_LANG_MAX_NUM  = 13
} ENUM_SUBTITLE_LANGUAGE;

typedef enum menu_language {
    MENU_LANG_ENGLISH  = 0,
    MENU_LANG_CHINESE  = 1,
    MENU_LANG_JAPANESE = 2,
    MENU_LANG_SPANISH  = 3,
    MENU_LANG_FRENCH   = 4,
    MENU_LANG_GERMAN   = 5,
    MENU_LANG_ITALIAN  = 6,
    MENU_LANG_KOREAN   = 7,
    MENU_LANG_DUTCH    = 8,
    MENU_LANG_RUSSIAN  = 9,
    MENU_LANG_SCHINESE = 10,
    MENU_LANG_MAX_NUM  = 11
} ENUM_MENU_LANGUAGE;

typedef struct _language_setting {
    ENUM_OSD_LANGUAGE osd_lang;
    ENUM_TEXT_ENCODING text_subtitle_encoding;
    ENUM_AUDIO_LANGUAGE audio_lang;
    ENUM_SUBTITLE_LANGUAGE subtitle_lang;
    ENUM_MENU_LANGUAGE menu_lang;
    ENUM_MINIKBD_LANGUAGE minibkd_lang ;
} LANGUAGE_SETTING;

#if AP_CONFIG_WITH(APCFG_HW_DTV)
typedef enum tv_input_dtv {
    AIR=0,
    CABLE_AUTO,
    CABLE_STD,
    CABLE_HRC,
    CABLE_IRC,
} ENUM_TV_INPUT_DTV;

typedef struct _channel_setting_dtv {
    ENUM_TV_INPUT_DTV tv_input;
} CHANNEL_SETTING_DTV;
#endif

// **************************************************************************
// SETUP Menu : TV section
// **************************************************************************
#if AP_CONFIG_WITH(APCFG_HW_ATV) || AP_CONFIG_WITH(APCFG_HW_DTV)
typedef enum _tv_type {
    TV_TYPE_NONE,
    TV_TYPE_ANALOG,
    TV_TYPE_DIGITAL
} ENUM_TV_TYPE;

typedef enum tv_input {
    ANTENNA,
    CABLE
} ENUM_TV_INPUT;

typedef struct _cni_station {
    unsigned short    cni;
    unsigned short    ni;
    ENUM_CNI_STATION  station;
} CNI_STATION;

typedef struct _tv_channel {
    unsigned char    status;
    unsigned char    standard;
    CNI_STATION      cni_station;
    unsigned int     freq;
    char             name[16];
    unsigned char    pay_tv;
} TV_CHANNEL_INFO;

#define CHANNEL_MAX_NUM 128
typedef struct _channel_setting {
    ENUM_TV_INPUT tv_input;
    ENUM_TV_REGION tv_region;
    ENUM_ON_OFF antenna_power;
    TV_CHANNEL_INFO channel_table[CHANNEL_MAX_NUM];
} CHANNEL_SETTING;
#endif

// **************************************************************************
// SETUP Menu : Audio Setting
// **************************************************************************
typedef enum _speaker_out {
    TWO_CHANNEL,
    FIVE_P_ONE_CHANNEL,
    SPEAKER_MAX
} ENUM_SPEAKER_OUT;

typedef enum _audio_agc_mode {
    AUDIO_AGC_DRC_OFF,
    AUDIO_AGC_DRC_ON,
    AUDIO_AGC_COMFORT
} ENUM_AUDIO_AGC_MODE;

#define COMP_SCALE_FULL ((long)0x7fffffff)
#define COMP_SCALE_HALF ((long)0x40000000)
#define COMP_SCALE_NONE ((long)0x00000000)

typedef enum _downmix_mode {
    DOWNMIX_AUTO_DETECT,
    DOWNMIX_DOLBY_SURROUND, // Lt/Rt
    DOWNMIX_STEREO          // Lo/Ro
} ENUM_DOWNMIX_MODE;

typedef enum _compress_mode {
    COMPRESS_CUSTOM_A,
    COMPRESS_CUSTOM_D,
    COMPRESS_LINEOUT,
    COMPRESS_RF
} ENUM_COMPRESS_MODE;

typedef enum _spdif_mode {
    SPDIF_RAW,
    SPDIF_LPCM,
    HDMI_RAW,
    HDMI_LPCM,
    HDMI_LPCM_MULTI_CH,
    DDP_SPDIF_RAW,	//for netflix bug RTEC-237
    SPDIF_MAX
} ENUM_SPDIF_MODE;

typedef enum _audio_digital_output_mode {
    AUDIO_DIGITAL_RAW,
    AUDIO_DIGITAL_LPCM_DUAL_CH,
    AUDIO_DIGITAL_LPCM_MULTI_CH,
    AUDIO_DIGITAL_AUTO,
    AUDIO_DIGITAL_MAX
} ENUM_AUDIO_DIGITAL_OUTPUT_MODE;

typedef enum _audio_output_channel {
    AUDIO_OUTPUT_2CH,
    AUDIO_OUTPUT_6CH,
    AUDIO_OUTPUT_8CH,
    AUDIO_OUTPUT_CHANNEL_MAX
} ENUM_AUDIO_OUTPUT_CHANNEL;

typedef struct _audio_drc_values {
	int cutValue;
	int boostValue;
} AUDIO_DRC_VALUES;

typedef enum _audio_hdmi_freq {
	AUDIO_HDMI_FREQ_27 = 27,
	AUDIO_HDMI_FREQ_74 = 74,
	AUDIO_HDMI_FREQ_148 = 148,
} AUDIO_HDMI_FREQ;

typedef struct _audio_setting {
    ENUM_SPEAKER_OUT speaker_out;
    ENUM_AUDIO_DIGITAL_OUTPUT_MODE hdmi_output_mode;
    ENUM_AUDIO_DIGITAL_OUTPUT_MODE spdif_output_mode;
    ENUM_SPDIF_MODE spdif_mode;
    ENUM_AUDIO_AGC_MODE agc_mode;
    bool dolby_prologic_enable;
	ENUM_AUDIO_OUTPUT_CHANNEL output_channel;
	int drc_cut_value;
	int drc_boost_value;
	ENUM_ON_OFF lossless_mode;
} AUDIO_SETTING;

// **************************************************************************
// SETUP Menu : Video Setting
// **************************************************************************
typedef enum video_system {
    VIDEO_NTSC,
    VIDEO_PAL,
    VIDEO_HD720_50HZ,  // 720 only have progressive
    VIDEO_HD720_60HZ,
    VIDEO_HD1080_50HZ, // 1080 can have interleaved or progressive
    VIDEO_HD1080_60HZ,
    VIDEO_HD1080_24HZ,
    VIDEO_HD2160_24HZ,	//3840*2160 24Hz
    VIDEO_HD2160_25HZ,	//3840*2160 25Hz
    VIDEO_HD2160_30HZ,	//3840*2160 30Hz
    VIDEO_HD4096_2160_24HZ,	//4096*2160 24Hz
    VIDEO_SVGA800x600,
    VIDEO_HD720_59HZ,
    VIDEO_HD1080_59HZ,
    VIDEO_HD1080_23HZ,
    VIDEO_HD2160_23HZ,
    VIDEO_HD2160_29HZ,
	VIDEO_HD2160_60HZ,	//3840*2160 60Hz
	VIDEO_HD1080_30HZ,
	VIDEO_HD2160_50HZ,	//3840*2160 50Hz
	VIDEO_HD4096_2160_50HZ, //4096*2160 50Hz
	VIDEO_HD4096_2160_60HZ, //4096*2160 60Hz
	VIDEO_HD4096_2160_25HZ, //4096*2160 25Hz
	VIDEO_HD4096_2160_30HZ, //4096*2160 30Hz
	#ifdef HW_VGA
    VIDEO_VGA1024x768,
	VIDEO_VGA1280x800,
	VIDEO_VGA1366x768,
	VIDEO_VGA1280x1024,
	VIDEO_VGA800x600,
	#endif
	VIDEO_DP_1080P_60HZ,
	VIDEO_DP_2160P_30HZ,
	VIDEO_HD2160_59HZ,
	VIDEO_HD1080_25HZ,
    VIDEO_SYSTEM_NUM,
} ENUM_VIDEO_SYSTEM;

typedef enum tv_system_auto_mode {
    TV_SYSTEM_AUTO_MODE_OFF,
    TV_SYSTEM_AUTO_MODE_PLAYBACK,
    TV_SYSTEM_AUTO_MODE_HDMI_EDID,
} ENUM_TV_SYSTEM_AUTO_MODE;

typedef enum video_hd_system {
    VIDEO_HD_NONE,
    VIDEO_HD_720P,
    VIDEO_HD_1080I,
    VIDEO_HD_SYSTEM_NUM,
} ENUM_VIDEO_HD_SYSTEM;

typedef enum video_rec_system {
    VIDEO_REC_NTSC,
    VIDEO_REC_PAL,
    VIDEO_REC_AUTO,
} ENUM_VIDEO_REC_SYSTEM;

typedef enum video_standard {
    VIDEO_INTERLACED,
    VIDEO_PROGRESSIVE,
} ENUM_VIDEO_STANDARD;

typedef enum _aspect_ratio {
    PS_4_3,
    LB_4_3,
    Wide_16_9,
    Wide_16_10,
    ASPECT_RATIO_MAX
} ENUM_ASPECT_RATIO;

typedef enum _rescale_mode {
    FIT_MODE,
    FULL_MODE,
    RESCALE_MODE_MAX
} ENUM_RESCALE_MODE;

typedef enum video_output {
    VOUT_YUV,
    VOUT_RGB,
    VOUT_MAX
} ENUM_VIDEO_OUTPUT;

#ifdef HW_VGA
typedef enum video_vga_resolution {
	VIDEO_VGA_RESOLUTION_AUTO = 0,
	VIDEO_VGA_RESOLUTION_1024X768,
	VIDEO_VGA_RESOLUTION_1280X800,
	VIDEO_VGA_RESOLUTION_1366X768,
	VIDEO_VGA_RESOLUTION_1280X1024,
	VIDEO_VGA_RESOLUTION_800X600,
	VIDEO_VGA_RESOLUTION_MAX,
} ENUM_VIDEO_VGA_RESOLUTION;
#endif

// TV setting menu
typedef enum {
	TV_SYS_HDMI_AUTO_DETECT,
	TV_SYS_NTSC,
	TV_SYS_PAL,
	TV_SYS_480P,
	TV_SYS_576P,
	TV_SYS_720P_50HZ,
	TV_SYS_720P_60HZ,
	TV_SYS_1080I_50HZ,
	TV_SYS_1080I_60HZ,
	TV_SYS_1080P_50HZ,
	TV_SYS_1080P_60HZ,
	TV_SYS_2160P_24HZ,
	TV_SYS_2160P_25HZ,
	TV_SYS_2160P_30HZ,
	TV_SYS_2160P_60HZ,
	TV_SYS_4096_2160P_24HZ,
	TV_SYS_2160P_50HZ,
	TV_SYS_4096_2160P_50HZ,
	TV_SYS_4096_2160P_60HZ,
	TV_SYS_4096_2160P_25HZ,
	TV_SYS_4096_2160P_30HZ,
	TV_SYS_NUM
} ENUM_TV_SYSTEM_USER;

#define MAX_BRIGHTNESS_VALUE     64
#define MIN_BRIGHTNESS_VALUE      0

#define MAX_CONTRAST_VALUE       64
#define MIN_CONTRAST_VALUE        0

#define MAX_HUE_VALUE            64
#define MIN_HUE_VALUE             0

#define MAX_SATURATION_VALUE     64
#define MIN_SATURATION_VALUE      0

typedef struct _video_setting {
    ENUM_VIDEO_SYSTEM video_system;    //NTSC or PAL
    ENUM_VIDEO_STANDARD video_standard;
    ENUM_VIDEO_SYSTEM video_system_manual;    //[2011/04/14]  add
    ENUM_VIDEO_STANDARD video_standard_manual;
    ENUM_VIDEO_SYSTEM video_sd_system;
    ENUM_TV_SYSTEM_AUTO_MODE tv_system_auto_mode;
    ENUM_ASPECT_RATIO aspect_ratio;
    ENUM_RESCALE_MODE rescale_mode;
    ENUM_VIDEO_OUTPUT video_output;
    ENUM_VIDEO_REC_SYSTEM video_rec_system;
    ENUM_ON_OFF auto_video_system;
    ENUM_ON_OFF dnr_mode;
    ENUM_ON_OFF zoom_out_mode;
    ENUM_ON_OFF preprocessing;
    ENUM_ON_OFF aware_copy_protection;
    int brightness;
    int contrast;
    int hue;
    int saturation;
#ifdef USE_HDMI
    int CurrentHdmiTVVid;
#endif /* USE_HDMI */
    ENUM_VIDEO_HD_SYSTEM hd_system;
    ENUM_ON_OFF video_system_1080p24hz;
    int error_concealment_level;
#ifdef USE_HDMI
    int hdmi_deep_color;
#endif
#ifdef TVE_DEC_SWITCH
    int tve_switch;
#endif
#ifdef ENABLE_HDMV_PLAYBACK
    ENUM_ON_OFF hdmv_navigation;
#endif
#ifdef HW_VGA
	ENUM_VIDEO_VGA_RESOLUTION vga_res; //20120606
#endif
} VIDEO_SETTING;

typedef struct _video_system_info {
    ENUM_VIDEO_SYSTEM video_system;
    ENUM_VIDEO_STANDARD video_standard;
} VIDEO_SYSTEM_INFO;

typedef struct _rectangle_dimension {
    int x;
    int y;
    int w;
    int h;
} RECTANGLE_DIMENSION;

// **************************************************************************
// SETUP Menu : Record Setting
// **************************************************************************

#include <video_quality.h>

typedef enum _auto_chapter {
    OFF,
    FIFTEEN_MINUTES,
    TWENTY_MINUTES,
    THIRTY_MINUTES,
    AUTO_CHAPTER_MAX,
    FIVE_MINUTES,
    TEN_MINUTES,
} ENUM_AUTO_CHAPTER;

#include <Application/AppClass/setupdef_rec_device.h>

typedef enum _rec_priority {
    MANUAL_RECORDING,
    SCHEDULE_RECORDING,
} ENUM_REC_PRIORITY;

typedef struct _rec_setting {
    ENUM_REC_QUALITY quality;
    ENUM_AUTO_CHAPTER auto_chapter;
    ENUM_ON_OFF auto_timeshift;
    ENUM_REC_DEVICE rec_device;
    ENUM_REC_DEVICE timeshift_device;
    ENUM_REC_PRIORITY rec_priority;
    char last_rec_file[256];
} REC_SETTING;

// **************************************************************************
// SETUP MENU : Play Setting
// **************************************************************************
typedef enum _slide_show_time {
    SLIDE_SHOW_OFF,
    SLIDE_SHOW_2S,
    SLIDE_SHOW_5S,
    SLIDE_SHOW_10S,
    SLIDE_SHOW_30S,
    SLIDE_SHOW_60S,
    SLIDE_SHOW_120S,
//	SLIDE_SHOW_300S,
    SLIDE_SHOW_MAX
} ENUM_SLIDE_SHOW_TIME;

typedef enum _trans_effect {
    #if defined(CASABLANCA)
    TRANS_EFFECT_OFF,
    TRANS_EFFECT_SHUFFLE,
    #if IS_CHIP(JUPITER)
    TRANS_EFFECT_FLIPING,
    TRANS_EFFECT_QUARTET,
    #else
    TRANS_EFFECT_CROSS_FADE,
    TRANS_EFFECT_WATERFALL,
    #endif
    TRANS_EFFECT_SLIDING,
    TRANS_EFFECT_CENTER_EXTEND,
    TRANS_EFFECT_2D3D_EXPAND,
    TRANS_EFFECT_DISSOLVE,
    TRANS_EFFECT_FADE_IN_OUT,
    TRANS_EFFECT_CROSS_FADE_KEN_BURN,
    TRANS_EFFECT_LEFT_TO_RIGHT,
    TRANS_EFFECT_TOP_TO_BOTTOM,
    TRANS_EFFECT_SNAKE,
    TRANS_EFFECT_STRIP_LEFT_DOWN,
	#if defined(RSSOGLES)
	TRANS_EFFECT_RIPPLE,
	TRANS_EFFECT_TEAR,
	TRANS_EFFECT_SLIDE,
	TRANS_EFFECT_PATTERN_MASK,
	TRANS_EFFECT_CUBE,
	TRANS_EFFECT_SHUTTER,
	TRANS_EFFECT_BOOK,
	TRANS_EFFECT_3D_KENBURN,
	TRANS_EFFECT_EXPOSURE,
	TRANS_EFFECT_PARTICLE,
	TRANS_EFFECT_3D_MIX_STYLE,
	TRANS_EFFECT_PAPER_FOLDING,
	#endif
    #else
    TRANS_EFFECT_OFF,
    TRANS_EFFECT_CROSS_FADE,
    TRANS_EFFECT_LEFT_TO_RIGHT,
    TRANS_EFFECT_TOP_TO_BOTTOM,
    TRANS_EFFECT_WATERFALL,
    TRANS_EFFECT_SNAKE,
    TRANS_EFFECT_DISSOLVE,
    TRANS_EFFECT_STRIP_LEFT_DOWN,
    TRANS_EFFECT_SHUFFLE,
    TRANS_EFFECT_CROSS_FADE_KEN_BURN
    #endif
} ENUM_TRANS_EFFECT;

typedef enum _mix_play {
    MIX_PLAY_OFF,
    MIX_PLAY_FOLDER,
    MIX_PLAY_PLAYLIST,
    MIX_PLAY_DEFAULT,
    MIX_PLAY_SELECTION
} ENUM_MIX_PLAY;

typedef struct _play_setting {
    ENUM_SLIDE_SHOW_TIME slide_show_time;
    ENUM_TRANS_EFFECT trans_effect;
    ENUM_MIX_PLAY mix_play;
} PLAY_SETTING;

// **************************************************************************
// SETUP Menu : System Setting
// **************************************************************************
typedef enum time_zone {
    TIME_ZONE_AUTO,
    TIME_ZONE_W120,
    TIME_ZONE_W115,
    TIME_ZONE_W110,
    TIME_ZONE_W105,
    TIME_ZONE_W100,
    TIME_ZONE_W095,
    TIME_ZONE_W090,
    TIME_ZONE_W085,
    TIME_ZONE_W080,
    TIME_ZONE_W075,
    TIME_ZONE_W070,
    TIME_ZONE_W065,
    TIME_ZONE_W060,
    TIME_ZONE_W055,
    TIME_ZONE_W050,
    TIME_ZONE_W045,
    TIME_ZONE_W040,
    TIME_ZONE_W035,
    TIME_ZONE_W030,
    TIME_ZONE_W025,
    TIME_ZONE_W020,
    TIME_ZONE_W015,
    TIME_ZONE_W010,
    TIME_ZONE_W005,
    TIME_ZONE_GMT,
    TIME_ZONE_E005,
    TIME_ZONE_E010,
    TIME_ZONE_E015,
    TIME_ZONE_E020,
    TIME_ZONE_E025,
    TIME_ZONE_E030,
    TIME_ZONE_E035,
    TIME_ZONE_E040,
    TIME_ZONE_E045,
    TIME_ZONE_E050,
    TIME_ZONE_E055,
    TIME_ZONE_E060,
    TIME_ZONE_E065,
    TIME_ZONE_E070,
    TIME_ZONE_E075,
    TIME_ZONE_E080,
    TIME_ZONE_E085,
    TIME_ZONE_E090,
    TIME_ZONE_E095,
    TIME_ZONE_E100,
    TIME_ZONE_E105,
    TIME_ZONE_E110,
    TIME_ZONE_E115,
    TIME_ZONE_E120,
    TIME_ZONE_NUM,
} ENUM_TIME_ZONE;

#define RATING_TYPE_D    0x01      //Sexually Suggestive Dialog
#define RATING_TYPE_L    0x02      //Adult Language
#define RATING_TYPE_S    0x04      //Sexual Situations
#define RATING_TYPE_V    0x08      //Violence
#define RATING_TYPE_FV   0x10      //Fantasy Violence
#define RATING_TYPE_ALL  0x20      //All of Above


#define OPENVCHIP_REGIONNAME_LEN			36
#define OPENVCHIP_DIMNAME_LEN				21
#define OPENVCHIP_VALUENAME_LEN				10
#define OPENVCHIP_REGIONTABLE_COUNT_MAX	1
#define OPENVCHIP_DIM_COUNT_MAX	255
#define OPENVCHIP_VALUE_COUNT_MAX	15
#define OPENVCHIP_DRAGON_DIMNAME_LEN 12


typedef struct _OPENVCHIP_DRAGON_RATING {
    char rating;
    unsigned char abbrevRatingLen;
    unsigned char abbrevRating[OPENVCHIP_VALUENAME_LEN];
} OPENVCHIP_DRAGON_RATING; //size = 16

typedef struct _OPENVCHIP_DRAGON_DIMENSION_NAME {
    unsigned char dimIndex;
    unsigned char nameLen;
    unsigned char graduatedScale;
    unsigned char ratingCount;
    unsigned char name[OPENVCHIP_DRAGON_DIMNAME_LEN];
} OPENVCHIP_DRAGON_DIMENSION_NAME; //size =


typedef struct _OPENVCHIP_VALUE {
//	int					abbrevRatingLen;
    unsigned char		abbrevRatingLen;
    unsigned char		abbrevRating[OPENVCHIP_VALUENAME_LEN];
} OPENVCHIP_VALUE;

typedef struct _OPENVCHIP_DIMENSION {
//	int					nameLen;
    unsigned char 	nameLen;
    unsigned char		graduatedScale;
    unsigned char		valueCount;
    unsigned char		name[OPENVCHIP_DIMNAME_LEN];
    OPENVCHIP_VALUE	value[OPENVCHIP_VALUE_COUNT_MAX];

} OPENVCHIP_DIMENSION;

typedef struct _OPENVCHIP_TABLE {
    //int					nameLen;
    unsigned char		nameLen;
    unsigned char		dimensionCount;
    char				isValid;
    unsigned char		name[OPENVCHIP_REGIONNAME_LEN]; //Region Name
    OPENVCHIP_DIMENSION	dimension[OPENVCHIP_DIM_COUNT_MAX];
} OPENVCHIP_TABLE;

typedef struct _OPEN_VCHIP_TABLE_INFO {
    //int					tableCount;
    unsigned char		tableCount;
    OPENVCHIP_TABLE	table[OPENVCHIP_REGIONTABLE_COUNT_MAX];
} OPENVCHIP_TABLE_INFO;


typedef struct _OPENVCHIP_RATING_VALUE {
    char			rating; //1: block 0: non-block
} OPENVCHIP_RATING_VALUE;

typedef struct _OPENVCHIP_RATING_DIMENSION {
    OPENVCHIP_RATING_VALUE			value[OPENVCHIP_VALUE_COUNT_MAX];
} OPENVCHIP_RATING_DIMENSION;

typedef struct _OPENVCHIP_RATING {
    OPENVCHIP_RATING_DIMENSION		dimension[OPENVCHIP_DIM_COUNT_MAX];
} OPENVCHIP_RATING;

typedef struct _OPEN_VCHIP_RATING_INFO {
    OPENVCHIP_RATING				table[OPENVCHIP_REGIONTABLE_COUNT_MAX];
} OPEN_VCHIP_RATING_INFO;

typedef struct _OPENVCHIP_CURRENT_DIMENSION {
    //int					tableIndex;
    //int					dimIndex;
    unsigned char			tableIndex;
    unsigned char			dimIndex;
    OPENVCHIP_DIMENSION		*dimension;
} OPENVCHIP_CURRENT_DIMENSION;

typedef struct _OPENVCHIP_CURRENT_RATGIN_DIMENSION {
    unsigned char				tableIndex;
    unsigned char				dimIndex;
    OPENVCHIP_RATING_DIMENSION	*dimension;
} OPENVCHIP_CURRENT_RATGIN_DIMENSION;

typedef enum _rating_level {
    RATING_OFF,
    RATE_1,
    RATE_2,
    RATE_3,
    RATE_4,
    RATE_5,
    RATE_6,
    RATE_7,
    RATE_8,
    RATE_MAX
} ENUM_RATING_LEVEL;

typedef enum _rating_region {
#ifdef DVB_T
    RATING_DEFAULT,
    RATING_FRANCE,
    RATING_GERMANY,
    RATING_ITALY,
    RATING_SWEDEN,
    RATING_SPAIN,
#else
    RATING_USA,
    RATING_CANADA,
#endif
} ENUM_RATING_REGION;

typedef enum _tv_rating_level {
    TV_RATING_OFF,
    TV_RATE_1,
    TV_RATE_2,
    TV_RATE_3,
    TV_RATE_4,
    TV_RATE_5,
    TV_RATE_6,
    TV_RATE_7,
    TV_RATE_8,
} ENUM_TV_RATING_LEVEL;

typedef struct _rating_setting {
    int isEnable;
    int isBlockUnrated;
    ENUM_RATING_REGION rating_region;
    int passwd;
    int timeout;
    ENUM_RATING_LEVEL rating_level;
//    ENUM_TV_RATING_LEVEL tv_rating_level; //USA MPAA, CAEN
//    ENUM_TV_RATING_LEVEL tv_x_rating_level; //USA TV, CAFR

    //to use NR in rating, use bitwise to present the level of rating
    int tv_rating_level; //USA MPAA, CAEN
    int tv_x_rating_level; //USA TV, CAFR
    unsigned char rating_type[RATE_MAX];
    OPENVCHIP_TABLE_INFO openVChip;
    OPEN_VCHIP_RATING_INFO openVChipRating;

} RATING;

typedef enum region_setting {
    REGION_CODE_ZERO	= 0, // Playable in all regions
    REGION_CODE_ONE     = 1,
    REGION_CODE_TWO     = 2,
    REGION_CODE_THREE   = 3,
    REGION_CODE_FOUR    = 4,
    REGION_CODE_FIVE    = 5,
    REGION_CODE_SIX     = 6,
    REGION_CODE_SEVEN   = 7, // Reserved for future use
    REGION_CODE_EIGHT   = 8, // International venues such as aircraft, cruise ships
    REGION_CODE_MAX_NUM = 9
} ENUM_REGION_SETTING;

typedef enum security_setting {
    SECURITY_ANONYMOUS,
    SECURITY_USER,
} ENUM_SECURITY_SETTING;

#ifdef USE_HDMI
typedef enum lip_sync_setting {
    LIP_SYNC_AUTO       = 0,
    LIP_SYNC_MANUAL     = 1,
    LIP_SYNC_OFF        = 2,
} ENUM_LIP_SYNC_SETTING;
#endif

typedef struct _system_setting {
    ENUM_TIME_ZONE time_zone;
    ENUM_ON_OFF day_light_saving;
    bool bAutoPlay;
    ENUM_ON_OFF seamless_playback;
    ENUM_ON_OFF angle_mark;
    ENUM_REGION_SETTING region;
    ENUM_SECURITY_SETTING security;
    ENUM_ON_OFF power_saving;
    bool initial;
    RATING rating;
	ENUM_ON_OFF	photoActualSize; // #23857
#if AP_CONFIG_WITH(APCFG_APP_UPNPDLNA1p5)
    ENUM_ON_OFF   dlna_dmr;
	ENUM_ON_OFF   dlna_dms;
	char dlna_dms_device_name[256];
	char dlna_dms_device_uuid[37];
#endif /* APCFG_APP_UPNP_1p5 */
    char system_device_uuid[37];
    char system_device_friendlyName[256];
#if AP_CONFIG_WITH(APCFG_HW_NET_WLAN0)
	ENUM_WIFI_FREQ wifiFreq;
#endif // AP_CONFIG_WITH(APCFG_HW_NET_WLAN0)
#ifdef ENABLE_WIFI_SOFT_AP
	bool bConcurrentMode;
	ENUM_SOFT_AP_MODE softwareApMode;
	ENUM_NET_REGION softwareApNetRegion;
	wireless_security softwareApSecurity;
#ifdef ENABLE_WIFI_BRIDGE_MODE
	EMUN_BRIDGE_INTERFACE wifiBridgeTarget;
#endif //ENABLE_WIFI_BRIDGE_MODE
#endif //ENABLE_WIFI_SOFT_AP
#if defined(REALTEK_WFDISPLAY) && !defined(DISABLE_WFDISPLAY_UI) && defined(WIFI_CONCURRENT)
	ENUM_P2P_CONCURRENT_STATE p2pConcurrentState;
	ENUM_ON_OFF 			p2pConcurrentSwitch;
#endif
#ifdef USE_CEC
    bool bEnableHdmiCec;
    bool bEnableHdmiCecOneTouchPlay;
    bool bEnableHdmiCecSystemStandby;
    bool bEnableHdmiCecAutoOsdLanguage;
    bool bEnableHdmiCecAutoPoweOnFromTv;
	int  CECPreferredLogicalAddress;
	bool bSystemAudioControl;
#endif
#ifdef USE_HDMI
    ENUM_LIP_SYNC_SETTING hdmiLipSyncSetting;
    int hdmiLipSyncValue;
#endif
} SYSTEM_SETTING;

/******************************************/
#if AP_CONFIG_WITH(APCFG_APP_BITTORRENT)
typedef struct _ims_setting {
    bool bImsDaemonState;
    int ImsHomeDirSetting;
    int ImsDownloadDirSetting;
}IMS_SETTING;
#endif
/******************************************/

// **************************************************************************
// SETUP Menu : Network Setting
// **************************************************************************
#include <Application/AppClass/setupdef_source_option.h>

typedef enum {
    OUTPUT_CVBS,
    OUTPUT_SVIDEO,
    OUTPUT_COMPONENT,
    OUTPUT_SCART,
    OUTPUT_DSUB,
    OUTPUT_HDMI,
    OUTPUT_MAX_NUM,
} OutputOption;

typedef enum {
    SORT_BY_NAME,
    SORT_BY_TIME,
    SORT_BY_NUMBER,
    SORT_BY_NEW_TITLE
} SortMode;

typedef enum {
    DEVICE_HDD,
    DEVICE_DISC
} DeviceOption;

typedef enum {
    REVERB_OFF,
    REVERB_ON_CATHEDRAL_1,
    REVERB_ON_CATHEDRAL_2,
    REVERB_ON_HALL,
    REVERB_ON_ROOM,
    REVERB_ON_BASEMENT
} KARAOKE_REVERB_MODE;

typedef enum VOCAL_REMOVAL_MODE {
    ENUM_VOCAL_REMOVAL_NONE,
    ENUM_VOCAL_REMOVAL_LEFT_CHANNEL,
    ENUM_VOCAL_REMOVAL_RIGHT_CHANNEL,
    ENUM_VOCAL_REMOVAL_LEFT_RIGHT_DIFFERENCE,
    ENUM_VOCAL_REMOVAL_ADVANCED
} VOCAL_REMOVAL_MODE;

typedef enum {
    DTV_REGION_TAIWAN = 0,
    DTV_REGION_DEFAULT,
    DTV_REGION_BERLIN,
    DTV_REGION_BIRMINGHAM,
    DTV_REGION_CHINA,
    DTV_REGION_COPENHAGEN,
    DTV_REGION_HAMBURG,
    DTV_REGION_HANNOVER,
    DTV_REGION_HELSINKI,
    DTV_REGION_LONDON,
    DTV_REGION_MADRID,
    DTV_REGION_PARIS,
    DTV_REGION_ROMA,
    DTV_REGION_STOCKHOLM,
    DTV_REGION_SYDNEY
} DTV_REGION;

typedef enum {
    DTV_CHANNEL_AUTO_UPDATE = 0,
    DTV_CHANNEL_USER_CONFIRM
} DTV_CHANNEL_ALTER_MODE;

//For DTV Preference Audio Language
typedef enum {
	DTV_AUDIO_LANG_BAQ = 0 ,
	DTV_AUDIO_LANG_EUS,
	DTV_AUDIO_LANG_BEL,
	DTV_AUDIO_LANG_BUL,
	DTV_AUDIO_LANG_CAT,
	DTV_AUDIO_LANG_HRV,
	DTV_AUDIO_LANG_CES,
	DTV_AUDIO_LANG_CZE,
	DTV_AUDIO_LANG_DAN,
	DTV_AUDIO_LANG_DUT,
	DTV_AUDIO_LANG_NLD,
	DTV_AUDIO_LANG_ENG,
	DTV_AUDIO_LANG_EST,
	DTV_AUDIO_LANG_FIN,
	DTV_AUDIO_LANG_FRA,
	DTV_AUDIO_LANG_FRE,
	DTV_AUDIO_LANG_GLG,
	DTV_AUDIO_LANG_DEU,
	DTV_AUDIO_LANG_GER,
	DTV_AUDIO_LANG_ELL,
	DTV_AUDIO_LANG_GRE,
	DTV_AUDIO_LANG_HUN,
	DTV_AUDIO_LANG_ICE,
	DTV_AUDIO_LANG_ISL,
	DTV_AUDIO_LANG_GLE,
	DTV_AUDIO_LANG_ITA,
	DTV_AUDIO_LANG_LAV,
	DTV_AUDIO_LANG_LIT,
	DTV_AUDIO_LANG_LTZ,
	DTV_AUDIO_LANG_NOR,
	DTV_AUDIO_LANG_POL,
	DTV_AUDIO_LANG_POR,
	DTV_AUDIO_LANG_RUS,
	DTV_AUDIO_LANG_SLO,
	DTV_AUDIO_LANG_SLV,
	DTV_AUDIO_LANG_SPA,
	DTV_AUDIO_LANG_SWE,
	DTV_AUDIO_LANG_UKR,
	DTV_AUDIO_LANG_CYM,
	DTV_AUDIO_LANG_WEL,
} ENUM_DTV_AUDIO_LANG;

//For DTV Preference Subtitle Language
typedef enum {
	DTV_SUBTITLE_LANG_BAQ = 0 ,
	DTV_SUBTITLE_LANG_EUS,
	DTV_SUBTITLE_LANG_BEL,
	DTV_SUBTITLE_LANG_BUL,
	DTV_SUBTITLE_LANG_CAT,
	DTV_SUBTITLE_LANG_HRV,
	DTV_SUBTITLE_LANG_CES,
	DTV_SUBTITLE_LANG_CZE,
	DTV_SUBTITLE_LANG_DAN,
	DTV_SUBTITLE_LANG_DUT,
	DTV_SUBTITLE_LANG_NLD,
	DTV_SUBTITLE_LANG_ENG,
	DTV_SUBTITLE_LANG_EST,
	DTV_SUBTITLE_LANG_FIN,
	DTV_SUBTITLE_LANG_FRA,
	DTV_SUBTITLE_LANG_FRE,
	DTV_SUBTITLE_LANG_GLG,
	DTV_SUBTITLE_LANG_DEU,
	DTV_SUBTITLE_LANG_GER,
	DTV_SUBTITLE_LANG_ELL,
	DTV_SUBTITLE_LANG_GRE,
	DTV_SUBTITLE_LANG_HUN,
	DTV_SUBTITLE_LANG_ICE,
	DTV_SUBTITLE_LANG_ISL,
	DTV_SUBTITLE_LANG_GLE,
	DTV_SUBTITLE_LANG_ITA,
	DTV_SUBTITLE_LANG_LAV,
	DTV_SUBTITLE_LANG_LIT,
	DTV_SUBTITLE_LANG_LTZ,
	DTV_SUBTITLE_LANG_NOR,
	DTV_SUBTITLE_LANG_POL,
	DTV_SUBTITLE_LANG_POR,
	DTV_SUBTITLE_LANG_RUS,
	DTV_SUBTITLE_LANG_SLO,
	DTV_SUBTITLE_LANG_SLV,
	DTV_SUBTITLE_LANG_SPA,
	DTV_SUBTITLE_LANG_SWE,
	DTV_SUBTITLE_LANG_UKR,
	DTV_SUBTITLE_LANG_CYM,
	DTV_SUBTITLE_LANG_WEL,
	DTV_SUBTITLE_LANG_NUM
} ENUM_DTV_SUBTITLE_LANG;

#define MAX_VOLUME_NUMBER 31
#define MID_VOLUME_NUMBER 16
#define MIN_VOLUME_NUMBER 0

#if defined(CASABLANCA)
typedef enum {
    SCREEN_SAVER_OFF,
    SCREEN_SAVER_2_MIN,
    SCREEN_SAVER_5_MIN,
    SCREEN_SAVER_10_MIN,
} SCREEN_SAVER_TIMING;

typedef enum {
    SCREEN_SAVER_DEFAULT,
    SCREEN_SAVER_PHOTO_ALBUM,
    SCREEN_SAVER_FLICKR,
} SCREEN_SAVER_OPTION;
#endif

typedef enum {
    KEY_TONE_VOLUME_0,
    KEY_TONE_VOLUME_1,
    KEY_TONE_VOLUME_2,
    KEY_TONE_VOLUME_3,
    KEY_TONE_VOLUME_4,
    KEY_TONE_VOLUME_5,
} KEY_TONE_VOLUME;

typedef struct _misc_setting {
    ENUM_ON_OFF   m_passthrough_mode;
    SourceOption  m_source;
    DeviceOption  m_device;
    int           m_channel;
    int           m_channel_d; // for digital TV
    int           m_dtvChannel;
    int           m_radioChannel;
    int           m_volume;
    SortMode      m_SortingModeHDD;
    SortMode      m_SortingModeDISC;
	ENUM_DTV_AUDIO_LANG m_dtvAudioLang;
	ENUM_DTV_SUBTITLE_LANG m_dtvSubtitleLang1;
	ENUM_DTV_SUBTITLE_LANG m_dtvSubtitleLang2;
	ENUM_ON_OFF   m_dtvSubtitleHardOfHearing;
	ENUM_ON_OFF   m_dtvDisableLCNs;
	DTV_REGION    m_dtvRegion;
	DTV_CHANNEL_ALTER_MODE m_dtvChannelAlterMode;
    ENUM_ON_OFF   m_KaraokeMode;
    int           m_mic_1_volume;
    int           m_mic_2_volume;
    int           m_music_volume;
    int           m_mic_echo_strength;
    VOCAL_REMOVAL_MODE    m_vocal_removal_mode;
    ENUM_KEY_SHIFT_STONE  m_keyshift_stone;
    ENUM_EQUALIZER_MODE   m_equalizer_mode;
    KARAOKE_REVERB_MODE   m_reverb_mode;
#if defined(CASABLANCA)
    SCREEN_SAVER_TIMING 	screen_saver_timing;
#else
    ENUM_ON_OFF   screen_saver;
#endif
    ENUM_ON_OFF   ken_burns;
    ENUM_ON_OFF   movie_preview;
    int           m_subtitleYOffset;
    int           m_subtitleSize;
    ENUM_SUBTITLE_TEXT_COLOR m_subtitleColor;
    int           m_subOffset;
    char          m_divx_code[32];
    bool          m_disable_ext_hdd_db;
    bool          m_disable_auto_scan_ext_hdd;
    char          *m_skinName;
#if defined(CASABLANCA)
    SCREEN_SAVER_OPTION	screen_saver_option;
    int 	folder_partition_id;
    int 	folder_field_id;
    char* 	folder_data_path;
#endif
    ENUM_ON_OFF bt_process;
    ENUM_ON_OFF samba_process;
    ENUM_ON_OFF neighbor_security;
    KEY_TONE_VOLUME key_tone_volume;
} MISC_SETTING;

typedef enum resume_setting {
    RESUME_NONE,
    RESUME_STOP,
    RESUME_POWERON,
} RESUME_SETTING;

typedef struct _bookmark_setting {
    RESUME_SETTING resume_mode;
    unsigned int signatureSize;
    char signature[16];
    unsigned int stateSize;
    char state[4096];
} BOOKMARK_SETTING;

typedef struct _parental_setting {
    char password[4];
    char countryCode[2];
    int level;
} PARENTAL_CONTROL_SETTING;

#define WLAN0_PROFILE_NUM 3
//#define WLAN0_PROFILE_NUM 8
// array = WLAN0_PROFILE_NUM+1
// last is for predefine during setting process
#define MAX_NETSTR_LEN 65
#define MAX_PPPSTR_LEN 256
#define MAX_FREQ_LEN 16
typedef struct _network_setting {
    ENUM_NET_INTERFACE default_route;
    ENUM_NET_MODE netMode;
    ENUM_NET_MODE netUiMode;
#if defined(REALTEK_WFDISPLAY) && !defined(DISABLE_WFDISPLAY_UI) && defined(WIFI_CONCURRENT)
	ENUM_P2P_CONCURRENT_STATE netP2pConcurrentState;
	ENUM_ON_OFF netP2pConSwitch;
#endif
    IPV4 ipvDNS;
    bool enable_dhcp_manual_dns;
    bool enable_eth0;
    bool enable_eth0_dhcp;
    IPV4 ipv_eth0_IP;
    IPV4 ipv_eth0_Mask;
    IPV4 ipv_eth0_Gateway;
    bool enable_wlan0;
    int wlan0_profile;
    int wlan0_profile_pre_setting;
    char wlan0_profile_name[WLAN0_PROFILE_NUM+1][MAX_NETSTR_LEN];
    ENUM_WIRELESS_MODE wlan0_mode[WLAN0_PROFILE_NUM+1];
    bool enable_wlan0_dhcp[WLAN0_PROFILE_NUM+1];
    IPV4 ipv_wlan0_IP[WLAN0_PROFILE_NUM+1];
    IPV4 ipv_wlan0_Mask[WLAN0_PROFILE_NUM+1];
    IPV4 ipv_wlan0_Gateway[WLAN0_PROFILE_NUM+1];
    char wlan0_apname[WLAN0_PROFILE_NUM+1][MAX_NETSTR_LEN];
    ENUM_WIRELESS_SECURITY wlan0_security[WLAN0_PROFILE_NUM+1];
    char wlan0_wep_key[WLAN0_PROFILE_NUM+1][MAX_NETSTR_LEN];
    int wlan0_active_wep_key[WLAN0_PROFILE_NUM+1];
    IPV4 ipv_wlan0_DNS[WLAN0_PROFILE_NUM+1];
    IPV4 ipv_wlan0_dhcpd_hostip[WLAN0_PROFILE_NUM+1];
    IPV4 ipv_wlan0_dhcpd_startip[WLAN0_PROFILE_NUM+1];
    IPV4 ipv_wlan0_dhcpd_endip[WLAN0_PROFILE_NUM+1];
    bool enable_ppp0;
    char ppp0_user_name[MAX_PPPSTR_LEN+1];
    char ppp0_password[MAX_PPPSTR_LEN+1];
    ENUM_NET_INTERFACE ppp0_interface;
#ifdef ENABLE_WIFI_SOFT_AP
    char software_ap_profile_name[MAX_NETSTR_LEN];
    ENUM_SOFT_AP_MODE software_ap_mode;
    char software_ap_apname[MAX_NETSTR_LEN];
    ENUM_WIRELESS_SECURITY software_ap_security;
    char software_ap_wep_key[MAX_NETSTR_LEN];
	bool enable_concurrent_mode;
#ifdef ENABLE_WIFI_BRIDGE_MODE
	EMUN_BRIDGE_INTERFACE software_ap_bridge_target;
#endif //ENABLE_WIFI_BRIDGE_MODE
#endif
	ENUM_WIFI_FREQ wifi_freq;
} NETWORK_SETTING;

#define MAX_NETB_ID_PW_LEN 20
#ifdef VCRPLUS
typedef struct vcrplus_ch_map
{
    int GCN_Ch;
    SourceOption Received_Source;
    int Received_Ch;
} VCRPLUS_CH_MAPPING;

#define CH_MAPPING_MAX  256
#endif


// **************************************************************************
// SETUP Menu :
// **************************************************************************
typedef struct _system_data {
    LANGUAGE_SETTING language_setting;
    VIDEO_SETTING video_setting;
    REC_SETTING rec_setting;
    AUDIO_SETTING audio_setting;
    PLAY_SETTING play_setting;
    SYSTEM_SETTING system_setting;
    NETWORK_SETTING network_setting;
#if	AP_CONFIG_WITH(APCFG_APP_BITTORRENT)
    IMS_SETTING ims_setting;
#endif
#if AP_CONFIG_WITH(APCFG_HW_ATV) || AP_CONFIG_WITH(APCFG_HW_DTV)
    CHANNEL_SETTING channel_setting;
#endif
#if AP_CONFIG_WITH(APCFG_HW_DTV)
    CHANNEL_SETTING_DTV channel_setting_dtv;
#endif
    MISC_SETTING misc_setting;
    BOOKMARK_SETTING bookmark_setting;
    PARENTAL_CONTROL_SETTING parental_control_setting;
#ifdef VCRPLUS
    VCRPLUS_CH_MAPPING vcrplus_channel_mapping_table[CH_MAPPING_MAX];
#endif
} SYSTEM_SETUP;

#endif


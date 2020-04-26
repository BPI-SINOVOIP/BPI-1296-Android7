#ifndef __SETUP_DEFAULT_TABLE_H_
#define __SETUP_DEFAULT_TABLE_H_
#include <Application/AppClass/SetupClass.h>

#define SYSTEM_DEVICE_UUID     "6401ab56-6067-4530-96d6-06603d5c5b90"
#ifdef ENABLE_SMART_DISPLAY_STAND_ALONE
#define SYSTEM_DEVICE_FRIENDLY "AirFun"
#else
#define SYSTEM_DEVICE_FRIENDLY "Device"
#endif
#define DEFAULT_DLNA_DMS_DEVICE_NAME ""
#define DEFAULT_DLNA_DMS_DEVICE_UUID "554739bc-e17e-437b-b9c0-b4892395d141"

const LANGUAGE_SETTING language_default =
    {OSD_LANG_ENGLISH,
#ifdef Libra_MS
    TEXT_ENCODING_WESTERN,
#else
    TEXT_ENCODING_UTF8,
#endif
    AUDIO_LANG_ENGLISH, SUBTITLE_LANG_ENGLISH, MENU_LANG_ENGLISH,MINIKD_LANG_ENGLISHLOWER};

#if AP_CONFIG_WITH(APCFG_HW_ATV) || AP_CONFIG_WITH(APCFG_HW_DTV)
const int tv_default[] = {CABLE, TV_REGION_NONE, SET_OFF};
#endif

const AUDIO_SETTING audio_default =
    {
        TWO_CHANNEL, AUDIO_DIGITAL_LPCM_DUAL_CH, AUDIO_DIGITAL_LPCM_DUAL_CH,
#ifdef USE_HDMI
        HDMI_LPCM,
#else
        SPDIF_LPCM,
#endif
        AUDIO_AGC_DRC_OFF, SET_OFF,
        AUDIO_OUTPUT_2CH,
	 	100,  // drc_cut_value
	 	100,  // drc_boost_value
		SET_OFF
    };

const VIDEO_SETTING video_default =
    {
		VIDEO_NTSC,
		VIDEO_INTERLACED,
		VIDEO_NTSC, 		//ENUM_VIDEO_SYSTEM video_system_manual
		VIDEO_INTERLACED,	//ENUM_VIDEO_STANDARD video_standard_manual
		VIDEO_NTSC,
        TV_SYSTEM_AUTO_MODE_HDMI_EDID, // update based on bug#25872, default is HDMI_EDID
		Wide_16_9,
		FIT_MODE,
		VOUT_YUV,
		VIDEO_REC_AUTO,
		SET_OFF,
		SET_OFF, /*DNR*/
		SET_OFF,
		SET_OFF,
	#ifdef COPYFREE
		SET_OFF,
	#else
		SET_ON,
	#endif
		(MAX_BRIGHTNESS_VALUE/2),
		(MAX_CONTRAST_VALUE/2),
		(MAX_HUE_VALUE/2),
		(MAX_SATURATION_VALUE/2),
	#ifdef USE_HDMI
		0,					//int  CurrentHdmiTVVid
	#endif /* USE_HDMI */
		VIDEO_HD_NONE,
		SET_OFF,
		0,
	#ifdef USE_HDMI
		0,    //int hdmi_deep_color
	#endif
	#ifdef TVE_DEC_SWITCH
		0,
	#endif
	#ifdef ENABLE_HDMV_PLAYBACK
	#ifdef ENABLE_OTT  // if ENABLE_OTT, BD menu default OFF
		SET_OFF,
	#else
		SET_ON,
	#endif
	#endif
	#ifdef HW_VGA
		VIDEO_VGA_RESOLUTION_AUTO, //20120606
	#endif
	};

const REC_SETTING rec_default =
    {RECORD_SP, TWENTY_MINUTES,
#if defined(ENABLE_PERMANENT_TIMESHIFT) && !defined(DTV_TUNER_2)
#if defined(PERMANENT_TIMESHIFT_DEFAULT_ON)
    SET_ON,
#else
    SET_OFF,
#endif
#else
    SET_OFF,
#endif
	REC_TO_HDD, REC_TO_HDD, MANUAL_RECORDING, ""};

const PLAY_SETTING play_default =
    {SLIDE_SHOW_OFF, TRANS_EFFECT_OFF, MIX_PLAY_FOLDER};

const SYSTEM_SETTING system_default =
    {
#if defined(DVB_S) //#if defined(DVB_S),  disable time zone auto item. default set to : TIME_ZONE_GMT
    TIME_ZONE_GMT,
#else
	TIME_ZONE_AUTO,
#endif
    SET_OFF, SET_ON, SET_ON, SET_ON, REGION_CODE_ZERO, SECURITY_ANONYMOUS, SET_OFF, 
#ifdef ENABLE_SMART_DISPLAY_STAND_ALONE   
	false, // initial flag
#else
    true,  // initial flag
#endif   
#if defined(DVB_T)
      {SET_OFF, SET_OFF, RATING_DEFAULT, 0x00, 5000,RATING_OFF, TV_RATING_OFF, TV_RATING_OFF, {0x1f,0x0f,0x1f,0x10,0x10,0x11,0x00,0x00,0x00}}, //Rating
#else
      {SET_OFF, SET_OFF, RATING_USA, 0x00, 5000, RATING_OFF, 1, 1, {0x1f,0x0f,0x1f,0x10,0x10,0x11,0x00,0x00,0x00}}, //Rating
#endif
	SET_OFF, //photoActualSize #23857
#if AP_CONFIG_WITH(APCFG_APP_UPNPDLNA1p5)
    SET_ON,//dlna dmr setting
#if defined(ENABLE_OTT)
    SET_OFF,//dlna dms setting
#else
	SET_ON,
#endif
    DEFAULT_DLNA_DMS_DEVICE_NAME,
    DEFAULT_DLNA_DMS_DEVICE_UUID,
#endif /* APCFG_APP_UPNP_1p5 */
    SYSTEM_DEVICE_UUID,
#if AP_CONFIG_WITH(APCFG_APP_UPNPDLNA1p5)
    SYSTEM_DEVICE_FRIENDLY,
#endif /* APCFG_APP_UPNP_1p5 */
#if AP_CONFIG_WITH(APCFG_HW_NET_WLAN0)
	WIFI_FREQ_24G, //wifiFreq;
#endif	//AP_CONFIG_WITH(APCFG_HW_NET_WLAN0)
#ifdef ENABLE_WIFI_SOFT_AP
	true, 					// bConcurrentMode;
	MODE_SOFT_AP_ONLY, 		// softwareApMode
	NET_REGION_USA, 		// softwareApNetRegion;
	WL_SECURITY_OPEN, 		// softwareApSecurity
#ifdef ENABLE_WIFI_BRIDGE_MODE
	SETUPCLASS_BRG_DISABLE, 	// wifiBridgeTarget		
#endif //DENABLE_WIFI_BRIDGE_MODE
#endif //ENABLE_WIFI_SOFT_AP
#if defined(REALTEK_WFDISPLAY) && !defined(DISABLE_WFDISPLAY_UI) && defined(WIFI_CONCURRENT)
	STATE_P2P_CONCURRENT, 	// p2pConcurrentState,  (STATE_P2P_CONCURRENT = 0,	STATE_P2P_BUSY, STATE_NET_BUSY)
	// 2013/03/20 add: 
	// for p2pConcurrentSwitch :
	// If SET_ON -> p2p Concurrent auto switch for P2P/DLNA,
	//      It don't need to reference to GetP2pConcurrentState(), always STATE_P2P_CONCURRENT
	// If SET_OFF -> p2p Concurrent only in STATE_P2P_CONCURRENT,
	//      But if any connect and playback start, need to stop other side of connection
	//      to keep best performance of playback side.	
	SET_OFF,	 // p2pConcurrentSwitch, // default should be set OFF
#endif
#ifdef USE_CEC
    true,true,true,false,true, //fix bug#26199 : set bEnableHdmiCecAutoOsdLanguage default as OFF
	0,
	false,
#endif
#ifdef USE_HDMI
    LIP_SYNC_AUTO, 250
#endif
    };

const int bookmark_default[] = {RESUME_STOP};

const MISC_SETTING misc_default =
    {
#if AP_CONFIG_WITH(APCFG_APP_RECORDABLE)
    SET_ON,
#else
    SET_OFF,
#endif
    SOURCE_DTV, DEVICE_HDD, 2, 1, 0, 0,
#if AP_CONFIG_WITH(APCFG_PRJ_AVHDD) || AP_CONFIG_WITH(APCFG_PRJ_PLAYER)
    MID_VOLUME_NUMBER,
#else
    MAX_VOLUME_NUMBER,
#endif
    SORT_BY_TIME, SORT_BY_TIME,
    DTV_AUDIO_LANG_ENG, /* dtv preferred audio lang default is ENG */
    DTV_SUBTITLE_LANG_ENG, /* dtv preferred subtitle lang 1 default is ENG */
    DTV_SUBTITLE_LANG_ENG, /* dtv preferred subtitle lang 2 default is ENG */
    SET_OFF, /* dtv subtitle Hard Of Hearing */
    SET_OFF, /* dtv Disable LCNs */
    DTV_REGION_TAIWAN,
    DTV_CHANNEL_AUTO_UPDATE, /* dtv channel alter mode */
    SET_OFF,
    6,
    6,
    25,
    0,
    ENUM_VOCAL_REMOVAL_NONE,
    ENUM_KEY_SHIFT_STONE_0,
    ENUM_EQUALIZER_RESERVED,
    REVERB_OFF,
#if defined(CASABLANCA)
    SCREEN_SAVER_5_MIN, /* Screen Saver Timing */
#else
    SET_ON, /* Screen Saver */
#endif
    SET_ON, /* Ken Burns*/
    SET_ON, /* Movie preview in GBrowserMenu */
    0,
    0,
    SUBTITLE_TEXT_WHITE,
    0,
    {0},
    0,
    0,
    (char *)"",	// skin name
#if defined(CASABLANCA)
    SCREEN_SAVER_DEFAULT,
    0,
    0,
    (char *)"",
#endif
    SET_OFF, //bt
    SET_OFF, //samba
    SET_OFF,
    KEY_TONE_VOLUME_1,
	};

#endif //__SETUP_DEFAULT_TABLE_H_


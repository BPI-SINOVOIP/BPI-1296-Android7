#ifndef _USER_CMD_H_
#define _USER_CMD_H_

#ifdef WIN32
#define __attribute__(x)
#endif

#include <sys/types.h>
#ifdef __cplusplus
class TimerHandler;
#else
typedef void TimerHandler;
#endif

/* Static Timer ID Range : 0x00000001 - 0x7fffffff */
#define DYNAMIC_TIMER_ID                        0xFFFFFFFF
#define DYNAMIC_TIMER_ID_BASE                   0xF0000000

typedef enum {
  USER_DEFINE_NETWORK_UPGRADE,
  USER_DEFINE_CMD_STRING,
  USER_DEFINE_ENTER_SCREEN_SAVER,
  USER_DEFINE_LEAVE_SCREEN_SAVER,
  USER_DEFINE_DFB_INPUT_NOTIFY,
  USER_DEFINE_WEBKIT_URL,
  USER_DEFINE_WEBKIT_TO_HOME_PAGE,
  USER_DEFINE_APP_SUSPEND,
  USER_DEFINE_APP_RESUME,
  USER_DEFINE_QUERY_STATUS,
  USER_DEFINE_REDRAW,
  USER_DEFINE_SWF_AUDIOPLAYBACK_END,
  USER_DEFINE_SWF_AUDIOPLAYBACK_RETURN,
  USER_DEFINE_SWF_AUDIOPLAYBACK_INVALID_FILE,
  USER_DEFINE_TOOLBAR_CONTROL,
  USER_DEFINE_WEBKIT_SET_OPTIONS,
  USER_DEFINE_WEBKIT_URL_NONBLOCK,
  USER_DEFINE_SET_WINDOW_RECT,
  USER_DEFINE_WEBKIT_SHOW_HIDE_WINDOWS,
  USER_DEFINE_MEDIA_FILE_UPLOAD,
  USER_DEFINE_START_SLIDE_SHOW,
  USER_DEFINE_MEDIA_FB_LOGIN_END,
  USER_DEFINE_WEBKIT_SET_ZOOM_FACTOR,
  USER_DEFINE_DIRECTCONNECT_IO_ERROR,
} USER_DEFINE_SUB_CMD;

typedef enum {
    /*  Command Type I: User Input */
    CMD_POWER          = 1,
    CMD_DISPLAY        = 2,
    CMD_SETUP          = 3,
    CMD_EJECT          = 4,
    CMD_SOURCE         = 5,
    CMD_PSCAN          = 6,
    CMD_ASPECT_RATIO   = 7,
    CMD_GCODE          = 8,
    CMD_SCHEDULE       = 9,
    CMD_DISC_UTILITY   = 10,
    CMD_HDD            = 11,
    CMD_DVD            = 12,
    CMD_USB            = 13,
    CMD_CARD_READER    = 14,
    CMD_DVCONTROL      = 15,
    CMD_FILE_COPY      = 16,
    CMD_MENU           = 17,
    CMD_GUIDE          = 18,
    CMD_UP             = 19,
    CMD_DOWN           = 20,
    CMD_LEFT           = 21,
    CMD_RIGHT          = 22,
    CMD_SELECT         = 23, CMD_ENTER = 23, // for backward-compatibility
    CMD_RETURN         = 24,
    CMD_DELETE         = 25,
    CMD_INSTANT_REPLAY = 26,
    CMD_CMSKIP         = 27,
    CMD_ZERO           = 28,
    CMD_ONE            = 29,
    CMD_TWO            = 30,
    CMD_THREE          = 31,
    CMD_FOUR           = 32,
    CMD_FIVE           = 33,
    CMD_SIX            = 34,
    CMD_SEVEN          = 35,
    CMD_EIGHT          = 36,
    CMD_NINE           = 37,
    CMD_RECORD_PLAY    = 38,   // Record Play == Record Start
    CMD_PLAY           = 39,
    CMD_PAUSE          = 40,
    CMD_STOP           = 41,
    CMD_NEXT           = 42,
    CMD_PREV           = 43,
    CMD_FFWD           = 44,
    CMD_FRWD           = 45,
    CMD_SFWD           = 46,
    CMD_SRWD           = 47,
    CMD_SEARCH         = 48,
    CMD_REPEAT         = 49,
    CMD_AB_REPEAT      = 50,
    CMD_STITLE         = 51,
    CMD_AUDIO          = 52,
    CMD_ANGLE          = 53,
    CMD_ZOOM           = 54,
    CMD_VOL_UP         = 55,
    CMD_VOL_DOWN       = 56,
    CMD_MUTE           = 57,
    CMD_ZOOM_IN        = 58,
    CMD_ZOOM_OUT       = 59,
    CMD_QUALITY        = 60,
    CMD_EPG            = 61,
    CMD_EDIT           = 62,

    // Extra
    CMD_BOOKMARK_SET          = 63,
    CMD_BOOKMARK_RESTORE      = 64,
    CMD_NEXT_FILE             = 65,
    CMD_PREV_FILE             = 66,
    CMD_ONETOUCH_COPY         = 67,
    CMD_STOP_PLAYBACK         = 68,

    // used by script string command
    CMD_STR_CMD               = 69,
    CMD_START_BROWSER_DIR_CMD = 70,
    CMD_START_VIDEO_PLAYBACK  = 71,
    CMD_START_PHOTO_PLAYBACK  = 72,
    CMD_START_AUDIO_PLAYBACK  = 73,
    CMD_START_SWF_PLAYBACK    = 74,

    // used by ATSC channel number.
    CMD_POINT                 = 75,
    CMD_TELETEXT              = 76,
    CMD_TELETEXT_SUBTITLE     = 77,
    CMD_SUBTITLE_RESUME       = 78,
    CMD_STILE_TYPE            = 79,
    CMD_DISPLAY_VOUT          = 80,

    CMD_PAGE_UP               = 81,
    CMD_PAGE_DOWN             = 82,

    // for adjusting subtitle delay
    CMD_SUBTITLE_DELAY_PLUS   = 83,
    CMD_SUBTITLE_DELAY_MINUS  = 84,
    CMD_SUBTITLE_CODE_PAGE    = 85,

    // for DLNA DMR
    CMD_DLNADMR_ACTIVATE      = 86,
    CMD_DLNADMR_WARNING       = 87,

    // Following Items are stale and may not use in the future
    CMD_TITLE                 = 88,
    CMD_TOOLS                 = 89,

    // for Network
    CMD_NET_GET_COMMAND       = 90,
    CMD_MS_AP_COMM            = 91,
    CMD_MEDIA_SERVER          = 92,
    CMD_MEDIA_CLIENT          = 93,
    CMD_NET_DISCONNECTED      = 94,
    CMD_NET_CONNECTED         = 95,
    CMD_NET_WIFI_UNPLUG       = 96,
    CMD_NET_WPS_BTN           = 97,

    CMD_FORCE_POWER           = 98,
    CMD_SAFE_EJECT            = 99,

    // External Force Control Code [via UART]
    CMD_SET_QUALITY           = 100,
    CMD_SET_LANGUAGE          = 101,
    CMD_FORCE_BROWSER         = 102,
    CMD_FORCE_PASSTHROUGH     = 103,
    CMD_FORCE_RECORD          = 104,
    CMD_FORCE_TIMESHIFT       = 105,
    CMD_FORCE_PLAY_LAST_TITLE = 106,
    CMD_SET_LIVEPAUSE_DEV     = 107,
    CMD_SHOW_REVISION         = 108,
    CMD_CONFIG_VIN_MODE       = 109,

    CMD_NETFLIX               = 110,
    CMD_VUDU                  = 111,

    CMD_YOUTUBE               = 112,
    CMD_VTUNER                = 113,

    CMD_NETFLIX_DIAL	      = 114,	// RTEC-251
	CMD_WEBKIT_DUMP_HEAP      = 115, //+JT
	CMD_DIAL_SWITCH_HOME	  = 118,
	CMD_DIAL_SWITCH           = 119,

    //Multi-purpose four color keys
    CMD_OPTION_RED            = 120,
    CMD_OPTION_GREEN          = 121,
    CMD_OPTION_YELLOW         = 122,
    CMD_OPTION_BLUE           = 123,
    CMD_OPTION                = 124,
    CMD_RELOAD_TVSYSTEM       = 125,
    CMD_BROWSER_REFRESH       = 126,

	CMD_START_PLAYBACK		  = 127,

    /* Command Type II: Null Operation */
    CMD_NOP                        = -1,

    /* Command Type III: Tokens for Internal Event */
	// for DLNA DMR
    CMD_RESOURCE_TIMING_SWITCH     = -2,
    CMD_PRINT_RESOURCE_TIMING      = -3,
    CMD_VOLUME_NUM                 = -4,
    CMD_DLNADMR_RESTART            = -5,
    CMD_DLNADMR_HIDELOADING        = -6,
    CMD_DLNADMR_SHOWLOADING        = -7,
	CMD_DLNA_DMRDMS_START 			= -8,
	CMD_DLNA_DMRDMS_STOP 			= -9,
	CMD_DLNA_DMRDMS_DELAY_START		= -10,
	CMD_NET_RESTART_SOFTAP			= -11,
	CMD_DLNADMR_INTERNAL 			= -12,
                                   
    CMD_IMS3D_RELOAD_MODEL         = -13,
    CMD_IMS3D_RELOAD_TEXTURE       = -14,
    CMD_IMS3D_RELOAD_RSSSCRIPT     = -15,
    CMD_IMS3D_RELOAD_ALL           = -16,
    CMD_IMS3D_PRINTSCREEN          = -18,
    
    CMD_TIMER                      = -20,
    // for qttestbrowser standalone process
    CMD_BROWSER_EXIT               = -21,
	CMD_BROWSER_ENABLE_KEYNAV      = -22,
	CMD_BROWSER_DISABLE_KEYNAV     = -23,
	CMD_BROWSER_DELETEALLHISTORY   = -24,
	CMD_BROWSER_GIVEUPFOCUS        = -25,
	CMD_BROWSER_INVALIDATERECT     = -26,
	CMD_BROWSER_INITIALIZED		   = -27, //+JT
    CMD_BROWSER_READY			   = -28, //+JT
    CMD_BROWSER_TERMINATED         = -29,
	CMD_BROWSER_STOPDMR            = -31,
	CMD_BROWSER_RESUMEDMR          = -32,
	CMD_BROWSER_WARM_INIT          = -34,
	CMD_BROWSER_WARM_UNINIT        = -35,
	CMD_BROWSER_CLEAR_COOKIES      = -36,
	
    CMD_SCHEDULE_UPDATE            = -30,  // via Web Interface
    CMD_VKBD_STRING                = -33,  //for QtTestBrowser get string from virtualkeyboard
    CMD_DISC_INSERTED              = -40,  // Disc Insertion Event
	CMD_NET_APPLY_P2P_CONCURRENT_STATE	= -41, 
	CMD_NET_RESTART_P2P_WIFI_DIRECT   	= -42, 
	CMD_NET_START_P2P_WIFI_DIRECT   	= -43, 

    CMD_STORAGE_UPDATED            = -50,  // Storage has been updated
    CMD_USB_UPDATED_TIMESHIFTABLE  = -51,  // USB has been updated and timeshiftable.
    CMD_HID_UPDATED                = -52,
    CMD_USB_MOUSE                  = -55,
    CMD_USB_KEYBD                  = -56,
    CMD_TERMINATE                  = -60,  // terminate file record AP
    CMD_ONPAINT                    = -61,   //added by sunling: indicate need to repaint UI.
    // We occupy -62 to -69. reserved.
    CMD_CUSTOM_END                 = -62, // @Fixme cmd_custom is internal custom message, user should use CMD_CUSTOM + messageID, from 0 up to 7.
    CMD_CUSTOM                     = -69,
    CMD_COMPLETED                  = -70,  // indicating playback completion
    CMD_TIMESHIFT                  = -80,  // for sync timeshift restart
    CMD_REQUEST_PML                = -90,  // request parental management level (PML)

    // for communication with Android
    CMD_ANDROID_SWITCH             = -91,
    CMD_ANDROID_READY              = -92,

    CMD_KEY_UP                     = -99,	// press key release
    CMD_USB_PLUGGED_IN             = -100, // USB Plugged-In Event
    CMD_USB_DVD_EVENT              = -101,  // USB DVD Plug-In or UnPlug
    CMD_FLOW_EVENT                 = -102,

    /* Command Type IV: Tokens helping QA/Debugging */
    CMD_TIMELOGGING                = -103,
    CMD_AUDIO_NEXT                 = -104,
    CMD_FLOW_ERR_EVENT             = -105,
    CMD_USER_DEFINE                = -106,
    CMD_DRMRENTAL_EXPIRED          = -107,
    CMD_DRMRENTAL_REQUEST          = -108,

    //add for gtalk
    CMD_IM_STATUS                  = -109,
    CMD_IM_FILE_STATUS             = -110,
    CMD_IM_MSG_TXT                 = -111,
    CMD_PLAYATTIME                 = -112,
    // for skype events
    CMD_SKYPE_EVENT                = -113,
    CMD_DB_SCAN_START              = -116,
    CMD_DB_SCAN_END                = -117,
    CMD_NET_WIFI_PLUG              = -118,

    // for bug 37599
    CMD_EDID_CHECKSUM_UPDATE = -120,

    // used by HDMI HDCP ON/OFF number.
    CMD_HDCP_ON                    = -122,
    CMD_HDCP_OFF                   = -123,
    // used by HDMI HOT PLUG DETECT.
    CMD_HDMI_HPD                   = -124,
    CMD_HDMI_READY                 = -125,
    CMD_HDMI_YST_MODE              = -126,
    CMD_DECODE_FINISH              = -127,
    CMD_DVDMENU                    = -128,

} USER_CMD;

typedef struct _command_buffer {
    char        length;
    char        type;
    union
    {
#if !defined(WIN32)
		int32_t          idApp    __attribute__((packed));
#else
		int 			idApp    __attribute__((packed));
#endif
        TimerHandler    *pHandler __attribute__((packed));
    };
 #if !defined(WIN32)
	int64_t 	timestamp __attribute__((packed));	// in us(10^-3 seconds)
#else
	long long timestamp __attribute__((packed));	// in us(10^-3 seconds)
#endif

    char        repeat; // 1 for repeated token, 0 for independent token
    char        parm[113];
} COMMAND_BUFFER;

//------------------------------------------------------------------------------
// Added by Zack. operator== is needed for rtk::tools::SafeQueue
//
inline bool operator==(const COMMAND_BUFFER& Cmd1, const COMMAND_BUFFER& Cmd2)
{
    return Cmd1.type == Cmd2.type;
}

inline bool operator!=(const COMMAND_BUFFER& Cmd1, const COMMAND_BUFFER& Cmd2)
{
    return !(Cmd1 == Cmd2);
}

//------------------------------------------------------------------------------

#endif


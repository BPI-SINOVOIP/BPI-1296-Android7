#ifndef _RTK_FW_DBG_INFO_RPC_H_
#define _RTK_FW_DBG_INFO_RPC_H_

#define S_OK        0x10000000


#define FW_INFO_ERROR_PRINT(format, ...) printk(KERN_ERR "[FW_INFO_ERROR] " format "\n", ## __VA_ARGS__)
#define FW_INFO_PRINT(format, ...) printk(KERN_WARNING "[FW_INFO] " format "\n", ## __VA_ARGS__)



typedef u_int HRESULT;

typedef struct RPCRES_LONG {
	HRESULT result;
	u_int data;
}RPCRES_LONG;


typedef struct { 
	u_int  info;
	RPCRES_LONG retval;
	u_int ret;
} RPC_DEFAULT_INPUT_T;



//From FW amw_kernel_rpc.h
typedef enum
{
    ENUM_KERNEL_RPC_CREATE_AGENT,
    ENUM_KERNEL_RPC_INIT_RINGBUF,
    ENUM_KERNEL_RPC_PRIVATEINFO,
    ENUM_KERNEL_RPC_RUN,
    ENUM_KERNEL_RPC_PAUSE,
    ENUM_KERNEL_RPC_SWITCH_FOCUS,
    ENUM_KERNEL_RPC_MALLOC_ADDR,
    ENUM_KERNEL_RPC_VOLUME_CONTROL,
    ENUM_KERNEL_RPC_FLUSH,
    ENUM_KERNEL_RPC_CONNECT,
    ENUM_KERNEL_RPC_SETREFCLOCK,
    ENUM_KERNEL_RPC_DAC_I2S_CONFIG,
    ENUM_KERNEL_RPC_DAC_SPDIF_CONFIG,
    ENUM_KERNEL_RPC_HDMI_OUT_EDID,
    ENUM_KERNEL_RPC_HDMI_OUT_EDID2,
    ENUM_KERNEL_RPC_HDMI_SET,
    ENUM_KERNEL_RPC_HDMI_MUTE,
    ENUM_KERNEL_RPC_ASK_DBG_MEM_ADDR,
    ENUM_KERNEL_RPC_DESTROY,
    ENUM_KERNEL_RPC_STOP,
    ENUM_KERNEL_RPC_CHECK_READY,   // check if Audio get memory pool from AP
    ENUM_KERNEL_RPC_GET_MUTE_N_VOLUME,   // get mute and volume level
    ENUM_KERNEL_RPC_EOS,
    ENUM_KERNEL_RPC_ADC0_CONFIG,
    ENUM_KERNEL_RPC_ADC1_CONFIG,
    ENUM_KERNEL_RPC_ADC2_CONFIG,
#if defined(AUDIO_TV_PLATFORM)
???
    ENUM_KERNEL_RPC_BBADC_CONFIG,
    ENUM_KERNEL_RPC_I2SI_CONFIG,
    ENUM_KERNEL_RPC_SPDIFI_CONFIG,
#endif // AUDIO_TV_PLATFORM
    ENUM_KERNEL_RPC_HDMI_OUT_VSDB,
    ENUM_VIDEO_KERNEL_RPC_CONFIG_TV_SYSTEM,
    ENUM_VIDEO_KERNEL_RPC_CONFIG_HDMI_INFO_FRAME,
    ENUM_VIDEO_KERNEL_RPC_QUERY_DISPLAY_WIN,
    ENUM_VIDEO_KERNEL_RPC_PP_INIT_PIN,
    ENUM_KERNEL_RPC_INIT_RINGBUF_AO,
    ENUM_VIDEO_KERNEL_RPC_VOUT_EDID_DATA,
    ENUM_KERNEL_RPC_AUDIO_POWER_SET,
    ENUM_VIDEO_KERNEL_RPC_VOUT_VDAC_SET,
    ENUM_VIDEO_KERNEL_RPC_QUERY_CONFIG_TV_SYSTEM,
    ENUM_KERNEL_RPC_AUDIO_CONFIG,
    ENUM_KERNEL_RPC_AIO_PRIVATEINFO,
    ENUM_KERNEL_RPC_QUERY_FW_DEBUG_INFO
} ENUM_AUDIO_KERNEL_RPC_CMD_FW_DBG;



struct VIDEO_RPC_VOUT_EDID_DATA_FW_DBG {
	/* HDR metadata */
	u_char et;
	u_char sm;
	u_char max_luminace;
	u_char max_frame_avg;
	u_char min_luminace;
	/* Color characteristics */
	u_char red_green_lo;
	u_char black_white_lo;
	u_char red_x;
	u_char red_y;
	u_char green_x;
	u_char green_y;
	u_char blue_x;
	u_char blue_y;
	u_char white_x;
	u_char white_y;
	/* Colorimetry Data */
	u_char color_space;
	/* Reserved */
	u_char reserved[32];
 };
typedef struct VIDEO_RPC_VOUT_EDID_DATA_FW_DBG VIDEO_RPC_VOUT_EDID_DATA_FW_DBG;


struct RPC_FW_QUERY_RETURN {
	char query_return[200];
	u_int reserved1;
	u_int reserved2;
	u_int reserved3;
	u_int reserved4;
};
typedef struct RPC_FW_QUERY_RETURN RPC_FW_QUERY_RETURN;


void rtk_fw_dbg_info_register_ion_client(const char *name);
void rtk_fw_dbg_info_deregister_ion_client(const char *name);



int FW_DBG_RPC_ToAgent_Vout_EDIDdata(struct VIDEO_RPC_VOUT_EDID_DATA_FW_DBG *arg);
int FW_DBG_KERNEL_RPC_QUERY_FW_DEBUG_INFO(int *query_what, RPC_FW_QUERY_RETURN *query_ret);


enum ENUM_FW_DBG_QUERY_TYPE {
	ENUM_QUERY_VO_TV_SYSTEM_CONFIG = 0,
	ENUM_QUERY_VO_PLANE = 1,
	ENUM_QUERY_LOW_DELAY = 2,
	ENUM_QUERY_OSD_SETTING = 3,
	ENUM_QUERY_SUB_SETTING = 4,
	ENUM_QUERY_DROP_REPEAT = 5,
	ENUM_QUERY_VIDEO_INFO = 6,
	ENUM_QUERY_HDR_STATUS = 7,
};
typedef enum ENUM_FW_DBG_QUERY_TYPE ENUM_FW_DBG_QUERY_TYPE;

#endif//_HDMITX_RPC_H_


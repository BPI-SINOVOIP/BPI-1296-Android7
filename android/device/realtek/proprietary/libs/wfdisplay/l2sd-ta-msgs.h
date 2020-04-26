#ifndef __L2SD_TA_MSGS_H__
#define __L2SD_TA_MSGS_H__

//TODO: Can this include and file be removed?
//#include "wfd-util.h"

#ifndef ETH_ALEN
    #define ETH_ALEN 6
#endif

#define ETH_P_WIDI_NOTIF	0x888F

#define L2SDTA_MAX_SSID_LEN			32
#define L2SDTA_DEVICE_NAME_LEN		32

// Currently 20 bytes are needed for Query and 24 bytes are needed for Trigger
#define L2SDTA_QUERY_TRIGGER_VE_LEN	24
#define L2SDTA_PRIMARY_DEV_LEN		8
#define L2SDTA_SERVICE_VE_LEN		23

#define L2SDTA_MSG_TYPE_QUERY_OR_TRIGGER		1
#define L2SDTA_MSG_TYPE_SERVICE					2
#define L2SDTA_MSG_TYPE_ASSOC_STATUS			3
#define L2SDTA_MSG_TYPE_P2P_PROBE				4
#define WIDI_MSG_TYPE_P2P_WFD_IE                8

#define L2SDTA_DISASSOCIATED					0
#define L2SDTA_ASSOCIATED						1
#define L2SDTA_WPS_STATUS_FAIL                  2
#define L2SDTA_WPS_STATUS_SUCCESS               3
#define L2SDTA_DEAUTHENTICATED					4
#define P2P_PROVISION_REQ_RECV              	5
#define P2P_GO_REQ_RECIVED              		6
#define P2P_GO_CONFIRM_RECEIVED	                7
#define P2P_ASSOCIATED              			8
#define P2P_WPS_STATUS_FAIL             		9
#define P2P_WPS_STATUS_SUCESS               	10
#define P2P_DEAUTHENTICATED             		11
#define P2P_INVITE_REQ_RECEIVED                 13
#define P2P_INVITE_RSP_SENT                     14
#define P2P_WPS_SUCCESS                         15


#define INTEL_SMI_CODE  "\x00\x01\x57"
#define INTEL_OUI_CODE  "\x00\x17\x35"
// Intel SMI (0x00:0x01:0x57)
#define IS_INTEL_SMI(x)         (x[0] == 0x00 && x[1] == 0x01 && x[2] == 0x57)
// Intel OUI (0x00:0x17:0x35)
#define IS_INTEL_OUI(x)         (x[0] == 0x00 && x[1] == 0x17 && x[2] == 0x35)

#define	WPS_VENDOR_EXT_CODE		0x1049

struct l2sd_p2p_probe_wps_ie_msg_t {
    uint8_t src_mac[ETH_ALEN];
    uint8_t channel;
    uint8_t ssid_len;
    uint8_t ssid[L2SDTA_MAX_SSID_LEN];
    uint8_t category_id;
} __attribute__ ((packed));

struct l2sd_p2p_wfd_ie_msg_t {
	uint8_t type;
	uint8_t frame_type; // 1: probe req, 2: GO Nego Req, 3: GO Nego Confirm, 4: Provision Dicovery Req, 5: Invite Req
	uint8_t src_mac[ETH_ALEN];
	uint8_t dataLen;
	uint8_t data[0];
} __attribute__ ((packed));

struct l2sd_query_or_trig_msg_t {
    uint8_t src_mac[ETH_ALEN];
    uint8_t channel;
    uint8_t ssid_len;
    uint8_t ssid[L2SDTA_MAX_SSID_LEN];
    uint8_t qa_ta_ext[L2SDTA_QUERY_TRIGGER_VE_LEN];
} __attribute__ ((packed));

struct l2sd_service_msg_t {
    uint8_t dst_mac[ETH_ALEN];
    uint8_t channel;
    char sa_device_name[L2SDTA_DEVICE_NAME_LEN];
    uint8_t sa_primary_dev[L2SDTA_PRIMARY_DEV_LEN];
    uint8_t sa_ext[L2SDTA_SERVICE_VE_LEN];
} __attribute__ ((packed));

struct l2sd_assoc_msg_t {
    uint8_t peer_mac[ETH_ALEN];
    uint8_t assoc_stat;
    uint8_t ssid_len;
    uint8_t ssid[L2SDTA_MAX_SSID_LEN];
} __attribute__ ((packed));

struct l2sd_wps_status_msg_t {
    uint8_t peer_mac[ETH_ALEN];
    uint8_t wps_status;
} __attribute__ ((packed));

struct _l2_msg_t {
    uint8_t msg_type;
    union {
        struct l2sd_p2p_probe_wps_ie_msg_t l2sd_p2p_probe_wps_ie_msg;
        struct l2sd_p2p_wfd_ie_msg_t l2sd_p2p_wfd_ie_msg;
        struct l2sd_query_or_trig_msg_t l2sd_query_or_trig_msg;
        struct l2sd_service_msg_t l2sd_service_msg;
        struct l2sd_assoc_msg_t l2sd_assoc_msg;
        struct l2sd_wps_status_msg_t l2sd_wps_status_msg;
    } __attribute__ ((packed)) u;
} __attribute__ ((packed));

typedef struct _l2_msg_t l2_msg_t;



/* This is intel implementaion for WFD IE --- Start*/
#define WFD_IE_DEVICE_INFO_SUB 0x00
#define WFD_IE_ASSOC_BSSID_SUB 0x01
#define WFD_IE_COUPLED_SINK_SUB 0x06

#define WFD_IE_NOT_AVAILABLE_FOR_WFD_SESSION 257
#define WFD_IE_AVAILABLE_FOR_WFD_SESSION 273

/* WFD IE Status */
struct wfd_ie_subelement_status {
    /* Stadard 1.10 does not say anything about this */
} __attribute__ ((packed));

/* WFD IE Device Information Subelement*/
struct wfd_ie_subelement_device_info {
    uint8_t sub_element_id;
    uint16_t length;
    uint16_t wfd_device_information;
    uint16_t session_control_port;
    uint16_t device_max_tpt;
} __attribute__ ((packed));

/* WFD IE Associated BSSID Subelement*/
struct wfd_ie_subelement_associated_bss {
    uint8_t sub_element_id;
    uint16_t length;
    uint8_t assoc_bssid[ETH_ALEN];
} __attribute__ ((packed));

/* WFD IE Coupled Sink Status Bitmap */
struct wfd_ie_coupled_sink_status {
    uint8_t sub_element_id;
    uint16_t length;
    uint8_t connected_status_bitmap;
} __attribute__ ((packed));

/* WFD IE Video Formats Subelement*/
struct wfd_ie_subelement_video_format {
    uint8_t sub_element_id;
    uint16_t length;
    uint32_t cea_resolution;
    uint32_t vesa_resolution;
    uint32_t hh_resolution;
    uint32_t native_resolution;
    uint8_t profiles_bitmap;
    uint8_t levels_bitmap;
    uint8_t latency_field;
    uint16_t min_slice_field;
    uint16_t slice_encoding_param_bitmap;
    uint8_t video_frame_control_support_bitmap;
} __attribute__ ((packed));

/* WFD IE 3D Video Formats Subelement */
struct wfd_ie_subelement_3d_video_format {
    uint8_t sub_element_id;
    uint8_t length;
    uint32_t video_3d_capbility;
    uint8_t native_resolution;
    uint8_t profiles_bitmap;
    uint8_t levels_bitmap;
    uint8_t latency_field;
    uint16_t min_slice_field;
    uint16_t slice_encoding_param_bitmap;
    uint8_t video_frame_control_support_bitmap;
} __attribute__ ((packed));

/* WFD IE audio Formats Subelement */
struct wfd_ie_subelement_audio_format {
    uint8_t sub_element_id;
    uint8_t length;
    uint32_t lpcm_modes_bitmap;
    uint8_t lpcm_decoder_latency;
    uint32_t aac_modes_bitmap;
    uint8_t aac_decoder_latency;
    uint32_t ac3_modes_bitmap;
    uint8_t ac3_decoder_latency;
} __attribute__ ((packed));

/* WFD IE Content Protection Subelement */
struct wfd_ie_subelement_content_protection_format {
    uint8_t sub_element_id;
    uint8_t length;
    uint32_t content_protection_bitmap;
} __attribute__ ((packed));

/* WFD Session Information Subelement */

/* WFD IE*/
struct _l2sd_p2p_wfd_ie {
    uint8_t element_id;
    uint8_t length;
    uint8_t oui_and_oui_type[4];
    struct wfd_ie_subelement_device_info wfd_ie_device_info;
    struct wfd_ie_subelement_associated_bss associated_bssid_subelement;

} __attribute__ ((packed));

typedef struct _l2sd_p2p_wfd_ie l2sd_p2p_wfd_ie;
#endif // __L2SD_TA_MSGS_H__

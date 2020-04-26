#ifndef __DELIVER_INFO_TO_DVDPLAYER_H__
#define __DELIVER_INFO_TO_DVDPLAYER_H__

#include "wifi_rtsp_defines.h"

#define MIRACAST_WIFI_SOURCE_FILE		"/tmp/miracast_wifi_source"
#define MIRACAST_HDCP2_INFO_FILE		"/tmp/miracast_hdcp2_info"

typedef struct 
{
	SOCKET_TS_DESC_CONFIG	config;
	SOCKET_CW_OFFSET		key;
	SOCKET_CW_OFFSET		iv;
} deliver_hdcp2_info;

bool write_wifi_source_file(struct wifi_source *src);
bool write_hdcp2_info_file(void);

#endif

#ifndef __WIFI_RTSP_CMDS_H
#define __WIFI_RTSP_CMDS_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "wifi_rtsp_defines.h"

#define WIFI_RTP_DEFAULT_PORT        24030
#define WIFI_I2C_DEFAULT_PORT        23030
#define WIFI_HDCP2_DEFAULT_PORT      25030

// utility functions
struct ltstr
{
        bool operator()(const std::string s1, const std::string s2) const
        {
                return s1.compare(s2) < 0;
        }
};

bool rtsp_parse_for_pattern(int nfd, char *buffer, int blen, const char * pattern, int plen, int timeout_sec=0);
void rtsp_payload_parse(int nfd, int contentLength, std::vector<std::string> &result);

#define KEYBOARD_SET              0x00000002
#define MOUSE_SET                 0x00000004
#define MT_SET					  0x00000008    // Multi Touch
#define SET_INPUT_TYPE_GENERIC(x) (x &= 0xfffffffe)
#define SET_INPUT_TYPE_HIDC(x)    (x |= 0x00000001)
#define IS_INPUT_GENERIC(x)       ((x & 0x00000001) == 0)
#define IS_INPUT_HIDC(x)          (x & 0x00000001)
#define KEY_BOARD_ENABLE(x)       (x |= KEYBOARD_SET)
#define KEY_BOARD_DISABLE(x)      (x &= ~(KEYBOARD_SET))
#define MOUSE_BOARD_ENABLE(x)     (x |= MOUSE_SET)
#define MOUSE_BOARD_DISABLE(x)    (x &= ~(MOUSE_SET))
#define MT_BOARD_ENABLE(x)     	  (x |= MT_SET)
#define MT_BOARD_DISABLE(x)    	  (x &= ~(MT_SET))
#define IS_KEYBOARD_ENABLE(x)     (x & KEYBOARD_SET)
#define IS_MOUSE_ENABLE(x)        (x & MOUSE_SET)
#define IS_MT_ENABLE(x)           (x & MT_SET)

void wifi_rtsp_header_parse(int nfd, std::map<std::string, std::string, ltstr> &result);

// handle different RTSP cmds from wifi
bool wifi_rtsp_cmd_options(struct wifi_source *src, const char* options); // M1
bool wifi_rtsp_send_options(struct wifi_source *src); // send M2
bool wifi_rtsp_cmd_getparameter(struct wifi_source * src, const char * options);
bool wifi_rtsp_cmd_setparameter(struct wifi_source *src, const char * options);
bool wifi_rtsp_cmd_trigger(struct wifi_source *src, WIFI_TRIGGER_METHOD method);

// i2c related 
int wifi_i2c_init(void);
bool wifi_i2c_incoming(int nfd);

// hdcp2 related
#define HDCP2_NEGO_STATUS_NONE           0
#define HDCP2_NEGO_STATUS_STARTED        1
#define HDCP2_NEGO_STATUS_TIMEDOUT       2
#define HDCP2_NEGO_STATUS_DONE           3
int  wifi_hdcp2_init(void);
int  wifi_hdcp2_set(void);
bool wifi_hdcp2_process_msg(int nfd);
int wifi_hdcp2_status(SOCKET_CW_OFFSET* aeskey, SOCKET_CW_OFFSET * aesiv);

// uibc related
void hid_keyboard_init(void);
void send_hid_keyboard(struct wifi_source *src, unsigned char type, unsigned short ascii);
void send_hid_keyboard(unsigned char type, unsigned short dfb_key_id, unsigned short dfb_key_symbol);
void send_hid_keyboard2(unsigned char type, unsigned short dfb_key_id, unsigned short dfb_key_symbol);
void wifi_uibc_event(unsigned char category, unsigned char * body, unsigned short length);

#endif

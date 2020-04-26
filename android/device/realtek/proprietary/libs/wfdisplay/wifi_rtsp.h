#ifndef __WIFI_RTSP_RTK_H
#define __WIFI_RTSP_RTK_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "SocketAVData.h"
#include "wifi_rtsp_defines.h"
#include "directfb_keyboard.h"
//
//enum for hdcp thread state
enum HDCP_THREAD_STATE {
    EM_HDCP_THREAD_STATE_START,
    EM_HDCP_THREAD_STATE_PENDING,
    EM_HDCP_THREAD_STATE_STOP,
    EM_HDCP_THREAD_STATE_EXIT,
};

// functions to be used by extern program
bool wifi_rtsp_init(char* ip, int port, void (*callback)(WIFI_SOURCE_STATE, WIFI_SOURCE_STATE) = NULL);
bool wifi_rtsp_quit();
bool wifi_rtsp_pause();     // send pause from sink, must be in play state
bool wifi_rtsp_play();
bool wifi_rtsp_sendidr();   // send idr, for video recovery
bool wifi_rtsp_teardown();  // 
bool wifi_rtsp_standby(); // standby request


WIFI_SOURCE_STATE wifi_rtsp_state();
void set_wifi_rtsp_state(WIFI_SOURCE_STATE iState);
int is_rtsp_thread_run();
int is_wfd_m7();

//constant value for uibc
typedef enum _HID_INTERFACE_TYPE{
	UIBC_HIDC_INFRAED				= 0,
	UIBC_HIDC_USB					= 1,
	UIBC_HIDC_BLUETOOTH				= 2,
	UIBC_HIDC_ZIGBEE				= 3,
	UIBC_HIDC_WIFI					= 4,
	UIBC_HIDC_RESERVED				= 5,
	UIBC_HIDC_VENDER_DEFINE			= 255
} UIBC_HIDC_INTERFACE_TYPE;

typedef enum _HID_TYPE{
	UIBC_HIDC_TYPE_KEYBOARD			= 0,
	UIBC_HIDC_TYPE_MOUSE				= 1,
	UIBC_HIDC_TYPE_SINGLE_TOUCH		= 2,
	UIBC_HIDC_TYPE_MULTI_TOUCH		= 3,
	UIBC_HIDC_TYPE_JOYSTICK			= 4,
	UIBC_HIDC_TYPE_CAMERA			= 5,
	UIBC_HIDC_TYPE_GESTURE			= 6,
	UIBC_HIDC_TYPE_REMOTE_CONTROL	= 7,
	UIBC_HIDC_TYPE_RESERVED			= 8,
	UIBC_HIDC_TYPE_VENDER_DEFINE		= 255			
} UIBC_HID_TYPE;

#define UIBC_HIDC_REPORT		(0)
#define UIBC_HIDC_DESCRIPTOR	(1)

#define UIBC_HID_INPUT_BODY_HEADER_SIZE	(5)
//constant value for HIDC MOUSE
#define HIDC_MOUSE_REPORT_SIZE		(4)
#define HIDC_KEYBOARD_REPORT_SIZE	(8)

#define UIBC_HIDC_MOUSE_INPUT_BODY_SIZE	(UIBC_HID_INPUT_BODY_HEADER_SIZE + HIDC_MOUSE_REPORT_SIZE)
#define UIBC_HIDC_KEYBOARD_INPUT_BODY_SIZE	(UIBC_HID_INPUT_BODY_HEADER_SIZE + HIDC_KEYBOARD_REPORT_SIZE)

//HIDC mouse button mask
#define HIDC_MOUSE_LEFT_BUTTON_MASK		(0x04)
#define HIDC_MOUSE_MIDDLE_BUTTON_MASK	(0x02)
#define HIDC_MOUSE_RIGHT_BUTTON_MASK	(0x01)

#define MOUSE_BUTTON_DOWN              0
#define MOUSE_BUTTON_UP                1
#define MOUSE_MOTION                   2
#define MOUSE_WHEEL		       3

#define MT_DOWN					       4  // MT = Multi Touch Screen
#define MT_UP					       5	
#define MT_MOTION				       6	

#define KEYBOARD_DOWN                  2
#define KEYBOARD_UP                    3
#define INPUT_GENERIC                  0
#define INPUT_HIDC                     1
#define WFD_TIMEOUT                 150
#define POLICE_LOOP_TIME  			   60 // seconds
#define CHK_HDCP1X_LOOP_TIME  		   30 // seconds

#define MAX_MT_SLOT     10
//
typedef struct {
	unsigned char status;
	unsigned char contact_id;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
} TOUCHPOINT;

void send_keyboard_action(unsigned char type, unsigned short ascii, unsigned short dfb_key_id);
void send_mouse_action(unsigned char type, unsigned short x, unsigned short y, unsigned char buttons);
void send_mt_action(unsigned char type, unsigned short pointer_num, TOUCHPOINT* touch_data, unsigned short active_id);  // Multi Touch
void wifi_uibc_init(void);


void wifi_get_audioinfo(SOCKET_MEDIA_TYPE *atype, long *chan_num, long *sampleRate, long* bitspersample);
void wifi_get_videoinfo(bool *is_3d, long *fps, int *mode3d, long * width, long* height);
#endif

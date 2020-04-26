#ifndef _RTD_MESSAGES_H_
#define _RTD_MESSAGES_H_

#ifdef DVR
	#include "rtd_types.h"
#else
	#include <rtd_types.h>
#endif

/*
#define MSG_QUEUE_GLOBAL		(getuid() + 300)
#define MSG_QUEUE_IO_COMMAND	1701
#define MSG_QUEUE_URGENT		1702

#define DTV_MESSAGE_TYPE		2008

#ifdef XTEST_ENABLE
    #define DTV_MESSAGE_STRING_LEN  64
    #define DTV_MESSAGE_SIZE   DTV_MESSAGE_STRING_LEN+sizeof(unsigned int)
#else
   #define DTV_MESSAGE_SIZE     sizeof(unsigned int)
#endif

#ifdef XTEST_ENABLE
    typedef struct DTV_MESSAGE_T {
        long type;
        UINT32 data;
        INT8 string[DTV_MESSAGE_STRING_LEN];
    }DTV_MESSAGE;
#else
    typedef struct DTV_MESSAGE_T {
	   long type;
	   UINT32 data;
    }DTV_MESSAGE;
#endif
*/


//*************************************************************
// DEFINE
//*************************************************************
// message should start after 300 because state chart has its own message starting from 0.
#define RC_MSG_BEGIN                     300

#define DTV_MODULE_MSG_BEGIN			1000

#define UI_MSG_BEGIN      	            2000

#define LIB_MSG_BEGIN                   3000

#define KERNEL_MSG_BEGIN                4000
#define ROS_MSG_BEGIN					4001
#define TP_MSG_BEGIN					4100
#define ATV_MSG_BEGIN					4200
#define IO_MSG_BEGIN					4300

#define TEST_MSG_BEGIN                  5000


//*************************************************************
// ENUM
//*************************************************************
enum LIB_MSG_TYPE {
	MSG_LIB_NULL = LIB_MSG_BEGIN,

	// Timer Message.
	MSG_LIB_TIMER,

	// SI messages.
	MSG_LIB_SI_MV_ALARM_TIMEOUT,
	MSG_LIB_SI_SV_ALARM_TIMEOUT,
	MSG_LIB_SI_MV_EPG_RESERVATION,
	MSG_LIB_SI_SV_EPG_RESERVATION,
	MSG_LIB_SI_MV_GOT_EIT,
	MSG_LIB_SI_SV_GOT_EIT,
	MSG_LIB_SI_MV_GOT_EIT0,
	MSG_LIB_SI_SV_GOT_EIT0,
	MSG_LIB_SI_MV_RESET_CHANNEL,
	MSG_LIB_SI_SV_RESET_CHANNEL,
	MSG_LIB_SI_MV_STREAM_CHANGE_BEGIN,
	MSG_LIB_SI_MV_STREAM_CHANGE_END,
	MSG_LIB_SI_SV_STREAM_CHANGE_BEGIN,
	MSG_LIB_SI_SV_STREAM_CHANGE_END,
	MSG_LIB_SI_RESET_TIME,
	MSG_LIB_SI_NEW_PMT,
	MSG_LIB_SI_CH_INFO_READY,
	MSG_LIB_SI_MV_GOT_EAM,
	MSG_LIB_SI_MV_GOT_DRRT,

	// CA Message description please see dtv/include/system/chi/chihandler.h
	MSG_LIB_CA_OPEN_HTML,
	MSG_LIB_CA_OPEN_MSGBOX,	
 	MSG_LIB_CA_CLOSE_DIALOG,
};

enum SYSTEM_MSG_TYPE {
	MSG_SYS_NULL = KERNEL_MSG_BEGIN,

    // ROS messages.
    MSG_SYS_ROS_NULL = ROS_MSG_BEGIN,
	MSG_SYS_MV_READY,
	MSG_SYS_SV_READY,
	MSG_SYS_MV_DISP_FMT_CHANGE,
	MSG_SYS_SV_DISP_FMT_CHANGE,
	MSG_SYS_MV_SET_COMPRESS_READY,
	MSG_SYS_SV_SET_COMPRESS_READY,
	MSG_SYS_ROS_HANG,
	MSG_SYS_MA_READY,
	MSG_SYS_SA_READY,

	// TP messages.
	MSG_SYS_TP_NULL = TP_MSG_BEGIN,
	MSG_SYS_TP0_SIGNAL_HALT,
	MSG_SYS_TP0_SIGNAL_RESUME,
	MSG_SYS_TP1_SIGNAL_HALT,
	MSG_SYS_TP1_SIGNAL_RESUME,
	MSG_SYS_TP0_FIFO_FULL,
	MSG_SYS_TP1_FIFO_FULL,
	MSG_SYS_TP_RESET,

	// 2620 ATV messages
	MSG_SYS_ATV_NULL = ATV_MSG_BEGIN,
	MSG_SYS_ATV_CH_INFO_READY,
	MSG_SYS_MV_VDC_DECTECT,
	MSG_SYS_SV_VDC_DECTECT,
	MSG_SYS_VBI_VCHIP_MAIN_READY,
	MSG_SYS_VBI_VCHIP_SUB_READY,
	MSG_SYS_SHOW_NO_SIGNAL,
	MSG_SYS_HDMI_DECTECT,
	MSG_SYS_ADC_DECTECT,
	MSG_SYS_NO_SIGNAL,

	// I/O messages
	MSG_SYS_IO_NULL = IO_MSG_BEGIN,
	MSG_SYS_HDCP_AUTHENTICATION,
};

#endif


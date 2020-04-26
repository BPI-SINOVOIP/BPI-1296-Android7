#ifndef _HEARTBEAT_H__
#define _HEARTBEAT_H__

#include <stdio.h>
#include <unistd.h>
#include <cutils/log.h>

#define HCI_CMD_VNDR_HEARTBEAT      0xFC94

#define CONFIG_ITEM_TIMEOUT           "HeartBeatTimeOut"
#define CONFIG_ITEM_LOG               "HeartBeatLog"
#define CONFIG_GROUP_HEARTBEAT             "HeartBeat"


#endif

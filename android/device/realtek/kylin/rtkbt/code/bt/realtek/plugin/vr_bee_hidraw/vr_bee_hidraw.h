#ifndef __RTKBT_BEE_VOICE_HIDRAW_H__
#define __RTKBT_BEE_VOICE_HIDRAW_H__
#include <stdio.h>
#include <unistd.h>
#include "rtkbt_api.h"
#include <cutils/log.h>
//#include "bt_trace.h"
//#include "gki.h"
//#include "gki_target.h"


#define VOHOG_DATA_IN_PATH     "/data/misc/bluedroid/.vohog_in"
#define VOHOG_CTRL_PATH        "/data/misc/bluedroid/.vohog_ctrl"

#define UIPC_CH_ID_RTKBT_VR_CTRL     UIPC_CH_ID_RTKBT_CHANNEL_1
#define UIPC_CH_ID_RTKBT_DATA        UIPC_CH_ID_RTKBT_CHANNEL_2
#define UIPC_CH_ID_RTKBT_VR_AUDIO    UIPC_CH_ID_RTKBT_CHANNEL_3

#define VOHOG_CTRL_CMD_START  1
#define VOHOG_CTRL_CMD_STOP   0

//Add for Bee Voice, to update GATT MTU
#define BEE_VOICE_GATT_MTU  200
//End for Bee Voice

//default for bee1
const char KEYDOWN_DEFAULT_VAL[8] = {0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00};
const char KEYUP_DEFAULT_VAL[8] = {0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const char KEY_DEFAULT_LEN = 8;

//key value load from config
char *keyDown = NULL;
char *keyUp = NULL;
unsigned int keyLen = 0;
unsigned int useDefault = 1;

/*voice type*/
enum
{
    RTKBT_IFLYTEK_INPUT_ACK = 0xF8,
    RTKBT_IFLYTEK_INPUT_DATA = 0xF9,
    RTKBT_IFLYTEK_OUTPUT_DATA = 0xFA
};

#define RTKBT_INPUT_DATA_BEE        0x5A
#define RTKBT_OUTPUT_REPORT_BEE     0x5A

#define RTKBT_INPUT_REPORT_TYPE     0x01
#define RTKBT_INPUT_DATA_TYPE       0x01
#define RTKBT_OUTPUT_REPORT_TYPE    0x02

#define CONFIG_GROUP_RCU            "Rcu"
#define CONFIG_ITEM_RCU_VID         "Vid"
#define CONFIG_ITEM_RCU_PID         "Pid"
#define CONFIG_ITEM_RCU_KEYLENGTH   "KeyLength"
#define CONFIG_ITEM_RCU_KEYDOWN     "KeyDwon"
#define CONFIG_ITEM_RCU_KEYUP       "KeyUp"

struct RcuStatusNotification
{
    unsigned char opcode;
    unsigned char args[8];
};


void RTKBT_Bee_init(RTKBT_API_CALLBACK * callbacks);
void RTKBT_Bee_cleanup();


#endif



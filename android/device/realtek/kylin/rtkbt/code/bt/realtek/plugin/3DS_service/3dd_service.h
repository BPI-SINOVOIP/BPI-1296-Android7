#include <stdio.h>
#include <unistd.h>
#include "rtkbt_api.h"
#include <cutils/log.h>

#define MANUFACT_LENGTH     0x06
const UINT8 manufact_4_3dd[5] = {
        0x0f, 0x00,         //fix id, use broadcom id
        0x00,               //fixed Shall be set to 0. If this field is not 0, Legacy 3DG shall skip this section
        0x01,               //3dd supported
        0x50                //Path Loss Threshold
};
static DEV_CLASS dev_class_4_3dd = {0x08, 0x04, 0x3c};

static UINT8 sync_train_param_4_3dd[] = {0x80, 0x00,     //interval min
                                        0x80, 0x00,     //interval max
                                        0xFE, 0xFF, 0x00, 0x00,  //synchronization_trainTO
                                        0x00            //service data
};

static UINT8 connless_slbc_param[] = {0x01,      //enable
                                     0x01,      //LT_ADDR
                                     0x00,      //LPO_Allowed
                                     0xEF, 0x33,    //Packet_Type
                                     0x80, 0x00,    //interval_min
                                     0x80, 0x00,    //interval_max
                                     0xFE, 0x0F     //CSB_supervisionTO
};

static UINT8 connless_slbc_data[] = {0x01,          //LT_ADDR
                                     0x03,          //fragment
                                     0x11,          //data_length
                                     0x00, 0x00, 0x00, 0x00,        //see 3DS spec page 39
                                     0x00, 0x00,        //Bluetooth clock phase at Frame Sync Instant(us)
                                     0x0a, 0x00,        //Left lens shutter open offset(us)
                                     0x0a, 0x80,        //Left lens shutter close offset(us)
                                     0x0a, 0x00,        //Right lens shutter open offset(us)
                                     0x0a, 0x80,        //Right lens shutter close offset(us)
                                     0x2C, 0x41,        //Frame Sync Period(us)
                                     0x08               //Frame Sync Period Fraction(1/256 us)
};
#define FRAME_SYNC_TOGGLE       0x28
#define HANDSHAKE_3DG           0x23

#define HCI_SYNC_TRAIN_CMPL_EVT             0x4F
#define HCI_CONNLESS_SL_BC_TO_EVT           0x52
#define HCI_SL_PAGE_RESPONSE_TO_EVT         0x54

#define HCI_INQ_SCAN_INTERVAL               0x45
#define HCI_INQ_SCAN_WINDOW                 0x34

#define HCI_PAGE_SCAN_INTERVAL              0x100
#define HCI_PAGE_SCAN_WINDOW                0x65
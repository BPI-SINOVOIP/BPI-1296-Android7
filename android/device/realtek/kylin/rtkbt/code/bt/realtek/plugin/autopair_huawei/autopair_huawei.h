
#include <stdio.h>
#include <unistd.h>
#include "rtkbt_api.h"
#include <cutils/log.h>

#define HCI_RTKBT_AUTOPAIR_EVT                                  0x30
#define HCI_VENDOR_TV_ADD_DEVICE_TO_POWERON_WHITE_LIST          0x7B
#define HCI_VENDOR_TV_REMOVE_DEVICE_FROM_POWERON_WHITE_LIST     0x7C
#define HCI_VENDOR_TV_CLEAR_POWERON_WHITE_LIST                  0x7D
#define HCI_VENDOR_TV_AUTOPAIR_COMMAND                          0x77
#define HCI_VENDOR_TV_PARAM_LEN                                    7


#define RTKBT_STORAGE_FILL_PROPERTY(p_prop, t, l, p_v) \
         (p_prop)->type = t;(p_prop)->len = l; (p_prop)->val = (p_v);

extern void rtkbt_process_huawei_autopaire_vendor_event(UINT8 len, UINT8  *data);



#include <stdio.h>
#include <unistd.h>
#include "rtkbt_api.h"
#include <cutils/log.h>

#define HCI_RTKBT_AUTOPAIR_EVT                               0x30

extern void rtkbt_process_autopaire_vendor_event(UINT8 len, UINT8  *data);


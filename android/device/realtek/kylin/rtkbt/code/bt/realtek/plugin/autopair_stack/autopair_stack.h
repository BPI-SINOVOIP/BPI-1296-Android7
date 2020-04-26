
#include <stdio.h>
#include <unistd.h>
#include "rtkbt_api.h"
#include <cutils/log.h>

#define HCI_RTKBT_AUTOPAIR_EVT                              0x30
#define HCI_RTKBT_AUTOPAIR_CMD                              0x77

#define CONFIG_GROUP_AUTOPAIR_TYPE                          "Autopair_type"
#define CONFIG_ITEM_TYPE                                    "Type"

#define CONFIG_GROUP_ADDRESS_PAIR                           "Address_pair"
#define CONFIG_ITEM_ADDRESS                                 "Address"
#define CONFIG_ITEM_ADDRESS_MASK                            "Address_mask"

#define CONFIG_GROUP_NAME_PAIR                              "Name_pair"
#define CONFIG_ITEM_NAME                                    "Name"
#define CONFIG_ITEM_NAME_MASK                               "Name_mask"

#define CONFIG_GROUP_MANUF_PAIR                             "Manufact_pair"
#define CONFIG_ITEM_MANUF_DATA                              "Manufact_data"
#define CONFIG_ITEM_MANUF_MASK                              "Manufact_mask"

#define RTKBT_ADDRESS_PAIR                                  1
#define RTKBT_NAME_PAIR                                     2
#define RTKBT_MANUF_PAIR                                    8

#define RTKBT_NAME_MAX                                      256
extern void rtkbt_process_autopaire_vendor_event(UINT8 len, UINT8  *data);


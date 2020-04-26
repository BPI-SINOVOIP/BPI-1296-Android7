
#include <stdio.h>
#include <unistd.h>
#include "rtkbt_api.h"
#include <cutils/log.h>
#include <sys/times.h>

#define HCI_RTKBT_FWLOG_SUB_EVT 0x20
#define CONFIG_GROUP_EVENT "Event"
#define CONFIG_ITEM_EVENT_ENABLE "Enable"
#define CONFIG_ITEM_EVENT_FILEPATH "Path"

#define FWLOG_BUF_SIZE 256
#define FWLOG_FIFO_SIZE 16
#define FWLOG_FORMAT_BUF_SIZE 512

typedef struct {
    unsigned int size;
    unsigned char evt_buf[FWLOG_BUF_SIZE];
}tEVT_BUF;

typedef struct {
    tEVT_BUF buf_fifo[FWLOG_FIFO_SIZE];
    unsigned char head_num;
    unsigned char tail_num;
    unsigned char total_valid_num;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}tFWLOG_RAW_FIFO;

extern void rtkbt_process_fwlog_vendor_event(UINT8 len, UINT8  *data);


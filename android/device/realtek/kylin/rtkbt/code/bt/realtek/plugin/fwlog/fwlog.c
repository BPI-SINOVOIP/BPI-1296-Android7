#include "fwlog.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <utime.h>

/*********************************************************************************
**  This plugin gets fw log over hci event
**********************************************************************************/


/**********************************************************************************
**  Static variables
**********************************************************************************/
static int fw_log_fd = -1;
static int fw_logging_enabled = FALSE;
static char fw_logfile_path[256] = "/sdcard/fwlog";

static int half_flag = 0;
static tFWLOG_RAW_FIFO gfwlogbuf;
static pthread_t fwlog_parser_threadID;
static unsigned char gState;

static RTKBT_API_CALLBACK * rtkbt_api_callbacks = NULL;


/*******************************************************************************
** initialize fwlog raw data fifo
*******************************************************************************/
void rtkbt_fwlog_fifo_init(tFWLOG_RAW_FIFO *fwlog_fifo) {
    fwlog_fifo->head_num = 0;
    fwlog_fifo->tail_num = 0;
    fwlog_fifo->total_valid_num = 0;
    pthread_mutex_init(&fwlog_fifo->mutex, NULL);
    pthread_cond_init(&fwlog_fifo->cond, NULL);
}

/*******************************************************************************
** get a buffer from the fwlog fifo
*******************************************************************************/
int rtkbt_fwlog_fifo_getbuf(tFWLOG_RAW_FIFO *fwlog_fifo, unsigned char *buf, int bufsize) {
    int ret = 0;
    pthread_mutex_lock(&fwlog_fifo->mutex);
    while(1) {
        if(fwlog_fifo->total_valid_num == 0) {
            struct timeval now;
            struct timespec timeout;
            int retcode;

            gettimeofday(&now, NULL);
            timeout.tv_sec = now.tv_sec + 1;
            timeout.tv_nsec = now.tv_usec * 1000;

            retcode = pthread_cond_timedwait(&fwlog_fifo->cond, &fwlog_fifo->mutex, &timeout);
            if (retcode == ETIMEDOUT)
            {
                pthread_mutex_unlock(&fwlog_fifo->mutex);
                ret = -1;
                break;
            }
        }
        else
        {
            ALOGD("head_num: %d", fwlog_fifo->head_num);
            //if((fwlog_fifo->head_num >= 0) && (fwlog_fifo->head_num < FWLOG_FIFO_SIZE)) {
                ret = fwlog_fifo->buf_fifo[fwlog_fifo->head_num].size;
                if(bufsize >= ret) {
                    memcpy(buf, fwlog_fifo->buf_fifo[fwlog_fifo->head_num].evt_buf, ret);
                    fwlog_fifo->total_valid_num --;
                    fwlog_fifo->head_num ++;
                    if(fwlog_fifo->head_num >= FWLOG_FIFO_SIZE)
                        fwlog_fifo->head_num = 0;
                }else { //buffer size error
                    ret = -2;
                }
            //}
            pthread_mutex_unlock(&fwlog_fifo->mutex);
            break;
        }
    }
    return ret;
}

/*******************************************************************************
** put a buffer into the fwlog fifo
*******************************************************************************/
void rtkbt_fwlog_fifo_putbuf(tFWLOG_RAW_FIFO *fwlog_fifo, unsigned char *buf, unsigned int bufsize) {
    tEVT_BUF *p_cur_buf;
    if(bufsize > FWLOG_BUF_SIZE)
        return;

    pthread_mutex_lock(&fwlog_fifo->mutex);
    if(fwlog_fifo->total_valid_num >= FWLOG_FIFO_SIZE) {
        ALOGD("fwlog putbuf: full !!!");
        //notice that fwlog_fifo->tail_num == fwlog_fifo->head_num,
        //we should overwrite the oldest data,
        //move fwlog_fifo->head_num ahead by one step to
        //ensure that the client get the oldest buffer first,
        //mutex is locked for overwriting data
        p_cur_buf = fwlog_fifo->buf_fifo + fwlog_fifo->tail_num;
        p_cur_buf->size = bufsize;
        memcpy(p_cur_buf->evt_buf, buf, bufsize);
        fwlog_fifo->tail_num ++;
        if(fwlog_fifo->tail_num >= FWLOG_FIFO_SIZE)
            fwlog_fifo->tail_num = 0;

        fwlog_fifo->head_num ++;
        if(fwlog_fifo->head_num >= FWLOG_FIFO_SIZE)
            fwlog_fifo->head_num = 0;
        pthread_mutex_unlock(&fwlog_fifo->mutex);
    }
    else {
        pthread_mutex_unlock(&fwlog_fifo->mutex);
        p_cur_buf = fwlog_fifo->buf_fifo + fwlog_fifo->tail_num;
        p_cur_buf->size = bufsize;
        memcpy(p_cur_buf->evt_buf, buf, bufsize);
        fwlog_fifo->tail_num ++;
        if(fwlog_fifo->tail_num >= FWLOG_FIFO_SIZE)
            fwlog_fifo->tail_num = 0;

        pthread_mutex_lock(&fwlog_fifo->mutex);
        fwlog_fifo->total_valid_num ++;
        pthread_mutex_unlock(&fwlog_fifo->mutex);
        pthread_cond_signal(&fwlog_fifo->cond);
    }
}

/*******************************************************************************
** insert time value into raw fwlog
*******************************************************************************/
void rtkbt_generate_format_fwlog(UINT8 *log, UINT8 len)
{
    UINT8 timestamp[9];
    UINT8 buf[FWLOG_FORMAT_BUF_SIZE];
    struct timeval tv;
    time_t temp_time;
    struct tm *read_time;
    int src_count;
    int target_count;
    int temp;

    if(fw_log_fd < 0) {
        return;
    }

    //generate the time format, 9 bytes
    gettimeofday(&tv, NULL);
    temp_time = tv.tv_sec;
    read_time = localtime(&temp_time);
    temp = tv.tv_usec/1000;

    *((UINT16 *)timestamp) = read_time->tm_year;
    *((UINT8 *)timestamp + 2) = read_time->tm_mon;
    *((UINT8 *)timestamp + 3) = read_time->tm_mday;
    *((UINT8 *)timestamp + 4) = read_time->tm_hour;
    *((UINT8 *)timestamp + 5) = read_time->tm_min;
    *((UINT8 *)timestamp + 6) = read_time->tm_sec;
    //*((UINT16 *)timestamp + 7) = (UINT16)(tv.tv_usec/1000);
    timestamp[7] = temp & 0x00ff;
    timestamp[8] = (temp & 0xff00) >> 8;

    src_count = 0;
    target_count = 0;
    //generate formatted fwlog
    memset(buf, 0, FWLOG_FORMAT_BUF_SIZE);
    while((src_count < len) && (target_count < FWLOG_FORMAT_BUF_SIZE)) {
        if(log[src_count] == 0xA5) {
            if((src_count == 0) && (half_flag == 1)) {
                lseek(fw_log_fd, -1, SEEK_CUR);
                memcpy(buf + target_count, timestamp, 9);
                target_count += 9;
                buf[target_count++] = 0xA5;
            }else if(src_count == (len - 1)) {
                half_flag = 1;
            }else if(log[src_count + 1] == 0xA5) {
                memcpy(buf + target_count, timestamp, 9);
                target_count += 9;
                buf[target_count++] = 0xA5;
                src_count++;
            }
        }else {
            half_flag = 0;
        }
        buf[target_count] = log[src_count];
        src_count++;
        target_count++;
    }

    write(fw_log_fd, buf, target_count);
}

void* rtkbt_generate_fwlog_loop(void *arg) {
    unsigned char buf[FWLOG_BUF_SIZE];
    int logsize;

    ALOGD("fwlog generator thread entered");

    rtkbt_fwlog_fifo_init(&gfwlogbuf);

    while(gState) {
        logsize = rtkbt_fwlog_fifo_getbuf(&gfwlogbuf, buf, FWLOG_BUF_SIZE);
        if(logsize > 0) {
            rtkbt_generate_format_fwlog(buf, logsize);
        }
    }
    ALOGD("fwlog generator thread exit");
    return NULL;
}


/*******************************************************************************
**  process fwlog vendor event
*******************************************************************************/
void rtkbt_process_fwlog_vendor_event(UINT8 len, UINT8  *data)
{
    UINT8 * pp = data;
    UINT8 subevent_cod = *pp;
    pp++;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_process_fwlog_vendor_event: rtkbt_api_callbacks is null \n");
        return;
    }

    ALOGI("rtkbt_process_fwlog_vendor_event subevent_cod = %d \n", subevent_cod);
    switch(subevent_cod) {
        case HCI_RTKBT_FWLOG_SUB_EVT:
            if(fw_log_fd != -1) {
                //rtkbt_generate_format_fwlog(pp + 1, len - 2);
                rtkbt_fwlog_fifo_putbuf(&gfwlogbuf, pp + 1, len - 2);
            }
            break;

        default :
            break;
    }
}


/*******************************************************************************
   * **
   * ** function rtkbt_fwlog_cmd
   * **
   * ** Description Enable/Disable fwlog over HCI event/ACL data

   * ** Arguments   id :  RTKBT_FWLOG_ID
                    cmd:  fwlog vendor cmd
                        HCI_VENDOR_SET_TV_AUTO_PAIR      0xFC27
                    data:
                                    Parameter       Size (octets)       Description
                                    --------------------------------------------------------------------------
                                    TYPE                1               0x00: over HCI event
                                                                        0x01: over ACL data
                                    --------------------------------------------------------------------------
                                    Conn_Handle         2               the connection handle for fwlog over ACL data
                                    --------------------------------------------------------------------------
                                    Enable              1               0x00: Disable
                                                                        0x01: Enable
                                    --------------------------------------------------------------------------

                    len: legth of data

   * ** Returns     int 1:success   0:fail
*********************************************************************************/

static int rtkbt_fwlog_Generic_cmd(int id, int enable, unsigned char * data, int len)
{
    int status;
    int cmd_opcode = 0x27;
    unsigned char fwlog_enable[4] = {0x00,0xff,0x00,0x01};
    unsigned char fwlog_disable[4] = {0x00,0xff,0x00,0x00};

    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_fwlog_Generic_cmd: rtkbt_api_callbacks is null \n");
        return 0;
    }

    if(id != RTKBT_FWLOG_ID)
        return 0;

    ALOGI("send fwlog generic cmd enable = %d\n", enable);
    if(enable == 1)
        status = rtkbt_api_callbacks->BTM_VendorSpecificCommand(cmd_opcode, len, (UINT8 *)fwlog_enable, NULL);
    else
        status = rtkbt_api_callbacks->BTM_VendorSpecificCommand(cmd_opcode, len, (UINT8 *)fwlog_disable, NULL);

    return (status <= 1 ? 1 : 0);
}


static int rtkbt_fwlog_Hook_func(int event, void * arg, int len)
{
    int i = 0;
    //int status = BTM_NO_RESOURCES;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_fwlog_Hook_func: rtkbt_api_callbacks is null \n");
        return i;
    }
    switch(event)
    {
        case RTKBT_HOOK_ENABLE_BT_COMPLETE:          // args: N/A    len:0
            //only enable/disable when BT opened
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            if(rtkbt_api_callbacks) {
                rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_fwlog_vendor_event, TRUE);
                rtkbt_fwlog_Generic_cmd(RTKBT_FWLOG_ID, fw_logging_enabled, NULL, 4);
            }
            break;
        case RTKBT_HOOK_DISABLE_BT_BEGIN:            // args: N/A    len:0
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            if(rtkbt_api_callbacks) {
                rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_fwlog_vendor_event, FALSE);
                if( (fw_logging_enabled == TRUE) && (fw_log_fd != -1)) {
                    ALOGW("rtkbt_fwlog_Hook_func: close fw_log_fd \n");
                    close(fw_log_fd);
                    fw_log_fd = -1;
                }
            }
            rtkbt_api_callbacks = NULL;
            break;
        case RTKBT_HOOK_DISABLE_BT_COMPLETE:         // args: int    len:0
            ALOGI("RECV HOOK(%d) arg={%d}", event, (int)arg);
            break;

        case RTKBT_HOOK_AUTOPAIR_VND_EVENT:
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            break;

        default:
            break;
    }
    return i;
}

/*******************************************************************************
**  Get configs
********************************************************************************/
static void rtkbt_fwlog_Get_config_value(void) {
    char * enable = rtkbt_api_callbacks->GetConfig(RTKBT_FWLOG_ID, CONFIG_GROUP_EVENT, CONFIG_ITEM_EVENT_ENABLE);
    char * path = rtkbt_api_callbacks->GetConfig(RTKBT_FWLOG_ID, CONFIG_GROUP_EVENT, CONFIG_ITEM_EVENT_FILEPATH);

    if(enable != NULL)
        ALOGW("rtkbt_fwlog_Get_config_value, enable is %s", enable);
    else
        ALOGW("rtkbt_fwlog_Get_config_value, enable is NULL");

    if(path != NULL)
        ALOGW("rtkbt_fwlog_Get_config_value, path is %s", path);
    else
        ALOGW("rtkbt_fwlog_Get_config_value, path is NULL");

    if(enable && path) {
        ALOGW("fwlog: enable & filepath found");
        if(!strcmp(enable, "true")) {
            ALOGW("fwlog: enabled! file path is %s", path);
            fw_logging_enabled = true;
            memcpy(fw_logfile_path, path, 50);
        }
        else {
            ALOGW("fwlog: Disabled!");
            fw_logging_enabled = false;
        }
    }
}


/*******************************************************************************
   * **
   * ** Typedef   fwlog init
   * **
   * ** Description must call first if you need fwlog function
   * ** Arguments RTKBT_API_CALLBACK *
   * ** Returns    void
*********************************************************************************/
static int init(RTKBT_API_CALLBACK * callbacks, RTKBT_API_INIT_ARG * arg)
{
    char fwlog_back_path[256] = {0};

    if(NULL == callbacks)
        return -1;
    ALOGI("begin to fwlog init \n");
    if(callbacks->size != sizeof(RTKBT_API_CALLBACK))
    {
        ALOGE("size error: ver %08x, size %d\n", callbacks->version, callbacks->size);
        return -2;
    }
    rtkbt_api_callbacks = callbacks;
    rtkbt_api_callbacks->RegisterGenericCommand(RTKBT_FWLOG_ID, rtkbt_fwlog_Generic_cmd);
    rtkbt_api_callbacks->RegisterHook(RTKBT_FWLOG_ID, rtkbt_fwlog_Hook_func);

    /* get configs & open fwlog.log */
    rtkbt_fwlog_Get_config_value();
    if(fw_logging_enabled == true) {

        ALOGW("fw log has been enabled");
        strncpy(fwlog_back_path, fw_logfile_path, 50);
        strcat(fwlog_back_path , ".last");
        rename(fw_logfile_path, fwlog_back_path);

        fw_log_fd = open(fw_logfile_path,
                        O_WRONLY | O_CREAT | O_TRUNC,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if(fw_log_fd == -1) {
            ALOGW("fw log has been disabled -- open failed, errno-%s", strerror(errno));
            fw_logging_enabled = FALSE;
        }

/*
        fw_log_fd_org= open("/sdcard/fwlog",
                        O_WRONLY | O_CREAT | O_TRUNC,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if(fw_log_fd_org == -1) {
            ALOGW("fw log has been disabled -- open failed, errno-%s", strerror(errno));
            fw_logging_enabled = FALSE;
        }
*/
        gState = 1;
        pthread_create(&fwlog_parser_threadID, NULL, rtkbt_generate_fwlog_loop, NULL);
    }
    return 0;
}

PLUGIN_INIT(init);


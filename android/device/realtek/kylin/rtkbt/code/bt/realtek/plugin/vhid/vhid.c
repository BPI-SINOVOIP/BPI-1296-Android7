#define LOG_TAG "VHID"
#include <ctype.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <linux/uhid.h>
#include <cutils/log.h>
#include <stdlib.h>

#include "rtkbt_api.h"
#include "vhid.h"

static char *rtkbt_uhid_path = "/dev/uhid";
static char *power_vhid_name = "rtkbt_power_vhid";

static UINT8 vendor_command_power_on[7]= {0x00,0x01,0x01,0x20,0x00,0x60,0x00};
static UINT8 vendor_command_power_off[7]= {0x00,0x00,0x01,0x0A,0x00,0x64,0x00};

static int power_vhid_fd = -1;
static int power_vhid_last_key_index = -1;

static UINT8 power_key_down[9]= {0x01,0x00,0x00,0x3b,0x00,0x00,0x00,0x00,0x00};
static UINT8 power_key_up[9]= {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static UINT16 power_key_len = 9;

static UINT8 rcu_ignore_report_id[256];
static int rcu_ingore_report_len = 0;

static int rcu_vhid_fd = -1;
static int rcu_vhid_thread_id = -1;
static int rcu_vhid_keep_polling = 0;
static int rcu_vhid_vid = 0;
static int rcu_vhid_pid = 0;
static int rcu_vhid_ver = 0;

static int loglevel = 0;

static bool isIngoreReportID(UINT8 report_id);

static void dump_data(char * tag, unsigned char * data, int len)
{
    int i;
    char * buf,*p;
    buf = malloc(len*4+strlen(tag)+32);
    p = buf;

    p += sprintf(p, "%s:", tag);
    for(i=0; i < len; i++)
    {
        if((i%16) == 0)
            p += sprintf(p, "\n0X%08x:", i);
        p += sprintf(p, " %02x", data[i]);
    }
    *p++ = '\n';
    *p = 0;
    ALOGD("%s", buf);
    free(buf);
};

static RTKBT_API_CALLBACK* rtkbt_api_callbacks = NULL;

static inline pthread_t create_thread(void *(*start_routine)(void *), void * arg){
    ALOGI("%s", __FUNCTION__);
    pthread_attr_t thread_attr;

    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    pthread_t rcu_vhid_thread_id = -1;
    if ( pthread_create(&rcu_vhid_thread_id, &thread_attr, start_routine, arg)!=0 )
    {
        ALOGE("pthread_create : %s", strerror(errno));
        return -1;
    }
    ALOGI("create_thread: thread created successfully");
    return rcu_vhid_thread_id;
}
/* Internal function to parse the events received from UHID driver*/
static int vhid_event(int *p_fd)
{
    struct uhid_event ev;
    BT_HDR  *p_buf;
    UINT8 *data ;
    ssize_t ret;
    memset(&ev, 0, sizeof(ev));
    tPlugin_comm g_dev_cb;
    rtkbt_api_callbacks->Send_PluginMsg(RTKBT_VR_ID, VHID_2_VR_G_DEV_CB, &g_dev_cb);

    if(!p_fd)
    {
        ALOGE("%s: Device not found",__FUNCTION__);
        return -1;
    }
    ret = read(*p_fd, &ev, sizeof(ev));
    if (ret == 0) {
        ALOGE("%s: Read HUP on uhid-cdev %s", __FUNCTION__,
                                                 strerror(errno));
        return -EFAULT;
    } else if (ret < 0) {
        ALOGE("%s:Cannot read uhid-cdev: %s", __FUNCTION__,
                                                strerror(errno));
        return -errno;
    } else if (ret != sizeof(ev)) {
        ALOGE("%s:Invalid size read from uhid-dev: %d != %d",
                            __FUNCTION__, ret, sizeof(ev));
        return -EFAULT;
    }

    switch (ev.type) {
    case UHID_START:
        if(loglevel) ALOGD("VHID_START from uhid-dev\n");
        break;
    case UHID_STOP:
        if(loglevel) ALOGD("VHID_STOP from uhid-dev\n");
        break;
    case UHID_OPEN:
        if(loglevel) ALOGD("VHID_OPEN from uhid-dev\n");
        break;
    case UHID_CLOSE:
        if(loglevel) ALOGD("VHID_CLOSE from uhid-dev\n");
        break;
    case UHID_OUTPUT:
        if(loglevel) ALOGD("VHID_OUTPUT: Report type = %d, report_size = %d"
                            ,ev.u.output.rtype, ev.u.output.size);
        //Send SET_REPORT with feature report if the report type in output event is FEATURE
        if(ev.u.output.rtype == UHID_FEATURE_REPORT) {
            if(loglevel) ALOGD("report type is feature type!");
        } else if(ev.u.output.rtype == UHID_OUTPUT_REPORT) {
            if(ev.u.output.size > 0){
                if(!g_dev_cb.vr_dev_cb.p_dev_cb) {
                    if(loglevel) ALOGD("p_dev_cb is null not send output data!");
                        break;
                }
                if((p_buf = (BT_HDR *) rtkbt_api_callbacks->rtkbt_plugin_getbuf((UINT16)(sizeof(BT_HDR) + ev.u.output.size ))) != NULL)
                {
                    memset(p_buf, 0, sizeof(BT_HDR) + ev.u.output.size);
                    p_buf->len = ev.u.output.size;
                    p_buf->layer_specific = BTA_HH_RPTT_OUTPUT;
                    data = (UINT8 *)(p_buf + 1);
                    memcpy(data, ev.u.output.data, ev.u.output.size);
                    if(loglevel) ALOGD("BTA_HhSendData to send output data!");
                    rtkbt_api_callbacks->BTA_HhSendData(g_dev_cb.vr_dev_cb.p_dev_cb->hid_handle, NULL, p_buf);
                    rtkbt_api_callbacks->rtkbt_plugin_freebuf(p_buf);
                }
            } else {
                if(loglevel) ALOGD("not send output data!");
            }
		} else {
            if(loglevel) ALOGD("report type is input type! cannot write!");
        }
        break;
    case UHID_OUTPUT_EV:
        if(loglevel) ALOGD("VHID_OUTPUT_EV from uhid-dev\n");
        break;
    case UHID_FEATURE:
        if(loglevel) ALOGD("VHID_FEATURE from uhid-dev\n");
        break;
    case UHID_FEATURE_ANSWER:
        if(loglevel) ALOGD("VHID_FEATURE_ANSWER from uhid-dev\n");
        break;

    default:
        if(loglevel) ALOGD("Invalid event from uhid-dev: %u\n", ev.type);
    }

    return 0;
}
static void *rcu_vhid_poll_event_thread(void *arg)
{

    int *p_fd = arg;
    struct pollfd pfds[1];
    int ret;
    //int i = 0;
    pfds[0].fd = *p_fd;
    pfds[0].events = POLLIN;

    ALOGI("%s", __FUNCTION__);
    while(rcu_vhid_keep_polling){
        ret = poll(pfds, 1, 500);
        if (ret < 0) {
            ALOGE("%s: Cannot poll for fds: %s\n", __FUNCTION__, strerror(errno));
            break;
        }
        if (pfds[0].revents & POLLIN) {
            if(loglevel) ALOGD("btif_hh_poll_event_thread: POLLIN");
            ret = vhid_event(p_fd);
            if (ret){
                break;
            }
        }
    }
    rcu_vhid_thread_id = -1;
    return 0;
}

static inline void close_poll_thread(int thread_id)
{
    ALOGI("%s", __FUNCTION__);
    rcu_vhid_keep_polling = 0;
    if(thread_id > 0)
        pthread_detach(thread_id);
    return;
}
static int uhid_write(int fd, const struct uhid_event *ev)
{
    ssize_t ret;
    ret = write(fd, ev, sizeof(*ev));
    if (ret < 0){
        int rtn = -errno;
        ALOGE("%s: Cannot write to uhid:%s", __FUNCTION__, strerror(errno));
        return rtn;
    } else if (ret != sizeof(*ev)) {
        ALOGE("%s: Wrong size written to uhid: %d != %d",
                                                    __FUNCTION__, ret, sizeof(*ev));
        return -EFAULT;
    } else {
        return 0;
    }
}

static int send_power_key(UINT8 *key_report)
{
    if(power_vhid_fd < 0) {
        ALOGE("power_vhid_fd < 0 !!!");
        return 0;
    }
    if(loglevel) ALOGD("%s", __FUNCTION__);
    struct uhid_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_INPUT;
    ev.u.input.size = power_key_len;
    if(power_key_len > sizeof(ev.u.input.data)){
        ALOGW("%s:report size greater than allowed size",__FUNCTION__);
        return -1;
    }

    memcpy(ev.u.input.data, key_report, power_key_len);

    return uhid_write(power_vhid_fd, &ev);

}

static void send_hid_info(int fd, char *dev_name, UINT16 vendor_id,
                             UINT16 product_id, UINT16 version, UINT8 ctry_code,
                             int dscp_len, UINT8 *p_dscp)
{
    int result;
    struct uhid_event ev;

    if (fd < 0) {
        ALOGE("fd < 0!!!");
        return;
    }

    ALOGI("%s: fd = %d, name = [%s], dscp_len = %d", __FUNCTION__,
                                                                    fd, dev_name, dscp_len);
    ALOGI("%s: vendor_id = 0x%04x, product_id = 0x%04x, version= 0x%04x,"
                                                                    "ctry_code=0x%02x",__FUNCTION__,
                                                                    vendor_id, product_id,
                                                                    version, ctry_code);

//Create and send hid descriptor to kernel
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_CREATE;
    strncpy((char*)ev.u.create.name, dev_name, sizeof(ev.u.create.name) - 1);
    ev.u.create.rd_size = dscp_len;
    ev.u.create.rd_data = p_dscp;
    ev.u.create.bus = BUS_BLUETOOTH;
    ev.u.create.vendor = vendor_id;
    ev.u.create.product = product_id;
    ev.u.create.version = version;
    ev.u.create.country = ctry_code;
    result = uhid_write(fd, &ev);

    ALOGI("%s: fd = %d, dscp_len = %d, result = %d", __FUNCTION__,
                                                                    fd, dscp_len, result);

    if (result) {
        ALOGE("%s: Error: failed to send DSCP, result = %d", __FUNCTION__, result);

        /* The HID report descriptor is corrupted. Close the driver. */
        close(fd);
        power_vhid_fd = -1;
    }
}

static void rtkbt_power_process_vendor_event(UINT8 len, UINT8  *data)
{
    int i, key_index, key_type;

    if(loglevel) ALOGD("rtkbt_power_process_vendor_event len=%d", len);
    for(i=0; i<len; i++)
        ALOGD("%d", data[i]);
    if(len == 3 && data[0] == 0x2F)
    {
        key_index = data[1];
        key_type = data[2];
    } else
        return;

    ALOGI("rtkbt_power_process_vendor_event len=%d, receive power vendor event", len);
    if(key_index == power_vhid_last_key_index)
        return;
    if(key_type == POWER_KEY_LONG)
    {
        //long press power key
        send_power_key(power_key_down);
        usleep(1000000);
        send_power_key(power_key_up);
    } else if(key_type == POWER_KEY_SHORT)
    {
        send_power_key(power_key_down);
        send_power_key(power_key_up);
    }
    power_vhid_last_key_index = key_index;
}

static int rtkbt_power_vhid_setup() {
    static UINT8 * power_vhid_map;
    int power_vhid_map_len = 0;
    char * map = rtkbt_api_callbacks->GetConfig(RTKBT_VHID_ID, CONFIG_GROUP_POWER, CONFIG_ITEM_POWER_REPORTMAP);
    if(map == NULL)
    {
        ALOGW("%s rtkbt_api_callbacks->GetConfig(%d,%s,%s) return NULL", __FUNCTION__, RTKBT_VHID_ID, CONFIG_GROUP_POWER, CONFIG_ITEM_POWER_REPORTMAP);
        return -1;
    }
    ALOGI("%s rtkbt_api_callbacks->GetConfig(%d,%s,%s) return %s", __FUNCTION__, RTKBT_VHID_ID, CONFIG_GROUP_POWER, CONFIG_ITEM_POWER_REPORTMAP, map);
    power_vhid_map = malloc(strlen(map));
    if(power_vhid_map == NULL)
    {
        ALOGE("%s malloc return NULL", __FUNCTION__);
        return -1;
    }
    if(rtkbt_api_str2hex(map, power_vhid_map, &power_vhid_map_len))
    {
        ALOGE("%s rtkbt_api_str2hex(%s) return error", __FUNCTION__, map);
        free(power_vhid_map);
        return -3;
    }
    if(loglevel) ALOGD("rtkbt_power_vhid_setup, power_vhid_map_len=0x%08x power_vhid_map=%s", power_vhid_map_len, power_vhid_map);
    if(loglevel) dump_data("rtkbt_power_vhid_setup, map:", power_vhid_map, power_vhid_map_len);
    if(power_vhid_fd<0) {
        power_vhid_fd = open(rtkbt_uhid_path, O_RDWR | O_CLOEXEC);
        if (power_vhid_fd < 0) {
            ALOGE("%s: Error: failed to open rcu_uhid, err:%s", __FUNCTION__,strerror(errno));
            free(power_vhid_map);
            return -4;
        } else
            ALOGE("%s: uhid fd = %d", __FUNCTION__, power_vhid_fd);
        send_hid_info(power_vhid_fd, power_vhid_name, POWER_VHID_VENDOR_ID, POWER_VHID_PRODUCT_ID, POWER_VHID_VERSION, POWER_VHID_COUNTRYCODE, power_vhid_map_len, power_vhid_map);
    } else
        ALOGD("rtkbt_power_vhid_setup already setup!");

    rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_power_process_vendor_event, TRUE);
    free(power_vhid_map);
    ALOGI("rtkbt_power_vhid_setup setup success!");
    return 0;
}

static int rtkbt_rcu_vhid_setup() {
    UINT8 * rcu_vhid_map;
    char rcu_vhid_name[256];
    char rcu_ignore_report_id_name[256];
    int rcu_vhid_map_len = 0;
    char item_name[256];
    char * map;
    char * p_ignore_report_id;
    char * vid = rtkbt_api_callbacks->GetConfig(RTKBT_VHID_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_VID);
    char * pid = rtkbt_api_callbacks->GetConfig(RTKBT_VHID_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_PID);
    char * ver = rtkbt_api_callbacks->GetConfig(RTKBT_VHID_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_VER);
    char * name = rtkbt_api_callbacks->GetConfig(RTKBT_VHID_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_NAME);

    memset(item_name, 0 , sizeof(item_name));
    memset(rcu_vhid_name, 0 , sizeof(rcu_vhid_name));
    memset(rcu_ignore_report_id, 0 , sizeof(rcu_ignore_report_id));
    if(vid && pid)
    {
        rcu_vhid_vid = strtol(vid, NULL, 16);
        rcu_vhid_pid = strtol(pid, NULL, 16);
        if(ver){
            rcu_vhid_ver = strtol(ver, NULL, 16);
            sprintf(item_name,"%s_%s_%04x_%s_%04x_%s_%04x", CONFIG_ITEM_RCU_ReportMap_Prefix, CONFIG_ITEM_RCU_VID, rcu_vhid_vid, CONFIG_ITEM_RCU_PID, rcu_vhid_pid, CONFIG_ITEM_RCU_VER, rcu_vhid_ver);
            sprintf(rcu_vhid_name,"%s_%s_%04x_%s_%04x_%s_%04x", RCU_VHID_NAME_Prefix, CONFIG_ITEM_RCU_VID, rcu_vhid_vid, CONFIG_ITEM_RCU_PID, rcu_vhid_pid, CONFIG_ITEM_RCU_VER, rcu_vhid_ver);
            sprintf(rcu_ignore_report_id_name,"%s_%s_%04x_%s_%04x_%s_%04x", CONFIG_ITEM_RCU_Ignore_ReportID, CONFIG_ITEM_RCU_VID, rcu_vhid_vid, CONFIG_ITEM_RCU_PID, rcu_vhid_pid, CONFIG_ITEM_RCU_VER, rcu_vhid_ver);
        }else{
            sprintf(item_name,"%s_%s_%04x_%s_%04x", CONFIG_ITEM_RCU_ReportMap_Prefix, CONFIG_ITEM_RCU_VID, rcu_vhid_vid, CONFIG_ITEM_RCU_PID, rcu_vhid_pid);
            sprintf(rcu_vhid_name,"%s_%s_%04x_%s_%04x", RCU_VHID_NAME_Prefix, CONFIG_ITEM_RCU_VID, rcu_vhid_vid, CONFIG_ITEM_RCU_PID, rcu_vhid_pid);
            sprintf(rcu_ignore_report_id_name,"%s_%s_%04x_%s_%04x", CONFIG_ITEM_RCU_Ignore_ReportID, CONFIG_ITEM_RCU_VID, rcu_vhid_vid, CONFIG_ITEM_RCU_PID, rcu_vhid_pid);
        }
    }else if(name){
        sprintf(item_name,"%s_%s_%s", CONFIG_ITEM_RCU_ReportMap_Prefix, CONFIG_ITEM_RCU_NAME, name);
        sprintf(rcu_vhid_name,"%s_%s_%s", RCU_VHID_NAME_Prefix, CONFIG_ITEM_RCU_NAME, name);
        sprintf(rcu_ignore_report_id_name,"%s_%s_%s", CONFIG_ITEM_RCU_Ignore_ReportID, CONFIG_ITEM_RCU_NAME, name);
    }else{
        ALOGE("%s: vid=%s, pid=%s, ver=%s, name=%s ==> Config Error!\n", __FUNCTION__, vid, pid, ver, name);
        return -1;
    }
    ALOGI("%s: vid=%04x, pid=%04x, ver=%04x, name=%s ==> item_name=%s\n", __FUNCTION__, rcu_vhid_vid, rcu_vhid_pid, rcu_vhid_ver, name, item_name);
    p_ignore_report_id = rtkbt_api_callbacks->GetConfig(RTKBT_VHID_ID,CONFIG_GROUP_RCU,rcu_ignore_report_id_name);
    if(NULL != p_ignore_report_id)
    {
        if(rtkbt_api_str2hex(p_ignore_report_id,rcu_ignore_report_id,&rcu_ingore_report_len))
        {
            ALOGE("%s: rtkbt_api_str2hex(%s) return error", __FUNCTION__, p_ignore_report_id);
            return -3;
        }
    }
    else
    {
        rcu_ingore_report_len = 0;
        ALOGW("%s: rtkbt_api_callbacks->GetConfig(%d,%s,%s) return NULL", __FUNCTION__, RTKBT_VHID_ID, CONFIG_GROUP_RCU, rcu_ignore_report_id_name);
    }
    map = rtkbt_api_callbacks->GetConfig(RTKBT_VHID_ID, CONFIG_GROUP_RCU, item_name);
    if(map == NULL)
    {
        ALOGW("%s: rtkbt_api_callbacks->GetConfig(%d,%s,%s) return NULL", __FUNCTION__, RTKBT_VHID_ID, CONFIG_GROUP_RCU, item_name);
        return -1;
    }
    ALOGI("%s: rtkbt_api_callbacks->GetConfig(%d,%s,%s) return %s", __FUNCTION__, RTKBT_VHID_ID, CONFIG_GROUP_RCU, item_name, map);
    rcu_vhid_map = malloc(strlen(map));
    if(rcu_vhid_map == NULL)
    {
        ALOGE("%s: malloc return NULL", __FUNCTION__);
        return -1;
    }
    if(rtkbt_api_str2hex(map, rcu_vhid_map, &rcu_vhid_map_len))
    {
        ALOGE("%s: rtkbt_api_str2hex(%s) return error", __FUNCTION__, map);
        free(rcu_vhid_map);
        return -3;
    }
    if(rcu_vhid_fd<0) {
        rcu_vhid_fd = open(rtkbt_uhid_path, O_RDWR | O_CLOEXEC);
        if (rcu_vhid_fd < 0) {
            ALOGE("%s: Error: failed to open rcu_uhid, err:%s", __FUNCTION__,strerror(errno));
            free(rcu_vhid_map);
            return -1;
        } else
            ALOGI("%s: rcu_uhid fd = %d", __FUNCTION__, rcu_vhid_fd);
        if(loglevel) ALOGD("rtkbt_rcu_vhid_setup, vid=%04x, pid=%02x, ver=%02x, rcu_vhid_map_len=0x%08x", rcu_vhid_vid, rcu_vhid_pid, rcu_vhid_ver, rcu_vhid_map_len);
        if(loglevel) dump_data("rtkbt_rcu_vhid_setup, map:", rcu_vhid_map, rcu_vhid_map_len);
        send_hid_info(rcu_vhid_fd, rcu_vhid_name, rcu_vhid_vid, rcu_vhid_pid, rcu_vhid_ver, RCU_VHID_COUNTRYCODE, rcu_vhid_map_len, rcu_vhid_map);
        rcu_vhid_keep_polling = 1;
        rcu_vhid_thread_id = create_thread(rcu_vhid_poll_event_thread, (void*)&rcu_vhid_fd);
    } else
        ALOGD("rtkbt_rcu_vhid_setup already setup!");
    free(rcu_vhid_map);
    ALOGI("rtkbt_rcu_vhid_setup setup success!");
    return 0;
}

static void rtkbt_power_vhid_destroy()
{
    ALOGI("%s", __FUNCTION__);
    struct uhid_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_DESTROY;
    uhid_write(power_vhid_fd, &ev);
    close(power_vhid_fd);
    power_vhid_fd = -1;

    rtkbt_api_callbacks->BTM_RegisterForVSEvents ((tBTM_VS_EVT_CB *)rtkbt_power_process_vendor_event, FALSE);
}

static void rtkbt_rcu_vhid_destroy() {
    ALOGI("%s", __FUNCTION__);
    if(rcu_vhid_fd < 0)
        return;
    struct uhid_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_DESTROY;
    uhid_write(rcu_vhid_fd, &ev);
    close_poll_thread(rcu_vhid_thread_id);
    close(rcu_vhid_fd);
    rcu_vhid_fd = -1;
    rcu_vhid_thread_id = -1;
}

static int Vhid_Hook_RTKBT_HOOK_BLE_HH_NOTIY_handler(int event, void * arg, int len)
{
    struct uhid_event ev;
    RTKBT_HOOK_BLE_HH_NOTIY_ARG * notification = (RTKBT_HOOK_BLE_HH_NOTIY_ARG * )arg;
    if(notification->p_dev_cb->dscp_info.vendor_id == rcu_vhid_vid) {
        if((rcu_vhid_ver)&&(rcu_vhid_ver != notification->p_dev_cb->dscp_info.version))
            return 0;
        if(rcu_vhid_fd < 0) {
            ALOGE("power_vhid_fd < 0 !!!");
            return 0;
        }
        UINT8 report_id = notification->p_rpt->rpt_id;
        if(isIngoreReportID(report_id))
            return 0;
        memset(&ev, 0, sizeof(ev));
        ev.type = UHID_INPUT;
        ev.u.input.size = (notification->p_data->len)+1;
        if(ev.u.input.size > sizeof(ev.u.input.data)){
            ALOGW("%s:report size greater than allowed size",__FUNCTION__);
            return 0;
        }
        ev.u.input.data[0] = notification->p_rpt->rpt_id;
        memcpy(&(ev.u.input.data[1]), notification->p_data->value, notification->p_data->len);
        if(loglevel) ALOGD("%s %02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n", __FUNCTION__,\
            ev.u.input.data[0], ev.u.input.data[1], ev.u.input.data[2], ev.u.input.data[3],\
            ev.u.input.data[4], ev.u.input.data[5], ev.u.input.data[6], ev.u.input.data[7], ev.u.input.data[8]);
        if(uhid_write(rcu_vhid_fd, &ev) < 0)
            return 0;
        return 1;
    }
    return 0;
}

static int rtkbt_api_vhid_comm_func(int event, RTKBT_HOOK_HID_STATUS_ARG * arg, int len)
{

     int status = arg->status;
     if(status == 0)
     {
        unsigned char upKey[9] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
        struct uhid_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.type = UHID_INPUT;
        ev.u.input.size = 9;
        if(ev.u.input.size > sizeof(ev.u.input.data))
        {
           ALOGW("%s:report size greater than allowed size",__FUNCTION__);
           return 0;
        }
        memcpy(&(ev.u.input.data[0]), upKey, 9);
        if(loglevel) ALOGD("%s %02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n", __FUNCTION__,\
             ev.u.input.data[0], ev.u.input.data[1], ev.u.input.data[2], ev.u.input.data[3],\
             ev.u.input.data[4], ev.u.input.data[5], ev.u.input.data[6], ev.u.input.data[7], ev.u.input.data[8]);
        if(uhid_write(rcu_vhid_fd, &ev) < 0)
             return 0;
        return 1;
     }
     return 0;

}
static int Vhid_Hook_func(int event, void * arg, int len)
{
    int res = 0;
    switch(event)
    {
        case RTKBT_HOOK_ENABLE_BT_COMPLETE:
            rtkbt_power_vhid_setup();
            rtkbt_rcu_vhid_setup();
            break;
        case RTKBT_HOOK_DISABLE_BT_BEGIN:
            rtkbt_power_vhid_destroy();
            rtkbt_rcu_vhid_destroy();
            break;
        case RTKBT_HOOK_BLE_HH_NOTIY:
            return Vhid_Hook_RTKBT_HOOK_BLE_HH_NOTIY_handler(event, arg, len);
            break;
        case RTKBT_HOOK_HID_STATUS:
            res = rtkbt_api_vhid_comm_func(event,arg,len);
            break;
        default:
            break;
    }
    return res;
}

int init(RTKBT_API_CALLBACK * callbacks, RTKBT_API_INIT_ARG * arg)
{
    if(NULL == callbacks)
        return -1;
    ALOGI("VHID init: ver %08x, size %d(%d) loglevel:%d\n", callbacks->version, callbacks->size, sizeof(RTKBT_API_CALLBACK), arg->loglevel);
    if(callbacks->size != sizeof(RTKBT_API_CALLBACK))
    {
        ALOGE("size error: ver %08x, size %d(%d)\n", callbacks->version, callbacks->size, sizeof(RTKBT_API_CALLBACK));
        return -2;
    }
    rtkbt_api_callbacks = callbacks;
    loglevel = arg->loglevel;
    rtkbt_api_callbacks->RegisterHook(RTKBT_VHID_ID, Vhid_Hook_func);
    ALOGI("VHID init success!: ver %08x, size %d(%d) loglevel:%d\n", callbacks->version, callbacks->size, sizeof(RTKBT_API_CALLBACK), arg->loglevel);
    return 0;
}

static bool isIngoreReportID(UINT8 report_id)
{
    bool isIngore = false;
    if(report_id <= 0 || rcu_ingore_report_len == 0)
        return isIngore;
    int i;
    for(i = 0; i < rcu_ingore_report_len; i++)
    {
        if(report_id == rcu_ignore_report_id[i])
        {
            isIngore = true;
            break;
        }
    }
    return isIngore;
}
PLUGIN_INIT(init);


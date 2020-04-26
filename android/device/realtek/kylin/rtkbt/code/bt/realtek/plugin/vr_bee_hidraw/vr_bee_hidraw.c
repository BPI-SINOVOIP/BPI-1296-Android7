#define LOG_TAG "RTKBT_VR_HIDRAW_BEE"
#include "vr_bee_hidraw.h"
#include <string.h>
#include <stdlib.h>

static RTKBT_API_CALLBACK * rtkbt_api_callbacks = NULL;
tBTA_HH_DEV_CB * Global_p_dev_cb = NULL;

static int voice_pid = 0;
static int voice_vid = 0;

static int gVoiceState;

//tBTA_GATTC_DATA bee_mtu;

void RTKBT_Bee_set_dev_cb(tBTA_HH_DEV_CB *p_dev_cb)
{
   Global_p_dev_cb = p_dev_cb;
}

tBTA_HH_DEV_CB * RTKBT_Bee_get_dev_cb(void)
{
    return Global_p_dev_cb;
}

//COEX: enable/disable LE dynamic priority
//opcode:0x95
//data(1 byte):
//      enable  0x01
//      diable  0x00
static int rtkbt_vr_bee_COEX_Generic_cmd(int id, int enable, unsigned char * data, int len)
{
    int status;
    int cmd_opcode = 0x95;
    unsigned char le_dynamic_priority_enable = 0x01;
    unsigned char le_dynamic_priority_disable= 0x00;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_huawei_autopair_Generic_cmd: rtkbt_api_callbacks is null \n");
        return 0;
    }

    if(id != RTKBT_VR_ID)
        return 0;

    ALOGI("send vr bee coex generic cmd enable = %d\n", enable);
    if(enable == 1)
        status = rtkbt_api_callbacks->BTM_VendorSpecificCommand(cmd_opcode, len, (UINT8 *)(&le_dynamic_priority_enable), NULL);
    else
        status = rtkbt_api_callbacks->BTM_VendorSpecificCommand(cmd_opcode, len, (UINT8 *)(&le_dynamic_priority_disable), NULL);
    return (status <= 1 ? 1 : 0);
}


static int rtkbt_api_Bee_comm_func(int id, int event, tPlugin_comm * plugin_arg)
{
    if(id != RTKBT_VR_ID) {
        ALOGE("%s : wrong id", __FUNCTION__);
        return -1;
    }
    switch (event)
    {
        case VHID_2_VR_G_DEV_CB:
        {
            plugin_arg->vr_dev_cb.p_dev_cb = RTKBT_Bee_get_dev_cb();
        }
        break;

        default:
        break;
    }
    return 0;
}


static int rtkbt_voice_BleHHNotify_func(int event, RTKBT_HOOK_BLE_HH_NOTIY_ARG * arg, int len)
{
    tBTA_HH_DEV_CB* p_dev_cb;
    tBTA_HH_LE_RPT* p_rpt;
    tBTA_GATTC_NOTIFY *p_data;
    p_dev_cb = arg->p_dev_cb;
    p_rpt = arg->p_rpt;
    p_data = arg->p_data;
    char* pKeyVal;
    ALOGI("RECV HOOK(%d) arg={%08x} len=%d", event, (unsigned int)arg, len);
    ALOGI("rtkbt_test_BleHHNotify_func: %04x,%04x,%d,%d\n",p_dev_cb->dscp_info.vendor_id,p_dev_cb->dscp_info.product_id, p_rpt->rpt_id, p_rpt->rpt_type);
    ALOGI("rtkbt_test_BleHHNotify_func: %02x:%02x:%02x:%02x:%02x:%02x",p_data->bda[0],p_data->bda[1],p_data->bda[2],p_data->bda[3],p_data->bda[4],p_data->bda[5]);

    UINT8 report_id = p_rpt->rpt_id;
    if((voice_vid && voice_pid) && (p_dev_cb->dscp_info.vendor_id == voice_vid) && (p_dev_cb->dscp_info.product_id == voice_pid)) {

        switch(report_id)
        {
            case 0x01://
                if(p_data->len == keyLen)
                {
                    if(gVoiceState == 0)
                    {
                        if(useDefault == 1)pKeyVal = (char *)KEYDOWN_DEFAULT_VAL;
                        else pKeyVal = keyDown;
                        if(memcmp(pKeyVal, p_data->value, keyLen) == 0)
                        {
                            ALOGD("voice start");
                            gVoiceState = 1;
                            rtkbt_vr_bee_COEX_Generic_cmd(RTKBT_VR_ID, 0, NULL, 1);
                            RTKBT_Bee_set_dev_cb(p_dev_cb);
                        }
                    }
                    else if(gVoiceState == 1)
                    {
                        if(useDefault == 1)pKeyVal = (char *)KEYUP_DEFAULT_VAL;
                        else pKeyVal = keyUp;
                        if(memcmp(pKeyVal, p_data->value, keyLen) == 0)
                        {
                            ALOGD("voice stop");
                            gVoiceState = 0;
                            rtkbt_vr_bee_COEX_Generic_cmd(RTKBT_VR_ID, 1, NULL, 1);
                        }
                    }
                    /*
                    int i = 0;
                    while(i < p_data->len)
                    {
                        if(F5_value[i] == p_data->value[i])
                        {
                            ALOGI("pdata is 0x%x", p_data->value[i]);
                            i++;
                        }
                        else
                            break;
                    }
                    if(i == 8)
                    {
                        ALOGD("voice start");
                        gVoiceState = 1;
                        rtkbt_vr_bee_COEX_Generic_cmd(RTKBT_VR_ID, 0, NULL, 1);
                        RTKBT_Bee_set_dev_cb(p_dev_cb);
                    }else{
                        if(gVoiceState == 1) {
                            ALOGD("stop voice");
                            //any key down here to stop voice
                            if(p_data->value[1] != BEE_KEY_UP_VALUE) {
                                gVoiceState = 0;
                                rtkbt_vr_bee_COEX_Generic_cmd(RTKBT_VR_ID, 1, NULL, 1);
                            }
                        }
                    }*/
                }
                break;
            default:
                break;
        }
    }
    return 0;
}

static int rtkbt_voice_NotifyHIDStatus_func(int event, RTKBT_HOOK_HID_STATUS_ARG * arg, int len)
{
    int status = arg->status;
    bt_bdaddr_t* addr = arg->addr;
    BD_ADDR bdaddr;
    memcpy(bdaddr, addr->address,BD_ADDR_LEN);
    tBTA_HH_DEV_CB * p_cb_temp = rtkbt_api_callbacks->bta_hh_le_find_dev_cb_by_bda(bdaddr);
    ALOGI("RECV HOOK(%d) arg={%08x} len=%d", event, (unsigned int)arg, len);
    ALOGI("rtkbt_test_NotifyHIDStatus_func: %d, %02x:%02x:%02x:%02x:%02x:%02x", status,addr->address[0],addr->address[1],addr->address[2],addr->address[3],addr->address[4],addr->address[5]);
    if(status == 0) {
        if(!p_cb_temp) {
            ALOGD("p_cb_temp not found");
            return -1;
        }
        if(voice_vid && voice_pid) {
            if((voice_vid != p_cb_temp->dscp_info.vendor_id) || (voice_pid != p_cb_temp->dscp_info.product_id)) {
                ALOGD("rtkbt_voice_NotifyHIDStatus_func, not the vid");
                return -2;
            }else {
                if(gVoiceState == 1) {
                    gVoiceState = 0;
                    rtkbt_vr_bee_COEX_Generic_cmd(RTKBT_VR_ID, 1, NULL, 1);
                }
            }
        }else {
            ALOGD("voice_vid or voice_pid not set!");
        }
    }
    return 0;
}


int rtkbt_voice_get_byte_array_config(char* src, char* arr, int len)
{
    char *front, *tail;
    int ret = -1;
    int i = 0;

    if((src == NULL) || (arr == NULL) || (len <= 0))
        goto retval;

    front = src;

    while(i < len) {
        tail = strstr(front, ",");
        if(tail == NULL) {
            tail = strstr(front, ";");
        }

        if(tail == NULL) {
            ALOGE("failed to get byte-array value: No%d", i + 1);
            goto retval;
        }

        *tail++ = 0;
        *(arr+i) = (strtol(front, NULL, 16)) & 0xff;
        front = tail;
        i++;
    }

    if(i == len)ret = i;

retval:
    return ret;
}



static void rtkbt_voice_get_config_value(void)
{
    char *vid = rtkbt_api_callbacks->GetConfig(RTKBT_VR_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_VID);
    char *pid = rtkbt_api_callbacks->GetConfig(RTKBT_VR_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_PID);

    char *keyLenStr = rtkbt_api_callbacks->GetConfig(RTKBT_VR_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_KEYLENGTH);
    char *keyDownStr = rtkbt_api_callbacks->GetConfig(RTKBT_VR_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_KEYDOWN);
    char *keyUpStr = rtkbt_api_callbacks->GetConfig(RTKBT_VR_ID, CONFIG_GROUP_RCU, CONFIG_ITEM_RCU_KEYUP);

    int keyLenVal = 0;
    char *keyDownVal = NULL;
    char *keyUpVal = NULL;
    //int i = 0;

    if(vid && pid)
    {
        voice_vid = strtol(vid, NULL, 16);
        voice_pid = strtol(pid, NULL, 16);
    }

    if(keyLenStr)
    {
        keyLenVal = strtol(keyLenStr, NULL, 16);
        if(keyLenVal > 0)
        {
            keyDownVal = (char *)malloc(keyLenVal);
            if(keyDownVal == NULL)
            {
                ALOGE("failed to alloc memory for Key Down Value");
                goto error;
            }
            if(rtkbt_voice_get_byte_array_config(keyDownStr, keyDownVal, keyLenVal) != keyLenVal)
            {
                ALOGE("failed to load Key Down Value");
                goto error;
            }
            keyUpVal = (char *)malloc(keyLenVal);
            if(keyUpVal == NULL)
            {
                ALOGE("failed to alloc memory for Key Up Value");
                goto error;
            }
            if(rtkbt_voice_get_byte_array_config(keyUpStr, keyUpVal, keyLenVal) != keyLenVal)
            {
                ALOGE("failed to load Key Up Value");
                goto error;
            }

            if(((keyDown = (char *)malloc(keyLenVal)) == NULL) ||
                ((keyUp = (char *)malloc(keyLenVal)) == NULL))
            {
                ALOGE("failed to alloc memory for Key Down & Key Up Value");
                goto error;
            }

            keyLen = keyLenVal;
            memcpy(keyDown, keyDownVal, keyLen);
            memcpy(keyUp, keyUpVal, keyLen);
            free(keyDownVal);
            free(keyUpVal);
            useDefault = 0;
            return;
        }
    }

error:
    ALOGE("Use Default Settings");
    useDefault = 1;
    keyLen = KEY_DEFAULT_LEN;
    if(keyDownVal != NULL)
    {
        free(keyDownVal);
    }
    if(keyUpVal != NULL)
    {
        free(keyUpVal);
    }
    if(keyDown != NULL)
    {
        free(keyDown);
        keyDown = NULL;
    }
    if(keyUp != NULL)
    {
        free(keyUp);
        keyUp = NULL;
    }
}
static void rtkbt_voice_remove_config_value(void)
{
    voice_pid = 0;
    voice_vid =0;
    if(useDefault == 0)
    {
        if(keyDown != NULL)
        {
            free(keyDown);
            keyDown = NULL;
        }
        if(keyUp != NULL)
        {
            free(keyUp);
            keyUp = NULL;
        }
        useDefault = 1;
    }
}

//Add for Bee Voice, to update GATT MTU
static void rtkbt_voice_update_gatt_mtu(tBTA_HH_DEV_CB *arg, int len)
{

    if( (arg != NULL) && (len == sizeof(tBTA_HH_DEV_CB))) {
        if((rtkbt_api_callbacks != NULL) && (rtkbt_api_callbacks->BTA_GATTC_ConfigureMTU != NULL))
            rtkbt_api_callbacks->BTA_GATTC_ConfigureMTU(arg->conn_id, BEE_VOICE_GATT_MTU);
    }

}
//End for Bee Voice

static int rtkbt_voice_Hook_func(int event, void * arg, int len)
{
    int i = 0;
    ALOGE("RECV HOOK(%d) arg={N/A}", event);
    switch(event)
    {
        case RTKBT_HOOK_ENABLE_BT_COMPLETE:          // args: N/A    len:0
            ALOGE("RECV HOOK(%d) arg={N/A}", event);
            rtkbt_voice_get_config_value();
            break;
        case RTKBT_HOOK_DISABLE_BT_BEGIN:            // args: N/A    len:0
            ALOGE("RECV HOOK(%d) arg={N/A}", event);
            rtkbt_voice_remove_config_value();
            RTKBT_Bee_cleanup();
            break;
        case RTKBT_HOOK_DISABLE_BT_COMPLETE:         // args: int    len:0
             ALOGE("RTKBT_HOOK_DISABLE_BT_COMPLETE .RECV HOOK(%d) arg={%d}", event, (int)arg);
            break;
        case RTKBT_HOOK_BLE_HH_NOTIY:                // args: RTKBT_HOOK_BLE_HH_NOTIY_ARG   len:sizeof(RTKBT_HOOK_BLE_HH_NOTIY_ARG)
            i = rtkbt_voice_BleHHNotify_func(event, arg, len);
            break;
        case RTKBT_HOOK_HID_STATUS:
            i = rtkbt_voice_NotifyHIDStatus_func(event, arg, len);
            break;
        //Add for Bee Voice, to update GATT MTU
        case RTKBT_HOOK_HH_LE_OPEN_CMPL:
            rtkbt_voice_update_gatt_mtu(arg, len);
            break;
        //End for Bee Voice
        default:
            break;
    }
    return i;
}


  /*******************************************************************************
   * **
   * ** Typedef   RTKBT_Bee_init
   * **
   * ** Description must call first if you need voice function
   * ** Arguments RTKBT_API_CALLBACK *
   * ** Returns    void
    *********************************************************************************/
static int init(RTKBT_API_CALLBACK * callbacks, RTKBT_API_INIT_ARG * arg)
{
    if(NULL == callbacks)
        return -1;
    if(callbacks->size != sizeof(RTKBT_API_CALLBACK))
    {
        ALOGE("size error: ver %08x, size %d\n", callbacks->version, callbacks->size);
        return -2;
    }
    rtkbt_api_callbacks = callbacks;
    rtkbt_api_callbacks->RegisterHook(RTKBT_VR_ID, rtkbt_voice_Hook_func);
    rtkbt_api_callbacks->RegisterPluginExMsg(RTKBT_VR_ID,rtkbt_api_Bee_comm_func);
    gVoiceState = 0;
	return 0;
}

void RTKBT_Bee_cleanup()
{
    ALOGI("RTKBT_Bee_cleanup to close ctrl and data socket");
}

PLUGIN_INIT(init);


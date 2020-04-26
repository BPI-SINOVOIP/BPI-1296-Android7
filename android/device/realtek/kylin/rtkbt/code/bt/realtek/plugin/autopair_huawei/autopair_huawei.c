#include "autopair_huawei.h"
#include "rtkbt_api.h"

/**********************************************************************************
           huawei Autopair plugin can not coexist with Autopair plugin
           it must be attention to rtkbt_plugins.conf in system/etc/bluetooth/
           choose the right *.so
***********************************************************************************/

/**********************************************************************************
**  Static variables
**********************************************************************************/
static RTKBT_API_CALLBACK * rtkbt_api_callbacks = NULL;

/*******************************************************************************
**  process autopair vendor event
*******************************************************************************/
void rtkbt_process_huawei_autopair_vendor_event(UINT8 len, UINT8  *data)
{
    UINT8 * pp = data;
    UINT8 subevent_cod = *pp;
    pp++;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_process_huawei_autopair_vendor_event: rtkbt_api_callbacks is null \n");
        return;
    }

    ALOGI("rtkbt_process_huawei_autopair_vendor_event subevent_cod = %d \n", subevent_cod);
    switch(subevent_cod) {
        case HCI_RTKBT_AUTOPAIR_EVT :
                    rtkbt_api_callbacks->SendGenericEvent(RTKBT_HUAWEI_AUTOPAIR, subevent_cod, (unsigned char *)pp, (len-1));
                    rtkbt_api_callbacks->btm_ble_process_adv_pkt((UINT8 *)(data+1));
        break;

        default :
        break;
    }
}

/*******************************************************************************
   * **
   * ** function rtkbt_autopair_Generic_cmd
   * **
   * ** Description If the autopair APK need autopair scan and need vendor event
                    send the generic cmd to the controller

   * ** Arguments   id :  RTKBT_AUTOPAIR_ID
                    cmd:  autopair vendor cmd
                        HCI_VENDOR_SET_TV_AUTO_PAIR      0xFC77
                    data:
                                    Parameter       Size (octets)       Description
                                    --------------------------------------------------------------------------
                                    ScanType            1               0x00: Passive scan
                                                                        0x01: Active scan
                                                                        0xFF: Disable
                                    --------------------------------------------------------------------------
                                    ScanInterval        2               Background LE scan interval (unit: 625 us)
                                    --------------------------------------------------------------------------
                                    ScanWindow          2               Background LE scan window (unit: 625 us)
                                    --------------------------------------------------------------------------
                                    OwnAddrType         1               0x00: Public device address
                                                                        0x01: Random device address
                                    --------------------------------------------------------------------------
                                    ScanFilterPolicy    1                   *******
                                    --------------------------------------------------------------------------
                    len: legth of data

   * ** Returns     int 1:success   0:fail
*********************************************************************************/

static int rtkbt_huawei_autopair_Generic_cmd(int id, int cmd_opcode, unsigned char * data, int len)
{
    int status;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_huawei_autopair_Generic_cmd: rtkbt_api_callbacks is null \n");
        return 0;
    }

    if(id != RTKBT_HUAWEI_AUTOPAIR)
        return 0;
    if(NULL == data || len != 7)
    {
        ALOGE("rtkbt_huawei_autopair_Generic_cmd: param is wrong\n");
        return 0;
    }

    status = rtkbt_api_callbacks->BTM_VendorSpecificCommand(cmd_opcode, len, (UINT8 *)data, NULL);
    return (status <= 1 ? 1 : 0);
}

/********************************************************************************************
* **
* ** function rtkbt_huawei_device_to_poweron_white_list
* **
* ** Description Add the LE hid device which is not the huawei remoter to poweron white list
                 so the LE hid can wake up the host by reconnecting too

* ** Arguments

            #define HCI_VENDOR_TV_ADD_DEVICE_TO_POWERON_WHITE_LIST          0xFC7B
            #define HCI_VENDOR_TV_REMOVE_DEVICE_FROM_POWERON_WHITE_LIST 0xFC7C
            typedef struct _PACKED_ HCI_VENDOR_TV_ADD_DEV_TO_POWERON_WL_CMD_PARAM_
            {
                UINT8 addr_type;
                UINT8 addr[6];
            } HCI_VENDOR_TV_ADD_DEV_TO_POWERON_WL_CMD_PARAM;


            #define HCI_VENDOR_TV_CLEAR_POWERON_WHITE_LIST              0xFC7D

*********************************************************************************************/
static void rtkbt_huawei_device_to_poweron_white_list(RTKBT_HOOK_LE_HID_STATUS_ARG* arg)
{
    unsigned char wl_cmd_param[7] = {0};
    bt_property_t remote_properties;
    bt_remote_version_t info;
    bt_bdaddr_t addr;
    memset(&remote_properties, 0, sizeof(remote_properties));
    if(!arg->p_dev_rec || !rtkbt_api_callbacks) {
        ALOGE("rtkbt_huawei_device_to_poweron_white_list: rtkbt_api_callbacks or p_dev_rec is null \n");
        return;
    }
    memcpy(addr.address,arg->p_dev_rec->bd_addr,BD_ADDR_LEN);
    if(arg->add_or_remv) {
        if(rtkbt_api_callbacks->bta_hh_le_find_dev_cb_by_bda(arg->p_dev_rec->bd_addr)) {
            RTKBT_STORAGE_FILL_PROPERTY(&remote_properties,BT_PROPERTY_REMOTE_VERSION_INFO,
                                        sizeof(bt_remote_version_t),&info);
            rtkbt_api_callbacks->btif_storage_get_remote_device_property(&addr, &remote_properties);
            if(info.manufacturer != 0x5d) {
                ALOGI("%s: it is not huawei remote, send bd address to white list", __FUNCTION__);
                //memcpy(&wl_cmd_param[1], arg->p_dev_rec->bd_addr,BD_ADDR_LEN);
                int i;
                for(i = 0; i < BD_ADDR_LEN; i++) {
                    wl_cmd_param[i+1] = arg->p_dev_rec->bd_addr[BD_ADDR_LEN - i -1];
                }
                wl_cmd_param[0] = arg->p_dev_rec->ble.ble_addr_type;
                rtkbt_api_callbacks->BTM_VendorSpecificCommand(HCI_VENDOR_TV_ADD_DEVICE_TO_POWERON_WHITE_LIST,
                                                               HCI_VENDOR_TV_PARAM_LEN, (UINT8 *)wl_cmd_param, NULL);
            }
        }
    }
    else {
        if(rtkbt_api_callbacks->bta_hh_le_find_dev_cb_by_bda(arg->p_dev_rec->bd_addr)) {
            RTKBT_STORAGE_FILL_PROPERTY(&remote_properties,BT_PROPERTY_REMOTE_VERSION_INFO,
                                        sizeof(bt_remote_version_t),&info);
            rtkbt_api_callbacks->btif_storage_get_remote_device_property(&addr, &remote_properties);
            if(info.manufacturer != 0x5d) {
                ALOGI("%s: it is not huawei remote, send bd address to white list", __FUNCTION__);
                //memcpy(&wl_cmd_param[1], arg->p_dev_rec->bd_addr,BD_ADDR_LEN);
                int i;
                for(i = 0; i < BD_ADDR_LEN; i++) {
                    wl_cmd_param[i+1] = arg->p_dev_rec->bd_addr[BD_ADDR_LEN - i -1];
                }
                wl_cmd_param[0] = arg->p_dev_rec->ble.ble_addr_type;
                rtkbt_api_callbacks->BTM_VendorSpecificCommand(HCI_VENDOR_TV_REMOVE_DEVICE_FROM_POWERON_WHITE_LIST,
                                                               HCI_VENDOR_TV_PARAM_LEN, (UINT8 *)wl_cmd_param, NULL);
            }
        }
    }
}

static int rtkbt_huawei_autopair_Hook_func(int event, void * arg, int len)
{
    int i = 0;
    int status = BTM_NO_RESOURCES;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_autopair_Hook_func: rtkbt_api_callbacks is null \n");
        return i;
    }
    switch(event)
    {
        case RTKBT_HOOK_ENABLE_BT_COMPLETE:          // args: N/A    len:0
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            status = rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_huawei_autopair_vendor_event, TRUE);
            unsigned char autopair_data_enable[7] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00};
            rtkbt_huawei_autopair_Generic_cmd(RTKBT_HUAWEI_AUTOPAIR, HCI_VENDOR_TV_AUTOPAIR_COMMAND, autopair_data_enable, HCI_VENDOR_TV_PARAM_LEN);
            break;
        case RTKBT_HOOK_DISABLE_BT_BEGIN:            // args: N/A    len:0
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            if(rtkbt_api_callbacks) {
                unsigned char autopair_data_disable[7] = {0xFF,0x00,0x00,0x00,0x00,0x00,0x00};
                rtkbt_huawei_autopair_Generic_cmd(RTKBT_HUAWEI_AUTOPAIR, HCI_VENDOR_TV_AUTOPAIR_COMMAND, autopair_data_disable, HCI_VENDOR_TV_PARAM_LEN);
                rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_huawei_autopair_vendor_event, FALSE);
            }
            rtkbt_api_callbacks = NULL;
            break;
        case RTKBT_HOOK_DISABLE_BT_COMPLETE:         // args: int    len:0
            ALOGI("RECV HOOK(%d) arg={%d}", event, (int)arg);
            break;

        case RTKBT_HOOK_AUTOPAIR_VND_EVENT:
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            break;

        case RTKBT_HOOK_LE_HID_WHITE_LIST_STATUS:
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            rtkbt_huawei_device_to_poweron_white_list(arg);
            break;

        default:
            break;
    }
    return i;
}


/*******************************************************************************
   * **
   * ** Typedef   autopair init
   * **
   * ** Description must call first if you need autopair function
   * ** Arguments RTKBT_API_CALLBACK *
   * ** Returns    void
*********************************************************************************/
int init(RTKBT_API_CALLBACK * callbacks, RTKBT_API_INIT_ARG * arg)
{
    if(NULL == callbacks)
        return -1;
    ALOGI("begin to huawei autopair init \n");
    if(callbacks->size != sizeof(RTKBT_API_CALLBACK))
    {
        ALOGE("size error: ver %08x, size %d\n", callbacks->version, callbacks->size);
        return -2;
    }
    rtkbt_api_callbacks = callbacks;
    rtkbt_api_callbacks->RegisterGenericCommand(RTKBT_HUAWEI_AUTOPAIR, rtkbt_huawei_autopair_Generic_cmd);
    rtkbt_api_callbacks->RegisterHook(RTKBT_HUAWEI_AUTOPAIR, rtkbt_huawei_autopair_Hook_func);
    return 0;
}


PLUGIN_INIT(init);


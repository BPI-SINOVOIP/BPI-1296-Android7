#define LOG_TAG "STACK_AUTOPAIR"
#include "autopair_stack.h"
#include "rtkbt_api.h"
#include <ctype.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <cutils/log.h>
#include <stdlib.h>


#define LOG_ENABLE  FALSE

#define warn(fmt, ...) ALOGW ("## WARNING : %s(L%d): " fmt "## \n",__FUNCTION__, __LINE__, ## __VA_ARGS__)
#define error(fmt, ...) ALOGE ("## ERROR : %s(L%d): " fmt "## \n",__FUNCTION__, __LINE__, ## __VA_ARGS__)

#if LOG_ENABLE
#define info(fmt, ...)  ALOGI ("%s(L%d): " fmt,"\n",__FUNCTION__, __LINE__,  ## __VA_ARGS__)
#define debug(fmt, ...) ALOGD ("%s(L%d): " fmt,"\n",__FUNCTION__, __LINE__,  ## __VA_ARGS__)
#else
#define info(fmt, ...)  ;
#define debug(fmt, ...) ;
#endif

/**********************************************************************************
**  Static variables
**********************************************************************************/
static RTKBT_API_CALLBACK * rtkbt_api_callbacks = NULL;
static UINT8 *address        = NULL;
static UINT8 *name           = NULL;
static UINT8 *manufact_data  = NULL;
static UINT8 *mask_value     = NULL;
static bool sAutoPairFlag = false;
/*******************************************************************************
**  process autopair vendor event
*******************************************************************************/
void rtkbt_process_autopair_vendor_event(UINT8 len, UINT8  *data)
{
    UINT8 * pp = data;
    UINT8 subevent_cod = *pp;
    pp++;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_process_autopair_vendor_event: rtkbt_api_callbacks is null \n");
        return;
    }

    ALOGI("rtkbt_process_autopair_vendor_event subevent_cod = %d \n", subevent_cod);
    switch(subevent_cod) {
        case HCI_RTKBT_AUTOPAIR_EVT :
            rtkbt_api_callbacks->SendGenericEvent(RTKBT_AUTOPAIR_ID, subevent_cod, (unsigned char *)pp, (len-1));
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
                                    Rcu Address Value   6               Rcu Address Value
                                    --------------------------------------------------------------------------
                                    Rcu Address Mask    6               Rcu Address Mask
                                    --------------------------------------------------------------------------
                                    CheckType           1               EIR type
                                    --------------------------------------------------------------------------
                                    Value_length        1               The length of data to check starting
                                                                        from the EIR data
                                    --------------------------------------------------------------------------
                                    Value               Value_length    value which is compaired by controller
                                                                        to make vendor event
                                    --------------------------------------------------------------------------
                                    Mask_value          Value_length    mask which is used by controller
                                    --------------------------------------------------------------------------
                    len: legth of data

   * ** Returns     int 1:success   0:fail
*********************************************************************************/

static int rtkbt_autopair_Generic_cmd(int id, int command, unsigned char * data, int len)
{
    int status;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_autopair_Generic_cmd: rtkbt_api_callbacks is null \n");
        return 0;
    }
    if(!sAutoPairFlag)
    {
        error("rtkbt_autopair_Generic_cmd: BT is not enable \n");
        return 0;
    }
    if(id != RTKBT_AUTOPAIR_ID)
        return 0;
    ALOGI("send autopair generic cmd \n");
    status = rtkbt_api_callbacks->BTM_VendorSpecificCommand(command, len, (UINT8 *)data, NULL);
    return (status <= 1 ? 1 : 0);
}

static int rtkbt_autopair_Generate_vendor_cmd(bool enable)
{
    char * autopair_type = rtkbt_api_callbacks->GetConfig(RTKBT_AUTOPAIR_ID, CONFIG_GROUP_AUTOPAIR_TYPE,CONFIG_ITEM_TYPE);
    if(!autopair_type) {
        error("can't get autopair_type!");
        return -1;
    }
    unsigned char pair_type = strtol(autopair_type, NULL, 16);
    debug("pari_type = 0x%02x", pair_type);

    if(!enable) {
        unsigned char vendor_cmd[21] = {0};
        vendor_cmd[0] = 0xFF;
        rtkbt_autopair_Generic_cmd(RTKBT_AUTOPAIR_ID,HCI_RTKBT_AUTOPAIR_CMD, vendor_cmd, sizeof(vendor_cmd));
        goto err;
    }

    switch (pair_type) {

    case RTKBT_ADDRESS_PAIR :
        {
            char *address_str   = rtkbt_api_callbacks->GetConfig(RTKBT_AUTOPAIR_ID, CONFIG_GROUP_ADDRESS_PAIR,CONFIG_ITEM_ADDRESS);
            char *mask_str      = rtkbt_api_callbacks->GetConfig(RTKBT_AUTOPAIR_ID, CONFIG_GROUP_ADDRESS_PAIR,CONFIG_ITEM_ADDRESS_MASK);
            address             = malloc(strlen(address_str));
            mask_value          = malloc(strlen(mask_str));

            int i =0;
            int address_len = 0;
            int mask_len = 0;
            if(!address_str || !mask_str) {
                error("address str or mask_str is NULL");
                goto err;
            }
            if(rtkbt_api_str2hex(address_str, address, &address_len))
            {
                error("rtkbt_api_str2hex(%s) return error", address_str);
                goto err;
            }
            if(rtkbt_api_str2hex(mask_str, mask_value, &mask_len))
            {
                error("rtkbt_api_str2hex(%s) return error", mask_str);
                goto err;
            }

            debug("address_len = %d, mask_len = %d", address_len, mask_len);
            if(address_len > 6 || mask_len > 6) {
                error("address or mask error!");
                goto err;
            }
            unsigned char vendor_cmd[21] = {0};
            vendor_cmd[0] = 0x01;
            for(i = 0; i < address_len; i++)
            {
                vendor_cmd[12 - i] = address[i];
                vendor_cmd[18 - i] = mask_value[i];
            }
            rtkbt_autopair_Generic_cmd(RTKBT_AUTOPAIR_ID,HCI_RTKBT_AUTOPAIR_CMD, vendor_cmd, 21);
        }
        break;

    case RTKBT_NAME_PAIR:
        {
            char *name_str      = rtkbt_api_callbacks->GetConfig(RTKBT_AUTOPAIR_ID, CONFIG_GROUP_NAME_PAIR, CONFIG_ITEM_NAME);
            char *mask_str      = rtkbt_api_callbacks->GetConfig(RTKBT_AUTOPAIR_ID, CONFIG_GROUP_NAME_PAIR, CONFIG_ITEM_NAME_MASK);
            name                = malloc(strlen(name_str));
            mask_value          = malloc(strlen(mask_str));

            int name_len = 0;
            int mask_len = 0;
            if(rtkbt_api_str2hex(name_str, name, &name_len))
            {
                error("rtkbt_api_str2hex(%s) return error", name_str);
                goto err;
            }
            if(rtkbt_api_str2hex(mask_str, mask_value, &mask_len))
            {
                error("rtkbt_api_str2hex(%s) return error", mask_str);
                goto err;
            }

            debug("name_len = %d, mask_len = %d", name_len, mask_len);

            if(name_len != mask_len) {
                warn("name len is not pair with mask_len!");
            }

            if(name_len > 0) {
                unsigned char vendor_cmd[100] = {0};
                vendor_cmd[0] = 0x01;
                vendor_cmd[19] = 0x09;
                vendor_cmd[20] = (unsigned char)name_len;
                memcpy(&vendor_cmd[21], name, name_len);
                memcpy(&vendor_cmd[21 + name_len], mask_value, mask_len);
                rtkbt_autopair_Generic_cmd(RTKBT_AUTOPAIR_ID, HCI_RTKBT_AUTOPAIR_CMD, vendor_cmd, (21 + name_len + mask_len));
            }
        }
        break;

    case RTKBT_MANUF_PAIR:
        {
            char *manuf_str     = rtkbt_api_callbacks->GetConfig(RTKBT_AUTOPAIR_ID, CONFIG_GROUP_MANUF_PAIR, CONFIG_ITEM_MANUF_DATA);
            char *mask_str      = rtkbt_api_callbacks->GetConfig(RTKBT_AUTOPAIR_ID, CONFIG_GROUP_MANUF_PAIR, CONFIG_ITEM_MANUF_MASK);
            manufact_data       = malloc(strlen(manuf_str));
            mask_value          = malloc(strlen(mask_str));

            int manuf_len = 0;
            int mask_len = 0;
            if(rtkbt_api_str2hex(manuf_str, manufact_data, &manuf_len))
            {
                error("rtkbt_api_str2hex(%s) return error", manuf_str);
                goto err;
            }
            if(rtkbt_api_str2hex(mask_str, mask_value, &mask_len))
            {
                error("rtkbt_api_str2hex(%s) return error", mask_str);
                goto err;
            }

            debug("manuf_len = %d, mask_len = %d", manuf_len, mask_len);

            if(manuf_len != mask_len) {
                warn("name len is not pair with mask_len!");
            }

            unsigned char vendor_cmd[100] = {0};
            vendor_cmd[0] = 0x01;
            vendor_cmd[19] = 0xFF;
            vendor_cmd[20] = (unsigned char)manuf_len;
            memcpy(&vendor_cmd[21], manufact_data, manuf_len);
            memcpy(&vendor_cmd[21 + manuf_len], mask_value, mask_len);
            rtkbt_autopair_Generic_cmd(RTKBT_AUTOPAIR_ID, HCI_RTKBT_AUTOPAIR_CMD, vendor_cmd, (21 + manuf_len + mask_len));
        }
        break;

    default:
        break;
    }
    return 1;

err:
    if(address)
        free(address);
    if(name)
        free(name);
    if(manufact_data)
        free(manufact_data);
    if(mask_value)
        free(mask_value);
    address = NULL;
    name = NULL;
    manufact_data = NULL;
    mask_value = NULL;
    return -1;
}

static int rtkbt_autopair_Hook_func(int event, void * arg, int len)
{
    int i = 0;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_autopair_Hook_func: rtkbt_api_callbacks is null \n");
        return i;
    }
    switch(event)
    {
        case RTKBT_HOOK_ENABLE_BT_COMPLETE:          // args: N/A    len:0
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_autopair_vendor_event, TRUE);
            sAutoPairFlag = true;
            rtkbt_autopair_Generate_vendor_cmd(true);
            break;
        case RTKBT_HOOK_DISABLE_BT_BEGIN:            // args: N/A    len:0
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            if(rtkbt_api_callbacks)
                rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_autopair_vendor_event, FALSE);
            rtkbt_autopair_Generate_vendor_cmd(false);
            rtkbt_api_callbacks = NULL;
            sAutoPairFlag = false;
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
    info("begin to autopair init \n");
    if(callbacks->size != sizeof(RTKBT_API_CALLBACK))
    {
        error("size error: ver %08x, size %d\n", callbacks->version, callbacks->size);
        return -2;
    }
    rtkbt_api_callbacks = callbacks;
    rtkbt_api_callbacks->RegisterGenericCommand(RTKBT_AUTOPAIR_ID, rtkbt_autopair_Generic_cmd);
    rtkbt_api_callbacks->RegisterHook(RTKBT_AUTOPAIR_ID, rtkbt_autopair_Hook_func);
    sAutoPairFlag = false;
    return 0;
}

PLUGIN_INIT(init);


#define LOG_TAG "APP_AUTOPAIR"
#include "autopair.h"
#include "rtkbt_api.h"

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
        error("rtkbt_process_autopair_vendor_event: rtkbt_api_callbacks is null");
        return;
    }

    debug("rtkbt_process_autopair_vendor_event subevent_cod = %d", subevent_cod);
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
        error("rtkbt_autopair_Generic_cmd: rtkbt_api_callbacks is null");
        return 0;
    }
    if(!sAutoPairFlag)
    {
        error("rtkbt_autopair_Generic_cmd: BT is not enable \n");
        return 0;
    }
    if(id != RTKBT_AUTOPAIR_ID)
        return 0;
    debug("send autopair generic cmd");
    status = rtkbt_api_callbacks->BTM_VendorSpecificCommand(command, len, (UINT8 *)data, NULL);
    return (status <= 1 ? 1 : 0);
}

static int rtkbt_autopair_Hook_func(int event, void * arg, int len)
{
    int i = 0;
    if(!rtkbt_api_callbacks) {
        error("rtkbt_autopair_Hook_func: rtkbt_api_callbacks is null");
        return i;
    }
    switch(event)
    {
        case RTKBT_HOOK_ENABLE_BT_COMPLETE:          // args: N/A    len:0
            info("RECV HOOK(%d) arg={N/A}", event);
            rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_autopair_vendor_event, TRUE);
            sAutoPairFlag = true;
            break;
        case RTKBT_HOOK_DISABLE_BT_BEGIN:            // args: N/A    len:0
            info("RECV HOOK(%d) arg={N/A}", event);
            if(rtkbt_api_callbacks)
                rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_autopair_vendor_event, FALSE);
            rtkbt_api_callbacks = NULL;
            sAutoPairFlag = false;
            break;
        case RTKBT_HOOK_DISABLE_BT_COMPLETE:         // args: int    len:0
            info("RECV HOOK(%d) arg={%d}", event, (int)arg);
            break;

        case RTKBT_HOOK_AUTOPAIR_VND_EVENT:
            info("RECV HOOK(%d) arg={N/A}", event);
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
    info("begin to autopair init");
    if(callbacks->size != sizeof(RTKBT_API_CALLBACK))
    {
        error("size error: ver %08x, size %d", callbacks->version, callbacks->size);
        return -2;
    }
    rtkbt_api_callbacks = callbacks;
    rtkbt_api_callbacks->RegisterGenericCommand(RTKBT_AUTOPAIR_ID, rtkbt_autopair_Generic_cmd);
    rtkbt_api_callbacks->RegisterHook(RTKBT_AUTOPAIR_ID, rtkbt_autopair_Hook_func);
    sAutoPairFlag = false;
    return 0;
}

PLUGIN_INIT(init);


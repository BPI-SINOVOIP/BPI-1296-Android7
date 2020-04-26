#include "3dd_service.h"
#include "rtkbt_api.h"

#define LOG_ENABLE  FALSE

#define warn(fmt, ...) ALOGW ("## WARNING : %s(L%d): " fmt "##",__FUNCTION__, __LINE__, ## __VA_ARGS__)
#define error(fmt, ...) ALOGE ("## ERROR : %s(L%d): " fmt "##",__FUNCTION__, __LINE__, ## __VA_ARGS__)

#if LOG_ENABLE
#define info(fmt, ...)  ALOGI ("%s(L%d): " fmt,__FUNCTION__, __LINE__,  ## __VA_ARGS__)
#define debug(fmt, ...) ALOGD ("%s(L%d): " fmt,__FUNCTION__, __LINE__,  ## __VA_ARGS__)
#else
#define info(fmt, ...)  ;
#define debug(fmt, ...) ;
#endif

/**********************************************************************************
**  Static variables
**********************************************************************************/
static RTKBT_API_CALLBACK * rtkbt_api_callbacks = NULL;
static INT8 eir_tx_power                        = 0;
static INT8 sRunning3DMode                      = 0;
static DEV_CLASS dev_class_org;
static BOOLEAN mSyncTrainActive                 = FALSE;
/* 3dd Extended Inquiry Response */
static tBTA_DM_EIR_CONF eir_cfg_4_3dd =
{
    50,    /* minimum length of local name when it is shortened */
           /* if length of local name is longer than this and EIR has not enough */
           /* room for all UUID list then local name is shortened to this length */
#if (BTA_EIR_CANNED_UUID_LIST == TRUE)
    8,
    NULL,
#else
    {   /* mask of UUID list in EIR */
        0xFFFFFFFF, /* LSB is the first UUID of the first 32 UUIDs in BTM_EIR_UUID_LKUP_TBL */
        0xFFFFFFFF  /* LSB is the first UUID of the next 32 UUIDs in BTM_EIR_UUID_LKUP_TBL */
        /* BTM_EIR_UUID_LKUP_TBL can be overrided */
    },
#endif
    NULL,   /* Inquiry TX power         */
    0,      /* length of flags in bytes */
    NULL,   /* flags for EIR */
    5,      /* length of manufacturer specific in bytes */
    (UINT8 *)manufact_4_3dd,   /* manufacturer specific */
    0,      /* length of additional data in bytes */
    NULL    /* additional data */
};


/**********************************************************************************
    static functions
***********************************************************************************/
static void rtkbt_3dd_read_inq_tx_power_cback(void * result);
static void rtkbt_start_3dd_service();
static void rtkbt_stop_3dd_service();
static void rtkbt_3dg_associated();
static void rtkbt_frame_sync_toggle(BOOLEAN status);
static void rtkbt_write_sync_train_param();
static void rtkbt_set_connless_slbc(BOOLEAN enable);
static void rtkbt_start_sync_train();
static void rtkbt_set_connless_slbc_data(UINT8 * data);
static void rtkbt_receive_event_4_3dd(BT_HDR * p_msg);

/*******************************************************************************
**  process 3dd vendor event
*******************************************************************************/
static void rtkbt_process_3dd_vendor_event(UINT8 len, UINT8  *data)
{
    UINT8 * pp = data;
    UINT8 subevent_cod = *pp;
    BOOLEAN status;
    pp++;
    if(!rtkbt_api_callbacks)
        return;
    debug("subevent_cod = %d \n", subevent_cod);
    switch(subevent_cod) {
        case HANDSHAKE_3DG :
            pp++;
            rtkbt_api_callbacks->SendGenericEvent(RTKBT_3DD_ID, subevent_cod, (unsigned char *)pp, (len-2));//notify apk the 3dg address
            //rtkbt_3dg_associated();
        break;
        case FRAME_SYNC_TOGGLE :
            status = (*pp == 0x01 ? TRUE : FALSE);
            rtkbt_api_callbacks->SendGenericEvent(RTKBT_3DD_ID, subevent_cod, (unsigned char *)pp, len-1);//notify apk the 3dg address
            rtkbt_frame_sync_toggle(status);
        break;

        default :
            warn("unknow 3dd subevent !");
        break;
    }
}


static void rtkbt_3dg_associated()
{
    debug();
    UINT8 data[] = {0x0a, 0x00, 0x00,
                    0x0a, 0x00, 0x80,
                    0x0a, 0x00, 0x00,
                    0x0a, 0x00, 0x80
    };
    rtkbt_write_sync_train_param();

    rtkbt_set_connless_slbc(TRUE);

    rtkbt_start_sync_train();

    rtkbt_set_connless_slbc_data(data);
}


static void rtkbt_write_sync_train_param()
{
    if(rtkbt_api_callbacks)
        rtkbt_api_callbacks->btsnd_hcic_write_sync_train_param(sync_train_param_4_3dd);
}

static void rtkbt_set_connless_slbc(BOOLEAN status)
{
    debug("status = %d", status);
    if(rtkbt_api_callbacks) {
        if(status == TRUE)
            connless_slbc_param[0] = 0x01;        //enable
        else
            connless_slbc_param[0] = 0x00;       //disable
        rtkbt_api_callbacks->btsnd_hcic_set_clb(connless_slbc_param);
    }
}

static void rtkbt_start_sync_train()
{
    if(rtkbt_api_callbacks) {
        rtkbt_api_callbacks->btsnd_hcic_start_sync_train();
        mSyncTrainActive = TRUE;
    }
}

static void rtkbt_receive_event_4_3dd(BT_HDR * p_msg)
{
    UINT8   *p = (UINT8 *)(p_msg + 1) + p_msg->offset;
    UINT8   hci_evt_code, hci_evt_len;
    UINT8   data[] = {0x0a, 0x00, 0x00,
                      0x0a, 0x00, 0x80,
                      0x0a, 0x00, 0x00,
                      0x0a, 0x00, 0x80
                      };

    STREAM_TO_UINT8  (hci_evt_code, p);
    STREAM_TO_UINT8  (hci_evt_len, p);

    debug("hci_evt_cod : %d, hci_evt_len : %d", hci_evt_code, hci_evt_len);
    switch (hci_evt_code)
    {
        case HCI_SET_TRIGGERED_CLOCK_CAPTURE_EVT :

            break;

        case HCI_SYNC_TRAIN_CMPL_EVT :
        {

            rtkbt_write_sync_train_param();

            rtkbt_set_connless_slbc(TRUE);

            rtkbt_start_sync_train();

            rtkbt_set_connless_slbc_data(data);
        }
            break;

        case HCI_CONNLESS_SL_BC_TO_EVT :
        // connectionless_broadcast_data_timeout
        // function purpose :
        // 3DG lost sync , notify 3DD to resend sync
            rtkbt_set_connless_slbc(FALSE);
            rtkbt_set_connless_slbc(TRUE);
            rtkbt_write_sync_train_param();
            rtkbt_start_sync_train();
            rtkbt_set_connless_slbc_data(data);
            break;

        case HCI_SL_PAGE_RESPONSE_TO_EVT :
            rtkbt_set_connless_slbc(TRUE);
            rtkbt_write_sync_train_param();
            rtkbt_start_sync_train();
            rtkbt_set_connless_slbc_data(data);
            break;

        default :
            break;
    }
}

/*********************************************************************************
    data = L_open_L|L_open_H|L_open_T|L_colse_L|L_close_H|L_close_T|R_open_L|R_open_H|R_open_T|R_colse_L|R_close_H|R_close_T|
**********************************************************************************/
static void rtkbt_set_connless_slbc_data(UINT8 * data)
{
    rtkbt_api_callbacks->btsnd_hcic_write_clb_data(connless_slbc_data, 0x11);
}

static void rtkbt_frame_sync_toggle(BOOLEAN status)
{
    warn("status : %d, mSyncTrainActive : %d", status, mSyncTrainActive);
    if(status)
        mSyncTrainActive = status;
    else {
        if(status != mSyncTrainActive)
            rtkbt_set_connless_slbc(FALSE);
    }
}


/**********************************************************************************
    **
    **function rtkbt_3dd_Generic_cmd
    **Description If the 3dd APK need start 3dd sync, it will send generic cmd to this module. This
                function only init 3dd.
    **Arguments   id :  RTKBT_3DD_ID
                        cmd : 0:stop 3dd
                        data : null
                        len : 0
   ** return int 1:success 0 : fail
***********************************************************************************/
static int rtkbt_3dd_Generic_cmd(int id, int command, unsigned char * data, int len)
{
    warn("begin to 3dd service command id = %d,command=%d\n",id,command);
    if(id != RTKBT_3DD_ID)
        return 0;
    if(len == 2 && data != NULL)
    {
        info("begin 3dd mode len = %d",len);
        connless_slbc_data[9] = connless_slbc_data[13]= data[0];
        connless_slbc_data[10] = connless_slbc_data[14] = data[1];
        connless_slbc_data[11] = connless_slbc_data[15] = data[0];
        connless_slbc_data[12] = connless_slbc_data[16] = data[1];
    }
    info("3dd sync flow start = %d", command);
    if(command == 1) {
        sleep(1);
        sRunning3DMode = 1;
        rtkbt_start_3dd_service();
    }
    else if(command == 0){
        rtkbt_stop_3dd_service();
        sRunning3DMode = 0;
    }
    else{
       rtkbt_3dg_associated();
    }

    return 1;
}

static void rtkbt_start_3dd_service()
{
    int status;
    BT_HDR * p_data;
    warn("rtkbt_start_3dd_service callback = %d\n",rtkbt_api_callbacks != NULL);
    if(rtkbt_api_callbacks) {
        //disable inquiry scan first
        rtkbt_api_callbacks->BTM_SetDiscoverability(BTM_NON_DISCOVERABLE,0,0);

        //change the device class,it must change before BTM_SetDiscoverability
        UINT8 * p_dev_class = rtkbt_api_callbacks->BTM_ReadDeviceClass();
        rtkbt_api_callbacks->BTM_SetDeviceClass(dev_class_4_3dd);
        memcpy(dev_class_org, p_dev_class, DEV_CLASS_LEN);

        //write eir data
        p_data = (BT_HDR *) rtkbt_api_callbacks->rtkbt_plugin_getbuf((UINT16)(sizeof(BT_HDR) + HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_EXT_INQ_RESP));
        memset(p_data,0,(sizeof(BT_HDR) + HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_EXT_INQ_RESP));
        UINT8 *pp = (UINT8 *)(p_data + 1);
        pp += 4;
        UINT8_TO_STREAM (pp, MANUFACT_LENGTH);
        UINT8_TO_STREAM (pp, 0xFF);
        ARRAY_TO_STREAM (pp, manufact_4_3dd,(MANUFACT_LENGTH -1));
        UINT8_TO_STREAM (pp, 2);      /* Length field */
        UINT8_TO_STREAM (pp, BTM_EIR_TX_POWER_LEVEL_TYPE);
        UINT8_TO_STREAM (pp, eir_tx_power);
        info("1rtkbt_start_3dd_service status = %d", status);
        rtkbt_api_callbacks->btsnd_hcic_write_inqscan_type(0x01);
        rtkbt_api_callbacks->btsnd_hcic_write_pagescan_type(0x01);
        rtkbt_api_callbacks->btsnd_hcic_write_ext_inquiry_response((void *)p_data, 0x00);
        //write general inquiry scan
        status = rtkbt_api_callbacks->BTM_SetDiscoverability(BTM_GENERAL_DISCOVERABLE,HCI_INQ_SCAN_WINDOW,HCI_INQ_SCAN_INTERVAL);
        rtkbt_api_callbacks->btsnd_hcic_write_pagescan_cfg(HCI_PAGE_SCAN_INTERVAL,HCI_PAGE_SCAN_WINDOW);
        rtkbt_api_callbacks->btsnd_hcic_set_reserved_lt_addr(0x01);
        info("rtkbt_start_3dd_service status = %d", status);
    }
}

static void rtkbt_stop_3dd_service()
{
    debug();
    rtkbt_set_connless_slbc(FALSE);
    rtkbt_api_callbacks->BTM_SetDiscoverability(BTM_NON_DISCOVERABLE,0,0);
    rtkbt_api_callbacks->BTM_SetDeviceClass(dev_class_org);
    tBTA_DM_MSG p_data;
    p_data.set_eir_cfg.p_eir_cfg = &eir_cfg_4_3dd;
    rtkbt_api_callbacks->bta_dm_set_eir_config(&p_data);
    rtkbt_api_callbacks->btsnd_hcic_write_scan_enable(0);
}

/**********************************************************************************
    **
    **function rtkbt_3dd_Generic_cmd
    **Description this function  reads the power level used to transmit the FHS and EIR data
                packets. This can be used directly in the Tx Power Level EIR data type
***********************************************************************************/
static void rtkbt_3dd_read_inq_tx_power_cback(void * result)
{
    tBTM_INQ_TXPWR_RESULTS* p_result = (tBTM_INQ_TXPWR_RESULTS* )result;
    if(p_result->status == BTM_SUCCESS) {
        eir_tx_power = p_result->tx_power;
    }
    eir_cfg_4_3dd.bta_dm_eir_inq_tx_power = &eir_tx_power;
}

static int rtkbt_3dd_Hook_func(int event, void * arg, int len)
{
    int i = 0;
    switch(event)
    {
        case RTKBT_HOOK_ENABLE_BT_COMPLETE:          // args: N/A    len:0
            info("RECV HOOK(%d) arg={N/A}", event);
            tBTM_STATUS status;
            //set reserved LT_ADDR
            rtkbt_api_callbacks->btsnd_hcic_set_reserved_lt_addr(0x01);
            rtkbt_api_callbacks->BTM_ReadInquiryRspTxPower((tBTM_CMPL_CB *)rtkbt_3dd_read_inq_tx_power_cback);
            status = rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_3dd_vendor_event, TRUE);
            UINT8 mVendorCmd[7] = {0x00,0x01,0x00,0x64,0x00,0x0A,0x00};
            status = rtkbt_api_callbacks->BTM_VendorSpecificCommand(0xFC91 ,7,mVendorCmd,NULL);
            if(status)
                error("register for vendor event fial, status = %d",status);
            break;
        case RTKBT_HOOK_DISABLE_BT_BEGIN:            // args: N/A    len:0
            warn("RECV HOOK(%d) arg={N/A}", event);
            if(rtkbt_api_callbacks)
                rtkbt_api_callbacks->BTM_RegisterForVSEvents((tBTM_VS_EVT_CB *)rtkbt_process_3dd_vendor_event, FALSE);
            rtkbt_api_callbacks = NULL;
            mSyncTrainActive = FALSE;
            break;
        case RTKBT_HOOK_DISABLE_BT_COMPLETE:         // args: int    len:0
            warn("RECV HOOK(%d) arg={%d}", event, (int)arg);
            break;

        case RTKBT_HOOK_AUTOPAIR_VND_EVENT:
            info("RECV HOOK(%d) arg={N/A}", event);
            break;

        case RTKBT_HOOK_RECEIVE_HCI_VENDOR_EVENT:
            info("RECV HOOK(%d)", event);
            if(sRunning3DMode == 1)
                 rtkbt_receive_event_4_3dd((BT_HDR *) arg);
            break;
        default:
            break;
    }
    return i;
}

/*******************************************************************************
   * **
   * ** Typedef   3dd_service init
   * **
   * ** Description must call first if you need autopair function
   * ** Arguments RTKBT_API_CALLBACK *
   * ** Returns    void
*********************************************************************************/
int init(RTKBT_API_CALLBACK * callbacks, RTKBT_API_INIT_ARG * arg)
{
    if(NULL == callbacks)
        return -1;
    warn("begin to 3dd service init \n");
    if(callbacks->size != sizeof(RTKBT_API_CALLBACK))
    {
        error("size error: ver %08x, size %d\n", callbacks->version, callbacks->size);
        return -2;
    }
    rtkbt_api_callbacks = callbacks;
    rtkbt_api_callbacks->RegisterGenericCommand(RTKBT_3DD_ID, rtkbt_3dd_Generic_cmd);
    rtkbt_api_callbacks->RegisterHook(RTKBT_3DD_ID, rtkbt_3dd_Hook_func);
    return 0;
}

PLUGIN_INIT(init);

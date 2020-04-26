#include "heartbeat.h"
#include "rtkbt_api.h"
#include "rtk_poll.h"

/**********************************************************************************
**  Static variables
**********************************************************************************/

static RTKBT_API_CALLBACK * rtkbt_api_callbacks = NULL;
static const uint32_t DEFALUT_HEARTBEAT_TIMEOUT_MS = 1000; //send a per sercond
int heartBeatLog = 0;
static int heartBeatTimeout= 0;
static bool heartbeatFlag = false;
static int heartbeatCount= 0;
static UINT16 nextSeqNum= 1;
static UINT16 cleanupFlag = 0;

static void rtkbt_heartbeat_cmpl_cback (tBTM_VSC_CMPL *p_params);


static void cleanup();

static void heartbeat_timed_out(union sigval arg)
{
    UNUSED(arg);
    heartbeatCount++;
    if(heartbeatCount >= 3)
    {
        ALOGE("heartbeat_timed_out: heartbeatCount = %d, expected nextSeqNum = %d",heartbeatCount, nextSeqNum);
        ALOGE("heartbeat_timed_out,controller may be suspend! Now restart bluedroid stack\n");
        usleep(1000);
        kill(getpid(), SIGKILL);
        return;
    }
    if(heartbeatFlag)
    {
         rtkbt_api_callbacks->BTM_VendorSpecificCommand(HCI_CMD_VNDR_HEARTBEAT,0,NULL,rtkbt_heartbeat_cmpl_cback);
         poll_timer_flush();
    }
}

static void rtkbt_heartbeat_cmpl_cback (tBTM_VSC_CMPL *p_params)
{
   UINT8  status = 0;
   UINT16 seqnum;
   UINT8  *p = p_params->p_param_buf;
   STREAM_TO_UINT8(status,p);
   STREAM_TO_UINT16(seqnum,p);
   if(status == 0 && seqnum == nextSeqNum)
   {
        nextSeqNum = (seqnum+1);
        heartbeatCount = 0;
   }
   else
   {
        if(cleanupFlag == 1)
        {
            ALOGW("Already cleanup, ignore.");
            return;
        }
        ALOGE("rtkbt_heartbeat_cmpl_cback: Current SeqNum = %d,should SeqNum=%d, status = %d", seqnum, nextSeqNum, status);
        ALOGE("heartbeat event missing:  restart bluedroid stack\n");
        usleep(1000);
        kill(getpid(), SIGKILL);
   }
}

static void rtkbt_heartbeat_beginTimer_func(void)
{
    char * log = rtkbt_api_callbacks->GetConfig(RTKBT_HEARTBEAT_ID, CONFIG_GROUP_HEARTBEAT, CONFIG_ITEM_LOG);
    char * timeout = rtkbt_api_callbacks->GetConfig(RTKBT_HEARTBEAT_ID, CONFIG_GROUP_HEARTBEAT, CONFIG_ITEM_TIMEOUT);
    if(log && timeout)
    {
        heartBeatTimeout= strtol(timeout, NULL, 16);
        heartBeatLog= strtol(log, NULL, 16);
        poll_init(heartbeat_timed_out,heartBeatTimeout);
    }
    else
    {
        heartBeatLog = 0;
        poll_init(heartbeat_timed_out,DEFALUT_HEARTBEAT_TIMEOUT_MS);
    }
    poll_enable(TRUE);
    rtkbt_api_callbacks->BTM_VendorSpecificCommand(HCI_CMD_VNDR_HEARTBEAT,0,NULL,rtkbt_heartbeat_cmpl_cback);
    poll_timer_flush();

}
static int rtkbt_heartbeat_Hook_func(int event, void * arg, int len)
{
    int i = 0;
    if(!rtkbt_api_callbacks) {
        ALOGE("rtkbt_heartbeat_Hook_func: rtkbt_api_callbacks is null \n");
        return i;
    }
    switch(event)
    {
        case RTKBT_HOOK_ENABLE_BT_COMPLETE:          // args: N/A    len:0
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            rtkbt_heartbeat_beginTimer_func();
            break;
        case RTKBT_HOOK_DISABLE_BT_BEGIN:            // args: N/A    len:0
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            cleanup();
            break;
        case RTKBT_HOOK_DISABLE_BT_COMPLETE:         // args: int    len:0
            ALOGI("RECV HOOK(%d) arg={%d}", event, (int)arg);
            break;

        case RTKBT_HOOK_HEARTBEAT_EVENT_COMPLETE:
            ALOGI("RECV HOOK(%d) arg={N/A}", event);
            break;

        default:
            break;
    }
    return i;
}


/*******************************************************************************
   * **
   * ** Typedef   heartbeat init
   * **
   * ** Description must call first if you need heartbeat function
   * ** Arguments RTKBT_API_CALLBACK *
   * ** Returns    void
*********************************************************************************/
static int init(RTKBT_API_CALLBACK * callbacks, RTKBT_API_INIT_ARG * arg)
{
    if(NULL == callbacks)
        return -1;
    ALOGI("begin to heartbeat init \n");
    if(callbacks->size != sizeof(RTKBT_API_CALLBACK))
    {
        ALOGE("size error: ver %08x, size %d\n", callbacks->version, callbacks->size);
        return -2;
    }
    cleanupFlag = 0;
    rtkbt_api_callbacks = callbacks;
    rtkbt_api_callbacks->RegisterHook(RTKBT_HEARTBEAT_ID, rtkbt_heartbeat_Hook_func);
    heartbeatFlag = true;
    heartbeatCount = 0;
    return 0;
}

static void cleanup()
{
    heartbeatFlag = false;
    rtkbt_api_callbacks = NULL;
    cleanupFlag = 1;
    nextSeqNum = 1;
    poll_enable(FALSE);
    poll_cleanup();
}

PLUGIN_INIT(init);



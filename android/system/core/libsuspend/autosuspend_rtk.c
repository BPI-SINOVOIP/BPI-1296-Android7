
//#define LOG_NDEBUG 0

#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cutils/properties.h>

#define LOG_TAG "libsuspend"
#include <cutils/log.h>

#include "autosuspend_ops.h"

#define CTRL_STANDBY    3
#define CTRL_SUSPEND    1

static pthread_t            suspend_thread;
static pthread_mutex_t      suspend_mutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t       suspend_cond    = PTHREAD_COND_INITIALIZER;

static enum {
    SUSPEND_ON,
    SUSPEND_MEM,
}                           suspend_state   = SUSPEND_ON;

static int hasRtkStrongSuspendBlockers()
{
    FILE *fd = fopen("/sys/power/wake_lock","r");
    if(fd != NULL){
        char buffer[512] = {'\0'};
        fread(buffer,1,512,fd);
        fclose(fd);
        ALOGD("wake_lock: %s",buffer);
        if(strstr(buffer,"PowerManagerService.RtkStrongLocks")!=NULL)
            return 1;
    }
    return 0;
}

// added from trunk-7.0 for new xen suspend policy
static void resolve_suspend_ctrl_value(int *pCtrl) {

    // default value is S1
    *pCtrl = CTRL_STANDBY;

    // check xen mode
    char value[PROPERTY_VALUE_MAX];
    memset(value,0x0,PROPERTY_VALUE_MAX);
    property_get("ro.boot.storage",value,"emmc");
    ALOGD("ro.boot.storage %s",value);
    if(strncasecmp(value,"xen",strlen("xen"))==0){
        ALOGD("xen detected, run S3 suspend");
        *pCtrl = CTRL_SUSPEND;
        return;
    }

    // then check property setting
    memset(value,0x0,PROPERTY_VALUE_MAX);
    property_get("persist.rtk.suspend.mode",value,"standby");

    if(strncasecmp(value,"standby",strlen("standby"))==0){
        *pCtrl = CTRL_STANDBY;
    }else{
        *pCtrl = CTRL_SUSPEND;
    }
}

static void *suspend_thread_func(void *arg __attribute__((unused)))
{
    int ret;

    while (1) {
        pthread_cond_wait(&suspend_cond, &suspend_mutex);
        if (suspend_state != SUSPEND_MEM)
            continue;

        // read property and set screen off policy
        char value[PROPERTY_VALUE_MAX];
        memset(value,0x0,PROPERTY_VALUE_MAX);
        property_get("persist.rtk.screenoff.suspend",value,"0");
        int ctrl = atoi(value);

        // system has strong suspend blockers, disable suspend
        if(hasRtkStrongSuspendBlockers() > 0){
            ctrl = 0;
        }

        if(ctrl == 1) {
            // has suspend, choice suspend mode
            int suspendCtrl = CTRL_STANDBY;
            resolve_suspend_ctrl_value(&suspendCtrl);

            switch (suspendCtrl) {
                case CTRL_SUSPEND:
                    property_set("rtk.hwc.suspend.ctl", "1");
                    break;
                case CTRL_STANDBY:
                default:
                    property_set("rtk.hwc.suspend.ctl", "3");
                    break;
            }
        }else{
            property_set("rtk.hwc.suspend.ctl", "-1"); // screen off no suspend
        }

#if 0
        if(ctrl == 1){
            memset(value,0x0,PROPERTY_VALUE_MAX);
            property_get("persist.rtk.suspend.mode",value,"standby");

            if(strncasecmp(value,"standby",strlen("standby"))==0)
                property_set("rtk.hwc.suspend.ctl", "3");
            else
                property_set("rtk.hwc.suspend.ctl", "1");
        }else{
            property_set("rtk.hwc.suspend.ctl", "-1"); // screen off no suspend
        }
#endif

    }
    return NULL;
}

static int autosuspend_rtk_enable(void)
{
    int ret = 0;

    ALOGV("autosuspend_rtk_enable\n");

    suspend_state = SUSPEND_MEM;
    pthread_cond_signal(&suspend_cond);

    ALOGV("autosuspend_rtk_enable done\n");

    return ret;
}

static int autosuspend_rtk_disable(void)
{
    int ret = 0;

    ALOGV("autosuspend_rtk_disable\n");

    suspend_state = SUSPEND_ON;
    pthread_cond_signal(&suspend_cond);

    ALOGV("autosuspend_rtk_disable done\n");

    return ret;
}

struct autosuspend_ops autosuspend_rtk_ops = {
        .enable = autosuspend_rtk_enable,
        .disable = autosuspend_rtk_disable,
};

struct autosuspend_ops *autosuspend_rtk_init(void)
{
    int ret = 0;

    ret = pthread_create(&suspend_thread, NULL, suspend_thread_func, NULL);
    if (ret) {
        ALOGE("Error creating thread\n");
        goto err_pthread_create;
    }

    ALOGI("Selected rtk support\n");
    return &autosuspend_rtk_ops;

err_pthread_create:
    return NULL;
}

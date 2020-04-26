/*
 * Copyright (c) 2014 Realtek Semiconductor Corp.
 */
//#define LOG_NDEBUG 0
//#define LOG_TAG "HdmiRxSvcJni"
#include <utils/Log.h>

#include <jni.h>
#include <JNIHelp.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <linux/videodev2.h>
#include <cutils/properties.h>
#include <media/stagefright/foundation/hexdump.h>

//#include "rcp_api.h"
#include "hwm_binder_api.h"
#define SRAM_DTCPIP_PKEY_ENTRY (0x3E0 >> 3)

#define HDMI_RX_DEV             "/dev/video250"
#define VIDIOC_ENABLE_RX_HDCP   _IOWR('V', BASE_VIDIOC_PRIVATE+2, struct rx_hdcp_keyset)

#define HDMI_RX_HDCP_KEY14      "/tmp/_custom_param_h13_rx"
#define HDMI_RX_HDCP_KEY22      "/tmp/_custom_param_"
#define HDCP_KEY_LEN14          336
#define HDCP_KEY_LEN22          864

#if 0
#define HDCP_AKSV_LEN           5
#define HDCP_PK_LEN             320
#define HDCP_ZERO_LEN           11

struct rx_hdcp_key {
    unsigned char ksv[HDCP_AKSV_LEN];
    unsigned char private_key[HDCP_PK_LEN];
    unsigned char zero[HDCP_ZERO_LEN];
} __attribute__ ((packed));
#endif

struct rx_hdcp_keyset {
    unsigned char key14[HDCP_KEY_LEN14];
    unsigned char key22[HDCP_KEY_LEN22];
} __attribute__ ((packed));

namespace android {

static int setHdmiRxHdcpKeyInternal(const char *type, const char *fn, unsigned char *encbuf, unsigned char *decbuf, size_t len)
{
    int ret = 0;
    int fd = -1;
    int cnt = 0;

    fd = open(fn, O_RDONLY);
    if(fd == -1){
        ALOGE("failed to open HDMI Rx HDCP%s key: %s", type, strerror(errno));
        return 1;
    }

    cnt = read(fd, encbuf, len);
    close(fd);
    if(cnt != (ssize_t)len){
        ALOGE("invalid HDMI Rx HDCP%s key len:%d < %d", type, cnt, len);
        return 1;
    }

#if 1
    //decrypt HDCP key
    //ret = RCP_AES_CwKey_ECB_Cipher(0, SRAM_DTCPIP_PKEY_ENTRY, encbuf, decbuf, len); //dec with cp reg mode
    ret = Binder_CwKey_ECB_Cipher(encbuf, 0, SRAM_DTCPIP_PKEY_ENTRY, decbuf, len);
    if(ret == 0){
        bzero(decbuf, len); //make sure decbuf are 0s on error
        ALOGE("failed to decrypt HDCP%s key", type);
        return 1;
    }

#if 0
    ALOGD("================ encrypted key ==================");
    hexdump(encbuf, len);
    ALOGD("================ decrypted key ==================");
    hexdump(decbuf, len);
#endif
#if 0
    //dump decrypted HDCP key to file
    fd = open("/tmp/gg", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IROTH);
    write(fd, decbuf, len);
    close(fd);
#endif
#else
    //copy decrypted HDCP key directly
    memcpy(decbuf, encbuf, len);
#endif

    return 0;
}

static void realtek_server_RtkHDMIRxService_setHdmiRxHdcpKey(JNIEnv *env, jobject obj)
{
    //if Tx HDCP is not enabled, don't send Rx HDCP key to driver
    char val[PROPERTY_VALUE_MAX];
    property_get("ro.config.enablehdcp", val, "false");
    if(!strcmp(val, "0") || !strcmp(val, "false"))
        return;

    int fd = -1;
    int ret = 0;
    struct rx_hdcp_keyset encKeySet;
    struct rx_hdcp_keyset decKeySet;

    bzero(&encKeySet, sizeof(encKeySet));
    bzero(&decKeySet, sizeof(decKeySet));

    // ================= HDCP 1.4 =================
    if(0 != setHdmiRxHdcpKeyInternal("1.4", HDMI_RX_HDCP_KEY14, encKeySet.key14, decKeySet.key14, HDCP_KEY_LEN14)){
        return;
    }

    // ================= HDCP 2.2 =================
    if(0 != setHdmiRxHdcpKeyInternal("2.2", HDMI_RX_HDCP_KEY22, encKeySet.key22, decKeySet.key22, HDCP_KEY_LEN22)){
        //return;   //it's ok that HDCP 2.2 key is not valid
    }

    fd = open(HDMI_RX_DEV, O_RDWR);
    if(fd == -1){
        ALOGE("failed to open HDMI Rx device:%s (%s)", HDMI_RX_DEV, strerror(errno));
        return;
    }

    ret = ioctl(fd, VIDIOC_ENABLE_RX_HDCP, &decKeySet);
    close(fd);
    if(ret != 0){
        ALOGE("Failed to set HDMI Rx HDCP key: %s", strerror(errno));
        return;
    }

    ALOGD("set HDMI Rx HDPC key successfully!");
}

/*
 * JNI registration.
 */
static JNINativeMethod gMethods[] = {
    /* name, signature, funcPtr */
    { "nativeSetHdmiRxHdcpKey", "()V", (void*) realtek_server_RtkHDMIRxService_setHdmiRxHdcpKey },
};

int register_realtek_server_RtkHDMIRxService(JNIEnv* env)
{
    return jniRegisterNativeMethods(env, "com/realtek/server/RtkHDMIRxService",
            gMethods, NELEM(gMethods));
}

} // namespace android

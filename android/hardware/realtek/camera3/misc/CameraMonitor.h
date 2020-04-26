#ifndef _CAMERA_MONITOR_
#define _CAMERA_MONITOR_

#include <pthread.h>
#include <sysutils/NetlinkEvent.h>
#include "Common.h"
#include "misc.h"

typedef int (*refresh_cameras)();

namespace android_camera_hal3 {

typedef enum {
    HDCP_STATE_OFF,
    HDCP_STATE_ON,
    //HDCP_STATE_1X,
    //HDCP_STATE_2X
}ENUM_HDCP_STATE;

class CameraMonitor {

public:
    static CameraMonitor *Instance();
    virtual ~CameraMonitor();

    void handleSwitchEvent(NetlinkEvent *evt);
    void onInitialize();
    void setCallback(refresh_cameras cb) { mCallback = cb; }
    ENUM_HDCP_STATE getHDCPState() { return mHDCPState; }
    void updateHdcpState();
    void updateHDMITxState();

    // public APIs to allow clients query HDMITx status
    bool getHDMIPlugged() { return mHDMITxPlugged; }
    HDMITX_STATUS * getTxStatus () { return &mHDMITxStatus; }

private:
    CameraMonitor();
    static CameraMonitor *sInstance;

    pthread_t mUeventThread;
    static void *pthread_entry(void *ptr);

    refresh_cameras mCallback;

    ENUM_HDCP_STATE mHDCPState;
    HDMITX_STATUS   mHDMITxStatus;
    bool            mHDMITxPlugged;
    bool            readHDCPStateString(char *buf, size_t size);

};

};

#endif

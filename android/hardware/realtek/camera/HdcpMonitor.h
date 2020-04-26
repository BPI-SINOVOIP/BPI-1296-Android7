/*
 * Copyright 2016 Realtek Semiconductor Corp.
 */
#ifndef _HDCPMONITOR_H_
#define _HDCPMONITOR_H_
#include "NetlinkManager.h"

class HdcpMonitor
{
public:
    static HdcpMonitor *Instance();
    virtual ~HdcpMonitor();

    void handleHdcpSwitchEvent(NetlinkEvent *evt);

    bool getHdcpStatus() {return mHdcpStatus ? true : false; }

    void onInitialize();
protected:

private:
    HdcpMonitor();
    static HdcpMonitor *sInstance;
    int mHdcpStatus;
};

#endif //_HDCPMONITOR_H_

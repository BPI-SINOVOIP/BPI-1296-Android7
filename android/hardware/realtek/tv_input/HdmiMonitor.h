/*
 * Copyright 2015 Realtek Semiconductor Corp.
 */
#ifndef _HDMIMONITOR_H_
#define _HDMIMONITOR_H_
#include "TvInputMonitor.h"
#include "NetlinkManager.h"

// stream id supported by realtek HDMI
#define STREAM_ID_SIDEBAND          0xaa    // use a special number to test TIF
#define STREAM_ID_BUFFER_PRODUCER   0xff

enum {
    COLOR_RGB,
    COLOR_YUV444,
    COLOR_YUV422,
    COLOR_UNKNOWN,
};

class HdmiMonitor : public TvInputMonitor
{
public:
    static HdmiMonitor *Instance();
    virtual ~HdmiMonitor();

    void handleHdmiSwitchEvent(NetlinkEvent *evt);

    bool isHdmiDevice() { return mSrcHdmiRx ? true : false; }
    bool isHdmiReady() { return mSrcReady ? true : false; }
    bool isHdmiProgressive() { return mSrcProgressive ? true : false; }
    int getHdmiWidth() { return mSrcWidth; }
    int getHdmiHeight() { return mSrcHeight; }
    int getHdmiFps() { return mSrcFps; }
    int getHdmiColor() { return mSrcColor; }
    void notifyAvailability(int rxstate, int portid = 0, int streamid = 0);

    virtual void onInitialize();
    virtual int getStreamConfigurations(int device_id, int* num_configurations, const tv_stream_config_t** configs);
protected:

private:
    HdmiMonitor();
    int retrieveHdmiRxVideoInfo();

    static HdmiMonitor *sInstance;

    int mSrcHdmiRx;
    int mSrcReady;
    int mSrcProgressive;
    int mSrcColor;
    int mSrcWidth;
    int mSrcHeight;
    int mSrcFps;

    int mRxState;

    tv_stream_config_t *mConfigs;
};

#endif //_HDMIMONITOR_H_

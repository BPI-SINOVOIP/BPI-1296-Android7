/*
 * Copyright 2015 Realtek Semiconductor Corp.
 */
#ifndef _HDMIHW_H_
#define _HDMIHW_H_

#include "TvInputHw.h"
#include "HdmiMonitor.h"
#include "V4L2Device.h"

class HdmiHw : public TvInputHw
{
public:
    HdmiHw(int device_id, tv_stream_t *stream);
    ~HdmiHw();
    virtual int requestCapture(buffer_handle_t buffer, uint32_t seq);
    virtual int cancelCapture(uint32_t seq);

protected:

private:

    int mDeviceId;
    int mStreamId;
    int mStreamType;
    tv_stream_t *mStreamConfig;
    HdmiMonitor *mMonitor;
    V4L2Device  *mDev;
};

#endif //_HDMIHW_H_

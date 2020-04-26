/*
 * Copyright 2015 Realtek Semiconductor Corp.
 */
#ifndef _TVINPUTMONITOR_H_
#define _TVINPUTMONITOR_H_

#include "TvInputHal.h"

class TvInputMonitor
{
public:
    static TvInputMonitor *Instance(int type);
    TvInputMonitor();
    virtual ~TvInputMonitor() = 0;
    void registerCallback(struct tv_input_device* dev, const tv_input_callback_ops_t* callback, void* data);
    virtual void onInitialize() = 0;
    virtual int getStreamConfigurations(int device_id, int* num_configurations, const tv_stream_config_t** configs) = 0;

protected:
    void notify(tv_input_event_t* event);

private:
    struct tv_input_device* mDev;
    const tv_input_callback_ops_t* mCb;
    void *mData;
};

#endif //_TVINPUTMONITOR_H_

/*
 * Copyright 2015 Realtek Semiconductor Corp.
 */
#ifndef _TVINPUTHW_H_
#define _TVINPUTHW_H_

#include <hardware/tv_input.h>

#include "TvInputHal.h"

class TvInputHw
{
public:
    static TvInputHw *open(int device_id, tv_stream_t *stream, tv_stream_config_t *pConfig);
    static void close(TvInputHw *hw);
    TvInputHw(int device_id, int stream_id);
    virtual ~TvInputHw() = 0;
    virtual int requestCapture(buffer_handle_t buffer, uint32_t seq) = 0;
    virtual int cancelCapture(uint32_t seq) = 0;
    void registerCallback(struct tv_input_device* dev, const tv_input_callback_ops_t* callback, void* data);
    int getDeviceId() { return mDeviceId; }
    int getStreamId() { return mStreamId; }

protected:
    void notify(tv_input_event_t* event);
    static void onCaptureDone(void *hw, buffer_handle_t buffer, uint32_t seq, int result);

private:
    struct tv_input_device* mDev;
    const tv_input_callback_ops_t* mCb;
    void *mData;
    int mDeviceId;
    int mStreamId;
};

#endif //_TVINPUTHW_H_

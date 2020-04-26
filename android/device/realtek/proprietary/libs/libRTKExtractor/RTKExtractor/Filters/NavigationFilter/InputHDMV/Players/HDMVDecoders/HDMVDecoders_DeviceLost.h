//-----------------------------------------------------------------------------
// HDMVDecoders_DeviceLost.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODERS_DEVICELOST_H__
#define __HDMVDECODERS_DEVICELOST_H__

typedef enum
{
    eDeviceLost = 1,
    eDeviceNew = 2,
    eDeviceVideoSize = 3
} DeviceState;

//--------------------------------------------------------------------------
// Device Lost Interface
///
class HDMVDecoders_DeviceLost
{
public:
	virtual void DeviceLost(void)			= 0;
	virtual void DeviceNew(void)			= 0;
};

#endif // __HDMVDECODERS_DEVICELOST_H__

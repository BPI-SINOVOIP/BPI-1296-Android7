/*
 *  dptx_core.h - RTK display port driver header file
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __DPTX_CORE_H__
#define __DPTX_CORE_H__

#include <linux/switch.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/semaphore.h>

#include "dptx_rpc.h"
#include "dptx_hwapi.h"
#include "dptx_edid.h"

#define DPTX_MAX_CLKS 3
#define DPTX_MAX_RESET 4

#define RTK_DPTX_DEBUG 1

#if RTK_DPTX_DEBUG
#define DPTX_DEBUG(format, ...) printk(KERN_ERR "[DPTX_DBG] " format, ## __VA_ARGS__)
#else
#define DPTX_DEBUG(format, ...)
#endif

#define DPTX_ERR(format, ...) printk(KERN_ERR "[DPTX_ERR] " format, ## __VA_ARGS__)

//#define SELF_TEST

enum {
	DPTX_GET_SINK_CAPABILITY,
	DPTX_GET_RAW_EDID,
	DPTX_GET_LINK_STATUS,
	DPTX_GET_VIDEO_CONFIG,
	DPTX_SEND_AVMUTE,
	DPTX_CONFIG_TV_SYSTEM,
	DPTX_CONFIG_AVI_INFO,
	DPTX_SET_FREQUNCY,
	DPTX_SEND_AUDIO_MUTE,
	DPTX_SEND_AUDIO_VSDB_DATA,
	DPTX_SEND_AUDIO_EDID2,
	DPTX_CHECK_TMDS_SRC,	
};

struct rtk_dptx_switch {
	struct switch_dev sw;

	unsigned int hpd_irq;
	unsigned int hpd_gpio;
	unsigned int state;

	struct delayed_work work;
};

struct rtk_dptx_device {
	struct rtk_dptx_hwinfo hwinfo;

	struct clk *clks[DPTX_MAX_CLKS];
	struct reset_control *rstc[DPTX_MAX_RESET];
	
	unsigned int dptx_irq;
	unsigned int power_state;
	unsigned int ignore_edid;

	struct device *dev;
	struct miscdevice miscdev;

	struct rtk_dptx_switch swdev;
	struct ion_client *rpc_ion_client;

	asoc_dptx_t cap;
};

int register_dptx_switch(struct rtk_dptx_device *dptx_dev);
int dptx_switch_get_state(struct rtk_dptx_switch *swdev);
int rtk_dptx_switch_suspend(struct rtk_dptx_device *dptx_dev);
int rtk_dptx_switch_resume(struct rtk_dptx_device *dptx_dev);
#endif  //__DPTX_CORE_H__

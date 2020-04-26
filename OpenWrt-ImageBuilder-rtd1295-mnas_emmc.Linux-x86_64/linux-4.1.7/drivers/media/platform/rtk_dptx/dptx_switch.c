/*
 *  dptx_switch.c - RTK display port driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/of_device.h>

#include "dptx_core.h"

#define DPTX_SWITCH_NAME "dptx"

static irqreturn_t rtk_hpd_isr(int irq, void *dev_id)
{
	struct rtk_dptx_switch *swdev = (struct rtk_dptx_switch *)dev_id;

	schedule_delayed_work(&swdev->work, 1);

	return IRQ_HANDLED;
}

static ssize_t dptx_switch_print_state(struct switch_dev *sdev, char *buffer)
{
	struct rtk_dptx_switch *swdev = container_of(sdev, struct rtk_dptx_switch, sw);

	return sprintf(buffer, "%d", swdev->state);
}

int dptx_switch_get_state(struct rtk_dptx_switch *swdev)
{
	swdev->state = gpio_get_value(swdev->hpd_gpio);
	return swdev->state;
}

static void dptx_switch_work_func(struct work_struct *work)
{
	struct rtk_dptx_switch *swdev = container_of(work, struct rtk_dptx_switch, work.work);
	struct rtk_dptx_device *dptx_dev = container_of(swdev, struct rtk_dptx_device, swdev);
	int ret;
	unsigned char *block;
	struct edid *edid;

	swdev->state = gpio_get_value(swdev->hpd_gpio);
	if(swdev->state!=switch_get_state(&swdev->sw)) {
		DPTX_ERR("%s\n", swdev->state?"plugged in":"pulled out");
//		switch_set_state(&swdev->sw, swdev->state);

		if(swdev->state) {
			if(!dptx_dev->ignore_edid) {
				ret = ReadEDID(&dptx_dev->hwinfo, dptx_dev->cap.edid_ptr, 256);
				if(ret<0) {
					printk(KERN_ERR "Read EDID fail\n");
					return;
				}
			}
			block = dptx_dev->cap.edid_ptr;
			dptx_dev->cap.sink_cap_available = true;

			edid = (struct edid *)dptx_dev->cap.edid_ptr;
			memset(&dptx_dev->cap.sink_cap, 0, sizeof(struct sink_capabilities_t));
			dptx_add_edid_modes(edid, &dptx_dev->cap.sink_cap);
			dptx_edid_to_eld(edid, &dptx_dev->cap.sink_cap);
#ifdef SELF_TEST
			dptx_config_tv_system(&dptx_dev->hwinfo);
#endif
		}

		switch_set_state(&swdev->sw, swdev->state);
	}
}

int rtk_dptx_switch_suspend(struct rtk_dptx_device *dptx_dev)
{
	struct rtk_dptx_switch *swdev;

	swdev = &dptx_dev->swdev;

	disable_irq(swdev->hpd_irq);

	swdev->state = 0;
	switch_set_state(&swdev->sw, swdev->state);

	return 0;
}

int rtk_dptx_switch_resume(struct rtk_dptx_device *dptx_dev)
{
	struct rtk_dptx_switch *swdev;

	swdev = &dptx_dev->swdev;
	enable_irq(swdev->hpd_irq);
	schedule_delayed_work(&swdev->work, 10);

	return 0;
}

int register_dptx_switch(struct rtk_dptx_device *dptx_dev)
{
	struct rtk_dptx_switch *swdev;
	struct device_node *hpd;
	int ret;

	swdev = &dptx_dev->swdev;
	swdev->sw.name = DPTX_SWITCH_NAME;
	swdev->sw.print_state = dptx_switch_print_state;
	ret = switch_dev_register(&swdev->sw);
	if(ret < 0)
		return -1;

	INIT_DELAYED_WORK(&swdev->work, dptx_switch_work_func);

	hpd = of_get_child_by_name(dptx_dev->dev->of_node, "dp_hpd");
	
	swdev->hpd_gpio = of_get_gpio(hpd, 0);
	if(swdev->hpd_gpio < 0) {
		DPTX_ERR("[%s] could not get gpio from of \n", __FUNCTION__);
		return -1;
	}
	if(gpio_request(swdev->hpd_gpio, hpd->name)) {
		DPTX_ERR("[%s] gpio request fail\n", __FUNCTION__);
		return -1;
	}
	gpio_direction_input(swdev->hpd_gpio);
	gpio_set_debounce(swdev->hpd_gpio, 30*1000);

	swdev->hpd_irq = irq_of_parse_and_map(hpd, 0);
	if(!swdev->hpd_irq) {
		DPTX_ERR("[%s] hot plug gpio irq request fail\n", __FUNCTION__);
		return -1;
	}
	irq_set_irq_type(swdev->hpd_irq, IRQ_TYPE_EDGE_BOTH);
	if(request_irq(swdev->hpd_irq, rtk_hpd_isr, IRQF_SHARED, "DP_HPD", swdev)) {
		DPTX_ERR("[%s] request irq fail\n", __FUNCTION__);
		return -1;
	}
		
	switch_set_state(&swdev->sw, 0);
	schedule_delayed_work(&swdev->work, 200);

	return 0;
}

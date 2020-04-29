/*
 *  Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __REALTEK_THERMAL_SENSOR_H__
#define __REALTEK_THERMAL_SENSOR_H__

/* Thermal sensor base address
 *   0x9801D150 (scpu wrapper)
 *   0x980124B4 (emmc wrapper)
 */
#define TM_SENSOR_CTRL0    0x00
#define TM_SENSOR_CTRL1    0x04
#define TM_SENSOR_CTRL2    0x08
#define TM_SENSOR_CTRL3    0x0C
#define TM_SENSOR_CTRL4    0x10
#define TM_SENSOR_CTRL5    0x14
#define TM_SENSOR_STATUS1  0x18
#define TM_SENSOR_STATUS2  0x1C

#include <linux/io.h>
#include <linux/delay.h>
#include <linux/bitops.h>

static inline int _SIGN_EXT(int sign_bit, unsigned int val)
{
	return ((int)(((unsigned int)0 - (BIT(sign_bit) & (val))) | (val)));
}

static inline void thermal_sensor_dump_reg(void * reg, const char *name)
{
	pr_err("%s/TM_SENSOR_STATUS1 = %08x\n", name, readl(reg + TM_SENSOR_STATUS1));
	pr_err("%s/TM_SENSOR_STATUS2 = %08x\n", name, readl(reg + TM_SENSOR_STATUS2));
}

static inline int thermal_sensor_check_status_error(void * reg)
{
	unsigned int val = readl(reg + TM_SENSOR_STATUS2);
	return val == 0 || val == 0x3FFFFF;
}

static inline void thermal_sensor_hw_reset_no_delay(void * reg)
{
	writel(0x01904001, reg + TM_SENSOR_CTRL2);
	writel(0x01924001, reg + TM_SENSOR_CTRL2);
}

static inline void thermal_sensor_hw_reset(void * reg)
{
	thermal_sensor_hw_reset_no_delay(reg);
	usleep_range(25000, 26000); // delay 25 ms
}

static inline int thermal_sensor_get_temp(void * reg)
{
	unsigned int val = readl(reg + TM_SENSOR_STATUS1);
	return _SIGN_EXT(18, val) * 1000 / 1024;
}

#endif /* __REALTEK_THERMAL_SENSOR_H__ */

/*
 * rtk-regulator.h - Realtek Regulator Common
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __RTK_REGULATOR_H
#define __RTK_REGULATOR_H

#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>

struct regmap_field;

enum {
	RRF_ON = 0,
	RRF_NMODE,
	RRF_SMODE,
	RRF_NVO,
	RRF_SVO,
	RRF_MAX
};

#define RTK_REGULATOR_TIME_SEL_VOLT_UP   0x1
#define RTK_REGULATOR_SET_SVOLT_SMODE    0x2

struct rtk_desc {
	struct regulator_desc desc;
	unsigned int flags;
	struct regmap_field *fields[RRF_MAX];

	int smode_enable_val;
	int smode_disable_val;
	int svo_smode_val;

	struct regulator_state state_mem;
	struct regulator_state state_coldboot;
	struct regulator_dev *rdev;
	int volup_delay;
};

static inline const char *rtk_desc_name(struct rtk_desc *gd)
{
	return gd->desc.name;
}

#define to_rtk_desc(_desc) container_of(_desc, struct rtk_desc, desc)

int rtk_regulator_of_parse_cb(struct device_node *np,
	const struct regulator_desc *desc, struct regulator_config *config);
int rtk_regulator_set_voltage_time_sel(struct regulator_dev *rdev,
	unsigned int old_selector, unsigned int new_selector);
int rtk_regulator_set_suspend_enable(struct regulator_dev *rdev);
int rtk_regulator_set_suspend_disable(struct regulator_dev *rdev);
int rtk_regulator_set_suspend_voltage(struct regulator_dev *rdev, int uV);

void rtk_regulator_prepare_suspend_state(struct regulator_dev *rdev,
	int is_coldboot);

#endif


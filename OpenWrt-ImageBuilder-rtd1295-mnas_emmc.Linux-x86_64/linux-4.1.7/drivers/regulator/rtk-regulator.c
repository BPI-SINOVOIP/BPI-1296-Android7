/*
 * rtk-regulator.c - Realtek Regulator Common
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/of.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>

#include <soc/realtek/rtk_regmap.h>
#include "rtk-regulator.h"

int rtk_regulator_of_parse_cb(struct device_node *np,
	const struct regulator_desc *desc, struct regulator_config *config)
{
	struct rtk_desc *gd = config->driver_data;
	struct device_node *child;
	unsigned int val;

	child = of_get_child_by_name(np, "regulator-state-coldboot");
	if (!child)
		child = of_get_child_by_name(np, "regulator-state-mem");
	if (child) {
		struct regulator_state *state = &gd->state_coldboot;

		if (of_property_read_bool(child, "regulator-on-in-suspend"))
			state->enabled = true;

		if (of_property_read_bool(child, "regulator-off-in-suspend"))
			state->disabled = true;

		if (!of_property_read_u32(child, "regulator-suspend-microvolt",
			&val))
			state->uV = val;
	}

	return 0;
}

int rtk_regulator_set_voltage_time_sel(struct regulator_dev *rdev,
	unsigned int old_selector, unsigned int new_selector)
{
	struct rtk_desc *gd = to_rtk_desc(rdev->desc);

	if (new_selector > old_selector)
		return gd->volup_delay;
	return 0;
}

int rtk_regulator_set_suspend_enable(struct regulator_dev *rdev)
{
	struct rtk_desc *gd = to_rtk_desc(rdev->desc);

	dev_info(rdev_get_dev(rdev), "%s\n", __func__);
	regmap_field_write(gd->fields[RRF_SMODE], gd->smode_enable_val);
	return 0;
}

int rtk_regulator_set_suspend_disable(struct regulator_dev *rdev)
{
	struct rtk_desc *gd = to_rtk_desc(rdev->desc);

	dev_info(rdev_get_dev(rdev), "%s\n", __func__);
	regmap_field_write(gd->fields[RRF_SMODE], gd->smode_disable_val);
	return 0;
}

int rtk_regulator_set_suspend_voltage(struct regulator_dev *rdev,
	int uV)
{
	struct rtk_desc *gd = to_rtk_desc(rdev->desc);
	int vsel;

	dev_info(rdev_get_dev(rdev), "%s\n", __func__);

	vsel = regulator_map_voltage_iterate(gd->rdev, uV, uV);
	if (vsel < 0)
		return -EINVAL;

	if (gd->flags & RTK_REGULATOR_SET_SVOLT_SMODE) {
		if (!gd->fields[RRF_SMODE])
			return -EINVAL;
		regmap_field_write(gd->fields[RRF_SMODE], gd->svo_smode_val);
	}

	if (!gd->fields[RRF_SVO])
		return -EINVAL;
	regmap_field_write(gd->fields[RRF_SVO], vsel);

	return 0;
}

void rtk_regulator_prepare_suspend_state(struct regulator_dev *rdev,
	int is_coldboot)
{
	struct rtk_desc *gd = to_rtk_desc(rdev->desc);
	struct regulator_state *state;

	state = &gd->rdev->constraints->state_mem;
	*state = is_coldboot ? gd->state_coldboot : gd->state_mem;
}


/*
 * sy8089a-regulator.c - Silergy Sy8089a Regulator Driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/pwm.h>

struct sy8089a_priv {
	struct pwm_device *pwm;
	unsigned int sel;
};

static unsigned int volt_tabl[] = {
	1100000,
	1088000, 1070000, 1050000, 1030000,
	1016000,  998000,  980000,  962000,
	 943000,  925000,  907000,  888000,
	 870000,  850000,  833000,  816000,
	 800000,
};
static unsigned int vol_to_duty[] = {
	0,
	76, 152, 227, 303,
	378, 454, 530, 605,
	668, 744, 819, 895,
	971, 1046, 1122, 1197,
	1260
};

static int sy8089a_get_voltage_sel(struct regulator_dev *rdev)
{
	struct sy8089a_priv *priv = rdev_get_drvdata(rdev);

	return priv->sel;
}

static int sy8089a_set_voltage_sel(struct regulator_dev *rdev, unsigned sel)
{
	struct sy8089a_priv *priv = rdev_get_drvdata(rdev);
	unsigned int period;
	unsigned int duty = 0;

	if (sel == 0) {
		duty = 0;
		period = 75;
	} else {
		period = 1260;
		duty =  vol_to_duty[sel];
	}

	priv->sel = sel;
	pwm_config(priv->pwm, duty, period);

	return 0;
}

static struct regulator_ops sy8089a_ops  = {
	.list_voltage    = regulator_list_voltage_table,
	.map_voltage     = regulator_map_voltage_iterate,
	.set_voltage_sel = sy8089a_set_voltage_sel,
	.get_voltage_sel = sy8089a_get_voltage_sel,
};

struct regulator_desc desc = {
	.owner = THIS_MODULE,
	.name = "pwm-reg-0",
	.id = 0,
	.ops = &sy8089a_ops,
	.type = REGULATOR_VOLTAGE,
	.of_match = "pwm-reg-0",

	.n_voltages  = ARRAY_SIZE(volt_tabl),
	.volt_table  = volt_tabl,

};

static int sy8089a_probe(struct platform_device *pdev)
{
	struct sy8089a_priv *priv;
	struct regulator_config cfg = {};
	struct regulator_dev *rdev;
	int ret;

	pr_info("%s: begin\n", __func__);

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->pwm = devm_of_pwm_get(&pdev->dev, pdev->dev.of_node, NULL);
	if (IS_ERR(priv->pwm)) {
		ret = PTR_ERR(priv->pwm);
		dev_err(&pdev->dev, "Failed to get pwm: %d\n", ret);
		return ret;
	}

	pwm_config(priv->pwm, 0, pwm_get_period(priv->pwm));

	cfg.dev = &pdev->dev;
	cfg.driver_data = priv;
	rdev = devm_regulator_register(&pdev->dev, &desc, &cfg);

	if (IS_ERR(rdev)) {
		ret = PTR_ERR(rdev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", ret);
		return ret;
	}

	pr_info("%s: done\n", __func__);

	return 0;
}

static const struct of_device_id sy8089a_ids[] = {
	{.compatible = "silergy,sy8089a" },
	{}
};

static struct platform_driver sy8089a_driver = {
	.probe  = sy8089a_probe,
	.driver = {
		.name   = "sy8089a",
		.owner  = THIS_MODULE,
		.of_match_table = of_match_ptr(sy8089a_ids),
	},
};
module_platform_driver_probe(sy8089a_driver, sy8089a_probe);


/*
 * cgc-rtd129x.c - RTD129x clock-gate controller
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define pr_fmt(fmt) "clk: " fmt

#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/bitops.h>
#include <linux/pm.h>
#include <linux/suspend.h>
#include <soc/realtek/rtk_mmio.h>
#include "common.h"
#include "clk-mmio-gate.h"

#define MAX_CLOCK_GATES  32

struct cgc_desc {
	struct device *dev;
	const char *names[MAX_CLOCK_GATES];
	int num_names;

	unsigned int pm_mask;
	unsigned int pm_data;

	struct clk_reg_init_data init_data;
	struct clk_onecell_data clk_data;
};

static int init_clk_gates(struct cgc_desc *cgcd)
{
	int i;
	struct clk_onecell_data *data = &cgcd->clk_data;
	struct device_node *np = cgcd->dev->of_node;

	data->clks = kcalloc(cgcd->num_names, sizeof(struct clk *),
		GFP_KERNEL);
	if (!data->clks)
		return -ENOMEM;
	data->clk_num = cgcd->num_names;

	for (i = 0; i < cgcd->num_names; i++) {
		struct clk_mmio_gate *gate;
		const char *name = cgcd->names[i];
		struct clk_init_data init = { 0 };

		if (!name || !name[0])
			continue;

		init.name = name;
		init.ops = &clk_mmio_gate_ops;
		init.flags = CLK_IS_BASIC;
		if (of_property_match_string(np, "ignore-unused-clocks",
			name) >= 0) {
			init.flags |= CLK_IGNORE_UNUSED;
		}

		gate = kzalloc(sizeof(*gate), GFP_KERNEL);
		gate->bit_idx = i;
		gate->base.hw.init = &init;
		clk_reg_init(&gate->base, &cgcd->init_data);

		data->clks[i] = clk_register(NULL, &gate->base.hw);
		if (IS_ERR(data->clks[i])) {
			dev_warn(cgcd->dev, "failed to register %s, %ld\n",
				name, PTR_ERR(data->clks[i]));
			data->clks[i] = NULL;
			continue;
		}

		clk_register_clkdev(data->clks[i], name, NULL);

		if (of_property_match_string(np, "ignore-pm-clocks",
			name) >= 0) {
			continue;
		}

		/* add to pm_list */
		cgcd->pm_mask |= BIT(i);
	}

	return of_clk_add_provider(np, of_clk_src_onecell_get, data);
}

#if CONFIG_PM
static int rtk_cgc_suspend(struct device *dev)
{
	struct cgc_desc *cgcd = dev_get_drvdata(dev);
	struct clk_reg clk_reg;

#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		return 0;
#endif
	dev_info(dev, "[CLK] Enter %s\n", __func__);

	/* create a clk_reg for read/write reg */
	clk_reg_init(&clk_reg, &cgcd->init_data);
	cgcd->pm_data = clk_reg_read(&clk_reg, 0);

	dev_info(dev, "[CLK] Exit %s\n", __func__);
	return 0;
}

static int rtk_cgc_resume(struct device *dev)
{
	struct cgc_desc *cgcd = dev_get_drvdata(dev);
	struct clk_reg clk_reg;

#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		return 0;
#endif
	dev_info(dev, "[CLK] Enter %s\n", __func__);

	clk_reg_init(&clk_reg, &cgcd->init_data);
	clk_reg_update(&clk_reg, 0, cgcd->pm_mask, cgcd->pm_data);

	dev_info(dev, "[CLK] Exit %s\n", __func__);
	return 0;
}
#else /* !CONFIG_PM */
#define rtk_cgc_suspend NULL
#define rtk_cgc_resume NULL
#endif

static const struct dev_pm_ops rtk_cgc_pm_ops = {
	.suspend_noirq = rtk_cgc_suspend,
	.resume_noirq = rtk_cgc_resume,
};

static int rtk_cgc_probe(struct platform_device *pdev)
{
	struct cgc_desc *cgcd;
	void __iomem *reg;
	struct regmap *regmap;
	int offset = 0;

	dev_info(&pdev->dev, "[CLK] %s\n", __func__);

	cgcd = kzalloc(sizeof(*cgcd), GFP_KERNEL);
	if (!cgcd)
		return -EINVAL;

	reg = of_iomap(pdev->dev.of_node, 0);
	regmap = of_get_rtk_mmio_regmap_with_offset(pdev->dev.of_node, 0,
		&offset);
	if (!reg && IS_ERR_OR_NULL(regmap))
		return -EINVAL;
	if (IS_ERR(regmap))
		regmap = NULL;

	cgcd->num_names = of_property_count_strings(pdev->dev.of_node,
		"clock-output-names");
	if (cgcd->num_names < 0)
		return cgcd->num_names;
	of_property_read_string_array(pdev->dev.of_node, "clock-output-names",
		cgcd->names, cgcd->num_names);

	cgcd->dev = &pdev->dev;
	cgcd->init_data.reg = reg;
	if (regmap) {
		dev_info(&pdev->dev, "use mmio regmap\n");
		cgcd->init_data.offset = offset;
		cgcd->init_data.regmap = regmap;
	}

	platform_set_drvdata(pdev, cgcd);

	init_clk_gates(cgcd);

	return 0;
}

static const struct of_device_id rtk_cgc_match[] = {
	{.compatible = "realtek,clock-gate-controller"},
	{}
};

static struct platform_driver rtk_cgc_driver = {
	.probe = rtk_cgc_probe,
	.driver = {
		.name = "rtk-cgc",
		.of_match_table = rtk_cgc_match,
		.pm = &rtk_cgc_pm_ops,
	},
};

static int __init rtk_cgc_init(void)
{
	return platform_driver_register(&rtk_cgc_driver);
}
core_initcall(rtk_cgc_init);

/*
 * reset-rtk.c - Realtek reset controller & reset control
 *
 * Copyright (C) 2016-2017 Realtek Semiconductor Corporation
 * Copyright (C) 2016-2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/regmap.h>
#include <linux/reset.h>
#include <linux/reset-controller.h>
#include <linux/reset-helper.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <soc/realtek/rtk_mmio.h>
#include <dt-bindings/reset/rtk,reset.h>

static DEFINE_SPINLOCK(rtk_reset_async_lock);
static LIST_HEAD(rtk_reset_async_list);

#define RESET_ASYNC  BIT(0)
#define RESET_NO_PM  BIT(1)

struct reset_priv {
	struct reset_controller_dev rcdev;
	struct regmap *regmap;
	int offset;
	unsigned int flags;

	unsigned int async_data;
	unsigned int async_group;
	struct list_head async_list;

	unsigned int pm_data;
	unsigned int pm_ignore_bits;
};

#define to_reset_priv(_p) container_of((_p), struct reset_priv, rcdev)

static int rtk_reset_assert(struct reset_controller_dev *rcdev,
		unsigned long id)
{
	struct reset_priv *priv = to_reset_priv(rcdev);
	int offset = priv->offset;

	if (priv->flags & RESET_ASYNC) {
		if (id == RTK_RSTN_DO_SYNC)
			return -EINVAL;
	}

	regmap_update_bits(priv->regmap, offset, BIT(id), 0);

	return 0;
}

static void rtk_reset_deassert_async(unsigned int group)
{
	struct reset_priv *p;

	list_for_each_entry(p, &rtk_reset_async_list, async_list) {
		if (p->async_group != group)
			continue;

		if (!p->async_data)
			continue;

		regmap_update_bits(p->regmap, p->offset, p->async_data,
			p->async_data);
		p->async_data = 0;
	}
}

static int rtk_reset_deassert(struct reset_controller_dev *rcdev,
		unsigned long id)
{
	struct reset_priv *priv = to_reset_priv(rcdev);
	unsigned int mask;
	unsigned int offset = priv->offset;
	unsigned long flags;

	if (priv->flags & RESET_ASYNC) {
		spin_lock_irqsave(&rtk_reset_async_lock, flags);
		if (id == RTK_RSTN_DO_SYNC)
			rtk_reset_deassert_async(priv->async_group);
		else
			priv->async_data |= BIT(id);
		spin_unlock_irqrestore(&rtk_reset_async_lock, flags);
	} else {
		mask = BIT(id);
		regmap_update_bits(priv->regmap, offset, mask, mask);
	}

	return 0;
}

static int rtk_reset_reset(struct reset_controller_dev *rcdev,
		unsigned long id)
{
	int ret;

	ret = rtk_reset_assert(rcdev, id);
	if (ret)
		return ret;

	return rtk_reset_deassert(rcdev, id);
}

static int rtk_reset_status(struct reset_controller_dev *rcdev,
		unsigned long id)
{
	struct reset_priv *priv = to_reset_priv(rcdev);
	unsigned int offset = 0, val;
	int ret;

	ret = regmap_read(priv->regmap, offset, &val);
	return !(val & BIT(id));
}

static struct reset_control_ops rtk_reset_ops = {
	.assert   = rtk_reset_assert,
	.deassert = rtk_reset_deassert,
	.reset    = rtk_reset_reset,
	.status   = rtk_reset_status,
};

static int rtk_reset_of_xlate(struct reset_controller_dev *rcdev,
		const struct of_phandle_args *reset_spec)
{
	struct reset_priv __maybe_unused *priv = to_reset_priv(rcdev);
	int id;

	if (reset_spec->args_count != 1)
		return -EINVAL;

	id = (int)reset_spec->args[0];
	return id;
}

static int rtk_reset_parse_dt(struct device_node *np, struct reset_priv *priv)
{
	int val;

	if (!of_property_read_u32(np, "async-group", &val)) {
		priv->flags |= RESET_ASYNC | RESET_NO_PM;
		priv->async_group = val;
		list_add(&priv->async_list, &rtk_reset_async_list);
	}

	if (of_find_property(np, "no-pm", NULL))
		priv->flags |= RESET_NO_PM;

	if (!of_property_read_u32(np, "boot-clear-bits", &val))
		regmap_update_bits(priv->regmap, priv->offset, val, 0);

	of_property_read_u32(np, "pm-ignore-bits", &priv->pm_ignore_bits);

	return 0;
}

static int rtk_reset_suspend(struct device *dev)
{
	struct reset_priv *priv = dev_get_drvdata(dev);

	if (priv->flags & RESET_NO_PM)
		return 0;

#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		return 0;
#endif
	dev_info(dev, "[RESET] Enter %s\n", __func__);

	regmap_read(priv->regmap, priv->offset,	&priv->pm_data);

	dev_info(dev, "[RESET] Exit %s\n", __func__);
	return 0;
}

static int rtk_reset_resume(struct device *dev)
{
	struct reset_priv *priv = dev_get_drvdata(dev);

	if (priv->flags & RESET_NO_PM)
		return 0;

#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		return 0;
#endif
	dev_info(dev, "[RESET] Enter %s\n", __func__);

	regmap_update_bits(priv->regmap, priv->offset, ~priv->pm_ignore_bits,
		priv->pm_data);

	dev_info(dev, "[RESET] Exit %s\n", __func__);
	return 0;
}

static const struct dev_pm_ops rtk_reset_pm_ops = {
	.suspend_noirq = rtk_reset_suspend,
	.resume_noirq = rtk_reset_resume,
};

static int rtk_reset_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct reset_priv *priv;
	struct regmap *regmap;
	int offset = 0;
	int ret;
	unsigned int val;

	dev_info(&pdev->dev, "[RESET] %s\n", __func__);

	priv = devm_kzalloc(&pdev->dev, sizeof(struct reset_priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	regmap = of_get_rtk_mmio_regmap_with_offset(np, 0, &offset);
	if (IS_ERR_OR_NULL(regmap))
		return -EINVAL;

	priv->regmap = regmap;
	priv->offset = offset;

	priv->rcdev.owner = THIS_MODULE;
	priv->rcdev.ops = &rtk_reset_ops;
	priv->rcdev.of_node = np;
	priv->rcdev.of_reset_n_cells = 1;
	priv->rcdev.of_xlate = rtk_reset_of_xlate;

	rtk_reset_parse_dt(np, priv);

	ret = reset_controller_register(&priv->rcdev);
	if (ret)
		return ret;

	if (!of_property_read_u32(np, "boot-clear-bits", &val)) {
		 regmap_update_bits(regmap, offset, val, 0);
	}

	platform_set_drvdata(pdev, priv);

	return ret;
}

static const struct of_device_id rtk_reset_match[] = {
	{.compatible = "realtek,reset-controller"},
	{}
};

static struct platform_driver rtk_reset_driver = {
	.probe = rtk_reset_probe,
	.driver = {
		.name   = "rtk-reset",
		.of_match_table = rtk_reset_match,
		.pm = &rtk_reset_pm_ops,
	},
};

static int __init rtk_reset_init(void)
{
	return platform_driver_register(&rtk_reset_driver);
}
core_initcall(rtk_reset_init);

/*
 * deprecated api
 */
static struct device_node *__rcp_np;

struct reset_control *rstc_get(const char *name)
{
	struct reset_control *rstc = ERR_PTR(-EINVAL);
	struct device_node *child;

	pr_notice("Deprecated API rstc_get is used by %s, PLEASE use of_reset_control_get\n",
		name);

	if (!__rcp_np)
		return ERR_PTR(-ENOENT);

	for_each_child_of_node(__rcp_np, child) {
		rstc = of_reset_control_get(child, name);
		if (!IS_ERR(rstc))
			return rstc;
	}

	pr_err("Failed to get rstc for %s\n", name);
	return rstc;
}
EXPORT_SYMBOL(rstc_get);

static const struct of_device_id rtk_rcp_match[] = {
	{.compatible = "realtek,reset-control-provider"},
	{}
};

static int rtk_rcp_probe(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "[RESET] deprecated API OK\n");
	__rcp_np = pdev->dev.of_node;
	return 0;
}

static struct platform_driver rtk_rcp_driver = {
	.probe = rtk_rcp_probe,
	.driver = {
		.name   = "rtk-rcp",
		.of_match_table = rtk_rcp_match,
	},
};

static int __init rtk_rcp_init(void)
{
	return platform_driver_register(&rtk_rcp_driver);
}
core_initcall(rtk_rcp_init);


/*
 * Copyright (C) 2017, Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/uio_driver.h>

struct tp_uio_info {
	struct uio_info info;
	struct device *dev;
	struct clk *clk;
};

static int rtk_tp_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int ret;
	struct resource res;
	struct tp_uio_info *priv;
	struct uio_info *info;

	dev_info(dev, "[TP] %s\n", __func__);

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	info = &priv->info;

	priv->clk = clk_get(dev, NULL);
	if (IS_ERR_OR_NULL(priv->clk)) {
		dev_warn(dev, "failed to get clk: %ld\n", PTR_ERR(priv->clk));
		priv->clk = NULL;
	}

	ret = of_address_to_resource(pdev->dev.of_node, 0, &res);
	if (ret) {
		dev_err(dev, "failed to get resource\n");
		goto free_mem;
	}

	info->mem[0].addr = res.start;
	info->mem[0].size = ALIGN(resource_size(&res), PAGE_SIZE);
	info->mem[0].memtype = UIO_MEM_PHYS;

	info->version = "0.0.1";
	info->name = "rtk-tp";
#ifdef CONFIG_UIO_ASSIGN_MINOR
	info->minor = 249;
#endif
	clk_prepare_enable(priv->clk);

	ret = uio_register_device(dev, info);
	if (ret) {
		dev_err(dev, "failed to register uio device\n");
		goto free_mem;
	}

	platform_set_drvdata(pdev, info);
	return 0;
free_mem:
	kfree(info);
	return ret;

}

static int rtk_tp_remove(struct platform_device *pdev)
{
	struct uio_info *info = platform_get_drvdata(pdev);
	struct tp_uio_info *priv = container_of(info, struct tp_uio_info, info);
	struct device *dev = &pdev->dev;

	dev_info(dev, "[TP] %s\n", __func__);

	clk_disable_unprepare(priv->clk);
	clk_put(priv->clk);

	uio_unregister_device(info);
	platform_set_drvdata(pdev, NULL);
	kfree(info);

	return 0;
}

static const struct of_device_id rtk_tp_ids[] = {
	{ .compatible = "realtek,tp" },
	{}
};

static struct platform_driver rtk_tp_driver = {
	.probe		= rtk_tp_probe,
	.remove		= rtk_tp_remove,
	.driver		= {
		.name	= "rtk-tp",
		.owner	= THIS_MODULE,
		.of_match_table = rtk_tp_ids,
	},
};
module_platform_driver_probe(rtk_tp_driver, rtk_tp_probe);

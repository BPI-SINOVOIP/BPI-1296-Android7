/*
 * Copyright (C) 2015-2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/uio_driver.h>

static int rtk_rbus_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int ret;
	struct resource res;
	struct uio_info *info;

	dev_info(dev, "[RBUS] %s\n", __func__);

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	ret = of_address_to_resource(pdev->dev.of_node, 0, &res);
	if (ret) {
		dev_err(dev, "failed to get resource\n");
		goto free_mem;
	}


	info->mem[0].addr = res.start;
	info->mem[0].size = ALIGN(resource_size(&res), PAGE_SIZE);
	info->mem[0].memtype = UIO_MEM_PHYS;

	info->version = "0.0.1";
	info->name = "rtk-rbus";
#ifdef CONFIG_UIO_ASSIGN_MINOR
	info->minor = 250;
#endif

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

static int rtk_rbus_remove(struct platform_device *pdev)
{
	struct uio_info *info = platform_get_drvdata(pdev);
	struct device *dev = &pdev->dev;

	dev_info(dev, "[RBUS] %s\n", __func__);

	uio_unregister_device(info);
	platform_set_drvdata(pdev, NULL);
	kfree(info);

	return 0;
}

static const struct of_device_id rtk_rbus_ids[] = {
	{ .compatible = "realtek,rbus" },
	{}
};

static struct platform_driver rtk_rbus_driver = {
	.probe		= rtk_rbus_probe,
	.remove		= rtk_rbus_remove,
	.driver		= {
		.name	= "rtk-rbus",
		.owner	= THIS_MODULE,
		.of_match_table = rtk_rbus_ids,
	},
};
module_platform_driver_probe(rtk_rbus_driver, rtk_rbus_probe);

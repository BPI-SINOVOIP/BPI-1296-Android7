/*
 * Copyright (C) 2015-2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/uio_driver.h>
#include "reg_md.h"
#include "uio_rtk_md.h"

struct md_uio_info {
	struct uio_info info;
	struct device *dev;
	void __iomem *base;
	struct clk *clk;
	int irq;
};

enum {
	MdClearWriteData = 0,
	MdWriteData = BIT(0),
	MdGo = BIT(1),
	MdEndianSwap = BIT(2),
};

static inline void rtk_md_init_reg(struct device *dev)
{
	struct md_uio_info *priv = dev_get_drvdata(dev);
	struct uio_info *info = &priv->info;
	struct md_hw *md_hw = info->mem[1].internal_addr;
	int i;

	 MDREG_INFO *reg_info = (MDREG_INFO *)priv->base;

	for (i = 0 ; i < MD_NUM_ENGINES ; i++) {
		/* Stop Moving Data */
		reg_info->MdCtrl[i].Value =
			(MdGo | MdEndianSwap | MdClearWriteData);
		reg_info->MdCtrl[i].Value = (MdEndianSwap | MdWriteData);
		reg_info->MdCmdBase[i].Value = md_hw->engine[i].CmdBase;
		reg_info->MdCmdReadPtr[i].Value = md_hw->engine[i].CmdBase;
		reg_info->MdCmdWritePtr[i].Value = md_hw->engine[i].CmdBase;
		reg_info->MdCmdLimit[i].Value = md_hw->engine[i].CmdLimit;
		reg_info->MdInstCnt[i].Value = 0;

		dev_info(dev, "[MD] Engine[%d] Ctrl:%08x Base:%08x ReadPtr:%08x WritePtr:%08x Limit:%08x\n",
			i,
			reg_info->MdCtrl[i].Value,
			reg_info->MdCmdBase[i].Value,
			reg_info->MdCmdReadPtr[i].Value,
			reg_info->MdCmdWritePtr[i].Value,
			reg_info->MdCmdLimit[i].Value);

		md_hw->engine[i].CmdWritePtr = md_hw->engine[i].CmdBase;
	}
}

static inline void rtk_md_enable_icg(struct device *dev)
{
	struct md_uio_info *priv = dev_get_drvdata(dev);
	unsigned int val;

	val = readl(priv->base + 0x180);
	val |= 0x0000000f;
	writel(val, priv->base + 0x180);
}

static void rtk_md_init_drv(struct device *dev)
{
	struct md_uio_info *priv = dev_get_drvdata(dev);
	struct uio_info *info = &priv->info;
	struct md_hw *md_hw = info->mem[1].internal_addr;
	int i;
	void *virt = info->mem[2].internal_addr;
	dma_addr_t phys = info->mem[2].addr;

	for (i = 0 ; i < MD_NUM_ENGINES ; i++) {
		struct md_engine *eng = &md_hw->engine[i];

		eng->CmdBuf   = (unsigned long)virt + MD_CMDBUF_SIZE * i;
		eng->CmdBase  = phys + MD_CMDBUF_SIZE * i;
		eng->CmdLimit = eng->CmdBase + MD_CMDBUF_SIZE;
		eng->BufSize  = MD_CMDBUF_SIZE;
	}

	rtk_md_init_reg(dev);
	rtk_md_enable_icg(dev);
}

static irqreturn_t md_irq_handler(int irq, struct uio_info *info)
{
	struct md_uio_info *priv = container_of(info, struct md_uio_info, info);
	int i;
	MDREG_INFO *reg_info = (MDREG_INFO *)priv->base;

	pr_debug("[MD] interrupt...\n");

	for (i = 0 ; i < MD_NUM_ENGINES ; i++) {
		if (reg_info->MdInte[i].Fields.smq_empty_en &&
			reg_info->MdInts[i].Fields.smq_empty) {

			pr_debug("[MD] smq empty interrupt...\n");
			reg_info->MdInte[i].Value = 0x8;
		}
	}

	return IRQ_HANDLED;
}


static inline void print_uio_mem(struct device *dev, struct uio_info *info,
	int index)
{
	struct uio_mem *umem = &info->mem[index];

	dev_info(dev,
		"mem[%d] phys:0x%pa virt:0x%p size:0x%pa type:%d name:%s\n",
		index, &umem->addr, umem->internal_addr, &umem->size,
		umem->memtype, umem->name);
}


static int rtk_md_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct md_uio_info *priv;
	struct uio_info *info;
	int ret;
	size_t size = 0;
	void *virt = NULL;
	dma_addr_t phys = 0;
	struct resource res;

	dev_info(dev, "[MD] %s\n", __func__);

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->clk = clk_get(dev, NULL);
	if (IS_ERR(priv->clk)) {
		dev_warn(dev, "failed to get clk: %ld.\n", PTR_ERR(priv->clk));
		priv->clk = NULL;
	}

	ret = of_address_to_resource(pdev->dev.of_node, 0, &res);
	if (ret) {
		dev_err(dev, "failed to get resource.\n");
		goto free_priv;
	}

	ret = platform_get_irq(pdev, 0);
	if (ret < 0) {
		dev_err(dev, "failed to get irq.\n");
		goto free_priv;
	}
	priv->irq = ret;
	priv->dev = dev;
	priv->base = ioremap(res.start, resource_size(&res));

	dev_info(dev, "resouce: %pr\n", &res);
	dev_info(dev, "irq: %d\n", priv->irq);

	info = &priv->info;

	/* md register space */
	info->mem[0].name = "MD reg space";
	info->mem[0].addr = res.start;
	info->mem[0].size = ALIGN(resource_size(&res), PAGE_SIZE);
	info->mem[0].internal_addr = priv->base;
	info->mem[0].memtype = UIO_MEM_PHYS;
	print_uio_mem(dev, info, 0);

	/* md driver data */
	size = ALIGN(sizeof(struct md_hw), PAGE_SIZE);
	virt = dma_alloc_coherent(dev, size, &phys, GFP_KERNEL);
	if (!virt) {
		ret = -ENOMEM;
		goto free_base;
	}
	info->mem[1].name = "MD driver data";
	info->mem[1].addr = phys;
	info->mem[1].size = size;
	info->mem[1].internal_addr = virt;
	info->mem[1].memtype = UIO_MEM_PHYS;
	print_uio_mem(dev, info, 1);

	/* md command queue */
	size = ALIGN(MD_CMDBUF_SIZE * MD_NUM_ENGINES, PAGE_SIZE);
	virt = dma_alloc_coherent(dev, size, &phys, GFP_KERNEL | GFP_DMA);
	if (!virt) {
		ret = -ENOMEM;
		goto free_drv_data;
	}
	info->mem[2].name = "MD command queue";
	info->mem[2].addr = phys;
	info->mem[2].size = size;
	info->mem[2].internal_addr = virt;
	info->mem[2].memtype = UIO_MEM_PHYS;
	print_uio_mem(dev, info, 2);

#ifdef CONFIG_UIO_ASSIGN_MINOR
	info->minor = 252;
#endif
	info->version = "0.0.1";
	info->name = "RTK-MD";
	info->irq = priv->irq;
	info->irq_flags = IRQF_SHARED;
	info->handler = md_irq_handler;

	ret = uio_register_device(dev, info);
	if (ret) {
		dev_err(dev, "failed to register uio device.\n");
		goto free_cmd_queue;
	}

	clk_prepare_enable(priv->clk);

	platform_set_drvdata(pdev, priv);

	rtk_md_init_drv(dev);

	return 0;
free_cmd_queue:
	dma_free_coherent(dev, info->mem[2].size, info->mem[2].internal_addr,
		info->mem[2].addr);
free_drv_data:
	dma_free_coherent(dev, info->mem[1].size, info->mem[1].internal_addr,
		info->mem[1].addr);
free_base:
	iounmap(priv->base);
	clk_put(priv->clk);
free_priv:
	kfree(priv);

	return ret;
}

static int rtk_md_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct md_uio_info *priv = platform_get_drvdata(pdev);
	struct uio_info *info = &priv->info;

	dev_info(dev, "[MD] %s\n", __func__);

	uio_unregister_device(info);
	platform_set_drvdata(pdev, NULL);
	dma_free_coherent(dev, info->mem[2].size, info->mem[2].internal_addr,
		info->mem[2].addr);
	dma_free_coherent(dev, info->mem[1].size, info->mem[1].internal_addr,
		info->mem[1].addr);
	iounmap(info->mem[0].internal_addr);
	clk_put(priv->clk);
	kfree(priv);

	return 0;
}

static const struct of_device_id rtk_md_ids[] = {
	{ .compatible = "realtek,md" },
	{}
};

static struct platform_driver rtk_md_driver = {
	.probe = rtk_md_probe,
	.remove = rtk_md_remove,
	.driver = {
		.name = "rtk-md",
		.owner = THIS_MODULE,
		.of_match_table = rtk_md_ids,
	},
};
module_platform_driver_probe(rtk_md_driver, rtk_md_probe);

/*
 *  dptx_core.c - RTK display port driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/reset.h>

#include <linux/delay.h>
#include <linux/types.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>

#include "dptx_core.h"
#define LINK_TRAIN_RETRY 50

#ifdef SELF_TEST
static void rtk_dptx_powerchk(struct rtk_dptx_device *dptx_dev)
{
	void __iomem *chk_reg;
	unsigned int reg;
	
	chk_reg = ioremap(0x98000190, 0x1);
	reg = readl(chk_reg);
	if(reg==0xbf)
		dptx_dev->power_state = 1;
	else
		dptx_dev->power_state = 0;

	iounmap(chk_reg);
}
#endif
static int rtk_tveint_en(struct rtk_dptx_device *dptx_dev, int en)
{
	struct device *dev = dptx_dev->dev;
	void  __iomem *tve_reg;
	void  __iomem *vdac_reg;
	unsigned int reg, reg1;
	bool clken;
	int ret = -1;

	/* check tve clock enable or not */
	clken = __clk_is_enabled(dptx_dev->clks[0]);
	if(!clken) {
		dev_info(dev, "tve clock is disable, ignore\n");
		return ret;
	}

	tve_reg = ioremap(0x9801801c, 0x1);
	vdac_reg = ioremap(0x980183a0, 0x1);
	reg = readl(tve_reg);

	if((reg & 0x4) && !en) {
		reg1 = readl(vdac_reg);
		reg1 = reg1 & ~0x40000000;
		writel(reg1, vdac_reg);
		writel(0x4, tve_reg);
		ret = 0;
	} else if(!(reg & 0x4) && en) {
		writel(0x5, tve_reg);
		ret = 0;
	}
	iounmap(tve_reg);
	iounmap(vdac_reg);
	if(ret < 0)
		dev_info(dev, "tve interrupt = 0x%x, ignore\n", reg);
	return ret;
}

#ifdef SELF_TEST
static void rtk_hdmi_power_on(void)
{
	void __iomem *chk_reg;
	unsigned int i;

	chk_reg = ioremap(0x98000190, 0x1);
	writel(0x11a, chk_reg);
	for(i = 0; i < 4096 ; i++);
	writel(0x1be, chk_reg);
	for(i = 0; i < 4096 ; i++);
	writel(0xbf, chk_reg);
	for(i = 0; i < 4096 ; i++);
	iounmap(chk_reg);
}

static void rtk_disp_reset_on(void)
{
	void __iomem *chk_reg;
	unsigned int reg;

	chk_reg = ioremap(0x98000050, 0x1);
	reg = readl(chk_reg);
	reg = reg | 0x00008000;
	writel(reg, chk_reg);
	iounmap(chk_reg);
}

static void rtk_dptx_close(struct rtk_dptx_device *dptx_dev)
{
	dptx_close_phy(&dptx_dev->hwinfo);
	reset_control_assert(dptx_dev->rstc[0]);
	dptx_close_pll(&dptx_dev->hwinfo);
}
#endif
static void rtk_dptx_initial(struct rtk_dptx_device *dptx_dev)
{
	int ret;

	ret = rtk_tveint_en(dptx_dev, 0);
	msleep(20);

	DpTxPixelPLLSetting(&dptx_dev->hwinfo);
	DpTxDpPLLSetting(&dptx_dev->hwinfo);
	DpTxInitial(&dptx_dev->hwinfo);

	if(ret == 0)
		rtk_tveint_en(dptx_dev, 1);
}

static void rtk_dptx_reinit(struct rtk_dptx_device *dptx_dev)
{
	int ret;

	ret = rtk_tveint_en(dptx_dev, 0);
	dptx_lvdsint_en(&dptx_dev->hwinfo, 0);
	msleep(20);

	dptx_close_phy(&dptx_dev->hwinfo);
	reset_control_assert(dptx_dev->rstc[0]);
	dptx_close_pll(&dptx_dev->hwinfo);

	DpTxPixelPLLSetting(&dptx_dev->hwinfo);
	reset_control_deassert(dptx_dev->rstc[0]);
	DpTxDpPLLSetting(&dptx_dev->hwinfo);
	DpTxInitial(&dptx_dev->hwinfo);

	if(ret == 0)
		rtk_tveint_en(dptx_dev, 1);
}

int rtk_dptx_open(struct inode *inode, struct file *filp)
{
	struct miscdevice *misc=filp->private_data;
	struct rtk_dptx_device *dptx_dev = container_of(misc, struct rtk_dptx_device, miscdev);

	if (nonseekable_open(inode, filp))
		return -ENODEV;

	filp->private_data = dptx_dev;
	
	return 0;
}

static ssize_t rtk_dptx_write(struct file *filp, const char __user *buf,
        size_t count, loff_t *pos)
{
	struct rtk_dptx_device *dptx_dev = (struct rtk_dptx_device *)filp->private_data;
	struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM arg;
	int i, ret;

	RPC_TOAGENT_DPTX_QUERY_TV_System(dptx_dev->rpc_ion_client, &arg);
	if(buf[0]==48) {
		dptx_dev->hwinfo.out_type = DP_FORMAT_720P_60;
		rtk_dptx_reinit(dptx_dev);
		Set_720p_1lane(&dptx_dev->hwinfo);
		arg.videoInfo.pedType = VO_STANDARD_DP_FORMAT_1280_720P_60;
	} else if(buf[0]==49) {
		dptx_dev->hwinfo.out_type = DP_FORMAT_1080P_60;
		rtk_dptx_reinit(dptx_dev);
		Set_1080p_2lane(&dptx_dev->hwinfo);
		arg.videoInfo.pedType = VO_STANDARD_DP_FORMAT_1920_1080P_60;
	} else if(buf[0]==50) {
		dptx_dev->hwinfo.out_type = DP_FORMAT_2160P_30;
		rtk_dptx_reinit(dptx_dev);
		Set_2160p30_4lane(&dptx_dev->hwinfo);
		arg.videoInfo.pedType = VO_STANDARD_DP_FORMAT_2160P_30;
	} else if(buf[0]==51) {
		dptx_dev->hwinfo.out_type = DP_FORMAT_2160P_60;
		rtk_dptx_reinit(dptx_dev);
		Set_2160p60_4lane(&dptx_dev->hwinfo);
		arg.videoInfo.pedType = VO_STANDARD_DP_FORMAT_3840_2160P_60;
	} else if(buf[0]==52) {
		dptx_dev->hwinfo.out_type = DP_FORMAT_1024_768;
		rtk_dptx_reinit(dptx_dev);
		Set_2160p60_4lane(&dptx_dev->hwinfo);
		arg.videoInfo.pedType = VO_STANDARD_DP_FORMAT_1024_768P_60;
	} else {
		rtk_dptx_reinit(dptx_dev);
		return count;
	}

	for(i=0; i<LINK_TRAIN_RETRY; i++) {
		ret = DpTxLinkTraining(&dptx_dev->hwinfo);
		if(ret < 0)
			DPTX_ERR("link training fail %d\n", i);
		else
			break;
		msleep(100);
	}

	arg.interfaceType = 4;//VO_HDMI_AND_DISPLAY_PORT_SAME_SOURCE;
	arg.videoInfo.standard = VO_STANDARD_NTSC_J;
	RPC_TOAGENT_DPTX_Config_TV_System(dptx_dev->rpc_ion_client, &arg);

	return count;
}

static long rtk_dptx_ioctl(struct file* filp,unsigned int cmd, unsigned long arg)
{
	struct rtk_dptx_device *dptx_dev = (struct rtk_dptx_device *)filp->private_data;
	asoc_dptx_t* cap = &dptx_dev->cap;
	struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM tv_system;
	int ret=0;
	int mode;
	struct rtk_dptx_hwinfo *hwinfo = &dptx_dev->hwinfo;

	switch(cmd) {
	case DPTX_GET_SINK_CAPABILITY:
		if(!(cap->sink_cap_available)) {
			DPTX_ERR("[%s]sink cap is not available\n", __FUNCTION__);
			ret = -ENOMSG;
		}
		if(copy_to_user((void __user *)arg, &cap->sink_cap, sizeof(cap->sink_cap))) {
			DPTX_ERR("[%s]failed to copy to user !\n", __FUNCTION__);
			ret = -EFAULT;
		}
		return ret;
	case DPTX_CONFIG_TV_SYSTEM:
		if (copy_from_user(&tv_system, (void __user *)arg, sizeof(tv_system))) {
			DPTX_ERR("[%s]failed to copy from user !\n", __FUNCTION__);
			return -EFAULT;
		}
		mode = tv_system.videoInfo.pedType;
		switch(mode) {
		case VO_STANDARD_DP_FORMAT_1920_1080P_60:
			hwinfo->out_type = DP_FORMAT_1080P_60;
			break;
		case VO_STANDARD_DP_FORMAT_2160P_30:
			hwinfo->out_type = DP_FORMAT_2160P_30;
			break;
		case VO_STANDARD_DP_FORMAT_3840_2160P_60:
			hwinfo->out_type = DP_FORMAT_2160P_60;
			break;
		case VO_STANDARD_DP_FORMAT_1280_720P_60:
			hwinfo->out_type = DP_FORMAT_720P_60;
			break;
		case VO_STANDARD_DP_FORMAT_1024_768P_60:
			hwinfo->out_type = DP_FORMAT_1024_768;
			break;
		case VO_STANDARD_DP_FORMAT_1280_800P_60:
			hwinfo->out_type = DP_FORMAT_1280_800;
			break;
		case VO_STANDARD_DP_FORMAT_1440_768P_60:
			hwinfo->out_type = DP_FORMAT_1440_768;
			break;
		case VO_STANDARD_DP_FORMAT_1440_900P_60:
			hwinfo->out_type = DP_FORMAT_1440_900;
			break;
		case VO_STANDARD_DP_FORMAT_960_544P_60:
			hwinfo->out_type = DP_FORMAT_960_544;
			break;
		}
		rtk_dptx_reinit(dptx_dev);
		ret = dptx_config_tv_system(&dptx_dev->hwinfo);
		if(ret>=0)
			RPC_TOAGENT_DPTX_Config_TV_System(dptx_dev->rpc_ion_client, &tv_system);
		return ret;
	default:
		DPTX_ERR(" Unknown ioctl cmd %08x", cmd);
		return -EFAULT;
	}
	return 0;
}

static long rtk_dptx_compat_ioctl(struct file* file,unsigned int cmd, unsigned long arg)
{
	if (!file->f_op->unlocked_ioctl)
		return -ENOTTY;
	switch(cmd) {
		case DPTX_GET_SINK_CAPABILITY:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case DPTX_CONFIG_TV_SYSTEM:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		default:
			DPTX_ERR(" Unknown ioctl cmd %08x", cmd);
			return -EFAULT;
	}
	return 0;
}

static struct file_operations dptx_fops = {
	.owner = THIS_MODULE,
	.open = rtk_dptx_open,
	.write = rtk_dptx_write,
	.unlocked_ioctl = rtk_dptx_ioctl,
	.compat_ioctl = rtk_dptx_compat_ioctl,
};

static irqreturn_t rtk_dptx_isr(int irq, void *dev_id)
{
	struct rtk_dptx_device *dptx_dev = (struct rtk_dptx_device *)dev_id;
	
	DpTxIRQHandle(&dptx_dev->hwinfo);

	return IRQ_HANDLED;
}

static int rtk_dptx_probe(struct platform_device *pdev)
{
	struct rtk_dptx_device *dptx_dev;
	struct device *dev = &pdev->dev;
	int i;
#ifdef SELF_TEST
	rtk_hdmi_power_on();
	rtk_disp_reset_on();
#endif
	dptx_dev = devm_kzalloc(dev, sizeof(struct rtk_dptx_device), GFP_KERNEL);
	if(!dptx_dev) {
		dev_err(dev, "[%s] devm_kzalloc fail\n", __FUNCTION__);
		return -ENOMEM;
	}

	dptx_dev->cap.edid_ptr = devm_kzalloc(dev, sizeof(unsigned char)*1024, GFP_KERNEL);
	dptx_dev->dev = dev;
	dev->platform_data = dptx_dev;

	dptx_dev->rpc_ion_client = get_ion_client("dptx_driver");

//	rtk_dptx_powerchk(dptx_dev);

	// get and initialize hardware information
	sema_init(&dptx_dev->hwinfo.sem, 0);
	dptx_dev->hwinfo.reg_base = of_iomap(dev->of_node, 0);
	dptx_dev->hwinfo.pll_base = of_iomap(dev->of_node, 1);
	dptx_dev->hwinfo.lvds_base = of_iomap(dev->of_node, 2);
	dptx_dev->hwinfo.vo_base = of_iomap(dev->of_node, 3);

	// get clock and reset 
	dptx_dev->rstc[0] = reset_control_get(dev, "lvds");
	dptx_dev->rstc[1] = reset_control_get(dev, "vo");
	dptx_dev->rstc[2] = reset_control_get(dev, "dp");
	dptx_dev->rstc[3] = reset_control_get(dev, "tve");

	dptx_dev->hwinfo.out_type = DP_FORMAT_720P_60;

	dptx_dev->clks[0] = of_clk_get(dev->of_node, 0);
	for(i=1; i<DPTX_MAX_CLKS; i++) {
		dptx_dev->clks[i] = of_clk_get(dev->of_node, i);
		if(IS_ERR(dptx_dev->clks)) {
			dev_err(dev, "[%s] of_clk_get fail\n", __FUNCTION__);
			return -ENODEV;
		}
		clk_prepare_enable(dptx_dev->clks[i]);
	}

	for(i=0; i<DPTX_MAX_RESET; i++) {
		if (IS_ERR_OR_NULL(dptx_dev->rstc[i])) 
			dev_warn(dev, "[%s] reset_control_get %d fail\n",
				__FUNCTION__, i);
		else
			reset_control_deassert(dptx_dev->rstc[i]);
	}

	dptx_dev->ignore_edid = 0;
	of_property_read_u32(dev->of_node, "ignore-edid", &dptx_dev->ignore_edid);

	dptx_dev->miscdev.minor = MISC_DYNAMIC_MINOR;
	dptx_dev->miscdev.name = "dptx";
	dptx_dev->miscdev.mode = 0666;
	dptx_dev->miscdev.fops = &dptx_fops;

	if(misc_register(&dptx_dev->miscdev)) {
		dev_err(dev, "[%s] misc register fail\n", __FUNCTION__);
		return -ENODEV;
	}

	dptx_dev->dptx_irq = irq_of_parse_and_map (pdev->dev.of_node, 0);

	if(request_irq(dptx_dev->dptx_irq, rtk_dptx_isr, IRQF_SHARED, "DP_TX", dptx_dev)) {
		dev_err(dev, "[%s] request irq fail\n", __FUNCTION__);
		return -ENODEV;
	}
	
	rtk_dptx_initial(dptx_dev);
	
	register_dptx_switch(dptx_dev);
	return 0;
}

#ifdef CONFIG_PM
static int rtk_dptx_suspend(struct device *dev)
{
	struct rtk_dptx_device *dptx_dev;

	dptx_dev = dev->platform_data;

	disable_irq(dptx_dev->dptx_irq);
	rtk_dptx_switch_suspend(dptx_dev);

	return 0;
}

static void rtk_dptx_shutdown(struct platform_device *pdev)
{
	rtk_dptx_suspend(&pdev->dev);
}

static int rtk_dptx_resume(struct device *dev)
{
	struct rtk_dptx_device *dptx_dev;

	dptx_dev = dev->platform_data;

	DpTxPixelPLLSetting(&dptx_dev->hwinfo);
	reset_control_deassert(dptx_dev->rstc[0]);
	DpTxDpPLLSetting(&dptx_dev->hwinfo);
	DpTxInitial(&dptx_dev->hwinfo);

	enable_irq(dptx_dev->dptx_irq);
	rtk_dptx_switch_resume(dptx_dev);

	return 0;
}
#endif

static const struct of_device_id rtk_dptx_dt_ids[] = {
	{ .compatible = "Realtek,rtk129x-dptx", },
	{},
};
MODULE_DEVICE_TABLE(of, rtk_dptx_dt_ids);

#ifdef CONFIG_PM
static const struct dev_pm_ops rtk_dptx_pm_ops = {
	.suspend    = rtk_dptx_suspend,
	.resume     = rtk_dptx_resume,
};
#else
static const struct dev_pm_ops rtk_dptx_pm_ops = {};
#endif

static struct platform_driver rtk_dptx_driver = {
	.probe = rtk_dptx_probe,
	.driver = {
		.name = "rtk_dptx",
		.owner = THIS_MODULE, 
		.of_match_table = of_match_ptr(rtk_dptx_dt_ids),
#ifdef CONFIG_PM
		.pm = &rtk_dptx_pm_ops,
#endif
	},
#ifdef CONFIG_PM
	.shutdown = rtk_dptx_shutdown,
#endif

};
static int __init rtk_dptx_init(void)
{
    if (platform_driver_register(&rtk_dptx_driver)) {
		DPTX_DEBUG("can't add driver\n");
		return -EFAULT;
	}
	return 0;
}

static void __exit rtk_dptx_exit(void)
{

}

module_init(rtk_dptx_init);
module_exit(rtk_dptx_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Realtek Display Port kernel module");


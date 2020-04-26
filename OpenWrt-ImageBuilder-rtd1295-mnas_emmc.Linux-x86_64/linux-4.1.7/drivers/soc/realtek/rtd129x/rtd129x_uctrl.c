/*
 * rtd129x_uctrl.c - Userspace Control Interface for RTD-129x
 *
 * Copyright (C) 2016-2017, Realtek Semiconductor Corporation
 * Copyright (C) 2016-2017  Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) "uctrl: " fmt

#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/reset.h>
#include <linux/power-control.h>
#include <linux/uaccess.h>


#define RTK_UCTRL_IOCTL_MAGIC            0xEB
#define RTK_UCTRL_IOCTL_NONE             _IO(RTK_UCTRL_IOCTL_MAGIC,  0x00)
/* v1.0 */
#define RTK_UCTRL_IOCTL_CLK_EN_TP_SET    _IO(RTK_UCTRL_IOCTL_MAGIC,  0x01)
#define RTK_UCTRL_IOCTL_CLK_EN_TP_CLEAR  _IO(RTK_UCTRL_IOCTL_MAGIC,  0x02)
#define RTK_UCTRL_IOCTL_CLK_EN_TP_STATE \
	_IOR(RTK_UCTRL_IOCTL_MAGIC, 0x03, unsigned int)
#define RTK_UCTRL_IOCTL_RSTN_TP_SET      _IO(RTK_UCTRL_IOCTL_MAGIC,  0x04)
#define RTK_UCTRL_IOCTL_RSTN_TP_CLEAR    _IO(RTK_UCTRL_IOCTL_MAGIC,  0x05)
#define RTK_UCTRL_IOCTL_RSTN_TP_STATE  \
	_IOR(RTK_UCTRL_IOCTL_MAGIC, 0x06, unsigned int)
/* v1.1 */
#define RTK_UCTRL_IOCTL_GET_VERSION \
	_IOR(RTK_UCTRL_IOCTL_MAGIC, 0x10, unsigned int)
#define RTK_UCTRL_IOCTL_SET \
	_IOW(RTK_UCTRL_IOCTL_MAGIC, 0x11, unsigned int)
#define RTK_UCTRL_IOCTL_CLEAR \
	_IOW(RTK_UCTRL_IOCTL_MAGIC, 0x12, unsigned int)
#define RTK_UCTRL_IOCTL_STATE \
	_IOWR(RTK_UCTRL_IOCTL_MAGIC, 0x13, unsigned int)

#define __APPLY_VMASK(_v)    ((_v) & 0xffff)
#define RTK_UCTRL_MAKE_VERSION(_maj, _min) \
	((__APPLY_VMASK(_min) << 16) | __APPLY_VMASK(_maj))
#define RTK_UCTRL_GET_VERSION_MAJOR(_v)  __APPLY_VMASK(_v)
#define RTK_UCTRL_GET_VERSION_MINOR(_v)  __APPLY_VMASK((_v) >> 16)

enum RTK_UCTRL_ITEM_ID {
	RTK_UCTRL_CLK_EN_TP,
	RTK_UCTRL_RSTN_TP,
	RTK_UCTRL_CLK_EN_SE,
	RTK_UCTRL_RSTN_SE,
	RTK_UCTRL_CLK_EN_MD,
	RTK_UCTRL_RSTN_MD,

	RTK_UCTRL_LIST_END
};

enum RTK_UCTRL_ITEM_TYPE {
	RTK_UCTRL_CLK_EN,
	RTK_UCTRL_RSTC,
};

struct rtk_uctrl_item {
	int id;
	int type;
	const char *name;
	void *ref;
};

static struct rtk_uctrl_item items[] = {
	{ .id = RTK_UCTRL_CLK_EN_TP, .type = RTK_UCTRL_CLK_EN, .name = "tp", },
	{ .id = RTK_UCTRL_RSTN_TP,   .type = RTK_UCTRL_RSTC,   .name = "tp", },
	{ .id = RTK_UCTRL_CLK_EN_SE, .type = RTK_UCTRL_CLK_EN, .name = "se", },
	{ .id = RTK_UCTRL_RSTN_SE,   .type = RTK_UCTRL_RSTC,   .name = "se", },
	{ .id = RTK_UCTRL_CLK_EN_MD, .type = RTK_UCTRL_CLK_EN, .name = "md", },
	{ .id = RTK_UCTRL_RSTN_MD,   .type = RTK_UCTRL_RSTC,   .name = "md", },
	{ .id = RTK_UCTRL_LIST_END }
};

#define foreach_item(_p, list) \
	for (_p = list; _p->id != RTK_UCTRL_LIST_END; _p++)

#define clkp(_p)  ((struct clk *)(_p))
#define rstcp(_p) ((struct reset_control *)(_p))

static struct miscdevice mdev;
static DEFINE_SEMAPHORE(rtk_uctrl_sem);

static int rtk_uctrl_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static inline int __clk_enable(struct clk *clk)
{
	pr_debug("ENTER %s\n", __func__);

	if (!clk)
		return -ENODEV;

	clk_prepare_enable(clk);

	return 0;
}

static inline int __clk_disable(struct clk *clk)
{
	pr_debug("ENTER %s\n", __func__);

	if (!clk)
		return -ENODEV;

	clk_disable_unprepare(clk);

	return 0;
}

static inline int __clk_get_enabled(struct clk *clk)
{
	pr_debug("ENTER %s\n", __func__);

	if (!clk)
		return -ENODEV;

	return __clk_is_enabled(clk);
}

static inline int __rstc_deassert(struct reset_control *rstc)
{
	pr_debug("ENTER %s\n", __func__);

	if (!rstc)
		return -ENODEV;

	reset_control_deassert(rstc);

	return 0;
}

static inline int __rstc_assert(struct reset_control *rstc)
{
	pr_debug("ENTER %s\n", __func__);

	if (!rstc)
		return -ENODEV;

	reset_control_assert(rstc);

	return 0;
}

static inline int __rstc_get_status(struct reset_control *rstc)
{
	pr_debug("ENTER %s\n", __func__);

	if (!rstc)
		return -ENODEV;

	return !reset_control_status(rstc);
}

static long rtk_uctrl_ioctl(struct file *filp, unsigned int cmd,
	unsigned long arg)
{
	int ret = 0;
	struct rtk_uctrl_item *p;
	int id;
	int state;
	unsigned int _cmd;

	switch (cmd) {
	case RTK_UCTRL_IOCTL_CLK_EN_TP_SET:
	case RTK_UCTRL_IOCTL_CLK_EN_TP_CLEAR:
	case RTK_UCTRL_IOCTL_CLK_EN_TP_STATE:
	case RTK_UCTRL_IOCTL_RSTN_TP_SET:
	case RTK_UCTRL_IOCTL_RSTN_TP_CLEAR:
	case RTK_UCTRL_IOCTL_RSTN_TP_STATE:
		pr_err("cmd %d nolonger supportted\n", cmd);
		return -EINVAL;

	case RTK_UCTRL_IOCTL_GET_VERSION:
		_cmd = RTK_UCTRL_IOCTL_GET_VERSION;
		break;

	default:
		_cmd = cmd;
		ret = copy_from_user(&id, (unsigned int __user *)arg,
			sizeof(unsigned int));
		if (ret)
			return ret;

		break;
	}

	foreach_item(p, items) {
		if (p->id == id)
			break;
	}

	if (p->id == RTK_UCTRL_LIST_END)
		return -EINVAL;

	/* down(&rtk_uctrl_sem); */

	switch (_cmd) {
	case RTK_UCTRL_IOCTL_GET_VERSION:
		state = RTK_UCTRL_MAKE_VERSION(1, 1);
		break;

	case RTK_UCTRL_IOCTL_SET:
		if (p->type == RTK_UCTRL_CLK_EN)
			ret = __clk_enable(clkp(p->ref));
		else if (p->type == RTK_UCTRL_RSTC)
			ret = __rstc_deassert(rstcp(p->ref));
		else
			ret = -EINVAL;

		if (ret)
			return ret;

		break;

	case RTK_UCTRL_IOCTL_CLEAR:
		if (p->type == RTK_UCTRL_CLK_EN)
			ret = __clk_disable(clkp(p->ref));
		else if (p->type == RTK_UCTRL_RSTC)
			ret = __rstc_assert(rstcp(p->ref));
		else
			ret = -EINVAL;

		if (ret)
			return ret;
		break;

	case RTK_UCTRL_IOCTL_STATE:
		if (p->type == RTK_UCTRL_CLK_EN)
			state = __clk_get_enabled(clkp(p->ref));
		else if (p->type == RTK_UCTRL_RSTC)
			state = __rstc_get_status(rstcp(p->ref));
		else
			state = -EINVAL;

		if (state != 0 && state != 1)
			ret = state;

		break;
	}


	switch (cmd) {
	case RTK_UCTRL_IOCTL_CLK_EN_TP_SET:
	case RTK_UCTRL_IOCTL_CLK_EN_TP_CLEAR:
	case RTK_UCTRL_IOCTL_RSTN_TP_SET:
	case RTK_UCTRL_IOCTL_RSTN_TP_CLEAR:
		break;
	case RTK_UCTRL_IOCTL_SET:
	case RTK_UCTRL_IOCTL_CLEAR:
		break;
	default:
		ret = copy_to_user((unsigned int __user *)arg,
			&state, sizeof(unsigned int));
		break;
	}

	/* up(&rtk_uctrl_sem); */

	return ret;
}

static long rtk_uctrl_compact_ioctl(struct file *filp, unsigned int cmd,
	unsigned long arg)
{
	return rtk_uctrl_ioctl(filp, cmd, (unsigned long)arg);
}

static int rtk_uctrl_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static const struct file_operations rtk_uctrl_fops = {
	.owner          = THIS_MODULE,
	.open           = rtk_uctrl_open,
	.unlocked_ioctl = rtk_uctrl_ioctl,
	.compat_ioctl   = rtk_uctrl_compact_ioctl,
	.release        = rtk_uctrl_release,
};

static int __init rtk_uctrl_probe(struct platform_device *pdev)
{
	int ret;
	struct rtk_uctrl_item *p;
	struct device *dev = &pdev->dev;

	dev_info(dev, "[UCTRL] %s\n", __func__);

	mdev.minor  = MISC_DYNAMIC_MINOR;
	mdev.name   = "uctrl";
	mdev.fops   = &rtk_uctrl_fops;
	mdev.parent = NULL;

	ret = misc_register(&mdev);
	if (ret) {
		pr_err("Failed to register misc device: %d\n", ret);
		return ret;
	}

	foreach_item(p, items) {
		switch (p->type) {
		case RTK_UCTRL_CLK_EN:
			p->ref = (void *)clk_get(dev, p->name);
			if (IS_ERR_OR_NULL(p->ref)) {
				pr_warn("Failed to get %s: %d\n", p->name,
					(int)PTR_ERR(p->ref));
				p->ref = NULL;
			}
			break;

		case RTK_UCTRL_RSTC:
			p->ref = (void *)reset_control_get(dev, p->name);
			if (IS_ERR_OR_NULL(p->ref)) {
				pr_warn("Failed to get %s: %d\n", p->name,
					(int)PTR_ERR(p->ref));
				p->ref = NULL;
			}
			break;

		default:
			pr_warn("Unknown type: %d\n", p->type);
			break;
		}
	}

	return 0;
}

static const struct of_device_id rtk_uctrl_match[] = {
	{.compatible = "realtek,userspace-control"},
	{}
};

static struct platform_driver rtk_uctrl_driver = {
	.probe = rtk_uctrl_probe,
	.driver = {
		.name = "rtk-uctrl",
		.of_match_table = rtk_uctrl_match,
	},
};

static int __init rtk_uctrl_init(void)
{
	return platform_driver_register(&rtk_uctrl_driver);
}
late_initcall(rtk_uctrl_init);

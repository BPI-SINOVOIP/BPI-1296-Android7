/*
 *  phy-rtk-usb3.c RTK usb3.0 phy driver
 *
 * copyright (c) 2017 realtek semiconductor corporation
 *
 * this program is free software; you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation; either version 2 of the license, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/usb/otg.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/usb/ch11.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>

#include "phy-rtk-usb.h"

#define RTK_USB3PHY_NAME "rtk-usb3phy"

static struct reg_addr {
	void __iomem *REG_MDIO_CTL;
};

static struct phy_data {
	int size;
	u8 *addr;
	u16 *data;

	u16 saved_trim_value;//=0xFFFF;
	u8 connected;//=0;
};

#define WAIT_VBUSY_RETRY	3
#define USB_ST_BUSY		BIT(7)

int utmi_wait_register(void __iomem *reg, u32 mask, u32 result);

static int rtk_usb_phy3_wait_vbusy(struct reg_addr *regAddr)
{
	return utmi_wait_register(regAddr->REG_MDIO_CTL, BIT(7), 0);
}

static u32 rtk_usb_phy_read(struct rtk_usb_phy_s *rtk_phy, char addr)
{
	volatile unsigned int regVal;
	struct reg_addr * regAddr = rtk_phy->reg_addr;

	regVal = (addr << 8);

	writel(regVal, regAddr->REG_MDIO_CTL);

	rtk_usb_phy3_wait_vbusy(regAddr);

	return readl(regAddr->REG_MDIO_CTL);
}

static int rtk_usb_phy_write(struct rtk_usb_phy_s *rtk_phy, char addr, u16 data)
{
	volatile unsigned int regVal;
	struct reg_addr * regAddr = rtk_phy->reg_addr;

	regVal = BIT(0)     |
			(addr << 8) |
			(data <<16);

	writel(regVal, regAddr->REG_MDIO_CTL);

	rtk_usb_phy3_wait_vbusy(regAddr);

	return 0;
}

static void rtk_usb_phy_shutdown(struct usb_phy *phy)
{
	/* Todo */
}

void rtk_usb3_phy_toggle(struct usb_phy *usb3_phy, bool isConnect);

static int rtk_usb_phy_init(struct usb_phy *phy)
{
	struct rtk_usb_phy_s *rtk_phy = (struct rtk_usb_phy_s*) phy;
	struct reg_addr *regAddr = rtk_phy->reg_addr;
	int index = 0;
	struct phy_data *phy_data = rtk_phy->phy_data;
	u8 *addr = phy_data->addr;
	u16 *data = phy_data->data;

	dev_info(phy->dev, "%s Init RTK USB 3.0 PHY\n", __FILE__);

	for (index = 0; index < phy_data->size; index++) {
		rtk_usb_phy_write(rtk_phy, *(addr + index), *(data + index));
	}

	rtk_usb3_phy_toggle(phy, false);

	for (index = 0; index < phy_data->size; index++)
		dev_dbg(phy->dev, "[USB3_PHY], addr = 0x%02x, data = 0x%08x\n",
				index, rtk_usb_phy_read(rtk_phy, index));

	dev_info(phy->dev, "%s Initialized RTK USB 3.0 PHY\n", __FILE__);
	return 0;
}

void rtk_usb3_phy_toggle(struct usb_phy *usb3_phy, bool isConnect)
{
	struct rtk_usb_phy_s *rtk_phy = NULL;
	struct phy_data *phy_data = NULL;

	if (usb3_phy != NULL && usb3_phy->dev != NULL)
		rtk_phy = dev_get_drvdata(usb3_phy->dev);

	if (rtk_phy == NULL) {
		pr_err("%s %s ERROR! NO this device", __FILE__, __func__);
		return;
	}

	phy_data = rtk_phy->phy_data;
	if (phy_data) {
		u8 *addr = phy_data->addr;
		u16 *data = phy_data->data;
		size_t index;

		for (index = 0; index < phy_data->size; index++) {
			if (*(addr + index) == 0x09) {
				mdelay(1);
				dev_info(rtk_phy->dev, "%s ########## to toggle PHY addr 0x09 BIT(9)\n", __func__);
				rtk_usb_phy_write(rtk_phy, *(addr + index), *(data + index) & (~BIT(9)));
				mdelay(1);
				rtk_usb_phy_write(rtk_phy, *(addr + index), *(data + index));
			}
		}
		dev_info(rtk_phy->dev, "%s ########## PHY addr 0x1f = 0x%08x\n", __func__,
				rtk_usb_phy_read(rtk_phy, 0x1f));
	}

}
EXPORT_SYMBOL(rtk_usb3_phy_toggle);

void rtk_usb3_phy_state(u32 state)
{
	struct device_node *node = NULL;
	struct device_node *prev_node = NULL;
	struct platform_device *pdev;
	struct rtk_usb_phy_s *rtk_phy = NULL;
	struct phy_data *phy_data;

	pr_info("%s: Not enable %s~", __FILE__, __func__);
	return;

	do {
		node = of_find_compatible_node(prev_node, NULL, "Realtek,rtk119x-usb3phy");
		if (node == NULL)
			break;
		pdev = of_find_device_by_node(node);
		if (pdev != NULL)
			rtk_phy = platform_get_drvdata(pdev);

		if (rtk_phy == NULL) {
			pr_err("%s %s ERROR! NO this device", __FILE__, __func__);
			break;
		}

		phy_data = rtk_phy->phy_data;

		//Note TODO Check if add to 1295 soc
		if (true)
			pr_err("%s %s TODO check if add to 1295!!!", __FILE__, __func__);
		else //if (soc_is_rtk1195() && (realtek_rev() < RTK1195_REV_C))
		{
			if ((state & USB_PORT_STAT_CONNECTION) && (phy_data->connected == 0)) {
				dev_info(rtk_phy->dev, "%s ------ USB3 connected\n", __func__);
				phy_data->connected = 1;
				rtk_usb_phy_write(rtk_phy, 0x1D, 0xA03E);
			} else if (!(state & USB_PORT_STAT_CONNECTION) && (phy_data->connected == 1)) {
				dev_info(rtk_phy->dev, "%s ------ USB3 disconnected\n", __func__);
				phy_data->connected = 0;
				phy_data->saved_trim_value = 0xFFFF;	// reset to init phase
				rtk_usb_phy_write(rtk_phy, 0x1D, 0xA03F);
			}
		}

		rtk_phy = NULL;
		prev_node = node;
	} while (prev_node != NULL);
}
EXPORT_SYMBOL(rtk_usb3_phy_state);

unsigned int grayToBinary(unsigned int num)
{
    unsigned int mask;
    for (mask = num >> 1; mask != 0; mask = mask >> 1)
    {
        num = num ^ mask;
    }
    return num;
}

void rtk_usb3_phy_trim(u16 state)
{
	struct device_node *node = NULL;
	struct device_node *prev_node = NULL;
	struct platform_device *pdev;
	struct rtk_usb_phy_s *rtk_phy = NULL;
	struct phy_data *phy_data;
	u16 tmp = 0;

	pr_info("%s: Not enable %s~", __FILE__, __func__);
	return;

	do {
		node = of_find_compatible_node(prev_node, NULL, "Realtek,rtk119x-usb3phy");
		if (node == NULL)
			break;
		pdev = of_find_device_by_node(node);
		if (pdev != NULL)
			rtk_phy = platform_get_drvdata(pdev);

		if (rtk_phy == NULL) {
			pr_err("%s %s ERROR! NO this device", __FILE__, __func__);
			break;
		}

		phy_data = rtk_phy->phy_data;
		switch (state) {
			case USB_SS_PORT_LS_U0:
				if ((phy_data->connected == 1) && (phy_data->saved_trim_value == 0xFFFF)) {
					phy_data->saved_trim_value = rtk_usb_phy_read(rtk_phy, 0x1f)>>27;
					dev_info(rtk_phy->dev, "%s ------ get usb3 phy: %04x\n", __func__, phy_data->saved_trim_value);
				}
				break;
			case USB_SS_PORT_LS_U3:
				if (phy_data->saved_trim_value != 0xFFFF) {
					tmp = rtk_usb_phy_read(rtk_phy, 0x19)>>16;
					tmp &= ~(0xfe00);
					rtk_usb_phy_write(rtk_phy, 0x19, tmp | grayToBinary(phy_data->saved_trim_value)<<11);
					dev_info(rtk_phy->dev, "%s ------ rewrite [0x19]:%04x\n", __func__, rtk_usb_phy_read(rtk_phy, 0x19)>>16);
				}
				break;
			default:
				break;
		}

		rtk_phy = NULL;
		prev_node = node;
	} while (prev_node != NULL);

}
EXPORT_SYMBOL(rtk_usb3_phy_trim);

#ifdef CONFIG_DYNAMIC_DEBUG
extern struct dentry *create_phy_debug_root(void);

static int rtk_usb3_parameter_show(struct seq_file *s, void *unused)
{
	struct rtk_usb_phy_s		*rtk_phy = s->private;
	struct phy_data *phy_data = rtk_phy->phy_data;
	u8 *addr = phy_data->addr;
	u16 *data = phy_data->data;
	int index;

	for (index = 0; index < phy_data->size; index++)
		seq_printf(s, "[USB3_PHY], addr = 0x%02x, data = 0x%08x\n",
				index, rtk_usb_phy_read(rtk_phy, index));

	return 0;
}

static int rtk_usb3_parameter_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_usb3_parameter_show, inode->i_private);
}

static const struct file_operations rtk_usb3_parameter_fops = {
	.open			= rtk_usb3_parameter_open,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int rtk_usb3_set_parameter_show(struct seq_file *s, void *unused)
{
	struct rtk_usb_phy_s	*rtk_phy = s->private;

	seq_printf(s, "Set Phy parameter by following command\n");
	seq_printf(s, "echo \"addr value\" > set_parameter\n");
	seq_printf(s, "echo \"0x00 0x4008\" > set_parameter\n");
	seq_printf(s, "echo \"0x21 0x88AA\" > set_parameter\n");

	return 0;
}

static int rtk_usb3_set_parameter_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_usb3_set_parameter_show, inode->i_private);
}

static ssize_t rtk_usb3_set_parameter_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct rtk_usb_phy_s		*rtk_phy = s->private;
	struct phy_data *phy_data = rtk_phy->phy_data;
	u8 *addr = phy_data->addr;
	u16 *data = phy_data->data;
	int index;
	char			buffer[40];
	char *buf = buffer;
	u32 _addr;
	u32 value;

	if (copy_from_user(&buffer, ubuf, min_t(size_t, sizeof(buffer) - 1, count)))
		return -EFAULT;

	buf = skip_spaces(buf);
	sscanf(buf, "%x %x", &_addr, &value);

	for (index = 0; index < phy_data->size; index++) {
		if (_addr == *(addr + index)) {
			*(data + index) = value;
			rtk_usb_phy_write(rtk_phy, *(addr + index), *(data + index));
		}
	}

	return count;
}

static const struct file_operations rtk_usb3_set_parameter_fops = {
	.open			= rtk_usb3_set_parameter_open,
	.write			= rtk_usb3_set_parameter_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int rtk_usb3_toggle_show(struct seq_file *s, void *unused)
{
	struct rtk_usb_phy_s		*rtk_phy = s->private;

	seq_printf(s, "ehco 1 to toggle Page1 addr 0xe0 BIT(2)\n");

	return 0;
}

static int rtk_usb3_toggle_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_usb3_toggle_show, inode->i_private);
}

static ssize_t rtk_usb3_toggle_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct rtk_usb_phy_s		*rtk_phy = s->private;
	unsigned long		flags;
	char			buf[32];

	if (copy_from_user(&buf, ubuf, min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	if (!strncmp(buf, "1", 1))
		rtk_usb3_phy_toggle(&rtk_phy->phy, false);

	return count;
}

static const struct file_operations rtk_usb3_toggle_fops = {
	.open			= rtk_usb3_toggle_open,
	.write			= rtk_usb3_toggle_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static inline void create_debug_files(struct rtk_usb_phy_s *rtk_phy) {
	struct dentry *phy_debug_root = NULL;

	dev_dbg(rtk_phy->dev, "%s", __func__);

	phy_debug_root = create_phy_debug_root();

	if (!phy_debug_root) {
		dev_err(rtk_phy->dev, "%s Error phy_debug_root is NULL", __func__);
		return;
	}
	rtk_phy->debug_dir = debugfs_create_dir(dev_name(rtk_phy->dev), phy_debug_root);
	if (!rtk_phy->debug_dir) {
		dev_err(rtk_phy->dev, "%s Error debug_dir is NULL", __func__);
		return;
	}

	if (!debugfs_create_file("parameter", S_IRUGO, rtk_phy->debug_dir, rtk_phy,
						&rtk_usb3_parameter_fops))
		goto file_error;

	if (!debugfs_create_file("set_parameter", S_IRUGO | S_IWUSR, rtk_phy->debug_dir, rtk_phy,
						&rtk_usb3_set_parameter_fops))
		goto file_error;

	if (!debugfs_create_file("toggle", S_IRUGO | S_IWUSR, rtk_phy->debug_dir, rtk_phy,
						&rtk_usb3_toggle_fops))
		goto file_error;

	return;

file_error:
	debugfs_remove_recursive(rtk_phy->debug_dir);
}
#endif //CONFIG_DYNAMIC_DEBUG

static int rtk_usb3phy_probe(struct platform_device *pdev)
{
	struct rtk_usb_phy_s *rtk_usb_phy;
	struct device *dev = &pdev->dev;
	struct reg_addr *addr;
	struct phy_data *phy_data;
	int	ret = 0;

	rtk_usb_phy = devm_kzalloc(dev, sizeof(*rtk_usb_phy), GFP_KERNEL);
	if (!rtk_usb_phy)
		return -ENOMEM;

	rtk_usb_phy->dev			= &pdev->dev;
	rtk_usb_phy->phy.dev		= rtk_usb_phy->dev;
	rtk_usb_phy->phy.label		= RTK_USB3PHY_NAME;
	rtk_usb_phy->phy.init		= rtk_usb_phy_init;
	rtk_usb_phy->phy.shutdown	= rtk_usb_phy_shutdown;

	addr = devm_kzalloc(dev, sizeof(*addr), GFP_KERNEL);
	if (!addr)
		return -ENOMEM;
	addr->REG_MDIO_CTL = of_iomap(dev->of_node, 0);
	rtk_usb_phy->reg_addr = addr;
	dev_dbg(dev, "%s %s REG_MDIO_CTL=%p\n", __FILE__, __func__, addr->REG_MDIO_CTL);

	phy_data = devm_kzalloc(dev, sizeof(*phy_data), GFP_KERNEL);
	if (!phy_data)
		return -ENOMEM;
	phy_data->saved_trim_value = 0xFFFF;
	phy_data->connected = 0;

	if (dev->of_node) {
		ret = of_property_read_u32_index(dev->of_node, "phy_data_size", 0, &phy_data->size);
		if (ret)
			goto err;
		phy_data->addr = devm_kzalloc(dev, sizeof(u8)*phy_data->size, GFP_KERNEL);
		if (!phy_data->addr)
			return -ENOMEM;
		phy_data->data = devm_kzalloc(dev, sizeof(u16)*phy_data->size, GFP_KERNEL);
		if (!phy_data->data)
			return -ENOMEM;
		ret = of_property_read_u8_array(dev->of_node, "phy_data_addr", phy_data->addr, phy_data->size);
		if (ret)
			goto err;
		ret = of_property_read_u16_array(dev->of_node, "phy_data_revA", phy_data->data, phy_data->size);
		if (ret)
			goto err;
	}
	rtk_usb_phy->phy_data = phy_data;

	platform_set_drvdata(pdev, rtk_usb_phy);

	//ret = usb_add_phy(&rtk_usb_phy->phy, USB_PHY_TYPE_USB3);
	//if (ret)
	//	goto err;

#ifdef CONFIG_DYNAMIC_DEBUG
	create_debug_files(rtk_usb_phy);
#endif

	dev_info(&pdev->dev, "%s Probe RTK USB 3.0 PHY\n", __FILE__);
err:
	return ret;
}

static int rtk_usb3phy_remove(struct platform_device *pdev)
{
	//struct rtk_usb_phy_s *rtk_usb_phy = platform_get_drvdata(pdev);

	//usb_remove_phy(&rtk_usb_phy->phy);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id usbphy_rtk_dt_match[] = {
	{ .compatible = "Realtek,rtk119x-usb3phy", },
	{ .compatible = "Realtek,rtd129x-usb3phy", },
	{},
};
MODULE_DEVICE_TABLE(of, usbphy_rtk_dt_match);
#endif

static struct platform_driver rtk_usb3phy_driver = {
	.probe		= rtk_usb3phy_probe,
	.remove		= rtk_usb3phy_remove,
	.driver		= {
		.name	= RTK_USB3PHY_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(usbphy_rtk_dt_match),
	},
};

module_platform_driver(rtk_usb3phy_driver);
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" RTK_USB3PHY_NAME);

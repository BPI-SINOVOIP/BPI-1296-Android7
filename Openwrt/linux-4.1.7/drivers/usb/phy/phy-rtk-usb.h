/*
 *  phy-rtk-usb.h RTK usb phy header file
 *
 * copyright (c) 2017 realtek semiconductor corporation
 *
 * this program is free software; you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation; either version 2 of the license, or
 * (at your option) any later version.
 */

#ifndef __PHY_RTK_USB_H__
#define __PHY_RTK_USB_H__

struct rtk_usb_phy_s {
	struct usb_phy phy;
	struct device *dev;

	int portN;
	void *reg_addr;
	void *phy_data;

#ifdef CONFIG_DYNAMIC_DEBUG
	struct dentry		*debug_dir;
#endif
};

struct rtk_usb_phy_data_s {
	char addr;
	char data;
};


#endif /* __PHY_RTK_USB_H__ */

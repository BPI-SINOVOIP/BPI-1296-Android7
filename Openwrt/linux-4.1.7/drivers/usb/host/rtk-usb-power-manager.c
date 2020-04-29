/*
 *  rtk-usb-power-manager.c RTK Host Power Control Driver for All USB.
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/usb.h>
#include <linux/usb/of.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/reset-helper.h> // rstc_get
#include <linux/reset.h>
#include <linux/clkdev.h>   // clk_get
#include <linux/clk-provider.h>
#include <linux/power-control.h>
#include <linux/suspend.h>
#include <linux/regulator/consumer.h>
#include <soc/realtek/rtd129x_cpu.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>

#define CRT_SOFT_RESET1 	0x0
#define CRT_SOFT_RESET2 	0x4
#define CRT_CLOCK_ENABLE1 	0xc

struct manager_data {
	void __iomem *crt_base;
	struct device *dev;

	/*GPIO*/
	unsigned int type_c_pow_gpio;
	unsigned int u2host_pow_gpio;
	unsigned int u3host_pow_gpio;

	int port0;
	int port1;
	int port2;
	int port3;
	bool disable_usb;

	struct device_node *drd_node;
	struct device_node *u2host_node;
	struct device_node *ehci_node;
	struct device_node *ohci_node;
	struct device_node *u3host_node;

	bool usb_iso_mode;
	bool usb_power_cut;
	bool en_usb_storage_reprobe;

	struct workqueue_struct *wq_usb_manager;

#ifdef CONFIG_DYNAMIC_DEBUG
	struct dentry *debug_dir;
#endif

	spinlock_t		lock;
};

/*[ROOT_CAUSE] usb SATA not stable after power on
 *commit ba77afca700fa8586607fcfa9091b863f4a3b620
 */
extern bool rescue_type ;

/* [ROOT_CAUSE]: quirk device result in compatibility issue
 * [SOLUTION]: clear port power and reset port if got error
 * commit 42e905e60eaadd3fd8374db23dc644b98080717e
 */
extern int RTK_usb_probe_device(struct device *dev);
extern int RTK_usb_unbind_device(struct device *dev);

int RTK_usb_reprobe_usb_storage(struct usb_device *udev)
{
	struct device_node *node = of_find_compatible_node(NULL, NULL,
		    "Realtek,rtd129x-usb-power-manager");
	struct platform_device *pdev = NULL;
	static struct manager_data *data = NULL;

	if (data == NULL) {
		node = of_find_compatible_node(NULL, NULL,
		    "Realtek,rtd129x-usb-power-manager");
		if (node != NULL)
			pdev = of_find_device_by_node(node);
		if (pdev != NULL) {
			data = platform_get_drvdata(pdev);
		}
	}
	if (data == NULL) {
		pr_err("%s ERROR no manager_data", __func__);
		return -ENODEV;
	}

	/* [ROOT_CAUSE]: quirk device result in compatibility issue
	 * [SOLUTION]: clear port power and reset port if got error
	 * commit 42e905e60eaadd3fd8374db23dc644b98080717e
	 * [DEV_FIX]MediaNAS_V0.1 can't recognise the 5411 hub after power on
	 * [SOLUTION] dont unbind and probe again after usb_add_device , and set it as quirk device
	 * commit 75b83399dcf5e38ae39ce16d730f5824fccbb3bf
	 * [DEV_FIX]xhci hub 2.0  can't be recognize after unplug and plug again
	 * commit fbb4cc12d549b5193d51e88d6d0bc916a6e4d5d3
	 * [ROOT_CAUSE] USB hdd can't be mounted cuz scsi not stable during power-on stage
	 * commit dbb572028226bba6b504ff859fe3f05e5b7efbdf
	 * [ROOT_CAUSE] USB hdd can't be mounted cuz scsi not stable during power-on stage
	 * commit 8fa320af443950c0dbc9875546159d9c57cd97f2
	 * [ROOT_CAUSE]config USB device error ,result in NULL pointer
	 * commit 0d7ba5cbee8ff84a99f926a0a00128fe9c6c6104
	 * [ROOT_CAUSE] usb SATA not stable after power on
	 * commit ba77afca700fa8586607fcfa9091b863f4a3b620
	 * [ROOT_CAUSE] if se config err , udev->actconfig will be NULL
	 * commit 2ebede9fe6b6347ecf446a06e958dfd0a671b27d
	 */
	//hcy added for quirk HDD
	if (data->en_usb_storage_reprobe) {
		int usb_storage = 0;
		int i;

		if (udev->actconfig) {
			for (i = 0; i < 1/*udev->actconfig->desc.bNumInterfaces*/; i++) {
				struct usb_host_config *config = udev->actconfig;
				struct usb_interface *intf = config->interface[i];
				struct usb_interface_descriptor *desc;
				desc = &intf->cur_altsetting->desc;

				dev_info(&udev->dev , "bInterfaceClass = %d "
					    "(jiffies 0x%2x = %d ms)\n",  desc->bInterfaceClass,
					    jiffies - INITIAL_JIFFIES,
					    jiffies_to_msecs(jiffies - INITIAL_JIFFIES));
				if (desc->bInterfaceClass == USB_CLASS_MASS_STORAGE) {
					usb_storage = 1;
					break;
				}
			}
		}

		if ((jiffies - INITIAL_JIFFIES) < 0x400
			    && (usb_storage ||
			    udev->descriptor.bDeviceClass == USB_CLASS_MASS_STORAGE)) {
			msleep(100);
			dev_info(&udev->dev , "%s unbind-probe dev_name = %s "
				    "(jiffies= 0x%2x = %d ms)\n", __func__,
				    dev_name(&udev->dev), jiffies - INITIAL_JIFFIES,
				    jiffies_to_msecs(jiffies - INITIAL_JIFFIES));
			RTK_usb_unbind_device(&udev->dev);
			//hcy added for waiting some scsi HDD stable after power on
			if (rescue_type) {
				dev_err(&udev->dev ,"** rescue type **\n");
				msleep(5000);
			} else {
				msleep(10000);
			}

			dev_info(&udev->dev , "%s bind-probe dev_name = %s "
				    "(jiffies= 0x%2x = %d ms)\n", __func__,
				    dev_name(&udev->dev), jiffies - INITIAL_JIFFIES,
				    jiffies_to_msecs(jiffies - INITIAL_JIFFIES));
			RTK_usb_probe_device(&udev->dev);
		}
	}
	return 0;
}

int rtk_usb_power_manager_schedule_work(struct device *usb_dev, struct work_struct *work) {
	struct device_node *usb_node = usb_dev->of_node;
	struct device_node *node = of_find_compatible_node(NULL, NULL, "Realtek,rtd129x-usb-power-manager");
	struct platform_device *pdev = NULL;
	struct manager_data *data = NULL;
	bool on = true;

	if (node != NULL)
		pdev = of_find_device_by_node(node);
	if (pdev != NULL) {
		data = platform_get_drvdata(pdev);
	}
	if (data == NULL) {
		pr_err("%s ERROR no manager_data", __func__);
		return -ENODEV;
	}
    dev_dbg(data->dev, "%s Enter ..\n", __func__);

	dev_info(data->dev, "%s for %s", __func__, dev_name(usb_dev));

	if (work == NULL) {
		dev_err(data->dev, "%s, work is NULL");
		return -1;
	}
	spin_lock(&data->lock);

	queue_work(data->wq_usb_manager, work);

	spin_unlock(&data->lock);

    dev_dbg(data->dev, "%s Exit ..\n", __func__);
	return 0;
}

static inline void __power_control_set_power(const char *name, bool power_on)
{
    struct power_control * pctrl = power_control_get(name);
    if (!pctrl) {
        pr_debug("%s: Failed to get power_control %s\n",  __func__, name);
        return;
    }

    if (power_on)
        power_control_power_on(pctrl);
    else
        power_control_power_off(pctrl);
}

/* enable hw_pm (L4 ICG)
 *   The hw_pm function will be reset after doing soft_reset, so 
 *   only enable is provided.
 */
static __maybe_unused void __rtk_usb_set_hw_pm_enable(struct manager_data *data)
{
    struct device *dev = data->dev;

    dev_dbg(dev, "set usb_hw_pm\n");

    /* for hw_pm, enable is equal to power_off */
    if (data->port0)
        __power_control_set_power("pctrl_l4_icg_usb_p0", false);
    if (data->port1)
        __power_control_set_power("pctrl_l4_icg_usb_p1", false);
    if (data->port2)
        __power_control_set_power("pctrl_l4_icg_usb_p2", false);
    if (data->port3)
        __power_control_set_power("pctrl_l4_icg_usb_p3", false);
}


/* set usb charger power */
static __maybe_unused void __rtk_usb_set_charger_power(struct manager_data *data, bool power_on)
{
    struct device *dev = data->dev;
    void __iomem *reg_charger  = ioremap(0x98007044, 0x4);
    unsigned int val = 0;

    if (power_on) {
        val |= data->port0 << 0;
        val |= data->port1 << 1;
        val |= data->port2 << 2;
        val |= data->port3 << 3;
    }

    dev_dbg(dev, "set usb_charger to 0x%08x\n", val);

    writel(val, reg_charger);

    iounmap(reg_charger);
}

/* set usb power domain */
static void __rtk_usb_set_pd_power(struct manager_data* data, bool power_on)
{
	struct device *dev = data->dev;
	void __iomem *reg_phy_ctrl = ioremap(0x98013340, 0x4);
	void __iomem *reg_phy_ctrl_p0 = ioremap(0x98013208, 0x4);
	void __iomem *reg_phy_ctrl_p1 = ioremap(0x98013c08, 0x4);
	void __iomem *reg_phy_ctrl_p2 = ioremap(0x9801382c, 0x4);
	void __iomem *reg_phy_ctrl_p3 = ioremap(0x98013e08, 0x4);
	void __iomem *reg_usb_ctrl = ioremap(0x98007fb0, 0x4);
	void __iomem *reg_usb0_sram_pwr = ioremap(0x98007f7c, 0x4);
	void __iomem *reg_usb0_sram_pwr_ctrl = ioremap(0x98007f80, 0x4);
	void __iomem *reg_usb1_sram_pwr = ioremap(0x98007f9c, 0x4);
	void __iomem *reg_usb1_sram_pwr_ctrl = ioremap(0x98007fa0, 0x4);

	if (power_on && (data->port0 || data->port1 || data->port2 || data->port3)) {

		dev_dbg(dev, "set usb_power_domain/p0 on\n");
		__power_control_set_power("pctrl_usb_p0_mac", 1);
		__power_control_set_power("pctrl_usb_p0_phy", 1);
		__power_control_set_power("pctrl_usb_p0_iso", 0);

		dev_dbg(dev, "set usb_power_domain/p3 on\n");
		__power_control_set_power("pctrl_usb_p3_phy", data->port3);
		__power_control_set_power("pctrl_usb_p3_mac", data->port3);
		__power_control_set_power("pctrl_usb_p3_iso", !data->port3);

		dev_info(dev, "set USB power_domain on ([0x98007fb0]=0x%08x, "
			    "[0x98007f7c]=0x%08x, [0x98007f80]=0x%08x)\n",
			    readl(reg_usb_ctrl), readl(reg_usb0_sram_pwr),
			    readl(reg_usb0_sram_pwr_ctrl));

	} else {
		/* isolate UPHY A->D */
		writel(BIT(0) | BIT(1) | readl(reg_phy_ctrl), reg_phy_ctrl);
		if (data->port0) /*UTMI reset to PHY */
			writel(BIT(0) | readl(reg_phy_ctrl_p0), reg_phy_ctrl_p0);
		if (data->port1) /* UTMI reset to PHY */
			writel(BIT(0) | readl(reg_phy_ctrl_p1), reg_phy_ctrl_p1);
		if (data->port2) /* UTMI reset to PHY */
			writel(BIT(0) | readl(reg_phy_ctrl_p2), reg_phy_ctrl_p2);
		if (data->port3) /* UTMI reset to PHY */
			writel(BIT(0) | readl(reg_phy_ctrl_p3), reg_phy_ctrl_p3);

		writel(~(BIT(4) | BIT(5) | BIT(6)) & readl(reg_usb_ctrl), reg_usb_ctrl);

		dev_dbg(dev, "set USB Analog phy power off ([0x98007fb0]=0x%08x)\n",
			    readl(reg_usb_ctrl));
		if (data->usb_power_cut &&
			    get_rtd129x_cpu_revision() >= RTD129x_CHIP_REVISION_B00) {
			writel((BIT(8) | BIT(9)) | readl(reg_usb_ctrl), reg_usb_ctrl);
			// port0-port2 sram
			writel(0, reg_usb0_sram_pwr);
			writel(0x00000f01, reg_usb0_sram_pwr_ctrl);
			// port3 sram
			if (get_rtd129x_cpu_id() == RTK1296_CPU_ID) {
				writel(0, reg_usb1_sram_pwr);
				if (get_rtd129x_cpu_revision() < RTD129x_CHIP_REVISION_B00)
					writel(0x00000f06, reg_usb1_sram_pwr_ctrl);
				else
					writel(0x00000f01, reg_usb1_sram_pwr_ctrl);
			}
			writel(~(BIT(0) | BIT(1) | BIT(4) | BIT(5) |BIT(6)) &
				    readl(reg_usb_ctrl), reg_usb_ctrl);

			dev_info(dev, "Set USB power_domain off ([0x98007fb0]=0x%08x, "
				    "[0x98007f7c]=0x%08x, [0x98007f80]=0x%08x)\n",
				    readl(reg_usb_ctrl), readl(reg_usb0_sram_pwr),
				    readl(reg_usb0_sram_pwr_ctrl));
		}
	}
	iounmap(reg_usb_ctrl);
	iounmap(reg_phy_ctrl);
	iounmap(reg_phy_ctrl_p0);
	iounmap(reg_phy_ctrl_p1);
	iounmap(reg_phy_ctrl_p2);
	iounmap(reg_phy_ctrl_p3);
	iounmap(reg_usb0_sram_pwr);
	iounmap(reg_usb0_sram_pwr_ctrl);
	iounmap(reg_usb1_sram_pwr);
	iounmap(reg_usb1_sram_pwr_ctrl);
}

static int __rtk_usb_host_reset(struct manager_data* data) {
	struct device *dev = data->dev;
	void __iomem *crt_reg = data->crt_base;
	/* GET clock */
	struct clk *clk_usb = clk_get(NULL, "clk_en_usb");

	/* GET reset controller */
	struct reset_control *reset_phy0 = reset_control_get(dev, "u2phy0");
	struct reset_control *reset_phy1 = reset_control_get(dev, "u2phy1");
	struct reset_control *reset_phy2 = reset_control_get(dev, "u2phy2");
	struct reset_control *reset_phy3 = reset_control_get(dev, "u2phy3");
	struct reset_control *reset_u3_phy0 = reset_control_get(dev, "u3phy0");
	struct reset_control *reset_u3_phy1 = reset_control_get(dev, "u3phy1");
	struct reset_control *reset_u3_phy0_mdio = reset_control_get(dev, "u3mdio0");
	struct reset_control *reset_u3_phy1_mdio = reset_control_get(dev, "u3mdio1");
	struct reset_control *reset_usb = reset_control_get(dev, "usb");
	struct reset_control *reset_usb_apply = reset_control_get(dev, "apply");

	dev_dbg(dev, "Realtek USB init CRT_SOFT_RESET1=%x, CRT_SOFT_RESET2=%x, CRT_CLOCK_ENABLE1=%x\n",
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET1)),
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET2)),
		(uint32_t)(readl(crt_reg + CRT_CLOCK_ENABLE1)));

	reset_control_assert(reset_phy0);
	reset_control_assert(reset_phy1);
	reset_control_assert(reset_phy2);
	reset_control_assert(reset_phy3);
	reset_control_assert(reset_u3_phy0);
	reset_control_assert(reset_u3_phy1);
	reset_control_assert(reset_u3_phy0_mdio);
	reset_control_assert(reset_u3_phy1_mdio);
	reset_control_assert(reset_usb);
	wmb();

	dev_dbg(dev, "Realtek USB init 0/5 CRT_SOFT_RESET1=%x, CRT_SOFT_RESET2=%x, CRT_CLOCK_ENABLE1=%x\n",
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET1)),
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET2)),
		(uint32_t)(readl(crt_reg + CRT_CLOCK_ENABLE1)));

#if 0 //Directly use write register to reset u2phy pll, due to port0 to port2 must reset Simultaneously.
// CRT_SOFT_RESET1 usb part
#define rstn_phy1          BIT(9)
#define rstn_phy0          BIT(8)
#define rstn_u3_phy0 BIT(2)
// CRT_SOFT_RESET2 usb part
#define rstn_u3_phy1 BIT(4)
#define rstn_phy2 BIT(3)
#define rstn_phy3 BIT(2)

	{
		int reset1_pll_flag = 0;
		int reset2_pll_flag = 0;

		// Enable usb phy reset
		if (data->port0)
			reset1_pll_flag |= rstn_phy0 | rstn_u3_phy0;

		if (data->port1)
			reset1_pll_flag |= rstn_phy1;

		if (data->port2)
			reset2_pll_flag |= rstn_phy2;

		if (data->port3)
			reset2_pll_flag |= rstn_phy3 | rstn_u3_phy1;

		writel(reset1_pll_flag | readl(crt_reg + CRT_SOFT_RESET1), crt_reg + CRT_SOFT_RESET1);
		writel(reset2_pll_flag | readl(crt_reg + CRT_SOFT_RESET2), crt_reg + CRT_SOFT_RESET2);
	}
#else
	// Enable usb phy reset
	/* DEASSERT: set rstn bit to 1 */
	if (data->port0 | data->port1 | data->port2) {
		reset_control_deassert(reset_phy0);
		reset_control_deassert(reset_u3_phy0);
		reset_control_deassert(reset_phy1);
		reset_control_deassert(reset_phy2);
	}
	if (data->port3) {
		reset_control_deassert(reset_phy3);
		reset_control_deassert(reset_u3_phy1);
	}
	reset_control_deassert(reset_usb_apply);
#endif

	mdelay(2);
	dev_dbg(dev,"Realtek USB init 1/5 Enable PLL (CRT_SOFT_RESET1=%x, CRT_SOFT_RESET2=%x, CRT_CLOCK_ENABLE1=%x)\n",
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET1)),
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET2)),
		(uint32_t)(readl(crt_reg + CRT_CLOCK_ENABLE1)));

	// Trigger USB clk (enable -> disable)
	clk_prepare_enable(clk_usb); // = clk_prepare + clk_enable
	dev_dbg(dev, "Realtek USB init 2/5 enable usb clk (CRT_SOFT_RESET1=%x, CRT_SOFT_RESET2=%x, CRT_CLOCK_ENABLE1=%x)\n",
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET1)),
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET2)),
		(uint32_t)(readl(crt_reg + CRT_CLOCK_ENABLE1)));

	clk_disable_unprepare(clk_usb); // = clk_disable + clk_unprepare
	mdelay(1);

	dev_dbg(dev, "Realtek USB init 3/5 disable usb clk (CRT_SOFT_RESET1=%x, CRT_SOFT_RESET2=%x, CRT_CLOCK_ENABLE1=%x)\n",
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET1)),
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET2)),
		(uint32_t)(readl(crt_reg + CRT_CLOCK_ENABLE1)));

	// Enable USB reset
	if (data->port0) {
		reset_control_deassert(reset_u3_phy0_mdio);
	}
	if (data->port3) {
		reset_control_deassert(reset_u3_phy1_mdio);
	}
	reset_control_deassert(reset_usb_apply);

	reset_control_deassert(reset_usb);
	mdelay(1);
	dev_dbg(dev, "Realtek USB init 4/5 Turn on RSTN_USB (CRT_SOFT_RESET1=%x, CRT_SOFT_RESET2=%x, CRT_CLOCK_ENABLE1=%x)\n",
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET1)),
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET2)),
		(uint32_t)(readl(crt_reg + CRT_CLOCK_ENABLE1)));

	// Enable USB clk
	clk_prepare_enable(clk_usb); // = clk_prepare + clk_enable
	mdelay(2);

	dev_info(dev, "Realtek USB init 5/5 OK (CRT_SOFT_RESET1=%x, CRT_SOFT_RESET2=%x, CRT_CLOCK_ENABLE1=%x)\n",
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET1)),
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET2)),
		(uint32_t)(readl(crt_reg + CRT_CLOCK_ENABLE1)));

    clk_put(clk_usb);

	return 0;
}

static int __usb_clear_clock_reset(struct manager_data* data) {
	struct device *dev = data->dev;
	void __iomem *crt_reg = data->crt_base;
	/* GET clock */
	struct clk *clk_usb = clk_get(NULL, "clk_en_usb");

	/* GET reset controller */
	struct reset_control *reset_phy0 = reset_control_get(dev, "u2phy0");
	struct reset_control *reset_phy1 = reset_control_get(dev, "u2phy1");
	struct reset_control *reset_phy2 = reset_control_get(dev, "u2phy2");
	struct reset_control *reset_phy3 = reset_control_get(dev, "u2phy3");
	struct reset_control *reset_u3_phy0 = reset_control_get(dev, "u3phy0");
	struct reset_control *reset_u3_phy1 = reset_control_get(dev, "u3phy1");
	struct reset_control *reset_u3_phy0_mdio = reset_control_get(dev, "u3mdio0");
	struct reset_control *reset_u3_phy1_mdio = reset_control_get(dev, "u3mdio1");
	struct reset_control *reset_usb = reset_control_get(dev, "usb");
	struct reset_control *reset_usb_apply = reset_control_get(dev, "apply");

	dev_dbg(dev, "Realtek USB init CRT_SOFT_RESET1=%x, CRT_SOFT_RESET2=%x, CRT_CLOCK_ENABLE1=%x\n",
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET1)),
		(uint32_t)(readl(crt_reg + CRT_SOFT_RESET2)),
		(uint32_t)(readl(crt_reg + CRT_CLOCK_ENABLE1)));

	reset_control_assert(reset_phy0);
	reset_control_assert(reset_phy1);
	reset_control_assert(reset_phy2);
	reset_control_assert(reset_phy3);
	reset_control_assert(reset_u3_phy0);
	reset_control_assert(reset_u3_phy1);
	reset_control_assert(reset_u3_phy0_mdio);
	reset_control_assert(reset_u3_phy1_mdio);
	reset_control_assert(reset_usb);

	clk_disable_unprepare(clk_usb); // = clk_disable + clk_unprepare

	clk_put(clk_usb);

	return 0;
}

static int __drd_type_c_power_on_off(struct manager_data *data, bool on) {
	struct device *dev = data->dev;
	int ret = 0;
	int gpio = data->type_c_pow_gpio;
		if (gpio_is_valid(gpio)) {
			if (gpio_direction_output(gpio, on)) {
				dev_err(dev, "%s ERROR set gpio fail\n", __func__);
				ret = -1;
			} else dev_info(dev, "%s type c power %s by gpio (id=%d) OK\n",
						__func__, on?"on":"off", gpio);
	}
	return ret;
}

int rtk_usb_type_c_power_on_off(struct device *type_c_dev, bool on) {
	static struct manager_data *s_data = NULL;
	struct device_node *node = NULL;
	struct platform_device *pdev = NULL;
	struct manager_data *data = NULL;
	int ret;

	if (s_data == NULL) {
		node = of_find_compatible_node(NULL, NULL, "Realtek,rtd129x-usb-power-manager");
		if (node != NULL)
			pdev = of_find_device_by_node(node);
		if (pdev != NULL)
			s_data = platform_get_drvdata(pdev);
	}
	data = s_data;
	if (data == NULL) {
		dev_err(data->dev, "%s ERROR no manager_data", __func__);
		return -ENODEV;
	}

	dev_dbg(data->dev, "%s power %s for %s", __func__,
			on?"on":"off", dev_name(type_c_dev));

	spin_lock(&data->lock);

	ret = __drd_type_c_power_on_off(data, on);

	spin_unlock(&data->lock);

	return ret;
}

static int rtk_usb_drd_gpio_power_on_off(struct manager_data *data, bool on) {
	struct device *dev = data->dev;
	int gpio = data->type_c_pow_gpio;
	struct device_node	*node = data->drd_node;
	struct device_node	*next_node;
	int dr_mode = USB_DR_MODE_UNKNOWN;

	if (node)
		next_node = of_get_next_child(node, NULL);
	if (next_node) {
		dr_mode = of_usb_get_dr_mode(next_node);
		next_node = of_get_next_child(node, next_node);
	}
	/* For type c port power on/off for type c driver disable */
	if (USB_DR_MODE_HOST == dr_mode &&
		    !of_device_is_available(next_node)) {
		/* To enable usb3.0 tx/rx switch */
		void __iomem *usb_typec_ctrl_cc1_0 = ioremap(0x9801334c, 0x4);
		writel(BIT(29), usb_typec_ctrl_cc1_0);
		__iounmap(usb_typec_ctrl_cc1_0);

		if (gpio_is_valid(gpio)) {
			if (gpio_direction_output(gpio, on))
				dev_err(dev, "%s ERROR set gpio fail\n", __func__);
			else
				dev_info(dev, "%s No type c driver power %s "
					    "by gpio (id=%d) OK\n",
					    __func__, on?"on":"off", gpio);
		}
	}
	return 0;
}

static int rtk_usb_u2host_gpio_power_on_off(struct manager_data *data, bool on) {
	struct device *dev = data->dev;
	int u2host_pow_gpio = data->u2host_pow_gpio;

	if (gpio_is_valid(u2host_pow_gpio)) {
		if (gpio_request(u2host_pow_gpio, "u2host_pow_gpio"))         //request gpio
			dev_err(dev, "%s ERROR Request u2host_pow_gpio (id=%d) fail\n",__func__, u2host_pow_gpio);
		else if (u2host_pow_gpio != -1) {
			if (gpio_direction_output(u2host_pow_gpio, on))
				dev_err(dev, "%s ERROR u2host power %s fail\n", __func__, on?"on":"off");
			gpio_free(u2host_pow_gpio);
			dev_dbg(dev, "%s to set u2host power %s gpio (id=%d) OK\n", __func__, on?"on":"off", u2host_pow_gpio);
		}
	}
	return 0;
}

static int rtk_usb_u3host_gpio_power_on_off(struct manager_data *data, bool on) {
	struct device *dev = data->dev;
	int u3host_pow_gpio = data->u3host_pow_gpio;

	if (gpio_is_valid(u3host_pow_gpio)) {
		if (gpio_request(u3host_pow_gpio, "u3host_pow_gpio"))         //request gpio
			dev_err(dev, "%s ERROR Request u3host_pow_gpio (id=%d) fail\n",__func__, u3host_pow_gpio);
		else if (u3host_pow_gpio != -1) {
			if (gpio_direction_output(u3host_pow_gpio, on))
				dev_err(dev, "%s ERROR u3host power %s fail\n", __func__, on?"on":"off");
			gpio_free(u3host_pow_gpio);
			dev_dbg(dev, "%s to set u3host power %s gpio (id=%d) OK\n", __func__, on?"on":"off", u3host_pow_gpio);
		}
	}
	// add workaround to enable QA board u2host power
	if (get_rtd129x_cpu_id() == RTK1296_CPU_ID) {
		int qa_borad_gpio19 = 19;
		if (gpio_is_valid(qa_borad_gpio19)) {
			if (gpio_request(qa_borad_gpio19, "qa_u2host_pow_gpio"))         //request gpio
				dev_err(dev, "%s ERROR Request QA u2host_pow_gpio (id=%d) fail\n",__func__, qa_borad_gpio19);
			else if (qa_borad_gpio19 != -1) {
				if (gpio_direction_output(qa_borad_gpio19, on))
					dev_err(dev, "%s ERROR QA u2host power %s fail\n", __func__, on?"on":"off");
				gpio_free(qa_borad_gpio19);
				dev_info(dev, "%s [Workaround] to set QA board u2host power %s gpio (id=%d) OK\n",
						__func__, on?"on":"off", qa_borad_gpio19);
			}
		}
	}
	return 0;
}

static int __usb_suspend_port_suspend(struct manager_data *data) {
	struct device *dev = data->dev;

	dev_info(dev, "%s", __func__);
	if (data->port0) {
		void __iomem *reg_u3_port = ioremap(0x980282c0, 0x4);
		void __iomem *reg_u2_port = ioremap(0x98028200, 0x4);
		dev_dbg(dev, "set dwc3_drd port suspend\n");
		writel(readl(reg_u3_port) | BIT(17), reg_u3_port);
		writel(readl(reg_u2_port) | BIT(6), reg_u2_port);
		iounmap(reg_u3_port);
		iounmap(reg_u2_port);
	}
	if (data->port1) {
		void __iomem *reg_u2_port = ioremap(0x98031200, 0x4);
		dev_dbg(dev, "set dwc3_u2host port suspend\n");
		writel(readl(reg_u2_port) | BIT(6), reg_u2_port);
		iounmap(reg_u2_port);
	}
	if (data->port2) {
		dev_info(dev, "TODO set ehci port suspend\n");
	}
	if (data->port3) {
		void __iomem *reg_u3_port = ioremap(0x981F82c0, 0x4);
		void __iomem *reg_u2_port = ioremap(0x981F8200, 0x4);
		dev_dbg(dev, "set dwc3_drd port suspend\n");
		writel(readl(reg_u3_port) | BIT(17), reg_u3_port);
		writel(readl(reg_u2_port) | BIT(6), reg_u2_port);
		iounmap(reg_u3_port);
		iounmap(reg_u2_port);
	}
	return 0;
}

static int __usb_resume_port_resume(struct manager_data *data) {
	struct device *dev = data->dev;

	dev_info(dev, "%s", __func__);
	if (data->port0) {
		void __iomem *reg_u3_port = ioremap(0x980282c0, 0x4);
		void __iomem *reg_u2_port = ioremap(0x98028200, 0x4);
		dev_dbg(dev, "set dwc3_drd port resume\n");
		writel(readl(reg_u3_port) & ~BIT(17), reg_u3_port);
		writel(readl(reg_u2_port) & ~BIT(6), reg_u2_port);
		iounmap(reg_u3_port);
		iounmap(reg_u2_port);
	}
	if (data->port1) {
		void __iomem *reg_u2_port = ioremap(0x98031200, 0x4);
		dev_dbg(dev, "set dwc3_u2host port resume\n");
		writel(readl(reg_u2_port) & ~BIT(6), reg_u2_port);
		iounmap(reg_u2_port);
	}
	if (data->port2) {
		dev_info(dev, "TODO set ehci port resume\n");
	}
	if (data->port3) {
		void __iomem *reg_u3_port = ioremap(0x981F82c0, 0x4);
		void __iomem *reg_u2_port = ioremap(0x981F8200, 0x4);
		dev_dbg(dev, "set dwc3_drd port resume\n");
		writel(readl(reg_u3_port) & ~BIT(17), reg_u3_port);
		writel(readl(reg_u2_port) & ~BIT(6), reg_u2_port);
		iounmap(reg_u3_port);
		iounmap(reg_u2_port);
	}
	return 0;
}

static int rtk_usb_suspend_power_off(struct manager_data *data) {
	struct device *dev = data->dev;
	bool off = false;

	dev_info(dev, "%s", __func__);
	if (data->port0) {
		rtk_usb_drd_gpio_power_on_off(data, off);
	}
	if (data->port1 || data->port2) {
		rtk_usb_u2host_gpio_power_on_off(data, off);
	}
	if (data->port3) {
		rtk_usb_u3host_gpio_power_on_off(data, off);
	}
	return 0;
}

static int rtk_usb_resume_power_on(struct manager_data *data) {
	struct device *dev = data->dev;
	bool on = true;

	dev_info(dev, "%s", __func__);
	if (data->port0) {
		rtk_usb_drd_gpio_power_on_off(data, on);
	}
	if (data->port1 || data->port2) {
		rtk_usb_u2host_gpio_power_on_off(data, on);
	}
	if (data->port3) {
		rtk_usb_u3host_gpio_power_on_off(data, on);
	}
	return 0;
}

int rtk_usb_init_power_on(struct device *usb_dev) {
	struct device_node *usb_node = usb_dev->of_node;
	struct device_node *node = of_find_compatible_node(NULL, NULL, "Realtek,rtd129x-usb-power-manager");
	struct platform_device *pdev = NULL;
	struct manager_data *data = NULL;
	bool on = true;

	if (node != NULL)
		pdev = of_find_device_by_node(node);
	if (pdev != NULL) {
		data = platform_get_drvdata(pdev);
	}
	if (data == NULL) {
		dev_err(data->dev, "%s ERROR no manager_data", __func__);
		return -ENODEV;
	}

	dev_info(data->dev, "%s for %s", __func__, dev_name(usb_dev));
	spin_lock(&data->lock);
	if (data->port0 && (data->drd_node->phandle == usb_node->phandle)) {
		dev_dbg(data->dev, "%s %s power on port 0", __func__, dev_name(usb_dev));
		rtk_usb_drd_gpio_power_on_off(data, on);
	}
	if (data->port1 || data->port2) {
		static int count = 0;
		if ((data->u2host_node->phandle == usb_node->phandle) ||
			(data->ehci_node->phandle == usb_node->phandle) ||
			(data->ohci_node->phandle == usb_node->phandle)) {
			count++;
		}
		if (count == (data->port1 + data->port2)) {
			dev_dbg(data->dev, "%s %s power on port 1 and port 2", __func__, dev_name(usb_dev));
			rtk_usb_u2host_gpio_power_on_off(data, on);
			count = 0;
		}
	}
	if (data->port3 && (data->u3host_node->phandle == usb_node->phandle)) {
		dev_dbg(data->dev, "%s %s power on port 3", __func__, dev_name(usb_dev));
		rtk_usb_u3host_gpio_power_on_off(data, on);
	}

	spin_unlock(&data->lock);
	return 0;
}

static int rtk_usb_gpio_init(struct manager_data *data) {
	struct device *dev = data->dev;
	int type_c_pow_gpio = data->type_c_pow_gpio;
	int u2host_pow_gpio = data->u2host_pow_gpio;
	int u3host_pow_gpio = data->u3host_pow_gpio;

	// drd Type C
	if (gpio_is_valid(type_c_pow_gpio)) {
		if (gpio_direction_output(type_c_pow_gpio, 0))
			dev_err(dev, "%s ERROR type_c-power-gpio fail\n", __func__);
		else dev_dbg(dev, "%s first to close type c power by gpio (id=%d) OK\n", __func__, type_c_pow_gpio);
	}

	// u2host and ehci
	if (gpio_is_valid(u2host_pow_gpio)) {
		if (gpio_request(u2host_pow_gpio, "u2host_pow_gpio"))         //request gpio
			dev_err(dev, "%s ERROR Request u2host_pow_gpio (id=%d) fail\n",__func__, u2host_pow_gpio);
		else if (u2host_pow_gpio != -1) {
			if (gpio_direction_output(u2host_pow_gpio, 0))
				dev_err(dev, "%s ERROR disable u2host power fail\n", __func__);
			gpio_free(u2host_pow_gpio);
			dev_dbg(dev, "%s to disable u2host power gpio (id=%d) OK\n", __func__, u2host_pow_gpio);
		}
	}

	// u3host
	if (gpio_is_valid(u3host_pow_gpio)) {
		if (gpio_request(u3host_pow_gpio, "u3host_pow_gpio"))         //request gpio
			dev_err(dev, "%s ERROR Request u3host_pow_gpio (id=%d) fail\n",__func__, u3host_pow_gpio);
		else if (u3host_pow_gpio != -1) {
			if (gpio_direction_output(u3host_pow_gpio, 0))
				dev_err(dev, "%s ERROR disable u3host power fail\n", __func__);
			gpio_free(u3host_pow_gpio);
			dev_dbg(dev, "%s to disable u3host power gpio (id=%d) OK\n", __func__, u3host_pow_gpio);
		}
	}
	return 0;
}

static int rtk_usb_init(struct manager_data *data) {
	struct device *dev = data->dev;

	dev_dbg(dev, "Realtek USB init ....\n");

    __rtk_usb_set_pd_power(data, 1);

	if (data->disable_usb) {
		dev_err(dev, "Realtek USB No any usb be enabled ....\n");
		return 0;
	}

	__rtk_usb_host_reset(data);

	__rtk_usb_set_hw_pm_enable(data);

	rtk_usb_gpio_init(data);

	dev_dbg(dev, "Realtek USB init Done\n");

	return 0;
}

#ifdef CONFIG_DYNAMIC_DEBUG
static int rtk_port0_power_show(struct seq_file *s, void *unused)
{
	struct manager_data		*data = s->private;

	seq_printf(s, "To Control Port 0 (DWC3 DRD power)\n");
	seq_printf(s, "echo \"on\" > port0_power\n");
	seq_printf(s, "echo \"off\" > port0_power\n");

	return 0;
}

static int rtk_port0_power_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_port0_power_show, inode->i_private);
}

static ssize_t rtk_port0_power_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct manager_data		*data = s->private;
	unsigned long		flags;
	char			buffer[40];
	char *buf = buffer;

	if (copy_from_user(&buffer, ubuf, min_t(size_t, sizeof(buffer) - 1, count)))
		return -EFAULT;

	spin_lock_irqsave(&data->lock, flags);
	if (!strncmp(buf, "on", 2)) {
		__drd_type_c_power_on_off(data, true);
	} else if (!strncmp(buf, "off", 3)) {
		__drd_type_c_power_on_off(data, false);
	} else
		dev_err(data->dev, "UNKNOWN input (%s)", buf);

	spin_unlock_irqrestore(&data->lock, flags);
	return count;
}

static const struct file_operations rtk_port0_power_fops = {
	.open			= rtk_port0_power_open,
	.write			= rtk_port0_power_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int rtk_port1_power_show(struct seq_file *s, void *unused)
{
	struct manager_data		*data = s->private;

	seq_printf(s, "To Control Port 1 (DWC3 u2host power)\n");
	seq_printf(s, "echo \"on\" > port1_power\n");
	seq_printf(s, "echo \"off\" > port1_power\n");

	return 0;
}

static int rtk_port1_power_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_port1_power_show, inode->i_private);
}

static ssize_t rtk_port1_power_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct manager_data		*data = s->private;
	unsigned long		flags;
	char			buffer[40];
	char *buf = buffer;

	if (copy_from_user(&buffer, ubuf, min_t(size_t, sizeof(buffer) - 1, count)))
		return -EFAULT;

	spin_lock_irqsave(&data->lock, flags);
	if (!strncmp(buf, "on", 2)) {
		rtk_usb_u2host_gpio_power_on_off(data, true);
	} else if (!strncmp(buf, "off", 3)) {
		rtk_usb_u2host_gpio_power_on_off(data, false);
	} else
		dev_err(data->dev, "UNKNOWN input (%s)", buf);

	spin_unlock_irqrestore(&data->lock, flags);
	return count;
}

static const struct file_operations rtk_port1_power_fops = {
	.open			= rtk_port1_power_open,
	.write			= rtk_port1_power_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int rtk_port2_power_show(struct seq_file *s, void *unused)
{
	struct manager_data		*data = s->private;

	seq_printf(s, "To Control Port 2 (EHCI/OHCI power)\n");
	seq_printf(s, "echo \"on\" > port2_power\n");
	seq_printf(s, "echo \"off\" > port2_power\n");

	return 0;
}

static int rtk_port2_power_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_port2_power_show, inode->i_private);
}

static const struct file_operations rtk_port2_power_fops = {
	.open			= rtk_port2_power_open,
	.write			= rtk_port1_power_write, /*Note port2 and port1 use same GPIO*/
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int rtk_port3_power_show(struct seq_file *s, void *unused)
{
	struct manager_data		*data = s->private;

	seq_printf(s, "To Control Port 3 (DWC3 u3host power)\n");
	seq_printf(s, "echo \"on\" > port3_power\n");
	seq_printf(s, "echo \"off\" > port3_power\n");

	return 0;
}

static int rtk_port3_power_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_port3_power_show, inode->i_private);
}

static ssize_t rtk_port3_power_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct manager_data		*data = s->private;
	unsigned long		flags;
	char			buffer[40];
	char *buf = buffer;

	if (copy_from_user(&buffer, ubuf, min_t(size_t, sizeof(buffer) - 1, count)))
		return -EFAULT;

	spin_lock_irqsave(&data->lock, flags);
	if (!strncmp(buf, "on", 2)) {
		rtk_usb_u3host_gpio_power_on_off(data, true);
	} else if (!strncmp(buf, "off", 3)) {
		rtk_usb_u3host_gpio_power_on_off(data, false);
	} else
		dev_err(data->dev, "UNKNOWN input (%s)", buf);

	spin_unlock_irqrestore(&data->lock, flags);
	return count;
}

static const struct file_operations rtk_port3_power_fops = {
	.open			= rtk_port3_power_open,
	.write			= rtk_port3_power_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int rtk_iso_mode_show(struct seq_file *s, void *unused)
{
	struct manager_data		*data = s->private;
	unsigned long		flags;

	spin_lock_irqsave(&data->lock, flags);

	seq_printf(s, "usb_iso_mode %s\n", data->usb_iso_mode?"Enable":"Disable");

	spin_unlock_irqrestore(&data->lock, flags);

	return 0;
}

static int rtk_iso_mode_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_iso_mode_show, inode->i_private);
}

static ssize_t rtk_iso_mode_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct manager_data		*data = s->private;
	unsigned long		flags;
	char			buffer[40];
	char *buf = buffer;

	if (copy_from_user(&buffer, ubuf, min_t(size_t, sizeof(buffer) - 1, count)))
		return -EFAULT;

	spin_lock_irqsave(&data->lock, flags);
	if (!strncmp(buf, "enable", 6)) {
		data->usb_iso_mode = true;
	} else if (!strncmp(buf, "disable", 7)) {
		data->usb_iso_mode = false;
	} else
		dev_err(data->dev, "UNKNOWN input (%s)", buf);

	spin_unlock_irqrestore(&data->lock, flags);
	return count;
}

static const struct file_operations rtk_iso_mode_fops = {
	.open			= rtk_iso_mode_open,
	.write			= rtk_iso_mode_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static u32 rtk_usb_reg_addr = 0;
static u32 rtk_usb_reg_value = 0;

static int rtk_usb_reg_show(struct seq_file *s, void *unused)
{
	struct manager_data		*data = s->private;
	void __iomem *addr;
	if (rtk_usb_reg_addr) {
		addr = ioremap(rtk_usb_reg_addr, 0x4);
		seq_printf(s, "Register Addr 0x%x = 0x%x\n", rtk_usb_reg_addr, readl(addr));

		iounmap(addr);
	} else {
		seq_printf(s, "read register: (1) echo \"0x98XXXXXX\" > reg (2) cat reg\n");
		seq_printf(s, "write register: (1) echo \"0x98XXXXXX value\" > reg (2) cat reg\n");
	}
	return 0;
}

static int rtk_usb_reg_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_usb_reg_show, inode->i_private);
}

static ssize_t rtk_usb_reg_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	struct seq_file		*s = file->private_data;
	struct manager_data		*data = s->private;
	unsigned long		flags;
	char			buffer[40] = "";
	char *buf = buffer;

	u32 addr, value;
	int ret;

	if (copy_from_user(&buffer, ubuf, min_t(size_t, sizeof(buffer) - 1, count)))
		return -EFAULT;

	if (!strncmp(buf, "0x98", 4)) {
		ret = sscanf(buf, "%x %x", &addr, &value);
		if (addr >= 0x98000000 && addr <= 0x98200000) {
			rtk_usb_reg_addr = addr;
			if (ret > 1) {
				void __iomem *v_addr = ioremap(rtk_usb_reg_addr, 0x4);
				rtk_usb_reg_value = value;
				writel(rtk_usb_reg_value, v_addr);
				iounmap(v_addr);
			}
		} else
			dev_err(data->dev, "Error: to set addr = 0x%x, value = 0x%x\n", addr, value);
	} else
		rtk_usb_reg_addr = 0;

	return count;
}

static const struct file_operations rtk_usb_reg_fops = {
	.open			= rtk_usb_reg_open,
	.write			= rtk_usb_reg_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int rtk_usb_debug_show(struct seq_file *s, void *unused)
{
	struct manager_data		*data = s->private;

	seq_printf(s, "To Enable/Disable Debug register\n");
	seq_printf(s, "echo \"enable\" > debug\n");
	seq_printf(s, "echo \"disable\" > debug\n");

	return 0;
}

static int rtk_usb_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, rtk_usb_debug_show, inode->i_private);
}

static ssize_t rtk_usb_debug_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	static struct dentry *reg_file = NULL;
	struct seq_file		*s = file->private_data;
	struct manager_data		*data = s->private;
	char			buf[32];

	if (copy_from_user(&buf, ubuf, min_t(size_t, sizeof(buf) - 1, count)))
		return -EFAULT;

	if (!strncmp(buf, "enable", 6)) {
		if (!reg_file)
		reg_file = debugfs_create_file("reg", S_IRUGO | S_IWUSR, data->debug_dir,
				data, &rtk_usb_reg_fops);
	} else if (!strncmp(buf, "disable", 7)) {
		debugfs_remove(reg_file);
		reg_file = NULL;
	}

	return count;
}

static const struct file_operations rtk_usb_debug_fops = {
	.open			= rtk_usb_debug_open,
	.write			= rtk_usb_debug_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static inline void create_debug_files(struct manager_data *data) {

	dev_err(data->dev, "%s", __func__);

	data->debug_dir = debugfs_create_dir("power_manager", usb_debug_root);
	if (!data->debug_dir) {
		dev_err(data->dev, "Error debug_dir is NULL", __func__);
		return;
	}

	if (data->port0
			&& !debugfs_create_file("port0_power", S_IRUGO | S_IWUSR,
					data->debug_dir, data, &rtk_port0_power_fops))
		goto file_error;

	if (data->port1
			&& !debugfs_create_file("port1_power", S_IRUGO | S_IWUSR,
					data->debug_dir, data, &rtk_port1_power_fops))
		goto file_error;

	if (data->port2
			&& !debugfs_create_file("port2_power", S_IRUGO | S_IWUSR,
					data->debug_dir, data, &rtk_port2_power_fops))
		goto file_error;

	if (data->port3
			&& !debugfs_create_file("port3_power", S_IRUGO | S_IWUSR,
					data->debug_dir, data, &rtk_port3_power_fops))
		goto file_error;

	if (!debugfs_create_file("usb_iso_mode", S_IRUGO | S_IWUSR,
					data->debug_dir, data, &rtk_iso_mode_fops))
		goto file_error;

	if (!debugfs_create_file("debug", S_IRUGO | S_IWUSR, data->debug_dir, data,
						&rtk_usb_debug_fops))
		goto file_error;

	return;

file_error:
	debugfs_remove_recursive(data->debug_dir);
}
#endif //CONFIG_DYNAMIC_DEBUG

static int rtk_usb_power_manager_probe(struct platform_device *pdev) {
	struct device		*dev = &pdev->dev;
	struct device_node	*node = dev->of_node;
	struct manager_data 	*data;
	unsigned int gpio;
	int ret = 0;
	unsigned long probe_time = jiffies;

	dev_info(dev, "ENTER %s", __func__);
	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->crt_base = of_iomap(pdev->dev.of_node, 0);
	if (data->crt_base == NULL) {
		dev_err(&pdev->dev, "error mapping memory for crt_base\n");
		ret = -EFAULT;
		goto err1;
	}
	data->dev = dev;

	if (node && of_device_is_available(node)) {
		gpio = of_get_named_gpio(node, "realtek,type_c-power-gpio", 0);

		if (gpio_is_valid(gpio)) {
			data->type_c_pow_gpio = gpio;
			dev_dbg(dev, "%s get type_c-power-gpio (id=%d) OK\n", __func__, gpio);
		 } else {
			data->type_c_pow_gpio = -1;
			dev_err(dev, "Error type_c-power-gpio no found");
		}

		gpio = of_get_named_gpio(node, "realtek,u2host-power-gpio", 0);
		if (gpio_is_valid(gpio)) {
			data->u2host_pow_gpio = gpio;
			dev_dbg(dev, "%s get u2host-power-gpio (id=%d) OK\n", __func__, gpio);
		 } else {
			data->u2host_pow_gpio = -1;
			dev_err(dev, "Error u2host-power-gpio no found");
		}
		gpio = of_get_named_gpio(node, "realtek,u3host-power-gpio", 0);
		if (gpio_is_valid(gpio)) {
			data->u3host_pow_gpio = gpio;
			dev_dbg(dev, "%s get u3host-power-gpio (id=%d) OK\n", __func__, gpio);
		 } else {
			data->u3host_pow_gpio = -1;
			dev_dbg(dev, " u3host-power-gpio no found");
		}
	}

	if (node && of_device_is_available(node)) {
		data->drd_node = of_parse_phandle(node, "port0", 0);
		data->u2host_node = of_parse_phandle(node, "port1", 0);
		data->ehci_node = of_parse_phandle(node, "port2", 0);
		data->ohci_node = of_parse_phandle(node, "port2", 1);
		data->u3host_node = of_parse_phandle(node, "port3", 0);

		data->disable_usb = true;
		data->port0 = 0;
		if (data->drd_node && of_device_is_available(data->drd_node)) {
			dev_info(dev, "%s status is okay", data->drd_node->name);
			data->port0++;
		}

		data->port1 = 0;
		if (data->u2host_node && of_device_is_available(data->u2host_node)) {
			dev_err(dev, "%s status is okay", data->u2host_node->name);
			data->port1++;
		}

		data->port2 = 0;
		if (data->ehci_node && of_device_is_available(data->ehci_node)) {
			dev_err(dev, "%s status is okay", data->ehci_node->name);
			data->port2++;
		}
		if (data->ohci_node && of_device_is_available(data->ohci_node)) {
			dev_err(dev, "%s status is okay", data->ohci_node->name);
			data->port2++;
		}

		data->port3 = 0;
		if (data->u3host_node && of_device_is_available(data->u3host_node)) {
			dev_err(dev, "%s status is okay", data->u3host_node->name);
			data->port3++;
		}
		if (data->port0 || data->port1 || data->port2 || data->port3) {
			data->disable_usb = false;
		}

		if (of_property_read_bool(node, "usb_iso_mode"))
			data->usb_iso_mode = true;
		else
			data->usb_iso_mode = false;

		if (of_property_read_bool(node, "usb_power_cut"))
			data->usb_power_cut = true;
		else
			data->usb_power_cut = false;

		if (of_property_read_bool(node, "en_usb_storage_reprobe"))
			data->en_usb_storage_reprobe = true;
		else
			data->en_usb_storage_reprobe = false;
	}

	rtk_usb_init(data);

	platform_set_drvdata(pdev, data);

	data->wq_usb_manager = create_singlethread_workqueue("rtk_usb_manager");

#ifdef CONFIG_DYNAMIC_DEBUG
	create_debug_files(data);
#endif

	dev_info(&pdev->dev, "%s OK (take %d ms)\n", __func__, jiffies_to_msecs(jiffies - probe_time));
	return 0;

err1:
	dev_err(&pdev->dev, "%s: Probe fail, %d\n", __func__, ret);

	return ret;
}

static int rtk_usb_power_manager_remove(struct platform_device *pdev) {
	dev_info(&pdev->dev, "%s\n", __func__);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id rtk_usb_power_manager_match[] = {
	{ .compatible = "Realtek,rtd129x-usb-power-manager" },
	{},
};
MODULE_DEVICE_TABLE(of, rtk_usb_power_manager_match);
#endif

#ifdef CONFIG_PM_SLEEP

static int rtk_usb_power_manager_prepare(struct device *dev) {
	struct manager_data *data = dev_get_drvdata(dev);
	int ret = 0;

	dev_info(dev, "[USB] Enter %s\n", __func__);
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY){
		//For idle mode
		dev_info(dev, "[USB] %s Idle mode\n", __func__);
	} else {
		dev_info(dev,  "[USB] %s Suspend mode\n", __func__);
	}

	dev_info(dev, "[USB] Exit %s\n", __func__);
	return ret;
}

static void rtk_usb_power_manager_complete(struct device *dev) {
	struct manager_data *data = dev_get_drvdata(dev);

	dev_info(dev, "[USB] Enter %s\n", __func__);
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY){
		//For idle mode
		dev_info(dev, "[USB] %s S1 (Standby mode)\n", __func__);
	} else {
		dev_info(dev,  "[USB] %s S3 (Suspend-to-RAM mode)\n", __func__);
		rtk_usb_resume_power_on(data);
	}
	dev_info(dev, "[USB] Exit %s\n", __func__);
}

static int rtk_usb_power_manager_suspend(struct device *dev) {
	struct manager_data *data = dev_get_drvdata(dev);

	dev_info(dev, "[USB] Enter %s\n", __func__);
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY){
		//For idle mode
		dev_info(dev, "[USB] %s S1 (Standby mode)\n", __func__);
	} else {
		//For suspend mode
		dev_info(dev,  "[USB] %s S3 (Suspend-to-RAM mode)\n", __func__);
		if (!data->usb_iso_mode) {
			rtk_usb_suspend_power_off(data);
			__rtk_usb_set_pd_power(data, 0);
		} else {
			__usb_suspend_port_suspend(data);
		}

	}
	dev_info(dev, "[USB] Exit %s\n", __func__);
	return 0;
}

static int rtk_usb_power_manager_resume(struct device *dev) {
	struct manager_data *data = dev_get_drvdata(dev);
	struct clk *clk_usb = clk_get(NULL, "clk_en_usb");

	dev_info(dev, "[USB] Enter %s\n", __func__);
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY){
		//For idle mode
		dev_info(dev, "[USB] %s S1 (Standby mode)\n", __func__);
	} else {
		//For suspend mode
		dev_info(dev, "[USB] %s S3 (Suspend-to-RAM mode)\n", __func__);

		if (!data->usb_iso_mode) {
			__rtk_usb_set_pd_power(data, 1);

			clk_disable_unprepare(clk_usb); // = clk_disable + clk_unprepare
			__rtk_usb_host_reset(data);

			__rtk_usb_set_hw_pm_enable(data);

			rtk_usb_gpio_init(data);
			/* rtk_usb_resume_power_on move to complete*/
			//rtk_usb_resume_power_on(data);
		} else {
			__usb_resume_port_resume(data);
		}
	}
	dev_info(dev, "[USB] Exit %s\n", __func__);
	return 0;
}

static const struct dev_pm_ops rtk_usb_power_manager_pm_ops = {
	.prepare = rtk_usb_power_manager_prepare,
	.complete = rtk_usb_power_manager_complete,
	SET_LATE_SYSTEM_SLEEP_PM_OPS(rtk_usb_power_manager_suspend, rtk_usb_power_manager_resume)
};

#define DEV_PM_OPS	(&rtk_usb_power_manager_pm_ops)
#else
#define DEV_PM_OPS	NULL
#endif /* CONFIG_PM_SLEEP */

static void rtk_usb_manager_shutdown(struct platform_device *pdev)
{
	struct device		*dev = &pdev->dev;
	struct manager_data *data = dev_get_drvdata(dev);

	dev_info(dev, "[USB] Enter %s S5 (shutdown)\n",
		    __func__);

	rtk_usb_gpio_init(data);
	__rtk_usb_set_pd_power(data, 0);
	__usb_clear_clock_reset(data);

	dev_info(dev, "[USB] Exit %s\n", __func__);
	return 0;
}

static struct platform_driver rtk_usb_power_manager_driver = {
	.probe		= rtk_usb_power_manager_probe,
	.remove		= rtk_usb_power_manager_remove,
	.driver		= {
		.name	= "rtk-usb-power-manager",
		.of_match_table = of_match_ptr(rtk_usb_power_manager_match),
		.pm = DEV_PM_OPS,
	},
	.shutdown = rtk_usb_manager_shutdown,
};

static int __init rtk_usb_power_manager_driver_init(void) {
	return platform_driver_register(&(rtk_usb_power_manager_driver));
}
subsys_initcall(rtk_usb_power_manager_driver_init);
//module_init(rtk_usb_power_manager_driver_init);

static void __exit rtk_usb_power_manager_driver_exit(void) {
	platform_driver_unregister(&(rtk_usb_power_manager_driver));
}
//module_exit(rtk_usb_power_manager_driver_exit);

MODULE_ALIAS("platform:rtk-usb-power-manager");
MODULE_LICENSE("GPL");

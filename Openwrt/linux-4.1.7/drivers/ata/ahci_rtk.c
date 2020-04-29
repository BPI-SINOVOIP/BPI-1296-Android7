/*
 * AHCI SATA platform driver
 *
 * Copyright 2004-2005  Red Hat, Inc.
 *   Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2010  MontaVista Software, LLC.
 *   Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/pm.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/libata.h>
#include <linux/ahci_platform.h>
#include "ahci.h"
#include <linux/delay.h>

#include <linux/switch.h>
#include <linux/of_address.h>
#include <linux/reset.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/suspend.h>
#include <soc/realtek/rtd129x_cpu.h>

#include <scsi/scsi_device.h>
#include "libata.h"

//#define HOTPLUG_TEST
#define FLOW_CHANGE

#define DRV_NAME_RTK "ahci_rtk"
#define HPC 0
#define HPC_PLUS 1
#define IC_PROCESS HPC_PLUS
#define CHK_STAT_FREQ 200    //ms

#define MAX_TXDRV_LV 8
#define MAX_PORT 2
#define RESET_NUM 3
struct reset_control *rstc[MAX_PORT][RESET_NUM];

void __iomem *base = NULL;
void __iomem *ukbase = NULL;

struct task_struct *rtk_sata_dev_task = NULL;
int RTK_SATA_DEV_FLAG = 0;

/*enum {
	status_init = 0,
	status_start,
	status_finish,
	status_done,
};*/
enum {
	init_host = 0,
	link_check,
};
#ifdef HOTPLUG_TEST
enum {
	power_off = 0,
	power_on,
};
#endif
struct ahci_port_data {
	unsigned int dev_detect;
	unsigned int speed;
	unsigned int phy_status;
	unsigned int rescan_flag;
	unsigned int txdrv_sel;
	unsigned int txdrv_tbl[6];
	struct reset_control *rstc[RESET_NUM];
	unsigned int reset_flag;
	void __iomem *port_reg;
};

struct rtk_ahci_dev {
	struct platform_device *pdev;
	struct device *dev;
	void __iomem *base;
	void __iomem *ukbase;

	unsigned int port_num;
	struct ahci_port_data *port[MAX_PORT];

	unsigned char swname[MAX_PORT][20];
	int poweren_io[MAX_PORT];
	int present_io[MAX_PORT];
	int hpd_irq[MAX_PORT];
	struct switch_dev sw[MAX_PORT];

	unsigned int speed_limit;
	unsigned int spread_spectrum;
	unsigned int powerctl_en;
	unsigned int rx_sensitivity;
	unsigned int host_en;
	unsigned int link_status;
#ifdef HOTPLUG_TEST
	unsigned int power_status;
	struct delayed_work work2;
#endif
	struct task_struct * task;
	struct delayed_work work;
	struct ahci_host_priv *hpriv;

	unsigned int loopback_mode;
	unsigned int loopback_mode_en;
	unsigned int chip_version;
};

struct rtk_ahci_dev *ahci_dev = NULL;

static const struct ata_port_info ahci_port_info = {
	.flags		= AHCI_FLAG_COMMON,
	.pio_mask	= ATA_PIO4,
	.udma_mask	= ATA_UDMA6,
	.port_ops	= &ahci_platform_ops,
};

static struct scsi_host_template ahci_platform_sht = {
	AHCI_SHT(DRV_NAME_RTK),
};

static const int TX_DRV_TABLE[MAX_TXDRV_LV*6] = {
	0x94a52011, 0x94a56011, 0x94a5a011, 0x385a2111, 0x385a6111, 0x385aa111, 	//max_500mV
	0x94a62011, 0x94a66011, 0x94a6a011, 0x486a2111, 0x486a6111, 0x486aa111, 	//max_573mV
	0x94a72011, 0x94a76011, 0x94a7a011, 0x587a2111, 0x587a6111, 0x587aa111, 	//max_683mV
	0x94aa2011, 0x94aa6011, 0x94aaa011, 0x88aa2111, 0x88aa6111, 0x88aaa111, 	//max_786mV
	0x94aa2011, 0x94aa6011, 0x94aaa011, 0x88aa2111, 0x88aa6111, 0x88aaa111, 	//max_786mV
	0x94aa2011, 0x94aa6011, 0x94aaa011, 0x88aa2111, 0x88aa6111, 0x88aaa111, 	//max_786mV
	0x94aa2011, 0x94aa6011, 0x94aaa011, 0x88aa2111, 0x88aa6111, 0x88aaa111, 	//max_786mV
	0x94aa2011, 0x94aa6011, 0x94aaa011, 0x88aa2111, 0x88aa6111, 0x88aaa111, 	//max_786mV
};

static const char *rst_name[MAX_PORT][20] = {
	{
//		"sata_func_exist_0",
//		"rstn_sata_phy_pow_0",
		"rstn_sata_0",
		"rstn_sata_phy_0",
		"rstn_sata_phy_pow_0",
	},
	{
//		"sata_func_exist_1",
//		"rstn_sata_phy_pow_1",
		"rstn_sata_1",	
		"rstn_sata_phy_1",
		"rstn_sata_phy_pow_1",
	},
};

static int rtk_check_phy_calibration(unsigned int port)
{
	void __iomem *chk_reg;
	unsigned int reg;
	unsigned int calibrate = 0;

	chk_reg = ioremap(0x980171e0, 0x1);
	reg = readl(chk_reg);
	if(port == 0)
		calibrate = reg & 0x1F;
	else if(port == 1)
		calibrate = (reg & 0x3E0) >> 5;
	printk("SATA calibrate = %d\n", calibrate);

	iounmap(chk_reg);
	return calibrate;
}

static void writel_delay(unsigned int value, void __iomem *address)
{
	writel(value, address);
	mdelay(1);
}

static void config_sata_phy(unsigned int port)
{
	void __iomem *base = ahci_dev->base;
	int i;

	writel_delay(port, base + 0xF64);
	
	writel_delay(0x00001111, base + 0xF60);
	writel_delay(0x00005111, base + 0xF60);
	writel_delay(0x00009111, base + 0xF60);
#if IC_PROCESS == HPC
	writel_delay(0x538E0411, base + 0xF60);
	writel_delay(0x538E4411, base + 0xF60);
	writel_delay(0x538E8411, base + 0xF60);

	writel_delay(0x3b6a0511, base + 0xF60);
	writel_delay(0x3b6a4511, base + 0xF60);
	writel_delay(0x3b6a8511, base + 0xF60);

	writel_delay(0xE0500111, base + 0xF60);
	writel_delay(0xE0504111, base + 0xF60);
	writel_delay(0xE04C8111, base + 0xF60);

	writel_delay(0x00110611, base + 0xF60);
	writel_delay(0x00114611, base + 0xF60);
	writel_delay(0x00118611, base + 0xF60);
	
	writel_delay(0xA6000A11, base + 0xF60);
	writel_delay(0xA6004A11, base + 0xF60);
	writel_delay(0xA6008A11, base + 0xF60);

	writel_delay(0x27FD8211, base + 0xF60);
	writel_delay(0xA6408A11, base + 0xF60);
	writel_delay(0x041BA611, base + 0xF60);
#else
	if(ahci_dev->spread_spectrum==0) {
		printk("[SATA] spread-spectrum disable\n");
		writel_delay(0x538E0411, base + 0xF60);
		writel_delay(0x538E4411, base + 0xF60);
		writel_delay(0x538E8411, base + 0xF60);
	} else {
		printk("[SATA] spread-spectrum enable\n");
		writel_delay(0x738E0411, base + 0xF60);
		writel_delay(0x738E4411, base + 0xF60);
		writel_delay(0x738E8411, base + 0xF60);

		writel_delay(0x35910811, base + 0xF60);
		writel_delay(0x35914811, base + 0xF60);
		writel_delay(0x35918811 , base + 0xF60);
		
		writel_delay(0x02342711, base + 0xF60);
		writel_delay(0x02346711, base + 0xF60);
		writel_delay(0x0234a711, base + 0xF60);
	}
	writel_delay(0x336a0511, base + 0xF60);
	writel_delay(0x336a4511, base + 0xF60);
	writel_delay(0x336a8511, base + 0xF60);

	writel_delay(0xE0700111, base + 0xF60);
    writel_delay(0xE05C4111, base + 0xF60);
    writel_delay(0xE04A8111, base + 0xF60);

	writel_delay(0x00150611, base + 0xF60);
    writel_delay(0x00154611, base + 0xF60);
    writel_delay(0x00158611, base + 0xF60);

	writel_delay(0xC6000A11, base + 0xF60);
	writel_delay(0xC6004A11, base + 0xF60);
	writel_delay(0xC6008A11, base + 0xF60);

	writel_delay(0x70000211, base + 0xF60);
	writel_delay(0x70004211, base + 0xF60);
	writel_delay(0x70008211, base + 0xF60);

	writel_delay(0xC6600A11, base + 0xF60);
	writel_delay(0xC6604A11, base + 0xF60);
	writel_delay(0xC6608A11, base + 0xF60);

	writel_delay(0x20041911, base + 0xF60);
	writel_delay(0x20045911, base + 0xF60);
	writel_delay(0x20049911, base + 0xF60);

	writel_delay(0x94aa2011, base + 0xF60);
	writel_delay(0x94aa6011, base + 0xF60);
	writel_delay(0x94aaa011, base + 0xF60);
#endif
//default
/*	writel_delay(0x17171511, base + 0xF60);
	writel_delay(0x17175511, base + 0xF60);
	writel_delay(0x17179511, base + 0xF60);
	
	writel_delay(0x77701611, base + 0xF60);
	writel_delay(0x77705611, base + 0xF60);
	writel_delay(0x77709611, base + 0xF60);

//all on
	writel_delay(0x00001511, base + 0xF60);
	writel_delay(0x00005511, base + 0xF60);
	writel_delay(0x00009511, base + 0xF60);
	
	writel_delay(0x00001611, base + 0xF60);
	writel_delay(0x00005611, base + 0xF60);
	writel_delay(0x00009611, base + 0xF60);
*/
//CMU en on
	writel_delay(0x17171511, base + 0xF60);
	writel_delay(0x17175511, base + 0xF60);
	writel_delay(0x17179511, base + 0xF60);
	
	writel_delay(0x07701611, base + 0xF60);
	writel_delay(0x07705611, base + 0xF60);
	writel_delay(0x07709611, base + 0xF60);

	writel_delay(0x40002a11, base + 0xF60);
	writel_delay(0x40006a11, base + 0xF60);
	writel_delay(0x4000aa11, base + 0xF60);
// for rx sensitivity
	if(ahci_dev->chip_version == RTD129x_CHIP_REVISION_A00 || ahci_dev->chip_version == RTD129x_CHIP_REVISION_A01) {
		writel_delay(0x72100911, base + 0xF60);
		writel_delay(0x72104911, base + 0xF60);
		writel_delay(0x72108911, base + 0xF60);
	} else if(ahci_dev->chip_version >= RTD129x_CHIP_REVISION_B00) {
		writel_delay(0x42100911, base + 0xF60);
		writel_delay(0x42104911, base + 0xF60);
		writel_delay(0x42108911, base + 0xF60);
	}
/*	if(ahci_dev->port[port]->phy_status==0) {
		writel_delay(0x27640311, base + 0xF60);
		writel_delay(0x27644311, base + 0xF60);
		writel_delay(0x27648311, base + 0xF60);
	} else if(ahci_dev->port[port]->phy_status==2) {
		writel_delay(0x27710311, base + 0xF60);
		writel_delay(0x27714311, base + 0xF60);
		writel_delay(0x27718311, base + 0xF60);
	}*/

	if(ahci_dev->chip_version == RTD129x_CHIP_REVISION_A00 || ahci_dev->chip_version == RTD129x_CHIP_REVISION_A01) {
		writel_delay(0x27750311, base + 0xF60);
		writel_delay(0x276e4311, base + 0xF60);
		writel_delay(0x276c8311, base + 0xF60);
	} else if(ahci_dev->chip_version >= RTD129x_CHIP_REVISION_B00) {
		if(rtk_check_phy_calibration(port) >= 0x16)
			writel_delay(0x27730311, base + 0xF60);
		else
			writel_delay(0x27710311, base + 0xF60);
		writel_delay(0x276d4311, base + 0xF60);
		writel_delay(0x276d8311, base + 0xF60);

		writel_delay(0x7c002a11, base + 0xF60);
		writel_delay(0x7c006a11, base + 0xF60);
		writel_delay(0x7c00aa11, base + 0xF60);
	}
	writel_delay(0x29001011, base + 0xF60);
	writel_delay(0x29005011, base + 0xF60);
	writel_delay(0x29009011, base + 0xF60);

	for(i=0; i<6; i++) {
		printk("[SATA] Port%d TX_DRV%d = 0x%x\n", port, i, ahci_dev->port[port]->txdrv_tbl[i]);
		writel_delay(ahci_dev->port[port]->txdrv_tbl[i], base + 0xF60);
	}
// RX power saving off
	writel_delay(0x40000C11, base + 0xF60);
	writel_delay(0x40004C11, base + 0xF60);
	writel_delay(0x40008C11, base + 0xF60);

	writel_delay(0x00271711, base + 0xF60);
	writel_delay(0x00275711, base + 0xF60);
	writel_delay(0x00279711, base + 0xF60);
}

static void config_sata_mac(unsigned int port)
{
	unsigned int val;
	void __iomem *base, *port_base;

	base = ahci_dev->base;
	port_base = ahci_dev->port[port]->port_reg;
	
	writel_delay(port, base + 0xF64);
	/* SATA MAC */
//	writel_delay(0x2, port_base + 0x144);
	writel_delay(0x6726ff81, base);
	val = readl(base);
	writel_delay(0x6737ff81, base);
	val = readl(base);

//	writel_delay(0x83090c15, base + 0xbc);
//	writel_delay(0x83090c15, base + 0xbc);
	
	writel_delay(0x80000001, base + 0x4);
	writel_delay(0x80000002, base + 0x4);

	val = readl(base + 0x14);
	writel_delay((val & ~0x1), base + 0x14);
	val = readl(base + 0xC);
	writel_delay((val | 0x3), base + 0xC);
	val = readl(base + 0x18);
	val |= port << 1;
	writel_delay(val, base + 0x18);

	writel_delay(0xffffffff, port_base + 0x114);
//	writel_delay(0x05040000, port_base + 0x100);
//	writel_delay(0x05040400, port_base + 0x108);

	val = readl(port_base + 0x170);
	writel_delay(0x88, port_base + 0x170);
	val = readl(port_base + 0x118);
	writel_delay(0x10, port_base + 0x118);
	val = readl(port_base + 0x118);
	writel_delay(0x4016, port_base + 0x118);
	val = readl(port_base + 0x140);
	writel_delay(0xf000, port_base + 0x140);
	
	writel_delay(0x3c300, base + 0xf20);
	if(ahci_dev->loopback_mode)
		writel_delay(0x9700, base + 0xA4);
	else
		writel_delay(0x700, base + 0xA4);
//Set to Auto mode
	if(ahci_dev->port[port]->speed == 0)
		writel_delay(0xA, base + 0xF68);
	else if(ahci_dev->port[port]->speed == 2)
		writel_delay(0x5, base + 0xF68);
	else if(ahci_dev->port[port]->speed == 1)
		writel_delay(0x0, base + 0xF68);

//	while(1) {
//		val = readl(port_base + 0x128);
//		printk(KERN_ERR "%s: reg base+0x128 = 0x%x\n", __func__, val);
//		msleep(200);
//	}
}

static int send_oob(unsigned int port)
{
	unsigned int val=0;

	if(port==0) {
		val = readl(ahci_dev->ukbase + 0x80);
		val |= 0x115;
	} else if(port==1) {
		val = readl(ahci_dev->ukbase + 0x80);
		val |= 0x12A;
	}
	writel(val, ahci_dev->ukbase + 0x80);

	return 0;
}

void set_rx_sensitivity(unsigned int port, unsigned int rx_sens)
{
	void __iomem *base, *port_base;
	unsigned int val;

	base = ahci_dev->base;
	port_base = ahci_dev->port[port]->port_reg;
	
	writel_delay(port, base + 0xF64);
	
	val = readl(port_base + 0x12C);
	val = val & ~0x1;
	writel_delay(val, port_base + 0x12c);

	if(rx_sens==0) {
		printk("[SATA] change rx_sensitivy to %d\n", rx_sens);
		writel_delay(0x27640311, base + 0xF60);
		writel_delay(0x27644311, base + 0xF60);
		writel_delay(0x27648311, base + 0xF60);
	} else if(rx_sens==2) {
		printk("[SATA] change rx_sensitivy to %d\n", rx_sens);
		writel_delay(0x27710311, base + 0xF60);
		writel_delay(0x27714311, base + 0xF60);
		writel_delay(0x27718311, base + 0xF60);
	}
	val = val | 0x1;
	writel_delay(val, port_base + 0x12c);
	val = val & ~0x1;
	writel_delay(val, port_base + 0x12c);

	val = val | 0x4;
	writel_delay(val, port_base + 0x12c);
	val = val & ~0x4;
	writel_delay(val, port_base + 0x12c);
}

int change_rx_sensitivity(unsigned int id)
{
	unsigned int port;
	if(id > ahci_dev->port_num) {
		printk("[SATA] this port is larger than port number");
		return -1;
	}
	
	port = id - 1;
	if(ahci_dev->port[port]->phy_status==0)
		ahci_dev->port[port]->phy_status = 2;
	else
		ahci_dev->port[port]->phy_status = 0;

	set_rx_sensitivity(port, ahci_dev->port[port]->phy_status);
	return 0;
}
EXPORT_SYMBOL_GPL(change_rx_sensitivity);
#if 0
static void set_speed_limit(unsigned int port, unsigned int speed)
{
	void __iomem *base, *port_base;
	base = ahci_dev->base;
	port_base = ahci_dev->port[port]->port_reg;
	
	writel_delay(port, base + 0xF64);
	if(speed == 2) {
		writel_delay(0x5, base + 0xF68);
		writel_delay(0x20, port_base + 0x12c);
		writel_delay(0x21, port_base + 0x12c);
		writel_delay(0x20, port_base + 0x12c);
	} else if(speed == 1) {
		writel_delay(0x0, base + 0xF68);
		writel_delay(0x10, port_base + 0x12c);
		writel_delay(0x11, port_base + 0x12c);
		writel_delay(0x10, port_base + 0x12c);
	} else {
		writel_delay(0xA, base + 0xF68);
		writel_delay(0x00, port_base + 0x12c);
		writel_delay(0x01, port_base + 0x12c);
		writel_delay(0x00, port_base + 0x12c);
	}
}

static int thread_status_check(void *data)
{
	int i, cnt=0;
	void __iomem *base, *port_base;
	struct platform_device *pdev = data;
	struct device *dev = &pdev->dev;
	struct ahci_host_priv *hpriv = ahci_dev->hpriv;
	int readynum=0, rc;
	unsigned int reg_link=0, reg_err=0, have_try[MAX_PORT];
	struct ata_host *host;

	set_current_state(TASK_INTERRUPTIBLE);
	
	while(!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);
		base = ahci_dev->base;
		msleep(CHK_STAT_FREQ);
		if(ahci_dev->link_status==status_init) {
			readynum = cnt = 0;
			for(i=0; i<ahci_dev->port_num; i++) {
				ahci_dev->port[i]->dev_detect = 0;
				ahci_dev->port[i]->speed = ahci_dev->speed_limit;
				have_try[i] = 0;
			}
			ahci_dev->link_status = status_start;
		}
		if(ahci_dev->link_status == status_start) {
			printk("[SATA] Start to check link status\n");
			for(i=0; i<ahci_dev->port_num; i++) {
				port_base = ahci_dev->port[i]->port_reg;
				if(ahci_dev->port[i]->dev_detect != 1) {
					reg_link = readl(port_base + 0x128);
					reg_err = readl(port_base + 0x130);
					if((reg_link&0xF)==0x3 && reg_err==0x04050002) {
						printk("[SATA] Port%d Link OK, status = 0x%x, rx_en=%d, speed=%d\n", i, reg_link, ahci_dev->port[i]->phy_status, ahci_dev->port[i]->speed);
						ahci_dev->port[i]->dev_detect = 1;
						readynum++;
					} else {
						printk("[SATA] Port%d link fail, rx_en=%d, speed=%d\n", i, ahci_dev->port[i]->phy_status, ahci_dev->port[i]->speed);
						if(!have_try[i]) {
							if(ahci_dev->port[i]->phy_status==0) {
								ahci_dev->port[i]->phy_status=1;
								set_rx_sensitivity(i, ahci_dev->port[i]->phy_status, ahci_dev->port[i]->speed);
							} else if(ahci_dev->port[i]->phy_status==1) {
								ahci_dev->port[i]->phy_status=0;
								set_rx_sensitivity(i, ahci_dev->port[i]->phy_status, ahci_dev->port[i]->speed);
							}
							have_try[i] = 1;
						} else {
							have_try[i] = 0;
							if(ahci_dev->port[i]->speed==0) {
								ahci_dev->port[i]->speed = 2;
								ahci_dev->port[i]->phy_status = ahci_dev->rx_sensitivity;
								set_rx_sensitivity(i, ahci_dev->port[i]->phy_status, ahci_dev->port[i]->speed);
		//						set_speed_limit(i, ahci_dev->port[i]->speed);
							} else
								readynum++;
							
		//					else if(ahci_dev->port[i]->speed==2) {
		//						ahci_dev->port[i]->speed = 1;
		//						ahci_dev->port[i]->phy_status = ahci_dev->rx_sensitivity;
		//						set_rx_sensitivity(i, ahci_dev->port[i]->phy_status, ahci_dev->port[i]->speed);
		//						set_speed_limit(i, ahci_dev->port[i]->speed);
		//					} else
		//						readynum++;
						}
					}
				}
			}
			if(readynum == ahci_dev->port_num) {
				printk("[SATA] Phy link finish\n");
				ahci_dev->link_status = status_finish;
			}
		}
		if(ahci_dev->link_status == status_finish) {
			printk("[SATA] Link finish\n");
			host = dev_get_drvdata(ahci_dev->dev);
			if(host == NULL) {
				printk("[SATA] No host!! start initial\n");
				ahci_platform_init_host(pdev, hpriv, &ahci_port_info, &ahci_platform_sht);
			} else {
				printk("[SATA] Have host!! resume host\n");
				rc = ahci_platform_resume_host(dev);
				if (rc)
					printk(KERN_ERR "[SATA] Can't resume host\n");
			}
			ahci_dev->link_status = status_done;
		}	
	}
	return 0;
}
#endif

#ifdef HOTPLUG_TEST
static void rtk_ahci_power(struct work_struct *work)
{
	if(ahci_dev->power_status) {
		printk(KERN_ERR "[SATA] Power Off SATA\n");
		ahci_dev->power_status = power_off;
		gpio_set_value(ahci_dev->poweren_io[0], 0);
		schedule_delayed_work(&ahci_dev->work2, 1500);
	} else {
		printk(KERN_ERR "[SATA] Power on SATA\n");
		ahci_dev->power_status = power_on;
		gpio_set_value(ahci_dev->poweren_io[0], 1);
		schedule_delayed_work(&ahci_dev->work2, 3000);
	}
}
#endif

static void rtk_ahci_host_init(struct work_struct *work)
{
	struct platform_device *pdev = ahci_dev->pdev;
	struct ahci_host_priv *hpriv = ahci_dev->hpriv;
	struct ata_host *host;
	struct ata_port *ap;
	int i;

	if(ahci_dev->loopback_mode)
		return;

	host = dev_get_drvdata(ahci_dev->dev);

	if(ahci_dev->link_status == init_host) {
		ahci_platform_init_host(pdev, hpriv, &ahci_port_info, &ahci_platform_sht);
		ahci_dev->link_status = link_check;
		schedule_delayed_work(&ahci_dev->work, 2000);
	} else {
		for(i=0; i<ahci_dev->port_num; i++) {
			ap = host->ports[i];
			if(ahci_dev->port[i]->reset_flag==0) {
				reset_control_deassert(ahci_dev->port[i]->rstc[2]);
				ahci_dev->port[i]->reset_flag = 1;
			}
			mdelay(5);
			if(!ap->link.sata_spd && !ap->hotplug_flag) {
				if(ahci_dev->port[i]->reset_flag) {
					reset_control_assert(ahci_dev->port[i]->rstc[2]);
					ahci_dev->port[i]->reset_flag = 0;
				}
			}
		}
		schedule_delayed_work(&ahci_dev->work, 200);
	}
}

void rtk_scsi_dev_rescan(struct work_struct *work)
{
	struct ata_port *ap =
		container_of(work, struct ata_port, scsi_userscan_task.work);
	int cnt;

	if(ahci_dev->port[ap->port_no]->rescan_flag == 1)
		return;
	cnt = 0;
	while(1) {
		if(ap->scsi_host->shost_state == SHOST_RUNNING )
			break;
		else {
			cnt++;
			if(cnt>=5)
				break;
		}
		msleep(200);
	}
	ata_scsi_user_scan(ap->scsi_host, 0, 0, 0);
	ahci_dev->port[ap->port_no]->rescan_flag = 1;
}

static int rtk_sata_dev_fun(void *data)
{
	struct ata_host *host;
	struct scsi_device *sdev;
	int i;

	set_current_state(TASK_INTERRUPTIBLE);
	while(!kthread_should_stop()){
		schedule();
		host = dev_get_drvdata(ahci_dev->dev);
		if(host != NULL) {
			if(RTK_SATA_DEV_FLAG==2) {
				for(i=0; i<ahci_dev->port_num; i++) {
					if(host->ports[i]->scsi_host->shost_state == SHOST_RUNNING ) {
						printk("[SATA] scsi_remove_device start\n");
						sdev = host->ports[i]->link.device[0].sdev;
						if(sdev != NULL) {
							scsi_remove_device(sdev);
							ahci_dev->port[i]->rescan_flag = 0;
						}
					}
				}
			}
		}
		set_current_state(TASK_INTERRUPTIBLE);
	}

	return 0;
}

static irqreturn_t rtk_satahpd_isr(int irq, void *dev_id)
{
	int port, ret;
	if(irq == ahci_dev->hpd_irq[0])
		port = 0;
	else if(irq == ahci_dev->hpd_irq[1])
		port = 1;
	else
		return IRQ_HANDLED;
	ret = gpio_get_value(ahci_dev->present_io[port]);
	gpio_set_value(ahci_dev->poweren_io[port], !ret);
	
	return IRQ_HANDLED;
}
static ssize_t switch_present_print_state(struct switch_dev *sdev, char *buffer)
{
	int ret = 0;
	if(!strcmp(sdev->name, "SATA0_present")) {
		if(ahci_dev->present_io[0]>=0) {
			ret = gpio_get_value(ahci_dev->present_io[0]);
			switch_set_state(sdev, ret);
		}
	}
	if(!strcmp(sdev->name, "SATA1_present")) {
		if(ahci_dev->present_io[1]>=0) {
			ret = gpio_get_value(ahci_dev->present_io[1]);
			switch_set_state(sdev, ret);
		}
	}
	return sprintf(buffer, "%d\n", ret);
}

void start_link(int port)
{
#ifdef FLOW_CHANGE
	if(port>=ahci_dev->port_num) {
		printk(KERN_ERR "[SATA] port number not support\n");
		return;
	}
	reset_control_deassert(ahci_dev->port[port]->rstc[2]);
	send_oob(port);
#endif
}
EXPORT_SYMBOL_GPL(start_link);

static int ahci_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct ahci_host_priv *hpriv;
	struct ahci_port_data *port_data;
	struct device_node *child;
	int child_node;
	int ret, i, j, size;
	const u32 *prop;
	
	ahci_dev = devm_kzalloc(dev, sizeof(struct rtk_ahci_dev), GFP_KERNEL);
	if(!ahci_dev) {
		dev_err(dev, "can't allocate struct ahci_dev\n");
		return -ENOMEM;
	}
	RTK_SATA_DEV_FLAG = 0;
	ahci_dev->pdev = pdev;
	ahci_dev->dev = &pdev->dev;

	ahci_dev->chip_version = get_rtd129x_cpu_revision();

	ahci_dev->base = of_iomap(pdev->dev.of_node, 0);
	if (!ahci_dev->base) {
		dev_err(dev, "no mmio space(SATA host)\n");
		return -EINVAL;
	}

	ahci_dev->ukbase = of_iomap(pdev->dev.of_node, 1);
	if (!ahci_dev->ukbase) {
		dev_err(dev, "no mmio space(ukbase)\n");
		return -EINVAL;
	}

	//Get port number from device tree
	child_node = of_get_child_count(dev->of_node);
	ahci_dev->port_num = child_node;
	if(!ahci_dev->port_num)
		ahci_dev->port_num = 1;
	else if(ahci_dev->port_num > MAX_PORT)
		ahci_dev->port_num = MAX_PORT;
	

	//Get reset information
	for(i=0; i<ahci_dev->port_num; i++) {
		port_data = devm_kzalloc(dev, sizeof(struct ahci_port_data), GFP_KERNEL);
		if(!port_data) {
			dev_err(dev, "can't allocate struct ahci_port_data\n");
			return -ENOMEM;
		}
		for(j=0; j<RESET_NUM; j++) {
			port_data->rstc[j] = reset_control_get(&pdev->dev, rst_name[i][j]);
			if(!port_data->rstc[j]) {	
				dev_err(dev, "can't not get reset\n");
				return -EINVAL;
			}
		}
		ahci_dev->port[i] = port_data;
		ahci_dev->port[i]->port_reg = ahci_dev->base + i*0x80;
		ahci_dev->port[i]->rescan_flag = 0;
	}

	ahci_dev->powerctl_en = 0;
	of_property_read_u32(dev->of_node, "powerctl-en", &ahci_dev->powerctl_en);

	if(child_node) {
		for_each_child_of_node(dev->of_node, child) {
			of_property_read_u32(child, "reg", &i);
			if(i < MAX_PORT) {
				ahci_dev->port[i]->txdrv_sel = 3;
				of_property_read_u32(child, "tx-driving", &ahci_dev->port[i]->txdrv_sel);
				if(ahci_dev->port[i]->txdrv_sel > (MAX_TXDRV_LV-1))
					ahci_dev->port[i]->txdrv_sel = MAX_TXDRV_LV-1;
				prop = of_get_property(child, "tx-driving-tbl", &size);
				if(prop!=NULL) {
					size /= sizeof(unsigned int);
					if(size == 6) {
						for(j=0; j<size; j++)
							ahci_dev->port[i]->txdrv_tbl[j] = of_read_number(prop+j, 1);
						ahci_dev->port[i]->txdrv_sel = MAX_TXDRV_LV;
					} else
						printk(KERN_ERR "[SATA] tx driving table size error\n");
				}
				if(ahci_dev->port[i]->txdrv_sel!=(MAX_TXDRV_LV)) {
					for(j=0; j<6; j++)
						 ahci_dev->port[i]->txdrv_tbl[j] = TX_DRV_TABLE[(ahci_dev->port[i]->txdrv_sel*6)+j]; 
				}

				ahci_dev->poweren_io[i] = of_get_gpio(child, 0);
				if(ahci_dev->poweren_io[i]<0) {
					dev_err(dev, "can't find gpio to enable sata power\n");
					continue;		
				}
				gpio_request(ahci_dev->poweren_io[i], child->name);
#ifdef HOTPLUG_TEST
				ahci_dev->power_status = power_on;
#endif
				if(ahci_dev->powerctl_en == 0)
					gpio_set_value(ahci_dev->poweren_io[i], 1);
				gpio_free(ahci_dev->poweren_io[i]);
				ahci_dev->present_io[i] = of_get_gpio(child, 1);
				if(ahci_dev->present_io[i]<0)
					continue;
				gpio_request(ahci_dev->present_io[i], child->name);
				gpio_direction_input(ahci_dev->present_io[i]);

				if(ahci_dev->powerctl_en==0) {
					sprintf(ahci_dev->swname[i], "SATA%d_present", i);
					ahci_dev->sw[i].name = ahci_dev->swname[i];
					ahci_dev->sw[i].print_state = switch_present_print_state;
					switch_dev_register(&ahci_dev->sw[i]);
				} else {
					ahci_dev->hpd_irq[i] = irq_of_parse_and_map(child, 0);
					irq_set_irq_type(ahci_dev->hpd_irq[i], IRQ_TYPE_EDGE_BOTH);
					if(request_irq(ahci_dev->hpd_irq[i], rtk_satahpd_isr, IRQF_SHARED, "SATA_HPD", &ahci_dev))
						dev_err(dev, "can't register hpd irq\n");
					ret = gpio_get_value(ahci_dev->present_io[i]);
					gpio_set_value(ahci_dev->poweren_io[i], !ret);
				}
			}
		}
	}

	ahci_dev->spread_spectrum = 0;
	ahci_dev->rx_sensitivity = 0;
	ahci_dev->speed_limit = 0;
	ahci_dev->loopback_mode_en = 0;
//	of_property_read_u32(dev->of_node, "hotplug-en", &ahci_dev->hotplug_en);
	of_property_read_u32(dev->of_node, "rx-sensitivity", &ahci_dev->rx_sensitivity);
	of_property_read_u32(dev->of_node, "spread-spectrum", &ahci_dev->spread_spectrum);
	of_property_read_u32(dev->of_node, "speed-limit", &ahci_dev->speed_limit);
	of_property_read_u32(dev->of_node, "loopback-mode", &ahci_dev->loopback_mode_en);

	hpriv = ahci_platform_get_resources(pdev);
	if (IS_ERR(hpriv))
		return PTR_ERR(hpriv);

	ahci_dev->hpriv = hpriv;
	ret = ahci_platform_enable_resources(hpriv);
	if (ret)
		return ret;

	for(i=0; i<ahci_dev->port_num; i++) {
		reset_control_deassert(ahci_dev->port[i]->rstc[0]);
		reset_control_deassert(ahci_dev->port[i]->rstc[1]);
	}

	for(i=0; i<ahci_dev->port_num; i++) {
		ahci_dev->port[i]->phy_status = ahci_dev->rx_sensitivity;
		ahci_dev->port[i]->speed = ahci_dev->speed_limit;
		config_sata_mac(i);
		config_sata_phy(i);
		ahci_dev->port[i]->reset_flag = 1;
#ifndef FLOW_CHANGE
		reset_control_deassert(ahci_dev->port[i]->rstc[2]);
		send_oob(i);
#endif
	}

//	if (of_device_is_compatible(dev->of_node, "Realtek,ahci-sata"))
//		hpriv->flags |= AHCI_HFLAG_NO_FBS | AHCI_HFLAG_NO_NCQ;
	ahci_dev->link_status = init_host;
//	ahci_dev->task = kthread_run(thread_status_check, pdev, "sata_status_check");
	INIT_DELAYED_WORK(&ahci_dev->work, rtk_ahci_host_init);
	dev_info(dev, "[SATA] mac init finish\n");
	schedule_delayed_work(&ahci_dev->work, 350);
#ifdef HOTPLUG_TEST
	INIT_DELAYED_WORK(&ahci_dev->work2, rtk_ahci_power);
	schedule_delayed_work(&ahci_dev->work2, 1500);
#endif
	rtk_sata_dev_task = kthread_run(rtk_sata_dev_fun, &pdev, "rtk_sata_dev_handler");
	if (IS_ERR(rtk_sata_dev_task)) {
		ret = PTR_ERR(rtk_sata_dev_task);
		rtk_sata_dev_task = NULL;
		return -1;
	}

	return 0;
}

int rtk_enter_loopback_mode(struct ata_port *ap)
{
	struct device *dev;
	struct ata_host *host;
	struct ahci_host_priv *hpriv;
	int rc, i, j, port;
	void __iomem *base, *port_base;
	unsigned int val, retry;

	if(!ahci_dev->loopback_mode_en)
		return 0;

	base = ahci_dev->base;
	port = ap->port_no;
	if(port >= ahci_dev->port_num) {
		printk(KERN_ERR "no port\n");
		return 0;
	}
	port_base = ahci_dev->port[port]->port_reg;

	dev = ahci_dev->dev;
	host = dev_get_drvdata(dev);
	hpriv = host->private_data;
	ahci_dev->loopback_mode = 1;

	printk(KERN_ERR "Port%d : %s\n", port, __FUNCTION__);

	ahci_stop_engine(ap);
	rc = ahci_platform_suspend_host(dev);

	ahci_platform_disable_resources(hpriv);

	for(i=0; i<ahci_dev->port_num; i++)
		for(j=0; j<RESET_NUM; j++)
			reset_control_assert(ahci_dev->port[i]->rstc[j]);

	ahci_platform_enable_resources(hpriv);

	for(i=0; i<ahci_dev->port_num; i++) {
		reset_control_deassert(ahci_dev->port[i]->rstc[0]);
		reset_control_deassert(ahci_dev->port[i]->rstc[1]);
	}
	config_sata_mac(port);
	config_sata_phy(port);
	reset_control_deassert(ahci_dev->port[port]->rstc[2]);
	ahci_platform_suspend_host(dev);
	send_oob(port);

	while(1) {
		val = readl(port_base + 0x128);
		if(val==0x133 || val==0x113 || val==0x123) {
			printk(KERN_ERR "link ok, Gen%d\n", (val&0x30)>>4);
			break;
		}
	}
	if(port==0)
		writel(0, base + 0xf4);
	else if(port==1)
		writel(0x00010000, base + 0xf4);
	val = readl(port_base + 0x118);
	val = val | 0x8;
	writel(val, port_base + 0x118);
	val = val & ~0x8;
	val = val | 0x1;
	writel(val, port_base + 0x118);

	retry = 0;
	while(1) {
		val = readl(base + 0xA0);
		if(val == 0x7E10) {
			printk(KERN_ERR "Port%d : Enter loopback mode success\n", port);
			break;
		} else {
			printk(KERN_ERR "Port%d : check register value, 0x9803f0a0 = 0x%x\n", port, val);
			retry++;
		}
		if(retry>=20) {
			printk(KERN_ERR "Port%d : Enter loopback mode fail\n", port);
			break;
		}
		msleep(500);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(rtk_enter_loopback_mode);

static int rtk_ahci_suspend(struct device *dev)
{
	struct ata_host *host;
	struct ahci_host_priv *hpriv;
	int rc, i, j;

	printk("[SATA] enter %s\n", __FUNCTION__);

//	if(!ahci_dev->host_en)
//		goto exit_suspend;
//	if(ahci_dev->link_status != status_done)
//		ahci_dev->link_status = status_done;

	host = dev_get_drvdata(dev);
	hpriv = host->private_data;

	rc = ahci_platform_suspend_host(dev);
	if (rc)
		return rc;
	if(RTK_PM_STATE == PM_SUSPEND_STANDBY) {
		ahci_platform_disable_clks(hpriv);
	} else {
		ahci_platform_disable_resources(hpriv);
		for(i=0; i<ahci_dev->port_num; i++)
			for(j=0; j<RESET_NUM; j++)
				reset_control_assert(ahci_dev->port[i]->rstc[j]);
	}

	printk("[SATA] exit %s\n", __FUNCTION__);
	return 0;
}

static int rtk_ahci_resume(struct device *dev)
{
	struct ata_host *host;
	struct ahci_host_priv *hpriv;
	struct device_node *child;
	int rc, i;

	printk("[SATA] enter %s\n", __FUNCTION__);

//	if(!ahci_dev->host_en)
//		goto exit_resume;

	host = dev_get_drvdata(dev);
	hpriv = host->private_data;
	
	RTK_SATA_DEV_FLAG = 1;

	for_each_child_of_node(dev->of_node, child) {
		of_property_read_u32(child, "reg", &i);
		if(i < MAX_PORT) {
			ahci_dev->poweren_io[i] = of_get_gpio(child, 0);
			if(ahci_dev->poweren_io[i]<0)
				dev_err(dev, "can't find gpio to enable sata power\n");
			gpio_request(ahci_dev->poweren_io[i], child->name);
			if(ahci_dev->powerctl_en == 0)
				gpio_set_value(ahci_dev->poweren_io[i], 1);
			gpio_free(ahci_dev->poweren_io[i]);
		}
	}

	if(RTK_PM_STATE == PM_SUSPEND_STANDBY) {
		ahci_platform_enable_clks(hpriv);
	} else {
		rc = ahci_platform_enable_resources(hpriv);
		if (rc)
			return rc;

		for(i=0; i<ahci_dev->port_num; i++) {
			reset_control_deassert(ahci_dev->port[i]->rstc[0]);
			reset_control_deassert(ahci_dev->port[i]->rstc[1]);
		}

		for(i=0; i<ahci_dev->port_num; i++) {
			config_sata_mac(i);
			config_sata_phy(i);
#ifndef FLOW_CHANGE
			reset_control_deassert(ahci_dev->port[i]->rstc[2]);
			send_oob(i);
#endif
		}
	}
	rc = ahci_platform_resume_host(dev);
	if (rc)
		goto disable_resources;

	/* We resumed so update PM runtime state */
	pm_runtime_disable(dev);
	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);

	printk("[SATA] exit %s\n", __FUNCTION__);
	return 0;

disable_resources:
	ahci_platform_disable_resources(hpriv);

	return rc;
}
//static SIMPLE_DEV_PM_OPS(ahci_pm_ops, ahci_platform_suspend, ahci_platform_resume);
static SIMPLE_DEV_PM_OPS(ahci_pm_ops, rtk_ahci_suspend, rtk_ahci_resume);

static const struct of_device_id ahci_of_match[] = {
	{ .compatible = "Realtek,ahci-sata", },
	{},
};
MODULE_DEVICE_TABLE(of, ahci_of_match);

static struct platform_driver ahci_driver = {
	.probe = ahci_probe,
	.remove = ata_platform_remove_one,
	.driver = {
		.name = DRV_NAME_RTK,
		.of_match_table = ahci_of_match,
		.pm = &ahci_pm_ops,
	},
};
module_platform_driver(ahci_driver);

MODULE_DESCRIPTION("AHCI SATA platform driver");
MODULE_AUTHOR("Anton Vorontsov <avorontsov@ru.mvista.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:ahci");

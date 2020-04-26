/*
 * rtd129x_barrier.c - bus barrier
 *
 * Copyright (c) 2017 Realtek Semiconductor Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/io.h>
#include <linux/memblock.h>
#include <linux/delay.h>
#include <linux/printk.h>
#include <asm/io.h>

#include <soc/realtek/memory.h>
#include <soc/realtek/barrier.h>

#define RBUS_SYNC	0x0001A020

int rtk_rbus_barrier_flag = 0;

extern void __iomem *rbus_addr;

void rtk_bus_sync(void)
{
	if(rtk_rbus_barrier_flag == 1){
		writel_relaxed(0x00001234, rbus_addr + RBUS_SYNC);
	}
	dmb(ishst);
}
EXPORT_SYMBOL(rtk_bus_sync);

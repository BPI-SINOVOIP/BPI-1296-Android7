/*
 * rtd129x_memory_remap.c
 *
 * Copyright (c) 2017 Realtek Semiconductor Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#include <linux/atomic.h>
#include <linux/dmi.h>
#include <linux/efi.h>
#include <linux/export.h>
#include <linux/memblock.h>
#include <linux/mm_types.h>
#include <linux/bootmem.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_address.h>
#include <linux/preempt.h>
#include <linux/rbtree.h>
#include <linux/rwsem.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/init.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/mmu_context.h>
#include <asm/mmu.h>
#include <asm/pgtable.h>

#include <soc/realtek/memory.h>
#include <linux/io.h>

#ifdef CONFIG_RTK_RPC
extern volatile void __iomem *rpc_ringbuf_base;
extern volatile void __iomem *rpc_common_base;
#endif /* CONFIG_RTK_RPC */

#ifdef CONFIG_RTK_RBUS_BARRIER
extern void __iomem *rbus_addr;
extern int rtk_rbus_barrier_flag;
#endif /* CONFIG_RTK_RBUS_BARRIER */

extern struct of_device_id __rtk_mem_remap_of_table[];

static const struct of_device_id __rtk_mem_remap_of_table_sentinel
	__used __section(__rtk_mem_remap_of_table_end);

#ifdef CONFIG_RTK_RBUS_BARRIER
static void rtk_get_rbus_addr(struct device_node *np)
{
	rbus_addr = of_iomap(np, 0);
    if (rbus_addr)
	rtk_rbus_barrier_flag = 1;
}
#endif /* CONFIG_RTK_RBUS_BARRIER */

#define REALTEK_MAP_OF_DECLARE(name, compat, fn) \
	OF_DECLARE_1(rtk_mem_remap, name, compat, fn)

static void rtk_mem_remap_init(struct device_node *np)
{
#ifdef CONFIG_RTK_RBUS_BARRIER
	rtk_get_rbus_addr(np);
#endif /* CONFIG_RTK_RBUS_BARRIER */

#ifdef CONFIG_RTK_RPC
#if defined(CONFIG_ION_RTK_PHOENIX) || defined(CONFIG_XEN)
	rpc_ringbuf_base = (void *)(u64)of_iomap(np, 5);
#else
	rpc_ringbuf_base = (void *)(u64)of_iomap(np, 4);
#endif /* defined(CONFIG_ION_RTK_PHOENIX) || defined(CONFIG_XEN) */

	rpc_common_base = (void *)(u64)of_iomap(np, 1);
#endif /* CONFIG_RTK_RPC */

	printk(KERN_INFO "[RTK_MEM_REMAP] initialized\n");
}

static void rtk_mem_remap_parent_init(struct device_node *np)
{
    struct device_node *child;
#ifdef CONFIG_RTK_RBUS_BARRIER
    child = of_get_child_by_name(np, "rbus");
    if (child)
        rbus_addr = of_iomap(child, 0);

    if (rbus_addr)
        rtk_rbus_barrier_flag = 1;
#endif /* CONFIG_RTK_RBUS_BARRIER */

#ifdef CONFIG_RTK_RPC
    child = of_get_child_by_name(np, "ringbuf");
    if (child)
        rpc_ringbuf_base = of_iomap(child, 0);

    child = of_get_child_by_name(np, "common");
    if (child)
        rpc_common_base = of_iomap(child, 0);
#endif /* CONFIG_RTK_RPC */

	printk(KERN_INFO "[RTK_MEM_REMAP] initialized\n");
}

void __init rtk_mem_remap_of_init(void)
{
	struct device_node *np;
	unsigned int rtk_mem_remap = 0;

	for_each_compatible_node(np, NULL, "Realtek,rtk1295-mem_remap"){
        if (of_get_child_count(np) >= 3) {
            rtk_mem_remap_parent_init(np);
            rtk_mem_remap++;
            break;
        }
		rtk_mem_remap_init(np);
		rtk_mem_remap++;
	}

	if (!rtk_mem_remap)
		pr_crit("%s: no matching rtk_mem_remap found\n", __func__);

}

REALTEK_MAP_OF_DECLARE(realtek_memory_remap, "Realtek,rtk1295-mem_remap", rtk_mem_remap_init);

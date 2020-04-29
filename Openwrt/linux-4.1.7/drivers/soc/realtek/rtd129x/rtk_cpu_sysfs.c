/*
 * rtk_cpu_sysfs.c - provides /sys/kernel/cpu/
 *
 * Copyright (C) 2017, Realtek Semiconductor Corporation
 * Copyright (C) 2017,  Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/kobject.h>

struct kobject *kernel_cpu_kobj;
EXPORT_SYMBOL_GPL(kernel_cpu_kobj);

static int __init kernel_cpu_kobj_init(void)
{
	kernel_cpu_kobj = kobject_create_and_add("cpu", kernel_kobj);
	if (!kernel_cpu_kobj)
		return -ENOMEM;
	return 0;
}
postcore_initcall(kernel_cpu_kobj_init);

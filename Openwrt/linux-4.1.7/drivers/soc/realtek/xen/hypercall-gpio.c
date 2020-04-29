/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define pr_fmt(fmt) "xen-rtk: " fmt

#include <linux/cpu.h>
#include <xen/xen.h>
#include <xen/interface/rtk-hypercall.h>
#include <asm/xen/hypercall.h>
#include <asm/xen/interface.h>


int xen_rtk_interdomain_gpio_lock(unsigned long timeout) {
	struct xen_rtk_gpio_lock lock_param;
	int rc = 0;

	lock_param.timeout = timeout;
	rc =  HYPERVISOR_rtk_hypercall_op(XENRTK_gpio_lock, &lock_param);

	if (rc != 0) {
		pr_warn("GPIO LOCK FAIL!!\n");
	}

	return rc;
}

int xen_rtk_interdomain_gpio_unlock(unsigned long timeout) {
	struct xen_rtk_gpio_lock lock_param;
	int rc = 0;

	lock_param.timeout = timeout;
	rc =  HYPERVISOR_rtk_hypercall_op(XENRTK_gpio_unlock, &lock_param);

	if (rc != 0) {
		pr_warn("GPIO UNLOCK FAIL!!\n");
	}

	return rc;
}

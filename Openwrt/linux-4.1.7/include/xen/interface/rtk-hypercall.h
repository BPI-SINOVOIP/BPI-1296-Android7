/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __XEN_PUBLIC_RTKHYPERCALL_H__
#define __XEN_PUBLIC_RTKHYPERCALL_H__

/*
 * Inter-Domain hypervisor lock.
 *
 */
#define XENRTK_gpio_lock	0
#define XENRTK_gpio_unlock	1
struct xen_rtk_gpio_lock {
	// Input
	unsigned long timeout;
};
typedef struct xen_rtk_gpio_lock xen_rtk_gpio_lock_t;
DEFINE_GUEST_HANDLE_STRUCT(xen_rtk_gpio_lock_t);

#endif //__XEN_PUBLIC_RTKHYPERCALL_H__

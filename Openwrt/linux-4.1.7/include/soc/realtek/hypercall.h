/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __SOC_RTK_HYPERCALL_H__
#define __SOC_RTK_HYPERCALL_H__

#ifdef CONFIG_RTK_XEN_GPIO
extern int xen_rtk_interdomain_gpio_lock(unsigned long timeout);
extern int xen_rtk_interdomain_gpio_unlock(unsigned long timeout);

#define xen_gpio_lock(t)	xen_rtk_interdomain_gpio_lock(t)
#define xen_gpio_unlock(t)	xen_rtk_interdomain_gpio_unlock(t)
#endif //CONFIG_RTK_XEN_GPIO

#endif //__SOC_RTK_HYPERCALL_H__

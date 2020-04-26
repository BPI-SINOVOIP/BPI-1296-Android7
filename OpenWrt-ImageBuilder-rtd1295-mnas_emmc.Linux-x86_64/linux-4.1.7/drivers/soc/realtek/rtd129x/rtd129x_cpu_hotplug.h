/*
 * rtd129x_cpu_hotplug.h
 *
 * Copyright (c) 2017 Realtek Semiconductor Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#ifndef DRIVERS_SOC_REALTEK_RTD129x_RTD129x_CPU_HOTPLUG_H_
#define DRIVERS_SOC_REALTEK_RTD129x_RTD129x_CPU_HOTPLUG_H_

#include <asm/virt.h>

void __cpu_do_lowpower(unsigned long secondary_entry_addr);

static inline void cpu_do_lowpower(unsigned long secondary_entry_addr)
{
	typeof(__cpu_do_lowpower) *cpu_lowpower_fn;
	cpu_lowpower_fn = (void *)virt_to_phys(__cpu_do_lowpower);
	cpu_lowpower_fn(secondary_entry_addr);
	unreachable();
}

#endif /* DRIVERS_SOC_REALTEK_RTD129x_RTD129x_CPU_HOTPLUG_H_ */

/*
 *  Copyright (C) 2015-2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __RTK_THERMAL_H__
#define __RTK_THERMAL_H__

#include <linux/ktime.h>
#define __RTK_THERMAL_DEBUG__	0

#if __RTK_THERMAL_DEBUG__
#define THERMAL_DEBUG(format, ...) printk("[THERMAL DBG]" format "\n", ## __VA_ARGS__)
#else
#define THERMAL_DEBUG(format, ...)
#endif

#define THERMAL_ERROR(format, ...) printk(KERN_ERR "[THERMAL ERR]" format "\n", ## __VA_ARGS__)
#define THERMAL_INFO(format, ...) printk(KERN_WARNING "[THERMAL]" format "\n", ## __VA_ARGS__)


#define wr_reg(x,y)                     writel(y,(volatile unsigned int*)(x))
#define rd_reg(x)                       readl((volatile unsigned int*)(x))

#define THERMAL_MASK (THERMAL_0_EN | THERMAL_1_EN)

enum FlagMode {
	SET,
	CLEAR,
	ASSIGN
};

enum rtk_thermal_flags_t {
	TREND_URGENT			= 0x1U << 0,
	TRIP_SHUTDOWN			= 0x1U << 1,
	THERMAL_0_EN			= 0x1U << 2,
	THERMAL_1_EN			= 0x1U << 3,
};

enum rtk_cdev_id {
	CDEV_CPUFREQ = 0,
	CDEV_CPU_CORE_CTRL = 1,
	CDEV_FAN_CTRL = 2,
	CDEV_SHUTDOWN = 3,
	CDEV_DEVFREQ_GPU = 4,
	CDEV_CPUFREQ_DUMMY = 5,
	/* INVALID */
	CDEV_INVALID = -1
};

struct rtk_trip_point {
	int                    temp;
	int                    hyst;
	enum thermal_trip_type type;
	enum rtk_cdev_id       cdev_id;
	int                    cdev_idx;
};

struct rtk_freq {
	int freq;
	int lv;
};

#define RTK_FREQ_GET_LOWER(_table, _index)  (_table[(_index) * 2].lv)
#define RTK_FREQ_GET_UPPER(_table, _index)  (_table[(_index) * 2 + 1].lv)

struct rtk_thermal_priv {
	struct device *dev;
	struct thermal_zone_device *tz_dev;

	/* cdev cpufreq_cooling */
	struct thermal_cooling_device *cdev_cpufreq;
	int n_freqs;
	struct rtk_freq *freqs;

	/* cdev cpu_core_control_cooling */
	struct thermal_cooling_device *cdev_cc;
	struct notifier_block cc_nb;
	struct cpumask core_control_mask;

	/* critical index */
	int crit_idx;

	/* trip points */
	int n_trips;
	struct rtk_trip_point *trips;

	/* thermal_zone */
	enum thermal_device_mode mode;
	int	monitoringRateMs;
	unsigned long flags;
	void __iomem *reg;
	void __iomem *reg_cr;

	/* userspace control */
	struct kobj_attribute limit_cpu_attr;
	int bind_limit_cpu;

#ifdef CONFIG_RTK_THERMAL_EOH
	unsigned int eoh_state;
#define THERMAL_SENSOR_EOH_NONE    0
#define THERMAL_SENSOR_EOH_START   1
#define THERMAL_SENSOR_EOH_POLL    2
	unsigned int eoh_freq;
	unsigned int eoh_restore_ms;
	ktime_t eoh_time;
	struct notifier_block eoh_cpufreq_nb;
#endif /* CONFIG_RTK_THERMAL_EOH */
};

#ifdef CONFIG_RTK_THERMAL_EOH
void rtk_thermal_eoh_handle_restore(struct rtk_thermal_priv *tdev);
void rtk_thermal_eoh_handle_overheat(struct rtk_thermal_priv *tdev,
				     int i, enum thermal_trend trend);
int rtk_thermal_add_eoh(struct rtk_thermal_priv *tdev);
void rtk_thermal_remove_eoh(struct rtk_thermal_priv *tdev);
#else
static inline
void rtk_thermal_eoh_handle_restore(struct rtk_thermal_priv *tdev)
{
}

static inline
void rtk_thermal_eoh_handle_overheat(struct rtk_thermal_priv *tdev,
				     int i, enum thermal_trend trend)
{
}

static inline
int rtk_thermal_add_eoh(struct rtk_thermal_priv *tdev)
{
	return 0;
}

static inline
void rtk_thermal_remove_eoh(struct rtk_thermal_priv *tdev)
{
}

#endif /* CONFIG_RTK_THERMAL_EOH */

#ifdef CONFIG_RTK_THERMAL_CPU_CORE_COOLING
struct thermal_cooling_device *cpu_core_cooling_register(struct cpumask *cpus);
int cpu_core_cooling_unregister(struct thermal_cooling_device *cdev);
#else
static inline struct thermal_cooling_device *cpu_core_cooling_register(struct cpumask *cpus)
{
	return NULL;
}

static inline int cpu_core_cooling_unregister(struct thermal_cooling_device *cdev)
{
	return 0;
}
#endif /* CONFIG_RTK_THERMAL_CPU_CORE_COOLING */


#include "../thermal_core.h"

static inline void thermal_zone_reset_cdev(struct thermal_zone_device *tz)
{
	struct thermal_instance *pos;

	/* clear all target in thermal_instance */
	list_for_each_entry(pos, &tz->thermal_instances, tz_node) {
		if (pos->target == THERMAL_NO_TARGET)
			continue;
		pos->target = THERMAL_NO_TARGET;
		pos->cdev->updated = false;
	}

	/* update all cdev */
	list_for_each_entry(pos, &tz->thermal_instances, tz_node)
		thermal_cdev_update(pos->cdev);
}

static inline bool cdev_is_cpufreq(struct thermal_cooling_device *cdev)
{
	return !strncmp("thermal-cpufreq-0", cdev->type, THERMAL_NAME_LENGTH);
}

#endif /* __RTK_THERMAL_H__ */

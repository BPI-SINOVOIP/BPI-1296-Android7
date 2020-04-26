/*
 * pwrctrl-rtk.h - Realtek Power-Control Header File
 *
 * Copyright (C) 2016-2017 Realtek Semiconductor Corporation
 * Copyright (C) 2016-2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __REALTEK_POWER_CONTROL_H
#define __REAKTEK_POWER_CONTROL_H

#include <linux/of.h>
#include <linux/notifier.h>
#include <linux/power-control.h>

#define REALTEK_PCM_MAGIC 0xabcdef00

struct rtk_pcm_pwrctrl {
	unsigned int __magic;
	struct power_control pctrl;
	unsigned int flags;
	unsigned int use_cnt;
	struct device_node *np;

	struct notifier_block nb;
	int pm_saved;
};

#define nb_to_rkt_pcm_pwrctrl(_nb) container_of(_nb, struct rtk_pcm_pwrctrl, nb)

#define REALTEK_PCM_L4_ICG        BIT(0)
#define REALTEK_PCM_ANALOG        BIT(1)
#define REALTEK_PCM_USED          BIT(8)

static inline bool rtk_pcm_pwrctrl_is_icg(struct rtk_pcm_pwrctrl *core)
{
	return !!(core->flags & REALTEK_PCM_L4_ICG);
}

static inline void rtk_pcm_pwrctrl_mark_as_used(struct rtk_pcm_pwrctrl *core)
{
	core->flags |= REALTEK_PCM_USED;
}

static inline bool rtk_pcm_pwrctrl_is_used(struct rtk_pcm_pwrctrl *core)
{
	return !!(core->flags & REALTEK_PCM_USED);
}


/* notify events */
#define REALTEK_PCM_SUSPEND_EARLY   0x1
#define REALTEK_PCM_SUSPEND         0x2
#define REALTEK_PCM_RESUME          0x3
#define REALTEK_PCM_RESUME_LATE     0x4
#define REALTEK_PCM_POWEROFF_UNUSED 0x5

struct rtk_pcm;
int register_rtk_pcm_notifier(struct rtk_pcm *pcm, struct notifier_block *nb);
int unregister_rtk_pcm_notifier(struct rtk_pcm *pcm, struct notifier_block *nb);

static inline void rtk_pcm_pwrctrl_pm_save_power(struct rtk_pcm_pwrctrl *core)
{
	core->pm_saved = power_control_is_powered_on(&core->pctrl);
}

static inline void rtk_pcm_pwrctrl_pm_restore_power(struct rtk_pcm_pwrctrl *core)
{
	int powered = power_control_is_powered_on(&core->pctrl);
	if (core->pm_saved == 0 && powered == 1)
		power_control_power_off(&core->pctrl);
	if (core->pm_saved == 1 && powered == 0)
		power_control_power_on(&core->pctrl);
}

#define OF_READ_U32(_np, _name, _val) \
({ \
	int _ret = of_property_read_u32(_np, _name, &(_val)); \
	(_ret); \
})

#define OF_READ_U32_OPTIONAL(_np, _name, _val, _def) \
	if (of_property_read_u32(_np, _name, &(_val))) \
		_val = _def;

#define OF_READ_FLAGS(_np, _name, _val, _flag) \
	if (of_find_property(_np, _name, NULL)) \
		_val |= _flag;

#define OF_READ_STRING_OPTIONAL(_np, _name, _val, _def) \
	if (of_property_read_string(_np, _name, &(_val))) \
		_val = _def;

int of_rtk_pcm_pwrctrl_register(struct device *dev, struct rtk_pcm_pwrctrl *core);
__init int rtk_pwrctrl_init(struct device *dev, struct device_node *np);
__init int rtk_pwrctrl_timer_init(struct device *dev, struct device_node *np);
__init int rtk_pcpd_init(struct device *dev, struct device_node *np);


#endif

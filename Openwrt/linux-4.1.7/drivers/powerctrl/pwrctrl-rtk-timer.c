/*
 * pwrctrl-rtk-timer.c - Realtek Timer-wrapped Power-Control implementation
 *
 * Copyright (C) 2016-2017 Realtek Semiconductor Corporation
 * Copyright (C) 2016-2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define pr_fmt(fmt) "pwrctrl: " fmt

#include <linux/slab.h>
#include <linux/of.h>
#include <linux/power-control.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include "power_control_driver.h"
#include "pwrctrl-rtk.h"

static DEFINE_SPINLOCK(rtk_pwrctrl_timer_lock);

struct rtk_pwrctrl_timer {
	struct rtk_pcm_pwrctrl core;
	struct power_control *parent;
	struct timer_list timer;
	int	expired_time;
	volatile int do_power_off;
};

#define to_rtk_pwrctrl_timer(_ctrl) \
	container_of(_ctrl, struct rtk_pwrctrl_timer, core.pctrl)

#define _name(_pwr) ((_pwr)->core.pctrl.name)

static int rtk_pwrctrl_timer_pcm_notify(struct notifier_block *nb,
	unsigned long action, void *unused)
{
	struct rtk_pwrctrl_timer *pwr = container_of(nb, struct rtk_pwrctrl_timer, core.nb);

	switch (action) {
	case REALTEK_PCM_SUSPEND_EARLY:
		if (pwr->do_power_off) {
			pr_debug("%s: timer->parent power_off\n",  pwr->core.pctrl.name);
			power_control_power_off(pwr->parent);
			return NOTIFY_OK;
		}
		break;
	default:
		break;
	}

	return NOTIFY_DONE;
}


static int rtk_pwrctrl_timer_power_on(struct power_control * pctrl)
{
	struct rtk_pwrctrl_timer *pwr = to_rtk_pwrctrl_timer(pctrl);
	int ret;
	unsigned long flags;

	spin_lock_irqsave(&rtk_pwrctrl_timer_lock, flags);

	pr_debug("[PCT] %s::%s\n", _name(pwr), __func__);

	pwr->do_power_off = 0;

	ret = power_control_is_powered_on(pwr->parent);
	if (!ret) {
		pr_debug("[PCT] %s::%s: do power_on\n", _name(pwr), __func__);
		ret = power_control_power_on(pwr->parent);
	}

	spin_unlock_irqrestore(&rtk_pwrctrl_timer_lock, flags);

	return ret;
}

static void rtk_pwrctrl_timer_expired(unsigned long data)
{
	struct rtk_pwrctrl_timer *pwr = (struct rtk_pwrctrl_timer *)data;

	spin_lock(&rtk_pwrctrl_timer_lock);

	pr_debug("[PCT] %s::%s\n", _name(pwr),  __func__);

	if (pwr->do_power_off) {
		pr_debug("[PCT] %s::%s: do power_off\n", _name(pwr), __func__);
		power_control_power_off(pwr->parent);
	}

	spin_unlock(&rtk_pwrctrl_timer_lock);
}

static int rtk_pwrctrl_timer_power_off(struct power_control * pctrl)
{
	struct rtk_pwrctrl_timer *pwr = to_rtk_pwrctrl_timer(pctrl);
	unsigned long flags;

	spin_lock_irqsave(&rtk_pwrctrl_timer_lock, flags);

	pr_debug("[PCT] %s::%s\n", _name(pwr), __func__);

	pwr->do_power_off = 1;
	if (!timer_pending(&pwr->timer)) {
		pwr->timer.expires = get_jiffies_64() + pwr->expired_time;
		add_timer(&pwr->timer);
	} else {
		mod_timer(&pwr->timer, get_jiffies_64() + pwr->expired_time);
	}

	spin_unlock_irqrestore(&rtk_pwrctrl_timer_lock, flags);

	return 0;
}

static int rtk_pwrctrl_timer_is_powered_on(struct power_control * pctrl)
{
	struct rtk_pwrctrl_timer *pwr = to_rtk_pwrctrl_timer(pctrl);
	int ret;

	ret = power_control_is_powered_on(pwr->parent);
	return ret;
}

static struct power_control_ops rtk_pwrctrl_timer_ops = {
	.power_on  = rtk_pwrctrl_timer_power_on,
	.power_off = rtk_pwrctrl_timer_power_off,
	.is_powered_on = rtk_pwrctrl_timer_is_powered_on,
};

int  __init rtk_pwrctrl_timer_init(struct device *dev, struct device_node *np)
{
	struct rtk_pwrctrl_timer *pwr;
	int ret;

	pwr = kzalloc(sizeof(*pwr), GFP_KERNEL);
	if (!pwr)
		return -ENOMEM;

	pwr->core.pctrl.ops = &rtk_pwrctrl_timer_ops;
	pwr->core.pctrl.of_node = np;
	pwr->core.nb.notifier_call = rtk_pwrctrl_timer_pcm_notify;
	OF_READ_U32_OPTIONAL(np, "pwr,expired-time-sec", pwr->expired_time, 30);
	pwr->expired_time *= HZ;

	pwr->parent = of_power_control_get(np, NULL);
	if (IS_ERR_OR_NULL(pwr->parent)) {
		ret = -EINVAL;
		goto error;
	}

	init_timer(&pwr->timer);
	pwr->timer.data     = (unsigned long)pwr;
	pwr->timer.function = rtk_pwrctrl_timer_expired;

	ret = of_rtk_pcm_pwrctrl_register(dev, &pwr->core);
	if (ret)
		goto error;

	return 0;
error:
	if (pwr)
		kfree(pwr);
	return ret;
}

/*
 * pwrctrl-rtk.c - Realtek generic Power-Control implementation
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
#include <linux/of_address.h>
#include <linux/power-control.h>
#include <linux/reset.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include "power_control_driver.h"
#include "pwrctrl-rtk.h"

struct __reg {
	void * __iomem reg;
	unsigned int shift;
	unsigned int width;
	unsigned int value_on;
	unsigned int value_off;
};

struct __reset {
	int num_reset;
	struct reset_control *resets[4];
};

#define POW_REF_MAX 3
struct __pow_ref {
	unsigned int flags;
	struct power_control *p[POW_REF_MAX];
	struct clk *c[POW_REF_MAX];
	struct reset_control *r[POW_REF_MAX];
};

#define REF_INIT       BIT(0)
#define REF_POW_SET    BIT(1)
#define REF_CLK_SET    BIT(2)
#define REF_RESET_SET  BIT(3)

struct pwrctrl_entry {
	unsigned int type;
	unsigned int flags;
	struct device_node *np;
	struct device_node *parent;
	union {
		struct __reg reg;
		struct __reset reset;
	};
};

#define PWRCTRL_TYPE_REG    0x0
#define PWRCTRL_TYPE_RESET  0x1

#define PWRCTRL_IGNORE_POWER_STATE  BIT(0)
#define PWRCTRL_IGNORE_POWER_OFF    BIT(1)
#define PWRCTRL_IGNORE_POWER_ON     BIT(2)
#define PWRCTRL_VALUE_INVERT        BIT(3)

struct rtk_pwrctrl {
	struct rtk_pcm_pwrctrl core;

	int powered;
	int num_entry;
	struct pwrctrl_entry *entries;
	struct __pow_ref ref;
	spinlock_t lock;
};

#define to_rtk_pwrctrl(_pctrl) container_of(_pctrl, struct rtk_pwrctrl, core.pctrl)

static int of_parse_pwrctrl_entry(struct device_node *np, struct device_node *parent,
	struct pwrctrl_entry *pwre)
{
	int i;

	pwre->np = np;
	pwre->parent = parent;

	OF_READ_U32_OPTIONAL(np, "pwr,type", pwre->type, PWRCTRL_TYPE_REG);

	pwre->flags = 0;
	OF_READ_FLAGS(np, "pwr,never-on", pwre->flags, PWRCTRL_IGNORE_POWER_ON);
	OF_READ_FLAGS(np, "pwr,never-off", pwre->flags, PWRCTRL_IGNORE_POWER_OFF);
	OF_READ_FLAGS(np, "pwr,no-state", pwre->flags, PWRCTRL_IGNORE_POWER_STATE);
	OF_READ_FLAGS(np, "pwr,value-inv", pwre->flags, PWRCTRL_VALUE_INVERT);

	switch (pwre->type) {
	case PWRCTRL_TYPE_REG:
		i = of_property_match_string(parent, "reg-names", np->name);
		if (i < 0)
			i = 0;
		pwre->reg.reg = of_iomap(parent, i);

		OF_READ_U32_OPTIONAL(np, "pwr,width", pwre->reg.width, 32);
		OF_READ_U32_OPTIONAL(np, "pwr,shift", pwre->reg.shift, 0);
		OF_READ_U32_OPTIONAL(np, "pwr,value-on", pwre->reg.value_on, BIT(pwre->reg.width) - 1);
		OF_READ_U32_OPTIONAL(np, "pwr,value-off", pwre->reg.value_off, 0);

		if (pwre->flags & PWRCTRL_VALUE_INVERT) {
			unsigned int t = pwre->reg.value_on;
			pwre->reg.value_on = pwre->reg.value_off;
			pwre->reg.value_off = t;
		}
		break;
	case PWRCTRL_TYPE_RESET:
		for (i = 0; i < 4; i++) {
			pwre->reset.resets[i] = of_reset_control_get_by_index(np, i);
			if (IS_ERR_OR_NULL(pwre->reset.resets[i]))
				break;
		}
		pwre->reset.num_reset = i;
		break;
	}
	return 0;
}

static int __set_entry_power(struct pwrctrl_entry *pwre, int on)
{
	if (!on && (pwre->flags & PWRCTRL_IGNORE_POWER_OFF))
		return -EPERM;

	if (on && (pwre->flags & PWRCTRL_IGNORE_POWER_ON))
		return -EPERM;

	if (pwre->type == PWRCTRL_TYPE_REG) {
		struct __reg *r = &pwre->reg;

		unsigned int mask = (BIT(r->width) - 1) << r->shift;
		unsigned int data  = (on ? r->value_on : r->value_off) << r->shift;
		unsigned int val;

		val = readl(r->reg);
		val &= ~mask;
		val |= data;
		writel(val, r->reg);
	} else if (pwre->type == PWRCTRL_TYPE_RESET) {
		int i;
		for (i = 0; i < pwre->reset.num_reset; i++)
			reset_control_assert(pwre->reset.resets[i]);
		for (i = 0; i < pwre->reset.num_reset; i++)
			reset_control_deassert(pwre->reset.resets[i]);
	}

	return 0;
}

static int __get_entry_power(struct pwrctrl_entry *pwre, unsigned int *on)
{
	if (pwre->flags & PWRCTRL_IGNORE_POWER_STATE)
		return -EPERM;

	if (pwre->type == PWRCTRL_TYPE_REG) {
		struct __reg *r = &pwre->reg;
		unsigned int val;

		val = readl(r->reg);
		val = (val >> r->shift) & (BIT(r->width) - 1);

		if (val == r->value_on)
			*on = 1;
		else if (val == r->value_off)
			*on = 0;
		else
			return -EINVAL;
		return 0;
	}

	return -EINVAL;
}

static int __init_ref(struct device_node *np, struct __pow_ref *ref)
{
	int i;

	WARN_ON(of_count_phandle_with_args(np, "powerctrls", "#powerctrl-cells") > POW_REF_MAX);
	WARN_ON(of_count_phandle_with_args(np, "clocks", "#clock-cells") > POW_REF_MAX);
	WARN_ON(of_count_phandle_with_args(np, "reset", "#reset-cells") > POW_REF_MAX);

	for (i = 0; i < POW_REF_MAX; i++) {
		ref->p[i] = __of_power_control_get_by_index(np, i);
		if (IS_ERR(ref->p[i])) {
			ref->p[i] = NULL;
			break;
		}
	}

	for (i = 0; i < POW_REF_MAX; i++) {
		ref->c[i] = of_clk_get(np, i);
		if (IS_ERR(ref->c[i])) {
			ref->c[i] = NULL;
			break;
		}
	}

	for (i = 0; i < POW_REF_MAX; i++) {
		ref->r[i] = of_reset_control_get_by_index(np, i);
		if (IS_ERR(ref->r[i])) {
			ref->r[i] = NULL;
			break;
		}
	}
	return 0;
}


static int __check_ref(struct __pow_ref *ref)
{
	int i;
	for (i = 0; i < POW_REF_MAX; i++) {
		if (ref->p[i] && power_control_is_powered_on(ref->p[i]) != 1)
			return -EINVAL;
		if (ref->c[i] &&  __clk_is_enabled(ref->c[i]) == 0)
			return -EINVAL;
		if (ref->r[i] && reset_control_status(ref->r[i]) > 0)
			return -EINVAL;
	}
	return 0;
}

static int __save_and_enable_ref(struct __pow_ref *ref)
{
	int i;

	for (i = 0; i < POW_REF_MAX; i++) {
		if (ref->p[i] && power_control_is_powered_on(ref->p[i]) != 1) {
			power_control_power_on(ref->p[i]);
			ref->flags |= BIT(POW_REF_MAX * i + 0);
		}	

		if (ref->c[i] &&  __clk_is_enabled(ref->c[i]) == 0) {
			clk_prepare_enable(ref->c[i]);
			ref->flags |= BIT(POW_REF_MAX * i + 1);
		}

		if (ref->r[i] && reset_control_status(ref->r[i]) > 0) {
			reset_control_deassert(ref->r[i]);
			ref->flags |= BIT(POW_REF_MAX * i + 2);
		}
	}

	return 0;
}

static void __restore_ref(struct __pow_ref *ref)
{
	int i;
	for (i = 0; i < POW_REF_MAX; i++) {
		if (ref->flags & BIT(POW_REF_MAX * i + 0))
			power_control_power_off(ref->p[i]);
		if (ref->flags & BIT(POW_REF_MAX * i + 1))
			clk_disable_unprepare(ref->c[i]);
		if (ref->flags & BIT(POW_REF_MAX * i + 2))
			reset_control_assert(ref->r[i]);
	}
	ref->flags = 0;;
}

static int rtk_pwrctrl_power_on(struct power_control *pctrl)
{
	struct rtk_pwrctrl *pwr = to_rtk_pwrctrl(pctrl);
	int i;
	unsigned long flags;

	rtk_pcm_pwrctrl_mark_as_used(&pwr->core);

	if (rtk_pcm_pwrctrl_is_icg(&pwr->core) &&__check_ref(&pwr->ref))
		return -EPERM;
	__save_and_enable_ref(&pwr->ref);

	spin_lock_irqsave(&pwr->lock, flags);
	for (i = 0; i < pwr->num_entry; i++)
		__set_entry_power(&pwr->entries[i], 1);
	pwr->powered = 1;
	spin_unlock_irqrestore(&pwr->lock, flags);

	 __restore_ref(&pwr->ref);

	return 0;
}

static int rtk_pwrctrl_power_off(struct power_control *pctrl)
{
	struct rtk_pwrctrl *pwr = to_rtk_pwrctrl(pctrl);
	int i;
	unsigned long flags;

	rtk_pcm_pwrctrl_mark_as_used(&pwr->core);

	if (rtk_pcm_pwrctrl_is_icg(&pwr->core) &&__check_ref(&pwr->ref))
		return -EPERM;
	__save_and_enable_ref(&pwr->ref);

	spin_lock_irqsave(&pwr->lock, flags);
	for (i = 0; i < pwr->num_entry; i++)
		__set_entry_power(&pwr->entries[i], 0);
	pwr->powered = 0;
	spin_unlock_irqrestore(&pwr->lock, flags);

	__restore_ref(&pwr->ref);

	return 0;

}

static int rtk_pwrctrl_is_powered_on(struct power_control *pctrl)
{
	struct rtk_pwrctrl *pwr = to_rtk_pwrctrl(pctrl);
	int i;
	int ret;
	int val;
	int cnt_on = 0, cnt_off = 0;

	rtk_pcm_pwrctrl_mark_as_used(&pwr->core);

	if (__check_ref(&pwr->ref))
		return -EPERM;

	for (i = 0; i < pwr->num_entry; i++) {
		val = -1;
		ret = __get_entry_power(&pwr->entries[i], &val);
		if (ret)
			continue;
		if (val == 0)
			cnt_off ++;
		if (val == 1)
			cnt_on ++;
	}

	if (cnt_on && !cnt_off)
		return 1;
	if (cnt_off && !cnt_on)
		return 0;

	return pwr->powered;
}

static struct power_control_ops rtk_pwrctrl_ops = {
	.power_off = rtk_pwrctrl_power_off,
	.power_on = rtk_pwrctrl_power_on,
	.is_powered_on = rtk_pwrctrl_is_powered_on,
};

static int of_count_available_ref_devices(struct device_node *np)
{
	int ret = 0;
	struct property *prop;
	const char *str;
	struct device_node *ref_np;

	of_property_for_each_string(np, "ref,name", prop, str) {
		ref_np = of_find_node_by_name(NULL, str);
		if (ref_np && of_device_is_available(ref_np))
			ret ++;
	}

	of_property_for_each_string(np, "ref,compatible", prop, str) {
		ref_np = of_find_compatible_node(NULL, NULL, str);
		if (ref_np && of_device_is_available(ref_np))
			ret ++;
	}

	return ret;
}

static int rtk_pwrctrl_pcm_notify(struct notifier_block *nb,
        unsigned long action, void *unused)
{
	struct rtk_pcm_pwrctrl *core = nb_to_rkt_pcm_pwrctrl(nb);
	int ret;

	switch (action) {
	case REALTEK_PCM_POWEROFF_UNUSED:
		if (rtk_pcm_pwrctrl_is_used(core))
			break;

		if (rtk_pcm_pwrctrl_is_icg(core)) {
			ret = power_control_power_off(&core->pctrl);
			pr_info("%s: enable_hw_pm returns %d\n", core->pctrl.name, ret);
			return NOTIFY_OK;
		}

		if (of_count_available_ref_devices(core->np) > 0)
			break;

		ret = power_control_power_off(&core->pctrl);
		pr_info("%s: power_off_unused returns %d\n", core->pctrl.name, ret);
		return NOTIFY_OK;

	case REALTEK_PCM_SUSPEND:
		pr_debug("%s: save\n",  core->pctrl.name);
		rtk_pcm_pwrctrl_pm_save_power(core);
		return NOTIFY_OK;

	case REALTEK_PCM_RESUME:
		pr_debug("%s: restore\n",  core->pctrl.name);
		rtk_pcm_pwrctrl_pm_restore_power(core);
		return NOTIFY_OK;

	default:
		break;
	}

	return NOTIFY_DONE;
}

int __init rtk_pwrctrl_init(struct device *dev, struct device_node *np)
{
	struct rtk_pwrctrl *pwr;
	struct device_node *child;
	int i;
	int ret;

	pwr = kzalloc(sizeof(*pwr), GFP_KERNEL);
	if (!pwr)
		return -EINVAL;


	pwr->num_entry = of_get_child_count(np);
	if (!pwr->num_entry) {
		ret = 0;
		pr_warn("%s: Empty pwrctrl\n", np->name);
	}

	pwr->entries = kcalloc(pwr->num_entry, sizeof(*pwr->entries), GFP_KERNEL);
	if (!pwr->entries) {
		ret = -ENOMEM;
		goto free_mem;
	}

	i = 0;
	for_each_child_of_node(np, child) {
		ret = of_parse_pwrctrl_entry(child, np, &pwr->entries[i]);
		if (ret)
			goto free_entries;
		i++;
	}

	__init_ref(np, &pwr->ref);

	pwr->core.pctrl.ops = &rtk_pwrctrl_ops;
	pwr->core.pctrl.of_node = np;
	pwr->core.nb.notifier_call = rtk_pwrctrl_pcm_notify;

	ret = of_rtk_pcm_pwrctrl_register(dev, &pwr->core);
	if (ret)
		goto free_entries;

	return 0;

free_entries:
	kfree(pwr->entries);
free_mem:
	kfree(pwr);

	return ret;
}

/*
 * pwrctrl-rtk-drv.c - Realtek Power-Control Driver Core
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
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/suspend.h>
#include <linux/pm_domain.h>
#include <asm/uaccess.h>
#include <soc/realtek/rtd129x_cpu.h>
#include <dt-bindings/soc/rtk,pwrctrl.h>
#include "power_control_driver.h"
#include "pwrctrl-rtk.h"

struct rtk_pcm {
	struct device *dev;
	struct blocking_notifier_head pm_notifier_list;
	struct delayed_work dwork;
	unsigned int type;
	struct list_head pcpds;
};

static struct workqueue_struct *rtk_pcm_wq;

static int rtk_pcm_send_event(struct rtk_pcm *pcm, unsigned long action, void *unused);

static inline int of_chip_revsion_is_match(struct device_node *np, int rev_val)
{
	const u32 *prop;
	int i;
	int size;
	bool exclusive_mode = false;

	prop = of_get_property(np, "realtek,chip-rev,inc", &size);
	if (prop)
		goto start;

	exclusive_mode = true;
	prop = of_get_property(np, "realtek,chip-rev,exc", &size);
	if (prop)
		goto start;

	return 1;
start:

	size /= sizeof(u32);
	for (i = 0; i < size; i++) {
		u32 val = of_read_number(prop++, 1);

		if (val == rev_val)
			return exclusive_mode ? 0 : 1;
	}

	return exclusive_mode ? 1 : 0;
}

struct rtk_pcpd {
	struct generic_pm_domain pd;
	struct device *dev;
	struct device_node *np;
	struct power_control *control;
	struct list_head list;
};

#define to_rtk_pcpd(_pd) container_of(_pd, struct rtk_pcpd, pd)

static int rtk_pcpd_power_on(struct generic_pm_domain *pd)
{
	struct rtk_pcpd *pcpd = to_rtk_pcpd(pd);

	pr_info("%s: %s\n", __func__, pd->name);
	power_control_power_on(pcpd->control);

	return 0;
}

static int rtk_pcpd_power_off(struct generic_pm_domain *pd)
{
	struct rtk_pcpd *pcpd = to_rtk_pcpd(pd);

	pr_info("%s: %s\n", __func__, pd->name);
	power_control_power_off(pcpd->control);

	return 0;
}

static int rtk_pcpd_attach_dev(struct generic_pm_domain *pd, struct device *dev)
{
	pr_info("%s: %s attach %s %s\n", __func__, pd->name, dev_driver_string(dev), dev_name(dev));
	return 0;
}

int __init rtk_pcpd_init(struct device *dev, struct device_node *np)
{
	struct rtk_pcm *pcm = dev_get_drvdata(dev);
	struct rtk_pcpd *pcpd;
	struct of_phandle_args args;
	int state;
	int ret = 0;

	pcpd = kzalloc(sizeof(*pcpd), GFP_KERNEL);
	if (!pcpd)
		return -ENOMEM;

	OF_READ_STRING_OPTIONAL(np, "pd,name", pcpd->pd.name, np->name);
	pcpd->pd.power_off = rtk_pcpd_power_off;
	pcpd->pd.power_on = rtk_pcpd_power_on;
	pcpd->pd.attach_dev = rtk_pcpd_attach_dev;

	pcpd->dev = dev;
	pcpd->np = np;

	args.np = np;
	args.args_count = 0;
	pcpd->control = of_power_control_get_from_provider(&args);
	if (IS_ERR(pcpd->control)) {
		ret = PTR_ERR(pcpd->control);
		dev_err(dev, "failed to get %s: %d\n", power_control_get_name(pcpd->control), ret);
		goto free_mem;
	}

	state = 0;
	if (of_find_property(np , "pd,default-off", NULL))
		state = 1;

	pm_genpd_init(&pcpd->pd, NULL, state);
	of_genpd_add_provider_simple(np, &pcpd->pd);
	list_add(&pcpd->list, &pcm->pcpds);

	return 0;
free_mem:
	kfree(pcpd);
	return ret;
}

static int rtk_pcm_pcpd_subdomain_init(struct device *dev)
{
	struct device_node *np;

	for_each_child_of_node(dev->of_node, np) {
		struct generic_pm_domain *child_domain, *parent_domain;
		struct of_phandle_args args;

		args.np = np;
		args.args_count = 0;
		child_domain = of_genpd_get_from_provider(&args);

		if (IS_ERR(child_domain))
			continue;

		if (of_parse_phandle_with_args(np, "power-domains",
			"#power-domain-cells", 0, &args) != 0)
			continue;

		parent_domain = of_genpd_get_from_provider(&args);
		if (IS_ERR(parent_domain))
			continue;

		if (pm_genpd_add_subdomain(parent_domain, child_domain))
			dev_warn(dev, "%s failed to add subdomain: %s\n",
				parent_domain->name, child_domain->name);
		else
			dev_info(dev, "%s has as child subdomain: %s.\n",
				parent_domain->name, child_domain->name);
	}
	return 0;																																                of_node_put(np);
}

static int rtk_pcm_pcpd_restore(struct rtk_pcm *pcm)
{
	struct rtk_pcpd *pcpd;
	int st;

	/* Checking status the pds after resume. If the status is
	 * GPD_STATE_POWER_OFF, power off the pd via power control.
	 */
	list_for_each_entry(pcpd, &pcm->pcpds, list) {
		if (pcpd->pd.status == GPD_STATE_POWER_OFF) {
			st = power_control_is_powered_on(pcpd->control);
			if (st == 1)
				power_control_power_off(pcpd->control);
		}
	}
	return 0;
}

static const struct of_device_id rtk_pwrctrl_of_table [] = {
	{.compatible = "realtek,pwrctrl", .data = rtk_pwrctrl_init },
	{.compatible = "realtek,pwrctrl-timer", .data = rtk_pwrctrl_timer_init },
	{}
};

static int __init rtk_pcm_pwrctrl_init(struct device *dev)
{
	struct rtk_pcm *pcm = dev_get_drvdata(dev);
	struct device_node *np;
	const struct of_device_id *matches = rtk_pwrctrl_of_table;
	const struct of_device_id *match;

	int ret = 0;
	int chip_rev = get_rtd129x_cpu_revision() >> 16;

	for_each_matching_node_and_match(np, matches, &match) {
		int (*func)(struct device *, struct device_node *) = match->data;

		if (of_get_parent(np) != dev->of_node)
			continue;
		if (!of_device_is_available(np))
			continue;
		if (!of_chip_revsion_is_match(np, chip_rev))
			continue;

		ret = func(dev, np);
		if (ret)
			dev_err(dev, "%s: failed to initialize power-control: %d\n", np->name, ret);

		if (pcm->type != PCM_TYPE_GENPD)
			continue;

		/* after a control is ready, translate it into a pd */
		ret = rtk_pcpd_init(dev, np);
		if (ret)
			dev_err(dev, "%s failed to initialize power-domain: %d\n", np->name, ret);
	}
	return 0;
}

static void do_pcm_work(struct work_struct *w)
{
	struct rtk_pcm *pcm = container_of(w, struct rtk_pcm, dwork.work);

	dev_info(pcm->dev, "[PWRCTRL] power_off_unused\n");
	rtk_pcm_send_event(pcm, REALTEK_PCM_POWEROFF_UNUSED, NULL);
}

int register_rtk_pcm_notifier(struct rtk_pcm *pcm, struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&pcm->pm_notifier_list, nb);
}

int unregister_rtk_pcm_notifier(struct rtk_pcm *pcm, struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&pcm->pm_notifier_list, nb);
}

static int rtk_pcm_send_event(struct rtk_pcm *pcm, unsigned long action, void *unused)
{
	if (pcm->type != PCM_TYPE_MGR)
		return -EPERM;

	return blocking_notifier_call_chain(&pcm->pm_notifier_list, action, unused);
}

int of_rtk_pcm_pwrctrl_register(struct device *dev,
	struct rtk_pcm_pwrctrl *core)
{
	struct device_node *np = core->pctrl.of_node;
	struct rtk_pcm *pcm = dev_get_drvdata(dev);
	int ret;

	if (!np)
		return -EINVAL;

	core->__magic = REALTEK_PCM_MAGIC;
	OF_READ_STRING_OPTIONAL(np, "pwr,name", core->pctrl.name, np->name);

	if (strstr(np->name, "l4_icg"))
		core->flags |= REALTEK_PCM_L4_ICG;

	core->np = np;

	ret = power_control_register(&core->pctrl);
	if (ret)
		return -EINVAL;

	if (rtk_pcm_pwrctrl_is_icg(core))
		power_control_mark_as_hw_pm(&core->pctrl);

	if (core->nb.notifier_call)
		register_rtk_pcm_notifier(pcm, &core->nb);

	return 0;
}

static int rtk_pcm_suspend(struct device *dev)
{
	struct rtk_pcm *pcm = dev_get_drvdata(dev);

#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		return 0;
#endif
	dev_err(dev, "[PWRCTRL] Enter %s\n", __func__);
	rtk_pcm_send_event(pcm, REALTEK_PCM_SUSPEND_EARLY, NULL);
	rtk_pcm_send_event(pcm, REALTEK_PCM_SUSPEND, NULL);
	dev_err(dev, "[PWRCTRL] Exit %s\n", __func__);
	return 0;
}

static int rtk_pcm_resume(struct device *dev)
{
	struct rtk_pcm *pcm = dev_get_drvdata(dev);

#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		return 0;
#endif
	dev_err(dev, "[PWRCTRL] Enter %s\n", __func__);
	rtk_pcm_send_event(pcm, REALTEK_PCM_RESUME, NULL);
	rtk_pcm_send_event(pcm, REALTEK_PCM_RESUME_LATE, NULL);
	rtk_pcm_pcpd_restore(pcm);
	dev_err(dev, "[PWRCTRL] Exit %s\n", __func__);
	return 0;
}

static const struct dev_pm_ops rtk_pcm_pm_ops = {
	.suspend_noirq = rtk_pcm_suspend,
	.resume_noirq = rtk_pcm_resume,
};

static int rtk_pcm_probe(struct platform_device *pdev)
{
	struct rtk_pcm *pcm;

	dev_info(&pdev->dev, "[PWRCTRL] probe\n");

	pcm = kzalloc(sizeof(*pcm), GFP_KERNEL);
	if (!pcm)
		return -ENOMEM;

	OF_READ_U32_OPTIONAL(pdev->dev.of_node, "pcm,type", pcm->type, PCM_TYPE_MGR);

	pcm->dev = &pdev->dev;
	BLOCKING_INIT_NOTIFIER_HEAD(&pcm->pm_notifier_list);
	INIT_DELAYED_WORK(&pcm->dwork, do_pcm_work);
	INIT_LIST_HEAD(&pcm->pcpds);

	dev_set_drvdata(&pdev->dev, pcm);

	rtk_pcm_pwrctrl_init(&pdev->dev);

	if (pcm->type == PCM_TYPE_GENPD)
		rtk_pcm_pcpd_subdomain_init(&pdev->dev);

	if (pcm->type == PCM_TYPE_MGR)
		queue_delayed_work(rtk_pcm_wq, &pcm->dwork, 50*HZ);

	return 0;
}

static const struct of_device_id rtk_pcm_match[] = {
	{.compatible = "realtek,129x-pcm"},
	{}
};

static struct platform_driver rtk_pcm_driver = {
	.probe = rtk_pcm_probe,
	.driver = {
		.name   = "rtk-pcm",
		.of_match_table = rtk_pcm_match,
		.pm = &rtk_pcm_pm_ops,
	},
};

static int __init rtk_pcm_init(void)
{
	rtk_pcm_wq = create_workqueue("pwrctrl_wq");
	return platform_driver_register(&rtk_pcm_driver);
}
arch_initcall(rtk_pcm_init);
